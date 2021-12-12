#include <pic32mx.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "waveforms.h"
#include "midi_freq.h"
#include "uart.h"
#include "dac.h"


#define BUFFER_SIZE 1024
#define SAMPLE_RATE 22050
#define SAMPLE_DURATION (1.0f / SAMPLE_RATE)
#define NUM_OSCILLATORS 9

#define ATTACK 60
#define RELEASE 10
#define ENVELOPE_MAX 0xefff

#define ENV_ENABLE

int envelope_attack = ATTACK;
int envelope_release = RELEASE;

int16_t* wave_table = sine_table;

typedef enum
{
	attack_s = 0,
	sustain_s,
	release_s
}osc_state_e;

struct Oscillator {
	int key;
	int indexAdder;
	unsigned int index;
	bool running;
	osc_state_e state;
	int attack_cnt;
	int release_cnt;
	bool note_off;
	int envelope_max;
};
struct Oscillator osc[NUM_OSCILLATORS] = {0};

void osc_init(void)
{
	int i;
	for(i = 0; i < NUM_OSCILLATORS; i++)
	{
		osc[i].running = false;
		osc[i].note_off = false;
		osc[i].envelope_max = ENVELOPE_MAX;
	}
}

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}


void handleNoteOn(int key, int vel) {
	int i;
	int key_index = 0;
	bool index_found = false;

	for (i = 0; i < NUM_OSCILLATORS; i++) {
		if (osc[i].key == key)
		{
			key_index = i;
			index_found = true;
			break;
		}
	}
	for (i = 0; (i < NUM_OSCILLATORS) && index_found == false; i++) {
		if (osc[i].running == false)
		{
			key_index = i;
			index_found = true;
			break;
		}
	}
	if (index_found)
	{
		osc[key_index].running = true;
		osc[key_index].key = key;
		osc[key_index].indexAdder = 350000 * freq_table[key]; // 175000
		osc[key_index].state = attack_s;
		osc[key_index].attack_cnt = 0x0;
		osc[key_index].note_off = false;
		vel = vel << 9;
		if (vel > 0xefff)
		{
			vel = 0xefff;
		}
		osc[key_index].envelope_max = vel;
	}

	PORTE = key;
}

void handleNoteOff(int key) {
	int i;
	for (i = 0; i < NUM_OSCILLATORS; i++) {
		if (osc[i].key == key) {
			osc[i].note_off = true;
			osc[i].release_cnt = osc[i].envelope_max;
			osc[i].key = 0;
		}
	}
}

unsigned int polyphony(int16_t* table) {
	int i;
	int amplitude = 0;
	int envelope_amplitude = 0;
	int envelop_max = 0;
	
	for (i = 0; i < NUM_OSCILLATORS; i++) {
		if (osc[i].running) {
			envelop_max = osc[i].envelope_max;
			osc[i].index += osc[i].indexAdder;
			switch(osc[i].state)
			{
				case attack_s:
					envelope_amplitude = osc[i].attack_cnt;
					osc[i].attack_cnt += envelope_attack;
					if (osc[i].attack_cnt >= envelop_max)
					{
						osc[i].attack_cnt = envelop_max;
						osc[i].state = sustain_s;
					}
				break;

				case sustain_s:
					envelope_amplitude = envelop_max;
					if (osc[i].note_off)
					{
						osc[i].note_off = false;
						osc[i].release_cnt = envelop_max;
						osc[i].state = release_s;
					}
				break;

				case release_s:
					envelope_amplitude = osc[i].release_cnt;
					osc[i].release_cnt -= envelope_release;
					if (osc[i].release_cnt <= 0)
					{
						osc[i].release_cnt = 0;
						osc[i].running = false;
					}
				break;
					/* Error */
				default:
				break;
			}
			amplitude += ((envelope_amplitude >> 6) * table[osc[i].index>>22]) >> 10;
		}
	}

	amplitude >>= 2;
	amplitude += 512;

	return amplitude;
}

