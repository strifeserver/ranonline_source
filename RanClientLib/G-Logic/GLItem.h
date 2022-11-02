#ifndef GLITEM_H_
#define GLITEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <list>
#include <vector>

#include "./GLItemDef.h"
#include "./GLCharDefine.h"

#include "../../EngineLib/Common/ByteStream.h"
#include "../../EngineLib/Common/SerialFile.h"
#include "../../EngineLib/G-Logic/GLDefine.h"
#include "../../EngineLib/G-Logic/GLList.h"
#include "../../EngineLib/Meshs/DxSimpleMeshMan.h"
#include "../../EngineLib/DxOctree/DxLandDef.h"
#include "../../EngineLib/G-Logic/GLQuadTree.h"

enum PETTYPE
{
	PETTYPE_NONE    = -1,
	PETTYPE_A		= 0,
	PETTYPE_B		= 1,
	PETTYPE_C		= 2,
	PETTYPE_D		= 3,
	PETTYPE_E		= 4,//ADD PET TYPE
	PETTYPE_F		= 5,
	PETTYPE_G		= 6,
	PETTYPE_H		= 7,
	PETTYPE_I		= 8,
	PETTYPE_J		= 9,
	PETTYPE_K		= 10,
	PETTYPE_L		= 11,
	PETTYPE_M		= 12,
	PETTYPE_N		= 13,
	PETTYPE_O		= 14,
	PETTYPE_P		= 15,
	PETTYPE_Q		= 16,
	PETTYPE_R		= 17,
	PETTYPE_S		= 18,
	PETTYPE_T		= 19,
	PETTYPE_U		= 20,
	PETTYPE_V		= 21,
	PETTYPE_W		= 22,
	PETTYPE_X		= 23,
	PETTYPE_Y		= 24,
	PETTYPE_Z		= 25,
	PETTYPE_NEWSIZE = 26,
};

enum SUMMON_TYPE
{
	SUMMON_TYPE_NONE    = -1,
	SUMMON_TYPE_A	   = 0,
	SUMMON_TYPE_SIZE    = 1,

};

//change jdev , 
enum PET_ACCESSORYTYPE
{
	PET_ACCETYPE_A		= 0,	// �Ӹ�, ����
	PET_ACCETYPE_B		= 1,	// ��, �ٸ�, ����
	PET_ACCETYPE_SIZE	= 2,
};

enum VEHICLE_TYPE
{
	VEHICLE_TYPE_NONE		= -1,
	VEHICLE_TYPE_BOARD		= 0,
	VEHICLE_TYPE_SCUTER		= 1,
	VEHICLE_TYPE_BIKE_BT5	= 2,
	VEHICLE_TYPE_BIKE_BT7	= 3,
	VEHICLE_TYPE_BIKE_BT9	= 4,
	VEHICLE_TYPE_BIKE_RARE	= 5,
	VEHICLE_TYPE_BIKE_XMAS	= 6,
	VEHICLE_TYPE_H	= 7,
	VEHICLE_TYPE_I	= 8,
	VEHICLE_TYPE_J	= 9,
	VEHICLE_TYPE_CAR_A= 10,
	VEHICLE_TYPE_CAR_B= 11,
	VEHICLE_TYPE_CAR_C= 12,
	VEHICLE_TYPE_CAR_D= 13,
	
	VEHICLE_TYPE_REARCAR_A= 14,
	VEHICLE_TYPE_REARCAR_B= 15,
	VEHICLE_TYPE_REARCAR_C= 16,
	VEHICLE_TYPE_REARCAR_D= 17,
	
	VEHICLE_TYPE_SIZE= 18,
};

enum VEHICLE_ACCESSORYTYPE
{
	VEHICLE_ACCETYPE_SKIN		= 0, // skin
	VEHICLE_ACCETYPE_PARTS_A	= 1, // parts
	VEHICLE_ACCETYPE_PARTS_B	= 2, // parts
	VEHICLE_ACCETYPE_PARTS_C	= 3, // parts
	//ACCE_TYPE_SIZE		= 4,
	VEHICLE_ACCETYPE_PARTS_D	= 4, // manhura
	VEHICLE_ACCETYPE_PARTS_E	= 5, // sadle & back
	VEHICLE_ACCETYPE_PARTS_F	= 6, // flag
	VEHICLE_ACCETYPE_SIZE		= 7,
};
//add abl
enum EMCPSINDEX
{
	CPSIND_NULL		= -1,
	CPSIND_M_LEFT	= 0,
	CPSIND_M_RIGHT	= 1,
	CPSIND_W_LEFT	= 2,
	CPSIND_W_RIGHT	= 3,
	CPSIND_SIZE		= 4,
};

#define ITEMEDIT_VER 0x0118

namespace ITEM
{
	GLITEM_ATT assign_att ( glold::GLITEM_ATT_102 emOld );
	GLITEM_ATT assign_att ( glold_103::GLITEM_ATT_103 emOld );

	struct SRANDOM_OPT
	{
		enum { VERSION = 0x0100 };

		char			szNAME[SRANDOM_DATA::NAME_LEN];

		SRANDOM_OPT ()
		{
			memset(szNAME, 0, sizeof(char) * SRANDOM_DATA::NAME_LEN);
		}

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	struct SDRUG_100
	{
		BOOL		bInstance;
		EMITEM_DRUG	emDrug;
		WORD		wPileNum;

		BOOL		bRatio;

		union
		{
			struct { WORD	wCureVolume; };
			struct { WORD	wArrowNum; };
		};

		DWORD		dwCureDISORDER;
	};

	//	?? ?, ??? ?? ???.
	struct SDRUG
	{
		enum { VERSION = 0x0101 };

		__time64_t	tTIME_LMT;		//	??? ???? ?? ?? ??.
		BOOL		bInstance;		//	??? ??.
		EMITEM_DRUG	emDrug;			//	?? ??.
		WORD		wPileNum;		//	?????. ( old : wApplyNum )

		BOOL		bRatio;			//	?? ?? ??.

		union
		{
			struct { WORD	wCureVolume; };	//	?? ??. ( ?? ??? ??. )
			struct { WORD	wArrowNum; };	//	?? ????. ( ??? ??. )
		};

		DWORD		dwCureDISORDER;	//	"DISORDER" ?? ??.

		SDRUG ()
			: tTIME_LMT(0)
			, bInstance(FALSE)
			, emDrug(ITEM_DRUG_NUNE)
			, wPileNum(1)
			
			, bRatio(FALSE)
			, wCureVolume(0)
			, wArrowNum(0)
			
			, dwCureDISORDER(NULL)
		{
		}

		void Assign ( SDRUG_100 &sDRUG );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	//	Skill ??, ?? ???.
	struct SSKILLBOOK
	{
		enum { VERSION = 0x0100 };

		SNATIVEID	sSkill_ID;		//	?? Skill.

		SSKILLBOOK ()
			: sSkill_ID(NATIVEID_NULL())
		{
		}

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	struct SGRINDING_100
	{
		SGRIND				sNO[EMGRINDING_NO];
	};

	struct SGRINDING_101
	{
		EMGRINDING_CLASS	emCLASS;
		SGRIND				sNO[EMGRINDING_NO];
	};

	struct SGRINDING_102
	{
		EMGRINDING_CLASS	emCLASS;
		bool				bHIGH;
		EMGRINDING_TYPE		emTYPE;
	};

	//	?? ???, ?? ???.
	struct SGRINDING
	{
		enum { VERSION = 0x0103 };

		EMGRINDING_CLASS	emCLASS;
		EMGRINDER_TYPE		emGRINDER_TYPE;
		EMGRINDING_TYPE		emTYPE;

		SGRINDING ()
			: emCLASS(EMGRINDING_CLASS_ARM)
			, emGRINDER_TYPE(EMGRINDER_NORMAL)
			, emTYPE(EMGRINDING_NUNE)

		{
		}

		void Assign ( SGRINDING_100 &sOldData )
		{
		}

		void Assign ( SGRINDING_101 &sOldData )
		{
			emCLASS = sOldData.emCLASS;

			switch ( sOldData.sNO[0].emTYPE )
			{
			case 3:	emTYPE = EMGRINDING_DAMAGE;		break;
			case 4:	emTYPE = EMGRINDING_DEFENSE;	break;
			};
		}

		void Assign (  SGRINDING_102 &sOldData )
		{
			emCLASS = sOldData.emCLASS;
			emTYPE = sOldData.emTYPE;

			if ( !sOldData.bHIGH )	emGRINDER_TYPE = EMGRINDER_NORMAL;
			else emGRINDER_TYPE = EMGRINDER_HIGH;
		}

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	struct SADDON
	{
		EMITEM_ADDON	emTYPE;		//	?? ??.
		int				nVALUE;		//	?? ?.

		SADDON ()
			: emTYPE(EMADD_NONE)
			, nVALUE(0)
		{

		}
	};

	struct SVAR
	{
		EMITEM_VAR		emTYPE;		//	?? ??.
		float			fVariate;	//	?? ?.

		SVAR ()
			: emTYPE(EMVAR_NONE)
			, fVariate(0.0f)
		{
		}
	};

	struct SVOL
	{
		EMITEM_VAR	emTYPE;		//	?? ??.
		float		fVolume;	//	?? ?.

		SVOL ()
			: emTYPE(EMVAR_NONE)
			, fVolume(0)
		{
		}
	};

	struct SSATE_BLOW_100
	{
		EMSTATE_BLOW	emTYPE;		//	?? ??.
		float			fRATE;		//	?? ??.
		float			fLIFE;		//	?? ??.
		float			fVALUE;		//	?? ?.
	};

	struct SSATE_BLOW
	{
		EMSTATE_BLOW	emTYPE;		//	?? ??.
		float			fRATE;		//	?? ??.
		float			fLIFE;		//	?? ??.
		float			fVAR1;		//	?? ? 1.
		float			fVAR2;		//	?? ? 2.

		SSATE_BLOW ()
			: emTYPE(EMBLOW_NONE)
			, fRATE(0.0f)
			, fLIFE(0.0f)
			, fVAR1(0.0f)
			, fVAR2(0.0f)
		{
		}
	};

