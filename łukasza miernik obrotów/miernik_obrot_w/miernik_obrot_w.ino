#include <LiquidCrystal.h>
LiquidCrystal lcd(4,5,9,10,11,12);
const int hallPin=2;
const unsigned long sampleTime=1000;
const int maxRPM = 10200; 
void setup() 
{
  pinMode(hallPin,INPUT);
  Serial.begin(9600); 
  lcd.begin(16, 2);
  lcd.print("OBROTOMIERZ");
  delay(1000);
  lcd.clear();
}

void loop() 
{
  delay(100);
  int rpm=getRPM();
  lcd.clear();
  displayRPM(rpm);
  displayBar(rpm);
}
 
int getRPM()
{
  // sample for sampleTime in millisecs
  int kount=0;
  boolean kflag=LOW;
  unsigned long currentTime=0;
  unsigned long startTime=millis();
  while (currentTime<=sampleTime)
  {
    if (digitalRead(hallPin)==HIGH)
    {
      kflag=HIGH;
    }
    if (digitalRead(hallPin)==LOW && kflag==HIGH)
    {
      kount++;
      kflag=LOW;
    }
    currentTime=millis()-startTime;
  }
  int kount2rpm = int(60000./float(sampleTime))*kount;
  return kount2rpm;
}
    
void displayRPM(int rpm) 
{
  lcd.clear();
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 0); 
  // print the number of seconds since reset:
  lcd.print(rpm,DEC);
  lcd.setCursor(7,0);
  lcd.print("RPM");
}

void displayBar(int rpm)
{
  int numOfBars=map(rpm,0,maxRPM,0,15);
  lcd.setCursor(0,1);
  if (rpm!=0)
  {
  for (int i=0; i<=numOfBars; i++)
   {
        lcd.setCursor(i,1);
        lcd.write(1023);
      }
  }
} 
