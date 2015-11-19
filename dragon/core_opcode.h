/*********************************************************************/
/*                  Copyright 2006 JDS UNIPHASE                      */
/*********************************************************************

    DESCRIPTION: This header contains core command opcode.

    HISTORY
    VER DATE		NAME			DESCRIPTION
    07/12/2006		J.YE			Created
**********************************************************************/
#ifndef CORE_OPCODE_
#define CORE_OPCODE_

#define CORE_OPCODE_START			0
#define PROC_CORE_OPCODE_START		0x100
#define FACT_CORE_OPCODE_START		0x200
#define INT_CORE_OPCODE_START		0x300


enum
{
        /* ------------------------
         * core Opcode
         * ----------------------*/
        CLS_OPCODE = CORE_OPCODE_START,		// 0
        ESE_OPCODE,							// 1
        ESEQ_OPCODE,						// 2
        ESRQ_OPCODE,						// 3
        IDNQ_OPCODE,						// 4

        OPC_OPCODE,							// 5
        OPCQ_OPCODE,						// 6
        RST_OPCODE,							// 7
        SRE_OPCODE,							// 8
        SREQ_OPCODE,						// 9

        STBQ_OPCODE,						// A
        TSTQ_OPCODE,						// B
        WAI_OPCODE,							// C
        REM_OPCODE,

        BUSYQ_OPCODE,						// E
        RESET_OPCODE,						// F
        TESTQ_OPCODE,						// 10
        CONFQ_OPCODE,						// 11

        INFOQ_OPCODE,						// 12
        LOCK_OPCODE,						// 13
        LOCKQ_OPCODE,						// 14
        ERRQ_OPCODE,						// 15
        ERR_CLS_OPCODE						// 16
};

enum
{

        // proc:cmds
        EVTC_OPCODE =PROC_CORE_OPCODE_START,	// 0x100  event channel opcode
        SAV_OPCODE,							// 18
        RCL_OPCODE,							// 19
        SDS_OPCODE,							// 1A
        TBA_SHUTDOWN_OPCODE,				// 1D
        OSCMD_OPCODE,						// 1B
        NDEVQ_OPCODE,						// 1C PROC:NDEV?
        DSTATUSQ_OPCODE,					// 1C PROC:DSTATUS?

        PROC_GUI_OPCODE,					// 0x108 PROC:GUI
        PROC_PROFILEQ_OPCODE,				// 0x109 PROC:PROF?
        PROC_PROPERTYQ_OPCODE,				// 0x10A PROC:PROP?

        // --------------------------
        //	:LOCK:cmd
        // --------------------------
        LOCK_REALE_OPCODE,					// 1E

        /* --------------------------------------------------
         * :DEVice:cmd
         * --------------------------------------------------*/
        DEV_INFOQ_OPCODE,					// 1F

        /* --------------------------------------------------
         * :FAULt:cmd
         * --------------------------------------------------*/
        FAULT_DEVQ_OPCODE,					// 20  FAUlt:cmd
        FAULT_SLOTQ_OPCODE,					// 21

        /* --------------------------------------------------
         * :STATus:cmd
         * --------------------------------------------------*/
        STAT_DEVQ_OPCODE,					// 0x110
        STAT_DEVRST_OPCODE,					// 0x111

        // -------------------------------------------------
        // GUI UPDATE command
        // ------------------------------------------------
        GUI_UPDATE_DEV_STATUS_OPCODE,		// 0x112
        GUI_UPDATE_DEV_FAULT_OPCODE			// 0x113


};

enum
{
        /* --------------------------------------------------
         * :FACTory:cmd
         * --------------------------------------------------*/
        FACT_WRITE_OPCODE = FACT_CORE_OPCODE_START,		// 24 FACT:CCI:WRITE
        FACT_READ_OPCODE,					// 25 FACT:CCI:READ?
        FACT_EPWR_OPCODE,					// 26
        FACT_EPREAD_OPCODE,					// 27
        FACT_CRCCHK_OPCODE,					// 28
        FACT_MONITOR_OPCODE,				// 29

