
#ifndef GLSKILL_H_
#define GLSKILL_H_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000

#include "./GLItemDef.h"
#include "./GLCharDefine.h"

#include "../../EngineLib/Common/basestream.h"
#include "../../EngineLib/G-Logic/GLDefine.h"
#include "../../EngineLib/Meshs/DxSkinAniMan.h"
#define SKILLEDIT_VER 0x0106


enum GLSKILL_ATT
{
	SKILLATT_NOTHING	= 0,
	SKILLATT_SWORD		= 1,
	SKILLATT_BLADE		= 2,
	SKILLATT_SWORDBLADE	= 3,
	SKILLATT_DAGGER		= 4,
	SKILLATT_SPEAR		= 5,
	SKILLATT_STICK		= 6,
	SKILLATT_GAUNT		= 7,
	SKILLATT_BOW		= 8,
	SKILLATT_THROW		= 9,

	SKILLATT_GUN		= 10,
	SKILLATT_RAILGUN	= 11,
	SKILLATT_PORTALGUN	= 12,

	SKILLATT_SHOTGUN		= 13,
	SKILLATT_RAILPORTALGUN	= 14,

	SKILLATT_SCYTHE		= 15,
	SKILLATT_DUALSPEAR	= 16,
	SKILLATT_SHURIKEN	= 17,

	SKILLATT_FIST	= 18,
	SKILLATT_WAND	= 19,
	SKILLATT_CUBE	= 20,
	SKILLATT_WHIP	= 21,
	SKILLATT_HAMMER		= 22,
	SKILLATT_SHIELD		= 23,
	SKILLATT_UMBRELLA	= 24,

	SKILLATT_NOCARE		= 25,
	SKILLATT_NSIZE		= 26,

};

namespace SKILL
{
	GLITEM_ATT			GLSKILATT_TO_GLITEMATT( GLSKILL_ATT emSKILLATT );

	enum
	{
		MAX_SZNAME		= 32,
		MAX_SZFILE		= 99,
		MAX_SZCOMMENT	= 128,

		MAX_LEVEL		= 9,
	};

	enum EMROLE
	{
		EMROLE_NORMAL	= 0,		//	??.
		EMROLE_PASSIVE	= 1,		//	??? ??. ( ???? ?? ??? ???? ?? ?? ?? )
		EMROLE_NSIZE	= 2,
	};

	enum EMAPPLY	//	??? ??? ??.
	{
		EMAPPLY_PHY_SHORT	= 0,	//	?? ???.
		EMAPPLY_PHY_LONG	= 1,	//	?? ???.
		EMAPPLY_MAGIC		= 2,	//	?? ???.
	
		EMAPPLY_NSIZE		= 3
	};

	enum EMTYPES
	{
		EMFOR_HP				= 0,	//	??? ??.
		EMFOR_MP				= 1,	//	??? ??.
		EMFOR_SP				= 2,	//	??? ??.

		EMFOR_VARHP				= 3,	//	??? ???.
		EMFOR_VARMP				= 4,	//	??? ???.
		EMFOR_VARSP				= 5,	//	??? ???.

		EMFOR_DEFENSE			= 6,	//	??? ??.
		EMFOR_CURE				= 7,	//	??. ( ?? ?? ?? )

		EMFOR_HITRATE			= 8,	//	???.
		EMFOR_AVOIDRATE			= 9,	//	???.

		EMFOR_VARAP				= 10,	//	?? (??,??,??) ???.

		EMFOR_VARDAMAGE			= 11,	//	??? ???.
		EMFOR_VARDEFENSE		= 12,	//	??? ???.

		EMFOR_PA				= 13,	//	???.
		EMFOR_SA				= 14,	//	???.
		EMFOR_MA				= 15,	//	???.

		EMFOR_HP_RATE			= 16,	//	HP ???.
		EMFOR_MP_RATE			= 17,	//	MP ???.
		EMFOR_SP_RATE			= 18,	//	SP ???.

		EMFOR_PET_GETALLITEMS	= 19,	//	?? ??? ??	// PetData
		EMFOR_PET_GETRAREITEMS	= 20,	//	?? ???? ??
		EMFOR_PET_GETPOTIONS	= 21,	//	??? ???? ??
		EMFOR_PET_GETMONEY		= 22,	//	?? ??
		EMFOR_PET_GETSTONE		= 23,	//	??? ???? ??
		EMFOR_PET_HEAL			= 24,	//	??? HP ?? ?? ??
		EMFOR_PET_SUPPROT		= 25,	//	??? HP, MP, SP ??? ??
		EMFOR_PET_BACKUP_ATK	= 26,	//	??? ???(?) ??
		EMFOR_PET_BACKUP_DEF	= 27,	//	??? ???(?) ??
		EMFOR_PET_AUTOMEDISON	= 28,	//	?? ?? ??
		EMFOR_PET_PROTECTITEMS	= 29,	//	??? ??? ????? ??? ??
		EMFOR_RESIST			= 30,	//	?? ???
		EMFOR_SUMMONTIME		= 31,
		EMFOR_CP_RATE			= 32,	//add cp
		EMFOR_CP_GAIN			= 33,	//add cp
		EMFOR_PET_BACKUP_HITRATE= 34,
		EMFOR_PET_BACKUP_AVOID	= 35,
		FOR_TYPE_SIZE			= 36
	};

	enum EMEFFECTIME
	{
		EMTIME_FIRST		= 0,		//	?? ??? ???.
		EMTIME_IMPACT		= 1,		//	?? ?? ??.
		EMTIME_TARG_OVR		= 2,		//	?? ?? ???? ??? ??.
		EMTIME_PASSIVE		= 3,		//	??? ??? ???.
		EMTIME_REV01		= 4,
		EMTIME_EFFECTACT	= 5,
		EMTIME_NSIZE		= 6,
	};

	enum EMEFFECTPOS
	{
		EMPOS_GROUND			= 0,		//	?? ??.
		EMPOS_STRIKE			= 1,		//	????? ??. ( ??, ?? ??? ???(????) ??. )
		EMPOS_NSIZE				= 2,
	};
		
	enum EMSPECIALSKILLTYPE
	{
		EMSSTYPE_NONE		= 0,		//	????
		EMSSTYPE_TRANSFORM	= 1,		//	??
		EMSSTYPE_SUMMON		= 2,
		EMSSTYPE_TRANSCHF	= 3,		//add panther
		EMSSTYPE_NSIZE		= 4,
	};

	enum EMTRANSFORM_PART
	{
		EMTRANSFORM_HEAD	= 0,		    // ??
		EMTRANSFORM_UPPER	= 1,			// ??
		EMTRANSFORM_LOWER	= 2,			// ??
		EMTRANSFORM_HAND	= 3,			// ?
		EMTRANSFORM_LHAND	= 4,			// ?? ??
		EMTRANSFORM_RHAND	= 5,			// ??? ??
		EMTRANSFORM_FOOT	= 6,			// ?? ??
		EMTRANSFORM_FACE	= 7,			// ??		

		EMTRANSFORM_NSIZE	= 8,				
		
	};

	enum EMACTION//enum SPEC_STIGMA_TYPE //jdev pending
	{
		EMSTIGMA_NORMAL	= 0,
		EMSTIGMA_BUFF	= 1,
		EMSTIGMA_DEBUFF	= 2,
		EMSTIGMA_LIMIT	= 3,
		EMSTIGMA_SIZE	= 4,
	};

	enum SPEC_HALLUCINATE_TYPE //jdev pending
	{
		EMHALLUCINATE_TRANSFORM		= 0,
		EMHALLUCINATE_GESTURE		= 1,
		EMHALLUCINATE_SIZE			= 2,
	};

	enum SPEC_DOMINATE_TYPE //jdev pending
	{
		EMDOMINATE_OWN		= 0,
		EMDOMINATE_CASTER	= 1,
		EMDOMINATE_RANDOM	= 2,
		EMDOMINATE_FIXING	= 3,
		EMDOMINATE_SIZE		= 4,
	};

	enum SPEC_ASIGN_TYPE //jdev pending
	{
		EMASIGN_BEFORE		= 0,
		EMASIGN_AFTER		= 1,
		EMASIGN_SIZE		= 2,
	};

	enum SPEC_INC_TYPE //jdev pending
	{
		EMINC_UP		= 0,
		EMINC_DOWN		= 1,
		EMINC_SIZE		= 2,
	};

	enum SPEC_INVISIBLE_USE_TYPE //jdev pending
	{
		EMINVISIBLE_COMBAT		= 0,
		EMINVISIBLE_SIZE		= 1,
	};

	enum SPEC_INVISIBLE_ANI_TYPE //jdev pending
	{
		EMINVISIBLE_NONE		= 0,
		EMINVISIBLE_WALK		= 1,
		EMINVISIBLE_TELEPORT	= 2,
		EMINVISIBLE_ANI_SIZE	= 3,
	};

