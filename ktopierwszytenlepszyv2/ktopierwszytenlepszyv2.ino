
void czewin()
{
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void czeprz()
{
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(A0, LOW);
}


void ziewin()
{
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(A0, LOW);
}


void zieprz()
{
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void juzmozna()
{
  for(int i=0;i<3;i++)
  {
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  delay(100);
  } 
}


void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);

juzmozna();

  
}

// the loop function runs over and over again forever
void loop() 
{


  
  if ( digitalRead(11) )
  {
  ziewin();
  while(1)
  {
    ;
  }
  }
  else if ( digitalRead(10) )
  {
  czewin();
  while(1)
  {
    ;
  }
  }





else if ( digitalRead(10) && digitalRead(11) )
  {



    
  float d = random(1, 2);
  if (d>1,5)
  {
    czewin();
    while(1)
    {
      ;
    }
  }
  else
  {
    ziewin();
    while(1)
    {
      ;
    }
  }



  
  }




  
  else 
  {
  czeprz();
  zieprz();
  }

}
  
