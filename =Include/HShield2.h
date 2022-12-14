/*++ HackShield SDK header file   

Copyright (C), AhnLab, Inc. 2002-2008, All rights reserved.

Module Name:

    HShield.h

Abstract:

    This header file defines the data types and constants, and exposes APIs 
	that are defined by HackShield Library.

Tag Information:
	
	This header file is auto-generated at $$HSHIELD_BUILD_DATETIME$$.

--*/
           
#ifndef _HSHIELD_H_INC
#define _HSHIELD_H_INC

// HShield Library Version Info
#define STATE_DEV		0x00
#define STATE_ALPHA		0x01
#define STATE_BETA		0x02
#define STATE_RC_RTM	0x03
//#define	HSVERSION(M,m,p,s1,s2,b) (M&0x0F)<<28|(m&0x0F)<<24|(p&0x1F)<<19|(s1&0x03)<<17|(s2&0x07)<<14|(b&0x3FFF)

// Initialize Option
#define AHNHS_CHKOPT_SPEEDHACK					0x2
#define AHNHS_CHKOPT_READWRITEPROCESSMEMORY 	0x4
#define AHNHS_CHKOPT_KDTRACER					0x8
#define AHNHS_CHKOPT_OPENPROCESS				0x10
#define AHNHS_CHKOPT_AUTOMOUSE					0x20
#define AHNHS_CHKOPT_MESSAGEHOOK                0x40
#define AHNHS_CHKOPT_PROCESSSCAN                0x80


#define AHNHS_CHKOPT_ALL (	AHNHS_CHKOPT_SPEEDHACK \
							| AHNHS_CHKOPT_READWRITEPROCESSMEMORY \
							| AHNHS_CHKOPT_KDTRACER \
							| AHNHS_CHKOPT_OPENPROCESS \
							| AHNHS_CHKOPT_AUTOMOUSE \
							| AHNHS_USE_LOG_FILE \
							| AHNHS_ALLOW_SVCHOST_OPENPROCESS \
							| AHNHS_ALLOW_LSASS_OPENPROCESS \
							| AHNHS_ALLOW_CSRSS_OPENPROCESS \
							| AHNHS_DONOT_TERMINATE_PROCESS )

#define AHNHS_USE_LOG_FILE                      0x100
#define AHNHS_ALLOW_SVCHOST_OPENPROCESS         0x400
#define AHNHS_ALLOW_LSASS_OPENPROCESS           0x800
#define AHNHS_ALLOW_CSRSS_OPENPROCESS           0x1000
#define AHNHS_DONOT_TERMINATE_PROCESS           0x2000
#define AHNHS_DISPLAY_HACKSHIELD_LOGO           0x4000
#define AHNHS_CHKOPT_LOADLIBRARY				0x10000

#define AHNHS_CHKOPT_PROTECTSCREEN				0x20000
#define AHNHS_CHKOPT_PROTECTSCREENEX			0x40000
#define AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION	0x80000
#define AHNHS_CHKOPT_ABNORMAL_FUNCTION_CALL		0x100000
#define AHNHS_CHKOPT_ANTIFREESERVER				0x200000
#define AHNHS_ALLOW_SWITCH_WINDOW				0x400000
#define AHNHS_CHKOPT_STANDALONE					0x800000
#define AHNHS_CHKOPT_PROTECT_D3DX				0x1000000
#define AHNHS_CHKOPT_SELF_DESTRUCTION			0x2000000
#define AHNHS_DISPLAY_HACKSHIELD_TRAYICON		0x4000000
#define AHNHS_CHKOPT_DETECT_VIRTUAL_MACHINE		0x8000000
#define	AHNHS_CHKOPT_UPDATED_FILE_CHECK			0x10000000
#define AHNHS_CHKOPT_SEND_MONITOR_ONCE			0x20000000
#define AHNHS_CHKOPT_SEND_MONITOR_DELAY			0x40000000
#define AHNHS_CHKOPT_ABNORMAL_FUNCTION_CALL_V2	0x80000000


// SpeedHack Sensing Ratio
#define	AHNHS_SPEEDHACK_SENSING_RATIO_HIGHEST	0x1
#define	AHNHS_SPEEDHACK_SENSING_RATIO_HIGH		0x2
#define	AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL	0x4
#define	AHNHS_SPEEDHACK_SENSING_RATIO_LOW		0x8
#define	AHNHS_SPEEDHACK_SENSING_RATIO_LOWEST	0x10
#define	AHNHS_SPEEDHACK_SENSING_RATIO_GAME		0x20

//
// SpeedHack Extra option
//
#define AHNHS_SPEEDHACK_EXOPT_ALLOW_SLOW		0x80000000

// SelfDestruction Ratio
#define	AHNHS_SELFDESTRUCTION_RATIO_FAST			0x100	
#define	AHNHS_SELFDESTRUCTION_RATIO_NORMAL			0x200	
#define	AHNHS_SELFDESTRUCTION_RATIO_SLOW			0x400	
#define	AHNHS_SELFDESTRUCTION_RATIO_VERYSLOW		0x800	





//ERROR CODE
#define HS_ERR_OK                               0x00000000		
#define HS_ERR_UNKNOWN                          0x00000001		
#define HS_ERR_INVALID_PARAM                    0x00000002		
#define HS_ERR_NOT_INITIALIZED                  0x00000003		
#define HS_ERR_COMPATIBILITY_MODE_RUNNING       0x00000004		
#define HS_ERR_ALREADY_UNINITIALIZED			0x00000005		
#define HS_ERR_STRING_CONVERSION_FAILED			0x00000006	
#define HS_ERR_EXCEPTION_RAISED					0x00000007		
	
#define HS_ERR_INVALID_LICENSE                  0x00000100		
#define HS_ERR_INVALID_FILES                    0x00000101		
#define HS_ERR_INIT_DRV_FAILED                  0x00000102
#define HS_ERR_ANOTHER_SERVICE_RUNNING          0x00000103		
#define HS_ERR_ALREADY_INITIALIZED              0x00000104		
#define HS_ERR_DEBUGGER_DETECT                  0x00000105		
#define HS_ERR_EXECUTABLE_FILE_CRACKED			0x00000106		
#define HS_ERR_NEED_ADMIN_RIGHTS				0x00000107		
#define HS_ERR_MODULE_INIT_FAILED				0x00000108	

