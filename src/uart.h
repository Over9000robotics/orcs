/**
 * file @uart.h
 * @todo  modify uart0_init -> give him baud rate from main
 */

#ifndef UART_H_
#define UART_H_


int uart0_init();


/**
 * Transmits n bytes from tx_buffer declared in main
 * @param uart0_filestream - return value of open(...) function
 * @param *p_tx_buffer - pointer for tx_buffer declared in main
 * @param n - number of bytes to transmit
 */
void uart0_transmit(int uart0_filestream, unsigned char *p_tx_buffer, int n);


/**
 * @param uart0_filestream - return value of open(...) function
 * @param *p_rx_buffer - pointer for rx_buffer declared in main
 * @param n - number of bytes to read
 */
int uart0_receive_bytes(int uart0_filestream, unsigned char *p_rx_buffer, int n);

#endif
