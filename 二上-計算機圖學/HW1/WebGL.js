var VSHADER_SOURCE = `
    attribute vec4 a_Position;
    attribute vec4 a_Color;
    varying vec4 v_Color;
    void main(){
        gl_Position = a_Position;
        gl_PointSize = 6.0;
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

var shapeFlag = 'p'; //p: point, h: hori line: v: verti line, t: triangle, q: square, c: circle
var colorFlag = 'r'; //r g b

var g_points = [];
var g_horiLines = [];
var g_vertiLines = [];
var g_triangles = [];
var g_squares = [];

var g_pColors = [];
var g_hColors = [];
var g_vColors = [];
var g_tColors = [];
var g_qColors = [];

var pointCount = 0;
var hlCount = 0;
var vlCount = 0;
var triangleCount = 0;
var squareCount = 0;

function compileShader(gl, vShaderText, fShaderText){
    //Build vertex and fragment shader objects
    var vertexShader = gl.createShader(gl.VERTEX_SHADER)
    var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER)
    //Set up shader text source
    gl.shaderSource(vertexShader, vShaderText)
    gl.shaderSource(fragmentShader, fShaderText)
    //Compile vertex shader
    gl.compileShader(vertexShader)
    if(!gl.getShaderParameter(vertexShader, gl.COMPILE_STATUS)){
        console.log('vertex shader error');
        var message = gl.getShaderInfoLog(vertexShader); 
        console.log(message);//print shader compiling error message
    }
    //Compile fragment shader
    gl.compileShader(fragmentShader)
    if(!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)){
        console.log('fragment shader error');
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

function main(){
    //Get the canvas context
    var canvas = document.getElementById('webgl');
    var gl = canvas.getContext('webgl2');
    if(!gl){
        console.log('Failed to get the rendering context for WebGL');
        return ;
    }

    // compile shader and use program
    let renderProgram = compileShader(gl, VSHADER_SOURCE, FSHADER_SOURCE);
    gl.useProgram(renderProgram);

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    // mouse and key event
    canvas.onmousedown = function(ev){click(ev, gl, canvas, shapeFlag, colorFlag, renderProgram)};
    document.onkeydown = function(ev){keydown(ev)};
}

function keydown(ev){
    //implment keydown event
    if(ev.key == 'p'){
        shapeFlag = 'p';
        console.log('Current pattern: point');
    }
    if(ev.key == 'h'){
        shapeFlag = 'h';
        console.log('Current pattern: horizontal line');
    }
    if(ev.key == 'v'){
        shapeFlag = 'v';
        console.log('Current pattern: vertical line');
    }
    if(ev.key == 't'){
        shapeFlag = 't';
        console.log('Current pattern: triangle');
    }
    if(ev.key == 'q'){
        shapeFlag = 'q';
        console.log('Current pattern: square');
    }
    if(ev.key == 'r'){
        colorFlag = 'r';
        console.log('Current color: red');
    }
    if(ev.key == 'g'){
        colorFlag = 'g';
        console.log('Current color: green');
    }
    if(ev.key == 'b'){
        colorFlag = 'b';
        console.log('Current color: blue');
    }
}

function pushColor(flag, array){
    if(flag == 'r'){
        array.push(1.0);
    }
    else{
        array.push(0.0);
    }
    if(flag == 'g'){
        array.push(1.0);
    }
    else{
        array.push(0.0);
    }
    if(flag == 'b'){
        array.push(1.0);
    }
    else{
        array.push(0.0);
    }
}

function startLoc(count){
    if(count<5){
        return 0;
    }
    else{
        return count-5;
    }
}

function click(ev, gl, canvas, shapeFlag, colorFlag, program){
    //mouse click: recall our quiz1 in class
    var x = ev.clientX;
    var y = ev.clientY;
    var rect = ev.target.getBoundingClientRect();

    x = ((x - rect.left) - canvas.height/2)/(canvas.height/2)
    y = (canvas.width/2 - (y - rect.top))/(canvas.height/2)
    //you might want to do something here
    switch(shapeFlag){
        case 'p':
            pointCount++;
            g_points.push(x);
            g_points.push(y);
            pushColor(colorFlag, g_pColors);
            break;
        case 'h':
            hlCount++;
            g_horiLines.push(-1.0);
            g_horiLines.push(y);
            g_horiLines.push(1.0);
            g_horiLines.push(y);
            for(var i=0 ; i<2 ; i++){
                pushColor(colorFlag, g_hColors);
            }
            break;
        case 'v':
            vlCount++;
            g_vertiLines.push(x);
            g_vertiLines.push(-1.0);
            g_vertiLines.push(x);
            g_vertiLines.push(1.0);
            for(var i=0 ; i<2 ; i++){
                pushColor(colorFlag, g_vColors);
            }
            break;
        case 't':
            triangleCount++;
            g_triangles.push(x);
            g_triangles.push(y+0.04);
            g_triangles.push(x-0.04);
            g_triangles.push(y-0.04);
            g_triangles.push(x+0.04);
            g_triangles.push(y-0.04);
            for(var i=0 ; i<3 ; i++){
                pushColor(colorFlag, g_tColors);
            }
            break;
        case 'q':
            squareCount++;
            g_squares.push(x-0.04);
            g_squares.push(y+0.04);
            g_squares.push(x+0.04);
            g_squares.push(y+0.04);
            g_squares.push(x-0.04);
            g_squares.push(y-0.04);
            g_squares.push(x+0.04);
            g_squares.push(y-0.04);
            for(var i=0 ; i<4 ; i++){
                pushColor(colorFlag, g_qColors);
            }
            break;
    }
    draw(gl, program);
}

function vertexBuffer(gl, array, num, type, attribute){
    //create buffer
    var buffer = gl.createBuffer();
    if(!buffer){
        console.log("failed to create the buffer object");
    }
    //bind buffer
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.bufferData(gl.ARRAY_BUFFER, array, gl.STATIC_DRAW);

    var a_attribute = gl.getAttribLocation(gl.getParameter(gl.CURRENT_PROGRAM), attribute);

    gl.vertexAttribPointer(a_attribute, num, type, false, 0, 0);
    gl.enableVertexAttribArray(a_attribute);

    return true;
}

function draw(gl, program){
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.useProgram(program);

    pPositionBuffer = vertexBuffer(gl, new Float32Array(g_points), 2, gl.FLOAT, 'a_Position');
    pColorBuffer = vertexBuffer(gl, new Float32Array(g_pColors), 3, gl.FLOAT, 'a_Color');
    gl.drawArrays(gl.POINTS, startLoc(pointCount), pointCount);

    hPositionBuffer = vertexBuffer(gl, new Float32Array(g_horiLines), 2, gl.FLOAT, 'a_Position');
    hColorBuffer = vertexBuffer(gl, new Float32Array(g_hColors), 3, gl.FLOAT, 'a_Color');
    gl.drawArrays(gl.LINES, startLoc(hlCount)*2, hlCount*2);

    vPositionBuffer = vertexBuffer(gl, new Float32Array(g_vertiLines), 2, gl.FLOAT, 'a_Position');
    vColorBuffer = vertexBuffer(gl, new Float32Array(g_vColors), 3, gl.FLOAT, 'a_Color');
    gl.drawArrays(gl.LINES, startLoc(vlCount)*2, vlCount*2);

    tPositionBuffer = vertexBuffer(gl, new Float32Array(g_triangles), 2, gl.FLOAT, 'a_Position');
    tColorBuffer = vertexBuffer(gl, new Float32Array(g_tColors), 3, gl.FLOAT, 'a_Color');
    gl.drawArrays(gl.TRIANGLES, startLoc(triangleCount)*3, triangleCount*3);

    sPositionBuffer = vertexBuffer(gl, new Float32Array(g_squares), 2, gl.FLOAT, 'a_Position');
    sColorBuffer = vertexBuffer(gl, new Float32Array(g_qColors), 3, gl.FLOAT, 'a_Color');
    // gl.drawArrays(gl.TRIANGLE_STRIP, startLoc(squareCount)*4, squareCount*4);
    // gl.drawArrays(gl.QUADS, startLoc(squareCount)*4, squareCount*4);
    for(var i=startLoc(squareCount) ; i<squareCount; i++){
        gl.drawArrays(gl.TRIANGLE_STRIP, i*4, 4);
    }
}