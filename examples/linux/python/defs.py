''' FILE WITH DEFINES / CONST FOR THE WHOLE PROJECT '''
''' IEC 62386-102:2009, IEC 62386-207, IEC 62386-209:2011 '''

# return values
RET_NO_DALI  = -2
RET_ERROR    = -1
RET_NONE     = 0
RET_SUCCESS  = 1

RET_BUSFAULT = -10
RET_ZERO     = -20


# -----------------------------------------------------------------------
# LW14 I2C device addresses
# -----------------------------------------------------------------------

LW14_I2C_ADDRESS_1 = 0x23  # 7Bit default address from LW14
LW14_I2C_ADDRESS_2 = 0x00  # 7Bit
LW14_I2C_ADDRESS_3 = 0x00  # 7Bit
LW14_I2C_ADDRESS_4 = 0x00  # 7Bit

# Register of LED-Warrior14
LW14_REG_STATUS    = 0x00  # Read only
LW14_REG_COMMAND   = 0x01  # Write/Read
LW14_REG_CONFIG    = 0x02  # Write only
LW14_REG_SIGNATURE = 0xF0  # Read only
LW14_REG_ADDRESS   = 0xFE  # Write only

# Answers of 'status' register
LW14_STATE_BUS_FAULT  = 0x80
LW14_STATE_BUSY       = 0x40
LW14_STATE_OVERRUN    = 0x20
LW14_STATE_FRAMEERROR = 0x10
LW14_STATE_VALID      = 0x08
LW14_STATE_TIMEFRAME  = 0x04
LW14_STATE_2BYTE      = 0x02
LW14_STATE_1BYTE      = 0x01
LW14_STATE_NONE       = 0x00

# Value for 'config' register
# 1 = lowest, 5 = highest, other will be clipped
LW14_CONFIG_PRIO_1 = 0x01
LW14_CONFIG_PRIO_2 = 0x02  # Default value
LW14_CONFIG_PRIO_3 = 0x03
LW14_CONFIG_PRIO_4 = 0x04
LW14_CONFIG_PRIO_5 = 0x05

# Special bits for DALI address byte (YAAA AAAS)
# Y: 0 = short address, 1 = group/broadcast
# S: 0 = DACP mode,     1 = Command mode
LW14_MODE_DACP  = 0x00
LW14_MODE_CMD   = 0x01
LW14_ADR_SINGLE = 0x00
LW14_ADR_GROUP  = 0x80

LW14_MAX_SINGLE = 64
LW14_MAX_GROUP  = 16
LW14_MAX_DACP   = 254

LW14_BROADCAST  = 0x3F


# -----------------------------------------------------------------------
# IEC 62386-102 General defines
# -----------------------------------------------------------------------

DA_YES  = 0xFF  # DALI "Yes" response
DA_NO   = 0x00  # DALI "No" response
DA_MASK = 0xFF

# Address mode aliases (DA_ names for LW14 constants)
DA_MODE_DACP    = LW14_MODE_DACP    # S=0: DACP mode (direct output value)
DA_MODE_COMMAND = LW14_MODE_CMD     # S=1: Command mode
DA_GROUP_BIT    = LW14_ADR_GROUP    # Y=1: Group/broadcast address
DA_SHORT_BIT    = LW14_ADR_SINGLE   # Y=0: Short address

# Maximum address values
DA_MAX_SHORT = 0x3F  # 0...63  -> 64 devices
DA_MAX_GROUP = 0x0F  # 0...15  -> 16 groups
DA_MAX_SCENE = 0x0F  # 0...15  -> 16 scenes

# DALI timings
# 1 Te = 416.67 µs  (bit time at 1200 baud)
DA_TE                         = (1 / (2 * 1.2)) * 1000  # ~416.67 µs
DA_FORWARD_FRAME_TIME         = 38 * DA_TE
DA_BACKWARD_FRAME_TIME        = 22 * DA_TE
DA_SETTLING_TIME_FF_TO_FF     = 22 * DA_TE
DA_SETTLING_TIME_BF_TO_FF     = 22 * DA_TE
DA_SETTLING_TIME_FF_TO_BF_MIN =  7 * DA_TE
DA_SETTLING_TIME_FF_TO_BF_MAX = 22 * DA_TE


