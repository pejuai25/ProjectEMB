#include <Arduino.h>
#include "internet.hpp"
#include <ArduinoJson.h>
#include <FirebaseESP32.h>
#define RXp2 16
#define TXp2 17
FirebaseData firebaseData;
float pM_M;
float pmilk;
float pmint;
float pgum;
float usuario;
float ganancias_mym = 200;
float ganancias_milk = 0;
float ganancias_mint = 0;
float ganancias_gum = 0;
float cantidad_mym = 0;
float cantidad_milk = 0;
float cantidad_mint = 0;
float cantidad_gum = 0;
float saldo_1 = 0;
float saldo_2 = 0;
char gmym[13];
char gmilk[13];
char gmint[13];
char ggum[13];
char cmym[13];
char cmilk[13];
char cmint[13];
char cgum[13];
char s1[13];
char s2[13];



// Funcion: Procesar datos recibidor por serial desde el Arduino
void Procesar_Serial() {
  StaticJsonDocument <256> doc;
  deserializeJson(doc, Serial2);
  // Leer variables
  pM_M = doc ["pM_M"];
  pmilk = doc ["pmilk"];
  pmint = doc ["pmint"];
  pgum = doc ["pgum"];
  usuario = doc ["usuario"];
  Serial.println(usuario);
  
}

