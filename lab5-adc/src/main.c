/***********************************************************************
 * 
 * Use Analog-to-digital conversion to read push buttons on LCD keypad
 * shield and display it on LCD screen.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");  // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");  // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");  // Put button name here

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX |= (1UL<<6);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX &= ~((1UL<<0)|(1UL<<1)|(1UL<<2)|(1UL<<3));
    // Enable ADC module
    ADCSRA |= (1UL<<7);
    // Enable conversion complete interrupt
    ADCSRA |= (1UL<<3);
    // Set clock prescaler to 128
    ADCSRA |= (1UL<<0)|(1UL<<1)|(1UL<<2);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    static uint8_t overfl=0;
    overfl++;
    if(overfl>=3){
        // Start ADC conversion
        ADCSRA |= (1UL<<6);
        overfl=0;
    }
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
    static uint16_t o_val=10000;
    uint16_t value;
    float voltage;
    char string[4];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    value = ADC;

    if(o_val!=value){ 
        // Convert "value" to "string" and display it
        //sprintf(string,"%4d",value);
        sprintf(string,"%4d",(uint16_t)((((float)value/(1023.0)))*5000.0));
        lcd_gotoxy(8, 0); lcd_puts(string);  // Put ADC value in decimal
        sprintf(string,"%3X",value);
        lcd_gotoxy(13,0); lcd_puts(string);  // Put ADC value in hexadecimal
        lcd_gotoxy(8, 1);
        lcd_puts("      "); //delete chars from display
        lcd_gotoxy(8, 1);
        if(value<25) lcd_puts("RIGHT");
        if(value<126&&value>76) lcd_puts("UP");
        if(value<271&&value>221) lcd_puts("DOWN");
        if(value<428&&value>378) lcd_puts("LEFT");
        if(value<701&&value>601) lcd_puts("SELECT");
        if(value>1000) lcd_puts("NONE");
        o_val=value;
    }
    
}