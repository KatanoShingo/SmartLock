b#include <nRF24L01.h>
#include <RF24.h>
#define button 0                        
int request;
RTC_DATA_ATTR int keyStatus;

RF24 radio(4, 22);               // CE,CSNピンの指定
const byte address[6] = "00001";  // データを受信するアドレス

void setup()
{
  // シリアルポートの初期化
  Serial.begin(115200);

  pinMode(button, INPUT);

  radio.begin();                  // 無線オブジェクトの初期化
  radio.openWritingPipe(address); // データ送信先のアドレスを指定
  radio.setPALevel(RF24_PA_MIN);  // 出力を最小に
  radio.stopListening();          // 送信側として設定
  
}

void loop()
{      
  if(digitalRead(button)==0)
  {
    radio.write(&request, sizeof(request)); 
  }
}
