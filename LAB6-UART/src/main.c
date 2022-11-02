/***********************************************************************
 * 
 * Use USART unit and transmit data between ATmega328P and computer.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and transmit UART data four times 
 *           per second.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize USART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));
    
    // Configure 16-bit Timer/Counter1 to transmit UART data
    // Set prescaler to 262 ms and enable overflow interrupt
    TIM1_overflow_4ms();
    TIM1_overflow_interrupt_enable();
    TIM0_overflow_16ms();
    TIM0_overflow_interrupt_enable();


    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ringbuffer for transmitting via UART

    //uart_puts("\x1b[4;32m");  // 4: underline style; 32: green foreground
    uart_puts("1 ... return value of timer\r\n");
    uart_puts("2 ... reset the timer\r\n");
    //uart_puts("\x1b[0m");     // 0: reset all attributes

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
 * Purpose:  Transmit UART data four times per second.
 **********************************************************************/
ISR(TIMER0_OVF_vect){
    char string[8];
    static uint8_t r_char;
    int16_t val;
    static uint8_t nof=0;
    // Transmit UART string(s)
    //uart_puts("Paris\r\n");
    nof++;
    if(nof>=16){
      r_char=uart_getc();
      if(r_char!='\0'){
        uart_putc(r_char);
        if(r_char==49){
          uart_puts("\nTimer0 value is: ");
          //val=TCNT1;
          //itoa(val,string,10);
          //uart_puts(string);
          //uart_puts("\n\r");
        }
        else if(r_char==50){
       //   TCNT1=0;
          uart_puts("\nTimer0 value is: ");
        /*  val=TCNT1;
          itoa(val,string,10);
          uart_puts(string);
          uart_puts("\n\r");*/
        }
  /*     uart_putc(r_char);
        uart_puts("\t");
        itoa(r_char,string,10);
        uart_puts(string);
        uart_puts("\t 0x");
        itoa(r_char,string,16);
        uart_puts(string);
        uart_puts("\t bin:");
        itoa(r_char,string,2);
        uart_puts(string);
        uart_puts("\r\n");*/
        uart_puts("1 ... return value of timer\r\n");
        uart_puts("2 ... reset the timer\r\n");
      }
      nof=0;
    }
}

ISR(TIMER1_OVF_vect){
  ;
}