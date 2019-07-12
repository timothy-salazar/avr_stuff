/*
* This code was retrireved from:
*   Tinysaur tutorial - Tutorial 002: Fading LED x1
*   https://tinusaur.org/tutorials/tutorial-002-fading-led-x1/
*   July 8, 2019
*
* Modified by: Timothy Salazar
* email: timothy.salazar@colorado.edu
*/

#include <avr/io.h>
#include <util/delay.h>
#define LED1_PORT PB0

void StopDelay(void);

int main(void) {
  // Set the LED port number as output.
  DDRB |= (1 << LED1_PORT);
  uint8_t fade_out = 0;
#define BIAS_MAX 800
#define BIAS_MIN 1
  uint16_t bias = BIAS_MIN;
  // Begin an infinite loop. This is how most programs work.
  while (1) {
    // Set the LED port bit to "1" - LED will be turned on.
    PORTB |= (1 << LED1_PORT);
    // Wait a little. The delay function simply does N-number of "empty" loops.
    _delay_loop_2(bias);
    // Set the LED port bit to "0" - LED will be turned off.
    PORTB &= ~(1 << LED1_PORT);
    // Wait a little.
    _delay_loop_2(BIAS_MAX - bias);
    if (fade_out == 0) {
      bias++;
      if (bias >= BIAS_MAX - 1) fade_out = 1;
    }
    else {
      bias--;
      if (bias <= BIAS_MIN) {
          // adding a delay when it turns off makes it look a bit more natural
          StopDelay();
          StopDelay();
          fade_out = 0;
      }
    }
    // Do that again ...
  }
  // Return the mandatory for the "main" function value.
  return (0);
}

void StopDelay(void)
{
    volatile unsigned int del = 40000;
    while(del--);
}
