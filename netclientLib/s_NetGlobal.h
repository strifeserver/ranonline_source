#ifndef S_NETGLOBAL_H_
#define S_NETGLOBAL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * \ingroup NetServerLib
 *
 *
 * \par requirements
 * Client win98 or later WinSock2\n
 * Server win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2002.05.30
 *
 * \author Jgkim
 *
 * \par license
 * Copyright(c) Mincoms. All rights reserved.
 * 
 * \todo 
 *
 * \bug 
 *
 */

#define STRSAFE_LIB
#define STRSAFE_NO_DEPRECATE

#include <winsock2.h>
#include <atltime.h>
#include <Mmsystem.h>
#include "minLzo.h"
#include "minUuid.h"

#include "GLDefine.h"
#include "../RanClientLib/G-Logic/GLItemLink/GLItemLink.h"

// nProtect GameGuard Server module
#include "ggsrv.h"

// Apex RSA
#include "gamecode.h"

// Must Install Platform SDK
// Visit and install http://www.microsoft.com/msdownload/platformsdk/sdkupdate/
#define STRSAFE_LIB
#define STRSAFE_NO_DEPRECATE
#include "strsafe.h" // Safe string function

// Auto Link Library Files
#pragma comment (lib, "ws2_32.lib") // Windows socket2 library
#pragma comment (lib, "Winmm.lib") // Windows multimedia library
#pragma comment (lib, "strsafe.lib") // Safe string library
#pragma comment (lib, "ggsrvlib.lib") // nProtect GameGuard server library

static CMinLzo* g_pMinLzo;

// DELETE Function definition
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

// Definitions
// #define can changed to const...
// ex) const int NET_NOTIRY = WM_USER + 99;
#define NET_NOTIFY					WM_USER + 99  // Network Notify Message

//////////////////////////////////////////////////////////////////////////////
// server type [type]
// 1 : Login server
// 2 : Session server
// 3 : Field server
// 4 : Agent server, Game Server
#define NET_SERVER_LOGIN	        1
#define NET_SERVER_SESSION	        2
#define NET_SERVER_FIELD	        3
#define NET_SERVER_AGENT	        4

//////////////////////////////////////////////////////////////////////////////
// Client Slot Type (CNetUser)
#define NET_SLOT_CLIENT				1
#define NET_SLOT_AGENT				2
#define NET_SLOT_FIELD				3
#define NET_SLOT_SESSION			4
#define NET_SLOT_FLDSVR				5

#define NET_DEFAULT_PORT			4141 

//#define NET_DATA_BUFSIZE			1024    // Data Buffer Size
#define NET_DATA_BUFSIZE			2048    // Data Buffer Size //invi fix
#define NET_DATA_MSG_BUFSIZE		4096    // Client Message Buffer Size
#define NET_RECV_POSTED				1		// IOCP receive post
#define NET_SEND_POSTED				0		// IOCP send post
#define NET_DATA_CLIENT_MSG_BUFSIZE 16384   // 8192

#define NET_OK						1
#define NET_ERROR					-1
#define NET_CLIENT_LISTEN			50
#define NET_TIME_OUT				180000	// 1000 msec * 60 sec * 3 = 3 minutes
#define NET_BLOCK_TIME              1800000 // 1000 msec * 60 sec * 30 = 30�е��� block ����� �Ѵ�.

// Connection protocols
// This version not support UDP protocol
#define NET_TCP						0
#define NET_UDP						1

enum EM_NET_STATE
{
	NET_OFFLINE    = 0, //! ���� ����
	NET_ONLINE     = 1, //! ����� ����
	//NET_CLOSE_WAIT = 2, //! ���� ���� ������
};

//#define NET_ONLINE					1
//#define NET_OFFLINE					0

#define NET_MAX_CLIENT				1000	///< Default max client number of per server

#define MAX_WORKER_THREAD		    32       ///< ���ڰ� 4�������϶� �����̹߻�
                                            ///< �ǵ������� 6 ���� �ߴ�.
#ifndef MAX_CHAR_LENGTH
#define MAX_CHAR_LENGTH	            255
#endif

#define MAX_SERVER_GROUP			20		///< �ִ� �����׷�
#define MAX_SERVER_NUMBER			10		///< �����׷�� �ִ� ��������

#define MAX_IP_LENGTH               20      ///< IP ��ȣ ���ڿ� �ִ밪
#define SVR_STATE_UPDATE_TIME		180000	///< 300000 msec = 5 min:���Ӽ��� ���� ������Ʈ

#define FIELDSERVER_MAX				10      ///< ä�δ� �ִ� �ʵ弭�� ����
#define MAX_CHANNEL_NUMBER          10      ///< ������ �ִ� ä�� ����

#define NET_RESERVED_SLOT			100     ///< FIELDSERVER_MAX * MAX_CHANNEL_NUMBER 
                                            ///< ������ ����� ���� ����� Ŭ���̾�Ʈ ���� slot
//#define NET_SESSION_START           90
//#define NET_SESSION_END             99

#define NET_RESERVED_SESSION		99      ///< NET_RESERVED_SLOT - 1	             
                                            ///< ���Ǽ������� ����� ���� ����� Ŭ���̾�Ʈ ���� slot

#define NET_STATE_LOGIN				1		///< �α��μ��� ���ӻ���
#define NET_STATE_AGENT				2		///< ���Ӽ��� ���ӻ���
#define NET_STATE_FIELD				3		///< ���Ӽ��� ���ӻ���
#define NET_STATE_BOARD				4		///< ���Ӽ��� ���ӻ���
#define NET_STATE_CTRL				5		///< ���� ��Ʈ�� ���α׷� ���ӻ���

#define NET_PACKET_HEAD				0
#define NET_PACKET_BODY				1

// string data size
#define USR_ID_LENGTH				20
#define USR_PASS_LENGTH				USR_ID_LENGTH
#define CHR_ID_LENGTH				33
#define USR_RAND_PASS_LENGTH        6
#define MAX_TRACING_NUM				20

#define GLOBAL_CHAT_MSG_SIZE		981
#define CHAT_MSG_SIZE				100
#define TRACING_LOG_SIZE			256
#define DB_SVR_NAME_LENGTH			50
#define DB_NAME_LENGTH				50

// #define MAX_SERVERCHAR_NUM			2	// �ִ� ������ �� �ִ� ĳ���� ��	
// �ذ��� �߰�
#if defined(KRT_PARAM) || defined(_RELEASED) || defined ( KR_PARAM ) || defined ( TW_PARAM ) || defined ( HK_PARAM )|| defined( TH_PARAM ) || defined ( MYE_PARAM ) || defined ( MY_PARAM ) || defined ( CH_PARAM ) || defined ( PH_PARAM ) || defined ( JP_PARAM ) 
#define MAX_ONESERVERCHAR_NUM		16 // �� �������� ����� �ִ� ĳ���� ����
#else
#define MAX_ONESERVERCHAR_NUM		4 // �� �������� ����� �ִ� ĳ���� ����
#endif

#define SERVER_NAME_LENGTH			50
#define MAP_NAME_LENGTH				50
#define MAX_APEX_PACKET_LENGTH		640

#define ENCRYPT_KEY					12

//////////////////////////////////////////////////////////////////////////////
// "GID": GameID   (����ڰ� ���� �Է��ߴ� ID) - e.g. test1     16 �ڸ�
// "UID": UniqueID (�ý����� �ο��� ����� ID) - e.g. 15ftgr     6 �ڸ�
// "SSNHEAD": ������� - e.g. 731201                             6 �ڸ�
// "SEX": ����(M:����, F:����)                                   1 �ڸ�
// "NAMECHECK": �Ǹ�Ȯ�� ����(1=����, 0=������)                  1 �ڸ�
// "IP": ������ IP
// "STATUS": ����� ����(A: ��ȸ��, W: ��ȸ��(��ȹ����), D: Ż���ûȸ��, B: �ҷ�����)
#define DAUM_MAX_PARAM_LENGTH       500
#define DAUM_MAX_GID_LENGTH         20
#define DAUM_MAX_UID_LENGTH         20
#define DAUM_MAX_SSNHEAD_LENGTH     7
#define DAUM_MAX_SEX_LENGTH         2
#define DAUM_USERPASS				20
#define KOR_MAX_GID_LENGTH			20
#define KOR_USERIP					20
#define KOR_ALLOW_TIME_ERROR        20
#define KOR_ROWCOUNT_ERROR	        21
#define KOR_ALLOW_TIME_DIFF_PLUS   21600 // 6 Hour
#define KOR_ALLOW_TIME_DIFF_MINUS -3600 // 1 Hour

//////////////////////////////////////////////////////////////////////////////
// TLoginName (varchar 15):	The most important field which identifies a unique gamer. 
//                          Use this as key to tables in your RAN Game Database 
//                          to maintain a record of each gamer. 
// TGender (char):		    Gender of the gamer 
// TIcNo   (char 15): 		Identity Number (User_No) of the gamer 
// TDOB    (char 10):       Date of Birth of the gamer. 
#define TERRA_TID_ENCODE            96
#define TERRA_TID_DECODE            36
#define TERRA_TLOGIN_NAME           15
#define TERRA_TGENDER               1
#define TERRA_TICNO                 15
#define TERRA_TDOB                  10
#define TERRA_USERPASS				20

// GSP Global Server Platform
#define GSP_USERID	51
#define GSP_USERIP  16
#define GSP_ALLOW_TIME_DIFF_PLUS   21600 // 6 Hour
#define GSP_ALLOW_TIME_DIFF_MINUS -3600 // 1 Hour
#define GSP_ALLOW_TIME_ERROR       20

//////////////////////////////////////////////////////////////////////////////
// Excite Japan
#define EXCITE_USER_ID				20
#define EXCITE_TDATE				14
#define EXCITE_MD5					32
#define EXCITE_SEC_PASS				16
#define EXCITE_SEC_ID				16

//////////////////////////////////////////////////////////////////////////////
// Gonzo Japan
#define JAPAN_USER_ID				16
#define JAPAN_USER_PASS				16

// Grobal Service
#define GS_USER_ID			20
#define GS_USER_PASS		20


//! ������ Ÿ��
enum EMSEVERTYPE
{
	SERVER_LOGIN	= 1,
	SERVER_SESSION	= 2,
	SERVER_FIELD	= 3,
	SERVER_AGENT	= 4,
	SERVER_BOARD	= 5,
};

enum EM_SMS
{
	SMS_DEFAULT  = 0,
	SMS_LENGTH   = 80, // SMS �� ��ü���� ����.
	SMS_SENDER   = 14, // SMS ������ ��� ����.
	SMS_RECEIVER = 14  // SMS �޴� ��� ����.
};

//! ��ŷ���α׷� DB �α׿� ��ȣ
enum EMHACKPROGRAM
{
	DEFAULT_HACK_NUM  =   0,
	NPROTECT_HACK_NUM = 998,
	APEX_HACK_NUM     = 999, // APEX �α�
};

//! �±��� ��������
#define THAI_LIMIT_MIN 180 // �±��� 18 �� ���ϴ� �Ϸ� 180 �� ���ȸ� ���Ӱ���
#define THAI_CHECK_TIME 60000 // 1000 msec X 60 = 1 min

//! �±� ��������
enum EMTHAIUSERFLAG
{
	THAI_UNDER18  = 0, // 18 �� ����
	THAI_OVER18   = 1, // 18 �� �̻�
	THAI_FREETIME = 2, // 24 �ð� ��밡��
};

//! ����� Ÿ�Լ���
enum EMUSERTYPE
{
    USER_COMMON  =  0, // �Ϲݻ����
    USER_PREMIUM =  1, // �����̾� ����
	USER_SPECIAL = 10, // Ư������� (���ӹ� ��ü ����, ���� ���)
	USER_GM4     = 19, // GM 4 ��, Web GM ��
	USER_GM3	 = 20, // GM 3 ��
	USER_GM2	 = 21, // GM 2 ��
	USER_GM1	 = 22, // GM 1 ��
	USER_MASTER  = 30, // Master
};

enum EM_NEW_CHA_FB
{
	EM_CHA_NEW_FB_SUB_OK	= 1, // ĳ���� ���� ����
	EM_CHA_NEW_FB_SUB_DUP	= 2, // ���Ӽ���->Ŭ���̾�Ʈ : ���� �̸��� ĳ���Ͱ� �̹� ������
	EM_CHA_NEW_FB_SUB_LENGTH= 3, // ĳ���� ���ڼ� ����
	EM_CHA_NEW_FB_SUB_MAX	= 4, // ���Ӽ���->Ŭ���̾�Ʈ : ���̻�ĳ���� ����� ����(�ִ�ĳ���� ���� �ʰ�)
	EM_CHA_NEW_FB_SUB_ERROR = 5, // ���Ӽ���->Ŭ���̾�Ʈ : �����߻� ����� ����(�ý��ۿ���, ������ �ʴ� �̸�)
	EM_CHA_NEW_FB_SUB_THAICHAR_ERROR = 6, // �±���� �ϼ����� ���� ���� ���� ����
	EM_CHA_NEW_FB_SUB_VNCHAR_ERROR = 7,	 // ��Ʈ�� ���� ���� ����
	EM_CHA_NEW_FB_SUB_BAD_NAME = 8,
};

enum EMNET_MSG_CHAT
{
	CHAT_TYPE_GLOBAL		= (1), // ������ �۷ι� �޽���
	CHAT_TYPE_NORMAL		= (2), // �Ϲ��� ä�� �޽���
	CHAT_TYPE_PARTY			= (3), // ��Ƽ������ ���޵Ǵ� �޽���
	CHAT_TYPE_PRIVATE		= (4), // �������� �޽���, �Ӹ�, �ӻ���
	CHAT_TYPE_GUILD			= (5), // �������� ���޵Ǵ� �޽���
	CHAT_TYPE_LOUDSPEAKER	= (6), // Ȯ����.
	CHAT_TYPE_ALLIANCE		= (7), // ���Ϳ��� ���޵Ǵ� �޽���

	CHAT_TYPE_CTRL_GLOBAL	= (8),
	CHAT_TYPE_CTRL_GLOBAL2	= (9),
	CHAT_TYPE_GLOBAL_STAFF		= (10),
};

// ���� ��������
enum EMSERVICE_PROVIDER
{
	SP_MINCOMS  = 0, // Korea (Mincoms)
	SP_KOREA    = 1, // Korea (Daum game)
	SP_TAIWAN   = 2, // Taiwan (Feya) 
	SP_CHINA    = 3, // China
	SP_JAPAN    = 4, // Japan
	SP_TERRA    = 5, // PH,VN (Terra) 
	SP_THAILAND = 6, // Thailand (Terra)
	SP_GLOBAL   = 7, // Global Service ( GSP ) 
	SP_MALAYSIA = 8, // MY,MYE 
	SP_GS		= 9, // Global Service ( �ű� ) 
};

enum EM_LOGIN_FB_SUB
{	
    EM_LOGIN_FB_SUB_OK             =  0, // �α��� ����
	EM_LOGIN_FB_SUB_FAIL	       =  1, // �α��� ����
	EM_LOGIN_FB_SUB_SYSTEM	       =  2, // �ý��ۿ����� �α��� ����
	EM_LOGIN_FB_SUB_USAGE	       =  3, // �㰡�� ���ð� ����
	EM_LOGIN_FB_SUB_DUP		       =  4, // �ߺ��� ����
	EM_LOGIN_FB_SUB_INCORRECT      =  5, // ID/PWD ����ġ = (���ο� ������ ���� �Ǵ� ���Է¿䱸),
	EM_LOGIN_FB_SUB_IP_BAN	       =  6, // ID/PWD �� ��ġ������ IP �� ���Ұ��� ����= (���� ���ܵ� IP),	
	EM_LOGIN_FB_SUB_BLOCK          =  7, // ID �� ���ϵ� ����
	EM_LOGIN_FB_SUB_UNCON          =  8, // Daum : �������� �α��ε���Ÿ (���Ӱ� ������ �α����� ���ӽ���)
	EM_LOGIN_FB_SUB_EXPIRED        =  9, // Daum : �����ð��� �ʰ��� ������̱� ������, ���� �������� �϶�� �޽����� ������
	EM_LOGIN_FB_SUB_GID_ERR        = 10, // Daum : GID�� ���� ������ ���Ӱ� �α���
	EM_LOGIN_FB_SUB_UID_ERR        = 11, // Daum : UID�� ���� ������ ���Ӱ� �α���
	EM_LOGIN_FB_SUB_UNKNOWN        = 12, // Daum : ���� ���Խ���
	EM_LOGIN_FB_SUB_SSNHEAD        = 13, // Daum : �ֹι�ȣ����
	EM_LOGIN_FB_SUB_ADULT          = 14, // Daum : ������������
	EM_LOGIN_FB_CH_FULL            = 15, // ä���� Full �� ���� ���ӺҰ�
	EM_LOGIN_FB_THAI_UNDER18_3HOUR = 16, // 18 ���̸� 3 �ð� ��� 
	EM_LOGIN_FB_THAI_UNDER18_TIME  = 17, // 18 ���̸� ���ӺҰ��� �ð�
	EM_LOGIN_FB_THAI_OVER18_TIME   = 18, // 18 ���̻� ���ӺҰ��� �ð�
	EM_LOGIN_FB_SUB_RANDOM_PASS    = 19, // �����н����� ����
	EM_LOGIN_FB_SUB_PASS_OK		   = 20, // �ʱ� �н����� ������Ʈ ����
	EM_LOGIN_FB_SUB_ALREADYOFFLINE = 21, // �α��μ��������� �̹� ���������϶�
	EM_LOGIN_FB_SUB_SECID_ALREADY  = 22, // �Ƶڰ� �ߺ��˴ϴ�.
};

// ģ������ Friend
struct CHA_FRIEND
{
	int nChaS;
	int nChaFlag;
	TCHAR szCharName[CHR_ID_LENGTH+1];

	CHA_FRIEND() 
		: nChaS(0)
		, nChaFlag(0)
	{
		memset( szCharName, 0, sizeof(TCHAR) * (CHR_ID_LENGTH+1) );
	};
};
typedef CHA_FRIEND* LPCHA_FRIEND;

// ģ������(SMS ����)
struct CHA_FRIEND_SMS
{
	int nChaS;
	int nChaFlag;
	TCHAR szCharName[CHR_ID_LENGTH+1];
	TCHAR szPhoneNumber[SMS_RECEIVER];

	CHA_FRIEND_SMS() 
		: nChaS(0)
		, nChaFlag(0)
	{
		memset( szCharName, 0, sizeof(TCHAR) * (CHR_ID_LENGTH+1) );
		memset( szPhoneNumber, 0, sizeof(TCHAR) * (SMS_RECEIVER) );
	};
};
typedef CHA_FRIEND_SMS* LPCHA_FRIEND_SMS;

// BASIC User Information
// 28 Bytes
struct USER_INFO_BASIC
{
	INT		nUserNum;	
	USHORT	nKeyDirection;
	USHORT	nKey;
	TCHAR	szUserID[GSP_USERID];
	
	USER_INFO_BASIC()
	{
		nUserNum = 0;
		nKeyDirection = 0;
		nKey = 0;
		memset( szUserID, 0, sizeof(TCHAR) * (GSP_USERID) );
	};
};
typedef USER_INFO_BASIC* LPUSER_INFO_BASIC;

struct PER_IO_OPERATION_DATA
{
	OVERLAPPED	Overlapped;
	WSABUF		DataBuf;
	CHAR		Buffer[NET_DATA_BUFSIZE];
	BOOL		OperationType;
	DWORD		dwTotalBytes;
	DWORD		dwSndBytes;
	DWORD		dwRcvBytes;
	INT			nRefCount;

	PER_IO_OPERATION_DATA()
	{
		Overlapped.Internal		= 0;
		Overlapped.InternalHigh	= 0;
		Overlapped.Offset		= 0;
		Overlapped.OffsetHigh	= 0;
		Overlapped.hEvent		= 0;
		DataBuf.buf				= Buffer;
		DataBuf.len				= NET_DATA_BUFSIZE;	
		OperationType			= NET_SEND_POSTED;		
		dwTotalBytes			= 0;
		dwSndBytes				= 0;
		dwRcvBytes				= 0;
		nRefCount				= 0; // Set ref count to 0
	};

	void Reset(void)
	{		
		DataBuf.len				= NET_DATA_BUFSIZE;
		OperationType			= NET_SEND_POSTED;
		dwTotalBytes			= 0;
		dwSndBytes				= 0;
		dwRcvBytes				= 0;
		nRefCount				= 0; // Set ref count to 0
	};
};
typedef PER_IO_OPERATION_DATA* LPPER_IO_OPERATION_DATA;

struct G_SERVER_INFO
{
	int	nServerType;			  ///< ���� ����
	int	nServerGroup;			  ///< ���� �׷� ��ȣ
	int	nServerNumber;			  ///< ���� ��ȣ
	int	nServerField;			  ///< �ʵ��ȣ	
	int	nServerMaxClient;		  ///< Max clients
	int	nServerCurrentClient;	  ///< Current connected client number
	int nServerChannel;           ///< Channel number
	int nServerChannelNumber;     ///< Agent's channel number
	int nServerChannellMaxClient; ///< Channel max client
	int	nServicePort;
	int	nControlPort;
	unsigned long ulServerIP;
	bool bPk;                     ///< Channel PK information

	char	szServerName[SERVER_NAME_LENGTH+1];
	char	szServerIP[MAX_IP_LENGTH+1];
	char	szUserID[USR_ID_LENGTH+1];
	char	szUserPass[USR_PASS_LENGTH+1];	

	G_SERVER_INFO()
	{
		memset( szServerName, 0, sizeof(char) * (SERVER_NAME_LENGTH+1) );
		memset( szServerIP,   0, sizeof(char) * (MAX_IP_LENGTH+1) );
		memset( szUserID,     0, sizeof(char) * (USR_ID_LENGTH+1) );
		memset( szUserPass,   0, sizeof(char) * (USR_PASS_LENGTH+1) );

		ulServerIP               = INADDR_NONE;
        nServicePort             = 0;
	    nControlPort             = 0;
	    nServerType              = 0;
	    nServerGroup             = 0;
	    nServerNumber            = 0;
	    nServerField             = 0;
	    nServerMaxClient         = 0;
	    nServerCurrentClient     = 0;
	    nServerChannel           = 0;
	    nServerChannelNumber     = 0;
	    nServerChannellMaxClient = 0;
	    bPk                      = true; 
	};

	friend bool operator==( const G_SERVER_INFO& lhs, const G_SERVER_INFO& rhs )
	{
		if ( (::strcmp(lhs.szServerIP, rhs.szServerIP) == 0) &&
			 (lhs.nServicePort == rhs.nServicePort) )
		{
			return true;
		}
		else 
		{
			return false;
		}
	};
};
typedef G_SERVER_INFO* LPG_SERVER_INFO;

struct G_SERVER_CUR_INFO
{
	int	nServerCurrentClient;	// Corrent connected client number
	int	nServerMaxClient;		// Max clients

	G_SERVER_CUR_INFO()
		: nServerCurrentClient(0)
		, nServerMaxClient(0)
	{
	};
};
typedef G_SERVER_CUR_INFO* LPG_SERVER_CUR_INFO;

struct G_SERVER_CUR_INFO_LOGIN
{
	char szServerIP[MAX_IP_LENGTH+1]; ///< Server IP Address
	int nServicePort;            ///< Server Port
	int nServerGroup;                ///< Server Group Number
	int nServerNumber;               ///< Channel Number or Server Number
	int nServerCurrentClient;	     ///< Channel Current Client
	int nServerMaxClient;		     ///< Channel Max Client
    bool bPK;                         ///< Channel PK information

	G_SERVER_CUR_INFO_LOGIN()
		: nServicePort(0)
		, nServerGroup(0)
		, nServerNumber(0)
		, nServerCurrentClient(0)
		, nServerMaxClient(0)
		, bPK(true)
	{
		memset( szServerIP, 0, sizeof(char) * (MAX_IP_LENGTH+1) );		
	};
};
typedef G_SERVER_CUR_INFO_LOGIN* LPG_SERVER_CUR_INFO_LOGIN;
typedef G_SERVER_CUR_INFO_LOGIN** LPPG_SERVER_CUR_INFO_LOGIN;

// field_server [map id] [map name] [server_name] [ip] [port] 
struct F_SERVER_INFO // Field Server Information
{			
	char szServerName[SERVER_NAME_LENGTH+1];	
	char szServerIP[MAX_IP_LENGTH+1];
	unsigned long ulServerIP;
	int	nServicePort;

	F_SERVER_INFO()
	{
		::memset( this, 0, sizeof(F_SERVER_INFO) );
		ulServerIP = INADDR_NONE;
	};

	friend bool operator== ( const F_SERVER_INFO& lhs, const F_SERVER_INFO& rhs )
	{
		if ( (::strcmp(lhs.szServerIP, rhs.szServerIP) == 0) &&
			 (lhs.nServicePort == rhs.nServicePort) )
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	bool IsValid ()
	{
		return ( szServerName[0] && szServerIP[0] && nServicePort && (ulServerIP!=INADDR_NONE) );
	}
};
typedef F_SERVER_INFO* LPF_SERVER_INFO;

struct CHANNEL_INFO
{
	bool bPK;

	CHANNEL_INFO()
	{
		bPK = true;
	};
};
typedef CHANNEL_INFO* LPCHANNEL_INFO;

///////////////////////////////////////////////////////////////////////////////
// Base Msg
// If you change base message, all message number will be change
// If user find packet order, change NET_MSG_BASE number
// All Message Number must less than 2 byte(65535)

// Memo : �Ʒ� �⺻ �޼����� �ߺ��� ���� �ʵ��� �����Ѵ�.
//
#if defined(CH_PARAM)
	#define NET_MSG_BASE				1016				  // �⺻ �޽���
#elif defined(HK_PARAM)
	#define NET_MSG_BASE				1004				  // �⺻ �޽���
#elif defined(ID_PARAM)
	#define NET_MSG_BASE				974					  // �⺻ �޽���
#elif defined(JP_PARAM)
	#define NET_MSG_BASE				1028				  // �⺻ �޽���
#elif defined(KR_PARAM)
	#define NET_MSG_BASE				984					  // �⺻ �޽���
#elif defined(KRT_PARAM)
	#define NET_MSG_BASE				968					  // �⺻ �޽���
#elif defined(MY_PARAM)
	#define NET_MSG_BASE				967					  // �⺻ �޽���
#elif defined(MYE_PARAM)
	#define NET_MSG_BASE				1002				  // �⺻ �޽���
#elif defined(PH_PARAM)
	#define NET_MSG_BASE				1014				  // �⺻ �޽���
#elif defined(VN_PARAM)
	#define NET_MSG_BASE				1018				  // �⺻ �޽���
#elif defined(TW_PARAM)
	#define NET_MSG_BASE				994					  // �⺻ �޽���
#elif defined(TH_PARAM)
	#define NET_MSG_BASE				986					  // �⺻ �޽���
#elif defined(GS_PARAM)
	#define NET_MSG_BASE				1001				  // �⺻ �޽���
#else
	#define NET_MSG_BASE				992					  // �⺻ �޽���
#endif

/*
#define NET_MSG_LGIN				(NET_MSG_BASE +  507) // �α��μ��� �޽���
#define NET_MSG_LOBBY				(NET_MSG_BASE + 1013) // ���Ӽ��� �κ�
#define NET_MSG_LOBBY_MAX			(NET_MSG_BASE + 1509) // ���Ӽ��� �κ� �ִ밪

#define NET_MSG_GCTRL				(NET_MSG_BASE + 2011) // ���Ӽ��� �����÷���
#define NET_MSG_GCTRL_MAX			(NET_MSG_BASE + 4010) // ���Ӽ��� �����÷��� �ִ밪
*/

// 2005-11-04
//#define NET_MSG_LGIN				(NET_MSG_BASE +  413) // �α��μ��� �޽���
//#define NET_MSG_LOBBY				(NET_MSG_BASE +  913) // ���Ӽ��� �κ�
//#define NET_MSG_LOBBY_MAX			(NET_MSG_BASE + 1413) // ���Ӽ��� �κ� �ִ밪
//
//#define NET_MSG_GCTRL				(NET_MSG_BASE + 1823) // ���Ӽ��� �����÷���
//#define NET_MSG_GCTRL_MAX			(NET_MSG_BASE + 3823) // ���Ӽ��� �����÷��� �ִ밪

// 2007-04-11
#define NET_MSG_LGIN				(NET_MSG_BASE +  450) // �α��μ��� �޽���
#define NET_MSG_LOBBY				(NET_MSG_BASE +  950) // ���Ӽ��� �κ�
#define NET_MSG_LOBBY_MAX			(NET_MSG_BASE + 1450) // ���Ӽ��� �κ� �ִ밪

#define NET_MSG_GCTRL				(NET_MSG_BASE + 1900) // ���Ӽ��� �����÷���
#define NET_MSG_GCTRL_MAX			(NET_MSG_BASE + 3900) // ���Ӽ��� �����÷��� �ִ밪

// Login information
enum EMNET_MSG
{
	NET_MSG_BASE_ZERO           = 0,

	///////////////////////////////////////////////////////////////////////////
	// Version check and downloading new version
	// Do not change version and Crypt key message
	NET_MSG_VERSION_OK			= 100, //< ������ ��ġ��
	NET_MSG_VERSION_INFO		= 110, //< ���� ������ ����
	NET_MSG_VERSION_REQ			= 120, //< ���� ������ ��û��
	NET_MSG_REQ_CRYT_KEY		= 130, //< Ŭ���̾�Ʈ->���Ӽ��� : ��ȣŰ ��û
	NET_MSG_SND_CRYT_KEY		= 140, //< ���Ӽ���->Ŭ���̾�Ʈ : ��ȣŰ ����	
	NET_MSG_RANDOM_NUM          = 141, //< Agent->Client : �����ѹ�
//	NET_MSG_PING_REQUEST		= 150, //< �������� ping �� ��û�Ѵ�.
//	NET_MSG_PING_ANSWER		    = 151, //< �������� ping �� �����Ѵ�.	
    NET_MSG_HEARTBEAT_SERVER_REQ= 153, //< SERVER->SERVER Server HeartBeat Request
    NET_MSG_HEARTBEAT_SERVER_ANS= 154, //< SERVER->SERVER Server HeartBeat Answer
    NET_MSG_SVR_DOWN            = 155, //< Server System Down!
	NET_MSG_SVR_CLOSECLIENT     = 156, //< Server Close Client
	NET_MSG_HEARTBEAT_CLIENT_REQ= 160, //< SERVER->CLIENT Client HeartBeat Request
	NET_MSG_HEARTBEAT_CLIENT_ANS= 161, //< CLIENT->SERVER Client HeartBeat Answer
	NET_MSG_COMPRESS            = 170, //< Compressed message	

	///////////////////////////////////////////////////////////////////////////	
	// Server info message
	NET_MSG_REQ_GAME_SVR		= (NET_MSG_LGIN + 100), //< Ŭ���̾�Ʈ�� -> �α��μ��� : ���� ������ ������ ��û
	NET_MSG_SND_GAME_SVR	    = (NET_MSG_LGIN + 110), //< �α��μ���   -> Ŭ���̾�Ʈ : ���� ������ ������ ����
	NET_MSG_SND_GAME_SVR_END	= (NET_MSG_LGIN + 120), //< �α��μ���   -> Ŭ���̾�Ʈ : ���� ���� ���� ���� ��
	NET_MSG_REQ_FULL_SVR_INFO	= (NET_MSG_LGIN + 130), //< ���Ǽ��� -> �α��μ���, ���Ӽ��� : ���� ��ü ������ ��û�Ѵ�.
	NET_MSG_SND_FULL_SVR_INFO	= (NET_MSG_LGIN + 140), //< �α��μ���, ���Ӽ��� -> ���Ǽ��� : ���� ��ü ������ Session ������ �����Ѵ�.
	NET_MSG_REQ_CUR_STATE		= (NET_MSG_LGIN + 150), //< ���Ǽ��� -> �α��μ���, ���Ӽ��� : ���� ��������= (������),�� ��û�Ѵ�.
	NET_MSG_SND_CUR_STATE		= (NET_MSG_LGIN + 160), //< �α��μ���, ���Ӽ��� -> ���Ǽ��� : ���� ��������= (������),�� �����Ѵ�.
	NET_MSG_SND_CHANNEL_STATE   = (NET_MSG_LGIN + 161), //< Agent->Session : ���� ä�λ��¸� �����Ѵ�.
	NET_MSG_REQ_SVR_INFO		= (NET_MSG_LGIN + 170), //< �α��μ��� -> ���Ǽ��� : ���Ӽ����� ���������� ��û�Ѵ�.
	NET_MSG_SND_SVR_INFO		= (NET_MSG_LGIN + 180), //< ���Ǽ��� -> �α��μ��� : ���Ӽ����� ���������� �����Ѵ�.
	NET_MSG_SND_SVR_INFO_RESET	= (NET_MSG_LGIN + 190), //< ���Ǽ��� -> �α��μ��� : ���Ӽ����� ���������� �����϶�� ����.
	NET_MSG_REQ_ALL_SVR_INFO    = (NET_MSG_LGIN + 191), //< �������α׷�->���Ǽ��� : ��� ������ ���������� ��û�Ѵ�.
	NET_MSG_SND_ALL_SVR_INFO    = (NET_MSG_LGIN + 192), //< ���Ǽ���->�������α׷� : ��� ������ ���������� �����Ѵ�.
	NET_MSG_SND_ALL_SVR_INFO_S  = (NET_MSG_LGIN + 193), //< ���Ǽ���->�������α׷� : ������������ ����
	NET_MSG_SND_ALL_SVR_INFO_E	= (NET_MSG_LGIN + 194), //< ���Ǽ���->�������α׷� : ���������� ��� �������� �˸���.
	NET_MSG_RECONNECT_FIELD     = (NET_MSG_LGIN + 195), //< ���Ǽ���->������Ʈ : �ʵ�� �ٽ� �����϶�.
	NET_MSG_I_AM_AGENT          = (NET_MSG_LGIN + 196), //< Agent->Field : �ڽ��� ������ſ� �������� �˸�

	NET_MSG_LOGOUT_G_S			= (NET_MSG_LGIN + 200), //< Agent->Session : �α׾ƿ� ����	
	NET_MSG_LOGOUT_G_S_S		= (NET_MSG_LGIN + 201), //< Agent->Session : �α׾ƿ� ����. ����� ID �� ����
	DAUM_NET_MSG_LOGOUT_G_S     = (NET_MSG_LGIN + 202), //< Agent->Session : Daum �α׾ƿ� ����

	//< User Information message
	NET_MSG_REQ_USER_INFO		= (NET_MSG_LGIN + 210), //< ����� ������ ��û�Ѵ�.
	NET_MSG_USER_INFO			= (NET_MSG_LGIN + 220), //< ����� ������ �����Ѵ�.

	//< Server control message	
	NET_MSG_SVR_CMD				= (NET_MSG_LGIN + 300),
	NET_MSG_SVR_PAUSE			= (NET_MSG_LGIN + 310), //< ������ ��� �����.
	NET_MSG_SVR_RESUME			= (NET_MSG_LGIN + 311), //< ���߾��� ������ �ٽ� ������Ų��.	
	NET_MSG_SVR_RESTART			= (NET_MSG_LGIN + 312), //< ������ ������ ���߰� ���Ӱ� ������Ų��.
	NET_MSG_SVR_STOP			= (NET_MSG_LGIN + 313), //< ������ ������ �����.
	NET_MSG_SND_CHANNEL_FULL_STATE = (NET_MSG_LGIN + 314), //< ������ ä�� FULL ���¸� LoginServer�� ������.

	//<-----------------------------------------------------------------------------------------------------[LOBBY]	
	// NET_MSG_LOGIN				= (NET_MSG_LOBBY + 100), //< �α��� 
	NET_MSG_LOGIN_2 			= (NET_MSG_LOBBY + 107), //< �α���
	NET_MSG_LOGIN_FB			= (NET_MSG_LOBBY + 108), //< �α��� ���	
	NET_MSG_LOGIN_FB_SUB_OK_FIELD=(NET_MSG_LOBBY + 109), //< �α��μ�������� �ʵ弭���� �����ؼ� �����ϰ� �Ѵ�.		
	
