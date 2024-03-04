/*Sample code for IEC62386 for send command/DACP and get QUERY on an ESP32*/

#include <Arduino.h>
#include <Wire.h>
#include "iec_62386.h"    //List of command bytes for IEC62386-102:2009 standard
#include "lw14_register.h"

/*!ATTENTION!*/
#define NOP() asm volatile ("nop") //Workaround for delayMicrosecondeExt function


void setup() 
{
    Wire.begin(21,22);
    Serial.begin(115200);
}

//delayMicroseconds on ESP32 faulty, workaround 
void IRAM_ATTR delayMicrosecondsExt(uint32_t us)
{
    uint32_t m = micros(); 
    if(us)
    {    
        while((uint32_t)(micros() - m) < us) 
        {
            NOP();
        }
    }
}

//Generate DALI short address, DALI address format: 0 AAA AAA S
uint8_t GetShortAddress(uint8_t dali_adr, uint8_t mode)
{
    return ((DA_SHORT_BIT | ((dali_adr & DA_MAX_SHORT) << 1)) | mode);
}

//Generate DALI group address, DALI address format: 0 AAA AAA S
uint8_t GetGroupAddress(uint8_t dali_adr, uint8_t mode)
{
    return ((DA_GROUP_BIT | ((dali_adr & DA_MAX_GROUP) << 1)) | mode);
}

//Generate DALI broadcast address, DALI address format: 0 AAA AAA S
uint8_t GetBroadcastAddress(uint8_t mode)
{
    return ((DA_GROUP_BIT | (0x3F << 1)) | mode);
}

//Wait until DALI bus is ready to send next data
void WaitForReady()
{
    uint8_t result = 0x00;

    while (true)
    {
		//Set register to read from
		Wire.beginTransmission(LW14_I2C);
		Wire.write(I2C_REG_STATUS);
		Wire.endTransmission();

        Wire.requestFrom(LW14_I2C, 1);
        result = Wire.read();

        if ((result & STATUS_BUSY) == 0x00) //exit if busyflag is gone
            break;

        delayMicrosecondsExt(DA_TE);
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

//Send value to all DALI devices (for special commands like 'TERMINATE', 'DTR', etc)
void SendValue(uint8_t byte1, uint8_t byte2)
{
    Wire.beginTransmission(LW14_I2C);
    Wire.write(I2C_REG_COMMAND);        //Register: command
    Wire.write((uint8_t)byte1);         //DALI address
    Wire.write((uint8_t)byte2);         //Value
    Wire.endTransmission();
}

void loop() 
{
	//Send MIN and MAX over broadcast every 1 sec.
	uint8_t adr = GetBroadcastAddress(DA_MODE_COMMAND); 
	SendCommand(adr, DA_MIN);
	delay(1000);
	SendCommand(adr, DA_MAX);
	delay(1000);
}


