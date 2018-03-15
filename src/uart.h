/**
 * @file uart.h
 */
#ifndef UART_H_
#define UART_H_

#include "packet.h"
#include "stdint.h"
#include <termios.h>

int uart0_input_flush(void);
int uart1_input_flush(void);

int uart0_output_flush();

int uart0_io_flush();

void uart1_init (speed_t baud_rate);
void uart1_transmit(uint8_t* p_tx_buffer, int n);
int uart1_receive_byte(uint8_t* p_rx_buffer);

/**
 * Initialises uart to selected baud rate
 * @param baud_rate - desired baud rate
 */
void uart0_init (speed_t baud_rate);

/**
 * Transmits n bytes from tx_buffer declared in main
 * @param *p_tx_buffer - pointer for tx_buffer declared in main
 * @param n - number of bytes to transmit
 */
void uart0_transmit(uint8_t *p_tx_buffer, int n);


/**
 * Reads n bytes from UART input file
 * @param *p_rx_buffer - pointer for rx_buffer declared in main
 * @return number of success read bytes
 */
int uart0_receive_byte(uint8_t *p_rx_buffer);

/**
 * Trys to read incoming packet
 * @return packet adress if packet received, 0 if not received
 */
t_packet* uart_try_read_packet(void);


/**
 * Sends data packet
 * @param *packet - adress of packet which needs to be sent
 */
void uart_send_packet(t_packet* packet);

#endif
