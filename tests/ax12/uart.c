/**
 * @file uart.c
 * @brief inits uart to desired baud rate.
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

//#include "packet.h"
#include "color.h"
#include "config.h"
#include "actuator.h"

static int uart0_filestream = -1;
static int uart1_filestream = -1;

static int8_t no_msgs = 1;

int uart0_input_flush(void)
{
	return tcflush(uart0_filestream, TCIFLUSH);
}

int uart1_input_flush(void)
{
	return tcflush(uart1_filestream, TCIFLUSH);
}

int uart0_output_flush()
{
	return tcflush(uart0_filestream, TCOFLUSH);
}

int uart0_io_flush()
{
	return tcflush(uart0_filestream, TCIOFLUSH);
}

void uart1_init (speed_t baud_rate)
{
	uart1_filestream = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode

	if (uart1_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error: %d\n", uart0_filestream);
		printf("For error details, refer to fnctl.h\n");
		printf("Unable to open UART.  Ensure it is not in use by another application\n");
	}

	struct termios options;
	tcgetattr(uart1_filestream, &options);
	options.c_cflag = baud_rate | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	usleep(200000);
	tcflush(uart1_filestream, TCIFLUSH);
	tcsetattr(uart1_filestream, TCSANOW, &options);
}

void uart0_init (speed_t baud_rate)
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
	usleep(200000);
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
}

void uart0_transmit(uint8_t* p_tx_buffer, int n)
{
	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, p_tx_buffer, n);	//Filestream, bytes to write, number of bytes to write
#ifdef DEBUG
		printf("uart0_transmit: count = %d \n", count);
		int i;
		for(i = 0; i < n; i++)
		{
			print_blue();
			printf("Uart: sent: 0x%x '%c' \n",*(p_tx_buffer+i), *(p_tx_buffer+i));
			print_reset();
		}
#endif
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
		printf("uart0_filestream != -1 (uart0_transmit)\n");
	}
}

void uart1_transmit(uint8_t* p_tx_buffer, int n)
{
	if (uart1_filestream != -1)
	{
		int count = write(uart1_filestream, p_tx_buffer, n);	//Filestream, bytes to write, number of bytes to write
#ifdef DEBUG_UART1
		printf("uart1_transmit: count = %d \n", count);
		int i;
		for(i = 0; i < n; i++)
		{
			print_blue();
			printf("Uart1: sent: 0x%x '%c' \n",*(p_tx_buffer+i), *(p_tx_buffer+i));
			print_reset();
		}
#endif
		if (count < 0)
		{
			int val;
			val = fcntl(uart1_filestream, F_GETFL, 0);
			printf("file status = 0x%x\n", val);

			print_red();
			printf("Uart1: \n");
			print_reset();
			perror("TX error: ");
		}
	}
	else
	{
		print_red();
		printf("Uart1:");
		print_reset();
		printf("uart1_filestream != -1 (uart1_transmit) \n");
	}
}

int uart0_receive_byte(uint8_t* p_rx_buffer)
{
	//tcflush(uart0_filestream, TCIFLUSH);

	if (uart0_filestream != -1)
	{
		int rx_length = read(uart0_filestream, p_rx_buffer, 1);
		if(rx_length == -1)
		{
			if(no_msgs)
			{
#ifdef DEBUG
				print_yellow();
				printf("Uart: ");
				print_reset();
				printf("No messages.\n");
#endif
				no_msgs = 0;
				return 0;
			}
		}
		else if (rx_length > -1)
		{
#ifdef DEBUG
			print_blue();
			printf("Uart: ");
			print_reset();
			printf("received byte - 0x%x\n", *p_rx_buffer);
#endif
			no_msgs = 1;
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

int uart1_receive_byte(uint8_t* p_rx_buffer)
{
	//tcflush(uart0_filestream, TCIFLUSH);

	if (uart1_filestream != -1)
	{
		int rx_length = read(uart1_filestream, p_rx_buffer, 1);
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
			printf("Uart1: ");
			print_reset();
			printf("error %d\n", rx_length);
			return 0;
		}
	}
	else
	{
		print_red();
		printf("Uart1: ");
		print_reset();
		printf("uar1_filestream != -1 (uart0_receieve_bytes)");
	}
	return 0;
}

//static t_packet rx_packet;
/*
t_packet* uart_try_read_packet(void)
{
	uint8_t rx_buffer_header[MAX_PKT_SIZE];
	uint8_t rx_buffer_data[MAX_PKT_SIZE-PACKET_HEADER];
	int bytes_num;
	uint8_t crc = 0;
	uint8_t type = 0;
	uint8_t size = 0;
	int data_sum = 0;

	bytes_num = uart0_receive_byte(rx_buffer_header);

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
				bytes_num = uart0_receive_byte(rx_buffer_header+i);
			}while(bytes_num == 0);
		}
		crc  = rx_buffer_header[0];
		type = rx_buffer_header[1];
		size = rx_buffer_header[2];

		if(((size + type) & 0x0F) != (crc >> 4))
		{
			printf("Packet: crc HIGH checksum error\n");
			printf("\t type: %c 0x%x \n", type, type);
			printf("\t crc: %c 0x%x \n", crc, crc);
			printf("\t size: %c 0x%x \n", size, size);
			return 0;
		}
		for(i = 0; i < size; i++)
		{
			do
			{
				bytes_num = 0;
				bytes_num = uart0_receive_byte(rx_buffer_data+i);
			}while(bytes_num == 0);
		}

		//calculate data_sum
		data_sum = 0;
		for(i = 0; i < size; i++)
		{
			data_sum += rx_buffer_data[i];
		}
		if((data_sum & 0x0F) != (crc & 0x0F))
		{
			printf("Packet: crc LOW checksum error\n");
			printf("\t type: %c 0x%x \n", type, type);
			printf("\t crc: %c 0x%x \n", crc, crc);
			printf("\t size: %c 0x%x \n", size, size);
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

	return 0;
}
*/
/*
void uart_send_packet(t_packet* packet)
{
	uint8_t* p_tx_buffer;

	p_tx_buffer = (uint8_t*) (packet);

#ifdef DEBUG
	printf("p_tx_buffer: %p\n", (void*) p_tx_buffer);
	printf("packet: %p\n\n", (void*) packet);
	print_packet(packet);
#endif

	uart0_transmit(p_tx_buffer, PACKET_HEADER + packet->size);
}
*/
void uart_send_axpacket(t_axPacket* packet)
{
	uint8_t* p_tx_buffer = (uint8_t*) packet;
	
	uart1_transmit(p_tx_buffer, packet->length);
	
	#ifdef AX_PKT_DEBUG
	int i=0;
	for(i=0; i<packet->length; i++)
	{
		print_yellow();
		printf("AX: ");
		print_reset();
		printf("%x \n", *(p_tx_buffer+i));
	}
	#endif
}
