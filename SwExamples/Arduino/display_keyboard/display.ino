/* Compact TFT Graphics Library v2 - see http://www.technoblogy.com/show?2LMJ
   David Johnson-Davies - www.technoblogy.com - 20th January 2021
   ATtiny402 @ 20 MHz (internal oscillator; BOD disabled)
   
   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/
*/

// TFT colour display **********************************************

#define CASET  0x2A // Define column address
#define RASET  0x2B // Define row address
#define RAMWR  0x2C // Write to display RAM

// Send a byte to the display without CS ... faster
void DataFast (uint8_t d) {
#ifdef PICO
  SPI.transfer(d);
#endif
#ifdef ERGO
  SPI1.transfer(d);
#endif  
}

// Send a byte to the display
void Data (uint8_t d) {
  digitalWrite(TFT_CS , LOW);
#ifdef PICO
  SPI.transfer(d);
#endif
#ifdef ERGO
  SPI1.transfer(d);
#endif  
  digitalWrite(TFT_CS , HIGH);
}

// Send a command to the display
void Command (uint8_t c) {
  digitalWrite(TFT_DC , LOW);
  Data(c);
  digitalWrite(TFT_DC, HIGH);
}

// Send a command followed by two data words
void Command2 (uint8_t c, uint16_t d1, uint16_t d2) {
  digitalWrite(TFT_DC, LOW);
  Data(c);
  digitalWrite(TFT_DC, HIGH);
  Data(d1>>8); Data(d1); Data(d2>>8); Data(d2);
}
  
void InitDisplay () {
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);  
  digitalWrite(TFT_DC, HIGH);                  // Data

#ifdef PICO
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, HIGH);  
  digitalWrite(TFT_RST, LOW);
  digitalWrite(TFT_RST, HIGH);  
  SPI.setTX(TFT_MOSI);
  SPI.setSCK(TFT_SCLK);
  SPI.setCS(TFT_CS);
  SPI.begin();
#endif

#ifdef ERGO
  SPI1.begin();
//  SPI1.beginTransaction (SPISettings (40000000, MSBFIRST, SPI_MODE0));
//  SPI1.setClockDivider(20); 
#endif

  Command(0x01);                           // Software reset
  delay(150);                              // delay 150 ms
  Command(0x11);                           // Out of sleep mode
  delay(500);                              // delay 500 ms
  Command(0x3A); Data(0x55);               // Set color mode - 16-bit color
  Command(0x20+invert);                    // Invert
  Command(0x36); Data(rotate<<5);          // Set orientation
}

void DisplayOn () {
  Command(0x29);                           // Display on
  delay(10);
}

void ClearDisplay () {
  Command2(CASET, yoff, yoff + ysize - 1);
  Command2(RASET, xoff, xoff + xsize - 1);
  Command(0x3A); Data(0x03);               // 12-bit colour
  Command(RAMWR);
  digitalWrite(TFT_CS , LOW);
  for (int i=0; i<xsize/2; i++) {
    for (int j=0; j<ysize * 3; j++) {
    DataFast(0);
    }
  }
  digitalWrite(TFT_CS , HIGH);
  Command(0x3A); Data(0x05);               // Back to 16-bit colour
}

unsigned int Colour (int r, int g, int b) {
  return (r & 0xf8)<<8 | (g & 0xfc)<<3 | b>>3;
}

// Move current plot position to x,y
void MoveTo (int x, int y) {
  xa0 = x; ya0 = y;
}

// Plot point at x,y
void plotPoint (int x, int y) {
  Command2(RASET, yoff+y, yoff+y);
  Command2(CASET, xoff+x, xoff+x);
  Command(RAMWR); Data(color>>8); Data(color & 0xff);
}

// Draw a line to x,y
void DrawTo (int x, int y) {
  int sx, sy, e2, err;
  int dx = abs(x - xa0);
  int dy = abs(y - ya0);
  if (xa0 < x) sx = 1; else sx = -1;
  if (ya0 < y) sy = 1; else sy = -1;
  err = dx - dy;
  for (;;) {
    plotPoint(xa0, ya0);
    if (xa0==x && ya0==y) return;
    e2 = err<<1;
    if (e2 > -dy) { err = err - dy; xa0 = xa0 + sx; }
    if (e2 < dx) { err = err + dx; ya0 = ya0 + sy; }
  }
}

void FillRect (int w, int h) {
  Command2(RASET, ya0+yoff, ya0+yoff+h-1);
  Command2(CASET, xa0+xoff, xa0+xoff+w-1);
  Command(RAMWR);
  for (int p=0; p<w*h*2; p++) {
    Data(color>>8); Data(color & 0xff);
  }
}

