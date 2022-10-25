#include "stdafx.h"
#include "./GLSkill.h"
#include "./GLGaeaServer.h"
#include "./GLAgentServer.h"
#include "./GLogicData.h"
#include "./GLStringTable.h"

#include "../NpcTalk/NpcDialogueSet.h"

#include "../../EngineLib/Common/SeqRandom.h"
#include "../../EngineLib/Common/StringFile.h"
#include "../../EngineLib/Common/SerialFile.h"
#include "../../EngineLib/Common/STRINGUTILS.h"
#include "../../EngineLib/Common/SUBPATH.h"
#include "../../EngineLib/DxOctree/DxLandMan.h"
#include "../../EngineLib/DxResponseMan.h"
#include "../../EngineLib/G-Logic/GLOGIC.h"
#include "../../EngineLib/G-Logic/GLPeriod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLSkillMan& GLSkillMan::GetInstance()
{
	static GLSkillMan Instance;
	return Instance;
}

const char* GLSkillMan::_FILEHEAD = "GLCROW";

namespace SKILL
{

	GLITEM_ATT			GLSKILATT_TO_GLITEMATT( GLSKILL_ATT emSKILLATT )
	{
		GLITEM_ATT	emATT = ITEMATT_NOTHING;

		switch( emSKILLATT )
		{
		case SKILLATT_NOTHING:
			emATT = ITEMATT_NOTHING;
			break;
		case SKILLATT_SWORD:
			emATT = ITEMATT_SWORD;
			break;
		case SKILLATT_BLADE:
			emATT = ITEMATT_SABER;
			break;
		case SKILLATT_SWORDBLADE:
			emATT = ITEMATT_SWORDSABER;
			break;
		case SKILLATT_DAGGER:
			emATT = ITEMATT_DAGGER;
			break;
		case SKILLATT_SPEAR:
			emATT = ITEMATT_SPEAR;
			break;
		case SKILLATT_STICK:
			emATT = ITEMATT_STICK;
			break;
		case SKILLATT_GAUNT:
			emATT = ITEMATT_GAUNT;
			break;
		case SKILLATT_BOW:
			emATT = ITEMATT_BOW;
			break;
		case SKILLATT_THROW:
			emATT = ITEMATT_THROW;
			break;

		case SKILLATT_GUN:
			emATT = ITEMATT_DUAL_GUN;
			break;
		case SKILLATT_RAILGUN:
			emATT = ITEMATT_RAIL_GUN;
			break;
		case SKILLATT_PORTALGUN:
			emATT = ITEMATT_PORTAL_GUN;
			break;

		case SKILLATT_SHOTGUN:
			emATT = ITEMATT_GUN;
			break;
		case SKILLATT_RAILPORTALGUN:
			emATT = ITEMATT_GUN;
			break;

		case SKILLATT_SCYTHE:
			emATT = ITEMATT_SCYTHE;
			break;
		case SKILLATT_DUALSPEAR:
			emATT = ITEMATT_DUALSPEAR;
			break;
		case SKILLATT_SHURIKEN:
			emATT = ITEMATT_SHURIKEN;
			break;

		case SKILLATT_FIST:
			emATT = ITEMATT_FIST;
			break;
		case SKILLATT_WAND:
			emATT = ITEMATT_WAND;
			break;
		case SKILLATT_CUBE:
			emATT = ITEMATT_CUBE;
			break;

		case SKILLATT_WHIP:
			emATT = ITEMATT_WHIP;
			break;
		case SKILLATT_HAMMER:
			emATT = ITEMATT_HAMMER;
			break;
		case SKILLATT_SHIELD:
			emATT = ITEMATT_SHIELD;
			break;
		case SKILLATT_UMBRELLA:
			emATT = ITEMATT_UMBRELLA;
			break;
		case SKILLATT_NOCARE:
			emATT = ITEMATT_NOCARE;
			break;
		};

		return emATT;

	};

	// Csv ??? ? ??
	int iCsvCur = 0;

