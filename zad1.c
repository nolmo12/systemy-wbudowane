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

void __delay_us(unsigned long us)
{
    __delay32(FCY / 1000000 * us);
}

void __delay_ms(unsigned long ms)
{
    __delay32(FCY / 1000 * ms);
}

void binaryCounterUp(unsigned int *counter)
{
    unsigned portValue = 0;
    char currentS6 = 0, currentS7 = 1, currentS4 = 0, prevS6 = 0, prevS7 = 1, prevS4 = 0;
    while(1)
    {
        LATA = portValue;
        prevS4 = PORTDbits.RD13;
        prevS6 = PORTDbits.RD6;
        prevS7 = PORTDbits.RD7;
        __delay32(150000);
        currentS4 = PORTDbits.RD13;
        currentS6 = PORTDbits.RD6;
        currentS7 = PORTDbits.RD7;
        
        portValue++;
        __delay_ms(100);
            
        if(currentS4 - prevS4 == 1)
        {
            (*counter)++;
            break;
        }
    }
}

void binaryCounterDown(unsigned int *counter)
{
    unsigned portValue = 255;
    char currentS6 = 0, currentS7 = 1, currentS4 = 0, prevS6 = 0, prevS7 = 1, prevS4 = 0;
    while(1)
    {
        LATA = portValue;
        prevS4 = PORTDbits.RD13;
        __delay32(150000);
        currentS4 = PORTDbits.RD13;
        portValue--;
        __delay_ms(100);
        
        if(currentS4 - prevS4 == 1)
        {
            (*counter)++;
            break;
        }
    }
}

unsigned int getGrayCode(unsigned int number)
{
    return (number ^ (number >> 1));
}

void grayCounterUp(unsigned int *counter)
{
    unsigned int portValue = 0;
    unsigned int grayCode = 0;
    char currentS6 = 0, currentS7 = 1, currentS4 = 0, prevS6 = 0, prevS7 = 1, prevS4 = 0;

    while(1)
    {
        grayCode = getGrayCode(portValue);
        LATA = grayCode;
        prevS4 = PORTDbits.RD13;
        __delay32(150000);
        currentS4 = PORTDbits.RD13;


        portValue++;
        
        __delay_ms(100);

        if(currentS4 - prevS4 == 1) 
        {
            (*counter)++;
            break;
        }
    }
}

void grayCounterDown(unsigned int *counter)
{
    unsigned int portValue = 255;
    unsigned int grayCode = 0;
    char currentS6 = 0, currentS7 = 1, currentS4 = 0, prevS6 = 0, prevS7 = 1, prevS4 = 0;

    while(1)
    {
        grayCode = getGrayCode(portValue);
        LATA = grayCode;
        prevS4 = PORTDbits.RD13;
        __delay32(150000);
        currentS4 = PORTDbits.RD13;


        portValue--;
        
        __delay_ms(100);

        if(currentS4 - prevS4 == 1) 
        {
            (*counter)++;
            break;
        }
    }
}

void decimalToBCD(unsigned int decimal, unsigned int *bcd) 
{
    bcd[0] = decimal / 10;
    bcd[1] = decimal % 10;
}

void bcdCounterUp(unsigned int *counter) 
{
    unsigned int portValue = 0;
    unsigned int bcd[2];

    while(1)
    {
        decimalToBCD(portValue, bcd);
        

        LATA = (bcd[0] << 4) | bcd[1];
        
        char currentS4 = PORTDbits.RD13;

        __delay32(15000);
        
        char newS4 = PORTDbits.RD13;

        portValue++;
        __delay_ms(100);
        
        if(newS4 - currentS4 == 1) 
        {
            (*counter)++;
            break;
        }
    }
}

void bcdCounterDown(unsigned int *counter) 
{
    unsigned int portValue = 255;
    unsigned int bcd[2];

    while(1)
    {
        decimalToBCD(portValue, bcd);
        

        LATA = (bcd[0] << 4) | bcd[1];
        
        char currentS4 = PORTDbits.RD13;

        __delay32(15000);

        char newS4 = PORTDbits.RD13;

        portValue--;
        
        if(newS4 - currentS4 == 1) 
        {
            (*counter)++;
            break;
        }
        __delay_ms(100);
    }
}

void snake(unsigned int *counter)
{
    unsigned int portValue = 7;
    int goingRight = 1;

    while(1)
    {
        LATA = portValue;
        
        char currentS4 = PORTDbits.RD13;

        __delay32(15000);

        char newS4 = PORTDbits.RD13;

        
        if(goingRight == 1)
        {
            if(portValue << 1 > 192)
                goingRight = 0;
            portValue = portValue << 1;
        }
        else
        {
            if(portValue >> 1 < 8)
                goingRight = 1;
            portValue = portValue >> 1;
        }
        
        
        if(newS4 - currentS4 == 1) 
        {
            (*counter)++;
            break;
        }
        __delay_ms(100);
    }
}

void queue(unsigned int *counter)
{
unsigned int portValue = 7;
    int goingRight = 1;

    while(1)
    {
        LATA = portValue;
        
        char currentS4 = PORTDbits.RD13;

        __delay32(15000);

        char newS4 = PORTDbits.RD13;

        
        if(goingRight == 1)
        {
            if(portValue << 1 > 192)
                goingRight = 0;
            portValue = portValue << 1;
        }
        else
        {
            if(portValue >> 1 < 8)
                goingRight = 1;
            portValue = portValue >> 1;
        }
        
        
        if(newS4 - currentS4 == 1) 
        {
            (*counter)++;
            break;
        }
        __delay_ms(100);
    }
}


void randomNumberGenerator(unsigned int *counter)
{
    unsigned int portValue = 7;
    int goingRight = 1;

    while(1)
    {
        LATA = portValue;
        
        char currentS4 = PORTDbits.RD13;

        __delay32(15000);

        char newS4 = PORTDbits.RD13;

        
        if(goingRight == 1)
        {
            if(portValue << 1 > 192)
                goingRight = 0;
            portValue = portValue << 1;
        }
        else
        {
            if(portValue >> 1 < 8)
                goingRight = 1;
            portValue = portValue >> 1;
        }
        
        
        if(newS4 - currentS4 == 1) 
        {
            (*counter)++;
            break;
        }
        __delay_ms(100);
    } 
}


int main(void)
{
    char currentS4 = 0, prevS4 = 0;
    
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    
    unsigned counter = 0;
    
    while(1)
    {
        LATA = 0;
        prevS4 = PORTDbits.RD13;
        __delay32(150000);
        currentS4 = PORTDbits.RD13;
        
        if(currentS4 - prevS4 == 1)
        {
            counter++;
        }
        
        switch(counter % 9)
        {
            case 0:
                binaryCounterUp(&counter);
                break;
            case 1:
                binaryCounterDown(&counter);
                break;
            case 2:
                grayCounterUp(&counter);
                break;
            case 3:
                grayCounterDown(&counter);
                break;
            case 4:
                bcdCounterUp(&counter);
                break;
            case 5:
                bcdCounterDown(&counter);
                break;
            case 6:
                snake(&counter);
                break;
            case 7:
                queue(&counter);
                break;
            case 8:
                randomNumberGenerator(&counter);
                break;
            default:
                break;
        }
        
    }
    
    return 0;
}
