#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>

#include <wiringPi.h> //for delay
#include "uart.h"
#include "color.h"
#include "motioncmd.h"
#include "packet.h"

//TRUE  = 1
//FALSE = 0

void init_main()
{
	tx_packets_init();
	uart_pkt_en(TRUE);
}

int main()
{
	init_main();
	unsigned char  tx_buffer[255];
	unsigned char* p_tx_buffer = tx_buffer;
	
	unsigned char  rx_buffer[255];
	unsigned char* p_rx_buffer = rx_buffer;
	
	int uart0_filestream;
	int uart0_bytes_receieved  = 0;
	
	uart0_filestream = uart0_init(B57600);
	printf("(main) uart0_filestream: %d\n", uart0_filestream);
	

	packet_prepare(MOTION_GET_STATUS_AND_POSITION);
	packet_put_byte(150);
	packet_put_word(12345);
	packet_end();
	print_packet(get_selected_tx_packet(0));
	
	
	
	while(1)
	{
			t_packet  rx_pkt;
			t_packet* rx_pkt_ptr = &rx_pkt;
			//int i = 0;
			//int n = 0;
			
			rx_pkt_ptr = try_read_packet();
			if (rx_pkt_ptr != 0)
			{
				print_rx_packet(rx_pkt_ptr);
			}
			delay(500);
			/*
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
			
			uart0_transmit(p_tx_buffer, n);
			delay (1000);	//wait 1 second
			
			uart0_bytes_receieved = uart0_receive_bytes(p_rx_buffer, 255);
			for(i = 0; i < uart0_bytes_receieved; i++)
			{
				print_blue();
				printf("Uart: ");
				print_reset();
				printf("byte - %c\n", rx_buffer[i]);
			}
			delay(1000);
			* */
	}
	return 0;
}
