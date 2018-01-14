/**
 * @file packet.c
 */

#include <stdio.h>
#include <stdint.h>
#include "packet.h"
#include "color.h"
#include "uart.h"

static t_packet tx_packets[MAX_TX_PACKETS];
static t_packet* tx_packet_ptr = tx_packets;
static int8_t free_packets = 10;
static uint8_t uart_enabled;

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

void packet_prepare(uint8_t type)
{
	if(free_packets > 0)	//Find free package
	{
		tx_packet_ptr = find_free_packet(); // tx_packet_ptr = &tx_packets[i]
	
		if(tx_packet_ptr)
		{	
			free_packets--;
			tx_packet_ptr -> status = writing_packet;
			tx_packet_ptr -> type = type;
			tx_packet_ptr -> size = 0;
			
			printf("\nfree_packets: %d\n",free_packets);
			printf("tx_packet_ptr -> status: %d\n", tx_packet_ptr -> status);	
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
