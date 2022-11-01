#include <Wire.h>
#include "iec_62386.h"		//List of command bytes for IEC62386-102:2009 standard
#include "lw14_register.h"	//Register of LED-Warrior14


//Generate DALI short address, DALI address format: 0 AAA AAA S
uint8_t LW14GetShortAddr(uint8_t dali_adr, uint8_t mode)
{
	return ((DA_SHORT_BIT | ((dali_adr & DA_MAX_SHORT) << 1)) | mode);
}

//Generate DALI group address, DALI address format: 0 AAA AAA S
uint8_t LW14GetGroupAddr(uint8_t dali_grp, uint8_t mode)
{
	return ((DA_GROUP_BIT | ((dali_grp & DA_MAX_GROUP) << 1)) | mode);
}

//Generate DALI broadcast address, DALI address format: 0 AAA AAA S
uint8_t LW14GetBroadcastAddr(uint8_t mode)
{
	return (0xFE | mode);
}

//Set I2C register to read from
void LW14SetI2CReadRegister(uint8_t reg)
{
	Wire.beginTransmission(LW14_I2C);
	Wire.write(reg);
	Wire.endTransmission();
}

//Wait until DALI bus is ready to send next data. DALI is slow!
void LW14WaitForReady()
{
	uint8_t result = 0x00;

	while (true)
	{
		LW14SetI2CReadRegister(I2C_REG_STATUS);

		Wire.requestFrom(LW14_I2C, 1);
		result = Wire.read();

		if ((result & STATUS_BUSY) == 0x00) //exit if busyflag is gone
			break;

		delayMicroseconds(DA_TE);
	}
}

//Send command into the DALI bus
void LW14SendCommand(uint8_t dali_adr, uint8_t value)
{
	uint8_t adr = LW14GetShortAddr(dali_adr, DA_MODE_COMMAND);	
	
	Wire.beginTransmission(LW14_I2C);
	Wire.write(I2C_REG_COMMAND); 	//Register: command
	Wire.write((uint8_t)adr);		//DALI address
	Wire.write((uint8_t)value);	 	//Value
	Wire.endTransmission();
}

void setup()
{
	Wire.begin();
}

void loop()
{
	//Lets blink device with address '1'
	  LW14SendCommand(1, DA_MAX);
    delay(1000);
    LW14SendCommand(1, DA_MIN);
    delay(1000);
}