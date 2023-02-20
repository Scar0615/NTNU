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

function compileShader(gl, vShaderText, fShaderText){
    //Build vertex and fragment shader objects
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

    //link shader to program (by a self-define function)
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

//The following three function is for creating vertex buffer, but link to shader to user later
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

var mouseLastX, mouseLastY;
var mouseDragging = false;
var angleX = 0, angleY = 0;
var gl, canvas;
var mvpMatrix;
var modelMatrix;
var normalMatrix;
var nVertex;
var cameraX = 3, cameraY = 3, cameraZ = 7;
var lightX = 0, lightY = 5, lightZ = 3;
var cube = [];
var train = [];
var mustang = [];
var pyramid = [];
var cylinderVertices = [];
var cylinder = [];
var coordX = 0, coordY = 0;
var recCoordX, recCoordY;
var diffCoordX = 0, diffCoordY = 0;
var rotateAngle;
var mustangScale = 0.15;
var trainScale = 0.4;
var wholeScale = 1.0;

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

    /*triangle: dot1:(0.5,1.0,0.5), dot2:(0.0,0.0,0.0), dot3:(1.0,0.0,0.0), dot4:(1.0,0.0,1.0), dot5:(0.0,0.0,1.0)*/
    /*18 dots*/
    pyramidVertices = [
        0.5,1.0,0.5, 1.0,0.0,0.0, 1.0,0.0,1.0,//side 1:134
        0.5,1.0,0.5, 0.0,0.0,1.0, 1.0,0.0,1.0,//side 2:154
        0.5,1.0,0.5, 0.0,0.0,0.0, 0.0,0.0,1.0,//side 3:125
        0.5,1.0,0.5, 0.0,0.0,0.0, 1.0,0.0,0.0,//side 4:123
        1.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,1.0, 1.0,0.0,0.0, 0.0,0.0,1.0, 1.0,0.0,1.0//bottom:325-354
    ];
    pyramidNormals = getNormalOnVertices(pyramidVertices);
    let p = initVertexBufferForLaterUse(gl, pyramidVertices, pyramidNormals, null);
    pyramid.push(p);

    /*cylinder: 2 dots for center and 102 dots for side*/
    /*line 232-255: define the element of cylinder*/
    let radius=1.0;

    //front center(1st dot)
    cylinderElements = [0.0,0.0,0.0];

    //2nd to 52nd dot(front side)
    for(var i=0; i<=50; i++){
        var degree = i * (2 * Math.PI / 50);
        var x = radius * Math.cos(degree);
        var y = radius * Math.sin(degree);
        cylinderElements.push(x,y,0.0);
    }

    //back center(53rd dot)
    cylinderElements.push(0.0,0.0,1.0);

    //54th to 104th dot(back side)
    for(var i=0; i<=50; i++){
        var degree = i * (2 * Math.PI / 50);
        var x = radius * Math.cos(degree);
        var y = radius * Math.sin(degree);
        cylinderElements.push(x,y,1.0);
    }

    /*line 257-282: draw cylinder with 200 triangles*/
    //front
    for(var i=1; i<=50; i++){
        cylinderVertices.push(0.0,0.0,0.0); //center
        cylinderVertices.push(cylinderElements[i*3],cylinderElements[i*3+1],cylinderElements[i*3+2]); //dot 1
        cylinderVertices.push(cylinderElements[i*3+3],cylinderElements[i*3+4],cylinderElements[i*3+5]); //dot 2
    }

    //back
    for(var i=53; i<=102; i++){
        cylinderVertices.push(0.0,0.0,1.0); //center
        cylinderVertices.push(cylinderElements[i*3],cylinderElements[i*3+1],cylinderElements[i*3+2]); //dot 1
        cylinderVertices.push(cylinderElements[i*3+3],cylinderElements[i*3+4],cylinderElements[i*3+5]); //dot 2
    }

    //side
    for(var i=1; i<=50; i++){
        //first triangle
        cylinderVertices.push(cylinderElements[i*3],cylinderElements[i*3+1],cylinderElements[i*3+2]); //dot 1
        cylinderVertices.push(cylinderElements[i*3+3],cylinderElements[i*3+4],cylinderElements[i*3+5]); //dot 2
        cylinderVertices.push(cylinderElements[i*3+153],cylinderElements[i*3+154],cylinderElements[i*3+155]); //dot 3
        //second triangle
        cylinderVertices.push(cylinderElements[i*3+3],cylinderElements[i*3+4],cylinderElements[i*3+5]); //dot 1
        cylinderVertices.push(cylinderElements[i*3+153],cylinderElements[i*3+154],cylinderElements[i*3+155]); //dot 2
        cylinderVertices.push(cylinderElements[i*3+156],cylinderElements[i*3+157],cylinderElements[i*3+158]); //dot 3
    }

    cylinderNormals = getNormalOnVertices(cylinderVertices);
    let q = initVertexBufferForLaterUse(gl, cylinderVertices, cylinderNormals, null);
    cylinder.push(q);

    //train 3D model
    response = await fetch('steam-train.obj');
    text = await response.text();
    obj = parseOBJ(text);
    for( let i=0; i < obj.geometries.length; i ++ ){
        let o = initVertexBufferForLaterUse(gl, obj.geometries[i].data.position, obj.geometries[i].data.normal, obj.geometries[i].data.texcoord);
        train.push(o);
    }

    //mustang 3D model
    response = await fetch('1967-shelby-ford-mustang.obj');
    text = await response.text();
    obj = parseOBJ(text);
    for( let i=0; i < obj.geometries.length; i ++ ){
        let o = initVertexBufferForLaterUse(gl, obj.geometries[i].data.position, obj.geometries[i].data.normal, obj.geometries[i].data.texcoord);
        mustang.push(o);
    }

    mvpMatrix = new Matrix4();
    modelMatrix = new Matrix4();
    normalMatrix = new Matrix4();

    gl.enable(gl.DEPTH_TEST);
    draw(); //draw it once before mouse move

    canvas.onmousedown = function(ev){mouseDown(ev)};
    canvas.onmousemove = function(ev){mouseMove(ev)};
    canvas.onmouseup = function(ev){mouseUp(ev)};
    canvas.onmousewheel = function(ev){
        if(ev.deltaY <= -1 && wholeScale<=3.0){
            wholeScale += 0.1;
            console.log('Current Scale: ',wholeScale);
        }
        else if(ev.deltaY >= 1 && wholeScale>0.2){
            wholeScale -= 0.1;
            console.log('Current Scale: ',wholeScale);
        }
        draw();
    };

    var slider1 = document.getElementById("xCoordinate");
    slider1.oninput = function(){
        recCoordX = coordX;
        coordX = this.value/50.0;
        diffCoordX += Math.abs(coordX-recCoordX)*50;
        draw();
    }

    var slider2 = document.getElementById("yCoordinate");
    slider2.oninput = function(){
        recCoordY = coordY;
        coordY = this.value/50.0;
        diffCoordY += Math.abs(coordY-recCoordY)*50;
        draw();
    }
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
    gl.clearColor(0,0,0,1);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    let mdlMatrix = new Matrix4();
    //big flat cuboid
    mdlMatrix.setIdentity();
    mdlMatrix.translate(0.0, -0.5*wholeScale, 0.0);
    mdlMatrix.scale(4.0*wholeScale, 0.1*wholeScale, 4.0*wholeScale);
    drawOneObject(cube, mdlMatrix, 0.4, 0.4, 0.4); //gray

    //train
    mdlMatrix.setIdentity();
    mdlMatrix.translate(-0.5*wholeScale, -0.55*wholeScale, -0.5*wholeScale);
    mdlMatrix.scale(trainScale*wholeScale, trainScale*wholeScale, trainScale*wholeScale);
    drawOneObject(train, mdlMatrix, 0.0, 0.6, 0.3); //green

    //ford mustang
    mdlMatrix.setIdentity();
    mdlMatrix.translate(-1.5*wholeScale, -0.4*wholeScale, 0.2*wholeScale);
    mdlMatrix.scale(mustangScale*wholeScale, mustangScale*wholeScale, mustangScale*wholeScale);
    drawOneObject(mustang, mdlMatrix, 1.0, 0.1, 0.1); //red

    //multiple joint object
    //calculation for rotate angle of cuboids
    rotateAngle = diffCoordX + diffCoordY;
    while( rotateAngle>90 ){
        rotateAngle -= 90;
    }
    if( rotateAngle>=45 && rotateAngle<=90 ){
        rotateAngle = 90 - rotateAngle;
    }

    mdlMatrix.setIdentity();
    mdlMatrix.translate(coordX*wholeScale, -0.4*wholeScale, coordY*wholeScale);
    pushMatrix();
    mdlMatrix.scale(0.2*wholeScale, 0.2*wholeScale, 0.2*wholeScale);
    drawOneObject(pyramid, mdlMatrix, 1.0, 0.5, 0.0); //orange
    mdlMatrix.scale(5.0/wholeScale, 5.0/wholeScale, 5.0/wholeScale);
    popMatrix();

    mdlMatrix.translate(0.1*wholeScale, 0.3*wholeScale, 0.0);
    pushMatrix();
    mdlMatrix.scale(0.1*wholeScale, 0.1*wholeScale, 0.2*wholeScale);
    drawOneObject(cylinder, mdlMatrix, 1.0, 0.5, 0.0); //orange
    mdlMatrix.scale(10.0/wholeScale, 10.0/wholeScale, 5.0/wholeScale);
    popMatrix();

    mdlMatrix.translate(0.0, 0.1*wholeScale, 0.1*wholeScale);
    mdlMatrix.rotate(rotateAngle, 0.0, 0.0, 1.0);
    mdlMatrix.translate(0.0, 0.2*wholeScale, 0.0);
    pushMatrix();
    mdlMatrix.scale(0.1*wholeScale, 0.2*wholeScale, 0.1*wholeScale);
    drawOneObject(cube, mdlMatrix, 1.0, 0.5, 0.0); //orange
    mdlMatrix.scale(10.0/wholeScale, 5.0/wholeScale, 10.0/wholeScale);
    popMatrix();

    mdlMatrix.translate(0.0, 0.3*wholeScale, -0.1*wholeScale);
    pushMatrix();
    mdlMatrix.scale(0.1*wholeScale, 0.1*wholeScale, 0.2*wholeScale);
    drawOneObject(cylinder, mdlMatrix, 1.0, 0.5, 0.0); //orange
    mdlMatrix.scale(10.0/wholeScale, 10.0/wholeScale, 5.0/wholeScale);
    popMatrix();

    mdlMatrix.translate(0.0*wholeScale, 0.1*wholeScale, 0.1*wholeScale);
    mdlMatrix.rotate(rotateAngle, 0.0, 0.0, 1.0);
    mdlMatrix.translate(0.0, 0.2*wholeScale, 0.0);
    pushMatrix();
    mdlMatrix.scale(0.1*wholeScale, 0.2*wholeScale, 0.1*wholeScale);
    drawOneObject(cube, mdlMatrix, 1.0, 0.5, 0.0); //orange
    mdlMatrix.scale(10.0/wholeScale, 5.0/wholeScale, 10.0/wholeScale);
    popMatrix();
}