# -----------------------------------------------------------------------
# IEC 62386-102 Commands 0..9  (YAAA AAAS xxxx xxxx)
# -----------------------------------------------------------------------

DA_OFF                  = 0x00
DA_UP                   = 0x01
DA_DOWN                 = 0x02
DA_STEP_UP              = 0x03
DA_STEP_DOWN            = 0x04
DA_MAX                  = 0x05
DA_MIN                  = 0x06
DA_STEP_DOWN_OFF        = 0x07
DA_ON_STEP_UP           = 0x08
DA_ENABLE_DACP_SEQUENCE = 0x09



# -----------------------------------------------------------------------
# IEC 62386-102 Commands 16..31  (YAAA AAAS 0001 xxxx)
# -----------------------------------------------------------------------

DA_SCENE      = 0x10  # 0x10 - 0x1F
DA_SCENE_MASK = 0x1F

DA_GO_TO_SCENE = DA_SCENE


# -----------------------------------------------------------------------
# IEC 62386-102 Commands 32..47  (YAAA AAAS xxxx xxxx, send twice within 100ms!)
# -----------------------------------------------------------------------

DA_RESET              = 0x20
DA_DTR_ACTUAL_LEVEL   = 0x21
DA_DTR_MAX_LEVEL      = 0x2A
DA_DTR_MIN_LEVEL      = 0x2B
DA_DTR_SYS_FAIL_LEVEL = 0x2C
DA_DTR_POWER_ON_LEVEL = 0x2D
DA_DTR_FADE_TIME      = 0x2E
DA_DTR_FADE_RATE      = 0x2F




# -----------------------------------------------------------------------
# IEC 62386-102 Commands 64..127  (YAAA AAAS 0x10 xxxx, send twice!)
# -----------------------------------------------------------------------

DA_ADD_SCENE    = 0x40  # 0x40 - 0x4F
DA_REMOVE_SCENE = 0x50  # 0x50 - 0x5F
DA_ADD_GROUP    = 0x60  # 0x60 - 0x6F
DA_REMOVE_GROUP = 0x70  # 0x70 - 0x7F




# -----------------------------------------------------------------------
# IEC 62386-102 Commands 128..  (YAAA AAAS xxxx xxxx)
# -----------------------------------------------------------------------

DA_DTR_AS_SHORT_ADDRESS = 0x80
DA_DTR_0                = 0xA3

DA_DTR_AS_SHORT_ADDRESS = DA_DTR_AS_SHORT_ADDRESS
DA_ENABLE_WRITE_MEMORY  = 0x81


# -----------------------------------------------------------------------
# IEC 62386-102 Query commands  (YAAA AAAS xxxx xxxx)
# -----------------------------------------------------------------------

