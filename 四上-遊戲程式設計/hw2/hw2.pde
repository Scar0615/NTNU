int fontSize = 32, i = 0, loc = 500, size = 300, angle = 6;
PImage sun, moon;

void setup(){
  size(1000, 600);
  background(0, 192, 255);
  PFont myFont = createFont("標楷體",fontSize);
  textFont(myFont);
  
  // 請置入至少兩張 2D 彩色圖片(圖片需去背)
  sun = loadImage("sunny.png");
  moon = loadImage("moon.png");
  
  frameRate(30);
}

void windmill(int loc, int size){
  //風車根部
  noStroke();
  fill(200);
  rect(loc - size/20, height - size, size/10, size);
  
  //風車關節
  noStroke();
  fill(200, 255, 0);
  circle(loc ,height - size, size/10);
}

void draw(){
  // 讓風車的扇葉自動旋轉
  if (i <= 360){
    background(0, 192, 255);
    pushMatrix();
    translate(loc, height - size);
    fill(#08C69F);
    rotate(radians(i));
    rect(size*(-0.5), -25, size, 50);
    rect(-25, size*(-0.5), 50, size);
    
    println("i = "+i+", ang ="+radians(i));
    popMatrix();
    i += angle;
  }
  else{
    i = 0;
  }
  
  // 畫面上須打上作業名稱及學號與姓名
  fill(255);
  textSize(fontSize);
  textAlign(CENTER, CENTER);
  text("2D 點陣彩圖置入及換圖", width/2, height/12);
  text("by 40847016S 謝尚恆", width/2, height/6);
  
  // 2D 彩色圖片須可跟隨著滑鼠移動，並點滑鼠右鍵可以換為另外一張圖片
  imageMode(CENTER);
  if(mousePressed){
    image(moon,mouseX,mouseY,100,100);
  }
  else{
    image(sun,mouseX,mouseY,100,100);
  }
  
  // 以向量繪圖指令製作一個具有造型的圖案物件
  windmill(loc, size);
}

// 太陽的圖源：https://www.flaticon.com/free-icons/sun
// 月亮的圖源：https://www.flaticon.com/free-icons/moon