	BOOL SSPECIAL_SKILL::LOAD_100( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> strEffectName;
		SFile >> dwRemainSecond;
		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}
		return TRUE;
	}

	BOOL SSPECIAL_SKILL::LOAD_101( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> strEffectName;
		SFile >> dwRemainSecond;

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}
		return TRUE;
	}

	
	BOOL SSPECIAL_SKILL::LOAD_102( basestream &SFile )
	{
		DWORD dwDATA;
		string	sData;

		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> strEffectName;
		SFile >> dwRemainSecond;

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		SFile >> sData; //add panther

		return TRUE;
	}
	
	BOOL SSPECIAL_SKILL::LOAD( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> strEffectName;
		SFile >> dwRemainSecond;

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		for( int i = 0; i < MAX_LEVEL; i++ )
		{
			SFile >> sSummon[i].sSummon.dwID;
			SFile >> sSummon[i].fSummon;
			SFile >> sSummon[i].fSummon1;
			SFile >> sSummon[i].dwSummon;
			SFile >> sSummon[i].wSummon1;
			SFile >> sSummon[i].bSummon;
			SFile >> sSummon[i].dwSummon1;
		}

		return TRUE;
	}

	BOOL SSPECIAL_SKILL::SAVE( CSerialFile &SFile )
	{
		SFile << (DWORD)emSSTYPE;
		SFile << strEffectName;
		SFile << dwRemainSecond;


		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile << strTransform_Man[i];
			SFile << strTransform_Woman[i];
		}

		for( int i = 0; i < MAX_LEVEL; i++ )
		{
			SFile << sSummon[i].sSummon.dwID;
			SFile << sSummon[i].fSummon;
			SFile << sSummon[i].fSummon1;
			SFile << sSummon[i].dwSummon;
			SFile << sSummon[i].wSummon1;
			SFile << sSummon[i].bSummon;
			SFile << sSummon[i].dwSummon1;
		}

		return TRUE;
	}


	BOOL SSPECIAL_SKILL::LOAD_GS_101( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;

		SFile >> dwRemainSecond;
		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		SFile >> strEffectName;
		return TRUE;
	}

	BOOL SSPECIAL_SKILL::LOAD_GS_103( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;

		SFile >> strEffectName;

		SFile >> dwRemainSecond;
		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		SSUMMON_DATA_103		sSummon[MAX_LEVEL];

		for( int i = 0; i < MAX_LEVEL; i++ )
		{
			SFile >> sSummon[i].sSummon.dwID;
			SFile >> sSummon[i].dwSummon;
			SFile >> sSummon[i].fSummon;
			SFile >> sSummon[i].bSummon;
			SFile >> sSummon[i].wSummon;
		}

		return TRUE;
	}

	BOOL SSPECIAL_SKILL::LOAD_GS_104( basestream &SFile )
	{
		DWORD dwDATA;

		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;

		SFile >> strEffectName;

		SFile >> dwRemainSecond;
		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		SSUMMON_DATA		sSummon[MAX_LEVEL];

		for( int i = 0; i < MAX_LEVEL; i++ )
		{
			sSummon[i].sSummon.wMainID = 0;
			sSummon[i].sSummon.wSubID = 0;

			SFile >> sSummon[i].sSummon.dwID;
			SFile >> sSummon[i].fSummon;
			SFile >> sSummon[i].fSummon1;
			SFile >> sSummon[i].dwSummon;
			SFile >> sSummon[i].wSummon1;
			SFile >> sSummon[i].bSummon;
			SFile >> sSummon[i].dwSummon1;

		}

		return TRUE;
	}

	SSPECIAL_SKILL& SSPECIAL_SKILL::operator= ( const SSPECIAL_SKILL &value )
	{
		emSSTYPE	   = value.emSSTYPE;
		strEffectName  = value.strEffectName;
		dwRemainSecond = value.dwRemainSecond;

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			strTransform_Man[i]		  = value.strTransform_Man[i];
			strTransform_Woman[i]	  = value.strTransform_Woman[i];
		}
		
		for( int i = 0; i < MAX_LEVEL; i++ )
		{
			sSummon[i].sSummon.wMainID = value.sSummon[i].sSummon.wMainID;
			sSummon[i].sSummon.wSubID = value.sSummon[i].sSummon.wSubID;
			sSummon[i].fSummon = value.sSummon[i].fSummon;
			sSummon[i].fSummon1 = value.sSummon[i].fSummon1;
			sSummon[i].dwSummon = value.sSummon[i].dwSummon;
			sSummon[i].wSummon1 = value.sSummon[i].wSummon1;
			sSummon[i].bSummon	=	value.sSummon[i].bSummon;
			sSummon[i].dwSummon1 = value.sSummon[i].dwSummon1;
		}

		return *this;
	}

	VOID SSPECIAL_SKILL::SaveCsvHead ( std::fstream &SFile )
	{
        SFile << "Special Skill Type" << ",";
		SFile << "Special Skill Effect" << ",";
		SFile << "Effect Remain Second" << ",";

		SFile << "Summon MID" << ",";	//add summon
		SFile << "Summon SID" << ",";	//add summon
		SFile << "Summon Time" << ",";	//add summon

		SFile << "MAN_HEAD_File" << ",";
		SFile << "MAN_FACE_File" << ",";
		SFile << "MAN_UPPER_File" << ",";
		SFile << "MAN_LOWER_File" << ",";
		SFile << "MAN_LHAND_File" << ",";
		SFile << "MAN_RHAND_File" << ",";
		SFile << "MAN_FOOT_File" << ",";
		SFile << "MAN_HAND_File" << ",";
		SFile << "WOMAN_HEAD_File" << ",";
		SFile << "WOMAN_FACE_File" << ",";
		SFile << "WOMAN_UPPER_File" << ",";
		SFile << "WOMAN_LOWER_File" << ",";
		SFile << "WOMAN_LHAND_File" << ",";
		SFile << "WOMAN_RHAND_File" << ",";
		SFile << "WOMAN_FOOT_File" << ",";
		SFile << "WOMAN_HAND_File" << ",";

		SFile << "TRANSFORM_CHF" << ","; //add panther
	}

	VOID SSPECIAL_SKILL::SaveCsv ( std::fstream &SFile )
	{
		SFile << (DWORD)emSSTYPE << ",";
		STRUTIL::OutputStrCsv( SFile, strEffectName );
		SFile << dwRemainSecond << ",";

		SFile << sSummonID.wMainID << ","; //add summon
		SFile << sSummonID.wSubID << ","; //add summon
		SFile << dwSummonTime << ","; //add summon


		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			STRUTIL::OutputStrCsv( SFile, strTransform_Man[i] );
		}
		for(int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			STRUTIL::OutputStrCsv( SFile, strTransform_Woman[i] );
		}

		STRUTIL::OutputStrCsv( SFile, strTransform_Chf ); //add panther
	}

	VOID SSPECIAL_SKILL::LoadCsv ( CStringArray &StrArray )
	{
		emSSTYPE	   = (EMSPECIALSKILLTYPE)atoi( StrArray[ iCsvCur++ ] );
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strEffectName );
		dwRemainSecond = (DWORD)atoi( StrArray[ iCsvCur++ ] );

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTransform_Man[i] );
		}
		for(int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTransform_Woman[i] );
		}

		for(int i = 0; i < MAX_LEVEL; i++)
		{
			sSummon[i].sSummon.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].sSummon.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].fSummon = (float)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].fSummon1 = (float)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].dwSummon = (DWORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].wSummon1 = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].bSummon = atoi( StrArray[ iCsvCur++ ] ) != 0 ? true : false;
			sSummon[i].dwSummon1 = (WORD)atoi( StrArray[ iCsvCur++ ] );
		}

		
	}



	void SEXT_DATA::Assign ( const SEXT_DATA_100 &OldData )
	{
		emANIMTYPE = OldData.emANIMTYPE;
		emANISTYPE = OldData.emANISTYPE;

		emTARG = OldData.emTARG;
		strTARG[EMELEMENT_SPIRIT] = OldData.szTARG;
		
		emSELFZONE01 = OldData.emSELFZONE01;
		strSELFZONE01[EMELEMENT_SPIRIT] = OldData.szSELFZONE01;
		
		emTARGZONE01 = OldData.emTARGZONE01;
		strTARGZONE01[EMELEMENT_SPIRIT] = OldData.szTARGZONE01;

		emTARGZONE02 = OldData.emTARGZONE02;
		strTARGZONE02[EMELEMENT_SPIRIT] = OldData.szTARGZONE02;

		emSELFBODY = OldData.emSELFBODY;
		strSELFBODY[EMELEMENT_SPIRIT] = OldData.szSELFBODY;

		emTARGBODY01 = OldData.emTARGBODY01;
		strTARGBODY01[EMELEMENT_SPIRIT] = OldData.szTARGBODY01;

		emTARGBODY02 = OldData.emTARGBODY02;
		strTARGBODY02[EMELEMENT_SPIRIT] = OldData.szTARGBODY02;

		sICONINDEX = OldData.sICONINDEX;
		strICONFILE = OldData.szICONFILE;

		strCOMMENTS = OldData.szCOMMENTS;
	}

	void SEXT_DATA::Assign ( const SEXT_DATA_101 &OldData )
	{
		emANIMTYPE = OldData.emANIMTYPE;
		emANISTYPE = OldData.emANISTYPE;

		emTARG = OldData.emTARG;
		strTARG[EMELEMENT_SPIRIT] = OldData.szTARG;
		
		emSELFZONE01 = OldData.emSELFZONE01;
		strSELFZONE01[EMELEMENT_SPIRIT] = OldData.szSELFZONE01;

		emSELFZONE02 = OldData.emSELFZONE02;
		strSELFZONE02[EMELEMENT_SPIRIT] = OldData.szSELFZONE02;

		emTARGZONE01 = OldData.emTARGZONE01;
		strTARGZONE01[EMELEMENT_SPIRIT] = OldData.szTARGZONE01;

		emTARGZONE02 = OldData.emTARGZONE02;
		strTARGZONE02[EMELEMENT_SPIRIT] = OldData.szTARGZONE02;

		emSELFBODY = OldData.emSELFBODY;
		strSELFBODY[EMELEMENT_SPIRIT] = OldData.szSELFBODY;

		emTARGBODY01 = OldData.emTARGBODY01;
		strTARGBODY01[EMELEMENT_SPIRIT] = OldData.szTARGBODY01;

		emTARGBODY02 = OldData.emTARGBODY02;
		strTARGBODY02[EMELEMENT_SPIRIT] = OldData.szTARGBODY02;

		sICONINDEX = OldData.sICONINDEX;
		strICONFILE = OldData.szICONFILE;

		strCOMMENTS = OldData.szCOMMENTS;
	}

	void SEXT_DATA::Assign ( const SEXT_DATA_102 &OldData )
	{
		emANIMTYPE = OldData.emANIMTYPE;
		emANISTYPE = OldData.emANISTYPE;

		emTARG = OldData.emTARG;
		emTARG_POSA = OldData.emTARG_POSA;
		emTARG_POSB = OldData.emTARG_POSB;
		strTARG[EMELEMENT_SPIRIT] = OldData.szTARG;
		
		emSELFZONE01 = OldData.emSELFZONE01;
		emSELFZONE01_POS = OldData.emSELFZONE01_POS;
		strSELFZONE01[EMELEMENT_SPIRIT] = OldData.szSELFZONE01;

		emSELFZONE02 = OldData.emSELFZONE02;
		emSELFZONE02_POS = OldData.emSELFZONE02_POS;
		strSELFZONE02[EMELEMENT_SPIRIT] = OldData.szSELFZONE02;

		emTARGZONE01 = OldData.emTARGZONE01;
		emTARGZONE01_POS = OldData.emTARGZONE01_POS;
		strTARGZONE01[EMELEMENT_SPIRIT] = OldData.szTARGZONE01;

		emTARGZONE02 = OldData.emTARGZONE02;
		emTARGZONE02_POS = OldData.emTARGZONE02_POS;
		strTARGZONE02[EMELEMENT_SPIRIT] = OldData.szTARGZONE02;

		emSELFBODY = OldData.emSELFBODY;
		strSELFBODY[EMELEMENT_SPIRIT] = OldData.szSELFBODY;

		emTARGBODY01 = OldData.emTARGBODY01;
		strTARGBODY01[EMELEMENT_SPIRIT] = OldData.szTARGBODY01;

		emTARGBODY02 = OldData.emTARGBODY02;
		strTARGBODY02[EMELEMENT_SPIRIT] = OldData.szTARGBODY02;

		sICONINDEX = OldData.sICONINDEX;
		strICONFILE = OldData.szICONFILE;

		strCOMMENTS = OldData.szCOMMENTS;
	}

	void SEXT_DATA::Assign ( const SEXT_DATA_103 &value )
	{
		int i=0;

		emANIMTYPE = value.emANIMTYPE;
		emANISTYPE = value.emANISTYPE;

		emTARG = value.emTARG;
		emTARG_POSA = value.emTARG_POSA;
		emTARG_POSB = value.emTARG_POSB;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARG[i] = value.strTARG[i];
		
		emSELFZONE01 = value.emSELFZONE01;
		emSELFZONE01_POS = value.emSELFZONE01_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFZONE01[i] = value.strSELFZONE01[i];

		emSELFZONE02 = value.emSELFZONE02 ;
		emSELFZONE02_POS = value.emSELFZONE02_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFZONE02[i] = value.strSELFZONE02[i];

		emTARGZONE01 = value.emTARGZONE01;
		emTARGZONE01_POS = value.emTARGZONE01_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGZONE01[i] = value.strTARGZONE01[i];
			
		emTARGZONE02 = value.emTARGZONE02;
		emTARGZONE02_POS = value.emTARGZONE02_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGZONE02[i] = value.strTARGZONE02[i];

		emSELFBODY = value.emSELFBODY;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFBODY[i] = value.strSELFBODY[i];

		emTARGBODY01 = value.emTARGBODY01;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGBODY01[i] = value.strTARGBODY01[i];

		emTARGBODY02 = value.emTARGBODY02;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGBODY02[i] = value.strTARGBODY02[i];

		strHOLDOUT = value.strHOLDOUT;

		sICONINDEX = value.sICONINDEX;
		strICONFILE = value.strICONFILE;
		
		strCOMMENTS = value.strCOMMENTS;
	}

	void SEXT_DATA::Assign ( const SEXT_DATA_104 &value )
	{
		int i=0;

		emANIMTYPE = value.emANIMTYPE;
		emANISTYPE = value.emANISTYPE;

		fDELAY4DAMAGE = value.fDELAY4DAMAGE;
		emTARG = value.emTARG;
		emTARG_POSA = value.emTARG_POSA;
		emTARG_POSB = value.emTARG_POSB;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARG[i] = value.strTARG[i];
		
		emSELFZONE01 = value.emSELFZONE01;
		emSELFZONE01_POS = value.emSELFZONE01_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFZONE01[i] = value.strSELFZONE01[i];

		emSELFZONE02 = value.emSELFZONE02 ;
		emSELFZONE02_POS = value.emSELFZONE02_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFZONE02[i] = value.strSELFZONE02[i];

		emTARGZONE01 = value.emTARGZONE01;
		emTARGZONE01_POS = value.emTARGZONE01_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGZONE01[i] = value.strTARGZONE01[i];
			
		emTARGZONE02 = value.emTARGZONE02;
		emTARGZONE02_POS = value.emTARGZONE02_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGZONE02[i] = value.strTARGZONE02[i];

		emSELFBODY = value.emSELFBODY;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFBODY[i] = value.strSELFBODY[i];

		emTARGBODY01 = value.emTARGBODY01;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGBODY01[i] = value.strTARGBODY01[i];

		emTARGBODY02 = value.emTARGBODY02;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGBODY02[i] = value.strTARGBODY02[i];

		strHOLDOUT = value.strHOLDOUT;

		sICONINDEX = value.sICONINDEX;
		strICONFILE = value.strICONFILE;
		
		strCOMMENTS = value.strCOMMENTS;
	}

	void SEXT_DATA::Assign ( const SEXT_DATA_105 &value )
	{
		int i=0;

		emANIMTYPE = value.emANIMTYPE;
		emANISTYPE = value.emANISTYPE;

		fDELAY4DAMAGE = value.fDELAY4DAMAGE;
		emTARG = value.emTARG;
		emTARG_POSA = value.emTARG_POSA;
		emTARG_POSB = value.emTARG_POSB;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARG[i] = value.strTARG[i];
		
		emSELFZONE01 = value.emSELFZONE01;
		emSELFZONE01_POS = value.emSELFZONE01_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFZONE01[i] = value.strSELFZONE01[i];

		emSELFZONE02 = value.emSELFZONE02 ;
		emSELFZONE02_POS = value.emSELFZONE02_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFZONE02[i] = value.strSELFZONE02[i];

		emTARGZONE01 = value.emTARGZONE01;
		emTARGZONE01_POS = value.emTARGZONE01_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGZONE01[i] = value.strTARGZONE01[i];
			
		emTARGZONE02 = value.emTARGZONE02;
		emTARGZONE02_POS = value.emTARGZONE02_POS;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGZONE02[i] = value.strTARGZONE02[i];

		emSELFBODY = value.emSELFBODY;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strSELFBODY[i] = value.strSELFBODY[i];

		emTARGBODY01 = value.emTARGBODY01;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGBODY01[i] = value.strTARGBODY01[i];

		emTARGBODY02 = value.emTARGBODY02;
		for ( i=0; i<EMELEMENT_OLDMAX; ++i )		strTARGBODY02[i] = value.strTARGBODY02[i];

		strHOLDOUT = value.strHOLDOUT;

		sICONINDEX = value.sICONINDEX;
		strICONFILE = value.strICONFILE;
		
		strCOMMENTS = value.strCOMMENTS;
	}

	void SEXT_DATA::Assign ( const SEXT_DATA_106 &value )
	{
		int i=0;

		emANIMTYPE = value.emANIMTYPE;
		emANISTYPE = value.emANISTYPE;

		fDELAY4DAMAGE = value.fDELAY4DAMAGE;
		emTARG = value.emTARG;
		emTARG_POSA = value.emTARG_POSA;
		emTARG_POSB = value.emTARG_POSB;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARG[i] = value.strTARG[i];
		
		emSELFZONE01 = value.emSELFZONE01;
		emSELFZONE01_POS = value.emSELFZONE01_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strSELFZONE01[i] = value.strSELFZONE01[i];

		emSELFZONE02 = value.emSELFZONE02 ;
		emSELFZONE02_POS = value.emSELFZONE02_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strSELFZONE02[i] = value.strSELFZONE02[i];

		emTARGZONE01 = value.emTARGZONE01;
		emTARGZONE01_POS = value.emTARGZONE01_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGZONE01[i] = value.strTARGZONE01[i];
			
		emTARGZONE02 = value.emTARGZONE02;
		emTARGZONE02_POS = value.emTARGZONE02_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGZONE02[i] = value.strTARGZONE02[i];

		emSELFBODY = value.emSELFBODY;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strSELFBODY[i] = value.strSELFBODY[i];

		emTARGBODY01 = value.emTARGBODY01;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGBODY01[i] = value.strTARGBODY01[i];

		emTARGBODY02 = value.emTARGBODY02;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGBODY02[i] = value.strTARGBODY02[i];

		strHOLDOUT = value.strHOLDOUT;

		sICONINDEX = value.sICONINDEX;
		strICONFILE = value.strICONFILE;
		
		strCOMMENTS = value.strCOMMENTS;
	}

	BOOL SEXT_DATA::SAVE ( CSerialFile &SFile )
	{
		SFile << (DWORD)emANIMTYPE;
		SFile << (DWORD)emANISTYPE;

		SFile << fDELAY4DAMAGE;
		SFile << (DWORD)emTARG;
		SFile << (DWORD)emTARG_POSA;
		SFile << (DWORD)emTARG_POSB;
		SFile << bTARG_ONE;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strTARG[i];
		
		SFile << (DWORD)emSELFZONE01;
		SFile << (DWORD)emSELFZONE01_POS;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strSELFZONE01[i];

		SFile << (DWORD)emSELFZONE02;
		SFile << (DWORD)emSELFZONE02_POS;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strSELFZONE02[i];

		SFile << (DWORD)emSELFZONE03;
		SFile << (DWORD)emSELFZONE03_POS;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strSELFZONE03[i];

		SFile << (DWORD)emTARGZONE01;
		SFile << (DWORD)emTARGZONE01_POS;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strTARGZONE01[i];
			
		SFile << (DWORD)emTARGZONE02;
		SFile << (DWORD)emTARGZONE02_POS;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strTARGZONE02[i];

		SFile << (DWORD)emTARGZONE03;
		SFile << (DWORD)emTARGZONE03_POS;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strTARGZONE03[i];

		SFile << (DWORD)emSELFBODY;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strSELFBODY[i];

		SFile << (DWORD)emTARGBODY01;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strTARGBODY01[i];

		SFile << (DWORD)emTARGBODY02;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << strTARGBODY02[i];

		SFile << strHOLDOUT;

		SFile << sICONINDEX.dwID;
		SFile << strICONFILE;
		
		SFile << strCOMMENTS;
		
		SFile << sADDSKILL.dwID; 
		SFile << dwADDSKILLLEVEL; 

		return TRUE;
	}

	BOOL SEXT_DATA_103::LOAD ( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA;	emANIMTYPE = EMANI_MAINTYPE(dwDATA);
		SFile >> dwDATA;	emANISTYPE = EMANI_SUBTYPE(dwDATA);

		SFile >> dwDATA;	emTARG = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARG_POSA = EMEFFECTPOS(dwDATA);
		SFile >> dwDATA;	emTARG_POSB = EMEFFECTPOS(dwDATA);

		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARG[i];
		
		SFile >> dwDATA;	emSELFZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFZONE01[i];

		SFile >> dwDATA;	emSELFZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFZONE02[i];

		SFile >> dwDATA;	emTARGZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGZONE01[i];
			
		SFile >> dwDATA;	emTARGZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGZONE02[i];

		SFile >> dwDATA;	emSELFBODY = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFBODY[i];

		SFile >> dwDATA;	emTARGBODY01 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGBODY01[i];

		SFile >> dwDATA;	emTARGBODY02 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGBODY02[i];

		SFile >> strHOLDOUT;

		SFile >> sICONINDEX.dwID;
		SFile >> strICONFILE;
		
		SFile >> strCOMMENTS;
	
		return TRUE;
	}

	BOOL SEXT_DATA::LOAD_GS_0110( basestream &SFile )
	{
		DWORD dwDATA,dw1;
		SFile >> dwDATA;	emANIMTYPE = EMANI_MAINTYPE(dwDATA);
		SFile >> dwDATA;	emANISTYPE = EMANI_SUBTYPE(dwDATA);

		SFile >> dw1;

		SFile >> sICONINDEX.dwID;
		SFile >> strICONFILE;

		SFile >> strCOMMENTS;

		SFile >> dwDATA;	emTARG = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARG_POSA = EMEFFECTPOS(dwDATA);
		SFile >> dwDATA;	emTARG_POSB = EMEFFECTPOS(dwDATA);

		SFile >> fDELAY4DAMAGE;

		SFile >> strHOLDOUT;

		SFile >> dwDATA;	emSELFZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFZONE01[i];

		SFile >> dwDATA;	emSELFZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFZONE02[i];

		SFile >> dwDATA;	emSELFZONE03 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE03_POS = EMEFFECTPOS(dwDATA);

		std::string strSELFZONE03[EMELEMENT_MAXNUM];
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFZONE03[i];

		SFile >> bTARG_ONE;
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARG[i];

		SFile >> dwDATA;	emTARGZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGZONE01[i];

		SFile >> dwDATA;	emTARGZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGZONE02[i];

		std::string strTARGZONE03[EMELEMENT_MAXNUM];
		SFile >> dwDATA;	emTARGZONE03 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE03_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGZONE03[i];

		SFile >> dwDATA;	emTARGBODY01 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGBODY01[i];

		SFile >> dwDATA;	emTARGBODY02 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGBODY02[i];

		SFile >> dwDATA;	emSELFBODY = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFBODY[i];

		SFile >> sADDSKILL.dwID;//dw1;	sADDSKILL.dwID;
		SFile >> dwADDSKILLLEVEL;// dw1;	dwADDSKILLLEVEL;

		return TRUE;
	}

	BOOL SEXT_DATA_104::LOAD ( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA;	emANIMTYPE = EMANI_MAINTYPE(dwDATA);
		SFile >> dwDATA;	emANISTYPE = EMANI_SUBTYPE(dwDATA);

		SFile >> fDELAY4DAMAGE;
		SFile >> dwDATA;	emTARG = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARG_POSA = EMEFFECTPOS(dwDATA);
		SFile >> dwDATA;	emTARG_POSB = EMEFFECTPOS(dwDATA);

		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARG[i];
		
		SFile >> dwDATA;	emSELFZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFZONE01[i];

		SFile >> dwDATA;	emSELFZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFZONE02[i];

		SFile >> dwDATA;	emTARGZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGZONE01[i];
			
		SFile >> dwDATA;	emTARGZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGZONE02[i];

		SFile >> dwDATA;	emSELFBODY = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFBODY[i];

		SFile >> dwDATA;	emTARGBODY01 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGBODY01[i];

		SFile >> dwDATA;	emTARGBODY02 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGBODY02[i];

		SFile >> strHOLDOUT;

		SFile >> sICONINDEX.dwID;
		SFile >> strICONFILE;
		
		SFile >> strCOMMENTS;
	
		return TRUE;
	}

	BOOL SEXT_DATA_105::LOAD ( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA;	emANIMTYPE = EMANI_MAINTYPE(dwDATA);
		SFile >> dwDATA;	emANISTYPE = EMANI_SUBTYPE(dwDATA);

		SFile >> fDELAY4DAMAGE;
		SFile >> dwDATA;	emTARG = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARG_POSA = EMEFFECTPOS(dwDATA);
		SFile >> dwDATA;	emTARG_POSB = EMEFFECTPOS(dwDATA);
		SFile >> bTARG_ONE;

		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARG[i];
		
		SFile >> dwDATA;	emSELFZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFZONE01[i];

		SFile >> dwDATA;	emSELFZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFZONE02[i];

		SFile >> dwDATA;	emTARGZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGZONE01[i];
			
		SFile >> dwDATA;	emTARGZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGZONE02[i];

		SFile >> dwDATA;	emSELFBODY = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strSELFBODY[i];

		SFile >> dwDATA;	emTARGBODY01 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGBODY01[i];

		SFile >> dwDATA;	emTARGBODY02 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_OLDMAX; ++i )
			SFile >> strTARGBODY02[i];

		SFile >> strHOLDOUT;

		SFile >> sICONINDEX.dwID;
		SFile >> strICONFILE;
		
		SFile >> strCOMMENTS;
	
		return TRUE;
	}

	BOOL SEXT_DATA_106::LOAD ( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA;	emANIMTYPE = EMANI_MAINTYPE(dwDATA);
		SFile >> dwDATA;	emANISTYPE = EMANI_SUBTYPE(dwDATA);

		SFile >> fDELAY4DAMAGE;
		SFile >> dwDATA;	emTARG = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARG_POSA = EMEFFECTPOS(dwDATA);
		SFile >> dwDATA;	emTARG_POSB = EMEFFECTPOS(dwDATA);
		SFile >> bTARG_ONE;

		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARG[i];
		
		SFile >> dwDATA;	emSELFZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFZONE01[i];

		SFile >> dwDATA;	emSELFZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFZONE02[i];

		SFile >> dwDATA;	emTARGZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGZONE01[i];
			
		SFile >> dwDATA;	emTARGZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGZONE02[i];

		SFile >> dwDATA;	emSELFBODY = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFBODY[i];

		SFile >> dwDATA;	emTARGBODY01 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGBODY01[i];

		SFile >> dwDATA;	emTARGBODY02 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGBODY02[i];

		SFile >> strHOLDOUT;

		SFile >> sICONINDEX.dwID;
		SFile >> strICONFILE;
		
		SFile >> strCOMMENTS;
	
		return TRUE;
	}

	BOOL SEXT_DATA::LOAD ( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA;	emANIMTYPE = EMANI_MAINTYPE(dwDATA);
		SFile >> dwDATA;	emANISTYPE = EMANI_SUBTYPE(dwDATA);

		SFile >> fDELAY4DAMAGE;
		SFile >> dwDATA;	emTARG = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARG_POSA = EMEFFECTPOS(dwDATA);
		SFile >> dwDATA;	emTARG_POSB = EMEFFECTPOS(dwDATA);
		SFile >> bTARG_ONE;

		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARG[i];
		
		SFile >> dwDATA;	emSELFZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFZONE01[i];

		SFile >> dwDATA;	emSELFZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFZONE02[i];

		SFile >> dwDATA;	emSELFZONE03 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emSELFZONE03_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFZONE03[i];

		SFile >> dwDATA;	emTARGZONE01 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE01_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGZONE01[i];
			
		SFile >> dwDATA;	emTARGZONE02 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE02_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGZONE02[i];
	
		SFile >> dwDATA;	emTARGZONE03 = EMEFFECTIME(dwDATA);
		SFile >> dwDATA;	emTARGZONE03_POS = EMEFFECTPOS(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGZONE03[i];

		SFile >> dwDATA;	emSELFBODY = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strSELFBODY[i];

		SFile >> dwDATA;	emTARGBODY01 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGBODY01[i];

		SFile >> dwDATA;	emTARGBODY02 = EMEFFECTIME(dwDATA);
		for ( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile >> strTARGBODY02[i];

		SFile >> strHOLDOUT;

		SFile >> sICONINDEX.dwID;
		SFile >> strICONFILE;
		
		SFile >> strCOMMENTS;

		SFile >> sADDSKILL.dwID;//dw1;	sADDSKILL.dwID;
		SFile >> dwADDSKILLLEVEL;// dw1;	dwADDSKILLLEVEL;
	
		return TRUE;
	}

	SEXT_DATA& SEXT_DATA::operator= ( const SEXT_DATA &value )
	{
		int i=0;

		emANIMTYPE = value.emANIMTYPE;
		emANISTYPE = value.emANISTYPE;

		fDELAY4DAMAGE = value.fDELAY4DAMAGE;
		emTARG = value.emTARG;
		emTARG_POSA = value.emTARG_POSA;
		emTARG_POSB = value.emTARG_POSB;
		bTARG_ONE = value.bTARG_ONE;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARG[i] = value.strTARG[i];
		
		emSELFZONE01 = value.emSELFZONE01;
		emSELFZONE01_POS = value.emSELFZONE01_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strSELFZONE01[i] = value.strSELFZONE01[i];

		emSELFZONE02 = value.emSELFZONE02 ;
		emSELFZONE02_POS = value.emSELFZONE02_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strSELFZONE02[i] = value.strSELFZONE02[i];

		emSELFZONE03 = value.emSELFZONE03 ;
		emSELFZONE03_POS = value.emSELFZONE03_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strSELFZONE03[i] = value.strSELFZONE03[i];

		emTARGZONE01 = value.emTARGZONE01;
		emTARGZONE01_POS = value.emTARGZONE01_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGZONE01[i] = value.strTARGZONE01[i];
			
		emTARGZONE02 = value.emTARGZONE02;
		emTARGZONE02_POS = value.emTARGZONE02_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGZONE02[i] = value.strTARGZONE02[i];
	
		emTARGZONE03 = value.emTARGZONE03;
		emTARGZONE03_POS = value.emTARGZONE03_POS;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGZONE03[i] = value.strTARGZONE03[i];

		emSELFBODY = value.emSELFBODY;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strSELFBODY[i] = value.strSELFBODY[i];

		emTARGBODY01 = value.emTARGBODY01;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGBODY01[i] = value.strTARGBODY01[i];

		emTARGBODY02 = value.emTARGBODY02;
		for ( i=0; i<EMELEMENT_MAXNUM; ++i )		strTARGBODY02[i] = value.strTARGBODY02[i];

		strHOLDOUT = value.strHOLDOUT;

		sICONINDEX = value.sICONINDEX;
		strICONFILE = value.strICONFILE;
		
		strCOMMENTS = value.strCOMMENTS;

		sADDSKILL = value.sADDSKILL;
		dwADDSKILLLEVEL = value.dwADDSKILLLEVEL;
		return *this;
	}

	VOID SLEARN::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "emBRIGHT" << ",";
		SFile << "dwCLASS" << ",";
		SFile << "sSKILL wMainID" << ",";
		SFile << "sSKILL wSubID" << ",";
		SFile << "sWEAPON wMainID" << ",";
		SFile << "sWEAPON wSubID" << ",";


		for( int i=1; i<10; ++i )
		{
			SFile << "sLVL_STEP " << i << " dwSKP" << ",";
			SFile << "sLVL_STEP " << i << " dwLEVEL" << ",";

			SFile << "sLVL_STEP " << i << " sSTATS wPow" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wStr" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wSpi" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wDex" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wInt" << ",";
			SFile << "sLVL_STEP " << i << " sSTATS wSta" << ",";


			SFile << "sLVL_STEP " << i << " dwSKILL_LVL" << ",";
			SFile << "sLVL_STEP " << i << " dwREBORN" << ",";

			SFile << "sLVL_STEP " << i << " dwMoneyReq" << ",";
			SFile << "sLVL_STEP " << i << " dwPremPoint" << ",";
			SFile << "sLVL_STEP " << i << " dwVotePoint" << ",";

		}
	}

	VOID SLEARN::SaveCsv ( std::fstream &SFile )
	{
		SFile << emBRIGHT << ",";
		SFile << dwCLASS << ",";
		SFile << sSKILL.wMainID << ",";
		SFile << sSKILL.wSubID << ",";
		SFile << sWEAPON.wMainID << ",";
		SFile << sWEAPON.wSubID << ",";

		for( int i=0; i<9; ++i )
		{
			SFile << sLVL_STEP[i].dwSKP << ",";
			SFile << sLVL_STEP[i].dwLEVEL << ",";

			SFile << sLVL_STEP[i].sSTATS.dwPow << ",";
			SFile << sLVL_STEP[i].sSTATS.dwStr << ",";
			SFile << sLVL_STEP[i].sSTATS.dwSpi << ",";
			SFile << sLVL_STEP[i].sSTATS.dwDex << ",";
			SFile << sLVL_STEP[i].sSTATS.dwInt << ",";
			SFile << sLVL_STEP[i].sSTATS.dwSta << ",";

			SFile << sLVL_STEP[i].dwSKILL_LVL << ",";
			SFile << sLVL_STEP[i].dwREBORN << ",";

			SFile << sLVL_STEP[i].dwMoneyReq << ",";
			SFile << sLVL_STEP[i].dwPremPoint << ",";
			SFile << sLVL_STEP[i].dwVotePoint << ",";
		}
	}

	

	VOID SLEARN::LoadCsv ( CStringArray &StrArray )
	{
		emBRIGHT = (EMBRIGHT)atoi( StrArray[ iCsvCur++ ] );
		dwCLASS = (DWORD)atol( StrArray[ iCsvCur++ ] );
		sSKILL.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sSKILL.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sWEAPON.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sWEAPON.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		for( int i=0; i<9; ++i )
		{
			sLVL_STEP[i].dwSKP = (DWORD)atol( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].dwLEVEL = (DWORD)atol( StrArray[ iCsvCur++ ] );

			sLVL_STEP[i].sSTATS.dwPow = (DWORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.dwStr = (DWORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.dwSpi = (DWORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.dwDex = (DWORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.dwInt = (DWORD)atoi( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].sSTATS.dwSta = (DWORD)atoi( StrArray[ iCsvCur++ ] );

			sLVL_STEP[i].dwSKILL_LVL = (DWORD)atol( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].dwREBORN = (DWORD)atol( StrArray[ iCsvCur++ ] );

			sLVL_STEP[i].dwMoneyReq = (DWORD)atol( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].dwPremPoint = (DWORD)atol( StrArray[ iCsvCur++ ] );
			sLVL_STEP[i].dwVotePoint = (DWORD)atol( StrArray[ iCsvCur++ ] );
		}
	}

	

	VOID SEXT_DATA::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "emANIMTYPE" << ",";
		SFile << "emANISTYPE" << ",";

		SFile << "fDELAY4DAMAGE" << ",";
		SFile << "emTARG" << ",";
		SFile << "emTARG_POSA" << ",";
		SFile << "emTARG_POSB" << ",";
		SFile << "bTARG_ONE" << ",";
		for( int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << "strTARG " << i+1 << ",";

		SFile << "emSELFZONE01" << ",";
		SFile << "emSELFZONE01_POS" << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << "strSELFZONE01 " << i+1 << ",";

		SFile << "emSELFZONE02" << ",";
		SFile << "emSELFZONE02_POS" << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << "strSELFZONE02 " << i+1 << ",";

		SFile << "emTARGZONE01" << ",";
		SFile << "emTARGZONE01_POS" << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << "strTARGZONE01 " << i+1 << ",";

		SFile << "emTARGZONE02" << ",";
		SFile << "emTARGZONE02_POS" << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << "strTARGZONE02 " << i+1 << ",";

		SFile << "emSELFBODY" << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << "strSELFBODY " << i+1 << ",";

		SFile << "emTARGBODY01" << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << "strTARGBODY01 " << i+1 << ",";

		SFile << "emTARGBODY02" << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			SFile << "strTARGBODY02 " << i+1 << ",";

		SFile << "strHOLDOUT" << ",";

		SFile << "sICONINDEX wMainID" << ",";
		SFile << "sICONINDEX wSubID" << ",";
		SFile << "strICONFILE" << ",";

		SFile << "strCOMMENTS" << ",";
	}

	VOID SEXT_DATA::SaveCsv ( std::fstream &SFile )
	{
		SFile << emANIMTYPE << ",";
		SFile << emANISTYPE << ",";

		SFile << fDELAY4DAMAGE << ",";
		SFile << emTARG << ",";
		SFile << emTARG_POSA << ",";
		SFile << emTARG_POSB << ",";
		SFile << bTARG_ONE << ",";
		for( int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::OutputStrCsv( SFile, strTARG[i] );

		SFile << emSELFZONE01 << ",";
		SFile << emSELFZONE01_POS << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::OutputStrCsv( SFile, strSELFZONE01[i] );

		SFile << emSELFZONE02 << ",";
		SFile << emSELFZONE02_POS << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::OutputStrCsv( SFile, strSELFZONE02[i] );

		SFile << emTARGZONE01 << ",";
		SFile << emTARGZONE01_POS << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::OutputStrCsv( SFile, strTARGZONE01[i] );

		SFile << emTARGZONE02 << ",";
		SFile << emTARGZONE02_POS << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::OutputStrCsv( SFile, strTARGZONE02[i] );

		SFile << emSELFBODY << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::OutputStrCsv( SFile, strSELFBODY[i] );

		SFile << emTARGBODY01 << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::OutputStrCsv( SFile, strTARGBODY01[i] );

		SFile << emTARGBODY02 << ",";
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::OutputStrCsv( SFile, strTARGBODY02[i] );

		STRUTIL::OutputStrCsv( SFile, strHOLDOUT );

		SFile << sICONINDEX.wMainID << ",";
		SFile << sICONINDEX.wSubID << ",";
		STRUTIL::OutputStrCsv( SFile, strICONFILE );

		STRUTIL::OutputStrCsv( SFile, strCOMMENTS );
	}


	
	VOID SEXT_DATA::LoadCsv ( CStringArray &StrArray )
	{
		emANIMTYPE = (EMANI_MAINTYPE)atoi( StrArray[ iCsvCur++ ] );
		emANISTYPE = (EMANI_SUBTYPE)atoi( StrArray[ iCsvCur++ ] );

		fDELAY4DAMAGE = (float)atof( StrArray[ iCsvCur++ ] );
		emTARG = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		emTARG_POSA = (EMEFFECTPOS)atoi( StrArray[ iCsvCur++ ] );
		emTARG_POSB = (EMEFFECTPOS)atoi( StrArray[ iCsvCur++ ] );
		bTARG_ONE = (bool)( atoi( StrArray[ iCsvCur++ ] ) != 0 );
		for( int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTARG[i] );

		emSELFZONE01 = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		emSELFZONE01_POS = (EMEFFECTPOS)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strSELFZONE01[i] );

		emSELFZONE02 = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		emSELFZONE02_POS = (EMEFFECTPOS)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strSELFZONE02[i] );

		emSELFZONE03 = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		emSELFZONE03_POS = (EMEFFECTPOS)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strSELFZONE03[i] );

		emTARGZONE01 = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		emTARGZONE01_POS = (EMEFFECTPOS)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTARGZONE01[i] );

		emTARGZONE02 = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		emTARGZONE02_POS = (EMEFFECTPOS)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTARGZONE02[i] );

		emTARGZONE03 = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		emTARGZONE03_POS = (EMEFFECTPOS)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTARGZONE03[i] );


		emSELFBODY = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strSELFBODY[i] );

		emTARGBODY01 = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTARGBODY01[i] );

		emTARGBODY02 = (EMEFFECTIME)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<EMELEMENT_MAXNUM; ++i )
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTARGBODY02[i] );

		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strHOLDOUT );

		sICONINDEX.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sICONINDEX.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strICONFILE );

		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strCOMMENTS );
		
		sADDSKILL.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sADDSKILL.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		dwADDSKILLLEVEL = (DWORD)atoi( StrArray[ iCsvCur++ ] );
	}


	void CDATA_LVL::Assign ( CDATA_LVL_100 &sOldData )
	{
		fDELAYTIME		= sOldData.fDELAYTIME;

		fLIFE			= sOldData.fLIFE;
		wAPPLYRANGE		= sOldData.wAPPLYRANGE;
		wAPPLYNUM		= sOldData.wAPPLYNUM;
		wAPPLYANGLE		= sOldData.wAPPLYANGLE;
		wPIERCENUM		= sOldData.wPIERCENUM;
		wTARNUM			= 1;

		fBASIC_VAR		= sOldData.fBASIC_VAR;

		wUSE_ARROWNUM	= sOldData.wUSE_ARROWNUM;
		wUSE_CHARMNUM	= 0;

		wUSE_EXP		= 0;
		wUSE_HP			= sOldData.wUSE_HP;
		wUSE_MP			= sOldData.wUSE_MP;
		wUSE_SP			= sOldData.wUSE_SP;

		wUSE_HP_PTY		= sOldData.wUSE_HP_PTY;
		wUSE_MP_PTY		= sOldData.wUSE_MP_PTY;
		wUSE_SP_PTY		= sOldData.wUSE_SP_PTY;

		wUSE_CP			= 0;
		wUSE_BULLET		= 0;
		wUSE_EXTRA		= 0;

	}

	void CDATA_LVL::Assign ( CDATA_LVL_101 &sOldData )
	{
		fDELAYTIME		= sOldData.fDELAYTIME;

		fLIFE			= sOldData.fLIFE;
		wAPPLYRANGE		= sOldData.wAPPLYRANGE;
		wAPPLYNUM		= sOldData.wAPPLYNUM;
		wAPPLYANGLE		= sOldData.wAPPLYANGLE;
		wPIERCENUM		= sOldData.wPIERCENUM;
		wTARNUM			= sOldData.wTARNUM;

		fBASIC_VAR		= sOldData.fBASIC_VAR;

		wUSE_ARROWNUM	= sOldData.wUSE_ARROWNUM;
		wUSE_CHARMNUM	= sOldData.wUSE_CHARMNUM;

		wUSE_EXP		= sOldData.wUSE_EXP;
		wUSE_HP			= sOldData.wUSE_HP;
		wUSE_MP			= sOldData.wUSE_MP;
		wUSE_SP			= sOldData.wUSE_SP;

		wUSE_HP_PTY		= sOldData.wUSE_HP_PTY;
		wUSE_MP_PTY		= sOldData.wUSE_MP_PTY;
		wUSE_SP_PTY		= sOldData.wUSE_SP_PTY;

		wUSE_CP			= 0;
		wUSE_BULLET		= 0;
		wUSE_EXTRA		= 0;
	}

	void CDATA_LVL::LOAD_GS_110 ( basestream &SFile )
	{
		WORD w(0);
		SFile >> fDELAYTIME;
		SFile >> fLIFE;
		SFile >> wAPPLYRANGE;
		SFile >> wAPPLYNUM;
		SFile >> wAPPLYANGLE;
		SFile >> wPIERCENUM;
		SFile >> wTARNUM;

		SFile >> w;

		SFile >> fBASIC_VAR;
		SFile >> wUSE_ARROWNUM;
		SFile >> wUSE_CHARMNUM;
		SFile >> wUSE_BULLET;

		SFile >> wUSE_EXP;
		SFile >> wUSE_HP;
		SFile >> wUSE_MP;
		SFile >> wUSE_SP;
		SFile >> wUSE_CP;

		SFile >> wUSE_HP_PTY;
		SFile >> wUSE_MP_PTY;
		SFile >> wUSE_SP_PTY;
		SFile >> w;
		SFile >> w;
		SFile >> w;
	}

	void SAPPLY::Assign ( SAPPLY_100 &sOldData )
	{
		//	?? ??? ??.
		emBASIC_TYPE			= sOldData.emBASIC_TYPE;

		if ( sOldData.emELEMENT == EMELEMENT_OLDMAX ) emELEMENT = EMELEMENT_ARM;
		else emELEMENT				= sOldData.emELEMENT;
		dwCUREFLAG				= sOldData.dwCUREFLAG;

		for ( int i=0; i<MAX_LEVEL; ++i )	sDATA_LVL[i].Assign ( sOldData.sDATA_LVL[i] );

		//	?? ?? ??.
		emADDON					= sOldData.emADDON;
		memcpy ( fADDON_VAR, sOldData.fADDON_VAR, sizeof(float)*MAX_LEVEL );

		//	???? ??.
		emSTATE_BLOW			= sOldData.emSTATE_BLOW;
		memcpy ( sSTATE_BLOW, sOldData.sSTATE_BLOW, sizeof(SKILL::SSTATE_BLOW)*MAX_LEVEL );

		//	???? ??.
		emSPEC					= sOldData.emSPEC;
		for ( int i = 0; i < MAX_LEVEL; ++i )
		{
			sSPEC[i].Assign( sOldData.sSPEC[i] );
		}
	}

	void SAPPLY::Assign ( SAPPLY_101 &sOldData )
	{
		//	?? ??? ??.
		emBASIC_TYPE			= sOldData.emBASIC_TYPE;
		if ( sOldData.emELEMENT == EMELEMENT_OLDMAX ) emELEMENT = EMELEMENT_ARM;
		else emELEMENT				= sOldData.emELEMENT;
		dwCUREFLAG				= sOldData.dwCUREFLAG;

		//for ( int i=0; i<MAX_LEVEL; ++i )	sDATA_LVL[i] = sOldData.sDATA_LVL[i];
		for ( int i=0; i<MAX_LEVEL; ++i )	sDATA_LVL[i].Assign ( sOldData.sDATA_LVL[i] );

		//	?? ?? ??.
		emADDON					= sOldData.emADDON;
		memcpy ( fADDON_VAR, sOldData.fADDON_VAR, sizeof(float)*MAX_LEVEL );

		//	???? ??.
		emSTATE_BLOW			= sOldData.emSTATE_BLOW;
		memcpy ( sSTATE_BLOW, sOldData.sSTATE_BLOW, sizeof(SKILL::SSTATE_BLOW)*MAX_LEVEL );

		//	???? ??.
		emSPEC					= sOldData.emSPEC;
		for ( int i = 0; i < MAX_LEVEL; ++i )
		{
			sSPEC[i].Assign( sOldData.sSPEC[i] );
		}
	}

	void SAPPLY::Assign ( SAPPLY_102 &sOldData )
	{
		//	?? ??? ??.
		emBASIC_TYPE			= sOldData.emBASIC_TYPE;
		if ( sOldData.emELEMENT == EMELEMENT_OLDMAX ) emELEMENT = EMELEMENT_ARM;
		else emELEMENT				= sOldData.emELEMENT;
		dwCUREFLAG				= sOldData.dwCUREFLAG;

		//for ( int i=0; i<MAX_LEVEL; ++i )	sDATA_LVL[i] = sOldData.sDATA_LVL[i];
		for ( int i=0; i<MAX_LEVEL; ++i )	sDATA_LVL[i].Assign ( sOldData.sDATA_LVL[i] );

		//	?? ?? ??.
		emADDON					= sOldData.emADDON;
		memcpy ( fADDON_VAR, sOldData.fADDON_VAR, sizeof(float)*MAX_LEVEL );

		//	???? ??.
		emSTATE_BLOW			= sOldData.emSTATE_BLOW;
		memcpy ( sSTATE_BLOW, sOldData.sSTATE_BLOW, sizeof(SKILL::SSTATE_BLOW)*MAX_LEVEL );

		//	???? ??.
		emSPEC					= sOldData.emSPEC;
		for ( int i = 0; i < MAX_LEVEL; ++i )
		{
			sSPEC[i].Assign( sOldData.sSPEC[i] );
		}
	}

	void SAPPLY::Assign ( SAPPLY_103 &sOldData )
	{
		//	?? ??? ??.
		emBASIC_TYPE			= sOldData.emBASIC_TYPE;
		if ( sOldData.emELEMENT == EMELEMENT_OLDMAX ) emELEMENT = EMELEMENT_ARM;
		else emELEMENT				= sOldData.emELEMENT;
		dwCUREFLAG				= sOldData.dwCUREFLAG;

		//for ( int i=0; i<MAX_LEVEL; ++i )	sDATA_LVL[i] = sOldData.sDATA_LVL[i];
		for ( int i=0; i<MAX_LEVEL; ++i )	sDATA_LVL[i].Assign ( sOldData.sDATA_LVL[i] );

		//	?? ?? ??.
		emADDON					= sOldData.emADDON;
		memcpy ( fADDON_VAR, sOldData.fADDON_VAR, sizeof(float)*MAX_LEVEL );

		//	???? ??.
		emSTATE_BLOW			= sOldData.emSTATE_BLOW;
		memcpy ( sSTATE_BLOW, sOldData.sSTATE_BLOW, sizeof(SKILL::SSTATE_BLOW)*MAX_LEVEL );

		//	???? ??.
		emSPEC					= sOldData.emSPEC;
		for ( int i = 0; i < MAX_LEVEL; ++i )
		{
			sSPEC[i] = sOldData.sSPEC[i] ;
		}
	}


	BOOL SAPPLY::LOAD_GS_110 ( basestream &SFile )
	{

		DWORD dwData(0);

		SFile >> dwData;	emBASIC_TYPE = SKILL::EMTYPES(dwData);
		SFile >> dwData;	emELEMENT = EMELEMENT(dwData);

		SFile >> dwCUREFLAG;

		for ( int i=0; i<MAX_LEVEL; ++i )
		{
			sDATA_LVL[i].LOAD_GS_110( SFile );
		}

		SFile >> dwData;	emSTATE_BLOW = EMSTATE_BLOW(dwData);

		for ( int i=0; i<MAX_LEVEL; ++i )
		{
			SKILL::SSTATE_BLOW sBlow;
			SFile.ReadBuffer ( &sBlow, sizeof(SKILL::SSTATE_BLOW) );

			sSTATE_BLOW[i].fRATE = sBlow.fRATE;
			sSTATE_BLOW[i].fVAR1 = sBlow.fVAR1;
			sSTATE_BLOW[i].fVAR2 = sBlow.fVAR2;
		}

		

		for ( DWORD i=0; i<10; ++ i )
		{
			sADDONS[i].emADDON = EMIMPACTA_NONE;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				sADDONS[i].fADDON_VAR[lvl] = 0.0f;
			}
		}	

		DWORD dwNumber = 0;
		SFile >> dwNumber;

		for ( DWORD i=0; i<dwNumber; ++ i )
		{
			if ( i >= dwNumber )	continue;

			DWORD dwDATA = 0;
			float fData = 0;

			SFile >> dwDATA;

			sADDONS[i].emADDON = (EMIMPACT_ADDON)dwDATA;
			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> sADDONS[i].fADDON_VAR[lvl];
			}

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> fData;
			}
		}

		emADDON = sADDONS[0].emADDON;
		for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
		{
			fADDON_VAR[lvl] = sADDONS[0].fADDON_VAR[lvl];
		}

		
		
		for ( DWORD i=0; i<10; ++ i )
		{
			sSPECS[i].emSPEC = EMSPECA_NULL;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				sSPECS[i].sSPEC[lvl].fVAR1 = 0.0f;
				sSPECS[i].sSPEC[lvl].fVAR2 = 0.0f;
				sSPECS[i].sSPEC[lvl].dwFLAG = 0;
				sSPECS[i].sSPEC[lvl].dwNativeID = NATIVEID_NULL();
			}
		}

		//sspec read
		dwNumber = 0;
		SFile >> dwNumber;

		for ( DWORD i=0; i<dwNumber; ++ i )
		{
			if ( i >= dwNumber )	continue;

			DWORD dwDATA = 0;
			SFile >> dwDATA;

			float f; //dummy again

			sSPECS[i].emSPEC = (EMSPEC_ADDON)dwDATA;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> sSPECS[i].sSPEC[lvl].fVAR1;
				SFile >> sSPECS[i].sSPEC[lvl].fVAR2;
				SFile >> f;//this might be var3~4?
				SFile >> f;//none variable ignored, dont know proper usage
				SFile >> sSPECS[i].sSPEC[lvl].dwFLAG;
				SFile >> sSPECS[i].sSPEC[lvl].dwNativeID.dwID;	
			}
		}

		emSPEC = sSPECS[0].emSPEC;
		for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
		{
			sSPEC[lvl].dwFLAG = sSPECS[0].sSPEC[lvl].dwFLAG;
			sSPEC[lvl].dwNativeID = sSPECS[0].sSPEC[lvl].dwNativeID;
			sSPEC[lvl].fVAR1 = sSPECS[0].sSPEC[lvl].fVAR1;
			sSPEC[lvl].fVAR2 = sSPECS[0].sSPEC[lvl].fVAR2;
		}

		SFile >> dwNumber;

		return TRUE;
	}

	BOOL SAPPLY::LOAD_GS_111 ( basestream &SFile )
	{

		DWORD dwData(0);
		SFile >> dwData;	emBASIC_TYPE = SKILL::EMTYPES(dwData);
		SFile >> dwData;	emELEMENT = EMELEMENT(dwData);

		SFile >> dwCUREFLAG;

		for ( int i=0; i<MAX_LEVEL; ++i )
		{
			sDATA_LVL[i].LOAD_GS_110( SFile );
		}

		SFile >> dwData;	emSTATE_BLOW = EMSTATE_BLOW(dwData);

		for ( int i=0; i<MAX_LEVEL; ++i )
		{
			SKILL::SSTATE_BLOW sBlow;
			SFile.ReadBuffer ( &sBlow, sizeof(SKILL::SSTATE_BLOW) );

			sSTATE_BLOW[i].fRATE = sBlow.fRATE;
			sSTATE_BLOW[i].fVAR1 = sBlow.fVAR1;
			sSTATE_BLOW[i].fVAR2 = sBlow.fVAR2;
		}

		

		for ( DWORD i=0; i<10; ++ i )
		{
			sADDONS[i].emADDON = EMIMPACTA_NONE;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				sADDONS[i].fADDON_VAR[lvl] = 0.0f;
			}
		}	

		DWORD dwNumber = 0;
		SFile >> dwNumber;

		for ( DWORD i=0; i<dwNumber; ++ i )
		{
			if ( i >= dwNumber )	continue;

			DWORD dwDATA = 0;
			float fData = 0;

			SFile >> dwDATA;

			sADDONS[i].emADDON = (EMIMPACT_ADDON)dwDATA;
			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> sADDONS[i].fADDON_VAR[lvl];
			}

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> fData;
			}
		}

		emADDON = sADDONS[0].emADDON;
		for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
		{
			fADDON_VAR[lvl] = sADDONS[0].fADDON_VAR[lvl];
		}

		
		
		for ( DWORD i=0; i<10; ++ i )
		{
			sSPECS[i].emSPEC = EMSPECA_NULL;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				sSPECS[i].sSPEC[lvl].fVAR1 = 0.0f;
				sSPECS[i].sSPEC[lvl].fVAR2 = 0.0f;
				sSPECS[i].sSPEC[lvl].dwFLAG = 0;
				sSPECS[i].sSPEC[lvl].dwNativeID = NATIVEID_NULL();
				sSPECS[i].sSPEC[lvl].dwNativeID2 = NATIVEID_NULL();
			}
		}

		//sspec read
		dwNumber = 0;
		SFile >> dwNumber;

		for ( DWORD i=0; i<dwNumber; ++ i )
		{
			if ( i >= dwNumber )	continue;

			DWORD dwDATA = 0;
			SFile >> dwDATA;

			float f; //dummy again

			sSPECS[i].emSPEC = (EMSPEC_ADDON)dwDATA;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> sSPECS[i].sSPEC[lvl].fVAR1;
				SFile >> sSPECS[i].sSPEC[lvl].fVAR2;
				SFile >> f;//this might be var3~4?
				SFile >> f;//none variable ignored, dont know proper usage
				SFile >> sSPECS[i].sSPEC[lvl].dwFLAG;
				SFile >> sSPECS[i].sSPEC[lvl].dwNativeID.dwID;	
				SFile >> sSPECS[i].sSPEC[lvl].dwNativeID2.dwID;	
			}
		}

		emSPEC = sSPECS[0].emSPEC;
		for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
		{
			sSPEC[lvl].dwFLAG = sSPECS[0].sSPEC[lvl].dwFLAG;
			sSPEC[lvl].dwNativeID = sSPECS[0].sSPEC[lvl].dwNativeID;
			sSPEC[lvl].dwNativeID2 = sSPECS[0].sSPEC[lvl].dwNativeID2;
			sSPEC[lvl].fVAR1 = sSPECS[0].sSPEC[lvl].fVAR1;
			sSPEC[lvl].fVAR2 = sSPECS[0].sSPEC[lvl].fVAR2;
		}

		SFile >> dwNumber;

		return TRUE;
	}

	BOOL SAPPLY::LOAD_GS_112 ( basestream &SFile )
	{

		DWORD dwData(0);
		SFile >> dwData;	emBASIC_TYPE = SKILL::EMTYPES(dwData);
		SFile >> dwData;	emELEMENT = EMELEMENT(dwData);

		SFile >> dwCUREFLAG;

		for ( int i=0; i<MAX_LEVEL; ++i )
		{
			sDATA_LVL[i].LOAD_GS_110( SFile );
		}

		SFile >> dwData;	emSTATE_BLOW = EMSTATE_BLOW(dwData);

		for ( int i=0; i<MAX_LEVEL; ++i )
		{
			SKILL::SSTATE_BLOW sBlow;
			SFile.ReadBuffer ( &sBlow, sizeof(SKILL::SSTATE_BLOW) );

			sSTATE_BLOW[i].fRATE = sBlow.fRATE;
			sSTATE_BLOW[i].fVAR1 = sBlow.fVAR1;
			sSTATE_BLOW[i].fVAR2 = sBlow.fVAR2;
		}

		

		for ( DWORD i=0; i<10; ++ i )
		{
			sADDONS[i].emADDON = EMIMPACTA_NONE;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				sADDONS[i].fADDON_VAR[lvl] = 0.0f;
			}
		}	

		DWORD dwNumber = 0;
		SFile >> dwNumber;

		for ( DWORD i=0; i<dwNumber; ++ i )
		{
			if ( i >= dwNumber )	continue;

			DWORD dwDATA = 0;
			float fData = 0;

			SFile >> dwDATA;

			sADDONS[i].emADDON = (EMIMPACT_ADDON)dwDATA;
			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> sADDONS[i].fADDON_VAR[lvl];
			}

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> fData;
			}
		}

		emADDON = sADDONS[0].emADDON;
		for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
		{
			fADDON_VAR[lvl] = sADDONS[0].fADDON_VAR[lvl];
		}

		
		
		for ( DWORD i=0; i<10; ++ i )
		{
			sSPECS[i].emSPEC = EMSPECA_NULL;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				sSPECS[i].sSPEC[lvl].fVAR1 = 0.0f;
				sSPECS[i].sSPEC[lvl].fVAR2 = 0.0f;
				sSPECS[i].sSPEC[lvl].dwFLAG = 0;
				sSPECS[i].sSPEC[lvl].dwNativeID = NATIVEID_NULL();
				sSPECS[i].sSPEC[lvl].dwNativeID2 = NATIVEID_NULL();
			}
		}

		//sspec read
		dwNumber = 0;
		SFile >> dwNumber;

		for ( DWORD i=0; i<dwNumber; ++ i )
		{
			if ( i >= dwNumber )	continue;

			DWORD dwDATA = 0;
			SFile >> dwDATA;

			float f; //dummy again

			sSPECS[i].emSPEC = (EMSPEC_ADDON)dwDATA;

			for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
			{
				SFile >> sSPECS[i].sSPEC[lvl].fVAR1;
				SFile >> sSPECS[i].sSPEC[lvl].fVAR2;
				SFile >> f;//this might be var3~4?
				SFile >> f;//none variable ignored, dont know proper usage
				SFile >> sSPECS[i].sSPEC[lvl].dwFLAG;
				SFile >> sSPECS[i].sSPEC[lvl].dwNativeID.dwID;	
				SFile >> sSPECS[i].sSPEC[lvl].dwNativeID2.dwID;	
			}
		}

		emSPEC = sSPECS[0].emSPEC;
		for ( DWORD lvl=0; lvl<MAX_LEVEL; ++ lvl )
		{
			sSPEC[lvl].dwFLAG = sSPECS[0].sSPEC[lvl].dwFLAG;
			sSPEC[lvl].dwNativeID = sSPECS[0].sSPEC[lvl].dwNativeID;
			sSPEC[lvl].dwNativeID2 = sSPECS[0].sSPEC[lvl].dwNativeID2;
			sSPEC[lvl].fVAR1 = sSPECS[0].sSPEC[lvl].fVAR1;
			sSPEC[lvl].fVAR2 = sSPECS[0].sSPEC[lvl].fVAR2;
		}

		SFile >> dwNumber;

		return TRUE;
	}

	VOID SAPPLY::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "emBASIC_TYPE" << ",";
		SFile << "emELEMENT" << ",";
		SFile << "dwCUREFLAG" << ",";
		for( int i=1; i<10; ++i )
		{
			SFile << "sDATA_LVL " << i << " fDELAYTIME" << ",";
			SFile << "sDATA_LVL " << i << " fLIFE" << ",";
			SFile << "sDATA_LVL " << i << " wAPPLYRANGE" << ",";
			SFile << "sDATA_LVL " << i << " wAPPLYNUM" << ",";
			SFile << "sDATA_LVL " << i << " wAPPLYANGLE" << ",";
			SFile << "sDATA_LVL " << i << " wPIERCENUM" << ",";
			SFile << "sDATA_LVL " << i << " wTARNUM" << ",";
			SFile << "sDATA_LVL " << i << " fBASIC_VAR" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_ARROWNUM" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_CHARMNUM" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_EXP" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_HP" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_MP" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_SP" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_HP_PTY" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_MP_PTY" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_SP_PTY" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_CP" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_BULLET" << ",";
			SFile << "sDATA_LVL " << i << " wUSE_EXTRA" << ",";
		}

		SFile << "emADDON" << ",";
		for(int i=1; i<10; ++i )
			SFile << "fADDON_VAR " << i << ",";

		SFile << "emSTATE_BLOW" << ",";
		for(int i=1; i<10; ++i )
		{
			SFile << "sSTATE_BLOW " << i << " fRATE" << ",";
			SFile << "sSTATE_BLOW " << i << " fVAR1" << ",";
			SFile << "sSTATE_BLOW " << i << " fVAR2" << ",";
		}

		SFile << "emSPEC" << ",";
		for(int i=1; i<10; ++i )
		{
			SFile << "sSPEC " << i << " fVAR1" << ",";
			SFile << "sSPEC " << i << " fVAR2" << ",";
			SFile << "sSPEC " << i << " dwFLAG" << ",";
			SFile << "sSPEC " << i << " Native MID"	<< ",";
			SFile << "sSPEC " << i << " Native SID"	<< ",";
		}

		for(int i = 0; i < SKILL::MAX_LEVEL; ++i )
		{
			SFile << "sSPECS " << i << " emSPEC" << ",";
			for(int j = 0; j < SKILL::MAX_LEVEL; ++j )
			{
				SFile << "sSPECS_" << i << "_" << j << " fVAR1"				<< ",";
				SFile << "sSPECS_" << i << "_" << j << " fVAR2"				<< ",";
				SFile << "sSPECS_" << i << "_" << j << " dwFLAG"			<< ",";
				SFile << "sSPECS_" << i << "_" << j << " dwNativeID MID"	<< ",";
				SFile << "sSPECS_" << i << "_" << j << " dwNativeID SID"	<< ",";
				SFile << "sSPECS_" << i << "_" << j << " dwNativeID2 MID"	<< ",";
				SFile << "sSPECS_" << i << "_" << j << " dwNativeID2 SID"	<< ",";
			}
		}
	}

	VOID SAPPLY::SaveCsv ( std::fstream &SFile )
	{
		SFile << emBASIC_TYPE << ",";
		SFile << emELEMENT << ",";
		SFile << dwCUREFLAG << ",";
		for( int i=0; i<9; ++i )
		{
			SFile << sDATA_LVL[i].fDELAYTIME << ",";
			SFile << sDATA_LVL[i].fLIFE << ",";
			SFile << sDATA_LVL[i].wAPPLYRANGE << ",";
			SFile << sDATA_LVL[i].wAPPLYNUM << ",";
			SFile << sDATA_LVL[i].wAPPLYANGLE << ",";
			SFile << sDATA_LVL[i].wPIERCENUM << ",";
			SFile << sDATA_LVL[i].wTARNUM << ",";
			SFile << sDATA_LVL[i].fBASIC_VAR << ",";
			SFile << sDATA_LVL[i].wUSE_ARROWNUM << ",";
			SFile << sDATA_LVL[i].wUSE_CHARMNUM << ",";
			SFile << sDATA_LVL[i].wUSE_EXP << ",";
			SFile << sDATA_LVL[i].wUSE_HP << ",";
			SFile << sDATA_LVL[i].wUSE_MP << ",";
			SFile << sDATA_LVL[i].wUSE_SP << ",";
			SFile << sDATA_LVL[i].wUSE_HP_PTY << ",";
			SFile << sDATA_LVL[i].wUSE_MP_PTY << ",";
			SFile << sDATA_LVL[i].wUSE_SP_PTY << ",";
			SFile << sDATA_LVL[i].wUSE_CP << ",";
			SFile << sDATA_LVL[i].wUSE_BULLET << ",";
			SFile << sDATA_LVL[i].wUSE_EXTRA << ",";
		}

		SFile << emADDON << ",";
		for(int i=0; i<9; ++i )
			SFile << fADDON_VAR[i] << ",";


		SFile << emSTATE_BLOW << ",";
		for(int i=0; i<9; ++i )
		{
			SFile << sSTATE_BLOW[i].fRATE << ",";
			SFile << sSTATE_BLOW[i].fVAR1 << ",";
			SFile << sSTATE_BLOW[i].fVAR2 << ",";
		}

		SFile << emSPEC << ",";
		for(int i=0; i<9; ++i )
		{
			SFile << sSPEC[i].fVAR1 << ",";
			SFile << sSPEC[i].fVAR2 << ",";
			SFile << sSPEC[i].dwFLAG << ",";
			SFile << sSPEC[i].dwNativeID.wMainID << ",";
			SFile << sSPEC[i].dwNativeID.wSubID << ",";
		}

		for(int i = 0; i < SKILL::MAX_LEVEL; ++i )
		{
			SFile << sSPECS[i].emSPEC << ",";
			for(int j = 0; j < SKILL::MAX_LEVEL; ++j )
			{
				SFile << sSPECS[i].sSPEC[j].fVAR1 << ",";
				SFile << sSPECS[i].sSPEC[j].fVAR2 << ",";
				SFile << sSPECS[i].sSPEC[j].dwFLAG << ",";
				SFile << sSPECS[i].sSPEC[j].dwNativeID.wMainID << ",";
				SFile << sSPECS[i].sSPEC[j].dwNativeID.wSubID << ",";
				SFile << sSPECS[i].sSPEC[j].dwNativeID2.wMainID << ",";
				SFile << sSPECS[i].sSPEC[j].dwNativeID2.wSubID << ",";
			}
		}
	}


	
	VOID SAPPLY::LoadCsv ( CStringArray &StrArray )
	{
		iCsvCur = 0;

		emBASIC_TYPE = (SKILL::EMTYPES)atoi( StrArray[ iCsvCur++ ] );
		emELEMENT = (EMELEMENT)atoi( StrArray[ iCsvCur++ ] );
		dwCUREFLAG = (DWORD)atol( StrArray[ iCsvCur++ ] );
		for( int i=0; i<9; ++i )
		{
			sDATA_LVL[i].fDELAYTIME = (float)atof( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].fLIFE = (float)atof( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wAPPLYRANGE = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wAPPLYNUM = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wAPPLYANGLE = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wPIERCENUM = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wTARNUM = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].fBASIC_VAR = (float)atof( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_ARROWNUM = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_CHARMNUM = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_EXP = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_HP = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_MP = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_SP = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_HP_PTY = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_MP_PTY = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_SP_PTY = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_CP = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_BULLET = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sDATA_LVL[i].wUSE_EXTRA = (WORD)atoi( StrArray[ iCsvCur++ ] );	
		}

		emADDON = (EMIMPACT_ADDON)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<9; ++i )
			fADDON_VAR[i] = (float)atof( StrArray[ iCsvCur++ ] );

		emSTATE_BLOW = (EMSTATE_BLOW)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<9; ++i )
		{
			sSTATE_BLOW[i].fRATE = (float)atof( StrArray[ iCsvCur++ ] );
			sSTATE_BLOW[i].fVAR1 = (float)atof( StrArray[ iCsvCur++ ] );
			sSTATE_BLOW[i].fVAR2 = (float)atof( StrArray[ iCsvCur++ ] );
		}

		emSPEC = (EMSPEC_ADDON)atoi( StrArray[ iCsvCur++ ] );
		for(int i=0; i<9; ++i )
		{
			sSPEC[i].fVAR1 = (float)atof( StrArray[ iCsvCur++ ] );
			sSPEC[i].fVAR2 = (float)atof( StrArray[ iCsvCur++ ] );
			sSPEC[i].dwFLAG = (DWORD)atol( StrArray[ iCsvCur++ ] );
			sSPEC[i].dwNativeID.wMainID = (WORD)atol( StrArray[ iCsvCur++ ] );
			sSPEC[i].dwNativeID.wSubID = (WORD)atol( StrArray[ iCsvCur++ ] );
		}

		for(int i = 0; i < SKILL::MAX_LEVEL; ++i )
		{
			sSPECS[i].emSPEC = (EMSPEC_ADDON)atoi(StrArray[ iCsvCur++] );
			for(int j = 0; j < SKILL::MAX_LEVEL; ++j )
			{
				sSPECS[i].sSPEC[j].fVAR1 = (float)atof( StrArray[ iCsvCur++ ] );
				sSPECS[i].sSPEC[j].fVAR2 = (float)atof( StrArray[ iCsvCur++ ] );
				sSPECS[i].sSPEC[j].dwFLAG = (DWORD)atol( StrArray[ iCsvCur++ ] );
				sSPECS[i].sSPEC[j].dwNativeID.wMainID = (WORD)atol( StrArray[ iCsvCur++ ] );
				sSPECS[i].sSPEC[j].dwNativeID.wSubID = (WORD)atol( StrArray[ iCsvCur++ ] );
				sSPECS[i].sSPEC[j].dwNativeID2.wMainID = (WORD)atol( StrArray[ iCsvCur++ ] );
				sSPECS[i].sSPEC[j].dwNativeID2.wSubID = (WORD)atol( StrArray[ iCsvCur++ ] );
			}
		}
	}

	
	void SBASIC::Assign ( SBASIC_100 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		
		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= ITEM::assign_att ( sOldData.emUSE_LITEM );
		emUSE_RITEM				= ITEM::assign_att ( sOldData.emUSE_RITEM );

	}

	void SBASIC::Assign ( SBASIC_101 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		
		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= ITEM::assign_att ( sOldData.emUSE_LITEM );
		emUSE_RITEM				= ITEM::assign_att ( sOldData.emUSE_RITEM );

	}

	void SBASIC::Assign ( SBASIC_102 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		
		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= ITEM::assign_att ( sOldData.emUSE_LITEM );
		emUSE_RITEM				= ITEM::assign_att ( sOldData.emUSE_LITEM );

	}

	void SBASIC::Assign ( SBASIC_103 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		
		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= sOldData.emUSE_LITEM;
		emUSE_RITEM				= sOldData.emUSE_RITEM;

	}
	void SBASIC::Assign ( SBASIC_105 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;

		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= sOldData.emUSE_LITEM;
		emUSE_RITEM				= sOldData.emUSE_RITEM;

	}

		void SBASIC::Assign ( SBASIC_106 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;

		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= sOldData.emUSE_LITEM;
		emUSE_RITEM				= sOldData.emUSE_RITEM;

		bLearnView				= sOldData.bLearnView;

	}

	void SBASIC::Assign ( SBASIC_107 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;

		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= sOldData.emUSE_LITEM;
		emUSE_RITEM				= sOldData.emUSE_RITEM;

		bLearnView				= sOldData.bLearnView;

	}


	void SBASIC::Assign ( SBASIC_108 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;

		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= sOldData.emUSE_LITEM;
		emUSE_RITEM				= sOldData.emUSE_RITEM;

		bLearnView				= sOldData.bLearnView;

	}


	void SBASIC::Assign ( SBASIC_109 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;

		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= sOldData.emUSE_LITEM;
		emUSE_RITEM				= sOldData.emUSE_RITEM;

		bLearnView				= sOldData.bLearnView;

	}


	bool SBASIC::LOAD_GS_0112 ( basestream &SFile )
	{
		DWORD dwData(0);
		WORD wData(0);

		SFile >> sNATIVEID.dwID;

		SFile >> bLearnView;

		SFile >> wData;

		SFile.ReadBuffer( &szNAME, sizeof(char)*MAX_SZNAME+1 );

		SFile >> dwMAXLEVEL;
		SFile >> dwGRADE;

		SFile >> dwData; emAPPLY = EMAPPLY( dwData );
		SFile >> dwData; emROLE = EMROLE( dwData );

		SFile >> dwData; //emACTION = EMACTION( dwData );

		SFile >> wTARRANGE;

		SFile >> wData;

		SFile >> dwData; emIMPACT_TAR = EMIMPACT_TAR( dwData );
		SFile >> dwData; emIMPACT_REALM = EMIMPACT_REALM( dwData );
		SFile >> dwData; emIMPACT_SIDE = EMIMPACT_SIDE( dwData );
		

		GLSKILL_ATT	emLEFT;
		GLSKILL_ATT	emRIGHT;

		SFile >> dwData; emLEFT = GLSKILL_ATT( dwData );
		SFile >> dwData; emRIGHT = GLSKILL_ATT( dwData );

		emUSE_LITEM	= GLSKILATT_TO_GLITEMATT( emLEFT );
		emUSE_RITEM	= GLSKILATT_TO_GLITEMATT( emRIGHT );

		SFile >> dwData;

		return TRUE;
	}

	bool SBASIC::LOAD_GS_0113 ( basestream &SFile )
	{
		DWORD dwData(0);
		WORD wData(0);

		SFile >> sNATIVEID.dwID;

		SFile >> bLearnView;

		SFile >> wData;

		SFile.ReadBuffer( &szNAME, sizeof(char)*MAX_SZNAME+1 );

		
		SFile >> dwMAXLEVEL;
		SFile >> dwGRADE;

		SFile >> dwData; emAPPLY = EMAPPLY( dwData );
		SFile >> dwData; emROLE = EMROLE( dwData );

		SFile >> dwData; //emACTION = EMACTION( dwData );

		SFile >> wTARRANGE;

		SFile >> wData;

		SFile >> dwData; emIMPACT_SIDE = EMIMPACT_SIDE( dwData );
		SFile >> dwData; emIMPACT_TAR = EMIMPACT_TAR( dwData );
		SFile >> dwData; emIMPACT_REALM = EMIMPACT_REALM( dwData );
		
		GLSKILL_ATT	emLEFT;
		GLSKILL_ATT	emRIGHT;

		SFile >> dwData; emLEFT = GLSKILL_ATT( dwData );
		SFile >> dwData; emRIGHT = GLSKILL_ATT( dwData );

		emUSE_LITEM	= GLSKILATT_TO_GLITEMATT( emLEFT );
		emUSE_RITEM	= GLSKILATT_TO_GLITEMATT( emRIGHT );

		SFile >> dwData;

		return TRUE;
	}

	bool SBASIC::LOAD_GS_0114 ( basestream &SFile )
	{
		DWORD dwData(0);
		WORD wData(0);

		SFile >> sNATIVEID.dwID;

		SFile >> bLearnView;

		SFile >> wData;

		SFile.ReadBuffer( &szNAME, sizeof(char)*MAX_SZNAME+1 );

		
		SFile >> dwMAXLEVEL;
		SFile >> dwGRADE;

		SFile >> dwData; emAPPLY = EMAPPLY( dwData );
		SFile >> dwData; emROLE = EMROLE( dwData );

		SFile >> dwData; //emACTION = EMACTION( dwData );

		SFile >> wTARRANGE;

		SFile >> wData;

		SFile >> dwData; emIMPACT_SIDE = EMIMPACT_SIDE( dwData );
		SFile >> dwData; emIMPACT_TAR = EMIMPACT_TAR( dwData );
		SFile >> dwData; emIMPACT_REALM = EMIMPACT_REALM( dwData );
		
		GLSKILL_ATT	emLEFT;
		GLSKILL_ATT	emRIGHT;

		SFile >> dwData; emLEFT = GLSKILL_ATT( dwData );
		SFile >> dwData; emRIGHT = GLSKILL_ATT( dwData );

		emUSE_LITEM	= GLSKILATT_TO_GLITEMATT( emLEFT );
		emUSE_RITEM	= GLSKILATT_TO_GLITEMATT( emRIGHT );

		SFile >> dwData;

		return TRUE;
	}

	VOID SBASIC::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "sNATIVEID wMainID" << ",";
		SFile << "sNATIVEID wSubID" << ",";
		SFile << "szNAME" << ",";
		SFile << "dwGRADE" << ",";
		SFile << "dwMAXLEVEL" << ",";
		SFile << "emROLE" << ",";
		SFile << "emAPPLY" << ",";
		SFile << "emIMPACT_TAR" << ",";
		SFile << "emIMPACT_REALM" << ",";
		SFile << "emIMPACT_SIDE" << ",";
		SFile << "wTARRANGE" << ",";
		SFile << "emUSE_LITEM" << ",";
		SFile << "emUSE_RITEM" << ",";
		SFile << "bLearnView" << ",";
	}

	VOID SBASIC::SaveCsv ( std::fstream &SFile )
	{
		SFile << sNATIVEID.wMainID << ",";
		SFile << sNATIVEID.wSubID << ",";

		std::string str = szNAME;
		STRUTIL::OutputStrCsv( SFile, str );

		SFile << dwGRADE << ",";
		SFile << dwMAXLEVEL << ",";
		SFile << emROLE << ",";
		SFile << emAPPLY << ",";
		SFile << emIMPACT_TAR << ",";
		SFile << emIMPACT_REALM << ",";
		SFile << emIMPACT_SIDE << ",";
		SFile << wTARRANGE << ",";
		SFile << emUSE_LITEM << ",";
		SFile << emUSE_RITEM << ",";
		SFile << bLearnView << ",";
	}

	
	VOID SBASIC::LoadCsv ( CStringArray &StrArray )
	{
		iCsvCur = 0;

		sNATIVEID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sNATIVEID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		std::string str;
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], str );
		memset( szNAME, 0, sizeof(char) * MAX_SZNAME );
		StringCchCopy( szNAME, MAX_SZNAME, str.c_str() );

		dwGRADE = (DWORD)atol( StrArray[ iCsvCur++ ] );
		dwMAXLEVEL = (DWORD)atol( StrArray[ iCsvCur++ ] );
		emROLE = (EMROLE)atoi( StrArray[ iCsvCur++ ] );
		emAPPLY = (EMAPPLY)atoi( StrArray[ iCsvCur++ ] );
		emIMPACT_TAR = (EMIMPACT_TAR)atoi( StrArray[ iCsvCur++ ] );
		emIMPACT_REALM = (EMIMPACT_REALM)atoi( StrArray[ iCsvCur++ ] );
		emIMPACT_SIDE = (EMIMPACT_SIDE)atoi( StrArray[ iCsvCur++ ] );
		wTARRANGE = (WORD)atoi( StrArray[ iCsvCur++ ] );
		emUSE_LITEM = (GLITEM_ATT)atoi( StrArray[ iCsvCur++ ] );
		emUSE_RITEM = (GLITEM_ATT)atoi( StrArray[ iCsvCur++ ] );
		bLearnView = atoi( StrArray[ iCsvCur++ ] ) != 0 ? true : false;
	}

	
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
BOOL GLSKILL::SaveFile ( CSerialFile &SFile )
{
	SFile << DWORD(VERSION);

	SFile << (DWORD)FILE_SBASIC;
	{
		SFile << DWORD(SKILL::SBASIC::VERSION);
		SFile << (DWORD)sizeof(SKILL::SBASIC);

		CString cstrName;
		cstrName.Format( _T("SN_%03d_%03d"), m_sBASIC.sNATIVEID.wMainID, m_sBASIC.sNATIVEID.wSubID ); // by ??
		StringCchCopy( m_sBASIC.szNAME, SKILL::MAX_SZNAME, cstrName.GetString() );
		SFile.WriteBuffer ( &m_sBASIC, sizeof(SKILL::SBASIC) );
	}

	SFile << (DWORD)FILE_SAPPLY;
	{
		SFile << DWORD(SKILL::SAPPLY::VERSION);
		SFile << (DWORD)sizeof(SKILL::SAPPLY);
		SFile.WriteBuffer ( &m_sAPPLY, sizeof(SKILL::SAPPLY) );
	}

	SFile << (DWORD)FILE_SLEARN;
	{
		SFile << DWORD(SKILL::SLEARN::VERSION);
		SFile << (DWORD)sizeof(SKILL::SLEARN);
		SFile.WriteBuffer ( &m_sLEARN, sizeof(SKILL::SLEARN) );
	}

	SFile << (DWORD)FILE_SEXT_DATA;
	{
		SFile << DWORD(SKILL::SEXT_DATA::VERSION);
		SFile << (DWORD)sizeof(SKILL::SEXT_DATA);
		
		CString cstrName;
		cstrName.Format( _T("SD_%03d_%03d"), m_sBASIC.sNATIVEID.wMainID , m_sBASIC.sNATIVEID.wSubID ); // by ??
		m_sEXT_DATA.strCOMMENTS = cstrName.GetString();
		m_sEXT_DATA.SAVE ( SFile );
	}

	SFile << (DWORD)FILE_SPECIAL_SKILL;
	{
		SFile << DWORD(SKILL::SSPECIAL_SKILL::VERSION);
		SFile << (DWORD)sizeof(SKILL::SSPECIAL_SKILL);
		m_sSPECIAL_SKILL.SAVE( SFile );
	}
	
	SFile << (DWORD)FILE_END_DATA;

	return TRUE;
}

