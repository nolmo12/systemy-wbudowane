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

int isButtonPressed(int button) {
    // Simple debouncing mechanism
    if (button == 1 && PORTDbits.RD13 == 1) {
        __delay_ms(20); // Debounce delay
        if (PORTDbits.RD13 == 1) {
            while (PORTDbits.RD13 == 1); // Wait for button release
            return 1;
        }
    } else if (button == 2 && PORTDbits.RD14 == 1) {
        __delay_ms(20); // Debounce delay
        if (PORTDbits.RD14 == 1) {
            while (PORTDbits.RD14 == 1); // Wait for button release
            return 1;
        }
    }
    return 0;
}

void binaryCounterUp(unsigned int *counter)
{
    unsigned portValue = 0;

    while(1)
    {
        LATA = portValue;
        portValue++;
        __delay_ms(100);
        
        if (isButtonPressed(1)) {
            (*counter)++;
            break;
        }
    }
}

void binaryCounterDown(unsigned int *counter)
{
    unsigned portValue = 255;

    while(1)
    {
        LATA = portValue;
        portValue--;
        __delay_ms(100);
        
        if (isButtonPressed(1)) {
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

    while(1)
    {
        LATA = getGrayCode(portValue);
        portValue++;
        __delay_ms(100);

        if (isButtonPressed(1)) {
            (*counter)++;
            break;
        }
    }
}

void grayCounterDown(unsigned int *counter)
{
    unsigned int portValue = 255;

    while(1)
    {
        LATA = getGrayCode(portValue);
        portValue--;
        __delay_ms(100);

        if (isButtonPressed(1)) {
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
        portValue++;
        __delay_ms(100);
        
        if (isButtonPressed(1)) {
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
        portValue--;
        __delay_ms(100);
        
        if (isButtonPressed(1)) {
            (*counter)++;
            break;
        }
    }
}

void snake(unsigned int *counter)
{
    unsigned int portValue = 7;
    int goingRight = 1;

    while(1)
    {
        LATA = portValue;
        __delay_ms(100);

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

        if (isButtonPressed(1)) {
            (*counter)++;
            break;
        }
    }
}

void queue(unsigned int *counter)
{
    unsigned int portValue = 7;
    int goingRight = 1;

    while(1)
    {
        LATA = portValue;
        __delay_ms(100);

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

        if (isButtonPressed(1)) {
            (*counter)++;
            break;
        }
    }
}

void randomNumberGenerator(unsigned int *counter)
{
    unsigned int lfsr = 0xACE1u;  // Initial seed (non-zero)
    unsigned int bit;

    while (1)
    {
        // Generate next pseudo-random number using LFSR
        bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
        lfsr = (lfsr >> 1) | (bit << 15);

        LATA = lfsr & 0xFF;  // Display the lower 8 bits on PORTA
        __delay_ms(100);

        if (isButtonPressed(1)) {
            (*counter)++;
            break;
        }
    }
}

void bitShiftSequence(unsigned int *counter) {
    unsigned portValue = 1;
    TRISA = 0x0000;

    while (portValue < 255)
    {
        LATA = portValue;
        __delay32(1000000);

        // Check if the current `portValue` needs a reset to avoid overflow
        if ((portValue & (portValue + 1)) == 0) {
            portValue |= portValue + 1; // Set the next bit to avoid overflow in the next iteration
        } else {
            portValue += (portValue & -portValue) << 1; // Double the step by adding the lowest set bit times two
        }

        if (isButtonPressed(1)) {
            (*counter)++;
            break;
        }
    }
}

int main(void)
{
    TRISA = 0x0000;
    TRISD = 0xFFFF;
    
    unsigned counter = 0;
    
    while(1)
    {
        LATA = 0;
        __delay_ms(150);  // Adjusted to use __delay_ms

        if (isButtonPressed(2)) {
            counter++;
        }
        
        switch(counter % 10)
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
            case 9:
                bitShiftSequence(&counter);
                break;
            default:
                break;
        }
    }
    
    return 0;
}
