#!/usr/bin/python
#Simple LED-Warrior14 scratch for send data

import defs    #import the defines from file defs.py (defs will used as prefix for this)

import sys
import time
import smbus #use smbus for i2c
from time import sleep

#Select I2C interface
i2c = smbus.SMBus(1)

class lw14:
    def __init__(self):
        self.i2c_bus = defs.LW14_I2C_ADDRESS_1  #I2C address for the specific LW14
        self.dali_adr = 0   #DALI address 
        self.dali_mode = defs.LW14_MODE_DACP

    #send method from SMBus API from linux / python
    def _i2c_write(self, data):
        #try to send data
        try:
            return i2c.write_i2c_block_data(self.i2c_bus, defs.LW14_REG_COMMAND, data)

        except IOError as e:
            print ("I/O error({0}): {1}".format(e.errno, e.strerror))
            return defs.RET_ERROR

    def _i2c_read(self, reg):
        try:
            #return i2c.read_i2c_block_data(self.i2c_bus, reg)
            return i2c.read_byte_data(self.i2c_bus, reg)

        except IOError as e:
            print ("I/O error({0}): {1}".format(e.errno, e.strerror))
            return defs.RET_ERROR

    #generate the dali address in the following pattern:   Y AAA AAA S
    #Y -> set 1 for groups, 0 for single device
    #AAA AAA -> the adress. for single 0...63, for group 0...15, for broadcast (all devices) Y=1 and AAA AAA = 63
    #S -> set 1 for COMMANDS, 0 for DACP
    def SetDaliAddress(self, adr, y, s):
        self.dali_adr = (y | (adr << 1) | s)
        return self.dali_adr

    def SetMode(self, s):
        self.dali_mode = s
        return self.dali_mode

    #set the i2c address for the class
    def SetI2cBus(self, i2c):
        self.i2c_bus = i2c

    #Check the status register
    def WaitForReady(self):

        while(1):
            #sleep(0.005)
            r = self._i2c_read(defs.LW14_REG_STATUS) #returns an array
    
            if (r & defs.LW14_STATE_BUS_FAULT) == defs.LW14_STATE_BUS_FAULT:
                return defs.RET_ERROR

            elif (r & defs.LW14_STATE_BUSY) != defs.LW14_STATE_BUSY:
                #print ("Bus ready")
                return defs.RET_SUCCESS

            elif r == defs.RET_ERROR:
                return defs.RET_ERROR               


    #Wait until possible data will availabl
    def WaitForValidReply(self):

        while(1):
            r = self._i2c_read(defs.LW14_REG_STATUS)

            if r == defs.RET_ERROR:
                return defs.RET_ERROR

            if (r & defs.LW14_STATE_BUS_FAULT) == defs.LW14_STATE_BUS_FAULT:
                return defs.RET_ERROR | defs.RET_BUSFAULT

            if (r & (defs.LW14_STATE_VALID | defs.LW14_STATE_1BYTE)) == (defs.LW14_STATE_VALID | defs.LW14_STATE_1BYTE):
                return defs.RET_SUCCESS

            if (r & defs.LW14_STATE_FRAMEERROR) == defs.LW14_STATE_FRAMEERROR:
                return defs.RET_ERROR

            if (r & defs.LW14_STATE_OVERRUN) == defs.LW14_STATE_OVERRUN:
                return defs.RET_ERROR

            # BUSY and TIMEFRAME both cleared = bus fully settled, no reply received
            #if (r & (defs.LW14_STATE_BUSY | defs.LW14_STATE_TIMEFRAME)) == 0:
            #    return defs.RET_NO_DALI

            if r == 0x00:
                return defs.RET_NO_DALI

    #store commands must be send twice
    def SendStore(self, data):
        ret = self._i2c_write(data)
        if ret != defs.RET_ERROR:
            self.WaitForReady()
            ret = self._i2c_write(data)
            return defs.RET_SUCCESS
        else:
            return defs.RET_ERROR

    #Send data to the device
    def SendData(self, value):
        #array to send
        data = [self.dali_adr, value]
        return self._i2c_write(data)

    #set a value into DTR of all devices on DALI bus    
    def SetDTR(self, value):
        data = [defs.DA_DTR_0, value] 
        return self._i2c_write(data)            

