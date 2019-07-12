/*
* Title: Keyer
* Author: Timothy Salazar
* Date: 12 July 2019
*
* Use was made of the following resources in writing this code.
*
* Title: CrossPack for AVR Development 20131216
* URL: https://www.obdev.at/products/crosspack/index.html
* License: GNU Public License
* Comments: Provided the prototype for this project

*
* Title: AVR306: Using the AVR USART on tinyAVR and megaAVR devices
* URL: http://ww1.microchip.com/downloads/en/AppNotes/Atmel-1451-Using-the-AVR\
-USART-on-tinyAVR-and-megaAVR-devices_ApplicationNote_AVR306.pdf
* Accessed: 13 June 2019
* Comments: Helped choose settings appropriate for application
*
* Title: UART Polling example
* URL: https://www.microchip.com/wwwAppNotes/AppNotes.aspx?appnote=en591470
* Accessed: 13 June 2019
* Comment: This provided a skeleton for my program, demonstrated key
* principles (I'm still learning a lot), and provided the InitUART and
* TransmitByte functions (though I modified them slightly).
*
* Copyright (C) 2016 Atmel Corporation. All rights reserved.
*
* \page License
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
* 3. The name of Atmel may not be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an
*    Atmel microcontroller product.
*
* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/


#include "avr/io.h"
/* Value taken from Table 3-2, pg 5 of:
* AVR306: Using the AVR USART on tinyAVR and megaAVR devices
* Assumes that we're using U2X and a baud rate of 9600
*/
#define UBRR_VAL    12
/* Prototypes */
void InitUART(unsigned char ubrr_val);

void TransmitByte(unsigned char data);

int key_check[10];