	enum SPEC_SWAPPOS_DIR_TYPE //jdev pending
	{
		EMSWAPPOS_DIR_ALL		= 0,
		EMSWAPPOS_DIR_SELF		= 1,
		EMSWAPPOS_DIR_TARGET	= 2,
		EMSWAPPOS_DIR_SIZE	= 3,
	};

	struct SSTATE_BLOW
	{
		float			fRATE;		//	?? ??.
		float			fVAR1;		//	?? ? 1.
		float			fVAR2;		//	?? ? 2.

		SSTATE_BLOW () :
			fRATE(0.0f),
			fVAR1(0.0f),
			fVAR2(0.0f)
		{
		}
	};

	struct SSPEC_100
	{
		float	fVAR1;
		float	fVAR2;
		DWORD	dwFLAG;
		
		SSPEC_100 () :
			fVAR1(0.0f),
			fVAR2(0.0f),
			dwFLAG(NULL)
		{
		}

	};

	struct SSPEC
	{
		float		fVAR1;
		float		fVAR2;
		DWORD		dwFLAG;
		SNATIVEID	dwNativeID;
		SNATIVEID	dwNativeID2;
		
		SSPEC () :
			fVAR1(0.0f),
			fVAR2(0.0f),
			dwFLAG(NULL),
			dwNativeID(NATIVEID_NULL()),
			dwNativeID2(NATIVEID_NULL())
		{
		}

		void Assign( SSPEC_100 &sOldData )
		{
			fVAR1 = sOldData.fVAR1;
			fVAR2 = sOldData.fVAR2;
			dwFLAG = sOldData.dwFLAG;
		}
	};

	struct CDATA_LVL_100
	{
		float			fDELAYTIME;			//	??? ??.

		float			fLIFE;				//	????.
		WORD			wAPPLYRANGE;		//	?? ??.
		WORD			wAPPLYNUM;			//	?? ??.
		WORD			wAPPLYANGLE;		//	?? ??.
		WORD			wPIERCENUM;			//	?? ??.

		float			fBASIC_VAR;			//	???.

		//	??? ??-??. ( ???? )
		WORD			wUSE_ARROWNUM;		//	?? ?? ??.

		WORD			wUSE_HP;			//	??? HP ???.
		WORD			wUSE_MP;			//	??? MP ???.
		WORD			wUSE_SP;			//	??? SP ???.

		WORD			wUSE_HP_PTY;		//	??? HP ??? ???.
		WORD			wUSE_MP_PTY;		//	??? MP ??? ???.
		WORD			wUSE_SP_PTY;		//	??? SP ??? ???.
	};

	struct CDATA_LVL_101
	{
		float			fDELAYTIME;			//	??? ??.

		float			fLIFE;				//	????.
		WORD			wAPPLYRANGE;		//	?? ??.
		WORD			wAPPLYNUM;			//	?? ??.
		WORD			wAPPLYANGLE;		//	?? ??.
		WORD			wPIERCENUM;			//	?? ??.
		WORD			wTARNUM;			//	?? ??.

		float			fBASIC_VAR;			//	???.

		//	??? ??-??. ( ???? )
		WORD			wUSE_ARROWNUM;		//	?? ?? ??.
		WORD			wUSE_CHARMNUM;		//	?? ?? ??.

		WORD			wUSE_EXP;			//	??? EXP ???.
		WORD			wUSE_HP;			//	??? HP ???.
		WORD			wUSE_MP;			//	??? MP ???.
		WORD			wUSE_SP;			//	??? SP ???.

		WORD			wUSE_HP_PTY;		//	??? HP ??? ???.
		WORD			wUSE_MP_PTY;		//	??? MP ??? ???.
		WORD			wUSE_SP_PTY;		//	??? SP ??? ???.
	};

	//	?? ? ??? ??.
	struct CDATA_LVL
	{
		float			fDELAYTIME;			//	??? ??.

		float			fLIFE;				//	????.
		WORD			wAPPLYRANGE;		//	?? ??.
		WORD			wAPPLYNUM;			//	?? ??.
		WORD			wAPPLYANGLE;		//	?? ??.
		WORD			wPIERCENUM;			//	?? ??.
		WORD			wTARNUM;			//	?? ??.

		float			fBASIC_VAR;			//	???.

		//	??? ??-??. ( ???? )
		WORD			wUSE_ARROWNUM;		//	?? ?? ??.
		WORD			wUSE_CHARMNUM;		//	?? ?? ??.

		WORD			wUSE_EXP;			//	??? EXP ???.
		WORD			wUSE_HP;			//	??? HP ???.
		WORD			wUSE_MP;			//	??? MP ???.
		WORD			wUSE_SP;			//	??? SP ???.

		WORD			wUSE_HP_PTY;		//	??? HP ??? ???.
		WORD			wUSE_MP_PTY;		//	??? MP ??? ???.
		WORD			wUSE_SP_PTY;		//	??? SP ??? ???.

		WORD			wUSE_CP;			//	??? EXP ???.
		WORD			wUSE_BULLET;
		WORD			wUSE_EXTRA;
	

		void Assign ( CDATA_LVL_100 &sOldData );
		void Assign ( CDATA_LVL_101 &sOldData );

		void LOAD_GS_110 ( basestream &SFile );

		CDATA_LVL (void) :
			fDELAYTIME(0.0f),

			fLIFE(0.0f),
			wAPPLYRANGE(0),
			wAPPLYNUM(1),
			wAPPLYANGLE(0),
			wPIERCENUM(1),
			wTARNUM(1),

			fBASIC_VAR(0.0f),

			wUSE_ARROWNUM(0),
			wUSE_CHARMNUM(0),

			wUSE_EXP(0),
			wUSE_HP(0),
			wUSE_MP(0),
			wUSE_SP(0),

			wUSE_HP_PTY(0),
			wUSE_MP_PTY(0),
			wUSE_SP_PTY(0),

			wUSE_CP(0),
			wUSE_BULLET(0),
			wUSE_EXTRA(0)
		{
		}
	};

	struct SBASIC_100
	{
		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		glold::GLITEM_ATT_102	emUSE_LITEM;			//	???? ?? ?? ??.
		glold::GLITEM_ATT_102	emUSE_RITEM;			//	???? ??? ?? ??.
	};

	struct SBASIC_101
	{
		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		glold::GLITEM_ATT_102	emUSE_LITEM;			//	???? ?? ?? ??.
		glold::GLITEM_ATT_102	emUSE_RITEM;			//	???? ??? ?? ??.
	};

	struct SBASIC_102
	{
		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		glold_103::GLITEM_ATT_103	emUSE_LITEM;			//	???? ?? ?? ??.
		glold_103::GLITEM_ATT_103	emUSE_RITEM;			//	???? ??? ?? ??.
	};

	struct SBASIC_103
	{
		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		GLITEM_ATT			emUSE_LITEM;				//	???? ?? ?? ??.
		GLITEM_ATT			emUSE_RITEM;				//	???? ??? ?? ??.
	};

	struct SBASIC_105
	{
		enum { VERSION = SKILLEDIT_VER, };

		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.
		DWORD				dwMAXLEVEL;					//	?? ??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )

		WORD				wTARRANGE;					//	?? ?? ??.

		GLITEM_ATT			emUSE_LITEM;				//	???? ?? ?? ??.
		GLITEM_ATT			emUSE_RITEM;				//	???? ??? ?? ??.
	};

	struct SBASIC_106
	{
		enum { VERSION = SKILLEDIT_VER, };

		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.
		DWORD				dwMAXLEVEL;					//	?? ??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		GLITEM_ATT			emUSE_LITEM;				//	???? ?? ?? ??.
		GLITEM_ATT			emUSE_RITEM;				//	???? ??? ?? ??.

		bool				bLearnView;					//  GM? ??? ??
	};

	struct SBASIC_107
	{
		enum { VERSION = SKILLEDIT_VER, };

		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.
		DWORD				dwMAXLEVEL;					//	?? ??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		GLITEM_ATT			emUSE_LITEM;				//	???? ?? ?? ??.
		GLITEM_ATT			emUSE_RITEM;				//	???? ??? ?? ??.

		bool				bLearnView;					//  GM? ??? ??
	};

	struct SBASIC_108
	{
		enum { VERSION = SKILLEDIT_VER, };

		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.
		DWORD				dwMAXLEVEL;					//	?? ??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		GLITEM_ATT			emUSE_LITEM;				//	???? ?? ?? ??.
		GLITEM_ATT			emUSE_RITEM;				//	???? ??? ?? ??.

		bool				bLearnView;					//  GM? ??? ??
	};

	struct SBASIC_109
	{
		enum { VERSION = SKILLEDIT_VER, };

		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.
		DWORD				dwMAXLEVEL;					//	?? ??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		GLITEM_ATT			emUSE_LITEM;				//	???? ?? ?? ??.
		GLITEM_ATT			emUSE_RITEM;				//	???? ??? ?? ??.

		bool				bLearnView;					//  GM? ??? ??
	};




	struct SBASIC
	{
		enum { VERSION = SKILLEDIT_VER, };

		SNATIVEID			sNATIVEID;					//	?? ID. ( ?? ???? ???? ?? ID )

