#include<EEPROM.h> // библиотека для работы с памятью EEPROM
#include<Servo.h> // библиотека для работы с сервомотором
#define patternLenth 5
#define patternInputTime 10000 // время за которое нужно сделать стуки
#define sensitivity 80 // чувствительность
#define margin 100
#define sw 7
#define servoPin 3 // к этому контакту подключен сервомотор
#define openGate 0
#define closeGate 180
long slot[patternLenth+1];
int pattern[patternLenth];
int flag=0;
int acceptFlag=0;
int knok;
Servo myServo;
void setup() 
{
  pinMode(sw, INPUT_PULLUP);
  myServo.attach(servoPin);
  myServo.write(180);
  Serial.begin(9600);
}
void loop() 
{
   int i=0;
   if(digitalRead(sw) == LOW)
   {   
      Serial.println("Start");
      delay(1000);
      long stt= millis();
      while(millis()<(stt+patternInputTime))
      {
        int temp=analogRead(A0);
        if(temp>sensitivity && flag==0 && i<=patternLenth)
        {
             delay(10);
             flag=1;
             slot[i++]=millis()-stt;
             //Serial.println(slot[i-1] - stt);
             if(i>patternLenth)
             break;
        }
        else if(temp == 0)
        flag=0;
      }
      
      long stp=millis();
      Serial.println("Stop");
     // Serial.println(stp-stt);
      for(int i=0;i<patternLenth;i++)
      {
         knok=1;
         if(slot[i+1]-slot[i] <500 )
            pattern[i]=0;
         else
            pattern[i]=1;
         Serial.println(pattern[i]);
      }
      if(digitalRead(sw) == 0)
      {
         for(int i=0;i<patternLenth;i++)
            EEPROM.write(i,pattern[i]);
         while(digitalRead(sw) == 0);
      }
      else
      {
         if(knok == 1)
         {
            for(int i=0;i<patternLenth;i++)
            {
               if(pattern[i] == EEPROM.read(i))
               {
                  Serial.println(acceptFlag++);
               }
               else
               {
                    Serial.println("Break");
                    break;                
               }
            }
         }
         Serial.println(acceptFlag);
         if(acceptFlag >=  patternLenth-1)
         {
             Serial.println(" Accepted");
             myServo.write(openGate);
             delay(5000);
             myServo.write(closeGate);
         }
         else
            Serial.println("Rejected");
      }
      for(int i=0;i<patternLenth;i++)
      {
         pattern[i]=0;
         slot[i]=0;
      }
      slot[i]=0;
      acceptFlag=0;
   }
}
