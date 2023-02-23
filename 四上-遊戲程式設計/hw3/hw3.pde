import ddf.minim.*;
Minim minim;//宣告
AudioPlayer in;//宣告
PImage rick, pause, play, mute, unmute, stop, restart;
int size = 50;

void setup(){
  size(800, 600);
  minim = new Minim(this);
  in = minim.loadFile("rickroll.mp3");
  in.play();
  //載入背景
  rick = loadImage("rick.jpg");
  
  //載入按鈕圖案
  pause = loadImage("pause.png");
  play = loadImage("play.png");
  mute = loadImage("mute.png");
  unmute = loadImage("unmute.png");
  stop = loadImage("stop.png");
  restart = loadImage("reload.png");
  
  PFont myFont = createFont("標楷體",32);
  textFont(myFont);
}

void draw(){
  //放置背景
  imageMode(CENTER);
  image(rick,width/2, height/2,width, height);
  
  //音效線
  stroke(255, 0, 0);
  for(int i=0; i < in.left.size()-1; i++){
    line(i, height/2 + in.left.get(i)*height/2, i+1, height/2 +in.left.get(i+1)*height/2);
  }
  
  //需打學號及姓名
  fill(255);
  textAlign(CENTER, CENTER);
  text("by 40847016S 謝尚恆", width/2, height/6);
  
  //控制面板須至少有音樂/影片的播放、暫停、靜音、停止、重新播放等功能
  stroke(0, 0, 255);
  square(225, 450, size); //播放、暫停
  square(325, 450, size); //靜音、解除靜音
  square(425, 450, size); //停止
  square(525, 450, size); //重新播放
  
  //按鈕狀態偵測
  imageMode(CORNER);
  ////播放、暫停
  if (in.isPlaying()){
    image(pause, 225, 450, size, size);
  }
  else{
    image(play, 225, 450, size, size);
  }
  
  ////靜音、解除靜音
  if (in.isMuted()){
    image(mute, 325, 450, size, size);
  }
  else{
    image(unmute, 325, 450, size, size);
  }
  
  ////停止撥放
  image(stop, 425, 450, size, size);
  
  ////再次撥放
  image(restart, 525, 450, size, size);
}

void mousePressed(){
  //播放、暫停
  if (mouseX > 225 && mouseX < 225 + size && mouseY > 450 && mouseY < 450 + size) {
    if (in.isPlaying()){
      in.pause();
    }
    else{
      in.loop();
    }
  }
  
  //靜音、解除靜音
  if (mouseX > 325 && mouseX < 325 + size && mouseY > 450 && mouseY < 450 + size) {
    if (in.isMuted()){
      in.unmute();
    }
    else{
      in.mute();
    }
  }
  
  //停止撥放
  if (mouseX > 425 && mouseX < 425 + size && mouseY > 450 && mouseY < 450 + size) {
    minim.stop();
  }
  
  //再次撥放
  if (mouseX > 525 && mouseX < 525 + size && mouseY > 450 && mouseY < 450 + size) {
    minim.stop();
    in = minim.loadFile("rickroll.mp3");
    in.play();
  }
}

void stop(){
  in.close();//音樂停止
  minim.stop();
  super.stop();
}

//背景圖圖源: https://www.radiopop.cl/06-de-febrero-de-1966-nace-rick-astley/
//按鈕圖源: https://www.flaticon.com/
