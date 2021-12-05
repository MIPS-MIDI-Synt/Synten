#include <pic32mx.h>
#include <math.h>
#include <stdint.h>
#include "sine_wave.h"

#define BUFFER_SIZE 1024
#define SAMPLE_RATE 22050
#define SAMPLE_DURATION (1.0f / SAMPLE_RATE)
#define NUM_OSCILLATORS 32

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

#if 0
void message_handler(char status, char key, char vel){
    int freq;
    // int midi_chan = (midimessage & 0x000f0000) >> 16;

    if(status >> 8 = 8){ // note off
        // call functon to stop note playing
    }
    if(status >> 8 = 9){ // note on
    float midi_key  = (midimessage & 0x00007f00) >> 8;
    int midi_vel  = (midimessage & 0x0000007f); 
    // freq = powf(2.0f, ((float)key-69.0f)/12.0f) * 440;
	// freq = 2<<((key-69)/12.0f)*440;
	freq = midi_key; 
    // call function with freq
    }
}
#endif
// MIDI Baudrate = 31250, to sync MIDI and UART we set baudrate of UART to 31250
int calculate_baudrate_divider(int sysclk, int baudrate, int highspeed) {
	int pbclk, uxbrg, divmult;
	unsigned int pbdiv;
	
	divmult = (highspeed) ? 4 : 16; // Pic32 has 16-bit prescaler for baud rate generator
	/* Periphial Bus Clock is divided by PBDIV in OSCCON */
	pbdiv = (OSCCON & 0x180000) >> 19; // pbdiv = OSSCON<20:19>
	pbclk = sysclk >> pbdiv;
	
	/* Multiply by two, this way we can round the divider up if needed */
	uxbrg = ((pbclk * 2) / (divmult * baudrate)) - 2; // = 
	/* We'll get closer if we round up */
	if (uxbrg & 1) // if odd, div2 add 1
		uxbrg >>= 1, uxbrg++;
	else
		uxbrg >>= 1; // else only div 2
	return uxbrg;
}

#if 1
                          /* PIC32 signal, CHIPKIT Pin #  */
//#define DAC_LDAC_PIN		3 /* RD3,          9              */
#define DAC_CS_PIN		3 /* RD3,          9              */
#define DAC_SDI_PIN		6 /* RD6,          36             */
#define DAC_SCK_PIN		7 /* RD7,          37             */


/* MCP4921 DAC */
void MCP4921_init(void)
{
	/* Set all pins used by DAC to outputs */
	TRISDCLR |= (1 << DAC_CS_PIN) | 
					(1 << DAC_SDI_PIN) | 
					(1 << DAC_SCK_PIN);
	
	//TRISD = 0;
					
	/* LDAC and CS is active low so set pins high.
	   LDAC can also be tied low if transfer to the DAC
		on rising edge of CS is desired. */
	PORTD |= (1 << DAC_CS_PIN);

	PORTD &= ~((1 << DAC_SDI_PIN) | (1 << DAC_SCK_PIN));

}

void MCP4921_write(uint16_t data)
{
	int i = 0;
	/* Data to the DAC is transferred as 16 bits */
	uint16_t dac_data = 0;
	/* Take the 12 least significant bits */
	dac_data |= (data & 0xfff);
	/* Output Power-down Control bit */
	dac_data |= (1 << 12);
	/* Gain 1x */
	dac_data |= (1 << 13);
	/* Buffer V_ref */
	dac_data |= (1 << 14);

	/* Set CS low */
	PORTD &= ~(1 << DAC_CS_PIN);

	/* Send data to DAC, MSB first */
	for (i = 0; i < 16; ++i)
	{
		if (dac_data & 0x8000)
		{
			PORTD |= (1 << DAC_SDI_PIN);
		}
		else
		{
			PORTD &= ~(1 << DAC_SDI_PIN);
		}
		dac_data <<= 1;
		/* Clock in data */
		PORTD |= (1 << DAC_SCK_PIN);
		PORTD &= ~(1 << DAC_SCK_PIN);

	}
	/* Set CS high */
	PORTD |= (1 << DAC_CS_PIN);
}
#endif

void uart1_init(void)
{
	/* Configure UART1 for 115200 baud, no interrupts */
	U1BRG = calculate_baudrate_divider(80000000, 31250, 0); // Initializes U1BRG register for 31250 baud
	U1STA = 0; // 
	/* 8-bit data, no parity, 1 stop bit */
	U1MODE = 0x8000; 
	/* Enable transmit and recieve */
	U1STASET = 0x1400;	//


	// Interrupts:

	IEC(0) |= 0x08000000; // Interrupt enable bit 27 in IEC0 
	IPC(6) |= 0x1F; // Interupt priority max = 7 IPC6<4:2>, sup-priority max = 3 IPC6<1:0>
	//IPC(6) |= 0x0D; // Interupt priority max = 7 IPC6<4:2>, sup-priority max = 3 IPC6<1:0>

	// OBS: Below conflicting with row 44,46
	U1STA &= ~0xC0;
	U1STA |= 0x00; // U1STA<7:6> Receive interrupt mode = 10 > Flag when buffer becomes 3/4 (24bits = One MIDI message)
	// Interrupt flag @ IFS0<27>
	// xxxx xxxx xxxx xxxx xxxx xxxx 00xx xxxx
	enable_interrupt();
}

void timer2_init() {

	T2CONCLR = 0x8000; // Turn off timer
	/* Prescaler of 16 gives a clock to tim2 of 2.5 MHz */
	T2CON = 0x4<<4; // Change bit 6-4 (TCKPS) to config prescale 1:16, set bits to: 100
	//T2CON |= (1<<3); // T32 bit set to 1: Combine T2 & T3 for 32 bit 
	// PR2 = period; // Set timeout period
  	T2CONSET = 0x8000; // Turn on timer 

}