BOOL GLSKILL::LoadFile ( basestream &SFile, bool bPastLoad, bool bGS /*= FALSE */)
{
	DWORD dwVersion;
	SFile >> dwVersion;
	GASSERT(dwVersion==VERSION);

	DWORD dwDataType;
	DWORD dwVer=0, dwSize=0;

	SFile >> dwDataType;
	while ( dwDataType!=FILE_END_DATA )
	{
		switch ( dwDataType )
		{
		case FILE_SBASIC:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if( bPastLoad )
				{
					if ( dwVer==0x0100 )
					{
						GASSERT(sizeof(SKILL::SBASIC_100)==dwSize);

						SKILL::SBASIC_100 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_100) );
						m_sBASIC.Assign ( sOldData );
					}
					else if ( dwVer==0x0101 )
					{
						GASSERT(sizeof(SKILL::SBASIC_101)==dwSize);

						SKILL::SBASIC_101 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_101) );
						m_sBASIC.Assign ( sOldData );
					}
					else if ( dwVer==0x0102 )
					{
						GASSERT(sizeof(SKILL::SBASIC_102)==dwSize);

						SKILL::SBASIC_102 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_102) );
						m_sBASIC.Assign ( sOldData );
					}
					else if ( dwVer==0x0103 )
					{
						GASSERT(sizeof(SKILL::SBASIC_103)==dwSize);

						SKILL::SBASIC_103 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_103) );
						m_sBASIC.Assign ( sOldData );
					}
					else if (dwVer==0x0104)
					{
						GASSERT(sizeof(SKILL::SBASIC_105)==dwSize);

						SKILL::SBASIC_105 sOldData;
						SFile.ReadBuffer ( &m_sBASIC, sizeof(SKILL::SBASIC_105) );
						m_sBASIC.Assign ( sOldData );
					}
					else if (dwVer==0x0105)
					{
						GASSERT(sizeof(SKILL::SBASIC_105)==dwSize);

						SKILL::SBASIC_105 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_105) );
						m_sBASIC.Assign ( sOldData );
					}
					else if (dwVer==0x0106)
					{
						GASSERT(sizeof(SKILL::SBASIC_106)==dwSize);

						SKILL::SBASIC_106 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_106) );
						m_sBASIC.Assign ( sOldData );
					}
					else if (dwVer==0x0107)
					{
						GASSERT(sizeof(SKILL::SBASIC_107)==dwSize);

						SKILL::SBASIC_107 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_107) );
						m_sBASIC.Assign ( sOldData );
					}
					else if (dwVer==0x0108)
					{
						GASSERT(sizeof(SKILL::SBASIC_108)==dwSize);

						SKILL::SBASIC_108 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_108) );
						m_sBASIC.Assign ( sOldData );
					}
					else if (dwVer==0x0109)
					{
						GASSERT(sizeof(SKILL::SBASIC_109)==dwSize);

						SKILL::SBASIC_109 sOldData;
						SFile.ReadBuffer ( &sOldData, sizeof(SKILL::SBASIC_109) );
						m_sBASIC.Assign ( sOldData );
					}
					else if (dwVer==SKILL::SBASIC::VERSION)
					{
						GASSERT(sizeof(SKILL::SBASIC)==dwSize);
						SFile.ReadBuffer ( &m_sBASIC, sizeof(SKILL::SBASIC) );
					}
					else if (dwVer==0x0112)
					{
						//ran gs 1201
						m_sBASIC.LOAD_GS_0112 ( SFile );
					}
					else if (dwVer==0x0114)
					{
						//ran gs 1201
						m_sBASIC.LOAD_GS_0114 ( SFile );
					}
					else
					{
						CString strTemp("");
						strTemp.Format( "SKILL::SBASIC unknown data version.PastLoad Version:%d",dwVer );
						MessageBox ( NULL, strTemp, "ERROR", MB_OK );
						SFile.SetOffSet(SFile.GetfTell()+dwSize);
						return E_FAIL;
					}

				}
				else
				{
					if (dwVer==SKILL::SBASIC::VERSION)
					{
						GASSERT(sizeof(SKILL::SBASIC)==dwSize);
						SFile.ReadBuffer ( &m_sBASIC, sizeof(SKILL::SBASIC) );
					}
					else
					{
						MessageBox ( NULL, "SKILL::SBASIC unknown data version.", "ERROR", MB_OK );
						SFile.SetOffSet(SFile.GetfTell()+dwSize);
						return E_FAIL;
					}
				}	
			}
			break;
		case FILE_SAPPLY:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					SKILL::SAPPLY_100 sOldApply;
					GASSERT(0x0100==dwVer&&sizeof(SKILL::SAPPLY_100)==dwSize);

					SFile.ReadBuffer ( &sOldApply, sizeof(SKILL::SAPPLY_100) );
					m_sAPPLY.Assign ( sOldApply );
				}
				else if ( dwVer==0x0101 )
				{
					SKILL::SAPPLY_101 sOldApply;
					GASSERT(0x0101==dwVer&&sizeof(SKILL::SAPPLY_101)==dwSize);

					SFile.ReadBuffer ( &sOldApply, sizeof(SKILL::SAPPLY_101) );
					m_sAPPLY.Assign ( sOldApply );

				}
				else if ( dwVer==0x0102 )
				{
					SKILL::SAPPLY_102 sOldApply;
					GASSERT(0x0102==dwVer&&sizeof(SKILL::SAPPLY_102)==dwSize);

					SFile.ReadBuffer ( &sOldApply, sizeof(SKILL::SAPPLY_102) );
					m_sAPPLY.Assign ( sOldApply );

				}
				else if ( dwVer==0x0103 )
				{
					SKILL::SAPPLY_103 sOldApply;
					GASSERT(0x0103==dwVer&&sizeof(SKILL::SAPPLY_103)==dwSize);

					SFile.ReadBuffer ( &sOldApply, sizeof(SKILL::SAPPLY_103) );
					m_sAPPLY.Assign ( sOldApply );

				}
				else if ( dwVer==SKILL::SAPPLY::VERSION )
				{
					GASSERT(sizeof(SKILL::SAPPLY)==dwSize);
					SFile.ReadBuffer ( &m_sAPPLY, sizeof(SKILL::SAPPLY) );
				}
				else if ( dwVer==0x0110 )
				{
					//ran gs 1001
					m_sAPPLY.LOAD_GS_110 ( SFile );
				}
				else if ( dwVer==0x0111 )
				{
					//ran gs 1001
					m_sAPPLY.LOAD_GS_111 ( SFile );
				}
				else if ( dwVer==0x0112 )
				{
					//ran gs 1001
					m_sAPPLY.LOAD_GS_112 ( SFile );
				}
				else
				{
						CString strTemp("");
						strTemp.Format( "SAPPLY Unknown Data Version.PastLoad Version:%d",dwVer );
						MessageBox ( NULL, strTemp, "ERROR", MB_OK );
						SFile.SetOffSet(SFile.GetfTell()+dwSize);
						return E_FAIL;
				}
			}
			break;
		case FILE_SLEARN:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					GASSERT(sizeof(SKILL::SLEARN_100)==dwSize);
					SKILL::SLEARN_100 sLEARN_OLD;
					SFile.ReadBuffer ( &sLEARN_OLD, sizeof(SKILL::SLEARN_100) );
					m_sLEARN.Assign ( sLEARN_OLD );
				}
				else if ( dwVer==0x0101 )
				{
					GASSERT(sizeof(SKILL::SLEARN_101)==dwSize);
					SKILL::SLEARN_101 sLEARN_OLD;
					SFile.ReadBuffer ( &sLEARN_OLD, sizeof(SKILL::SLEARN_101) );
					m_sLEARN.Assign ( sLEARN_OLD );
				}
				else if ( dwVer==0x0102 )
				{
					if ( bGS )
					{
						GASSERT(sizeof(SKILL::SLEARN_PH_102)==dwSize);
						SKILL::SLEARN_PH_102 sLEARN_OLD;
						SFile.ReadBuffer ( &sLEARN_OLD, sizeof(SKILL::SLEARN_PH_102) );
						m_sLEARN.Assign ( sLEARN_OLD );
					}
					else
					{
						GASSERT(sizeof(SKILL::SLEARN_102)==dwSize);
						SKILL::SLEARN_102 sLEARN_OLD;
						SFile.ReadBuffer ( &sLEARN_OLD, sizeof(SKILL::SLEARN_102) );
						m_sLEARN.Assign ( sLEARN_OLD );
					}	
				}
				else if ( dwVer==0x0103 )
				{
					GASSERT(sizeof(SKILL::SLEARN_103)==dwSize);
					SKILL::SLEARN_103 sLEARN_OLD;
					SFile.ReadBuffer ( &sLEARN_OLD, sizeof(SKILL::SLEARN_103) );
					m_sLEARN.Assign ( sLEARN_OLD );
				}
				else if ( dwVer==0x0104)
				{
					if ( bGS )
					{
						GASSERT(sizeof(SKILL::SLEARN_PH_104)==dwSize);
						SKILL::SLEARN_PH_104 sLEARN_OLD;
						SFile.ReadBuffer ( &sLEARN_OLD, sizeof(SKILL::SLEARN_PH_104) );
						m_sLEARN.Assign ( sLEARN_OLD );
					}
					else
					{
						GASSERT(sizeof(SKILL::SLEARN_104)==dwSize);
						SKILL::SLEARN_104 sLEARN_OLD;
						SFile.ReadBuffer ( &sLEARN_OLD, sizeof(SKILL::SLEARN_104) );
						m_sLEARN.Assign ( sLEARN_OLD );
					}	
				}
				else if ( dwVer==SKILL::SLEARN::VERSION )
				{
					GASSERT(sizeof(SKILL::SLEARN)==dwSize);
					SFile.ReadBuffer ( &m_sLEARN, sizeof(SKILL::SLEARN) );
				}
				else
				{
					CString strTemp("");
						strTemp.Format( "SLEARN Unknown Data Version.PastLoad Version:%d",dwVer );
						MessageBox ( NULL, strTemp, "ERROR", MB_OK );
						SFile.SetOffSet(SFile.GetfTell()+dwSize);
						return E_FAIL;
				}
			}
			break;
		case FILE_SEXT_DATA:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					SKILL::SEXT_DATA_100 sOLD_DATA;

					GASSERT(sizeof(SKILL::SEXT_DATA_100)==dwSize);
					SFile.ReadBuffer ( &sOLD_DATA, sizeof(SKILL::SEXT_DATA_100) );
					m_sEXT_DATA.Assign ( sOLD_DATA );
				}
				else if ( dwVer==0x0101 )
				{
					SKILL::SEXT_DATA_101 sOLD_DATA;

					GASSERT(sizeof(SKILL::SEXT_DATA_101)==dwSize);
					SFile.ReadBuffer ( &sOLD_DATA, sizeof(SKILL::SEXT_DATA_101) );
					m_sEXT_DATA.Assign ( sOLD_DATA );
				}
				else if ( dwVer==0x0102 )
				{
					SKILL::SEXT_DATA_102 sOLD_DATA;

					GASSERT(sizeof(SKILL::SEXT_DATA_102)==dwSize);
					SFile.ReadBuffer ( &sOLD_DATA, sizeof(SKILL::SEXT_DATA_102) );
					m_sEXT_DATA.Assign ( sOLD_DATA );
				}
				else if ( dwVer==0x0103 )
				{
					SKILL::SEXT_DATA_103 sOLD_DATA;
					GASSERT(sizeof(sOLD_DATA)==dwSize);
					sOLD_DATA.LOAD ( SFile );

					m_sEXT_DATA.Assign ( sOLD_DATA );
				}
				else if ( dwVer==0x0104 )
				{
					SKILL::SEXT_DATA_104 sOLD_DATA;
					GASSERT(sizeof(sOLD_DATA)==dwSize);
					sOLD_DATA.LOAD ( SFile );

					m_sEXT_DATA.Assign ( sOLD_DATA );
				}
				else if ( dwVer==0x0105 )
				{
					SKILL::SEXT_DATA_105 sOLD_DATA;
					GASSERT(sizeof(sOLD_DATA)==dwSize);
					sOLD_DATA.LOAD ( SFile );

					m_sEXT_DATA.Assign ( sOLD_DATA );
				}
				else if ( dwVer==0x0106 )
				{
					SKILL::SEXT_DATA_106 sOLD_DATA;
					GASSERT(sizeof(sOLD_DATA)==dwSize);
					sOLD_DATA.LOAD ( SFile );

					m_sEXT_DATA.Assign ( sOLD_DATA );
				}
				else if ( dwVer==SKILL::SEXT_DATA::VERSION)
				{
					m_sEXT_DATA.LOAD ( SFile );
				}
				else if ( dwVer==0x0110 )
				{
					//ran gs
					m_sEXT_DATA.LOAD_GS_0110( SFile );
				}
				else
				{
					CString strTemp("");
						strTemp.Format( "SEXT_DATA Unknown Data Version.PastLoad Version:%d",dwVer );
						MessageBox ( NULL, strTemp, "ERROR", MB_OK );
						SFile.SetOffSet(SFile.GetfTell()+dwSize);
						return E_FAIL;
				}
			}
			break;
		case FILE_SPECIAL_SKILL:
			SFile >> dwVer;
			SFile >> dwSize;

			if ( bGS )
			{
				if ( dwVer==0x0104 )
				{
					m_sSPECIAL_SKILL.LOAD ( SFile );
				}
				else if ( dwVer==0x0103 )
				{
					m_sSPECIAL_SKILL.LOAD_GS_103 ( SFile );
				}
				else if ( dwVer==0x0101 )
				{
					m_sSPECIAL_SKILL.LOAD_GS_101 ( SFile );
				}
				else if ( dwVer==0x0100 )
				{
					m_sSPECIAL_SKILL.LOAD_100 ( SFile );
				}
				else
				{
					CString strTemp("");
						strTemp.Format( "SPECIAL_SKILL Unknown Data Version.PastLoad Version:%d",dwVer );
						MessageBox ( NULL, strTemp, "ERROR", MB_OK );
						SFile.SetOffSet(SFile.GetfTell()+dwSize);
						return E_FAIL;
				}
			}
			else
			{
				if ( dwVer==SKILL::SSPECIAL_SKILL::VERSION  )
				{
					m_sSPECIAL_SKILL.LOAD ( SFile );
				}
				else if ( dwVer==0x0101 )
				{
					m_sSPECIAL_SKILL.LOAD_101 ( SFile );
				}
				else if ( dwVer==0x0100 )
				{
					m_sSPECIAL_SKILL.LOAD_100 ( SFile );
				}
				else
				{
					CString strTemp("");
						strTemp.Format( "SPECIAL_SKILL Unknown Data Version.PastLoad Version:%d",dwVer );
						MessageBox ( NULL, strTemp, "ERROR", MB_OK );
						SFile.SetOffSet(SFile.GetfTell()+dwSize);
						return E_FAIL;
				}
			}
			break;
		};

		SFile >> dwDataType;
	};

	return TRUE;
}

