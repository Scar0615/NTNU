var VSHADER_SOURCE = `
    attribute vec4 a_Position;
    attribute vec4 a_Normal;
    attribute vec2 a_TexCoord;
    uniform mat4 u_MvpMatrix;
    uniform mat4 u_modelMatrix;
    uniform mat4 u_normalMatrix;
    uniform mat4 u_MvpMatrixOfLight;
    varying vec4 v_PositionFromLight;
    varying vec3 v_Normal;
    varying vec3 v_PositionInWorld;
    varying vec2 v_TexCoord;
    void main(){
        gl_Position = u_MvpMatrix * a_Position;
        v_PositionInWorld = (u_modelMatrix * a_Position).xyz; 
        v_Normal = normalize(vec3(u_normalMatrix * a_Normal));
        v_PositionFromLight = u_MvpMatrixOfLight * a_Position; //for shadow
        v_TexCoord = a_TexCoord;
    }    
`;

var FSHADER_SOURCE = `
    precision mediump float;
    uniform vec3 u_LightPosition;
    uniform vec3 u_ViewPosition;
    uniform float u_Ka;
    uniform float u_Kd;
    uniform float u_Ks;
    uniform float u_shininess;
    uniform sampler2D u_Sampler;
    uniform sampler2D u_ShadowMap;
    varying vec3 v_Normal;
    varying vec3 v_PositionInWorld;
    varying vec2 v_TexCoord;
    varying vec4 v_PositionFromLight;
    const float deMachThreshold = 0.005;
    void main(){
        // let ambient and diffuse color are u_Color 
        // (you can also input them from ouside and make them different)
        vec3 texColor = texture2D( u_Sampler, v_TexCoord ).rgb;
        vec3 ambientLightColor = texColor;
        vec3 diffuseLightColor = texColor;
        // assume white specular light (you can also input it from ouside)
        vec3 specularLightColor = vec3(1.0, 1.0, 1.0);        

        vec3 ambient = ambientLightColor * u_Ka;

        vec3 normal = normalize(v_Normal);
        vec3 lightDirection = normalize(u_LightPosition - v_PositionInWorld);
        float nDotL = max(dot(lightDirection, normal), 0.0);
        vec3 diffuse = diffuseLightColor * u_Kd * nDotL;

        vec3 specular = vec3(0.0, 0.0, 0.0);
        if(nDotL > 0.0) {
            vec3 R = reflect(-lightDirection, normal);
            // V: the vector, point to viewer       
            vec3 V = normalize(u_ViewPosition - v_PositionInWorld); 
            float specAngle = clamp(dot(R, V), 0.0, 1.0);
            specular = u_Ks * pow(specAngle, u_shininess) * specularLightColor; 
        }

        //***** shadow
        vec3 shadowCoord = (v_PositionFromLight.xyz/v_PositionFromLight.w)/2.0 + 0.5;
        vec4 rgbaDepth = texture2D(u_ShadowMap, shadowCoord.xy);
        /////////******** LOW precision depth implementation ********///////////
        float depth = rgbaDepth.r;
        float visibility = (shadowCoord.z > depth + deMachThreshold) ? 0.3 : 1.0;
  
        gl_FragColor = vec4( ambient + diffuse + specular, 1.0 );
    }
`;

var VSHADER_SOURCE_ENVCUBE = `
  attribute vec4 a_Position;
  varying vec4 v_Position;
  void main() {
    v_Position = a_Position;
    gl_Position = a_Position;
  } 
`;

var FSHADER_SOURCE_ENVCUBE = `
  precision mediump float;
  uniform samplerCube u_envCubeMap;
  uniform mat4 u_viewDirectionProjectionInverse;
  varying vec4 v_Position;
  void main() {
    vec4 t = u_viewDirectionProjectionInverse * v_Position;
    gl_FragColor = textureCube(u_envCubeMap, normalize(t.xyz / t.w));
  }
`;

var VSHADER_QUAD_SOURCE = `
    attribute vec4 a_Position;
    void main(){
        gl_Position = a_Position;
    }    
`;

