#include <ESP32Servo.h>
Servo penguin;
// WiFi情報
const char* ssid = "xxxxxx";
const char* pass = "xxxxxx";

#define motor 13 
#define switching 5 

void unlock(void)
{
    digitalWrite( switching, HIGH );
    
    for (int i = 0; i <= 120; i++) 
    {
      penguin.write(130-i);
      delay(10);
    } 
    delay(1000);
    digitalWrite( switching, LOW );
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
}

// 初期化
void setup()
{
    pinMode(switching, OUTPUT);
    penguin.attach(motor);
  
    // シリアルポートの初期化
    Serial.begin(115200);
}

// 処理ループ
void loop()
{
  delay(100);
}
