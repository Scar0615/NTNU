PImage backgroundImg,imgwin,imglose,imgstart,imgboom,imgP1,imgP2,imgP3,imgP4,imgP5,imgE1,imgE2,imgPB1,imgEB1,img311,img312,img313,img321,img322,img323,img331,img332;
int XSpeed=0,YSpeed=0;
int time;
int num=10;

int enemyHealth=1000;
int score=0;
int enemyType=0;
int damage=20;                                 //你可以藉由調整子彈傷害加速遊戲測試

float e1X,e1Y,e2X,e2Y,e3X,e3Y;
int E1health=100,E2health=100,E3health=100;

float pX,pY;
float[] pbX=new float [20];
float[] pbY=new float [20];
float[] pbmove=new float [20];
int playerHealth=3;

float[] e1bX=new float [100];
float[] e1bY=new float [100];
float[] e1btype=new float [100];
float eX=0,eY=0;

int phase=0;

int tileSize = 80;
int xPoint = 0;
int yPoint = 0;
int landX = 0;
int landY;

int radius = 0;
float x = 0;
float speed = 3;
int direction = 1;

String s = "Score: ";

void setup(){

  size(700,840);
  frameRate(120);
  surface.setTitle("alien"); //enter the game name here
  surface.setLocation(100,100);                            //enter x,y to set the suface place
  backgroundImg = loadImage("star_sky_tiles1.png");   //find the background.png(.jpg)to replace the background
  
  imgP1=loadImage("Spaceship1.png");                               //find the P1-5.png(.jpg) to replace 
  imgP2=loadImage("Spaceship1.png");
  imgP3=loadImage("Spaceship2.png");
  imgP4=loadImage("Spaceship2.png");
  imgP5=loadImage("Spaceship2.png");
  
  imgPB1=loadImage("Bullet1_flip.png");//plz put the bullet img                     
  imgEB1=loadImage("Bullet2.png");                                   
  
  imgE1=loadImage("Boss1.png");
  imgE2=loadImage("Boss2.png");
  
  img311=loadImage("Monster1-1.png");
  img312=loadImage("Monster1-2.png");
  img313=loadImage("Monster1-3.png");
  img321=loadImage("Monster2-1.png");
  img322=loadImage("Monster2-2.png");
  img323=loadImage("Monster2-3.png");
  img331=loadImage("Monster3-1.png");
  img332=loadImage("Monster3-2.png");
  
  imgwin=loadImage("victory.png");
  imglose=loadImage("gameover.png");
  imgstart=loadImage("startpage.png");
  imgboom=loadImage("Explode.png");
  
  eX=250;
  eY=0;
  enemyType=0;
  enemyHealth=1000;
  
  e1X=80;e1Y=-100;E1health=100;
  e2X=280;e2Y=-100;E2health=100;
  e3X=480;e3Y=-100;E3health=100;  
  
  playerHealth=3;
  pX=width/2-37;
  pY=height/2+200;
  
  for(int i=0;i<100;i++){
    e1btype[i]=0;
  }
  
  textAlign(LEFT, BOTTOM);
  textSize(50);
}

float tmpX;
float tmpY;

void drawEnemy(){

  if(enemyType==0){
    if(time%420==0){
      tmpX=random(0,450);
      tmpY=random(0,110);
      tmpX=(tmpX-eX)/420;
      tmpY=(tmpY-eY)/420;
    }
    eX+=tmpX;
    eY+=tmpY;
    drawenemybullet(1);
  }else if(enemyType==1){
    if(time%300==0){
      tmpX=random(0,450);
      tmpY=random(0,110);
      tmpX=(tmpX-eX)/300;
      tmpY=(tmpY-eY)/300;
    }
    eX+=tmpX;
    eY+=tmpY;
    drawenemybullet(2);
  }else if(enemyType==2){
    if(eY>-300){
      eY-=10;
    }else{
      drawenemy(1);
      drawenemy(2);
      drawenemy(3);
    }
    if(eY<=-300){
      eY=-500;
      eX=-200;
    }
  }else if(enemyType==3){
    if(eY<=0){
      eX=250;
      eY+=10;
    }
    drawenemybullet(1);
    drawenemybullet(2);
  }
  
  if(enemyHealth>=800){
    enemyType=0;
  }else if(enemyHealth>=600){
    enemyType=1;
  }else if(enemyHealth>=400){
    enemyType=2;
  }else if(enemyHealth>=200){
    enemyType=3;
  }else if(enemyHealth<=0){
    phase=2;
  }
  
  if(time%40<20){
    //rect(eX,eY,230,270);
    image(imgE1,eX,eY,230,270);
  }else{
    //rect(eX,eY,230,270);
    image(imgE2,eX,eY,230,270);
  }
}