VOID GLSKILL::SaveCsvHead ( std::fstream &SFile )
{
	SKILL::SBASIC::SaveCsvHead( SFile );
	SKILL::SLEARN::SaveCsvHead( SFile );
	SKILL::SEXT_DATA::SaveCsvHead( SFile );
	SKILL::SSPECIAL_SKILL::SaveCsvHead( SFile );
	SFile << std::endl;

	SKILL::SAPPLY::SaveCsvHead( SFile );
	SFile << std::endl;
}

VOID GLSKILL::SaveCsv ( std::fstream &SFile )
{
	m_sBASIC.SaveCsv( SFile );
	m_sLEARN.SaveCsv( SFile );
	m_sEXT_DATA.SaveCsv( SFile );
	m_sSPECIAL_SKILL.SaveCsv( SFile );
	SFile << std::endl;

	m_sAPPLY.SaveCsv( SFile );
	SFile << std::endl;
}


VOID GLSKILL::LoadCsv ( CStringArray &StrArray, int iLine )
{
	if( iLine )
	{
		m_sBASIC.LoadCsv( StrArray );
		m_sLEARN.LoadCsv( StrArray );
		m_sEXT_DATA.LoadCsv( StrArray );
		m_sSPECIAL_SKILL.LoadCsv( StrArray );
	}
	else
	{
		m_sAPPLY.LoadCsv( StrArray );
	}
}

