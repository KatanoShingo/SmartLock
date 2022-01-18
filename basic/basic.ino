#include <ESP32Servo.h>
Servo penguin;

#define motor 13 
#define switching 5 
#define button 0
int keyStatus;
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
    pinMode(button, INPUT);
    pinMode(switching, OUTPUT);
    penguin.attach(motor);
  
    // シリアルポートの初期化
    Serial.begin(115200);
}

// 処理ループ
void loop()
{
  if(digitalRead(button)==0)
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
