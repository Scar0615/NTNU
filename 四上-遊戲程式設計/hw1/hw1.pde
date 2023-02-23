int len = 50;
int fontSize = 32;

void setup(){
  size(1100, 550);
  background(64, 0, 64);
  PFont myFont = createFont("標楷體",fontSize);
  textFont(myFont);
}

void drawRect(int startX,int startY,int l,int h,int r,int g,int b){
  noStroke();
  fill(r,g,b);
  rect(startX*len, startY*len, l*len, h*len);
}

void drawEllipse(float startX,float startY,float d,int r,int g,int b){
  noStroke();
  fill(r,g,b);
  circle(startX, startY, d);
  fill(64, 0, 64);
  ellipse(startX + d*0.5, startY, d, d);
}

void draw(){
  //HW1_向量繪圖造型練習, 40847016S, 謝尚恆
  fill(255);
  textSize(fontSize);
  text("HW1_向量繪圖造型練習, 40847016S, 謝尚恆", 250, 100); 
  
  //先畫四棟建築物B->A->C->D
  drawRect(5, 4, 5, 7, 64, 64, 64);
  drawRect(2, 5, 5, 6, 0, 0, 0);
  drawRect(10, 3, 7, 8, 0, 0, 0);
  drawRect(17, 6, 3, 5, 96, 96, 96);
  
  //畫窗戶
  for(int i=6 ; i<=8 ; i+=2){
    drawRect(3, i, 1, 1, 255, 225, 150);
  }
  for(int i=4 ; i<=8 ; i+=2){
    for(int j=11 ; j<=13 ; j+=2){
      drawRect(j, i, 1, 1, 255, 225, 150);
    }
  }
  
  //畫月亮
  drawEllipse(75, 75, 100, 255, 225, 150);
  
  mouseDragged();
}

//加分：跟著滑鼠畫出星星
void mouseDragged(){
  fill(255);
  delay(50);
  circle(mouseX, mouseY, 10);
}
