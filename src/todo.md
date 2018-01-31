TODO
=====

- packet.c
- packet.h
- implement color.h better

UART - Motion
-------------
+ uart_transmit_bytes
+ uart_receieve_bytes

+ write python script that parses rx packet on PC
+ test uart_transmit_packet



+ ask for current position
+ print current position as integer
+ set current position

+ check if command which is sent - process if incomed 'A' character
- main reorganize

scheduler -> task -> motion_go(x,y,velocity);
				  -> motion_stop()
				  -> motion_get_status(); //where to put?
				  
motion:    implement motion_go -> packet_send(type, data) -> uart_transmit
