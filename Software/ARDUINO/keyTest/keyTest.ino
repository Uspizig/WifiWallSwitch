//Definitionen für Keyboard
uint8_t keyboard_val = 0;
 #define KEYPRESS_DETECTION_VALUE 0  
//Keyboard PINS ESP32-S3
  #define ROW_NUM     3 
  #define COLUMN_NUM  3 
  #define ROW1 7
  #define ROW2 15
  #define ROW3 16
  #define COL1 4
  #define COL2 5
  #define COL3 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("starting up");
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  keyboard_val =  key_test2();
  Serial.println(millis()); //Serial.print(" keyboard_val"); Serial.println(keyboard_val);
  delay(500);
}








int key_test2(void){
    String str;
    int rueckgabe = 0;
    int nRow = 3;
    int nCol = 3;
    int KeyBoard[nRow][nCol] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    uint8_t colPins[3] = {ROW1, ROW2, ROW3}; //connect to the column pinouts of the keypad
    pinMode(ROW1, OUTPUT);
    pinMode(ROW2, OUTPUT);
    pinMode(ROW3, OUTPUT);
    pinMode(COL1, INPUT);
    pinMode(COL2, INPUT);
    pinMode(COL3, INPUT);
    
    //Abfrage der Tasten
    for (int i = 0; i < nCol; i++){
      digitalWrite(colPins[i], LOW);  
      KeyBoard[i][0] = digitalRead(COL1); 
      KeyBoard[i][1] = digitalRead(COL2); 
      KeyBoard[i][2] = digitalRead(COL3); 
      digitalWrite(colPins[i], HIGH);  
      Serial.print("Col"); Serial.print(i);
      Serial.print(KeyBoard[i][0]);
      Serial.print(KeyBoard[i][1]);
      Serial.println(KeyBoard[i][2]);
      
    }
    
    
    //DebugAusgabe
    //#define WEB_DEBUG_SERIAL 
     for (int i = 0; i < nCol; i++){
      for (int b = 0; b < nRow; b++){
        int a = i+1; int c = b+1;
        if (KeyBoard[i][b] < KEYPRESS_DETECTION_VALUE){         
          #ifdef WEB_DEBUG_SERIAL
            Serial.print(str + a + c +":" + KeyBoard[i][b] +" \n");
          #endif
          rueckgabe = (a*10)+c;
        }
        }
      }
    return rueckgabe;
}