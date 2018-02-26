#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <wiringPi.h> //for delay

#include "spi.h"



int main (void)
{
	
  unsigned char databuf[100];
  int i;
  
  spi_open_port (DEVICE_0, 500000);
  
  for(i=0; i<100; i++)
  {
	  databuf[i] = i;
  }
  
  while(1)
  {
	  spi_rw (0, databuf, 50);
	  delay(1000);
  }
  
  return 0 ;
}
