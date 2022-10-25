#ifndef ITEMSETLISTOPTION_TEST_
#define ITEMSETLISTOPTION_TEST_

#include <iostream>
#include <windows.h>

#include "../../EngineLib/G-Logic/GLDefine.h"

struct SIDSETITEMLIST
{
	float	fMainID;
	float	fSubID;
	SIDSETITEMLIST(): fMainID( 0.f )
					, fSubID( 0.f )
	{
	}
};

struct SPARTOPTION
{
	DWORD	dwType;
	DWORD	dwValue;
	SPARTOPTION() : 
	dwType( 0 )
	, dwValue( 0 )
	{
	}
};
class CItemSetListsOption
{
public:
	enum
	{
		SLOT_HEADGEAR			=		0,
		SLOT_UPPER ,
		SLOT_LOWER ,
		SLOT_HAND ,
		SLOT_FOOT ,
		SLOT_RHAND ,
		SLOT_LHAND ,
		SLOT_NECK ,
		SLOT_WRIST ,
		SLOT_RFINGER ,
		SLOT_LFINGER ,
		SLOT_RHAND_S ,
		SLOT_LHAND_S ,
		SLOT_VEHICLE ,
		SLOT_WING,
		SLOT_AURA,
		SLOT_BELT,
		SLOT_EARRING,
		SLOT_ACCESSORY,
		SLOT_CON,
		SLOT_MAX ,
	};
	enum
	{
		MAX_PIECE_TYPE			=		20,
		MAX_PARTOPTION_TYPE		=		13,
	};
public:
	SIDSETITEMLIST			m_sItemSetLists[MAX_PIECE_TYPE];
	SPARTOPTION				m_sPartOption[ MAX_PARTOPTION_TYPE ];
	DWORD					m_dwOptionCount;

public:
	BOOL					Load( const std::string & strFileName );

public:
	CItemSetListsOption();
	~CItemSetListsOption();

};

#endif