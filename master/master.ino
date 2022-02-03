#include <nRF24L01.h>
#include <RF24.h>
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <WiFi.h>
#include <time.h>
#define button 0                        
int request;
RTC_DATA_ATTR int keyStatus;

RF24 radio(4, 22);               // CE,CSNピンの指定
const byte address[6] = "00001";  // データを受信するアドレス

char auth[] = "YourAuthToken";

const char* ssid       = "YOUR_SSID";
const char* password   = "YOUR_PASS";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600*9;
const int   daylightOffset_sec = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup()
{
  // シリアルポートの初期化
  Serial.begin(115200);

  //WiFiに接続する
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

  //時間を初期化
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //不要になったWiFiを切り離す
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  pinMode(button, INPUT);

  radio.begin();                  // 無線オブジェクトの初期化
  radio.openWritingPipe(address); // データ送信先のアドレスを指定
  radio.setPALevel(RF24_PA_MIN);  // 出力を最小に
  radio.stopListening();          // 送信側として設定
  
  Blynk.setDeviceName("Blynk");

  Blynk.begin(auth);
  Blynk.run();
}

void loop()
{      
  Blynk.run();
  if(digitalRead(button)==0)
  {
    radio.write(&request, sizeof(request)); 
  }
}