unsigned int timer2_get_cnt(void)
{
	//unsigned int tim_cnt = ((TMR3 << 16) & 0xffff0000) | (TMR2 & 0x0000ffff);
	//return tim_cnt;
	return TMR2;
}

void init() {

	/* On Uno32, we're assuming we're running with sysclk == 80 MHz */
	/* Periphial bust can run at a maximum of 40 MHz, setting PBDIV to 1 divides sysclk with 2 */
	OSCCON &= ~0x180000; // Reset pbdiv to 0
	OSCCON |= 0x080000; // Set pbdiv to 1
	// ODCE = 0; // Push-Pull output
	// TRISECLR = 0xFF; // Låt va så länge,sätt alla pins till output
	TRISE = 0x0; 
	//TRISF = 0xFFFFFFFF;
	TRISF = 0x00000000;

	// /* Configure UART1 for 115200 baud, no interrupts */
	// U1BRG = calculate_baudrate_divider(80000000, 31250, 0); // Initializes U1BRG register for 31250 baud
	// U1STA = 0; // 
	// /* 8-bit data, no parity, 1 stop bit */
	// U1MODE = 0x8000; 
	// /* Enable transmit and recieve */
	// U1STASET = 0x1400;	//


	// Interrupts:

	// IEC(0) |= 0x08000000; // Interrupt enable bit 27 in IEC0 
	// IPC(6) |= 0x1F; // Interupt priority max = 7 IPC6<4:2>, sup-priority max = 3 IPC6<1:0>
	//IPC(6) |= 0x0D; // Interupt priority max = 7 IPC6<4:2>, sup-priority max = 3 IPC6<1:0>

	// OBS: Below conflicting with row 44,46
	// U1STA &= ~0xC0;
	// U1STA |= 0x00; // U1STA<7:6> Receive interrupt mode = 10 > Flag when buffer becomes 3/4 (24bits = One MIDI message)
	// Interrupt flag @ IFS0<27>
	// xxxx xxxx xxxx xxxx xxxx xxxx 00xx xxxx
	// enable_interrupt();
	// return;
	PORTE = 0;
	timer2_init();
	//uart1_init();
	MCP4921_init();

}

void error_handler(void)
{
	while(1)
	{
		delay(1000000);
		PORTE ^= (1 << 2);
	}
}

volatile char midi_buff[3];
volatile int midi_msg_cnt = 0;

void uart_isr( void ){
	unsigned char tmp;
	unsigned int fifo_reg = 0;
	int i = 0;
	delay(100000);
	PORTE ^= (1 << 4);
	if(IFS(0) & 0x08000000){
		//tmp = U1RXREG & 0xFFFFFF;
		//fifo_reg = U1RXREG;
		midi_buff[midi_msg_cnt++] = U1RXREG & 0xff;
		//while(U1STA & (1 << 9));
		//U1TXREG = U1RXREG;

		// U1RXREG = 0;
		//while(U1STA & (1 << 9)); //make sure the write buffer is not full 
		//U1TXREG = tmp;
		// for(i = 0; i < 3; ++i)
		// {
		// 	while(U1STA & (1 << 9));
		// 	U1TXREG = fifo_reg & 0xff;
		// 	fifo_reg >>= 8;

		// }
		
		PORTE = tmp; // Nytillagd
		IFS(0) &= 0xF7FFFFFF; // Acknowledge interrupt
	}
}
int main(void) {
	unsigned int i_acc = 0;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int m = 0;
	unsigned int tim_ticks = 0;
	// delay(1000);


	init();
	#define LOOP_PERIOD_COUNT 50
	for (;;) { // Uses polling, might switch to interrupts, also ignore write buffer and U1TXREG
		// delay(1000);
		tim_ticks = timer2_get_cnt();
		//MCP4921_write((sine_table[i] + sine_table[j] + sine_table[k] + sine_table[m]) >> 3);
		MCP4921_write(sine_table[i]);

		//i = i & 0x3ff;
		//i += 50000000;
		i_acc += 600000000;
		i = i_acc >> 22;


		/*
			While operating in 32-bit mode, the SIDL bit (TxCON<13>) of consecutive odd number timers 
			of the 32-bit timer pair has an affect on the timer operation. 
			All other bits in this register have no affect.
		*/
		while ((timer2_get_cnt() - tim_ticks) < LOOP_PERIOD_COUNT);
		/*
		2^32 = 4294967296 / 50000000 = 85.89934592
		(2^32 / i) * looptime = 0.00004 * 85.89934592 = 0.00343597383 s => 291.038305143 Hz
		

		*/
		/*
		i = i & 0x3ff;
		++i;
		*/
		// unsigned char tmp;
		// while(!(U1STA & 0x1)); //wait for read buffer to have a value
		// delay(1000000);
		// tmp = U1RXREG & 0xFFFFFF; // Går detta för att hämta ut 24 bits?
		// while(U1STA & (1 << 9)); //make sure the write buffer is not full 
		// U1TXREG = tmp;
		// PORTE = tmp;
		// if (midi_msg_cnt >= 3)
		// {
		// 	for (i = 0; i < 3; ++i)
		// 	{
		// 		// while(U1STA & (1 << 9));
		// 		// U1TXREG = midi_buff[i];
		// 		midibuff[i] = ~midibuff[i];
		// 	}
		// 	midi_msg_cnt = 0;
		// 	char status = midi_buff[0];
		// 	char key = midibuff[1];
		// 	char vel = midibuff[2];
		// 	message_handler(status, key, vel);
		// }
		// delay(1000000);
		// PORTE ^= (1 << 4);
	}

	return 0;
}