function drawOneObject(obj, mdlMatrix, colorR, colorG, colorB){
    //model Matrix (part of the mvp matrix)
    modelMatrix.setRotate(angleY, 1, 0, 0);//for mouse rotation
    modelMatrix.rotate(angleX, 0, 1, 0);//for mouse rotation
    modelMatrix.multiply(mdlMatrix);
    //mvp: projection * view * model matrix  
    mvpMatrix.setPerspective(30, 1, 1, 100);
    mvpMatrix.lookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
    mvpMatrix.multiply(modelMatrix);

    //normal matrix
    normalMatrix.setInverseOf(modelMatrix);
    normalMatrix.transpose();

    gl.uniform3f(program.u_LightPosition, lightX, lightY, lightZ);
    gl.uniform3f(program.u_ViewPosition, cameraX, cameraY, cameraZ);
    gl.uniform1f(program.u_Ka, 0.2);
    gl.uniform1f(program.u_Kd, 0.7);
    gl.uniform1f(program.u_Ks, 1.0);
    gl.uniform1f(program.u_shininess, 10.0/wholeScale);
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
    
    function newGeometry(){
        // If there is an existing geometry and it's
        // not empty then start a new one.
        if (geometry && geometry.data.position.length) {
            geometry = undefined;
        }
    }
    
    function setGeometry(){
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
        geometry.data = Object.fromEntries(Object.entries(geometry.data).filter(([, array]) => array.length > 0));
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