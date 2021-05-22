#include <SPI.h>
//#define ERGO
#define PICO
#include "config.h"
#include "fonts.h"
#include <PString.h>



// Globals DISPLAYDRIVER - current plot position and colours
int xa0, ya0;
int color = white; // White
int background = black;      // Black


byte Layout = 0;
String editorText = "";   // text for editor
byte cursor = 0;

#define MaxBuffer 500    //text buffer
char screen[MaxBuffer];
PString display(screen, sizeof(screen)); //

// Setup **********************************************

void setup() {

  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT,HIGH); // Backlight on
  display.begin();
  InitDisplay();
  ClearDisplay();
  DisplayOn();
//  MoveTo(0,0);
//     color = blue;
//     FillRect(240,135);
//  MoveTo(0,0);
//     color = white;
//     FillRect(240,16);
//     color = red; 
beep();
}

void loop () {
     MoveTo(0, 0);
     color = red;
     background = white;
     display.begin(); 
     MoveTo(0, 0);    
char    key = getKey();

  if (key) {
 switch (Layout) {
    case 0:
      if (cursor==0) {
       display.print(" abc ");         
      }
      else
      {
       display.print(" abc ");        
      }

      break;
    case 1:
    display.print("<ABC> [,]");
      break;
    case 2:
    display.print("<123> [.]");
      break;
  } 
      display.println(cursor);    

    displayUpdate16();         
     MoveTo(0, 16);
     color = white;
     background = black;
//     plotPoint(100,0);
    display.begin(); 
   
    byte l=editorText.length();
    editorText=editorText.substring(0, cursor)+key+editorText.substring(cursor,l);
    cursor++;

    display.print(editorText); //.substring(0, cursor)+"_"+editorText.substring(cursor,l)+" ")
    playTone( 50, 6000);
}



       
//    display.print(12);
//    display.println("xdagerogyuq43o8ygt43q87tgq87gt8q7gtq873gt08q7gt83q7gt834q7tg08");
//    display.println("abc");
//    display.print("ABC");
    displayUpdate();

     
//  // Horizontal axis
//  int xinc = (xsize-x1)/20;
//  MoveTo(x1, y1); DrawTo(xsize-1, y1);
//  for (int i=0; i<=20; i=i+4) {
//    int mark = x1+i*xinc;
//    MoveTo(mark, y1); DrawTo(mark, y1-2);
//    // Draw histogram
//    if (i != 20) {
//      int bar = xinc*4/3;
//      fore = Colour(0, 0, 255);
//      MoveTo(mark+bar*2-1,y1+1); FillRect(bar, 5+random(ysize-y1-20));
//      fore = Colour(0, 255, 0);
//      MoveTo(mark+bar,y1+1); FillRect(bar, 5+random(ysize-y1-20));
//      fore = Colour(255, 0, 0);
//      MoveTo(mark+1,y1+1); FillRect(bar, 5+random(ysize-y1-20));
//      fore = 0xFFFF;
//    }
//    int tens = i/10;
//    if (tens != 0) {
//      MoveTo(mark-7, y1-11); PlotChar(tens+'0');
//      MoveTo(mark-1, y1-11); PlotChar(i%10+'0');
//    } else { MoveTo(mark-3, y1-11); PlotChar(i%10+'0'); }
//  }
//  // Vertical axis
//  int yinc = (ysize-y1)/20;
//  MoveTo(x1, y1); DrawTo(x1, ysize-1);
//  for (int i=0; i<=20; i=i+5) {
//    int mark = y1+i*yinc;
//    MoveTo(x1, mark); DrawTo(x1-2, mark);
//    int tens = i/10;
//    if (tens != 0) { MoveTo(x1-15, mark-4); PlotChar(tens+'0'); }
//    MoveTo(x1-9, mark-4); PlotChar(i%10+'0');
//  }

}

void beep ()
{
  playTone( 50, 1000);
  playTone( 50, 3000);
  playTone( 50, 2000);
}

void beep1 ()
{
  playTone( 100, 6000);
}

void beep2 ()
{
beep();
}


void playTone(long duration, int freq) {
        pinMode(25,OUTPUT);
        digitalWrite(25,HIGH);
  pinMode(speaker,OUTPUT);
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        digitalWrite(speaker,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(speaker, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
        digitalWrite(25,LOW);
}
