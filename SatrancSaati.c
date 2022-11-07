// P1 = A2
// p2 = A3
// S  = B1 
// +  = B2
// -  = B3
// Bz = A1
#include <16F628A.h>
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

int8 salise  =0, saniye  =0, dakika  =5, 
     salise2 =0, saniye2 =0, dakika2 =5,
     ayr=0,butonsure=0,arttirma=0,pause=0;
int1 uzunbuton=0,kisabuton=0,start=0,start2=0;

void geri(){
//////========== G E R Ý  S A Y I M  F O N K S. ===============================
  if(salise==255)         { saniye--; salise=9; }
  else if(saniye==255)    { dakika--; saniye=59; }
  if(salise2==255)        { saniye2--; salise2=9; }
  else if(saniye2==255)   { dakika2--; saniye2=59; }  
}

void ayar(){
//////========== A Y A R  B U T O N U  F O N K S. =============================
if (ayr == 1) { lcd_gotoxy(7,2); lcd_putc("<");}
if (ayr == 1 && input(S_arti)) { dakika++; while(input(S_arti));  }
if (ayr == 1 && input(S_eksi)) { dakika--; while(input(S_eksi));  }

if (ayr == 2) {lcd_gotoxy(6,2); lcd_gotoxy(6,2); lcd_putc("< ");}
if (ayr == 2 && input(S_arti)) { saniye++; while(input(S_arti));  }
if (ayr == 2 && input(S_eksi)) { saniye--; while(input(S_eksi));  }

if (ayr == 3) {lcd_gotoxy(6,2); lcd_putc("  "); lcd_gotoxy(10,2); lcd_putc("> ");}
if (ayr == 3 && input(S_arti)) { dakika2++; while(input(S_arti));  }
if (ayr == 3 && input(S_eksi)) { dakika2--; while(input(S_eksi));  }

if (ayr == 4) {lcd_putc("  "); lcd_gotoxy(10,2); lcd_putc(" >");}
if (ayr == 4 && input(S_arti)) { saniye2++; while(input(S_arti));  }
if (ayr == 4 && input(S_eksi)) { saniye2--; while(input(S_eksi));  }

if (ayr == 5) {lcd_gotoxy(10,2); lcd_putc("  "); lcd_gotoxy(8,1); printf(lcd_putc,"%02d",arttirma);}
if (ayr == 5 && input(S_arti)) { arttirma++; while(input(S_arti));  }
if (ayr == 5 && input(S_eksi)) { arttirma--; while(input(S_eksi));  }

if (ayr==6) {lcd_gotoxy(8,1); lcd_putc("  "); ayr=0;}
}

void reset(){
//////========== R E S E T ====================================================
start=0, start2=0,
saniye=0,dakika=5,
saniye2=0,dakika2=5,
arttirma=0,ayr=0;
lcd_gotoxy(8,1); lcd_putc("  ");
}

void uzunkisa_buton (int inp, int minsure ){
//////========== U Z U N - K I S A  B U T O N  F O N K S. =====================
// int1 uzunbuton = 0, kisabuton = 0;   Baþa deðiþken olarak tanýmlanmalý
// int butonsure = 0; */                Baþa deðiþken olarak tanýmlanmalý        
    if (butonsure >=1 && butonsure<=10 && input(inp)==0 ) {  kisabuton=1; }   // butondan çekildikten sonra
    if (!input(inp)) {butonsure=0;}  
    
    if(input(inp)) { butonsure++; 
                     if (butonsure >= minsure) {uzunbuton=1; while(input(inp)); }
                       //lcd_gotoxy(8,1); printf(lcd_putc,"%02d",butonsure);  // sadece buton süresini gözlemlemek için  
                       delay_ms(10); }
                       
//////========== O Y U N A  B A Þ L A M A  F O N K S. =========================
if (uzunbuton == 1 && dakika >=1 && dakika2>=1 && start==0 && start2==0) {   ayr=0;
                        lcd_gotoxy(8,1); printf(lcd_putc,"%02d",arttirma); 
                        lcd_gotoxy(6,2);  lcd_putc("  "); 
                        lcd_gotoxy(10,2); lcd_putc("  ");
                        start=1;                      
                        output_high(Bz); delay_ms(15); output_low(Bz); butonsure=0; uzunbuton=0; }

//////========== P A U S E  B U T O N =========================================
if (kisabuton == 1 && start==1) { start=0; pause=1; output_high(Bz); delay_ms(8); output_low(Bz); butonsure=0; kisabuton=0; } 
if (kisabuton == 1 && start2==1) { start2=0; pause=2; output_high(Bz); delay_ms(8); output_low(Bz); butonsure=0; kisabuton=0; } 
if(kisabuton==1 && pause==1) { start=1; pause=0; output_high(Bz); delay_ms(8); output_low(Bz); butonsure=0; kisabuton=0; }
if(kisabuton==1 && pause==2) { start2=1; pause=0; output_high(Bz); delay_ms(8); output_low(Bz); butonsure=0; kisabuton=0; }

//////========== R E S E T  B U T O N =========================================
if (uzunbuton == 1 && (start==1 || start2==1)) { reset();  output_high(Bz); delay_ms(10); output_low(Bz); butonsure=0; uzunbuton=0; }

//////========== S Ü R E  A Y A R  B U T O N ==================================
if (kisabuton == 1 && start==0 && start2==0)   { ayr++; butonsure=0; kisabuton=0; }
}