void drawenemy(int n){
  if(n==1){
    if(E1health>=0){
      if(e1Y<50){  
        e1X=80;
        e1Y+=10;
      }else{
        drawenemybullet(31);
      }
    }
  }else if(n==2){
    if(E2health>=0){
      if(e2Y<100){
        e2X=280;
        e2Y+=10;
      }else{
        drawenemybullet(32);
      }
    }
  }else if (n==3){
    if(E3health>=0){
      if(e2Y<50){
        e3X=480;
        e3Y+=10;
      }else{
        drawenemybullet(33);
      }
    }
  }
  
  if(n==1){
    if(E1health>=0){
      if(time%60<20){
        //rect(e1X+30,e1Y+20,80,120);
        image(img311,e1X,e1Y,100,150);
      }else if(time%60<40){
        //rect(e1X+30,e1Y+20,80,120);
        image(img312,e1X,e1Y,100,150);
      }else{
        //rect(e1X+30,e1Y+20,80,120);
        image(img313,e1X,e1Y,100,150);
      }
    }
  }else if(n==2){
    if(E2health>=0){
      if(time%60<20){
        //rect(e2X+30,e2Y+20,80,120);
        image(img321,e2X,e2Y,100,150);
      }else if(time%60<40){
        //rect(e2X+30,e2Y+20,80,120);
        image(img322,e2X,e2Y,100,150);
      }else {
        //rect(e2X+30,e2Y+20,80,120);
        image(img323,e2X,e2Y,100,150);
      }
    }
  }else if(n==3){
    if(E3health>=0){
      if(time%60<30){
        //rect(e3X+30,e3Y+20,80,120);
        image(img331,e3X,e3Y,100,150);
      }else{
        //rect(e3X+30,e3Y+20,80,120);
        image(img332,e3X,e3Y,100,150);
      }
    }
  }
  if(E1health<=0&&E2health<=0&&E3health<=0){
    enemyHealth=399;
    e1X=80;e1Y=-200;
    e2X=280;e2Y=-200;
    e3X=480;e3Y=-200;
  }
  
}

int count;
void drawenemybullet(int kind){
  if(kind==1){
    if(time%60==0){
      for(int i=0;i<100;i++){
        if(e1btype[i]==0){
          e1btype[i]=1;
          e1bX[i]=eX+random(0,200);
          e1bY[i]=eY+200;
          break;
        }
      }
    }
  }else if(kind==2){
    if(time%120==0){
      for (int i=0;i<96;i++){
        if(e1btype[i]==0){
          e1btype[i]=20;
          e1btype[i+1]=21;
          e1btype[i+2]=22;
          e1bX[i]=eX+100;
          e1bX[i+1]=eX+100;
          e1bX[i+2]=eX+100;
          e1bY[i]=eY+200;
          e1bY[i+1]=eY+200;
          e1bY[i+2]=eY+200;
          break;
        }
      }
    }
  }else if(kind>=31&&kind<34){
    if(time%60==0){
      for(int i=0;i<100;i++){
        if(e1btype[i]==0){
          e1btype[i]=kind;
          if(kind==31){
            e1bX[i]=e1X+60;
            e1bY[i]=e1Y+100;
            break;
          }else if(kind==32){
            e1bX[i]=e2X+60;
            e1bY[i]=e2Y+100;
            break;
          }else if(kind==33){
            e1bX[i]=e3X+60;
            e1bY[i]=e3Y+100;
            break;
          }
        }
      }
    }
  }
  
  
  for(int i=0;i<100;i++){
    if(e1btype[i]==0){
      e1bX[i]=0;
      e1bY[i]=-30;
    }else if(e1btype[i]==1){
      e1bY[i]+=10;
    }else if(e1btype[i]<24&&e1btype[i]>=20){
      if(e1bY[i]>pY){
        if(e1btype[i]==20){
          e1bY[i]+=5;
        }else if(e1btype[i]==21){
          e1bX[i]-=5;
        }else if(e1btype[i]==22){
          e1bX[i]+=5;
        }
      }else {
        e1bY[i]+=5;
      }
    }else if(e1btype[i]>=31&&e1btype[i]<34){
      e1bY[i]+=5;
    }
  }
  
  for(int i=0;i<100;i++){
    image(imgEB1,e1bX[i],e1bY[i],20,20);
    //rect(e1bX[i],e1bY[i],20,20);
  }
  
  for(int i=0;i<100;i++){
    if(e1bY[i]>height){
      e1btype[i]=0;
    }
  }
  
  for(int i=0;i<100;i++){
    if(e1btype[i]!=0&&((e1bX[i]<pX+70&&e1bX[i]>pX&&e1bY[i]>pY&&e1bY[i]<pY+90)||
    (e1bX[i]+20<pX+70&&e1bX[i]+20>pX&&e1bY[i]>pY&&e1bY[i]<pY+90)||
    (e1bX[i]<pX+70&&e1bX[i]>pX&&e1bY[i]+20>pY&&e1bY[i]+20<pY+90)||
    (e1bX[i]+20<pX+70&&e1bX[i]+20>pX&&e1bY[i]+20>pY&&e1bY[i]+20<pY+90))){
      playerHealth-=1;
      image(imgboom,e1bX[i]-40,e1bY[i]-40,100,100);
      //rect(e1bX[i],e1bY[i],40,40);
      e1btype[i]=0;
    }
  }
  
}

