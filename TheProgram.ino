#include <LiquidCrystal.h>
#include <DS3231.h>
LiquidCrystal lcd(12,11,5,4,3,2);
DS3231  rtc(SDA, SCL);

//Define Pins
int MoisturePin=A3;
int RelayPin=10;

//Set up screen loops
long PreviousMillis = 0;
long LCDInterval=4000;
int Screen=0;
int LastScreen=1;
bool ScreenChanged=true;
#define MOISTURE 0
#define TIME 1

//Set up moisture calculations
float MoistureValue=0;
float PercentMoisture=100;
long MoistureInterval=5000;
long PreviousMoistureMillis=0;

//Misc
char* LastWateredTime;
char* LastWateredDate;

void MoistureScreen(float P) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Current moisture");
  lcd.setCursor(0,1);
  lcd.print(P);
  lcd.print("%");
}

void LastTime() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Last watered");
  lcd.setCursor(0,1);
  lcd.print(LastWateredDate);
  lcd.print(" ");
  lcd.print(LastWateredTime);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  rtc.begin();
  rtc.setDate(4,6,2016);
  rtc.setTime(13,50,00);
  pinMode(RelayPin,OUTPUT);
  digitalWrite(RelayPin,LOW);
}
void loop() {
  unsigned long CurrentMillis=millis();
  if(CurrentMillis-PreviousMoistureMillis>MoistureInterval){
   PreviousMoistureMillis=CurrentMillis;
   MoistureValue=analogRead(MoisturePin);
   PercentMoisture=((MoistureValue/600L)*100L);
   Serial.println(MoistureValue);
   if (PercentMoisture<30){
    LastWateredTime=rtc.getTimeStr(1);
    LastWateredDate=rtc.getDateStr(1,3);
    digitalWrite(RelayPin,HIGH);
    delay(5000);
    digitalWrite(RelayPin,LOW);
   }
   if (PercentMoisture>70){
    digitalWrite(RelayPin,LOW);
   }
  }
  if(CurrentMillis-PreviousMillis>LCDInterval){
    PreviousMillis=CurrentMillis;
    Screen++;
    if (Screen>LastScreen){
      Screen=0;
    }
    ScreenChanged=true;
  }
  if (ScreenChanged){
    ScreenChanged=false;
    switch(Screen) {
      case MOISTURE:
        MoistureScreen(PercentMoisture);
        break;
      case TIME:
        LastTime();
        break;
    }
  }
}
