#include <Wire.h>
#include "lw14_register.h"  //Register of LED-Warrior14

bool bOnce;  //only run once after startup arduino

//Set new I2C Address to LED-Warrior14
void LW14SetAddress(uint8_t old_adr, uint8_t new_adr) 
{
  uint8_t adr = (new_adr & 0xFC);  //Bit 0+1 will be set on hardware

  Wire.beginTransmission(old_adr);
  Wire.write(I2C_REG_ADDRESS);  //Register: set address
  Wire.write((uint8_t)adr);     //new I2C device address
  Wire.write((uint8_t)~adr);    //~
  Wire.endTransmission();
}

void setup() 
{
  bOnce = false;
  Wire.begin();
  Serial.begin(9600);
}

void loop() 
{
  uint8_t error = 0;
  uint8_t address = 0;
  int count = 0;

  Serial.println("Scanning I2C bus...");

  for (address = 1; address < 127; address++) 
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");

      if (address < 16)
        Serial.print("0"); //Add a leading 0 if address < 16 (0x0F)

      Serial.println(address, HEX);
      count++;
    } 
    else if (error == 4) 
    {
      Serial.print("Unknow error at address 0x");

      if (address < 16)
        Serial.print("0"); //Add a leading 0 if address < 16 (0x0F)

      Serial.println(address, HEX);
    }
  }

  if (count == 0) 
    Serial.println("No I2C devices found\n");
  else
  {
    Serial.print("I2C devices found: ");
    Serial.print(count);
    Serial.print("\n");
  }

  delay(1000);

  //Run only once
  if (bOnce == false) 
  {
    LW14SetAddress(LW14_I2C, 0x60);  //Attention: new I2C address will add Bit 0+1 from IO-Pins (hardware)
    bOnce = true;
  }

  delay(4000);
}