	struct SSUIT_100
	{
		enum { ADDON_SIZE = 4 };

		EMSUIT		emSuit;						//	??? ?? ??.
		BOOL		bBothHand;					//	?? ?? ????.
		EMITEM_HAND	emHand;						//	?? ? ??.

		glold::GLITEM_ATT_102	emAttack;					//	????.
		WORD		wAttRange;					//	?? ?? ??.

		short		nHitRate;					//	???. ( +/- ) (%)
		short		nAvoidRate;					//	???. ( +/- ) (%)

		//GLPADATA_OLD	gdDamage;					//	???. ( +/- )
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;					//	??. ( +/- )

		SRESIST		sResist;					//	???.

		WORD		wReModelNum;				//	?? ?? ??.		( '0' ?? ?? )

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SSATE_BLOW_100	sBLOW;					//	?? ?? ??.
	};

	struct SSUIT_101
	{
		enum { ADDON_SIZE = 4 };

		EMSUIT		emSuit;						//	??? ?? ??.
		BOOL		bBothHand;					//	?? ?? ????.
		EMITEM_HAND	emHand;						//	?? ? ??.

		glold::GLITEM_ATT_102	emAttack;					//	????.
		WORD		wAttRange;					//	?? ?? ??.

		short		nHitRate;					//	???. ( +/- ) (%)
		short		nAvoidRate;					//	???. ( +/- ) (%)

		//GLPADATA_OLD	gdDamage;					//	???. ( +/- )
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;					//	??. ( +/- )

		SRESIST		sResist;					//	???.

		WORD		wReModelNum;				//	?? ?? ??.		( '0' ?? ?? )

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SSATE_BLOW	sBLOW;						//	?? ?? ??.
	};

	struct SSUIT_102
	{
		enum { ADDON_SIZE = 4 };
		EMSUIT		emSuit;						//	??? ?? ??.
		bool		bBothHand;					//	?? ?? ????.
		bool		bBig;						//	??.
		EMITEM_HAND	emHand;						//	?? ? ??.

		glold::GLITEM_ATT_102	emAttack;		//	????.
		WORD		wAttRange;					//	?? ?? ??.
		WORD		wReqSP;						//	??? ??? SP.

		short		nHitRate;					//	???. ( +/- ) (%)
		short		nAvoidRate;					//	???. ( +/- ) (%)

		//GLPADATA_OLD	gdDamage;					//	???. ( +/- )
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;					//	??. ( +/- )

		SRESIST		sResist;					//	???.

		WORD		wReModelNum;				//	?? ?? ??.		( '0' ?? ?? )

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SSATE_BLOW	sBLOW;						//	?? ?? ??.
	};

	struct SSUIT_103
	{
		enum { ADDON_SIZE = 4 };

		EMSUIT		emSuit;						//	??? ?? ??.
		bool		bBothHand;					//	?? ?? ????.
		bool		bBig;						//	??.
		EMITEM_HAND	emHand;						//	?? ? ??.

		glold_103::GLITEM_ATT_103	emAttack;	//	????.
		WORD		wAttRange;					//	?? ?? ??.
		WORD		wReqSP;						//	??? ??? SP.

		short		nHitRate;					//	???. ( +/- ) (%)
		short		nAvoidRate;					//	???. ( +/- ) (%)

		//GLPADATA_OLD	gdDamage;					//	???. ( +/- )
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;					//	??. ( +/- )

		SRESIST		sResist;					//	???.

		WORD		wReModelNum;				//	?? ?? ??.		( '0' ?? ?? )

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SSATE_BLOW	sBLOW;						//	?? ?? ??.
	};

	struct SSUIT_104
	{
		enum { ADDON_SIZE = 4 };
		EMSUIT		emSuit;
		bool		bBothHand;
		bool		bBig;
		EMITEM_HAND	emHand;

		GLITEM_ATT	emAttack;
		WORD		wAttRange;
		WORD		wReqSP;

		short		nHitRate;
		short		nAvoidRate;

		//GLPADATA_OLD	gdDamage;
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;

		SRESIST_101	sResist;

		WORD		wReModelNum;

		SADDON		sADDON[ADDON_SIZE];
		SVAR		sVARIATE;
		SSATE_BLOW	sBLOW;
	};

	struct SSUIT_105
	{
		enum { ADDON_SIZE = 4 };
		EMSUIT		emSuit;
		bool		bBothHand;
		bool		bBig;
		EMITEM_HAND	emHand;

		GLITEM_ATT	emAttack;
		WORD		wAttRange;
		WORD		wReqSP;

		short		nHitRate;
		short		nAvoidRate;

		//GLPADATA_OLD	gdDamage;
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;

		SRESIST		sResist;

		WORD		wReModelNum;				//	?? ?? ??.		( '0' ?? ?? )

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SSATE_BLOW	sBLOW;						//	?? ?? ??.
	};

	struct SSUIT_106
	{
		enum { ADDON_SIZE = 4 };

		EMSUIT		emSuit;						//	??? ?? ??.
		DWORD		dwHAND;						//	? ?? ??.
		EMITEM_HAND	emHand;						//	?? ? ??.

		GLITEM_ATT	emAttack;					//	????.
		WORD		wAttRange;					//	?? ?? ??.
		WORD		wReqSP;						//	??? ??? SP.

		short		nHitRate;					//	???. ( +/- ) (%)
		short		nAvoidRate;					//	???. ( +/- ) (%)

		//GLPADATA_OLD	gdDamage;					//	???. ( +/- )
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;					//	??. ( +/- )

		SRESIST		sResist;					//	???.

		//	?? ?? ??.		( '0' ?? ?? ),
		//	??, ?? ?????? ??? ??.
		WORD		wReModelNum;

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SSATE_BLOW	sBLOW;						//	?? ?? ??.
	};

	//:P
	struct SSUIT_107
	{
		enum { ADDON_SIZE = 4 };

		EMSUIT		emSuit;						//	??? ?? ??.
		DWORD		dwHAND;						//	? ?? ??.
		EMITEM_HAND	emHand;						//	?? ? ??.

		GLITEM_ATT	emAttack;					//	????.
		WORD		wAttRange;					//	?? ?? ??.
		WORD		wReqSP;						//	??? ??? SP.

		short		nHitRate;					//	???. ( +/- ) (%)
		short		nAvoidRate;					//	???. ( +/- ) (%)

		//GLPADATA_OLD	gdDamage;					//	???. ( +/- )
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;					//	??. ( +/- )

		SRESIST		sResist;					//	???.

		//	?? ?? ??.		( '0' ?? ?? ),
		//	??, ?? ?????? ??? ??.
		WORD		wReModelNum;

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SVOL		sVOLUME;					//  ??? ??. By ??
		SSATE_BLOW	sBLOW;						//	?? ?? ??.

	};

	struct SSUIT_108
	{
		enum { ADDON_SIZE = 8 };

		EMSUIT		emSuit;						//	??? ?? ??.
		DWORD		dwHAND;						//	? ?? ??.
		EMITEM_HAND	emHand;						//	?? ? ??.

		GLITEM_ATT	emAttack;					//	????.
		WORD		wAttRange;					//	?? ?? ??.
		WORD		wReqSP;						//	??? ??? SP.

		short		nHitRate;					//	???. ( +/- ) (%)
		short		nAvoidRate;					//	???. ( +/- ) (%)

		//GLPADATA_OLD	gdDamage;					//	???. ( +/- )
		WORD		wLow;
		WORD		wHigh;

		short		nDefense;					//	??. ( +/- )

		SRESIST		sResist;					//	???.

		//	?? ?? ??.		( '0' ?? ?? ),
		//	??, ?? ?????? ??? ??.
		WORD		wReModelNum;

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SVOL		sVOLUME;					//  ??? ??. By ??
		SSATE_BLOW	sBLOW;						//	?? ?? ??.
	};

	enum	EMHAND
	{
		EMHAND_BOTHHAND		= 0x0001,
		EMHAND_BIG			= 0x0002,
		EMHAND_BROOM		= 0x0004,
	};

	//	?? ???, ?? ???.
	struct SSUIT // By ??
	{
		enum { ADDON_SIZE = 8, VERSION = 0x0109 };

		EMSUIT		emSuit;						//	??? ?? ??.
		DWORD		dwHAND;						//	? ?? ??.
		EMITEM_HAND	emHand;						//	?? ? ??.

		GLITEM_ATT	emAttack;					//	????.
		WORD		wAttRange;					//	?? ?? ??.
		WORD		wReqSP;						//	??? ??? SP.

		short		nHitRate;					//	???. ( +/- ) (%)
		short		nAvoidRate;					//	???. ( +/- ) (%)

		GLPADATA	gdDamage;					//	???. ( +/- )
		short		nDefense;					//	??. ( +/- )

		SRESIST		sResist;					//	???.

		//	?? ?? ??.		( '0' ?? ?? ),
		//	??, ?? ?????? ??? ??.
		WORD		wReModelNum;

		//	?? ?? ???.
		SADDON		sADDON[ADDON_SIZE];			//	?? ??.
		SVAR		sVARIATE;					//	??? ??.
		SVOL		sVOLUME;					//  ??? ??. By ??
		SSATE_BLOW	sBLOW;						//	?? ?? ??.

		SSUIT ()
			: emSuit(SUIT_HEADGEAR)
			, dwHAND(NULL)
			, emHand(HAND_RIGHT)
			
			, emAttack(ITEMATT_NOTHING)
			, wAttRange(4)
			, wReqSP(0)
			
			, nHitRate(0)
			, nAvoidRate(0)
			
			, gdDamage(0,0)
			, nDefense(0)
			
			, wReModelNum(0)
		{
		}

		void Assign ( SSUIT_100 &Suit100 );
		void Assign ( SSUIT_101 &Suit101 );
		void Assign ( SSUIT_102 &Suit102 );
		void Assign ( SSUIT_103 &Suit103 );
		void Assign ( SSUIT_104 &Suit104 );
		void Assign ( SSUIT_105 &Suit105 );
		void Assign ( SSUIT_106 &Suit106 );
		//add 8 addon
		void Assign ( SSUIT_107 &Suit107 );
		void Assign ( SSUIT_108 &Suit108 );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

