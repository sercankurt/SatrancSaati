// P1 = A2
// p2 = A3
// S  = B1 
// +  = B2
// -  = B3
// Bz = A1
// LCD -> SDA = B5
// LCD -> SCL = B4
#include <16F648A.h>
#fuses INTRC_IO, NOWDT, NOMCLR, NOPROTECT
#use delay(internal=4M)

#use i2c (Master, sda=PIN_B5, scl=PIN_B4, force_sw)
#include "i2c_flex_lcd.c"

#define      P1           pin_a2
#define      P2           pin_a3
#define      Set          pin_b1
#define      S_arti       pin_b2
#define      S_eksi       pin_b3
#define      Bz           pin_a1

#use fast_io(a)
#use fast_io(b)

unsigned int8 salise  =0, saniye  =0, dakika  =0, 
              salise2 =0, saniye2 =0, dakika2 =0,
              ayr=0,butonsure=0,arttirma=0,pause=0,
              SL;
int1 uzunbuton=0,kisabuton=0,start=0,start2=0,bz_d,TRF;

void buzzer(int in, int s_buzzer, int j_buzzer)
{
//////========== B U Z Z E R  U Y A R I =======================================
if(bz_d==1){
output_high(in); delay_ms(s_buzzer); output_low(in); delay_ms(50);
output_high(in); delay_ms(s_buzzer); output_low(in); delay_ms(50); 
output_high(in); delay_ms(s_buzzer); output_low(in); delay_ms(50); 
output_high(in); delay_ms(j_buzzer); output_low(in); delay_ms(50); }
}

void kisa_buzzer(int bz_sure)
{
//////========== M E N U  B U Z Z E R  U Y A R I ==============================
if(bz_d==1){
output_high(Bz); delay_ms(bz_sure); output_low(Bz); }
}

void geri(){
//////========== G E R İ  S A Y I M  F O N K S. ===============================
  if(salise==255)         { saniye--; salise=9; }
  else if(saniye==255)    { dakika--; saniye=59; }
  if(salise2==255)        { saniye2--; salise2=9; }
  else if(saniye2==255)   { dakika2--; saniye2=59; }  
}

void ayar(){
//////========== A Y A R  B U T O N U  F O N K S. =============================
switch (ayr) {

case 1: lcd_gotoxy(7,1); printf(lcd_putc,"%d+%d ",dakika,arttirma);
arttirma=0;
dakika=1; dakika2=1;
saniye=0; saniye2=0;
break;

case 2: lcd_gotoxy(7,1); printf(lcd_putc,"%d+%d",dakika,arttirma);
arttirma=1;
dakika=2; dakika2=2;
saniye=0; saniye2=0;
break;

case 3: lcd_gotoxy(7,1); printf(lcd_putc,"%d+%d",dakika,arttirma);
arttirma=0;
dakika=3; dakika2=3;
saniye=0; saniye2=0; 
break;

case 4: lcd_gotoxy(7,1); printf(lcd_putc,"%d+%d",dakika,arttirma);
arttirma=2;
dakika=3; dakika2=3;
saniye=0; saniye2=0; 
break;

case 5: lcd_gotoxy(7,1); printf(lcd_putc,"%d+%d",dakika,arttirma);
arttirma=0;
dakika=5; dakika2=5;
saniye=0; saniye2=0; 
break;

case 6: lcd_gotoxy(7,1); printf(lcd_putc,"%d+%d",dakika,arttirma);
arttirma=3;
dakika=5; dakika2=5;
saniye=0; saniye2=0; 
break;

case 7: lcd_gotoxy(7,1); printf(lcd_putc,"%d+%d",dakika,arttirma);
arttirma=0;
dakika=10; dakika2=10;
saniye=0; saniye2=0; 
break;


case 8: lcd_gotoxy(7,1); printf(lcd_putc,"%d+%d",dakika,arttirma);
arttirma=10;
dakika=15; dakika2=15;
saniye=0; saniye2=0; 
break;}

if (ayr == 9) { lcd_gotoxy(7,1); arttirma=0; lcd_putc("     "); lcd_gotoxy(7,2); lcd_putc("<");}
if (ayr == 9 && input(S_arti)) { dakika++; while(input(S_arti));  }
if (ayr == 9 && input(S_eksi)) { dakika--; while(input(S_eksi));  }

if (ayr == 10) {lcd_gotoxy(6,2); lcd_gotoxy(6,2); lcd_putc("< ");}
if (ayr == 10 && input(S_arti)) { saniye++; while(input(S_arti));  }
if (ayr == 10 && input(S_eksi)) { saniye--; while(input(S_eksi));  }

if (ayr == 11) {lcd_gotoxy(6,2); lcd_putc("  "); lcd_gotoxy(10,2); lcd_putc("> ");}
if (ayr == 11 && input(S_arti)) { dakika2++; while(input(S_arti));  }
if (ayr == 11 && input(S_eksi)) { dakika2--; while(input(S_eksi));  }

if (ayr == 12) {lcd_putc("  "); lcd_gotoxy(10,2); lcd_putc(" >");}
if (ayr == 12 && input(S_arti)) { saniye2++; while(input(S_arti));  }
if (ayr == 12 && input(S_eksi)) { saniye2--; while(input(S_eksi));  }

if (ayr == 13) {lcd_gotoxy(10,2); lcd_putc("  "); lcd_gotoxy(8,1); printf(lcd_putc,"+%d ",arttirma);}
if (ayr == 13 && input(S_arti)) { arttirma++; while(input(S_arti));  }
if (ayr == 13 && input(S_eksi)) { arttirma--; while(input(S_eksi));  }

if (ayr == 14) {lcd_gotoxy(7,1); printf(lcd_putc,"Bz:%d ",bz_d);}
if (ayr == 14 && input(S_arti)) { bz_d++; write_eeprom(0,bz_d); buzzer(Bz,15,20); while(input(S_arti));  }
if (ayr == 14 && input(S_eksi)) { bz_d--; write_eeprom(0,bz_d); while(input(S_eksi));  }

if (ayr == 15) {lcd_gotoxy(7,1); printf(lcd_putc,"-s:%02d",SL);}
if (ayr == 15 && input(S_arti)) { SL++; write_eeprom(1,SL); while(input(S_arti));  }
if (ayr == 15 && input(S_eksi)) { SL--; write_eeprom(1,SL); while(input(S_eksi));  }


if (ayr==16) {lcd_gotoxy(7,1); lcd_putc("     "); ayr=1;}

}