void uart1_send(char* data, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		while(!(U1STA & (1 << 8)));
		U1TXREG = data[i];
	}
	while(!(U1STA & (1 << 8)));
	U1TXREG = '\n';
}

void message_handler(char status, char key, char vel)
{
	unsigned int statusBits = (status >> 4) & 0xF;
	switch (statusBits)
	{
	case 8:
		handleNoteOff(key);
		break;
	case 9:
		handleNoteOn(key, vel);
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
	
	TRISD |= 0xFE0; // Set bit 11 through 5 as 1 = input, for buttons and switches
	TRISF |= 1<<1; // Set bit 1 as 1 = input for button 1.

	PORTE = 0;
	timer2_init();
	uart1_init();
	uart2_init();
	MCP4921_init();
	osc_init();
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
#if 1
		midi_buff[midi_msg_cnt++] = midi_message;
		if (midi_msg_cnt == 3)
		{
			message_ready = true;
			midi_msg_cnt = 0;
		}
#else
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
#endif
		
		
		//midi_buff[midi_msg_cnt++] = fifo_reg & 0xff;
		IFS(1) &= ~(1<<9);
	}
}

bool btn1pressed = false;
bool btn2pressed = false;
bool btn3pressed = false;
bool btn4pressed = false;

void modSound( void ) {
	char str[30];
	if (getsw() & 1){

			if (getbtns() & 4) { // Button 4
				
				if (btn4pressed == false)
				{
					btn4pressed = true;
					envelope_attack -= 10;
					//sprintf(str, "envelope_attack: %i", envelope_attack);
					uart1_send((char*)&envelope_attack, 4);
				}
			}
			else
			{
				btn4pressed = false;
			}

			if (getbtns() & 2) { // Button 3
				
				if (btn3pressed == false)
				{
					btn3pressed = true;
					envelope_attack += 10;
					//sprintf(str, "envelope_attack: %i", envelope_attack);
					uart1_send((char*)&envelope_attack, 4);
				}
			}
			else
			{
				btn3pressed = false;
			}

			if (getbtns() & 1)
			{ // Button 2
				if (btn2pressed == false)
				{
					btn2pressed = true;
					envelope_release -= 10;
					//sprintf(str, "envelope_release: %i", envelope_release);
					uart1_send((char*)&envelope_release, 4);				}
			}
			else
			{
				btn2pressed = false;
			}

			if (getbtns1())
			{ // Button 1
				if (btn1pressed == false)
				{
					btn1pressed = true;
					envelope_release += 10;
					//sprintf(str, "envelope_release: %i", envelope_release);
					uart1_send((char*)&envelope_release, 4);
				}
			}
			else
			{
				btn1pressed = false;
			}

			if (envelope_attack < 1) {
				envelope_attack = 1;
			}
			if (envelope_release < 1) {
				envelope_release = 1;
			}
		}
	else 
	{
		if (getbtns() & 4)
		{ // Button 4
			wave_table = sine_table;
		}
		if (getbtns() & 2)
		{ // Button 3
			wave_table = square_table;
		}
		if (getbtns() & 1)
		{ // Button 2
			wave_table = sawtooth_table;
		}
		if (getbtns1())
		{ // Button 1
			wave_table = triangle_table;
		}
	}
}

int main(void) {
	unsigned int tim_ticks = 0;

	init();
	#define LOOP_PERIOD_COUNT 100  // 50
	for (;;) {
		
		tim_ticks = timer2_get_cnt();

		MCP4921_write(polyphony(wave_table));	

		if (message_ready)
		{
			message_ready = false;
			char status = midi_buff[0];
			char key = midi_buff[1];
			char vel = midi_buff[2];
			//uart1_send((char*)midi_buff, 3);
			message_handler(status, key, vel);
		}
		
		modSound();

		while ((timer2_get_cnt() - tim_ticks) < LOOP_PERIOD_COUNT);
	}
	return 0;
}


