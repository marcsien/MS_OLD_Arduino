#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"          // biblioteka DHT
#include <SD.h>
#include <SPI.h>
#include <LiquidCrystal.h>

#define DHTPIN 6 // numer pinu sygnałowego
#define DHTTYPE DHT11     // typ czujnika (DHT11). Jesli posiadamy DHT22 wybieramy DHT22

const int chipSelect = 10;

LiquidCrystal lcd(8, 7, 5, 4, 3, 2);
dht.setup(DHTPIN);

void setup()
{
  
  lcd.print("Inicjalizacja karty SD...");
  delay(1000);
  lcd.clear();

  dht.begin();
  lcd.begin(16, 2);

  if (!SD.begin(chipSelect)) {
    lcd.print("Brak lub uszkodzona karta SD");
    return;
  }
  lcd.print("Ukończono.");
}

void loop()
{
  // Odczyt temperatury i wilgotności powietrza
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
}