var FSHADER_QUAD_SOURCE = `
    precision mediump float;
    uniform sampler2D u_ShadowMap;
    void main(){ 
      //TODO-2: look up the depth from u_ShaodowMap and draw on quad (just one line)
      gl_FragColor = texture2D(u_ShadowMap, vec2(gl_FragCoord.xy/800.0));
    }
`;

var VSHADER_SHADOW_SOURCE = `
      attribute vec4 a_Position;
      uniform mat4 u_MvpMatrix;
      void main(){
          gl_Position = u_MvpMatrix * a_Position;
      }
  `;

var FSHADER_SHADOW_SOURCE = `
      precision mediump float;
      void main(){
        /////////** LOW precision depth implementation **/////
        gl_FragColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
      }
  `;

function compileShader(gl, vShaderText, fShaderText){
    //////Build vertex and fragment shader objects
    var vertexShader = gl.createShader(gl.VERTEX_SHADER)
    var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER)
    //The way to  set up shader text source
    gl.shaderSource(vertexShader, vShaderText)
    gl.shaderSource(fragmentShader, fShaderText)
    //compile vertex shader
    gl.compileShader(vertexShader)
    if(!gl.getShaderParameter(vertexShader, gl.COMPILE_STATUS)){
        console.log('vertex shader error');
        var message = gl.getShaderInfoLog(vertexShader); 
        console.log(message);//print shader compiling error message
    }
    //compile fragment shader
    gl.compileShader(fragmentShader)
    if(!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)){
        console.log('fragment shader error');
        var message = gl.getShaderInfoLog(fragmentShader);
        console.log(message);//print shader compiling error message
    }

    /////link shader to program (by a self-define function)
    var program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    //if not success, log the program info, and delete it.
    if(!gl.getProgramParameter(program, gl.LINK_STATUS)){
        alert(gl.getProgramInfoLog(program) + "");
        gl.deleteProgram(program);
    }

    return program;
}

/////BEGIN:///////////////////////////////////////////////////////////////////////////////////////////////
/////The folloing three function is for creating vertex buffer, but link to shader to user later//////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function initAttributeVariable(gl, a_attribute, buffer){
  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
  gl.vertexAttribPointer(a_attribute, buffer.num, buffer.type, false, 0, 0);
  gl.enableVertexAttribArray(a_attribute);
}

function initArrayBufferForLaterUse(gl, data, num, type) {
  // Create a buffer object
  var buffer = gl.createBuffer();
  if (!buffer) {
    console.log('Failed to create the buffer object');
    return null;
  }
  // Write date into the buffer object
  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
  gl.bufferData(gl.ARRAY_BUFFER, data, gl.STATIC_DRAW);

  // Store the necessary information to assign the object to the attribute variable later
  buffer.num = num;
  buffer.type = type;

  return buffer;
}

function initVertexBufferForLaterUse(gl, vertices, normals, texCoords){
  var nVertices = vertices.length / 3;

  var o = new Object();
  o.vertexBuffer = initArrayBufferForLaterUse(gl, new Float32Array(vertices), 3, gl.FLOAT);
  if( normals != null ) o.normalBuffer = initArrayBufferForLaterUse(gl, new Float32Array(normals), 3, gl.FLOAT);
  if( texCoords != null ) o.texCoordBuffer = initArrayBufferForLaterUse(gl, new Float32Array(texCoords), 2, gl.FLOAT);
  //you can have error check here
  o.numVertices = nVertices;

  gl.bindBuffer(gl.ARRAY_BUFFER, null);
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, null);

  return o;
}
/////END://///////////////////////////////////////////////////////////////////////////////////////////////
/////The folloing three function is for creating vertex buffer, but link to shader to user later//////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

