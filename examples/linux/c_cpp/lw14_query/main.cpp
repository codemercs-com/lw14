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
#include <iomanip>

#include "lw14.h"
#include "iec_62386.h"

// Write I2C report
int lw14_Write(int fd, uint8_t i2c, uint8_t *buffer, int n)
{
	int ret = 0;

	if (ioctl(fd, I2C_SLAVE, i2c) >= 0)
	{
		ret = write(fd, buffer, n);
		if (ret != n)
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

	return ret;
}

// Read I2C report
int lw14_Read(int fd, uint8_t i2c, uint8_t *buffer, int n)
{
	int ret = 0;
	if (ioctl(fd, I2C_SLAVE, i2c) >= 0)
	{
		ret = read(fd, buffer, n);
		if (ret != n)
			std::cout << "ERROR: " << __FUNCTION__ << std::endl;
	}
	else
		std::cout << "ERROR: " << __FUNCTION__ << std::endl;

	return ret;
}

// Set I2C register to read from
int lw14_SetReadRegister(int fd, uint8_t i2c, uint8_t value)
{
	uint8_t buffer[1] = {value};
	return lw14_Write(fd, i2c, buffer, 1);
}

// Get short address
int lw14_SetShortAddress(int device, uint8_t mode)
{
	return (((device & 63) << 1) | mode);
}

void lw14_PrintStatusMsg(uint8_t value)
{
	if ((value & STATUS_1BYTE) == STATUS_1BYTE)
		std::cout << "Status: 1 Byte = 1" << std::endl;
	else if ((value & STATUS_1BYTE) != STATUS_1BYTE)
		std::cout << "Status: 1 Byte = 0" << std::endl;

	if ((value & STATUS_2BYTE) == STATUS_2BYTE)
		std::cout << "Status: 2 Byte = 1" << std::endl;
	else if ((value & STATUS_2BYTE) != STATUS_2BYTE)
		std::cout << "Status: 2 Byte = 0" << std::endl;

	if ((value & STATUS_3BYTE) == STATUS_3BYTE)
		std::cout << "Status: 3 Byte = 1" << std::endl;
	else if ((value & STATUS_3BYTE) != STATUS_3BYTE)
		std::cout << "Status: 3 Byte = 0" << std::endl;

	if ((value & STATUS_TIMEFRAME) == STATUS_TIMEFRAME)
		std::cout << "Status: Timeframe = 1" << std::endl;
	else if ((value & STATUS_TIMEFRAME) == 0x00)
		std::cout << "Status: Timeframe = 0" << std::endl;

	if ((value & STATUS_VALID) == STATUS_VALID)
		std::cout << "Status: Valid = 1" << std::endl;
	else if ((value & STATUS_VALID) == 0x00)
		std::cout << "Status: Valid = 0" << std::endl;

	if ((value & STATUS_FRAME_ERROR) == STATUS_FRAME_ERROR)
		std::cout << "Status: Frame Error = 1" << std::endl;
	else if ((value & STATUS_FRAME_ERROR) == 0x00)
		std::cout << "Status: Frame Error = 0" << std::endl;

	if ((value & STATUS_OVERRUN) == STATUS_OVERRUN)
		std::cout << "Status: Overrun = 1" << std::endl;
	else if ((value & STATUS_OVERRUN) == 0x00)
		std::cout << "Status: Overrun = 0" << std::endl;

	if ((value & STATUS_BUSY) == STATUS_BUSY)
		std::cout << "Status: Busy = 1" << std::endl;
	else if ((value & STATUS_1BYTE) == 0x00)
		std::cout << "Status: Busy = 0" << std::endl;

	if ((value & STATUS_BUS_FAULT) == STATUS_BUS_FAULT)
		std::cout << "Status: Bus Fault = 1" << std::endl;
	else if ((value & STATUS_BUSY) == 0x00)
		std::cout << "Status: Bus Fault = 0" << std::endl;
}

uint8_t lw14_GetStatus(int fd, uint8_t i2c)
{
	uint8_t buffer[1] = {0};

	lw14_SetReadRegister(fd, i2c, I2C_REG_STATUS);

	if (lw14_Read(fd, i2c, buffer, 1) != 1)
		return 0x00;
	else
		return buffer[0];
}

uint8_t lw14_WaitForReady(int fd, uint8_t i2c)
{
	uint8_t result = 0x00;

	while (true)
	{
		lw14_SetReadRegister(fd, i2c, I2C_REG_STATUS);
		result = lw14_GetStatus(fd, i2c);

		if ((result & STATUS_BUSY) == 0x00) // exit if busyflag is gone
			break;

		// exit if bus is faulty (no Power, short, etc.)
		if ((result & STATUS_BUS_FAULT) == STATUS_BUS_FAULT)
		{
			std::cout << __FUNCTION__ << ": Bus Fault" << std::endl;
			break;
		}

		usleep(DA_TE);
	}

	return result;
}

uint8_t lw14_WaitForValidReply(int fd, uint8_t i2c)
{
	uint8_t result = 0x00;

	while (true)
	{
		lw14_SetReadRegister(fd, i2c, I2C_REG_STATUS);
		result = lw14_GetStatus(fd, i2c);

		// exit if bus is faulty (no Power, short, etc.)
		if ((result & STATUS_BUS_FAULT) == STATUS_BUS_FAULT)
		{
			std::cout << __FUNCTION__ << ": Bus Fault" << std::endl;
			break;
		}

		// Continue if bus is not ready
		if ((result & STATUS_BUSY) == STATUS_BUSY)
		{
			usleep(DA_TE);
			continue;
		}

		if ((result & (STATUS_VALID | STATUS_1BYTE)) == (STATUS_VALID | STATUS_1BYTE))
			return result;
		else if (result == 0x00) // No device
			return 0x00;

		usleep(DA_TE);
	}

	return result;
}

int lw14_SendCommand(int fd, uint8_t i2c, uint8_t dali_adr, uint8_t value)
{
	uint8_t buffer[3] = {0};
	buffer[0] = I2C_REG_COMMAND;
	buffer[1] = lw14_SetShortAddress(dali_adr, DA_MODE_COMMAND); // DALI address
	buffer[2] = value;											 // DALI command / value

	return lw14_Write(fd, i2c, buffer, 3);
}

// Query only work with short address on DALI, returns a 8Bit value
uint8_t lw14_GetQueryValue(int device, uint8_t i2c)
{
	lw14_SetReadRegister(device, i2c, I2C_REG_COMMAND);
	uint8_t buffer[1] = {};
	lw14_Read(device, i2c, buffer, 1);
	return buffer[0];
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

	lw14_WaitForReady(device, LW14_I2C);						  // Wait for a "silent" bus
	lw14_SendCommand(device, LW14_I2C, 1, DA_QUERY_ACTUAL_LEVEL); // Send query command to device
	uint8_t ret = lw14_WaitForValidReply(device, LW14_I2C);		  // Wait for reply

	// Is a 1 Byte value return and is valid?
	if ((ret & (STATUS_1BYTE | STATUS_VALID)) == (STATUS_1BYTE | STATUS_VALID))
	{
		uint8_t query = lw14_GetQueryValue(device, LW14_I2C); // Get query value
		std::cout << "Query: " << std::dec << (query & 0xFF) << std::endl;
	}
	else
	{
		lw14_PrintStatusMsg(ret);
	}

	close(device);
}
