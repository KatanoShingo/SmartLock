#include <ESP32Servo.h>
#include <nRF24L01.h>

int request;
Servo penguin;
#include <RF24.h>
#define motor 13 
#define switching 5 
#define button 0
RTC_DATA_ATTR int keyStatus;

RF24 radio(4, 22);               // CE,CSNピンの指定
const byte address[6] = "00001";  // データを受信するアドレス
void open(void)
{
  digitalWrite( switching, HIGH );
  for (int i = 0; i <= 120; i++) 
  {
    penguin.write(130-i);
    delay(10);
  } 
  digitalWrite( switching, LOW );
  keyStatus=0;
}

void lock()
{
  digitalWrite( switching, HIGH );
    
  for (int i = 0; i <= 120; i++) 
  {
    penguin.write(10+i);
    delay(10);
  } 
  digitalWrite( switching, LOW );
  keyStatus=1;
}

// 初期化
void setup()
{
  // シリアルポートの初期化
  Serial.begin(115200);

  pinMode(button, INPUT);
  pinMode(switching, OUTPUT);
  penguin.attach(motor);
  
  radio.begin();                    // 無線オブジェクトの初期化
  radio.openReadingPipe(0, address);// データ受信アドレスを指定
  radio.setPALevel(RF24_PA_MIN);    // 出力を最小に
  radio.startListening();           // 受信側として設定
 
  delay(10);
  if(radio.available())
  {
    radio.read(&request, sizeof(request));
    if(request==0)
    {
      if(keyStatus==0)
      {
        lock();
      }
      else
      {
        open();
      }
    }
  }          
  
  esp_sleep_enable_timer_wakeup(2 * 1000 * 1000);
  esp_deep_sleep_start();
}

void loop()
{             
}
