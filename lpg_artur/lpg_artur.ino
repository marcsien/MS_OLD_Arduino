
// na tej stronie jak podłączyć czujnik : http://akademia.nettigo.pl/ds18b20/ ( jest też tam jak dołączyć biblioteki gdybyś nie miał pobranych )
// nie wiem na jakim stopniu zaawansowania jesteś ale dla pewności narzędzia - plytka i port trzeba ustawić

#include <OneWire.h>
#include <DS18B20.h>

// Numer pinu do którego podłaczasz czujnik
#define ONEWIRE_PIN 2

// Adres czujnika
byte address[8] = {0x28, 0xB1, 0x6D, 0xA1, 0x3, 0x0, 0x0, 0x11};   // na wyżej podanej stronie jest jak sprawdzić adres czujnika, najpierw podłącz czujnik, wgraj program do odczytu adresu czujnika , skopiuj ten adres i podmień go tutaj w tej linijce ) 

OneWire onewire(ONEWIRE_PIN); // onewire to obiekt typu  OneWire
DS18B20 sensors(&onewire); // sensors to obiekt typu DS18B20 & oznacza że jest w tym nawiasie nie bezpośrednio obiekt onewire sam w sobie tylko jego adres - jak chcesz się zagłębić to poczytaj o wskaźnikach c/c++
float temp = 0 ; // ( wartosc początkowa 0 zeby domyślnie lpg był wyłączony ) 

void piszcz()
{
  digitalWrite(5, HIGH);
  delay(300); // czeka 300 milisekund - dobierz sobie ten czas experymentalnie zeby ładnie brzmiało ;p
  digitalWrite(5, LOW);
  delay(50);
  digitalWrite(5, HIGH);
  delay(300); // czeka 300 milisekund
  digitalWrite(5, LOW);
  delay(50);
}

void setup() {
  pinMode(3, OUTPUT); // led zielona , druga nóżka do masy przez rezystor
  pinMode(4, OUTPUT); // led czerwona, jak wyżej
  pinMode(5, OUTPUT); // buzzer
  pinMode(6, OUTPUT); // cewka przekaźnika - jak będziesz przerabiał to daj znać , na razie zostaw niepodłączone 
  // 4 powyzsze linijki to ustawienie pinów 3 4 5 6 jako wyjscia , niestety we wszystkich mikroklockach to trzeba robić bo domyslnie sąpowyłączane ze względu na oszczędzanie prądu czy coś tam
  sensors.begin(); // begin() to program tak jak by obiektu sensors , jego instrukcje są w bibliotece i nas nie interesuja
  sensors.request(address); // reguest() to też program sensors ale tutaj w nawiasie jest wymagany adres
}

void loop() {
  if (sensors.available()) // available sprawdza czy są dostępne czujniki - jeśli nie będą to cały ten if będzie pominięty bo "sensors.available()" będzie miał wartość 0
  {
    float temperature = sensors.readTemperature(address); 
    temp = temperature; // zrobiłem tak bo temperature to zmienna lokalna - jest ważna tylko wewnątrz tego ifa ( bo tutaj zadeklarowana ) , natomiast temp jest zmienną globalną obowiązującą w całym sketchu                
    sensors.request(address);
  }

  if (temp > 15) // jesli zmienna temp wynosi wiecej niż 15:
{
  digitalWrite(3, HIGH); // zapala zieloną
  digitalWrite(4, LOW); // gasi czerwoną
  digitalWrite(6, HIGH); // podaje napiecie na cewkę przekaźnika
  
} 
  else if (temp == 15) // == to nie jest to samo co = ( wstukaj sobie w necie "operatory logiczne arduino")
{
piszcz(); // wykonuje podprogram o nazwie piszcz (jego instrukcje są wyżej)    
}
  else // w kazdym innym przypadku:
{
  digitalWrite(6, LOW); // przekaźnik wyłączony
  digitalWrite(3, LOW); // gasi zieloną
  digitalWrite(4, HIGH); // zapala czerwoną
}


  
}