		void SETBOTHHAND ( bool bBOTH )		{ (bBOTH) ? (dwHAND|=EMHAND_BOTHHAND) : (dwHAND&=~EMHAND_BOTHHAND); }
		void SETBIG ( bool bBIG )			{ (bBIG) ? (dwHAND|=EMHAND_BIG) : (dwHAND&=~EMHAND_BIG); }
		void SETBROOM ( bool bBROOM )		{ (bBROOM) ? (dwHAND|=EMHAND_BROOM) : (dwHAND&=~EMHAND_BROOM); }

		bool IsBOTHHAND () const			{ return NULL!=(dwHAND&EMHAND_BOTHHAND); }
		bool IsBIG () const					{ return NULL!=(dwHAND&EMHAND_BIG); }
		bool IsBROOM () const				{ return NULL!=(dwHAND&EMHAND_BROOM); }
	};

	struct SBASIC_100
	{
		SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )

		char		szName[ITEM_SZNAME];		//	???? ??.	
		EMITEMLEVEL	emLevel;					//	??? ?? ??.

		DWORD		dwFlags;					//	Trade ??.
		DWORD		dwPrice;					//	??? ??.

		EMITEM_TYPE	emItemType;					//	??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		WORD		wReqLevel;					//	?? Level.
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		char		szFieldFile[MAX_PATH];				//	?? ?? ??.
		char		szInventoryFile[MAX_PATH];			//	???? ?? ??.
		char		szWearingFIle[GLCI_NUM][MAX_PATH];	//	?? ??.
		char		szComment[ITEM_SZCOMMENT];			//	???? ?? ??? ??.
	};

	struct SBASIC_101
	{
		SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )

		char		szName[ITEM_SZNAME];		//	???? ??.	
		EMITEMLEVEL	emLevel;					//	??? ?? ??.

		DWORD		dwFlags;					//	Trade ??.
		DWORD		dwPrice;					//	??? ??.

		EMITEM_TYPE	emItemType;					//	??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		WORD		wReqLevel;					//	?? Level.
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		char		szSelfBodyEffect[MAX_PATH];	//	?? ??? ?? ?? ???.
		char		szTargBodyEffect[MAX_PATH];	//	?? ??? ?? ?? ???.
		char		szTargetEffect[MAX_PATH];	//	?? ?? ???.

		char		szFieldFile[MAX_PATH];				//	?? ?? ??.
		char		szInventoryFile[MAX_PATH];			//	???? ?? ??.
		char		szWearingFIle[GLCI_NUM][MAX_PATH];	//	?? ??.
		char		szComment[ITEM_SZCOMMENT];			//	???? ?? ??? ??.
	};

	struct SBASIC_102
	{
		SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )

		char		szName[ITEM_SZNAME];		//	???? ??.	
		EMITEMLEVEL	emLevel;					//	??? ?? ??.

		DWORD		dwFlags;					//	Trade ??.
		DWORD		dwPrice;					//	??? ??.

		EMITEM_TYPE	emItemType;					//	??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		WORD		wReqLevel;					//	?? Level.
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		char		szSelfBodyEffect[MAX_PATH];	//	?? ??? ?? ?? ???.
		char		szTargBodyEffect[MAX_PATH];	//	?? ??? ?? ?? ???.
		char		szTargetEffect[MAX_PATH];	//	?? ?? ???.

		char		szFieldFile[MAX_PATH];				//	?? ?? ??.
		char		szInventoryFile[MAX_PATH];			//	???? ?? ??.
		char		szWearingFIle[GLCI_NUM][MAX_PATH];	//	?? ??.
		char		szComment[ITEM_SZCOMMENT];			//	???? ?? ??? ??.
	};

	struct SBASIC_104
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwPrice;					// ??? ??.

		EMITEM_TYPE	emItemType;					// ??? ??.

		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevel;					//	?? Level.
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.
	};

