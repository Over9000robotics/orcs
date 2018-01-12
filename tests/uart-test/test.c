#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h> //for delay
#include "uart.h"
#include "color.h"

int main()
{
	unsigned char  tx_buffer[255];
	unsigned char* p_tx_buffer = tx_buffer;
	
	unsigned char  rx_buffer[255];
	unsigned char* p_rx_buffer = rx_buffer;
	
	int uart0_filestream;
	int uart0_bytes_receieved  = 0;
	
	uart0_filestream = uart0_init();
	printf("(main) uart0_filestream: %d\n", uart0_filestream);
	
	while(1)
	{
			int i = 0;
			int n = 0;
			unsigned char c;
			printf("Type character array to send: \n");
			
			do
			{
				scanf("%c",&c);
				tx_buffer[i++] = c;
			}
			while(c != 'e');
			n = i;
			printf("\n");
		
			printf("Typed array: \n");
			print_blue();
			for(i = 0; i < n; i++)
			{
				printf("%c", tx_buffer[i]);
			}
			print_reset();
			printf("\n");
			
			uart0_transmit(uart0_filestream, p_tx_buffer, n);
			delay (1000);	//wait 1 second
			
			uart0_bytes_receieved = uart0_receive_bytes(uart0_filestream, p_rx_buffer, 255);
			delay(1000);
	}
	return 0;
}