        FACT_UPIMAGE_OPCODE,				//    FACT:UPIMage
        FACT_DIAG_OPCODE,					// 29 FACT:DIAG?
        FACT_MODULE_INFOQ_OPCODE,			// 29 FACT:MODUle:INFOrmation?
        FACT_MODULE_UPDATEQ_OPCODE,

        FACT_MMEM_STRING_OPCODE,
        FACT_MMEM_STRINGQ_OPCODE,

        FACT_FLASH_NAME_OPCODE,				// 29 FACT:FLASh:NAME
        FACT_FLASH_NAMEQ_OPCODE,			// 29 FACT:FLASh:NAME?
        FACT_FLASH_VRID_OPCODE,				// 29 FACT:FLASh:VRID
        FACT_FLASH_VRIDQ_OPCODE,			// 29 FACT:FLASh:VRID?
        FACT_FLASH_SN_OPCODE,				// 29 FACT:FLASh:SN
        FACT_FLASH_SNQ_OPCODE,				// 29 FACT:FLASh:SN?
        FACT_FLASH_PN_OPCODE,				// 29 FACT:FLASh:PN
        FACT_FLASH_PNQ_OPCODE,				// 29 FACT:FLASh:PN?
        FACT_FLASH_HWREV_OPCODE,			// 29 FACT:FLASh:HWREV
        FACT_FLASH_HWREVQ_OPCODE,			// 29 FACT:FLASh:HWREV?
        FACT_FLASH_DATE_OPCODE,				// 29 FACT:FLASh:DATE
        FACT_FLASH_DATEQ_OPCODE,			// 29 FACT:FLASh:DATE?
        FACT_FLASH_DES_OPCODE,				// 29 FACT:FLASh:DES
        FACT_FLASH_DESQ_OPCODE,				// 29 FACT:FLASh:DES?
        FACT_FLASH_NDEV_OPCODE,				// 29 FACT:FLASh:NDEV
        FACT_FLASH_NDEVQ_OPCODE,			// 29 FACT:FLASh:NDEV?
        FACT_FLASH_SAVE_OPCODE,				// 29 FACT:FLASh:SAVE
        FACT_FLASH_INFO_OPCODE,				// 29 FACT:FLASh:INFO?
        FACT_FLASH_REST_OPCODE,				// 29 FACT:FLASh:REST
};

enum
{
        /* --------------------------------------------------
         * Internal opcode
         * -------------------------------------------------*/
        CARD_INIT_OPCODE = INT_CORE_OPCODE_START,	// 2C
        CARD_STATUSQ_OPCODE,				// 2D
        CARD_NUMDEVQ_OPCODE,				// 2E

        CARD_CONFIGQ_OPCODE,				// 2F
        DEV_PROFILEQ_OPCODE,				// 30

        MON_TEMP_CHECK_OPCODE,				// 31
        MON_BUSY_CHECK_OPCODE,				// 32
        MON_BUSY_CHECK_ENABLE_OPCODE,		// 33
        MON_STATUS_CHECK_OPCODE,			// 34

        MON_OPC_CHECK_OPCODE,				// 35
        MON_OPC_CHECK_ENABLE_OPCODE,		// 36
        MON_STATUS_QUERY_OPCODE,			// 37
        MON_CARD_CONFIG_OPCODE,				// 38
        MON_CARD_CONFIG_GUI_OPCODE,			// 39	// card config and update GUI
        MON_CARD_CONFIG_STARTUP_OPCODE,		// 3A	// card config for initialization
        MON_CARD_CONFIG_END_OPCODE,			// 3B 	// finish card config for initialization
        INTERNAL_SM_CSTATUS_OPCODE,			// 3C
        MON_CARD_LED_OPCODE,				//		card led opcode

        MODU_SHUTDOWN_OPCODE,					// module reset

        PSR_SUPER_STATUS_OPCODE,
        PSR_SOCKET_OPCODE,

        END_OF_CORE_OPCODE,

};
;


#endif /*CORE_OPCODE_*/