#define HS_ERR_START_ENGINE_FAILED              0x00000200		
#define HS_ERR_ALREADY_SERVICE_RUNNING          0x00000201		
#define HS_ERR_DRV_FILE_CREATE_FAILED           0x00000202		
#define HS_ERR_REG_DRV_FILE_FAILED              0x00000203		
#define HS_ERR_START_DRV_FAILED                 0x00000204		
#define HS_ERR_START_THREAD_FAILED			    0x00000205 
#define HS_ERR_ALREADY_GAME_STARTED	            0x00000206		
#define HS_ERR_VIRTUAL_MACHINE_DETECT			0x00000207		

#define HS_ERR_SERVICE_NOT_RUNNING              0x00000301		
#define HS_ERR_SERVICE_STILL_RUNNING            0x00000302		
#define HS_ERR_ALREADY_EXISTED					0x00000303		
#define HS_ERR_INVALID_DATA						0x00000304		
#define HS_ERR_NOT_SUPPORTED					0x00000305		

#define HS_ERR_NEED_UPDATE                      0x00000401		

#define HS_ERR_API_IS_HOOKED					0x00000501		

#define HS_ERR_GET_SDKVERSION_FAILED			0x00000601	

#define HS_ERR_LMP_START						0x00000701		


#define HS_ERR_HSMS_WAIT_TIME_OUT				0x00000801
#define HS_ERR_HSMS_WAIT_FAILED					0x00000802
#define HS_ERR_HSMS_NOT_RUNNING					0x00000803		

// Server-Client ERROR CODE
#define HS_ERR_ANTICPCNT_MAKEACKMSG_INVALIDPARAM				0x00010000
#define HS_ERR_ANTICPCNT_MAKEACKMSG_MAKESESSIONKEY_FAIL			0x00010001
#define HS_ERR_ANTICPCNT_MAKEACKMSG_INITCRYPT_FAIL				0x00010002
#define HS_ERR_ANTICPCNT_MAKEACKMSG_DECRYPTMESSAGE_FAIL			0x00010003
#define HS_ERR_ANTICPCNT_MAKEACKMSG_GETMEMHASH_FAIL				0x00010004
#define HS_ERR_ANTICPCNT_MAKEACKMSG_GETMODNAME_FAIL				0x00010005
#define HS_ERR_ANTICPCNT_MAKEACKMSG_GETFILEHASH_FAIL			0x00010006
#define HS_ERR_ANTICPCNT_MAKEACKMSG_GETHSHIELDHASH_FAIL			0x00010007
#define HS_ERR_ANTICPCNT_MAKEACKMSG_MAKEHSHIELDFLAG_FAIL		0x00010008
#define HS_ERR_ANTICPCNT_MAKEACKMSG_MAKEACKSEQUENCENUM_FAIL		0x00010009
#define HS_ERR_ANTICPCNT_MAKEACKMSG_ENCRYPTMESSAGE_FAIL			0x0001000A

#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_INVALIDPARAM			0x00010010
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_MAKESESSIONKEY_FAIL		0x00010011
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_INITCRYPT_FAIL			0x00010012
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_DECRYPTMESSAGE_FAIL		0x00010013
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_GETGUIDFROMFILE_FAIL	0x00010014
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_MAKEACKSEQUENCENUM_FAIL	0x00010015
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_ENCRYPTMESSAGE_FAIL		0x00010016	

#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_INVALIDPARAM			0x00010020
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_OPENFILEMAPPING_FAIL	0x00010021
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_MAPVIEWOFFILE_FAIL		0x00010022
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_INITCRYPT_FAIL			0x00010023
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_DECRYPTMESSAGE_FAIL	0x00010024
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_INVALIDMMF				0x00010025
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_OPENEVENT_FAIL			0x00010026
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_GETMODFILENAME_FAIL	0x00010027
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_GETINSTRUCTION_FAIL	0x00010028
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_ENCRYPTMESSAGE_FAIL	0x00010029

#define HS_ERR_ANTICPCNT_DEBUGGER_DETECTED						0x00010030


// Server-Client Ex ฟฌตฟ น๖ภ ฟกทฏ ฤฺตๅ มคภว
#define HS_ERR_ANTICPXCNT_BASECODE								0xE4010000

