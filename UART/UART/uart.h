#ifndef __UART_H__ 
#define __UART_H__

int calculate_baudrate_divider(int sysclk, int baudrate, int highspeed);

void uart1_init(void);

void uart2_init(void);

#endif