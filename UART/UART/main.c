#include <pic32mx.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "waveforms.h"
#include "midi_freq.h"
#include "uart.h"
#include "dac.h"


#define BUFFER_SIZE 1024
#define SAMPLE_RATE 22050
#define SAMPLE_DURATION (1.0f / SAMPLE_RATE)
#define NUM_OSCILLATORS 9

volatile int freq = 0;

struct Oscillator {
	int key;
	int indexAdder;
	unsigned int index;
	bool running;
};
struct Oscillator osc[NUM_OSCILLATORS] = {0};

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}


void handleNoteOn(int key) {
	int i;
	for (i = 0; i < NUM_OSCILLATORS; i++) {
		if (osc[i].running == false) {
			osc[i].running = true;
			osc[i].key = key;
			freq = freq_table[key];
			PORTE = key;
			osc[i].indexAdder = 350000*freq; //175000
			break;
		}
	}
}

void handleNoteOff(int key) {
	int i;
	for (i = 0; i < NUM_OSCILLATORS; i++) {
		if (osc[i].key == key) {
			osc[i].running = false;
			break;
		}
	}
}

unsigned int polyphony(uint16_t* table) {
	int i;
	unsigned int amplitude = 0;
	int numOscillators = 0;
	for (i = 0; i < NUM_OSCILLATORS; i++) {
		if (osc[i].running) {
			numOscillators++;
			osc[i].index += osc[i].indexAdder;
			amplitude += table[osc[i].index>>22];
		}
	}
	amplitude /= numOscillators;

	return amplitude;
}


void message_handler(char status, char key, char vel){

	unsigned int statusBits = (status >> 4)&0xF;
	U1TXREG = statusBits;
    switch(statusBits) {
		case 8:
			handleNoteOff(key);
			break;
		case 9:
			handleNoteOn(key);
			break;
		default:
			break;
	}
	
}





void timer2_init() {

	T2CONCLR = 0x8000; // Turn off timer
	T2CON = 0x5<<4; // Change bit 6-4 (TCKPS) to config prescale 1:32, set bits to: 101 
	/* Prescaler of 32 gives a clock to tim2 of 80MHz/32 = 2.5 MHz */
  	T2CONSET = 0x8000; // Turn on timer 

}

unsigned int timer2_get_cnt(void)
{
	//unsigned int tim_cnt = ((TMR3 << 16) & 0xffff0000) | (TMR2 & 0x0000ffff);
	//return tim_cnt;
	return TMR2;
}

void init() {
	// TRISD = TRISD | 0xFE0; // Set BTNs as input
	/* On Uno32, we're assuming we're running with sysclk == 80 MHz */
	/* Periphial bust can run at a maximum of 40 MHz, setting PBDIV to 1 divides sysclk with 2 */
	OSCCON &= ~0x180000; // Reset pbdiv to 0
	OSCCON |= 0x080000; // Set pbdiv to 1
	// ODCE = 0; // Push-Pull output
	// TRISECLR = 0xFF; // Låt va så länge,sätt alla pins till output
	TRISE = 0x0; 
	// TRISF = 0xFFFFFFFF;
	TRISF = 0x00000000;
	
	PORTE = 0;
	timer2_init();
	uart1_init();
	uart2_init();
	MCP4921_init();
}

volatile char midi_buff[3];
volatile int midi_msg_cnt = 0;
volatile bool message_reciving = false;
volatile int message_size = 0;
volatile bool message_ready = false;

void uart_isr( void ){
	unsigned char tmp;
	unsigned char midi_message = 0;
	int i = 0;

	if(IFS(1) & 1<<9){	// UART1: IFS(0) & 0x08000000
		midi_message = U2RXREG;
		if(message_reciving == false){
			if((midi_message & 0xf0) == 0xc0 || (midi_message & 0xf0) == 0xd0){
				message_size = 2;
			}
			else{
				message_size = 3;
			}
			message_reciving = true;
			midi_buff[midi_msg_cnt++] = midi_message;

		}
		else{
			midi_buff[midi_msg_cnt++] = midi_message;
			if(midi_msg_cnt == message_size){
				message_ready = true;
				midi_msg_cnt = 0;
			}
		}
		
		
		//midi_buff[midi_msg_cnt++] = fifo_reg & 0xff;
		PORTE =  midi_message;
		IFS(1) &= ~(1<<9);
	}
}

void buttonPlay( void ) {
	// int btn4 = getbtns() & 4; // btn4 = 1xx
	// int btn3 = getbtns() & 2; // btn3 = x1x
	// int btn2 = getbtns() & 1; // btn2 = xx1
	int sw1 = getsw() & 1;
	int sw2 = getsw() & 2;
	int sw3 = getsw() & 4;
	int sw4 = getsw() & 8;

	if (sw1) {
		message_handler(0x9, 12, 0x1); // 
	}
	if (sw2) {
		message_handler(0x9, 34, 0x1); // 
	}
	if (sw3) {
		message_handler(0x9, 56, 0x1); // 
	}
	if (sw4) {
		message_handler(0x9, 80, 0x1); // 
	}
	
}

int main(void) {
	unsigned int tim_ticks = 0;

	init();
	#define LOOP_PERIOD_COUNT 100  // 50
	for (;;) {

		// buttonPlay();
		// if (getsw() & 1) { // If switch 1, play with buttons instead
		// 	buttonPlay(); // SPELA MED KNAPPAR
		// }	
		
		tim_ticks = timer2_get_cnt();

		MCP4921_write(polyphony(sine_table));	

		if (message_ready)
		{
			message_ready = false;
			char status = midi_buff[0];
			char key = midi_buff[1];
			char vel = midi_buff[2];
			message_handler(status, key, vel);
		}
		
		
		while ((timer2_get_cnt() - tim_ticks) < LOOP_PERIOD_COUNT);


	}

	return 0;
}


