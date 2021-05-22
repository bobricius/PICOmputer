char getKey ()
{
char key=0;
char key1 = getKeyOne();
//    delay(10);

char key2 = getKeyOne();
if (key1==key2) {
  key =key1;
 while (getKeyOne()) {
  delay(5);
    digitalWrite(TFT_BACKLIGHT,HIGH); // Backlight on
  }
}
                        if (key == KEY_SHIFT)
                          {
//                            beep1();
                            delay(300);
                            Layout++;
                            if (Layout >= 3) {Layout = 0;}
                            key = 0;  
                           }
                  if (key>9){ Layout = 0; }                          
 return key;
}


char getKeyOne ()
{
char key=0;

// SETUP KEYBOARD
    for(byte x=0; x<COLS; x++) {
        pinMode(colPins[x], OUTPUT);
        digitalWrite(colPins[x], HIGH);
    }
    for (byte x=0; x<ROWS; x++) {
        pinMode(rowPins[x], INPUT_PULLUP);
    }


    for (byte colIndex=0; colIndex < COLS; colIndex++) {
        // col: set to output to low
        byte curCol = colPins[colIndex];
        digitalWrite(curCol, LOW);
        // row: interate through the rows
        for (byte rowIndex=0; rowIndex < ROWS; rowIndex++) {
            byte curRow = rowPins[rowIndex];
            pinMode(curRow, INPUT_PULLUP);
            if (digitalRead(curRow)==0)
            {
                  switch (Layout) {
                    case 0:
                      key=directKeys[rowIndex][colIndex];
                      break;
                    case 1:
                      key=shiftKeys[rowIndex][colIndex];
                      break;
                    case 2:
                      key=symbolKeys[rowIndex][colIndex];
                      break;
                    default:
                      Layout = 0;
                      break;
                  }
                  digitalWrite(TFT_BACKLIGHT,HIGH); // Backlight on
//                  beep1();
//                  delay(100);

                            return key;       
        }

    }
        // disable the column
        pinMode(curCol, HIGH);
  }

  digitalWrite(TFT_BACKLIGHT,HIGH); // Backlight on  
  return key;  
}
