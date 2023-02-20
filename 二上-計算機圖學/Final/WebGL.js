var VSHADER_SOURCE = `
    attribute vec4 a_Position;
    attribute vec4 a_Normal;
    uniform mat4 u_MvpMatrix;
    uniform mat4 u_modelMatrix;
    uniform mat4 u_normalMatrix;
    varying vec3 v_Normal;
    varying vec3 v_PositionInWorld;
    void main(){
        gl_Position = u_MvpMatrix * a_Position;
        v_PositionInWorld = (u_modelMatrix * a_Position).xyz; 
        v_Normal = normalize(vec3(u_normalMatrix * a_Normal));
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
    uniform vec3 u_Color;
    varying vec3 v_Normal;
    varying vec3 v_PositionInWorld;
    void main(){
        // let ambient and diffuse color are u_Color 
        // (you can also input them from ouside and make them different)
        vec3 ambientLightColor = u_Color;
        vec3 diffuseLightColor = u_Color;
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

        gl_FragColor = vec4( ambient + diffuse + specular, 1.0 );
    }
`;

var VSHADER_SOURCE_TEX = `
    attribute vec4 a_Position;
    attribute vec4 a_Normal;
    attribute vec2 a_TexCoord;
    uniform mat4 u_MvpMatrix;
    uniform mat4 u_modelMatrix;
    uniform mat4 u_normalMatrix;
    varying vec3 v_Normal;
    varying vec3 v_PositionInWorld;
    varying vec2 v_TexCoord;
    void main(){
        gl_Position = u_MvpMatrix * a_Position;
        v_PositionInWorld = (u_modelMatrix * a_Position).xyz; 
        v_Normal = normalize(vec3(u_normalMatrix * a_Normal));
        v_TexCoord = a_TexCoord;
    }    
`;

var FSHADER_SOURCE_TEX = `
    precision mediump float;
    uniform vec3 u_LightPosition;
    uniform vec3 u_ViewPosition;
    uniform float u_Ka;
    uniform float u_Kd;
    uniform float u_Ks;
    uniform float u_shininess;
    uniform sampler2D u_Sampler;
    varying vec3 v_Normal;
    varying vec3 v_PositionInWorld;
    varying vec2 v_TexCoord;
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

var VSHADER_SOURCE_TEXTURE_ON_CUBE = `
  attribute vec4 a_Position;
  attribute vec4 a_Normal;
  uniform mat4 u_MvpMatrix;
  uniform mat4 u_modelMatrix;
  uniform mat4 u_normalMatrix;
  varying vec4 v_TexCoord;
  varying vec3 v_Normal;
  varying vec3 v_PositionInWorld;
  void main() {
    gl_Position = u_MvpMatrix * a_Position;
    v_TexCoord = a_Position;
    v_PositionInWorld = (u_modelMatrix * a_Position).xyz; 
    v_Normal = normalize(vec3(u_normalMatrix * a_Normal));
  } 