//	??? ??
bool GLSKILL::IsSkillFact ()
{
	bool bFact(false);
	
	switch ( m_sAPPLY.emBASIC_TYPE )
	{
	case SKILL::EMFOR_VARHP:
	case SKILL::EMFOR_VARMP:
	case SKILL::EMFOR_VARSP:
	case SKILL::EMFOR_DEFENSE:
	case SKILL::EMFOR_HITRATE:
	case SKILL::EMFOR_AVOIDRATE:
	case SKILL::EMFOR_VARAP:
	case SKILL::EMFOR_VARDAMAGE:
	case SKILL::EMFOR_VARDEFENSE:
	case SKILL::EMFOR_PA:
	case SKILL::EMFOR_SA:
	case SKILL::EMFOR_MA:
	case SKILL::EMFOR_HP_RATE:
	case SKILL::EMFOR_MP_RATE:
	case SKILL::EMFOR_SP_RATE:
	case SKILL::EMFOR_RESIST:
	case SKILL::EMFOR_SUMMONTIME: //add summon
	case SKILL::EMFOR_CP_RATE:		//add cp
	case SKILL::EMFOR_CP_GAIN:	//add cp
		bFact = true;
		break;
	};

	if ( m_sAPPLY.emADDON!=EMIMPACTA_NONE )
		bFact = true;

	for(int i = 0; i < SKILL::MAX_LEVEL; i++)
	{
		switch ( m_sAPPLY.sSPECS[i].emSPEC )
		{
		case EMSPECA_REFDAMAGE:
		case EMSPECA_NONBLOW:
		case EMSPECA_PIERCE:
		case EMSPECA_TARRANGE:
		case EMSPECA_MOVEVELO:
		case EMSPECA_INVISIBLE:
		case EMSPECA_RECVISIBLE:
		case EMSPECA_ATTACKVELO:
		case EMSPECA_SKILLDELAY:
		case EMSPECA_PSY_DAMAGE_REDUCE:
		case EMSPECA_MAGIC_DAMAGE_REDUCE:
		case EMSPECA_PSY_DAMAGE_REFLECTION:
		case EMSPECA_MAGIC_DAMAGE_REFLECTION:
		case EMSPECA_DEFENSE_SKILL_ACTIVE:
		case EMSPECA_CONTINUEDAMAGE:
		case EMSPECA_CURSE:
		case EMSPECA_ATTACKRANGE:
		case EMSPECA_ATTACKANGLE:
		case EMSPECA_NOPOTION: //add antipot
		case EMSPECA_NOSKILL: //add antiskill
		case EMSPECA_STUN:
		case EMSPECA_IMMUNE:
		case EMSPECA_GETEXP:
		case EMSPECA_DAMAGEABSORB:
		case EMSPECA_ITEMDROP:
		case EMSPECA_GOLDDROP:
		case EMSPECA_EXPRATE:
		case EMSPECA_TALKTONPC:
		case EMSPECA_SPECIFIC_ITEMDROP:
		case EMSPECA_ATTACK_INT:
		case EMSPECA_ATTACK_RATE:
		case EMSPECA_DEF_INT:
		case EMSPECA_DEF_RATE:
		case EMSPECA_ACCURACY_RATE:
		case EMSPECA_EVASION_RATE:
			bFact = true;
			break;
		};
	}
	
	return bFact;
}

