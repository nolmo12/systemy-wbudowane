#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "buttons.h"
#include "adc.h"
#define TOTAL_TIME (300) // w sekundach
#define FCY             4000000UL

void __delay_us(unsigned long us)
{
    __delay32(FCY / 1000000 * us);
}

void __delay_ms(unsigned long ms)
{
    __delay32(FCY / 1000 * ms);
}


volatile unsigned int seconds = TOTAL_TIME;
int main(void) {
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
 // ADC ma czytać potencjometr
 ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
unsigned int suwak;
    //TRISA = 0x1111;
    char buffer[16]; // bufor na znaki do wyświetlenia na ekranie
    LCD_Initialize(); // inicjalizacja LCD
    unsigned int remainingSeconds = seconds;
    int flaga = 0;
while(remainingSeconds > 1) {
    if(remainingSeconds > 1800)
    {
        remainingSeconds = 1800;
    }
    suwak = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
    unsigned char normalizedValue = suwak >> 2;

    if (BUTTON_IsPressed ( BUTTON_S3 ) == true) {
        remainingSeconds = remainingSeconds + 30;
    }
    if ((BUTTON_IsPressed ( BUTTON_S4 ) == true)) {
        remainingSeconds = remainingSeconds - 30;
    }
    if ((BUTTON_IsPressed ( BUTTON_S5 ) == true))
    {
        if(flaga == 0){
            flaga = 1;
        }
        else{
            flaga = 0;
        }
    } 
    if ((BUTTON_IsPressed ( BUTTON_S6 ) == true)) {
        flaga = 0;
        remainingSeconds = seconds;
    }
    if(flaga == 0)
    {
        remainingSeconds--;
        
    }
    double procent =  (normalizedValue/255.0)*100.0;
    unsigned char minutes = remainingSeconds / 60;
    unsigned char secs = remainingSeconds % 60;
    sprintf(buffer, "Czas: %02u:%02u %03u", minutes, secs,(int) procent); // zapis czasu do bufora
// wyświetlenie na ekranie
    LCD_ClearScreen();
    LCD_PutString(buffer, strlen(buffer));
    __delay_ms(1000); //czekamy sekundę
 
}
    sprintf(buffer, "%12s", "koniec czasu");
    LCD_ClearScreen();
    LCD_PutString(buffer, strlen(buffer));
    __delay_ms(1000); //czekamy 10 sekund
return 0;
}
