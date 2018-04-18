/**
 * @file uart_ax.c
 */
#include "uart.h"
#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART func open parameters
#include <termios.h>		//Used for UART

#include "packet.h"
#include "color.h"
#include "config.h"
#include "actuator.h"
#include "uart_ax.h"

static int uart_ax_filestream = -1;
static int8_t no_msgs = 1;

int uart_ax_input_flush(void)
{
	return tcflush(uart_ax_filestream, TCIFLUSH);
}


int uart_ax_output_flush(void)
{
	return tcflush(uart_ax_filestream, TCOFLUSH);
}

void uart_ax_init(speed_t baud_rate)
{
	uart_ax_filestream = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	printf("REMINDER: set ax to ttyUSB1 later \n");
	if (uart_ax_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error: %d\n", uart_ax_filestream);
		printf("For error details, refer to fnctl.h\n");
		printf("Unable to open UART for ax.  Ensure it is not in use by another application\n");
	}

	struct termios options;
	tcgetattr(uart_ax_filestream, &options);
	options.c_cflag = baud_rate | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	usleep(200000);
	tcflush(uart_ax_filestream, TCIFLUSH);
	tcsetattr(uart_ax_filestream, TCSANOW, &options);
}

void uart_ax_transmit(uint8_t* p_tx_buffer, int n)
{
	if (uart_ax_filestream != -1)
	{
		int count = write(uart_ax_filestream, p_tx_buffer, n);	//Filestream, bytes to write, number of bytes to write
#ifdef DEBUG_UART1
		printf("uart_ax_filestream: count = %d \n", count);
		int i;
		for(i = 0; i < n; i++)
		{
			print_blue();
			printf("Uart_ax: sent: 0x%x '%c' \n",*(p_tx_buffer+i), *(p_tx_buffer+i));
			print_reset();
		}
#endif
		if (count < 0)
		{
			int val;
			val = fcntl(uart_ax_filestream, F_GETFL, 0);
			printf("file status = 0x%x\n", val);

			print_red();
			printf("Uart_ax: \n");
			print_reset();
			perror("TX error: ");
		}
	}
	else
	{
		print_red();
		printf("Uart_ax:");
		print_reset();
		printf("uart_ax_filestream != -1 (uart_ax_filestream) \n");
	}
}

int uart_ax_receive_byte(uint8_t* p_rx_buffer)
{
	//tcflush(uart0_filestream, TCIFLUSH);

	if (uart_ax_filestream != -1)
	{
		int rx_length = read(uart_ax_filestream, p_rx_buffer, 1);
		if(rx_length == -1)
		{
			if(no_msgs)
			{
#ifdef DEBUG_UART1
				print_yellow();
				printf("Uart1: ");
				print_reset();
				printf("No messages.\n");
#endif
				no_msgs = 0;
				return 0;
			}
		}
		else if (rx_length > -1)
		{
#ifdef DEBUG_UART1
			print_blue();
			printf("Uart1: ");
			print_reset();
			printf("received byte - 0x%x\n", *p_rx_buffer);
#endif
			no_msgs = 1;
			return rx_length;
		}
		else
		{
			print_red();
			printf("Uart_ax: ");
			print_reset();
			printf("error %d\n", rx_length);
			return 0;
		}
	}
	else
	{
		print_red();
		printf("Uart_ax: ");
		print_reset();
		printf("uart_ax_filestream != -1\n");
	}
	return 0;
}

void uart_send_axpacket(t_axPacket* packet)
{
	uint8_t* p_tx_buffer = (uint8_t*) (packet);

	*(p_tx_buffer+packet->length+3) = packet->checksum;
	
	uart_ax_transmit(p_tx_buffer, packet->length+4);
	
	#ifdef AX_PKT_DEBUG
	printf("\n");
	int i=0;
	for(i=0; i<packet->length + 4; i++)
	{
		print_yellow();
		printf("AX: ");
		print_reset();
		printf("0x%x \n", *(p_tx_buffer+i));
	}
	#endif
}
