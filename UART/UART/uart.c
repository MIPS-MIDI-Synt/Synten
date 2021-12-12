#include <pic32mx.h>





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

void uart1_init(void)
{
	/* Configure UART1 for 115200 baud, no interrupts */
	U1BRG = calculate_baudrate_divider(80000000, 31250, 0); // Initializes U1BRG register for 31250 baud
	U1STA = 0; // 
	/* 8-bit data, no parity, 1 stop bit */
	U1MODE = 0x8000; 
	/* Enable transmit and recieve */
	U1STASET = 0x1400;	//

}

void uart2_init(void)
{
	/* Configure UART1 for 115200 baud, no interrupts */
	U2BRG = calculate_baudrate_divider(80000000, 31250, 0); // Initializes U1BRG register for 31250 baud
	U2STA = 0; // 
	/* 8-bit data, no parity, 1 stop bit */
	U2MODE = 0x8000; 
	/* Enable transmit and recieve */
	U2STASET = 0x1400;	//


	// Interrupts:
    
	IEC(1) |= 1 << 9; // Interrupt enable bit 27 in IEC0 
	IPC(8) |= 0x1F; // Interupt priority max = 7 IPC8<4:2>, sup-priority max = 3 IPC8<1:0>

	// OBS: Below conflicting with row 44,46
	U2STA &= ~0xC0;
	U2STA |= 0x00; // U1STA<7:6> Receive interrupt mode = 10 > Flag when buffer becomes 3/4 (24bits = One MIDI message)
	// Interrupt flag @ IFS0<27>
	// xxxx xxxx xxxx xxxx xxxx xxxx 00xx xxxx
	enable_interrupt();
}