DA_QUERY_STATUS                = 0x90
DA_QUERY_CONTROL_GEAR          = 0x91  # Return: Yes or No
DA_QUERY_LAMP_FAILURE          = 0x92  # Return: Yes or No
DA_QUERY_LAMP_POWER_ON         = 0x93  # Return: Yes or No
DA_QUERY_LIMIT_ERROR           = 0x94  # Return: Yes or No
DA_QUERY_RESET_STATE           = 0x95  # Return: Yes or No
DA_QUERY_MISSING_SHORT_ADDRESS = 0x96  # Return: Yes or No
DA_QUERY_VERSION_NUMBER        = 0x97  # Return: must be 1
DA_QUERY_CONTENT_DTR           = 0x98  # Return: Content of DTR (8 Bit)
DA_QUERY_DEVICE_TYPE           = 0x99  # Return: Value between 0..255
DA_QUERY_PHYSICAL_MIN_LEVEL    = 0x9A  # Return: 8 Bit value
DA_QUERY_POWER_FAILURE         = 0x9B  # Return: Yes or No
DA_QUERY_CONTENT_DTR1          = 0x9C  # Return: Content of DTR1 (8 Bit)
DA_QUERY_CONTENT_DTR2          = 0x9D  # Return: Content of DTR2 (8 Bit)
DA_QUERY_ACTUAL_LEVEL          = 0xA0  # Return: Actual output (0...254)
DA_QUERY_MAX_LEVEL             = 0xA1  # Return: Stored MAX value
DA_QUERY_MIN_LEVEL             = 0xA2  # Return: Stored MIN value
DA_QUERY_POWER_ON_LEVEL        = 0xA3  # Return: Stored POWER ON value
DA_QUERY_SYSTEM_FAILURE_LEVEL  = 0xA4  # Return: Stored SYSTEM FAILURE value
DA_QUERY_FADE_TIME_RATE        = 0xA5  # Return: FADE TIME (Bit 7..4) and FADE RATE (Bit 3..0)
DA_QUERY_SCENE_LEVEL           = 0xB0  # 0xB0 - 0xBF: Stored scene value (8 Bit)
DA_QUERY_GROUPS_0_TO_7         = 0xC0  # Return: Bitmask GROUP 0..7
DA_QUERY_GROUPS_8_TO_15        = 0xC1  # Return: Bitmask GROUP 8..15
DA_QUERY_RANDOM_ADDRESS_H      = 0xC2  # Return: Upper 8 Bit of 24 Bit random address
DA_QUERY_RANDOM_ADDRESS_M      = 0xC3  # Return: Middle 8 Bit of 24 Bit random address
DA_QUERY_RANDOM_ADDRESS_L      = 0xC4  # Return: Lower 8 Bit of 24 Bit random address
DA_READ_MEMORY_LOCATION        = 0xC5
DA_QUERY_FAST_FADE_TIME          = 0xFD  # Return: 8 Bit value
DA_QUERY_MIN_FADE_TIME           = 0xFE  # Return: 8 Bit value
DA_QUERY_EXTENDED_VERSION_NUMBER = 0xFF  # Return: must be 1




# -----------------------------------------------------------------------
# IEC 62386-102 Extended commands (commissioning / addressing)
# -----------------------------------------------------------------------

DA_EXT_TERMINATE                = 0xA1
DA_EXT_DATA_TRANSFER_REGISTER   = 0xA3
DA_EXT_INITIAISE                = 0xA5
DA_EXT_RANDOMISE                = 0xA7
DA_EXT_COMPARE                  = 0xA9
DA_EXT_WITHDRAW                 = 0xAB
DA_EXT_SEARCHADDRH              = 0xB1
DA_EXT_SEARCHADDRM              = 0xB3
DA_EXT_SEARCHADDRL              = 0xB5
DA_EXT_PROGRAMM_SHORT_ADDRESS   = 0xB7
DA_EXT_VERIFY_SHORT_ADDRESS     = 0xB9
DA_EXT_QUERY_SHORT_ADDRESS      = 0xBB
DA_EXT_PHYSICAL_SELECTION       = 0xBD
DA_EXT_ENABLE_DEVICE_TYPE_X     = 0xC1
DA_EXT_DATA_TRANSFER_REGISTER_1 = 0xC3
DA_EXT_DATA_TRANSFER_REGISTER_2 = 0xC5
DA_EXT_WRITE_MEMORY_LOCATION    = 0xC7




# -----------------------------------------------------------------------
# IEC 62386-207 LED-specific commands  (YAAA AAAS xxxx xxxx)
# -----------------------------------------------------------------------

