int lew = 9;           
int praw = 10;
int czuj1 = 2;
int czuj2 = 3;
int czuj3 = 4;
int czuj4 = 5;
int czuj5 = 6;
int czuj6 = 7;
int czuj7 = 8;
int st1 = 0;
int st2 = 0;
int st3 = 0;
int st4 = 0;
int st5 = 0;
int st6 = 0;
int st7 = 0;


void setup() {
  pinMode(lew, OUTPUT);
  pinMode(praw, OUTPUT);
  pinMode(czuj1, INPUT);
  pinMode(czuj2, INPUT);
  pinMode(czuj3, INPUT);
  pinMode(czuj4, INPUT);
  pinMode(czuj5, INPUT);
  pinMode(czuj6, INPUT);
  pinMode(czuj7, INPUT);
}
void loop() {
  st1 = digitalRead(czuj1);
  st2 = digitalRead(czuj2);
  st3 = digitalRead(czuj3);
  st4 = digitalRead(czuj4);
  st5 = digitalRead(czuj5);
  st6 = digitalRead(czuj6);
  st7 = digitalRead(czuj7);


  if (st4==1) 
  {
  analogWrite(lew, 250);
  analogWrite(praw, 250);
  } 
  else 
  {
  if (st3==1 && st5==1)
  {
  analogWrite(lew, 250);
  analogWrite(praw, 250);
  }
  else
  {
  if (st2==1 && st6==1)
  {
  analogWrite(lew, 250);
  analogWrite(praw, 250);
  }
  else 
  {
    if (st1==1 && st7==1)
    {
        analogWrite(lew, 250);
  analogWrite(praw, 250);
 
    }
    else 
    {
      if (st1==1)
      {
          analogWrite(lew, 0);
  analogWrite(praw, 250);
      }
      else
      {
        if (st2==1)
        {
            analogWrite(lew, 0);
  analogWrite(praw, 200);
        }
        else
        {
          if (st3==1)
          {
              analogWrite(lew, 0);
  analogWrite(praw, 150);
          }
          else
          {
            if (st5==1)
            {
                analogWrite(lew, 150);
  analogWrite(praw, 0);
            }
            else 
            {
              if (st6==1)
              {
                  analogWrite(lew, 200);
  analogWrite(praw, 0);
              }
              else
              {
                if (st7==1)
                {
                    analogWrite(lew, 250);
  analogWrite(praw, 0);
                }
                else
                 {
                   analogWrite(lew, 100);
  analogWrite(praw, 100);
                 }
                }
              }
            }
          }
        }
      }
    }
  }
  }
    }
  



