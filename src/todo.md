TODO
=====

- packet.c
- packet.h
- implement color.h better

UART - Motion
-------------
+ uart_transmit_bytes
+ uart_receieve_bytes

- write python script that parses rx packet on PC
- test uart_transmit_packet

- ask for current position
- set current position


scheduler: motion_go(x,y,velocity);
motion:    implement motion_go -> packet_send(type, data) -> uart_transmit
