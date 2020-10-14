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
* URL: http://ww1.microchip.com/downloads/en/AppNotes/Atmel-1451-Using-the-AVR-USART-on-tinyAVR-and-megaAVR-devices_ApplicationNote_AVR306.pdf
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
* http://ww1.microchip.com/downloads/en/AppNotes/
* Atmel-1451-Using-the-AVR-USART-on-tinyAVR-and-megaAVR-devices_ApplicationNote_AVR306.pdf
*
* Assumes that we're using U2X and a baud rate of 9600
*/
#define UBRR_VAL    12
/* Prototypes */
// Setup
void InitUART(unsigned char ubrr_val);
void InitPins(void);
// Read state of keys
unsigned int ReadKeys(unsigned int key_state);
// Sending keys over UART to Bluefruit
void TransmitKeys(unsigned int key_state);
void SendString(char message[]);
void TransmitByte(unsigned char data);
// Receiving reply over UART
unsigned char ReceiveByte(void);
// Utility
void Delay(void);

unsigned int key_state = 0;
char zero_message[6] = {'k', 'e', 'y', ' ', '0', '\0'};
char one_message[6] = {'k', 'e', 'y', ' ', '1', '\0'};
char two_message[6] = {'k', 'e', 'y', ' ', '2', '\0'};
char three_message[6] = {'k', 'e', 'y', ' ', '3', '\0'};
char four_message[6] = {'k', 'e', 'y', ' ', '4', '\0'};
char five_message[6] = {'k', 'e', 'y', ' ', '5', '\0'};
char six_message[6] = {'k', 'e', 'y', ' ', '6', '\0'};
char seven_message[6] = {'k', 'e', 'y', ' ', '7', '\0'};
char eight_message[6] = {'k', 'e', 'y', ' ', '8', '\0'};
char nine_message[6] = {'k', 'e', 'y', ' ', '9', '\0'};

char keyboard_cmd[16] = {'A','T','+','B','L','E','K','E','Y','B','O','A','R','D','=', '\0'};
char end_string[3] = {'\r', '\n', '\0'};

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

/* Initialize Pins */
void InitPins(void)
{
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

    // output pin: MODE
    /* Connected to Mod pin on bluefruit. If it is driven high, the 
       bluefruit module will enter command mode. If it is driven low, the
       bluefruit module will enter data mode.  */
    DDRD |= (1 << PD5); // Set this pin as an output
    PORTD |= (1 << PD5); // Output high (source)

    // output pin: CTS
    /* Connected to CTS pin on bluefruit. If it is driven high, the 
       bluefruit module will wait to send data to the MCU. If it's 
       driven low, the module will know that it's clear to send (exactly
       what's on the label)  */
    DDRD |= (1 << PD6); // Set this pin as an output
    PORTD |= (1 << PD6); // Start in "not clear to send" mode

    // B REGISTER

    // input pin: green wire, key 0
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
    Delay();
}

int main(void)
{
    /* Set the baudrate to 9800 bps using internal 8MHz RC Oscillator */

    InitUART(UBRR_VAL);

    /* Set the input/output pins and enable pull-up resistors */
    InitPins();


    for(;;)
    {   //
        // PIN VALUES: 
        // PINA - 3  - 0x3
        // PINB - 31 - 0x1f
        // PIND - 14 - 0xe

        /* This is true if none of the keys are pressed */
        if (!(~PINA & 0x3) & !(~PINB & 0x1f) & !(~PIND & 0xe)){
            /* If none of the keys are pressed, AND if there's 
               something stored in key_state, then this next bit 
               comes into play */
            if (key_state != 0) {
                /* This passes key_state to TransmitKeys to be relayed to the
                    bluetooth module, then resets the value to 0 */
                TransmitKeys(key_state);
                key_state = 0;
            }
        }
        else {
            // Delay between a change in the state of the keys and reading the new state
            Delay();
            key_state = ReadKeys(key_state);
        }     
    }
    return 0;
}

unsigned int ReadKeys(unsigned int key_state)
{
    if (!(PINB & (1 << PB0))) {    // is switch closed?
        key_state |= (1 << 0);     // switch is closed, save key
    }
    if (!(PINB & (1 << PB1))) {    // is switch closed??
        key_state |= (1 << 1);     // switch is closed, save key
    }
    if (!(PINB & (1 << PB2))) {    // is switch closed?
        key_state |= (1 << 2);     // switch is closed, save key
    }
    if (!(PINB & (1 << PB3))) {    // is switch closed?
        key_state |= (1 << 3);     // switch is closed, save key
    }
    if (!(PIND & (1 << PD3))) {    // is switch closed?
        key_state |= (1 << 4);     // switch is closed, save key
    }
    if (!(PIND & (1 << PD4))) {    // is switch closed?
        key_state |= (1 << 5);     // switch is closed, save key
    }
    if (!(PIND & (1 << PD2))) {    // is switch closed?
        key_state |= (1 << 6);     // switch is closed, save key
    }
    if (!(PINB & (1 << PB4))) {    // is switch closed?
        key_state |= (1 << 7);     // switch is closed, save key
    }
    if (!(PINA & (1 << PA1))) {    // is switch closed?
        key_state |= (1 << 8);     // switch is closed, save key
    }
    if (!(PINA & (1 << PA0))) {    // is switch closed?
        key_state |= (1 << 9);     // switch is closed, save key
    } 
    return key_state;
}

void TransmitKeys(unsigned int key_state)
{
    SendString(keyboard_cmd);
    if (key_state & (1 << 0)) {
        SendString(zero_message);
    }
    if (key_state & (1 << 1)) {
        SendString(one_message);
    }
    if (key_state & (1 << 2)) {
        SendString(two_message);
    }
    if (key_state & (1 << 3)) {
        SendString(three_message);
    }
    if (key_state & (1 << 4)) {
        SendString(four_message);
    }
    if (key_state & (1 << 5)) {
        SendString(five_message);
    }
    if (key_state & (1 << 6)) {
        SendString(six_message);
    }
    if (key_state & (1 << 7)) {
        SendString(seven_message);
    }
    if (key_state & (1 << 8)) {
        SendString(eight_message);
    }
    if (key_state & (1 << 9)) {
        SendString(nine_message);
    }
    SendString(end_string);
}

void SendString(char message[])
{ 
    int i = 0;
    do {
        TransmitByte(message[i]);
        i++;
    } while (message[i]!='\0');    
}

void TransmitByte( unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !(UCSRA & (1<<UDRE)) );
    /* Start transmission */
    UDR = data;
}

unsigned char ReceiveByte(void)
{
    /* Wait for incoming data */
    while ( !(UCSRA & (1<<RXC)) );
    /* Return the data */
    return UDR;
}

void Delay(void)
{
    /* This delay is inserted after a change in the key state 
    is detected but before we process and transmit the result.
    Interval for a baud rate of 9600 should be ~= 5 ms, which is the 
    amount of time recommended by:
    "AVR243: Matrix Keyboard Decoder", pg 6:
    http://ww1.microchip.com/downloads/en/Appnotes/doc2532.pdf
    */
    volatile unsigned int del = 50;
    while(del--);
}
