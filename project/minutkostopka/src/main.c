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

//global variable for ADC values
uint16_t val1,val2;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    GPIO_mode_input_nopull(&DDRD,PD2);      //CLK from rotary encoder
    GPIO_mode_input_nopull(&DDRD,PD3);      //data from rotary encoder
    GPIO_mode_input_nopull(&DDRB,PB2);      //switch from joystick

    uint8_t customChars[] = {0b10000,0b11000,0b11100,0b11110,0b11110,0b11100,0b11000,0b10000,
                            0b00000,0b11011,0b11011,0b11011,0b11011,0b11011,0b11011,0b00000};

    lcd_command(1<<LCD_CGRAM);       // Set addressing to CGRAM (Character Generator RAM) ie to individual lines of character patterns
    for (uint8_t i = 0; i < 2*8; i++)  // Copy new character patterns line by line to CGRAM
        lcd_data(customChars[i]);
    lcd_command(1<<LCD_DDRAM);       // Set addressing back to DDRAM (Display Data RAM) ie to character codes

    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("ST :");
    lcd_gotoxy(1, 1); lcd_puts("MIN:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(6, 0); lcd_puts("00:00:00"); 
    lcd_gotoxy(6, 1); lcd_puts("00:00:00"); 
    lcd_gotoxy(15, 0); lcd_putc(0); 
    lcd_gotoxy(15, 1); lcd_putc(0);   

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

    uint16_t last=10000,act,cnt=0,cnt_o=1000;
    char str[4];

    last=GPIO_read(&DDRD,2);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        act=GPIO_read(&PIND,PD2);
        if(act!=last){
            if(GPIO_read(&PIND,PD3)!=act) cnt++;
            else{
                if(cnt!=0) cnt--;
            }
        }
        last=act;
        itoa(cnt,str,10);
        if(cnt!=cnt_o){
            lcd_gotoxy(8, 1); lcd_puts("    ");
            lcd_gotoxy(8, 1); lcd_puts(str);
            cnt_o=cnt;
        }
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
    char str[4];
    overfl++;
    uint16_t i;
    if(overfl>=10){
        // Start ADC conversion¨
        ADMUX &= ~((1UL<<0)|(1UL<<1)|(1UL<<2)|(1UL<<3));
        for(i=0;i<10000;i++);
        ADCSRA |= (1UL<<6);

        do{
            ;
        }while(ADCSRA&16==1);

        val1 = ADC;

        itoa(val1,str,10);
        lcd_gotoxy(3, 1); lcd_puts("    ");
        lcd_gotoxy(3, 1); lcd_puts(str);

        ADMUX |= (1UL<<0);
        for(i=0;i<10000;i++);
        ADCSRA |= (1UL<<6);
        
        val2 = ADC;
       
        itoa(val2,str,10);
        lcd_gotoxy(3, 0); lcd_puts("    ");
        lcd_gotoxy(3, 0); lcd_puts(str);

        overfl=0;
    }
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect){
    ;
}