// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRIPLL           // Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL))
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)
#include <xc.h>
#include <libpic30.h>
#include "buttons.h"
#include "adc.h"

#define FCY             4000000UL

void __delay_us(unsigned long us)
{
    __delay32(FCY / 1000000 * us);
}

void __delay_ms(unsigned long ms)
{
    __delay32(FCY / 1000 * ms);
}

int main(void) {
 // Domy?lna konfiguracja ADC
 ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
 // ADC ma czyta? potencjometr
 ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
 // output LEDów PORT A
 TRISA = 0x0000;
 unsigned int value;
 unsigned int time;
 unsigned int flag;
 while (1)
 {
 // Czytanie 10-bit warto?ci z potencjometru
 value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
 // B??d?
 if (value == 0xFFFF) {
 // W razie czego przechodzimy do kolejnej iteracji
 continue;
 }
 // normalizacja zakresu do 0-255
 unsigned char normalizedValue = value >> 2;
 // Output na port A (LEDy)
 if(normalizedValue >=128 || flag == 1){
     if(flag != 1){
         time = 15;
         while(time != 0)
         {
            LATA = 0b00000001;
            __delay_ms(500); 
            LATA = 0b00000000;
            __delay_ms(500);
            time--;
         }
     flag = 1;
     }
     else{
     __delay32(1500000); 
     LATA = 0b11111111;
     __delay32(1500000); 
     LATA = 0b11111111;    
     }
     
 }
 else{
     LATA = 0b10000000;
 }
 if(BUTTON_IsPressed ( BUTTON_S3 ) == true){
     flag = 0;
     __delay_us(2000);
     LATA = 0b00000000;
     __delay_ms(5000);
 }
 }
 return 0;
}