	DAUM_NET_MSG_LOGIN			= (NET_MSG_LOBBY + 110), //< Daum �α���
	DAUM_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 111), //< Daum �α��� ��� Agent->Client	

	MET_MSG_SVR_FULL            = (NET_MSG_LOBBY + 112), //< ���� Full

	CHINA_NET_MSG_LOGIN         = (NET_MSG_LOBBY + 113), //< China �α���
	CHINA_NET_MSG_LOGIN_FB      = (NET_MSG_LOBBY + 114), //< China �α��� ��� Agent->Client

	GSP_NET_MSG_LOGIN           = (NET_MSG_LOBBY + 115), //< GSP �α���
	GSP_NET_MSG_LOGIN_FB        = (NET_MSG_LOBBY + 116), //< GSP �α��� ��� AGent->Client

	TERRA_NET_MSG_LOGIN         = (NET_MSG_LOBBY + 120), //< Terra �α���
	TERRA_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 121), //< Terra �α��� ��� Agent->Client

	TERRA_NET_MSG_PASSCHECK     = (NET_MSG_LOBBY + 122), //< Terra Password Check
	DAUM_NET_MSG_PASSCHECK      = (NET_MSG_LOBBY + 123), //< Daum Password Check
	NET_MSG_PASSCHECK_FB		= (NET_MSG_LOBBY + 124), //< Password Check ��� Agent->Client
	EXCITE_NET_MSG_PASSCHECK    = (NET_MSG_LOBBY + 125), //< Excite Password Check

	EXCITE_NET_MSG_LOGIN        = (NET_MSG_LOBBY + 130), //< Excite �α���
	EXCITE_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 131), //< Excite �α��� ��� Agent->Client

	JAPAN_NET_MSG_LOGIN         = (NET_MSG_LOBBY + 132), //< �Ϻ�(Gonzo) �α���
	JAPAN_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 133), //< �Ϻ�(Gonzo) �α��� ��� Agent->Client
	JAPAN_NET_MSG_UUID			= (NET_MSG_LOBBY + 134), //< �Ϻ�(Gonzo) �α��� UUID

	GS_NET_MSG_LOGIN			= (NET_MSG_LOBBY + 135), //< GS �α���
	GS_NET_MSG_LOGIN_FB			= (NET_MSG_LOBBY + 136), //< GS �α��� ��� Agent->Client

	THAI_NET_MSG_LOGIN          = (NET_MSG_LOBBY + 140), //< Thailand �α���
	THAI_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 141), //< Thailand �α��� ��� Agent->Client

	NET_MSG_REQ_RAND_KEY        = (NET_MSG_LOBBY + 150), //< ����Ű ��û Client->Agent
	
	NET_MSG_SND_ENCRYPT_KEY		= (NET_MSG_LOBBY + 160), //< �α��� ����Ű ���� Login->Session

	//< Character create message
	NET_MSG_CHA_INFO			= (NET_MSG_LOBBY + 200), //< ĳ���� ����
	NET_MSG_CHA_NEW				= (NET_MSG_LOBBY + 201), //< ���ο� ĳ���� ����
	NET_MSG_CHA_NEW_FB			= (NET_MSG_LOBBY + 202), //< ĳ���� ���� ����/���� �޽���
	
	NET_MSG_CHA_DEL				= (NET_MSG_LOBBY + 210), //< Client->Agent : �ɸ��� ����
	NET_MSG_CHA_DEL_FB			= (NET_MSG_LOBBY + 211), //< Agent->Client : �ɸ��� ���� ���
	NET_MSG_CHA_DEL_FB_OK		= (NET_MSG_LOBBY + 212), //< Agent->Client : �ɸ��� ���� ����
	NET_MSG_CHA_DEL_FB_ERROR	= (NET_MSG_LOBBY + 213), //< Agent->Client : �ɸ��� ���� ����
	DAUM_NET_MSG_CHA_DEL		= (NET_MSG_LOBBY + 214), //< Client->Agent : �ɸ��� ����
    NET_MSG_CHA_DEL_FB_CLUB	    = (NET_MSG_LOBBY + 215), //< Agent->Client : �ɸ��� ���� ����(Ŭ���������̱� ������ ������ ���� �ʴ´�)
	TERRA_NET_MSG_CHA_DEL		= (NET_MSG_LOBBY + 216), //< Client->Agent : �ɸ��� ����
	EXCITE_NET_MSG_CHA_DEL		= (NET_MSG_LOBBY + 217), //< Client->Agent : �ɸ��� ����
	GSP_NET_MSG_CHA_DEL		    = (NET_MSG_LOBBY + 218), //< Client->Agent : �ɸ��� ����
	JAPAN_NET_MSG_CHA_DEL		= (NET_MSG_LOBBY + 219), //< Client->Agent : �ɸ��� ����
	GS_NET_MSG_CHA_DEL			= (NET_MSG_LOBBY + 220), //< Client->Agent : �ɸ��� ����

	NET_MSG_CHA_DECREASE        = (NET_MSG_LOBBY + 240), //< Agent->Session : ĳ���� ����, ���� ����
	NET_MSG_TEST_CHA_DECREASE   = (NET_MSG_LOBBY + 241), //< Agent->Session : ĳ���� ����, ���� ����
	DAUM_NET_MSG_CHA_DECREASE   = (NET_MSG_LOBBY + 242), //< DAUM : Agent->Session : ĳ���� ����, ���� ����
	DAUM_NET_MSG_TEST_CHA_DECREASE=(NET_MSG_LOBBY+ 243), //< DAUM : Agent->Session : ĳ���� ����, ���� ����

	NET_MSG_CHA_INCREASE        = (NET_MSG_LOBBY + 244), //< Agent->Session : ĳ���� ����, ���� ����
	NET_MSG_TEST_CHA_INCREASE   = (NET_MSG_LOBBY + 245), //< Agent->Session : ĳ���� ����, ���� ����
	DAUM_NET_MSG_CHA_INCREASE   = (NET_MSG_LOBBY + 246), //< DAUM : Agent->Session : ĳ���� ����, ���� ����
	DAUM_NET_MSG_TEST_CHA_INCREASE= (NET_MSG_LOBBY+247),//< DAUM : Agent->Session : ĳ���� ����, ���� ����

	NET_MSG_REQ_CHA_ALL_INFO	= (NET_MSG_LOBBY + 300), //< ��� ĳ������ ������ ��û�Ѵ�.
	NET_MSG_REQ_CHA_INFO		= (NET_MSG_LOBBY + 301), //< Ư�� ĳ������ ������ ��û�Ѵ�.
	NET_MSG_REQ_CHA_BINFO		= (NET_MSG_LOBBY + 302), //< Ư�� ĳ������ �⺻= (ĳ���ͼ���ȭ��),���� ��û�Ѵ�.
	NET_MSG_REQ_CHA_FEEDBACK	= (NET_MSG_LOBBY + 303), //< ĳ���� ���� �ǵ�鵥����
	NET_MSG_CHA_INFO_LOGIN		= (NET_MSG_LOBBY + 304), //< �α��μ������� Ŭ���̾�Ʈ���� �Ѱ��ִ� ĳ���� ����
	NET_MSG_REQ_CHA_BAINFO		= (NET_MSG_LOBBY + 305), //< Ŭ���̾�Ʈ->���Ӽ��� : ĳ���� �������� ��û
	NET_MSG_CHA_BAINFO			= (NET_MSG_LOBBY + 306), //< ���Ӽ���->Ŭ���̾�Ʈ : ĳ���� ��������	
	
	NET_MSG_LOBBY_CHAR_SEL		= (NET_MSG_LOBBY + 390), //< �ʵ弭��->Ŭ���̾�Ʈ : ĳ���� ���ÿ� ����.
	NET_MSG_LOBBY_CHAR_JOIN		= (NET_MSG_LOBBY + 391), //< �ʵ弭��->Ŭ���̾�Ʈ : ���õ� ĳ���� ������ ����.
	NET_MSG_LOBBY_CHAR_ITEM		= (NET_MSG_LOBBY + 392), //< �ʵ弭��->Ŭ���̾�Ʈ : ���õ� ĳ���� �κ��丮 �������� ����.
	NET_MSG_LOBBY_CHAR_JOIN_FB	= (NET_MSG_LOBBY + 393), //< �ʵ弭��->Ŭ���̾�Ʈ : ���õ� ĳ������ �������� ���� �޽���.
	NET_MSG_LOBBY_CHAR_SKILL	= (NET_MSG_LOBBY + 394), //<	�ʵ弭��->Ŭ���̾�Ʈ : ���õ� ĳ���Ͱ� ��� ��ų ���� ����.
	NET_MSG_LOBBY_QUEST_END		= (NET_MSG_LOBBY + 395), //<	����Ʈ �Ϸ� ����.
	NET_MSG_LOBBY_QUEST_PROG	= (NET_MSG_LOBBY + 396), //<	����Ʈ ���� ����.
	NET_MSG_LOBBY_CHARGE_ITEM	= (NET_MSG_LOBBY + 397), //<	���� ������ ����.
	NET_MSG_LOBBY_CHAR_PUTON	= (NET_MSG_LOBBY + 398), //< �ʵ弭��->Ŭ���̾�Ʈ : ���õ� ĳ���� ���� �������� ����.

	NET_MSG_LOBBY_CLUB_INFO		= (NET_MSG_LOBBY + 399), //<	Ŭ�� ����.
	NET_MSG_LOBBY_CLUB_MEMBER	= (NET_MSG_LOBBY + 400), //<	Ŭ�� ���.
	NET_MSG_LOBBY_CLUB_ALLIANCE	= (NET_MSG_LOBBY + 401), //<	Ŭ�� ����.
	NET_MSG_LOBBY_CLUB_BATTLE	= (NET_MSG_LOBBY + 402), //<	Ŭ�� ��Ʋ.

	NET_MSG_CHARPOS_FROMDB2AGT	= (NET_MSG_LOBBY + 403), //<	ĳ���� ��ġ DB ���� ���
	
	NET_MSG_LOBBY_ITEM_COOLTIME = (NET_MSG_LOBBY + 404), //<	������ ��Ÿ��
	
	NET_MSG_LOBBY_GAME_JOIN			= (NET_MSG_LOBBY + 411), //< Ŭ���̾�Ʈ  ->������Ʈ���� : �κ񿡼� ������ ĳ���ͷ� ��������	
	NET_MSG_LOBBY_GAME_COMPLETE		= (NET_MSG_LOBBY + 412), //< Ŭ���̾�Ʈ  ->Ŭ���̾�Ʈ   : ���ӿ� �� ������ ��� �޾Ƽ� ���ӿ� ��.
	NET_MSG_GAME_JOIN_OK			= (NET_MSG_LOBBY + 413), //< �ʵ弭��    ->���Ǽ���     : �ش� ĳ���Ͱ� ���μ���
	MET_MSG_GAME_JOIN_FIELDSVR		= (NET_MSG_LOBBY + 414), //< ������Ʈ����->�ʵ弭��     : �ش�ĳ���Ͱ� ���ӿ� ���ο�û
	MET_MSG_GAME_JOIN_FIELDSVR_FB	= (NET_MSG_LOBBY + 415), //< �ʵ弭��->������Ʈ����:���õ� ĳ���ͷ� ������

	NET_MSG_CONNECT_CLIENT_FIELD	= (NET_MSG_LOBBY + 416), //<	������Ʈ����->Ŭ���̾�Ʈ   : �ش��ʵ弭���� ���� ����
	NET_MSG_JOIN_FIELD_IDENTITY		= (NET_MSG_LOBBY + 417), //<	Ŭ���̾�Ʈ  ->�ʵ弭��     : ���ο� Field ������ ������ �ڽ��� �ν��� ����.

	NET_MSG_LOBBY_REQ_GAME_JOIN		= (NET_MSG_LOBBY + 421), //< Ŭ���̾�Ʈ  ->Ŭ���̾�Ʈ   : �������̽��� ���õ� �ɸ����� ������ �����ϱ� ���ؼ�.
	
	NET_MSG_AGENT_REQ_JOIN			= (NET_MSG_LOBBY + 422), //< ������ƮDB  ->������Ʈ   : db���� ���� char�� ���ӽõ�.
	NET_MSG_FIELD_REQ_JOIN			= (NET_MSG_LOBBY + 423), //< �ʵ�DB		->�ʵ弭��   : db���� ���� char�� ���ӽõ�.

	NET_MSG_EVENT_LOTTERY           = (NET_MSG_LOBBY + 430), //< Ŭ���̾�Ʈ  ->������Ʈ   : �����Է�
	NET_MSG_EVENT_LOTTERY_FB        = (NET_MSG_LOBBY + 431), //< ������Ʈ    ->Ŭ���̾�Ʈ : ���ǰ��

	NET_MSG_LOBBY_CHINA_ERROR		= (NET_MSG_LOBBY + 432), //< �߱� �α��ν� �÷��� �����ð��� 5�ð��� �Ѱ� �������� �����ð��� 5�ð��� ���� �ʾ��� ��� ���� �޽���

	NET_MSG_LOBBY_CHAR_VIETNAM_ITEM = (NET_MSG_LOBBY + 433), //< ��Ʈ�� �κ��丮 ������ ���� �޽���

	NET_MSG_LOBBY_CHAR_PUTON_EX		= (NET_MSG_LOBBY + 434), //< �ʵ弭��->Ŭ���̾�Ʈ : ���õ� ĳ���� ���� �������� ����. ( �Ѱ��� ���� )

	//<-----------------------------------------------------------------------------------------------------[GAME CONTRL]
	
	NET_MSG_COMBINE					= (NET_MSG_GCTRL +  10), //< ���� �޽���. ( �ʵ� ���� -> Ŭ���̾�Ʈ )

	///////////////////////////////////////////////////////////////////////////
	// nProtect
	NET_MSG_GAMEGUARD_AUTH          = (NET_MSG_GCTRL +  20), //< nProtect GameGuard Auth ��û �޽���
	NET_MSG_GAMEGUARD_ANSWER        = (NET_MSG_GCTRL +  21), //< nProtect GameGuard Auth Answer �޽���
	NET_MSG_GAMEGUARD_AUTH_1        = (NET_MSG_GCTRL +  22), //< nProtect GameGuard Auth ��û �޽���
	NET_MSG_GAMEGUARD_ANSWER_1      = (NET_MSG_GCTRL +  23), //< nProtect GameGuard Auth Answer �޽���
	NET_MSG_GAMEGUARD_AUTH_2        = (NET_MSG_GCTRL +  24), //< nProtect GameGuard Auth ��û �޽���
	NET_MSG_GAMEGUARD_ANSWER_2      = (NET_MSG_GCTRL +  25), //< nProtect GameGuard Auth Answer �޽���

	// Apex ����
	NET_MSG_APEX_DATA				= (NET_MSG_GCTRL +	30), //< Apex Data �޼���
	NET_MSG_APEX_ANSWER				= (NET_MSG_GCTRL +	31), //< Apex Answer �޼���
