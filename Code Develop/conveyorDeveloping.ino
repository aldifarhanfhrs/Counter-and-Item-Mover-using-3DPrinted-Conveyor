//referensi drv8825: https://lastminuteengineers.com/drv8825-stepper-motor-driver-arduino-tutorial/

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <EEPROM.h>

#define btnUp   9
#define btnOk   7
#define btnDown 8
#define btnBack 6

#define ledBlue 3
#define ledGreen 2

const int dirPin = 11;
const int stepPin = 10;
const int stepsPerRevolution = 200;

bool statusBtnUp   = false;
bool statusBtnOk   = false;   
bool statusBtnDown = false;
bool statusBtnBack = false;

bool statusAkhirBtnUp   = false;
bool statusAkhirBtnOk   = false;
bool statusAkhirBtnDown = false;
bool statusAkhirBtnBack = false;

bool UP   = false;
bool _OK   = false;
bool DOWN = false;
bool BACK = false;

int countValue  = 0;

volatile bool buttonPressedUp = false;
volatile bool buttonPressedDown = false;
volatile bool buttonPressedOK = false;
volatile bool buttonPressedBack = false;

Adafruit_SSD1306 lcd(128, 64, &Wire, -1);
//--------------------------------------------------------------------------------
int halaman  = 1;
int menuItem = 1;
//--------------------------------------------------------------------------------
long int counter = 0;
int hitObject = false;
int Count_limit = 0;
int irSensor = A1;
int CV;

int kondisi = HIGH;
int hitObject = false;
//--------------------------------------------------------------------------------

void setup(){
  Serial.begin(9600);
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  lcd.clearDisplay();

  pinMode(btnUp,   INPUT_PULLUP);
  pinMode(btnOk,   INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnBack, INPUT_PULLUP);
  pinMode (irSensor, INPUT);

  pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
    
  pinMode (ledBlue, OUTPUT);
  pinMode (ledGreen, OUTPUT);

    // Set the spinning direction CW/CCW:
  digitalWrite(dirPin, HIGH);

  //deklarasi EEPROM Count Value
  //EEPROM.commit();
  CV = EEPROM.read(countValue);

  if (CV>= 255) {CV = 0;}

}

void loop(){
  tampil();
  counter_();

  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);

  counter = 0;
  statusBtnUp   = digitalRead(btnUp);
  statusBtnOk   = digitalRead(btnOk);
  statusBtnDown = digitalRead(btnDown);
  statusBtnBack = digitalRead(btnBack);

  saatUpDitekan();
  saatOkDitekan();
  saatDownDitekan();
  saatBackDitekan();


//--------------------------------------------------------------------------------
  //  untuk button up
  if (UP && halaman == 1) {
    UP = false;
    menuItem --;
    if (menuItem < 1)menuItem = 2;
  }
  //untuk button down
  if (DOWN && halaman == 1) {
    DOWN = false;
    menuItem ++;
    if (menuItem > 2)menuItem = 1;
  }

  //  untuk button ok
  if (_OK == true) {
    _OK = false;
    if (halaman == 1 && menuItem == 1) {
      halaman = 2;
    } else if (halaman == 1 && menuItem == 2) {
      halaman = 3;
    } else if (halaman == 1 && menuItem == 3) {
      halaman = 4;
    } else if (halaman == 1 && menuItem == 4) {
      halaman = 5;
    } else if (halaman == 1 && menuItem == 5) {
      halaman = 6;
    }
  }

  //  untuk button back
  if (BACK == true) {
    BACK = false;
    if (halaman == 2 || halaman == 3 || halaman == 4 || halaman == 5 || halaman == 6) {
      halaman = 1;
    }
  }
 }
//----------------------------------------------------------------------------
void saatUpDitekan() {
  if (statusBtnUp != statusAkhirBtnUp) {
    if (statusBtnUp == 0) {
      UP = true;
    }
    delay(50);
  }
  statusAkhirBtnUp = statusBtnUp;
}

void saatOkDitekan() {
  if (statusBtnOk != statusAkhirBtnOk) {
    if (statusBtnOk == 0) {
      _OK = true;
    }
    delay(50);
  }
  statusAkhirBtnOk = statusBtnOk;
}

void saatDownDitekan() {
  if (statusBtnDown != statusAkhirBtnDown) {
    if (statusBtnDown == 0) {
      DOWN = true;
    }
    delay(50);
  }
  statusAkhirBtnDown = statusBtnDown;
}