	struct SBASIC_105
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwPrice;					// ??? ??.

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevel;					//	?? Level.
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.
	};

	struct SBASIC_106
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwPrice;					// ??? ??.

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevel;					//	?? Level.
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.
	};

	struct SBASIC_107
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwPrice;					// ??? ??.

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevel;					//	?? Level.
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 
	};

	struct SBASIC_109
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwPrice;					// ??? ??.

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevel;					//	?? Level.
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_EX];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 
	};

	struct SBASIC_110
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwPrice;					// ??? ??.

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevel;					//	?? Level.

		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_NEWSEX];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 
	};

	struct SBASIC_111
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwPrice;					// ??? ??.

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevelDW;				//	?? Level ???
		WORD		wReqLevelUP;				//  ?? Level ???
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_NEWSEX];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 
	};

	struct SBASIC_112
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwBuyPrice;					// ??? ?? ??.
		DWORD		dwSellPrice;				// ??? ?? ??

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevelDW;				//	?? Level ???
		WORD		wReqLevelUP;				//  ?? Level ???
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_NEWSEX];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 

		SNATIVEID		sSubID;						// Map mid/sid
		WORD			wPosX;						// X??
		WORD			wPosY;						// Y??
	};


	struct SBASIC_113
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwBuyPrice;					// ??? ?? ??.
		DWORD		dwSellPrice;				// ??? ?? ??

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevelDW;				//	?? Level ???
		WORD		wReqLevelUP;				//  ?? Level ???
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_NEWSEX];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 

		SNATIVEID		sSubID;						// Map mid/sid
		WORD			wPosX;						// X??
		WORD			wPosY;						// Y??
	};

	struct SBASIC_114
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwBuyPrice;					// ??? ?? ??.
		DWORD		dwSellPrice;				// ??? ?? ??

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevelDW;				//	?? Level ???
		WORD		wReqLevelUP;				//  ?? Level ???
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_NEWSEX];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 

		SNATIVEID		sSubID;						// mid/sid ( ?? MID/SID ) 
		WORD			wPosX;						// X??
		WORD			wPosY;						// Y??

		DWORD			dwCoolTime;					// ??? ?? ???
		EMCOOL_TYPE		emCoolType;					// ??? ??
	};



	struct SBASIC_115
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwBuyPrice;					// ??? ?? ??.
		DWORD		dwSellPrice;				// ??? ?? ??

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevelDW;				//	?? Level ???
		WORD		wReqLevelUP;				//  ?? Level ???
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_2015];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 

		SNATIVEID		sSubID;						// mid/sid ( ?? MID/SID ) 
		WORD			wPosX;						// X??
		WORD			wPosY;						// Y??

		DWORD			dwCoolTime;					// ??? ?? ???
		EMCOOL_TYPE		emCoolType;					// ??? ??

		//add class
		DWORD			dwRebornReq;	
	};

	struct SBASIC_116
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwBuyPrice;					// ??? ?? ??.
		DWORD		dwSellPrice;				// ??? ?? ??

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevelDW;				//	?? Level ???
		WORD		wReqLevelUP;				//  ?? Level ???
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_2015];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 

		SNATIVEID		sSubID;						// mid/sid ( ?? MID/SID ) 
		WORD			wPosX;						// X??
		WORD			wPosY;						// Y??

		DWORD			dwCoolTime;					// ??? ?? ???
		EMCOOL_TYPE		emCoolType;					// ??? ??

		//add class
		DWORD			dwRebornReq;

		//add summoncard
		SNATIVEID		sSumID;	 //summon ID
		SUMMON_TYPE		m_emSumType; //summond Type

		DWORD			dwSlotNum;//add itemslot
	};

	struct SBASIC_117
	{
		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwBuyPrice;					// ??? ?? ??.
		DWORD		dwSellPrice;				// ??? ?? ??

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevelDW;				//	?? Level ???
		WORD		wReqLevelUP;				//  ?? Level ???
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		//SCHARSTATS	sReqStats;	
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_2015];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 

		SNATIVEID		sSubID;						// mid/sid ( ?? MID/SID ) 
		WORD			wPosX;						// X??
		WORD			wPosY;						// Y??

		DWORD			dwCoolTime;					// ??? ?? ???
		EMCOOL_TYPE		emCoolType;					// ??? ??

		//add class
		DWORD			dwRebornReq;

		//add summoncard
		SNATIVEID		sSumID;	 //summon ID
		SUMMON_TYPE		m_emSumType; //summond Type

		DWORD			dwSlotNum;//add itemslot

		std::string		strABLFile[CPSIND_SIZE];//add abl
		std::string		strVPSFile;//add vps
	};

	struct SBASIC
	{
		enum { VERSION = ITEMEDIT_VER };

		SNATIVEID	sNativeID;					// ?? ID. ( ??? ?? ?? ???? ???? ?? ID )
		SNATIVEID	sGroupID;					// ???

		std::string	strName;					// ???? ??.	
		EMITEMLEVEL	emLevel;					// ??? ?? ??.

		WORD		wGradeAttack;				// ?? ?? ??
		WORD		wGradeDefense;				// ?? ?? ??

		float		fExpMultiple;				// ??? ?? ??

		WORD		wReserved1;					// ?? ??
		WORD		wReserved2;					// ?? ??
		WORD		wReserved3;					// ?? ??
		WORD		wReserved4;					// ?? ??
		WORD		wReserved5;					// ?? ??

		DWORD		dwFlags;				    // Trade ??.
		DWORD		dwBuyPrice;					// ??? ?? ??.
		DWORD		dwSellPrice;				// ??? ?? ??

		EMITEM_TYPE	emItemType;					// ??? ??.

		//	?? ??.
		EMBRIGHT	emReqBright;				//	?? ??. ( ?/? )
		DWORD		dwReqCharClass;				//	??? ? ?? ???. ( EMCHARCLASS Flags )
		DWORD		dwReqSchool;				//	??? ? ?? ??. 
		WORD		wReqLevelDW;				//	?? Level ???
		WORD		wReqLevelUP;				//  ?? Level ???
		WORD		wReqPA;						//	?? ???.
		WORD		wReqSA;						//	?? ???.
		SCHARSTATS	sReqStats;					//	?? ??? Stats ??.

		WORD		wInvenSizeX;				//	???? ???.
		WORD		wInvenSizeY;				//	???? ???.

		SNATIVEID	sICONID;					//	??? ???.

		std::string		strSelfBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargBodyEffect;		//	?? ??? ?? ?? ???.
		std::string		strTargetEffect;		//	?? ?? ???.

		std::string		strFieldFile;				//	?? ?? ??.
		std::string		strInventoryFile;			//	???? ?? ??.
		std::string		strWearingFile[GLCI_NUM_2015];	//	?? ??.
		std::string		strWearingFile2[GLCI_NUM_2015];	//	?? ??.
		std::string		strComment;					//	???? ?? ??? ??.

		// PET
		std::string		strPetWearingFile;			//  ?? ????. 

		SNATIVEID		sSubID;						// mid/sid ( ?? MID/SID ) 
		WORD			wPosX;						// X??
		WORD			wPosY;						// Y??

		DWORD			dwCoolTime;					// ??? ?? ???
		EMCOOL_TYPE		emCoolType;					// ??? ??

		//add class
		DWORD			dwRebornReq;

		//add summoncard
		SNATIVEID		sSumID;	 //summon ID
		SUMMON_TYPE		m_emSumType; //summond Type

		DWORD			dwSlotNum;//add itemslot

		std::string		strABLFile[CPSIND_SIZE];//add abl
		std::string		strVPSFile;//add vps


		SBASIC () 
			: sNativeID(false)
			, sGroupID(false)

			, emLevel(LEVEL_NORMAL)

			, wGradeAttack(0)
			, wGradeDefense(0)

			, fExpMultiple(1.0f)

			, wReserved1(0)
			, wReserved2(0)
			, wReserved3(0)
			, wReserved4(0)
			, wReserved5(0)

			, dwFlags(TRADE_ALL)
			, dwBuyPrice(1)
			, dwSellPrice(1)

			, emItemType(ITEM_SUIT)
			, emReqBright(BRIGHT_BOTH)
			, dwReqCharClass(GLCC_NONE)
			, dwReqSchool(GLSCHOOL_ALL)
			, wReqLevelDW(0)
			, wReqLevelUP(0)
			, wReqPA(0)
			, wReqSA(0)
			, wInvenSizeX(1)
			, wInvenSizeY(1)
			, sICONID(0,0)
			, sSubID( false )
			, wPosX( 0 )
			, wPosY( 0 )
			, dwCoolTime( 0 )
			, emCoolType( EMCOOL_ITEMID )
			, dwRebornReq( 0 )
			, sSumID(false)
			, m_emSumType(SUMMON_TYPE_A)
			, dwSlotNum(0)
		{
		}

		void Assign ( SBASIC_100 &sOld );
		void Assign ( SBASIC_101 &sOld );
		void Assign ( SBASIC_102 &sOld );

		bool LOAD_103 ( basestream &SFile );
		bool LOAD_104 ( basestream &SFIle );
		bool LOAD_105 ( basestream &SFile );
		bool LOAD_106 ( basestream &SFile );
		bool LOAD_107 ( basestream &SFile );
		bool LOAD_108 ( basestream &SFile );
		bool LOAD_109 ( basestream &SFile );
		bool LOAD_110 ( basestream &SFile );
		bool LOAD_111 ( basestream &SFile );
		bool LOAD_112 ( basestream &SFile );
		bool LOAD_113 ( basestream &SFile );
		//add class
		bool LOAD_114 ( basestream &SFile );
		bool LOAD_115 ( basestream &SFile );
		bool LOAD_116 ( basestream &SFile );
		bool LOAD_117 ( basestream &SFile );

		bool LOAD ( basestream &SFile );
		bool SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

		SBASIC& operator = ( const SBASIC& rvalue );

		bool IsSALE ()			{ return (dwFlags&TRADE_SALE)!=NULL; }
		bool IsEXCHANGE ()		{ return (dwFlags&TRADE_EXCHANGE)!=NULL; }
		bool IsTHROW ()			{ return (dwFlags&TRADE_THROW)!=NULL; }
		bool IsEVENT ()			{ return (dwFlags&TRADE_EVENT_SGL)!=NULL; }

		bool IsDISGUISE ()		{ return (dwFlags&ITEM_DISGUISE)!=NULL; }

		bool IsCHANNEL_ALL ()	{ return (dwFlags&ITEM_CHANNEL_ALL)!=NULL; }

		bool IsGarbage()		{ return (dwFlags&TRADE_GARBAGE)!=NULL; }
		bool IsCoolTime()		{ return ( dwCoolTime != 0 ); }

		bool IsLocker()		{ return (dwFlags&TRADE_LOCKER)!=NULL; }
		bool IsOwner()				{ return (dwFlags&ITEM_OWNERSHIP)!=NULL; }//add ownership
		bool IsPointRecover()		{ return (dwFlags&ITEM_POINT_RECOVER)!=NULL; }//add point sell
		bool IsCostumeCombine()		{ return (dwFlags&ITEM_COSTUME_COMBINE)!=NULL; }	//add jitem
		bool IsNoFail()				{ return (dwFlags&ITEM_NO_FAIL)!=NULL; } //add nofailups
		bool IsCanColor()			{ return (dwFlags&ITEM_COLORING)!=NULL; } //add itemcolor


	};

	struct SGENERATE
	{
		enum { VERSION = 0x0100, SPECID_NULL = 0, LIMIT_NOUSE = 0 };

		DWORD dwSpecID;			//	??? ?? ?.
		DWORD dwLimitTime;		//	?? ?? ??.
		DWORD dwLimitTimeGen;	//	??? ?? ?? ??.

		SGENERATE ()
			: dwSpecID(SPECID_NULL)
			, dwLimitTime(LIMIT_NOUSE)
			, dwLimitTimeGen(LIMIT_NOUSE)
		{
		}

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	typedef std::list<SNATIVEID>	LISTITEM;
	typedef LISTITEM::iterator		LISTITEM_ITER;

	struct SBOX_ITEM
	{
		SNATIVEID	nidITEM;
		DWORD		dwAMOUNT;

		SBOX_ITEM ()
			: nidITEM(false)
			, dwAMOUNT(1)
		{
		}
	};

	struct SBOX
	{
		enum { VERSION = 0x0101, ITEM_SIZE = 10, };
		SBOX_ITEM	sITEMS[ITEM_SIZE];

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

		SBOX ();
		void CLEAR ();
		bool VALID ();

		bool INSERT ( const SNATIVEID &nidITEM, DWORD dwAMOUNT );
		bool DEL ( int nIndex );
	};

	struct SRANDOMITEM
	{
		float		fRATE;
		SNATIVEID	nidITEM;

		SRANDOMITEM()
			: fRATE(0)
		{
		}
	};

	struct SRANDOMBOX
	{
		enum { VERSION = 0x0100 };

		std::vector<SRANDOMITEM>	vecBOX;

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

		void CLEAR ();
		bool VALID ();

		bool INSERT ( const SNATIVEID &nidITEM, float fRATE );
		bool DEL ( int nIndex );
	};

	struct SQUESTIONITEM // by ??
	{
		enum { VERSION = 0x0100 };

		EMITEM_QUESTION	emType;
		float	fTime;
		float	fExp; // ??? ?? 0.04~0.1%
		WORD	wParam1;
		WORD	wParam2;

		SQUESTIONITEM() 
			: emType(QUESTION_NONE)
			, fTime(0.0f)
			, fExp(0.0f)
			, wParam1(0)
			, wParam2(0)
		{
		}

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	struct SPET	// PetData
	{
		enum { VERSION = 0x0102 };

		DWORD			dwPetID;   // ����ID
		PET_ACCESSORYTYPE	emType;    // �Ǽ�����TYPE
		PETTYPE			emPetType; // TYPE
		SNATIVEID		sPetID;    // CROWID


		SPET() 
			: dwPetID(0)
			, emType(PET_ACCETYPE_SIZE)
			, emPetType(PETTYPE_NEWSIZE)
			, sPetID(NATIVEID_NULL())
		{
		}
		
		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	struct SVEHICLE	// PetData
	{
		enum { VERSION = 0x0100 };

		VEHICLE_TYPE	emVehicleType; // TYPE

		SVEHICLE() 
			: emVehicleType(VEHICLE_TYPE_SIZE)
		{
		}
		
		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		static VOID SaveCsvHead_old ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID SaveCsv_old ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	struct SPETSKINPACKITEMDATA
	{
		SNATIVEID sMobID; // ?? ? ID
        float	  fScale; // ?? ?? ??
		float	  fRate;  // ?? ??

		SPETSKINPACKITEMDATA() :
			sMobID(NATIVEID_NULL()),
			fScale(1.0f),
			fRate(0.0f)			
		{
		}
	};

	struct SPETSKINPACKITEM	// PetData
	{
		enum { VERSION = 0x0100 };

		std::vector<SPETSKINPACKITEMDATA> vecPetSkinData;
		DWORD							  dwPetSkinTime; // ???? ??.

		SPETSKINPACKITEM() :
				dwPetSkinTime(0)
		{
		}

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

		void CLEAR ();
		bool VALID ();

		bool INSERT ( const SNATIVEID &sMobID, float fRate, float fScale );
		bool DEL ( int nIndex );
	};
};

//	Note : ??? ?? ??.
//
struct SITEM
{
	enum
	{
		VERSION = 0x0104,

		FILE_SBASIC			= 1,	// ?????
		FILE_SSUIT			= 2,	// ??
		FILE_SDRUG			= 3,	// ??
		FILE_SSKILLBOOK		= 4,	// ???
		FILE_SGRINDING		= 5,	// ?????
		FILE_SGENERATE		= 6,	// 
		FILE_BOX			= 7,	// ? ????
		FILE_RANDOMBOX		= 8,	// ????
		FILE_QUESTIONITEM	= 9,	// Question Item
		FILE_RANDOMITEM		= 10,	// ?????
		FILE_PET			= 11,	// ? ???	// PetData
		FILE_VEHICLE		= 12,   // ? ? 
		FILE_PETSKINPACK	= 13,	// ? ???

		FILE_END_DATA		= 0xEDEDEDED,
	};

	ITEM::SBASIC		sBasicOp;
	ITEM::SSUIT			sSuitOp;
	ITEM::SDRUG			sDrugOp;
	ITEM::SSKILLBOOK	sSkillBookOp;
	ITEM::SGRINDING		sGrindingOp;
	ITEM::SGENERATE		sGenerateOp;
	ITEM::SBOX			sBox;
	ITEM::SRANDOMBOX	sRandomBox;
	ITEM::SQUESTIONITEM	sQuestionItem;
	ITEM::SRANDOM_OPT	sRandomOpt;
	ITEM::SPET			sPet;	// PetData
	ITEM::SVEHICLE		sVehicle;
	ITEM::SPETSKINPACKITEM  sPetSkinPack;

	BOOL SaveFile ( CSerialFile &SFile );
	BOOL LoadFile ( basestream &SFile, bool bPastLoad );

	static VOID SaveCsvHead ( std::fstream &SFile );
	static VOID SaveCsvHead_old ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile );
	VOID SaveCsv_old ( std::fstream &SFile );
	VOID LoadCsv ( CStringArray &StrArray );

	SITEM ()
	{
	}

	SITEM ( const SITEM &value )
	{
		operator=(value);
	}

	SITEM& operator = ( const SITEM& rvalue );

	//	?? ?? ???? ?? ????? ??.
	bool SAME_INVENSIZE ( const SITEM& sItem ) const
	{
		return ( sBasicOp.wInvenSizeX==sItem.sBasicOp.wInvenSizeX && sBasicOp.wInvenSizeY==sItem.sBasicOp.wInvenSizeY );
	}

	bool ISINSTANCE () const
	{
		return ( sDrugOp.bInstance!=FALSE );
	}

	bool IsTIMELMT ()
	{
		return (sBasicOp.dwFlags&ITEM_TIMELMT)!=NULL && sDrugOp.tTIME_LMT!=0;
	}

	//	?? ??? ????? ??.
	bool ISPILE () const
	{
		return ( sDrugOp.bInstance && (sDrugOp.wPileNum>1) );
	}

	bool ISEVENTITEM () const
	{
		return (sBasicOp.dwFlags&TRADE_EVENT_SGL)!=NULL;
	}

	WORD GETAPPLYNUM () const;

	DWORD GETSELLPRICE ( WORD wTurnNum ) const
	{
		if ( ISPILE() )
		{
			WORD wAPPLYNUM = GETAPPLYNUM();
			DWORD dwPRICE = DWORD ( sBasicOp.dwSellPrice * wTurnNum / float(wAPPLYNUM) );
			return dwPRICE;
		}

		return sBasicOp.dwSellPrice;
	}

	float GetExpMultiple()				{ return sBasicOp.fExpMultiple; }

	const char* GetName ();
	const char* GetSelfBodyEffect ()	{	return sBasicOp.strSelfBodyEffect.c_str(); }
	const char* GetTargBodyEffect ()	{	return sBasicOp.strTargBodyEffect.c_str(); }
	const char* GetTargetEffect ()		{	return sBasicOp.strTargetEffect.c_str(); }

	const char* GetFieldFile ()			{	return sBasicOp.strFieldFile.c_str(); }
	const char* GetInventoryFile ()		{	return sBasicOp.strInventoryFile.c_str(); }
	const char* GetRandomOptFile ()		{	return sRandomOpt.szNAME; }

	const char* GetWearingFile ( EMCHARINDEX emIndex )	{ return sBasicOp.strWearingFile[emIndex].c_str(); }
	const char* GetWearingFile2 ( EMCHARINDEX emIndex )	{ return sBasicOp.strWearingFile2[emIndex].c_str(); }
	const char* GetComment ();

	// PET
	const char* GetPetWearingFile () { return sBasicOp.strPetWearingFile.c_str(); }
	const char* GetCpsFile ( EMCPSINDEX emCpsInd ) { return sBasicOp.strABLFile[emCpsInd].c_str(); }//add abl
	const char* GetVpsFile () { return sBasicOp.strVPSFile.c_str(); }//add vps
};

enum EMITEMGEN
{
	EMGEN_DEFAULT	= 0,
	EMGEN_INIT		= 1,
	EMGEN_SHOP		= 2,
	EMGEN_NPC		= 3,
	EMGEN_QUEST		= 4,
	EMGEN_MOB		= 5,
	EMGEN_GMEDIT	= 6,
	EMGEN_OLD		= 7,
	EMGEN_BILLING	= 8,
	EMGEN_GMEDIT2   = 9,
	EMGEN_ITEMMIX	= 10,
	EMGEN_GATHERING	= 11,
	EMGEN_SYSTEM	= 12,

	EMGEN_ALL		= -1,
};

struct SITEMCUSTOM_100
{
	SNATIVEID	sNativeID;

	WORD		wReModel;
	WORD		wTurnNum;

	short		nHitRate;
	short		nAvoidRate;

	short		nDamage;
	short		nDefense;

	SRESIST_101	sResist;
};

struct SITEMCUSTOM_101
{
	SNATIVEID	sNativeID;

	WORD		wReModel;
	WORD		wTurnNum;

	short		nHitRate;
	short		nAvoidRate;

	short		nDamage;
	short		nDefense;

	SRESIST		sResist;
};

struct SITEMCUSTOM_102
{
	SNATIVEID	sNativeID;

	BYTE		cGenType;
	BYTE		cFieldID;
	DWORD		dwGenNum;

	WORD		wReModel;
	WORD		wTurnNum;

	short		nHitRate;
	short		nAvoidRate;

	short		nDamage;
	short		nDefense;

	SRESIST		sResist;
};

struct SITEMCUSTOM_103
{
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )

	BYTE		cGenType;					//	?? ??.
	BYTE		cFieldID;					//	?? ??.
	LONGLONG	lnGenNum;					//	?? ??.

	BYTE		cReModelReq;				//	?? ?? ??.
	BYTE		cReModelSuc;				//	?? ?? ??.

	WORD		wTurnNum;					//	?????. ( wUsedNum )

	short		nHitRate;					//	???. ( +/- ) (%)
	short		nAvoidRate;					//	???. ( +/- ) (%)

	short		nDamage;					//	???. ( +/- )
	short		nDefense;					//	??. ( +/- )

	SRESIST		sResist;					//	???.
};

struct SITEMCUSTOM_104
{
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.

	BYTE		cGenType;					//	?? ??.
	BYTE		cFieldID;					//	?? ??.
	LONGLONG	lnGenNum;					//	?? ??.

	BYTE		cReModelReq;				//	?? ?? ??.
	BYTE		cReModelSuc;				//	?? ?? ??.

	WORD		wTurnNum;					//	?????. ( wUsedNum )

	short		nHitRate;					//	???. ( +/- ) (%)
	short		nAvoidRate;					//	???. ( +/- ) (%)

	short		nDamage;					//	???. ( +/- )
	short		nDefense;					//	??. ( +/- )

	SRESIST		sResist;					//	???.
};

struct SITEMCUSTOM_105
{
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.

	BYTE		cGenType;					//	?? ??.
	BYTE		cFieldID;					//	?? ??.
	LONGLONG	lnGenNum;					//	?? ??.

	WORD		wTurnNum;					//	?????. ( wUsedNum )

	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.

	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.
};

struct SITEMCUSTOM_106
{
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.

	BYTE		cGenType;					//	?? ??.
	BYTE		cFieldID;					//	?? ??.
	LONGLONG	lnGenNum;					//	?? ??.

	WORD		wTurnNum;					//	?????. ( wUsedNum )

	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.

	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.
};

struct SITEMCUSTOM_107
{
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.
	__time64_t	tDISGUISE;					//	??? ???? ?? ?? ??.

	BYTE		cGenType;					//	?? ??.
	BYTE		cFieldID;					//	?? ??.
	LONGLONG	lnGenNum;					//	?? ??.

	WORD		wTurnNum;					//	?????. ( wUsedNum )

	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.

	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.
};

struct SITEMCUSTOM_108
{
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.
	__time64_t	tDISGUISE;					//	??? ???? ?? ?? ??.

	BYTE		cGenType;					//	?? ??.
	BYTE		cChnID;						//	?? ??.
	BYTE		cFieldID;					//	?? ??.
	LONGLONG	lnGenNum;					//	?? ??.

	WORD		wTurnNum;					//	?????. ( wUsedNum )

	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.

	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.
};

struct SITEMCUSTOM_109
{
public:
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.
	__time64_t	tDISGUISE;					//	??? ???? ?? ?? ??.

	LONGLONG	lnGenNum;					//	?? ??.
	WORD		wTurnNum;					//	?????. ( wUsedNum )
	BYTE		cGenType;					//	?? ??.
	BYTE		cChnID;						//	?? ??.

	BYTE		cFieldID;					//	?? ??.

public:
	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.
	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.

public:
	//	?? ?? ??.
	BYTE		cOptTYPE1;
	BYTE		cOptTYPE2;
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1;
	short		nOptVALUE2;
	short		nOptVALUE3;
	short		nOptVALUE4;
};

struct SITEMCUSTOM_110
{
public:
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.
	__time64_t	tDISGUISE;					//	??? ???? ?? ?? ??.

	LONGLONG	lnGenNum;					//	?? ??.
	WORD		wTurnNum;					//	?????. ( wUsedNum )
	BYTE		cGenType;					//	?? ??.
	BYTE		cChnID;						//	?? ??.

	BYTE		cFieldID;					//	?? ??.

public:
	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.
	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.
public:
	//	?? ?? ??.
	BYTE		cOptTYPE1;
	BYTE		cOptTYPE2;
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1;
	short		nOptVALUE2;
	short		nOptVALUE3;
	short		nOptVALUE4;

	DWORD		dwPetID;	// ???
};

struct SITEMCUSTOM_111
{
public:
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.
	__time64_t	tDISGUISE;					//	??? ???? ?? ?? ??.

	LONGLONG	lnGenNum;					//	?? ??.
	WORD		wTurnNum;					//	?????. ( wUsedNum )
	BYTE		cGenType;					//	?? ??.
	BYTE		cChnID;						//	?? ??.

	BYTE		cFieldID;					//	?? ??.

public:
	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.
	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.

public:
	//	?? ?? ??.
	BYTE		cOptTYPE1;
	BYTE		cOptTYPE2;
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1;
	short		nOptVALUE2;
	short		nOptVALUE3;
	short		nOptVALUE4;

	DWORD		dwPetID;	// ???
	DWORD		dwVehicleID; // ????(DB)
};
//add ownership
//add new version
struct SITEMCUSTOM_112
{
public:
	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.
	__time64_t	tDISGUISE;					//	??? ???? ?? ?? ??.

	LONGLONG	lnGenNum;					//	?? ??.
	WORD		wTurnNum;					//	?????. ( wUsedNum )
	BYTE		cGenType;					//	?? ??.
	BYTE		cChnID;						//	?? ??.

	BYTE		cFieldID;					//	?? ??.

public:
	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.
	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.

public:
	//	?? ?? ??.
	BYTE		cOptTYPE1;
	BYTE		cOptTYPE2;
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1;
	short		nOptVALUE2;
	short		nOptVALUE3;
	short		nOptVALUE4;

	DWORD		dwPetID;	// ???
	DWORD		dwVehicleID; // ????(DB)

	bool		bVietnamGainItem;	
};
//add itemcolor
struct SITEMCUSTOM_113
{
public:
	SNATIVEID	sNativeID;					//	���� ID. ( ������ ���� ���� �����ÿ� �ο��Ǵ� ���� ID )
	SNATIVEID	nidDISGUISE;				//	�ڽ��� ���� ��Ų.
	__time64_t	tBORNTIME;					//	���Ѻ� �������� ��� ��ȿ �Ⱓ.
	__time64_t	tDISGUISE;					//	���Ѻ� �ڽ����� ��� ��ȿ �Ⱓ.

	LONGLONG	lnGenNum;					//	���� ��ȣ.
	WORD		wTurnNum;					//	��밡�ɷ�. ( wUsedNum )
	BYTE		cGenType;					//	���� Ÿ��.
	BYTE		cChnID;						//	ä�� ��ȣ.

	BYTE		cFieldID;					//	�ʵ� ��ȣ.

public:
	BYTE		cDAMAGE;					//	���ݷ� ���� ���.
	BYTE		cDEFENSE;					//	���� ���� ���.
	BYTE		cRESIST_FIRE;				//	����(ȭ) ���� ���.
	BYTE		cRESIST_ICE;				//	����(��) ���� ���.
	BYTE		cRESIST_ELEC;				//	����(��) ���� ���.
	BYTE		cRESIST_POISON;				//	����(��) ���� ���.
	BYTE		cRESIST_SPIRIT;				//	����(��) ���� ���.
	BYTE        cISDROPITEM;                 // modified for itemdrop status ^_^ x-f0rce

public:
	//	���� �ɼ� ���.
	BYTE		cOptTYPE1; // ���ݷ�
	BYTE		cOptTYPE2; // ������
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1; // ���ݷ� %
	short		nOptVALUE2; // ������ %
	short		nOptVALUE3;
	short		nOptVALUE4;

public:
	DWORD		dwPetID;	 // ��Ƶ�
	DWORD		dwVehicleID; // Ż�;Ƶ�(DB)

	bool		bVietnamGainItem;	 //	��Ʈ�� Ž�� �κ� ������( ��Ʈ�� ���� )

	//add ownership
	DWORD		dwOwnerID;
};
//

//	Note : SITEMCUSTOM - 
//	i990x@yahoo.com	
//	JDev Warning!
//	When you changed version for SITEMCOSTUM Adjust also the functions listed below 
//  to avoid breaking character data and quest data!
//		"BOOL SCHARDATA2::GETPUTONITEMS_BYBUF ()"
//		"BOOL SETPUTONITEMS_BYBUF ()"
//		"BOOL GLInventory::SETITEM_BYBUFFER ()"
//		"BOOL GLQUEST::LOAD ()" - INVENQARRAY
//		"BOOL GLQUEST_STEP::LOAD ()" - INVENQARRAY
//
struct SITEM_LOBY;
struct SITEMCLIENT;

struct SITEMCUSTOM
{
public:
	enum { VERSION = 0x0114 };

	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.
	__time64_t	tBORNTIME;					//	??? ???? ?? ?? ??.
	__time64_t	tDISGUISE;					//	??? ???? ?? ?? ??.

	LONGLONG	lnGenNum;					//	?? ??.
	WORD		wTurnNum;					//	?????. ( wUsedNum )
	BYTE		cGenType;					//	?? ??.
	BYTE		cChnID;						//	?? ??.

	BYTE		cFieldID;					//	?? ??.

public:
	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.
	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.

public:
	//	?? ?? ??.
	BYTE		cOptTYPE1; // ???
	BYTE		cOptTYPE2; // ???
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1; // ??? %
	short		nOptVALUE2; // ??? %
	short		nOptVALUE3;
	short		nOptVALUE4;

public:
	DWORD		dwPetID;	 // ???
	DWORD		dwVehicleID; // ????(DB)

	bool		bVietnamGainItem;	 //	??? ?? ?? ???( ??? ?? )

	//add ownership
	DWORD		dwOwnerID;

	WORD		wColor;//add itemcolor


public:
	SITEMCUSTOM ();
	SITEMCUSTOM ( SNATIVEID	sNID );

	void ResetGrind ( EMGRINDING_TYPE emTYPE );

	bool operator == ( const SITEMCUSTOM &value );
	bool operator != ( const SITEMCUSTOM &value );

	void Assign ( const SITEMCUSTOM_100 &sOld );
	void Assign ( const SITEMCUSTOM_101 &sOld );
	void Assign ( const SITEMCUSTOM_102 &sOld );
	void Assign ( const SITEMCUSTOM_103 &sOld );
	void Assign ( const SITEMCUSTOM_104 &sOld );
	void Assign ( const SITEMCUSTOM_105 &sOld );
	void Assign ( const SITEMCUSTOM_106 &sOld );
	void Assign ( const SITEMCUSTOM_107 &sOld );
	void Assign ( const SITEMCUSTOM_108 &sOld );
	void Assign ( const SITEMCUSTOM_109 &sOld );
	void Assign ( const SITEMCUSTOM_110 &sOld );
	void Assign ( const SITEMCUSTOM_111 &sOld );
	//add ownership
	void Assign ( const SITEMCUSTOM_112 &sOld );
	void Assign ( const SITEMCUSTOM_113 &sOld );//add itemcolor

	void Assign ( const SITEM_LOBY &sOldLoby );
	void Assign ( const SITEMCLIENT &sClient );

	void Assign ( const SITEMLINK &sItemLink );
	void Convert ( SITEMLINK &sItemLink );
	void SetGM_GEN ( DWORD dwUserNum )
	{		
		// ? ??? ?????? GMCharEdit ?? ?? ???? ??/??? ??? ????.
		// ?? ??? ?? ?? ??? ???.
		// 2005-12-16 Jgkim
		// cGenType = EMGEN_MOB; /// ?? ??? ??? ?? ???? ????
		cGenType = EMGEN_GMEDIT; /// GMCharEdit ? ??? ?? ????
		cFieldID = 0xff;
		lnGenNum = dwUserNum;
	}
	
	void SetGM_GEN2 ( DWORD dwUserNum )
	{		
		// ? ??? ?????? GMCharEdit ?? ?? ???? ??/??? ??? ????.
		// ?? ??? ?? ?? ??? ???.
		// 2005-12-16 Jgkim
		// cGenType = EMGEN_MOB; /// ?? ??? ??? ?? ???? ????
		cGenType = EMGEN_GMEDIT2; /// GMCharEdit ? ??? ?? ????
		cFieldID = 0xff;
		lnGenNum = dwUserNum;
	}

	//! GMCharEdit ? ??? ????? ????.
	bool IsGM_GENITEM () const
	{
		return cGenType == EMGEN_GMEDIT;
	}

public:
	DWORD GETGRADE_DAMAGE () const;			//	���� ��޿� ���� ���ݷ� ���귮.
	WORD GETGRADE_DEFENSE () const;			//	���� ��޿� ���� ���� ���귮.
	WORD GETGRADE_RESIST_FIRE () const;		//	���� ��޿� ���� ������ ���귮.
	WORD GETGRADE_RESIST_ICE () const;		//	���� ��޿� ���� ������ ���귮.
	WORD GETGRADE_RESIST_ELEC () const;		//	���� ��޿� ���� ������ ���귮.
	WORD GETGRADE_RESIST_POISON () const;	//	���� ��޿� ���� ������ ���귮.
	WORD GETGRADE_RESIST_SPIRIT () const;	//	���� ��޿� ���� ������ ���귮.
	BYTE GETGRADE ( EMGRINDING_TYPE emTYPE ) const;
	BYTE GETGRADE_EFFECT () const;

public:
	BYTE grade_damage () const;
	BYTE grade_defense () const;
	BYTE grade_fire () const				{ return cRESIST_FIRE; }
	BYTE grade_ice () const					{ return cRESIST_ICE; }
	BYTE grade_elec () const				{ return cRESIST_ELEC; }
	BYTE grade_poison () const				{ return cRESIST_POISON; }
	BYTE grade_spirit () const				{ return cRESIST_SPIRIT; }
 

public:
	GLPADATA GETDAMAGE () const;			//	�⺻ ����� + ������ + ����.
	DWORD GETMaDAMAGE () const;				//	�⺻ ����ġ + ������ + ����.
	short GETDEFENSE () const;				//	�⺻ ���� + ������ + ����.
	short GETHITRATE () const;				//	�⺻ ������ + ������.
	short GETAVOIDRATE () const;			//	�⺻ ������ + ������.

	WORD GETATTRANGE () const;				//	�⺻ ���ݰŸ� + ������.
	WORD GETREQ_SP () const;				//	�⺻ sp�Ҹ� + ������.

	WORD GETRESIST_FIRE () const;			//	�⺻ ����(ȭ) + ������.
	WORD GETRESIST_ICE () const;			//	�⺻ ����(��) + ������.
	WORD GETRESIST_ELEC () const;			//	�⺻ ����(��) + ������.
	WORD GETRESIST_POISON () const;			//	�⺻ ����(��) + ������.
	WORD GETRESIST_SPIRIT () const;			//	�⺻ ����(��) + ������.

	int GETADDHP () const;					//	�ΰ� �ɼ� hp ���� + ������.
	int GETADDMP () const;					//	�ΰ� �ɼ� hp ���� + ������.
	int GETADDSP () const;					//	�ΰ� �ɼ� hp ���� + ������.
	int GETADDMA () const;					//	�ΰ� �ɼ� hp ���� + ������. 

	float GETINCHP () const;				//	Ư�� �ɼ� hp ������ + ������.
	float GETINCMP () const;				//	Ư�� �ɼ� mp ������ + ������.
	float GETINCSP () const;				//	Ư�� �ɼ� sp ������ + ������.
	float GETINCAP () const;				//	Ư�� �ɼ� ap ������ + ������.

	float GETMOVESPEED() const;				//  ?? ?? ???? ?? + ???
	float GETMOVESPEEDR() const;			//  ?? ?? ???? ??? 

	//add directval
	float GETHITRATE_PER () const;
	float GETAVOIDRATE_PER () const;
	WORD GET_STAT_POW () const;	
	WORD GET_STAT_VIT () const;		
	WORD GET_STAT_INT () const;		
	WORD GET_STAT_DEX () const;		
	WORD GET_STAT_STM () const;	

	int GET_MELEE () const;	
	int GET_MESSILE () const;
	int GET_ENERGY () const;

	int GET_HP_POTION () const;	
	int GET_MP_POTION () const;	
	int GET_SP_POTION () const;

	float GET_CP_GAIN () const;	
	int	GET_CP () const;	

	float GET_CRIT_DMG () const;
	float GET_BLOW_DMG () const;
	float GET_CRIT_RATE () const;
	float GET_BLOW_RATE () const;




public:
	GLPADATA getdamage () const;	//	??? + ??? ?.
	WORD getmadamage () const;		//	?? + ??? ??.
	short getdefense () const;		//	??? + ??? ??.

public:
	float GETOptVALUE ( EMRANDOM_OPT emOPT ) const;
	short GETOptVALUE_SHORT ( EMRANDOM_OPT emOPT ) const;

public:
	EMRANDOM_OPT GETOptTYPE1 () const	{	return (EMRANDOM_OPT)cOptTYPE1; }
	EMRANDOM_OPT GETOptTYPE2 () const	{	return (EMRANDOM_OPT)cOptTYPE2; }
	EMRANDOM_OPT GETOptTYPE3 () const	{	return (EMRANDOM_OPT)cOptTYPE3; }
	EMRANDOM_OPT GETOptTYPE4 () const	{	return (EMRANDOM_OPT)cOptTYPE4; }

	float GETOptVALUE1 () const;
	float GETOptVALUE2 () const;
	float GETOptVALUE3 () const;
	float GETOptVALUE4 () const;

	BOOL IsPerRandOpt( INT nRandOPT ) const;
	BOOL IsSetRandOpt() const;

protected:
	float GETOptVALUE ( EMRANDOM_OPT emOPT, short nVALUE ) const;
	void SETOptVALUE ( int nIndex, EMRANDOM_OPT emOPT, float fVALUE );
	void SETOptVALUEEmpty( EMRANDOM_OPT emOPT, float fVALUE );	// ITEMREBUILD_MARK
	int RESETOptVALUE( int nOpt = -1 );
	bool IsValidRESETOpt( int nIndex );

public:
	bool GENERATE_RANDOM_OPT( bool bNew = true, int nOpt = -1 );
	int GENERATE_RANDOM_OPT_SE( const SRANDOM_OPT_SET* pOptSet );

public:
	friend struct SITEMCLIENT;
	friend class GLITEMLMT;
};

BOOL SETPUTONITEMS_BYBUF ( SITEMCUSTOM *pPutOnItems, CByteStream &ByteStream );


struct SITEM_LOBY
{
public:
	enum { VERSION = 0x0101 };

	SNATIVEID	sNativeID;					//	?? ID. ( ??? ?? ?? ???? ???? ?? ID )
	SNATIVEID	nidDISGUISE;				//	??? ?? ??.

	LONGLONG	lnGenNum;					//	?? ??.
	WORD		wTurnNum;					//	?????. ( wUsedNum )
	BYTE		cGenType;					//	?? ??.
	BYTE		cChnID;						//	?? ??.

	BYTE		cFieldID;					//	?? ??.

public:
	BYTE		cDAMAGE;					//	??? ?? ??.
	BYTE		cDEFENSE;					//	??? ?? ??.
	BYTE		cRESIST_FIRE;				//	??(?) ?? ??.
	BYTE		cRESIST_ICE;				//	??(?) ?? ??.
	BYTE		cRESIST_ELEC;				//	??(?) ?? ??.
	BYTE		cRESIST_POISON;				//	??(?) ?? ??.
	BYTE		cRESIST_SPIRIT;				//	??(?) ?? ??.

	//	?? ?? ??.
	BYTE		cOptTYPE1; // ???
	BYTE		cOptTYPE2; // ???
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1; // ??? %
	short		nOptVALUE2; // ??? %
	short		nOptVALUE3;
	short		nOptVALUE4;

public:
	SITEM_LOBY ();
	SITEM_LOBY ( SNATIVEID	sNID );

	bool operator == ( const SITEM_LOBY &value );
	bool operator != ( const SITEM_LOBY &value );

	void Assign ( const SITEMCUSTOM &sItemCustom );
};

BOOL SETPUTONITEMS_BYBUF ( SITEM_LOBY *pPutOnItems, CByteStream &ByteStream );


//	Note : ?????? ?? ??. ( ???? ?? ? ??? ??. )
//
struct SITEMCLIENT
{
	SNATIVEID	sNativeID;
	SNATIVEID	nidDISGUISE;

	BYTE		cOptTYPE1; // ??????? ?????.
	short		nOptVALUE1; // ??????? ?????.

	BYTE		cGRADE;
	WORD		wTurnNum;

	WORD		wColor; //add itemcolor

	SITEMCLIENT () 
		: sNativeID(false)
		, nidDISGUISE(false)
		, cGRADE(0)
		, wTurnNum(0)
		, cOptTYPE1(0) 
		, nOptVALUE1(0)
		, wColor(0)//add itemcolor
	{
	}

	SITEMCLIENT ( const SNATIVEID &sNID ) 
		: sNativeID(sNID)
		, nidDISGUISE(false)
		, cGRADE(0)
		, wTurnNum(0)
		, cOptTYPE1(0) 
		, nOptVALUE1(0) 
		, wColor(0)//add itemcolor
	{
	}

	SITEMCLIENT& Assign ( const SITEMCUSTOM &Input )
	{
		//Exact count grind by input type - x-f0rce
		sNativeID = Input.sNativeID;
		nidDISGUISE = Input.nidDISGUISE;
		wTurnNum = Input.wTurnNum;
		cGRADE = Input.grade_damage()>Input.grade_defense() ? Input.grade_damage() : Input.grade_defense();
		wColor = Input.wColor;//add itemcolor

		nOptVALUE1 = Input.GETOptVALUE_SHORT( EMR_OPT_MOVE_SPEED );		
		if ( nOptVALUE1 != 0 ) cOptTYPE1 = (BYTE) EMR_OPT_MOVE_SPEED;
		
		return *this;
	}
	EMRANDOM_OPT GETOptTYPE1 () const	{	return (EMRANDOM_OPT)cOptTYPE1; }

	BYTE GETGRADE () const		{ return cGRADE; }	BYTE GETGRADE_EFFECT () const;
};

//	Note : ???? ??? ???? ?? ??.
//
struct SDROP_ITEM
{
	SITEMCUSTOM	sItemCustom;			//	??? ???.

	SNATIVEID	sMapID;					//	? ID.
	DWORD		dwCeID;					//	? ID.
	DWORD		dwGlobID;				//	?? ??? ????.
	D3DXVECTOR3	vPos;					//	? ??.

	SDROP_ITEM ()
		: dwCeID(0)
		, dwGlobID(0)
		, vPos(0,0,0)
	{
	}
};

struct SDROP_CLIENT_ITEM
{
	SITEMCLIENT	sItemClient;

	DWORD		dwGlobID;				//	?? ??? ????.
	D3DXVECTOR3	vPos;					//	? ??.

	float		fAge;

	SDROP_CLIENT_ITEM ()
		: dwGlobID(0)
		, vPos(0,0,0)
		, fAge(0)
	{
	}

	SDROP_CLIENT_ITEM& Assign ( SDROP_ITEM &Input )
	{
		sItemClient.Assign ( Input.sItemCustom );

		dwGlobID = Input.dwGlobID;
		vPos = Input.vPos;

		return *this;
	}
};

#ifndef GLLandMan
class GLLandMan;
class GLLandNode;
typedef SQUADNODE<GLLandNode> LANDQUADNODE;
#endif //GLLandMan

#ifndef GLLandManClient
class GLClientNode;
class GLLandNode;
typedef SQUADNODE<GLClientNode>		CLIENTQUADNODE;
#endif //GLLandMan

//	Note : Server ? ItemDrop ?? ???.
//
class CItemDrop
{
public:
	union
	{
		struct
		{
			SDROP_ITEM			sDrop;			//	?? ??.
		};

		struct
		{
			SITEMCUSTOM			sItemCustom;	//	??? ???.

			SNATIVEID			sMapID;			//	? ID.
			DWORD				dwCeID;			//	? ID.
			DWORD				dwGlobID;		//	?? ??? ????.
			D3DXVECTOR3			vPos;			//	? ??.
		};
	};

public:
	float					fAge;			//	?? ??.

	EMGROUP					emGroup;		//	?? ???? ??.
	DWORD					dwHoldGID;		//	?? ????.

	LANDQUADNODE*			pQuadNode;		//	?? ?? ??.
	SGLNODE<CItemDrop*>*	pCellList;		//	? ??? ??.
	SGLNODE<CItemDrop*>*	pGlobList;		//	?? ??? ??.

public:
	bool IsTakeItem ( DWORD dwPartyID, DWORD dwGaeaID );
	bool IsDropOut();

	void Update ( float fElapsedTime );

public:
	CItemDrop(void)
		: fAge(0.0f)
		
		, emGroup(EMGROUP_ONE)
		, dwHoldGID(GAEAID_NULL)
		
		, pQuadNode(NULL)
		, pCellList(NULL)
		, pGlobList(NULL)
	  {
	  }
};

typedef CItemDrop* PITEMDROP;

typedef CGLLIST<PITEMDROP>	ITEMDROPLIST;
typedef SGLNODE<PITEMDROP>	ITEMDROPNODE;


//	Note : Client ? ItemDrop ?? ???.
//
class DxSimMesh;
struct CLIPVOLUME;
class CItemClientDrop
{
public:
	union
	{
		struct
		{
			SDROP_CLIENT_ITEM	sDrop;			//	?? ??.
		};

		struct
		{
			SITEMCLIENT			sItemClient;	//	??? ???.

			SNATIVEID			sMapID;			//	? ID.
			DWORD				dwCeID;			//	? ID.
			DWORD				dwGlobID;		//	?? ??? ????.
			D3DXVECTOR3			vPos;			//	? ??.
		};
	};

public:
	float						fAge;			//	?? ??.
	float						fNextMsgDelay;	//	?? ???? ??? ??? ?? ???? ?????? ??.

	D3DXMATRIX					matWld;			//	??? ??.
	D3DXVECTOR3					vMax;
	D3DXVECTOR3					vMin;

	DxSimMesh*					pSimMesh;

	CLIENTQUADNODE*				pQuadNode;		//	?? ?? ??.
	SGLNODE<CItemClientDrop*>*	pCellList;		//	? ??? ??.
	SGLNODE<CItemClientDrop*>*	pGlobList;		//	?? ??? ??.

public:
	CItemClientDrop (void)
		: fAge(0.0f)
		, fNextMsgDelay(0.0f)
		
		, vMax(0,0,0)
		, vMin(0,0,0)
		
		, pSimMesh(NULL)
		
		, pQuadNode(NULL)
		, pCellList(NULL)
		, pGlobList(NULL)
	  {
	  }

	  bool IsCollision ( const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt ) const;
	  bool IsCollision ( CLIPVOLUME &cv ) const;
	  HRESULT RenderItem ( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matWld, D3DMATERIALQ* pMaterials=NULL );
};

typedef CItemClientDrop*			PITEMCLIENTDROP;

typedef CGLLIST<PITEMCLIENTDROP>	ITEMCLIENTDROPLIST;
typedef SGLNODE<PITEMCLIENTDROP>	ITEMCLIENTDROPNODE;


//	Note : ?? ??? ?? ??, ??? ?? ???.
//
enum EMITEMMAN
{
	ITEMMAN_EDITOR		= 0x0001,
	ITEMID_NOTFOUND		= 0xFFFF
};


//	Note : ????? ???? ??? ???? ???.
//
struct SINVENITEM_SAVE_100
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_100	sItemCustom;
};