//	NET_MSG_APEX_RSAKEY				= (NET_MSG_GCTRL +	32), //< Apex RSA Key �޼���
	// Apex ����( ȫ�� )
	NET_MSG_APEX_RETURN 			= (NET_MSG_GCTRL +	33), //< Apex Client Return 

	NET_MSG_REQ_FRIENDLIST			= (NET_MSG_GCTRL +  50), //< ģ�� ��� ���� ��û.
	NET_MSG_REQ_FRIENDADD			= (NET_MSG_GCTRL +  51), //< ģ�� �߰� ��û.
	NET_MSG_REQ_FRIENDADD_LURE		= (NET_MSG_GCTRL +  52), //< ģ�� �߰� ����.
	NET_MSG_REQ_FRIENDADD_ANS		= (NET_MSG_GCTRL +  53), //< ģ�� �߰� ���� ���� ȸ��.
	NET_MSG_REQ_FRIENDADD_FB		= (NET_MSG_GCTRL +  54), //< ģ�� �߰� ��û ���.
	NET_MSG_REQ_FRIENDDEL			= (NET_MSG_GCTRL +  55), //< ģ�� ���� ��û.
	NET_MSG_REQ_FRIENDDEL_FB		= (NET_MSG_GCTRL +  56), //< ģ�� ���� ���.

	NET_MSG_FRIENDINFO				= (NET_MSG_GCTRL +  57), //< ģ�� ����.
	NET_MSG_FRIENDSTATE				= (NET_MSG_GCTRL +  58), //< ģ�� ����.

	NET_MSG_REQ_FRIENDBLOCK			= (NET_MSG_GCTRL +  59), //< �ӼӸ� ���� ��û.
	NET_MSG_REQ_FRIENDBLOCK_FB		= (NET_MSG_GCTRL +  60), //< �ӼӸ� ���� ��û ���.

	NET_MSG_CHAT					= (NET_MSG_GCTRL + 100), //< ä�ø޽��� ���� ( Ŭ���̾�Ʈ->���Ӽ��� )
	NET_MSG_CHAT_FB					= (NET_MSG_GCTRL + 101), //< ä�ø޽��� ���� ( ���Ӽ���->Ŭ���̾�Ʈ )
	NET_MSG_CHAT_PRIVATE_FAIL		= (NET_MSG_GCTRL + 102), //< ä��, �ӼӸ� �����⿡ �����Ͽ��� ���. ( ���Ӽ���->Ŭ���̾�Ʈ )	

	NET_MSG_CHAT_LOUDSPEAKER		= (NET_MSG_GCTRL + 103), //< Ȯ���� �޽ý�.
	NET_MSG_CHAT_LOUDSPEAKER_AGT	= (NET_MSG_GCTRL + 104), //< Ȯ���� �޽��� agt �� ����.
	NET_MSG_CHAT_LOUDSPEAKER_FB		= (NET_MSG_GCTRL + 105), //< Ȯ���� �޽��� ó�� FB.

	NET_MSG_CHAT_BLOCK				= (NET_MSG_GCTRL + 106), //< ä�� ���� ������ ���. ( ���Ӽ��� -> Ŭ���̾�Ʈ )

	NET_MSG_FIELDSVR_CHARCHK		= (NET_MSG_GCTRL + 110), //< �ʵ弭���� �ɸ��� �������� ���� CHAR ���� ���� �޽��� ����.
	NET_MSG_FIELDSVR_CHARCHK_FB		= (NET_MSG_GCTRL + 111), //< �ʵ弭���� �ɸ��� �������� ���� CHAR ���� ���� �޽��� ����.

	NET_MSG_GCTRL_FIELDSVR_OUT		= (NET_MSG_GCTRL + 115), //< �ʵ弭������ ������ ���� ���� ó���� ��Ź. ( �ʵ弭�� �̵���. )
	NET_MSG_GCTRL_FIELDSVR_OUT_FB	= (NET_MSG_GCTRL + 116), //< �ʵ弭������ ������ ���� ���� ó�� ���. ( �ʵ弭�� �̵���. )

	NET_MSG_GCTRL_REBIRTH_OUT		= (NET_MSG_GCTRL + 117), //< �ٸ� �ʵ弭������ ��Ȱ�ϱ� ���� ���� ó�� ��Ź.
	NET_MSG_GCTRL_REBIRTH_OUT_FB	= (NET_MSG_GCTRL + 118), //< �ٸ� �ʵ弭������ ��Ȱ�ϱ� ���� ���� ó�� ���.

	NET_MSG_GCTRL_DROP_ITEM			= (NET_MSG_GCTRL + 120),
	NET_MSG_GCTRL_DROP_MONEY		= (NET_MSG_GCTRL + 121),

	NET_MSG_GCTRL_DROP_PC			= (NET_MSG_GCTRL + 123),
	NET_MSG_GCTRL_DROP_CROW			= (NET_MSG_GCTRL + 124),
	NET_MSG_GCTRL_DROP_MATERIAL		= (NET_MSG_GCTRL + 125),

	NET_MSG_GCTRL_DROP_OUT			= (NET_MSG_GCTRL + 127),
	NET_MSG_GCTRL_DROP_OUT_FORCED	= (NET_MSG_GCTRL + 128),

	NET_MSG_GCTRL_REQ_GATEOUT_REQ	= (NET_MSG_GCTRL + 130),
	NET_MSG_GCTRL_REQ_GATEOUT		= (NET_MSG_GCTRL + 131),
	NET_MSG_GCTRL_REQ_GATEOUT_FB	= (NET_MSG_GCTRL + 132),
	NET_MSG_GCTRL_REQ_LANDIN		= (NET_MSG_GCTRL + 133),

	NET_MSG_GCTRL_REQ_READY			= (NET_MSG_GCTRL + 134),

	NET_MSG_GCTRL_LAND_INFO			= (NET_MSG_GCTRL + 135),
	NET_MSG_GCTRL_SERVER_INFO		= (NET_MSG_GCTRL + 136),
	NET_MSG_GCTRL_SCHOOLFREEPK_FLD	= (NET_MSG_GCTRL + 137),

	NET_MSG_GCTRL_ACTSTATE			= (NET_MSG_GCTRL + 139),
	NET_MSG_GCTRL_MOVESTATE			= (NET_MSG_GCTRL + 140),
	NET_MSG_GCTRL_MOVESTATE_BRD		= (NET_MSG_GCTRL + 141),

	NET_MSG_GCTRL_GOTO				= (NET_MSG_GCTRL + 142),
	NET_MSG_GCTRL_GOTO_BRD			= (NET_MSG_GCTRL + 143),

	NET_MSG_GCTRL_ATTACK			= (NET_MSG_GCTRL + 144),
	NET_MSG_GCTRL_ATTACK_BRD		= (NET_MSG_GCTRL + 145),
	NET_MSG_GCTRL_ATTACK_CANCEL		= (NET_MSG_GCTRL + 146),
	NET_MSG_GCTRL_ATTACK_CANCEL_BRD	= (NET_MSG_GCTRL + 147),
	NET_MSG_GCTRL_ATTACK_AVOID		= (NET_MSG_GCTRL + 149),
	NET_MSG_GCTRL_ATTACK_AVOID_BRD	= (NET_MSG_GCTRL + 150),
	NET_MSG_GCTRL_ATTACK_DAMAGE		= (NET_MSG_GCTRL + 151),
	NET_MSG_GCTRL_ATTACK_DAMAGE_BRD	= (NET_MSG_GCTRL + 152),

	NET_MSG_GCTRL_ACTION_BRD		= (NET_MSG_GCTRL + 153),	//<	�ൿ ��ȭ.
	NET_MSG_GCTRL_UPDATE_STATE		= (NET_MSG_GCTRL + 154),	//<	���� ��ȭ ������Ʈ.
	NET_MSG_GCTRL_UPDATE_EXP		= (NET_MSG_GCTRL + 155),	//<	����ġ ��ȭ�� �߻�.
	NET_MSG_GCTRL_UPDATE_MONEY		= (NET_MSG_GCTRL + 156),	//<	����ġ ��ȭ�� �߻�.

	NET_MSG_GCTRL_UPDATE_SP			= (NET_MSG_GCTRL + 157),	//<	SP ��ȭ�� �߻�.
	NET_MSG_GCTRL_UPDATE_LP			= (NET_MSG_GCTRL + 158),	//<	Living Point ��ȭ�� �߻�.
	NET_MSG_GCTRL_UPDATE_SKP		= (NET_MSG_GCTRL + 160),	//<	��ų ����Ʈ ��ȭ�� �߻�.

	NET_MSG_GCTRL_UPDATE_STATE_BRD	= (NET_MSG_GCTRL + 161),	//<	���� ��ȭ ������Ʈ.
	NET_MSG_GCTRL_UPDATE_PASSIVE_BRD= (NET_MSG_GCTRL + 162),	//<	passive skill ���� ��ȭ ������Ʈ.

	NET_MSG_GCTRL_POSITIONCHK_BRD	= (NET_MSG_GCTRL + 163),	//<	���������� ��ġ�� Ŭ���̾�Ʈ���� üũ�ϱ� ���ؼ�.

	NET_MSG_GCTRL_UPDATE_BRIGHT		= (NET_MSG_GCTRL + 164),	//<	bright(�Ӽ�)ġ ������Ʈ.
	NET_MSG_GCTRL_UPDATE_BRIGHT_BRD	= (NET_MSG_GCTRL + 165),	//<	bright(�Ӽ�)ġ ������Ʈ.
	NET_MSG_GCTRL_UPDATE_STATS		= (NET_MSG_GCTRL + 166),	//<	stats ������Ʈ.

	NET_MSG_GCTRL_UPDATE_FLAGS		= (NET_MSG_GCTRL + 167),	//<	flags ������Ʈ.

	NET_MSG_GCTRL_PUSHPULL_BRD		= (NET_MSG_GCTRL + 171),	//<	�аų� ���� �׼� �߻���.
	NET_MSG_GCTRL_JUMP_POS_BRD		= (NET_MSG_GCTRL + 172),	//<	�ɸ����� ��ġ�� ���������� ����Ǿ��� ���. Ŭ���̾�Ʈ�� �ݿ�.

	NET_MSG_GCTRL_INVEN_INSERT		= (NET_MSG_GCTRL + 200),	//<	�ι꿡 ������ ���Ե�.
	NET_MSG_GCTRL_INVEN_DELETE		= (NET_MSG_GCTRL + 201),	//<	�ι�   ������ ����.
	NET_MSG_GCTRL_INVEN_DEL_INSERT	= (NET_MSG_GCTRL + 202),	//<	�ι꿡 �ִ� ������ ������ ������ ����.

	NET_MSG_GCTRL_PUTON_RELEASE		= (NET_MSG_GCTRL + 203),	//<	������ �������� ������.
	NET_MSG_GCTRL_PUTON_RELEASE_BRD	= (NET_MSG_GCTRL + 204),	//<	������ �������� ������.
	NET_MSG_GCTRL_PUTON_UPDATE		= (NET_MSG_GCTRL + 205),	//<	���Կ� ������ ����.
	NET_MSG_GCTRL_PUTON_UPDATE_BRD	= (NET_MSG_GCTRL + 206),	//<	[���] ��� �������� �����.

	NET_MSG_GCTRL_INVEN_DRUG_UPDATE	= (NET_MSG_GCTRL + 207),	//<	�Ҹ��� ������ �Ҹ���� ������Ʈ.
	NET_MSG_GCTRL_PUTON_DRUG_UPDATE	= (NET_MSG_GCTRL + 208),	//<	�Ҹ��� ������ �Ҹ���� ������Ʈ.

	NET_MSG_GCTRL_INVEN_ITEM_UPDATE	= (NET_MSG_GCTRL + 209),	//<	�ι� ������ ��ġ ������Ʈ.

	NET_MSG_GCTRL_PICKUP_MONEY		= (NET_MSG_GCTRL + 221),	//<	�� ����.
	NET_MSG_GCTRL_PICKUP_ITEM		= (NET_MSG_GCTRL + 222),	//<	������ ���� �˸�.

	NET_MSG_GCTRL_2_FRIEND_REQ		= (NET_MSG_GCTRL + 231),	//<	ģ���� ���� ��û.
	NET_MSG_GCTRL_2_FRIEND_CK		= (NET_MSG_GCTRL + 232),	//<	ģ���� ���� ģ���� �ִ��� �˻��� ��ġ ��ȯ.
	NET_MSG_GCTRL_2_FRIEND_AG		= (NET_MSG_GCTRL + 233),	//<	ģ���� ���� AG�� ȸ�� ( ģ�� ��ġ ���� ).
	NET_MSG_GCTRL_2_FRIEND_FLD		= (NET_MSG_GCTRL + 234),	//<	ģ���� ���� FLD�� �̵� ����.
	NET_MSG_GCTRL_2_FRIEND_FB		= (NET_MSG_GCTRL + 235),	//<	ģ���� ���� ó�� ��� �뺸.

	NET_MSG_GCTRL_REQ_TAKE_FB		= (NET_MSG_GCTRL + 300),	//<	�ݱ� �õ� ���.
	NET_MSG_GCTRL_REQ_FIELD_TO_INVEN= (NET_MSG_GCTRL + 301),	//<	������,�� �ݱ� �õ�.
	NET_MSG_GCTRL_REQ_FIELD_TO_HOLD	= (NET_MSG_GCTRL + 302),	//<	Field�� �ִ� �������� ��� �õ�.

	NET_MSG_GCTRL_REQ_INVEN_TO_HOLD	= (NET_MSG_GCTRL + 303),	//<	�ι꿡 �ִ� �������� ��� �õ�.
	NET_MSG_GCTRL_REQ_INVEN_EX_HOLD	= (NET_MSG_GCTRL + 304),	//<	�ι꿡 �ִ� �������� ��� �õ�.

	NET_MSG_GCTRL_REQ_SLOT_TO_HOLD	= (NET_MSG_GCTRL + 305),	//<	������, ���Կ��� ������.
	NET_MSG_GCTRL_REQ_SLOT_EX_HOLD	= (NET_MSG_GCTRL + 306),	//<	������, ���Կ� �ִ°Ͱ� �տ� ��� �ִ°� ��ȯ.

	NET_MSG_GCTRL_REQ_HOLD_TO_FIELD	= (NET_MSG_GCTRL + 307),	//<	�������� �ٴڿ� ����Ʈ��.
	NET_MSG_GCTRL_REQ_HOLD_TO_INVEN	= (NET_MSG_GCTRL + 308),	//<	�������� INVEN�� ����.
	NET_MSG_GCTRL_REQ_HOLD_TO_SLOT	= (NET_MSG_GCTRL + 309),	//<	������, �տ��� ��������.

	NET_MSG_GCTRL_REQ_HOLD_FB		= (NET_MSG_GCTRL + 310),	//<	�ι꿡 �ִ� �������� ��� �õ��� ��� �ǵ��.
	NET_MSG_GCTRL_REQ_INVEN_TO_SLOT	= (NET_MSG_GCTRL + 311),	//<	�ι꿡 �ִ� �������� ���Կ� �ֱ�.

	NET_MSG_GCTRL_REQ_MONEY_TO_FIELD= (NET_MSG_GCTRL + 312),	//<	���� �ʵ忡 ������.
	NET_MSG_GCTRL_REQ_INVEN_SPLIT	= (NET_MSG_GCTRL + 313),	//<	�κ��� �ִ� ��ħ ������ �и�.

	NET_MSG_GCTRL_PUTON_CHANGE		= (NET_MSG_GCTRL + 314),	//<	���� ����.
	NET_MSG_GCTRL_PUTON_CHANGE_BRD	= (NET_MSG_GCTRL + 315),	//<	[���] ���� ���� �˸�.
	NET_MSG_GCTRL_PUTON_CHANGE_AG	= (NET_MSG_GCTRL + 316),	//<	Agent�� ���� ���� �˸�.
	NET_MSG_GCTRL_REQ_SLOT_CHANGE	= (NET_MSG_GCTRL + 317),	//<	���� ���� ü����.

	NET_MSG_GCTRL_REQ_BUY_FROM_NPC	= (NET_MSG_GCTRL + 350),	//<	NPC���� ��ǰ �Ǹ� �õ�.
	NET_MSG_GCTRL_REQ_SALE_TO_NPC	= (NET_MSG_GCTRL + 351),	//<	NPC���� ��ǰ �Ǹ� �õ�.

	NET_MSG_GCTRL_REQ_REBIRTH		= (NET_MSG_GCTRL + 380),	//<	��Ȱ ��û.
	NET_MSG_GCTRL_REQ_REBIRTH_FB	= (NET_MSG_GCTRL + 381),	//<	��Ȱ ��û ����.

	NET_MSG_GCTRL_REQ_LEVELUP		= (NET_MSG_GCTRL + 382),	//<	���� ��û.
	NET_MSG_GCTRL_REQ_LEVELUP_FB	= (NET_MSG_GCTRL + 383),	//<	���� ��û ����.
	NET_MSG_GCTRL_REQ_LEVELUP_BRD	= (NET_MSG_GCTRL + 384),	//<	���� ȿ��.

	NET_MSG_GCTRL_REQ_STATSUP		= (NET_MSG_GCTRL + 385),	//<	STATS UP
	NET_MSG_GCTRL_REQ_STATSUP_FB	= (NET_MSG_GCTRL + 386),	//<	STATS UP ����.

	NET_MSG_GCTRL_REQ_INVENDRUG		= (NET_MSG_GCTRL + 400),	//<	�ι� ��ǰ �Ա�.
	NET_MSG_GCTRL_REQ_LEARNSKILL	= (NET_MSG_GCTRL + 401),	//<	�ι� ��ų ���� ��û.
	NET_MSG_GCTRL_REQ_LEARNSKILL_FB	= (NET_MSG_GCTRL + 402),	//<	�ι� ��ų ���� ����.
	NET_MSG_GCTRL_REQ_SKILLUP		= (NET_MSG_GCTRL + 403),	//<	��ų ���׷��̵� ��û.
	NET_MSG_GCTRL_REQ_SKILLUP_FB	= (NET_MSG_GCTRL + 404),	//<	��ų ���׷��̵� ����.

	NET_MSG_GCTRL_REQ_RECALL		= (NET_MSG_GCTRL + 405),	//<	��ȯ ������ ��� ��û.
	NET_MSG_GCTRL_REQ_RECALL_FB		= (NET_MSG_GCTRL + 406),	//<	��ȯ ������ ��� ����.
	NET_MSG_GCTRL_REQ_RECALL_AG		= (NET_MSG_GCTRL + 407),	//<	��ȯ�� ������Ʈ ������ ��û.

	NET_MSG_GCTRL_REQ_BUS			= (NET_MSG_GCTRL + 408),	//<	���� ���� ��û.
	NET_MSG_GCTRL_REQ_BUS_FB		= (NET_MSG_GCTRL + 409),	//<	���� ���� ��û ����.

	NET_MSG_GCTRL_REQ_SKILL			= (NET_MSG_GCTRL + 411),	//<	��ų �ߵ� ��û.
	NET_MSG_GCTRL_REQ_SKILL_FB		= (NET_MSG_GCTRL + 412),	//<	��ų �ߵ� ��û ����.
	NET_MSG_GCTRL_REQ_SKILL_BRD		= (NET_MSG_GCTRL + 413),	//<	��ų �ߵ�.
	NET_MSG_GCTRL_SKILLFACT_BRD		= (NET_MSG_GCTRL + 414),	//<	��ų ����� ü�� ��ȭ.
	NET_MSG_GCTRL_SKILLHOLD_BRD		= (NET_MSG_GCTRL + 415),	//<	��ų ����� ���� ȿ��.
	NET_MSG_GCTRL_SKILLCONSUME_FB	= (NET_MSG_GCTRL + 416),	//<	��ų �ߵ��� �Ҹ�Ǵ� ��ġ��.

	NET_MSG_GCTRL_SKILL_CANCEL		= (NET_MSG_GCTRL + 417),	//<	��ų ���.
	NET_MSG_GCTRL_SKILL_CANCEL_BRD	= (NET_MSG_GCTRL + 418),	//<	��ų ��� Ŭ���̾�Ʈ�� �ݿ�.

	NET_MSG_GCTRL_SKILLHOLD_RS_BRD	= (NET_MSG_GCTRL + 419),	//<	��ų ���� ȿ�� ����.
	NET_MSG_GCTRL_SKILLHOLDEX_BRD	= (NET_MSG_GCTRL + 420),	//<	��ų ����� ���� ȿ��.

	NET_MSG_GCTRL_STATEBLOW_BRD		= (NET_MSG_GCTRL + 426),	//<	�����̻�.
	NET_MSG_GCTRL_CURESTATEBLOW_BRD	= (NET_MSG_GCTRL + 427),	//<	�����̻�.
	NET_MSG_GCTRL_LANDEFFECT		= (NET_MSG_GCTRL + 428),	//< ������ ���õ� ���� �̻�

	NET_MSG_GCTRL_REQ_SKILLQ_ACTIVE	= (NET_MSG_GCTRL + 430),	//<	��ų �������� ��Ƽ�� ��ų.
	NET_MSG_GCTRL_REQ_SKILLQ_SET	= (NET_MSG_GCTRL + 431),	//<	��ų ������ ���� ��û.
	NET_MSG_GCTRL_REQ_SKILLQ_RESET	= (NET_MSG_GCTRL + 432),	//<	��ų ������ ���� ��û.
	NET_MSG_GCTRL_REQ_SKILLQ_FB		= (NET_MSG_GCTRL + 433),	//<	��ų ������ ��û�� ���� ������� Ŭ���̾�Ʈ�� ����.

	NET_MSG_GCTRL_REQ_ACTIONQ_SET	= (NET_MSG_GCTRL + 435),	//<	���ǽ��� ���� ��û.
	NET_MSG_GCTRL_REQ_ACTIONQ_RESET	= (NET_MSG_GCTRL + 436),	//<	���ǽ��� ���� ��û.
	NET_MSG_GCTRL_REQ_ACTIONQ_FB	= (NET_MSG_GCTRL + 437),	//<	���ǽ��� ��û�� ���� ������� Ŭ���̾�Ʈ�� ����.

	NET_MSG_GCTRL_REQ_GETSTORAGE		= (NET_MSG_GCTRL + 441),	//<	â�� ���� ��������.
	NET_MSG_GCTRL_REQ_GETSTORAGE_FB		= (NET_MSG_GCTRL + 442),	//<	â�� ���� ��������.
	NET_MSG_GCTRL_REQ_GETSTORAGE_ITEM	= (NET_MSG_GCTRL + 443),	//<	â�� ���� ��������.

	NET_MSG_GCTRL_REQ_STORAGEDRUG		= (NET_MSG_GCTRL + 444),	//<	â�� ��ǰ �Ա�.
	NET_MSG_GCTRL_REQ_STORAGESKILL		= (NET_MSG_GCTRL + 445),	//<	â�� ��ų �Ա�.

	NET_MSG_GCTRL_REQ_STORAGE_TO_HOLD	= (NET_MSG_GCTRL + 446),	//<	â���� �ִ� �������� ��� �õ�.
	NET_MSG_GCTRL_REQ_STORAGE_EX_HOLD	= (NET_MSG_GCTRL + 447),	//<	â���� �ִ� �������� ��� �õ�.
	NET_MSG_GCTRL_REQ_HOLD_TO_STORAGE	= (NET_MSG_GCTRL + 448),	//<	�������� â���� ����.

	NET_MSG_GCTRL_STORAGE_INSERT		= (NET_MSG_GCTRL + 449),	//<	â���� ������ ���Ե�.
	NET_MSG_GCTRL_STORAGE_DELETE		= (NET_MSG_GCTRL + 450),	//<	â�� ������ ����.
	NET_MSG_GCTRL_STORAGE_DEL_INSERT	= (NET_MSG_GCTRL + 451),	//<	â�� ������ ���� �� ����.
	NET_MSG_GCTRL_STORAGE_DRUG_UPDATE	= (NET_MSG_GCTRL + 452),	//<	â�� �Ҹ�ǰ ���Ƚ�� �ľ�.

	NET_MSG_GCTRL_STORAGE_SAVE_MONEY	= (NET_MSG_GCTRL + 453),	//<	â���� ���� ����.
	NET_MSG_GCTRL_STORAGE_DRAW_MONEY	= (NET_MSG_GCTRL + 454),	//<	â������ ���� ����.
	NET_MSG_GCTRL_STORAGE_UPDATE_MONEY	= (NET_MSG_GCTRL + 455),	//<	â���� �� �׼� ��ȭ.

	NET_MSG_GCTRL_REQ_STORAGE_SPLIT		= (NET_MSG_GCTRL + 456),	//<	â���� �ִ� ��ħ ������ �и�.
	NET_MSG_GCTRL_STORAGE_STATE			= (NET_MSG_GCTRL + 457),	//<	â�� ��밡�� �Ӽ�.
	NET_MSG_GCTRL_PREMIUM_STATE			= (NET_MSG_GCTRL + 458),	//<	�����̾� �Ӽ�.

	NET_MSG_GCTRL_PARTY_LURE			= (NET_MSG_GCTRL + 460),	//<	��Ƽ ���� ��û.			Ŭ���̾�Ʈ -> ����.
	NET_MSG_GCTRL_PARTY_LURE_FB			= (NET_MSG_GCTRL + 461),	//<	��Ƽ ���� ��û�� ���.	Ŭ���̾�Ʈ -> ����.
	NET_MSG_GCTRL_PARTY_LURE_TAR		= (NET_MSG_GCTRL + 462),	//<	��Ƽ ���� ��û.			���� -> Ŭ���̾�Ʈ.
	NET_MSG_GCTRL_PARTY_LURE_TAR_ANS	= (NET_MSG_GCTRL + 463),	//<	��Ƽ ���� ��û ����.	Ŭ���̾�Ʈ -> ����.

	NET_MSG_GCTRL_PARTY_SECEDE			= (NET_MSG_GCTRL + 464),	//<	��Ƽ Ż�� ��û.

	NET_MSG_GCTRL_PARTY_ADD				= (NET_MSG_GCTRL + 465),	//<	��Ƽ�� �߰�.
	NET_MSG_GCTRL_PARTY_ADD_FB			= (NET_MSG_GCTRL + 466),	//<	��Ƽ�� �߰� FB.

	NET_MSG_GCTRL_PARTY_DEL				= (NET_MSG_GCTRL + 467),	//<	��Ƽ�� ����.
	NET_MSG_GCTRL_PARTY_DEL_FB			= (NET_MSG_GCTRL + 468),	//<	��Ƽ�� ���� FB

	NET_MSG_GCTRL_PARTY_AUTHORITY		= (NET_MSG_GCTRL + 469),	//< ��Ƽ�� ����

	NET_MSG_GCTRL_PARTY_DISSOLVE		= (NET_MSG_GCTRL + 470),	//<	��Ƽ ��ü.

	NET_MSG_GCTRL_PARTY_FNEW			= (NET_MSG_GCTRL + 471),	//<	��Ƽ ����. (field).
	NET_MSG_GCTRL_PARTY_MBR_DETAIL		= (NET_MSG_GCTRL + 472),	//<	��Ƽ���� ���� ����.
	NET_MSG_GCTRL_PARTY_MBR_MOVEMAP		= (NET_MSG_GCTRL + 473),	//<	��Ƽ���� map �̵�.
	NET_MSG_GCTRL_PARTY_MBR_POINT		= (NET_MSG_GCTRL + 474),	//<	��Ƽ���� POINT ��ȭ.
	NET_MSG_GCTRL_PARTY_MBR_POS			= (NET_MSG_GCTRL + 475),	//<	��Ƽ���� ��ġ ��ȭ.
	NET_MSG_GCTRL_PARTY_MBR_PICKUP_BRD	= (NET_MSG_GCTRL + 476),	//<	��Ƽ���� ������ ������ �˸�.

	NET_MSG_GCTRL_PARTY_BRD				= (NET_MSG_GCTRL + 477),	//<	�ɸ����� ��Ƽ �Ҽ� ��ȭ�� �ֺ� Ŭ���̾�Ʈ�鿡 �˸�.
	NET_MSG_GCTRL_PARTY_MASTER_RENEW	= (NET_MSG_GCTRL + 478),	//< ��Ƽ������ �缳��.

	NET_MSG_GCTRL_TRADE					= (NET_MSG_GCTRL + 480),	//<	Ʈ���̵� ��û. ( Ŭ���̾�Ʈ1 -> ���� )
	NET_MSG_GCTRL_TRADE_FB				= (NET_MSG_GCTRL + 481),	//<	Ʈ���̵� ��û ����. ( ���� -> Ŭ���̾�Ʈ1 )
	NET_MSG_GCTRL_TRADE_TAR				= (NET_MSG_GCTRL + 482),	//<	Ʈ���̵� ��뿡�� ��û. ( ���� -> Ŭ���̾�Ʈ2 )
	NET_MSG_GCTRL_TRADE_TAR_ANS			= (NET_MSG_GCTRL + 483),	//<	Ʈ���̵� ����� ����. ( Ŭ���̾�Ʈ2 -> ���� )

	NET_MSG_GCTRL_TRADE_AGREE			= (NET_MSG_GCTRL + 484),	//<	Ʈ���̵� ���� ����.
	
	NET_MSG_GCTRL_TRADE_AGREE_TAR		= (NET_MSG_GCTRL + 486),	//<	Ʈ���̵� ���� ���� ���濡�� �˸�. ( ���濡��. )

	NET_MSG_GCTRL_TRADE_ITEM_REGIST		= (NET_MSG_GCTRL + 487),	//<	Ʈ���̵��� ������ ���.
	NET_MSG_GCTRL_TRADE_ITEM_REGIST_TAR	= (NET_MSG_GCTRL + 488),	//<	Ʈ���̵��� ������ ���. ( ���濡��. )
	NET_MSG_GCTRL_TRADE_ITEM_REMOVE		= (NET_MSG_GCTRL + 489),	//<	Ʈ���̵��� ������ ��Ͽ��� ����.
	NET_MSG_GCTRL_TRADE_ITEM_REMOVE_TAR	= (NET_MSG_GCTRL + 490),	//<	Ʈ���̵��� ������ ��Ͽ��� ����. ( ���濡��. )

	NET_MSG_GCTRL_TRADE_MONEY			= (NET_MSG_GCTRL + 491),	//<	Ʈ���̵��� �ݾ�.
	NET_MSG_GCTRL_TRADE_MONEY_TAR		= (NET_MSG_GCTRL + 492),	//<	Ʈ���̵��� �ݾ��� ���濡�� �˸�.

	NET_MSG_GCTRL_TRADE_COMPLETE_TAR	= (NET_MSG_GCTRL + 493),	//<	Ʈ���̵� �Ϸ�.

	NET_MSG_GCTRL_TRADE_CANCEL			= (NET_MSG_GCTRL + 494),	//<	Ʈ���̵� ���.
	NET_MSG_GCTRL_TRADE_CANCEL_TAR		= (NET_MSG_GCTRL + 495),	//<	Ʈ���̵� ���. ( ���濡��. )

	NET_MSG_GCTRL_INVEN_BOXOPEN			= (NET_MSG_GCTRL + 496),	//<	box open ��û.
	NET_MSG_GCTRL_INVEN_BOXOPEN_FB		= (NET_MSG_GCTRL + 497),	//<	box open ��û FB.

	NET_MSG_GCTRL_INVEN_DISGUISE		= (NET_MSG_GCTRL + 498),	//<	���忡 �ڽ����� ������.
	NET_MSG_GCTRL_INVEN_DISGUISE_FB		= (NET_MSG_GCTRL + 499),	//<	���忡 �ڽ����� ������ FB.

	NET_MSG_GCTRL_INVEN_GRINDING		= (NET_MSG_GCTRL + 501),	//<	INVEN ������ ���� �õ�.
	NET_MSG_GCTRL_INVEN_GRINDING_FB		= (NET_MSG_GCTRL + 502),	//<	INVEN ������ ���� �õ�.

	NET_MSG_GCTRL_REGEN_GATE			= (NET_MSG_GCTRL + 503),	//<	��Ȱ ��ġ ����.
	NET_MSG_GCTRL_REGEN_GATE_FB			= (NET_MSG_GCTRL + 504),	//<	��Ȱ ��ġ ���� FB.

	NET_MSG_GCTRL_CURE					= (NET_MSG_GCTRL + 505),	//<	ġ�� ��û.
	NET_MSG_GCTRL_CURE_FB				= (NET_MSG_GCTRL + 506),	//<	ġ�� ��û FB.

	NET_MSG_GCTRL_CHARRESET				= (NET_MSG_GCTRL + 507),	//<	�ɸ��� ���� ���� ��û. ( stats, skill )
	NET_MSG_GCTRL_CHARRESET_FB			= (NET_MSG_GCTRL + 508),	//<	�ɸ��� ���� ���� ��û FB. ( stats, skill )
	
	NET_MSG_GCTRL_NPC_ITEM_TRADE		= (NET_MSG_GCTRL + 509),	//<	������ ��ȯ ��û.
	NET_MSG_GCTRL_NPC_ITEM_TRADE_FB		= (NET_MSG_GCTRL + 510),	//<	������ ��ȯ ��û FB.

	NET_MSG_GCTRL_CONFRONT				= (NET_MSG_GCTRL + 511),	//<	��� ��û to (����)
	NET_MSG_GCTRL_CONFRONT_TAR			= (NET_MSG_GCTRL + 512),	//<	��� ��û to (����)
	NET_MSG_GCTRL_CONFRONT_ANS			= (NET_MSG_GCTRL + 513),	//<	��� ��û ���� to (����)
	NET_MSG_GCTRL_CONFRONT_FB			= (NET_MSG_GCTRL + 514),	//<	��� ��û ���� to (��û��)

	NET_MSG_GCTRL_CONFRONT_START2_FLD	= (NET_MSG_GCTRL + 515),	//<	��� ���� to (�ʵ�)
	NET_MSG_GCTRL_CONFRONT_START2_CLT	= (NET_MSG_GCTRL + 516),	//<	��� ���� to (Ŭ���̾�Ʈ)
	NET_MSG_GCTRL_CONFRONT_FIGHT2_CLT	= (NET_MSG_GCTRL + 517),	//<	��� FIGHT to (Ŭ���̾�Ʈ)

	NET_MSG_GCTRL_CONFRONT_END2_FLD		= (NET_MSG_GCTRL + 518),	//<	��� ���� to (�ʵ�).
	NET_MSG_GCTRL_CONFRONT_END2_CLT		= (NET_MSG_GCTRL + 519),	//<	��� ���� to (Ŭ���̾�Ʈ).
	NET_MSG_GCTRL_CONFRONT_END2_CLT_BRD	= (NET_MSG_GCTRL + 520),	//<	��� ���� to (�ֺ� Ŭ���̾�Ʈ).
	NET_MSG_GCTRL_CONFRONT_END2_AGT		= (NET_MSG_GCTRL + 521),	//<	��� ���� to (������Ʈ).
	NET_MSG_GCTRL_CONFRONT_END2_CLT_MBR	= (NET_MSG_GCTRL + 522),	//<	��� ���� to (��� ����� Ŭ���̾�Ʈ).

	NET_MSG_GCTRL_CONFRONT_RECOVE		= (NET_MSG_GCTRL + 523),	//<	��ÿɼ� ȸ���� ��� Ƚ�� to (Ŭ���̾�Ʈ)

	NET_MSG_GCTRL_CONFRONTPTY_START2_FLD= (NET_MSG_GCTRL + 525),	//<	(��Ƽ) ��� ���� to (�ʵ�).
	NET_MSG_GCTRL_CONFRONTPTY_START2_CLT= (NET_MSG_GCTRL + 526),	//<	(��Ƽ) ��� ���� to (Ŭ���̾�Ʈ)

	NET_MSG_GCTRL_CONFRONTPTY_END2_FLD	= (NET_MSG_GCTRL + 527),	//<	(��Ƽ) ��� ���� to (�ʵ�).
	NET_MSG_GCTRL_CONFRONTPTY_END2_CLT	= (NET_MSG_GCTRL + 528),	//<	(��Ƽ) ��� ���� to (Ŭ���̾�Ʈ).

	NET_MSG_GCTRL_CONFRONTPTY_CHECKMBR_FLD = (NET_MSG_GCTRL + 529),	//<	(��Ƽ) ��ÿ� ������ ��� �˻� to (�ʵ�).
	NET_MSG_GCTRL_CONFRONTPTY_CHECKMBR_AGT = (NET_MSG_GCTRL + 529),	//<	(��Ƽ) ��ÿ� ������ ��� to (������Ʈ).

	NET_MSG_GCTRL_CONFRONTSPTY_EXP_FLD	= (NET_MSG_GCTRL + 530),	//<	�п� ��Ƽ ��� �¸��� ����ġ ��ü ������ exp �ο�.

	NET_MSG_GCTRL_CONFRONTCLB_START2_FLD= (NET_MSG_GCTRL + 531),	//<	(Ŭ��) ��� ���� to (�ʵ�).
	NET_MSG_GCTRL_CONFRONTCLB_START2_CLT= (NET_MSG_GCTRL + 532),	//<	(Ŭ��) ��� ���� to (Ŭ���̾�Ʈ)

	NET_MSG_GCTRL_CONFRONTCLB_END2_FLD	= (NET_MSG_GCTRL + 533),	//<	(Ŭ��) ��� ���� to (�ʵ�).
	NET_MSG_GCTRL_CONFRONTCLB_END2_CLT	= (NET_MSG_GCTRL + 534),	//<	(Ŭ��) ��� ���� to (Ŭ���̾�Ʈ).

	NET_MSG_GCTRL_CONFRONTCLB_CHECKMBR_FLD = (NET_MSG_GCTRL + 535),	//<	(Ŭ��) ��ÿ� ������ ��� �˻� to (�ʵ�).
	NET_MSG_GCTRL_CONFRONTCLB_CHECKMBR_AGT = (NET_MSG_GCTRL + 536),	//<	(Ŭ��) ��ÿ� ������ ��� to (������Ʈ).

	NET_MSG_GCTRL_SERVERTEST			= (NET_MSG_GCTRL + 537),	// Purpose for field server test
	NET_MSG_GCTRL_SERVERTEST_FB			= (NET_MSG_GCTRL + 538),

	NET_MSG_GCTRL_REQ_GENITEM_AGT		= (NET_MSG_GCTRL + 541),	//<	������ ���� ���� to (������Ʈ).
	NET_MSG_GCTRL_REQ_GENITEM_FLD_FB	= (NET_MSG_GCTRL + 542),	//<	������ ���� �亯 to (�ʵ�).

	NET_MSG_GCTRL_REQ_GESTURE			= (NET_MSG_GCTRL + 543),	//<	������ ����.
	NET_MSG_GCTRL_REQ_GESTURE_BRD		= (NET_MSG_GCTRL + 544),	//<	������ ���� BRD.

	NET_MSG_GCTRL_QITEMFACT_BRD			= (NET_MSG_GCTRL + 545),	//<	?������ ��ġ BRD.
	NET_MSG_GCTRL_QITEMFACT_END_BRD		= (NET_MSG_GCTRL + 546),	//<	?������ ���� BRD.

	NET_MSG_GCTRL_EVENTFACT_BRD			= (NET_MSG_GCTRL + 547),	//<	?������ �̺�Ʈ ���� BRD.
	NET_MSG_GCTRL_EVENTFACT_END_BRD		= (NET_MSG_GCTRL + 548),	//<	?������ �̺�Ʈ ���� BRD.
	NET_MSG_GCTRL_EVENTFACT_INFO		= (NET_MSG_GCTRL + 549),	//<	?������ �̺�Ʈ BRD.

	NET_MSG_GCTRL_INVEN_CLEANSER		= (NET_MSG_GCTRL + 551),	//<	������ ������ ����Ͽ� ��ȯ�� ������ �ǵ���.
	NET_MSG_GCTRL_INVEN_CLEANSER_FB		= (NET_MSG_GCTRL + 552),	//<	������ ������ ����Ͽ� ��ȯ�� ������ �ǵ��� FB.

	NET_MSG_GCTRL_INVEN_DEL_ITEM_TIMELMT= (NET_MSG_GCTRL + 553),	//<	���� �ð� ����� ������ ����.

	NET_MSG_GCTRL_INVEN_RESET_SKST		= (NET_MSG_GCTRL + 554),	//<	���� ��ų ���� ��û.
	NET_MSG_GCTRL_INVEN_RESET_SKST_FB	= (NET_MSG_GCTRL + 555),	//<	���� ��ų ���� ����.

	NET_MSG_GCTRL_INVEN_CHARCARD		= (NET_MSG_GCTRL + 560),	//<	�ɸ��� �߰� ī�� ��� ��û.
	NET_MSG_GCTRL_INVEN_CHARCARD_FB		= (NET_MSG_GCTRL + 561),	//<	�ɸ��� �߰� ī�� ��� ��û FB.

	NET_MSG_GCTRL_INVEN_INVENLINE		= (NET_MSG_GCTRL + 562),	//<	�κ����� �߰� ī�� ��� ��û.
	NET_MSG_GCTRL_INVEN_INVENLINE_FB	= (NET_MSG_GCTRL + 563),	//<	�κ����� �߰� ī�� ��� ��û FB.

	NET_MSG_GCTRL_INVEN_STORAGECARD		= (NET_MSG_GCTRL + 564),	//<	â�� �߰� ī�� ��� ��û.
	NET_MSG_GCTRL_INVEN_STORAGECARD_FB	= (NET_MSG_GCTRL + 565),	//<	â�� �߰� ī�� ��� ��û FB.

	NET_MSG_GCTRL_INVEN_STORAGEOPEN		= (NET_MSG_GCTRL + 566),	//<	��� â�� �̿� ī�� ��� ��û.
	NET_MSG_GCTRL_INVEN_STORAGEOPEN_FB	= (NET_MSG_GCTRL + 567),	//<	��� â�� �̿� ī�� ��� ��û FB.
	NET_MSG_GCTRL_INVEN_STORAGECLOSE	= (NET_MSG_GCTRL + 568),	//<	��� â�� �̿� ī�� ��� �Ϸ�.

	NET_MSG_GCTRL_INVEN_PREMIUMSET		= (NET_MSG_GCTRL + 569),	//<	�����̾��� ��� ��û.
	NET_MSG_GCTRL_INVEN_PREMIUMSET_FB	= (NET_MSG_GCTRL + 570),	//<	�����̾��� ��� ��û FB.

	NET_MSG_GCTRL_FIRECRACKER			= (NET_MSG_GCTRL + 571),	//<	���� ��� ��û.
	NET_MSG_GCTRL_FIRECRACKER_FB		= (NET_MSG_GCTRL + 572),	//<	���� ��� ��û FB.
	NET_MSG_GCTRL_FIRECRACKER_BRD		= (NET_MSG_GCTRL + 573),	//<	���� ���.

	NET_MSG_GCTRL_REVIVE				= (NET_MSG_GCTRL + 574),	//<	��ȥ�� ���
	NET_MSG_GCTRL_REVIVE_FB				= (NET_MSG_GCTRL + 575),	//<	��ȥ�� ��� FB.

	NET_MSG_GCTRL_PLAYERKILLING_ADD		= (NET_MSG_GCTRL + 576),	//<	player killing ����� �߰�.
	NET_MSG_GCTRL_PLAYERKILLING_DEL		= (NET_MSG_GCTRL + 577),	//<	player killing ����� ����.

	NET_MSG_GCTRL_GET_CHARGEDITEM_FROMDB	= (NET_MSG_GCTRL + 579),//<	������ ������ ���� DB���� �������� ��û
	NET_MSG_GCTRL_GET_CHARGEDITEM_FROMDB_FB = (NET_MSG_GCTRL + 580),//<	������ ������ ���� DB���� �������� ��û FB
	NET_MSG_GCTRL_CHARGED_ITEM_GET		= (NET_MSG_GCTRL + 581),	//<	������ ������ �������� ��û.
	NET_MSG_GCTRL_CHARGED_ITEM_GET_FB	= (NET_MSG_GCTRL + 582),	//<	������ ������ �������� ��û FB.
	NET_MSG_GCTRL_CHARGED_ITEM_DEL		= (NET_MSG_GCTRL + 583),	//<	������ ������ ����Ʈ���� ����.
	NET_MSG_GCTRL_CHARGED_ITEM2_INVEN	= (NET_MSG_GCTRL + 584),	//<	������ ������ �κ��� �ֱ� ��û.

	NET_MSG_GCTRL_INVEN_RANDOMBOXOPEN	= (NET_MSG_GCTRL + 585),	//<	random box open ��û.
	NET_MSG_GCTRL_INVEN_RANDOMBOXOPEN_FB= (NET_MSG_GCTRL + 586),	//<	random box open ��û FB.

	NET_MSG_GCTRL_INVEN_DISJUNCTION		= (NET_MSG_GCTRL + 587),	//<	�ڽ��� �и� ��û.
	NET_MSG_GCTRL_INVEN_DISJUNCTION_FB	= (NET_MSG_GCTRL + 588),	//<	�ڽ��� �и� ��û FB.

	NET_MSG_GCTRL_INVEN_HAIR_CHANGE		= (NET_MSG_GCTRL + 591),	//<	��Ÿ�� ���� ��û.
	NET_MSG_GCTRL_INVEN_HAIR_CHANGE_FB	= (NET_MSG_GCTRL + 592),	//<	��Ÿ�� ���� ��� FB.
	NET_MSG_GCTRL_INVEN_HAIR_CHANGE_BRD	= (NET_MSG_GCTRL + 593),	//<	��Ÿ�� ���� ��� BRD.

	NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE		= (NET_MSG_GCTRL + 594),	//<	��Ÿ�� ���� ��û.
	NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE_FB		= (NET_MSG_GCTRL + 595),	//<	��Ÿ�� ���� ��� FB.
	NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE_BRD	= (NET_MSG_GCTRL + 596),	//<	��Ÿ�� ���� ��� BRD.

	NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE		= (NET_MSG_GCTRL + 597),	//<	����÷� ���� ��û.
	NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_FB		= (NET_MSG_GCTRL + 598),	//<	����÷� ���� ��� FB.
	NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_BRD	= (NET_MSG_GCTRL + 599),	//<	����÷� ���� ��� BRD.

	NET_MSG_GCTRL_INVEN_FACE_CHANGE		= (NET_MSG_GCTRL + 601),	//<	�󱼽�Ÿ�� ���� ��û.
	NET_MSG_GCTRL_INVEN_FACE_CHANGE_FB	= (NET_MSG_GCTRL + 602),	//<	�󱼽�Ÿ�� ���� ��� FB.
	NET_MSG_GCTRL_INVEN_FACE_CHANGE_BRD	= (NET_MSG_GCTRL + 603),	//<	�󱼽�Ÿ�� ���� ��� BRD.

	NET_MSG_GCTRL_INVEN_REMODELOPEN		= (NET_MSG_GCTRL + 604),	//<	���� ��� �̿� ī�� ��� ��û.
	NET_MSG_GCTRL_INVEN_REMODELOPEN_FB	= (NET_MSG_GCTRL + 605),	//<	���� ��� �̿� ī�� ��� ��û FB.

	NET_MSG_GCTRL_INVEN_GARBAGEOPEN		= (NET_MSG_GCTRL + 606),	//<	������ ��� �̿� ī�� ��� ��û
	NET_MSG_GCTRL_INVEN_GARBAGEOPEN_FB	= (NET_MSG_GCTRL + 607),	//<	������ ��� �̿� ī�� ��� ��û FB.
	NET_MSG_GCTRL_GARBAGE_RESULT		= (NET_MSG_GCTRL + 608),	//<	������ ���� ��û
	NET_MSG_GCTRL_GARBAGE_RESULT_FB		= (NET_MSG_GCTRL + 609),	//<	������ ���� ��û FB.

	NET_MSG_GCTRL_CROW_MOVETO			= (NET_MSG_GCTRL + 611),
	NET_MSG_GCTRL_CROW_ATTACK			= (NET_MSG_GCTRL + 612),
	NET_MSG_GCTRL_CROW_AVOID			= (NET_MSG_GCTRL + 613),
	NET_MSG_GCTRL_CROW_DAMAGE			= (NET_MSG_GCTRL + 614),
	NET_MSG_GCTRL_CROW_SKILL			= (NET_MSG_GCTRL + 615),

	NET_MSG_GCTRL_INVEN_RENAME			= (NET_MSG_GCTRL + 620),	//<	�̸� ���� ��û.
	NET_MSG_GCTRL_INVEN_RENAME_FB		= (NET_MSG_GCTRL + 621),	//<	�̸� ���� ��� FB.
	NET_MSG_GCTRL_INVEN_RENAME_FROM_DB	= (NET_MSG_GCTRL + 622),	//<	DB ���� ��� FB.
	NET_MSG_GCTRL_INVEN_RENAME_AGTBRD	= (NET_MSG_GCTRL + 623),	//<	Agent�� ���ä�� BRD ��û.
	NET_MSG_GCTRL_INVEN_RENAME_BRD		= (NET_MSG_GCTRL + 624),	//<	�̸� ���� ��� BRD.

	NET_MSG_GCTRL_PARTY_MBR_RENAME_CLT	= (NET_MSG_GCTRL + 626),	//< �̸� ������ ��Ƽ������� �˸�.
	NET_MSG_GCTRL_CLUB_MBR_RENAME_CLT	= (NET_MSG_GCTRL + 627),	//< �̸� ������ Ŭ��������� �˸�.
	NET_MSG_GCTRL_FRIEND_RENAME_CLT		= (NET_MSG_GCTRL + 628),	//< �̸� ������ ģ���鿡�� �˸�.
	NET_MSG_GCTRL_FRIEND_PHONENUMBER_CLT= (NET_MSG_GCTRL + 629),	//< ����ȣ ������ ģ���鿡�� �˸�.
	NET_MSG_GCTRL_PARTY_MBR_RENAME_FLD	= (NET_MSG_GCTRL + 630),	//< �̸� ������ FieldServer�� �˸�.
	NET_MSG_GCTRL_CLUB_MBR_RENAME_FLD	= (NET_MSG_GCTRL + 631),	//< �̸� ������ FieldServer�� �˸�.
	NET_MSG_GCTRL_FRIEND_RENAME_FLD		= (NET_MSG_GCTRL + 632),	//< �̸� ������ FieldServer�� �˸�.
	NET_MSG_GCTRL_CHANGE_NAMEMAP	 	= (NET_MSG_GCTRL + 633),	//< GLLandMan �� NameMap ����


	NET_MSG_GCTRL_REQ_QUEST_START		= (NET_MSG_GCTRL + 646),	//<	����Ʈ ������ ��û.
	NET_MSG_GCTRL_REQ_QUEST_START_FB	= (NET_MSG_GCTRL + 647),	//<	����Ʈ ������ ��û FB.
	NET_MSG_GCTRL_REQ_QUEST_TALK		= (NET_MSG_GCTRL + 648),	//<	����Ʈ ��ȭ ���� ��û.
	NET_MSG_GCTRL_REQ_QUEST_TALK_FB		= (NET_MSG_GCTRL + 649),	//<	����Ʈ ��ȭ ���� ��û FB.

	NET_MSG_GCTRL_QUEST_PROG_GIVEUP		= (NET_MSG_GCTRL + 651),	//<	����Ʈ ���� ���� ��û.
	NET_MSG_GCTRL_QUEST_PROG_DEL		= (NET_MSG_GCTRL + 652),	//<	����Ʈ ���� ����Ʈ���� ����.
	NET_MSG_GCTRL_QUEST_END_DEL			= (NET_MSG_GCTRL + 653),	//<	����Ʈ �Ϸ� ����Ʈ���� ����.
	NET_MSG_GCTRL_QUEST_END_STREAM		= (NET_MSG_GCTRL + 654),	//<	����Ʈ �Ϸ� ����.

	NET_MSG_GCTRL_QUEST_PROG_STREAM		= (NET_MSG_GCTRL + 655),	//<	����Ʈ ���� ����.
	NET_MSG_GCTRL_QUEST_PROG_STEP_STREAM= (NET_MSG_GCTRL + 656),	//<	����Ʈ ���� step.
	NET_MSG_GCTRL_QUEST_PROG_INVEN		= (NET_MSG_GCTRL + 657),	//<	����Ʈ INVENTORY.

	NET_MSG_GCTRL_QUEST_PROG_NPCTALK	= (NET_MSG_GCTRL + 661),	//<	����Ʈ ���� npc ��ȭ.
	NET_MSG_GCTRL_QUEST_PROG_NPCTALK_FB	= (NET_MSG_GCTRL + 662),	//<	����Ʈ ���� npc ��ȭ.

	NET_MSG_GCTRL_QUEST_PROG_MOBKILL		= (NET_MSG_GCTRL + 663),	//<	����Ʈ ���� mob ����.
	NET_MSG_GCTRL_QUEST_PARTY_PROG_MOBKILL	= (NET_MSG_GCTRL + 664),	//<	����Ʈ ���� mob ����.( ��Ƽ����Ʈ �Ϸ�� )
	NET_MSG_GCTRL_QUEST_PROG_QITEM			= (NET_MSG_GCTRL + 665),	//<	����Ʈ ���� ����Ʈ ������ ����.
	NET_MSG_GCTRL_QUEST_PARTY_PROG_QITEM	= (NET_MSG_GCTRL + 666),	//<	����Ʈ ���� ����Ʈ ������ ����.( ��Ƽ����Ʈ �Ϸ�� )
	NET_MSG_GCTRL_QUEST_PROG_REACHZONE		= (NET_MSG_GCTRL + 667),	//<	����Ʈ ���� ��ġ ����.
	NET_MSG_GCTRL_QUEST_PROG_READ			= (NET_MSG_GCTRL + 668),	//<	����Ʈ �б� ��û ����.
	NET_MSG_GCTRL_QUEST_PROG_TIME			= (NET_MSG_GCTRL + 669),	//<	����Ʈ ��� �ð�.
	NET_MSG_GCTRL_QUEST_PROG_TIMEOVER		= (NET_MSG_GCTRL + 670),	//<	����Ʈ �ð� ���� �ʰ���.
	NET_MSG_GCTRL_QUEST_PROG_NONDIE			= (NET_MSG_GCTRL + 671),	//<	����Ʈ ��� ����.
	NET_MSG_GCTRL_QUEST_PROG_LEVEL			= (NET_MSG_GCTRL + 672),	//<	����Ʈ ���� ����.

	NET_MSG_GCTRL_QUEST_COMPLETE_FB		= (NET_MSG_GCTRL + 673),	//<	����Ʈ�� �Ϸ�Ǿ����� ���� ��ǰ�� ���� ���ѻ���.
	NET_MSG_GCTRL_REQ_QUEST_COMPLETE	= (NET_MSG_GCTRL + 674),	//<	����Ʈ �Ϸ� ��û ( ����ǰ ���� ).

	NET_MSG_GCTRL_QUEST_PROG_INVEN_TURN	= (NET_MSG_GCTRL + 681),	//<	����Ʈ �ι� ���� ����.
	NET_MSG_GCTRL_QUEST_PROG_INVEN_PICKUP= (NET_MSG_GCTRL + 682),	//<	����Ʈ �ι�� ������ ����.

	NET_MSG_GCTRL_QUEST_PROG_INVEN_INSERT= (NET_MSG_GCTRL + 683),	//<	����Ʈ �ι� ������ ����.
	NET_MSG_GCTRL_QUEST_PROG_INVEN_DELETE= (NET_MSG_GCTRL + 684),	//<	����Ʈ �ι� ������ ����.

	
	NET_MSG_GCTRL_PMARKET_TITLE			= (NET_MSG_GCTRL + 701),	//<	���� ������ Ÿ��Ʋ ����.
	NET_MSG_GCTRL_PMARKET_TITLE_FB		= (NET_MSG_GCTRL + 702),	//<	���� ������ Ÿ��Ʋ ����.

	NET_MSG_GCTRL_PMARKET_REGITEM		= (NET_MSG_GCTRL + 703),	//<	���� ������ �Ǹ� ������ ���.
	NET_MSG_GCTRL_PMARKET_REGITEM_FB	= (NET_MSG_GCTRL + 704),	//<	���� ������ �Ǹ� ������ ��� FB.

	NET_MSG_GCTRL_PMARKET_DISITEM		= (NET_MSG_GCTRL + 705),	//<	���� �������� ������ ����.
	NET_MSG_GCTRL_PMARKET_DISITEM_FB	= (NET_MSG_GCTRL + 706),	//<	���� �������� ������ ���� FB.
	NET_MSG_GCTRL_PMARKET_OPEN			= (NET_MSG_GCTRL + 707),	//<	���� ���� ����.
	NET_MSG_GCTRL_PMARKET_OPEN_FB		= (NET_MSG_GCTRL + 708),	//<	���� ���� ���� FB.
	NET_MSG_GCTRL_PMARKET_OPEN_BRD		= (NET_MSG_GCTRL + 709),	//<	���� ���� ���� BRD.

	NET_MSG_GCTRL_PMARKET_CLOSE			= (NET_MSG_GCTRL + 710),	//<	���� ���� ����.
	NET_MSG_GCTRL_PMARKET_CLOSE_FB		= (NET_MSG_GCTRL + 711),	//<	���� ���� ���� FB.
	NET_MSG_GCTRL_PMARKET_CLOSE_BRD		= (NET_MSG_GCTRL + 712),	//<	���� ���� ���� BRD.
	
	NET_MSG_GCTRL_PMARKET_BUY			= (NET_MSG_GCTRL + 713),	//<	���� �������� ����.
	NET_MSG_GCTRL_PMARKET_BUY_FB		= (NET_MSG_GCTRL + 714),	//<	���� �������� ���� FB.

	NET_MSG_GCTRL_PMARKET_ITEM_INFO		= (NET_MSG_GCTRL + 715),	//<	���� ������ �Ǹ� ������ ���� ��û.
	NET_MSG_GCTRL_PMARKET_ITEM_INFO_BRD	= (NET_MSG_GCTRL + 716),	//<	���� ������ �Ǹ� ������ BRD.
	NET_MSG_GCTRL_PMARKET_ITEM_UPDATE_BRD= (NET_MSG_GCTRL + 717),	//<	���� ������ �Ǹ� ������ Update BRD.

	NET_MSG_GCTRL_CLUB_NEW				= (NET_MSG_GCTRL + 720),	//<	Ŭ�� ����.
	NET_MSG_GCTRL_CLUB_NEW_2AGT			= (NET_MSG_GCTRL + 721),	//<	Ŭ�� ���� to AGENT.
	NET_MSG_GCTRL_CLUB_NEW_DB2AGT		= (NET_MSG_GCTRL + 722),	//<	Ŭ�� ���� DB to AGENT.
	NET_MSG_GCTRL_CLUB_NEW_2FLD			= (NET_MSG_GCTRL + 723),	//<	Ŭ�� ���� to FIELD.
	NET_MSG_GCTRL_CLUB_NEW_FB			= (NET_MSG_GCTRL + 724),	//<	Ŭ�� ���� FB.

	NET_MSG_GCTRL_CLUB_DISSOLUTION		= (NET_MSG_GCTRL + 725),	//<	Ŭ�� ��ü ��û.
	NET_MSG_GCTRL_CLUB_DISSOLUTION_2FLD	= (NET_MSG_GCTRL + 726),	//<	Ŭ�� ��ü ��û.
	NET_MSG_GCTRL_CLUB_DISSOLUTION_FB	= (NET_MSG_GCTRL + 727),	//<	Ŭ�� ��ü ��û FB.
	NET_MSG_GCTRL_CLUB_DEL_2FLD			= (NET_MSG_GCTRL + 728),	//<	Ŭ�� ��ü to FIELD.
	NET_MSG_GCTRL_CLUB_DEL_2CLT			= (NET_MSG_GCTRL + 729),	//<	Ŭ�� ���� to CLIENT. ( ��ü �� Ż�� )
	NET_MSG_GCTRL_CLUB_DEL_BRD			= (NET_MSG_GCTRL + 730),	//<	Ŭ�� ���� BRD. ( ��ü �� Ż�� )

	NET_MSG_GCTRL_CLUB_INFO_2FLD		= (NET_MSG_GCTRL + 731),	//<	Ŭ�� ���� to FLIED.
	NET_MSG_GCTRL_CLUB_INFO_2CLT		= (NET_MSG_GCTRL + 732),	//<	Ŭ�� ���� to CLIENT.
	NET_MSG_GCTRL_CLUB_INFO_DISSOLUTION	= (NET_MSG_GCTRL + 733),	//<	Ŭ�� ��ü���� ���� to CLIENT.
	NET_MSG_GCTRL_CLUB_MEMBER_2CLT		= (NET_MSG_GCTRL + 734),	//<	Ŭ�� ��� to CLIENT.

	NET_MSG_GCTRL_CLUB_INFO_BRD			= (NET_MSG_GCTRL + 735),	//<	Ŭ�� ���� BRD.
	NET_MSG_GCTRL_CLUB_INFO_MARK_BRD	= (NET_MSG_GCTRL + 736),	//<	Ŭ�� ��ũ ���� BRD.
	NET_MSG_GCTRL_CLUB_INFO_NICK_BRD	= (NET_MSG_GCTRL + 737),	//<	Ŭ�� �г��� ���� BRD.

	NET_MSG_GCTRL_CLUB_MARK_INFO		= (NET_MSG_GCTRL + 740),	//<	Ŭ�� ��ũ ��û.
	NET_MSG_GCTRL_CLUB_MARK_INFO_FB		= (NET_MSG_GCTRL + 741),	//<	Ŭ�� ��ũ ��ȯ.

	NET_MSG_GCTRL_CLUB_MARK_CHANGE		= (NET_MSG_GCTRL + 742),	//<	Ŭ�� ��ũ ���� ��û.
	NET_MSG_GCTRL_CLUB_MARK_CHANGE_2FLD	= (NET_MSG_GCTRL + 743),	//<	Ŭ�� ��ũ ���� ��û.
	NET_MSG_GCTRL_CLUB_MARK_CHANGE_2CLT	= (NET_MSG_GCTRL + 744),	//<	Ŭ�� ��ũ ���� ��û.
	NET_MSG_GCTRL_CLUB_MARK_CHANGE_FB	= (NET_MSG_GCTRL + 745),	//<	Ŭ�� ��ũ ���� FB.

	NET_MSG_GCTRL_CLUB_RANK				= (NET_MSG_GCTRL + 750),	//<	Ŭ�� ��ũ ����.
	NET_MSG_GCTRL_CLUB_RANK_2AGT		= (NET_MSG_GCTRL + 751),	//<	Ŭ�� ��ũ ����.
	NET_MSG_GCTRL_CLUB_RANK_2FLD		= (NET_MSG_GCTRL + 752),	//<	Ŭ�� ��ũ ����.
	NET_MSG_GCTRL_CLUB_RANK_2CLT		= (NET_MSG_GCTRL + 753),	//<	Ŭ�� ��ũ ����.
	NET_MSG_GCTRL_CLUB_RANK_FB			= (NET_MSG_GCTRL + 754),	//<	Ŭ�� ��ũ ���� FB.

	NET_MSG_GCTRL_CLUB_MEMBER_REQ		= (NET_MSG_GCTRL + 755),	//<	Ŭ�� ���� ��û.
	NET_MSG_GCTRL_CLUB_MEMBER_REQ_ASK	= (NET_MSG_GCTRL + 756),	//<	Ŭ�� ���� ����.
	NET_MSG_GCTRL_CLUB_MEMBER_REQ_ANS	= (NET_MSG_GCTRL + 757),	//<	Ŭ�� ���� �亯.
	NET_MSG_GCTRL_CLUB_MEMBER_REQ_2AGT	= (NET_MSG_GCTRL + 758),	//<	Ŭ�� ���� �亯.
	NET_MSG_GCTRL_CLUB_MEMBER_REQ_FB	= (NET_MSG_GCTRL + 759),	//<	Ŭ�� ���� ��û FB.

	NET_MSG_GCTRL_CLUB_MEMBER_ADD_2FLD	= (NET_MSG_GCTRL + 760),	//<	Ŭ�� ��� �߰� to FIELD.

	NET_MSG_GCTRL_CLUB_MEMBER_DEL		= (NET_MSG_GCTRL + 761),	//<	Ŭ�� ��� ���� ��û.
	NET_MSG_GCTRL_CLUB_MEMBER_DEL_2FLD	= (NET_MSG_GCTRL + 762),	//<	Ŭ�� ��� ���� ��û to FIELD.
	NET_MSG_GCTRL_CLUB_MEMBER_DEL_2CLT	= (NET_MSG_GCTRL + 763),	//<	Ŭ�� ��� ���� ��û ��� to CLIENT.
	NET_MSG_GCTRL_CLUB_MEMBER_DEL_FB	= (NET_MSG_GCTRL + 764),	//<	Ŭ�� ��� ���� ��û ��� FB.

	NET_MSG_GCTRL_CLUB_MEMBER_NICK		= (NET_MSG_GCTRL + 765),	//<	Ŭ�� ��� ����.
	NET_MSG_GCTRL_CLUB_MEMBER_NICK_FB	= (NET_MSG_GCTRL + 766),	//<	Ŭ�� ��� ���� FB.

	NET_MSG_GCTRL_CLUB_MEMBER_STATE		= (NET_MSG_GCTRL + 767),	//<	Ŭ�� ��� ����.
	NET_MSG_GCTRL_CLUB_MEMBER_POS		= (NET_MSG_GCTRL + 768),	//<	Ŭ�� ��� ��ġ.

	NET_MSG_GCTRL_CLUB_MEMBER_SECEDE	= (NET_MSG_GCTRL + 769),	//<	Ŭ�� ��� Ż��.
	NET_MSG_GCTRL_CLUB_MEMBER_SECEDE_FB	= (NET_MSG_GCTRL + 770),	//<	Ŭ�� ��� Ż�� FB.

	NET_MSG_GCTRL_CLUB_MEMBER_DB2DEL	= (NET_MSG_GCTRL + 771),	//<	Ŭ�� ��� ���� ��û.

	NET_MSG_GCTRL_CLUB_BATTLE_REMAIN_BRD = (NET_MSG_GCTRL + 775),	//<	���� Ŭ�� ������ ����.
	NET_MSG_GCTRL_CLUB_BATTLE_START_FLD	= (NET_MSG_GCTRL + 776),	//<	���� Ŭ�� ������ ����.
	NET_MSG_GCTRL_CLUB_BATTLE_START_BRD	= (NET_MSG_GCTRL + 777),	//<	���� Ŭ�� ������ ����.

	NET_MSG_GCTRL_CLUB_BATTLE_END_FLD	= (NET_MSG_GCTRL + 778),	//<	���� Ŭ�� ������ ����.
	NET_MSG_GCTRL_CLUB_BATTLE_END_BRD	= (NET_MSG_GCTRL + 779),	//<	���� Ŭ�� ������ ����.

	NET_MSG_GCTRL_CLUB_CD_CERTIFY		  = (NET_MSG_GCTRL + 780),	//<	cd ����.
	NET_MSG_GCTRL_CLUB_CD_CERTIFY_FB      = (NET_MSG_GCTRL + 780),	//<	cd FB
	NET_MSG_GCTRL_CLUB_CD_CERTIFY_BRD	  = (NET_MSG_GCTRL + 781),	//<	cd ���� BRD.
	NET_MSG_GCTRL_CLUB_CD_CERTIFY_ING_BRD = (NET_MSG_GCTRL + 782),	//<	cd ���� ��� BRD

	NET_MSG_GCTRL_TOWER_DESTROYED_BRD	  = (NET_MSG_GCTRL + 783),	//<	cd ���� BRD.
	NET_MSG_GCTRL_CLUB_CERTIFIED_AGT	= (NET_MSG_GCTRL + 787),	//<	cd ���� �Ϸ� ( agent�� �˸� )
	NET_MSG_GCTRL_CLUB_CERTIFIED_FLD	= (NET_MSG_GCTRL + 788),	//<	cd ���� �Ϸ� ( field�� �˸� )
	NET_MSG_GCTRL_CLUB_CERTIFIED_CLT	= (NET_MSG_GCTRL + 789),	//<	cd ���� �Ϸ� ( field�� �˸� )

	NET_MSG_GCTRL_CLUB_COMMISSION		= (NET_MSG_GCTRL + 790),	//<	���� ���� ������ ����.
	NET_MSG_GCTRL_CLUB_COMMISSION_FB	= (NET_MSG_GCTRL + 791),	//<	���� ���� ������ ���� FB.
	NET_MSG_GCTRL_CLUB_COMMISSION_FLD	= (NET_MSG_GCTRL + 792),	//<	���� ���� ������ ���� FLD.
	NET_MSG_GCTRL_CLUB_COMMISSION_BRD	= (NET_MSG_GCTRL + 793),	//<	���� ���� ������ ���� BRD.
	NET_MSG_GCTRL_CLUB_INCOME_MONEY_AGT	= (NET_MSG_GCTRL + 794),	//<	Ŭ���� ���� �߻� AGT.

	NET_MSG_GCTRL_CLUB_GETSTORAGE		= (NET_MSG_GCTRL + 795),	//<	â�� ���� ��û.
	NET_MSG_GCTRL_CLUB_GETSTORAGE_FB	= (NET_MSG_GCTRL + 796),	//<	â�� ���� ��û.
	NET_MSG_GCTRL_CLUB_GETSTORAGE_ITEM	= (NET_MSG_GCTRL + 797),	//<	â�� ���� ȸ��.

	NET_MSG_GCTRL_CLUB_STORAGE_TO_HOLD	= (NET_MSG_GCTRL + 798),	//<	â�� ������ ���ø���.
	NET_MSG_GCTRL_CLUB_STORAGE_EX_HOLD	= (NET_MSG_GCTRL + 799),	//<	â�� �����۰� �տ� �鸰 ������ ��ȯ.
	NET_MSG_GCTRL_CLUB_HOLD_TO_STORAGE	= (NET_MSG_GCTRL + 800),	//<	�տ� �鸰 ������ â���� �ֱ�.
	NET_MSG_GCTRL_CLUB_STORAGE_SPLIT	= (NET_MSG_GCTRL + 801),	//<	â���� ������ �����ϱ�.
	NET_MSG_GCTRL_CLUB_STORAGE_SAVE_MONEY= (NET_MSG_GCTRL + 802),	//<	���� ����.
	NET_MSG_GCTRL_CLUB_STORAGE_DRAW_MONEY= (NET_MSG_GCTRL + 803),	//<	���� ����.
	
	NET_MSG_GCTRL_CLUB_STORAGE_INSERT	= (NET_MSG_GCTRL + 804),	//<	â���� �ֱ�.
	NET_MSG_GCTRL_CLUB_STORAGE_DELETE	= (NET_MSG_GCTRL + 805),	//<	â������ ����.
	NET_MSG_GCTRL_CLUB_STORAGE_DEL_INS	= (NET_MSG_GCTRL + 806),	//<	������ �ֱ� ���ÿ� �ϱ�.
	NET_MSG_GCTRL_CLUB_STORAGE_UPDATE_ITEM= (NET_MSG_GCTRL + 807),	//<	â���� �ִ� �������� ���� ����.
	NET_MSG_GCTRL_CLUB_STORAGE_UPDATE_MONEY= (NET_MSG_GCTRL + 808),	//<	â���� ����ݾ� ����.

	NET_MSG_GCTRL_CLUB_COMMISSION_RV_BRD= (NET_MSG_GCTRL + 809),	//<	���� ���� ������ ���� ���� BRD.

	NET_MSG_GCTRL_CLUB_STORAGE_RESET	= (NET_MSG_GCTRL + 811),	//<	Ŭ�� â�� ������ ����.
	NET_MSG_GCTRL_CLUB_STORAGE_GET_DB	= (NET_MSG_GCTRL + 812),	//<	Ŭ�� â�� ������ db���� ����.

	NET_MSG_GCTRL_CLUB_INCOME_RENEW		= (NET_MSG_GCTRL + 815),	//<	Ŭ�� ���ͱ� ���� ��û ( client->field ).
	NET_MSG_GCTRL_CLUB_INCOME_UP		= (NET_MSG_GCTRL + 816),	//<	Ŭ�� ���ͱ� ���� ��û ( field->agent ).
	NET_MSG_GCTRL_CLUB_INCOME_DN		= (NET_MSG_GCTRL + 817),	//<	Ŭ�� ���ͱ� ���� ȸ�� ( agent->field ).
	NET_MSG_GCTRL_CLUB_INCOME_FB		= (NET_MSG_GCTRL + 818),	//<	Ŭ�� ���ͱ� ���� ��� ( field->agent ).

	NET_MSG_GCTRL_CLUB_NOTICE_REQ		= (NET_MSG_GCTRL + 821),	//<	Ŭ�� ���� ���� ��û. ( client->agent )
	NET_MSG_GCTRL_CLUB_NOTICE_FB		= (NET_MSG_GCTRL + 822),	//<	Ŭ�� ���� ���� ���. ( agent->client )
	NET_MSG_GCTRL_CLUB_NOTICE_FLD		= (NET_MSG_GCTRL + 823),	//<	Ŭ�� ���� �ʵ忡 �˸�. ( agent->field )
	NET_MSG_GCTRL_CLUB_NOTICE_CLT		= (NET_MSG_GCTRL + 824),	//<	Ŭ�� ���� Ŭ���̾�Ʈ�� �˸�. ( field->client )

	NET_MSG_GCTRL_CLUB_SUBMASTER		= (NET_MSG_GCTRL + 825),	//<	Ŭ�� �θ� ����.
	NET_MSG_GCTRL_CLUB_SUBMASTER_FB		= (NET_MSG_GCTRL + 826),	//<	Ŭ�� �θ� ���� FB.
	NET_MSG_GCTRL_CLUB_SUBMASTER_BRD	= (NET_MSG_GCTRL + 827),	//<	Ŭ�� �θ� ���� �˸� CLT.
	NET_MSG_GCTRL_CLUB_SUBMASTER_FLD	= (NET_MSG_GCTRL + 828),	//<	Ŭ�� �θ� ���� �˸� FLD.

	NET_MSG_GCTRL_CLUB_ALLIANCE_REQ		= (NET_MSG_GCTRL + 831),	//<	Ŭ�� ���� ��û.
	NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_ASK	= (NET_MSG_GCTRL + 832),	//<	Ŭ�� ���� ��û ASK.
	NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_ANS	= (NET_MSG_GCTRL + 833),	//<	Ŭ�� ���� ��û ANS.
	NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_FB	= (NET_MSG_GCTRL + 834),	//<	Ŭ�� ���� ��û ��� FB.

	NET_MSG_GCTRL_CLUB_ALLIANCE_ADD_FLD	= (NET_MSG_GCTRL + 837),	//<	Ŭ�� ���� Ŭ�� ADD FLD.
	NET_MSG_GCTRL_CLUB_ALLIANCE_ADD_CLT	= (NET_MSG_GCTRL + 838),	//<	Ŭ�� ���� Ŭ�� ADD CLT.

	NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_REQ	= (NET_MSG_GCTRL + 839),	//<	Ŭ�� ���� Ŭ�� ���� ��û.
	NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_FB	= (NET_MSG_GCTRL + 840),	//<	Ŭ�� ���� Ŭ�� ���� ��û FB.

	NET_MSG_GCTRL_CLUB_ALLIANCE_SEC_REQ	= (NET_MSG_GCTRL + 841),	//<	Ŭ�� ���� Ż�� ��û.
	NET_MSG_GCTRL_CLUB_ALLIANCE_SEC_FB	= (NET_MSG_GCTRL + 842),	//<	Ŭ�� ���� Ż�� ��û FB.

	NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_FLD	= (NET_MSG_GCTRL + 843),	//<	Ŭ�� ���� Ŭ�� DEL FLD.
	NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_CLT	= (NET_MSG_GCTRL + 844),	//<	Ŭ�� ���� Ŭ�� DEL CLT.

	NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_REQ	= (NET_MSG_GCTRL + 845),	//<	Ŭ�� ���� ��ü FLD.
	NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_FB	= (NET_MSG_GCTRL + 846),	//<	Ŭ�� ���� ��ü FLD.

	NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_FLD	= (NET_MSG_GCTRL + 847),	//<	Ŭ�� ���� ��ü FLD.
	NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_CLT	= (NET_MSG_GCTRL + 848),	//<	Ŭ�� ���� ��ü CLT.


	NET_MSG_GCTRL_CLUB_BATTLE_REQ		= (NET_MSG_GCTRL + 849),	//<	Ŭ�� ��Ʋ ��û 
	NET_MSG_GCTRL_CLUB_BATTLE_REQ_ASK	= (NET_MSG_GCTRL + 850),	//<	Ŭ�� ��Ʋ ��û ASK.
	NET_MSG_GCTRL_CLUB_BATTLE_REQ_ANS	= (NET_MSG_GCTRL + 851),	//<	Ŭ�� ��Ʋ ��û ANS.
	NET_MSG_GCTRL_CLUB_BATTLE_REQ_FB	= (NET_MSG_GCTRL + 852),	//<	Ŭ�� ��Ʋ ��û ��� FB.

	NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_FLD	= (NET_MSG_GCTRL + 853),	//<	Ŭ�� ��Ʋ ���� FLD.
	NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_CLT	= (NET_MSG_GCTRL + 854),	//<	Ŭ�� ��Ʋ ���� CLT(���).
	NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_CLT2= (NET_MSG_GCTRL + 855),	//<	Ŭ�� ��Ʋ ���� CLT2(��¥����).
	NET_MSG_GCTRL_CLUB_BATTLE_OVER_FLD	= (NET_MSG_GCTRL + 856),	//<	Ŭ�� ��Ʋ ���� FLD.
	NET_MSG_GCTRL_CLUB_BATTLE_OVER_CLT	= (NET_MSG_GCTRL + 857),	//<	Ŭ�� ��Ʋ ���� CLT.

	NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ		= (NET_MSG_GCTRL + 858),	//<	Ŭ�� ��Ʋ ���� ��û.
	NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_ASK	= (NET_MSG_GCTRL + 859),	//<	Ŭ�� ��Ʋ ���� ��û ASK.
	NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_ANS	= (NET_MSG_GCTRL + 860),	//<	Ŭ�� ��Ʋ ���� ��û ANS.
	NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_FB	= (NET_MSG_GCTRL + 861),	//<	Ŭ�� ��Ʋ ���� ��û ��� FB.
	
	NET_MSG_GCTRL_CLUB_BATTLE_SUBMISSION_REQ	= (NET_MSG_GCTRL + 862),	//<	Ŭ�� ��Ʋ �׺� ��û.
	NET_MSG_GCTRL_CLUB_BATTLE_SUBMISSION_REQ_FB	= (NET_MSG_GCTRL + 863),	//<	Ŭ�� ��Ʋ �׺� ��û ��� FB.

	NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE_AGT	= (NET_MSG_GCTRL + 864),	//<	Ŭ�� ��Ʋ ų�� UPDATE Field -> Agent
	NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE_FLD	= (NET_MSG_GCTRL + 865),	//<	Ŭ�� ��Ʋ ų�� UPDATE Agent -> Field
	NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE		= (NET_MSG_GCTRL + 866),	//<	Ŭ�� ��Ʋ ų�� UPDATE Ŭ���̾�Ʈ

	NET_MSG_GCTRL_CLUB_BATTLE_POINT_UPDATE		= (NET_MSG_GCTRL + 867),	//<	Ŭ�� ��Ʋ ų/������ ����

	NET_MSG_GCTRL_CLUB_BATTLE_LAST_KILL_UPDATE_FLD	= (NET_MSG_GCTRL + 868),	//<	Ŭ�� ��Ʋ ����� Field���� ���� Agent -> Field
	NET_MSG_GCTRL_CLUB_BATTLE_LAST_KILL_UPDATE_AGT	= (NET_MSG_GCTRL + 869),	//<	Ŭ�� ��Ʋ ����� Field���� ���� ��� Field -> Agent


	NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ		= (NET_MSG_GCTRL + 870),	//<	���� ��Ʋ ��û 
	NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_ASK	= (NET_MSG_GCTRL + 871),	//<	���� ��Ʋ ��û ASK.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_ANS	= (NET_MSG_GCTRL + 872),	//<	���� ��Ʋ ��û ANS.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_FB	= (NET_MSG_GCTRL + 873),	//<	���� ��Ʋ ��û ��� FB.

	NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ		= (NET_MSG_GCTRL + 874),	//<	���� ��Ʋ ���� ��û.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_ASK	= (NET_MSG_GCTRL + 875),	//<	���� ��Ʋ ���� ��û ASK.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_ANS	= (NET_MSG_GCTRL + 876),	//<	���� ��Ʋ ���� ��û ANS.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_FB	= (NET_MSG_GCTRL + 877),	//<	���� ��Ʋ ���� ��û ��� FB.

	NET_MSG_GCTRL_ALLIANCE_BATTLE_SUBMISSION_REQ	= (NET_MSG_GCTRL + 878),	//<	���� ��Ʋ �׺� ��û.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_SUBMISSION_REQ_FB	= (NET_MSG_GCTRL + 879),	//<	���� ��Ʋ �׺� ��û ��� FB.

	
	
	
	
	NET_MSG_GCTRL_LEVEL_EVENT_END_FLD		= (NET_MSG_GCTRL + 896),	//<	���� �̺�Ʈ ����.
	NET_MSG_GCTRL_LEVEL_EVENT_WARNING_FLD	= (NET_MSG_GCTRL + 897),	//< ���� �̺�Ʈ ��� �޼���.
	NET_MSG_GCTRL_LEVEL_EVENT_COUNTDOWN_FLD = (NET_MSG_GCTRL + 898),	//< ���� �̺�Ʈ ī��Ʈ �޼���.

	NET_MSG_GM_EVENT_EX					= (NET_MSG_GCTRL + 899),	//<	EX�̺�Ʈ ����.
	NET_MSG_GM_EVENT_EX_END				= (NET_MSG_GCTRL + 900),	//<	EX�̺�Ʈ ����.

	NET_MSG_GM_EVENT_EXP				= (NET_MSG_GCTRL + 901),	//<	����ġ �̺�Ʈ ����.
	NET_MSG_GM_EVENT_EXP_FB				= (NET_MSG_GCTRL + 902),	//<	����ġ �̺�Ʈ ���� FB.
	NET_MSG_GM_EVENT_EXP_END			= (NET_MSG_GCTRL + 903),	//<	����ġ �̺�Ʈ ����.
	NET_MSG_GM_EVENT_EXP_END_FB			= (NET_MSG_GCTRL + 904),	//<	����ġ �̺�Ʈ ���� FB.

	NET_MSG_GM_EVENT_ITEM_GEN			= (NET_MSG_GCTRL + 905),	//<	������ �߻��� �̺�Ʈ.
	NET_MSG_GM_EVENT_ITEM_GEN_FB		= (NET_MSG_GCTRL + 906),	//<	������ �߻��� �̺�Ʈ FB.

	NET_MSG_GM_EVENT_ITEM_GEN_END		= (NET_MSG_GCTRL + 907),	//<	������ �߻��� �̺�Ʈ ����.
	NET_MSG_GM_EVENT_ITEM_GEN_END_FB	= (NET_MSG_GCTRL + 908),	//<	������ �߻��� �̺�Ʈ ���� FB.

	NET_MSG_GM_EVENT_MONEY_GEN			= (NET_MSG_GCTRL + 909),	//<	�� �߻��� �̺�Ʈ.
	NET_MSG_GM_EVENT_MONEY_GEN_END		= (NET_MSG_GCTRL + 910),	//<	�� �߻��� �̺�Ʈ ����.

	NET_MSG_GCTRL_PERIOD				= (NET_MSG_GCTRL + 921),
	NET_MSG_GCTRL_WEATHER				= (NET_MSG_GCTRL + 922),
	NET_MSG_GCTRL_WHIMSICAL				= (NET_MSG_GCTRL + 923),
	NET_MSG_GCTRL_FIELDINFO_RESET		= (NET_MSG_GCTRL + 924),

	NET_MSG_SERVER_CTRL_WEATHER			= (NET_MSG_GCTRL + 931),	//<	������ ���� ����.
	NET_MSG_SERVER_CTRL_TIME			= (NET_MSG_GCTRL + 932),	//<	������ �ð� ����.
	NET_MSG_SERVER_CTRL_MONTH			= (NET_MSG_GCTRL + 933),	//<	������ �� ����.
	NET_MSG_SERVER_CTRL_GENITEMHOLD		= (NET_MSG_GCTRL + 934),	//<	������ ������ ������ ����.
	NET_MSG_SERVER_PLAYERKILLING_MODE	= (NET_MSG_GCTRL + 935),	//<	player killing on/off.

	NET_MSG_GM_MOVE2GATE				= (NET_MSG_GCTRL + 936),	//<	�ɸ��͸� gate no �� �̵�.
	NET_MSG_GM_MOVE2GATE_FLD			= (NET_MSG_GCTRL + 937),	//<	�ɸ��͸� gate no �� �̵� Agent -> Field
	NET_MSG_GM_MOVE2GATE_FB				= (NET_MSG_GCTRL + 938),	//<	�ɸ��͸� gate no �� �̵�.
	NET_MSG_GM_KICK_USER				= (NET_MSG_GCTRL + 940),	//<	Ư�� �÷��̾ ������.
	NET_MSG_GM_KICK_USER_PROC			= (NET_MSG_GCTRL + 941),	//<	Ư�� �÷��̾ ������.
	NET_MSG_GM_KICK_USER_PROC_FLD		= (NET_MSG_GCTRL + 942),    //<  Ư�� �÷��̾� ���� �������� Agent -> Field
	NET_MSG_GM_MOVE2MAPPOS				= (NET_MSG_GCTRL + 944),	//<	ĳ���͸� Ư������ Ư����ġ�� �̵�.
	NET_MSG_GM_MOVE2MAPPOS_FLD			= (NET_MSG_GCTRL + 945),	//<	ĳ���͸� Ư������ Ư����ġ�� �̵�. Agent -> Field

	NET_MSG_BLOCK_DETECTED				= (NET_MSG_GCTRL + 946),	//<	�ɸ��͸� gate no �� �̵�.
	NET_MSG_SERVER_GENERALCHAT			= (NET_MSG_GCTRL + 947),	//<	���� �˸� �޽��� �߻���.

	NET_MSG_GM_WHERE_NPC				= (NET_MSG_GCTRL + 948),	//<	NPC�� ��� ��ġ�� �ִ��� �˻�.
	NET_MSG_GM_WHERE_NPC_FLD			= (NET_MSG_GCTRL + 949),	//<	NPC�� ��� ��ġ�� �ִ��� �˻�. Agent -> Field
	NET_MSG_GM_WHERE_NPC_FB				= (NET_MSG_GCTRL + 950),	//<	NPC�� ��� ��ġ�� �ִ��� �˻�.

	NET_MSG_GM_WHERE_PC_MAP				= (NET_MSG_GCTRL + 951),	//<	PC�� ��� �㿡 �ִ��� �˻�.
	NET_MSG_GM_WHERE_PC_MAP_FB			= (NET_MSG_GCTRL + 952),	//<	PC�� ��� �㿡 �ִ��� �˻� FB.

	NET_MSG_GM_WHERE_PC_POS				= (NET_MSG_GCTRL + 953),	//<	PC�� ��� ��ġ�� �ִ��� �˻�.
	NET_MSG_GM_WHERE_PC_POS_FB			= (NET_MSG_GCTRL + 954),	//<	PC�� ��� ��ġ�� �ִ��� �˻� FB.

	NET_MSG_GM_MOVE2CHAR				= (NET_MSG_GCTRL + 955),	//<	ĳ���͸� Ư��ĳ���ͷ� �̵� ��û.
	NET_MSG_GM_MOVE2CHAR_POS			= (NET_MSG_GCTRL + 956),	//<	ĳ���͸� Ư��ĳ���ͷ� �̵� ��ġ Ȯ��.
	NET_MSG_GM_MOVE2CHAR_AG				= (NET_MSG_GCTRL + 957),	//<	ĳ���͸� Ư��ĳ���ͷ� �̵� ��ġ Ȯ�� ����.
	NET_MSG_GM_MOVE2CHAR_FLD			= (NET_MSG_GCTRL + 958),	//<	ĳ���͸� Ư��ĳ���ͷ� �̵� ��û ����.
	NET_MSG_GM_MOVE2CHAR_FB				= (NET_MSG_GCTRL + 959),	//<	ĳ���͸� Ư��ĳ���ͷ� �̵� ���� ���.

	NET_MSG_GM_BIGHEAD					= (NET_MSG_GCTRL + 961),	//<	BIG HEAD ��� on/off.
	NET_MSG_GM_BIGHEAD_BRD				= (NET_MSG_GCTRL + 962),	//<	BIG HEAD ��� on/off.

	NET_MSG_GM_BIGHAND					= (NET_MSG_GCTRL + 963),	//<	BIG HAND ��� on/off.
	NET_MSG_GM_BIGHAND_BRD				= (NET_MSG_GCTRL + 964),	//<	BIG HAND ��� on/off.

	NET_MSG_GM_MOB_GEN					= (NET_MSG_GCTRL + 965),	//<	�� �߻�.
	NET_MSG_GM_MOB_GEN_FLD				= (NET_MSG_GCTRL + 966),	//<	�� �߻�. Agent->Field
	NET_MSG_GM_MOB_DEL					= (NET_MSG_GCTRL + 967),	//<	�� ����.
	NET_MSG_GM_MOB_DEL_FLD				= (NET_MSG_GCTRL + 968),	//<	�� ����. Agent->Field
	NET_MSG_GM_MOB_LEVEL				= (NET_MSG_GCTRL + 969),	//<	��(LEVEL FILE) �߻�.
	NET_MSG_GM_MOB_LEVEL_CLEAR			= (NET_MSG_GCTRL + 970),	//<	��(LEVEL FILE) �߻� ����.

	NET_MSG_GM_CHAT_BLOCK_UACCOUNT		= (NET_MSG_GCTRL + 971),	//<	ä�� ���� (�����̸�)
	NET_MSG_GM_CHAT_BLOCK_CHARNAME		= (NET_MSG_GCTRL + 972),	//<	ä�� ���� (ĳ���̸�)
	NET_MSG_GM_CHAT_BLOCK_CHARID		= (NET_MSG_GCTRL + 973),	//<	ä�� ���� (ĳ����ȣ)
	NET_MSG_GM_CHAT_BLOCK_FB			= (NET_MSG_GCTRL + 974),	//<	ä�� ���� FB
	NET_MSG_GM_CHAT_BLOCK_FLD			= (NET_MSG_GCTRL + 975),	//<	ä�� ���� FLD

	NET_MSG_USER_CHAR_INFO_4NAME		= (NET_MSG_GCTRL + 977),	//< ĳ������ ���� ��� (�������ɾ�)
	NET_MSG_USER_CHAR_INFO_AGT_FB		= (NET_MSG_GCTRL + 978),	//<	ĳ������ ���� ��� agt FB.
	NET_MSG_USER_CHAR_INFO_FLD_FB		= (NET_MSG_GCTRL + 979),	//<	ĳ������ ���� ��� fld FB.
	NET_MSG_USER_CHAR_INFO_FLD			= (NET_MSG_GCTRL + 980),	//<	ĳ������ ���� ��� FLD.

	NET_MSG_GM_CHAR_INFO_4NAME			= (NET_MSG_GCTRL + 981),	//<	ĳ������ ���� ��� (ĳ���̸�).
	NET_MSG_GM_CHAR_INFO_4CHARID		= (NET_MSG_GCTRL + 982),	//<	ĳ������ ���� ��� (ĳ����ȣ).
	NET_MSG_GM_CHAR_INFO_AGT_FB			= (NET_MSG_GCTRL + 983),	//<	ĳ������ ���� ��� agt FB.
	NET_MSG_GM_CHAR_INFO_FLD_FB			= (NET_MSG_GCTRL + 984),	//<	ĳ������ ���� ��� fld FB.
	NET_MSG_GM_CHAR_INFO_FLD			= (NET_MSG_GCTRL + 985),	//<	ĳ������ ���� ��� FLD.

   
