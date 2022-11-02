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
#include "iec_62386.h"

#define DALI_CMD_SIZE 3 //Size of a I2C write transaktion, I2C address + DALI address + DALI value


//Get DALI broadcast address
int lw14_SetBroadcastAddress(unsigned char mode)
{
	return (0xFE | mode);
}

//Get DALI group address
int lw14_SetGroupAddress(int group, unsigned char mode)
{
	return (0x80 | ((group & 15) << 1) | mode);
}

//Get DALI device (short) address
int lw14_SetShortAddress(int device, unsigned char mode)
{
	return (((device & 63) << 1) | mode);
}

//Write I2C report
int lw14_Write(int fd, uint8_t i2c, uint8_t *buffer)
{
	int ret = 0;

	if (ioctl(fd, I2C_SLAVE, i2c) >= 0)
	{
		ret = write(fd, buffer, DALI_CMD_SIZE);
		if (ret != DALI_CMD_SIZE)
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

int main(int argc, char **argv)
{
	int device;
	unsigned long funcs;

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

	uint8_t buffer[DALI_CMD_SIZE] = {0};
	buffer[0] = I2C_REG_COMMAND;
	buffer[1] = lw14_SetShortAddress(1, DA_MODE_COMMAND);	//DALI address
	buffer[2] = 0x00;	//DALI command / value

	lw14_Write(device, LW14_I2C, buffer);

	close(device);

	return 0;
}
