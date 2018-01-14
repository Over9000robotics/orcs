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
typedef struct packet_t
{
	uint8_t sync;
	uint8_t crc;
	uint8_t type;
	uint8_t size;
	uint8_t data[MAX_PKT_SIZE-PACKET_HEADER];
	uint8_t packet_ptr;
	enum PacketStatus status;
}packet_t;

/**
 * \fn Prepares everything except message data 
 */

void packet_prepare(uint8_t type);

#endif