var mouseLastX, mouseLastY;
var mouseDragging = false;
var angleX = 0, angleY = 0;
var gl, canvas;
var mvpMatrix;
var modelMatrix;
var normalMatrix;
var nVertex;
var cameraX = 0, cameraY = 0, cameraZ = 12;
var lightX = 0.01, lightY = 4, lightZ = 5.1;
var cameraDirX = 0, cameraDirY = 0, cameraDirZ = -1;
var cubeMapTex;
var steveScale = 0.375;
var foxScale = 0.03;
var grassScale = 10.0;
var steve = [];
var fox = [];
var grass = [];
var textures = {};
var steveImgNames = ["Steve.png"];
var steveImgIndex = ["Steve.png", "Steve.png", "Steve.png", "Steve.png", "Steve.png", "Steve.png"];
var foxImgNames = ["texture.png"];
var foxImgIndex = ["texture.png"];
var grassImgNames = ["3ee9f2b9178eae5fbc7bfdec1d78f4d7.png", "2ab1c37cfdff720c6de2ddb07328f145.jpg", "grass_top_by_tardifice-d84jizt.png"];
var grassImgIndex = ["3ee9f2b9178eae5fbc7bfdec1d78f4d7.png", "grass_top_by_tardifice-d84jizt.png", "2ab1c37cfdff720c6de2ddb07328f145.jpg"];
var texCount = 0;
var snumTextures = steveImgNames.length;
var fnumTextures = foxImgNames.length;
var gnumTextures = grassImgNames.length;
var offScreenWidth = 2048, offScreenHeight = 2048;
var normalMode = true;
var fbo;