`;

var FSHADER_SOURCE_TEXTURE_ON_CUBE = `
  precision mediump float;
  varying vec4 v_TexCoord;
  uniform vec3 u_ViewPosition;
  uniform vec3 u_Color;
  uniform samplerCube u_envCubeMap;
  varying vec3 v_Normal;
  varying vec3 v_PositionInWorld;
  void main() {
    vec3 V = normalize(u_ViewPosition - v_PositionInWorld); 
    vec3 normal = normalize(v_Normal);
    vec3 R = reflect(-V, normal);
    gl_FragColor = vec4(0.78 * textureCube(u_envCubeMap, R).rgb + 0.3 * u_Color, 1.0);
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
        console.log('vertex shader ereror');
        var message = gl.getShaderInfoLog(vertexShader); 
        console.log(message);//print shader compiling error message
    }
    //compile fragment shader
    gl.compileShader(fragmentShader)
    if(!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)){
        console.log('fragment shader ereror');
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

function initVertexBufferForLaterUse(gl, vertices, normals, texCoords, tagents, bitagents, crossTexCoords){
  var nVertices = vertices.length / 3;

  var o = new Object();
  o.vertexBuffer = initArrayBufferForLaterUse(gl, new Float32Array(vertices), 3, gl.FLOAT);
  if( normals != null ) o.normalBuffer = initArrayBufferForLaterUse(gl, new Float32Array(normals), 3, gl.FLOAT);
  if( texCoords != null ) o.texCoordBuffer = initArrayBufferForLaterUse(gl, new Float32Array(texCoords), 2, gl.FLOAT);
  if( tagents != null ) o.tagentsBuffer = initArrayBufferForLaterUse(gl, new Float32Array(tagents), 3, gl.FLOAT);
  if( bitagents != null ) o.bitagentsBuffer = initArrayBufferForLaterUse(gl, new Float32Array(bitagents), 3, gl.FLOAT);
  if( crossTexCoords != null ) o.crossTexCoordsBuffer = initArrayBufferForLaterUse(gl, new Float32Array(crossTexCoords), 1, gl.FLOAT);
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
var angleX = 0, angleY = 0, rotateAngle = 0, steveTurn = 90;
var gl, canvas;
var mvpMatrix;
var modelMatrix;
var normalMatrix;
var nVertex;
var cameraX = 0, cameraY = 2, cameraZ = 15; //Y=0+2, Z=10+5
var lightX = 5, lightY = 1, lightZ = 7;
var cameraDirX = 0, cameraDirY = 0, cameraDirZ = -1;
var fbo;
var cubeMapTex;
var cube = [];
var pyramid = [];
var sphere = [];
var cubeScale = 0.4;
var pyramidScale = cubeScale*2;
var steveScale = 0.2;
var firstPersonView = 1;
var steve = [];
var textures = {};
var imgNames = ["Steve.png"];
var imgIndex = ["Steve.png", "Steve.png", "Steve.png", "Steve.png", "Steve.png", "Steve.png"];
var texCount = 0;
var numTextures = imgNames.length;
var offScreenWidth = 256, offScreenHeight = 256;
var firstCount = 0, thirdCount = 0;

//normal vector calculation (for the cube)
function getNormalOnVertices(vertices){
	var normals = [];
	var nTriangles = vertices.length/9;
	for(let i=0; i < nTriangles; i ++ ){
		var idx = i * 9 + 0 * 3;
		var p0x = vertices[idx+0], p0y = vertices[idx+1], p0z = vertices[idx+2];
		idx = i * 9 + 1 * 3;
		var p1x = vertices[idx+0], p1y = vertices[idx+1], p1z = vertices[idx+2];
		idx = i * 9 + 2 * 3;
		var p2x = vertices[idx+0], p2y = vertices[idx+1], p2z = vertices[idx+2];

		var ux = p1x - p0x, uy = p1y - p0y, uz = p1z - p0z;
		var vx = p2x - p0x, vy = p2y - p0y, vz = p2z - p0z;

		var nx = uy*vz - uz*vy;
		var ny = uz*vx - ux*vz;
		var nz = ux*vy - uy*vx;

		var norm = Math.sqrt(nx*nx + ny*ny + nz*nz);
		nx = nx / norm;
		ny = ny / norm;
		nz = nz / norm;

		normals.push(nx, ny, nz, nx, ny, nz, nx, ny, nz);
	}
	return normals;
}

async function main(){
    canvas = document.getElementById('webgl');
    gl = canvas.getContext('webgl2');
    if(!gl){
        console.log('Failed to get the rendering context for WebGL');
        return ;
    }

    //compile shader
    program = compileShader(gl, VSHADER_SOURCE, FSHADER_SOURCE);
    texProgram = compileShader(gl, VSHADER_SOURCE_TEX, FSHADER_SOURCE_TEX);
    cubeProgram = compileShader(gl, VSHADER_SOURCE_ENVCUBE, FSHADER_SOURCE_ENVCUBE);
    programTextureOnCube = compileShader(gl, VSHADER_SOURCE_TEXTURE_ON_CUBE, FSHADER_SOURCE_TEXTURE_ON_CUBE);

    //get location
    program.a_Position = gl.getAttribLocation(program, 'a_Position'); 
    program.a_Normal = gl.getAttribLocation(program, 'a_Normal'); 
    program.u_MvpMatrix = gl.getUniformLocation(program, 'u_MvpMatrix'); 
    program.u_modelMatrix = gl.getUniformLocation(program, 'u_modelMatrix'); 
    program.u_normalMatrix = gl.getUniformLocation(program, 'u_normalMatrix');
    program.u_LightPosition = gl.getUniformLocation(program, 'u_LightPosition');
    program.u_ViewPosition = gl.getUniformLocation(program, 'u_ViewPosition');
    program.u_Ka = gl.getUniformLocation(program, 'u_Ka'); 
    program.u_Kd = gl.getUniformLocation(program, 'u_Kd');
    program.u_Ks = gl.getUniformLocation(program, 'u_Ks');
    program.u_shininess = gl.getUniformLocation(program, 'u_shininess');
    program.u_Color = gl.getUniformLocation(program, 'u_Color');

    texProgram.a_Position = gl.getAttribLocation(texProgram, 'a_Position'); 
    texProgram.a_TexCoord = gl.getAttribLocation(texProgram, 'a_TexCoord'); 
    texProgram.a_Normal = gl.getAttribLocation(texProgram, 'a_Normal'); 
    texProgram.u_MvpMatrix = gl.getUniformLocation(texProgram, 'u_MvpMatrix'); 
    texProgram.u_modelMatrix = gl.getUniformLocation(texProgram, 'u_modelMatrix'); 
    texProgram.u_normalMatrix = gl.getUniformLocation(texProgram, 'u_normalMatrix');
    texProgram.u_LightPosition = gl.getUniformLocation(texProgram, 'u_LightPosition');
    texProgram.u_ViewPosition = gl.getUniformLocation(texProgram, 'u_ViewPosition');
    texProgram.u_Ka = gl.getUniformLocation(texProgram, 'u_Ka'); 
    texProgram.u_Kd = gl.getUniformLocation(texProgram, 'u_Kd');
    texProgram.u_Ks = gl.getUniformLocation(texProgram, 'u_Ks');
    texProgram.u_shininess = gl.getUniformLocation(texProgram, 'u_shininess');
    texProgram.u_Sampler = gl.getUniformLocation(texProgram, "u_Sampler");
    
    cubeProgram.a_Position = gl.getAttribLocation(cubeProgram, 'a_Position'); 
    cubeProgram.u_envCubeMap = gl.getUniformLocation(cubeProgram, 'u_envCubeMap'); 
    cubeProgram.u_viewDirectionProjectionInverse = gl.getUniformLocation(cubeProgram, 'u_viewDirectionProjectionInverse');

    programTextureOnCube.a_Position = gl.getAttribLocation(programTextureOnCube, 'a_Position'); 
    programTextureOnCube.a_Normal = gl.getAttribLocation(programTextureOnCube, 'a_Normal'); 
    programTextureOnCube.u_MvpMatrix = gl.getUniformLocation(programTextureOnCube, 'u_MvpMatrix'); 
    programTextureOnCube.u_modelMatrix = gl.getUniformLocation(programTextureOnCube, 'u_modelMatrix'); 
    programTextureOnCube.u_normalMatrix = gl.getUniformLocation(programTextureOnCube, 'u_normalMatrix');
    programTextureOnCube.u_ViewPosition = gl.getUniformLocation(programTextureOnCube, 'u_ViewPosition');
    programTextureOnCube.u_envCubeMap = gl.getUniformLocation(programTextureOnCube, 'u_envCubeMap'); 
    programTextureOnCube.u_Color = gl.getUniformLocation(programTextureOnCube, 'u_Color'); 
    
    var quad = new Float32Array(
		[
		  -1, -1, 1,
		   1, -1, 1,
		  -1,  1, 1,
		  -1,  1, 1,
		   1, -1, 1,
		   1,  1, 1
        ]); //just a quad
        
    fbo = initFrameBufferForCubemapRendering(gl);

    quadObj = initVertexBufferForLaterUse(gl, quad);

    mvpMatrix = new Matrix4();
    modelMatrix = new Matrix4();
    normalMatrix = new Matrix4();

    //3D object

    //big flat cuboid
    cubeVertices = [-1.0,1.0,1.0, -1.0,-1.0,1.0, 1.0,-1.0,1.0, -1.0,1.0,1.0, 1.0,-1.0,1.0, 1.0,1.0,1.0,   //face z = 1.0
        1.0,1.0,1.0, 1.0,-1.0,1.0, 1.0,-1.0,-1.0, 1.0,1.0,1.0, 1.0,-1.0,-1.0, 1.0,1.0,-1.0,   //face x = 1.0
        1.0,1.0,1.0, 1.0,1.0,-1.0, -1.0,1.0,1.0, 1.0,1.0,-1.0, -1.0,1.0,-1.0, -1.0,1.0,1.0,   //face y = 1.0
        -1.0,1.0,1.0, -1.0,-1.0,1.0, -1.0,-1.0,-1.0, -1.0,1.0,1.0, -1.0,-1.0,-1.0, -1.0,1.0,-1.0,   //face x = -1.0
        -1.0,-1.0,1.0, 1.0,-1.0,1.0, 1.0,-1.0,-1.0, -1.0,-1.0,1.0, 1.0,-1.0,-1.0, -1.0,-1.0,-1.0,   //face y = -1.0
        -1.0,1.0,-1.0, -1.0,-1.0,-1.0, 1.0,-1.0,-1.0, -1.0,1.0,-1.0, 1.0,-1.0,-1.0, 1.0,1.0,-1.0,   //face z = -1.0
    ];
    cubeNormals = getNormalOnVertices(cubeVertices);
    let o = initVertexBufferForLaterUse(gl, cubeVertices, cubeNormals, null);
    cube.push(o);

    //pyramid
    pyramidVertices = [ 0.0,1.0,0.0, 0.5,0.0,-0.5, 0.5,0.0,0.5,
                        0.0,1.0,0.0, -0.5,0.0,0.5, 0.5,0.0,0.5,
                        0.0,1.0,0.0, -0.5,0.0,-0.5, -0.5,0.0,0.5,
                        0.0,1.0,0.0, -0.5,0.0,-0.5, 0.5,0.0,-0.5,
                        0.5,0.0,-0.5, -0.5,0.0,-0.5, -0.5,0.0,0.5, 0.5,0.0,-0.5, -0.5,0.0,0.5, 0.5,0.0,0.5
    ];
    pyramidNormals = getNormalOnVertices(pyramidVertices);
    let p = initVertexBufferForLaterUse(gl, pyramidVertices, pyramidNormals, null);
    pyramid.push(p);

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

    for( let i=0; i < imgNames.length; i ++ ){
        let image = new Image();
        image.onload = function(){initTexture(gl, image, imgNames[i]);};
        image.src = imgNames[i];
    }

    //sphere
    response = await fetch('sphere.obj');
    text = await response.text();
    obj = parseOBJ(text);

    for( let i=0; i < obj.geometries.length; i ++ ){
      let o = initVertexBufferForLaterUse(gl, 
                                          obj.geometries[i].data.position,
                                          obj.geometries[i].data.normal, 
                                          obj.geometries[i].data.texcoord);
      sphere.push(o);
    }

    cubeMapTex = initCubeTexture("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg", 512, 512)

    //mouse&keyboard event
    canvas.onmousedown = function(ev){mouseDown(ev)};
    canvas.onmousemove = function(ev){mouseMove(ev)};
    canvas.onmouseup = function(ev){mouseUp(ev)};
    document.onkeydown = function(ev){keydown(ev)};
    var menu = document.getElementById("menu");
    menu.onchange = function() {
        if(this.value == "first") firstPersonView = 1;
        else firstPersonView = 0;
        draw();
    }

    var tick = function() {
        rotateAngle += 0.5;
        draw();
        requestAnimationFrame(tick);
      }
      tick();
}

var mdlMatrix = new Matrix4();
var matStack = [];

function pushMatrix(){
    matStack.push(new Matrix4(mdlMatrix));
}

function popMatrix(){
    mdlMatrix = matStack.pop();
}

function draw(){
    renderCubeMap(0, 0, 0);

    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(0.4,0.4,0.4,1);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.enable(gl.DEPTH_TEST);

    let rotateMatrix = new Matrix4();
	rotateMatrix.setRotate(angleX, 0, 1, 0);//for mouse rotation
	rotateMatrix.rotate(angleY, 1, 0, 0);//for mouse rotation
	var viewDir= new Vector3([cameraDirX, cameraDirY, cameraDirZ]);
    var newViewDir = rotateMatrix.multiplyVector3(viewDir);
    
    let vpMatrix = new Matrix4();
    vpMatrix.setPerspective(70, 1, 1, 100);
    vpMatrix.lookAt(cameraX, cameraY, cameraZ,   
                    cameraX + newViewDir.elements[0], 
                    cameraY + newViewDir.elements[1],
                    cameraZ + newViewDir.elements[2], 
                    0, 1, 0);

    drawEnvMap(vpMatrix);

    drawRegularObjects(vpMatrix);

    if( firstPersonView ){
        thirdCount = 0;
        if(firstCount==0){
            cameraY-=2;
            cameraZ-=5;
        }
        firstCount++;
    }
    else{
        firstCount = 0;
        if(thirdCount==0){
            cameraY+=2;
            cameraZ+=5;
        }
        thirdCount++;
    }
}

function drawRegularObjects(vpMatrix){
    let mdlMatrix = new Matrix4();

    //cube
    mdlMatrix.setIdentity();
    mdlMatrix.rotate(rotateAngle, 0, 1, 0);
    mdlMatrix.translate(2.5, 0, 0);
    mdlMatrix.scale(cubeScale, cubeScale, cubeScale);
    drawNoTex(cube, mdlMatrix, vpMatrix, 1.0, 0.5, 0.0);

    //pyramid
    mdlMatrix.setIdentity();
    mdlMatrix.translate(0, -cubeScale, 0);
    mdlMatrix.rotate(rotateAngle, 0, 1, 0);
    mdlMatrix.translate(-2.5, 0, 0);
    mdlMatrix.scale(pyramidScale, pyramidScale, pyramidScale);
    drawNoTex(pyramid, mdlMatrix, vpMatrix, 1.0, 0.5, 0.0);

    //steve
    mdlMatrix.setIdentity();
    if(firstCount == 0){
        mdlMatrix.translate(cameraX,cameraY-3,cameraZ-5);
    }
    else{
        mdlMatrix.translate(cameraX,cameraY-1,cameraZ);
    }
    mdlMatrix.rotate(steveTurn, 0, 1, 0);
    mdlMatrix.scale(steveScale, steveScale, steveScale);
    drawWithTex(steve, imgIndex, mdlMatrix, vpMatrix);

    //the sphere
    mdlMatrix.setIdentity();
    mdlMatrix.scale(0.5, 0.5, 0.5);
    drawObjectWithDynamicReflection(sphere, mdlMatrix, vpMatrix, 0.5, 0.5, 0.5);
}

function drawEnvMap(vpMatrix){
    var vpFromCamera = new Matrix4();
    vpFromCamera.multiply(vpMatrix);
    var vpFromCameraInverse = vpFromCamera.invert();
  
    //quad
    gl.useProgram(cubeProgram);
    gl.depthFunc(gl.LEQUAL);
    gl.uniformMatrix4fv(cubeProgram.u_viewDirectionProjectionInverse, 
                        false, vpFromCameraInverse.elements);
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubeMapTex);
    gl.uniform1i(cubeProgram.u_envCubeMap, 0);
    initAttributeVariable(gl, cubeProgram.a_Position, quadObj.vertexBuffer);
    gl.drawArrays(gl.TRIANGLES, 0, quadObj.numVertices);
}

function drawNoTex(obj, modelMatrix, vpMatrix, colorR, colorG, colorB){
    gl.useProgram(program);
    let mvpMatrix = new Matrix4();
    mvpMatrix.set(vpMatrix);
    mvpMatrix.multiply(modelMatrix);

    //normal matrix
    let normalMatrix = new Matrix4();
    normalMatrix.setInverseOf(modelMatrix);
    normalMatrix.transpose();

    gl.uniform3f(program.u_LightPosition, lightX, lightY, lightZ);
    //gl.uniform3f(program.u_ViewPosition, cameraX+camX, cameraY+camY, cameraZ+camZ)
    gl.uniform3f(program.u_ViewPosition, cameraX, cameraY, cameraZ);
    gl.uniform1f(program.u_Ka, 0.2);
    gl.uniform1f(program.u_Kd, 0.7);
    gl.uniform1f(program.u_Ks, 1.0);
    gl.uniform1f(program.u_shininess, 10.0);
    gl.uniform3f(program.u_Color, colorR, colorG, colorB);

    gl.uniformMatrix4fv(program.u_MvpMatrix, false, mvpMatrix.elements);
    gl.uniformMatrix4fv(program.u_modelMatrix, false, modelMatrix.elements);
    gl.uniformMatrix4fv(program.u_normalMatrix, false, normalMatrix.elements);

    for( let i=0; i < obj.length; i ++ ){
      initAttributeVariable(gl, program.a_Position, obj[i].vertexBuffer);
      initAttributeVariable(gl, program.a_Normal, obj[i].normalBuffer);
      gl.drawArrays(gl.TRIANGLES, 0, obj[i].numVertices);
    }
}

function drawWithTex(obj, ImgIndex, modelMatrix, vpMatrix){
    gl.useProgram(texProgram);
    let mvpMatrix = new Matrix4();
    mvpMatrix.set(vpMatrix);
    mvpMatrix.multiply(modelMatrix);

    //normal matrix
    let normalMatrix = new Matrix4();
    normalMatrix.setInverseOf(modelMatrix);
    normalMatrix.transpose();

    gl.uniform3f(texProgram.u_LightPosition, 0, 0, 3);
    // gl.uniform3f(texProgram.u_ViewPosition, cameraX+camX, cameraY+camY, cameraZ+camZ);
    gl.uniform3f(texProgram.u_ViewPosition, cameraX, cameraY, cameraZ);
    gl.uniform1f(texProgram.u_Ka, 0.2);
    gl.uniform1f(texProgram.u_Kd, 0.7);
    gl.uniform1f(texProgram.u_Ks, 1.0);
    gl.uniform1f(texProgram.u_shininess, 10.0);

    gl.uniformMatrix4fv(texProgram.u_MvpMatrix, false, mvpMatrix.elements);
    gl.uniformMatrix4fv(texProgram.u_modelMatrix, false, modelMatrix.elements);
    gl.uniformMatrix4fv(texProgram.u_normalMatrix, false, normalMatrix.elements);

    for( let i=0; i < obj.length; i ++ ){
      gl.activeTexture(gl.TEXTURE0);
      gl.bindTexture(gl.TEXTURE_2D, textures[ImgIndex[i]]);
      gl.uniform1i(texProgram.u_Sampler, 0);

      initAttributeVariable(gl, texProgram.a_Position, obj[i].vertexBuffer);
      initAttributeVariable(gl, texProgram.a_TexCoord, obj[i].texCoordBuffer);
      initAttributeVariable(gl, texProgram.a_Normal, obj[i].normalBuffer);

      gl.drawArrays(gl.TRIANGLES, 0, obj[i].numVertices);
    }
}

function drawObjectWithDynamicReflection(obj, modelMatrix, vpMatrix, colorR, colorG, colorB){
    gl.useProgram(programTextureOnCube);
    let mvpMatrix = new Matrix4();
    let normalMatrix = new Matrix4();
    mvpMatrix.set(vpMatrix);
    mvpMatrix.multiply(modelMatrix);
  
    //normal matrix
    normalMatrix.setInverseOf(modelMatrix);
    normalMatrix.transpose();
  
    //gl.uniform3f(programTextureOnCube.u_ViewPosition, cameraX+camX, cameraY+camY, cameraZ+camZ);
    gl.uniform3f(programTextureOnCube.u_ViewPosition, cameraX, cameraY, cameraZ);
    gl.uniform3f(programTextureOnCube.u_Color, colorR, colorG, colorB);
  
    gl.uniformMatrix4fv(programTextureOnCube.u_MvpMatrix, false, mvpMatrix.elements);
    gl.uniformMatrix4fv(programTextureOnCube.u_modelMatrix, false, modelMatrix.elements);
    gl.uniformMatrix4fv(programTextureOnCube.u_normalMatrix, false, normalMatrix.elements);
  
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, fbo.texture);
    gl.uniform1i(programTextureOnCube.u_envCubeMap, 0);
  
    for( let i=0; i < obj.length; i ++ ){
      initAttributeVariable(gl, programTextureOnCube.a_Position, obj[i].vertexBuffer);
      initAttributeVariable(gl, programTextureOnCube.a_Normal, obj[i].normalBuffer);
      gl.drawArrays(gl.TRIANGLES, 0, obj[i].numVertices);
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

function initTexture(gl, img, imgName){
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

function initFrameBufferForCubemapRendering(gl){
  var texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_CUBE_MAP, texture);

  // 6 2D textures
  gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
  for (let i = 0; i < 6; i++) {
    gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
                  gl.RGBA, offScreenWidth, offScreenHeight, 0, gl.RGBA, 
                  gl.UNSIGNED_BYTE, null);
  }

  //create and setup a render buffer as the depth buffer
  var depthBuffer = gl.createRenderbuffer();
  gl.bindRenderbuffer(gl.RENDERBUFFER, depthBuffer);
  gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, 
                          offScreenWidth, offScreenHeight);

  //create and setup framebuffer: linke the depth buffer to it (no color buffer here)
  var frameBuffer = gl.createFramebuffer();
  gl.bindFramebuffer(gl.FRAMEBUFFER, frameBuffer);
  gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, 
                              gl.RENDERBUFFER, depthBuffer);

  frameBuffer.texture = texture;

  gl.bindFramebuffer(gl.FRAMEBUFFER, null);

  return frameBuffer;
}

