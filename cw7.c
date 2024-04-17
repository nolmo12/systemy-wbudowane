/*
 * File:   newmainXC16.c
 * Author: local
 *
 * Created on April 10, 2024, 3:20 PM
 */
#pragma config POSCMOD = NONE             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRC           // Oscillator Select (Primary Oscillator with PLL module (HSPLL, ECPLL))
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF  

#include "xc.h"
#include <libpic30.h>

#define FCY             4000000UL
#define LCD_E           LATDbits.LATD4
#define LCD_RW          LATDbits.LATD5
#define LCD_RS          LATBbits.LATB15
#define LCD_DATA        LATE

#define LCD_CLEAR       0x01    //0b00000001
#define LCD_HOME        0x02    //0b00000010
#define LCD_ON          0x0C    //0b00001100
#define LCD_OFF         0x08    //0b00001000
#define LCD_CONFIG      0x38    //0b00111000
#define LCD_CURSOR      0x80    //0b10000000
#define LINE1           0x00
#define LINE2           0x40
#define LCD_CUST_CHAR   0x40    //0b01000000
#define LCD_SHIFT_R     0x1D    //0b00011100
#define LCD_SHIFT_L     0x1B    //0b00011011

void __delay_us(unsigned long us)
{
    __delay32(FCY / 1000000 * us);
}

void __delay_ms(unsigned long ms)
{
    __delay32(FCY / 1000 * ms);
}

void LCD_sendCommand(unsigned char command)
{
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_E = 1;
    LCD_DATA = command;
    __delay_us(50);
    LCD_E = 0;
}

void LCD_sendData(unsigned char data)
{
    LCD_RW = 0;
    LCD_RS = 1;
    LCD_E = 1;
    LCD_DATA = data;
    __delay_us(50);
    LCD_E = 0;
}

void LCD_print(char * string)
{
    while(*string)
    {
        LCD_sendData(*string++);
    }
}

void LCD_setCursor(unsigned char row, unsigned char col)
{
    unsigned char address;
    
    if(row == 1)
    {
        address = LCD_CURSOR + LINE1;
    }
    if(row == 2)
    {
        address = LCD_CURSOR + LINE2 + col;
    }
    
    LCD_sendCommand(address);
}

void LCD_init()
{
    __delay_ms(20);
    LCD_sendCommand(LCD_CONFIG);
    __delay_us(50);
    LCD_sendCommand(LCD_ON);
    __delay_us(50);
    LCD_sendCommand(LCD_CLEAR);
    __delay_ms(2);
    
}

void LCD_saveCustomChar(unsigned char slot, unsigned char *array)
{
    unsigned char i;
    LCD_sendCommand(LCD_CUST_CHAR + slot * 8);
    
    for(i = 0; i < 8; i++)
    {
        LCD_sendData(array[i]);
    }
}

unsigned char symbol1[8] = {
    0b00000,
    0b00000,
    0b11111,
    0b10000,
    0b11000,
    0b11100,
    0b01110,
    0b00111
};

int main(void)
{
    TRISB = 0x7FFF;
    TRISD = 0x0000;
    TRISE = 0x0000;
    
    LCD_init();
    LCD_saveCustomChar(0, symbol1);
    LCD_setCursor(2, 0);
    LCD_print("Hello world!");
    LCD_sendData(0);
    for(;;)
    {
        __delay_ms(500);
        LCD_sendCommand(LCD_SHIFT_L);
        __delay_ms(500);
    }

    return 0;
}
