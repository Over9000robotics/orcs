/**
 * @file packet.h
 * @brief Packet status tracking enumeration	\n
 * @todo pacet is free when verification is over
 * Packet structure	\n
 * Functions for manipulation with packets
 */
#ifndef PACKET_H_
#define PACKET_H_


#define MAX_PKT_SIZE 32
#define PACKET_HEADER 4
#define MAX_TX_PACKETS 10
#define PACKET_SYNC 0x3c

#include <stdint.h>

enum PacketStatus {
	free_to_use,
	writing_packet,
	ready_to_send,
	sending,
	free_to_verify
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
	enum PacketStatus status;
}t_packet;


/**
 *  Enables / disables uart using (second option would be CAN)
 * 	and it will be implemented later.
 *  @param uen 1 - enabled, 0 - disabled
 */
void uart_pkt_en(uint8_t uen);

/**
 * Trys to read data packet
 * @return packet adress if found, or 0 if packet not found
 */
t_packet* try_read_packet(void);


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

/**
 * Puts byte in data array of message packet
 * @param byte - byte which needs to put in
 */
void packet_put_byte(int8_t byte);

/**
 * Puts word in data array of message packet
 * @param word - two bytes which need to put in
 */
void packet_put_word(int16_t word);

/**
 * Ends packet with sync byte and checksum byte \n
 * Sends packet via UART \n
 * At the end, changes packet status to "free_to_use"
 */
void packet_end(void);

/**
 * Verifies if acknowledge (command 'A') packet has the same command in data byte as first
 * unverified packet
 * If verification was success, tx_packet status changes to "free_to_use"
 * Otherwise sends that packet again
 * @param packet - packet which is received
 */
void packet_verify(t_packet* packet);

t_packet* get_selected_tx_packet(uint8_t select);


/**
 * Function made for easier debugging
 * @param packet - adress of TX packet which needs to be printed
 */
void print_packet(t_packet* packet);

/**
 * Function made for easier debugging
 * @param packet - adress of RX packet to be printed
 */
void print_rx_packet(t_packet* packet);

#endif

