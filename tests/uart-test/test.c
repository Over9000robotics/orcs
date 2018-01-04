#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h> //for delay
#include "uart.h"

int main()
{
	unsigned char* p_tx_buffer;
	unsigned char* p_rx_buffer;
	
	int uart0_filestream;
	int uart0_transmit_success = 0;
	int uart0_receive_success = 0;
	
	uart0_filestream = uart0_init();
	printf("(main) uart0_filestream: %d\n", uart0_filestream);
	
	while(1)
	{
			uart0_transmit_success = uart0_transmit(uart0_filestream, 
														p_tx_buffer);
			delay (1000);	//wait 1 second
			
			uart0_receive_success = uart0_receive_bytes(uart0_filestream, 
														p_rx_buffer);
			delay(1000);
	}
	return 0;
}