// ฦฤถ๓นฬลอ ฐชภฬ ฟรนูธฃม๖ พสฝภดฯดู
#define HS_ERR_ANTICPXCNT_INVALID_PARAMETER						(HS_ERR_ANTICPXCNT_BASECODE + 0x1)
// ภ฿ธ๘ตศ ธธ๐ธฎ มึผาธฆ มขฑูวฯฟดฝภดฯดู
#define HS_ERR_ANTICPXCNT_INVALID_ADDRESS						(HS_ERR_ANTICPXCNT_BASECODE + 0x2)
// ธธ๐ธฎฐก บฮมทวีดฯดู
#define HS_ERR_ANTICPXCNT_NOT_ENOUGH_MEMORY						(HS_ERR_ANTICPXCNT_BASECODE + 0x3)
// CRC Tableภป รสฑโศญ วฯดยตฅ ฝวฦะวฯฟดฝภดฯดู
#define HS_ERR_ANTICPXCNT_CRC_TABLE_INIT_FAILED					(HS_ERR_ANTICPXCNT_BASECODE + 0x4)
// ธฝรม๖ ฑๆภฬฐก ฟรนูธฃม๖ พสฝภดฯดู
#define HS_ERR_ANTICPXCNT_BAD_LENGTH							(HS_ERR_ANTICPXCNT_BASECODE + 0x5)
// น๖ฦภว ลฉฑโฐก ภฝภดฯดู
#define HS_ERR_ANTICPXCNT_INSUFFICIENT_BUFFER					(HS_ERR_ANTICPXCNT_BASECODE + 0x6)
// ว๖ภ็ น๖ภฟกผญ ม๖ฟ๘วฯม๖ พสดย ฑโดษภฬดู
#define HS_ERR_ANTICPXCNT_NOT_SUPPORTED							(HS_ERR_ANTICPXCNT_BASECODE + 0x7)
// ฦฤภฯภป รฃภป ผ๖ พ๘ฝภดฯดู
#define HS_ERR_ANTICPXCNT_FILE_NOT_FOUND						(HS_ERR_ANTICPXCNT_BASECODE + 0x8)
// ภิทยนภบ ธฝรม๖ภว ลฉฑโฐก ฟรนูธฃม๖ พสฝภดฯดู
#define HS_ERR_ANTICPXCNT_INVALID_MESSAGE_SIZE					(HS_ERR_ANTICPXCNT_BASECODE + 0x9)
// ฟรนูธฅ ฦ๗ธไภฬ พฦดีดฯดู
#define HS_ERR_ANTICPXCNT_BAD_FORMAT							(HS_ERR_ANTICPXCNT_BASECODE + 0xA)
// ต๐น๖ฑื ป๓ศฒภป ฐจม๖วฯฟดฝภดฯดู
#define HS_ERR_ANTICPXCNT_DEBUGGER_DETECTED						(HS_ERR_ANTICPXCNT_BASECODE + 0xB)
// วูฝฏตๅ ธ๐ตโ ฐๆทฮฐก ภ฿ ธ๘ตวพ๚ฐลณช วูฝฏตๅ ธ๐ตโภฬ ฟรนูธฃม๖ พสฝภดฯดู
#define HS_ERR_ANTICPXCNT_BAD_HSHIELD_MODULE					(HS_ERR_ANTICPXCNT_BASECODE + 0xC)
// ลฌถ๓ภฬพ๐ฦฎ ธ๐ตโภฬ ฟรนูธฃม๖ พสฝภดฯดู
#define HS_ERR_ANTICPXCNT_BAD_CLIENT_FILE						(HS_ERR_ANTICPXCNT_BASECODE + 0xD)
// ผญน๖ทฮ บฮลอ ภด นภบ ฟไรป ธฝรม๖ฐก ฟรนูธฃม๖ พสฝภดฯดู
#define HS_ERR_ANTICPXCNT_BAD_REQUEST							(HS_ERR_ANTICPXCNT_BASECODE + 0xE)
// วูฝฏตๅ ฤฺพ๎ ฟฃม๘ภฬ มคป๓ภ๛ภธทฮ ตฟภวฯม๖ พสฝภดฯดู (ต๐นูภฬฝบ ตๅถ๓ภฬน๖)
#define HS_ERR_ANTICPXCNT_HSHIELD_CORE_ENGINE_NOT_WORKING		(HS_ERR_ANTICPXCNT_BASECODE + 0xF)
// วุลท ฝรตตฟก ภววุ ฝรฝบลภฬ ฟภตฟภ วา ฐๆฟ์ น฿ป
#define HS_ERR_ANTICPXCNT_UNKNOWN								(HS_ERR_ANTICPXCNT_BASECODE + 0xFF)


// CallBack Code
#define AHNHS_ACTAPC_STATUS_HACKSHIELD_RUNNING		0x010001		// วูฝฏตๅ ป๓ลย ศฎภฮ ฑโดษภป ป็ฟ๋วฯดย ฐๆฟ์ฟก ศฃรโ ตวดย ฤน้ภิดฯดู.  
																	// วูฝฏตๅฐก มคป๓ภ๛ภธทฮ ตฟภ วฯฐํ ภึดย ตฟพศ ภฬ ฤน้ภฬ ศฃรโ ตหดฯดู. 
																	// ฟภท๙ ฤฺตๅ ฐก พฦดฯนวทฮ ฐิภำฟกผญ ณปบฮภ๛ภธทฮ ศฎภฮ วีดฯดู. 
#define AHNHS_ACTAPC_DETECT_ALREADYHOOKED			0x010101		// ภฯบฮ APIฐก ภฬนฬ ศฤลทตวพ๎ ภึดย ป๓ลยภิดฯดู. (ฑืทฏณช ฝวมฆทฮดย ภฬธฆ ย๗ดวฯฐํ ภึฑโ ถงนฎฟก ศฤลทวมทฮฑืทฅภบ ตฟภวฯม๖ พสฝภดฯดู.)
#define AHNHS_ACTAPC_DETECT_AUTOMOUSE				0x010102		// ฟภลไธถฟ์ฝบ วเภงฐก ฐจม๖ตวพ๚ฝภดฯดู.	
#define AHNHS_ACTAPC_DETECT_AUTOMACRO				0x010104		// ฟภลไธลฉทฮ วเภงฐก ฐจม๖ตวพ๚ฝภดฯดู.	
#define AHNHS_ACTAPC_DETECT_HOOKFUNCTION			0x010301		// บธศฃ APIฟก ด๋วั ศฤลท วเภงฐก ฐจม๖ตวพ๚ฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_DRIVERFAILED			0x010302		// วุลท ย๗ด ตๅถ๓ภฬน๖ฐก ทฮตๅตวม๖ พสพาฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_SPEEDHACK				0x010303		// ฝบววตๅวูท๙ภว วมทฮฑืทฅฟก ภววุ ฝรฝบล ฝรฐฃภฬ บฏฐๆตวพ๚ฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_MESSAGEHOOK				0x010306		// ธฝรม๖ ศฤลทภฬ ฝรตตตวพ๚ภธธ็ ภฬธฆ ย๗ดวฯม๖ ธ๘ว฿ฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_KDTRACE					0x010307		// ต๐น๖ฐล ฦฎทกภฬฝฬภฬ น฿ปว฿ดู.(ฤฟณฮ ต๐น๖ฐล ศฐผบศญ, ภฬศฤ บ๊ทนภฬลฉ ฦ๗ภฮลอ รณธฎ)
#define AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED			0x010308		// ผณฤกตศ ต๐น๖ฐล ฦฎทกภฬฝฬภฬ บฏฐๆตวพ๚ดู.
#define AHNHS_ACTAPC_DETECT_SPEEDHACK_RATIO			0x01030B		// ฝบววตๅวู ฐจม๖ ฟษผวภฬ 'GAME'ภฬ ฐๆฟ์ ภฬ ฤน้ภธทฮ รึฑู 5รสตฟพศภว ฝรฐฃมคบธฐก ภดตหดฯดู.
#define AHNHS_ENGINE_DETECT_GAME_HACK				0x010501		// ฐิภำ วุลทล๘ภว ฝววเภฬ น฿ฐ฿ตวพ๚ฝภดฯดู.
#define AHNHS_ENGINE_DETECT_WINDOWED_HACK			0x010503		// รขธ๐ตๅ วุลทล๘
#define AHNHS_ACTAPC_DETECT_MULTI_LOADING			0x010504		// ลฌถ๓ภฬพ๐ฦฎ ธึฦผ ทฮต๙ ฐจม๖.
#define AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS	0x010601		// ฤฺตๅ ฦะฤก ฝรตตฐก ฐจม๖ตวพ๚ฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_MODULE_CHANGE			0x010701		// วูฝฏตๅ ฐทรธ๐ตโภฬ บฏฐๆตวพ๚ฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_ENGINEFAILED			0x010702		// ฟฃม๘ฑโดษภฬ มคป๓ภ๛ภฬม๖ พสฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_CODEMISMATCH			0x010703		// ฤฺตๅฐก ภฯฤกวฯม๖ พสฝภดฯดู. 
#define AHNHS_ACTAPC_DETECT_PROTECTSCREENFAILED		0x010704		// ศญธ้ บธศฃฑโดษภฬ มคป๓ภ๛ภฬม๖ พสฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP		0x010705		// บธศฃ ธ๐ตโฟก ด๋วุผญ ธธ๐ธฎ บฏมถฐก ฐจม๖ตวพ๚ฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_LMP_FAILED				0x010706		// วูฝฏตๅ ทฮฤร ธธ๐ธฎ บธศฃ ฑโดษภฬ มคป๓ภ๛ภฬม๖ พสฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_RMEM_MODIFY_FROM_LMP	0x010709		// บธศฃ ธ๐ตโฟก ด๋วุผญ ภะฑโภฟ๋ ธธ๐ธฎ บฏมถฐก ฐจม๖ตวพ๚ฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_ABNORMAL_FUNCTION_CALL	0x010801		// Game EXEณปภว ฤฺตๅธฆ ฟบฮ ธ๐ตโฟกผญ ภำภวทฮ ศฃรโวฯฟดฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_ANTIFREESERVER			0x010901		// Gameภฬ ฟรนูธฅ ผญน๖ทฮ มขผำภป วฯม๖ พสพาฝภดฯดู.
#define AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS	0x010A01	// HackShield บธพศ ฑโดษภฬ มคป๓ภ๛ภธทฮ ตฟภวฯม๖ พสฝภดฯดู.


