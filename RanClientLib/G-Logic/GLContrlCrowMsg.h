#pragma once

#include "GLContrlBaseMsg.h"

//namespace GLMSG
//{
//	#pragma pack(1)
//
//
//	// Revert to default structure packing
//	#pragma pack()
//};

enum EMTOWER_DESTROYED_REQ_FB
{
	EMTOWER_NOTHING				= 0,
	EMTOWER_DESTROYED_SG		= 1,
	EMTOWER_DESTROYED_SGMP		= 2,
	EMTOWER_DESTROYED_SGPHX		= 3,
	EMTOWER_DESTROYED_MP		= 4,
	EMTOWER_DESTROYED_MPSG		= 5,
	EMTOWER_DESTROYED_MPPHX		= 6,
	EMTOWER_DESTROYED_PHX		= 7,
	EMTOWER_DESTROYED_PHXSG		= 8,
	EMTOWER_DESTROYED_PHXMP		= 9,
};
namespace GLMSG
{
	#pragma pack(1)

	//---------------------------------------------------------------------------NET
	struct SNETCROW
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwGlobID;

		SNETCROW () 
			: dwGlobID(0)
		{
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETCROW_MOVETO : public SNETCROW
	{
		DWORD				dwActState;
		D3DXVECTOR3			vCurPos;
		D3DXVECTOR3			vTarPos;
		DWORD				dwCellID;
		BOOL				bTrace;	  // 추격을 위한 이동인지 유무

		SNETCROW_MOVETO () 
			: dwActState(NULL)
			, vCurPos(0,0,0)
			, vTarPos(0,0,0)
			, dwCellID(0)
			, bTrace(FALSE)
		{
			nmg.dwSize = sizeof(SNETCROW_MOVETO);
			nmg.nType = NET_MSG_GCTRL_CROW_MOVETO;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETCROW_ATTACK : public SNETCROW
	{
		EMCROW				emTarCrow;
		DWORD				dwTarID;
		DWORD				dwAType;

		SNETCROW_ATTACK () 
			: emTarCrow(CROW_PC)
			, dwTarID(0)
			, dwAType(0)
		{
			nmg.dwSize = sizeof(SNETCROW_ATTACK);
			nmg.nType = NET_MSG_GCTRL_CROW_ATTACK;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETCROW_AVOID : public SNETCROW
	{
		EMCROW				emTarCrow;
		DWORD				dwTarID;

		SNETCROW_AVOID () 
			: emTarCrow(CROW_PC)
			, dwTarID(0)
		{
			nmg.dwSize = sizeof(SNETCROW_AVOID);
			nmg.nType = NET_MSG_GCTRL_CROW_AVOID;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETCROW_DAMAGE : public SNETCROW
	{
		EMCROW				emTarCrow;
		DWORD				dwTarID;
		int					nDamage;
		DWORD				dwNowHP;
		DWORD				dwDamageFlag;

		SNETCROW_DAMAGE () 
			: emTarCrow(CROW_PC)
			, dwTarID(0)
			, nDamage(0)
			, dwNowHP(0)
			, dwDamageFlag(DAMAGE_TYPE_NONE)
		{
			nmg.dwSize = sizeof(SNETCROW_DAMAGE);
			nmg.nType = NET_MSG_GCTRL_CROW_DAMAGE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};


	//add tower down
	struct SNET_SW_TOWERDOWN
	{
		NET_MSG_GENERIC		nmg;

		bool		bOn;
		EMTOWER_DESTROYED_REQ_FB	emFB;
		WORD		emSchool;

		SNET_SW_TOWERDOWN()
			: emFB (EMTOWER_NOTHING)
			,bOn (false)
			,emSchool (0)
		{
			nmg.dwSize = sizeof(SNET_SW_TOWERDOWN);
			nmg.nType = NET_MSG_GCTRL_TOWERDOWN;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_SW_TOWERDOWN_BRD
	{
		NET_MSG_GENERIC		nmg;
		bool		bOn;
		EMTOWER_DESTROYED_REQ_FB	emFB;
		WORD		emSchool;

		SNET_SW_TOWERDOWN_BRD()
			: emFB (EMTOWER_NOTHING)
			,bOn (false)
			,emSchool (0)
		{
			nmg.dwSize = sizeof(SNET_SW_TOWERDOWN_BRD);
			nmg.nType = NET_MSG_GCTRL_TOWERDOWN_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETCROW_SKILL : public SNETCROW
	{
		DWORD				dwAType;
		SNATIVEID			skill_id;
		WORD				skill_lev;
	
		D3DXVECTOR3			vTARPOS;
		WORD				wTARNUM;
		STARID				sTARIDS[EMTARGET_NET];

		SNETCROW_SKILL () 
			: SNETCROW()
			, dwAType(0)
			, skill_id(0,0)
			, skill_lev(0)
			, vTARPOS(0,0,0)
			, wTARNUM(0)
		{
			nmg.dwSize = sizeof(SNETCROW_SKILL);
			nmg.nType = NET_MSG_GCTRL_CROW_SKILL;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		void CALCSIZE ()
		{
			nmg.dwSize = sizeof(SNETCROW_SKILL) - ( sizeof(STARID) * (EMTARGET_NET - wTARNUM) );
		}

		BOOL ADDTARGET ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARNUM>=EMTARGET_NET )
			{
				return FALSE;
			}
			else
			{
				sTARIDS[wTARNUM].wCrow = static_cast<EMCROW>(emCrow);
				sTARIDS[wTARNUM].wID = static_cast<WORD>(dwID);			
				wTARNUM++;
				CALCSIZE ();
				return TRUE;
			}
		}

		BOOL ADDTARGET ( const STARID &starget )
		{
			return ADDTARGET(starget.GETCROW(),starget.GETID());
		}
	};
	struct SNETCROW_OWNER : public SNETCROW
	{
		SW_SCHOOL_DATA emOWNER;

		SNETCROW_OWNER () 
			: emOWNER( SW_SCHOOL_MAX )
		{
			nmg.dwSize = sizeof(SNETCROW_OWNER);
			nmg.nType = NET_MSG_GCTRL_CROW_OWNER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	// Revert to default structure packing
	#pragma pack()
};

