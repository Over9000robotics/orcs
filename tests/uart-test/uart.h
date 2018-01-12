#ifndef UART_H_
#define UART_H_

int uart0_init();
void uart0_transmit(int uart0_filestream, unsigned char *p_tx_buffer, int n);
int uart0_receive_bytes(int uart0_filestream, unsigned char *p_rx_buffer, int n);

#endif