#pragma pack(push)
#pragma pack(8)

#ifndef __ANTICPX_TRANS_BUFFER__
#define __ANTICPX_TRANS_BUFFER__

#define ANTICPX_TRANS_BUFFER_MAX	400

#define	DEFAULT_HSMS_TIME_OUT		(5 * 1000)   // 5รส

typedef struct _AHNHS_TRANS_BUFFER
{
	unsigned char byBuffer[ANTICPX_TRANS_BUFFER_MAX];
	unsigned short nLength;
} AHNHS_TRANS_BUFFER, *PAHNHS_TRANS_BUFFER;

#endif // __ANTICPX_TRANS_BUFFER__

#ifndef __AHNHS_EXT_ERRORINFO__
#define __AHNHS_EXT_ERRORINFO__

typedef struct AHNHS_EXT_ERRORINFOA
{

	CHAR szServer[MAX_PATH];
	CHAR szUserId[MAX_PATH];
	CHAR szGameVersion[MAX_PATH];

} AHNHS_EXT_ERRORINFOA, *PAHNHS_EXT_ERRORINFOA;

typedef struct AHNHS_EXT_ERRORINFOW
{

	WCHAR szServer[MAX_PATH];
	WCHAR szUserId[MAX_PATH];
	WCHAR szGameVersion[MAX_PATH];

} AHNHS_EXT_ERRORINFOW, *PAHNHS_EXT_ERRORINFOW;
#if defined(UNICODE) | defined(_UNICODE)
	#define AHNHS_EXT_ERRORINFO	AHNHS_EXT_ERRORINFOW
#else
	#define	AHNHS_EXT_ERRORINFO	AHNHS_EXT_ERRORINFOA
#endif // !UNICODE

#endif __AHNHS_EXT_ERRORINFO__

#pragma pack(pop) // ฑโมธ alignment บนฑธ.






// CallBackฝร ภดตวดย ตฅภฬลอ
#pragma pack(push)
#pragma pack(1)

typedef struct tagACTAPCPARAM_DETECT_HOOKFUNCTION
{	
	char szFunctionName[128];		// file path
	char szModuleName[128];
	
} ACTAPCPARAM_DETECT_HOOKFUNCTION, *PACTAPCPARAM_DETECT_HOOKFUNCTION;


// ฟภลไธถฟ์ฝบ ฐทร APC Structure
typedef struct tagACTAPCPARAM_DETECT_AUTOMOUSE
{
	BYTE	byDetectType;			// AutoMouse ฐทร API ศฃรโ 1, AutoMouse ฐทร API บฏมถ 2
	DWORD	dwPID;					// AutoMouse วมทฮผผฝบ ถวดย API ฐก บฏมถตศ วมทฮผผฝบ
	CHAR	szProcessName[16+1];	// วมทฮผผฝบธํ
	CHAR	szAPIName[128];			// ศฃรโตศ API ธํ ถวดย บฏมถตศ API ธํ

} ACTAPCPARAM_DETECT_AUTOMOUSE, *PACTAPCPARAM_DETECT_AUTOMOUSE;

#define	EAGLE_AUTOMOUSE_APCTYPE_API_CALLED					1
#define	EAGLE_AUTOMOUSE_APCTYPE_API_ALTERATION				2
#define	EAGLE_AUTOMOUSE_APCTYPE_SHAREDMEMORY_ALTERATION		3

// ฟภลไธลลฉทฮ ฐทร APC Structure
typedef struct
{	
	BYTE byDetectType;		// AutoKeyboard 1, AutoMouse ฐทร API บฏมถ 2
	CHAR szModuleName[128];
	
} ACTAPCPARAM_DETECT_AUTOMACRO, *PACTAPCPARAM_DETECT_AUTOMACRO;