function renderCubeMap(camX, camY, camZ)
{
  //camera 6 direction to render 6 cubemap faces
  var ENV_CUBE_LOOK_DIR = [
      [1.0, 0.0, 0.0],
      [-1.0, 0.0, 0.0],
      [0.0, 1.0, 0.0],
      [0.0, -1.0, 0.0],
      [0.0, 0.0, 1.0],
      [0.0, 0.0, -1.0]
  ];

  //camera 6 look up vector to render 6 cubemap faces
  var ENV_CUBE_LOOK_UP = [
      [0.0, -1.0, 0.0],
      [0.0, -1.0, 0.0],
      [0.0, 0.0, 1.0],
      [0.0, 0.0, -1.0],
      [0.0, -1.0, 0.0],
      [0.0, -1.0, 0.0]
  ];

  gl.useProgram(program);
  gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);
  gl.viewport(0, 0, offScreenWidth, offScreenHeight);
  gl.clearColor(0.4, 0.4, 0.4,1);
  for (var side = 0; side < 6;side++){
    gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, 
                            gl.TEXTURE_CUBE_MAP_POSITIVE_X+side, fbo.texture, 0);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    let vpMatrix = new Matrix4();
    vpMatrix.setPerspective(90, 1, 1, 100);
    vpMatrix.lookAt(camX, camY, camZ,   
                    camX + ENV_CUBE_LOOK_DIR[side][0], 
                    camY + ENV_CUBE_LOOK_DIR[side][1],
                    camZ + ENV_CUBE_LOOK_DIR[side][2], 
                    ENV_CUBE_LOOK_UP[side][0],
                    ENV_CUBE_LOOK_UP[side][1],
                    ENV_CUBE_LOOK_UP[side][2]);
    
    drawEnvMap(vpMatrix);
    drawRegularObjects(vpMatrix);
  }
  gl.bindFramebuffer(gl.FRAMEBUFFER, null);
}