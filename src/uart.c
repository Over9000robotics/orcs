/**
 * @file uart.c
 * @brief inits uart to baud rate 115200. 
 * Functions for uart data manipulation.
 */
//NOTE: what to do with characters with parity error (IGNPAR)?
//TODO: Implement sending baud rate as a func parameter, ref: termios.h
//TODO: Implement selection for number of bytes to receieve
//TODO: Implement close(uart0_filestream);

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART func open parameters
#include <termios.h>		//Used for UART
#include "uart.h"

static int uart0_filestream = -1;

int uart0_init (/*t_speed baud_rate*/)
{
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error: %d\n", uart0_filestream);
		printf("For error details, refer to fnctl.h\n");
		printf("Unable to open UART.  Ensure it is not in use by another application\n");
	} 
	
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
	
	return uart0_filestream;
}

void uart0_transmit(uint8_t* p_tx_buffer, int n)
{
	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, p_tx_buffer, n);	//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			print_red();
			printf("Uart: ");
			print_reset();
			printf("TX error\n");
		}
	}
	else
	{
		print_red();
		printf("Uart:");
		print_reset();
		printf("uart0_filestream != -1 (uart0_transmit)");
	}
}

int uart0_receive_bytes(uint8_t* p_rx_buffer, int n)
{
	
	if (uart0_filestream != -1)
	{
		int rx_length = read(uart0_filestream, p_rx_buffer, n);
		if(rx_length == -1)
		{
			print_yellow();
			printf("Uart: ");
			print_reset();
			printf("No messages.\n");
			return 0;
		}
		else if (rx_length > -1)
		{
			p_rx_buffer[rx_length] = '\0';
			print_blue();
			printf("Uart: ");
			print_reset();
			printf("%d bytes read - %s\n", rx_length, p_rx_buffer);
			return rx_length;
		}
		else
		{
			print_red();
			printf("Uart: ");
			print_reset();
			printf("error %d\n", rx_length);
			return 0;
		}
	}
	else
	{
		print_red();
		printf("Uart: ");
		print_reset();
		printf("uart_filestream != -1 (uart0_receieve_bytes)");
	}
	return 0;
}

void uart_send_packet(t_packet* packet)
{
	uint8_t* p_tx_buffer;
	
	p_tx_buffer = (uint8_t*) (packet);
	
	printf("p_tx_buffer: %p\n", (void*) p_tx_buffer);
	printf("packet: %p\n\n", (void*) packet);
	
	uart0_transmit(p_tx_buffer, PACKET_HEADER + packet -> size);
}