struct SINVENITEM_SAVE_101
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_101	sItemCustom;
};

struct SINVENITEM_SAVE_102
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_102	sItemCustom;
};

struct SINVENITEM_SAVE_103
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_103	sItemCustom;
};

struct SINVENITEM_SAVE_104
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_104	sItemCustom;
};

struct SINVENITEM_SAVE_105
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_105	sItemCustom;
};

struct SINVENITEM_SAVE_106
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_106	sItemCustom;
};

struct SINVENITEM_SAVE_107
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_107	sItemCustom;
};

struct SINVENITEM_SAVE_108
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_108	sItemCustom;
};

struct SINVENITEM_SAVE_109
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_109	sItemCustom;
};

struct SINVENITEM_SAVE_110
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_110	sItemCustom;
};

struct SINVENITEM_SAVE_111
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_111	sItemCustom;
};

//add ownership
struct SINVENITEM_SAVE_112
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_112	sItemCustom;
};

//add itemcolor
struct SINVENITEM_SAVE_113
{
	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM_113	sItemCustom;
};

struct SINVENITEM_SAVE
{
	enum { VERSION = 0x0114 };

	WORD		wPosX;
	WORD		wPosY;

	SITEMCUSTOM	sItemCustom;

	SINVENITEM_SAVE ()
		: wPosX(0)
		, wPosY(0)
	{
	}