//-----------------------------------------------------------------------------------------------------------

	NET_MSG_GM_FREEPK					  = (NET_MSG_GCTRL + 986),	//<	���� ������
	NET_MSG_GM_FREEPK_BRD				  = (NET_MSG_GCTRL + 987),	//<	���� ������

	NET_MSG_GM_WARNING_MSG				  = (NET_MSG_GCTRL + 989),	//<	���� ������
	NET_MSG_GM_WARNING_MSG_FLD			  = (NET_MSG_GCTRL + 990),	//<	���� ������
	NET_MSG_GM_WARNING_MSG_BRD			  = (NET_MSG_GCTRL + 991),	//<	���� ������

	NET_MSG_GM_VIEWALLPLAYER			  = (NET_MSG_GCTRL + 992),	//<  ���� ��� �÷��̾� ǥ��
	NET_MSG_GM_VIEWALLPLAYER_FLD_REQ	  = (NET_MSG_GCTRL + 993),	//<  �ʵ弭���� ��û
	NET_MSG_GM_VIEWALLPLAYER_FLD_FB 	  = (NET_MSG_GCTRL + 994),	//<  �ʵ弭�� ó����� ȸ��

	NET_MSG_GM_GENCHAR					  = (NET_MSG_GCTRL + 995),	//<  �ɸ��� ��ȯ ��û
	NET_MSG_GM_GENCHAR_POS				  = (NET_MSG_GCTRL + 996),	//<  �ɸ��� ��ȯ ��û
	NET_MSG_GM_GENCHAR_AG				  = (NET_MSG_GCTRL + 997),	//<  �ɸ��� ��ȯ ��û
	NET_MSG_GM_GENCHAR_FLD				  = (NET_MSG_GCTRL + 998),	//<  �ɸ��� ��ȯ ��û
	NET_MSG_GM_GENCHAR_FB				  = (NET_MSG_GCTRL + 999),	//<  �ɸ��� ��ȯ ��û

	NET_MSG_GCTRL_SERVER_BRIGHTEVENT_INFO = (NET_MSG_GCTRL + 1001),	// �����̺�Ʈ
	NET_MSG_GCTRL_SERVER_CLUB_BATTLE_INFO = (NET_MSG_GCTRL + 1002),	// ������ ���� ����
	NET_MSG_GCTRL_REQ_SERVER_CLUB_BATTLE_REMAIN_AG = (NET_MSG_GCTRL + 1003), // ������ �����ð� ��û(Field->Agent->Client)

	NET_MSG_GCTRL_UPDATE_LASTCALL		= (NET_MSG_GCTRL + 1010),	// ���� ��ȯ ��ġ Ŭ���̾�Ʈ�� ����
	NET_MSG_GCTRL_UPDATE_STARTCALL		= (NET_MSG_GCTRL + 1011),	// ���� ��ȯ ��ġ Ŭ���̾�Ʈ�� ����

	NET_MSG_GM_COUNTDOWN_MSG_BRD		= (NET_MSG_GCTRL + 1020),	// �ڵ����� ī��Ʈ �޼���
	NET_MSG_GM_SHOWMETHEMONEY			= (NET_MSG_GCTRL + 1021),	//<  ĳ���ͷ� �� ����
	NET_MSG_GM_SHOWMETHEMONEY_FLD		= (NET_MSG_GCTRL + 1022),	//<  ĳ���ͷ� �� ����

