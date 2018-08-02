
#include "spi.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

SPI::SPI() {
	device = "/dev/spidev0.0";
	mode = SPI_MODE_0 | SPI_NO_CS;
	bits = 8;
	speed = 500000;
	delay = 0;
}
bool SPI::begin(void) {
	int ret = 0;

	fd = open(device, O_RDWR);
	if (fd < 0) {
		perror("SPI Init Error: can't open device");
		goto exit_noclose;
	}
	
	//SPI Mode
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1) {
		perror("SPI Init Error: can't set spi mode");
		goto exit_close;
	}
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1) {
		perror("SPI Init Error: can't get spi mode");
		goto exit_close;
	}
	
	//Bits per Word
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1) {
		perror("SPI Init Error: can't set bits per word");
		goto exit_close;
	}
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1) {
		perror("SPI Init Error: can't get bits per word");
		goto exit_close;
	}
	
	//Max speed Hz
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1) {
		perror("SPI Init Error: can't set max speed hz");
		goto exit_close;
	}
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1) {
		perror("SPI Init Error: can't get max speed hz");
		goto exit_close;
	}
	
	//Initialize the SPI transfer structs
	memset(&tr8,0,sizeof(struct spi_ioc_transfer));
	tr8.tx_buf = (unsigned long)tx8;
	tr8.rx_buf = (unsigned long)rx8;
	tr8.len = 1;
	tr8.speed_hz = speed;
	tr8.delay_usecs = delay;
	tr8.bits_per_word = bits;
	memset(&tr16,0,sizeof(struct spi_ioc_transfer));
	tr16.tx_buf = (unsigned long)tx16;
	tr16.rx_buf = (unsigned long)rx16;
	tr16.len = 2;
	tr16.speed_hz = speed;
	tr16.delay_usecs = delay;
	tr16.bits_per_word = bits;
	memset(&tr32,0,sizeof(struct spi_ioc_transfer));
	tr32.tx_buf = (unsigned long)tx32;
	tr32.rx_buf = (unsigned long)rx32;
	tr32.len = 4;
	tr32.speed_hz = speed;
	tr32.delay_usecs = delay;
	tr32.bits_per_word = bits;
	
	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	
	return true;
exit_close:
	close(fd);
exit_noclose:
	return false;
}
void SPI::end(void) {
	close(fd);
}
uint8_t SPI::read(void) {
	int ret;
	tx8[0] = 0;
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr8);
	if (ret < 1) {
		perror("SPI Error: failed to read 8 bit value.");
		return 0;
	}
	return rx8[0];
}
void SPI::write(uint8_t v) {
	int ret;
	tx8[0] = v;
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr8);
	if (ret < 1) {
		perror("SPI Error: failed to write 8 bit value.");
		return;
	}
}
void SPI::write16(uint16_t s) {
	int ret;
	tx16[0] = s >> 8; //Write out in Big Endian
	tx16[1] = s;
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr16);
	if (ret < 1) {
		perror("SPI Error: failed to write 16 bit value.");
		return;
	}
}
void SPI::write32(uint32_t w) {
	int ret;
	tx32[0] = w >> 24; //Write out in Big Endian
	tx32[1] = w >> 16;
	tx32[2] = w >> 8;
	tx32[3] = w;
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr32);
	if (ret < 1) {
		perror("SPI Error: failed to write 32 bit value.");
		return;
	}
}


int main(int argc, char *argv[])
{
	SPI spi;
	if (false == spi.begin()) {
		return -1;
	}
	
	spi.write(0x7);
	spi.write16(0x0704);
	spi.write32(0x01020304);
	
	spi.end();
	printf("Done");
	return 0;
}
