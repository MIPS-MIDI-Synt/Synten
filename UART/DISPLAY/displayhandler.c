#include <pic32mx.h>

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}
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

void init() {
	/* On Uno32, we're assuming we're running with sysclk == 80 MHz */
	/* Periphial bust can run at a maximum of 40 MHz, setting PBDIV to 1 divides sysclk with 2 */
	OSCCON &= ~0x180000; // Reset pbdiv to 0
	OSCCON |= 0x080000; // Set pbdiv to 1
}

int main(void) {
	unsigned char tmp;
	delay(10000000);
	ODCE = 0; // Bara output?
	TRISECLR = 0xFF; // ??

	init();
	
	/* Configure UART1 for 115200 baud, no interrupts */
	U1BRG = calculate_baudrate_divider(80000000, 31250, 0); // Initializes U1BRG register for 31250 baud
	U1STA = 0; // Varför nollas denna?
	/* 8-bit data, no parity, 1 stop bit */
	U1MODE = 0x8000; // Enable UART module by setting the ON bit
	/* Enable transmit and recieve */
	U1STASET = 0x1400;	// Only enable receive

	// Interrupts:
	IEC(0) |= 0x08000000; // Interrupt enable bit 27 in IEC0 
	IPC(6) |= 0x1F; // Interupt priority max = 7 IPC6<4:2>, sup-priority max = 3 IPC6<1:0>
	// OBS: Below conflicting with row 44,46
	U1STA |= 0xC0; // Receive interrupt mode = 11 > Flag when buffer becomes full (One word = One MIDI message)
	// Interrupt flag @ IFS0<27>
	// xxxx xxxx xxxx xxxx xxxx xxxx 11xx xxxx

	for (;;) { // Uses polling, might switch to interrupts, also ignore write buffer and U1TXREG
		while(!(U1STA & 0x1)); //wait for read buffer to have a value
		tmp = U1RXREG & 0xFF;
		while(U1STA & (1 << 9)); //make sure the write buffer is not full 
		U1TXREG = tmp;
		PORTE = tmp;
	}

	return 0;
}

