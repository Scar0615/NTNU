int block = 10; //設定陣列元素數量
int size = 80, speed = 4, playerSize = size / 2;
int[] x1 = new int[block]; //左上x點
int[] y1 = new int[block]; //左上y點
int[] x2 = new int[block]; //右下x點
int[] y2 = new int[block]; //右下y點
color[] blockColor = new color[block];
color playerColor;

void setup(){
  size(880,880);
  
  //給予物件初始值
  for (int i = 0; i < block; i++){
    if(i < 5){
      x1[i] = 0;
      y1[i] = size * (1+2*i);
    }
    else{
      x1[i] = width - size * (2*i-8);
      y1[i] = 0;
    }
    x2[i] = x1[i]+size;
    y2[i] = y1[i]+size;
    blockColor[i] = color(0, 128, random(0, 255));
  }
  
  playerColor = #00C000;
}

void draw(){
  background(128); //刷新背景
  
  //繪製障礙物初始位置
  noStroke();
  for (int i = 0; i < block; i++){
    fill(blockColor[i]);
    square(x1[i], y1[i], size);
  }
  
  //障礙物移動
  for(int i = 0; i < block; i++){
    if(i < 5){
      x1[i] = (x1[i] + speed*(i+1)) % width;
    }
    else{
      y1[i] = (y1[i] + speed*(i-4)) % height;
    }
  }
  
  //玩家位置
  fill(playerColor);
  rect(mouseX, mouseY, playerSize, playerSize);
  
  collision();
  
  //復活點
  strokeWeight(2);
  stroke(#FFFF00);
  fill(#C0C000);
  rect(size*5, size*5, size, size);
  
  revive();
}

//碰到障礙物
void collision(){
  for (int i = 0; i < block; i++){
    if(((mouseX > x1[i] && mouseY > y1[i]) || (mouseX+size > x1[i] && mouseY+size > y1[i])) && ((mouseX < x2[i] && mouseY < y2[i]) || (mouseX+size < x2[i] && mouseY+size < y2[i]))){
      playerColor = #C00000;
    }
  }
}

//碰到復活點
void revive(){
  if (((mouseX > size*5 && mouseY > size*5) || (mouseX+playerSize > size*5 && mouseY+playerSize > size*5)) && ((mouseX < size*6 && mouseY < size*6) || (mouseX+playerSize < size*6 && mouseY+playerSize < size*6))){
    playerColor = #00C000;
  }
}
