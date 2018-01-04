#ifndef UART_H_
#define UART_H_
#endif

int uart0_init();
int uart0_transmit(int uart0_filestream, unsigned char *p_tx_buffer);
int uart0_receive_bytes(int uart0_filestream, unsigned char *p_rx_buffer);
