/**
 * @file packet.c
 * @todo try_read_packet
 */
 
#include "packet.h"

#include <stdio.h>
#include <stdint.h>	

#include "color.h"
#include "uart.h"
#include "config.h"

static t_packet* rx_packet = 0;
static t_packet tx_packets[MAX_TX_PACKETS];
static t_packet* tx_packet_ptr = tx_packets;
static int8_t tx_free_packets = 10;
static uint8_t uart_enabled = 0;

void uart_pkt_en(uint8_t uen)
{
	if(uen)	/*if uen != 0*/
	{
		uart_enabled = 1;
	}
	else
	{
		uart_enabled = 0;
	}
	print_yellow();
	printf("Uart: ");
	
	if(uart_enabled)
	{
		print_blue();
		printf("ENABLED\n");
	}
	else
	{
		print_red();
		printf("NOT ENABLED\n");
	}
	print_reset();
}

void tx_packets_init(void)
{
	int i = 0;
	for (i = 0; i < MAX_TX_PACKETS; i++)
	{
		tx_packets[i].status = free_to_use;
	}
}

t_packet* find_free_packet(void)
{
	int i = 0;
	
	for(i = 0; i < MAX_TX_PACKETS; i++)
	{
			if(tx_packets[i].status == free_to_use)
			{
				return &tx_packets[i];
			}
	}
	return 0;
}

t_packet* try_read_packet(void)
{
	rx_packet = 0;
	if(uart_enabled == 1)
	{
		rx_packet = uart_try_read_packet();

		if(rx_packet != 0)
		{
#ifdef DEBUG
			print_yellow();
			printf("Packet: \n");
			print_reset();
#endif
		}

	}
	return rx_packet;
}

void packet_prepare(uint8_t type)
{
	if(tx_free_packets > 0)	//Find free package
	{
		tx_packet_ptr = find_free_packet(); // tx_packet_ptr = &tx_packets[i]
	
		if(tx_packet_ptr)
		{	
			tx_free_packets--;
			tx_packet_ptr -> status = writing_packet;
			tx_packet_ptr -> type = type;
			tx_packet_ptr -> size = 0;
			
			/*
			printf("\ntx_free_packets: %d\n",tx_free_packets);
			printf("tx_packet_ptr -> status: %d\n", tx_packet_ptr -> status);	
			*/
		}
		else
		{
			print_red();
			printf("Packet: ");
			print_reset();
			printf("No free packet.\n");
			return;
		}
	}
	else
	{
		print_red();
		printf("Packet: NO FREE PACKET\n");
		print_reset();
	}
}

void packet_put_byte(int8_t byte)
{
	if (tx_packet_ptr == 0) return;
	tx_packet_ptr -> data[tx_packet_ptr -> size++] = byte;
}

void packet_put_word(int16_t word)
{
	if (tx_packet_ptr == 0) return;
	tx_packet_ptr -> data[tx_packet_ptr -> size++] = word >> 8;
	tx_packet_ptr -> data[tx_packet_ptr -> size++] = word;
}

void packet_end(void)
{
	int crc = 0;
	int i;
	
	if (tx_packet_ptr == 0)
	{
		print_yellow();
		printf("Packet: ");
		print_reset();
		printf("packet not sent - tx_packet_ptr = 0\n");
		return;
	}
	else
	{
		tx_packet_ptr -> status = ready_to_send;
		tx_packet_ptr -> sync = PACKET_SYNC;
		
		for(i = 0; i < tx_packet_ptr -> size; i++)
		{
			crc += tx_packet_ptr -> data[i]; 
		}
		tx_packet_ptr -> crc = ((tx_packet_ptr -> size + tx_packet_ptr -> type) << 4) | (crc & 0x0F);
		tx_packet_ptr -> status = sending;
		
		uart_send_packet(tx_packet_ptr);
		
		//tx_packet_ptr -> status = free_to_verify;

		tx_packet_ptr -> status = free_to_use;
		tx_free_packets++;

	}
}

void packet_verify(t_packet* packet)
{
#ifdef DEBUG
	printf("Inside of packet_verify \n");
#endif
	int i;
	for(i=0; i<MAX_TX_PACKETS; i++)
	{
		if(tx_packets[i].status == free_to_verify)
		{
			if(tx_packets[i].type == packet -> data[0])
			{
#ifdef DEBUG_VERIFY
				print_green();
				printf("Packet: '%c' verified \n", tx_packets[i].type);
				print_packet(&tx_packets[i]);
				print_reset();
#endif
				tx_packets[i].status = free_to_use;
				tx_free_packets++;
				return;
			}
			else
			{
#ifdef DEBUG_VERIFY
				print_red();
				printf("Packet to resend: \n");
				print_packet(&tx_packets[i]);
#endif
				uart_send_packet(&tx_packets[i]);
			}
		}
	}
}

t_packet* get_selected_tx_packet(uint8_t select)
{
	return &tx_packets[select];
}

void print_packet(t_packet* packet)
{
	int i = 0;
	print_blue();
	printf("\nPacket number: 0x%x\n", packet - &tx_packets[0]);
	//printf("sync:   0x%x\n", packet -> sync);
	//printf("chsum:  0x%x\n", packet -> crc);
	//printf("status: 0x%x\n", packet -> status);
	printf("type:   %c\n", packet -> type);
	printf("size:   0x%x\n", packet -> size);
	for(i = 0; i < packet -> size; i++)
	{
		printf("data[%d]: 0x%x \n", i, packet -> data[i]);
	}
	printf("\n");
	print_reset();
}

void print_rx_packet(t_packet* packet)
{
	int i = 0;
	printf("\n");
	print_yellow();
	//printf("sync:   0x%x\n", packet -> sync);
	//printf("chsum:  0x%x\n", packet -> crc);
	//printf("status: 0x%x\n", packet -> status);
	printf("type:   %c\n", packet -> type);
	printf("size:   0x%x\n", packet -> size);
	for(i = 0; i < packet -> size; i++)
	{
		printf("data[%d]: 0x%x \n", i, packet -> data[i]);
	}
	printf("\n");
	print_reset();
}

