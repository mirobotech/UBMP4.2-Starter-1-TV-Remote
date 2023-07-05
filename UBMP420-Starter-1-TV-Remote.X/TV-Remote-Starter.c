/*==============================================================================
 Project: TVRemote-Starter              Activity: mirobo.tech/ubmp4-starter-1
 Date:    July 5, 2023
 
 UBMP4.2 Starter 1 project code for creating SONY-formatted (SIRC) IR remote
 control codes. Use the concepts introduced on the website and beginning with
 this starter program to learn code timing debugging techniques. Then, modify
 this program to create other IR protocols for your own use.
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
    UBMP4_config();             // Configure I/O for on-board UBMP4 devices
	
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


/* Learn More - Program Analysis Activities
 * 
 * 1.   Not all IR protocols encode data using a 40kHz carrier. The NEC protocol
 *      uses a 38kHz carrier, and the Philips RC-6 protocol uses a 36kHz
 *      carrier. Create and debug a pulse function to produce any number of
 *      modulated pulses at 38kHz, the carrier frequency that the demodulator
 *      on UBMP4 and CHRP4 is designed for.
 * 
 * 2.   The NEC protocol (or a slight modification of it) is used by many brands
 *      of televisions including LG and Samsung. Research the NEC protocol. What
 *      type of modulation is used to encode its data? What differences do you
 *      notice between the Sony and NEC data transmissions? What advantages and
 *      disadvantages does the NEC protocol provide over Sony (SIRC) protocol?
 * 
 * 3.   Determine the number of pulses required to create a start bit, a zero
 *      bit, and a one bit for the NEC protocol when using a 38kHz carrier.
 * 
 * 4.   You can clip an oscilloscope lead to the output pin of UBMP4's IR
 *      demodulator (U2) to view the remote control pulses from many type of
 *      remote control transmitters without needing to write any software code.
 *      See if you can decode and reverse engineer the IR protocol transmitted
 *      by a remote control transmitter that you own or have access to.
 * 
 * 5.   Create a flowchart of the software functions that would be needed to
 *      transmit a protocol different from the SIRC protocol.
 *
 * 6.   IR protocols can be created to control your own devices. Think about
 *      the types of commands you would need to encode to remotely control a
 *      robot, or a string of NeoPixel LEDs.
 */