async function main(){
    canvas = document.getElementById('webgl');
    gl = canvas.getContext('webgl2');
    if(!gl){
        console.log('Failed to get the rendering context for WebGL');
        return ;
    }

    program = compileShader(gl, VSHADER_SOURCE, FSHADER_SOURCE);
    gl.useProgram(program);

    program.a_Position = gl.getAttribLocation(program, 'a_Position'); 
    program.a_TexCoord = gl.getAttribLocation(program, 'a_TexCoord'); 
    program.a_Normal = gl.getAttribLocation(program, 'a_Normal'); 
    program.u_MvpMatrix = gl.getUniformLocation(program, 'u_MvpMatrix'); 
    program.u_modelMatrix = gl.getUniformLocation(program, 'u_modelMatrix'); 
    program.u_normalMatrix = gl.getUniformLocation(program, 'u_normalMatrix');
    program.u_LightPosition = gl.getUniformLocation(program, 'u_LightPosition');
    program.u_MvpMatrixOfLight = gl.getUniformLocation(program, 'u_MvpMatrixOfLight');
    program.u_ViewPosition = gl.getUniformLocation(program, 'u_ViewPosition');
    program.u_Ka = gl.getUniformLocation(program, 'u_Ka'); 
    program.u_Kd = gl.getUniformLocation(program, 'u_Kd');
    program.u_Ks = gl.getUniformLocation(program, 'u_Ks');
    program.u_shininess = gl.getUniformLocation(program, 'u_shininess');
    program.u_ShadowMap = gl.getUniformLocation(program, "u_ShadowMap");
    program.u_Sampler = gl.getUniformLocation(program, "u_Sampler");

    var quad = new Float32Array(
        [
          -1, -1, 0,
           1, -1, 0,
          -1,  1, 0,
          -1,  1, 0,
           1, -1, 0,
           1,  1, 0
        ]); //just a quad

    quadObj = initVertexBufferForLaterUse(gl, quad);

    var envQuad = new Float32Array(
		[
		  -1, -1, 1,
		   1, -1, 1,
		  -1,  1, 1,
		  -1,  1, 1,
		   1, -1, 1,
		   1,  1, 1
        ]); //just a quad

    envQuadObj = initVertexBufferForLaterUse(gl, envQuad);

    shadowProgram = compileShader(gl, VSHADER_SHADOW_SOURCE, FSHADER_SHADOW_SOURCE);

    shadowProgram.a_Position = gl.getAttribLocation(shadowProgram, 'a_Position');
    shadowProgram.u_MvpMatrix = gl.getUniformLocation(shadowProgram, 'u_MvpMatrix');
        
    programCube = compileShader(gl, VSHADER_SOURCE_ENVCUBE, FSHADER_SOURCE_ENVCUBE);

    programCube.a_Position = gl.getAttribLocation(programCube, 'a_Position'); 
    programCube.u_envCubeMap = gl.getUniformLocation(programCube, 'u_envCubeMap'); 
    programCube.u_viewDirectionProjectionInverse = gl.getUniformLocation(programCube, 'u_viewDirectionProjectionInverse');

    quadProgram = compileShader(gl, VSHADER_QUAD_SOURCE, FSHADER_QUAD_SOURCE);
    quadProgram.a_Position = gl.getAttribLocation(quadProgram, 'a_Position');
    quadProgram.u_ShadowMap = gl.getUniformLocation(quadProgram, "u_ShadowMap");

    fbo = initFrameBuffer(gl);

    //steve
    response = await fetch('steve.obj');
    text = await response.text();
    obj = parseOBJ(text);

    for( let i=0; i < obj.geometries.length; i ++ ){
      let o = initVertexBufferForLaterUse(gl, 
                                          obj.geometries[i].data.position,
                                          obj.geometries[i].data.normal, 
                                          obj.geometries[i].data.texcoord);
      steve.push(o);
    }

    for( let i=0; i < steveImgNames.length; i ++ ){
      let image = new Image();
      image.onload = function(){initTexture(gl, image, steveImgNames[i], snumTextures);};
      image.src = steveImgNames[i];
    }

    //fox
    response = await fetch('low-poly-fox-by-pixelmannen.obj');
    text = await response.text();
    obj = parseOBJ(text);

    for( let i=0; i < obj.geometries.length; i ++ ){
        let o = initVertexBufferForLaterUse(gl, 
                                            obj.geometries[i].data.position,
                                            obj.geometries[i].data.normal, 
                                            obj.geometries[i].data.texcoord);
        fox.push(o);
    }

    for( let i=0; i < foxImgNames.length; i ++ ){
        texCount = 0;
        let image = new Image();
        image.onload = function(){initTexture(gl, image, foxImgNames[i], fnumTextures);};
        image.src = foxImgNames[i];
    }

    //grass
    response = await fetch('mc-grass-block.obj');
    text = await response.text();
    obj = parseOBJ(text);

    for( let i=0; i < obj.geometries.length; i ++ ){
        let o = initVertexBufferForLaterUse(gl, 
                                            obj.geometries[i].data.position,
                                            obj.geometries[i].data.normal, 
                                            obj.geometries[i].data.texcoord);
        grass.push(o);
    }

    for( let i=0; i < grassImgNames.length; i ++ ){
        texCount = 0;
        let image = new Image();
        image.onload = function(){initTexture(gl, image, grassImgNames[i], gnumTextures);};
        image.src = grassImgNames[i];
    }
    
    mvpMatrix = new Matrix4();
    modelMatrix = new Matrix4();
    normalMatrix = new Matrix4();

    gl.enable(gl.DEPTH_TEST);

    cubeMapTex = initCubeTexture("posx.jpg", "negx.jpg", "negy.jpg", "posy.jpg", "posz.jpg", "negz.jpg", 512, 512)

    draw();//draw it once before mouse move

    canvas.onmousedown = function(ev){mouseDown(ev)};
    canvas.onmousemove = function(ev){mouseMove(ev)};
    canvas.onmouseup = function(ev){mouseUp(ev)};
    document.onkeydown = function(ev){keydown(ev)};
    var menu = document.getElementById("menu");
    menu.onchange = function(){
        if(this.value == "normal"){
            normalMode = true;
        }
        else{
            normalMode = false;
        }
        draw();
    }
}

