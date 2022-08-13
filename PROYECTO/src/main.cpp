#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "Arduino.h"
#include <avr/io.h>
#include <util/delay.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

Servo servos;
Servo servoa; 
#define RST_PIN         9          
#define SS_PIN        A4               


#include "HX711.h"
const int LOADCELL_DOUT_PIN = A3;
const int LOADCELL_SCK_PIN = 7;

HX711 scale;
MFRC522 mfrc522(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;
LiquidCrystal_I2C lcd(0X27,16,2);


uint16_t demora = 500; 
int status_dulce=0;
int status_servo=0;
int MyM= 0;
int Milk=1;
int mint=2;
int gum=4; 
 byte ActualUID[4];
byte Usuario1[4]= {0x4D, 0x5C, 0x6A, 0x45} ;
byte Usuario2[4]= {0xC1, 0x2F, 0xD6, 0x0E} ; 
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}
void setup()
{ 

    PORTB |= (1<<PB0); 
    PORTB |= (1<<PB2); 
    PORTB |= (1<<PB6); 
    PORTB |= (1<<PB7); 
   PORTC |= (1<<PC0); 
   PORTC |= (1<<PC1); 
   PORTC |= (1<<PC2); 
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(420.0983);
   scale.tare();
    SPI.begin();        
    mfrc522.PCD_Init();
  
 
 
   DDRD=0XFF;
   PORTD=0X00;
   lcd.begin(16,2);
   lcd.backlight();
    servos.attach(5);
    servoa.attach(6);
}
   void loop(){
    char M_M[100];
   char milk[100];
   char mint[100];
   char gum[100];
    float peso=(scale.get_units(10), 1);
    float pM_M=peso*0.0025;
     float pmilk=peso*0.001;
     float pmint=peso*0.00125;
     float pgum=peso*0.002;
  if(!(PINB&(1<<PB0))){
    lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("INSERTE SU TARJETA");
    _delay_ms (10000);
    if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  lcd.print(F("Card UID:"));
                  _delay_ms (10000);
                  lcd.clear();
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          _delay_ms (10000);
                          lcd.clear();
                        lcd.print(mfrc522.uid.uidByte[i], HEX);
                          _delay_ms (10000);
                          lcd.clear();   
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  } 
                  lcd.print("     ");    
                   lcd.clear();             
                  if(compareArray(ActualUID,Usuario1)){
                    lcd.println("Acceso concedido...");
                     _delay_ms (10000);
                      lcd.clear();
                  }
                  else if(compareArray(ActualUID,Usuario2)){
                    Serial.println("Acceso concedido...");
                     _delay_ms (10000);
                    lcd.clear();
            }else{
                    Serial.println("Acceso denegado...");
                     _delay_ms (10000);
                      lcd.clear();
                  mfrc522.PICC_HaltA();
            }
          
          }
      
	    }
  }
  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Elija su dulce:");
          if(!(PINB &(1<<PB2))){
     lcd.clear();
       lcd.setCursor(0,0);
      lcd.print("Elija su dulce:");
       }
         if(!(PINC &(1<<PC0))){
       status_dulce=status_dulce+1;
       status_servo=status_servo+90;
        servoa.write(status_servo);
      _delay_ms (demora);
      
   }

   if(!(PINC&(1<<PC2))){
      servos.write(90);
   }else{
    servos.write(0);
   }
     if(status_dulce==0){
        if(!(PINC&(1<<PC1))){
            lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("M&M: $2.5xKg");
     _delay_ms (10000);
    lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("Debe pagar por M&M");
       _delay_ms (10000);
        dtostrf(pM_M, 5, 1, M_M);
            lcd.clear();
           lcd.setCursor(0,0);
           lcd.print(M_M);
          _delay_ms (10000);
          }

         }if(status_dulce==1){
    if(!(PINC&(1<<PC1))){
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.write("Milk: $1xKg");
     _delay_ms (10000);
      lcd.clear();
           lcd.setCursor(0,0);
           lcd.write("Deber pagar por Milk");
       _delay_ms (10000);
       dtostrf(pmilk, 5, 1, milk);
            lcd.clear();
           lcd.setCursor(0,0);
           lcd.print(milk);
          _delay_ms (10000);
       
       } 
               
            

}if(status_dulce==2){
    if(!(PINC&(1<<PC1))){
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.write("Mint: $1.25xKg");
     _delay_ms (demora);
     lcd.clear();
           lcd.setCursor(0,0);
           lcd.write("Deber pagar por Mint");
       _delay_ms (10000);
       dtostrf(pmint, 5, 1, mint);
            lcd.clear();
           lcd.setCursor(0,0);
            lcd.print(mint);
          _delay_ms (10000);
       }
                
            

}if(status_dulce==3){
    if(!(PINC&(1<<PC1))){
           lcd.clear();
           lcd.setCursor(0,0);
         lcd.write("Gum: $2xKg");
     _delay_ms (demora);
        lcd.clear();
           lcd.setCursor(0,0);
           lcd.write("Deber pagar por Milk");
       _delay_ms (10000);
       dtostrf(pgum, 5, 1, gum);
               lcd.clear();
           lcd.setCursor(0,0);
            lcd.print(gum);
       _delay_ms (10000);
       }
                
            
}if(status_dulce==4){
   status_servo=0;
   status_dulce=0;
}
       
 
  }

