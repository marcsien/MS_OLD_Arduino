#include <Wire.h>   // standardowa biblioteka Arduino do komunikacji i2c
#include <LiquidCrystal_I2C.h> // biblioteka i2c do LCD
#include <Keypad.h> //biblioteka od klawiatury
#include <Password.h>//biblioteka do hasel

#define BUZZ 13 // plusowa nóżka brzęczyka do pinu 13

#define ZIEL 12 // plusowa nóżka zielonej LED idzie do pinu 12 cyfrowego, zolta i czerwona poniżej do odpowiednio a1 i a0
#define ZOLT A1
#define CZER A0

#define LASER1 9 // plusowe nóżki do laserów idą jak tutaj zdefiniowałem
#define LASER2 10
#define LASER3 11

#define CZUJ1 A2 // pin mierzacy napiecie czujnika 1 to a2 , pozostale czujniki jak ponizej
#define CZUJ2 A3
#define CZUJ3 A4

int progalarmu1 = 1024; // definicja i deklaracja zmiennej przechowującej prog alarmu dla czujnika 1 wartością 1024, pozostale czujniki jak ponizej
int progalarmu2 = 1024;
int progalarmu3 = 1024;

int pwmlasera1 = 255; // zmuienna przechowująca wspolczynnik wypenienia pwm dla lasera 1 , początkowa wartość to max czyli 255, w trakcie kalibracji dobierane są takie wsp zeby wartosc czujnkow wynosila odpowiednią wartość określoną w funkcji kalibracja
int pwmlasera2 = 255;
int pwmlasera3 = 255;


int stanniski1 = 0; // zmienna przechowująca wartość stanu niskiego czyli kiedy laser nie swieci - rozna w pomieszczeniach o roznych oswietleniach, chyba tej zmiennej nie uzywalem wcale ale to jako ciekawostka mozesz jakos w pracy wykorzystac
int stanniski2 = 0;
int stanniski3 = 0;

int stanwysoki1 = 1024; // zmienna przechowująca wartość stanu wysokiego czyli jak laser swieci 
int stanwysoki2 = 1024;
int stanwysoki3 = 1024;

double czulosc = 0.99; // czulosc - im blizej 1 tym czulszy alarm

bool stanhaslo = false; // zmienna typu bool z informacją o stanie wpisanego hasła, jesli jest false to pin byl wpisany xle, wpisuajc dobry zmienia sie to na true

