#include <Wire.h>
#include "iec_62386.h"
#include "lw14_register.h"

void setup()
{
	Wire.begin();
	Serial.begin(9600);
}

//Set I2C register to read from
void LW14SetI2CReadRegister(uint8_t reg)
{
	Wire.beginTransmission(LW14_I2C);
	Wire.write(reg);
	Wire.endTransmission();
}

//Get status register
uint8_t LW14GetStatus()
{
	uint8_t result = 0x00;
	LW14SetI2CReadRegister(I2C_REG_STATUS);
	Wire.requestFrom((uint8_t)LW14_I2C, (uint8_t)1);
	result = Wire.read();

	return result;
}

void loop()
{
	uint8_t buffer[3] = {0};
	size_t datalen = 0;
	uint8_t status = LW14GetStatus();

	if((status & STATUS_BUSY) == 0x00)
	{
		//Some data available
		if ((status & STATUS_3BYTE) > 0)
		{
			datalen = status & STATUS_3BYTE;	//Get the size of the dali data (8/16/24Bit)

			//Read date from Command register
			LW14SetI2CReadRegister(I2C_REG_COMMAND);
			Wire.requestFrom((uint8_t)LW14_I2C, (size_t)datalen, false);
			
			for(size_t i = 0; i<datalen; i++)
				buffer[i] = Wire.read();

			//Print the status register
			Serial.print("Status flags/bits: ");

			if((status & STATUS_BUS_FAULT) == STATUS_BUS_FAULT)
				Serial.print("BUS FAULT");

			if ((status & STATUS_BUSY) == STATUS_BUSY)
				Serial.print("   STATUS_BUSY");

			if ((status & STATUS_OVERRUN) == STATUS_OVERRUN)
				Serial.print("   STATUS_OVERRUN");

			if ((status & STATUS_FRAME_ERROR) == STATUS_FRAME_ERROR)
				Serial.print("   STATUS_FRAME_ERROR");

			if ((status & STATUS_VALID) == STATUS_VALID)
				Serial.print("   STATUS_VALID");

			if ((status & STATUS_TIMEFRAME) == STATUS_TIMEFRAME)
				Serial.print("   STATUS_TIMEFRAME");

			
			//if ((status & STATUS_3BYTE) > 0)
			//{
			//	Serial.print("   Bytes: ");
			//	Serial.print(((status & STATUS_3BYTE) & 0xFF), DEC);
			//}

			if (status != 0x00)
                Serial.println();

			//Print only if some data are available
			if(datalen > 0)
			{
				Serial.println("===== DATA =====");
				Serial.print("Byte Count: ");
				Serial.println(datalen, DEC);

				//Check for a query return value (8Bit telegram)
				if(datalen == STATUS_1BYTE)
				{
					Serial.print("Query Value: ");
					Serial.println(buffer[0], DEC);
				}
				else
				{
					Serial.print("Address: ");
					//Get the address type BROADCAST, GROUP or SINGLE/SHORT
					//DALI address format: Y AAA AAA S. For "real" device address you have to shift (>>) one right
					if((buffer[0] & DA_GROUP_BIT) == DA_GROUP_BIT)
					{
						if((buffer[0] & 0x7F) >= 0x7E)
							Serial.println("Broadcast");
						else
						{
							Serial.print("Group: ");
							Serial.println(((buffer[0] & 0x7E) >> 1), DEC);
						}
					}
					else
						Serial.println(((buffer[0] & 0x7E) >> 1), DEC);

					//Print the type COMMAND (Min, Max, Querys) or DACP (dimming values)
					Serial.print("Type: ");
					if((buffer[0] & DA_MODE_COMMAND) == DA_MODE_COMMAND)
						Serial.println("Command");
					else
						Serial.println("DACP");

					//16Bit telegram					
					if(datalen == STATUS_2BYTE)
					{
						Serial.print("Data 1: ");
						Serial.println(buffer[1], DEC);
					}
					
					//24Bit telegram
					if(datalen == STATUS_3BYTE)
					{
						Serial.print("Data 2: ");
						Serial.println(buffer[2], DEC);
					}
				}

				Serial.println("================");
			}
		}
	}
}
