//NOTE: what to do with characters with parity error (IGNPAR)?
//TODO: Implement sending baud rate as a func parameter, ref: termios.h
//TODO: Implement selection for number of bytes to receieve
//TODO: Implement close(uart0_filestream);

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART func open parameters
#include <termios.h>		//Used for UART
#include "uart.h"
#include "color.h"

int uart0_init (/*t_speed baud_rate*/)
{
	printf("INSIDE OF uart0_init\n");
	//-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD
	// (ie the alt0 function) respectively
	int uart0_filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. 
	//								When set read requests on the file 
	//								can return immediately with a failure status
	//								if there is no input immediately available 
	//								(instead of blocking). Likewise, write requests 
	//								can also return immediately with a failure 
	//								status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause
	//  the terminal device to become the controlling terminal for the process.
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error: %d\n", uart0_filestream);
		printf("For error details, refer to fnctl.h\n");
		printf("Unable to open UART.  Ensure it is not in use by another application\n");
	} 
	
	printf("infunc uart0_filestream: %d\n", uart0_filestream);

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, 
	//	B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want 
	//			to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
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

void uart0_transmit(int uart0_filestream, unsigned char* p_tx_buffer, int n)
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

int uart0_receive_bytes(int uart0_filestream, unsigned char *p_rx_buffer, int n)
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