void setup()
{
  Serial.begin(9600);
 Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
//wifi
connect_wifi();
Firebase.begin("https://project-4bb58-default-rtdb.firebaseio.com", "vKhNzaw5qWBp4Lwo0btMulydTLV2dJm2TmFf9PpF");
}
void loop()
{
    if (Serial2.available()) {Procesar_Serial();}
       // Procesar datos
  
    if(Firebase.getString(firebaseData, "/M&M/GananciasM&M")) {
      String gananciasmym = firebaseData.stringData();
      gananciasmym.toCharArray(gmym, 13); 
      if (atof(gmym) != ganancias_mym) {
        ganancias_mym = pM_M+atof(gmym);
        Firebase.setFloat(firebaseData, "/M&M/GananciasM&M", ganancias_mym);
        //Serial.println(ganancias_mym);
        Serial.println(usuario);
      }
    }
    if (Firebase.getString(firebaseData, "/Milk/GananciasMilk")) {
      String gananciasmilk = firebaseData.stringData();
      gananciasmilk.toCharArray(gmilk, 13); 
      if (atof(gmilk) != ganancias_milk) {
        ganancias_milk = pmilk+atof(gmilk);
        Firebase.setFloat(firebaseData, "/Milk/GananciasMilk", ganancias_milk);
       // Serial.println(ganancias_milk);
      }
    }
     if (Firebase.getString(firebaseData, "/Mint/GananciasMint")) {
      String gananciasmint = firebaseData.stringData();
      gananciasmint.toCharArray(gmint, 13); 
      if (atof(gmint) != ganancias_mint) {
        ganancias_mint = pmint+atof(gmint);
        Firebase.setFloat(firebaseData, "/Mint/GananciasMint", ganancias_mint);
        //Serial.println(ganancias_mint);
      }
    }
    
     if (Firebase.getString(firebaseData, "/Gum/GananciasGum")) {
      String gananciasgum = firebaseData.stringData();
      gananciasgum.toCharArray(ggum, 13); 
      if (atof(ggum) != ganancias_gum) {
        ganancias_gum = pgum+atof(ggum);
        Firebase.setFloat(firebaseData, "/Gum/GananciasGum", ganancias_gum);
        //Serial.println(ganancias_gum);
      }
    }
    if (Firebase.getString(firebaseData, "/M&M/CantidadM&M")) {
      String cantidadmym = firebaseData.stringData();
      cantidadmym.toCharArray(cmym, 13); 
      //Serial.println(cantidadmym);
      //Serial.println(cmym);
      if (atof(cmym) != cantidad_mym) {
        pM_M=2.5;
        cantidad_mym = atof(cmym)-(pM_M/2.5);
        Firebase.setFloat(firebaseData, "/M&M/CantidadM&M", cantidad_mym);
        //Serial.println(cantidad_mym);
      }
    }
    if (Firebase.getString(firebaseData, "/Milk/CantidadMilk")) {
      String cantidadmilk = firebaseData.stringData();
      cantidadmilk.toCharArray(cmilk, 13); 
      if (atof(cmilk) != cantidad_milk) {
        cantidad_milk = atof(cmilk);-(pmilk/1);
        Firebase.setFloat(firebaseData, "/Milk/CantidadMilk", cantidad_milk);
        //Serial.println(cantidad_milk);
      }
    }
     if (Firebase.getString(firebaseData, "/Mint/CantidadMint")) {
      String cantidadmint = firebaseData.stringData();
      cantidadmint.toCharArray(cmint, 13); 
      if (atof(cmint) != cantidad_mint) {
        cantidad_mint = atof(cmint)-(pmint/1.25);
        Firebase.setFloat(firebaseData, "/Mint/CantidadMint", cantidad_mint);
        //Serial.println(cantidad_mint);
      }
    }
    
     if (Firebase.getString(firebaseData, "/Gum/CantidadGum")) {
      String cantidadgum = firebaseData.stringData();
      cantidadgum.toCharArray(cgum, 13); 
      if (atof(cgum) != cantidad_gum) {
        cantidad_gum = atof(cgum)-(pgum/2);
        Firebase.setFloat(firebaseData, "/Gum/CantidadGum", cantidad_gum);
        //Serial.println(cantidad_gum);
      }
    }
    if(usuario==1){
      pmilk=20;
        if (Firebase.getString(firebaseData, "/Users/Saldo1")) {
      String saldo1 = firebaseData.stringData();
      saldo1.toCharArray(s1, 13); 
      if (atof(s1) != saldo_1) {
        if(pM_M!=0){
          saldo_1 = atof(s1)-pM_M;
        Firebase.setFloat(firebaseData, "/Users/Saldo1", saldo_1);
        Serial.println(saldo_1);
        }if(pmilk!=0){
          saldo_1 = atof(s1)-pmilk;
        Firebase.setFloat(firebaseData, "/Users/Saldo1", saldo_1);
        Serial.println(saldo_1);
        }if(pmint!=0){
          saldo_1 = atof(s1)-pmint;
        Firebase.setFloat(firebaseData, "/Users/Saldo1", saldo_1);
        Serial.println(saldo_1);
        }if(pgum!=0){
          saldo_1 = atof(s1)-pgum;
        Firebase.setFloat(firebaseData, "/Users/Saldo1", saldo_1);
        Serial.println(saldo_1);
        }

        }
      }
    }if(usuario==2){
        if (Firebase.getString(firebaseData, "/Users/Saldo2")) {
      String saldo2 = firebaseData.stringData();
      saldo2.toCharArray(s2, 13); 
      if (atof(s2) != saldo_2) {
        if(pM_M!=0){
          saldo_2 = pM_M-atof(s2);
        Firebase.setFloat(firebaseData, "/Users/Saldo2", saldo_2);
        Serial.println(saldo_2);
        }if(pmilk!=0){
          saldo_2 = pmilk-atof(s2);
        Firebase.setFloat(firebaseData, "/Users/Saldo2", saldo_2);
        Serial.println(saldo_2);
        }if(pmint!=0){
          saldo_2 = pmint-atof(s2);
        Firebase.setFloat(firebaseData, "/Users/Saldo2", saldo_2);
        Serial.println(saldo_2);
        }if(pgum!=0){
          saldo_2 = pgum-atof(s2);
        Firebase.setFloat(firebaseData, "/Users/Saldo2", saldo_2);
        Serial.println(saldo_2);
        }

        }
      }
    }
}
   