function draw(){
    gl.useProgram(shadowProgram);
    gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);
    gl.viewport(0, 0, offScreenWidth, offScreenHeight);
    gl.clearColor(0.0, 0.0, 0.0, 1);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.enable(gl.DEPTH_TEST);

    //grass cube
    let grassMdlMatrix = new Matrix4();
    grassMdlMatrix.setTranslate(0.0, -1.5, 0.0);
    grassMdlMatrix.scale(grassScale, grassScale/10, grassScale);
    let grassMvpFromLight = drawOffScreen(grass, grassMdlMatrix);

    //steve
    let steveMdlMatrix = new Matrix4();
    steveMdlMatrix.setTranslate(-1.5, -1.4, -1.0);
    steveMdlMatrix.scale(steveScale, steveScale, steveScale);
    let steveMvpFromLight = drawOffScreen(steve, steveMdlMatrix);

    //fox
    let foxMdlMatrix = new Matrix4();
    foxMdlMatrix.setTranslate(-0.5, -1.3, -1.0);
    foxMdlMatrix.scale(foxScale, foxScale, foxScale);
    let foxMvpFromLight = drawOffScreen(fox, foxMdlMatrix);

    let rotateMatrix = new Matrix4();
	rotateMatrix.setRotate(angleX, 0, 1, 0);//for mouse rotation
	rotateMatrix.rotate(angleY, 1, 0, 0);//for mouse rotation
	var viewDir= new Vector3([cameraDirX, cameraDirY, cameraDirZ]);
	var newViewDir = rotateMatrix.multiplyVector3(viewDir);
  
	var vpFromCamera = new Matrix4();
	vpFromCamera.setPerspective(60, 1, 1, 15);
	var viewMatrixRotationOnly = new Matrix4();
	viewMatrixRotationOnly.lookAt(cameraX, cameraY, cameraZ, 
								  cameraX + newViewDir.elements[0], 
								  cameraY + newViewDir.elements[1], 
								  cameraZ + newViewDir.elements[2], 
								  0, -1, 0);
	viewMatrixRotationOnly.elements[12] = 0; //ignore translation
	viewMatrixRotationOnly.elements[13] = 0;
	viewMatrixRotationOnly.elements[14] = 0;
	vpFromCamera.multiply(viewMatrixRotationOnly);
    var vpFromCameraInverse = vpFromCamera.invert();

    if( normalMode ){
        gl.bindFramebuffer(gl.FRAMEBUFFER, null);
        gl.viewport(0, 0, canvas.width, canvas.height);
        gl.clearColor(0.4,0.4,0.4,1);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        gl.enable(gl.DEPTH_TEST);

        //quad for cube map
        gl.useProgram(programCube);
        gl.depthFunc(gl.LEQUAL);
        gl.uniformMatrix4fv(programCube.u_viewDirectionProjectionInverse, 
                            false, vpFromCameraInverse.elements);
        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubeMapTex);
        gl.uniform1i(programCube.u_envCubeMap, 0);
        initAttributeVariable(gl, programCube.a_Position, envQuadObj.vertexBuffer);
        gl.drawArrays(gl.TRIANGLES, 0, envQuadObj.numVertices);

        gl.useProgram(program);

        //steve
        drawOneObject(steve, steveImgIndex, steveMdlMatrix, steveMvpFromLight, newViewDir);

        //fox
        drawOneObject(fox, foxImgIndex, foxMdlMatrix, foxMvpFromLight, newViewDir);

        //grass
        drawOneObject(grass, grassImgIndex, grassMdlMatrix, grassMvpFromLight, newViewDir);
    }
    else{
        //active the quadProgram
        gl.useProgram(quadProgram);
        gl.bindFramebuffer(gl.FRAMEBUFFER, null);
        gl.viewport(0, 0, canvas.width, canvas.height);
        //switch the destination back to normal canvas color buffer
        gl.clearColor(0.0,0.0,0.0,1);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        gl.enable(gl.DEPTH_TEST);
        //pass fbo.texture into the quadProgram
        gl.uniform1i(quadProgram.u_ShadowMap, 0);
        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_2D, fbo.texture);
        //draw the quad()
        initAttributeVariable(gl, quadProgram.a_Position, quadObj.vertexBuffer);
        gl.drawArrays(gl.TRIANGLES, 0, quadObj.numVertices);
    }
}