//-----------------------------------------------------------------------------------------------------------

	NET_MSG_REBUILD_RESULT				= (NET_MSG_GCTRL + 1025),	// ������ ���� - ����, ���� ���	// ITEMREBUILD_MARK
	NET_MSG_REBUILD_MOVE_ITEM			= (NET_MSG_GCTRL + 1026),	// ������ ���� - ������ �̵�
	NET_MSG_REBUILD_COST_MONEY			= (NET_MSG_GCTRL + 1027),	// ������ ���� - ���� ���
	NET_MSG_REBUILD_INPUT_MONEY			= (NET_MSG_GCTRL + 1028),	// ������ ���� - ���� �Է� �ݾ�

//-----------------------------------------------------------------------------------------------------------

	NET_MSG_PET_DROPPET					= (NET_MSG_GCTRL + 1030),	// ANYPET ����
	NET_MSG_CREATE_ANYPET				= (NET_MSG_GCTRL + 1031),	// ANYPET ����
	NET_MSG_PET_REQ_USECARD				= (NET_MSG_GCTRL + 1032),	// ��ī�� ���
	NET_MSG_PET_REQ_USECARD_FB			= (NET_MSG_GCTRL + 1033),
	NET_MSG_PET_REQ_UNUSECARD			= (NET_MSG_GCTRL + 1034),	// �� �Ҹ�
	NET_MSG_PET_REQ_UNUSECARD_FB		= (NET_MSG_GCTRL + 1035),
	NET_MSG_PET_REQ_DISAPPEAR			= (NET_MSG_GCTRL + 1036),	// ������ �Ҹ�(���̵���, ���������...)

	NET_MSG_CREATE_PET_FROMDB_FB		= (NET_MSG_GCTRL + 1037),	// �ֻ���(DB)
	NET_MSG_GET_PET_FROMDB_FB			= (NET_MSG_GCTRL + 1038),	// �ֻ���(DB)
	NET_MSG_GET_PET_FROMDB_ERROR		= (NET_MSG_GCTRL + 1039),	// �ֻ���(DB) ����

	NET_MSG_PET_REQ_UPDATE_MOVE_STATE	 = (NET_MSG_GCTRL + 1040),  // ������ ���� ����
	NET_MSG_PET_REQ_UPDATE_MOVE_STATE_FB = (NET_MSG_GCTRL + 1041),
	NET_MSG_PET_REQ_UPDATE_MOVE_STATE_BRD = (NET_MSG_GCTRL + 1042),
	
	NET_MSG_RETRIEVE_POINTS				= (NET_MSG_GCTRL + 1043),
	NET_MSG_RETRIEVE_POINTS_FB			= (NET_MSG_GCTRL + 1044),

	NET_MSG_PET_REQ_GOTO				= (NET_MSG_GCTRL + 1053),	// �̵�ó��
	NET_MSG_PET_REQ_STOP				= (NET_MSG_GCTRL + 1054),	// �̵�ó��
	NET_MSG_PET_REQ_GOTO_FB				= (NET_MSG_GCTRL + 1055),	// �̵�ó��
	NET_MSG_PET_GOTO_BRD				= (NET_MSG_GCTRL + 1056),	// �̵�ó��
	NET_MSG_PET_STOP_BRD				= (NET_MSG_GCTRL + 1057),	// �̵�ó��

	NET_MSG_PET_REQ_GIVEFOOD			= (NET_MSG_GCTRL + 1058),	// ����ֱ�
	NET_MSG_PET_REQ_GIVEFOOD_FB			= (NET_MSG_GCTRL + 1059),	// ����ֱ�
	NET_MSG_PET_REQ_GETFULL_FROMDB_FB	= (NET_MSG_GCTRL + 1060),	// ����ֱ�

	NET_MSG_PET_GETRIGHTOFITEM			= (NET_MSG_GCTRL + 1070),	// ���������� �ִ� ���������� ��û
	NET_MSG_PET_GETRIGHTOFITEM_FB		= (NET_MSG_GCTRL + 1071),	// ���������� �ִ� ���������� ��û
	NET_MSG_PET_NOTENOUGHINVEN			= (NET_MSG_GCTRL + 1072),	// �κ��丮�� �����Ͽ� ���� ������� ��

	NET_MSG_PET_REQ_RENAME				= (NET_MSG_GCTRL + 1075),	// �̸�����
	NET_MSG_PET_REQ_RENAME_FB			= (NET_MSG_GCTRL + 1076),	// �̸�����
	NET_MSG_PET_REQ_RENAME_BRD			= (NET_MSG_GCTRL + 1077),	// �̸�����
	NET_MSG_PET_REQ_RENAME_FROMDB_FB	= (NET_MSG_GCTRL + 1078),	// �̸�����

	NET_MSG_PET_REQ_CHANGE_COLOR		= (NET_MSG_GCTRL + 1080),	// �÷�����
	NET_MSG_PET_REQ_CHANGE_COLOR_FB		= (NET_MSG_GCTRL + 1081),	// �÷�����
	NET_MSG_PET_REQ_CHANGE_COLOR_BRD	= (NET_MSG_GCTRL + 1082),	// �÷�����

	NET_MSG_PET_REQ_CHANGE_STYLE		= (NET_MSG_GCTRL + 1085),	// ��Ÿ�Ϻ���
	NET_MSG_PET_REQ_CHANGE_STYLE_FB		= (NET_MSG_GCTRL + 1086),	// ��Ÿ�Ϻ���
	NET_MSG_PET_REQ_CHANGE_STYLE_BRD	= (NET_MSG_GCTRL + 1087),	// ��Ÿ�Ϻ���

	// Ŭ���̾�Ʈ ���� ���� (Field -> Client)
	NET_MSG_PET_UPDATECLIENT_FULL		= (NET_MSG_GCTRL + 1090),	// �������� ����

	NET_MSG_PET_REQ_SLOT_EX_HOLD		= (NET_MSG_GCTRL + 1100),	// ������ Ż��(SLOT<->HOLD)
	NET_MSG_PET_REQ_SLOT_EX_HOLD_FB		= (NET_MSG_GCTRL + 1101),	// ������ Ż��(SLOT<->HOLD)
	NET_MSG_PET_REQ_SLOT_EX_HOLD_BRD	= (NET_MSG_GCTRL + 1102),	// ������ Ż��(SLOT<->HOLD)
	NET_MSG_PET_REQ_SLOT_TO_HOLD		= (NET_MSG_GCTRL + 1103),	// ������ Ż��(SLOT->HOLD)
	NET_MSG_PET_REQ_SLOT_TO_HOLD_FB		= (NET_MSG_GCTRL + 1104),	// ������ Ż��(SLOT->HOLD)
	NET_MSG_PET_REQ_SLOT_TO_HOLD_BRD	= (NET_MSG_GCTRL + 1105),	// ������ Ż��(SLOT->HOLD)
	NET_MSG_PET_REQ_HOLD_TO_SLOT		= (NET_MSG_GCTRL + 1106),	// ������ Ż��(HOLD->SLOT)
	NET_MSG_PET_REQ_HOLD_TO_SLOT_FB		= (NET_MSG_GCTRL + 1107),	// ������ Ż��(HOLD->SLOT)
	NET_MSG_PET_REQ_HOLD_TO_SLOT_BRD	= (NET_MSG_GCTRL + 1108),	// ������ Ż��(HOLD->SLOT)

	NET_MSG_PET_REQ_LEARNSKILL			= (NET_MSG_GCTRL + 1110),	// �ֽ�ų ����
	NET_MSG_PET_REQ_LEARNSKILL_FB		= (NET_MSG_GCTRL + 1111),	// �ֽ�ų ����
	NET_MSG_PET_REQ_LEARNSKILL_BRD		= (NET_MSG_GCTRL + 1112),	// �ֽ�ų ����

	NET_MSG_PET_REQ_FUNNY				= (NET_MSG_GCTRL + 1115),	// Funny Gesture
	NET_MSG_PET_REQ_FUNNY_BRD			= (NET_MSG_GCTRL + 1116),	// Funny Gesture

	NET_MSG_PET_REMOVE_SLOTITEM			= (NET_MSG_GCTRL + 1118),	// �ֽ��Ծ����� ����
	NET_MSG_PET_REMOVE_SLOTITEM_FB		= (NET_MSG_GCTRL + 1119),	// �ֽ��Ծ����� ����
	NET_MSG_PET_REMOVE_SLOTITEM_BRD		= (NET_MSG_GCTRL + 1120),	// �ֽ��Ծ����� ����

	NET_MSG_PET_ATTACK					= (NET_MSG_GCTRL + 1125),	// ���ݸ��
	NET_MSG_PET_ATTACK_BRD				= (NET_MSG_GCTRL + 1126),	// ���ݸ��

	NET_MSG_PET_SAD						= (NET_MSG_GCTRL + 1127),	// Sad���
	NET_MSG_PET_SAD_BRD					= (NET_MSG_GCTRL + 1128),	// Sad���

	NET_MSG_PET_REQ_PETCARDINFO			= (NET_MSG_GCTRL + 1131),	// ��ī�� ����
	NET_MSG_PET_REQ_PETCARDINFO_FB		= (NET_MSG_GCTRL + 1132),	// ��ī�� ����

	NET_MSG_PET_REQ_PETREVIVEINFO		= (NET_MSG_GCTRL + 1133),	// �ֺ�Ȱ ����
	NET_MSG_PET_REQ_PETREVIVEINFO_FB	= (NET_MSG_GCTRL + 1134),	// �ֺ�Ȱ ����

	NET_MSG_PET_REQ_REVIVE				= (NET_MSG_GCTRL + 1135),	// ������ �� ��Ȱ
	NET_MSG_PET_REQ_REVIVE_FB			= (NET_MSG_GCTRL + 1136),	// ������ �� ��Ȱ
	NET_MSG_PET_REQ_REVIVE_FROMDB_FB	= (NET_MSG_GCTRL + 1137),	// ������ �� ��Ȱ

	NET_MSG_PET_ACCESSORY_DELETE		= (NET_MSG_GCTRL + 1140),	// �־Ǽ��縮 ����
	NET_MSG_PET_ACCESSORY_DELETE_BRD	= (NET_MSG_GCTRL + 1141),	// �־Ǽ��縮 ����

	NET_MSG_PET_REQ_SKILLCHANGE			= (NET_MSG_GCTRL + 1143),	// ��ų����
	NET_MSG_PET_REQ_SKILLCHANGE_FB		= (NET_MSG_GCTRL + 1144),	// ��ų����
	NET_MSG_PET_REQ_SKILLCHANGE_BRD		= (NET_MSG_GCTRL + 1145),	// ��ų����

	NET_MSG_PET_ADD_SKILLFACT			= (NET_MSG_GCTRL + 1146),	// SkillFact �߰�
	NET_MSG_PET_REMOVE_SKILLFACT		= (NET_MSG_GCTRL + 1147),	// SkillFact �߰�

	NET_MSG_PET_DUAL_SKILL				= (NET_MSG_GCTRL + 1148),
	NET_MSG_PET_DUAL_SKILL_FB			= (NET_MSG_GCTRL + 1149),
	NET_MSG_PET_DUAL_SKILL_BRD			= (NET_MSG_GCTRL + 1150),
	NET_MSG_PET_DUAL_SKILL_UPDATE_FB	= (NET_MSG_GCTRL + 1151),
	NET_MSG_SMS_PHONE_NUMBER			= (NET_MSG_GCTRL + 1200),	// ĳ���� ����ȣ ����/������Ʈ ��û
	NET_MSG_SMS_PHONE_NUMBER_FB			= (NET_MSG_GCTRL + 1201),	//<
	NET_MSG_SMS_PHONE_NUMBER_FROM_DB	= (NET_MSG_GCTRL + 1202),	//<	DB ���� ��� FB.
	NET_MSG_SMS_PHONE_NUMBER_AGTBRD		= (NET_MSG_GCTRL + 1203),	//<	Agent�� ���ä�� BRD ��û.

	NET_MSG_SMS_SEND					= (NET_MSG_GCTRL + 1204),
	NET_MSG_SMS_SEND_FB					= (NET_MSG_GCTRL + 1205),
	NET_MSG_SMS_SEND_FROM_DB			= (NET_MSG_GCTRL + 1206),

	NET_MSG_GM_MOB_GEN_EX				= (NET_MSG_GCTRL + 1210),   // ����ȯ(Ȯ�����߰�)
	NET_MSG_GM_MOB_GEN_EX_FLD			= (NET_MSG_GCTRL + 1211),   // ����ȯ(Ȯ�����߰�)
    NET_MSG_GM_MOB_DEL_EX				= (NET_MSG_GCTRL + 1212),   // ������(Ȯ�����߰�)
	NET_MSG_GM_MOB_DEL_EX_FLD			= (NET_MSG_GCTRL + 1213),   // ������(Ȯ�����߰�)

	NET_MSG_MGAME_ODDEVEN				= (NET_MSG_GCTRL + 1220),	// �̴� ���� - Ȧ¦(Ŭ���̾�Ʈ->�ʵ弭��)
	NET_MSG_MGAME_ODDEVEN_FB			= (NET_MSG_GCTRL + 1221),	// �̴� ���� - Ȧ¦(�ʵ弭��->Ŭ���̾�Ʈ)	

	NET_MSG_SERVERTIME_BRD				= (NET_MSG_GCTRL + 1230),	// ���� ����ð� BroadCasting

	NET_MSG_GCTRL_SET_SERVER_DELAYTIME     = (NET_MSG_GCTRL + 1240),	// ���� ���1
	NET_MSG_GCTRL_SET_SERVER_DELAYTIME_FB  = (NET_MSG_GCTRL + 1241),	// ���� ���1
	NET_MSG_GCTRL_SET_SERVER_DELAYTIME_AGT = (NET_MSG_GCTRL + 1242),	// ���� ���1
	NET_MSG_GCTRL_SET_SERVER_DELAYTIME_FLD = (NET_MSG_GCTRL + 1243),	// ���� ���1
	NET_MSG_GCTRL_SET_SERVER_SKIPPACKET    = (NET_MSG_GCTRL + 1244),	// ���� ���2
	NET_MSG_GCTRL_SET_SERVER_SKIPPACKET_FB = (NET_MSG_GCTRL + 1245),	// ���� ���2
	NET_MSG_GCTRL_SET_SERVER_SKIPPACKET_AGT = (NET_MSG_GCTRL + 1246),	// ���� ���2
	NET_MSG_GCTRL_SET_SERVER_SKIPPACKET_FLD = (NET_MSG_GCTRL + 1247),	// ���� ���2
	NET_MSG_GCTRL_SET_SERVER_CRASHTIME     = (NET_MSG_GCTRL + 1248),	// ���� ���3
	NET_MSG_GCTRL_SET_SERVER_CRASHTIME_FB  = (NET_MSG_GCTRL + 1249),	// ���� ���3
	NET_MSG_GCTRL_SET_SERVER_CRASHTIME_AGT = (NET_MSG_GCTRL + 1250),	// ���� ���3
	NET_MSG_GCTRL_SET_SERVER_CRASHTIME_FLD = (NET_MSG_GCTRL + 1251),	// ���� ���3

	NET_MSG_GCTRL_SERVER_LOGINMAINTENANCE_AGT = (NET_MSG_GCTRL + 1255),	//�α��μ��� MainTenance��� On/Off 

	NET_MSG_GCTRL_STORAGE_ITEM_UPDATE	= (NET_MSG_GCTRL + 1260 ),	//< ��Ŀ ������ ������Ʈ
	NET_MSG_REQ_FRIEND_CLUB_OPEN		= (NET_MSG_GCTRL + 1270 ),	//< ģ��,Ŭ��â ����(Ŭ���̾�Ʈ->������Ʈ)

	NET_MSG_GM_GETWHISPERMSG		    = (NET_MSG_GCTRL + 1402),	//<	ĳ������ �ӼӸ��� ����´�.
	NET_MSG_CHINA_GAINTYPE  		    = (NET_MSG_GCTRL + 1403),	//<	�߱� ��޺� ����

	NET_MSG_GM_LIMIT_EVENT_BEGIN		= (NET_MSG_GCTRL + 1404),	//<	�̺�Ʈ ����.
	NET_MSG_GM_LIMIT_EVENT_END			= (NET_MSG_GCTRL + 1405),	//<	�̺�Ʈ ����.

	NET_MSG_GM_LIMIT_EVENT_RESTART		= (NET_MSG_GCTRL + 1406),	//<	�̺�Ʈ �ٽ� ����.

	NET_MSG_GM_LIMIT_EVENT_APPLY_START	= (NET_MSG_GCTRL + 1407),	//<	Ŭ���̾�Ʈ�� �̺�Ʈ ���� ���������� �˸�
	NET_MSG_GM_LIMIT_EVENT_APPLY_END 	= (NET_MSG_GCTRL + 1408),	//<	Ŭ���̾�Ʈ�� �̺�Ʈ ���� �������� �˸�

	NET_MSG_GM_LIMIT_EVENT_BEGIN_FB		= (NET_MSG_GCTRL + 1409),   //<	Ŭ���̾�Ʈ�� �̺�Ʈ�� ���� �Ǿ����� �˸�
	NET_MSG_GM_LIMIT_EVENT_END_FB		= (NET_MSG_GCTRL + 1410),   //<	Ŭ���̾�Ʈ�� �̺�Ʈ�� ���� �Ǿ����� �˸�

	NET_MSG_GM_LIMIT_EVENT_TIME_REQ		= (NET_MSG_GCTRL + 1411),   //<	Ŭ���̾�Ʈ�� ������ �̺�Ʈ ������ ��û
	NET_MSG_GM_LIMIT_EVENT_TIME_REQ_FB	= (NET_MSG_GCTRL + 1412),   //<	������ Ŭ���̾�Ʈ�� �̺�Ʈ ������ ����
	NET_MSG_GM_LIMIT_EVENT_TIME_RESET	= (NET_MSG_GCTRL + 1413),   //<	AGENT������ FIELD�������� �α��� �ð��� RESET�϶�� ����

	NET_MSG_GM_PRINT_CROWLIST			= (NET_MSG_GCTRL + 1414),   //<	�ڽ��� �ִ� ���� �� ����Ʈ�� ����Ʈ��.
	NET_MSG_GM_PRINT_CROWLIST_FLD		= (NET_MSG_GCTRL + 1415),   //<	�ڽ��� �ִ� ���� �� ����Ʈ�� ����Ʈ��.
	NET_MSG_GM_PRINT_CROWLIST_FB		= (NET_MSG_GCTRL + 1416),   //<	�ڽ��� �ִ� ���� �� ����Ʈ�� ����Ʈ��.

	NET_MSG_SERVER_CTRL_WEATHER2		= (NET_MSG_GCTRL + 1417),	//<	������ ���� ����. ���ο� �޽���
	NET_MSG_GCTRL_MAPWEATHER			= (NET_MSG_GCTRL + 1418),	
	NET_MSG_GCTRL_MAPWHIMSICAL			= (NET_MSG_GCTRL + 1419),

	NET_MSG_GCTRL_ACTIVE_VEHICLE			= (NET_MSG_GCTRL + 1420),	//< Ŭ���̾�Ʈ�� Ż���� Ȱ��/��Ȱ�� ��û
	NET_MSG_GCTRL_ACTIVE_VEHICLE_FB			= (NET_MSG_GCTRL + 1421),	//< Ŭ���̾�Ʈ�� Ż���� Ȱ��/��Ȱ�� �˸�
	NET_MSG_GCTRL_ACTIVE_VEHICLE_BRD		= (NET_MSG_GCTRL + 1422),	//< ���� Ŭ���̾�Ʈ�� Ż���� Ȱ��/��Ȱ�� �˸�

	NET_MSG_GCTRL_GET_VEHICLE				= (NET_MSG_GCTRL + 1423),	//< Ŭ���̾�Ʈ�� Ż���� ���� ��û
	NET_MSG_GCTRL_GET_VEHICLE_FB			= (NET_MSG_GCTRL + 1424),	//< Ŭ���̾�Ʈ�� Ż���� ���� ����
	NET_MSG_GCTRL_GET_VEHICLE_BRD			= (NET_MSG_GCTRL + 1425),	//< ���� Ŭ���̾�Ʈ�� Ż�� ���� ����
	NET_MSG_GCTRL_UNGET_VEHICLE_FB			= (NET_MSG_GCTRL + 1426),	//< Ż�� ���� ��Ȱ��ȭ
	NET_MSG_GCTRL_UNGET_VEHICLE_BRD			= (NET_MSG_GCTRL + 1427),	//< Ż�� ���� ��Ȱ��ȭ

	NET_MSG_VEHICLE_CREATE_FROMDB_FB		= (NET_MSG_GCTRL + 1428),	//< ���� Ż�� ���� ( DB ) 
	NET_MSG_VEHICLE_GET_FROMDB_FB			= (NET_MSG_GCTRL + 1429),	//< Ż�� ���� ( DB )
	NET_MSG_VEHICLE_GET_FROMDB_ERROR		= (NET_MSG_GCTRL + 1430),	//< Ż�� ���� ( DB���� )
	
	NET_MSG_VEHICLE_ACCESSORY_DELETE		= (NET_MSG_GCTRL + 1431),	//< Ż���� �Ǽ����� ����
	NET_MSG_VEHICLE_ACCESSORY_DELETE_BRD	= (NET_MSG_GCTRL + 1432),	// Ż���� �Ǽ��縮 ���� �˸�
	
	NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD		= (NET_MSG_GCTRL + 1433 ),	// Ż�� slot ex hold
	NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_FB		= (NET_MSG_GCTRL + 1434 ),	// Ż�� slot ex hold
	NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_BRD	= (NET_MSG_GCTRL + 1435 ),	// Ż�� slot ex hold
	
	NET_MSG_VEHICLE_REQ_HOLD_TO_SLOT		= (NET_MSG_GCTRL + 1436 ),	// Ż�� hold to slot
	NET_MSG_VEHICLE_REQ_HOLD_TO_SLOT_FB		= (NET_MSG_GCTRL + 1437 ),	// Ż�� hold to slot
	
	NET_MSG_VEHICLE_REQ_SLOT_TO_HOLD		= (NET_MSG_GCTRL + 1438 ),	// Ż�� slot to hold
	NET_MSG_VEHICLE_REQ_SLOT_TO_HOLD_FB		= (NET_MSG_GCTRL + 1439 ),	// Ż�� slot to hold
	
	NET_MSG_VEHICLE_REMOVE_SLOTITEM			= (NET_MSG_GCTRL + 1440 ),	// item slot ����
	NET_MSG_VEHICLE_REMOVE_SLOTITEM_FB		= (NET_MSG_GCTRL + 1441 ),	// item slot ����
	NET_MSG_VEHICLE_REMOVE_SLOTITEM_BRD		= (NET_MSG_GCTRL + 1442 ),	// item slot ����

	NET_MSG_VEHICLE_REQ_GIVE_BATTERY		= (NET_MSG_GCTRL + 1443 ),	// ���͸� ����
	NET_MSG_VEHICLE_REQ_GIVE_BATTERY_FB		= (NET_MSG_GCTRL + 1444 ),	// ���͸� ����
	NET_MSG_VEHICLE_REQ_GETFULL_FROMDB_FB	= (NET_MSG_GCTRL + 1445 ),	// ���͸� ����
	NET_MSG_VEHICLE_UPDATE_CLIENT_BATTERY	= (NET_MSG_GCTRL + 1446 ),	// ���͸� ����
	NET_MSG_VEHICLE_REQ_GET_BATTERY_FROMDB_FB	= (NET_MSG_GCTRL + 1447 ),	// ���͸� �������� ( ���� )

	NET_MSG_VEHICLE_REQ_ITEM_INFO			= (NET_MSG_GCTRL + 1448),	// Ż�;����� ����
	NET_MSG_VEHICLE_REQ_ITEM_INFO_FB		= (NET_MSG_GCTRL + 1449),	// Ż�;����� ����

	NET_MSG_VIETNAM_GAINTYPE				= (NET_MSG_GCTRL + 1450),	// ��Ʈ�� Ž�� ���� �ý��� ���� ���� Ÿ��
	NET_MSG_VIETNAM_TIME_REQ				= (NET_MSG_GCTRL + 1451),	// ��Ʈ�� Ž�� ���� �ý��� �ð� ��û
	NET_MSG_VIETNAM_TIME_REQ_FB				= (NET_MSG_GCTRL + 1452),	// ��Ʈ�� Ž�� ���� �ý��� �ð� ��û
	NET_MSG_VIETNAM_GAIN_EXP				= (NET_MSG_GCTRL + 1453),	// ��Ʈ�� Ž�� ���� �ý��� ����ġ
	NET_MSG_VIETNAM_GAIN_MONEY				= (NET_MSG_GCTRL + 1454),	// ��Ʈ�� Ž�� ���� �ý��� ��
	NET_MSG_GCTRL_REQ_VNGAIN_TO_HOLD		= (NET_MSG_GCTRL + 1455),   //<	Ž�й��� �ι꿡 �ִ� �������� ��� �õ�.
	NET_MSG_GCTRL_REQ_VNGAIN_EX_HOLD		= (NET_MSG_GCTRL + 1456),	//<	Ž�й��� �ι꿡 �ִ� �������� ��� �õ�.
	NET_MSG_GCTRL_REQ_HOLD_TO_VNGAIN		= (NET_MSG_GCTRL + 1457),	//<	�������� Ž�й��� �ι꿡 ����
	NET_MSG_GCTRL_REQ_VNGAIN_INVEN_RESET	= (NET_MSG_GCTRL + 1458),	//< Ž�� ���� �ι� ��ü �ʱ�ȭ
	NET_MSG_GCTRL_REQ_VNINVEN_TO_INVEN      = (NET_MSG_GCTRL + 1459),	//< ������ ��ư���� Ž�� ���� �ι꿡�� �ι����� ������ �ű涧
	NET_MSG_GCTRL_REQ_VNINVEN_TO_INVEN_FB   = (NET_MSG_GCTRL + 1460),	//< ������ ��ư���� Ž�� ���� �ι꿡�� �ι����� ������ �ű涧
	NET_MSG_GCTRL_INVEN_VIETNAM_INVENGET	= (NET_MSG_GCTRL + 1461),	//<	��Ʈ�� Ž�� ���� ������ ���� �޽���
	NET_MSG_GCTRL_INVEN_VIETNAM_EXPGET_FB	= (NET_MSG_GCTRL + 1462),	//<	��Ʈ�� Ž�� ���� ������ ���� �޽���
	NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGET_FB	= (NET_MSG_GCTRL + 1463),	//<	��Ʈ�� Ž�� ���� ������ ���� �޽���
	NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGETNUM_UPDATE = (NET_MSG_GCTRL + 1464),	//<	��Ʈ�� Ž�� ���� ������ ���� �޽��� 
	NET_MSG_VIETNAM_ALLINITTIME				= (NET_MSG_GCTRL + 1465),	//<	��Ʈ�� Ž�� �����ý��� 12�ð� ������ �ʱ�ȭ �Ǿ�����
	
	NET_MSG_GCTRL_INVEN_GENDER_CHANGE		= (NET_MSG_GCTRL + 1466),	// ���� ����
	NET_MSG_GCTRL_INVEN_GENDER_CHANGE_FB	= (NET_MSG_GCTRL + 1467),	// ���� ����

	NET_MSG_MARKET_CHECKTIME				= (NET_MSG_GCTRL + 1468),	// ������ �������� �� 10�и��� �޽����� ���� ƨ�� ������ �����Ѵ�.

	NET_MSG_CHAT_CTRL_FB					= (NET_MSG_GCTRL + 1469), //< �����ڿ� ä�ø޽��� ���� ( ���Ӽ���->Ŭ���̾�Ʈ )

	NET_NON_REBIRTH_REQ						= (NET_MSG_GCTRL + 1470), //< ��Ȱ ��ų ���� ��� ����

	NET_MSG_REQ_SKILL_REVIVEL_FAILED		= (NET_MSG_GCTRL + 1471), //< ��Ȱ ��ų ���� �Ǿ��� ��� ����ڿ��� �޽����� ������.

	NET_QBOX_OPTION_REQ_AG					= (NET_MSG_GCTRL + 1472), //< QBox On/Off �ɼ� ������ -> ������Ʈ ����
	NET_QBOX_OPTION_REQ_FLD					= (NET_MSG_GCTRL + 1473), //< QBox On/Off �ɼ� ������Ʈ ���� -> �ʵ弭��
	NET_QBOX_OPTION_MEMBER					= (NET_MSG_GCTRL + 1474), //< QBox On/Off �ɼ� �ʵ� ���� -> �������..

	NET_MSG_UPDATE_TRACING_ALL				= (NET_MSG_GCTRL + 1475), //< ������ ĳ���͸� ���� ������Ʈ�Ѵ�.
	NET_MSG_UPDATE_TRACING_CHARACTER		= (NET_MSG_GCTRL + 1476), //< ������ ĳ���͸� Ư�� ĳ���͸� ������Ʈ�Ѵ�.
	NET_MSG_LOG_UPDATE_TRACING_CHARACTER	= (NET_MSG_GCTRL + 1477), //< // �������� ������ �α׸� agent������ ������.

	NET_MSG_TRACING_CHARACTER				= (NET_MSG_GCTRL + 1478), //< �ʵ�->Ŭ���̾�Ʈ ���� �������� �ƴ����� ������.

	NET_MSG_GCTRL_REQ_TELEPORT				= (NET_MSG_GCTRL + 1479),	//<	��ȯ�� ������ ��� ��û.
	NET_MSG_GCTRL_REQ_TELEPORT_FB			= (NET_MSG_GCTRL + 1480),	//<	��ȯ�� ������ ��� ����.
	NET_MSG_GCTRL_REQ_TELEPORT_AG			= (NET_MSG_GCTRL + 1481),	//<	��ȯ�� ������Ʈ ������ ��û.

	NET_MSG_GCTRL_GETEXP_RECOVERY			= (NET_MSG_GCTRL + 1482),	// ����ġ ȸ�� ������ ��û
	NET_MSG_GCTRL_GETEXP_RECOVERY_FB		= (NET_MSG_GCTRL + 1483),	// ����ġ ȸ�� ������ ��û FB
	NET_MSG_GCTRL_RECOVERY					= (NET_MSG_GCTRL + 1484),	// ����ġ ȸ�� ���
	NET_MSG_GCTRL_RECOVERY_FB				= (NET_MSG_GCTRL + 1485),	// ����ġ ȸ�� ��� FB
	NET_MSG_GCTRL_GETEXP_RECOVERY_NPC		= (NET_MSG_GCTRL + 1486),	// ����ġ ȸ�� ������ ��û (NPC)
	NET_MSG_GCTRL_GETEXP_RECOVERY_NPC_FB	= (NET_MSG_GCTRL + 1487),	// ����ġ ȸ�� ������ ��û FB (NPC)
	NET_MSG_GCTRL_RECOVERY_NPC				= (NET_MSG_GCTRL + 1488),	// ����ġ ȸ�� ��� (NPC)
	NET_MSG_GCTRL_RECOVERY_NPC_FB			= (NET_MSG_GCTRL + 1489),	// ����ġ ȸ�� ��� FB (NPC)
	NET_MSG_GCTRL_QUEST_PROG_LEAVEMAP		= (NET_MSG_GCTRL + 1490),	//<	����Ʈ ���̵� ����.
	NET_MSG_GM_CLASS_EVENT					= (NET_MSG_GCTRL + 1491),	//<	Ŭ������ �̺�Ʈ

	NET_MSG_VIETNAM_TIME_RESET				= (NET_MSG_GCTRL + 1492),	// ��Ʈ�� �ð� ���� //vietnamtest%%% && vietnamtest2

	NET_MSG_GCTRL_ITEMSHOPOPEN				= (NET_MSG_GCTRL + 1493),	//<	ItemShopOpen ( Japan ) 
	NET_MSG_GCTRL_ITEMSHOPOPEN_BRD			= (NET_MSG_GCTRL + 1494),	//<	ItemShopOpen ( Japan ) 

	NET_MSG_GM_VIEWWORKEVENT			    = (NET_MSG_GCTRL + 1495),	//< ���� agent������ ����� ��� �̺�Ʈ ����
	NET_MSG_GM_VIEWWORKEVENT_FB			    = (NET_MSG_GCTRL + 1496),	//< ���� agent������ ����� ��� �̺�Ʈ ���� ȸ��

	NET_MSG_PET_PETSKINPACKOPEN				= (NET_MSG_GCTRL + 1497),	//< �� ��Ų �� ������ ���
	NET_MSG_PET_PETSKINPACKOPEN_FB			= (NET_MSG_GCTRL + 1498),	//< �� ��Ų �� ������ ���
	NET_MSG_PET_PETSKINPACKOPEN_BRD			= (NET_MSG_GCTRL + 1499),	//< �� ��Ų �� ������ ���

	NET_MSG_GCTRL_PMARKET_SEARCH_ITEM		= (NET_MSG_GCTRL + 1500),	//< ���λ����� ������ �˻�
	NET_MSG_GCTRL_PMARKET_SEARCH_ITEM_RESULT = (NET_MSG_GCTRL + 1501),	//< ���λ����� ������ �˻� ���
	NET_MSG_GCTRL_PMARKET_SEARCH_ITEM_RESULT_REQ = (NET_MSG_GCTRL + 1502),	//< ���λ����� ������ �˻� ��� ��û
	
	NET_MSG_GCTRL_DEFENSE_SKILL_ACTIVE		= (NET_MSG_GCTRL + 1503),	//< �ǰݽ� ��ų �ߵ�
	
	NET_MSG_REQ_ATTENDLIST					= (NET_MSG_GCTRL + 1504),	//< �⼮���� ����Ʈ ��û
	NET_MSG_REQ_ATTENDLIST_FB				= (NET_MSG_GCTRL + 1505),	//< �⼮���� ����Ʈ ���
	NET_MSG_REQ_ATTENDANCE					= (NET_MSG_GCTRL + 1506),	//< �⼮ üũ
	NET_MSG_REQ_ATTENDANCE_FB				= (NET_MSG_GCTRL + 1507),	//< �⼮ üũ �亯
	NET_MSG_REQ_ATTEND_REWARD_FLD			= (NET_MSG_GCTRL + 1508),	//< �⼮ üũ �̺�Ʈ ������ ���� ( �ʵ�� )
	NET_MSG_REQ_ATTEND_REWARD_CLT			= (NET_MSG_GCTRL + 1509),	//< �⼮ üũ �̺�Ʈ ������ ���� ( Ŭ��� )

	NET_MSG_REQ_USE_SUMMON					= (NET_MSG_GCTRL + 1510),	//< ��ȯ�� ��û
	NET_MSG_REQ_USE_SUMMON_FB				= (NET_MSG_GCTRL + 1511),	//< ��ȯ�� ��û
	NET_MSG_REQ_USE_SUMMON_DEL				= (NET_MSG_GCTRL + 1512),	//< ��ȯ���� ����
	NET_MSG_SUMMON_REQ_GOTO					= (NET_MSG_GCTRL + 1513),	//< ��ȯ�� �̵�
	NET_MSG_SUMMON_GOTO_BRD					= (NET_MSG_GCTRL + 1514),	//< ��ȯ�� �̵�	
	NET_MSG_SUMMON_REQ_STOP					= (NET_MSG_GCTRL + 1515),	//< ��ȯ�� ����
	NET_MSG_SUMMON_REQ_STOP_BRD				= (NET_MSG_GCTRL + 1516),	//< ��ȯ�� ����
	NET_MSG_SUMMON_REQ_UPDATE_MOVE_STATE_BRD = (NET_MSG_GCTRL + 1517),	//< ��ȯ�� �̵����� ������Ʈ
	NET_MSG_SUMMON_REQ_UPDATE_MOVE_STATE	= (NET_MSG_GCTRL + 1518),	//< ��ȯ�� �̵����� ������Ʈ
	NET_MSG_CREATE_ANYSUMMON				= (NET_MSG_GCTRL + 1519),	//< �ٸ� Ŭ���� ��ȯ�� ����
	NET_MSG_DROP_ANYSUMMON					= (NET_MSG_GCTRL + 1520),	//< �ٸ� Ŭ���� ��ȯ�� ����
	NET_MSG_SUMMON_ATTACK					= (NET_MSG_GCTRL + 1521),	//< ��ȯ�� ����
	NET_MSG_SUMMON_ATTACK_BRD				= (NET_MSG_GCTRL + 1522),	//< ��ȯ�� ����
	NET_MSG_GCTRL_SUMMON_ATTACK_AVOID		= (NET_MSG_GCTRL + 1523),	//< ��ȯ�� ���� ȸ��
	NET_MSG_GCTRL_SUMMON_ATTACK_DAMAGE		= (NET_MSG_GCTRL + 1524),	//< ��ȯ�� ���� ������
	NET_MSG_SUMMON_REQ_SLOT_EX_HOLD			= (NET_MSG_GCTRL + 1525),	//< ��ȯ�� ����, ���Կ��� ������.
	NET_MSG_SUMMON_REQ_SLOT_EX_HOLD_FB		= (NET_MSG_GCTRL + 1526),	//< ��ȯ�� ����, ���Կ��� ������.
	NET_MSG_SUMMON_REQ_HOLD_TO_SLOT			= (NET_MSG_GCTRL + 1527),	//< ��ȯ�� ����, �տ��� ��������.
	NET_MSG_SUMMON_REQ_HOLD_TO_SLOT_FB		= (NET_MSG_GCTRL + 1528),	//< ��ȯ�� ����, �տ��� ��������.
	NET_MSG_SUMMON_REMOVE_SLOTITEM			= (NET_MSG_GCTRL + 1529),	//< ��ȯ�� ���� ����
	NET_MSG_SUMMON_REMOVE_SLOTITEM_FB		= (NET_MSG_GCTRL + 1530),	//< ��ȯ�� ���� ����
	NET_MSG_SUMMON_UPDATE_HP				= (NET_MSG_GCTRL + 1531),	//< ��ȯ�� HP ������Ʈ(ȸ��)
	NET_MSG_SUMMON_UPDATE_HP_BRD			= (NET_MSG_GCTRL + 1532),	//< ��ȯ�� HP ������Ʈ(ȸ��)

	NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE	= (NET_MSG_GCTRL + 1541),	//< �� ��Ÿ�� ���� ��û
	NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE_FB	= (NET_MSG_GCTRL + 1542),	//< �� ��Ÿ�� ���� ���
	NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE_BRD= (NET_MSG_GCTRL + 1543),	//< �� ��Ÿ�� ���� ���

	NET_MSG_REQ_MUST_LEAVE_MAP		= (NET_MSG_GCTRL + 1544),	//< �������� ������ �����ʾ� ���� ������ �� ��
	NET_MSG_REQ_MUST_LEAVE_MAP_FB	= (NET_MSG_GCTRL + 1545),	//< �������� ������ �����ʾ� ���� ������ �� ��
	NET_MSG_REQ_MUST_LEAVE_MAP_AG	= (NET_MSG_GCTRL + 1546),	//< �������� ������ �����ʾ� ���� ������ �� ��

	NET_MSG_CYBERCAFECLASS_UPDATE			= (NET_MSG_GCTRL + 1547),	//< �±� ���̹� ī�� ��� ������Ʈ
	
	NET_MSG_GCTRL_REQ_TAXI					= (NET_MSG_GCTRL + 1548),	//< �ý� ž�� ��û
	NET_MSG_GCTRL_REQ_TAXI_FB				= (NET_MSG_GCTRL + 1549),	//< �ý� ž�� ��û ���
	NET_MSG_GCTRL_REQ_TAXI_NPCPOS			= (NET_MSG_GCTRL + 1550),	//< �ý� ž�½� NPC��ǥ ��û
	NET_MSG_GCTRL_REQ_TAXI_NPCPOS_FB		= (NET_MSG_GCTRL + 1551),	//< �ý� ž�½� NPC��ǥ ��û ���
	
	NET_MSG_GCTRL_ETNRY_FAILED				= (NET_MSG_GCTRL + 1552),	//< ���� ���н� �޽���
	
	NET_MSG_GCTRL_ITEM_COOLTIME_UPDATE		= (NET_MSG_GCTRL + 1553),	//< ��Ÿ�� ������Ʈ
	NET_MSG_GCTRL_ITEM_COOLTIME_ERROR		= (NET_MSG_GCTRL + 1554),	//< ��Ÿ�ӿ� �ɷ��� ������

	NET_MSG_GCTRL_NPC_RECALL				= (NET_MSG_GCTRL + 1555),	//< NPC ȣ��.
	NET_MSG_GCTRL_NPC_RECALL_FB				= (NET_MSG_GCTRL + 1556),	//< NPC ȣ�� ���.

	NET_MSG_GCTRL_NPC_COMMISSION			= (NET_MSG_GCTRL + 1557),	//< NPC ȣ�� COMMISION ������Ʈ ��û ( Field->Agent ) 
	NET_MSG_GCTRL_NPC_COMMISSION_FB			= (NET_MSG_GCTRL + 1558),	//< NPC ȣ�� COMMISION ��� ( Agent -> Field ) 

	NET_MSG_GCTRL_CREATE_INSTANT_MAP_FLD	= (NET_MSG_GCTRL + 1559),	//< �ν��Ͻ� ���� ���� AGENT -> FIELD
	NET_MSG_GCTRL_CREATE_INSTANT_MAP_FB		= (NET_MSG_GCTRL + 1560),	//< �ν��Ͻ� ���� ���� ��û �ǵ��
	NET_MSG_GCTRL_CREATE_INSTANT_MAP_REQ	= (NET_MSG_GCTRL + 1561),	//< �ν��Ͻ� ���� ��û CLIENT -> FIELD -> AGENT
	NET_MSG_GCTRL_CREATE_INSTANT_MAP_DEL	= (NET_MSG_GCTRL + 1562),	//< �ν��Ͻ� ���� ���� 

	NET_MSG_GM_SHOP_INFO_REQ				= (NET_MSG_GCTRL + 1563),	//< ������ ���� ��û(GM���ɾ�)
	NET_MSG_GM_SHOP_INFO_FB					= (NET_MSG_GCTRL + 1564),	//< ������ ���� ����(GM���ɾ�)

	NET_MSG_GCTRL_INVEN_ITEM_MIX			= (NET_MSG_GCTRL + 1565),	//< ������ ���� ��û
	NET_MSG_GCTRL_INVEN_ITEM_MIX_FB			= (NET_MSG_GCTRL + 1566),	//< ������ ���� ��û ���

	NET_MSG_GCTRL_LIMITTIME_OVER			= (NET_MSG_GCTRL + 1567),	//< ���� ���н� �޽���

	NET_MSG_GCTRL_SERVER_LOGINMAINTENANCE_SESSION	= (NET_MSG_GCTRL + 1568),	// �������� LoginMaintenance ���� On/Off
	NET_MSG_GCTRL_SERVER_LOGINMAINTENANCE_LOGIN		= (NET_MSG_GCTRL + 1569),	// �α������� LoginMaintenance ���� On/Off
	
	NET_MSG_REQ_GATHERING					= (NET_MSG_GCTRL + 1570),	//< ä�� ��û
	NET_MSG_REQ_GATHERING_FB				= (NET_MSG_GCTRL + 1571),	//< ä�� ��û ���
	NET_MSG_REQ_GATHERING_BRD				= (NET_MSG_GCTRL + 1572),	//< ä�� ��û ���
	NET_MSG_REQ_GATHERING_RESULT			= (NET_MSG_GCTRL + 1573),	//< ä�� ���
	NET_MSG_REQ_GATHERING_RESULT_BRD		= (NET_MSG_GCTRL + 1574),	//< ä�� ���
	NET_MSG_REQ_GATHERING_CANCEL			= (NET_MSG_GCTRL + 1575),	//< ä���� ����
	NET_MSG_REQ_GATHERING_CANCEL_BRD		= (NET_MSG_GCTRL + 1576),	//< ä���� ����

	NET_MSG_GCTRL_CLUB_AUTHORITY_REQ		= (NET_MSG_GCTRL + 1577),	//<	Ŭ�������� ���� ����
	NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_ASK	= (NET_MSG_GCTRL + 1578),	//<	Ŭ�������� ���� ���� ����
	NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_ANS	= (NET_MSG_GCTRL + 1579),	//<	Ŭ�������� ���� ���� �亯
	NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_FB		= (NET_MSG_GCTRL + 1580),	//<	Ŭ�������� ���� ���� ���
	NET_MSG_GCTRL_CLUB_AUTHORITY_CLT		= (NET_MSG_GCTRL + 1581),	//<	Ŭ�������� ���� ����
	NET_MSG_GCTRL_CLUB_AUTHORITY_BRD		= (NET_MSG_GCTRL + 1582),	//<	Ŭ�������� ���� ����
	NET_MSG_GCTRL_CLUB_AUTHORITY_FLD		= (NET_MSG_GCTRL + 1583),	//<	Ŭ�������� ���� ���� (�ʵ�)

	NET_MSG_GCTRL_CLUB_DEATHMATCH_REMAIN_BRD	= (NET_MSG_GCTRL + 1584),	//<	Ŭ��������ġ �����ð�.
	NET_MSG_GCTRL_CLUB_DEATHMATCH_READY_FLD		= (NET_MSG_GCTRL + 1585),	//<	Ŭ��������ġ �غ�.
	NET_MSG_GCTRL_CLUB_DEATHMATCH_START_FLD		= (NET_MSG_GCTRL + 1586),	//<	Ŭ��������ġ ����.
	NET_MSG_GCTRL_CLUB_DEATHMATCH_START_BRD		= (NET_MSG_GCTRL + 1587),	//<	Ŭ��������ġ ����.

	NET_MSG_GCTRL_CLUB_DEATHMATCH_END_FLD		= (NET_MSG_GCTRL + 1588),	//<	Ŭ��������ġ ����.
	NET_MSG_GCTRL_CLUB_DEATHMATCH_END_BRD		= (NET_MSG_GCTRL + 1589),	//<	Ŭ��������ġ ����.

	NET_MSG_GCTRL_SERVER_CLUB_DEATHMATCH_INFO	= (NET_MSG_GCTRL + 1590),	//<	Ŭ��������ġ ���� ���� ( ���ӽ� ) 
	NET_MSG_GCTRL_REQ_SERVER_CLUB_DEATHMATCH_REMAIN_AG	= (NET_MSG_GCTRL + 1591),	//<	Ŭ��������ġ ���� ���� ( Agent�� �����ð� ��û )

	NET_MSG_GCTRL_CLUB_DEATHMATCH_POINT_UPDATE		= (NET_MSG_GCTRL + 1592),	//<	Ŭ��������ġ Kill/Death ��Ȳ ( �ڱ⸸ )
	NET_MSG_GCTRL_CLUB_DEATHMATCH_MYRANK_UPDATE		= (NET_MSG_GCTRL + 1593),	//<	Ŭ��������ġ Kill/Death ��Ȳ ( �ڱ⸸ )
	NET_MSG_GCTRL_CLUB_DEATHMATCH_RANKING_UPDATE	= (NET_MSG_GCTRL + 1594),	//<	Ŭ��������ġ ��ŷ ��Ȳ
	NET_MSG_GCTRL_CLUB_DEATHMATCH_RANKING_REQ		= (NET_MSG_GCTRL + 1595),	//<	Ŭ��������ġ ��Ȳ ��û

	//add pk
	NET_MSG_GCTRL_UPDATE_PKWIN						= (NET_MSG_GCTRL + 1596),
	NET_MSG_GCTRL_UPDATE_PKLOSS						= (NET_MSG_GCTRL + 1597),
	NET_MSG_GCTRL_UPDATE_PKWIN_BRD					= (NET_MSG_GCTRL + 1598),
	NET_MSG_GCTRL_UPDATE_PKLOSS_BRD					= (NET_MSG_GCTRL + 1599),

	NET_MSG_GCTRL_QITEMFACT_TIMEUPDATE_BRD			= (NET_MSG_GCTRL + 1600),	//add qbox
	NET_MSG_GCTRL_UPDATE_CP							= (NET_MSG_GCTRL + 1601),	//add cp

	NET_MSG_GCTRL_REQ_RANKNAME						= (NET_MSG_GCTRL + 1602), //add pkrank
	NET_MSG_GCTRL_REQ_RANKMARK						= (NET_MSG_GCTRL + 1603), //add pkrank
	NET_MSG_GCTRL_REQ_RANKNAME_FB					= (NET_MSG_GCTRL + 1604), //add pkrank
	NET_MSG_GCTRL_REQ_RANKMARK_FB					= (NET_MSG_GCTRL + 1605), //add pkrank
	NET_MSG_GCTRL_REQ_RANKNAME_BRD					= (NET_MSG_GCTRL + 1606), //add pkrank
	NET_MSG_GCTRL_REQ_RANKMARK_BRD					= (NET_MSG_GCTRL + 1607), //add pkrank

	NET_MSG_GCTRL_CANNOT_POT						= (NET_MSG_GCTRL + 1608),	//add antipot
	NET_MSG_GCTRL_REQ_ALLOW_BET						= (NET_MSG_GCTRL + 1609),	//add duel bet
	NET_MSG_GCTRL_REQ_ALLOW_BET_FB					= (NET_MSG_GCTRL + 1610),	//add duel bet

	NET_MSG_GCTRL_UPDATE_DUELWIN					= (NET_MSG_GCTRL + 1611), //add duel bet
	NET_MSG_GCTRL_UPDATE_DUELLOSS					= (NET_MSG_GCTRL + 1612), //add duel bet

	NET_MSG_GCTRL_INVEN_BIKECOLOR_CHANGE			= (NET_MSG_GCTRL + 1613),	//add bike color
	NET_MSG_GCTRL_INVEN_BIKECOLOR_CHANGE_FB			= (NET_MSG_GCTRL + 1614),	//add bike color
	NET_MSG_GCTRL_INVEN_BIKECOLOR_CHANGE_BRD		= (NET_MSG_GCTRL + 1615),	//add bike color

	NET_MSG_GCTRL_INVEN_GMITEM						= (NET_MSG_GCTRL + 1616),	//add itemcmd
	NET_MSG_GCTRL_INVEN_GMITEM_FB					= (NET_MSG_GCTRL + 1617),	//add itemcmd

	NET_MSG_GCTRL_SLOT_ITEMCOLOR_CHANGE				= (NET_MSG_GCTRL + 1618),	//add itemcolor
	NET_MSG_GCTRL_SLOT_ITEMCOLOR_CHANGE_FB			= (NET_MSG_GCTRL + 1619),	//add bike color
	NET_MSG_GCTRL_SLOT_ITEMCOLOR_CHANGE_BRD			= (NET_MSG_GCTRL + 1620),	//add bike color

	NET_MSG_GCTRL_REQ_STATSUPCMD					= (NET_MSG_GCTRL + 1621),	//add addstats cmd
	NET_MSG_GCTRL_REQ_STATSUPCMD_FB					= (NET_MSG_GCTRL + 1622),	//add addstats cmd
	NET_MSG_GCTRL_SCHOOL_WARS_REMAIN_BRD	= (NET_MSG_GCTRL + 1625),
	NET_MSG_GCTRL_SCHOOL_WARS_READY_FLD		= (NET_MSG_GCTRL + 1626),
	NET_MSG_GCTRL_SCHOOL_WARS_START_FLD		= (NET_MSG_GCTRL + 1627),
	NET_MSG_GCTRL_SCHOOL_WARS_START_BRD		= (NET_MSG_GCTRL + 1628),

	NET_MSG_GCTRL_SCHOOL_WARS_END_FLD		= (NET_MSG_GCTRL + 1629),
	NET_MSG_GCTRL_SCHOOL_WARS_END_BRD		= (NET_MSG_GCTRL + 1630),

	NET_MSG_GCTRL_SERVER_SCHOOL_WARS_INFO	= (NET_MSG_GCTRL + 1631),
	NET_MSG_GCTRL_REQ_SERVER_SCHOOL_WARS_REMAIN_AG	= (NET_MSG_GCTRL + 1632),	

	NET_MSG_GCTRL_SCHOOL_WARS_POINT_UPDATE		= (NET_MSG_GCTRL + 1633),
	NET_MSG_GCTRL_SCHOOL_WARS_MYRANK_UPDATE		= (NET_MSG_GCTRL + 1634),
	NET_MSG_GCTRL_SCHOOL_WARS_RANKING_UPDATE	= (NET_MSG_GCTRL + 1635),
	NET_MSG_GCTRL_SCHOOL_WARS_RANKING_REQ		= (NET_MSG_GCTRL + 1636),

	NET_MSG_GCTRL_SCHOOL_WARS_PPOINT_UPDATE			= (NET_MSG_GCTRL + 1637),
	NET_MSG_GCTRL_SCHOOL_WARS_MYPRANK_UPDATE		= (NET_MSG_GCTRL + 1638),
	NET_MSG_GCTRL_SCHOOL_WARS_PRANKING_UPDATE		= (NET_MSG_GCTRL + 1639),
	NET_MSG_GCTRL_SCHOOL_WARS_PRANKING_REQ			= (NET_MSG_GCTRL + 1640),

	//add sw buff
	NET_MSG_GM_SCHOOL_EVENT							= (NET_MSG_GCTRL + 1641),

	NET_MSG_GCTRL_SCHOOL_WARS_CERTIFIED_AGT	= (NET_MSG_GCTRL + 1642),	
	NET_MSG_GCTRL_SCHOOL_WARS_CERTIFIED_FLD	= (NET_MSG_GCTRL + 1643),	

	NET_MSG_GCTRL_SCHOOL_WARS_SETWINSCHOOL_BRD		= (NET_MSG_GCTRL + 1644),

	NET_MSG_GCTRL_VIEW_PROPERTIES				= (NET_MSG_GCTRL + 1645),//add charinfoview
	NET_MSG_GCTRL_VIEW_PROPERTIES_FB				= (NET_MSG_GCTRL + 1646),//add charinfoview
	NET_MSG_GCTRL_VIEW_PROPERTIES_TAR				= (NET_MSG_GCTRL + 1647),//add charinfoview
	
	NET_MSG_GCTRL_VIEWCHAROTEM					= (NET_MSG_GCTRL + 1648),
	NET_MSG_GCTRL_VIEWCHAROTEM_TAR				= (NET_MSG_GCTRL + 1649),
	NET_MSG_GCTRL_VIEWCHAROTEM_SLOT_TAR				= (NET_MSG_GCTRL + 1650),
	NET_MSG_GCTRL_REQ_ALLOW_VIEW_CHARINFO					= (NET_MSG_GCTRL + 1651),	//add duel bet
	NET_MSG_GCTRL_REQ_ALLOW_VIEW_CHARINFO_FB					= (NET_MSG_GCTRL + 1652),	//add duel bet

	//Add Royal Rumble - CNDev
	NET_MSG_GCTRL_ROYAL_RUMBLE_REMAIN_BRD		= (NET_MSG_GCTRL + 1653),
	NET_MSG_GCTRL_ROYAL_RUMBLE_READY_FLD		= (NET_MSG_GCTRL + 1654),
	NET_MSG_GCTRL_ROYAL_RUMBLE_START_FLD		= (NET_MSG_GCTRL + 1655),
	NET_MSG_GCTRL_ROYAL_RUMBLE_START_BRD		= (NET_MSG_GCTRL + 1656),

	NET_MSG_GCTRL_ROYAL_RUMBLE_END_FLD		= (NET_MSG_GCTRL + 1657),
	NET_MSG_GCTRL_ROYAL_RUMBLE_END_BRD		= (NET_MSG_GCTRL + 1658),

	NET_MSG_GCTRL_SERVER_ROYAL_RUMBLE_INFO		= (NET_MSG_GCTRL + 1659),
	NET_MSG_GCTRL_REQ_SERVER_ROYAL_RUMBLE_REMAIN_AG	= (NET_MSG_GCTRL + 1660),	

	NET_MSG_GCTRL_ROYAL_RUMBLE_PPOINT_UPDATE	= (NET_MSG_GCTRL + 1661),
	NET_MSG_GCTRL_ROYAL_RUMBLE_MYPRANK_UPDATE	= (NET_MSG_GCTRL + 1662),
	NET_MSG_GCTRL_ROYAL_RUMBLE_PRANKING_UPDATE	= (NET_MSG_GCTRL + 1663),
	NET_MSG_GCTRL_ROYAL_RUMBLE_PRANKING_REQ		= (NET_MSG_GCTRL + 1664),
	NET_MSG_GCTRL_SCHOOL_WAR_REMAIN_BRD				= (NET_MSG_GCTRL + 1636),	//<	Ŭ��������ġ �����ð�.
	NET_MSG_GCTRL_SCHOOL_WAR_READY_FLD				= (NET_MSG_GCTRL + 1637),	//<	Ŭ��������ġ �غ�.
	NET_MSG_GCTRL_SCHOOL_WAR_START_FLD				= (NET_MSG_GCTRL + 1638),	//<	Ŭ��������ġ ����.
	NET_MSG_GCTRL_SCHOOL_WAR_START_BRD				= (NET_MSG_GCTRL + 1639),	//<	Ŭ��������ġ ����.

	NET_MSG_GCTRL_SCHOOL_WAR_END_FLD				= (NET_MSG_GCTRL + 1640),	//<	Ŭ��������ġ ����.
	NET_MSG_GCTRL_SCHOOL_WAR_END_BRD				= (NET_MSG_GCTRL + 1641),	//<	Ŭ��������ġ ����.

	NET_MSG_GCTRL_SERVER_SCHOOL_WAR_INFO			= (NET_MSG_GCTRL + 1642),	//<	Ŭ��������ġ ���� ���� ( ���ӽ� ) 
	NET_MSG_GCTRL_REQ_SERVER_SCHOOL_WAR_REMAIN_AG	= (NET_MSG_GCTRL + 1643),	//<	Ŭ��������ġ ���� ���� ( Agent�� �����ð� ��û )

	NET_MSG_GCTRL_SCHOOL_WAR_POINT_UPDATE			= (NET_MSG_GCTRL + 1644),	//<	Ŭ��������ġ Kill/Death ��Ȳ ( �ڱ⸸ )
	NET_MSG_GCTRL_SCHOOL_WAR_MYRANK_UPDATE			= (NET_MSG_GCTRL + 1645),	//<	Ŭ��������ġ Kill/Death ��Ȳ ( �ڱ⸸ )
	NET_MSG_GCTRL_SCHOOL_WAR_RANKING_UPDATE			= (NET_MSG_GCTRL + 1646),	//<	Ŭ��������ġ ��ŷ ��Ȳ
	NET_MSG_GCTRL_SCHOOL_WAR_RANKING_REQ			= (NET_MSG_GCTRL + 1647),	//<	Ŭ��������ġ ��Ȳ ��û

	NET_MSG_GCTRL_TOWERDOWN_SG		=	(NET_MSG_GCTRL + 1665),
	NET_MSG_GCTRL_TOWERDOWN_SG_BRD	=	(NET_MSG_GCTRL + 1666),
	NET_MSG_GCTRL_TOWERDOWN_SG_FLD	=	(NET_MSG_GCTRL + 1667),
	NET_MSG_GCTRL_TOWERDOWN_MP		=	(NET_MSG_GCTRL + 1668),
	NET_MSG_GCTRL_TOWERDOWN_MP_BRD	=	(NET_MSG_GCTRL + 1669),
	NET_MSG_GCTRL_TOWERDOWN_MP_FLD	=	(NET_MSG_GCTRL + 1670),
	NET_MSG_GCTRL_TOWERDOWN	=	(NET_MSG_GCTRL + 1671),
	NET_MSG_GCTRL_TOWERDOWN_BRD	=	(NET_MSG_GCTRL + 1672),
	NET_MSG_GCTRL_TOWERDOWN_PHX_FLD	=	(NET_MSG_GCTRL + 1673),

	NET_MSG_GCTRL_DASH_BRD					= (NET_MSG_GCTRL + 1674),		//added by sid | 4.7.15 | dash-blink
	NET_MSG_GCTRL_BLINK_BRD					= (NET_MSG_GCTRL + 1675),	//added by sid | 4.7.15 | dash-blink

	NET_MSG_GCTRL_REQ_DISABLESKILLEFF				= (NET_MSG_GCTRL + 1865),
	NET_MSG_GCTRL_REQ_DISABLESKILLEFF_BRD			= (NET_MSG_GCTRL + 1866),
	NET_MSG_GCTRL_REQ_DISABLESKILLEFF_FB			= (NET_MSG_GCTRL + 1867),
	NET_MSG_GCTRL_UPDATE_MONEYTYPE					= (NET_MSG_GCTRL + 1868),
	NET_MSG_GCTRL_REQ_LEARNSKILL_NONINVEN			= (NET_MSG_GCTRL + 1869),
	NET_MSG_GCTRL_REQ_LEARNSKILL_NONINVEN_FB		= (NET_MSG_GCTRL + 1870),

	NET_MSG_REQ_CROWLIST							= (NET_MSG_GCTRL + 1827),
	NET_MSG_REQ_CROWLIST_FB							= (NET_MSG_GCTRL + 1828),
	NET_MSG_REQ_CROWLIST_FLD						= (NET_MSG_GCTRL + 1828),

	NET_MSG_REBUILD_MOVE_SEAL 						= (NET_MSG_GCTRL + 1830), //sealed card NaJDeV
	NET_MSG_AP_DT			                 	= (NET_MSG_GCTRL + 2063), //add autopots detection
	NET_MSG_AP_DT_FB			             	= (NET_MSG_GCTRL + 2064), //add autopots detection
	NET_MSG_AP_DT_BRD			             	= (NET_MSG_GCTRL + 2065), //add autopots detection
	NET_MSG_WPE_DT_BRD		                	= (NET_MSG_GCTRL + 2066), // wpe detection - JAYDev
	//dmk14
	NET_MSG_GCTRL_CROW_OWNER							= (NET_MSG_GCTRL + 1919),
	NET_MSG_GCTRL_TOWERWARS_REMAIN_BRD					= (NET_MSG_GCTRL + 1920),
	NET_MSG_GCTRL_TOWERWARS_START_BRD					= (NET_MSG_GCTRL + 1921),
	NET_MSG_GCTRL_TOWERWARS_END_BRD						= (NET_MSG_GCTRL + 1922),
	NET_MSG_GCTRL_TOWERWARS_READY_FLD					= (NET_MSG_GCTRL + 1923),
	NET_MSG_GCTRL_TOWERWARS_START_FLD					= (NET_MSG_GCTRL + 1924),
	NET_MSG_GCTRL_TOWERWARS_END_FLD						= (NET_MSG_GCTRL + 1925),
	NET_MSG_GCTRL_TOWERWARS_DOBATTLEEND_AGT				= (NET_MSG_GCTRL + 1926),
	NET_MSG_GCTRL_TOWERWARS_DOBATTLEEND_FLD				= (NET_MSG_GCTRL + 1927),
	NET_MSG_GCTRL_SERVER_TOWERWARS_INFO					= (NET_MSG_GCTRL + 1928),
	NET_MSG_GCTRL_REQ_SERVER_TOWERWARS_REMAIN_AG		= (NET_MSG_GCTRL + 1929),
	NET_MSG_GCTRL_TOWERWARS_CTFPOINT_UPDATE				= (NET_MSG_GCTRL + 1930),
	NET_MSG_GCTRL_TOWERWARS_CTFRANKING_UPDATE			= (NET_MSG_GCTRL + 1931),
	NET_MSG_GCTRL_TOWERWARS_MYCTFRANK_UPDATE			= (NET_MSG_GCTRL + 1932),
	NET_MSG_GCTRL_TOWERWARS_CTFRANKING_REQ				= (NET_MSG_GCTRL + 1933),
	NET_MSG_GCTRL_TOWERWARS_CTFICON_REQ					= (NET_MSG_GCTRL + 1934),
	NET_MSG_GCTRL_REQ_TO_AGENT_TOWERWARS_CAPTURE		= (NET_MSG_GCTRL + 1935),
	NET_MSG_GCTRL_TOWERWARS_CAPTURE_BRD					= (NET_MSG_GCTRL + 1936),
	NET_MSG_GCTRL_TOWERWARS_TOWER_UPDATE				= (NET_MSG_GCTRL + 1937),
	NET_MSG_GCTRL_TOWERWARS_CERTIFIED_AGT				= (NET_MSG_GCTRL + 1938),
	NET_MSG_GCTRL_TOWERWARS_CERTIFIED_FLD				= (NET_MSG_GCTRL + 1939),
	NET_MSG_GCTRL_TOWERWARS_SETWINSCHOOL_BRD			= (NET_MSG_GCTRL + 1940),
	NET_MSG_GCTRL_UPDATE_HP_MULTIPLIER					= (NET_MSG_GCTRL + 1941),
	NET_MSG_GCTRL_TOWERWARS_LASTCAPTURE_AGT				= (NET_MSG_GCTRL + 1942),
	NET_MSG_GCTRL_TOWERWARS_LASTCAPTURE_FLD				= (NET_MSG_GCTRL + 1943),
	NET_MSG_GCTRL_TOWERWARS_LASTCAPTURE_BRD				= (NET_MSG_GCTRL + 1944),
	NET_MSG_GCTRL_REQ_HOLD_ARRAY					= (NET_MSG_GCTRL + 1945),
	NET_MSG_GCTRL_REQ_HOLDARRAY_TO_INVEN			= (NET_MSG_GCTRL + 1946),
	NET_MSG_MOVE2MAPPOS				= (NET_MSG_GCTRL + 1947),	//	Mg PLAYER VIA GATE
	NET_MSG_MOVE2MAPPOS_FLD			= (NET_MSG_GCTRL + 1948),	//Mg PLAYER via gate
	NET_MSG_MOVE2MAPPOS_FB			= (NET_MSG_GCTRL + 1949),	//Mg PLAYER via gate
	NET_MSG_TYRANNY_PARTICIPATE				= (NET_MSG_GCTRL + 1950),
	NET_MSG_TYRANNY_PARTICIPATE_BRD					= (NET_MSG_GCTRL + 1951),
	NET_MSG_GCTRL_TOWERWARS_PREVIOUS_RECORD		= (NET_MSG_GCTRL + 1963),
	NET_MSG_GCTRL_TOWERWARS_PREVIOUS_RECORD_FLD = (NET_MSG_GCTRL + 1964),

	
	NET_MSG_GCTRL_GET_ITEMSHOP_FROMDB	= (NET_MSG_GCTRL + 2085),//<	������ ������ ���� DB���� �������� ��û
	NET_MSG_GCTRL_GET_ITEMSHOP_FROMDB_FB = (NET_MSG_GCTRL + 2086),//<	������ ������ ���� DB���� �������� ��û FB
	NET_MSG_GCTRL_BUY_ITEMSHOP = (NET_MSG_GCTRL + 2087),//<	������ ������ ���� DB���� �������� ��û
	NET_MSG_GCTRL_BUY_ITEMSHOP_FB = (NET_MSG_GCTRL + 2088),//<	������ ������ ���� DB���� �������� ��û FB
	NET_MSG_GCTRL_BUY_ITEMSHOP_ITEM	= (NET_MSG_GCTRL + 2089),	//<	������ ������ �κ��� �ֱ� ��û.
	NET_MSG_GCTRL_ADD_ITEMSHOP_ITEM	= (NET_MSG_GCTRL + 2090),	//<	������ ������ �κ��� �ֱ� ��û.
	NET_MSG_GCTRL_ADD_ITEMSHOP_ITEM_FB	= (NET_MSG_GCTRL + 2091),	//<	������ ������ �κ��� �ֱ� ��û.



};


