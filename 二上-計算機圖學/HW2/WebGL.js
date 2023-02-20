var VSHADER_SOURCE = `
        attribute vec4 a_Position;
        attribute vec4 a_Color;
        varying vec4 v_Color;
        uniform mat4 u_modelMatrix;
        void main(){
            gl_Position = u_modelMatrix * a_Position;
            v_Color = a_Color;
        }    
    `;

var FSHADER_SOURCE = `
        precision mediump float;
        varying vec4 v_Color;
        void main(){
            gl_FragColor = v_Color;
        }
    `;

function createProgram(gl, vertexShader, fragmentShader){
    //create the program and attach the shaders
    var program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    
    //if success, return the program. if not, log the program info, and delete it.
    if(gl.getProgramParameter(program, gl.LINK_STATUS)){
        return program;
    }
    alert(gl.getProgramInfoLog(program) + "");
    gl.deleteProgram(program);
}

function compileShader(gl, vShaderText, fShaderText){
    //Build vertex and fragment shader objects
    var vertexShader = gl.createShader(gl.VERTEX_SHADER)
    var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER)
    //The way to set up shader text source
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

function initArrayBuffer( gl, data, num, type, attribute){
    var buffer = gl.createBuffer();
    if(!buffer){
        console.log("failed to create the buffere object");
    }

    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.bufferData(gl.ARRAY_BUFFER, data, gl.STATIC_DRAW);

    var a_attribute = gl.getAttribLocation(gl.getParameter(gl.CURRENT_PROGRAM), attribute);

    gl.vertexAttribPointer(a_attribute, num, type, false, 0, 0);
    gl.enableVertexAttribArray(a_attribute);

    return true;
}

var transformMat = new Matrix4();
var matStack = [];
var u_modelMatrix;
function pushMatrix(){
    matStack.push(new Matrix4(transformMat));
}
function popMatrix(){
    transformMat = matStack.pop();
}
//variables for tx, red,green and yellow arms angle 
var tx = 0;
var ty = 0;
var size = 0;
var firstJoint = 0;
var secondJoint = 0;

function main(){
    //////Get the canvas context
    var canvas = document.getElementById('webgl');
    var gl = canvas.getContext('webgl2');
    if(!gl){
        console.log('Failed to get the rendering context for WebGL');
        return ;
    }

    program = compileShader(gl, VSHADER_SOURCE, FSHADER_SOURCE);
    redraw(gl); //call redarw here to show the initial image

    //setup the call back function of tx Sliders
    var txSlider = document.getElementById("Robot-X");
    txSlider.oninput = function() {
        tx = this.value / 100.0; //convert sliders value to -1 to +1
        redraw(gl);
    }

    //setup the call back function of ty Sliders
    var tySlider = document.getElementById("Robot-Y");
    tySlider.oninput = function() {
        ty = this.value / 100.0; //convert sliders value to -1 to +1
        redraw(gl);
    }

    //setup the call back function of the first joint Sliders
    var firstJointSlider = document.getElementById("firstJoint");
    firstJointSlider.oninput = function() {
        firstJoint = this.value;
        redraw(gl);
    }

    //setup the call back function of the second joint Sliders
    var secondJointSlider = document.getElementById("secondJoint");
    secondJointSlider.oninput = function() {
        secondJoint = this.value;
        redraw(gl);
    }

    //setup the call back function of the third joint Sliders
    var thirdJointSlider = document.getElementById("thirdJoint");
    thirdJointSlider.oninput = function() {
        thirdJoint = this.value; //convert sliders value to -90 to 90 degrees
        redraw(gl);
    }

    //setup the call back function of the size of whole robot
    var robotSizeSlider = document.getElementById("robotSize");
    robotSizeSlider.oninput = function() {
        size = this.value / 25.0; //the range is 1-100 and 100 means 4x, 25 means 1x
        redraw(gl);
    }
}