function drawOffScreen(obj, mdlMatrix){
    var mvpFromLight = new Matrix4();
    //model Matrix (part of the mvp matrix)
    let modelMatrix = new Matrix4();
    modelMatrix.setRotate(angleY, 1, 0, 0);
    modelMatrix.rotate(angleX, 0, 1, 0);
    modelMatrix.multiply(mdlMatrix);
    //mvp: projection * view * model matrix  
    mvpFromLight.setPerspective(70, offScreenWidth/offScreenHeight, 1, 15);
    mvpFromLight.lookAt(lightX, lightY, lightZ, 0, 0, 0, 0, 1, 0);
    mvpFromLight.multiply(modelMatrix);
  
    gl.uniformMatrix4fv(shadowProgram.u_MvpMatrix, false, mvpFromLight.elements);
  
    for( let i=0; i < obj.length; i ++ ){
      initAttributeVariable(gl, shadowProgram.a_Position, obj[i].vertexBuffer);
      gl.drawArrays(gl.TRIANGLES, 0, obj[i].numVertices);
    }
  
    return mvpFromLight;
}

function drawOneObject(objComponents, objCompImgIndex, mdlMatrix, mvpFromLight, newViewDir){
    //model Matrix (part of the mvp matrix)
    modelMatrix.setRotate(angleY, 1, 0, 0);//for mouse rotation
    modelMatrix.rotate(angleX, 0, 1, 0);//for mouse rotation
    modelMatrix.multiply(mdlMatrix);
    //mvp: projection * view * model matrix  
    mvpMatrix.setPerspective(30, 1, 1, 100);
    mvpMatrix.lookAt(cameraX, cameraY, cameraZ, cameraX + newViewDir.elements[0], cameraY + newViewDir.elements[1], cameraZ + newViewDir.elements[2], 0, 1, 0);
    mvpMatrix.multiply(modelMatrix);

    //normal matrix
    normalMatrix.setInverseOf(modelMatrix);
    normalMatrix.transpose();

    gl.uniform3f(program.u_LightPosition, 0, 0, 3);
    gl.uniform3f(program.u_ViewPosition, cameraX, cameraY, cameraZ);
    gl.uniform1f(program.u_Ka, 0.2);
    gl.uniform1f(program.u_Kd, 0.7);
    gl.uniform1f(program.u_Ks, 1.0);
    gl.uniform1f(program.u_shininess, 10.0);
    gl.uniform1i(program.u_ShadowMap, 0);
    gl.uniform1i(program.u_normalMode, normalMode);

    gl.uniformMatrix4fv(program.u_MvpMatrix, false, mvpMatrix.elements);
    gl.uniformMatrix4fv(program.u_modelMatrix, false, modelMatrix.elements);
    gl.uniformMatrix4fv(program.u_normalMatrix, false, normalMatrix.elements);
    gl.uniformMatrix4fv(program.u_MvpMatrixOfLight, false, mvpFromLight.elements);

    for( let i=0; i < objComponents.length; i ++ ){
      gl.activeTexture(gl.TEXTURE0);
      gl.bindTexture(gl.TEXTURE_2D, textures[objCompImgIndex[i]]);
      gl.uniform1i(program.u_Sampler, 0);

      initAttributeVariable(gl, program.a_Position, objComponents[i].vertexBuffer);
      initAttributeVariable(gl, program.a_TexCoord, objComponents[i].texCoordBuffer);
      initAttributeVariable(gl, program.a_Normal, objComponents[i].normalBuffer);

      gl.drawArrays(gl.TRIANGLES, 0, objComponents[i].numVertices);
    }
}