DA_REFERENCE_SYSTEM_POWER         = 0xE0
DA_ENABLE_CURRENT_PROTECTOR       = 0xE1
DA_DISABLE_CURRENT_PROTECTOR      = 0xE2
DA_SELECT_DIMMING_CURVE           = 0xE3  # DTR=1: linear, DTR=0: logarithmic (default)
DA_QUERY_GEAR_TYPE                = 0xED  # Return: 8 Bit value
DA_QUERY_DIMMING_CURVE            = 0xEE  # Return: 1 or 0
DA_QUERY_POSSIBLE_OPERATION_MODES = 0xEF  # Return: 8 Bit value
DA_QUERY_FEATURES                 = 0xF0  # Return: 8 Bit value
DA_QUERY_FAILURE_STATUS           = 0xF1  # Return: 8 Bit value
DA_QUERY_SHORT_CIRCUIT            = 0xF2  # Return: Yes or No
DA_QUERY_OPEN_CIRCUIT             = 0xF3  # Return: Yes or No
DA_QUERY_LOAD_DECREASE            = 0xF4  # Return: Yes or No
DA_QUERY_LOAD_INCREASE            = 0xF5  # Return: Yes or No
DA_QUERY_CURRENT_PROTECTOR_ACTIVE = 0xF6  # Return: Yes or No
DA_QUERY_THERMAL_SHUT_DOWN        = 0xF7  # Return: Yes or No
DA_QUERY_THERMAL_OVERLOAD         = 0xF8  # Return: Yes or No
DA_QUERY_REFERENCE_RUNNING        = 0xF9  # Return: Yes or No
DA_QUERY_REFERENCE_MEASUREMENT_FAILED = 0xFA  # Return: Yes or No
DA_QUERY_CURRENT_PROTECTOR_ENABLED    = 0xFB  # Return: Yes or No
DA_QUERY_OPERATING_MODE           = 0xFC  # Return: 8 Bit value


# -----------------------------------------------------------------------
# Memory bank access
# -----------------------------------------------------------------------

DA_MEMORYBANK_0 = 0x00
DA_MEMORYBANK_1 = 0x01

# Memory bank 0 register addresses
DA_MEMORY_0_LAST_PLACE      = 0x00  # Address of last accessible memory location
DA_MEMORY_0_CHECKSUM        = 0x01  # Checksum memory bank 0
DA_MEMORY_0_NUM_LAST_MEMORY = 0x02  # Number of last memory bank access
DA_MEMORY_0_OEM_GTIN_0      = 0x03  # OEM GTIN Byte 0 (MSB)
DA_MEMORY_0_OEM_GTIN_1      = 0x04
DA_MEMORY_0_OEM_GTIN_2      = 0x05
DA_MEMORY_0_OEM_GTIN_3      = 0x06
DA_MEMORY_0_OEM_GTIN_4      = 0x07
DA_MEMORY_0_OEM_GTIN_5      = 0x08
DA_MEMORY_0_REVISION_MSB    = 0x09  # Firmware version major
DA_MEMORY_0_REVISION_LSB    = 0x0A  # Firmware version minor
DA_MEMORY_0_OEM_SERIAL_1    = 0x0B  # OEM serial number Byte 1 (MSB)
DA_MEMORY_0_OEM_SERIAL_2    = 0x0C
DA_MEMORY_0_OEM_SERIAL_3    = 0x0D
DA_MEMORY_0_OEM_SERIAL_4    = 0x0E
DA_MEMORY_0_OEM_EXTRA       = 0x0F  # Additional OEM information

# Memory bank 1 register addresses
DA_MEMORY_1_LAST_PLACE       = 0x00  # Address of last accessible memory location
DA_MEMORY_1_CHECKSUM         = 0x01  # Checksum memory bank 1
DA_MEMORY_1_LOCK             = 0x02  # Lock byte (locked: 0xFF, unlocked: 0x55)
DA_MEMORY_1_OEM_GTIN_0       = 0x03  # OEM GTIN Byte 0 (MSB)
DA_MEMORY_1_OEM_GTIN_1       = 0x04
DA_MEMORY_1_OEM_GTIN_2       = 0x05
DA_MEMORY_1_OEM_GTIN_3       = 0x06
DA_MEMORY_1_OEM_GTIN_4       = 0x07
DA_MEMORY_1_OEM_GTIN_5       = 0x08
DA_MEMORY_1_OEM_SERIAL_1     = 0x09  # OEM serial number Byte 1 (MSB)
DA_MEMORY_1_OEM_SERIAL_2     = 0x0A
DA_MEMORY_1_OEM_SERIAL_3     = 0x0B
DA_MEMORY_1_OEM_SERIAL_4     = 0x0C  # OEM serial number Byte 4 (LSB)
DA_MEMORY_1_SUBSYSTEM_DEVNUM = 0x0D  # Subsystem (Bit 7..4), Device number (Bit 3..0)
DA_MEMORY_1_LIGHT_NUM_LOCK   = 0x0E  # Lamp number (lockable)
DA_MEMORY_1_LIGHT_NUM        = 0x0F  # Lamp type (not lockable)
DA_MEMORY_1_OEM_EXTRA        = 0x10  # Additional OEM information

