#ifndef CARD_OPCODE_H_
#define CARD_OPCODE_H_


#include "core_opcode.h"

#define SRC_OPCODE_START 		0x1000
#define MODU_OPCODE_START 		0x1100
#define MODU_OPCODE_CAL_START 	0x1200

enum
{
    DIAG_TEST_NONE,
    DIAG_TEST_FPGA,
    DIAG_TEST_FLASH,
    DIAG_TEST_COMM_MODULE,
};


enum
{

    SOUR_STAT_OPCODE = SRC_OPCODE_START,//:SOURce:STATe
    SOUR_STATQ_OPCODE,					//:SOURce:STATe?
    SOUR_MODE_OPCODE,					//:SOURce:MODE
    SOUR_MODEQ_OPCODE,					//:SOURce:MODE?
    SOUR_POW_OPCODE,					//:SOURce:POWer
    SOUR_POWQ_OPCODE,					//:SOURce:POWer?

    SOUR_FREQ_OPCODE,					//:SOURce:FREQuency
    SOUR_FREQQ_OPCODE,					//:SOURce:FREQuency?
    SOUR_TEMPQ_OPCODE,					//:SOURce:TEMPerature?

    SOUR_OFFS_POW_OPCODE,				//:SOURce:OFFSet:POWer
    SOUR_OFFS_POWQ_OPCODE,				//:SOURce:OFFSet:POWer?

    SOUR_CURR_OUTPUT_OPCODE,			//:SOURce:CURRent:OUTPUT
    SOUR_CURR_OUTPUTQ_OPCODE,			//:SOURce:CURRent:OUTPut?
    SOUR_CURR_UNIT_OPCODE,				//:SOURce:CURRent:UNIT
    SOUR_CURR_UNITQ_OPCODE,				//:SOURce:CURRent:UNIT?

    SOUR_MODU_ENAB_OPCODE,				//:SOURce:MODUlation:ENABle
    SOUR_MODU_ENABQ_OPCODE,				//:SOURce:MODUlation:ENABle?
    SOUR_MODU_FREQ_OPCODE,				//:SOURce:MODUlation:FREQuency
    SOUR_MODU_FREQQ_OPCODE,				//:SOURce:MODUlation:FREQuency?
    SOUR_MODU_TAG_OPCODE,				//:SOURce:MODUlation:TAG
    SOUR_MODU_DUTY_OPCODE,				//:SOURce:MODUlation:DUTYCycle
    SOUR_MODU_DUTYQ_OPCODE,				//:SOURce:MODUlation:DUTYCycle?
};

enum
{
    MODU_RESET_OPCODE = MODU_OPCODE_START,
    MODU_IDNQ_OPCODE,
    MODU_TEMPQ_OPCODE,
    MODU_STATUSQ_OPCODE,
    BP_INIT_OPCODE,					//
    BP_INIT_DONE_OPCODE,					//
    BP_MODU_STATUS_OPCODE
};

enum
{

    MODU_CAL_ENAB_OPCODE = MODU_OPCODE_CAL_START,	// module calibration
    MODU_CAL_ENABQ_OPCODE,				// module calibration
    MODU_CAL_SAVE_OPCODE,				// save module calibration data
    MODU_CAL_LOAD_OPCODE,				// load module calibration data
    MODU_CAL_GEN_OPCODE,				// module calibration general
    MODU_CAL_GENQ_OPCODE,				// Query module calibration general
    MODU_CAL_POW_OPCODE,				// module calibration power
    MODU_CAL_POWQ_OPCODE,				// Query module calibration power
    MODU_CAL_CURR_OPCODE,				// module calibration current
    MODU_CAL_CURRQ_OPCODE,				// Query module calibration current
    MODU_CAL_WAV_OPCODE,				// module calibration wavelength
    MODU_CAL_WAVQ_OPCODE,				// Query module calibration wavelength
    MODU_CAL_SETP_OPCODE,				// module calibration setpoint
    MODU_CAL_SETPQ_OPCODE,				// Query module calibration setpoint
    MODU_CAL_THERM_OPCODE,				// module calibration therm
    MODU_CAL_THERMQ_OPCODE,				// Query module calibration therm
    MODU_CAL_TECS_OPCODE,				// module calibration TEC status
    MODU_CAL_TECSQ_OPCODE,				// Query module calibration TEC status
    MODU_CAL_BFMS_OPCODE,				// module calibration BFM status
    MODU_CAL_BFMSQ_OPCODE,				// Query module calibration BFM status
    MODU_CAL_DAC_OPCODE,				// module calibration DAC channel
    MODU_CAL_ADCQ_OPCODE,				// Query module calibration ADC channel

    MON_LASER_STATE_OPCODE,
    MON_TEMP_OPCODE,
    MON_MODULE_WARNING_OPCODE,

    END_OF_CARD_OPCODE,

};


#endif /*CARD_OPCODE_H_*/
