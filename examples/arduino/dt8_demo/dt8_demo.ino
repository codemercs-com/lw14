#include <Wire.h>
#include "iec_62386.h"		//List of command bytes for IEC62386-102:2009 standard
#include "iec_62386-209.h"	//List if command bytes for IEC62386-209 extension
#include "lw14_register.h"	//Register of LED-Warrior14

//Generate DALI short address, DALI address format: 0 AAA AAA S
uint8_t GetShortAddr(uint8_t dali_adr, uint8_t mode)
{
	return ((DA_SHORT_BIT | ((dali_adr & DA_MAX_SHORT) << 1)) | mode);
}

//Generate DALI group address, DALI address format: 0 AAA AAA S
uint8_t GetGroupAddr(uint8_t dali_grp, uint8_t mode)
{
	return ((DA_GROUP_BIT | ((dali_grp & DA_MAX_GROUP) << 1)) | mode);
}

//Generate DALI broadcast address, DALI address format: 0 AAA AAA S
uint8_t GetBroadcastAddr(uint8_t mode)
{
    return ((DA_GROUP_BIT | (0x3F << 1)) | mode);
}

//Wait until DALI bus is ready to send next data. DALI is slow!
void WaitForReady()
{
	uint8_t result = 0x00;

	while (true)
	{
		//Set device register to I2C_REG_STATUS
		Wire.beginTransmission(LW14_I2C);
		Wire.write(I2C_REG_STATUS);
		Wire.endTransmission();

		Wire.requestFrom(LW14_I2C, 1);
		result = Wire.read();

		if ((result & STATUS_BUSY) == 0x00) //exit if busyflag is gone
			break;

		delayMicroseconds(DA_TE);
	}
}

void Send8Bit(uint8_t v1)
{
    Wire.beginTransmission(LW14_I2C);
    Wire.write(I2C_REG_COMMAND);
    Wire.write((uint8_t)v1);
    Wire.endTransmission();
}

void Send16Bit(uint8_t v1, uint8_t v2)
{
    Wire.beginTransmission(LW14_I2C);
    Wire.write(I2C_REG_COMMAND);
    Wire.write((uint8_t)v1);
	Wire.write((uint8_t)v2);
    Wire.endTransmission();
}

void Send24Bit(uint8_t v1, uint8_t v2, uint8_t v3)
{
    Wire.beginTransmission(LW14_I2C);
    Wire.write(I2C_REG_COMMAND);
    Wire.write((uint8_t)v1);
	Wire.write((uint8_t)v2);
	Wire.write((uint8_t)v3);
    Wire.endTransmission();
}

void setup() 
{
  Wire.begin();

}

int m_loops = 0;

//Set the values
uint8_t m_DevAdr = 2; 	//Device address/number
uint32_t m_X = 32767;	//Value for X-Coord (0..65535)
uint32_t m_Y = 0;		//Value for Y-Coord (0..65535)
uint8_t m_DACP = 5;	//Value for light output (0..254)

void loop() 
{
	if(m_loops == 0)
	{
		//Init
		uint8_t lsb = 0;
		uint8_t msb = 0;
		uint8_t adr = GetShortAddr(m_DevAdr, DA_MODE_COMMAND); //Generate the address with COMMAND bit

		//X-Coord:
		//1. Set X coord into DTR and DTR1
		//2. Store DTR/DTR1 as X
		//3. Adopting X
		lsb = (m_X & 0x00FF);
		msb = ((m_X & 0xFF00) >> 8);

		Send16Bit(DA_EXT_DATA_TRANSFER_REGISTER, lsb);
		WaitForReady();
		Send16Bit(DA_EXT_DATA_TRANSFER_REGISTER_1, msb);
		WaitForReady();
		Send16Bit(DA_EXT_ENABLE_DEVICE_TYPE_X, 8);
		WaitForReady();
		Send16Bit(adr, DA_209_SET_TEMP_X_COORD);
		WaitForReady();

		//Y-Coord:
		//1. Set Y coord into DTR and DTR1
		//2. Store DTR/DTR1 as Y
		//3. Adopting Y
		lsb = (m_Y & 0x00FF);
		msb = ((m_Y & 0xFF00) >> 8);

		Send16Bit(DA_EXT_DATA_TRANSFER_REGISTER, lsb);
		WaitForReady();
		Send16Bit(DA_EXT_DATA_TRANSFER_REGISTER_1, msb);
		WaitForReady();
		Send16Bit(DA_EXT_ENABLE_DEVICE_TYPE_X, 8);
		WaitForReady();
		Send16Bit(adr, DA_209_SET_TEMP_Y_COORD);
		WaitForReady();

		//Adopting X and Y for color mixing
		Send16Bit(DA_EXT_ENABLE_DEVICE_TYPE_X, 8);
		WaitForReady();
		Send16Bit(adr, DA_209_ACTIVATE);
		WaitForReady();

		//Send output value (0..254) as DACP
		adr = GetShortAddr(m_DevAdr, DA_MODE_DACP); //Generate the address with DACP bit
		Send16Bit(adr, m_DACP);
		WaitForReady();

		//stop loop() after first run
		m_loops = 1;
	}
}