function initCubeTexture(posXName, negXName, posYName, negYName, posZName, negZName, imgWidth, imgHeight)
{
	var texture = gl.createTexture();

	const faceInfos = [
		{
			target: gl.TEXTURE_CUBE_MAP_POSITIVE_X,
			fName: posXName,
		},
		{
			target: gl.TEXTURE_CUBE_MAP_NEGATIVE_X,
			fName: negXName,
		},
		{
			target: gl.TEXTURE_CUBE_MAP_POSITIVE_Y,
			fName: posYName,
		},
		{
			target: gl.TEXTURE_CUBE_MAP_NEGATIVE_Y,
			fName: negYName,
		},
		{
			target: gl.TEXTURE_CUBE_MAP_POSITIVE_Z,
			fName: posZName,
		},
		{
			target: gl.TEXTURE_CUBE_MAP_NEGATIVE_Z,
			fName: negZName,
		},
	];
	faceInfos.forEach((faceInfo) => {
		const {target, fName} = faceInfo;
		// setup each face so it's immediately renderable (avoid error message)
		gl.bindTexture(gl.TEXTURE_CUBE_MAP, texture);
		gl.texImage2D(target, 0, gl.RGBA, imgWidth, imgHeight, 0, 
		gl.RGBA, gl.UNSIGNED_BYTE, null);

		var image = new Image();
		image.onload = function(){
		gl.bindTexture(gl.TEXTURE_CUBE_MAP, texture);
		gl.texImage2D(target, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
		// gl.generateMipmap(gl.TEXTURE_CUBE_MAP);
		// gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
		gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
		};
		image.src = fName;
	});

	return texture;
}

function parseOBJ(text) {
  // because indices are base 1 let's just fill in the 0th data
  const objPositions = [[0, 0, 0]];
  const objTexcoords = [[0, 0]];
  const objNormals = [[0, 0, 0]];

  // same order as `f` indices
  const objVertexData = [
    objPositions,
    objTexcoords,
    objNormals,
  ];

  // same order as `f` indices
  let webglVertexData = [
    [],   // positions
    [],   // texcoords
    [],   // normals
  ];

  const materialLibs = [];
  const geometries = [];
  let geometry;
  let groups = ['default'];
  let material = 'default';
  let object = 'default';

  const noop = () => {};

  function newGeometry() {
    // If there is an existing geometry and it's
    // not empty then start a new one.
    if (geometry && geometry.data.position.length) {
      geometry = undefined;
    }
  }

  function setGeometry() {
    if (!geometry) {
      const position = [];
      const texcoord = [];
      const normal = [];
      webglVertexData = [
        position,
        texcoord,
        normal,
      ];
      geometry = {
        object,
        groups,
        material,
        data: {
          position,
          texcoord,
          normal,
        },
      };
      geometries.push(geometry);
    }
  }

  function addVertex(vert) {
    const ptn = vert.split('/');
    ptn.forEach((objIndexStr, i) => {
      if (!objIndexStr) {
        return;
      }
      const objIndex = parseInt(objIndexStr);
      const index = objIndex + (objIndex >= 0 ? 0 : objVertexData[i].length);
      webglVertexData[i].push(...objVertexData[i][index]);
    });
  }

  const keywords = {
    v(parts) {
      objPositions.push(parts.map(parseFloat));
    },
    vn(parts) {
      objNormals.push(parts.map(parseFloat));
    },
    vt(parts) {
      // should check for missing v and extra w?
      objTexcoords.push(parts.map(parseFloat));
    },
    f(parts) {
      setGeometry();
      const numTriangles = parts.length - 2;
      for (let tri = 0; tri < numTriangles; ++tri) {
        addVertex(parts[0]);
        addVertex(parts[tri + 1]);
        addVertex(parts[tri + 2]);
      }
    },
    s: noop,    // smoothing group
    mtllib(parts, unparsedArgs) {
      // the spec says there can be multiple filenames here
      // but many exist with spaces in a single filename
      materialLibs.push(unparsedArgs);
    },
    usemtl(parts, unparsedArgs) {
      material = unparsedArgs;
      newGeometry();
    },
    g(parts) {
      groups = parts;
      newGeometry();
    },
    o(parts, unparsedArgs) {
      object = unparsedArgs;
      newGeometry();
    },
  };

  const keywordRE = /(\w*)(?: )*(.*)/;
  const lines = text.split('\n');
  for (let lineNo = 0; lineNo < lines.length; ++lineNo) {
    const line = lines[lineNo].trim();
    if (line === '' || line.startsWith('#')) {
      continue;
    }
    const m = keywordRE.exec(line);
    if (!m) {
      continue;
    }
    const [, keyword, unparsedArgs] = m;
    const parts = line.split(/\s+/).slice(1);
    const handler = keywords[keyword];
    if (!handler) {
      console.warn('unhandled keyword:', keyword);  // eslint-disable-line no-console
      continue;
    }
    handler(parts, unparsedArgs);
  }

  // remove any arrays that have no entries.
  for (const geometry of geometries) {
    geometry.data = Object.fromEntries(
        Object.entries(geometry.data).filter(([, array]) => array.length > 0));
  }

  return {
    geometries,
    materialLibs,
  };
}

function mouseDown(ev){ 
    var x = ev.clientX;
    var y = ev.clientY;
    var rect = ev.target.getBoundingClientRect();
    if( rect.left <= x && x < rect.right && rect.top <= y && y < rect.bottom){
        mouseLastX = x;
        mouseLastY = y;
        mouseDragging = true;
    }
}

function mouseUp(ev){ 
    mouseDragging = false;
}

function mouseMove(ev){ 
    var x = ev.clientX;
    var y = ev.clientY;
    if( mouseDragging ){
        var factor = 100/canvas.height; //100 determine the spped you rotate the object
        var dx = factor * (x - mouseLastX);
        var dy = factor * (y - mouseLastY);

        angleX += dx; //yes, x for y, y for x, this is right
        angleY += dy;
    }
    mouseLastX = x;
    mouseLastY = y;

    draw();
}

function initFrameBuffer(gl){
    //create and set up a texture object as the color buffer
    var texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, offScreenWidth, offScreenHeight,
                    0, gl.RGBA, gl.UNSIGNED_BYTE, null);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    
  
    //create and setup a render buffer as the depth buffer
    var depthBuffer = gl.createRenderbuffer();
    gl.bindRenderbuffer(gl.RENDERBUFFER, depthBuffer);
    gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, 
                            offScreenWidth, offScreenHeight);
  
    //create and setup framebuffer: linke the color and depth buffer to it
    var frameBuffer = gl.createFramebuffer();
    gl.bindFramebuffer(gl.FRAMEBUFFER, frameBuffer);
    gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, 
                              gl.TEXTURE_2D, texture, 0);
    gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, 
                                gl.RENDERBUFFER, depthBuffer);
    frameBuffer.texture = texture;
    return frameBuffer;
}

