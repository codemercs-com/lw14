import defs
import sys

from lw14_class import lw14


if __name__ == "__main__":

	#get the arguments from commandline
	#len = 3, because filename is [0], dali-address is [1], dali-data is[2]
	if len(sys.argv) == 3:
		dali_device = int(sys.argv[1])
		dali_value = int(sys.argv[2])

	else:
		print ("NO ARGUMENTS FOUND: Please run as follow 'python <file>.py <dali_adr> <value>'")
		print ("<dali_adr> = 0...63 single device, 255 for broadcast")
		print ("<value> = 0...255 ")
		sys.exit()	#exit 

	dalibus = lw14()								#Create a new lw14 class
	dalibus.SetI2cBus(defs.LW14_I2C_ADDRESS_1)		#Set I2C address from LW14 

	
	#choose between broadcast and single device
	if dali_device == 255:
		dalibus.SetDaliAddress(defs.LW14_BROADCAST, defs.LW14_ADR_GROUP, defs.LW14_MODE_CMD)	
	else:
		dalibus.SetDaliAddress(dali_device, defs.LW14_ADR_SINGLE, defs.LW14_MODE_CMD)

	#send data
	dalibus.SendData(dali_value)										
	dalibus.WaitForReady() 		