	void Assign ( const SINVENITEM_SAVE_100 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_101 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_102 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_103 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_104 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_105 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_106 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_107 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_108 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_109 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_110 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}

	void Assign ( const SINVENITEM_SAVE_111 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}
	//add ownership
	void Assign ( const SINVENITEM_SAVE_112 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}
	//add itemcolor
	void Assign ( const SINVENITEM_SAVE_113 &sOld )
	{
		wPosX = sOld.wPosX;
		wPosY = sOld.wPosY;

		sItemCustom.Assign ( sOld.sItemCustom );
	}
};

//	Note : �������� �κ��丮�� ������ ���� ����.
//
struct SINVENITEM
{
	union
	{
		struct
		{
			SINVENITEM_SAVE		sSaveData;
		};

		struct
		{
			WORD				wPosX;
			WORD				wPosY;

			SITEMCUSTOM			sItemCustom;
		};
	};

	// added by ejsayaaa ItemMall
	WORD				wItemPrice;
	WORD				wItemStock;
	WORD				wItemCtg;
	WORD				wCurrency;
	char				szPurKey[PURKEY_LENGTH+1];


	WORD					wBackX;
	WORD					wBackY;

	SINVENITEM () 
		: wBackX(0)
		, wBackY(0)
	{
	}

	SINVENITEM ( WORD _wPosX, WORD _wPosY ) 
		: wBackX( _wPosX )
		, wBackY( _wPosY )
	{
	}

