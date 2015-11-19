
/*********************************************************************/
/*                  Copyright 2006 JDS UNIPHASE                      */
/*********************************************************************

  DESCRIPTION: Data structure share with GUI.
  	  
	HISTORY
    VER DATE		NAME			DESCRIPTION
	11/18/2015		J.YE			Created
**********************************************************************/

#ifndef EDFA_COMMON_H_
#define EDFA_COMMON_H_

typedef  unsigned char U8_t;



typedef struct
{
    int socket_id;
    U8_t source;
    U8_t status;
    U8_t rsvd1;
    U8_t rsvd2;
    char owner[24];
    char ipid[20];
}Lock_t;


typedef struct //__attribute__((__packed__))
{
    float	gain_min;					// min gain
    float	gain_max;					// max gain
    float	output_current_min;			// min output current
    float	output_current_max;			// max output current
    float	input_power_min;			// min input power
    float	input_power_max;			// max input power
    float	output_power_min;			// min output power
    float	output_power_max;			// max output power
    float	gsa_min;					// min gain seek accuracy
    float	gsa_max;					// max gain seek accuracy
    float	ght_min;					// min gain hold threshold
    float	ght_max;					// max gain hold threshold
    float	psa_min;					// min power seek accuracy
    float	psa_max;					// max power seek accuracy
    float	pht_min;					// min power hold threshold
    float	pht_max;					// max power hold threshold
    float	los_limit_min;				// min loss output shutdown
    float	los_limit_max;				// max loss output shutdown
    float   atime_min;					// min atime
    float   atime_max;					// max atime
    float   wav_min;					// min wavelength
    float   wav_max;					// max wavelength

    float   current_output;				// current output
    float   current_gain;				// current gain in constant current mode
    float   current_power;				// current power in constant current mode
    float   gain_output;				// gain output in constant power mode
    float   gain_seek_accuracy;			// gain seek accuracy
    float   gain_hold_threshold;		// gain hold threshold
    float   power_output;				// power output in constant power mode
    float   power_seek_accuracy;		// power seek accuracy
    float   power_hold_threshold;		// power hold threshold
    float	los_limit_value;			// los limit value
    float	atime;						// atime
    float	wavelength;					// wavelength

    U8_t	source_mode;				//0:power mode, 1:current mode, 2:gain mode
    U8_t	input_detector_presence;	// input ocm  0 or 1
    U8_t	output_detector_presence;	// output ocm	0 or 1
    U8_t	agc_presence;				// agc control status	0 or 1

    U8_t	als_enable_value;			// 1: enable 0:disable
    U8_t	los_state_value;			// 1: shutdown status, 0 not
    U8_t	laser_state;				// 1: laser source ON, 0: laser source OFF
    U8_t    band_type;					// C or L

}Edfa_Dev_Profile_t;


typedef struct //__attribute__((__packed__))
{
    char name[20];						// card name
    char serial_num[20];				// serial number
    char part_num[24];					// part number
    char fw_rev[8];						// fw revision
    char laser_modu_rev[8];
    char ocm_modu_rev[8];
    char hw_rev[8];						// hw revision
    char date[8];						// date yyyymmdd
    char desp[32];						// description
    U8_t vrid;
    U8_t ndev;
    U8_t rsvd1;
    U8_t rsvd2;

}Edfa_Card_Info_t;






#endif /*EDFA_COMMON_H_*/

