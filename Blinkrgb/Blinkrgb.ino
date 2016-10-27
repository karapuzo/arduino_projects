#include <PinChangeInt.h>
#include <limits.h>

int RPin=6;
int BPin=5;
int GPin=3;
int ButtonPin=8;
bool isButtonPressed=false;
bool stateOn=true;
signed long CHANGE_STATE_CYCLES=1800;

#include "PinChangeInt.h"

void HSV_to_RGB(float h, float s, float v, byte &r, byte &g, byte &b)
{
  int i,f,p,q,t;
  
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100;
  v /= 100;
  
  if(s == 0) {
    // Achromatic (grey)
    r = g = b = round(v*255);
    return;
  }

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch(i) {
    case 0:
      r = round(255*v);
      g = round(255*t);
      b = round(255*p);
      break;
    case 1:
      r = round(255*q);
      g = round(255*v);
      b = round(255*p);
      break;
    case 2:
      r = round(255*p);
      g = round(255*v);
      b = round(255*t);
      break;
    case 3:
      r = round(255*p);
      g = round(255*q);
      b = round(255*v);
      break;
    case 4:
      r = round(255*t);
      g = round(255*p);
      b = round(255*v);
      break;
    default: // case 5:
      r = round(255*v);
      g = round(255*p);
      b = round(255*q);
    }
}

int setRGB(int R, int G, int B)
{
  analogWrite(RPin,R); 
  analogWrite(GPin,G); 
  analogWrite(BPin,B); 
}

void ButtonPressed()
{
  if ( digitalRead(ButtonPin) == LOW)
  {
    double f=0;
    int r,g,b;
    unsigned long cycle=0;
    
    while (digitalRead(ButtonPin) == LOW)
    {
      cycle++;
      r=0;g=0;b=0;
      if (f<=255)
      {
        r=f;
      }
      else if (f>256 and f<512)
      {
        g=f-256;
      }
      else
      {
        b=f-512;
      }

      if (stateOn)
        setRGB(r,g,b);
        
      f=f+0.4;
      if (f>768)
        f=0;
      delay(10);
      if (cycle>CHANGE_STATE_CYCLES)
      {
        break;
      }
    }
   
    if (cycle<3) 
    {
      // this is just noise
    }
    else
    {
      isButtonPressed=true; 
      if (cycle>CHANGE_STATE_CYCLES) 
      {
        stateOn=!stateOn;
      }
    }
  }
  
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(RPin, OUTPUT);
  pinMode(GPin, OUTPUT);
  pinMode(BPin, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);
  attachPinChangeInterrupt(ButtonPin, ButtonPressed,FALLING); 
  Serial.begin(9600);
}



// the loop function runs over and over again forever
void loop() 
{
  byte Val[3],ValNew[3];
  float Step[3];

  HSV_to_RGB(random(0, 360),100,100,Val[0],Val[1],Val[2]);
      
  for (int i=1;i<1000000;i++)
  {
    

    HSV_to_RGB(random(0, 360),100,100,ValNew[0],ValNew[1],ValNew[2]);

    float distance=0;
    for (int j=0;j<3;j++)
    {
      distance+=pow((ValNew[j]-Val[j]),2);
    }
    distance=sqrt(distance);
   
    float steps=distance/6;
    for (int j=0;j<3;j++)
    {
      //ValNew[j]= random(0, 360);
      //ValNew[j]= random(0, 255*Factor[j]);
      Step[j]=(ValNew[j]-Val[j])/steps;
    }
    
    for (int j=0;j<steps;j++)
    {
       byte r,g,b;


       if (!stateOn)
       {
         setRGB(0,0,0);
       }
       else
       {
         setRGB( Val[0]+j*Step[0],Val[1]+j*Step[1], Val[2]+j*Step[2]); 
       }

       if (isButtonPressed && stateOn)
       {
         isButtonPressed=false;
         for (int k=0;k<5;k++)
         {
           setRGB(255,0,0);
           delay(250);
           setRGB(0,255,0);
           delay(250);
           setRGB(0,0,255);
           delay(250);
         }
       }
       delay(15);         
    }

    for (int j=0;j<3;j++)
      Val[j]= ValNew[j];
        
  }
  
}
