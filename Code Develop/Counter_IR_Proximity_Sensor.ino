#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define proximity 7

int counter = 0;
int kondisi = HIGH;
int hitObject = false;

LiquidCrystal_I2C lcd (0x27, 16, 2);

void setup()
{
  lcd.begin();
  lcd.backlight();
  pinMode (proximity, INPUT);
  Serial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("Counter: "); 
  
}

void loop()  
 {  
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
