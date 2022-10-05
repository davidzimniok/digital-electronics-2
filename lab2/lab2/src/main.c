/***********************************************************************
 * 
 * Blink LEDs in Arduino-style and use function from the delay library.
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
#define LED_RED PB0     // External active-low LED
#define BTN PD2     // button
#define SHORT_DELAY 250 // Delay in milliseconds
#ifndef F_CPU
# define F_CPU 16000000 // CPU frequency in Hz required for delay funcs
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>     // AVR device-specific IO definitions
#include <util/delay.h> // Functions for busy-wait delay loops
#include <gpio.h>

#define PB5 5         // FOR REGISTERS VERSION
#define PB0 0
#define PD2 2

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle LEDs and use delay library.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    uint8_t led_value = 0;  // Local variable to keep LED status

    //REGISTER MODE
    //DDRB |= (1<<LED_GREEN) | (1<<LED_RED);
    GPIO_mode_output(&DDRB, LED_GREEN);  // Set output mode in DDRB reg
    GPIO_mode_output(&DDRB, LED_RED);  // Set output mode in DDRB reg
    GPIO_mode_input_pullup(&DDRD,BTN); //set input mode to btn pin

    // Infinite loop
    while (1)
    {   
      //registers - 188B
    /*  _delay_ms(SHORT_DELAY);

        // Change LED value
        if (led_value == 0){
            PORTB &= ~((1<<LED_GREEN) | (1<<LED_RED));
            led_value = 1;
        }
        else{
            PORTB |= (1<<LED_GREEN) | (1<<LED_RED);
            led_value = 0;
        }*/

        //library - 182B
      if(GPIO_read(&PIND,PD2)==0){
        _delay_ms(SHORT_DELAY);
        if (led_value%2 == 0){
            GPIO_write_toggle(&PORTB,LED_RED);
            GPIO_write_toggle(&PORTB,LED_GREEN);  
        }
        led_value++;
      }

    }

    // Will never reach this
    return 0;
}