#ifdef ERGO
  #define TFT_CS  10
  #define TFT_DC  27
  #define TFT_BACKLIGHT 0 //10 Display backlight pin 
#endif

#ifdef PICO
  #define TFT_CS        17
  #define TFT_RST       21 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC        16
  #define TFT_MOSI     19
  #define TFT_SCLK     18
  #define TFT_BACKLIGHT 20 //10 Display backlight pin 
  #define speaker     0  
#endif


#define black 0x0000
#define white 0xFFFF
#define red 0xF800
#define green 0x07E0
#define blue 0x001F
#define cyan 0x07FF
#define magenta 0xF81F
#define yellow 0xFFE0
#define orange 0xFC00


#define KEY_SHIFT 2
#define KEY_UP 3
#define KEY_LEFT 4
#define KEY_DOWN 5
#define KEY_RIGHT 6
#define KEY_RETURN 10
#define KEY_DELETE 8
const byte ROWS = 6; // rows
const byte COLS = 5; // columns
char directKeys[ROWS][COLS] = {
  {KEY_RETURN, ' ', 'm', 'n', 'b',},
  {KEY_DELETE, 'l', 'k', 'j', 'h',},
  {'p', 'o', 'i', 'u', 'y',},
  {KEY_SHIFT, 'z', 'x', 'c', 'v',},
  {'a', 's', 'd', 'f', 'g',},
  {'q', 'w', 'e', 'r', 't',}  
};
char shiftKeys[ROWS][COLS] = {
  {KEY_RIGHT, ',', 'M', 'N', 'B',},
  {KEY_LEFT, 'L', 'K', 'J', 'H',},
  {'P', 'O', 'I', 'U', 'Y',},
  {KEY_SHIFT, 'Z', 'X', 'C', 'V',},
  {'A', 'S', 'D', 'F', 'G',},
  {'Q', 'W', 'E', 'R', 'T',}  
};

char symbolKeys[ROWS][COLS] = {
  {KEY_RIGHT, '.', ':', ';', 'u',}, //u uvodzovky
  {KEY_LEFT, '-', '*', '&', '+',},
  {'0', '9', '8', '7', '6',},
  {KEY_SHIFT, '(', ')', '?', '/',},
  {'!', '@', '#', '$', '%',},
  {'1', '2', '3', '4', '5',}  
};

#ifdef ERGO
byte colPins[COLS] = {0, 1, 3, 4, 5}; //connect to the column pinouts of the keypad 
byte rowPins[ROWS] = {28, 8, 7, 30, 31, 6}; //row pinouts of the keypad
#endif

#ifdef PICO
byte colPins[COLS] = {1, 2, 3, 4, 5}; //connect to the column pinouts of the keypad extra cursor keys 14
byte rowPins[ROWS] = {6, 9, 15, 8, 7, 22}; //row pinouts of the keypad
#endif

#ifdef ERGO
// 1.54" 240x240 display
int const ysize = 240, xsize = 240, yoff = 0, xoff = 0, invert = 1, rotate = 0; //0-brain 3- pico
#endif

#ifdef PICO
// 1.54" 240x240 display
int const ysize = 240, xsize = 240, yoff = 0, xoff = 0, invert = 0, rotate = 3; //0-brain 3- pico
#endif