void buzzer(int in, int s_buzzer)
{
//////========== B U Z Z E R  U Y A R I =======================================
output_high(in); delay_ms(s_buzzer); output_low(in); delay_ms(50);
output_high(in); delay_ms(s_buzzer); output_low(in); delay_ms(50); 
output_high(in); delay_ms(s_buzzer); output_low(in); delay_ms(50); 
output_high(in); delay_ms(100); output_low(in);      delay_ms(50);
}

void main() {
lcd_init(0x4e,16,2);
lcd_clear();
//set_tris_b(0xFF);
set_tris_a(0xFD);

//////========== I N T R O - A N Ý. ===========================================
     int s = 5;
     for (int ani=0;ani<17;ani++) { lcd_gotoxy(ani,1); lcd_putc(">>"); delay_ms(s);}
     for ( ani=17;ani>0;ani--) { lcd_gotoxy(ani,2); lcd_putc("<<"); delay_ms(s);}
     for ( ani=0;ani<17;ani++) { lcd_gotoxy(ani,1); lcd_putc("  "); delay_ms(s);}
     for ( ani=17;ani>0;ani--) { lcd_gotoxy(ani,2); lcd_putc("  "); delay_ms(s);}
      buzzer(Bz,25);
     lcd_gotoxy(1,1); lcd_putc("BEYAZ");   lcd_gotoxy(12,1); lcd_putc("SIYAH");    
     lcd_gotoxy(8,2); lcd_putc("||");
     
   while(TRUE)
   {
    uzunkisa_buton(Set, 17);
    ayar();
    geri();
//////========== B E Y A Z - S Ý Y A H  B U T O N =============================
    if(input(P1) && start==1)  { start=0;  start2=1;
                                     if(saniye+arttirma>=60)  { saniye=saniye+arttirma-60;   dakika++; }  
                                     else { saniye=saniye+arttirma; }                                     
                                     }
    if(input(P2) && start2==1) { start2=0; start=1;  
                                     if(saniye2+arttirma>=60) { saniye2=saniye2+arttirma-60; dakika2++;}  
                                     else { saniye2=saniye2+arttirma; }                                     
                                     }
    if (start==1)  { salise--; delay_ms(75);  }
    if (start2==1) { salise2--; delay_ms(75); }
    
//////========== L C D ======================================================== 
          lcd_gotoxy(1,2); printf(lcd_putc,"%02d:%02d",dakika,saniye);
          lcd_gotoxy(12,2); printf(lcd_putc,"%02d:%02d",dakika2,saniye2);
/* if (dakika==0 && saniye<10  && start==1 )  {lcd_gotoxy(1,1);  lcd_putc("B=   "); lcd_gotoxy(1,1);  printf(lcd_putc,"%02d",salise);  }
   if (dakika2==0 && saniye2<10 && start2==1)  {lcd_gotoxy(12,1); lcd_putc("S=   "); lcd_gotoxy(15,1); printf(lcd_putc,"%02d",salise2); }  */ 
   
//////========== S I N I R L A N D I R M A L A R ============================== 
    if(saniye==60) saniye=0;
    if(dakika==60) dakika=0; 
    if(saniye2==60) saniye2=0;
    if(dakika2==60) dakika2=0;
    if(arttirma==60) arttirma=0;
       
    if(input(Set)==1 &&  dakika<1)    { dakika=0; while(input(Set));}
    if(input(Set)==1 &&  dakika2<1)   { dakika2=0; while(input(Set));}
    if(input(Set)==1 &&  arttirma<1)  { arttirma=0; while(input(Set));}
    
    if (start==1  && dakika==0  && saniye==0  && salise==0)    { buzzer(Bz,25); start=0;  dakika=0;  saniye=0;  salise=0;  }
    if (start2==1 && dakika2==0 && saniye2==0 && salise2==0)   { buzzer(Bz,25); start2=0; dakika2=0; saniye2=0; salise2=0; }
   }
}