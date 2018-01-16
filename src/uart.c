/**
 * @file uart.c
 * @brief inits uart to baud rate 57600. 
 * Functions for uart data manipulation.
 * @todo what if checksum error (uart_try_read_packet)
 * @todo if byte reading is too long -> write WARNING!
 */
 
//TODO: Implement close(uart0_filestream);

#include "uart.h"
#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART func open parameters
#include <termios.h>		//Used for UART

#include "packet.h"
#include "color.h"

static int uart0_filestream = -1;

int uart0_init (speed_t baud_rate)
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
	options.c_cflag = baud_rate | CS8 | CLOCAL | CREAD;		//<Set baud rate
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

static t_packet rx_packet;

t_packet* uart_try_read_packet(void)
{
	uint8_t rx_buffer_header[MAX_PKT_SIZE];
	uint8_t rx_buffer_data[MAX_PKT_SIZE-PACKET_HEADER];
	int bytes_num;
	uint8_t crc = 0;
	uint8_t type = 0;
	uint8_t size = 0;
	int data_sum = 0;
	
	bytes_num = uart0_receive_bytes(rx_buffer_header, 1);
	
	//if 1 byte read success, and that byte is PACKET_SYNC, continue reading
	if(bytes_num == 1 && rx_buffer_header[0] == PACKET_SYNC)
	{
		//proveriti da li se sme odmah nastaviti citanje
		uint8_t i = 0;
		//read: type, crc, size (PACKET_SYNC has already been read)
		for(i = 0; i < PACKET_HEADER-1; i++)
		{
			do
			{
				bytes_num = 0;
				bytes_num = uart0_receive_bytes(rx_buffer_header+i, 1);
			}while(bytes_num == 0);
		}
		crc  = rx_buffer_header[0];
		type = rx_buffer_header[1];
		size = rx_buffer_header[2];
		
		printf("crc:  0x%x \n", crc);
		printf("type: 0x%x \n", type);
		printf("size: 0x%x \n", size);
		
		if(((size + type) & 0x0F) != (crc >> 4))
		{
			printf("crc HIGH checksum error");
			return 0;
		}
		for(i = 0; i < size; i++)
		{
			do
			{
				bytes_num = 0;
				bytes_num = uart0_receive_bytes(rx_buffer_data+i, 1);
			}while(bytes_num == 0);
		}
		
		//calculate data_sum
		data_sum = 0;
		for(i = 0; i < size; i++)
		{
			data_sum += rx_buffer_data[i];
			printf("data[%d]: 0x%x \n",i, rx_buffer_data[i]);
		}
		if(data_sum != (crc & 0x0F))
		{
			printf("crc LOW checksum error");
			return 0;
		}
		rx_packet.sync = PACKET_SYNC;
		rx_packet.crc  = crc;
		rx_packet.type = type;
		rx_packet.size = size;
		for(i = 0; i < size; i++)
		{
			rx_packet.data[i] = rx_buffer_data[i];
		}
		
		return &rx_packet;
	}
	else
	{
		return 0;
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