	bool operator== ( SINVENITEM &value )
	{
		return ( wPosX==value.wPosX&&wPosY==value.wPosY&&sItemCustom==value.sItemCustom);
	}

	bool operator!= ( SINVENITEM &value )
	{
		return ( !operator==(value) );
	}
};

struct SINVEN_POS	// ITEMREBUILD_MARK
{
	WORD	wPosX, wPosY;	// �κ��丮������ ��ġ

	SINVEN_POS()
		: wPosX( USHRT_MAX )
		, wPosY( USHRT_MAX )
	{
	}
	SINVEN_POS( WORD _wposx, WORD _wposy )
		: wPosX( _wposx )
		, wPosY( _wposy )
	{
	}
	VOID SET( WORD _wposx, WORD _wposy )
	{
		wPosX = _wposx;
		wPosY = _wposy;
	}
	BOOL VALID()
	{
		return (BOOL)( wPosX != USHRT_MAX && wPosY != USHRT_MAX );
	}
	VOID RESET()
	{
		wPosX = USHRT_MAX;
		wPosY = USHRT_MAX;
	}
};

enum EM_ITEM_MIX
{
	ITEMMIX_ITEMNUM = 5,
	ITEMMIX_CATEGORYNUM = 13,
};

struct ITEMMIX_DATA
{
	SNATIVEID	sNID;
	BYTE		nNum;
	
	
	ITEMMIX_DATA()
		: sNID ( false )
		, nNum ( 0 ) 
		
