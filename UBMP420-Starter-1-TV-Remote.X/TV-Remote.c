/*==============================================================================
 Project: TV-Remote                     Activity: mirobo.tech/ubmp4-starter-1
 Date:    May 9, 2023
 
 This starter programming project for the mirobo.tech UBMP4 modulates an IR LED
 to create SONY-formatted (SIRC) remote control codes. By using the concepts
 introduced in this program and by following the explanations and debugging
 techniques from the website, you should be able to easily modify this starter
 program to create IR transmissions modulated using other control protocols.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP420.h"         // Include UBMP4.2 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Sony IR command (data) codes
#define SONY_CH1    0x00        // Channel number button codes
#define SONY_CH2    0x01
#define	SONY_CH3    0x02
#define	SONY_CH4    0x03
#define	SONY_CH5    0x04
#define	SONY_CH6    0x05
#define	SONY_CH7    0x06
#define	SONY_CH8    0x07
#define	SONY_CH9    0x08
#define	SONY_CH0    0x09

#define	SONY_CHUP   0x10        // Channel up button code
#define	SONY_CHDN   0x11        // Channel down button code
#define SONY_VOLUP  0x12        // Volume up button code
#define	SONY_VOLDN  0x13        // Volume down buttoncode
#define	SONY_MUTE   0x14        // Mute button code
#define SONY_POWER  0x15        // Power on/off button code

#define	SONY_INPUT	0x25        // Input select button code

// Sony IR device (address) codes
#define SONY_TV     0x01        // TV device code

// Sony IR protocol modulation details
#define SONY_START_PULSES   96  // Number of 38kHz IR pulses in a start bit
#define SONY_1_PULSES       48  // Number of IR pulses to encode a one bit
#define SONY_0_PULSES       24  // Number of IR pulses to encode a zero bit
#define SONY_BIT_DELAY      600 // Space between data bits in microseconds
#define SONY_COMMAND_BITS   7   // Number of command (data) bits to be sent
#define SONY_DEVICE_BITS    5   // Number of device (address) bits to be sent


void ir_pulse_40k(unsigned int pulses) // Make requested number of 40kHz pulses
{
    for (pulses; pulses != 0; pulses--) {
        LED6 = 1;               // Create 25us period waves
        _delay(142);            // Use 12us on and 13us off for ~50% duty cycle
        LED6 = 0;               // (25% duty cycle can be used to save power)
        _delay(141);
    }
}

void ir_transmit_Sony(unsigned char device, unsigned char command)
{
    // Create Start bit followed by bit delay
    ir_pulse_40k(SONY_START_PULSES);
    __delay_us(SONY_BIT_DELAY);

    // Transmit command bits with bit delays
    for (unsigned char i = SONY_COMMAND_BITS; i != 0; i--)
    {
        if ((command & 0b00000001) == 1)  // Isolate LSB to determine its state
            ir_pulse_40k(SONY_1_PULSES);
        else
            ir_pulse_40k(SONY_0_PULSES);

        command = command >> 1; // Shift next bit into position
        __delay_us(SONY_BIT_DELAY);
    }

    // Transmit device bits with bit delays
    for (unsigned char i = SONY_DEVICE_BITS; i != 0; i--)
    {
        if ((device & 0b00000001) == 1)  // Isolate LSB to determine its state
            ir_pulse_40k(SONY_1_PULSES);
        else
            ir_pulse_40k(SONY_0_PULSES);

        device = device >> 1;   // Shift next bit into position
        __delay_us(SONY_BIT_DELAY);
    }
}

int main(void)
{
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
	
    while(1)
    {
        if(SW3 == 0)
        {
            ir_transmit_Sony(SONY_TV, SONY_POWER);
        }
        
        // Data frame delay between button presses (adjust based on protocol)
        __delay_ms(25);
        
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
 *      modulated pulses at one of these other commonly-used frequencies.
 * 
 * 2.   The NEC protocol (or a slight modification of it) is used by many brands
 *      of televisions including LG and Samsung. Research the NEC protocol. What
 *      type of modulation is used to encode its data?
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
