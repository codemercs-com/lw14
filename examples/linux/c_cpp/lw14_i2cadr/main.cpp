#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

#include "lw14.h"

//Write I2C report
int lw14_Write(int fd, uint8_t i2c, uint8_t *buffer)
{
	int ret = 0;

	if (ioctl(fd, I2C_SLAVE, i2c) >= 0)
	{
		ret = write(fd, buffer, 3);
		if (ret != 3)
		{
			std::cout << "ERROR: " << __FUNCTION__ << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "ERROR: " << __FUNCTION__ << std::endl;
		return 0;
	}

	return 1;
}

int32_t check_hex(std::string str, uint32_t len)
{
	int32_t ret = 0;
	std::string pool = "0123456789abcdef";
	std::string sub = "";

	if(str.length() != len)
		return -2; //Error: string length not equal 'len'

	//Check for valid hex digits
	for(uint32_t i=0; i<len; i++)
	{
		sub = str.at(i);
		ret += sub.find_first_of(pool, 0);
	}

	return ret;
}

int main(int argc, char **argv)
{
	int device;
	unsigned long funcs;
	uint8_t i2c_old = 0x00;
	uint8_t i2c_new = 0x00;

	//Some input checks
	if(argc != 3)
	{
		std::cout << "usage: ./lw14_i2cadr <old_adr> <new_adr>" << std::endl;
		return 0;
	}

	if(check_hex(argv[1], 2) != 0)
	{
		std::cout << "Input error for 'old' I2C address. Please use only hex digits, max length: 2." << std::endl;
		return 0;
	}

	if(check_hex(argv[2], 2) != 0)
	{
		std::cout << "Input error for 'new' I2C address. Please use only hex digits, max length: 2." << std::endl;
		return 0;
	}

	//Convert arguments to uint8_t
	i2c_old = std::stoul(argv[1], nullptr, 16);
	i2c_new = std::stoul(argv[2], nullptr, 16);

	// Open I2c interface
	if ((device = open("/dev/i2c-1", O_RDWR)) < 0)
	{
		perror("Failed to open I2C bus\n");
		exit(1);
	}

	if (ioctl(device, I2C_FUNCS, &funcs) < 0)
	{
		perror("ioctl() I2C_FUNCS failed\n");
		exit(1);
	}
	
	//Clear last 2 bits, they will be set by solder IO-Pins
	uint8_t i2c_adr = (i2c_new & 0xFC);

	uint8_t buffer[3] = {0};
	buffer[0] = I2C_REG_ADDRESS;
	buffer[1] = i2c_adr;
	buffer[2] = ~i2c_adr;

	lw14_Write(device, i2c_old, buffer);

	close(device);

	return 0;
}
