#pragma once
#include <set>
#include "GLCharDefine.h"

#define RANKING_TWNUM	100
#define CHAR_NAME		33
#define SW_TOWER_NUM	20 //tower numbers for packets

#define CTF_MAX_REVIVE_GATE			3 

enum SW_SCHOOL_DATA
{
	SW_SCHOOL_SG = 0,
	SW_SCHOOL_MP = 1,
	SW_SCHOOL_PHX = 2,
	SW_SCHOOL_MAX = 3
};
struct TW_PREVIOUS_RECORD
{
	WORD				wSchool;
	float				fElapsedTime;
	CString				strName;

	TW_PREVIOUS_RECORD()
		: wSchool( SW_SCHOOL_MAX )
		, fElapsedTime( 0.0f )
		, strName("")
	{
	}
};

struct TW_TIME
{
	DWORD dwWeekDay; // �� �� ȭ �� �� �� �� 1 2 3 4 5 6 7
	DWORD dwStartTime; // �ð� (0-23����)
	DWORD dwStartMin;

	TW_TIME () :
		dwWeekDay(0),
		dwStartTime(0),
		dwStartMin(0)
	{
	}
};

struct SSCHOOLWAR_TOWER_STATUS
{
	SNATIVEID	sCROWID;
	SW_SCHOOL_DATA	emHOLDER;
	SW_SCHOOL_DATA	emHOLDER_OLD;

	SSCHOOLWAR_TOWER_STATUS()
		: emHOLDER ( SW_SCHOOL_MAX )
		, emHOLDER_OLD( SW_SCHOOL_MAX )
	{
	}
};

typedef std::map< DWORD, SSCHOOLWAR_TOWER_STATUS >	SCHOOLWAR_TOWER_STATUS_MAP;
typedef SCHOOLWAR_TOWER_STATUS_MAP::iterator		SCHOOLWAR_TOWER_STATUS_MAP_ITER;

typedef std::vector<SSCHOOLWAR_TOWER_STATUS>		SCHOOLWAR_TOWER_STATUS_VEC;
typedef SCHOOLWAR_TOWER_STATUS_VEC::iterator		SCHOOLWAR_TOWER_STATUS_VEC_ITER;

struct STWP_RANK_EX
{
	EMCHARINDEX	dwClass;
	WORD	wSchool;
	WORD	wCharRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	DWORD	dwDamageNum;
	DWORD	dwHealNum;
	char	szCharName[CHAR_NAME+1];

	STWP_RANK_EX()
		: dwClass(GLCI_FIGHTER_M)
		, wSchool(0)
		, wCharRanking(0)
		, wKillNum (0)
		, wDeathNum(0)
		, wResuNum(0)
		, dwDamageNum(0)
		, dwHealNum(0)
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_NAME+1) );
	}

	void Init()
	{
		dwClass = GLCI_FIGHTER_M;
		wSchool = 0;
		wCharRanking = 0;
		wKillNum = 0;
		wDeathNum = 0;
		wResuNum = 0;
		dwDamageNum = 0;
		dwHealNum = 0;
		StringCchCopy( szCharName, CHAR_NAME+1, "" );
	}
};

struct STWP_RANK_INFO
{
	int		nIndex;   
	DWORD	dwCharID;
	DWORD	dwGaeaID;
	EMCHARINDEX	dwClass;
	WORD	wSchool;
	WORD	wCharRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	DWORD	dwDamageNum;
	DWORD	dwHealNum;

	char	szCharName[CHAR_NAME+1];	
	