// Generic message
// 8 bytes
struct NET_MSG_GENERIC
{
	DWORD		dwSize;		// Size of Message Data
	EMNET_MSG	nType;		// Message type
};
typedef NET_MSG_GENERIC* LPNET_MSG_GENERIC;

struct NET_HEARTBEAT_CLIENT_REQ
{
	NET_MSG_GENERIC nmg;	

	NET_HEARTBEAT_CLIENT_REQ()
	{
		nmg.dwSize = sizeof( NET_HEARTBEAT_CLIENT_REQ );
		nmg.nType = NET_MSG_HEARTBEAT_CLIENT_REQ;
	}
};

struct NET_HEARTBEAT_CLIENT_ANS
{
	NET_MSG_GENERIC nmg;

	CHAR	szEnCrypt[ENCRYPT_KEY+1];

	NET_HEARTBEAT_CLIENT_ANS()
	{
		nmg.dwSize = sizeof( NET_HEARTBEAT_CLIENT_ANS );
		nmg.nType = NET_MSG_HEARTBEAT_CLIENT_ANS;
		SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
	}
};

struct NET_HEARTBEAT_SERVER_REQ
{
	NET_MSG_GENERIC nmg;
	NET_HEARTBEAT_SERVER_REQ()
	{
		nmg.dwSize = sizeof( NET_HEARTBEAT_SERVER_REQ );
		nmg.nType  = NET_MSG_HEARTBEAT_SERVER_REQ; //< Server HeartBeat Request
	}
};

struct NET_HEARTBEAT_SERVER_ANS
{
	NET_MSG_GENERIC nmg;
	NET_HEARTBEAT_SERVER_ANS()
	{
		nmg.dwSize = sizeof( NET_HEARTBEAT_SERVER_ANS );
		nmg.nType  = NET_MSG_HEARTBEAT_SERVER_ANS; //< Server HeartBeat Answer
	}
};

//! Compressed or combined message
struct NET_COMPRESS
{
	NET_MSG_GENERIC nmg;
	bool bCompress; //< Compressed or not (combined)
	BYTE	nCheck;

	NET_COMPRESS()
	{
		nmg.dwSize = sizeof(NET_COMPRESS);
		nmg.nType  = NET_MSG_COMPRESS;
		bCompress = true;
		nCheck = 0;
	}
};
typedef NET_COMPRESS* LPNET_COMPRESS;

// Confirmation Message
struct NET_CONF
{
	NET_MSG_GENERIC nmg;
	USHORT			conf;
	NET_CONF()
	{
		nmg.dwSize = sizeof(NET_CONF);
	}
};
typedef NET_CONF* LPNET_CONF;

struct NET_FEEDBACK_DATA
{
	NET_MSG_GENERIC nmg;
	USHORT			nResult;
	NET_FEEDBACK_DATA()
	{
		nmg.dwSize = sizeof(NET_FEEDBACK_DATA);
	}
};
typedef NET_FEEDBACK_DATA* LPNET_FEEDBACK_DATA;

struct CRYPT_KEY
{
	USHORT			nKeyDirection;
	USHORT			nKey;

	CRYPT_KEY()
	{
		nKeyDirection = 0;
		nKey          = 0;
	}
};
typedef CRYPT_KEY* LPCRYPT_KEY;

struct NET_CRYPT_KEY
{
	NET_MSG_GENERIC nmg;
	CRYPT_KEY		ck;

