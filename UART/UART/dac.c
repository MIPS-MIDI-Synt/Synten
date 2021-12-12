#include <stdint.h>
#include <pic32mx.h>
#include "dac.h"

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

	PORTD &= ~((1 << DAC_SDI_PIN) | (1 << DAC_SCK_PIN)); // ??

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
	/* Gain 2x */
	dac_data |= (0 << 13);
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