#define	EAGLE_AUTOMACRO_APCTYPE_KEYBOARD	1
#define	EAGLE_AUTOMACRO_APCTYPE_MOUSE		2

// AHNHS_ENGINE_DETECT_GAME_HACK	Callback Structure
typedef struct tagAHNHS_ENGINE_DETECT_GAME_HACK_CALLBACK
{
	CHAR	szMessageA[128+1];		// Detect game hack name
	DWORD	cbMessageLen;			// name length
	DWORD	dwHackNo;

} AHNHS_ENGINE_DETECT_GAME_HACK_CALLBACK, *PAHNHS_ENGINE_DETECT_GAME_HACK_CALLBACK;

// AFC ฟไรป มคบธ ลธภิ นื ฑธมถรผ 
#define EAGLE_AFCEX_QUERYTYPE_THREADCOUNT			0x1
#define EAGLE_AFCEX_QUERYTYPE_FUNCTIONCOUNT			0x2
typedef struct 
{
	DWORD	dwType;
	DWORD	dwIndex;
	int		nCount;
} AHNHS_AFCEX_QUERY_INFO, *PAHNHS_AFCEX_QUERY_INFO;

#pragma pack(pop)


// Server-Client ฟฌตฟ น๖ภฟก ป็ฟ๋ตวดย น๖ฦภว ลฉฑโ
#define SIZEOF_REQMSG		ANTICPX_TRANS_BUFFER_MAX
#define SIZEOF_ACKMSG		ANTICPX_TRANS_BUFFER_MAX
#define SIZEOF_GUIDREQMSG	ANTICPX_TRANS_BUFFER_MAX
#define SIZEOF_GUIDACKMSG	ANTICPX_TRANS_BUFFER_MAX


// HackShield ตฟภ ป๓ลย ฐช  
enum HS_RUNNING_STATUS {
	HS_RUNNING_STATUS_CHECK_MONITORING_THREAD = 1,
};


// Callback Function Prototype
typedef int (__stdcall* PFN_AhnHS_Callback)(
	long lCode,
	long lParamSize,
	void* pParam 
);



// Export API
#if defined(__cplusplus)
extern "C"
{
#endif


#if defined(_NO_HACKSHIELD)

	#define	AHNHS_NOT_NULL	(1)	// ฦ๗ภฮลอฐชภป ธฎลฯวฯดย ฐๆฟ์, NOT NULLทฮ รณธฎ

	// ฑโบป ฑโดษ
	#define	_AhnHS_Initialize(arg1,arg2,arg3,arg4,arg5,arg6)	HS_ERR_OK
	#define	_AhnHS_StartService()				HS_ERR_OK
	#define	_AhnHS_StopService()				HS_ERR_OK
	#define _AhnHS_Uninitialize()				HS_ERR_OK
	#define _AhnHS_PauseService(arg1)			HS_ERR_OK
	#define _AhnHS_ResumeService()				HS_ERR_OK
	#define _AhnHS_CheckHackShieldRunningStatus()			HS_ERR_OK
	#define _AhnHS_SendHsLog(arg1,arg2,arg3)	;	
	#define _AhnHS_VerifyProtectedFunction()	HS_ERR_OK
	#define AHNHS_PROTECT_FUNCTION				__try{\
													HS_ERR_OK;	\
												}__finally{}
	#define AHNHS_PROTECT_FUNCTIONEX(arg1)		__try{\
												arg1=HS_ERR_OK	\
												}__finally{}
	#define _AhnHS_QueryPerformanceCounter(arg1,arg2)	TRUE
	#define _AhnHS_QueryPerformanceFrequency(arg1,arg2)	TRUE
	#define _AhnHS_GetTickCount(arg1)			HS_ERR_OK
		
	// ธ๐ดฯลอธต ผญบ๑ฝบ ฑโดษ
	#define _AhnHS_StartMonitor(arg1,arg2)		HS_ERR_OK
	#define _AhnHS_SetUserId(arg1)				;
	#define _AhnHS_UpdateMonitorInfo(arg1)		;
	#define _AhnHS_SetUserCustomInfo(arg1)		HS_ERR_OK
	#define _AhnHS_SendUserCustomInfo(arg1)		HS_ERR_OK

	// LMP ฑโดษ
	#define	_AhnHS_IsModuleSecure(arg1)			TRUE
	
	// ศฎภๅผญน๖ฟฌตฟ
	#define _AhnHS_MakeResponse(arg1,arg2,arg3)				HS_ERR_OK

	// the others
	#define _AhnHS_CheckAPIHooked(arg1,arg2,arg3)			HS_ERR_OK	
	#define _AhnHS_CheckFileCRC(arg1)						HS_ERR_OK
	#define _AhnHS_PreTranslateBufferedDIMessage(arg1)		HS_ERR_OK
	#define _AhnHS_PreTranslateBufferedDICounter(arg1)		HS_ERR_OK
	#define _AhnHS_PreTranslateImmediateDIMessage(arg1,arg2)	HS_ERR_OK
	#define _AhnHS_Direct3DCreate9(arg1)					AHNHS_NOT_NULL
	#define _AhnHS_Direct3DCreate8(arg1)					AHNHS_NOT_NULL
	#define _AhnHS_sendto(arg1,arg2,arg3,arg4,arg5,arg6)	HS_ERR_OK
	#define _AhnHS_GetSDKVersion(arg1)						HS_ERR_OK

	// AFC v2 Ex.
	#define _AhnHS_ThreadStart()							HS_ERR_OK
	#define _AhnHS_ThreadStop()								HS_ERR_OK
	#define _AhnHS_ThreadStartEx()							HS_ERR_OK
	#define _AhnHS_ThreadStopEx()							HS_ERR_OK
	#define _AhnHS_CheckProtectedStatus(arg1);				HS_ERR_OK
	#define _AhnHS_SetProtectedFunction(arg1);				HS_ERR_OK		
#else	//!_NO_HACKSHIELD

/*!
 * 
 * @remarks	วูฝฏตๅ รสฑโศญ วิผ๖
 *
 * @param	szFileName : [IN] Ehsvc.dllภว Full Path
 * @param	pfn_Callback : [OUT] Callback Function Pointer
 * @param	nGameCode : [IN] Game Code
 * @param	szLicenseKey : [IN] License Code
 * @param	unOption : [IN] Initialize Options
 * @param	unAdditionalRatio : [IN] SpeedHack Sensing Ratio + Self-Destruction Time Ratio
 *
 */
