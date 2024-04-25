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

//Send a value into the DTR register. Must be send twice!
void SendDTR(uint8_t value)
{
	SendCommand(DA_EXT_DATA_TRANSFER_REGISTER, value);
	WaitForReady();
	SendCommand(DA_EXT_DATA_TRANSFER_REGISTER, value);
	WaitForReady();
}

//Add the DTR value as the scene value. Must be send twice!
void AddScene(uint8_t adr, uint8_t scene)
{
	SendCommand(adr, DA_DTR_AS_SCENE | scene);
	WaitForReady();
	SendCommand(adr, DA_DTR_AS_SCENE | scene);
	WaitForReady();
}

//Remove from scene and set scene value to MASK (255).  Must be send twice!
void RemoveScene(uint8_t adr, uint8_t scene)
{
	SendCommand(adr, DA_REMOVE_FROM_SCENE | scene);
	WaitForReady();
	SendCommand(adr, DA_REMOVE_FROM_SCENE | scene);
	WaitForReady();
}

//Call a scene
void CallScene(uint8_t adr, uint8_t scene)
{
	SendCommand(adr, DA_GO_TO_SCENE | scene);
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

		//Add Scene
		//SendDTR(213);	//Send a value (0..255) into the DTR
		//WaitForReady();  
		//AddScene(adr, 9); //Set the DTR value as the scene value for a specific scene
		//WaitForReady();

		//Remove Scene
		RemoveScene(adr, 9); //Remove the scene from device. This will set the scene value to MASK (255)
        WaitForReady();

		m_once = 1;
	}
}
