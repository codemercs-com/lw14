/* IEC62386-209:2011 command list */

//#define DA_209_								//YAAA AAA1 xxxx xxxx
#define DA_209_SET_TEMP_X_COORD					0xE0 //224
#define DA_209_SET_TEMP_Y_COORD					0xE1 //225
#define DA_209_ACTIVATE 						0xE2 //226
#define DA_209_X_COORD_STEP_UP 					0xE3 //227
#define DA_209_X_COORD_STEP_DOWN 				0xE4 //228
#define DA_209_Y_COORD_STEP_UP 					0xE5 //229
#define DA_209_Y_COORD_STEP_DOWN 				0xE6 //230
#define DA_209_SET_TEMP_COLOR_TEMPERATURE 		0xE7//231
#define DA_209_COLOR_TEMP_STEP_COOLER 			0xE8 //232
#define DA_209_COLOR_TEMP_STEP_WARMER 			0xE9 //233
#define DA_209_SET_TEMP_PRIMARY_N_DIMMLVL 		0xEA //234
#define DA_209_SET_TEMP_RGB_DIMMLVL 			0xEB //235
#define DA_209_SET_TEMP_WAF_DIMMLVL 			0xEC //236
#define DA_209_SET_TEMP_RGBWAF_CONTROL 			0xED //237
#define DA_209_COPY_REPORT_TO_TEMP 				0xEE //238
//#define DA_209_* 								0xEF //239 //Reserved
#define DA_209_STORE_TY_PRIMARY_N 				0xF0 //240
#define DA_209_STORE_XY_COORD_PRIMARY_N 		0xF1 //241
#define DA_209_STORE_COLOR_TEMPERATURE_T_LIMIT 	0xF2 //242
#define DA_209_STORE_GEAR_FEATURE_STATUS 		0xF3 //243
//#define DA_209_* 								0xF4 //244 //Reserved 
#define DA_209_ASSIGN_COLOR_TO_LINKED_CHANNEL	0xF5 //245
#define DA_209_START_AUTO_CALIBRATION 			0xF6 //246

#define DA_209_QUERY_GEAR_FEATURE_STATUS 		0xF7 //247
/* Description DA_209_QUERY_GEAR_FEATURE_STATUS:
* Bit 0: Automatic Activation -> "0" = No
* Bit 1..5: Reserved -> "0"
* Bit 6: Automatic Calibration Supported -> "0" = No
* Bit 7: Repeat Automatic Calibtarion Supported -> "0" = No
*/

#define DA_209_QUERY_COLOR_STATUS 				0xF8 //248
/* Description DA_209_QUERY_COLOR_STATUS:
* Bit 0: XY Coords position out of range -> "0" = No
* Bit 1: Colortemperatur 'Tc' out of range -> "0" = No
* Bit 2: Auto calibration running -> "0" = No
* Bit 3: Auto calibraiton success -> "0" = No
* Bit 4: Color display XY Coords active -> "0" = No
* Bit 5: Color display 'Tc' actice -> "0" = No
* Bit 6: Color display 'primary N' active -> "0" = No
* Bit 7: Color display 'RGBWAF' active -> "0" = No
*/

#define DA_209_QUERY_COLOR_TYPE_FEATURES 		0xF9 //249
/* Description DA_209_QUERY_COLOR_TYPE_FEATURES:
* Bit 0: XY-Coord available -> "0" = No
* Bit 1: Colour temperatur 'Tc' available -> "0" = No
* Bit 2..4: Size of primary colours -> "0".."6"
* Bit 5..7: Size of RGBWAF channel -> "0".."6"
*/

#define DA_209_QUERY_COLOR_VALUE 				0xFA //250
/* Description DA_209_QUERY_COLOR_VALUE:
* Datasheet: Table 11, Page 38-39
*/

#define DA_209_QUERY_RGBWAF_CONTROL 			0xFB //251
/* Description DA_209_QUERY_RGBWAF_CONTROL:
* Bit 0: Output channel 0/Red -> "0" = not linked, "1" = linked
* Bit 1: Output channel 1/Green -> "0" = not linked, "1" = linked
* Bit 2: Output channel 2/Blue -> "0" = not linked, "1" = linked
* Bit 3: Output channel 3/White -> "0" = not linked, "1" = linked
* Bit 4: Output channel 4/Amber/Yellow -> "0" = not linked, "1" = linked
* Bit 5: Output channel 5/Freely selectable Color -> "0" = not linked, "1" = linked
* Bit 6..7:  -> "00" = Channel control, "01" = Color control, "10" = Default Color control, "11" = Reserved
*/

#define DA_209_QUERY_ASSIGNED_COLOR 			0xFC// 252
/* Description DA_209_QUERY_ASSIGNED_COLOR:
* Datasheet: Table 12, Page 40
* 0000 0000 = No Color assigned
* 0000 0001 = Red
* 0000 0002 = Green
* 0000 0003 = Blue
* 0000 0004 = White
* 0000 0005 = Amber/Yellow
* 0000 0006 = Freely seletable Color
*/

//#define DA_209_* 								0xFD //253 //Reserved
//#define DA_209_* 								0xFE //254 //Reserved
#define DA_209_QEURY_EXTENDES_VERSION_NUMBER	0xFF //255 //YAAA AAA1 1111 1111
/* Description DA_209_QEURY_EXTENDES_VERSION_NUMBER:
* Return "2"
*/


#define DA_209_ENABLE_DEVICE_TYPE_8 			0x110 //272 //1100 0001 0000 1000