int
__stdcall
_AhnHS_InitializeA (
	const char*			szFileNameA,
	PFN_AhnHS_Callback	pfn_Callback,
	int					nGameCode,
	const char*			szLicenseKeyA,
	unsigned int		unOption,
	unsigned int		unAdditionalRatio
	);
int
__stdcall
_AhnHS_InitializeW (
	const wchar_t*		szFileNameW,
	PFN_AhnHS_Callback	pfn_Callback,
	int					nGameCode,
	const wchar_t*			szLicenseKeyW,
	unsigned int		unOption,
	unsigned int		unAdditionalRatio
	);
#if defined(UNICODE) | defined(_UNICODE) 
	#define _AhnHS_Initialize	_AhnHS_InitializeW
#else
	#define _AhnHS_Initialize	_AhnHS_InitializeA
#endif // !UNICODE

/*!
 * 
 * @remarks	วูฝฏตๅ ฝรภ วิผ๖
 *
 */
int
__stdcall
_AhnHS_StartService ();


/*!
 * 
 * @remarks	วูฝฏตๅ มพทแ วิผ๖
 *
 */
int
__stdcall
_AhnHS_StopService ();


/*!
 * 
 * @remarks	วูฝฏตๅ ฑโดษ ม฿ม๖ วิผ๖ทฮ ว๖ภ็ดย ลฐบธตๅ ฐทร ธฝรม๖ ศลฐทร ฑโดษธธ ม฿ม๖ ฝรลฒดู.
 *
 * @param	unPauseOption : [IN] ม฿ม๖ วฯทมดย ฑโดษภว ฟษผว ( ว๖ภ็ดย AHNHS_CHKOPT_MESSAGEHOOKธธ ฐกดษ)
 *
 */
int
__stdcall
_AhnHS_PauseService (
	unsigned int unPauseOption
	);


/*!
 * 
 * @remarks	วูฝฏตๅ ฑโดษ ภ็ฐณ วิผ๖ทฮ ว๖ภ็ดย ลฐบธตๅ ฐทร ธฝรม๖ ศลฐทร ฑโดษธธ ภ็ฐณ ฝรลฒดู.
 *
 * @param	unResumeOption : [IN] ภ็ฐณ วฯทมดย ฑโดษภว ฟษผว ( ว๖ภ็ดย AHNHS_CHKOPT_MESSAGEHOOKธธ ฐกดษ)
 *
 */
int
__stdcall
_AhnHS_ResumeService (
	unsigned int unResumeOption
	);


/*!
 * 
 * @remarks	วูฝฏตๅ ฟฯภ มพทแ วิผ๖
 *
 */
int
__stdcall
_AhnHS_Uninitialize ();


int 
__stdcall
_AhnHS_CheckAPIHookedA (
	const char*	szModuleNameA,
	const char*	szFunctionNameA,
	const char*	szSpecificPathA
	);
int 
__stdcall
_AhnHS_CheckAPIHookedW (
	const wchar_t*	szModuleNameW,
	const wchar_t*	szFunctionNameW,
	const wchar_t*	szSpecificPathW
	);
#if defined(UNICODE) | defined(_UNICODE)
	#define _AhnHS_CheckAPIHooked	_AhnHS_CheckAPIHookedW
#else
	#define _AhnHS_CheckAPIHooked	_AhnHS_CheckAPIHookedA
#endif // !UNICODE


/*!
 * 
 * @remarks	ผญน๖ทฮบฮลอ ฟไรปนภป ฐๆฟ์ วุด็ ธฝรม๖ฟก ด๋วั ภภดไธฝรม๖ธฆ ธธต็ดู.
 *
 * @param	pbyRequest : [IN] ฟไรป ธฝรม๖ น๖ฦ
 * @param	ulRequestLength : [IN] ฟไรป ธฝรม๖ ฑๆภฬ
 * @param	pResponseBuffer : [OUT] ภภดไ ธฝรม๖ น๖ฦ
 *
 */
int
__stdcall
_AhnHS_MakeResponse (
	unsigned char *pbyRequest,
	unsigned long ulRequestLength, 
	PAHNHS_TRANS_BUFFER pResponseBuffer
	);

/*!
 * 
 * @remarks	DirectInputภว Buffered นๆฝฤฟกผญภว Scan ฤฺตๅ รณธฎ.
 *
 * @param	pbyRequest : [IN] ฟไรป ธฝรม๖ น๖ฦ
 *
 */
int
__stdcall
_AhnHS_PreTranslateBufferedDIMessage ( 
	IN USHORT usMakeCode 
	);

/*!
 * 
 * @remarks	DirectInputภว Buffered นๆฝฤฟกผญภว Scan ฤฺตๅ รณธฎ.
 *
 * @param	dwCnt : [IN] ภิทยภธทฮ รณธฎวฯดย ฐณผ๖ ( key down, upภป ต๛ทฮ รณธฎวฯดย ฐๆฟ์ดย 1 ป็ฟ๋)
 *
 */
int
__stdcall
_AhnHS_PreTranslateBufferedDICounter(DWORD dwCnt = 2);

/*!
 * 
 * @remarks	DirectInputภว Immediate นๆฝฤฟกผญภว Scan ฤฺตๅ รณธฎ.
 *
 * @param	pbyRequest : [IN] ฟไรป ธฝรม๖ น๖ฦ
 * @param	ulRequestLength : [IN] ฟไรป ธฝรม๖ ฑๆภฬ
 * @param	pResponseBuffer : [OUT] ภภดไ ธฝรม๖ น๖ฦ
 *
 */
int
__stdcall
_AhnHS_PreTranslateImmediateDIMessage ( 
   IN PBYTE pBitmap, 
   IN UINT unBitmapSize 
   );

/*!
 * 
 * @remarks	พศภวั Direct3DCreate9 APIธฆ ภงวั Wrapper Function
 *
 * @param	SDKVersion : [IN] Direct3DCreate9 APIภว ภฮภฺทฮ ต้พ๎ฐกดย SDK Version
 *
 */