function initTexture(gl, img, imgName, numTextures){
  var tex = gl.createTexture();
  gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);
  gl.bindTexture(gl.TEXTURE_2D, tex);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

  // Upload the image into the texture.
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);

  textures[imgName] = tex;

  texCount++;
  if( texCount == numTextures)draw();
}

function keydown(ev){ 
	//implment keydown event here
	let rotateMatrix = new Matrix4();
	rotateMatrix.setRotate(angleY, 1, 0, 0);//for mouse rotation
	rotateMatrix.rotate(angleX, 0, 1, 0);//for mouse rotation
	var viewDir= new Vector3([cameraDirX, cameraDirY, cameraDirZ]);
	var newViewDir = rotateMatrix.multiplyVector3(viewDir);
  
	if(ev.key == 'w'){ 
		cameraX += (newViewDir.elements[0] * 0.1);
		cameraY += (newViewDir.elements[1] * 0.1);
		cameraZ += (newViewDir.elements[2] * 0.1);
	}
	else if(ev.key == 's'){ 
	  cameraX -= (newViewDir.elements[0] * 0.1);
	  cameraY -= (newViewDir.elements[1] * 0.1);
	  cameraZ -= (newViewDir.elements[2] * 0.1);
	}
  
	console.log(cameraX, cameraY, cameraZ)
	draw();
}