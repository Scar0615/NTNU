//edit那欄的key才是真的key，所以修改成：
//edit -> formResponse
//第一個&entry -> ?entry
//https://docs.google.com/forms/d/e/1FAIpQLScM0MfKMe4y813DDYA4gNBuLBfICsvbIEkD07xhTmALiivk6Q/formResponse
//?entry.1084925456=name
//&entry.1851474555=level

//試算表
//https://docs.google.com/spreadsheets/d/189KqERKgXiKSIY_T9GMj66-Gs2k_HlIFE521IAMvW6I/edit?usp=sharing

//要改成：
//https://docs.google.com/spreadsheets/d/189KqERKgXiKSIY_T9GMj66-Gs2k_HlIFE521IAMvW6I/export?format=csv
//貼上這個網址後會直接下載成excel檔
import http.requests.*;

String CallURL;
String mUrl = "https://docs.google.com/spreadsheets/d/189KqERKgXiKSIY_T9GMj66-Gs2k_HlIFE521IAMvW6I/export?format=csv";
String[] playerName = {"Adam", "Bob", "Carter", "Dennis", "Ed"};
String[] playerLevel = {"95", "57", "71", "13", "39"};
String last;

PFont TCfont;
PostRequest runQuery;

void setup(){
  size(600, 400);
  TCfont = createFont("Arial",22);
  textFont(TCfont);
  frameRate(1);
}

void draw(){
  background(255);
  int randomIndex = int(random(0, 4));
  String name = playerName[randomIndex];
  String level = playerLevel[randomIndex];
  CallURL = "https://docs.google.com/forms/d/e/1FAIpQLScM0MfKMe4y813DDYA4gNBuLBfICsvbIEkD07xhTmALiivk6Q/formResponse";
  
  CallURL = CallURL + "?entry.1084925456=" + name;
  CallURL = CallURL + "&entry.1851474555=" + level;
  CallURL = CallURL.replace(" ", "%20"); //將空白轉換成%20 URLEncode
  
  runQuery = new PostRequest(CallURL);
  runQuery.send();
  
  textSize(22);
  fill(#DB8700);
  text("目前的資料是：", 140, 100);
  text("玩家名稱 = "+name, 140, 130);
  text("玩家等級 = "+level, 140, 160);
  
  String[] lines = loadStrings(mUrl);
  for (int i = 0; i < lines.length; i++){
      last = lines[i];
      println(i + ". " + lines[i]);
  }
  text("最後寫入的資料是：", 140, 260);
  text(last, 140, 290);
}
