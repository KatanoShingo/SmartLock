#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
WebServer server(80);
Servo penguin;
// WiFi情報
const char* ssid = "xxxxxx";
const char* pass = "xxxxxx";

#define motor 13 
#define switching 5 
double UnlockClosedDoorTime = 0;
unsigned long preTime;
double timeCountLock = 0;
String html;
double frameTime;
// 初期画面
void handleRoot(void)
{
    // HTMLを出力する
    server.send(200, "text/html", html);
}

void unlock(void)
{
    // 変数msgの文字列を送信する
    server.send(200, "text/html", html);

    digitalWrite( switching, HIGH );
    
    for (int i = 0; i <= 120; i++) 
    {
      penguin.write(130-i);
      delay(10);
    } 
    delay(1000);
    digitalWrite( switching, LOW );
    UnlockClosedDoorTime += frameTime;
}

void lock()
{
    delay(1000);
    digitalWrite( switching, HIGH );
    
    for (int i = 0; i <= 120; i++) 
    {
      penguin.write(10+i);
      delay(10);
    } 
    delay(1000);
    digitalWrite( switching, LOW );
    UnlockClosedDoorTime = 0;
}

// 存在しないアドレスが指定された時の処理
void handleNotFound(void)
{
    server.send(404, "text/plain", "Not Found.");
}

// 初期化
void setup()
{
    pinMode(switching, OUTPUT);
    penguin.attach(motor);
  
    // HTMLを組み立てる
    html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset=\"utf-8\">";
    html += "</head>";
    html += "<body>";
    html += "<a href=\"/unlook\">";
    html += "<div style=\"margin-left:10%;margin-right:10%;\"> ";
    html += "<input type=\"button\" style=\"width:100%;padding:100px;font-size:100px;\" value=\"ロック解除\" />";
    html += "</div>";
    html += "</body>";
  
    // シリアルポートの初期化
    Serial.begin(115200);

    // WiFiのアクセスポイントに接続
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    // ESP32のIPアドレスを出力
    Serial.println("WiFi Connected.");
    Serial.print("IP = ");
    Serial.println(WiFi.localIP());


    // 処理するアドレスを定義
    server.on("/", handleRoot);
    server.on("/unlook", unlock);
    server.onNotFound(handleNotFound);
    // Webサーバーを起動
    server.begin();
}

// 処理ループ
void loop()
{
  server.handleClient();
  
  unsigned long nowTime = millis();
  frameTime  = (nowTime-preTime)/1000.0;

  if(UnlockClosedDoorTime>0)
  {
    UnlockClosedDoorTime += frameTime;
  }
  
  if(UnlockClosedDoorTime > 10)
  {
    Serial.println("閉じる");
    lock();
  }
  
  preTime = nowTime;

  delay(100);
}
