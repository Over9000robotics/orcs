/**
 * @file packet.h
 * @brief Packet status tracking enumeration	\n
 * Packet structure	\n
 * Functions for manipulation with packets
 */
#ifndef PACKET_H_
#define PACKET_H_

#include <stdint.h>	

#define MAX_PKT_SIZE 32
#define PACKET_HEADER 4
#define MAX_TX_PACKETS 10
#define PACKET_SYNC 0x3c

enum PacketStatus {
	free_to_use,
	writing_packet,
	ready_to_send,
	sending
};

 /**
  * Motion UART packet
  *  SCTLxxxxxxxx
  *
  *  S - 1 Byte sync (0x3c)	\n
  *  C - 1 Byte checksum ( upper nibble - header checksum, lower\n nibble payload checksum )	\n
  *  T - 1 Byte type	\n
  *  L - 1 Byte payload length	\n
  *  x - L Bytes data	
  */
typedef struct t_packet
{
	uint8_t sync;
	uint8_t crc;
	uint8_t type;
	uint8_t size;
	uint8_t data[MAX_PKT_SIZE-PACKET_HEADER];
	uint8_t packet_byte_selector;
	enum PacketStatus status;
}t_packet;


/**
 *  Enables / disables uart using (second option would be CAN)
 * 	and it will be implemented later.
 *  @param uen 1 - enabled, 0 - disabled
 */
void uart_pkt_en(uint8_t uen);


/**
 *  Initialises packet status to "free_to_use"
 */
void tx_packets_init(void);


/**
 *   Looks for free packet
 * 	 @return free packet adress
 */
t_packet* find_free_packet(void);


/**
 * Prepares everything except message data 
 * @param type - type of command (defined in motioncmd.h)
 */
void packet_prepare(uint8_t type);


#endif

