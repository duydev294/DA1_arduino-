// Include thu vien 

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;
const int echo = 8;
const int trig = 9;
LiquidCrystal lcd(7, 6, 2, 3, 4, 5);

void save_data(String data ){
  File dataFile  = SD.open("datalog.txt",FILE_WRITE);
  if(dataFile){
    dataFile.println(data);
    dataFile.close();
    }
   
  }
  String print2digits(int number) {
  String buff;
  if (number >= 0 && number < 10) {
    buff += "0";
  }
  buff += String(number);
  return buff;
}
long long get_data(){
  unsigned long duration;
  long long distance;
   digitalWrite(trig,0); // Đặt chân Trig về mức thấp (0V)
   delayMicroseconds(2);
   digitalWrite(trig,1);// Đặt chân Trig về mức cao (5V)
   delayMicroseconds(5);// trễ 5 us để tạo xung rộng 5us
   digitalWrite(trig,0);// kéo chân Trig xuống mức thấp (0V)
   duration = pulseIn(echo,HIGH); 
   distance = (duration/58.824);// Hàm tính khoảng cách với thời gian đo được 
    return distance;
  }
void sendwifi(int data){
    Serial.print(String(data));
    Serial.print("\r");
  }
void send_get_data(){
  tmElements_t tm;
  String buff;
  String Time;
  String Time_LCD;
  long long distance = get_data();
  sendwifi(int(distance));
  
  if(RTC.read(tm)){
    Time = print2digits(tm.Hour)+":";
    Time += print2digits(tm.Minute)+":";
    Time += print2digits(tm.Second);
    Time_LCD = Time + "-" + String(tm.Day)+"/" + String(tm.Month)+"/" + String(tmYearToCalendar(tm.Year));
    Time += ", Date(D/M/Y) = "+String(tm.Day)+"/" + String(tm.Month)+"/" + String(tmYearToCalendar(tm.Year));
    buff = "Value: "+ String(int(distance) *10)+"mm"+ '\t' + Time;
    save_data(buff);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Value: "+ String(int(distance)*10)+"mm");
    lcd.setCursor(0,1);
    lcd.print(Time_LCD);
    }
    else{
       lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Value: "+ String(int(distance)*10)+"mm");
      lcd.setCursor(0,1);
      lcd.print("Time error!");
      }
    
  
  }


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("SANSLAB");
  SD.begin(chipSelect);
  pinMode(trig,OUTPUT);
  pinMode(echo, INPUT);
}


    
    
  
void loop() {
  // put your main code here, to run repeatedly:
  send_get_data();
  delay(2000);
}
