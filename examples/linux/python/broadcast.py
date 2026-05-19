import defs
import sys

from lw14_class import lw14


if __name__ == "__main__":

	#get the arguments from commandline
	#len = 3, because filename is [0], dali-address is [1], dali-data is[2]
	if len(sys.argv) == 3:
		dali_mode = int(sys.argv[1])
		dali_value = int(sys.argv[2])
		
	else:
		print ("NO ARGUMENTS FOUND: Please run as follow 'python <file>.py <mode> <value>'")
		print ("<mode> = 1 for COMMAND and 0 for DACP")
		print ("<value> = command or DACP value")
		sys.exit()	#exit 

	dalibus = lw14()								#Create a new lw14 class
	dalibus.SetI2cBus(defs.LW14_I2C_ADDRESS_1)		#Set I2C address from LW14 

	#address generation
	dalibus.SetDaliAddress(defs.LW14_BROADCAST, defs.LW14_ADR_GROUP, dali_mode)		    
	
	#send data
	dalibus.SendData(dali_value)
	dalibus.WaitForReady() 		