PVOID
__stdcall
_AhnHS_Direct3DCreate9 (
	IN UINT SDKVersion
	);


/*!
 * 
 * @remarks	พศภวั Direct3DCreate8 APIธฆ ภงวั Wrapper Function
 *
 * @param	SDKVersion : [IN] Direct3DCreate8 APIภว ภฮภฺทฮ ต้พ๎ฐกดย SDK Version
 *
 */
PVOID
__stdcall
_AhnHS_Direct3DCreate8 (
	IN UINT SDKVersion
	);


/*!
 * 
 * @remarks	พศภวั sendto APIธฆ ภงวั Wrapper Function (sendto APIฟอ ตฟภฯวฯฐิ ป็ฟ๋)
 *
 * @param	s : [IN] ผาฤฯ
 * @param	buf : [IN] ภผตษ น๖ฦ
 * @param	len : [IN] น๖ฦภว ฑๆภฬ
 * @param	flags : [IN] วิผ๖ ฟชศฐภป ณชลธณปดย วรทนฑื
 * @param	to : [IN] น๖ฦ(ตฅภฬลอ)ฐก ภผตษ ฟ๘ฐ ศฃฝบฦฎภว พ๎ตๅทนฝบ
 * @param	tolen : to บฏผ๖ภว ลฉฑโ
 *
 */
int
__stdcall
_AhnHS_sendto (			
	IN  int s,
	IN  char* buf,
	IN  int len,
	IN  int flags,
	IN  const struct sockaddr* to,
	IN  int tolen
	);

int
__stdcall
_AhnHS_GetSDKVersion ( 
	OUT DWORD *dwVersion 
	);

/*!
 * 
 * @remarks	วูฝฏตๅ ธ๐ดฯลอธต ผญบ๑ฝบธฆ ฝรภวีดฯดู.
 *
 * @param	IN AHNHS_EXT_ERRORINFO HsExtErrorInfo :  ธ๐ดฯลอธต ฐทร ภิทย มคบธต้
                                  - ผญน๖ URLมึผา. ( "123.456.789.123" or "www.monitor.com" )
								  - User ID		  ( "User1" )
								  - Game Version  ( "5.0.2.1" )
			IN LPCSTR szFileName : ehsvc.dll ภรผ ฐๆทฮ.
 */
int 
__stdcall 
_AhnHS_StartMonitorA ( 
		IN AHNHS_EXT_ERRORINFOA HsExtErrorInfoA,
		IN const char* szFileNameA 
		);		
int
__stdcall
_AhnHS_StartMonitorW ( 
		IN AHNHS_EXT_ERRORINFOW HsExtErrorInfoW,
		IN const wchar_t* szFileNameW 
		);
#if defined(UNICODE) | defined(_UNICODE)
	#define _AhnHS_StartMonitor	_AhnHS_StartMonitorW
#else
	#define _AhnHS_StartMonitor _AhnHS_StartMonitorA
#endif // !UNOCODE	


/*!
 * 
 * @remarks	วูฝฏตๅ ธ๐ดฯลอธต ผญบ๑ฝบธฆ ภงวั พฦภฬต๐ มคบธ ภิทย
 *
 * @param	IN LPCSTR szUserID : พฦภฬต๐ 
 *
 * @remarks
		_AhnHS_StartMonitorฟกผญตต ภฬภฬต๐ มคบธธฆ นภธณช วูฝฏตๅ รสฑโศญ ฝรมกฟก ภฏภ๚ มคบธธฆ พหม๖ ธ๘วฯดย ฐๆฟ์ฐก 
		ภึดู. ต๛ถ๓ผญ พฦภฬต๐ธฆ พฦดย ฝรมกฟก ภฬ วิผ๖ธฆ ศฃรโวฯฟฉ ภฏภ๚มคบธธฆ พ๒ดยดู.
		พฦภฬต๐ธฆ พ๒ฑโ ภฑ๎ม๖ดย พฦภฬต๐ มคบธฐก พ๘ภฬ ฟกทฏฐชภฬ ภผตศดู.
 */
void
__stdcall 
_AhnHS_SetUserIdA ( 
		IN const char* szUserIDA
		);
void
__stdcall
_AhnHS_SetUserIdW ( 
		IN const wchar_t* szUserIDW
		);

#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_SetUserId	_AhnHS_SetUserIdW
#else
	#define _AhnHS_SetUserId	_AhnHS_SetUserIdA
#endif // !UNICODE


/*!
 * 
 * @remarks	วูฝฏตๅ ธ๐ดฯลอธต ผญบ๑ฝบฐก ฝรภตศ ภฬศฤฟก ฐิภำฟกผญ บฏฐๆวฯฐํภฺ วฯดย ธ๐ดฯลอธต มคบธธฆ พ๗ตฅภฬฦฎวัดู.
 *
 * @param	IN AHNHS_EXT_ERRORINFO HsExtErrorInfo :  ธ๐ดฯลอธต ฐทร ภิทย มคบธต้
                                  - Server URL	  ( NULL )
		  - User ID            ( "User1" )
		  - Game Version  ( "5.0.2.1" )
 */
void
__stdcall 
_AhnHS_UpdateMonitorInfoA ( 
		IN AHNHS_EXT_ERRORINFOA HsExtMonInfoA
		);
void
__stdcall
_AhnHS_UpdateMonitorInfoW ( 
		IN AHNHS_EXT_ERRORINFOW HsExtMonInfoW
		);

#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_UpdateMonitorInfo	_AhnHS_UpdateMonitorInfoW
#else
	#define _AhnHS_UpdateMonitorInfo	_AhnHS_UpdateMonitorInfoA
#endif // !UNICODE


/*!
 * 
 * @remarks	วูฝฏตๅฐก ธ๐ดฯลอธต ผญน๖ทฮ มคบธ ภผฝร ฐํฐด ภำภวภว ร฿ฐกมคบธ ฑโภิ
 *
 * @param	IN LPCSTR szUserCostomInfo : UserCostomInfo มคบธ 
 *
 */
int
__stdcall 
_AhnHS_SetUserCustomInfoA ( 
		IN const char* szUserCostomInfoA
		);
int
__stdcall
_AhnHS_SetUserCustomInfoW ( 
		IN const wchar_t* szUserCustomInfoW
		);

