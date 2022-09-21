/***********************************************************************
 * 
 * Blink a LED in Arduino-style and use function from the delay library.
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2022 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Defines -----------------------------------------------------------*/
#define LED_GREEN PB5   // PB5 is AVR pin where green on-board LED 
                        // is connected
#define LED_OUT PB0   // PB0
#define DOT 250 // Delay in milliseconds
#define COMMA 750 // Delay in milliseconds
#ifndef F_CPU
# define F_CPU 16000000 // CPU frequency in Hz required for delay funcs
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>     // AVR device-specific IO definitions
#include <util/delay.h> // Functions for busy-wait delay loops


// -----
// This part is needed to use Arduino functions but also physical pin
// names. We are using Arduino-style just to simplify the first lab.
#include "Arduino.h"
#define PB0 8          // In Arduino world, PB5 is called "13"
// -----


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED and use delay library.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    pinMode(LED_OUT, OUTPUT);

    // Infinite loop
    while (1)
    {
        digitalWrite(LED_OUT,HIGH);
        _delay_ms(DOT);
        digitalWrite(LED_OUT,LOW);
        _delay_ms(DOT);
        digitalWrite(LED_OUT,HIGH);
        _delay_ms(DOT);
        digitalWrite(LED_OUT,LOW);
        _delay_ms(COMMA);
        digitalWrite(LED_OUT,HIGH);
        _delay_ms(COMMA);
        digitalWrite(LED_OUT,LOW);

        
    }

    return 0;
}