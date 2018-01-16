/**
 * @file uart.h
 * @todo  modify uart0_init -> give him baud rate from main
 */

#ifndef UART_H_
#define UART_H_

#include "packet.h"
#include "color.h"

/**
 * @return uart0_filestream (return value of open(...) function)
 */
int uart0_init();


/**
 * Transmits n bytes from tx_buffer declared in main
 * @param *p_tx_buffer - pointer for tx_buffer declared in main
 * @param n - number of bytes to transmit
 */
void uart0_transmit(uint8_t *p_tx_buffer, int n);


/**
 * Reads n bytes from UART input file
 * @param *p_rx_buffer - pointer for rx_buffer declared in main
 * @param n - number of bytes to read
 */
int uart0_receive_bytes(uint8_t *p_rx_buffer, int n);


/**
 * Sends data packet
 * @param *packet - adress of packet which needs to be sent
 */
void uart_send_packet(t_packet* packet);

#endif