const char* GLSKILL::GetName()
{
	const char* szName = GLStringTable::GetInstance().GetString( m_sBASIC.szNAME, GLStringTable::SKILL );
	if ( !szName )		return m_sBASIC.szNAME;
	
	return szName;
}

const char* GLSKILL::GetDesc()
{
	const char* szDesc = GLStringTable::GetInstance().GetString( m_sEXT_DATA.strCOMMENTS.c_str(), GLStringTable::SKILL );
	if ( !szDesc )		return "";
	
	return szDesc;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
GLSkillMan::GLSkillMan () 
	: m_pd3dDevice(NULL)
	, m_bModify(false)
{
	memset(m_szFileName, 0, sizeof(char) * (MAX_PATH));
}

GLSkillMan::~GLSkillMan ()
{
}

HRESULT GLSkillMan::OneTimeSceneInit ()
{		
	for ( int i=0; i<EMSKILLCLASS_NSIZE; i++ )
	for ( int j=0; j<MAX_CLASSSKILL; j++ )
	{
		m_pSkills[i][j] = NULL;
	}		

	return S_OK;
}

HRESULT GLSkillMan::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	return S_OK;
}

HRESULT GLSkillMan::DeleteDeviceObjects ()
{
	return S_OK;
}

HRESULT GLSkillMan::FinalCleanup ()
{
	for ( int i=0; i<EMSKILLCLASS_NSIZE; i++ )
	for ( int j=0; j<MAX_CLASSSKILL; j++ )
	{
		SAFE_DELETE(m_pSkills[i][j]);
	}		

	return S_OK;
}

