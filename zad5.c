#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"        // Include LCD library
#include "buttons.h"    // Include buttons library
#include "adc.h"        // Include ADC library
#define FCY             4000000UL

void __delay_us(unsigned long us)
{
    __delay32(FCY / 1000000 * us);
}

void __delay_ms(unsigned long ms)
{
    __delay32(FCY / 1000 * ms);
}


#define TOTAL_TIME (300) // w sekundach

volatile unsigned int seconds = TOTAL_TIME;

int main(void) {
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    unsigned int suwak;
    suwak = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
    unsigned char normalizedValue = suwak >> 2;
    double procent =  (normalizedValue/255.0)*100.0;
    

    TRISA = 0b010000000;
    char buffer[32];    // bufor na znaki do wyświetlenia na ekranie
    LCD_Initialize();   // inicjalizacja LCD
    unsigned int remainingSeconds1 = seconds;
    unsigned int remainingSeconds2 = seconds;
    int flag1 = 0;
    int flag2 = 0;
    unsigned char minutes1;
    unsigned char minutes2;
    unsigned char secs1;
    unsigned char secs2;

    while (remainingSeconds1 > 1 && remainingSeconds2 > 1) 
    {        
        minutes1 = remainingSeconds1 / 60;
        secs1 = remainingSeconds1 % 60;
        
        minutes2 = remainingSeconds2 / 60;
        secs2 = remainingSeconds2 % 60;
        
        sprintf(buffer, "%02u:%02u   %02u:%02u   ", minutes1, secs1, minutes2, secs2);
        LCD_ClearScreen();
        LCD_PutString(buffer, strlen(buffer));
        
        
        if (BUTTON_IsPressed(BUTTON_S3) == true) {
            flag1 = 1;
            flag2 = 0;
        } else if (BUTTON_IsPressed(BUTTON_S4) == true) {
            flag1 = 0;
            flag2 = 1;
        }
        if (flag1 == 1) {
            remainingSeconds1--;
            minutes1 = remainingSeconds1 / 60;
            secs1 = remainingSeconds1 % 60;
        }
        if (flag2 == 1) {
            remainingSeconds2--;
            minutes2 = remainingSeconds2 / 60;
            secs2 = remainingSeconds2 % 60;
        }

        if ((BUTTON_IsPressed(BUTTON_S5) == true) && (BUTTON_IsPressed(BUTTON_S3) == true)) {
            sprintf(buffer, "%02u:%02u   %02u:%02u    ff", 0, 0, 0, 0);
            LCD_ClearScreen();
            LCD_PutString(buffer, strlen(buffer));
            __delay32(4000000);
            return 0;
        }
        if ((BUTTON_IsPressed(BUTTON_S5) == true) && (BUTTON_IsPressed(BUTTON_S6) == true)) {
            sprintf(buffer, "%02u:%02u   %02u:%02u    remis", 0, 0, 0, 0);
            LCD_ClearScreen();
            LCD_PutString(buffer, strlen(buffer));
            __delay32(4000000);
            return 0;
        }
        if (BUTTON_IsPressed(BUTTON_S6) == true) {
            sprintf(buffer, "%02u:%02u   %02u:%02u    Mat", 0, 0, 0, 0);
            LCD_ClearScreen();
            LCD_PutString(buffer, strlen(buffer));
            __delay32(4000000);
            return 0;
        }
        __delay32(4000000);
    }
    sprintf(buffer, "%12s", "koniec czasu");
    LCD_ClearScreen();
    LCD_PutString(buffer, strlen(buffer));
    __delay32(4000000);
    return 0;
}
