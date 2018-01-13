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

 /*
 * Motion UART packet
 *  SCTLxxxxxxxx
 *
 *  S - 1 Byte sync (0x3c)
 *  C - 1 Byte checksum ( upper nibble - header checksum, lower nibble payload checksum )
 *  T - 1 Byte type
 *  L - 1 Byte payload length
 *  x - L Bytes data
 */
typedef struct packet_t
{
	uint8_t sync;
	uint8_t crc;
	uint8_t type;
	uint8_t size;
	uint8_t data[MAX_PKT_SIZE-PACKET_HEADER];
	uint8_t cursor;
	enum PacketStatus status;
}packet_t;

#endif