PGLSKILL GLSkillMan::GetData ( WORD wClass, WORD Index )
{
	if ( wClass>=EMSKILLCLASS_NSIZE )	return NULL;
	if ( Index>=MAX_CLASSSKILL )		return NULL;

	return m_pSkills[wClass][Index];
}

void GLSkillMan::SetData ( WORD wMID, WORD Index, const PGLSKILL pSkillData, bool binner )
{
	GASSERT(wMID<EMSKILLCLASS_NSIZE);
	GASSERT(Index<MAX_CLASSSKILL);

	if ( wMID>=EMSKILLCLASS_NSIZE )		return;
	if ( Index>=MAX_CLASSSKILL )		return;

	SAFE_DELETE(m_pSkills[wMID][Index]);

	m_pSkills[wMID][Index] = pSkillData;

	switch ( static_cast<EMSKILLCLASS>(wMID) )
	{
	case EMSKILL_FIGHTER_01:
	case EMSKILL_FIGHTER_02:
	case EMSKILL_FIGHTER_03:
	case EMSKILL_FIGHTER_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_FIGHTER_M | GLCC_FIGHTER_W;
		break;

	case EMSKILL_ARMS_01:
	case EMSKILL_ARMS_02:
	case EMSKILL_ARMS_03:
	case EMSKILL_ARMS_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_ARMS_M | GLCC_ARMS_W;
		break;

	case EMSKILL_ARCHER_01:
	case EMSKILL_ARCHER_02:
	case EMSKILL_ARCHER_03:
	case EMSKILL_ARCHER_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_ARCHER_M | GLCC_ARCHER_W;
		break;

	case EMSKILL_SPIRIT_01:
	case EMSKILL_SPIRIT_02:
	case EMSKILL_SPIRIT_03:
	case EMSKILL_SPIRIT_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_SPIRIT_M | GLCC_SPIRIT_W;
		break;

	case EMSKILL_EXTREME_01:
	case EMSKILL_EXTREME_02:
	case EMSKILL_EXTREME_03:
	case EMSKILL_EXTREME_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_EXTREME_M|GLCC_EXTREME_W;
		break;

		//add class
	case EMSKILL_SCIENCE_01:
	case EMSKILL_SCIENCE_02:
	case EMSKILL_SCIENCE_03:
	case EMSKILL_SCIENCE_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_SCIENCE_M|GLCC_SCIENCE_W;
		break;

		//add class
	case EMSKILL_ASSASIN_01:
	case EMSKILL_ASSASIN_02:
	case EMSKILL_ASSASIN_03:
	case EMSKILL_ASSASIN_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_ASSASIN_M|GLCC_ASSASIN_W;
		break;

	case EMSKILL_TESTING_01:
	case EMSKILL_TESTING_02:
	case EMSKILL_TESTING_03:
	case EMSKILL_TESTING_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_TESTING_M|GLCC_TESTING_W;
		break;

	case EMSKILL_SHAPER_01:
	case EMSKILL_SHAPER_02:
	case EMSKILL_SHAPER_03:
	case EMSKILL_SHAPER_04:
		pSkillData->m_sLEARN.dwCLASS = GLCC_SHAPER_M|GLCC_SHAPER_W;
		break;


	default:
		pSkillData->m_sLEARN.dwCLASS = GLCC_NONE;
		break;
	};

	if ( !binner )	m_bModify = true;
}

HRESULT GLSkillMan::LoadFile ( const char* szFile, bool bPastLoad, bool bGS /*= FALSE*/ )
{
	StringCchCopy(m_szFileName,MAX_PATH,szFile);

	char szFullPath[MAX_PATH] = {0};
	StringCchCopy ( szFullPath, MAX_PATH, GLOGIC::GetPath() );
	StringCchCat ( szFullPath, MAX_PATH, m_szFileName );

	std::auto_ptr<basestream> pBStream( GLOGIC::openfile_basestream(GLOGIC::bGLOGIC_ZIPFILE, 
																	GLOGIC::strGLOGIC_ZIPFILE.c_str(),
                                                                    szFullPath, 
																	szFile,
																	false,
																	GLOGIC::bGLOGIC_PACKFILE ) );

	if ( !pBStream.get() )
		return E_FAIL;

	basestream &SFile = *pBStream;

	DWORD dwFILEVER;
	char szFILETYPE[_MAX_FNAME];
	SFile.GetFileType( szFILETYPE, _MAX_FNAME, dwFILEVER );
	SFile.SetEncode ( true );

	DWORD dwSkillNum;
	SFile >> dwSkillNum;

	for ( DWORD i=0; i<dwSkillNum; i++ )
	{
		GLSKILL *pSkill = new GLSKILL;

		if( pSkill->LoadFile ( SFile, bPastLoad, bGS ) == E_FAIL )
			return E_FAIL;
		SetData ( pSkill->m_sBASIC.sNATIVEID.wMainID, pSkill->m_sBASIC.sNATIVEID.wSubID, pSkill, true );
	}

	return S_OK;
}

HRESULT GLSkillMan::SaveFile ( const char* szFile )
{
	StringCchCopy(m_szFileName,MAX_PATH,szFile);

	char szFullPath[MAX_PATH] = {0};
	StringCchCopy ( szFullPath, MAX_PATH, GLOGIC::GetPath() );
	StringCchCat ( szFullPath, MAX_PATH, m_szFileName );

	CSerialFile SFile;

	SFile.SetFileType ( _FILEHEAD, VERSION );
	if ( !SFile.OpenFile ( FOT_WRITE, szFullPath ) )	return E_FAIL;
	SFile.SetEncode ( true );

	DWORD dwSkillNum = 0;
	for ( int i=0; i<EMSKILLCLASS_NSIZE; i++ )
	for ( int j=0; j<MAX_CLASSSKILL; j++ )
	{
		if ( m_pSkills[i][j] )	dwSkillNum++;
	}

	SFile << dwSkillNum;

	for ( int i=0; i<EMSKILLCLASS_NSIZE; i++ )
	for ( int j=0; j<MAX_CLASSSKILL; j++ )
	{
		if ( m_pSkills[i][j] )	m_pSkills[i][j]->SaveFile ( SFile );
	}

	m_bModify = false;
	return S_OK;
}

WORD GLSkillMan::FindFreeSkillIndex ( WORD wClass )
{
	GASSERT(wClass<EMSKILLCLASS_NSIZE);

	for ( WORD i=0; i<MAX_CLASSSKILL; i++ )
	{
		if ( !m_pSkills[wClass][i] )			return i;
	}

	return 0xFFFF;
}

BOOL GLSkillMan::DeleteSkill ( WORD wClass, WORD Index )
{
	GASSERT(wClass<EMSKILLCLASS_NSIZE);
	GASSERT(Index<MAX_CLASSSKILL);

	SAFE_DELETE(m_pSkills[wClass][Index]);

	m_bModify = true;
	return TRUE;
}

void GLSkillMan::GetMaxSkill ( WORD& wClass, WORD& rIndex )
{
	wClass = EMSKILLCLASS_NSIZE;
	rIndex = MAX_CLASSSKILL;
}