	{
	}

	void RESET() { sNID = false; nNum = 0; }
};
struct ITEM_MIX
{
	DWORD			dwKey;

	ITEMMIX_DATA	sMeterialItem[ITEMMIX_ITEMNUM];
	ITEMMIX_DATA	sResultItem;

	DWORD		dwRate;
	DWORD		dwPrice;
	//Added by CN Dev ---- 4/08/16 12:00AM Item Mix Category
	DWORD		dwCategory;

	ITEM_MIX()
		: dwKey ( UINT_MAX )
		, dwRate( 0 )
		, dwPrice ( 0 )
		, dwCategory ( 0 )
	{
	}
};

struct SMIXDATA
{
	enum { SMIX_SLOTS = 5 };

	
	SITEMCUSTOM sItems[SMIX_SLOTS];
	SITEMCUSTOM sResult;

	SMIXDATA()
	{
	}
};
struct SPREVIEWDATA
{
	enum { PREVIEW_SLOTS = 6 };

	WORD wFace;
	WORD wHair;
	WORD wHairColor;
	SITEMCUSTOM sPutOn[PREVIEW_SLOTS];

	SPREVIEWDATA()
		: wFace( 0 )
		, wHair ( 0 )
		, wHairColor ( 0 )
	{
	}
};

struct SITEMVIEWDATA
{
	enum { ITEM_SLOTS = 6 };

	WORD wFace;
	WORD wHair;
	WORD wHairColor;
	SITEMCUSTOM sPutOn[ITEM_SLOTS];

	SITEMVIEWDATA()
		: wFace( 0 )
		, wHair ( 0 )
		, wHairColor ( 0 )
	{
	}
};

struct SITEMSEALDATA //sealed card NaJDeV
{
	int			nOption;
	EMRANDOM_OPT		emOpt;
	float			fValue;

	SITEMSEALDATA()
		: nOption( -1 )
		, emOpt( EMR_OPT_NULL )
		, fValue( 0.0f )
	{
	}

	void RESET()
	{
		nOption = -1;
		emOpt = EMR_OPT_NULL;
		fValue = 0.0f;
	}
};

namespace COMMENT
{
	extern std::string ADDON_NO[ITEM::SSUIT::ADDON_SIZE];
	//add by jdev for proper slot information
	//god damn official didnt included this!
	extern std::string ITEM_PUTON_SLOT[SLOT_NSIZE_S_2]; 
	extern std::string ITEM_VEHICLE_SLOT[VEHICLE_ACCETYPE_SIZE]; 
	extern std::string ITEM_PET_SLOT[PET_ACCETYPE_SIZE]; 
};

#endif // GLITEM_H_