void draw(){
  if(phase==0){
    score = 0;
    image(imgstart,0,0,width,height);
    
    if(mousePressed){
      setup(); 
      phase=1;
     }
    /*
    if(){    //if press start  
      phase=1;
    }
    */
  }else if(phase==1){
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
    
    time++;
    time=time%14400;
    //getKey();
    if(keyPressed){
      pX+=XSpeed;
      pY+=YSpeed;
    }
    text(s + str(score), 50, height-50);
    
    drawPlayer(); 
    drawEnemy();
    drawplayerbullet();
    check();
  }else if(phase==2){
    image(imgwin,0,0,width,height);
    text(s + str(score), 50, height-50);
    if(keyPressed){
      if(key=='p'|key=='P'){
        phase=0;
      }
    }
  }else if(phase==3){
    image(imglose,0,0,width,height);
    text(s + str(score), 50, height-50);
    if(keyPressed){
      if(key=='p'|key=='P'){
        phase=0;
      }
    }
  }
}


void drawplayerbullet(){
  if(time%20==0){
    for(int i=0;i<20;i++){
      if(pbmove[i]==0){
        pbmove[i]=1;
        pbX[i]=pX+25;
        pbY[i]=pY;
        break;
      }
    }
  }
  for(int i=0;i<20;i++){
    if(pbmove[i]==1){
      if(pbY[i]<-50){
        pbmove[i]=0;
      }
    }
  }
  for(int i=0;i<20;i++){
    if(pbmove[i]==0){
      pbX[i]=0;
      pbY[i]=-30;
    }else if(pbmove[i]==1){
      pbX[i]+=0;
      pbY[i]-=30;
    }
  }
  for(int i=0;i<20;i++){
    if(pbmove[i]==1&&pbX[i]>eX&&pbX[i]+20<eX+230&&pbY[i]>eY&&pbY[i]<eY+200){
      image(imgboom,pbX[i]-40,pbY[i]-40,100,100);
      //rect(pbX[i]-10,pbY[i],40,40);
      pbmove[i]=0;
      enemyHealth-=damage;
      score += 20;
    }else if(pbX[i]>e1X+30&&pbY[i]>e1Y+20&&pbX[i]<e1X+30+80&&pbY[i]<e1Y+120){
      image(imgboom,pbX[i]-40,pbY[i]-40,100,100);
      //rect(pbX[i]-10,pbY[i],40,40);
      pbmove[i]=0;
      E1health-=damage;
      score += 10;
    }else if(pbX[i]>e2X+30&&pbY[i]>e2Y+20&&pbX[i]<e2X+30+80&&pbY[i]<e2Y+120){
      image(imgboom,pbX[i]-40,pbY[i]-40,100,100);
      //rect(pbX[i]-10,pbY[i],40,40);
      pbmove[i]=0;
      E2health-=damage;
      score += 10;
    }else if(pbX[i]>e3X+30&&pbY[i]>e3Y+20&&pbX[i]<e3X+30+80&&pbY[i]<e3Y+120){
      image(imgboom,pbX[i]-40,pbY[i]-40,100,100);
      //rect(pbX[i]-10,pbY[i],40,40);
      pbmove[i]=0;
      E3health-=damage;
      score += 10;
    }
  }
  
  for(int i=0;i<20;i++){
    image(imgPB1,pbX[i]-10,pbY[i],40,40);
    //rect(pbX[i],pbY[i],20,30);
  }
  
  if(playerHealth<=0){
    phase=3;
  }
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
  if(time%5==0){
    //rect(pX,pY,70,90);
    image(imgP1,pX,pY,70,90);
  }
  else if(time%5==1){
    //rect(pX,pY,70,90);
    image(imgP2,pX,pY,70,90);
  }
  else if(time%5==2){
    //rect(pX,pY,70,90);
    image(imgP3,pX,pY,70,90);
  }
  else if(time%5==3){
    //rect(pX,pY,70,90);
    image(imgP4,pX,pY,70,90);
  }
  else if(time%5==4){
    //rect(pX,pY,70,90);
    image(imgP5,pX,pY,70,90);
  }
}

void check(){
  if(pX>=width-70){
    pX=width-70;
  }
  if(pX<=0){
    pX=0;
  }
  if(pY<=0){
    pY=0;
  }
  if(pY>height-100){
    pY=height-100;
  }
}

void keyPressed(){
  XSpeed=0;
  YSpeed=0;

  if(keyCode==39||key=='d'||key=='D'){
    if(pX>=width-70){
      pX=width-70;
    }else {
      XSpeed=8;
    }
  }
  if(keyCode==37||key=='a'||key=='A'){
    if(pX<=0){
      pX=0;
    }else {
      XSpeed=-8;
    }
  }
  if(keyCode==38||key=='w'||key=='W'){
    if(pY<=0){
      pY=0;
    }else {
      YSpeed=-8;
    }
  }
  if(keyCode==40||key=='s'||key=='S'){
    if(pY>height-100){
      pY=height-100;
    }else {
      YSpeed=8;
    }
  }
}
