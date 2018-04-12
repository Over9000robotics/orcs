#ifndef UART_AX_H_
#define UART_AX_H_

int uart_ax_input_flush(void);
int uart_ax_output_flush(void);
void uart_ax_init(speed_t baud_rate);
void uart_ax_transmit(uint8_t* p_tx_buffer, int n);
int uart_ax_receive_byte(uint8_t* p_rx_buffer);
void uart_send_axpacket(t_axPacket* packet);

#endif