		char				szNAME[MAX_SZNAME];			//	??? ??.
		DWORD				dwGRADE;					//	??.
		DWORD				dwMAXLEVEL;					//	?? ??.

		EMROLE				emROLE;						//	?? ?? ??. ( ??, ??, ??? )
		EMAPPLY				emAPPLY;					//	?? ??. ( ????, ????, ?? ?? )

		//	Skill ? ?? ?? ??.
		EMIMPACT_TAR		emIMPACT_TAR;				//	??. ( ??, ??, ?? ) &&
		EMIMPACT_REALM		emIMPACT_REALM;				//	??. ( ??, ??? ? ??, ? ?? ) &&
		EMIMPACT_SIDE		emIMPACT_SIDE;				//	"?" ( ? ??? ???, ? ??? ??? )
		
		WORD				wTARRANGE;					//	?? ?? ??.

		GLITEM_ATT			emUSE_LITEM;				//	???? ?? ?? ??.
		GLITEM_ATT			emUSE_RITEM;				//	???? ??? ?? ??.

		bool				bLearnView;					//  GM? ??? ??

		SBASIC (void) :
			dwGRADE(0),
			dwMAXLEVEL(9),

			emROLE(EMROLE_NORMAL),
			emAPPLY(EMAPPLY_MAGIC),

			emIMPACT_TAR(TAR_SPEC),
			emIMPACT_REALM(REALM_SELF),
			emIMPACT_SIDE(SIDE_ENERMY),

			wTARRANGE(50),

			emUSE_LITEM(ITEMATT_NOCARE),
			emUSE_RITEM(ITEMATT_NOCARE),
			bLearnView(FALSE)
		{
			SecureZeroMemory ( szNAME, sizeof(char)*MAX_SZNAME );
		}

