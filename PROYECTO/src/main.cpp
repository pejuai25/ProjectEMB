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
#include <RFID.h>
#include <Servo.h>

Servo servos;
Servo servoa; 
#define RST_PIN         9          
#define SS_PIN        10
RFID rfid(SS_PIN, RST_PIN);              
String rfidCard;

#include "HX711.h"
const int DOUT=A2;
const int CLK=7;

HX711 balanza;

LiquidCrystal_I2C lcd(0x3F,16,2);


uint16_t demora = 500; 
int status_dulce=0;
int status_servo=0;
int MyM= 0;
int Milk=1;
int mint=2;
int gum=4; 
float peso;
float pM_M;
float pmilk;
float pmint;
float pgum;
float usuario;
unsigned long millis_tiempo;
void Serial_Enviar() {
  String datos = "{\"pM_M\":";
  datos = datos + pM_M + ",\"pmilk\":";
  datos = datos + pmilk + ",\"pmint\":";
  datos = datos + pmint + ",\"pgum\":";
  datos = datos + pgum + ",\"usuario\":"; datos = datos + usuario + "}";
  Serial.print(datos);
  // Mostrar en consola los datos que se enviaron
  Serial.println("Datos enviados: pM_M=" + String(pM_M) + ", pmilk=" + String(pmilk) + ", pmint=" + String(pmint) + ", pgum=" + String(pgum)+", usuario=" + String(usuario));
}
void setup()
{ 
  PORTB |= (1<<PB0); //PAGAR
   PORTC |= (1<<PC0); // ELEGIR
   PORTC |= (1<<PC1); //PRECIO MOSTRAR
   PORTD |= (1<<PD4); //ABRIR
   PORTD |= (1<<PD3);// CANCELAR
 Serial.begin(9600);
 balanza.begin(DOUT, CLK);
    
  
  balanza.set_scale(186676.191);
   balanza.tare(20);
  SPI.begin();
  rfid.init();
  
 
 
  // DDRD=0XFF;
   //PORTD=0X00;
   lcd.init();
   lcd.backlight();
    servos.attach(5);
    servoa.attach(6);
}
   void loop(){
    char M_M[100];
   char milk[100];
   char mint[100];
   char gum[100];
   peso=(balanza.get_units(20), 1);
   pM_M=peso*0.0025;
  pmilk=peso*0.001;
  pmint=peso*0.00125;
  pgum=peso*0.002;
  delay(500);
 //PAGAR
  if(!(PINB&(1<<PB0))){
    lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("INSERTE SU TARJETA");
    _delay_ms (1000);
    if (rfid.isCard()) 
        {  
            if ( rfid.readCardSerial()) 
            {
                   rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
                  lcd.print(rfidCard);    
                   lcd.clear();             
                  if(rfidCard == "167 247 115 52"){
                    lcd.println("Acceso concedido...");
                     _delay_ms (10000);
                     usuario=1;
                      Serial_Enviar();
                      lcd.clear();
                      lcd.println("datos enviados");
                      _delay_ms (1000);
                      lcd.clear();
            }else{
                    lcd.println("Acceso denegado...");
                     _delay_ms (10000);
                      lcd.clear();
                  rfid.halt();
            }
          
          }
      
	    }
  }
    
  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Elija su dulce:");
      //CANCELAR
          if(!(PIND &(1<<PD3))){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Elija su dulce:");
        
       }//ELEGIR
         if(!(PINC &(1<<PC0))){
       status_dulce=status_dulce+1;
       status_servo=status_servo+45;
       Serial.println(status_servo);
        servos.write(status_servo);
      _delay_ms (demora);
      }
//ABRIR
   if(!(PIND&(1<<PD4))){
      servoa.write(90);
   }else{servoa.write(0);}
     if(status_dulce==0){//MOSTRAR
        if(!(PINC&(1<<PC1))){
            lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("M&M: $2.5xKg");
     _delay_ms (1000);
    lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("Saldo a Pagar:");
       _delay_ms (1000);
        dtostrf(pM_M, 5, 1, M_M);
            lcd.clear();
           lcd.setCursor(0,0);
           lcd.print(M_M);
          _delay_ms (1000);
          }

         }if(status_dulce==1){
    if(!(PINC&(1<<PC1))){
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Milk: $1xKg");
     _delay_ms (1000);
      lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("Saldo a Pagar:");
       _delay_ms (1000);
       dtostrf(pmilk, 5, 1, milk);
            lcd.clear();
           lcd.setCursor(0,0);
           lcd.print(milk);
          _delay_ms (1000);
       
       } 
               
            

}if(status_dulce==2){
    if(!(PINC&(1<<PC1))){
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Mint: $1.25xKg");
     _delay_ms (demora);
     lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("Saldo a Pagar:");
       _delay_ms (1000);
       dtostrf(pmint, 5, 1, mint);
            lcd.clear();
           lcd.setCursor(0,0);
            lcd.print(mint);
          _delay_ms (1000);
       }
                
            

}if(status_dulce==3){
    if(!(PINC&(1<<PC1))){
           lcd.clear();
           lcd.setCursor(0,0);
         lcd.print("Gum: $2xKg");
     _delay_ms (demora);
        lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("Saldo a Pagar:");
       _delay_ms (1000);
       dtostrf(pgum, 5, 1, gum);
               lcd.clear();
           lcd.setCursor(0,0);
            lcd.print(gum);
       _delay_ms (1000);
       }
                
            
}if(status_dulce==4){
   status_servo=0;
   status_dulce=0;
}
       
 
  }

