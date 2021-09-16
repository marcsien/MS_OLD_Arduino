#include "DHT.h"          // biblioteka DHT
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Servo.h>

#define DHTPIN 6          // numer pinu sygnałowego
#define DHTTYPE DHT11     // typ czujnika (DHT11). Jesli posiadamy DHT22 wybieramy DHT22

DHT dht(DHTPIN, DHTTYPE); // definicja czujnika

LiquidCrystal lcd(8, 7, 5, 4, 3, 2);

Servo serwomechanizm;

int pozycja = 0;
float t;
float h;

void setup()
{

  
  dht.begin();            // inicjalizacja czujnika
  lcd.begin(16, 2);       //inicjalizacja wyswietlacza
  lcd.print("DHT11...");
  delay(3000);
  lcd.clear();
  serwomechanizm.attach(9);

}

void loop()
{

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Sprawdzamy czy są odczytane wartości
  if (isnan(t) || isnan(h))
  {
    // Jeśli nie, wyświetlamy informację o błędzie
    lcd.setCursor(0, 0);
    lcd.print("BLAD !!");
  } else
  {
    // Jeśli tak, wyświetlamy wyniki pomiaru

    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.setCursor(7, 0);
    lcd.print(t, 2);

    lcd.setCursor(0, 1);
    lcd.print("Wilg:");
    lcd.setCursor(7, 1);
    lcd.print(h, 2);
  }

  pozycja=h/100*90;
  
  serwomechanizm.write(pozycja);
  delay(100);
 
  
}
