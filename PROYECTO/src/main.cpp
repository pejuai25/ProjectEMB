#define F_Cpu 4000000UL
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define RST_PIN 9 
#define SS_PIN  10  

#include <incluidos.cpp>

uint16_t demora = 500; 
int status_dulce=0;
char status_dulce2[5];
int MyM= 0;
int Milk=1;
int mint=2;
int gum=4;
MFRC522 mfrc522(SS_PIN, RST_PIN);  
int main(void)
{ 

    PORTB |= (1<<PB0); 
    PORTB |= (1<<PB2); 
    PORTB |= (1<<PB6); 
    PORTB |= (1<<PB7); 
   PORTC |= (1<<PC0); 
   PORTC |= (1<<PC1); 
   PORTC |= (1<<PC2); 
      char M_M[100];
   char milk[100];
   char mint[100];
   char gum[100];

   HX711_init(128);
  HX711_set_scale(11500.f);
  HX711_tare(10);
   Serial.begin(9600); 
  SPI.begin();          
  mfrc522.PCD_Init();    
   DDRD=0XFF;
   PORTD=0X00;
   lcd_init(LCD_DISP_ON);
   while(1){
  if(!(PINB&(1<<PB0))){
    lcd_clrscr();
  lcd_gotoxy(0,0);
    lcd_puts("INSERTE SU TARJETA");
 MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  byte block;
  byte len;
  MFRC522::StatusCode status;
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }lcd_clrscr();
  lcd_gotoxy(0,0);
    lcd_puts("**Tarjeta Detectada**");
  _delay_ms (1000);
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
   byte buffer1[18];
   block = 4;
    len = 18;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    return;
  }
  byte buffer2[18];
  block = 1;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
   if (status != MFRC522::STATUS_OK) {
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    return;
  }
    lcd_clrscr();
  lcd_gotoxy(0,0);
    lcd_puts("**Tarjeta Detectada**");
  _delay_ms (1000); 
    mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  }
  
  float w=HX711_get_units(10);
  float pM_M=w*0.0025;
     float pmilk=w*0.001;
     float pmint=w*0.00125;
     float pgum=w*0.002;
        lcd_gotoxy(0,0);
      lcd_puts("Elija su dulce:");
          if(!(PINB &(1<<PB2))){
     lcd_clrscr();
       lcd_gotoxy(0,0);
      lcd_puts("Elija su dulce:");
       }
         if(!(PINC &(1<<PC0))){
       status_dulce=status_dulce+1;
      _delay_ms (demora);
      
   }
     if(status_dulce==0){
        if(!(PINC&(1<<PC1))){
            lcd_clrscr();
         lcd_gotoxy(0,0);
         lcd_puts("M&M: $2.5xKg");
     _delay_ms (10000);
    lcd_clrscr();
           lcd_gotoxy(0,0);
           lcd_puts("Debe pagar por M&M");
       _delay_ms (10000);
     dtostrf(pM_M, 5, 1, M_M);
            lcd_clrscr();
           lcd_gotoxy(0,0);
           lcd_puts(M_M);
          _delay_ms (10000);
          }

         }if(status_dulce==1){
    if(!(PINC&(1<<PC1))){
         lcd_clrscr();
         lcd_gotoxy(0,0);
         lcd_puts("Milk: $1xKg");
     _delay_ms (10000);
      lcd_clrscr();
           lcd_gotoxy(0,0);
           lcd_puts("Deber pagar por Milk");
       _delay_ms (10000);
     dtostrf(pmilk, 5, 1, milk);
            lcd_clrscr();
           lcd_gotoxy(0,0);
           lcd_puts(milk);
          _delay_ms (10000);
       
       } 
               
            

}if(status_dulce==2){
    if(!(PINC&(1<<PC1))){
         lcd_clrscr();
         lcd_gotoxy(0,0);
         lcd_puts("Mint: $1.25xKg");
     _delay_ms (demora);
     lcd_clrscr();
           lcd_gotoxy(0,0);
           lcd_puts("Deber pagar por Mint");
       _delay_ms (10000);
     dtostrf(pmint, 5, 1, mint);
            lcd_clrscr();
           lcd_gotoxy(0,0);
           lcd_puts(mint);
          _delay_ms (10000);
       }
                
            

}if(status_dulce==3){
    if(!(PINC&(1<<PC1))){
         lcd_clrscr();
         lcd_gotoxy(0,0);
         lcd_puts("Gum: $2xKg");
     _delay_ms (demora);
     lcd_clrscr();
           lcd_gotoxy(0,0);
           lcd_puts("Deber pagar por Milk");
       _delay_ms (10000);
     dtostrf(pgum, 5, 1, gum);
            lcd_clrscr();
           lcd_gotoxy(0,0);
           lcd_puts(gum);
       _delay_ms (10000);
       }
                
            
}if(status_dulce==4){
   status_dulce=0;
}
       
 
  }
   }

