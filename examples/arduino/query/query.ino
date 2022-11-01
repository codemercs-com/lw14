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

//Wait for 8Bit query result from slave device
uint8_t LW14WaitForReply()
{
	uint8_t result = 0x00;

	while (true)
	{
		LW14SetI2CReadRegister(I2C_REG_STATUS);

		Wire.requestFrom(LW14_I2C, 1, false);
		result = Wire.read();

		//Continue if bus is not ready
		if((result & (STATUS_BUSY)) == STATUS_BUSY)
		{
			delayMicroseconds(DA_TE);
			continue;
		}

		if ((result & (STATUS_VALID | STATUS_1BYTE)) == (STATUS_VALID | STATUS_1BYTE))
			return result;
		else if (result == 0x00) //No device
			return 0;

		delayMicroseconds(DA_TE);
	}

	return result;
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

//Get value from QUERY command by DALI slave address
uint8_t LW14GetQuery(uint8_t dali_adr, uint8_t cmd)
{
    LW14WaitForReady();						//Wait for bus ready
    LW14SendCommand(dali_adr, cmd);			//Send query command
    uint8_t res = LW14WaitForReply();		//Wait for data

	//No one resonse, wrong DALI address
	if(res == 0x00)
		return res;

    //Set register to read from
    Wire.beginTransmission(LW14_I2C);
    Wire.write(I2C_REG_COMMAND);
    Wire.endTransmission();

    //Read from register
    Wire.requestFrom((uint8_t)LW14_I2C, (uint8_t)1);
    return Wire.read();
}

void setup()
{
	Wire.begin();
  Serial.begin(9600);
}

void loop()
{
	//Receive ACTUAL LEVEL Query from device '1'
	uint8_t query = 0;
	query = LW14GetQuery(1, DA_QUERY_ACTUAL_LEVEL);
	Serial.print("QUERY(1): Value = ");
	Serial.println(query, DEC);
	delay(2000);
}