void reset(){
//////========== R E S E T ====================================================
start=0,start2=0,
saniye=0,dakika=5,
saniye2=0,dakika2=5,
arttirma=0,ayr=0;
lcd_gotoxy(7,1); lcd_putc("     ");
}

void uzunkisa_buton (int inp, int minsure ){
//////========== U Z U N - K I S A  B U T O N  F O N K S. =====================
// int1 uzunbuton = 0, kisabuton = 0;   Başa değişken olarak tanımlanmalı
// int butonsure = 0; */                Başa değişken olarak tanımlanmalı        
    if (butonsure >=1 && butonsure<=10 && input(inp)==0 ) {  kisabuton=1; }   // butondan çekildikten sonra
    if (!input(inp)) {butonsure=0;}  
    
    if(input(inp)) { butonsure++; 
                     if (butonsure >= minsure) {kisa_buzzer(100); uzunbuton=1; while(input(inp)); }
                       //lcd_gotoxy(8,1); printf(lcd_putc,"%02d",butonsure);  // sadece buton süresini gözlemlemek için  
                       delay_us(10); }
                       
//////========== O Y U N A  B A Ş L A M A  F O N K S. =========================
if (uzunbuton == 1 && start==0 && start2==0) {   ayr=0;
                        lcd_gotoxy(7,1); printf(lcd_putc," +%d ",arttirma); 
                        lcd_gotoxy(6,2);  lcd_putc("  "); 
                        lcd_gotoxy(10,2); lcd_putc("  ");
                        if (TRF==0) start=1;
                        if (TRF==1) start2=1;
                        kisa_buzzer(400); butonsure=0; uzunbuton=0; }

//////========== P A U S E  B U T O N =========================================
if (kisabuton == 1 && start==1)  { start=0; pause=1; kisa_buzzer(25); butonsure=0; kisabuton=0; }                                
if (kisabuton == 1 && start2==1) { start2=0; pause=2; kisa_buzzer(25); butonsure=0; kisabuton=0; }
                                  
if(kisabuton==1 && pause==1) { start=1; pause=0; kisa_buzzer(25); butonsure=0; kisabuton=0;  }                                   
if(kisabuton==1 && pause==2) { start2=1; pause=0; kisa_buzzer(25); butonsure=0; kisabuton=0; }

if(pause==1) { lcd_gotoxy(6,2);  lcd_putc("!"); delay_ms(250); 
               lcd_gotoxy(6,2);  lcd_putc(" "); delay_ms(100); }  
if(pause==2) { lcd_gotoxy(11,2); lcd_putc("!"); delay_ms(250); 
               lcd_gotoxy(11,2); lcd_putc(" "); delay_ms(100); }  

//////========== R E S E T  B U T O N =========================================
if (uzunbuton == 1) 
{ reset();  kisa_buzzer(25); butonsure=0; uzunbuton=0; }

//////========== S Ü R E  A Y A R  B U T O N ==================================
if (kisabuton == 1 && start==0 && start2==0)   
{ ayr++; kisa_buzzer(15); butonsure=0; kisabuton=0; }
}

