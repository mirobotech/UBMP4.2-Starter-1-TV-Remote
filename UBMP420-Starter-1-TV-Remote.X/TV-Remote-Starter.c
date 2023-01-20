/*==============================================================================
 Project: TVRemote-Starter              Activity: mirobo.tech/ubmp4-starter-1
 Date:    January 20, 2023
 
 UBMP4.2 Starter 1 project code for creating SONY-formatted (SIRC) IR remote
 control codes. Use the concepts introduced in this program and the website 
 activity to learn debugging techniques, and then modify this program to create
 other IR protocols.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP420.h"         // Include UBMP4.2 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.


void ir_pulse_40k(unsigned int pulses) // Makes requested number of 40kHz pulses
{
    for (pulses; pulses != 0; pulses--) {
        LED6 = 1;               // Create 25us period waves
        __delay_us(12);         // Use 12us on and 13us off for ~50% duty cycle
        LED6 = 0;               // (25% duty cycle can be used to save power)
        __delay_us(13);
    }
}


int main(void)
{
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
	
    while(1)
    {
        ir_pulse_40k(96);       // Pulse test code
        __delay_ms(15);         // Delay between pulses
        
        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}