int main(void)
{
    /* Set the baudrate to 9800 bps using internal 8MHz RC Oscillator */

    InitUART(UBRR_VAL);

    // A REGISTER

    // input pin: black wire, key 8
    DDRA &= ~(1 << PA1);  // switch on pin 4 (PA1)
    PORTA |= (1 << PA1);  // enable pull-up resistor
    // input pin: white wire, key 9
    DDRA &= ~(1 << PA0);  // switch on pin 5 (PA0)
    PORTA |= (1 << PA0);  // enable pull-up resistor

    // D REGISTER

    // input pin: grey wire, key 6
    DDRD &= ~(1 << PD2);  // switch on pin 6 (PD2)
    PORTD |= (1 << PD2);  // enable pull-up resistor
    // input pin: purple wire, key 4
    DDRD &= ~(1 << PD3);  // switch on pin 7 (PD3)
    PORTD |= (1 << PD3);  // enable pull-up resistor
    // input pin: blue wire, key 5
    DDRD &= ~(1 << PD4);  // switch on pin 8 (PD4)
    PORTD |= (1 << PD4);  // enable pull-up resistor
    // input pin: green wire, key 0

    // B REGISTER

    DDRB &= ~(1 << PB0);  // switch on pin 12 (PB0)
    PORTB |= (1 << PB0);  // enable pull-up resistor
    // input pin: yellow wire, key 1
    DDRB &= ~(1 << PB1);  // switch on pin 13 (PB1)
    PORTB |= (1 << PB1);  // enable pull-up resistor
    // input pin: orange wire, key 2
    DDRB &= ~(1 << PB2);  // switch on pin 14 (PB2)
    PORTB |= (1 << PB2);  // enable pull-up resistor
    // input pin: red wire, key 3
    DDRB &= ~(1 << PB3);  // switch on pin 15 (PB3)
    PORTB |= (1 << PB3);  // enable pull-up resistor
    // input pin: brown wire, key 7
    DDRB &= ~(1 << PB4);  // switch on pin 16 (PB4)
    PORTB |= (1 << PB4);  // enable pull-up resistor


    int i = 0;
    for ( i = 0; i < 10; i++) {
        key_check[i] = 0;
    }
    for(;;)
    {

        /* wait for keys to be pressed */
        while ( !( (PINA & 3) | (PINB & 15) | (PIND & 28) ) );
        /* a_states = (PINA & 3); // mask for the a pins we're using
        b_states = (PINB & 15);
        d_states = (PIND & 28); */

        if (!(PINB & (1 << PB0)) && key_check[0]) {    // is switch closed?
            TransmitByte('0');     // switch is closed, transmit key
            TransmitByte('2');
            key_check[0] = 1;
        }
        if (!(PINB & (1 << PB1)) && key_check[1]) {    // is switch closed??
            TransmitByte('0');     // switch is closed, transmit key
            TransmitByte('3');
            key_check[1] = 1;
        }
        if (!(PINB & (1 << PB2)) && key_check[2]) {    // is switch closed?
            TransmitByte('0');     // switch is closed, transmit key
            TransmitByte('4');
            key_check[2] = 1;
        }
        if (!(PINB & (1 << PB3)) && key_check[3]) {    // is switch closed?
            TransmitByte('0');     // switch is closed, transmit key
            TransmitByte('5');
            key_check[3] = 1;
        }
        if (!(PIND & (1 << PD3)) && key_check[4]) {    // is switch closed?
            TransmitByte('0');     // switch is closed, transmit key
            TransmitByte('6');
            key_check[4] = 1;
        }
        if (!(PIND & (1 << PD4)) && key_check[5]) {    // is switch closed?
            TransmitByte('0');     // switch is closed, transmit key
            TransmitByte('7');
            key_check[5] = 1;
        }
        if (!(PIND & (1 << PD2)) && key_check[6]) {    // is switch closed?
            TransmitByte('0');      // switch is closed, transmit key
            TransmitByte('8');
            key_check[6] = 1;
        }
        if (!(PINB & (1 << PB4)) && key_check[7]) {    // is switch open?
            TransmitByte('0');      // switch is closed, transmit key
            TransmitByte('9');
            key_check[7] = 1;
        }
        if (!(PINA & (1 << PA1)) && key_check[8]) {    // is switch open?
            TransmitByte('0');      // switch is closed, transmit key
            TransmitByte('a');
            key_check[8] = 1;
        }
        if (!(PINA & (1 << PA0)) && key_check[9]) {    // is switch open?
            TransmitByte('0');      // switch is closed, transmit key
            TransmitByte('b');
            key_check[9] = 1;
        }
        // While the pins in the A, B, and D register we're using maintain the
        // same state, wait.
        // while ( !( (PINA & ~a_states ) | (PINB & ~b_states) | (PIND & ~d_states) ) );
        //while (( (PINA & 3) | (PINB & 15) | (PIND & 28) ) );
        //while ( ( (PINA & 3) | (PINB & 15) | (PIND & 28) ) );
        // check each key
        if ( (PINB & (1 << PB0)) && key_check[0] ){
        // if ( key_check[0] ) {
            TransmitByte('8');
            TransmitByte('2');
            key_check[0] = 0;
        }
        if ( (PINB & (1 << PB1)) && key_check[1] ){
        //if ( key_check[1] ) {
            TransmitByte('8');
            TransmitByte('3');
            key_check[1] = 0;
        }
        //if ( key_check[2] ) {
        if ( (PINB & (1 << PB2)) && key_check[2] ){
            TransmitByte('8');
            TransmitByte('4');
            key_check[2] = 0;
        }
        //if ( key_check[3] ) {
        if ( (PINB & (1 << PB3)) && key_check[3] ){
            TransmitByte('8');
            TransmitByte('5');
            key_check[3] = 0;
        }
        //if ( key_check[4] ) {
        if ( (PIND & (1 << PD3)) && key_check[4] ){
            TransmitByte('8');
            TransmitByte('6');
            key_check[4] = 0;
        }
        //if ( key_check[5] ) {
        if ( (PIND & (1 << PD4)) && key_check[5] ){
            TransmitByte('8');
            TransmitByte('7');
            key_check[5] = 0;
        }
        //if ( key_check[6] ) {
        if ( (PIND & (1 << PD2)) && key_check[6] ){
            TransmitByte('8');
            TransmitByte('8');
            key_check[6] = 0;
        }
        //if ( key_check[7] ) {
        if ( (PINB & (1 << PB4)) && key_check[7] ){
            TransmitByte('8');
            TransmitByte('9');
            key_check[7] = 0;
        }
        //if ( key_check[8] ) {
        if ( (PINA & (1 << PA1)) && key_check[8] ){
            TransmitByte('8');
            TransmitByte('a');
            key_check[8] = 0;
        }
        //if ( key_check[9] ) {
        if ( (PINA & (1 << PA0)) && key_check[9] ){
            TransmitByte('8');
            TransmitByte('b');
            key_check[9] = 0;
        }

        /* Signal End */
    }
    return 0;
}


/**
 * \file
 *
 * \brief UART Polling example
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */



/* Initialize UART */
void InitUART(unsigned char ubrr_val)
{
    /* Set the baud rate */
    UBRRH = (unsigned char)(ubrr_val>>8);
    UBRRL = (unsigned char)ubrr_val;
    /* Enable UART receiver and transmitter */
    UCSRB = ( (1<<RXEN) | (1<<TXEN) );
    /* Enable U2X (double speed) UART */
    UCSRA = (1<<U2X) | UCSRA;
}

void TransmitByte( unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !(UCSRA & (1<<UDRE)) );
    /* Start transmission */
    UDR = data;
}
