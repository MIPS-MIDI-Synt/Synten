#include <stdint.h>

                          /* PIC32 signal, CHIPKIT Pin #  */
#define DAC_CS_PIN		1 /* RD1,          5                  */
#define DAC_SDI_PIN		2 /* RD2,          6                  */
#define DAC_SCK_PIN		3 /* RD3,          9                  */


void MCP4921_init(void);

void MCP4921_write(uint16_t data);