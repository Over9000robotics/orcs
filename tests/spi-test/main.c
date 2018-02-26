#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <wiringPi.h> //for delay

#include "spi.h"

int main (void)
{
	
  unsigned char databuf[100];
  unsigned char rx_buff[100];
  int i;
  
  spi_open_port (DEVICE_1, 500000);
  
  for(i=0; i<100; i++)
  {
	  //databuf[i] = 0xFE;
	  rx_buff[i] = 0;
  }
  
  while(1)
  {
	  spi_rw (0, databuf, rx_buff, 1);
	  printf("rx_buff[0]: %d \n", rx_buff[0]);
	  delay(1000);
  }
  
  return 0 ;
}