const char* GLSkillMan::_LOGFILE = "_CrowData_Synce.txt";
const char* GLSkillMan::_STRINGTABLE = "SkillStrTable.txt";

static CString	GetAppPath ()
{
	CString strFullPath;
	CString strCommandLine;

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	strCommandLine = szPath;

	if ( !strCommandLine.IsEmpty() )
	{
		DWORD dwFind = strCommandLine.ReverseFind ( '\\' );
		if ( dwFind != -1 )
		{
			strFullPath = strCommandLine.Left ( dwFind );
			
			if ( !strFullPath.IsEmpty() )
			if ( strFullPath.GetAt(0) == '"' )
				strFullPath = strFullPath.Right ( strFullPath.GetLength() - 1 );
		}
	}

	return strFullPath;
}

bool GLSkillMan::ValidData ()
{
	for ( int i = 0; i < EMSKILLCLASS_NSIZE; i++ )
	for ( int j = 0; j < MAX_CLASSSKILL; j++ )
	{
		if ( m_pSkills[i][j] )		return true;
	}

	return false;
}

HRESULT GLSkillMan::SyncUpdateData ()
{
	//	Note : ??? ??? ??? ?? ???.
	//
	CString strAppPath;
	char szAppPath[MAX_PATH] = {0};
	char szFullPath[MAX_PATH] = {0};
	strAppPath = GetAppPath ();
	StringCchCopy ( szAppPath, MAX_PATH, strAppPath.GetString () );

	//	Note : DxBoneCollector ?? ?? ??.
	//
	StringCchCopy ( szFullPath, MAX_PATH, szAppPath );
	StringCchCat ( szFullPath, MAX_PATH, SUBPATH::OBJ_FILE_SKELETON );
	DxBoneCollector::GetInstance().OneTimeSceneInit ( szFullPath );

	//	Note : ????? ???.
	//
	CDebugSet::OneTimeSceneInit ( szAppPath );

	//	Note : Animation ?? ?? ??.
	//
	StringCchCopy ( szFullPath, MAX_PATH, szAppPath );
	StringCchCat ( szFullPath, MAX_PATH, SUBPATH::OBJ_FILE_ANIMATION );
	DxSkinAniMan::GetInstance().OneTimeSceneInit ( szFullPath );

	//	Note : SkinObject ?? ?? ??.
	//
	StringCchCopy ( szFullPath, MAX_PATH, szAppPath );
	StringCchCat ( szFullPath, MAX_PATH, SUBPATH::OBJ_FILE_SKINOBJECT );
	DxSkinObject::SetPath ( szFullPath );
	DxSkinCharDataContainer::GetInstance().SetPath ( szFullPath );
	DxSkinPieceContainer::GetInstance().SetPath ( szFullPath );

	//	Note : GLogic
	//
	StringCchCopy ( szFullPath, MAX_PATH, szAppPath );
	StringCchCat ( szFullPath, MAX_PATH, SUBPATH::GLOGIC_FILE );
	GLOGIC::SetPath ( szFullPath );

	GLCONST_CHAR::LOADFILE ( "default.charclass" );

	GLCONST_CHAR::cCONSTCLASS[GLCI_FIGHTER_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_FIGHTER_M] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_ARMS_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_ARMS_M] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_ARCHER_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_ARCHER_W] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_SPIRIT_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_SPIRIT_W] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_EXTREME_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_EXTREME_M] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_EXTREME_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_EXTREME_W] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_FIGHTER_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_FIGHTER_W] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_ARMS_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_ARMS_W] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_ARCHER_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_ARCHER_M] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_SPIRIT_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_SPIRIT_M] );

	//add class
	GLCONST_CHAR::cCONSTCLASS[GLCI_SCIENCE_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_SCIENCE_M] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_SCIENCE_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_SCIENCE_W] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_ASSASIN_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_ASSASIN_M] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_ASSASIN_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_ASSASIN_W] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_TESTING_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_TESTING_M] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_TESTING_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_TESTING_W] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_SHAPER_M].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_SHAPER_M] );
	GLCONST_CHAR::cCONSTCLASS[GLCI_SHAPER_W].LoadAniSet ( GLCONST_CHAR::szCharSkin[GLCI_SHAPER_W] );



	//int nCHAR_FLAGS[GLCI_NUM_NEWSEX] =
	//add class
	int nCHAR_FLAGS[GLCI_NUM_2015] =
	{
		GLCC_FIGHTER_M,
		GLCC_ARMS_M,
		GLCC_ARCHER_W,
		GLCC_SPIRIT_W,
		GLCC_EXTREME_M,
		GLCC_EXTREME_W,
		GLCC_FIGHTER_W,
		GLCC_ARMS_W,
		GLCC_ARCHER_M,
		GLCC_SPIRIT_M,
		//add class
		GLCC_SCIENCE_M,
		GLCC_SCIENCE_W,
		GLCC_ASSASIN_M,
		GLCC_ASSASIN_W,
		GLCC_TESTING_M,
		GLCC_TESTING_W,
		//
		GLCC_SHAPER_M,
		GLCC_SHAPER_W,

	
	};

	CDebugSet::ClearFile ( _LOGFILE );

	for ( int i = 0; i < EMSKILLCLASS_NSIZE; i++ )
	for ( int j = 0; j < MAX_CLASSSKILL; j++ )
	{
		if ( !m_pSkills[i][j] )		continue;

		GLSKILL &sSKILL = *m_pSkills[i][j];

		for ( int k=0; k<SKILL::MAX_LEVEL; ++k )
		{
			if ( sSKILL.m_sAPPLY.sDATA_LVL[k].wAPPLYNUM==0 )
			{
				CDebugSet::ToFile ( _LOGFILE, "[%03d][%03d] ??(%d) ????? 0 ???.", i, j, k );
			}

			if ( sSKILL.m_sAPPLY.sDATA_LVL[k].wTARNUM==0 )
			{
				CDebugSet::ToFile ( _LOGFILE, "[%03d][%03d] ??(%d) ????? 0 ???.", i, j, k );
			}
		}

		//for ( int nClass=0; nClass<GLCI_NUM_NEWSEX; ++nClass )
		//add class
		for ( int nClass=0; nClass<GLCI_NUM_2015; ++nClass )
		{
			if ( sSKILL.m_sLEARN.dwCLASS & nCHAR_FLAGS[nClass] )
			{
				GLCONST_CHARCLASS &cCHARCLASS = GLCONST_CHAR::cCONSTCLASS[nClass];

				VECANIATTACK &sANILIST = cCHARCLASS.m_ANIMATION[sSKILL.m_sEXT_DATA.emANIMTYPE][sSKILL.m_sEXT_DATA.emANISTYPE];

				if ( sANILIST.empty() )
				{
					CDebugSet::ToFile ( _LOGFILE, "[%03d][%03d][%s] ??? '????'? ???? ?? .", i, j, sSKILL.GetName() );
					continue;
				}

				SANIATTACK &sANI = sANILIST[0];

				if ( sANI.m_wDivCount==0 )
				{
					CDebugSet::ToFile ( _LOGFILE, "[%03d][%03d][%s] '????'? '????'? ??.", i, j, sSKILL.GetName() );
				}
			}
		}
	}

	CDebugSet::ToFile ( _LOGFILE, "---------------------sync-completion--------------------------------" );	

	return S_OK;
}

HRESULT GLSkillMan::SyncStringTable()
{
	char	szFullPathFileName[MAX_PATH] = "";
	StringCchCopy ( szFullPathFileName, MAX_PATH, GLOGIC::GetPath() );
	StringCchCat ( szFullPathFileName, MAX_PATH, _STRINGTABLE );

	//DeleteFile ( szFullPathFileName );

	CString strBuffer1, strBuffer;
	std::string strTemp;
	std::string::size_type idx;

	strBuffer = "// File : Skill String Table\r\n";
	strBuffer += "// Note : ?(ID)? ??? ??? ??? ????? ???.\r\n//\r\n";
		
	for ( int i = 0; i < EMSKILLCLASS_NSIZE; i++ )
	for ( int j = 0; j < MAX_CLASSSKILL; j++ )
	{
		if ( !m_pSkills[i][j] )		continue;
		const GLSKILL &sSKILL = *m_pSkills[i][j];

		if( sSKILL.m_sBASIC.szNAME[0] != _T('\0') )
		{
			strBuffer1.Format( _T("SN_%03d_%03d\t%s\r\n"), i, j, sSKILL.m_sBASIC.szNAME );
			strBuffer += strBuffer1;
		}
		
		if( !sSKILL.m_sEXT_DATA.strCOMMENTS.empty() )
		{
			strTemp = sSKILL.m_sEXT_DATA.strCOMMENTS;
			idx = strTemp.find( "\r\n" );
			while ( idx != std::string::npos )
			{
				strTemp.replace( idx, 2, " " );
				idx = strTemp.find( "\r\n" );
			}
			
			strBuffer1.Format( _T("SD_%03d_%03d\t%s\r\n"), i, j, strTemp.c_str() );
			strBuffer += strBuffer1;
		}
	}

	CFile file;
	file.Open( _T(szFullPathFileName), CFile::modeCreate|CFile::modeWrite ); // ?? ??
	file.Write( strBuffer.GetString(), strBuffer.GetLength()-2 ); // ?? ??
	file.Close();

	//delete [] szEncode;
	return S_OK;
}

HRESULT GLSkillMan::SaveCsvFile ( CWnd* pWnd )
{
	CFileDialog dlg( FALSE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"csv file (*.csv)|*.csv|", pWnd );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() != IDOK )
		return E_FAIL;

	std::fstream streamFILE;
	streamFILE.open ( dlg.GetPathName().GetString(), std::ios_base::out );

	for( int i=0; i<EMSKILLCLASS_NSIZE; ++i )
	{
		for( int j=0; j<MAX_CLASSSKILL; ++j )
		{
			if( !m_pSkills[i][j] )
				continue;

			// Csv Head
			GLSKILL::SaveCsvHead( streamFILE );

			// Csv Data
			GLSKILL *pSKILL = m_pSkills[i][j];
			pSKILL->SaveCsv( streamFILE );
		}
	}

	streamFILE.close();

	return S_OK;
}



HRESULT GLSkillMan::LoadCsvFile ( CWnd* pWnd )
{
	CFileDialog dlg( TRUE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"csv file (*.csv)|*.csv|", pWnd );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() != IDOK )
		return E_FAIL;

	CStringFile StrFile( 5000000 );		//Increased size to read many data from csv file -  ..|.. [-_-] ..|..
	if( !StrFile.Open ( dlg.GetPathName().GetString() ) )
		return E_FAIL;

	STRUTIL::ClearSeparator();
	STRUTIL::RegisterSeparator( "," );

	CString strLine;
	CStringArray StrArray;

	// Csv Head
	int iHead[2] = { 0, 0 };
	for( int i=1; i<3; ++i )
	{
		StrFile.GetNextLine( strLine );
		STRUTIL::StringSeparate( strLine, StrArray );
		iHead[ i % 2 ] = (int)StrArray.GetCount();
	}

	int iLine = 2;
	bool bHead = true;
	char szError[ 256 ];

	int iMID = EMSKILLCLASS_NSIZE;
	int iSID = MAX_CLASSSKILL;

	while( StrFile.GetNextLine( strLine ) )
	{
		if( ++iLine % 2 )
			bHead = !bHead;
		if( bHead )
			continue;

		STRUTIL::StringSeparate( strLine, StrArray );

		int iCount = (int)StrArray.GetCount();
		if( iCount < iHead[ iLine % 2 ] )
		{
			StringCchPrintf( szError, 256, "%d Line = include blank column, next item load failed", iLine );
			MessageBox( pWnd->GetSafeHwnd(), _T(szError), _T("Fail"), MB_OK );
			return E_FAIL;
		}

		if( iLine % 2 )
		{
			iMID = atoi( StrArray[0] );
			iSID = atoi( StrArray[1] );
		}

		if( iMID >= EMSKILLCLASS_NSIZE || iSID >= MAX_CLASSSKILL )
			continue;

		if( !m_pSkills[ iMID ][ iSID ] )
			m_pSkills[ iMID ][ iSID ] = new GLSKILL;

		// Csv Data
		GLSKILL *pSKILL = m_pSkills[ iMID ][ iSID ];
		pSKILL->LoadCsv( StrArray, iLine % 2 );
	}

	return S_OK;
}

namespace COMMENT
{
	std::string SKILL_ROLE[SKILL::EMROLE_NSIZE] =
	{
		"Active",
		"Passive"
	};

	std::string SKILL_APPLY[SKILL::EMAPPLY_NSIZE] =
	{
		"Melee",
		"Messile",
		"Magic"
	};

	std::string SKILL_TYPES[SKILL::FOR_TYPE_SIZE] = 
	{
		"Hp Transformation",
		"MP Transformation",
		"SP Transformation",

		"HP Transformation Frequency",
		"MP Transformation Frequency",
		"SP Transformation Frequency",

		"Defence Transformation Rate",
		"Detoxify",
		"Hit Percentage Transformation",
		"Prevention Transformation Frequency",
		"HP+MP+SP Transformation Frequency",

		"Attack Value Transformation",
		"Defend Value Transformation",

		"Close Quarter Fighting rate",
		"Hitting rate",
		"Magic Power Rate",

		"HP Increasing Rate",
		"MP Increasing Rate",
		"SP Increasing Rate",

		"Pet All Item",	// PetData
		"Pet Rare Item",
		"Pet Potion Item",
		"Pet Money",
		"Pet Stone Item",
		"Pet Master HP Transformation Frequency",
		"Pet Master HP+MP+SPTransformation Frequency",
		"Pet Master Attack Ability",
		"Pet Master Defense Ability",
		"Pet Auto Use Potion",
		"Pet Prevent Item",
		"Changed Resistance",
		"Increase SummonTime",
		"Add CP",
		"CP Rate"
	};

	float SKILL_TYPES_SCALE[SKILL::FOR_TYPE_SIZE] =
	{
		1.0f,
		1.0f,
		1.0f,

		100.0f,
		100.0f,
		100.0f,

		1.0f,
		1.0f,
		1.0f,
		1.0f,
		100.0f,
		
		1.0f,
		1.0f,
	
		1.0f,
		1.0f,
		1.0f,

		1.0f,
		1.0f,
		1.0f,

		1.0f,	// PetData
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		
		1.0f,
		1.0f,

		0.0f,
		0.0f
	};

	std::string SKILL_LEVEL[SKILL::MAX_LEVEL] =
	{
		"Grade01",
		"Grade02",
		"Grade03",
		"Grade04",
		"Grade05",
		"Grade06",
		"Grade07",
		"Grade08",
		"Grade09",
	};

	std::string SKILL_EFFTIME[SKILL::EMTIME_NSIZE] =
	{
		"Mobilizing",
		"Hitting",
		"at the effect ending",
		"Goal's Installation",
		"NULL",
		"When Effect Activated"
	};

	std::string SKILL_EFFPOS[SKILL::EMPOS_NSIZE] =
	{
		"Ground position",
		"Hit Position",
	};

	std::string SPECIAL_SKILL_TYPE[SKILL::EMSSTYPE_NSIZE] =
	{
		"None",
		"Transform(CPS)",
		"Summon",
		"(x)Transform(CHF)" //add panther
	};

	std::string EMACTION[SKILL::EMSTIGMA_SIZE] =
	{
		"Normal Attack",
		"Buff",
		"Debuff",
		"Limit"
	};

	std::string SPEC_HALLUCINATE_TYPE[SKILL::EMHALLUCINATE_SIZE] =
	{
		"Transformation",
		"Forced Emoticons"
	};

	std::string SPEC_DOMINATE_TYPE[SKILL::EMDOMINATE_SIZE] =
	{
		"Own",
		"Caster",
		"Random",
		"Fixing"
	};

	std::string SPEC_ASIGN_TYPE[SKILL::EMASIGN_SIZE] =
	{
		"Before",
		"After"
	};

	std::string SPEC_INC_TYPE[SKILL::EMINC_SIZE] =
	{
		"Increase",
		"Decrease"
	};

	std::string SPEC_INVISIBLE_USE_TYPE[SKILL::EMINVISIBLE_SIZE] =
	{
		"Combat Use"
	};

	std::string SPEC_INVISIBLE_ANI_TYPE[SKILL::EMINVISIBLE_ANI_SIZE] =
	{
		"None",
		"Walking",
		"Teleport"
	};

	std::string SPEC_SWAPPOS_DIR_TYPE[SKILL::EMSWAPPOS_DIR_SIZE] =
	{
		"All",
		"Self",
		"Target"
	};
};