PImage backgroundImg,imgP1,imgP2,imgP3,imgP4,imgP5;
int XSpeed,YSpeed;
float pX,pY;
int i;

int tileSize = 80;
int xPoint = 0;
int yPoint = 0;
int landX = 0;
int landY;

int radius = 0;
float x = 0;
float speed = 3;
int direction = 1;

void setup(){

  size(700,840);
  frameRate(60);
  surface.setTitle("plz enter the name of surface title"); //enter the game name here
  surface.setLocation(100,100);                            //enter x,y to set the suface place
  backgroundImg = loadImage("star_sky_tiles1.png");   //find the background.png(.jpg)to replace the background
  
  imgP1=loadImage("Spaceship1.png");                               //find the P1-5.png(.jpg) to replace 
  imgP2=loadImage("Spaceship1.png");
  imgP3=loadImage("Spaceship2.png");
  imgP4=loadImage("Spaceship2.png");
  imgP5=loadImage("Spaceship2.png");
  
  pX=width/2;
  pY=height/2;
}

void draw(){
  landY = height-tileSize;
  fillBG();
  for (int n = 0; n < 3; n++){
    for (int m = 0; m < 23; m++){
      landX += tileSize;
    }
    landX = 0;
    landY -= tileSize;
  }
  landY = 0;
  
  x += speed * direction;
  if ((x > width-120) || (x < radius)){
    direction = -direction;
  }
  
  drawPlayer(); 
}

void bgPattern(int x, int y){
  image(backgroundImg, x, y);
}

void fillBG(){
  for (int j = 0; j < 18; j++){
      for (int i = 0; i < 23; i++){
        bgPattern(xPoint, yPoint);
        println("xPoint = "+xPoint+", yPoint = "+yPoint);
        println("i = "+i+", j = "+j);
        xPoint += tileSize;
      }
      xPoint = 0;
      yPoint += tileSize;
  }
  yPoint = 0;
}

void drawPlayer(){
  //background(255);
  i++;
  
  if(keyPressed==false){
    XSpeed=0;
    YSpeed=0;
  }
  
  pX+=XSpeed;
  pY+=YSpeed;
  if(i%5==0){
    image(imgP1,pX,pY,width/5,height/5);
  }
  else if(i%5==1){
    image(imgP2,pX,pY,width/5,height/5);
  }
  else if(i%5==2){
    image(imgP3,pX,pY,width/5,height/5);
  }
  else if(i%5==3){
    image(imgP4,pX,pY,width/5,height/5);
  }
  else if(i%5==4){
    image(imgP5,pX,pY,width/5,height/5);
  }
}

void keyPressed(){
  if(keyPressed){
    if(keyCode==39||key=='d'||key=='D'){
      if(pX<width-137){
        XSpeed=8;
      }
      else{
        XSpeed=0;
      }
    }
    if(keyCode==37||key=='a'||key=='A'){
      if(pX>0){
        XSpeed=-8;
      }
      else{
        XSpeed=0;
      }
    }
    if(keyCode==38||key=='w'||key=='W'){
      if(pY>0){
        YSpeed=-8;
      }
      else{
        YSpeed=0;
      }
    }
    if(keyCode==40||key=='s'||key=='S'){
      if(pY<height-189){
        YSpeed=8;
      }
      else{
        YSpeed=0;
      }
    }
  }
  else{
    XSpeed=0;
    YSpeed=0;
  }
  
}
/*
class player{
  
  
}
*/

/*
void backgroundPlace(int x,int y){
  image(backgroundImg,x,y);
}
*/
