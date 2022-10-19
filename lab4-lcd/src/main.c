/***********************************************************************
 * 
 * Stopwatch by Timer/Counter2 on the Liquid Crystal Display (LCD)
 *
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2017 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 * Components:
 *   16x2 character LCD with parallel interface
 *     VSS  - GND (Power supply ground)
 *     VDD  - +5V (Positive power supply)
 *     Vo   - (Contrast)
 *     RS   - PB0 (Register Select: High for Data transfer, Low for Instruction transfer)
 *     RW   - GND (Read/Write signal: High for Read mode, Low for Write mode)
 *     E    - PB1 (Read/Write Enable: High for Read, falling edge writes data to LCD)
 *     D3:0 - NC (Data bits 3..0, Not Connected)
 *     D4   - PD4 (Data bit 4)
 *     D5   - PD5 (Data bit 5)
 *     D6   - PD6 (Data bit 3)
 *     D7   - PD7 (Data bit 2)
 *     A+K  - Back-light enabled/disabled by PB2
 * 
 **********************************************************************/

#define BACKLIGHT PB2

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions

void put_lcd1(int data, int pos, int mode);

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD screen when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    uint8_t cch[8*5]={0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,
                    0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,
                    0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,
                    0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,
                    0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};

    // Initialize display
    lcd_init(LCD_DISP_ON);
    GPIO_mode_output(&DDRB, BACKLIGHT);
    GPIO_write_low(&DDRB,BACKLIGHT);

    // Put string(s) on LCD screen
    lcd_gotoxy(11, 1);
    lcd_puts("I like Digital electronics!");
    lcd_gotoxy(1, 0);
    lcd_puts("00:00.");
    lcd_gotoxy(11, 0);
    lcd_putc('0');

    // Configuration of 8-bit Timer/Counter2 for Stopwatch update
    // Set the overflow prescaler to 16 ms and enable interrupt
    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
    TIM0_overflow_1ms();
    TIM0_overflow_interrupt_enable();
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();
  
    lcd_command(1<<LCD_CGRAM);       // Set addressing to CGRAM (Character Generator RAM) ie to individual lines of character patterns
    for (uint8_t i = 0; i < 5*8; i++)  // Copy new character patterns line by line to CGRAM
        lcd_data(cch[i]);
    lcd_command(1<<LCD_DDRAM);       // Set addressing back to DDRAM (Display Data RAM) ie to character codes

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines, ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD screen every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER2_OVF_vect)
{
    static uint8_t no_of_overflows = 0;
    static uint8_t tenths = 0,mins=0,sec=0;  // Tenths of a second
    uint16_t s2=0;
    char string[4];
  
    no_of_overflows++;
    if (no_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        no_of_overflows = 0;
        tenths++;
        put_lcd1(tenths,7,0);
        if(tenths==9) {
          sec++;
          s2=sec*sec;
          lcd_gotoxy(11, 0);
          sprintf(string,"%4d",s2);
          lcd_puts(string);
          put_lcd1(sec,4,1);
          tenths=0;
        }
        if(sec==60) {
          mins++;
          sec=0;
          put_lcd1(mins,1,1);
          put_lcd1(sec,4,1);
        }
        if(mins==60) {
          mins=0;
          put_lcd1(mins,1,1);
        }
    }
    // Else do nothing and exit the ISR
}

ISR(TIMER0_OVF_vect)
{
    static uint8_t symbol = 0;
    static uint8_t position = 0;
    static uint8_t no_of_overflows = 0;

    no_of_overflows++;
    if(no_of_overflows>=2*10){
      lcd_gotoxy(1+position, 1);
      lcd_putc(symbol);
      symbol++;
      if(symbol==5){
        symbol=0;
        position++;
      }
      if(position==9) { 
        position=0;
        lcd_gotoxy(1,1);
        lcd_puts("          ");
      }
      no_of_overflows=0;  
    }   
}

ISR(TIMER1_OVF_vect)
{
    static uint8_t i;
    static uint8_t no_of_overflows = 0;
    char pom;
    static uint8_t running_text[] = "I like Digital electronics! ";

    no_of_overflows++;
    if(no_of_overflows>=2){
      pom=running_text[0];
      for(i=0;i<strlen(running_text)-1;i++){
        running_text[i]=running_text[i+1];
      }
      running_text[strlen(running_text)-1]=pom;
      lcd_gotoxy(11, 1);
      lcd_puts(running_text);
      no_of_overflows=0;  
    }   
}

void put_lcd1(int data, int pos, int mode){
    char string[2];
    if(mode==1) sprintf(string,"%02d",data);
    else sprintf(string,"%d",data);
    lcd_gotoxy(pos, 0);
    lcd_puts(string);
}