DA_MEMORY_LOCK   = 0xFF  # Value to lock memory bank 1
DA_MEMORY_UNLOCK = 0x55  # Value to unlock memory bank 1


# -----------------------------------------------------------------------
# IEC 62386-209 Color control commands  (YAAA AAA1 xxxx xxxx)
# -----------------------------------------------------------------------

DA_209_SET_TEMP_X_COORD                = 0xE0  # 224
DA_209_SET_TEMP_Y_COORD                = 0xE1  # 225
DA_209_ACTIVATE                        = 0xE2  # 226
DA_209_X_COORD_STEP_UP                 = 0xE3  # 227
DA_209_X_COORD_STEP_DOWN               = 0xE4  # 228
DA_209_Y_COORD_STEP_UP                 = 0xE5  # 229
DA_209_Y_COORD_STEP_DOWN               = 0xE6  # 230
DA_209_SET_TEMP_COLOR_TEMPERATURE      = 0xE7  # 231
DA_209_COLOR_TEMP_STEP_COOLER          = 0xE8  # 232
DA_209_COLOR_TEMP_STEP_WARMER          = 0xE9  # 233
DA_209_SET_TEMP_PRIMARY_N_DIMMLVL      = 0xEA  # 234
DA_209_SET_TEMP_RGB_DIMMLVL            = 0xEB  # 235
DA_209_SET_TEMP_WAF_DIMMLVL            = 0xEC  # 236
DA_209_SET_TEMP_RGBWAF_CONTROL         = 0xED  # 237
DA_209_COPY_REPORT_TO_TEMP             = 0xEE  # 238
DA_209_STORE_TY_PRIMARY_N              = 0xF0  # 240
DA_209_STORE_XY_COORD_PRIMARY_N        = 0xF1  # 241
DA_209_STORE_COLOR_TEMPERATURE_T_LIMIT = 0xF2  # 242
DA_209_STORE_GEAR_FEATURE_STATUS       = 0xF3  # 243
DA_209_ASSIGN_COLOR_TO_LINKED_CHANNEL  = 0xF5  # 245
DA_209_START_AUTO_CALIBRATION          = 0xF6  # 246
DA_209_QUERY_GEAR_FEATURE_STATUS       = 0xF7  # 247
DA_209_QUERY_COLOR_STATUS              = 0xF8  # 248
DA_209_QUERY_COLOR_TYPE_FEATURES       = 0xF9  # 249
DA_209_QUERY_COLOR_VALUE               = 0xFA  # 250
DA_209_QUERY_RGBWAF_CONTROL            = 0xFB  # 251
DA_209_QUERY_ASSIGNED_COLOR            = 0xFC  # 252
DA_209_QUERY_EXTENDED_VERSION_NUMBER   = 0xFF  # 255
DA_209_ENABLE_DEVICE_TYPE_8            = 0x110  # 272


# -----------------------------------------------------------------------
# Fade time / fade rate lookup tables
# fadetime = [0,0.7,1.0,1.4,2.0,2.8,4.0,5.7,8.0,11.3,16.0,22.6,32.0,45.3,64.0,90.5]  # -> index = DALI value 0..15
# faderate = [0,358,253,179,127,89.4,63.3,44.7,31.6,22.4,15.8,11.2,7.9,5.6,4.0,2.8]   # -> index = DALI value 0..15, 0 is impossible!
# -----------------------------------------------------------------------


# -----------------------------------------------------------------------
# DALI default values
# -----------------------------------------------------------------------

DA_DEFAULT_MAX         = 254
DA_DEFAULT_MIN         = 1
DA_DEFAULT_SYSTEM_FAIL = 254
DA_DEFAULT_POWER_ON    = 254
DA_DEFAULT_FADE_RATE   = 7
DA_DEFAULT_FADE_TIME   = 0
DA_DEFAULT_MAX_SHORT   = 63
DA_DEFAULT_MAX_GROUP   = 15
DA_DEFAULT_MAX_SCENE   = 15
