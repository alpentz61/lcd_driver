#ifndef LINUX_SPI
#define LINUX_SPI

#include <stdint.h>				//uint_t
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>				//memset
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


class SPI {
public:
				SPI();
	bool 		begin(void);
	void 		end(void);
    uint8_t 	spiRead(void);
    void     	spiWrite(uint8_t v);
    void 		spiWrite16(uint16_t s);
    void 		spiWrite32(uint32_t w);
    void 		spiWritePixels(uint16_t *c, uint32_t l);
private:
	const char *device;
	uint8_t mode;
	uint8_t bits;
	uint32_t speed;
	uint16_t delay;
	int fd;
	uint8_t tx8[1];
	uint8_t rx8[1];
	uint8_t tx16[2];
	uint8_t rx16[2];
	uint8_t tx32[4];
	uint8_t rx32[4];
	struct spi_ioc_transfer tr8;
	struct spi_ioc_transfer tr16;
	struct spi_ioc_transfer tr32;
};

#endif
