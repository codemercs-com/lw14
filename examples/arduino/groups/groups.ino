#include <Wire.h>
#include "iec_62386.h"		//List of command bytes for IEC62386-102:2009 standard
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

//Send 8bit command into the DALI bus
void SendCommand(uint8_t dali_adr, uint8_t value)
{
    Wire.beginTransmission(LW14_I2C);
    Wire.write(I2C_REG_COMMAND); //Register: command
    Wire.write((uint8_t)dali_adr);            //DALI address
    Wire.write((uint8_t)value);  //Value
    Wire.endTransmission();
}

//Set a device to a group
void SetGroup(uint8_t adr, uint8_t grp)
{
	SendCommand(adr, DA_ADD_TO_GROUP | grp);
	WaitForReady();
	SendCommand(adr, DA_ADD_TO_GROUP | grp);
	WaitForReady();
}

//Remove a device from a group
void RemoveGroup(uint8_t adr, uint8_t grp)
{
	SendCommand(adr, DA_REMOVE_FROM_GROUP | grp);
	WaitForReady();
	SendCommand(adr, DA_REMOVE_FROM_GROUP | grp);
	WaitForReady();
}

void setup() 
{
	Wire.begin();
}

uint8_t m_once = 0;

void loop() 
{
	if(m_once == 0)
	{
		uint8_t adr = GetShortAddr(5, DA_MODE_COMMAND); 
		SetGroup(adr, 4);
		m_once = 1;
	}
}
