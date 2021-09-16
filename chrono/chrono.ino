int a;
int b;
int w;
int z;
float x;
float y;
float czas;
float v;
float m;
float e;
float f;

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("CHRONOGRAF");
  lcd.setCursor(0, 1);
  lcd.print("Marcinex");
  delay(2000);
  lcd.clear();
  czas=0;
  m=0.55;
}

void loop() {
  a =digitalRead(7);
  b =digitalRead(8);
  w =digitalRead(9);
  z =digitalRead(10);
  
  
  if (a==0){
    x=millis()*0.001;
  }
  if (b==0){
    y=millis()*0.001;
  }

  if (w==0){
    m=m-0.01;
    delay(200);
  }
  if (z==0){
    m=m+0.01;
    delay(200);
  }
  

  czas=y-x;
  v=0.0595/czas;
  e=0.0005*m*v*v;
  f=v*3.28084;
  
  lcd.setCursor(0, 0);
  lcd.print("f=");
  lcd.setCursor(2, 0);
  lcd.print(f,2);

  lcd.setCursor(0, 1);
  lcd.print("v=");
  lcd.setCursor(2, 1);
  lcd.print(v,1);

  lcd.setCursor(9, 0);
  lcd.print("m=");
  lcd.setCursor(11, 0);
  lcd.print(m,2);

  lcd.setCursor(9, 1);
  lcd.print("E=");
  lcd.setCursor(11, 1);
  lcd.print(e,2);


}