void saatBackDitekan() {
  if (statusBtnBack != statusAkhirBtnBack) {
    if (statusBtnBack == 0) {
      BACK = true;
    }
    delay(50);
  }
  statusAkhirBtnBack = statusBtnBack;
}
//----------------------------------------------------------------------------

void tampil() {
  if (halaman == 1) {
    lcd.clearDisplay();
    lcd.setTextSize(1);
    lcd.setTextColor(WHITE);
    lcd.setCursor(17, 0); //(X, Y);
    lcd.print("CONVEYOR COUNTER");
    lcd.setCursor(0, 7); //(X, Y);
    lcd.print("_____________________");

    if (menuItem == 1) {
      lcd.setCursor(5, 17);
      lcd.setTextColor(WHITE);
      lcd.setTextSize(2);
      lcd.print("=> SET MODE");
    } else {
      lcd.setCursor(5, 17);
      lcd.setTextSize(2);
      lcd.setTextColor(WHITE);
      lcd.print("   SET MODE");
    }

    if (menuItem == 2) {
      lcd.setCursor(5, 27);
      lcd.setTextColor(WHITE);
      lcd.setTextSize(2);
      lcd.print("=> SERVO MODE");
    } else {
      lcd.setCursor(5, 27);
      lcd.setTextSize(2);
      lcd.setTextColor(WHITE);
      lcd.print("   SERVO MODE");
    }

  
  } else if (halaman == 2) {

    lcd.clearDisplay();
    lcd.setTextColor(WHITE);
    lcd.setTextSize(1);
    lcd.setCursor(0, 0);
    lcd.print("PV:");
    lcd.setTextSize(2);
    lcd.setCursor(0, 10);
    //lcd.print(Tc,1);
    //lcd.setTextSize(1);
    //lcd.setCursor(0, 25);
    //lcd.print("-----------");
    
    lcd.setTextSize(1);
    lcd.setCursor(0, 32);    
    lcd.print("Err:");
    lcd.setTextSize(2);
    lcd.setCursor(0, 42);
    //lcd.print(Error,1); //(Tc,1)
    delay(100);
        
    lcd.setTextSize(1);
    lcd.setCursor(65, 0);
    lcd.print("SV:");
    lcd.setTextSize(2);
    lcd.setCursor(84, 0);
    //lcd.print(SV);

    lcd.setTextSize(1);
    lcd.setCursor(64, 20);
    lcd.print("Kp :");
    //lcd.print(Kp);

    lcd.setTextSize(1);
    lcd.setCursor(64, 30);
    lcd.print("Ki :");
    //lcd.print(Ki);

    lcd.setTextSize(1);
    lcd.setCursor(64, 40);
    lcd.print("Kd :");
    //lcd.print(Kd);

    lcd.setTextSize(1);
    lcd.setCursor(64, 50);
    lcd.print("PID:");
    //lcd.print(PID,1);
    //lcd.setTextSize(2);
    //lcd.setCursor(64, 10);
    //lcd.print(SV);
    
    
  } else if (halaman == 3) {
    CV = EEPROM.read(countValue);

      lcd.clearDisplay();
      lcd.setTextSize(2);
      lcd.setTextColor(WHITE);
      lcd.setCursor(0, 0);
      lcd.println("Set Value");
      lcd.setCursor(0, 20);
      lcd.setTextSize(4);
      lcd.print(CV);

      if (digitalRead(btnUp) == LOW){
        delay(20);
        counter --;
        //if(SV > 255) SV = 1;
        EEPROM.write(countValue, counter);
        //EEPROM.commit();
        lcd.display();
      }
      if (digitalRead(btnDown) == LOW){
        delay(20);
        counter ++;
        //if(SV < 0) SV = 255;
        EEPROM.write(countValue, counter);
        //EEPROM.commit();
        lcd.display();
      }
      //} 
    
  } 
  lcd.display();
}
//----------------------------------------------------------------------------
void counter_(){
  int hitung = digitalRead(proximity);
  if ((hitung == 0) && (hitObject == false))
  {      
    counter++;  
    hitObject = true;  
    Serial.print("Counter: ");  
    Serial.println(counter); 

    lcd.setCursor(12,0);  
    lcd.print(counter);  
    delay(100);  
  }  
  else if ((hitung == 1) && (hitObject == true))  
  {  
    hitObject = false;  
  //delay(100);  
  }  
}