//Call this funtion when we have to update the screen (eg. user input happens)
function redraw(gl)
{
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.useProgram(program);
    u_modelMatrix = gl.getUniformLocation(gl.getParameter(gl.CURRENT_PROGRAM), 'u_modelMatrix');
    
    triangleVertices = [-0.2, 0.2, -0.2, -0.2, 0.2, 0.0];
    rectVertices = [-0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5];
    var redTriangle = [1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0];
    var blueColor = [0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0 ];

    //draw a circle
    var circleVertices = [0.0, 0.0];
    var circleColor = [1.0, 1.0, 0.0]; //yellow
    var radius = 0.05*(size+1);
    for(var i=0; i<=50; i++){
        var degree = i * (2 * Math.PI / 50);
        var x = radius * Math.cos(degree);
        var y = radius * Math.sin(degree);
        circleVertices.push(x,y);
        circleColor.push(1.0, 1.0, 0.0);
    }

    //the element of claw
    var clawVertices = [
        0.05, 0.05, 0.05, -0.05, 0.2, -0.1, 0.2, -0.25, 
        0.1, -0.3, 0.15, -0.25, 0.15, -0.15, 0.0, -0.1, 
        -0.15, -0.15, -0.15, -0.25, -0.1, -0.3, -0.2, -0.25, 
        -0.2, -0.1, -0.05, -0.05, -0.05, 0.05, 0.05, 0.05
    ];
    
    var clawColor = [0.0, 1.0, 0.0]; //green
    for(var i=0; i<=17; i++){
        clawColor.push(0.0, 1.0, 0.0);
    }
    
    buffer1 = initArrayBuffer(gl, new Float32Array(triangleVertices), 2, gl.FLOAT, 'a_Position');
    buffer2 = initArrayBuffer(gl, new Float32Array(redTriangle), 3, gl.FLOAT, 'a_Color');

    transformMat.setIdentity();
    //translate whole robot
    transformMat.translate( (-0.4 + tx)*(size+1), (0.4 + ty)*(size+1), 0.0);
    pushMatrix();
    transformMat.scale(0.5*(size+1), 0.5*(size+1), 0.0);
    gl.uniformMatrix4fv(u_modelMatrix, false, transformMat.elements);
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, triangleVertices.length/2);//draw the blue one
    popMatrix();

    //first rectangle
    buffer1 = initArrayBuffer(gl, new Float32Array(rectVertices), 2, gl.FLOAT, 'a_Position');
    buffer2 = initArrayBuffer(gl, new Float32Array(blueColor), 3, gl.FLOAT, 'a_Color');
    transformMat.translate(0.1*(size+1), 0.0, 0.0);
    transformMat.rotate(firstJoint, 0.0, 0.0, 1.0);
    transformMat.translate(0.25*(size+1), 0.0, 0.0);
    pushMatrix();
    transformMat.scale(0.5*(size+1), 0.1*(size+1), 0.0);
    gl.uniformMatrix4fv(u_modelMatrix, false, transformMat.elements);
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, rectVertices.length/2);
    popMatrix();

    //first circle
    buffer1 = initArrayBuffer(gl, new Float32Array(circleVertices), 2, gl.FLOAT, 'a_Position');
    buffer2 = initArrayBuffer(gl, new Float32Array(circleColor), 3, gl.FLOAT, 'a_Color');
    transformMat.translate(0.3*(size+1), 0.0, 0.0);
    pushMatrix();
    gl.uniformMatrix4fv(u_modelMatrix, false, transformMat.elements);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, circleVertices.length/2);
    popMatrix();

    //second rectangle
    buffer1 = initArrayBuffer(gl, new Float32Array(rectVertices), 2, gl.FLOAT, 'a_Position');
    buffer2 = initArrayBuffer(gl, new Float32Array(blueColor), 3, gl.FLOAT, 'a_Color');
    transformMat.rotate(secondJoint, 0.0, 0.0, 1.0);
    transformMat.translate(0.0, -0.3*(size+1), 0.0);
    pushMatrix();
    transformMat.scale(0.1*(size+1), 0.5*(size+1), 0.0);
    gl.uniformMatrix4fv(u_modelMatrix, false, transformMat.elements);
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, rectVertices.length/2);
    popMatrix();

    //the claw
    buffer1 = initArrayBuffer(gl, new Float32Array(clawVertices), 2, gl.FLOAT, 'a_Position');
    buffer2 = initArrayBuffer(gl, new Float32Array(clawColor), 3, gl.FLOAT, 'a_Color');
    transformMat.translate(0.0, -0.2*(size+1), 0.0);
    transformMat.rotate(thirdJoint, 0.0, 0.0, 1.0);
    pushMatrix();
    transformMat.scale(size+1.0, size+1.0, 0.0);
    gl.uniformMatrix4fv(u_modelMatrix, false, transformMat.elements);
    gl.drawArrays(gl.LINE_LOOP, 0, clawVertices.length/2);
    popMatrix();
}