void main() {
lcd_init(0x4e,16,2);
lcd_clear();
set_tris_a(0xFD);
bz_d = read_eeprom(0);
SL   = read_eeprom(1);
output_low(pin_a1);
//////========== I N T R O - A N İ. ===========================================
     int s = 5;
     for (int ani=0;ani<17;ani++) { lcd_gotoxy(ani,1); lcd_putc(">>"); delay_ms(s);}
     for ( ani=17;ani>0;ani--) { lcd_gotoxy(ani,2); lcd_putc("<<"); delay_ms(s);}
     for ( ani=0;ani<17;ani++) { lcd_gotoxy(ani,1); lcd_putc("  "); delay_ms(s);}
     for ( ani=17;ani>0;ani--) { lcd_gotoxy(ani,2); lcd_putc("  "); delay_ms(s);}
      buzzer(Bz,25,25);
     lcd_gotoxy(1,1); lcd_putc("^^o^^");   lcd_gotoxy(12,1); lcd_putc("^^o^^");    
     lcd_gotoxy(8,2); lcd_putc("||");
     
   while(TRUE)
   {
    uzunkisa_buton(Set, 17);
    ayar();
    geri();
//////========== B E Y A Z - S İ Y A H  T A R A F  B E L İ R L E M E ==========    
    if(input(P2) && start==0 && start2==0 && pause==0) {TRF=1; lcd_gotoxy(1,1); lcd_putc("SIYAH"); lcd_gotoxy(12,1); lcd_putc("BEYAZ"); delay_ms(10); }
    if(input(P1) && start==0 && start2==0 && pause==0) {TRF=0; lcd_gotoxy(1,1); lcd_putc("BEYAZ"); lcd_gotoxy(12,1); lcd_putc("SIYAH"); delay_ms(10); }  
//////========== B E Y A Z - S İ Y A H  B U T O N =============================
    if(input(P2) && start==1)  { start=0;  start2=1;
                                     if(saniye+arttirma>=60)  { saniye=saniye+arttirma-60;   dakika++; }  
                                     else { saniye=saniye+arttirma; }                                     
                                     }
    if(input(P1) && start2==1) { start2=0; start=1;  
                                     if(saniye2+arttirma>=60) { saniye2=saniye2+arttirma-60; dakika2++;}  
                                     else { saniye2=saniye2+arttirma; }                                     
                                     }
    if (start==1)  { salise--; delay_ms(75);  }
    if (start2==1) { salise2--; delay_ms(75); }
//////========== L C D ======================================================== 
     lcd_gotoxy(1,2);  printf(lcd_putc,"%02d:%02d",dakika,saniye);
     lcd_gotoxy(12,2); printf(lcd_putc,"%02d:%02d",dakika2,saniye2);
//LCD=== s a l i s e ===LCD//
     if( (start==1  || start2==1) && dakika==0  && saniye<SL)  {lcd_gotoxy(1,2);  printf(lcd_putc,"%02d:0%d ",saniye,salise);  }
     if( (start2==1 || start==1)  && dakika2==0 && saniye2<SL) {lcd_gotoxy(12,2); printf(lcd_putc,"%02d:0%d",saniye2,salise2); }
//////========== S I N I R L A N D I R M A L A R ============================== 
    if(saniye==60)   saniye=0;
    if(dakika==60)   dakika=0; 
    if(saniye2==60)  saniye2=0;
    if(dakika2==60)  dakika2=0;
    if(arttirma==60) arttirma=0;
    if(SL==60)       SL=0;  
    
    if(input(S_eksi)==1 &&  dakika<1)     { dakika=0;   while(input(S_eksi)); }
    if(input(S_eksi)==1 &&  dakika2<1)    { dakika2=0;  while(input(S_eksi)); }
    if(input(S_eksi)==1 &&  arttirma==-1) { arttirma=0; while(input(S_eksi)); }
    if(input(S_eksi)==1 &&  SL==-1)       { SL=0;       while(input(S_eksi)); }
     
    if (start==1  && dakika==0  && saniye==0  && salise==0)    { buzzer(Bz,25,250); start=0;  dakika=0;  saniye=0;  salise=0;  }
    if (start2==1 && dakika2==0 && saniye2==0 && salise2==0)   { buzzer(Bz,25,250); start2=0; dakika2=0; saniye2=0; salise2=0; }
   }
}