Password password = Password( "7777" ); // obiekt z biblioteki Password , ustawilem haslo na 7777

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x3F - ten adres odczytalem za pomocą programu odpowiedniego ,w folderze gdzies znajdziesz
const byte ROWS = 4; // ile wierszy
const byte COLS = 3; //ile kolumn
byte rowPins[ROWS] = {2, 3, 4, 5}; //piny wierszy czyli gdzie idą odpowiednie połączenia
byte colPins[COLS] = {6, 7, 8}; //piny kolumn czyli gdzie idą odpowiednie połaczenia
char keys[ROWS][COLS] = { //mapowanie klawiatury
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

Keypad klawiatura = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //inicjalizacja klawiatury

int delaytime = 200; // w tej zmiennej jest ogólny delay programu czyli tak jak by zmienna loop wykonuje się 5x na sekunde, myśle że nie warto zmieniać, dobrze uklad pracuje z taką wartością

enum typystanow // typ enum przechowujący stany urządzenia, mozliwe są jak widać 3 stany
{
    rozbrojony = 0,
    uzbrojony = 1,
    alarm = 2,
};

typystanow stan = rozbrojony; //utworzenie zmiennej enumowej i początkowa wartość rozbrojony

void buzzon() //funkcja która  załącza brzęczyk
{
  digitalWrite(BUZZ, HIGH);
}

void buzzoff()//funkcja która wyłącza brzęczyk
{
  digitalWrite(BUZZ, LOW); 
}

void zielon() // f wlaczajaca zielona diode
{
  digitalWrite(ZIEL, HIGH);
}

void zieloff() //f wylaczajaca zielona diode , ponizej takie same funkcje dla czerwonej i zoltej
{
  digitalWrite(ZIEL, LOW); 
}

void czeron()
{
  digitalWrite(CZER, HIGH); 
}

void czeroff()
{
  digitalWrite(CZER, LOW); 
}

void zolton()
{
  digitalWrite(ZOLT, HIGH); 
}

void zoltoff()
{
  digitalWrite(ZOLT, LOW);  
}

int sredniaczujnikow() //funkcja licząca średnią z 3 czujników w danym momencie, poczatkowo jej używalem ale potem chyba uznalem ze lepiej kazdy czujnik osobno rozpatrywać bo czulszy uklad, mozesz napisać w pracy o tym że tak zrobiles i uzyskales lepsze wyniki, ale z promotorem pogadaj jeszcze
{
  int stan1 = analogRead(CZUJ1); // do stan 1 napiecie z czujnika 1 , moze przyjmowac od 0 do 1024
  int stan2 = analogRead(CZUJ2);
  int stan3 = analogRead(CZUJ3);
  int srednia = ((stan1 + stan2 + stan3 )/3); // tutaj proste liczenie 
  return srednia; // zwraca wartość typu int policzoną
}

void wylaczlasery() //f wylaczajaca lasery 
{
  analogWrite(LASER1,0); // analogWrite to funkcja do sterowania pwm
  analogWrite(LASER2,0);
  analogWrite(LASER3,0);
}

void wlaczlasery() // funkcja wlaczajaca lasery, kazdy swoim wlasnym wspolczynnikiem wypelnienia
{
  analogWrite(LASER1,pwmlasera1);
  analogWrite(LASER2,pwmlasera2);
  analogWrite(LASER3,pwmlasera3);
}

void kalibracja() // funkcja wykonujaca kalibracje, po uruchomieniu programu automatycznie
{
  analogWrite(LASER1,0);// te 3 linijki wylaczają lasery
  analogWrite(LASER2,0);
  analogWrite(LASER3,0);
  
  delay(1000); // sekunda na ustabilizowanie sie czujnika, jesli miałby jakąś tam bezwładnosć
  
  stanniski1 = analogRead(CZUJ1); // stanniski1 tam daje napięcie z  czujnika1 gdy laser nie swieci
  stanniski2 = analogRead(CZUJ2);
  stanniski3 = analogRead(CZUJ3);
  
  Serial.print("stan niski1:"); // Narzędzia - monitor portu szeregowego , tam ci sie wyswietla te wartosci 
  Serial.print(stanniski1);
  Serial.print("stan niski2:");
  Serial.print(stanniski2);
  Serial.print("stan niski3:");
  Serial.print(stanniski3);

   do{ // te trzy pętle zmneijszają na każdym czujniku wsp wypelnienia lasera aż wartość napiecia na czujnikach wyniesie 980, ta liczbe dobralem doswiadczalnie patrzac na wykres napięcia gdy się zmniejsza wspolczynnik wypelnienia na laserze tak żeby te 980 było na jak najbardziej stromym zboczu, zeby lepszą czułosć uzyskać
        pwmlasera1 -=1;
        analogWrite(LASER1,pwmlasera1);
        delay(20);
        Serial.println(analogRead(CZUJ1));//Narzędzia - kreślarka - tutaj beda te wlasnie wykresy 
     }while(analogRead(CZUJ1) > 980);

   do{
        pwmlasera2 -=1;
        analogWrite(LASER2,pwmlasera2);
        delay(20);
        Serial.println(analogRead(CZUJ2));
     } while(analogRead(CZUJ2) > 980);

   do{
        pwmlasera3 -=1;
        analogWrite(LASER3,pwmlasera3);
        delay(20);
        Serial.println(analogRead(CZUJ3));
     } while(analogRead(CZUJ3) > 980);


  stanwysoki1 = analogRead(CZUJ1); // stany wysokie wczytywane sa gdy laser świeci, w sensie napięcie na czujniku, na kazdy swieci laser o swoim wlasnym wsp wypelnienia dobranym powyzej
  stanwysoki2 = analogRead(CZUJ2);
  stanwysoki3 = analogRead(CZUJ3);

  progalarmu1 = czulosc * stanwysoki1; // liczymy progi alarmów dla kazdego czujnika osobno
  progalarmu2 = czulosc * stanwysoki2;
  progalarmu3 = czulosc * stanwysoki3;

  Serial.print("prog 1:"); // w monitorze portu szeregowego bedziesz mial te progi ile policzyło
  Serial.print(progalarmu1);
  Serial.print("prog 2:");
  Serial.print(progalarmu2);
  Serial.print("prog 3:");
  Serial.print(progalarmu3);
   
  for(int i = 0 ; i<3 ; i++) // pętla for robiąca pik pik na znak ze kalib ukończona
  {
    buzzon();
    delay(50);
    buzzoff();
    delay(50);
  }
  
  analogWrite(LASER1,0); // wylaczamy lasery
  analogWrite(LASER2,0);
  analogWrite(LASER3,0);
}

void wykrestrzech() // funkcja która rysuje wykres napięć z czujników, w trzech roznych kolorach , w kreślarce
{
  Serial.print(analogRead(CZUJ1));
  Serial.print(",");
  Serial.print(analogRead(CZUJ2));
  Serial.print(",");
  Serial.print(analogRead(CZUJ3));
  Serial.println();
}

void checkPassword()// funkcja sprawdzania hasła
{
  if (password.evaluate())//jeśli hasło się zgadza
  {  
      lcd.print(" POPRAWNE"); //pisze na lcd poprawne
      delay(1000);//czeka sekunde
      cls();// funkcja clear ponizej jest definicja , zwroc uwage gdzie jej uzywasz bo ona oprocz czyszczenia tej cos wypisuje
      stanhaslo = true; // jesli haslo ok to zmienna stanhasla na true ustawia
      password.reset(); // czysci tak jak by bufor wpisanego hasla bo juz nie potrzebny
      loop(); // wraca do wykonywania pętli loop
  }
  else//w przeciwnym razie
  {
      lcd.print(" ZLE"); // wypisuje 
      delay(1000);//poczekaj 1s
      cls();// funkcja clear ponizej jest definicja , zwroc uwage gdzie jej uzywasz bo ona oprocz czyszczenia tej cos wypisuje
      password.reset();// czysci tak jak by bufor wpisanego hasla bo juz nie potrzebny
      stanhaslo = false;// zmienna stanhaslo na false bo haslo xle wpisane
  }
}
void keypadEvent(KeypadEvent eKey)// funkcja która bedzie wywolywana podczas przerwania zdefiniowanego ponizej w setupie , przerwanie wyolywane wcisnieciem czegokolwiek
{ 
  switch (klawiatura.getState()){ // switch w zaleznosci od stanu podejmie odpowiednią decyzje
    case PRESSED: // jesli stan PRESSED to 
      Serial.println(eKey); // monitor portu szeregowego wypisze co wcisnales
      lcd.print('*'); // na wyswietlacz wypisuje gwiazdkę
      switch (eKey){ // switch w zaleznosci od tego co bedzie wcisniete podejmie odpowiednią decyzje 
        case '*': checkPassword(); break;//wywolanie funkcji wyzej - sprawdzanie hasła 
        case '#': password.reset(); break;// reset hasła
    default: 
      password.append(eKey);// dla innych klawiszy niz * lub # dopisuje do password wcisniety klawisz
     }
  }
}
void cls()//funkcja czyszczenia
{
  lcd.clear(); //czyści calkowicie wyswietlacz
  lcd.print("HASLO: "); // wypisuje w gornym wierszu napis haslo
  lcd.setCursor(0, 1); // ustawia kurson w dolny wiersz
}

void setup() 
{
  Serial.begin(9600); // inicjalizacja komunikacji serial , bez tego nie zadziala monitor portu szerego wego ani kreslarka
  
  pinMode(BUZZ, OUTPUT); // ponizej ustawia jako wejscia lub wyjscia odpowiednie piny , np czuj3 jako input
  
  pinMode(ZIEL, OUTPUT);
  pinMode(ZOLT, OUTPUT);
  pinMode(CZER, OUTPUT);

  pinMode(CZUJ1, INPUT);
  pinMode(CZUJ2, INPUT);
  pinMode(CZUJ3, INPUT);

  pinMode(LASER1, OUTPUT);
  pinMode(LASER2, OUTPUT);
  pinMode(LASER3, OUTPUT);
  
  zoltoff(); // wylacza diody 
  zieloff();
  czeroff();

  lcd.begin(16,2);   // Inicjalizacja LCD 2x16
  lcd.backlight(); // zalaczenie podwietlenia 
  lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
  lcd.print("___ ALARM V1 ___");
  delay(500);
  lcd.setCursor(0,1); //Ustawienie kursora w pozycji 0,1 (drugi wiersz, pierwsza kolumna)
  lcd.print("Michal  Szymczak");
  delay(2000);
  lcd.clear();

  lcd.print("___ ALARM V1 ___");
  lcd.setCursor(0,1);
  lcd.print("Kalibracja...");
  
  kalibracja(); //wywolanie funkcji kalibracja 
  
  klawiatura.addEventListener(keypadEvent); // to jest inicjalizacja przerwania od wcisnietego klawisza , od teraz wcisniecie czegokolwiek przerywa na chwile petle loop i odpala funkcje keypadEvent
}

void loop() 
{
  switch(stan) // co czas określony delaytime wykoywany jest ten switch ktory w zaleznosci od stanu urządzenia czyli tej zmiennej enum wykonuje odpowiednie polecenia ponizej napisane
  {
    case rozbrojony: // jesli zmienna stan jest rozbrojony czyli 0 to wykona instrukcje od klamry ponizej do break najblizszego
      {
        wylaczlasery(); // tych funkcji juz nie musze mam nadzieje tlumaczyc 
        lcd.clear();
        lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
        lcd.print("Alarm rozbrojony");
        lcd.setCursor(0,1); // Ustawienie kursora w pozycji 0,1 (drugi wiersz, pierwsza kolumna)
        lcd.print("# uzbroi alarm  ");
        zoltoff();
        zielon();
        czeroff();
        buzzoff();
        do // pętla która sie bedzie wykonywac dopoki stan = uzbrojony, jak sie stan zmieni to ta petla sie skonczy
        {
            wykrestrzech(); // wykresik w kreslarce
            char klawisz = klawiatura.getKey(); // spr czy cos jest wcisniete
            if (klawisz == '#') // jak # wcisniete 
            {
              for(int i = 0; i<4 ; i++) //pika kilka razy
              {
                buzzon();
                delay(50);
                buzzoff();
                delay(50);
              }
              stan = uzbrojony; // stan zmienia się na uzbrojony
            }
        }
        while(stan == 0); // jak stan zmieni sie na uzbrojony to w kolejnym przejsciu petli loop juz bedzie wykonywalo sie to co jest nizej po case uzbrojony

      break;    
      }
    case uzbrojony: // jesli stan uzbrojony to wykonuje te instrukcje do najblizszego break
      {
        wlaczlasery(); //funkcje ktorych nie trzeba tlumaczyc juz mam nadzieje 
        lcd.clear();
        lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
        lcd.print("Alarm uzbrojony");
        lcd.setCursor(0,1); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
        lcd.print("*>PIN>* rozbraja");
        czeroff();
        zolton();
        zieloff();
        buzzoff();
        do // znowu petla ktora sie wykonuje dopoki stan = uzbrojony
        {
          wykrestrzech(); //wykresik 
          char klawisz = klawiatura.getKey(); // co wcisniete 
          if(analogRead(CZUJ1) < progalarmu1 || analogRead(CZUJ2) < progalarmu2 || analogRead(CZUJ3) < progalarmu3) // warunki zalaczenia alarmu czyli napiecia na czujnikach mniejsze od progów przypisanych tym czujnikom , progi policzone w kalibracji
          {
            stan = alarm; // jeśli tak to zmienia stan na alarm, czyli w kolejnym przejsciu petli loop juz nie bedzie sie wykonywal ten case , tylko case alarm
          }
          else if ( stanhaslo == true ) // jednoczesnie sprawdzany warunek jesli bylo wpisane haslo poprawne
          {
            stan = rozbrojony ; // jesli haslo wpisane poprawne to stan rozbrojony 
            stanhaslo = false;  // uzylismy juz zmiennej stanhaslo wiec ją tak jak by resetujemy zeby poprawnie się stany zmienialy
          }
        }
        while(stan == 1); // warunek tej petli
        
      break;
      }
    case alarm: // jesli stan alarm to wykonuje te intrukcje do najbklizsego break
      {
        wylaczlasery();
        lcd.clear();
        lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
        lcd.print("Alarm !!!");
        lcd.setCursor(0,1); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
        lcd.print("*>PIN>* wylacza ");
        czeron();
        zoltoff();
        zieloff();
        buzzon(); // brzeczy w stanie alarmu

        do //petla wykonujaca sie dopoki stan rowna sie alarm
        {
          wykrestrzech(); // wykresik
          char klawisz = klawiatura.getKey();//co wcisnieto
          if(stanhaslo == true) // jesli stanhaslo bedzie true to
          {
            stan = rozbrojony; //zmieni sie stan na rozbrojony
            stanhaslo = false; //uzyta zmienna stanhaslo wiec ją resetujemy
          }
        }while(stan == alarm); //warunek tej petli
      break;
      }
  }
delay(delaytime); // co delaytime wykonuje się pętla loop
}