	NET_CRYPT_KEY()
	{
		nmg.dwSize = sizeof(NET_CRYPT_KEY);
		nmg.nType = NET_MSG_SND_CRYT_KEY;
	}
};
typedef NET_CRYPT_KEY* LPNET_CRYPT_KEY;

struct NET_RANDOMPASS_NUMBER
{
	NET_MSG_GENERIC nmg;
	INT nRandomNumber;

	NET_RANDOMPASS_NUMBER()
	{
		nmg.dwSize = sizeof(NET_RANDOMPASS_NUMBER);
		nmg.nType  = NET_MSG_RANDOM_NUM;
		nRandomNumber = 0;
	}
};

// Control program->Session Server
// Server state change command 
// NET_MSG_SVR_PAUSE			= (NET_MSG_LGIN + 310), // ������ ��� �����.
// NET_MSG_SVR_RESUME			= (NET_MSG_LGIN + 311), // ���߾��� ������ �ٽ� ������Ų��.	
// NET_MSG_SVR_RESTART			= (NET_MSG_LGIN + 312), // ������ ������ ���߰� ���Ӱ� ������Ų��.
// NET_MSG_SVR_STOP			    = (NET_MSG_LGIN + 313), // ������ ������ �����.
struct NET_SVR_CMD_CHANGE_STATE
{
	NET_MSG_GENERIC nmg;
	int nCmdType;
	int nSvrType;
	int nSvrGrp;
	int nSvrNum;
	int nSvrField;

	NET_SVR_CMD_CHANGE_STATE()
	{
		nmg.dwSize = sizeof(NET_SVR_CMD_CHANGE_STATE);
		nmg.nType = NET_MSG_SVR_CMD;
		nCmdType  = NET_MSG_SVR_RESTART;
		nSvrType  = -1;
		nSvrGrp   = -1;
		nSvrNum   = -1;
		nSvrField = -1;
	}
};
typedef NET_SVR_CMD_CHANGE_STATE* LPNET_SVR_CMD_CHANGE_STATE;

///////////////////////////////////////////////////////////////////////////////
// Login messages
// Taiwan / Hongkong
struct NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int             nChannel;
	CHAR            szRandomPassword[USR_RAND_PASS_LENGTH+1];
	CHAR			szPassword[USR_PASS_LENGTH+1];
	CHAR			szUserid  [USR_ID_LENGTH+1];
	CHAR			szEnCrypt[ENCRYPT_KEY+1];

	NET_LOGIN_DATA()
	{		
		nmg.dwSize  = sizeof(NET_LOGIN_DATA);
		nmg.nType  = NET_MSG_LOGIN_2;
		nChannel   = 0;
		SecureZeroMemory(szUserid,         sizeof(CHAR) * (USR_ID_LENGTH+1));
		SecureZeroMemory(szPassword,       sizeof(CHAR) * (USR_PASS_LENGTH+1));
		SecureZeroMemory(szRandomPassword, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
	}
};
typedef NET_LOGIN_DATA* LPNET_LOGIN_DATA;

///////////////////////////////////////////////////////////////////////////////
// Login messages
// China
#define RSA_ADD	4

struct CHINA_NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int             nChannel;
	TCHAR           szRandomPassword[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];

	CHINA_NET_LOGIN_DATA()
	{
		nmg.dwSize  = sizeof(CHINA_NET_LOGIN_DATA);
		nmg.nType  = CHINA_NET_MSG_LOGIN;
		nChannel   = 0;
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szRandomPassword, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_LOGIN_DATA* LPCHINA_NET_LOGIN_DATA;

///////////////////////////////////////////////////////////////////////////////
// Login messages
// Thailand (�±�)
struct THAI_NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int             nChannel;
	CHAR			szPassword[USR_PASS_LENGTH+1];
	CHAR			szUserid  [USR_ID_LENGTH+1];	

	THAI_NET_LOGIN_DATA()
	{		
		nmg.dwSize  = sizeof(THAI_NET_LOGIN_DATA);
		nmg.nType  = THAI_NET_MSG_LOGIN;
		nChannel   = 0;
		memset(szUserid,   0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset(szPassword, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));

	}
};
typedef THAI_NET_LOGIN_DATA* LPTHAI_NET_LOGIN_DATA;

///////////////////////////////////////////////////////////////////////////////
// Login messages
// Daum Game

//struct DAUM_NET_LOGIN_DATA
//{
//	NET_MSG_GENERIC nmg;
//	int             nChannel;
//	CHAR	        szParameter[DAUM_MAX_PARAM_LENGTH+1];	
//
//	DAUM_NET_LOGIN_DATA()
//	{
//		memset(this, 0, sizeof(DAUM_NET_LOGIN_DATA));
//		nmg.dwSize = sizeof(DAUM_NET_LOGIN_DATA);
//		nmg.nType = DAUM_NET_MSG_LOGIN;
//	}
//};
//typedef DAUM_NET_LOGIN_DATA* LPDAUM_NET_LOGIN_DATA;

struct DAUM_NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int nChannel;
	TCHAR szUUID[UUID_STR_LENGTH];	

	DAUM_NET_LOGIN_DATA()
	{
		nmg.dwSize = sizeof(DAUM_NET_LOGIN_DATA);
		nmg.nType = DAUM_NET_MSG_LOGIN;
		nChannel = 0;
		memset( szUUID, 0, sizeof(TCHAR) * (UUID_STR_LENGTH) );
	}
};
typedef DAUM_NET_LOGIN_DATA* LPDAUM_NET_LOGIN_DATA;

///////////////////////////////////////////////////////////////////////////////
// Login message
// Terra
struct TERRA_NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int nChannel;
	TCHAR szTID[TERRA_TID_ENCODE+1];

    TERRA_NET_LOGIN_DATA()
	{		
		nmg.dwSize = sizeof(TERRA_NET_LOGIN_DATA);
		nmg.nType = TERRA_NET_MSG_LOGIN;
		nChannel  = 0;
		memset(szTID, 0, sizeof(TCHAR) * (TERRA_TID_ENCODE+1));		
	}
};
typedef TERRA_NET_LOGIN_DATA* LPTERRA_NET_LOGIN_DATA;

// Ran Global Service GSP Login Data
struct GSP_NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int nChannel;
	TCHAR szUUID[UUID_STR_LENGTH];

	GSP_NET_LOGIN_DATA()
	{
		nmg.dwSize = sizeof(GSP_NET_LOGIN_DATA);
		nmg.nType = GSP_NET_MSG_LOGIN;
		nChannel  = 0;
		memset( szUUID, 0, sizeof(TCHAR) * (UUID_STR_LENGTH) );
	}
};
typedef GSP_NET_LOGIN_DATA* LPGSP_NET_LOGIN_DATA;

///////////////////////////////////////////////////////////////////////////////
// Password Check message
// Terra
struct TERRA_NET_PASSCHECK_DATA
{
	NET_MSG_GENERIC nmg;
	TCHAR szTLoginName[TERRA_TLOGIN_NAME+1];
	TCHAR szUserPass[TERRA_USERPASS+1];
	INT		nCheckFlag; // �н����� ������ ���ؼ� �߰�

    TERRA_NET_PASSCHECK_DATA()
		: nCheckFlag(0)
	{		
		nmg.dwSize = sizeof(TERRA_NET_PASSCHECK_DATA);
		nmg.nType = TERRA_NET_MSG_PASSCHECK;
		memset(szTLoginName, 0, sizeof(TCHAR) * (TERRA_TLOGIN_NAME+1));
		memset(szUserPass, 0, sizeof(TCHAR) * (TERRA_USERPASS+1));
	}
};
typedef TERRA_NET_PASSCHECK_DATA* LPTERRA_NET_PASSCHECK_DATA;

// Daum
struct DAUM_NET_PASSCHECK_DATA
{
	NET_MSG_GENERIC nmg;
	TCHAR	szDaumGID[DAUM_MAX_GID_LENGTH+1];
	TCHAR	szUserPass[DAUM_USERPASS+1];
	INT		nCheckFlag; // �н����� ������ ���ؼ� �߰�

	DAUM_NET_PASSCHECK_DATA()
		: nCheckFlag(0)
	{
		nmg.nType = DAUM_NET_MSG_PASSCHECK;
		nmg.dwSize = sizeof(DAUM_NET_PASSCHECK_DATA);
		memset(szDaumGID, 0, sizeof(szDaumGID));
		memset(szUserPass, 0, sizeof(szUserPass));
	}
};
typedef DAUM_NET_PASSCHECK_DATA* LPDAUM_NET_PASSCHECK_DATA;


struct GSP_NET_LOGIN_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	TCHAR           szGspUserID[GSP_USERID];	
	TCHAR           szIp[GSP_USERIP];
	TCHAR			szUUID[UUID_STR_LENGTH]; 
	INT				nClient;
	USHORT			nResult;
	INT				nUserNum;
	INT				nUserType;
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;

    GSP_NET_LOGIN_FEEDBACK_DATA2()
	{		
		nmg.dwSize = sizeof(GSP_NET_LOGIN_FEEDBACK_DATA2);
		memset( szGspUserID, 0, sizeof(TCHAR) * (GSP_USERID) );
		memset( szIp, 0, sizeof(TCHAR) * (GSP_USERIP) );
		memset( szUUID, 0, sizeof(TCHAR) * (UUID_STR_LENGTH) );
		nClient        = 0;
		nResult        = 0;
		nUserNum       = 0;
		nUserType      = 0;
		uChaRemain     = 0;
		uChaTestRemain = 0;
		tPremiumTime   = 0;
		tChatBlockTime = 0;
	}
};
typedef GSP_NET_LOGIN_FEEDBACK_DATA2* LPGSP_NET_LOGIN_FEEDBACK_DATA2;

// Excite Japan
struct EXCITE_NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int				nChannel;
	CHAR			szUID[EXCITE_USER_ID+1];
	CHAR			szTDATE[EXCITE_TDATE+1];
	CHAR            szMD5[EXCITE_MD5+1];

    EXCITE_NET_LOGIN_DATA()
	{
		nmg.dwSize = sizeof(EXCITE_NET_LOGIN_DATA);
		nmg.nType = EXCITE_NET_MSG_LOGIN;
		nChannel  = 0;
		memset(szUID,   0, sizeof(CHAR) * (EXCITE_USER_ID+1));
		memset(szTDATE, 0, sizeof(CHAR) * (EXCITE_TDATE+1));
		memset(szMD5,   0, sizeof(CHAR) * (EXCITE_MD5+1));
	}
};
typedef EXCITE_NET_LOGIN_DATA* LPEXCITE_NET_LOGIN_DATA;


// Excite
struct EXCITE_NET_PASSCHECK_DATA
{
	NET_MSG_GENERIC nmg;
	TCHAR	szExciteUserID[EXCITE_SEC_ID+1];
	TCHAR	szUserPass[EXCITE_SEC_PASS+1];
	INT		nCheckFlag; // �н����� ������ ���ؼ� �߰�

	EXCITE_NET_PASSCHECK_DATA()
		: nCheckFlag(0)
	{
		nmg.nType = EXCITE_NET_MSG_PASSCHECK;
		nmg.dwSize = sizeof(EXCITE_NET_PASSCHECK_DATA);
		memset(szExciteUserID, 0, sizeof(szExciteUserID));
		memset(szUserPass, 0, sizeof(szUserPass));
	}
};
typedef EXCITE_NET_PASSCHECK_DATA* LPEXCITE_NET_PASSCHECK_DATA;



struct EXCITE_NET_PASSCHECK_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	TCHAR			szExciteUserID[EXCITE_USER_ID+1];
	INT				nClient;
	USHORT			nResult;

	EXCITE_NET_PASSCHECK_FEEDBACK_DATA2()
		: nClient(0)
		, nResult(0)
	{
		memset(this, 0, sizeof( EXCITE_NET_PASSCHECK_FEEDBACK_DATA2 ));
		nmg.dwSize		= sizeof(EXCITE_NET_PASSCHECK_FEEDBACK_DATA2);
		memset(szExciteUserID, 0, sizeof( szExciteUserID ));
	}
};
typedef EXCITE_NET_PASSCHECK_FEEDBACK_DATA2* LPEXCITE_NET_PASSCHECK_FEEDBACK_DATA2;

struct JAPAN_NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int				nChannel;		
	CHAR			szUserid  [JAPAN_USER_ID+1];
	CHAR			szPassword[JAPAN_USER_PASS+1];
	CHAR			szEnCrypt[ENCRYPT_KEY+1];

	JAPAN_NET_LOGIN_DATA()
	{		
		nmg.dwSize  = sizeof(JAPAN_NET_LOGIN_DATA);
		nmg.nType  = JAPAN_NET_MSG_LOGIN;
		nChannel   = 0;
		SecureZeroMemory(szUserid,         sizeof(CHAR) * (JAPAN_USER_ID+1));
		SecureZeroMemory(szPassword,       sizeof(CHAR) * (JAPAN_USER_PASS+1));
		SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
	}
};
typedef JAPAN_NET_LOGIN_DATA* LPJAPAN_NET_LOGIN_DATA;

struct GS_NET_LOGIN_DATA
{
	NET_MSG_GENERIC nmg;
	int             nChannel;
	CHAR			szPassword[GS_USER_ID+1];
	CHAR			szUserid  [GS_USER_PASS+1];
	CHAR			szEnCrypt[ENCRYPT_KEY+1];

	GS_NET_LOGIN_DATA()
	{		
		nmg.dwSize  = sizeof(GS_NET_LOGIN_DATA);
		nmg.nType  = GS_NET_MSG_LOGIN;
		nChannel   = 0;
		SecureZeroMemory(szUserid,         sizeof(CHAR) * (GS_USER_ID+1));
		SecureZeroMemory(szPassword,       sizeof(CHAR) * (GS_USER_PASS+1));
		SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
	}
};
typedef GS_NET_LOGIN_DATA* LPGS_NET_LOGIN_DATA;

struct NET_LOGIN_DATA2
{
	NET_MSG_GENERIC nmg;
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szPassword[USR_PASS_LENGTH+1];
	CHAR			szIp[MAX_IP_LENGTH+1];
	INT				nClient;

	NET_LOGIN_DATA2()
	{
		nmg.nType = EMNET_MSG(0);
		nmg.dwSize = sizeof(NET_LOGIN_DATA2);
		memset(szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset(szPassword, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient = 0;
	}
};
typedef  NET_LOGIN_DATA2* LPNET_LOGIN_DATA2;

struct DAUM_NET_LOGIN_DATA2
{
	NET_MSG_GENERIC nmg;
	CHAR			szDaumGID[DAUM_MAX_GID_LENGTH+1];
	CHAR			szDaumUID[DAUM_MAX_UID_LENGTH+1];
	CHAR			szDaumSSNHEAD[DAUM_MAX_SSNHEAD_LENGTH+1];
	CHAR			szDaumSEX[DAUM_MAX_SEX_LENGTH+1];
	CHAR			szIp[MAX_IP_LENGTH+1];
	INT				nClient;

	DAUM_NET_LOGIN_DATA2()
	{
		nmg.nType = EMNET_MSG(0);
		nmg.dwSize = sizeof(DAUM_NET_LOGIN_DATA2);
		memset(szDaumGID, 0, sizeof(CHAR) * (DAUM_MAX_GID_LENGTH+1));
		memset(szDaumUID, 0, sizeof(CHAR) * (DAUM_MAX_UID_LENGTH+1));
		memset(szDaumSSNHEAD, 0, sizeof(CHAR) * (DAUM_MAX_SSNHEAD_LENGTH+1));
		memset(szDaumSEX, 0, sizeof(CHAR) * (DAUM_MAX_SEX_LENGTH+1));
		memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient = 0;
	}
};
typedef  DAUM_NET_LOGIN_DATA2* LPDAUMNET_LOGIN_DATA2;

struct NET_LOGIN_FEEDBACK_DATA
{
	NET_MSG_GENERIC nmg;

	TCHAR			szDaumGID[DAUM_MAX_GID_LENGTH+1];
	USHORT			nResult;
    USHORT          uChaRemain; // ����� �ִ� ĳ���� ����
	INT				nExtremeM;	// �ذ��� ���� ���� ���� ����
	INT				nExtremeW;	// �ذ��� ���� ���� ���� ����
	INT				nCheckFlag; // 2�� �н����带 �ޱ����� Check Flag ��
	INT				nPatchProgramVer;
	INT				nGameProgramVer;

	NET_LOGIN_FEEDBACK_DATA()
	{
		memset(this, 0, sizeof(NET_LOGIN_FEEDBACK_DATA));
		nmg.dwSize        = sizeof(NET_LOGIN_FEEDBACK_DATA);
		memset(szDaumGID, 0, sizeof(szDaumGID));
		nResult          = EM_LOGIN_FB_SUB_FAIL;
		nPatchProgramVer = 0;
		nGameProgramVer  = 0;
        uChaRemain       = 0;
		nExtremeM		 = 0;
		nExtremeW		 = 0;
		nCheckFlag		 = 0;
	}
};
typedef NET_LOGIN_FEEDBACK_DATA* LPNET_LOGIN_FEEDBACK_DATA;

struct NET_LOGIN_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	INT				nUserNum;
	INT				nUserType;
	INT				nExtremeM;				// �ذ��� ���� ���� ���� ����
	INT				nExtremeW;				// �ذ��� ���� ���� ���� ����
	INT				nCheckFlag;				// �н����带 �ޱ� ���� Check Flag ��
	INT				nThaiCC_Class;			// �±� User Ŭ���� Ÿ��
	INT				nMyCC_Class;			// �����̽þ� User Ŭ���� Ÿ��
	INT				nChinaGameTime;			// �߱� GameTime ����
	INT				nChinaOfflineTime;		// �߱� OfflienTiem ����
	INT				nChinaUserAge;			// �߱� UserAge ����
	INT				nVTGameTime;			//  ��Ʈ�� GameTime ����
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;
	__time64_t		tLastLoginTime;			// �߱�, ��Ʈ�� LastLoginTime ����
	WORD            wThaiPlayTime;			// Thailand (���� �÷����� �ð� ����:��)
	WORD            wThaiFlag;				// Thailand (����� ���� �÷���)
	
	NET_LOGIN_FEEDBACK_DATA2()
	{
		nmg.dwSize = sizeof(NET_LOGIN_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
		nUserNum       = 0;
		nUserType      = 0;
		nExtremeM	   = 0;
		nExtremeW	   = 0;
		uChaRemain     = 0;
		uChaTestRemain = 0;
		tPremiumTime   = 0;
		tChatBlockTime = 0;
		wThaiPlayTime  = 0;
		wThaiFlag      = 0;
		nCheckFlag	   = 0;
		nCheckFlag     = 0;
		nThaiCC_Class  = 0;
		nChinaGameTime = 0;
		nChinaOfflineTime = 0;
		tLastLoginTime = 0;
		nChinaUserAge = 0;
		nVTGameTime		= 0;
	}
};
typedef NET_LOGIN_FEEDBACK_DATA2* LPNET_LOGIN_FEEDBACK_DATA2;

///////////////////////////////////////////////////////////////////////////////
//
struct DAUM_NET_LOGIN_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	TCHAR			szDaumGID[DAUM_MAX_GID_LENGTH+1];
//	TCHAR			szDaumUID[DAUM_MAX_UID_LENGTH+1];
	TCHAR			szUUID[UUID_STR_LENGTH]; 
	TCHAR			szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	INT				nUserNum;
	INT				nUserType;
	INT				nExtremeM;	// �ذ��� ���� ���� ���� ����
	INT				nExtremeW;	// �ذ��� ���� ���� ���� ����
	INT				nCheckFlag; // Daum �н����带 �ޱ����� Check Flag ��
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;
	
	DAUM_NET_LOGIN_FEEDBACK_DATA2()
	{	
		nmg.dwSize = sizeof(DAUM_NET_LOGIN_FEEDBACK_DATA2);
		memset(szDaumGID, 0, sizeof(szDaumGID));
		memset( szUUID, 0, sizeof(TCHAR) * (UUID_STR_LENGTH) );
//		memset(szDaumUID, 0, sizeof(szDaumUID));
		memset(szIp, 0, sizeof(szIp));		
		nClient        = 0;
		nResult        = 0;
		nUserNum       = 0;
		nUserType      = 0;
		nExtremeM	   = 0;
		nExtremeW	   = 0;
		uChaRemain     = 0;
		uChaTestRemain = 0;
		tPremiumTime   = 0;
        tChatBlockTime = 0;
		nCheckFlag	   = 0;
	}
};
typedef DAUM_NET_LOGIN_FEEDBACK_DATA2* LPDAUM_NET_LOGIN_FEEDBACK_DATA2;

struct TERRA_NET_LOGIN_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	CHAR            szTerraTLoginName[TERRA_TLOGIN_NAME+1];
	CHAR            szTerraDecodedTID[TERRA_TID_DECODE+1];
	CHAR            szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	INT				nUserNum;
	INT				nUserType;
	INT				nExtremeM;				// �ذ��� ���� ���� ���� ����
	INT				nExtremeW;				// �ذ��� ���� ���� ���� ����
	INT				nCheckFlag;				// ���������� �н����带 �ޱ����� Check Flag ��
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
	INT				nMyCC_Class;			// �����̽þ� PC�� �̺�Ʈ
	INT				nVTGameTime;			//  ��Ʈ�� GameTime ����
	__time64_t		tLastLoginTime;			// �߱�, ��Ʈ�� LastLoginTime ����
    __time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;

    TERRA_NET_LOGIN_FEEDBACK_DATA2()
	{		
		nmg.dwSize = sizeof(TERRA_NET_LOGIN_FEEDBACK_DATA2);
		memset(szTerraTLoginName, 0, sizeof(CHAR) * (TERRA_TLOGIN_NAME+1));
		memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		memset(szTerraDecodedTID, 0, sizeof(CHAR) * (TERRA_TID_DECODE+1));
		nClient        = 0;
		nResult        = 0;
		nUserNum       = 0;
		nUserType      = 0;
		nExtremeM	   = 0;
		nExtremeW	   = 0;
		nCheckFlag	   = 0;
		uChaRemain     = 0;		
		uChaTestRemain = 0;
		tPremiumTime   = 0;
		tChatBlockTime = 0;
		nVTGameTime	   = 0;
		tLastLoginTime = 0;
		nMyCC_Class	   = 0;
	}
};
typedef TERRA_NET_LOGIN_FEEDBACK_DATA2* LPTERRA_NET_LOGIN_FEEDBACK_DATA2;

struct NET_PASSCHECK_FEEDBACK_DATA
{
	NET_MSG_GENERIC nmg;
	INT				nClient;
	USHORT			nResult;

	NET_PASSCHECK_FEEDBACK_DATA()
	{		
		memset(this, 0, sizeof( NET_PASSCHECK_FEEDBACK_DATA ));
		nmg.nType		= NET_MSG_PASSCHECK_FB;
		nmg.dwSize		= sizeof( NET_PASSCHECK_FEEDBACK_DATA );
		nClient			= 0;
		nResult			= 0;
	}
};
typedef NET_PASSCHECK_FEEDBACK_DATA* LPNET_PASSCHECK_FEEDBACK_DATA;

struct TERRA_NET_PASSCHECK_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	CHAR            szTerraTLoginName[TERRA_TLOGIN_NAME+1];
	INT				nClient;
	USHORT			nResult;

	TERRA_NET_PASSCHECK_FEEDBACK_DATA2()
	{
		memset(szTerraTLoginName, 0, sizeof(CHAR) * (TERRA_TLOGIN_NAME+1));
		memset(this, 0, sizeof( TERRA_NET_PASSCHECK_FEEDBACK_DATA2 ));
		nmg.dwSize		= sizeof(TERRA_NET_PASSCHECK_FEEDBACK_DATA2);
		nClient			= 0;
		nResult			= 0;
	}
};
typedef TERRA_NET_PASSCHECK_FEEDBACK_DATA2* LPTERRA_NET_PASSCHECK_FEEDBACK_DATA2;

struct DAUM_NET_PASSCHECK_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	TCHAR			szDaumGID[DAUM_MAX_GID_LENGTH+1];
	INT				nClient;
	USHORT			nResult;

	DAUM_NET_PASSCHECK_FEEDBACK_DATA2()
		: nClient(0)
		, nResult(0)
	{
		memset(this, 0, sizeof( DAUM_NET_PASSCHECK_FEEDBACK_DATA2 ));
		nmg.dwSize		= sizeof(DAUM_NET_PASSCHECK_FEEDBACK_DATA2);
		memset(szDaumGID, 0, sizeof( szDaumGID ));
	}
};
typedef DAUM_NET_PASSCHECK_FEEDBACK_DATA2* LPDAUM_NET_PASSCHECK_FEEDBACK_DATA2;

// nCheckFlag �� �߰�
struct EXCITE_NET_LOGIN_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	CHAR            szExciteUserID[EXCITE_USER_ID+1];
	CHAR            szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	INT				nUserNum;
	INT				nUserType;
	INT				nCheckFlag; // �Ϻ� �н����带 �ޱ����� Check Flag ��
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;

    EXCITE_NET_LOGIN_FEEDBACK_DATA2()
	{
		nmg.dwSize = sizeof(EXCITE_NET_LOGIN_FEEDBACK_DATA2);
		memset(szExciteUserID, 0, sizeof(CHAR) * (EXCITE_USER_ID+1));
		memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
		nUserNum       = 0;
		nUserType      = 0;
		nCheckFlag	   = 0;
		uChaRemain     = 0;
		uChaTestRemain = 0;
		tPremiumTime   = 0;
		tChatBlockTime = 0;
	}
};
typedef EXCITE_NET_LOGIN_FEEDBACK_DATA2* LPEXCITE_NET_LOGIN_FEEDBACK_DATA2;

// �Ϻ� Gonzo�� ȸ�� ����Ǹ鼭 ����ü�߰�
struct JAPAN_NET_LOGIN_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	CHAR            JapanUserID[JAPAN_USER_ID+1];
	CHAR            szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	INT				nUserNum;
	INT				nUserType;
	INT				nExtremeM;	// �ذ��� ���� ���� ���� ����
	INT				nExtremeW;	// �ذ��� ���� ���� ���� ����
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;

    JAPAN_NET_LOGIN_FEEDBACK_DATA2()
	{		
		nmg.dwSize = sizeof(JAPAN_NET_LOGIN_FEEDBACK_DATA2);
		memset(JapanUserID, 0, sizeof(CHAR) * (JAPAN_USER_ID+1));
		memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
		nUserNum       = 0;
		nUserType      = 0;
		nExtremeM	   = 0;
		nExtremeW	   = 0;
		uChaRemain     = 0;
		uChaTestRemain = 0;
		tPremiumTime   = 0;
		tChatBlockTime = 0;
	}
};
typedef JAPAN_NET_LOGIN_FEEDBACK_DATA2* LPJAPAN_NET_LOGIN_FEEDBACK_DATA2;

struct JAPAN_NET_LOGIN_UUID
{
	NET_MSG_GENERIC nmg;
	CHAR	szUUID[UUID_STR_LENGTH];
	
	JAPAN_NET_LOGIN_UUID()
	{		
		nmg.nType	= JAPAN_NET_MSG_UUID;
		nmg.dwSize	= sizeof(JAPAN_NET_LOGIN_UUID);
		memset( szUUID, 0, sizeof(CHAR) * UUID_STR_LENGTH );
	}
};

struct GS_NET_LOGIN_FEEDBACK_DATA2
{
	NET_MSG_GENERIC nmg;
	CHAR			szUserid[GS_USER_ID+1];
	CHAR			szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	INT				nUserNum;
	INT				nUserType;
	INT				nExtremeM;				// �ذ��� ���� ���� ���� ����
	INT				nExtremeW;				// �ذ��� ���� ���� ���� ����
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;
	
	GS_NET_LOGIN_FEEDBACK_DATA2()
	{
		nmg.dwSize = sizeof(GS_NET_LOGIN_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (GS_USER_ID+1));
		memset (szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
		nUserNum       = 0;
		nUserType      = 0;
		nExtremeM	   = 0;
		nExtremeW	   = 0;
		uChaRemain     = 0;
		uChaTestRemain = 0;
		tPremiumTime   = 0;
		tChatBlockTime = 0;
	}
};
typedef GS_NET_LOGIN_FEEDBACK_DATA2* LPGSNET_LOGIN_FEEDBACK_DATA2;

///////////////////////////////////////////////////////////////////////////////
// Agent Server -> Field Server �α��� ������ ����� ����
// Field Server ���� ������
struct NET_LOGIN_FEEDBACK_DATA3
{
	NET_MSG_GENERIC nmg;	
	CHAR			szUserid[USR_ID_LENGTH+1];	
	INT				nUserNum;
	
	NET_LOGIN_FEEDBACK_DATA3()
	{
		nmg.nType = EMNET_MSG(0);
		nmg.dwSize = sizeof(NET_LOGIN_FEEDBACK_DATA3);		
		nUserNum = 0;
		memset(szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
	}
};
typedef NET_LOGIN_FEEDBACK_DATA3* LPNET_LOGIN_FEEDBACK_DATA3;

struct NET_LOGOUT_DATA
{
	NET_MSG_GENERIC nmg;
	CHAR			szUserID[USR_ID_LENGTH+1];// �α׾ƿ� �ϴ� �����
	INT				nGameTime;				// �÷��̽ð�
	INT				nUserNum;				// ����� ��ȣ
	INT				nChaNum;

	NET_LOGOUT_DATA()
	{
		nmg.nType = EMNET_MSG(0);
		nmg.dwSize = sizeof(NET_LOGOUT_DATA);
		memset(szUserID, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		nGameTime = 0;
		nUserNum  = -1;
		nChaNum   = 0;
	}
};
typedef NET_LOGOUT_DATA* LPNET_LOGOUT_DATA;

struct DAUM_NET_LOGOUT_DATA
{	
	NET_MSG_GENERIC nmg;
	CHAR			szUID[DAUM_MAX_UID_LENGTH+1];// �α׾ƿ� �ϴ� �����
	INT				nGameTime;				// �÷��̽ð�
	INT				nUserNum;				// ����� ��ȣ
	INT				nChaNum;

	DAUM_NET_LOGOUT_DATA()
	{
		nmg.nType = EMNET_MSG(0);
		nmg.dwSize = sizeof(DAUM_NET_LOGOUT_DATA);
		memset(szUID, 0, sizeof(CHAR) * (DAUM_MAX_UID_LENGTH+1));
		nGameTime = 0;
		nUserNum  = -1;
		nChaNum   = 0;
	}
};
typedef DAUM_NET_LOGOUT_DATA* LPDAUM_NET_LOGOUT_DATA;

struct NET_LOGOUT_DATA_SIMPLE
{
	NET_MSG_GENERIC nmg;
	CHAR			szUserID[USR_ID_LENGTH+1];// �α׾ƿ� �ϴ� �����
	
	NET_LOGOUT_DATA_SIMPLE()
	{
		nmg.nType = EMNET_MSG(0);
		nmg.dwSize = sizeof(NET_LOGOUT_DATA_SIMPLE);
		memset(szUserID, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
	}
};
typedef NET_LOGOUT_DATA_SIMPLE* LPNET_LOGOUT_DATA_SIMPLE;

struct NET_CHARACTER_INC_DEC
{
	NET_MSG_GENERIC nmg;
	int	            nUserNum;	

	NET_CHARACTER_INC_DEC()
	{
		nmg.dwSize = sizeof(NET_CHARACTER_INC_DEC);
		nmg.nType = NET_MSG_CHA_DECREASE;
		nUserNum  = 0;		
	}
};
typedef NET_CHARACTER_INC_DEC* LPNET_CHARACTER_INC_DEC;

struct NET_CREATE_ACCOUNT_DATA
{
	NET_MSG_GENERIC nmg;
	CHAR			szAccountName[USR_ID_LENGTH+1];
	CHAR			szPassword[USR_PASS_LENGTH+1];
	CHAR			szUserName[USR_ID_LENGTH+1];