#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_SetUserCustomInfo	_AhnHS_SetUserCustomInfoW
#else
	#define _AhnHS_SetUserCustomInfo	_AhnHS_SetUserCustomInfoA
#endif // !UNICODE


/*!
 * 
 * @remarks	ธ๐ดฯลอธต ผญน๖ทฮ ภฏภ๚ฐก มคภววั ฟกทฏมคบธธฆ ภผ.
 *
 * @param	IN DWORD dwTimeout : wait time out value
 *
 */
int
__stdcall 
_AhnHS_SendUserCustomInfoA ( 
		IN const char* szUserCustomInfoA,
		IN DWORD dwTimeout = DEFAULT_HSMS_TIME_OUT
		);
int
__stdcall 
_AhnHS_SendUserCustomInfoW ( 
		IN const wchar_t* szUserCustomInfoW,
		IN DWORD dwTimeout = DEFAULT_HSMS_TIME_OUT
		);

#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_SendUserCustomInfo	_AhnHS_SendUserCustomInfoW
#else
	#define _AhnHS_SendUserCustomInfo	_AhnHS_SendUserCustomInfoA
#endif // !UNICODE

/*!
 * 
 * @remarks	ภิทยตศ ฐๆทฮฟก วุด็วฯดย ธ๐ตโณปฟก วูฝฏตๅ มคบธฐก มคป๓ภ๛ภธทฮ ฦ๗วิตวพ๎ ภึดยม๖ ศฎภฮวัดู.
 *
 * @param	const char* szModulePath : ธ๐ตโ ฐๆทฮ(ภรผ ฐๆทฮ)
 *
 * @remarks
		วุด็ ฐๆทฮภว ธ๐ตโพศฟก CSInspectorฟกผญ ฦะลท ภฬภฟก ปผบวั มคบธฐก  มธภ็วฯดยม๖ ศฎภฮวิ
 * @return
        TRUE : มคป๓ มธภ็ 
		FALSE : มธภ็วฯม๖ พสภฝ
 */

BOOL
__stdcall 
_AhnHS_IsModuleSecureA ( 
		IN const char* szModulePathA
		);
BOOL
__stdcall
_AhnHS_IsModuleSecureW ( 
		IN const wchar_t* szModulePathW
		);
#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_IsModuleSecure	_AhnHS_IsModuleSecureW
#else
	#define _AhnHS_IsModuleSecure	_AhnHS_IsModuleSecureA
#endif // !UNICODE



/*!
 * 
 * @remarks	
 *
 *
 */
int
__stdcall
_AhnHS_CheckFileCRCA ();

int
__stdcall
_AhnHS_CheckFileCRCW ();
#if defined(UNICODE) | defined(_UNICODE) 
	#define _AhnHS_CheckFileCRC	_AhnHS_CheckFileCRCW
#else
	#define _AhnHS_CheckFileCRC	_AhnHS_CheckFileCRCA
#endif // !UNICODE


/*!
 * 
 * วูฝฏตๅฟอ ฐทรตศ ฟฉทฏฐกม๖ นฎมฆธฆ ฐิภำป็ธฆ ล๋วฯม๖ พสฐํ บธฃฐิ รณธฎวฯฑโ ภงวุ ฐณน฿ตศ ฑโดษภธทฮผญ 
 * วุลทภฬ น฿ปวั ศฤ วูฝฏตๅภว ทฮฑืธฆ ฐิภำ ภฏภ๚ฐก ผฑลรวฯฟฉ พศทฆฟก บธณพ ผ๖ ภึดยฑโดษภฬดู.
 *
 * @param	dwError : วูฝฏตๅ ฟกทฏฤฺตๅ.   ex) AHNHS_ENGINE_DETECT_GAME_HACK(0x10501)
 * @param	szUserID : ฐิภำ ภฏภ๚ พฦภฬต๐. ex) "User1"
 *
 * @remarks
 *		วูฝฏตๅ ฟกทฏฤฺตๅดย _AhnHS_Initializeวิผ๖ภว 2น๘ยฐ ภฮภฺฐชฟก ภิทยตวดย วูฝฏตๅ ฤน้วิผ๖ภว 
 *		รนน๘ยฐ ภฮภฺภฮ lCode ฐชภฬดู.
 */

void
__stdcall 
_AhnHS_SendHsLogA ( IN DWORD dwError,
					IN const char* szUserID,
					IN const char* szHShieldPath
				  );
void
__stdcall
_AhnHS_SendHsLogW ( IN DWORD dwError,
					IN const wchar_t* szUserIDW,
					IN const wchar_t* szHShieldPathW
				  );
#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_SendHsLog	_AhnHS_SendHsLogW
#else
	#define _AhnHS_SendHsLog	_AhnHS_SendHsLogA
#endif // !UNICODE

int 
__stdcall 
_AhnHS_CheckHackShieldRunningStatus();


int
__stdcall 
_AhnHS_VerifyProtectedFunction();

#define AHNHS_PROTECT_FUNCTION		\
__try{								\
	_AhnHS_VerifyProtectedFunction();	\
}__finally{}						\


#define AHNHS_PROTECT_FUNCTIONEX(nRet)		\
__try{										\
	nRet = _AhnHS_VerifyProtectedFunction();	\
}__finally{}								\

BOOL
__stdcall 
_AhnHS_QueryPerformanceCounter( LARGE_INTEGER *lpPerformanceCount, int *pErr);

BOOL
__stdcall
_AhnHS_QueryPerformanceFrequency ( LARGE_INTEGER *lpFrequency, int *pErr);

unsigned long 
__stdcall
_AhnHS_GetTickCount (int *pErr);


// AFC v2 ext.
int
__stdcall 
_AhnHS_ThreadStart();

int
__stdcall 
_AhnHS_ThreadStop();

int 
__stdcall 
_AhnHS_ThreadStartEx();

int
__stdcall 
_AhnHS_ThreadStopEx();

int 
__stdcall 
_AhnHS_CheckProtectedStatus( IN OUT PVOID pData );

int
__stdcall 
_AhnHS_SetProtectedFunction( IN DWORD dwIndex );

#endif	//_NO_HACKSHIELD

#if defined(__cplusplus)
}
#endif

#endif _HSHIELD_H_INC