// Plot an ASCII character with bottom left corner at x,y 24x32
void PlotChar24 (char c) {
  int colour;
  Command2(RASET, yoff+ya0, yoff+ya0+31);
  Command2(CASET, xoff+xa0, xoff+xa0+23);
  Command(RAMWR);
  digitalWrite(TFT_CS , LOW);
    int fntAdr = (96*(c-32)); 
  for (int xx=0; xx<32; xx++) {  
    int bits1 = font24[fntAdr];
    fntAdr++;
    int bits2 = font24[fntAdr];
    fntAdr++;
    int bits3 = font24[fntAdr];
    fntAdr++;
      for (int yy=0; yy<8; yy++) {
        if (bits1>>(7-yy) & 1) colour = color; else colour = background;
          DataFast(colour>>8);
          DataFast(colour & 0xFF);  
      }
      for (int yy=0; yy<8; yy++) {
        if (bits2>>(7-yy) & 1) colour = color; else colour = background;
          DataFast(colour>>8);
          DataFast(colour & 0xFF);  
      }
      for (int yy=0; yy<8; yy++) {
        if (bits3>>(7-yy) & 1) colour = color; else colour = background;
          DataFast(colour>>8);
          DataFast(colour & 0xFF);  
      }    
  }
  xa0 = xa0+24;
  digitalWrite(TFT_CS , HIGH);
}




// Plot an ASCII character with bottom left corner at x,y 16x16
void PlotChar16 (char c) {
  int colour;
  Command2(RASET, yoff+ya0, yoff+ya0+15);
  Command2(CASET, xoff+xa0, xoff+xa0+15);
  Command(RAMWR);
    int fntAdr = (32*(c-32)); 
  digitalWrite(TFT_CS , LOW);
  for (int xx=0; xx<16; xx++) {  
    int bits1 = font16[fntAdr];
    fntAdr++;
    int bits2 = font16[fntAdr];
    fntAdr++;
      for (int yy=0; yy<8; yy++) {
        if (bits1>>(7-yy) & 1) colour = color; else colour = background;
          DataFast(colour>>8);
          DataFast(colour & 0xFF);  
          }
      for (int yy=0; yy<8; yy++) {
        if (bits2>>(7-yy) & 1) colour = color; else colour = background;
          DataFast(colour>>8);
          DataFast(colour & 0xFF);  
      }
   
  }
  xa0 = xa0 + 16;
  digitalWrite(TFT_CS , HIGH);
}

// Plot an ASCII character with bottom left corner at x,y 16x16
void PlotChar16x24 (char c) {
  int colour;
  Command2(RASET, yoff+ya0, yoff+ya0+23);
  Command2(CASET, xoff+xa0, xoff+xa0+15);
  Command(RAMWR);
    int fntAdr = (48*(c-32)); 
  for (int xx=0; xx<24; xx++) {  
    int bits1 = font16x24[fntAdr];
    fntAdr++;
    int bits2 = font16x24[fntAdr];
    fntAdr++;
      for (int yy=0; yy<8; yy++) {
        if (bits1>>(7-yy) & 1) colour = color; else colour = background;
          Data(colour>>8);
          Data(colour & 0xFF);  
      }
      for (int yy=0; yy<8; yy++) {
        if (bits2>>(7-yy) & 1) colour = color; else colour = background;
          Data(colour>>8);
          Data(colour & 0xFF);  
      }
   
  }
  xa0 = xa0 + 16;
}

// Plot text starting at the current plot position
void PlotText24(PGM_P p) {
  while (1) {
    char c = pgm_read_byte(p++);
    if (c == 0) return;
    PlotChar24(c);
  }
}

void displayUpdate() {
int p=0;
  while (1) {
    char c = screen[p++];
    if (c == 0) return;
    if (c == 13)   {
      ya0 = ya0+32;
      }
      else if (c == 10)
      {
      xa0 = 0;
      }
      else if (xa0  >= 240)
      {
      xa0 = 0;
      ya0 = ya0+32;
      }
      else
      {
           PlotChar24(c); 
      }

  }
}

void displayUpdate16() {
int p=0;
  while (1) {
    char c = screen[p++];
    if (c == 0) return;
    if (c == 13)   {
      ya0 = ya0+16;
      }
      else if (c == 10)
      {
      xa0 = 0;
      }
      else if (xa0  >= 240)
      {
      xa0 = 0;
      ya0 = ya0+16;
      }
      else
      {
           PlotChar16(c); 
      }

  }
}

void displayUpdate16x24() {
int p=0;
  while (1) {
    char c = screen[p++];
    if (c == 0) return;
    if (c == 13)   {
      ya0 = ya0+24;
      }
      else if (c == 10)
      {
      xa0 = 0;
      }
      else if (xa0  >= 240)
      {
      xa0 = 0;
      ya0 = ya0+24;
      }
      else
      {
           PlotChar16x24(c); 
      }

  }
}