	NET_CREATE_ACCOUNT_DATA()
	{
		nmg.nType = EMNET_MSG(0);
		nmg.dwSize = sizeof(NET_CREATE_ACCOUNT_DATA);
		memset(szAccountName, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset(szPassword, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset(szUserName, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
	}	
};
typedef NET_CREATE_ACCOUNT_DATA* LPNET_CREATE_ACCOUNT_DATA;

struct NET_CLIENT_VERSION
{
	NET_MSG_GENERIC	nmg;
	int	nPatchProgramVer;
	int	nGameProgramVer;

	NET_CLIENT_VERSION()
		: nPatchProgramVer(0)
		, nGameProgramVer(0)
	{
		memset( this, 0, sizeof(NET_CLIENT_VERSION) );
		nmg.dwSize = sizeof(NET_CLIENT_VERSION);
	}
};
typedef NET_CLIENT_VERSION* LPNET_CLIENT_VERSION;

struct NET_REQ_RAND_KEY
{
	NET_MSG_GENERIC nmg;
	NET_REQ_RAND_KEY()
	{		
		nmg.dwSize = sizeof(NET_REQ_RAND_KEY);
		nmg.nType = NET_MSG_REQ_RAND_KEY;
	}
};
typedef NET_REQ_RAND_KEY* LPNET_REQ_RAND_KEY;


struct NET_ENCRYPT_KEY
{
	NET_MSG_GENERIC nmg;
	
	CHAR			szEncryptKey[ENCRYPT_KEY+1];

	NET_ENCRYPT_KEY()
	{		
		nmg.dwSize = sizeof(NET_ENCRYPT_KEY);
		nmg.nType = NET_MSG_SND_ENCRYPT_KEY;

		memset(szEncryptKey, 0, sizeof(CHAR) * (ENCRYPT_KEY+1));
	}
};
typedef NET_ENCRYPT_KEY* LPNET_ENCRYPT_KEY;

/*
struct NET_MSG_PING
{
	NET_MSG_GENERIC nmg;
	DWORD			stime;
	NET_MSG_PING()
	{
		memset(this, 0, sizeof(NET_MSG_PING));
		nmg.dwSize = sizeof(NET_MSG_PING);
	}
};
typedef NET_MSG_PING* LPNET_MSG_PING;
*/
// Full ServerInfo 
struct NET_SERVER_INFO
{
	NET_MSG_GENERIC nmg;
	G_SERVER_INFO	gsi;
	NET_SERVER_INFO()
	{
		memset(this, 0, sizeof(NET_SERVER_INFO));
		nmg.dwSize = sizeof(NET_SERVER_INFO);
	}
};
typedef NET_SERVER_INFO* LPNET_SERVER_INFO;

struct NET_SERVER_CUR_INFO
{
	NET_MSG_GENERIC		nmg;
	G_SERVER_CUR_INFO	gsci;
	NET_SERVER_CUR_INFO()
	{
		memset(this, 0, sizeof(NET_SERVER_CUR_INFO));
		nmg.dwSize = sizeof(NET_SERVER_CUR_INFO);
	}
};
typedef NET_SERVER_CUR_INFO* LPNET_SERVER_CUR_INFO;

struct NET_SERVER_CHANNEL_INFO
{
	NET_MSG_GENERIC		nmg;
	int nServerGroup;
	int nChannel;
	int nChannelCurrentUser;
	int nChannelMaxUser;
	bool bPk;
	
	NET_SERVER_CHANNEL_INFO()
	{		
		nmg.nType = NET_MSG_SND_CHANNEL_STATE;
		nmg.dwSize = sizeof(NET_SERVER_CHANNEL_INFO);
		
		nServerGroup        = 0;
		nChannel            = 0;
		nChannelCurrentUser = 0;
		nChannelMaxUser     = 0;
		bPk                 = true;
	}
};
typedef NET_SERVER_CHANNEL_INFO* LPNET_SERVER_CHANNEL_INFO;

struct NET_SERVER_CHANNEL_FULL_INFO
{
	NET_MSG_GENERIC		nmg;
	int nServerGroup;
	int nChannel;
	bool bChannelFull;
	int nChannelCurrentUser;

	NET_SERVER_CHANNEL_FULL_INFO()
	{		
		nmg.nType = NET_MSG_SND_CHANNEL_FULL_STATE;
		nmg.dwSize = sizeof(NET_SERVER_CHANNEL_FULL_INFO);

		nServerGroup = 0;
		nChannel     = 0;
		bChannelFull = FALSE;
		nChannelCurrentUser = 0;
	}
};
typedef NET_SERVER_CHANNEL_INFO* LPNET_SERVER_CHANNEL_INFO;

struct NET_CUR_INFO_LOGIN
{
	NET_MSG_GENERIC			nmg;
	G_SERVER_CUR_INFO_LOGIN gscil;
	NET_CUR_INFO_LOGIN()
	{
        nmg.nType = EMNET_MSG(0);		
		nmg.dwSize = sizeof(NET_CUR_INFO_LOGIN);
	}
};
typedef NET_CUR_INFO_LOGIN* LPNET_CUR_INFO_LOGIN;

struct NET_RECONNECT_FILED
{
	NET_MSG_GENERIC	nmg;
	NET_RECONNECT_FILED()
	{
		nmg.dwSize = sizeof(NET_RECONNECT_FILED);
		nmg.nType = NET_MSG_RECONNECT_FIELD;
	}
};
typedef NET_RECONNECT_FILED* LPNET_RECONNECT_FILED;

struct NET_NEW_CHA
{
	NET_MSG_GENERIC		nmg;

	int					nIndex;		//	�ɸ��� ����. ( class )
	WORD				wSchool;	//	�б�.
	WORD				wFace;		//	��.
	WORD				wHair;		//	��� ��Ÿ��.
	WORD				wHairColor;	//	��� �÷�.
	WORD				wSex;		//	����
	char				szChaName[CHR_ID_LENGTH+1];

	NET_NEW_CHA() :
		nIndex(0),
		wSchool(0),
		wFace(0),
		wHair(0),
		wHairColor(0),
		wSex(0)
	{
		nmg.dwSize = sizeof(NET_NEW_CHA);
		nmg.nType = NET_MSG_CHA_NEW;
		memset(szChaName, 0, sizeof(char) * (CHR_ID_LENGTH+1));
	}
};
typedef NET_NEW_CHA* LPNET_NEW_CHA;

struct NET_NEW_CHA_FB
{
	NET_MSG_GENERIC		nmg;
	int					nResult;
	int					nChaNum;
    WORD                wChaRemain;
	int					nExtremeM; // �ذ��� ���� ���� ���� ����
	int					nExtremeW; // �ذ��� ���� ���� ���� ����
	NET_NEW_CHA_FB()
	{
		nmg.dwSize  = sizeof(NET_NEW_CHA_FB);
		nmg.nType  = NET_MSG_CHA_NEW_FB;
        nChaNum    = 0;
        wChaRemain = 0;
	}
};
typedef NET_NEW_CHA_FB* LPNET_NEW_CHA_FB;

// �κ񿡼� ���÷��� �� ĳ������ ���������� ��û�Ѵ�.
struct NET_CHA_REQ_BA_INFO
{
	NET_MSG_GENERIC nmg;
	// int             nChannel; // ����ڰ� �����Ϸ��� ä��

	NET_CHA_REQ_BA_INFO()
	{
		nmg.dwSize = sizeof(NET_CHA_REQ_BA_INFO);
		nmg.nType = NET_MSG_REQ_CHA_BAINFO;
		// nChannel  = 0;
	}
};
typedef NET_CHA_REQ_BA_INFO* LPNET_CHA_REQ_BA_INFO;

// ���� �������� �ش� ������ ���� ĳ������ ��ȣ�� �Ѱ��ش�.
struct NET_CHA_BBA_INFO
{
	NET_MSG_GENERIC		nmg;
	int					nChaSNum; // ���� �� �������� ������ ĳ���� ����
	int					nChaNum[MAX_ONESERVERCHAR_NUM];
	NET_CHA_BBA_INFO()
	{
		memset(this, 0, sizeof(NET_CHA_BBA_INFO));
		nmg.dwSize = sizeof(NET_CHA_BBA_INFO);
		nChaSNum = 0;
	}
};
typedef NET_CHA_BBA_INFO* LPNET_CHA_BBA_INFO;

// �ش� ĳ������ ������ ��û�Ѵ�.
struct NET_CHA_BA_INFO
{
	NET_MSG_GENERIC		nmg;
	int					nChaNum;
	NET_CHA_BA_INFO()
	{		
		nmg.dwSize = sizeof(NET_CHA_BA_INFO);
		nChaNum = 0;
	}
};
typedef NET_CHA_BA_INFO* LPNET_CHA_BA_INFO;

struct NET_CHA_DEL
{
	NET_MSG_GENERIC		nmg;
	INT					nChaNum; // ������ ĳ���� ��ȣ
	char				szPass2[USR_PASS_LENGTH+1];
	NET_CHA_DEL()
	{		
		nmg.dwSize = sizeof(NET_CHA_DEL);
		nChaNum = 0;
		memset(szPass2, 0, sizeof(char) * (USR_PASS_LENGTH+1));
	}
};
typedef NET_CHA_DEL* LPNET_CHA_DEL;

struct DAUM_NET_CHA_DEL
{
	NET_MSG_GENERIC		nmg;
	INT					nChaNum; // ������ ĳ���� ��ȣ
	
	DAUM_NET_CHA_DEL()
	{
		nmg.dwSize = sizeof(DAUM_NET_CHA_DEL);
		nChaNum = 0;
	}
};
typedef DAUM_NET_CHA_DEL* LPDAUM_NET_CHA_DEL;

/**
 * ĳ���� �������� Client->Agent
 * \return 
 */
struct TERRA_NET_CHA_DEL
{
	NET_MSG_GENERIC		nmg;
	INT					nChaNum; // ������ ĳ���� ��ȣ
	
	TERRA_NET_CHA_DEL()
	{
		nmg.dwSize = sizeof(TERRA_NET_CHA_DEL);
		nChaNum = 0;
	}
};
typedef TERRA_NET_CHA_DEL* LPTERRA_NET_CHA_DEL;

/**
 * ĳ���� �������� Client->Agent
 * \return 
 */
struct GSP_NET_CHA_DEL
{
	NET_MSG_GENERIC		nmg;
	INT					nChaNum; // ������ ĳ���� ��ȣ
	
	GSP_NET_CHA_DEL()
	{
		nmg.dwSize = sizeof(GSP_NET_CHA_DEL);
		nmg.nType = GSP_NET_MSG_CHA_DEL;
		nChaNum = 0;
	}
};
typedef GSP_NET_CHA_DEL* LPGSP_NET_CHA_DEL;

/**
 * ĳ���� �������� Client->Agent
 * \return 
 */
struct EXCITE_NET_CHA_DEL
{
	NET_MSG_GENERIC		nmg;
	INT					nChaNum; // ������ ĳ���� ��ȣ
	
	EXCITE_NET_CHA_DEL()
	{
		nmg.dwSize = sizeof(EXCITE_NET_CHA_DEL);
		nChaNum = 0;
	}
};
typedef EXCITE_NET_CHA_DEL* LPEXCITE_NET_CHA_DEL;

/**
 * ĳ���� �������� Client->Agent
 * \return 
 */
struct JAPAN_NET_CHA_DEL
{
	NET_MSG_GENERIC		nmg;
	INT					nChaNum; // ������ ĳ���� ��ȣ
	
	JAPAN_NET_CHA_DEL()
	{
		nmg.dwSize = sizeof(JAPAN_NET_CHA_DEL);
		nChaNum = 0;
	}
};
typedef JAPAN_NET_CHA_DEL* LPJAPAN_NET_CHA_DEL;

/**
 * ĳ���� �������� Client->Agent
 * \return 
 */
struct GS_NET_CHA_DEL
{
	NET_MSG_GENERIC		nmg;
	INT					nChaNum; // ������ ĳ���� ��ȣ
	char				szPass2[GS_USER_PASS+1];

	GS_NET_CHA_DEL()
	{		
		nmg.dwSize = sizeof(GS_NET_CHA_DEL);
		nChaNum = 0;
		memset(szPass2, 0, sizeof(char) * (GS_USER_PASS+1));
	}
};
typedef GS_NET_CHA_DEL* LPGS_NET_CHA_DEL;

struct NET_CHA_DEL_FB
{
	NET_MSG_GENERIC		nmg;	
	INT					nChaNum;	// ������ ĳ���� ��ȣ
	INT					nExtremeM;	// �ذ��� ���� ���� ���� ����
	INT					nExtremeW;	// �ذ��� ���� ���� ���� ����
    WORD                wChaRemain; // ���� ĳ���� ���� ����
	NET_CHA_DEL_FB()
	{
		nmg.dwSize  = sizeof(NET_CHA_DEL_FB);
		nChaNum		= 0;
        wChaRemain	= 0;
		nExtremeM	= 0;
		nExtremeW	= 0;
	}
};
typedef NET_CHA_DEL_FB* LPNET_CHA_DEL_FB;

// �������� �޽���
// Client -> Agent Server
struct NET_GAME_JOIN
{
	NET_MSG_GENERIC		nmg;
	INT					nChaNum; // ���� ������ ĳ���� ��ȣ
	NET_GAME_JOIN()
	{
		nmg.dwSize = sizeof(NET_GAME_JOIN);
		nChaNum = 0;
	}
};
typedef NET_GAME_JOIN* LPNET_GAME_JOIN;

enum EMGAME_JOINTYPE
{
	EMJOINTYPE_FIRST	= 0,	//	�ɸ��� ó�� ����������.
	EMJOINTYPE_MOVEMAP	= 1,	//	���̵��� ������ �̵��ϰ� �Ǿ� �ٽ� ����.
	EMJOINTYPE_REBIRTH	= 2,	//	��Ȱ���� ������ �̵��ϰ� �Ǿ� �ٽ� ����.
};

// �������� �޽���
// Agent -> Field
struct NET_GAME_JOIN_FIELDSVR
{
	NET_MSG_GENERIC		nmg;

	EMGAME_JOINTYPE		emType;
	DWORD				dwSlotAgentClient;

	char				szUID[USR_ID_LENGTH+1];
	INT					nUserNum;
	DWORD				dwUserLvl;

	INT					nChaNum;
	DWORD				dwGaeaID;

	SNATIVEID			sStartMap;
	DWORD				dwStartGate;
	D3DXVECTOR3			vStartPos;

	DWORD				dwActState;

    __time64_t          tPREMIUM;
    __time64_t          tCHATBLOCK;

	int					dwThaiCCafeClass;
	int					nMyCCafeClass;					// �����̽þ� PC�� �̺�Ʈ

	SChinaTime			sChinaTime;
	SEventTime			sEventTime;
	SVietnamGainSystem	sVietnamGainSystem;


	BOOL				bUseArmSub;

	NET_GAME_JOIN_FIELDSVR() :
		emType(EMJOINTYPE_FIRST),
		dwSlotAgentClient(0),
		
		nUserNum(0),
		dwUserLvl(0),

		nChaNum(0),
		dwGaeaID(0),
		
		dwStartGate(UINT_MAX),
		vStartPos(0,0,0),

		dwActState(NULL),
		tPREMIUM(0),
		tCHATBLOCK(0),

		bUseArmSub(FALSE),

		dwThaiCCafeClass(0),
		nMyCCafeClass(0)
	{
		nmg.dwSize	= sizeof(NET_GAME_JOIN_FIELDSVR);
		nmg.nType	= MET_MSG_GAME_JOIN_FIELDSVR;

		memset( szUID, 0, sizeof(char)*(USR_ID_LENGTH+1) );

		sChinaTime.Init();
		sEventTime.Init();
		sVietnamGainSystem.Init();
	}
};
typedef NET_GAME_JOIN_FIELDSVR* LPNET_GAME_JOIN_FIELDSVR;

struct NET_GAME_JOIN_FIELDSVR_FB
{
	NET_MSG_GENERIC nmg;

	EMGAME_JOINTYPE	emType;
	DWORD			dwSlotFieldAgent;

	DWORD			dwFieldSVR;
	DWORD           dwGaeaID;
	INT				nChaNum;

	NET_GAME_JOIN_FIELDSVR_FB () :
		emType(EMJOINTYPE_FIRST),
		dwSlotFieldAgent(0),

		dwFieldSVR(0),
		dwGaeaID(0),
		nChaNum(0)
	{
		nmg.nType   = MET_MSG_GAME_JOIN_FIELDSVR_FB;
		nmg.dwSize   = sizeof(*this);
	};	
};
typedef NET_GAME_JOIN_FIELDSVR_FB* LPNET_GAME_JOIN_FIELDSVR_FB;

struct NET_CONNECT_CLIENT_TO_FIELD
{
	NET_MSG_GENERIC	nmg;
	EMGAME_JOINTYPE	emType;
	DWORD			dwGaeaID;
	DWORD			dwSlotFieldAgent;
	int				nServicePort;
	CHAR			szServerIP[MAX_IP_LENGTH+1];

	NET_CONNECT_CLIENT_TO_FIELD() 
		: emType(EMJOINTYPE_FIRST)
	{
		nmg.dwSize = sizeof(NET_CONNECT_CLIENT_TO_FIELD);
		nmg.nType = NET_MSG_CONNECT_CLIENT_FIELD;
		dwSlotFieldAgent = 0;
	}
};
typedef NET_CONNECT_CLIENT_TO_FIELD* LPNET_CONNECT_CLIENT_TO_FIELD;

//	Ŭ���̾�Ʈ�� ������ Field Server�� ������ �ڽ��� ID Ȯ�� ��û.
struct NET_GAME_JOIN_FIELD_IDENTITY
{
	NET_MSG_GENERIC		nmg;
	EMGAME_JOINTYPE		emType;

	DWORD				dwGaeaID; 
	DWORD				dwSlotFieldAgent;
	CRYPT_KEY			ck;

	NET_GAME_JOIN_FIELD_IDENTITY () :
		emType(EMJOINTYPE_FIRST),

		dwGaeaID(0),
		dwSlotFieldAgent(0)
	{
		nmg.dwSize = sizeof(*this);
		nmg.nType = NET_MSG_JOIN_FIELD_IDENTITY;
	}
};
typedef NET_GAME_JOIN_FIELD_IDENTITY* LPNET_GAME_JOIN_FIELD_IDENTITY;

// ���������Ϸ�
// Field->Session
struct NET_GAME_JOIN_OK
{
	NET_MSG_GENERIC		nmg;
	INT					nUsrNum; // ����� ������ȣ(DB)	
	INT					nChaNum; // ĳ���͹�ȣ(DB)
	DWORD				dwGaeaID; // ���̾� ID
	CHAR				szUserID[USR_ID_LENGTH+1]; // ����� ID	
	CHAR				szChaName[CHR_ID_LENGTH+1]; // ĳ���͸�

	NET_GAME_JOIN_OK()
	{		
		nmg.dwSize = sizeof(NET_GAME_JOIN_OK);
		nmg.nType = NET_MSG_GAME_JOIN_OK;
		nUsrNum = -1;
		nChaNum = -1;
		dwGaeaID = -1;
		memset(szUserID, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset(szChaName, 0, sizeof(CHAR) * (CHR_ID_LENGTH+1));
	}
};
typedef NET_GAME_JOIN_OK* LPNET_GAME_JOIN_OK;

// ä�ø޽���
struct NET_CHAT
{
	NET_MSG_GENERIC	nmg;
	EMNET_MSG_CHAT	emType;						// ä�� Ÿ��
	CHAR			szName[CHR_ID_LENGTH+1];		// Ư�� ĳ���͸� or ����ڸ�
	CHAR			szChatMsg[CHAT_MSG_SIZE];	// ä�ø޽���
	SITEMLINK		sITEMLINK;

	NET_CHAT () :
		emType(CHAT_TYPE_NORMAL)
	{
		sITEMLINK = SITEMLINK();
		nmg.nType = NET_MSG_CHAT;
		nmg.dwSize = sizeof(NET_CHAT);

		memset(szName,    0, sizeof(CHAR) * (CHR_ID_LENGTH+1));
		memset(szChatMsg, 0, sizeof(CHAR) * (CHAT_MSG_SIZE));
	}
};
typedef NET_CHAT* LPNET_CHAT;

// ������ ä�ø޽���
// ��Ʈ�����α׷�->���Ǽ���
struct NET_CHAT_CTRL
{
	NET_MSG_GENERIC nmg;
	EMNET_MSG_CHAT	emType;
	CHAR			szServerIP[MAX_IP_LENGTH+1];
	INT				nServicePort;
	CHAR			szChatMsg[GLOBAL_CHAT_MSG_SIZE+1];

	NET_CHAT_CTRL()
	{		
		nmg.nType    = NET_MSG_CHAT;
		nmg.dwSize    = sizeof(NET_CHAT_CTRL);
		emType	     = CHAT_TYPE_CTRL_GLOBAL;
		nServicePort = 0;
		memset(szServerIP, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		memset(szChatMsg, 0, sizeof(CHAR) * (GLOBAL_CHAT_MSG_SIZE+1));
	}
};
typedef NET_CHAT_CTRL* LPNET_CHAT_CTRL;

struct NET_CHAT_CTRL2
{
	NET_MSG_GENERIC nmg;
	EMNET_MSG_CHAT	emType;	
	int				nSvrGrp;
	CHAR			szChatMsg[GLOBAL_CHAT_MSG_SIZE+1];	

	NET_CHAT_CTRL2()
	{
		nmg.nType	= NET_MSG_CHAT;
		nmg.dwSize	= sizeof(NET_CHAT_CTRL2);
		emType		= CHAT_TYPE_CTRL_GLOBAL2;
		nSvrGrp		= 0;
		memset(szChatMsg, 0, sizeof(CHAR) * (GLOBAL_CHAT_MSG_SIZE+1));
	}
};
typedef NET_CHAT_CTRL2* LPNET_CHAT_CTRL2;

// ä�ø޽��� FB
struct NET_CHAT_FB
{
	NET_MSG_GENERIC nmg;
	EMNET_MSG_CHAT	emType;						// ä�� Ÿ��
	CHAR			szName[CHR_ID_LENGTH+1];		// ������� ĳ���͸� or ����ڸ�
	CHAR			szChatMsg[CHAT_MSG_SIZE+1];	// ä�ø޽���
	SITEMLINK		sITEMLINK;

	NET_CHAT_FB ()
	{
		sITEMLINK = SITEMLINK();
		nmg.nType = NET_MSG_CHAT_FB;
		nmg.dwSize = sizeof(NET_CHAT_FB);
		emType = CHAT_TYPE_NORMAL;
		memset(szName, 0, sizeof(CHAR) * (CHR_ID_LENGTH+1));
		memset(szChatMsg, 0, sizeof(CHAR) * (CHAT_MSG_SIZE+1));
	}
};
typedef NET_CHAT_FB* LPNET_CHAT_FB;

// �����ڿ� �޽��� FB
struct NET_CHAT_CTRL_FB
{
	NET_MSG_GENERIC nmg;
	EMNET_MSG_CHAT	emType;						// ä�� Ÿ��
	CHAR			szChatMsg[GLOBAL_CHAT_MSG_SIZE+1];	// ä�ø޽���
	SITEMLINK		sITEMLINK;
	//add staffchat
	bool bStaff;

	NET_CHAT_CTRL_FB ()
	{
		sITEMLINK = SITEMLINK();
		nmg.nType = NET_MSG_CHAT_CTRL_FB;
		nmg.dwSize = sizeof(NET_CHAT_CTRL_FB);
		emType = CHAT_TYPE_GLOBAL;
		memset(szChatMsg, 0, sizeof(CHAR) * (GLOBAL_CHAT_MSG_SIZE+1));
		//add staffchat
		bStaff = false;
	}
};
typedef NET_CHAT_FB* LPNET_CHAT_FB;

struct NET_EVENT_LOTTERY
{
	NET_MSG_GENERIC nmg;
	CHAR szLotteryName[11];

	NET_EVENT_LOTTERY()
	{		
		nmg.nType = NET_MSG_EVENT_LOTTERY;
		nmg.dwSize = sizeof(NET_EVENT_LOTTERY);
		memset(szLotteryName, 0, sizeof(CHAR) * 11);
	}
};
typedef NET_EVENT_LOTTERY* LPNET_EVENT_LOTTERY;

///////////////////////////////////////////////////////////////////////////////
// �� ��������!! �̺�Ʈ 
// ���ϰ� :
// 1  1 �� P-4 PC                        (1� ��÷�Ǽ̽��ϴ�. ���ϵ帳�ϴ�. �ڼ��� ������ Ȩ�������� ������ �ֽʽÿ�.)
// 2  2 �� ȿ����                        (2� ��÷�Ǽ̽��ϴ�. ���ϵ帳�ϴ�. �ڼ��� ������ Ȩ�������� ������ �ֽʽÿ�.)
// 3  3 �� SONY ��ī                     (3� ��÷�Ǽ̽��ϴ�. ���ϵ帳�ϴ�. �ڼ��� ������ Ȩ�������� ������ �ֽʽÿ�.)
// 4  4 �� ���̸��� MP �÷��̾�          (4� ��÷�Ǽ̽��ϴ�. ���ϵ帳�ϴ�. �ڼ��� ������ Ȩ�������� ������ �ֽʽÿ�.)
// 5  5 �� MAX ���� ��ȭ ��ǰ�� 1 ������ (5� ��÷�Ǽ̽��ϴ�. ���ϵ帳�ϴ�. �ڼ��� ������ Ȩ�������� ������ �ֽʽÿ�.)
// 6  6 �� MBC Game ���� ���� ��ǰ��     (6� ��÷�Ǽ̽��ϴ�. ���ϵ帳�ϴ�. �ڼ��� ������ Ȩ�������� ������ �ֽʽÿ�.) 
// 7  �̹� ������ �Է��� �����          (�̹� ���ǿ� �����ϼ̽��ϴ�.)
// 8  PC �� IP �� �ƴ�                   (PC �� IP �� �ƴմϴ�. ������ PC �濡���� ���� �����մϴ�)
// 9  �̹� ����� ����                   (�̹� ����� �����Դϴ�)
// 10 �˼����� ����                      (����ó���� ������ �߻��Ͽ����ϴ�)
struct NET_EVENT_LOTTERY_FB
{
	NET_MSG_GENERIC nmg;
	int nResult;

	NET_EVENT_LOTTERY_FB()
	{
		nmg.nType = NET_MSG_EVENT_LOTTERY_FB;
		nmg.dwSize = sizeof(NET_EVENT_LOTTERY_FB);
		int nResult = 10;
	}
};
typedef NET_EVENT_LOTTERY_FB* LPNET_EVENT_LOTTERY_FB;

// �⼮�� �̺�Ʈ
struct USER_ATTEND_INFO
{
	__time64_t	tAttendTime;
	int			nComboAttend;
	bool		bAttendReward;

	USER_ATTEND_INFO()
	{
		tAttendTime = 0;
		nComboAttend = 0;
		bAttendReward = false;
	}
};
typedef USER_ATTEND_INFO* LPUSER_ATTEND_INFO;


// Apex ����( ȫ�� )
struct NET_APEX_RETURN
{
	NET_MSG_GENERIC nmg;
	int nReturn;

	NET_APEX_RETURN()
		: nReturn ( 0 )
	{
		nmg.nType  = NET_MSG_APEX_RETURN;
		nmg.dwSize = sizeof(NET_APEX_RETURN);		
	}
};

//! Apex �޼���
struct NET_APEX_DATA
{
	NET_MSG_GENERIC nmg;
	CHAR szData[MAX_APEX_PACKET_LENGTH];
	
	NET_APEX_DATA()
	{
		nmg.nType  = NET_MSG_APEX_DATA;
		nmg.dwSize = sizeof(NET_APEX_DATA);
		memset( szData, 0, sizeof(CHAR) * MAX_APEX_PACKET_LENGTH );
	}
};
typedef NET_APEX_DATA* LPNET_APEX_DATA;

//! Apex �޼���
struct NET_APEX_ANSWER
{
	NET_MSG_GENERIC nmg;
	WORD			wAction;
	WORD			wHour;

	NET_APEX_ANSWER()
	{
		nmg.nType  = NET_MSG_APEX_ANSWER;
		nmg.dwSize = sizeof(NET_APEX_ANSWER);
		wAction = 0;
		wHour = 0;
	}
};
typedef NET_APEX_ANSWER* LPNET_APEX_ANSWER;

//! nProtect GameGuard Auth ��û �޽���
struct NET_GAMEGUARD_AUTH
{
	NET_MSG_GENERIC nmg;
	GG_AUTH_DATA ggad;

	NET_GAMEGUARD_AUTH()
	{
		nmg.nType  = NET_MSG_GAMEGUARD_AUTH;
		nmg.dwSize = sizeof(NET_GAMEGUARD_AUTH);
		ggad.dwIndex = 0;
		ggad.dwValue1 = 0;
		ggad.dwValue2 = 0;
		ggad.dwValue3 = 0;
	}	
};
typedef NET_GAMEGUARD_AUTH* LPNET_GAMEGUARD_AUTH;

//! nProtect GameGuard Auth Answer �޽���
struct NET_GAMEGUARD_ANSWER
{
	NET_MSG_GENERIC nmg;
	GG_AUTH_DATA ggad;
	
	CHAR	szEnCrypt[ENCRYPT_KEY+1];


	NET_GAMEGUARD_ANSWER()
	{
		nmg.nType  = NET_MSG_GAMEGUARD_ANSWER;
		nmg.dwSize = sizeof(NET_GAMEGUARD_ANSWER);
		ggad.dwIndex = 0;
		ggad.dwValue1 = 0;
		ggad.dwValue2 = 0;
		ggad.dwValue3 = 0;
		SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
	}	
};
typedef NET_GAMEGUARD_ANSWER* LPNET_GAMEGUARD_ANSWER;

//! nProtect GameGuard Auth ��û �޽��� #1
struct NET_GAMEGUARD_AUTH_1
{
	NET_MSG_GENERIC nmg;
	GG_AUTH_DATA ggad;

	NET_GAMEGUARD_AUTH_1()
	{
		nmg.nType  = NET_MSG_GAMEGUARD_AUTH_1;
		nmg.dwSize = sizeof(NET_GAMEGUARD_AUTH_1);
		ggad.dwIndex = 0;
		ggad.dwValue1 = 0;
		ggad.dwValue2 = 0;
		ggad.dwValue3 = 0;
	}	
};
typedef NET_GAMEGUARD_AUTH_1* LPNET_GAMEGUARD_AUTH_1;

//! nProtect GameGuard Auth Answer �޽��� #1
struct NET_GAMEGUARD_ANSWER_1
{
	NET_MSG_GENERIC nmg;
	GG_AUTH_DATA ggad;

	NET_GAMEGUARD_ANSWER_1()
	{
		nmg.nType  = NET_MSG_GAMEGUARD_ANSWER_1;
		nmg.dwSize = sizeof(NET_GAMEGUARD_ANSWER_1);
		ggad.dwIndex = 0;
		ggad.dwValue1 = 0;
		ggad.dwValue2 = 0;
		ggad.dwValue3 = 0;
	}	
};
typedef NET_GAMEGUARD_ANSWER_1* LPNET_GAMEGUARD_ANSWER_1;

//! nProtect GameGuard Auth ��û �޽��� #2
struct NET_GAMEGUARD_AUTH_2
{
	NET_MSG_GENERIC nmg;
	GG_AUTH_DATA ggad;

	NET_GAMEGUARD_AUTH_2()
	{
		nmg.nType  = NET_MSG_GAMEGUARD_AUTH_2;
		nmg.dwSize = sizeof(NET_GAMEGUARD_AUTH_2);
		ggad.dwIndex = 0;
		ggad.dwValue1 = 0;
		ggad.dwValue2 = 0;
		ggad.dwValue3 = 0;
	}	
};
typedef NET_GAMEGUARD_AUTH_2* LPNET_GAMEGUARD_AUTH_2;

//! nProtect GameGuard Auth Answer �޽��� #2
struct NET_GAMEGUARD_ANSWER_2
{
	NET_MSG_GENERIC nmg;
	GG_AUTH_DATA ggad;

	NET_GAMEGUARD_ANSWER_2()
	{
		nmg.nType  = NET_MSG_GAMEGUARD_ANSWER_2;
		nmg.dwSize = sizeof(NET_GAMEGUARD_ANSWER_2);
		ggad.dwIndex = 0;
		ggad.dwValue1 = 0;
		ggad.dwValue2 = 0;
		ggad.dwValue3 = 0;
	}	
};
typedef NET_GAMEGUARD_ANSWER_2* LPNET_GAMEGUARD_ANSWER_2;

// ������ id �߰�
struct NET_UPDATE_TRACINGCHAR_ALL
{
	NET_MSG_GENERIC nmg;
	STracingData tracingData[MAX_TRACING_NUM];

	NET_UPDATE_TRACINGCHAR_ALL () 
	{
		nmg.nType = NET_MSG_UPDATE_TRACING_ALL;
		nmg.dwSize = sizeof(NET_UPDATE_TRACINGCHAR_ALL);
		
		BYTE i;
		for( i = 0; i < 10; i++ )
		{
			tracingData[i].dwUserNum      = 0;
			tracingData[i].strAccount     = "";
			tracingData[i].dwOnLine		  = 255;
			tracingData[i].nConnectServer = -1;
		}
	}
};

// ������ id �߰�
struct NET_UPDATE_TRACINGCHAR
{
	NET_MSG_GENERIC nmg;
	BYTE		 updateNum;
	STracingData tracingData;

	NET_UPDATE_TRACINGCHAR () :
		updateNum(0)
	{
		nmg.nType = NET_MSG_UPDATE_TRACING_CHARACTER;
		nmg.dwSize = sizeof(NET_UPDATE_TRACINGCHAR);

		tracingData.dwUserNum      = 0;
		tracingData.strAccount     = "";
		tracingData.dwOnLine		  = 255;
		tracingData.nConnectServer = -1;
	}
};


// �������� ������ �α׸� agent������ ������.
struct NET_LOG_UPDATE_TRACINGCHAR
{
	NET_MSG_GENERIC nmg;
	CHAR szAccount[USR_ID_LENGTH+1];
	int  nUserNum;
	CHAR szLogMsg[TRACING_LOG_SIZE];

	NET_LOG_UPDATE_TRACINGCHAR () :
			nUserNum(-1)
	{
		nmg.nType = NET_MSG_LOG_UPDATE_TRACING_CHARACTER;
		nmg.dwSize = sizeof(NET_LOG_UPDATE_TRACINGCHAR);
		memset(szAccount, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset(szLogMsg, 0, sizeof(CHAR) * (TRACING_LOG_SIZE));
	}
};

//< �ʵ�->Ŭ���̾�Ʈ ���� �������� �ƴ����� ������.
struct NET_TRACING_CHARACTER
{
	NET_MSG_GENERIC nmg;
	bool			bTracingChar;

	NET_TRACING_CHARACTER () :
		bTracingChar(FALSE)
	{
		nmg.nType = NET_MSG_TRACING_CHARACTER;
		nmg.dwSize = sizeof(NET_TRACING_CHARACTER);

	}
};


// ������ �������� �ִ� ������ȣ�� �������� ���� ����ü
struct VIEWLOGITEMEXCHANGEMAX
{
    LONGLONG llMaxNum;
    int  nMakeType;
    WORD wItemMain;
    WORD wItemSub;

    VIEWLOGITEMEXCHANGEMAX()
    {
        llMaxNum  = 0;
        nMakeType = 0;
        wItemMain = 0;
        wItemSub  = 0;
    }
};

// AGT->Session ���� LoginMaintenance On/Off
struct SNET_SERVER_LOGINMAINTENANCE_SESSION
{
	NET_MSG_GENERIC nmg;
	bool	bOn;

	SNET_SERVER_LOGINMAINTENANCE_SESSION ()
	{
		nmg.nType = NET_MSG_GCTRL_SERVER_LOGINMAINTENANCE_SESSION;
		nmg.dwSize = sizeof( SNET_SERVER_LOGINMAINTENANCE_SESSION );
		bOn = false;
	}
};

struct SNET_SERVER_LOGINMAINTENANCE_LOGIN
{
	NET_MSG_GENERIC nmg;
	bool	bOn;

	SNET_SERVER_LOGINMAINTENANCE_LOGIN ()
	{
		nmg.nType = NET_MSG_GCTRL_SERVER_LOGINMAINTENANCE_LOGIN;
		nmg.dwSize = sizeof( SNET_SERVER_LOGINMAINTENANCE_LOGIN );
		bOn = false;
	}
};

// Shop ���� ������ �����۸���Ʈ�� �������� ���� ����ü
// char 21 �ڸ�
#define PURKEY_LENGTH 30

struct SHOPPURCHASE
{
    CString strPurKey;
    WORD    wItemMain;
    WORD    wItemSub;

    SHOPPURCHASE()
    {
        wItemMain = 0;
        wItemSub  = 0;
    }

	SHOPPURCHASE( const SHOPPURCHASE &value ) 
		: strPurKey(value.strPurKey)
		, wItemMain(value.wItemMain)
		, wItemSub(value.wItemSub)
    {
	}

	SHOPPURCHASE& operator= ( const SHOPPURCHASE &value )
	{
		strPurKey = value.strPurKey;
		wItemMain = value.wItemMain;
		wItemSub = value.wItemSub;

		return *this;
	}
};

struct ITEMSHOP
{
   // CString strPurKey;
    SNATIVEID	sID;
	WORD	wItemPrice;
	WORD	wItemStock;
	WORD	wItemCtg;
	WORD	wItemCurrency;
	CString	strItemNum;

    ITEMSHOP()
    {
		sID.wMainID = 0;
        sID.wSubID  = 0;
        wItemPrice  = 0;
        wItemStock  = 0;
		wItemCtg	= 0;
		wItemCurrency = 0;
    }

	ITEMSHOP( const ITEMSHOP &value ) 
		: sID(value.sID)
		, wItemPrice(value.wItemPrice)
		, wItemStock(value.wItemStock)
		, wItemCtg(value.wItemCtg)
		, wItemCurrency(value.wItemCurrency)
		, strItemNum(value.strItemNum)
    {
	}

	ITEMSHOP& operator= ( const ITEMSHOP &value )
	{
		sID = value.sID;
		wItemPrice = value.wItemPrice;
		wItemStock = value.wItemStock;
		wItemCtg   = value.wItemCtg;
		wItemCurrency = value.wItemCurrency;
		strItemNum = value.strItemNum;

		return *this;
	}
};
///////////////////////////////////////////////////////////////////////////////
// Global Function

/// ��Ʈ��ũ �ʱ�ȭ
int	NET_InitializeSocket(void);
/// ��ũ��ũ ����
void NET_CloseSocket(void);
/// Winsock2 ���� �ڵ�->���ڿ� �Լ�
CString NET_ERROR_STRING(int nCode);

/******************************************************************************
* GameGuard ���� �ڵ� üũ �Լ�
* \param dwErrCode nProtect GameGuard �����ڵ�
*/
std::string GetGameGuardErrorString(DWORD dwErrCode);

#endif // S_NETGLOBAL_H_

//                             ,|     
//                           //|                              ,|
//                         //,/                             -~ |
//                       // / |                         _-~   /  ,
//                     /'/ / /                       _-~   _/_-~ |
//                    ( ( / /'                   _ -~     _-~ ,/'
//                     \~\/'/|             __--~~__--\ _-~  _/,
//             ,,)))))));, \/~-_     __--~~  --~~  __/~  _-~ /
//          __))))))))))))));,>/\   /        __--~~  \-~~ _-~
//         -\(((((''''(((((((( >~\/     --~~   __--~' _-~ ~|
//--==//////((''  .     `)))))), /     ___---~~  ~~\~~__--~ 
//        ))| @    ;-.     (((((/           __--~~~'~~/
//        ( `|    /  )      )))/      ~~~~~__\__---~~__--~~--_
//           |   |   |       (/      ---~~~/__-----~~  ,;::'  \         ,
//           o_);   ;        /      ----~~/           \,-~~~\  |       /|
//                 ;        (      ---~~/         `:::|      |;|      < >
//                |   _      `----~~~~'      /      `:|       \;\_____// 
//          ______/\/~    |                 /        /         ~------~
//        /~;;.____/;;'  /          ___----(   `;;;/               
//       / //  _;______;'------~~~~~    |;;/\    /          
//      //  | |                        /  |  \;;,\              
//     (<_  | ;                      /',/-----'  _>
//      \_| ||_                     //~;~~~~~~~~~ 
//          `\_|                   (,~~ 
//                                  \~\ 
//                                   ~~ 
// Fast Fast More Fast~! 
// Fly To The Moon~! 
// Fly Fly~!
// Jgkim