		void Assign ( SBASIC_100 &sOldData );
		void Assign ( SBASIC_101 &sOldData );
		void Assign ( SBASIC_102 &sOldData );
		void Assign ( SBASIC_103 &sOldData );
		void Assign ( SBASIC_105 &sOldData );
		void Assign ( SBASIC_106 &sOldData );
		void Assign ( SBASIC_107 &sOldData );
		void Assign ( SBASIC_108 &sOldData );
		void Assign ( SBASIC_109 &sOldData );
		bool LOAD_GS_0112 ( basestream &SFile );
		bool LOAD_GS_0113 ( basestream &SFile );
		bool LOAD_GS_0114 ( basestream &SFile );
		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );
	};

	struct SAPPLY_100
	{
		//	?? ??? ??.
		SKILL::EMTYPES		emBASIC_TYPE;				//	??? ??.
		EMELEMENT			emELEMENT;					//	( ?, ?, ?, ?, ?? ) ??.
		DWORD				dwCUREFLAG;					//	?? ??.
		SKILL::CDATA_LVL_100 sDATA_LVL[MAX_LEVEL];		//	??? ?? ?.

		//	?? ?? ??.
		EMIMPACT_ADDON		emADDON;					//	???? ??.
		float				fADDON_VAR[MAX_LEVEL];		//	?? ??.

		//	???? ??.
		EMSTATE_BLOW		emSTATE_BLOW;				//	?? ?? ?? ??.
		SKILL::SSTATE_BLOW	sSTATE_BLOW[MAX_LEVEL];		//	?? ?? ???.

		//	???? ??.
		EMSPEC_ADDON		emSPEC;						//	??.
		SKILL::SSPEC_100	sSPEC[MAX_LEVEL];			//	?? ?? ???.
	};

	struct SAPPLY_101
	{
		//	?? ??? ??.
		SKILL::EMTYPES		emBASIC_TYPE;				//	??? ??.
		EMELEMENT			emELEMENT;					//	( ?, ?, ?, ?, ?? ) ??.
		DWORD				dwCUREFLAG;					//	?? ??.
		SKILL::CDATA_LVL_101	sDATA_LVL[MAX_LEVEL];		//	??? ?? ?.

		//	?? ?? ??.
		EMIMPACT_ADDON		emADDON;					//	???? ??.
		float				fADDON_VAR[MAX_LEVEL];		//	?? ??.

		//	???? ??.
		EMSTATE_BLOW		emSTATE_BLOW;				//	?? ?? ?? ??.
		SKILL::SSTATE_BLOW	sSTATE_BLOW[MAX_LEVEL];		//	?? ?? ???.

		//	???? ??.
		EMSPEC_ADDON		emSPEC;						//	??.
		SKILL::SSPEC_100	sSPEC[MAX_LEVEL];			//	?? ?? ???.
	};

	struct SAPPLY_102
	{
		//	?? ??? ??.
		SKILL::EMTYPES		emBASIC_TYPE;				//	??? ??.
		EMELEMENT			emELEMENT;					//	( ?, ?, ?, ?, ?? ) ??.
		DWORD				dwCUREFLAG;					//	?? ??.
		SKILL::CDATA_LVL_101	sDATA_LVL[MAX_LEVEL];		//	??? ?? ?.

		//	?? ?? ??.
		EMIMPACT_ADDON		emADDON;					//	???? ??.
		float				fADDON_VAR[MAX_LEVEL];		//	?? ??.

		//	???? ??.
		EMSTATE_BLOW		emSTATE_BLOW;				//	?? ?? ?? ??.
		SKILL::SSTATE_BLOW	sSTATE_BLOW[MAX_LEVEL];		//	?? ?? ???.

		//	???? ??.
		EMSPEC_ADDON		emSPEC;						//	??.
		SKILL::SSPEC_100	sSPEC[MAX_LEVEL];			//	?? ?? ???.
	};

	struct SAPPLY_103
	{
		//	?? ??? ??.
		SKILL::EMTYPES		emBASIC_TYPE;				//	??? ??.
		EMELEMENT			emELEMENT;					//	( ?, ?, ?, ?, ?? ) ??.
		DWORD				dwCUREFLAG;					//	?? ??.
		SKILL::CDATA_LVL_101	sDATA_LVL[MAX_LEVEL];		//	??? ?? ?.

		//	?? ?? ??.
		EMIMPACT_ADDON		emADDON;					//	???? ??.
		float				fADDON_VAR[MAX_LEVEL];		//	?? ??.

		//	???? ??.
		EMSTATE_BLOW		emSTATE_BLOW;				//	?? ?? ?? ??.
		SKILL::SSTATE_BLOW	sSTATE_BLOW[MAX_LEVEL];		//	?? ?? ???.

		//	???? ??.
		EMSPEC_ADDON		emSPEC;						//	??.
		SKILL::SSPEC		sSPEC[MAX_LEVEL];			//	?? ?? ???.
	};

	struct SADDONS
	{
		

		EMIMPACT_ADDON		emADDON;
		float				fADDON_VAR[MAX_LEVEL];
	};

	struct SSPECS
	{
		EMSPEC_ADDON		emSPEC;
		SKILL::SSPEC		sSPEC[MAX_LEVEL];
	};

	struct SAPPLY
	{
		enum { VERSION = 0x0104, };

		//	?? ??? ??.
		SKILL::EMTYPES		emBASIC_TYPE;				//	??? ??.
		EMELEMENT			emELEMENT;					//	( ?, ?, ?, ?, ?? ) ??.
		DWORD				dwCUREFLAG;					//	?? ??.
		SKILL::CDATA_LVL	sDATA_LVL[MAX_LEVEL];		//	??? ?? ?.

		//	?? ?? ??.
		SADDONS sADDONS[10];
		EMIMPACT_ADDON		emADDON;					//	???? ??.
		float				fADDON_VAR[MAX_LEVEL];		//	?? ??.

		//	???? ??.
		EMSTATE_BLOW		emSTATE_BLOW;				//	?? ?? ?? ??.
		SKILL::SSTATE_BLOW	sSTATE_BLOW[MAX_LEVEL];		//	?? ?? ???.

		//	???? ??.
		SSPECS sSPECS[10];
		EMSPEC_ADDON		emSPEC;						//	??.
		SKILL::SSPEC		sSPEC[MAX_LEVEL];			//	?? ?? ???.

		void Assign ( SAPPLY_100 &sOldData );
		void Assign ( SAPPLY_101 &sOldData );
		void Assign ( SAPPLY_102 &sOldData );
		void Assign ( SAPPLY_103 &sOldData );

		BOOL LOAD_GS_110 ( basestream &SFile );
		BOOL LOAD_GS_111 ( basestream &SFile );
		BOOL LOAD_GS_112 ( basestream &SFile );


		SAPPLY (void) :
			emBASIC_TYPE(EMFOR_HP),
			emELEMENT(EMELEMENT_SPIRIT),
			dwCUREFLAG(NULL),

			emADDON(EMIMPACTA_NONE),
			emSTATE_BLOW(EMBLOW_NONE),
			emSPEC(EMSPECA_NULL)
		{
			for ( int i=0; i<MAX_LEVEL; ++i )	fADDON_VAR[i] = 0;
		}

		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

	};

	//	?? ? ?? ??.
	struct SLEARN_LVL_100
	{
		DWORD		dwSKP;					//	?? ?? ???.
		DWORD		dwLEVEL;				//	?? Level.
		//SCHARSTATS			sSTATS;					//	?? Stats.
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		DWORD		dwSKILL_LVL;			//	?? ???? ??.
	};
	struct SLEARN_LVL_101
	{
		DWORD		dwSKP;					//	?? ?? ???.
		DWORD		dwLEVEL;				//	?? Level.
		//SCHARSTATS			sSTATS;					//	?? Stats.
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		DWORD		dwSKILL_LVL;			//	?? ???? ??.
		DWORD		dwREBORN;
		
	};

	struct SLEARN_LVL_102
	{
		DWORD				dwSKP;					//	?? ?? ???.
		DWORD				dwLEVEL;				//	?? Level.
		//SCHARSTATS			sSTATS;					//	?? Stats.
		DWORD		dwPow;
		DWORD		dwStr;
		DWORD		dwSpi;
		DWORD		dwDex;
		DWORD		dwInt;
		DWORD		dwSta;

		DWORD				dwSKILL_LVL;			//	?? ???? ??.
		DWORD				dwREBORN;	
	};

	struct SLEARN_LVL_103
	{
		DWORD				dwSKP;					//	?? ?? ???.
		DWORD				dwLEVEL;				//	?? Level.
		SCHARSTATS			sSTATS;					//	?? Stats.
		

		DWORD				dwSKILL_LVL;			//	?? ???? ??.
		DWORD				dwREBORN;	
	};
		struct SLEARN_LVL_104
	{
		DWORD				dwSKP;					//	?? ?? ???.
		DWORD				dwLEVEL;				//	?? Level.
		SCHARSTATS			sSTATS;					//	?? Stats.
	

		DWORD				dwSKILL_LVL;			//	?? ???? ??.
		DWORD				dwREBORN;
	
		DWORD				dwMoneyReq;
		DWORD				dwPremPoint;
		DWORD				dwVotePoint;
	};

	struct SLEARN_LVL_106
	{
		DWORD				dwSKP;					//	?? ?? ???.
		DWORD				dwLEVEL;				//	?? Level.
		SCHARSTATS			sSTATS;					//	?? Stats.
	

		DWORD				dwSKILL_LVL;			//	?? ???? ??.
		DWORD				dwREBORN;
	
		DWORD				dwMoneyReq;
		DWORD				dwPremPoint;
		DWORD				dwVotePoint;
	};

		struct SLEARN_LVL_PH_100
	{
		DWORD		dwLEVEL;
		WORD		wPow;
		WORD		wStr;
		WORD		wSpi;
		WORD		wDex;
		WORD		wInt;
		WORD		wSta;

		DWORD		dwSKP;
		DWORD		dwSKILL_LVL;

		SLEARN_LVL_PH_100 (void) :
		dwSKP(1),
			dwLEVEL(1),
			dwSKILL_LVL(0)
		{
		}
	};

	struct SLEARN_LVL
	{
		DWORD				dwSKP;					//	?? ?? ???.
		DWORD				dwLEVEL;				//	?? Level.
		SCHARSTATS			sSTATS;					//	?? Stats.
	

		DWORD				dwSKILL_LVL;			//	?? ???? ??.
		DWORD				dwREBORN;
	
		DWORD				dwMoneyReq;
		DWORD				dwPremPoint;
		DWORD				dwVotePoint;

		SLEARN_LVL (void) :
			dwSKP(1),
			dwLEVEL(1),

			dwSKILL_LVL(0),
			dwREBORN(0),

			dwMoneyReq(0),
			dwPremPoint(0),
			dwVotePoint(0)

		{
		}

		void Assign( SLEARN_LVL_100 &sOldData )
		{
			dwSKP = sOldData.dwSKP;
			dwLEVEL = sOldData.dwLEVEL;
			//sSTATS = sOldData.sSTATS;
			sSTATS.dwPow		= 	sOldData.wPow;
			sSTATS.dwStr		= 	sOldData.wStr;
			sSTATS.dwSpi		= 	sOldData.wSpi;
			sSTATS.dwDex		= 	sOldData.wDex;
			sSTATS.dwInt		= 	sOldData.wInt;
			sSTATS.dwSta		= 	sOldData.wSta;

			dwSKILL_LVL			= sOldData.dwSKILL_LVL;
			dwREBORN			= 0;
			
			dwMoneyReq			= 0;
			dwPremPoint			= 0;
			dwVotePoint			= 0;
	
		}

		void Assign( SLEARN_LVL_101 &sOldData )
		{
			dwSKP = sOldData.dwSKP;
			dwLEVEL = sOldData.dwLEVEL;
			//sSTATS = sOldData.sSTATS;
			sSTATS.dwPow		= 	sOldData.wPow;
			sSTATS.dwStr		= 	sOldData.wStr;
			sSTATS.dwSpi		= 	sOldData.wSpi;
			sSTATS.dwDex		= 	sOldData.wDex;
			sSTATS.dwInt		= 	sOldData.wInt;
			sSTATS.dwSta		= 	sOldData.wSta;

			dwSKILL_LVL			= sOldData.dwSKILL_LVL;
			dwREBORN			= sOldData.dwREBORN;

			dwMoneyReq			= 0;
			dwPremPoint			= 0;
			dwVotePoint			= 0;
		}

			void Assign( SLEARN_LVL_102 &sOldData )
		{
			dwSKP = sOldData.dwSKP;
			dwLEVEL = sOldData.dwLEVEL;
			//sSTATS = sOldData.sSTATS;
			sSTATS.dwPow		= 	sOldData.dwPow;
			sSTATS.dwStr		= 	sOldData.dwStr;
			sSTATS.dwSpi		= 	sOldData.dwSpi;
			sSTATS.dwDex		= 	sOldData.dwDex;
			sSTATS.dwInt		= 	sOldData.dwInt;
			sSTATS.dwSta		= 	sOldData.dwSta;
			
			dwSKILL_LVL = sOldData.dwSKILL_LVL;
			dwREBORN = sOldData.dwREBORN;

			dwMoneyReq			= 0;
			dwPremPoint			= 0;
			dwVotePoint			= 0;
		}

		void Assign( SLEARN_LVL_103 &sOldData )
		{
			dwSKP = sOldData.dwSKP;
			dwLEVEL = sOldData.dwLEVEL;
			sSTATS = sOldData.sSTATS;
			
			dwSKILL_LVL = sOldData.dwSKILL_LVL;
			dwREBORN = sOldData.dwREBORN;

			dwMoneyReq			= 0;
			dwPremPoint			= 0;
			dwVotePoint			= 0;
		}
		void Assign( SLEARN_LVL_104 &sOldData )
		{
			dwSKP = sOldData.dwSKP;
			dwLEVEL = sOldData.dwLEVEL;
			sSTATS = sOldData.sSTATS;
			
			dwSKILL_LVL = sOldData.dwSKILL_LVL;
			dwREBORN = sOldData.dwREBORN;

			dwMoneyReq			= 0;
			dwPremPoint			= 0;
			dwVotePoint			= 0;
		}
		void Assign( SLEARN_LVL_106 &sOldData )
		{
			dwSKP = sOldData.dwSKP;
			dwLEVEL = sOldData.dwLEVEL;
			sSTATS = sOldData.sSTATS;
			
			dwSKILL_LVL = sOldData.dwSKILL_LVL;
			dwREBORN = sOldData.dwREBORN;

			dwMoneyReq			= 0;
			dwPremPoint			= 0;
			dwVotePoint			= 0;
		}
	};

	struct SLEARN_100
	{
		EMBRIGHT			emBRIGHT;					//	??.
		DWORD				dwCLASS;					//	??.
		SNATIVEID			sSKILL;						//	?? ????.
		//SNATIVEID			sWEAPON;						//	?? ????.

		SLEARN_LVL_100			sLVL_STEP[MAX_LEVEL];		//	?? ? ?? ??.
	};

	struct SLEARN_101
	{
		EMBRIGHT			emBRIGHT;					//	??.
		DWORD				dwCLASS;					//	??.
		SNATIVEID			sSKILL;						//	?? ????.
	//	SNATIVEID			sWEAPON;						//	?? ????.

		SLEARN_LVL_100			sLVL_STEP[MAX_LEVEL];		//	?? ? ?? ??.
	};

	struct SLEARN_102
	{
		EMBRIGHT			emBRIGHT;					//	??.
		DWORD				dwCLASS;					//	??.
		SNATIVEID			sSKILL;						//	?? ????.
		//SNATIVEID			sWEAPON;						//	?? ????.

		SLEARN_LVL_101			sLVL_STEP[MAX_LEVEL];		//	?? ? ?? ??.
	};

	struct SLEARN_103
	{
		EMBRIGHT			emBRIGHT;					//	??.
		DWORD				dwCLASS;					//	??.
		SNATIVEID			sSKILL;						//	?? ????.
		//SNATIVEID			sWEAPON;						//	?? ????.

		SLEARN_LVL_102			sLVL_STEP[MAX_LEVEL];		//	?? ? ?? ??.
	};

	struct SLEARN_104
	{
		EMBRIGHT			emBRIGHT;					//	??.
		DWORD				dwCLASS;					//	??.
		SNATIVEID			sSKILL;						//	?? ????.

		SLEARN_LVL_103			sLVL_STEP[MAX_LEVEL];		//	?? ? ?? ??.
	};
	struct SLEARN_105
	{

		EMBRIGHT			emBRIGHT;					//	??.
		DWORD				dwCLASS;					//	??.
		SNATIVEID			sSKILL;						//	?? ????.
	

		SLEARN_LVL_104			sLVL_STEP[MAX_LEVEL];		//	?? ? ?? ??.
	};

	struct SLEARN_106
	{
		EMBRIGHT			emBRIGHT;					//	??.
		DWORD				dwCLASS;					//	??.
		SNATIVEID			sSKILL;						//	?? ????.
		SNATIVEID			sWEAPON;						//	?? ????.

		SLEARN_LVL_106			sLVL_STEP[MAX_LEVEL];		//	?? ? ?? ??.
	};

	struct SLEARN_PH_102
	{
		DWORD				dwCLASS;
		EMBRIGHT			emBRIGHT;
		SNATIVEID			sSKILL;	
		SLEARN_LVL_PH_100		sLVL_STEP[MAX_LEVEL];
	};

	struct SLEARN_PH_104
	{
		DWORD				dwCLASS;
		EMBRIGHT			emBRIGHT;
		SNATIVEID			sSKILL;	
		SLEARN_LVL_PH_100		sLVL_STEP[MAX_LEVEL];
		DWORD				dwDummy;
		SNATIVEID			sWEAPON;   
		DWORD				dwDummy1;
	};

	struct SLEARN
	{
		enum { VERSION = 0x0107, };

		EMBRIGHT			emBRIGHT;					//	??.
		DWORD				dwCLASS;					//	??.
		SNATIVEID			sSKILL;						//	?? ????.
		SNATIVEID			sWEAPON;						//	?? ????.
		//DWORD				dwDummy;
		//DWORD				dwDummy1;

		SLEARN_LVL			sLVL_STEP[MAX_LEVEL];		//	?? ? ?? ??.

		SLEARN (void) :
			emBRIGHT(BRIGHT_BOTH),
			dwCLASS(GLCC_NONE),
			sSKILL(NATIVEID_NULL()),
			sWEAPON(NATIVEID_NULL())
			//dwDummy(0),
			//dwDummy1(0)
		{
		}

		void Assign ( SLEARN_100 &OldData )
		{
			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				//sLVL_STEP[i].dwSKP = 1;
				sLVL_STEP[i].Assign ( OldData.sLVL_STEP[i] );
			}
		}
		void Assign ( SLEARN_101 &OldData )
		{
			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				//sLVL_STEP[i].dwSKP = 1;
				sLVL_STEP[i].Assign ( OldData.sLVL_STEP[i] );
			}
		}

		void Assign ( SLEARN_102 &OldData )
		{
			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				//sLVL_STEP[i].dwSKP = 1;
				sLVL_STEP[i].Assign ( OldData.sLVL_STEP[i] );
			}
		}

		void Assign ( SLEARN_103 &OldData )
		{
			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				//sLVL_STEP[i].dwSKP = 1;
				sLVL_STEP[i].Assign ( OldData.sLVL_STEP[i] );
			}
		}

		void Assign ( SLEARN_104 &OldData )
		{
			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				//sLVL_STEP[i].dwSKP = 1;
				sLVL_STEP[i].Assign ( OldData.sLVL_STEP[i] );
			}
		}
		void Assign ( SLEARN_105 &OldData )
		{
			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				//sLVL_STEP[i].dwSKP = 1;
				sLVL_STEP[i].Assign ( OldData.sLVL_STEP[i] );
			}
		}

		void Assign ( SLEARN_106 &OldData )
		{
			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				//sLVL_STEP[i].dwSKP = 1;
				sLVL_STEP[i].Assign ( OldData.sLVL_STEP[i] );
			}
		}

		void Assign ( SLEARN_PH_102 &OldData )
		{
			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				sLVL_STEP[i].dwSKP = OldData.sLVL_STEP[i].dwSKP;
				sLVL_STEP[i].dwLEVEL = OldData.sLVL_STEP[i].dwLEVEL;
				sLVL_STEP[i].dwSKILL_LVL = OldData.sLVL_STEP[i].dwSKILL_LVL;
				sLVL_STEP[i].sSTATS.dwPow =  OldData.sLVL_STEP[i].wPow;
				sLVL_STEP[i].sSTATS.dwDex =  OldData.sLVL_STEP[i].wDex;
				sLVL_STEP[i].sSTATS.dwInt =  OldData.sLVL_STEP[i].wInt;
				sLVL_STEP[i].sSTATS.dwSpi =  OldData.sLVL_STEP[i].wSpi;
				sLVL_STEP[i].sSTATS.dwSta =  OldData.sLVL_STEP[i].wSta;
				sLVL_STEP[i].sSTATS.dwStr =  OldData.sLVL_STEP[i].wStr;
			}

			emBRIGHT = OldData.emBRIGHT;
			dwCLASS = OldData.dwCLASS;
			sSKILL = OldData.sSKILL;

		}

		void Assign ( SLEARN_PH_104 &OldData )
		{
			DWORD	dwDummy(0);
			DWORD	dwDummy1(0);

			for ( int i=0; i<MAX_LEVEL; ++i )
			{
				sLVL_STEP[i].dwSKP = OldData.sLVL_STEP[i].dwSKP;
				sLVL_STEP[i].dwLEVEL = OldData.sLVL_STEP[i].dwLEVEL;
				sLVL_STEP[i].dwSKILL_LVL = OldData.sLVL_STEP[i].dwSKILL_LVL;
				sLVL_STEP[i].sSTATS.dwPow =  OldData.sLVL_STEP[i].wPow;
				sLVL_STEP[i].sSTATS.dwDex =  OldData.sLVL_STEP[i].wDex;
				sLVL_STEP[i].sSTATS.dwInt =  OldData.sLVL_STEP[i].wInt;
				sLVL_STEP[i].sSTATS.dwSpi =  OldData.sLVL_STEP[i].wSpi;
				sLVL_STEP[i].sSTATS.dwSta =  OldData.sLVL_STEP[i].wSta;
				sLVL_STEP[i].sSTATS.dwStr =  OldData.sLVL_STEP[i].wStr;
			}

			emBRIGHT = OldData.emBRIGHT;
			dwCLASS = OldData.dwCLASS;
			sSKILL = OldData.sSKILL;
			dwDummy = 0;
			sWEAPON = OldData.sWEAPON;
			dwDummy1 = 0;
		}
		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

	};

	struct SEXT_DATA_100
	{
		EMANI_MAINTYPE		emANIMTYPE;						//	?? ??? ???? ????? ??.		
		EMANI_SUBTYPE		emANISTYPE;						//	?? ??? ???? ????? ??.		

		EMEFFECTIME			emTARG;
		char				szTARG[MAX_SZFILE];				//	?? ?? ???.
		
		EMEFFECTIME			emSELFZONE01;
		char				szSELFZONE01[MAX_SZFILE];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE01;
		char				szTARGZONE01[MAX_SZFILE];		//	?? ?? ???.
		EMEFFECTIME			emTARGZONE02;
		char				szTARGZONE02[MAX_SZFILE];		//	?? ?? ???.

		EMEFFECTIME			emSELFBODY;
		char				szSELFBODY[MAX_SZFILE];			//	?? ??? ?? ?? ???.
		EMEFFECTIME			emTARGBODY01;
		char				szTARGBODY01[MAX_SZFILE];		//	?? ??? ?? ?? ???.
		EMEFFECTIME			emTARGBODY02;
		char				szTARGBODY02[MAX_SZFILE];		//	?? ??? ?? ?? ???.

		SNATIVEID			sICONINDEX;						//	??? ???.
		char				szICONFILE[MAX_SZFILE];			//	??? ??? ?? ??.
		
		char				szCOMMENTS[MAX_SZCOMMENT];		//	??? ?? ??? ??.
	};

	struct SEXT_DATA_101
	{
		EMANI_MAINTYPE		emANIMTYPE;						//	?? ??? ???? ????? ??.		
		EMANI_SUBTYPE		emANISTYPE;						//	?? ??? ???? ????? ??.		

		EMEFFECTIME			emTARG;
		char				szTARG[MAX_SZFILE];				//	?? ?? ???.
		
		EMEFFECTIME			emSELFZONE01;
		char				szSELFZONE01[MAX_SZFILE];		//	?? ?? ???.

		EMEFFECTIME			emSELFZONE02;
		char				szSELFZONE02[MAX_SZFILE];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE01;
		char				szTARGZONE01[MAX_SZFILE];		//	?? ?? ???.
		
		EMEFFECTIME			emTARGZONE02;
		char				szTARGZONE02[MAX_SZFILE];		//	?? ?? ???.

		EMEFFECTIME			emSELFBODY;
		char				szSELFBODY[MAX_SZFILE];			//	?? ??? ?? ?? ???.
		EMEFFECTIME			emTARGBODY01;
		char				szTARGBODY01[MAX_SZFILE];		//	?? ??? ?? ?? ???.
		EMEFFECTIME			emTARGBODY02;
		char				szTARGBODY02[MAX_SZFILE];		//	?? ??? ?? ?? ???.

		SNATIVEID			sICONINDEX;						//	??? ???.
		char				szICONFILE[MAX_SZFILE];			//	??? ??? ?? ??.
		
		char				szCOMMENTS[MAX_SZCOMMENT];		//	??? ?? ??? ??.
	};

	struct SEXT_DATA_102
	{
		EMANI_MAINTYPE		emANIMTYPE;						//	?? ??? ???? ????? ??.		
		EMANI_SUBTYPE		emANISTYPE;						//	?? ??? ???? ????? ??.		

		EMEFFECTIME			emTARG;
		EMEFFECTPOS			emTARG_POSA;
		EMEFFECTPOS			emTARG_POSB;
		char				szTARG[MAX_SZFILE];				//	?? ?? ???.
		
		EMEFFECTIME			emSELFZONE01;
		EMEFFECTPOS			emSELFZONE01_POS;
		char				szSELFZONE01[MAX_SZFILE];		//	?? ?? ???.

		EMEFFECTIME			emSELFZONE02;
		EMEFFECTPOS			emSELFZONE02_POS;
		char				szSELFZONE02[MAX_SZFILE];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE01;
		EMEFFECTPOS			emTARGZONE01_POS;
		char				szTARGZONE01[MAX_SZFILE];		//	?? ?? ???.
		
		EMEFFECTIME			emTARGZONE02;
		EMEFFECTPOS			emTARGZONE02_POS;
		char				szTARGZONE02[MAX_SZFILE];		//	?? ?? ???.

		EMEFFECTIME			emSELFBODY;
		char				szSELFBODY[MAX_SZFILE];			//	?? ??? ?? ?? ???.
		EMEFFECTIME			emTARGBODY01;
		char				szTARGBODY01[MAX_SZFILE];		//	?? ??? ?? ?? ???.
		EMEFFECTIME			emTARGBODY02;
		char				szTARGBODY02[MAX_SZFILE];		//	?? ??? ?? ?? ???.

		SNATIVEID			sICONINDEX;						//	??? ???.
		char				szICONFILE[MAX_SZFILE];			//	??? ??? ?? ??.
		
		char				szCOMMENTS[MAX_SZCOMMENT];		//	??? ?? ??? ??.
	};

	struct SEXT_DATA_103
	{
		EMANI_MAINTYPE		emANIMTYPE;								//	?? ??? ???? ????? ??.		
		EMANI_SUBTYPE		emANISTYPE;								//	?? ??? ???? ????? ??.		

		EMEFFECTIME			emTARG;
		EMEFFECTPOS			emTARG_POSA;
		EMEFFECTPOS			emTARG_POSB;
		std::string			strTARG[EMELEMENT_OLDMAX];				//	?? ?? ???.
		
		EMEFFECTIME			emSELFZONE01;
		EMEFFECTPOS			emSELFZONE01_POS;
		std::string			strSELFZONE01[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emSELFZONE02;
		EMEFFECTPOS			emSELFZONE02_POS;
		std::string			strSELFZONE02[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE01;
		EMEFFECTPOS			emTARGZONE01_POS;
		std::string			strTARGZONE01[EMELEMENT_OLDMAX];		//	?? ?? ???.
			
		EMEFFECTIME			emTARGZONE02;
		EMEFFECTPOS			emTARGZONE02_POS;
		std::string			strTARGZONE02[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emSELFBODY;
		std::string			strSELFBODY[EMELEMENT_OLDMAX];			//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY01;
		std::string			strTARGBODY01[EMELEMENT_OLDMAX];		//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY02;
		std::string			strTARGBODY02[EMELEMENT_OLDMAX];		//	?? ??? ?? ?? ???.

		std::string			strHOLDOUT;								//	?? ???.

		SNATIVEID			sICONINDEX;								//	??? ???.
		std::string			strICONFILE;							//	??? ??? ?? ??.
		
		std::string			strCOMMENTS;							//	??? ?? ??? ??.

		BOOL LOAD ( basestream &SFile );
	};

	struct SEXT_DATA_104
	{
		EMANI_MAINTYPE		emANIMTYPE;								//	?? ??? ???? ????? ??.		
		EMANI_SUBTYPE		emANISTYPE;								//	?? ??? ???? ????? ??.		

		float				fDELAY4DAMAGE;							//	??? ?? ???.
		EMEFFECTIME			emTARG;
		EMEFFECTPOS			emTARG_POSA;
		EMEFFECTPOS			emTARG_POSB;
		std::string			strTARG[EMELEMENT_OLDMAX];				//	?? ?? ???.
		
		EMEFFECTIME			emSELFZONE01;
		EMEFFECTPOS			emSELFZONE01_POS;
		std::string			strSELFZONE01[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emSELFZONE02;
		EMEFFECTPOS			emSELFZONE02_POS;
		std::string			strSELFZONE02[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE01;
		EMEFFECTPOS			emTARGZONE01_POS;
		std::string			strTARGZONE01[EMELEMENT_OLDMAX];		//	?? ?? ???.
			
		EMEFFECTIME			emTARGZONE02;
		EMEFFECTPOS			emTARGZONE02_POS;
		std::string			strTARGZONE02[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emSELFBODY;
		std::string			strSELFBODY[EMELEMENT_OLDMAX];			//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY01;
		std::string			strTARGBODY01[EMELEMENT_OLDMAX];		//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY02;
		std::string			strTARGBODY02[EMELEMENT_OLDMAX];		//	?? ??? ?? ?? ???.

		std::string			strHOLDOUT;								//	?? ???.

		SNATIVEID			sICONINDEX;								//	??? ???.
		std::string			strICONFILE;							//	??? ??? ?? ??.
		
		std::string			strCOMMENTS;							//	??? ?? ??? ??.

		BOOL LOAD ( basestream &SFile );
	};

	struct SEXT_DATA_105
	{
		EMANI_MAINTYPE		emANIMTYPE;								//	?? ??? ???? ????? ??.		
		EMANI_SUBTYPE		emANISTYPE;								//	?? ??? ???? ????? ??.		

		float				fDELAY4DAMAGE;							//	??? ?? ???.
		EMEFFECTIME			emTARG;
		EMEFFECTPOS			emTARG_POSA;
		EMEFFECTPOS			emTARG_POSB;
		bool				bTARG_ONE;								//	?? ?? ??? ?? ??.
		std::string			strTARG[EMELEMENT_OLDMAX];				//	?? ?? ???.
		
		EMEFFECTIME			emSELFZONE01;
		EMEFFECTPOS			emSELFZONE01_POS;
		std::string			strSELFZONE01[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emSELFZONE02;
		EMEFFECTPOS			emSELFZONE02_POS;
		std::string			strSELFZONE02[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE01;
		EMEFFECTPOS			emTARGZONE01_POS;
		std::string			strTARGZONE01[EMELEMENT_OLDMAX];		//	?? ?? ???.
			
		EMEFFECTIME			emTARGZONE02;
		EMEFFECTPOS			emTARGZONE02_POS;
		std::string			strTARGZONE02[EMELEMENT_OLDMAX];		//	?? ?? ???.

		EMEFFECTIME			emSELFBODY;
		std::string			strSELFBODY[EMELEMENT_OLDMAX];			//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY01;
		std::string			strTARGBODY01[EMELEMENT_OLDMAX];		//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY02;
		std::string			strTARGBODY02[EMELEMENT_OLDMAX];		//	?? ??? ?? ?? ???.

		std::string			strHOLDOUT;								//	?? ???.

		SNATIVEID			sICONINDEX;								//	??? ???.
		std::string			strICONFILE;							//	??? ??? ?? ??.
		
		std::string			strCOMMENTS;							//	??? ?? ??? ??.

		BOOL LOAD ( basestream &SFile );

	};

	struct SEXT_DATA_106
	{
		EMANI_MAINTYPE		emANIMTYPE;								//	?? ??? ???? ????? ??.		
		EMANI_SUBTYPE		emANISTYPE;								//	?? ??? ???? ????? ??.		

		float				fDELAY4DAMAGE;							//	??? ?? ???.
		EMEFFECTIME			emTARG;
		EMEFFECTPOS			emTARG_POSA;
		EMEFFECTPOS			emTARG_POSB;
		bool				bTARG_ONE;								//	?? ?? ??? ?? ??.
		std::string			strTARG[EMELEMENT_MAXNUM];				//	?? ?? ???.
		
		EMEFFECTIME			emSELFZONE01;
		EMEFFECTPOS			emSELFZONE01_POS;
		std::string			strSELFZONE01[EMELEMENT_MAXNUM];		//	?? ?? ???.

		EMEFFECTIME			emSELFZONE02;
		EMEFFECTPOS			emSELFZONE02_POS;
		std::string			strSELFZONE02[EMELEMENT_MAXNUM];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE01;
		EMEFFECTPOS			emTARGZONE01_POS;
		std::string			strTARGZONE01[EMELEMENT_MAXNUM];		//	?? ?? ???.
			
		EMEFFECTIME			emTARGZONE02;
		EMEFFECTPOS			emTARGZONE02_POS;
		std::string			strTARGZONE02[EMELEMENT_MAXNUM];		//	?? ?? ???.

		EMEFFECTIME			emSELFBODY;
		std::string			strSELFBODY[EMELEMENT_MAXNUM];			//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY01;
		std::string			strTARGBODY01[EMELEMENT_MAXNUM];		//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY02;
		std::string			strTARGBODY02[EMELEMENT_MAXNUM];		//	?? ??? ?? ?? ???.

		std::string			strHOLDOUT;								//	?? ???.

		SNATIVEID			sICONINDEX;								//	??? ???.
		std::string			strICONFILE;							//	??? ??? ?? ??.
		
		std::string			strCOMMENTS;							//	??? ?? ??? ??.

		BOOL LOAD ( basestream &SFile );

	};

	struct SEXT_DATA
	{
		enum { VERSION = 0x0107, };

		EMANI_MAINTYPE		emANIMTYPE;								//	?? ??? ???? ????? ??.		
		EMANI_SUBTYPE		emANISTYPE;								//	?? ??? ???? ????? ??.		

		float				fDELAY4DAMAGE;							//	??? ?? ???.
		EMEFFECTIME			emTARG;
		EMEFFECTPOS			emTARG_POSA;
		EMEFFECTPOS			emTARG_POSB;
		bool				bTARG_ONE;								//	?? ?? ??? ?? ??.
		std::string			strTARG[EMELEMENT_MAXNUM];				//	?? ?? ???.
		
		EMEFFECTIME			emSELFZONE01;
		EMEFFECTPOS			emSELFZONE01_POS;
		std::string			strSELFZONE01[EMELEMENT_MAXNUM];		//	?? ?? ???.

		EMEFFECTIME			emSELFZONE02;
		EMEFFECTPOS			emSELFZONE02_POS;
		std::string			strSELFZONE02[EMELEMENT_MAXNUM];		//	?? ?? ???.

		EMEFFECTIME			emSELFZONE03;
		EMEFFECTPOS			emSELFZONE03_POS;
		std::string			strSELFZONE03[EMELEMENT_MAXNUM];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE01;
		EMEFFECTPOS			emTARGZONE01_POS;
		std::string			strTARGZONE01[EMELEMENT_MAXNUM];		//	?? ?? ???.
			
		EMEFFECTIME			emTARGZONE02;
		EMEFFECTPOS			emTARGZONE02_POS;
		std::string			strTARGZONE02[EMELEMENT_MAXNUM];		//	?? ?? ???.

		EMEFFECTIME			emTARGZONE03;
		EMEFFECTPOS			emTARGZONE03_POS;
		std::string			strTARGZONE03[EMELEMENT_MAXNUM];		//	?? ?? ???.

		EMEFFECTIME			emSELFBODY;
		std::string			strSELFBODY[EMELEMENT_MAXNUM];			//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY01;
		std::string			strTARGBODY01[EMELEMENT_MAXNUM];		//	?? ??? ?? ?? ???.

		EMEFFECTIME			emTARGBODY02;
		std::string			strTARGBODY02[EMELEMENT_MAXNUM];		//	?? ??? ?? ?? ???.

		std::string			strHOLDOUT;								//	?? ???.

		SNATIVEID			sICONINDEX;								//	??? ???.
		std::string			strICONFILE;							//	??? ??? ?? ??.
		
		std::string			strCOMMENTS;							//	??? ?? ??? ??.

		SNATIVEID			sADDSKILL;

		DWORD				dwADDSKILLLEVEL;

		inline void LIMIT ( EMELEMENT &emElement )
		{
			GASSERT(emElement<EMELEMENT_MAXNUM2);
			if (emElement==EMELEMENT_MAXNUM) emElement=EMELEMENT_SPIRIT;
		}

		bool VALIDTARG ( EMELEMENT emElement )					{  LIMIT(emElement); return strTARG[emElement].length()!=0; }

		const char* GETTARG ( EMELEMENT emElement )				{  LIMIT(emElement); return strTARG[emElement].c_str(); }
		const char* GETSELFZONE01 ( EMELEMENT emElement )		{  LIMIT(emElement); return strSELFZONE01[emElement].c_str(); }
		const char* GETSELFZONE02 ( EMELEMENT emElement )		{  LIMIT(emElement); return strSELFZONE02[emElement].c_str(); }
		const char* GETSELFZONE03 ( EMELEMENT emElement )		{  LIMIT(emElement); return strSELFZONE03[emElement].c_str(); }
		
		const char* GETTARGZONE01 ( EMELEMENT emElement )		{  LIMIT(emElement); return strTARGZONE01[emElement].c_str(); }
		const char* GETTARGZONE02 ( EMELEMENT emElement )		{  LIMIT(emElement); return strTARGZONE02[emElement].c_str(); }
		const char* GETTARGZONE03 ( EMELEMENT emElement )		{  LIMIT(emElement); return strTARGZONE03[emElement].c_str(); }

		const char* GETSELFBODY ( EMELEMENT emElement )			{  LIMIT(emElement); return strSELFBODY[emElement].c_str(); }

		const char* GETTARGBODY01 ( EMELEMENT emElement )		{  LIMIT(emElement); return strTARGBODY01[emElement].c_str(); }
		const char* GETTARGBODY02 ( EMELEMENT emElement )		{  LIMIT(emElement); return strTARGBODY02[emElement].c_str(); }

		SEXT_DATA (void) :
			emANIMTYPE(AN_SKILL),
			emANISTYPE(AN_SUB_00),

			fDELAY4DAMAGE(0.0f),

			emTARG_POSA(EMPOS_GROUND),
			emTARG_POSB(EMPOS_GROUND),
			bTARG_ONE(false),

			emSELFZONE01_POS(EMPOS_GROUND),
			emSELFZONE02_POS(EMPOS_GROUND),
			emSELFZONE03_POS(EMPOS_GROUND),
			emTARGZONE01_POS(EMPOS_GROUND),
			emTARGZONE02_POS(EMPOS_GROUND),
			emTARGZONE03_POS(EMPOS_GROUND),

			emTARG(EMTIME_IMPACT),
			emSELFZONE01(EMTIME_FIRST),
			emSELFZONE02(EMTIME_IMPACT),
			emSELFZONE03(EMTIME_IMPACT),
			emTARGZONE01(EMTIME_TARG_OVR),
			emTARGZONE02(EMTIME_PASSIVE),
			emTARGZONE03(EMTIME_PASSIVE),

			emSELFBODY(EMTIME_FIRST),
			emTARGBODY01(EMTIME_TARG_OVR),
			emTARGBODY02(EMTIME_PASSIVE),

			sICONINDEX(0,0),
			sADDSKILL(0,0),
			dwADDSKILLLEVEL(0)
		{
		}

		void Assign ( const SEXT_DATA_100 &OldData );
		void Assign ( const SEXT_DATA_101 &OldData );
		void Assign ( const SEXT_DATA_102 &OldData );
		void Assign ( const SEXT_DATA_103 &OldData );
		void Assign ( const SEXT_DATA_104 &OldData );
		void Assign ( const SEXT_DATA_105 &OldData );
		void Assign ( const SEXT_DATA_106 &OldData );

		BOOL SAVE ( CSerialFile &SFile );
		BOOL LOAD ( basestream &SFile );
		BOOL LOAD_GS_0110 ( basestream &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

		SEXT_DATA& operator= ( const SEXT_DATA &value );
	};

	struct SSPECIAL_SKILL_100
	{
		EMSPECIALSKILLTYPE	emSSTYPE;					//  ?? ?? ??
		std::string			strEffectName;				//  ??? ?? ??
		DWORD				dwRemainSecond;				//  ?? ?

		std::string			strTransform_Man[EMTRANSFORM_NSIZE];	//	??? ???? ????
		std::string			strTransform_Woman[EMTRANSFORM_NSIZE];	//	??? ???? ????
	};

	struct SSPECIAL_SKILL_101
	{
		EMSPECIALSKILLTYPE	emSSTYPE;					//  ?? ?? ??
		std::string			strEffectName;				//  ??? ?? ??
		DWORD				dwRemainSecond;				//  ?? ?

		std::string			strTransform_Man[EMTRANSFORM_NSIZE];	//	??? ???? ????
		std::string			strTransform_Woman[EMTRANSFORM_NSIZE];	//	??? ???? ????

		std::string			strTransform_Chf; //add panther

	};

	struct SSUMMON_DATA_103
	{
		//for dummy data
		SNATIVEID sSummon;
		DWORD	dwSummon;
		float	fSummon;
		BOOL	bSummon;
		WORD	wSummon;

		SSUMMON_DATA_103() :
		dwSummon(0),
			fSummon(0.0f),
			bSummon( FALSE ),
			wSummon(0)
		{
		}
	};

	struct SSUMMON_DATA
	{
		//for dummy data
		SNATIVEID		sSummon;
		float			fSummon;
		float			fSummon1;
		DWORD 			dwSummon;
		WORD			wSummon1;
		bool			bSummon;
		DWORD			dwSummon1;


		SSUMMON_DATA() :
			fSummon(0.0f),
			fSummon1(0.0f),
			dwSummon(0),
			wSummon1(0),
			bSummon(false),
			dwSummon1(0)

		{
		}
	};

	struct SSPECIAL_SKILL_102
	{
		//enum { VERSION = 0x0102, };

		EMSPECIALSKILLTYPE	emSSTYPE;					//  ?? ?? ??
		std::string			strEffectName;				//  ??? ?? ??
		DWORD				dwRemainSecond;				//  ?? ?

		std::string			strTransform_Man[EMTRANSFORM_NSIZE];	//	??? ???? ????
		std::string			strTransform_Woman[EMTRANSFORM_NSIZE];	//	??? ???? ????

		std::string			strTransform_Chf; //add panther

		float			fSummon;
		float			fSummon1;
		DWORD 			dwSummon;
		WORD			dwSummon1;
		bool			bSummon;
		DWORD			wSummon;
	};

	struct SSPECIAL_SKILL_OLD
	{

		EMSPECIALSKILLTYPE	emSSTYPE;					//  ?? ?? ??
		std::string			strEffectName;				//  ??? ?? ??
		DWORD				dwRemainSecond;				//  ?? ?

		std::string			strTransform_Man[EMTRANSFORM_NSIZE];	//	??? ???? ????
		std::string			strTransform_Woman[EMTRANSFORM_NSIZE];	//	??? ???? ????

		SSUMMON_DATA	sSummon[MAX_LEVEL];
	};


	struct SSPECIAL_SKILL
	{
		enum { VERSION = 0x0102, };

		EMSPECIALSKILLTYPE	emSSTYPE;					//  ?? ?? ??
		std::string			strEffectName;				//  ??? ?? ??
		DWORD				dwRemainSecond;				//  ?? ?
		SNATIVEID			sSummonID; //add summon
		DWORD				dwSummonTime;	//add summon

		std::string			strTransform_Man[EMTRANSFORM_NSIZE];	//	??? ???? ????
		std::string			strTransform_Woman[EMTRANSFORM_NSIZE];	//	??? ???? ????
		std::string			strTransform_Chf; //add panther

		SSUMMON_DATA		sSummon[MAX_LEVEL];


		SSPECIAL_SKILL() :
			emSSTYPE(EMSSTYPE_NONE),
			dwRemainSecond(0),
			sSummonID(NATIVEID_NULL()),//add summon
			dwSummonTime(0)//add summon
			{
			}

		void ResetAll()
		{
			strEffectName.clear();
			dwRemainSecond = 0;
			
			for( BYTE i = 0; i < SKILL::EMTRANSFORM_NSIZE; i++ )
			{
				strTransform_Man[i].clear();
				strTransform_Woman[i].clear();
			}
		}

		BOOL SAVE ( CSerialFile &SFile );
		BOOL LOAD_100 ( basestream &SFile );
		BOOL LOAD_101 ( basestream &SFile );
		BOOL LOAD_102 ( basestream &SFile );
		BOOL LOAD ( basestream &SFile );

		BOOL LOAD_GS_101 ( basestream &SFile );
		BOOL LOAD_GS_103 ( basestream &SFile );
		BOOL LOAD_GS_104 ( basestream &SFile );


		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray );

		SSPECIAL_SKILL & operator= ( const SSPECIAL_SKILL  &value );

	};

};


struct GLSKILL
{
	enum
	{
		VERSION			= 0x0100,

		FILE_SBASIC			= 1,
		FILE_SAPPLY			= 2,
		FILE_SLEARN			= 3,
		FILE_SEXT_DATA		= 4,
		FILE_SPECIAL_SKILL = 5,

		FILE_END_DATA	= 0xEDEDEDED,
	};

	SKILL::SBASIC		  m_sBASIC;
	SKILL::SAPPLY		  m_sAPPLY;
	SKILL::SLEARN		  m_sLEARN;
	SKILL::SEXT_DATA	  m_sEXT_DATA;
	SKILL::SSPECIAL_SKILL m_sSPECIAL_SKILL;

	GLSKILL ()
	{
	}

	BOOL SaveFile ( CSerialFile &SFile );
	BOOL LoadFile ( basestream &SFile, bool bPastLoad, bool bGS = FALSE );

	static VOID SaveCsvHead ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile );
	VOID LoadCsv ( CStringArray &StrArray, int iLine );


	GLSKILL& operator= ( GLSKILL &value )
	{
		m_sBASIC		 = value.m_sBASIC;
		m_sAPPLY		 = value.m_sAPPLY;
		m_sLEARN		 = value.m_sLEARN;
		m_sEXT_DATA		 = value.m_sEXT_DATA;
		m_sSPECIAL_SKILL = value.m_sSPECIAL_SKILL;

		return *this;
	}

	bool	IsSkillFact ();		//	??? ??
	const char* GetName();
	const char* GetDesc();
};
typedef GLSKILL* PGLSKILL;

class GLSkillMan
{
public:
	enum
	{
		ENCODE_VER = 0x0101,

		VERSION = 0x0101,
		MAX_CLASSSKILL = 99,
	};

	static const char* _FILEHEAD;
	static const char* _LOGFILE;
	static const char* _STRINGTABLE;

protected:
	char				m_szFileName[MAX_PATH];
	
	bool				m_bModify;
	PGLSKILL			m_pSkills[EMSKILLCLASS_NSIZE][MAX_CLASSSKILL];

	LPDIRECT3DDEVICEQ	m_pd3dDevice;

public:
	PGLSKILL GetData ( const WORD wClass, const WORD Index );
	PGLSKILL GetData ( const SNATIVEID &sID )	{ return GetData(sID.wMainID,sID.wSubID); }

	void	SetData ( WORD wClass, WORD Index, const PGLSKILL SkillData, bool binner=false );

public:
	WORD	FindFreeSkillIndex ( WORD wClass );
	BOOL	DeleteSkill ( WORD wClass, WORD Index );
	void	GetMaxSkill ( WORD& wClass, WORD& rIndex );	

public:
	bool	ValidData ();

public:
	bool	IsModify ()							{ return m_bModify; }

public:
	HRESULT OneTimeSceneInit ();
	HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT DeleteDeviceObjects ();
	HRESULT FinalCleanup ();

public:
	HRESULT LoadFile ( const char* szFile, bool bPastLoad, bool bGS = FALSE );
	HRESULT SaveFile ( const char* szFile );

public:
	HRESULT	SyncStringTable();
	HRESULT SyncUpdateData ();

public:
	HRESULT SaveCsvFile( CWnd* pWnd );
	HRESULT LoadCsvFile( CWnd* pWnd );


public:
	GLSkillMan ();

public:
	~GLSkillMan ();

public:
	static GLSkillMan& GetInstance();
};

namespace COMMENT
{
	enum { SKILL_ICON_SIZE=256 };

	extern std::string SKILL_ROLE[SKILL::EMROLE_NSIZE];
	extern std::string SKILL_APPLY[SKILL::EMAPPLY_NSIZE];

	extern std::string SKILL_TYPES[SKILL::FOR_TYPE_SIZE];
	extern float SKILL_TYPES_SCALE[SKILL::FOR_TYPE_SIZE];
	inline bool IsSKILL_PER ( SKILL::EMTYPES emTYPE )			{ return SKILL_TYPES_SCALE[emTYPE]==100.0f; }

	extern std::string SKILL_LEVEL[SKILL::MAX_LEVEL];

	extern std::string SKILL_EFFTIME[SKILL::EMTIME_NSIZE];
	extern std::string SKILL_EFFPOS[SKILL::EMPOS_NSIZE];
	extern std::string SPECIAL_SKILL_TYPE[SKILL::EMSSTYPE_NSIZE];

	extern std::string EMACTION[SKILL::EMSTIGMA_SIZE];
	extern std::string SPEC_HALLUCINATE_TYPE[SKILL::EMHALLUCINATE_SIZE];
	extern std::string SPEC_DOMINATE_TYPE[SKILL::EMDOMINATE_SIZE];
	extern std::string SPEC_ASIGN_TYPE[SKILL::EMASIGN_SIZE];
	extern std::string SPEC_INC_TYPE[SKILL::EMINC_SIZE];
	extern std::string SPEC_INVISIBLE_USE_TYPE[SKILL::EMINVISIBLE_SIZE];
	extern std::string SPEC_INVISIBLE_ANI_TYPE[SKILL::EMINVISIBLE_ANI_SIZE];
	extern std::string SPEC_SWAPPOS_DIR_TYPE[SKILL::EMSWAPPOS_DIR_SIZE];
};

#endif // GLSKILL_H_