### Store commands ###

    def StoreActualToDTR(self):
        data = [self.dali_adr, defs.DA_DTR_ACTUAL_LEVEL]
        return self._i2c_write(data)    

    def StoreMin(self):
        data = [self.dali_adr, defs.DA_DTR_MIN_LEVEL]
        return self.SendStore(data) 

    def StoreMax(self):
        data = [self.dali_adr, defs.DA_DTR_MAX_LEVEL]
        return self.SendStore(data) 

    def StoreSysFail(self):
        data = [self.dali_adr, defs.DA_DTR_SYS_FAIL_LEVEL]
        return self.SendStore(data) 

    def StorePowerOn(self):
        data = [self.dali_adr, defs.DA_DTR_POWER_ON_LEVEL]
        return self.SendStore(data) 

    #Valid values from 0...15
    def StoreFadeRate(self):
        data = [self.dali_adr, defs.DA_DTR_FADE_RATE]
        return self.SendStore(data) 

    #Valid values from 0...15
    # 0-> no fading
    def StoreFadeTime(self):
        data = [self.dali_adr, defs.DA_DTR_FADE_TIME]
        return self.SendStore(data) 
        
    #store the DTR value as scene value for device and scene
    def StoreScene(self, value):
        data = [self.dali_adr, 0x40 | (value & 0x0F)] 
        return self.SendStore(data)         

    #remove data from scene (without DTR values)
    def RemoveScene(self, value):
        adr = self.dali_adr | defs.LW14_MODE_CMD
        data = [adr, 0x50 | (value & 0x0F)] 
        return self.SendStore(data)         

    def StoreGroup(self, value):
        adr = self.dali_adr | defs.LW14_MODE_CMD
        data = [adr, 0x60 | (value & 0x0F)]
        return self.SendStore(data) 

    def RemoveGroup(self, value):
        adr = self.dali_adr | defs.LW14_MODE_CMD
        data = [adr, 0x70 | (value & 0x0F)]
        return self.SendStore(data) 

    def StoreAddress(self):
        adr = self.dali_adr | defs.LW14_MODE_CMD
        data = [adr, defs.DA_DTR_AS_SHORT_ADDRESS]
        return self.SendStore(data) 

### Query commands ###

    #Read data from DALI device
    def ReadQuery(self, value):
        data = [self.dali_adr, value]
        self._i2c_write(data)

        res = self.WaitForValidReply()

        if res == defs.RET_SUCCESS:
            read = self._i2c_read(defs.LW14_REG_COMMAND)

            #Clear command register
            self._i2c_read(defs.LW14_REG_COMMAND)
            return read
        else:
            #print ("WaitForValidReply:  %d" % res)
            return res #errorcode < 0
    
    #Get values from querys
    def QueryDTR(self):
        return self.ReadQuery(defs.DA_QUERY_CONTENT_DTR)

    def QueryMin(self):
        return self.ReadQuery(defs.DA_QUERY_MIN_LEVEL)

    def QueryMax(self):
        return self.ReadQuery(defs.DA_QUERY_MAX_LEVEL)

    def QuerySysFail(self):
        return self.ReadQuery(defs.DA_QUERY_SYSTEM_FAILURE_LEVEL)

    def QueryPowerOn(self):
        return self.ReadQuery(defs.DA_QUERY_POWER_ON_LEVEL)

    def QueryFadeRate(self):
        r = self.ReadQuery(defs.DA_QUERY_FADE_TIME_RATE)      
        return (r & 0x0F)

    def QueryFadeTime(self):
        r = self.ReadQuery(defs.DA_QUERY_FADE_TIME_RATE)  
        return (r & 0xF0) >> 4

    def QueryStatus(self):
        r = self.ReadQuery(defs.DA_QUERY_STATUS)
        return r