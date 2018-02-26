#ifndef SPI_H_
#define SPI_H_

#define DEVICE_0 0
#define DEVICE_1 1

int spi_open_port (int spi_device, unsigned int speed);

int spi_close_port (int spi_device);

int spi_rw (int spi_device, unsigned char *tx_buff, unsigned char *rx_buff, int length);

#endif
