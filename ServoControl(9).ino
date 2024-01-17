#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include "dht.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFPlayer_Mini_Mp3.h>

#define OLED_RESET 4
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(OLED_RESET);
SoftwareSerial mySerial(12, 13);
dht DHT;  

#define   Water_PUMP          2
#define   Fan_Pin             3
#define   DHT11_PIN           7 
#define   IR_PIN              8  //红外
#define   LED_Pin             10
#define   Voice_Pin           11
#define   Light_Pin           A0 
#define   Rain_Pin            A1 
#define   Water_Pin           A2 
#define   Soil_Pin            A3

int Light;
int Rain;
int Water_level;
int Soil_Moisture;
float Humi;
float Temp;
int state;
volatile byte i=0;

#define   Light_Threshold                1000    //光敏阈值，控制灯
#define   Rain_Threshold                 100    //水滴阈值
#define   Water_level_Threshold          50    //水位阈值  控制扬声器
#define   Soil_Moisture_Threshold        100    //土壤湿度阈值，控制水泵
#define   Temp_Threshold                 20    //温度阈值，控制风扇
#define   Humi_Threshold                 50    //湿度阈值，控制风扇
#define   VOL                            20     // 声音音量

void setup()
{
  IO_Init();
}

void loop()
{
  Data_Read();
  Date_Display();
  Blue_Model();
}

void IO_Init()
{
  pinMode(Fan_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(Water_PUMP, OUTPUT);
  digitalWrite(Fan_Pin, HIGH);
  digitalWrite(LED_Pin, HIGH);
  digitalWrite(Water_PUMP, LOW);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(WHITE);
  display.setCursor(0,16); 
  display.println("Welcome to Smart Farm "); 
  display.display();
  mySerial.begin (9600);
  mp3_set_serial (mySerial);
  delay(1);
  mp3_set_volume (VOL);
  mp3_play ();
  delay(3000);
  display.clearDisplay();
  display.setCursor(98,0); 
  display.println("BLUE"); 
  display.display();
}

void Data_Read()
{
  DHT.read11(DHT11_PIN);
  Light = analogRead(Light_Pin);
  Rain  = analogRead(Rain_Pin);
  Water_level = analogRead(Water_Pin);
  Soil_Moisture = analogRead(Soil_Pin);
  Temp  = DHT.temperature,1;
  Humi  = DHT.humidity,1;
}

void Date_Display()
{
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.println("Model:");
  display.setCursor(98,0); 
  display.println("BLUE");
  
  display.setCursor(0,8); 
  display.println("Light:");
  display.setCursor(36,8); 
  display.println(Light);
  
  display.setCursor(68,8); 
  display.println("Rain:");
  display.setCursor(98,8); 
  display.println(Rain);
  
  display.setCursor(0,16); 
  display.println("Water:");
  display.setCursor(36,16); 
  display.println(Water_level);
  
  display.setCursor(68,16); 
  display.println("Soil:");
  display.setCursor(98,16); 
  display.println(Soil_Moisture);

  display.setCursor(0,24); 
  display.println("Temp:");
  display.setCursor(36,24); 
  display.println(Temp);
  
  display.setCursor(68,24); 
  display.println("Humi:");
  display.setCursor(98,24); 
  display.println(Humi);

  if(Light > Light_Threshold )
  {
    display.setCursor(0,40); 
    display.println("LED ON!"); 
  }else {
    display.setCursor(0,40); 
    display.println("LED OFF!");
   }
  if(Temp > Temp_Threshold || Humi > Humi_Threshold || Rain > Rain_Threshold)
  {
    display.setCursor(68,40); 
    display.println("Fan ON!");
  }else {
    display.setCursor(68,40); 
    display.println("Fan OFF!");
   }
  if(Rain > Rain_Threshold )
  {
    display.setCursor(0,48); 
    display.println("Raining!");
  }else {
    display.setCursor(0,48); 
    display.println("Not Rain!");
  }
  if(Soil_Moisture < Soil_Moisture_Threshold )
  {
    display.setCursor(68,48); 
    display.println("Watering!");
  }else {
    display.setCursor(68,48); 
    display.println("Not Water!");
   }

   if(Water_level < Water_level_Threshold || digitalRead(IR_PIN) == 0)
  {
    display.setCursor(0,56); 
    display.println("BEEP!");
  }else
  {
    display.setCursor(0,56); 
    display.println("Not BEEP!");
  }
  display.display(); 
}


void Auto_Model()
{
  if(Light > Light_Threshold )
  {
    digitalWrite(LED_Pin, HIGH);  
  }else
  {
    digitalWrite(LED_Pin, LOW); 
  }
  if(Temp > Temp_Threshold || Humi > Humi_Threshold || Rain > Rain_Threshold)
  {
    digitalWrite(Fan_Pin, HIGH);
  }else
  {
    digitalWrite(Fan_Pin, LOW);
  }
  if(Soil_Moisture < Soil_Moisture_Threshold )
  {
    digitalWrite(Water_PUMP, HIGH);
  }else
  {
    digitalWrite(Water_PUMP, LOW); 
  }  
}

void MP3_Player()
{
  if(Light > Light_Threshold )
  {
    if(i==0)
    {
      mp3_play (2);
      delay(1500);
      i++;    
    }
  }else i=0;
  if(Temp > Temp_Threshold || Humi > Humi_Threshold || Rain > Rain_Threshold)
  {
    mp3_play (5);
    delay(1500);
  }
  if(Soil_Moisture < Soil_Moisture_Threshold )
  {
    mp3_play (6);
    delay(1500);  
  }
  if(Water_level < Water_level_Threshold)
  {
    mp3_play (7);
    delay(1500); 
  }
  if(Rain > Rain_Threshold )
  { 
    mp3_play (3);
    delay(1500);      
  }
  if(digitalRead(IR_PIN) == 0)
  {
    mp3_play (4);
    delay(1500); 
  }
}

void Blue_Model()
{
//  if(Serial.available()){ 
//    state = Serial.read();
//    Serial.println(Water_level);
//    Serial.println("this is working");
//
//  }
    if (mySerial.available()) {
      char receivedChar = mySerial.read(); // 读取字符
      Serial.print("Received: ");
      Serial.println(receivedChar);
      switch(receivedChar){
          case  '0':  digitalWrite(LED_Pin, HIGH);delay(1500);    break;
          case  '1':  digitalWrite(LED_Pin, LOW);delay(1500);   break;
          case  '2':  digitalWrite(Water_PUMP, HIGH);delay(1500);                break;
          case  '3':  digitalWrite(Water_PUMP, LOW);delay(1500);               break;
          case  '4':  digitalWrite(Fan_Pin, HIGH);delay(1500);                   break;
          case  '5':  digitalWrite(Fan_Pin, LOW);delay(1500);                  break;
          case  '6':  mp3_play (15);delay(1500);                  
                  while(1)
                  {
                    MP3_Player();
                    Data_Read();
                    Date_Display();
                    Auto_Model();
                    if(Serial.available() > 0) 
                    state = Serial.read();
                    if(receivedChar =='7')
                    {
                      mp3_play (14);delay(1500);
                      digitalWrite(Fan_Pin, LOW);
                      digitalWrite(LED_Pin, LOW);
                      digitalWrite(Water_PUMP, LOW);
                      break;
                    } 
                  }               
               
                  break;
    }
    }
}