	STWP_RANK_INFO()
        : nIndex ( -1 )
		, dwCharID( 0 )
		, dwGaeaID( 0 )
		, dwClass(GLCI_FIGHTER_M)
		, wSchool(0)
		, wCharRanking(0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, dwDamageNum(0)
		, dwHealNum(0)
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_NAME+1) );
	}

	STWP_RANK_INFO ( const STWP_RANK_INFO &value )
	{
		operator=(value);
	}

	STWP_RANK_INFO& operator = ( const STWP_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwCharID = rvalue.dwCharID;
		dwGaeaID = rvalue.dwGaeaID;
		dwClass = rvalue.dwClass;
		wSchool = rvalue.wSchool;
		wCharRanking = rvalue.wCharRanking;		
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wResuNum = rvalue.wResuNum;
		dwDamageNum = rvalue.dwDamageNum;
		dwHealNum = rvalue.dwHealNum;

		StringCchCopy( szCharName, CHAR_NAME+1, rvalue.szCharName );

		return *this;
	}

	bool operator < ( const STWP_RANK_INFO& sSwPlayerRank )
	{		
		if( ( wKillNum + wResuNum ) >= ( sSwPlayerRank.wKillNum + sSwPlayerRank.wResuNum ) &&
			( dwDamageNum + dwHealNum ) > ( sSwPlayerRank.dwDamageNum + sSwPlayerRank.dwHealNum ) &&
			 wDeathNum <= sSwPlayerRank.wDeathNum )
			return true;
		
		return  false;
	}

	bool operator == ( const STWP_RANK_INFO& sSwPlayerRank ) 
	{
		if ( wKillNum == sSwPlayerRank.wKillNum && wDeathNum == sSwPlayerRank.wDeathNum &&
			dwDamageNum == sSwPlayerRank.dwDamageNum && dwHealNum == sSwPlayerRank.dwHealNum &&
			wResuNum == sSwPlayerRank.wResuNum ) return true;

		return false;

	}
};

struct STWP_RANK
{
	int nIndex;

	EMCHARINDEX	dwClass;
	WORD	wSchool;
	WORD	wCharRanking;
	WORD	wKillNum;
	WORD	wDeathNum;
	WORD	wResuNum;
	DWORD	dwDamageNum;
	DWORD	dwHealNum;

	char szCharName[CHAR_NAME+1];

	STWP_RANK() 
		: nIndex(-1)
		, dwClass(GLCI_FIGHTER_M)
		, wSchool(0)
		, wCharRanking (0)
		, wKillNum(0)
		, wDeathNum(0)
		, wResuNum(0)
		, dwDamageNum(0)
		, dwHealNum(0)
	{
		memset( szCharName, 0, sizeof(char) * (CHAR_NAME+1) );
	}

	STWP_RANK ( const STWP_RANK &value )
	{
		operator=(value);
	}

	STWP_RANK& operator = ( const STWP_RANK& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClass = rvalue.dwClass;
		wSchool = rvalue.wSchool;
		wCharRanking = rvalue.wCharRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wResuNum = rvalue.wResuNum;
		dwDamageNum = rvalue.dwDamageNum;
		dwHealNum = rvalue.dwHealNum;

		StringCchCopy( szCharName, CHAR_NAME+1, rvalue.szCharName );
		return *this;
	}

	STWP_RANK ( const STWP_RANK_INFO &value )
	{
		operator=(value);
	}

	STWP_RANK& operator = ( const STWP_RANK_INFO& rvalue )
	{
		nIndex = rvalue.nIndex;
		dwClass = rvalue.dwClass;
		wSchool = rvalue.wSchool;
		wCharRanking = rvalue.wCharRanking;
		wKillNum = rvalue.wKillNum;
		wDeathNum = rvalue.wDeathNum;
		wResuNum = rvalue.wResuNum;
		dwDamageNum = rvalue.dwDamageNum;
		dwHealNum = rvalue.dwHealNum;

		StringCchCopy( szCharName, CHAR_NAME+1, rvalue.szCharName );
		return *this;
	}
};

typedef std::vector<STWP_RANK_INFO>		TWP_RANK_INFO_VEC;
typedef TWP_RANK_INFO_VEC::iterator		TWP_RANK_INFO_VEC_ITER;

typedef std::vector<STWP_RANK>			TWP_RANK_VEC;
typedef TWP_RANK_VEC::iterator			TWP_RANK_VEC_ITER;

typedef std::map<DWORD,STWP_RANK_INFO>	TWP_RANK_INFO_MAP;
typedef TWP_RANK_INFO_MAP::iterator		TWP_RANK_INFO_MAP_ITER;

typedef std::set<DWORD>					TWP_AWARD_CHAR;	
typedef TWP_AWARD_CHAR::iterator		TWP_AWARD_CHAR_ITER;

struct STWP_AWARD_ITEM
{
	SNATIVEID	nAwardItem[10];
	DWORD		dwAwardLimit;

	STWP_AWARD_ITEM()
		: dwAwardLimit(10)
	{	
		memset( nAwardItem, -1, sizeof( SNATIVEID ) * 10 );
	}
};