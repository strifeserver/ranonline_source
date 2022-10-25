#include "stdafx.h"
#include "./GLogicEx.h"
#include "./GLItemMan.h"
#include "./GLGaeaServer.h"
#include "./GLChar.h"
#include "./GLQuest.h"
#include "./GLQuestMan.h"
#include "./GLSchoolFreePK.h"
#include "./GLChar.h"
#include "../RanClientUILib/Interface/InnerInterface.h"
#include "../RanClientUILib/Interface/GameTextControl.h"
#include "../RanClientUILib/Interface/UITextControl.h"

#include "../RanClientUILib/Interface/ModalWindow.h"
#include "../RanClientUILib/Interface/ModalCallerID.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace GLOGICEX
{
	int CALCKILLEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan )
	{	
		GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
		if ( !pActor )	return 0;

		WORD wAttackerLev = wMYLEVEL;
		WORD wDefenserLev = pActor->GetLevel();
		DWORD dwBonusExp = pActor->GetBonusExp();

		//	Note : ����ġ ����	
		return GLOGICEX::GLKILLEXP(wAttackerLev,wDefenserLev,dwBonusExp);
	}

	int CALCATTACKEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan, DWORD dwDamage )
	{
		GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
		if ( !pActor )	return 0;

		WORD wTAR_LEVEL = pActor->GetLevel ();
		DWORD dwTAR_MAXHP = pActor->GetMaxHP ();
		DWORD dwBONUSEXP = pActor->GetBonusExp ();

		//	Note : ����ġ ����
		return GLOGICEX::GLATTACKEXP ( wMYLEVEL, wTAR_LEVEL, dwDamage, dwTAR_MAXHP, dwBONUSEXP );
	}

	int CALCKILLCP ( const STARGETID &cTargetID, const GLLandMan* pLandMan ) //add cp
	{
		GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
		if ( !pActor )	return 0;

		WORD wTAR_LEVEL = pActor->GetLevel ();
		return wTAR_LEVEL;
	}
};

void GLCHARLOGIC::RESET_DATA ()
{
	int i=0;

	SCHARDATA2::Assign ( SCHARDATA2() );

	m_CHARINDEX = GLCI_FIGHTER_M;
	m_SKILLDELAY.clear();

	for ( i=0; i<SKILLFACT_SIZE; ++i )
		m_sSKILLFACT[i] = SSKILLFACT();

	m_dwHOLDBLOW = NULL;
	m_bSTATEBLOW = false;

	for ( i=0; i<SKILLFACT_SIZE; ++i )
		m_sSTATEBLOWS[i] = SSTATEBLOW();
	
	m_fSTATE_MOVE = 1.0f;
	m_fSTATE_DELAY = 1.0f;

	m_fSTATE_DAMAGE = 1.0f;

	m_bSTATE_PANT = false;
	m_bSTATE_STUN = false;
	m_bTRANSFORM_EFF = false; //Transform Function 
	m_bINVISIBLE = false;
	m_bRECVISIBLE = false;

	m_fSKILL_MOVE = 0.0f;
	m_fOPTION_MOVE = 0.0f;

	m_sDefenseSkill.RESET();

	m_sSUMSTATS = SCHARSTATS();

	m_wSUM_AP = 0;
	m_wSUM_DP = 0;
	m_wSUM_PA = 0;
	m_wSUM_SA = 0;
	m_wSUM_MA = 0;

	m_sSUMRESIST = SRESIST();
	m_sSUMRESIST_SKILL = SRESIST();
	m_sSUMITEM = SSUM_ITEM();
	m_sSUM_PASSIVE = SPASSIVE_SKILL_DATA();

	m_nSUM_PIERCE = 0;
	m_fSUM_TARRANGE = 0;

	m_wACCEPTP = 0;
	m_wSUM_DisSP = 0;

	m_nHIT = 0;
	m_nSUM_HIT = 0;
	m_nAVOID = 0;
	m_nSUM_AVOID = 0;

	m_nDEFENSE_BODY = 0;
	m_nDEFENSE = 0;
	m_nDEFENSE_SKILL = 0;

	m_gdDAMAGE = GLPADATA();
	m_gdDAMAGE_SKILL = GLPADATA();
	m_gdDAMAGE_PHYSIC = GLPADATA();

	m_fDamageRate = 1.0f;
	m_fDefenseRate = 1.0f;

	m_fINCR_HP = 0.0f;
	m_fINCR_MP = 0.0f;
	m_fINCR_SP = 0.0f;

	m_fATTVELO = 0.0f;
	m_wATTRANGE = 0;
	m_wSUM_ATTRANGE = 0;
	m_fSKILLDELAY = 0.0f;

	m_emITEM_ATT = ITEMATT_NOTHING;

	m_emANISUBTYPE = AN_SUB_NONE;

	m_idACTIVESKILL = SNATIVEID(false);
	m_emANIMAINSKILL = AN_GUARD_N;
	m_emANISUBSKILL = AN_SUB_NONE;

	m_bUseArmSub = FALSE;

	for ( i=0; i<SLOT_TSIZE; ++i )
		m_pITEMS[i] = NULL;

	m_fIncHP = 0.0f;
	m_fIncMP = 0.0f;
	m_fCP_GAIN = 1.0f; //add cp
	m_fCP_SKILLGAIN = 0.0f; //add cp

	m_fCrit_Dmg = 0; //add critdmg
	m_fBlow_Dmg = 0; //add blowdmg
	m_fCrit_Rate = 0;//add critrate
	m_fBlow_Rate = 0;//add blowrate

	DEL_PLAYHOSTILE_ALL();	

	m_bVehicle = FALSE;
	m_fVehicleSpeedRate = 0.0f;
	m_fVehicleSpeedVol = 0.0f;

	m_sDamageSpec.RESET();

	m_ExpSkills.clear();

	m_bSafeZone = false;

	DISABLEALLLANDEFF();

	m_bAntiPot = false;  //add antipot
	m_bAntiSkill = false; //add skill
}

void GLCHARLOGIC::INIT_NEW_CHAR ( const EMCHARINDEX _emCIndex, const DWORD dwUserID, const DWORD dwServerID, const char* szCharName,
								 const WORD _wSchool, const WORD wHair, const WORD wFace, const WORD _wHairColor, const WORD _wSex )
{
	EMCHARINDEX emCIndex = _emCIndex;
	WORD wSchool = _wSchool;

	//if ( emCIndex>=GLCI_NUM_NEWSEX )			emCIndex = GLCI_FIGHTER_M;
	//add class
	if ( emCIndex>=GLCI_NUM_2015 )			emCIndex = GLCI_FIGHTER_M;

	if ( wSchool>=GLCONST_CHAR::wSCHOOLNUM )	wSchool = 0;

	SCHARDATA2 &CharData2 = GLCONST_CHAR::GET_CHAR_DATA2 ( wSchool, emCIndex );

	SCHARDATA2::Assign ( CharData2 );
	m_cInventory.SetItemGenTime();

	m_dwUserID		= dwUserID;			// ������ȣ.
	m_dwServerID	= dwServerID;		// �����׷�.
	StringCchCopy ( m_szName, CHAR_SZNAME, szCharName );	// ĳ���͸�.

	m_wSchool		= wSchool;
	m_wHair			= wHair;
	m_wFace			= wFace;
	m_wHairColor	= _wHairColor;
	m_wSex			= _wSex;

	INIT_DATA ( TRUE, TRUE );

	const GLCONST_CHARCLASS &cCONST = GLCONST_CHAR::cCONSTCLASS[emCIndex];

	if ( cCONST.dwHAIRNUM <= m_wHair )				m_wHair = 0;
	if ( cCONST.dwHEADNUM <= m_wFace )				m_wFace = 0;
	
	//	Note : �ʱ� ���� ��ġ ����. ( �б��� ���� Ʋ��. )
	//
	if ( GLCONST_CHAR::wSCHOOLNUM <= m_wSchool )	m_wSchool = 0;

	GLCONST_CHAR::nidSTARTMAP[m_wSchool];
	GLCONST_CHAR::dwSTARTGATE[m_wSchool];
}

const SCHARSTATS& GLCHARLOGIC::GETSTATS_ADD () const
{
	static SCHARSTATS sSTATS_SUM;
	sSTATS_SUM = m_sStats + m_sSUMITEM.sStats;
	
	return sSTATS_SUM;
}

const SCHARSTATS& GLCHARLOGIC::GETSTATS_ITEM () const
{
	return m_sSUMITEM.sStats;
}

void GLCHARLOGIC::OptionMoveUp()
{
	m_fOPTION_MOVE += 0.1f;

	if( m_fOPTION_MOVE >= 3.0f )
		m_fOPTION_MOVE = 3.0f;
}

void GLCHARLOGIC::OptionMoveDown()
{
	m_fOPTION_MOVE -= 0.1f;

	if( m_fOPTION_MOVE <= 0.0f )
		m_fOPTION_MOVE = 0.0f;
}

void GLCHARLOGIC::SUM_ADDITION ( float fCONFT_POINT_RATE )
{
	int i=0;
	int nLEVEL = GETLEVEL();												//	�ɸ����� ����.
	EMCHARINDEX emCI = GETCHARINDEX ();										//	�ɸ����� ���� �ε���.
	const GLCONST_CHARCLASS &cCHARCONST = GLCONST_CHAR::cCONSTCLASS[emCI];	//	�ɸ����� �����.
	int ZBLEVEL = (nLEVEL-1);												//	Zero base Level.

	//	Note : ��ú� ��ų ���갪 �ջ�.
	//
	SUM_PASSIVE ();

	//	Note : �����ۿ� ���� �ΰ� ȿ���� �ջ�.
	//
	SUM_ITEM ();

	//	Note : �⺻ ���� �� ���.
	//		STATS = �ʱ��ġ + ������ ���� �ڵ�������ġ + ĳ���� ��������Ʈ�� �к��� ��ġ + ������ �ɼ�.
	//
	m_sSUMSTATS.RESET();
	m_sSUMSTATS = cCHARCONST.sBEGIN_STATS + cCHARCONST.sLVLUP_STATS*ZBLEVEL + m_sStats + m_sSUMITEM.sStats;
	
	//	Note : ���, ��� ���.
	m_wSUM_AP = DWORD ( ( cCHARCONST.wBEGIN_AP + (cCHARCONST.fLVLUP_AP*ZBLEVEL) ) * cCHARCONST.fCONV_AP );
	m_wSUM_DP = DWORD ( ( cCHARCONST.wBEGIN_DP + (cCHARCONST.fLVLUP_DP*ZBLEVEL) ) * cCHARCONST.fCONV_DP );
	
	//	Note : ����ġ, ���ġ.
	m_wPA = DWORD ( ( cCHARCONST.wBEGIN_PA + (cCHARCONST.fLVLUP_PA*ZBLEVEL) ) * cCHARCONST.fCONV_PA );
	m_wSA = DWORD ( ( cCHARCONST.wBEGIN_SA + (cCHARCONST.fLVLUP_SA*ZBLEVEL) ) * cCHARCONST.fCONV_SA );

	//	Note : ����ġ, ���ġ, ����ġ�� STATS �� �ݿ�.
	m_wPA += DWORD ( m_sSUMSTATS.dwPow * cCHARCONST.fPA_POW + m_sSUMSTATS.dwDex * cCHARCONST.fPA_DEX );
	m_wSA += DWORD ( m_sSUMSTATS.dwPow * cCHARCONST.fSA_POW + m_sSUMSTATS.dwDex * cCHARCONST.fSA_DEX );
	m_wMA = DWORD ( m_sSUMSTATS.dwDex * cCHARCONST.fMA_DEX + m_sSUMSTATS.dwSpi * cCHARCONST.fMA_SPI + m_sSUMSTATS.dwInt * cCHARCONST.fMA_INT );

	//	Note : ����ġ, ���ġ, ����ġ�� SUM_ITEM �� �ݿ�.
	int nSUM_PA = m_sSUMITEM.nPA + m_sSUM_PASSIVE.m_nPA;
	GLOGICEX::VARIATION ( m_wPA, UINT_MAX, nSUM_PA );	// ��ȭ�Ǵ� ���� 0 <= x < 0xffff ������ ����.

	int nSUM_SA = m_sSUMITEM.nSA + m_sSUM_PASSIVE.m_nSA;
	GLOGICEX::VARIATION ( m_wSA, UINT_MAX, nSUM_SA );	// ��ȭ�Ǵ� ���� 0 <= x < 0xffff ������ ����.

	int nSUM_MA = m_sSUMITEM.nMA + m_sSUM_PASSIVE.m_nMA;
	GLOGICEX::VARIATION ( m_wMA, UINT_MAX, nSUM_MA );	// ��ȭ�Ǵ� ���� 0 <= x < 0xffff ������ ����.

	//	Note : �ջ� ��ġ ����.
	//
	m_wSUM_PA = m_wPA;
	m_wSUM_SA = m_wSA;
	m_wSUM_MA = m_wMA;

	//	Note : HP, MP, SP �ѷ� Ȯ��.
	//
	m_sHP.dwMax = DWORD ( ( m_sSUMSTATS.dwStr*cCHARCONST.fHP_STR + m_sSUMITEM.nHP + m_sSUM_PASSIVE.m_nHP ) );
	m_sHP.dwMax = DWORD ( m_sHP.dwMax * (1+m_sSUM_PASSIVE.m_fHP_RATE) * fCONFT_POINT_RATE );
	if (m_sHP.dwMax > GLCONST_CHAR::m_sHP_MAX ) m_sHP.dwMax = GLCONST_CHAR::m_sHP_MAX;
	m_sHP.LIMIT();

	m_sMP.dwMax = DWORD ( ( m_sSUMSTATS.dwSpi*cCHARCONST.fMP_SPI + m_sSUMITEM.nMP + m_sSUM_PASSIVE.m_nMP ) );
	m_sMP.dwMax = DWORD ( m_sMP.dwMax * (1+m_sSUM_PASSIVE.m_fMP_RATE) * fCONFT_POINT_RATE );
	if (m_sMP.dwMax > GLCONST_CHAR::m_sMP_MAX ) m_sMP.dwMax = GLCONST_CHAR::m_sMP_MAX;
	m_sMP.LIMIT();
	
	m_sSP.dwMax = DWORD ( ( m_sSUMSTATS.dwSta*cCHARCONST.fSP_STA + m_sSUMITEM.nSP + m_sSUM_PASSIVE.m_nSP ) );
	m_sSP.dwMax = DWORD ( m_sSP.dwMax * (1+m_sSUM_PASSIVE.m_fSP_RATE) * fCONFT_POINT_RATE );
	if (m_sSP.dwMax > GLCONST_CHAR::m_sSP_MAX ) m_sSP.dwMax = GLCONST_CHAR::m_sSP_MAX;
	m_sSP.LIMIT();

	//add cp
	m_sCP.dwMax = DWORD ( GLCONST_CHAR::m_sCP_MAX + m_sSUMITEM.nCP + m_sSUM_PASSIVE.m_nCP );
	m_sCP.dwMax = DWORD ( m_sCP.dwMax * fCONFT_POINT_RATE );
	m_sCP.LIMIT();
	
	
	//	Note : ������, ȸ���� ���.
	m_nHIT = int ( (m_sSUMSTATS.dwDex*cCHARCONST.fHIT_DEX) + (m_sSUMSTATS.dwPow*cCHARCONST.fHIT_POW) + m_sSUMITEM.nHitRate + m_sSUM_PASSIVE.m_nHIT );
	m_nAVOID = int ( (m_sSUMSTATS.dwDex*cCHARCONST.fAVOID_DEX) + (m_sSUMSTATS.dwPow*cCHARCONST.fAVOID_POW) + m_sSUMITEM.nAvoidRate + m_sSUM_PASSIVE.m_nAVOID );
	//part of add directval project
	//multiply here
//	m_nHIT = int( m_nHIT * ( 100.0f + m_sSUMITEM.fRateHit_Per )*0.01f );
//	m_nAVOID = int ( m_nAVOID * ( 100.0f + m_sSUMITEM.fRateAvoid_Per )*0.01f );

	m_nDEFENSE_BODY = int ( m_wSUM_DP + m_sSUMSTATS.dwDex*cCHARCONST.fDEFENSE_DEX );

	//	Note : ���� ( DP + POW*��� + ITEM_DEF ) * �������� ����.
	m_nDEFENSE_SKILL = m_nDEFENSE = int ( m_nDEFENSE_BODY + m_sSUMITEM.nDefense + m_sSUM_PASSIVE.m_nDEFENSE );

	//	Note : ���ݷ�.
	m_gdDAMAGE.dwMax = m_gdDAMAGE.dwLow = int ( m_wSUM_AP + m_sSUM_PASSIVE.m_nDAMAGE );
	m_gdDAMAGE_SKILL = m_gdDAMAGE;

	m_gdDAMAGE_PHYSIC = m_gdDAMAGE_SKILL;
	m_gdDAMAGE_PHYSIC.dwLow += m_sSUMITEM.gdDamage.dwLow;
	m_gdDAMAGE_PHYSIC.dwMax += m_sSUMITEM.gdDamage.dwMax;

	if ( ISLONGRANGE_ARMS() )	m_gdDAMAGE_PHYSIC.VAR_PARAM ( m_wSUM_SA );	//	��Ÿ� ����.
	else						m_gdDAMAGE_PHYSIC.VAR_PARAM ( m_wSUM_PA );	//	���� ����.

	//	Note : ���׷�. ( ������ �����ƿ��� ������ ����. )
	m_sSUMRESIST.RESET();
	m_sSUMRESIST = m_sSUM_PASSIVE.m_sSUMRESIST + m_sSUMITEM.sResist;

	//	Note : HP, MP, SP ȸ���� ����.
	m_fINCR_HP = GLCONST_CHAR::fHP_INC_PER + m_sSUMITEM.fIncR_HP + m_sSUM_PASSIVE.m_fINCR_HP;
	m_fINCR_MP = GLCONST_CHAR::fMP_INC_PER + m_sSUMITEM.fIncR_MP + m_sSUM_PASSIVE.m_fINCR_MP;
	m_fINCR_SP = GLCONST_CHAR::fSP_INC_PER + m_sSUMITEM.fIncR_SP + m_sSUM_PASSIVE.m_fINCR_SP;
	m_fCP_GAIN = GLCONST_CHAR::fCP_RATE + m_sSUMITEM.fCP_GAIN + m_sSUM_PASSIVE.m_fCP_GAIN; //add cp

	m_fCrit_Dmg = GLCONST_CHAR::dwCRITICAL_DAMAGE + m_sSUMITEM.fInc_Critical_Dmg + m_sSUM_PASSIVE.m_fCrit_Dmg; //add critdmg
	m_fBlow_Dmg = GLCONST_CHAR::dwCRUSHING_BLOW_DAMAGE + m_sSUMITEM.fInc_CrushingBlow_Dmg + m_sSUM_PASSIVE.m_fBlow_Dmg; //add blowdmg
	m_fCrit_Rate = m_sSUM_PASSIVE.m_fCrit_Rate + m_sSUMITEM.fIncR_Critical;//add critrate
	m_fBlow_Rate = m_sSUM_PASSIVE.m_fBlow_Rate + m_sSUMITEM.fIncR_CrushingBlow;//add blowrate

	//part of add directval project
	m_nHP_Potion_Rate = m_sSUMITEM.nHP_Potion_Rate;
	m_nMP_Potion_Rate = m_sSUMITEM.nMP_Potion_Rate;
	m_nSP_Potion_Rate = m_sSUMITEM.nSP_Potion_Rate;

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	if ( m_pITEMS[emRHand] )
	{
		m_emITEM_ATT = m_pITEMS[emRHand]->sSuitOp.emAttack;
		m_wATTRANGE = m_pITEMS[emRHand]->sSuitOp.wAttRange;
	}
	else
	{
		m_emITEM_ATT = ITEMATT_NOTHING;
		m_wATTRANGE = GLCONST_CHAR::wMAXATRANGE_SHORT;
	}

	//	Note : ���ڶ�� ��ġ�� �ջ�. ( STATS, LEVEL )
	//
	m_wACCEPTP = 0;
	m_wACCEPTP += CALC_ACCEPTP ( GET_SLOT_NID(emLHand) );
	m_wACCEPTP += CALC_ACCEPTP ( GET_SLOT_NID(emRHand) );

	//	Note : "SP�Һ�� ���Ǵ� �䱸��ġ" + "������ ���� �Һ� SP �ջ�"
	//
	m_wSUM_DisSP = m_wACCEPTP;

	SITEM* pRHAND = GET_SLOT_ITEMDATA ( emRHand );
	SITEM* pLHAND = GET_SLOT_ITEMDATA ( emLHand );

	if ( pRHAND )	m_wSUM_DisSP += pRHAND->sSuitOp.wReqSP;
	if ( pLHAND )	m_wSUM_DisSP += pLHAND->sSuitOp.wReqSP;

	m_sHP.LIMIT ();
	m_sMP.LIMIT ();
	m_sSP.LIMIT ();
	m_sCP.LIMIT ();
}

void GLCHARLOGIC::SUM_ITEM ()
{
	m_sSUMITEM.RESET();

//	UPDATE_ITEMSETLIST ();//add giftset

	for ( int i=0; i<SLOT_NSIZE_S_2; i++ )
	{
		EMSLOT emSLOT = static_cast<EMSLOT>(i);
		if ( !VALID_SLOT_ITEM(emSLOT) )					continue;

		const SITEMCUSTOM& sItemCustom = GET_SLOT_ITEM ( emSLOT );

		SITEM &sItem = *m_pITEMS[emSLOT];

		if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
		{//add jitem
			SITEM* pItem =GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );

			if (pItem)
			{
				if ( pItem->sBasicOp.IsCostumeCombine() )
				{	//combine addons
					for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
					{
						switch ( pItem->sSuitOp.sADDON[addon].emTYPE )
						{
						case EMADD_NONE:
							break;
						case EMADD_HITRATE:
							m_sSUMITEM.nHitRate		+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
						case EMADD_AVOIDRATE:
							m_sSUMITEM.nAvoidRate	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;

						case EMADD_DAMAGE:
							m_sSUMITEM.gdDamage.dwLow+= pItem->sSuitOp.sADDON[addon].nVALUE;
							m_sSUMITEM.gdDamage.dwMax+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;

						case EMADD_DEFENSE:
							m_sSUMITEM.nDefense		+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;

						case EMADD_HP:
							m_sSUMITEM.nHP			+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;

						case EMADD_CP:
							m_sSUMITEM.nCP			+= pItem->sSuitOp.sADDON[addon].nVALUE; //add cp
							break;

						case EMADD_MP:
							m_sSUMITEM.nMP			+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
						case EMADD_SP:
							m_sSUMITEM.nSP			+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;

						case EMADD_STATS_POW:
							m_sSUMITEM.sStats.dwPow	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
						case EMADD_STATS_STR:
							m_sSUMITEM.sStats.dwStr	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
						case EMADD_STATS_SPI:
							m_sSUMITEM.sStats.dwSpi	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
						case EMADD_STATS_DEX:
							m_sSUMITEM.sStats.dwDex	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
						case EMADD_STATS_INT:
							m_sSUMITEM.sStats.dwInt	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
						case EMADD_STATS_STA:
							m_sSUMITEM.sStats.dwSta	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;

						case EMADD_PA:
							m_sSUMITEM.nPA	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
						case EMADD_SA:
							m_sSUMITEM.nSA	+= pItem->sSuitOp.sADDON[addon].nVALUE;
							break;
			
						};
					}
					//combine rate of change
					switch ( pItem->sSuitOp.sVARIATE.emTYPE )
					{
					case EMVAR_HP:
						m_sSUMITEM.fIncR_HP += pItem->sSuitOp.sVARIATE.fVariate;
						break;
					case EMVAR_MP:
						m_sSUMITEM.fIncR_MP += pItem->sSuitOp.sVARIATE.fVariate;
						break;
					case EMVAR_SP:
						m_sSUMITEM.fIncR_SP += pItem->sSuitOp.sVARIATE.fVariate;
						break;

					case EMVAR_CP:
						m_sSUMITEM.fCP_GAIN += pItem->sSuitOp.sVARIATE.fVariate; //add cp
						break;

					case EMVAR_AP:
						m_sSUMITEM.fIncR_HP += pItem->sSuitOp.sVARIATE.fVariate;
						m_sSUMITEM.fIncR_MP += pItem->sSuitOp.sVARIATE.fVariate;
						m_sSUMITEM.fIncR_SP += pItem->sSuitOp.sVARIATE.fVariate;
						break;
					case EMVAR_MOVE_SPEED:
						m_sSUMITEM.fIncR_MoveSpeed += pItem->sSuitOp.sVARIATE.fVariate;
						break;
					case EMVAR_ATTACK_SPEED:
						m_sSUMITEM.fIncR_AtkSpeed	+= pItem->sSuitOp.sVARIATE.fVariate;	
						break;
					case EMVAR_CRITICAL_RATE:
						m_sSUMITEM.fIncR_Critical += pItem->sSuitOp.sVARIATE.fVariate;
						break;
					case EMVAR_CRUSHING_BLOW:
						m_sSUMITEM.fIncR_CrushingBlow += pItem->sSuitOp.sVARIATE.fVariate;
						break;

					case EMVAR_CRIT_DMG://add critdmg
						m_sSUMITEM.fInc_Critical_Dmg += pItem->sSuitOp.sVARIATE.fVariate;
						break;

					case EMVAR_BLOW_DMG://add blowdmg
						m_sSUMITEM.fInc_CrushingBlow_Dmg += pItem->sSuitOp.sVARIATE.fVariate;
						break;
					};

					//combine volume of change
					switch ( pItem->sSuitOp.sVOLUME.emTYPE )
					{
					case EMVAR_HP:
						m_sSUMITEM.fInc_HP += pItem->sSuitOp.sVOLUME.fVolume;
						break;
					case EMVAR_MP:
						m_sSUMITEM.fInc_MP += pItem->sSuitOp.sVOLUME.fVolume;
						break;
					case EMVAR_SP:
						m_sSUMITEM.fInc_SP += pItem->sSuitOp.sVOLUME.fVolume;
						break;

					case EMVAR_CP:
						m_sSUMITEM.fCP_GAIN += pItem->sSuitOp.sVOLUME.fVolume; //add cp
						break;

					case EMVAR_AP:
						m_sSUMITEM.fInc_HP += pItem->sSuitOp.sVOLUME.fVolume;
						m_sSUMITEM.fInc_MP += pItem->sSuitOp.sVOLUME.fVolume;
						m_sSUMITEM.fInc_SP += pItem->sSuitOp.sVOLUME.fVolume;
						break;
					case EMVAR_MOVE_SPEED:
						m_sSUMITEM.fInc_MoveSpeed +=pItem->sSuitOp.sVOLUME.fVolume;
						break;
					case EMVAR_ATTACK_SPEED:
						m_sSUMITEM.fInc_AtkSpeed	+= pItem->sSuitOp.sVOLUME.fVolume;
						break;
					case EMVAR_CRITICAL_RATE:
						m_sSUMITEM.fIncR_Critical += pItem->sSuitOp.sVOLUME.fVolume;
						break;
					case EMVAR_CRUSHING_BLOW:
						m_sSUMITEM.fIncR_CrushingBlow += pItem->sSuitOp.sVOLUME.fVolume;
						break;

					case EMVAR_CRIT_DMG://add critdmg
						m_sSUMITEM.fInc_Critical_Dmg += pItem->sSuitOp.sVOLUME.fVolume;
						break;

					case EMVAR_BLOW_DMG://add blowdmg
						m_sSUMITEM.fInc_CrushingBlow_Dmg += pItem->sSuitOp.sVOLUME.fVolume;
						break;
					};
				}
			}
		}

		//	������ ���� ���ݿ��� ������ ���� ����.
		if ( sItem.sBasicOp.emItemType==ITEM_CHARM )	continue;

		for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
		{
			switch ( sItem.sSuitOp.sADDON[addon].emTYPE )
			{
			case EMADD_NONE:
				break;
			case EMADD_HITRATE:
				m_sSUMITEM.nHitRate		+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_AVOIDRATE:
				m_sSUMITEM.nAvoidRate	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_DAMAGE:
				m_sSUMITEM.gdDamage.dwLow+= sItem.sSuitOp.sADDON[addon].nVALUE;
				m_sSUMITEM.gdDamage.dwMax+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_DEFENSE:
				m_sSUMITEM.nDefense		+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_HP:
				m_sSUMITEM.nHP			+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_MP:
				m_sSUMITEM.nMP			+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_SP:
				m_sSUMITEM.nSP			+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_CP:
				m_sSUMITEM.nCP			+= sItem.sSuitOp.sADDON[addon].nVALUE; //add cp
				break;

			case EMADD_STATS_POW:
				m_sSUMITEM.sStats.dwPow	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_STR:
				m_sSUMITEM.sStats.dwStr	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_SPI:
				m_sSUMITEM.sStats.dwSpi	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_DEX:
				m_sSUMITEM.sStats.dwDex	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_INT:
				m_sSUMITEM.sStats.dwInt	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_STATS_STA:
				m_sSUMITEM.sStats.dwSta	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;

			case EMADD_PA:
				m_sSUMITEM.nPA	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			case EMADD_SA:
				m_sSUMITEM.nSA	+= sItem.sSuitOp.sADDON[addon].nVALUE;
				break;
			//case EMADD_MA:	//	�� �ɼ��� GETMaDAMAGE() ���� ������ ���ѵ� ó��.
			//	m_sSUMITEM.nMA	+= sItem.sSuitOp.sADDON[addon].nVALUE;
			//	break;
			};
		}

		m_sSUMITEM.nHP += (int) sItemCustom.GETOptVALUE(EMR_OPT_HP);
		m_sSUMITEM.nMP += (int) sItemCustom.GETOptVALUE(EMR_OPT_MP);
		m_sSUMITEM.nSP += (int) sItemCustom.GETOptVALUE(EMR_OPT_SP);
		m_sSUMITEM.nCP += (int) sItemCustom.GETOptVALUE(EMR_OPT_CP); //add cp

		//	Note : ��ȭ�� ȿ��.
		switch ( sItem.sSuitOp.sVARIATE.emTYPE )
		{
		case EMVAR_HP:
			m_sSUMITEM.fIncR_HP += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_MP:
			m_sSUMITEM.fIncR_MP += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_SP:
			m_sSUMITEM.fIncR_SP += sItem.sSuitOp.sVARIATE.fVariate;
			break;

		case EMVAR_CP:
			m_sSUMITEM.fCP_GAIN += sItem.sSuitOp.sVARIATE.fVariate; //add cp
			break;

		case EMVAR_AP:
			m_sSUMITEM.fIncR_HP += sItem.sSuitOp.sVARIATE.fVariate;
			m_sSUMITEM.fIncR_MP += sItem.sSuitOp.sVARIATE.fVariate;
			m_sSUMITEM.fIncR_SP += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_MOVE_SPEED:
			// ���߿� �ջ����� ����
			if ( emSLOT != SLOT_VEHICLE ) m_sSUMITEM.fIncR_MoveSpeed += sItemCustom.GETMOVESPEEDR();
			else if ( m_bVehicle ) m_sSUMITEM.fIncR_MoveSpeed += m_fVehicleSpeedRate + sItemCustom.GETMOVESPEEDR();;
			break;
		case EMVAR_ATTACK_SPEED:
			m_sSUMITEM.fIncR_AtkSpeed	+= sItem.sSuitOp.sVARIATE.fVariate;	
			break;
		case EMVAR_CRITICAL_RATE:
			m_sSUMITEM.fIncR_Critical +=  sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_CRUSHING_BLOW:
			m_sSUMITEM.fIncR_CrushingBlow += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		case EMVAR_CRIT_DMG://add critdmg
			m_sSUMITEM.fInc_Critical_Dmg += sItem.sSuitOp.sVARIATE.fVariate;
			break;

		case EMVAR_BLOW_DMG://add blowdmg
			m_sSUMITEM.fInc_CrushingBlow_Dmg += sItem.sSuitOp.sVARIATE.fVariate;
			break;
		};

		//add directval
		m_sSUMITEM.fIncR_HP += sItemCustom.GETOptVALUE(EMR_OPT_HP_INC_RATE) + sItemCustom.GETOptVALUE(EMR_OPT_HMS_INC_RATE);
		m_sSUMITEM.fIncR_MP += sItemCustom.GETOptVALUE(EMR_OPT_MP_INC_RATE) + sItemCustom.GETOptVALUE(EMR_OPT_HMS_INC_RATE);
		m_sSUMITEM.fIncR_SP += sItemCustom.GETOptVALUE(EMR_OPT_SP_INC_RATE) + sItemCustom.GETOptVALUE(EMR_OPT_HMS_INC_RATE);
		m_sSUMITEM.fCP_GAIN += sItemCustom.GETOptVALUE(EMR_OPT_CP_GAIN) ;//add cp

		//	Note : ��ȭ�� ȿ��.
		switch ( sItem.sSuitOp.sVOLUME.emTYPE )
		{
		case EMVAR_HP:
			m_sSUMITEM.fInc_HP += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_MP:
			m_sSUMITEM.fInc_MP += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_SP:
			m_sSUMITEM.fInc_SP += sItem.sSuitOp.sVOLUME.fVolume;
			break;

		case EMVAR_CP:
			m_sSUMITEM.fCP_GAIN += sItem.sSuitOp.sVOLUME.fVolume; //add cp
			break;

		case EMVAR_AP:
			m_sSUMITEM.fInc_HP += sItem.sSuitOp.sVOLUME.fVolume;
			m_sSUMITEM.fInc_MP += sItem.sSuitOp.sVOLUME.fVolume;
			m_sSUMITEM.fInc_SP += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_MOVE_SPEED:
			// ���߿� �ջ����� ����
			if ( emSLOT != SLOT_VEHICLE ) m_sSUMITEM.fInc_MoveSpeed += sItemCustom.GETMOVESPEED();
			else if	( m_bVehicle ) m_sSUMITEM.fInc_MoveSpeed += m_fVehicleSpeedVol + sItemCustom.GETMOVESPEED();
			break;
		case EMVAR_ATTACK_SPEED:
			m_sSUMITEM.fInc_AtkSpeed	+= sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_CRITICAL_RATE:
			m_sSUMITEM.fIncR_Critical += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		case EMVAR_CRUSHING_BLOW:
			m_sSUMITEM.fIncR_CrushingBlow += sItem.sSuitOp.sVOLUME.fVolume;
			break;

		case EMVAR_CRIT_DMG://add critdmg
			m_sSUMITEM.fInc_Critical_Dmg += sItem.sSuitOp.sVOLUME.fVolume;
			break;

		case EMVAR_BLOW_DMG://add blowdmg
			m_sSUMITEM.fInc_CrushingBlow_Dmg += sItem.sSuitOp.sVOLUME.fVolume;
			break;
		};

		// �̵��ӵ� �����ɼ� ����( ������ ������ ���� m_fVehicleSpeedVol ) 
//		if ( emSLOT != SLOT_VEHICLE ) m_sSUMITEM.fInc_MoveSpeed += sItemCustom.GETOptVALUE( EMR_OPT_MOVE_SPEED );
//		else if ( m_bVehicle ) m_sSUMITEM.fInc_MoveSpeed += sItemCustom.GETOptVALUE( EMR_OPT_MOVE_SPEED );

		//	Note : ������ �⺻ �ɷ� ADD ��ġ.
		//
		m_sSUMITEM.gdDamage			+= sItemCustom.GETDAMAGE();
		m_sSUMITEM.nDefense			+= sItemCustom.GETDEFENSE();

		m_sSUMITEM.nAvoidRate		+= sItemCustom.GETAVOIDRATE();
		m_sSUMITEM.nHitRate			+= sItemCustom.GETHITRATE();

		//	 Note : ������ ���� [+] ��ġ �ջ�.
		//
		m_sSUMITEM.sResist.nElectric	+= sItemCustom.GETRESIST_ELEC();
		m_sSUMITEM.sResist.nFire		+= sItemCustom.GETRESIST_FIRE();
		m_sSUMITEM.sResist.nIce			+= sItemCustom.GETRESIST_ICE();
		m_sSUMITEM.sResist.nPoison		+= sItemCustom.GETRESIST_POISON();
		m_sSUMITEM.sResist.nSpirit		+= sItemCustom.GETRESIST_SPIRIT();

		m_sSUMITEM.sStats.dwPow += sItemCustom.GET_STAT_POW();//add directval statspow
		m_sSUMITEM.sStats.dwSpi += sItemCustom.GET_STAT_INT();//add directval statsint
		m_sSUMITEM.sStats.dwDex += sItemCustom.GET_STAT_DEX();//add directval statsdex
		m_sSUMITEM.sStats.dwSta += sItemCustom.GET_STAT_STM();//add directval statsstm
		m_sSUMITEM.sStats.dwStr += sItemCustom.GET_STAT_VIT();//add directval statsvit

		m_sSUMITEM.nPA += sItemCustom.GET_MELEE();//add  directval melee
		m_sSUMITEM.nSA += sItemCustom.GET_MESSILE();//add directval messile
		m_sSUMITEM.nMA += sItemCustom.GET_ENERGY();//add directval energy

		m_sSUMITEM.nHP_Potion_Rate += sItemCustom.GET_HP_POTION(); //add directval hppotion
		m_sSUMITEM.nMP_Potion_Rate += sItemCustom.GET_MP_POTION(); //add directval mppotion
		m_sSUMITEM.nSP_Potion_Rate += sItemCustom.GET_SP_POTION(); //add directval sppotion

		m_sSUMITEM.fCP_GAIN += sItemCustom.GET_CP_GAIN();//add directval cpgain
		m_sSUMITEM.nCP += sItemCustom.GET_CP();//add directval cp
		m_sSUMITEM.fInc_Critical_Dmg += sItemCustom.GET_CRIT_DMG();//add directval critrate
		m_sSUMITEM.fInc_CrushingBlow_Dmg += sItemCustom.GET_BLOW_DMG();//add directval blowdamage
		m_sSUMITEM.fIncR_Critical += sItemCustom.GET_CRIT_RATE();//add directval critrate
		m_sSUMITEM.fIncR_CrushingBlow += sItemCustom.GET_BLOW_RATE();//add directval blowrate

		//this is part of add directval project
		//this will work as multiplier for overall hitrate
		m_sSUMITEM.fRateAvoid_Per += sItemCustom.GETAVOIDRATE_PER();
		m_sSUMITEM.fRateHit_Per += sItemCustom.GETHITRATE_PER();

		//	Note : ���ġ�� ���ݷ� ����.
		//
		m_sSUMITEM.nMA	+= sItemCustom.GETMaDAMAGE();
	}
}

void GLCHARLOGIC::SUM_PASSIVE ()
{
	m_sSUM_PASSIVE = SPASSIVE_SKILL_DATA();
	
	if ( m_bVehicle ) return;

	if ( m_ExpSkills.empty() ) return;

	SKILL_MAP_ITER iter = m_ExpSkills.begin ();
	SKILL_MAP_ITER iter_end = m_ExpSkills.end ();
	for ( ; iter!=iter_end; ++iter )
	{
		const SCHARSKILL &sCharSkill = (*iter).second;
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sCharSkill.sNativeID.wMainID, sCharSkill.sNativeID.wSubID );
		if ( !pSkill )											continue;
		if ( pSkill->m_sBASIC.emROLE != SKILL::EMROLE_PASSIVE )	continue;

		//	Note : ������ ���� ���� �˻�.
		//
		bool bvalid_left(true), bvalid_right(true);
		GLITEM_ATT emITEM_LEFT = ITEMATT_NOTHING;
		GLITEM_ATT emITEM_RIGHT = ITEMATT_NOTHING;
		GLITEM_ATT emSKILL_LEFT = pSkill->m_sBASIC.emUSE_LITEM;
		GLITEM_ATT emSKILL_RIGHT = pSkill->m_sBASIC.emUSE_RITEM;

		EMSLOT emRHand = GetCurRHand();
		EMSLOT emLHand = GetCurLHand();

		if ( emSKILL_LEFT!=ITEMATT_NOCARE )
		{
			bvalid_left = false;
			SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
			if ( pItem )
			{
				emITEM_LEFT = pItem->sSuitOp.emAttack;
				bvalid_left = CHECHSKILL_ITEM(emSKILL_LEFT,emITEM_LEFT);
			}
		}

		if ( emSKILL_RIGHT!=ITEMATT_NOCARE )
		{
			bvalid_right = false;
			SITEM* pItem = GET_SLOT_ITEMDATA(emRHand);
			if ( pItem )
			{
				emITEM_RIGHT = pItem->sSuitOp.emAttack;
				bvalid_right = CHECHSKILL_ITEM(emSKILL_RIGHT,emITEM_RIGHT);
			}
		}

		if ( !(bvalid_left&&bvalid_right) )						continue;

		const SKILL::CDATA_LVL &sDATA_LVL = pSkill->m_sAPPLY.sDATA_LVL[sCharSkill.wLevel];
		const float &fADDON = pSkill->m_sAPPLY.fADDON_VAR[sCharSkill.wLevel];
		const SKILL::SSPEC &sSPEC = pSkill->m_sAPPLY.sSPEC[sCharSkill.wLevel];
		if ( GLCONST_CHAR::strTRANSFORM_SKIN[pSkill->m_sBASIC.sNATIVEID.wMainID][pSkill->m_sBASIC.sNATIVEID.wSubID] != "" ) m_bTRANSFORM_EFF = true; //Transform Function 

		switch ( pSkill->m_sAPPLY.emBASIC_TYPE )
		{
		case SKILL::EMFOR_HP:
			m_sSUM_PASSIVE.m_nHP += WORD ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_MP:
			m_sSUM_PASSIVE.m_nMP += WORD ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_SP:
			m_sSUM_PASSIVE.m_nSP += WORD ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_VARHP:
			m_sSUM_PASSIVE.m_fINCR_HP += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_VARMP:
			m_sSUM_PASSIVE.m_fINCR_MP += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_VARSP:
			m_sSUM_PASSIVE.m_fINCR_SP += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_CP_GAIN:
			m_sSUM_PASSIVE.m_fCP_GAIN += sDATA_LVL.fBASIC_VAR; //add cp
			break;

		case SKILL::EMFOR_DEFENSE:
			m_sSUM_PASSIVE.m_nDEFENSE += int ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_HITRATE:
			m_sSUM_PASSIVE.m_nHIT += int ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_AVOIDRATE:
			m_sSUM_PASSIVE.m_nAVOID += int ( sDATA_LVL.fBASIC_VAR );
			break;

		case SKILL::EMFOR_VARAP:
			m_sSUM_PASSIVE.m_fINCR_HP += sDATA_LVL.fBASIC_VAR;
			m_sSUM_PASSIVE.m_fINCR_MP += sDATA_LVL.fBASIC_VAR;
			m_sSUM_PASSIVE.m_fINCR_SP += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_VARDAMAGE:
			m_sSUM_PASSIVE.m_nDAMAGE += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_VARDEFENSE:
			m_sSUM_PASSIVE.m_nDEFENSE += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_PA:
			m_sSUM_PASSIVE.m_nPA += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_SA:
			m_sSUM_PASSIVE.m_nSA += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_MA:
			m_sSUM_PASSIVE.m_nMA += int(sDATA_LVL.fBASIC_VAR);
			break;

		case SKILL::EMFOR_HP_RATE:
			m_sSUM_PASSIVE.m_fHP_RATE += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_MP_RATE:
			m_sSUM_PASSIVE.m_fMP_RATE += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_SP_RATE:
			m_sSUM_PASSIVE.m_fSP_RATE += sDATA_LVL.fBASIC_VAR;
			break;

		case SKILL::EMFOR_CP_RATE: //add cp
			m_sSUM_PASSIVE.m_nCP += int ( sDATA_LVL.fBASIC_VAR );;
			break;

		case SKILL::EMFOR_RESIST:
			m_sSUM_PASSIVE.m_sSUMRESIST += (int)(sDATA_LVL.fBASIC_VAR);
			break;
		};

		for(int a = 0; a < SKILL::MAX_LEVEL; a++ )
		{
			const float fVAR = pSkill->m_sAPPLY.sADDONS[a].fADDON_VAR[sCharSkill.wLevel];

			const SKILL::SSPEC &sSPEC = pSkill->m_sAPPLY.sSPECS[a].sSPEC[sCharSkill.wLevel];

			switch ( pSkill->m_sAPPLY.sADDONS[a].emADDON )
			{
			case EMIMPACTA_HITRATE:
				m_sSUM_PASSIVE.m_nHIT += int ( fVAR );
				break;
			case EMIMPACTA_AVOIDRATE:
				m_sSUM_PASSIVE.m_nAVOID += int ( fVAR );
				break;
			
			case EMIMPACTA_DAMAGE:
				m_sSUM_PASSIVE.m_nDAMAGE += int ( fVAR );
				break;

			case EMIMPACTA_DEFENSE:
				m_sSUM_PASSIVE.m_nDEFENSE += int ( fVAR );
				break;

			case EMIMPACTA_VARHP:
				m_sSUM_PASSIVE.m_fINCR_HP += fVAR;
				break;
			case EMIMPACTA_VARMP:
				m_sSUM_PASSIVE.m_fINCR_MP += fVAR;
				break;
			case EMIMPACTA_VARSP:
				m_sSUM_PASSIVE.m_fINCR_SP += fVAR;
				break;

			case EMIMPACTA_VARAP:
				m_sSUM_PASSIVE.m_fINCR_HP += fVAR;
				m_sSUM_PASSIVE.m_fINCR_MP += fVAR;
				m_sSUM_PASSIVE.m_fINCR_SP += fVAR;
				break;

			case EMIMPACTA_DAMAGE_RATE:
				m_sSUM_PASSIVE.m_fDAMAGE_RATE += fVAR;
				break;

			case EMIMPACTA_DEFENSE_RATE:
				m_sSUM_PASSIVE.m_fDEFENSE_RATE += fVAR;
				break;

			case EMIMPACTA_PA:
				m_sSUM_PASSIVE.m_nPA += int(fVAR);
				break;

			case EMIMPACTA_SA:
				m_sSUM_PASSIVE.m_nSA += int(fVAR);
				break;

			case EMIMPACTA_MA:
				m_sSUM_PASSIVE.m_nMA += int(fVAR);
				break;

			case EMIMPACTA_HP_RATE:
				m_sSUM_PASSIVE.m_fHP_RATE += fVAR;
				break;

			case EMIMPACTA_MP_RATE:
				m_sSUM_PASSIVE.m_fMP_RATE += fVAR;
				break;

			case EMIMPACTA_SP_RATE:
				m_sSUM_PASSIVE.m_fSP_RATE += fVAR;
				break;

			case EMIMPACTA_CP_RATE:
				m_sSUM_PASSIVE.m_nCP += (int)fVAR; //add cp
				break;

			case EMIMPACTA_RESIST:
				m_sSUM_PASSIVE.m_sSUMRESIST += (int)fVAR;
				break;

			case EMIMPACTA_CP_GAIN:
				m_sSUM_PASSIVE.m_fCP_GAIN += fVAR; //add cp
				break;

			case EMSPECA_BLOW_DMG:
				m_sSUM_PASSIVE.m_fBlow_Dmg += fVAR; //add blowdmgskill
				break;

			case EMSPECA_CRUSHING_BLOW:
				m_sSUM_PASSIVE.m_fBlow_Rate += fVAR; //add Blowrateskill
				break;

			case EMSPECA_CRIT_RATE:
				m_sSUM_PASSIVE.m_fCrit_Rate += fVAR; //add critrateskill
				break;

			case EMSPECA_CRIT_DMG:
				m_sSUM_PASSIVE.m_fCrit_Dmg += fVAR; //add critdmgskill
				break;
			};

			switch ( pSkill->m_sAPPLY.sSPECS[a].emSPEC )
			{
			case EMSPECA_PIERCE:
				m_sSUM_PASSIVE.m_nPIERCE += int ( sSPEC.fVAR1 );
				break;

			case EMSPECA_TARRANGE:
				m_sSUM_PASSIVE.m_fTARRANGE += sSPEC.fVAR1;
				break;

			case EMSPECA_MOVEVELO:
				m_sSUM_PASSIVE.m_fMOVEVELO += sSPEC.fVAR1;
				break;

			case EMSPECA_ATTACKVELO:
				m_sSUM_PASSIVE.m_fATTVELO += sSPEC.fVAR1;
				break;

			case EMSPECA_GETEXP:
				m_fEXP_RATE += sSPEC.fVAR1;
				break;

			case EMSPECA_SKILLDELAY:
				m_sSUM_PASSIVE.m_fSKILLDELAY += sSPEC.fVAR1;
				break;

			case EMSPECA_ATTACK_INT:
				m_sSUM_PASSIVE.m_nDAMAGE += sSPEC.fVAR1;
				break;

			case EMSPECA_ATTACK_RATE:
				m_sSUM_PASSIVE.m_fDAMAGE_RATE += sSPEC.fVAR1;
				break;

			case EMSPECA_DEF_INT:
				m_sSUM_PASSIVE.m_nDEFENSE += sSPEC.fVAR1;
				break;

			case EMSPECA_DEF_RATE:
				m_sSUM_PASSIVE.m_fDEFENSE_RATE += sSPEC.fVAR1;
				break;

			case EMSPECA_ACCURACY_RATE:
				m_sSUM_PASSIVE.m_nHIT += sSPEC.fVAR1;
				break;

			case EMSPECA_EVASION_RATE:
				m_sSUM_PASSIVE.m_nAVOID += sSPEC.fVAR1;
				break;

			case EMSPECA_IMMUNE:break;
			
			case EMSPECA_PSY_DAMAGE_REDUCE:
				if ( m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReduce < sSPEC.fVAR1 )
					m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReduce = sSPEC.fVAR1;
				break;
			
			case EMSPECA_MAGIC_DAMAGE_REDUCE:
				if ( m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReduce < sSPEC.fVAR1 )
					m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReduce = sSPEC.fVAR1;
				break;
			
			case EMSPECA_PSY_DAMAGE_REFLECTION:
				if ( m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReflection < sSPEC.fVAR1 )
				{
					m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReflection = sSPEC.fVAR1;
					m_sSUM_PASSIVE.m_sDamageSpec.m_fPsyDamageReflectionRate = sSPEC.fVAR2;
				}
				break;
			
			case EMSPECA_MAGIC_DAMAGE_REFLECTION:
				if ( m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReflection < sSPEC.fVAR1 )
				{
					m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReflection = sSPEC.fVAR1;
					m_sSUM_PASSIVE.m_sDamageSpec.m_fMagicDamageReflectionRate = sSPEC.fVAR2;
				}
				break;
			case EMSPECA_DEFENSE_SKILL_ACTIVE:
				break;
			case EMSPECA_STUN:
				m_fATTVELO -= 1.0f;
				m_bSTATE_STUN = true;
				break;
			};
		}
	}
}

void GLCHARLOGIC::INIT_RECOVER ( int nRECOVER )
{
	m_sHP.CHECKMIN ( (m_sHP.dwMax*nRECOVER)/100 + 1 );
	m_sMP.CHECKMIN ( (m_sMP.dwMax*nRECOVER)/100 + 1 );
	m_sSP.CHECKMIN ( (m_sSP.dwMax*nRECOVER)/100 + 1 );

	int i;
	for ( i=0; i<EMBLOW_MULTI; ++i )		DISABLEBLOW ( i );
	for ( i=0; i<SKILLFACT_SIZE; ++i )		DISABLESKEFF ( i );
	m_sPETSKILLFACT.RESET ();
	m_sPETSKILLFACT2.RESET();
}

void GLCHARLOGIC::RELEASE_SLOT_ITEM ( EMSLOT _slot )
{
	m_PutOnItems[_slot] = SITEMCUSTOM ( SNATIVEID(false) );
	
	//	Note : ������ ������ �ʱ�ȭ.
	//
	m_pITEMS[_slot] = NULL;
}

void GLCHARLOGIC::SLOT_ITEM ( const SITEMCUSTOM &sItemCustom, EMSLOT _slot )
{
	m_PutOnItems[_slot] = sItemCustom;

	//	Note : ������ ������ �ʱ�ȭ.
	//
	if ( VALID_SLOT_ITEM(_slot) )
		m_pITEMS[_slot] =  GLItemMan::GetInstance().GetItem ( m_PutOnItems[_slot].sNativeID );
}

void GLCHARLOGIC::HOLD_ITEM ( const SITEMCUSTOM &sItemCustom )
{
	m_PutOnItems[SLOT_HOLD] = sItemCustom;

	//	Note : ������ ������ �ʱ�ȭ.
	//
	if ( VALID_SLOT_ITEM(SLOT_HOLD) )
		m_pITEMS[SLOT_HOLD] =  GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
}

void GLCHARLOGIC::RELEASE_HOLD_ITEM ()
{
	m_PutOnItems[SLOT_HOLD] = SITEMCUSTOM ( SNATIVEID(false) );

	//	Note : ������ ������ �ʱ�ȭ.
	//
	m_pITEMS[SLOT_HOLD] = NULL;
}


//	�޼�, ������ �������߿��� 'BLOW' �Ӽ��� �ִ� �������� ��ȯ.
SITEM* GLCHARLOGIC::GET_ELMT_ITEM ()
{
	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	SITEM* pRHandItem = GET_SLOT_ITEMDATA(emRHand);
	SITEM* pLHandItem = GET_SLOT_ITEMDATA(emLHand);

	if ( pRHandItem && pRHandItem->sSuitOp.sBLOW.emTYPE!=EMBLOW_NONE )
	{
		return pRHandItem;
	}
	else if ( pLHandItem && pLHandItem->sSuitOp.sBLOW.emTYPE!=EMBLOW_NONE )
	{
		return pLHandItem;
	}

	return NULL;
}

BOOL GLCHARLOGIC::INIT_DATA ( BOOL bNEW, BOOL bReGen, float fCONFT_POINT_RATE, bool bInitNowExp )
{
	int i = 0;
	m_CHARINDEX = GETCHARINDEX ();
	const GLCONST_CHARCLASS &cCHARCONST = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];

	//	Note : ������ ������ ��������.
	//
	memset ( m_pITEMS, 0x00, sizeof(SITEM*)*SLOT_TSIZE );
	for ( int i=0; i<SLOT_TSIZE; i++ )
	{
		if ( m_PutOnItems[i].sNativeID != NATIVEID_NULL() )
		{
			m_pITEMS[i] = GLItemMan::GetInstance().GetItem ( m_PutOnItems[i].sNativeID );

			//	������ ���̺��� ���� �������� ����.
			if ( !m_pITEMS[i] )		RELEASE_SLOT_ITEM ( EMSLOT(i) );
		}
	}

	//	Note : ADD �� ���.
	SUM_ADDITION( fCONFT_POINT_RATE );

	if ( bNEW )
	{
		m_bServerStorage = TRUE;

		//m_wStatsPoint = GLCONST_CHAR::wLVL_STATS_P;

		m_sHP.TO_FULL ();
		m_sMP.TO_FULL ();
		m_sSP.TO_FULL ();
	}
	else if ( bReGen )
	{
		INIT_RECOVER ();
	}

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	m_emANISUBTYPE = CHECK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand]  );

	GLITEM_ATT emRHAtt = ITEMATT_NOTHING;
	GLITEM_ATT emLHAtt = ITEMATT_NOTHING;

	if ( m_pITEMS[emRHand] )		emRHAtt = m_pITEMS[emRHand]->sSuitOp.emAttack;
	if ( m_pITEMS[emLHand] )		emLHAtt = m_pITEMS[emLHand]->sSuitOp.emAttack;

	if ( emRHAtt==ITEMATT_NOTHING )		m_wATTRANGE = GLCONST_CHAR::wMAXATRANGE_SHORT;
	else								m_wATTRANGE = m_pITEMS[emRHand]->sSuitOp.wAttRange;

	//	���� �������� ���������� �ִ� ����ġ ���.
	m_sExperience.lnMax = GET_LEVELUP_EXP();
	if( bInitNowExp ) m_sExperience.LIMIT();
	
	return TRUE;
}

BOOL GLCHARLOGIC::CHECKHIT ( const STARGETID &cTargetID, const GLLandMan* pLandMan, const BOOL bLowSP )
{
	int nAVOID = 0;
	EMBRIGHT emBright;

	if ( cTargetID.dwID == EMTARGET_NULL )	return FALSE;

	GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
	if ( !pActor )						return FALSE;
	
	nAVOID = pActor->GetAvoid ();
	emBright = pActor->GetBright ();

	EM_BRIGHT_FB bFB = GLOGICEX::GLSPACEGAP ( GETBRIGHT(), emBright, pLandMan->GETBRIGHT() );	
	int nHitRate = GLOGICEX::GLHITRATE ( GETHIT(), nAVOID, bFB );
	if ( bLowSP )		nHitRate = int(nHitRate*(1.0f-GLCONST_CHAR::fLOWSP_HIT_DROP));

	return ( nHitRate >= (RANDOM_POS*100) );
}

DWORD GLCHARLOGIC::CALCDAMAGE(
	int &rResultDAMAGE,
	const DWORD dwGaeaID,
	const STARGETID &cTargetID,
	const GLLandMan* pLandMan,
	const GLSKILL* pSkill,
	const DWORD dwskill_lev,
	const DWORD dwWeatherFlag,
	const dwDivCount)
{
#if defined(KRT_PARAM) || defined(KR_PARAM) || defined(CH_PARAM) || defined(TH_PARAM) || defined(ID_PARAM) || defined(JP_PARAM) || defined(MY_PARAM) || defined(MYE_PARAM) || defined(PH_PARAM) || defined ( GS_PARAM ) || defined(_RELEASED)
	return CALCDAMAGE_20060328(
		rResultDAMAGE,
		dwGaeaID,
		cTargetID,
		pLandMan,
		pSkill,
		dwskill_lev,
		dwWeatherFlag,
		dwDivCount);
#else
	return CALCDAMAGE_2004(
		rResultDAMAGE,
		dwGaeaID,
		cTargetID,
		pLandMan,
		pSkill,
		dwskill_lev,
		dwWeatherFlag,
		dwDivCount);
#endif
}

// 2006-03-28 Jgkim ��ȹ�� ��û���� ������ ���� ������
DWORD GLCHARLOGIC::CALCDAMAGE_20060328(
	int& rResultDAMAGE,
	const DWORD dwGaeaID,
	const STARGETID &cTargetID,
	const GLLandMan* pLandMan,
	const GLSKILL* pSkill,
	const DWORD dwskill_lev,
	const DWORD dwWeatherFlag,
	const dwDivCount)
{
	GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
	if ( !pActor )	return DAMAGE_TYPE_NONE;

	int nDEFENSE = pActor->GetDefense ();
	int nDEFAULT_DEFENSE = pActor->GetBodyDefense ();
	int nITEM_DEFENSE = pActor->GetItemDefense ();

	int nLEVEL = pActor->GetLevel ();
	float fSTATE_DAMAGE = pActor->GETSTATE_DAMAGE ();
	const SRESIST &sRESIST = pActor->GETRESIST (); // ����� �� ���װ�

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	WORD wGRADE = 0;
	const SITEMCUSTOM &sRHAND = GET_SLOT_ITEM(emRHand);
	if ( sRHAND.sNativeID != SNATIVEID(false) )
		wGRADE = sRHAND.GETGRADE(EMGRINDING_DAMAGE);

	//int nCrushingBlow = (int)( m_sSUMITEM.fIncR_CrushingBlow * 100 );
	int nCrushingBlow = (int)( m_fBlow_Rate );//add blowrate
	
	DWORD dwDamageFlag = DAMAGE_TYPE_NONE;
	bool bShock = false;
	bool bCrushingBlow = false;
	bool bCritical = false; 
	bool bPsyDamage = true;

	DAMAGE_SPEC	sDamageSpec = pActor->GetDamageSpec();
	float fDamageReduce = sDamageSpec.m_fPsyDamageReduce;
	float fDamageReflection = sDamageSpec.m_fPsyDamageReflection;
	float fDamageReflectionRate = sDamageSpec.m_fPsyDamageReflectionRate;

	DEFENSE_SKILL sDefenseSkill = pActor->GetDefenseSkill();
	
	GLPADATA gdDamage = m_gdDAMAGE_SKILL;
	if ( pSkill )
	{
		const SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[dwskill_lev];
		const SKILL::SSPEC &sSKILL_SPEC = pSkill->m_sAPPLY.sSPEC[dwskill_lev];

		//	Note : ��ų ���� ������ �Ҹ�Ǹ�, ������ ����ġ ����.
		//
		if ( sSKILL_DATA.wUSE_CHARMNUM > 0 )
		{
			SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
			if ( pItem )
			{
				GLPADATA &sDATA = pItem->sSuitOp.gdDamage;
				gdDamage.dwLow += sDATA.dwLow;
				gdDamage.dwHigh += sDATA.dwHigh;
			}
		}

		float fRESIST_G = 0;
		switch ( pSkill->m_sBASIC.emAPPLY )
		{
			//	���� ����.
		case SKILL::EMAPPLY_PHY_SHORT:
			gdDamage.dwLow += m_sSUMITEM.gdDamage.dwLow;
			gdDamage.dwMax += m_sSUMITEM.gdDamage.dwMax;
			
			gdDamage.VAR_PARAM ( m_wSUM_PA );
			// ���� ���ݽ� �Ӽ� ����ġ ���� ����
			// default.charset ���� ������
			// fRESIST_PHYSIC_G �� ��
			fRESIST_G = GLCONST_CHAR::fRESIST_PHYSIC_G;
			break;

			//	��Ÿ� ����.
		case SKILL::EMAPPLY_PHY_LONG:
			gdDamage.dwLow += m_sSUMITEM.gdDamage.dwLow;
			gdDamage.dwMax += m_sSUMITEM.gdDamage.dwMax;

			gdDamage.VAR_PARAM ( m_wSUM_SA );
			// ���� ���ݽ� �Ӽ� ����ġ ���� ����
			// default.charset ���� ������
			// fRESIST_PHYSIC_G �� ��
			fRESIST_G = GLCONST_CHAR::fRESIST_PHYSIC_G;
			fDamageReflection = 0.0f;
			fDamageReflectionRate = 0.0f;			
			break;

			//	���� ����.
    	case SKILL::EMAPPLY_MAGIC:
			nDEFENSE = 0;						//	���� �����϶��� �Ϲ� ���� ���õ�.
			nDEFAULT_DEFENSE = 0;				//	���� �����϶��� �Ϲ� ���� ���õ�.
			nITEM_DEFENSE = 0;					//	���� �����϶��� �Ϲ� ���� ���õ�.
			gdDamage.VAR_PARAM ( m_wSUM_MA );
			// ���� ���ݽ� �Ӽ� ����ġ ���� ����
			// default.charset ���� ������
			// fRESIST_G �� ��
			fRESIST_G = GLCONST_CHAR::fRESIST_G;
			fDamageReduce = sDamageSpec.m_fMagicDamageReduce;
			fDamageReflection = sDamageSpec.m_fMagicDamageReflection;
			fDamageReflectionRate = sDamageSpec.m_fMagicDamageReflectionRate;
			bPsyDamage = false;

			break;

		default:
			GASSERT(0&&"CALCDAMAGE() ���� �������� ������� �ʴ� ��ų.");
			break;
		};

		//if ( pSkill->m_sAPPLY.emSPEC == EMSPECA_CRUSHING_BLOW )	nCrushingBlow += (int) ( sSKILL_SPEC.fVAR2 * 100 ) ;

		EMELEMENT emELMT(EMELEMENT_SPIRIT);
		if ( pSkill->m_sAPPLY.emELEMENT==EMELEMENT_ARM )
		{
			SITEM *pITEM = GET_ELMT_ITEM ();
			if ( pITEM )	emELMT = STATE_TO_ELEMENT(pITEM->sSuitOp.sBLOW.emTYPE);
		}
		else
		{
			emELMT = pSkill->m_sAPPLY.emELEMENT;
		}

		short nRESIST = sRESIST.GetElement ( emELMT );
		//if ( nRESIST>99 )	nRESIST = 99;
		if( nRESIST>GLCONST_CHAR::iRESIST_MAX ) nRESIST = GLCONST_CHAR::iRESIST_MAX;
		float fPOWER = GLOGICEX::WEATHER_ELEMENT_POW ( emELMT, dwWeatherFlag, pLandMan->IsWeatherActive() );

		//	��ų�� ������ ��꿡 ������ �ִ� ���� �⺻���� Ư������ �˻�.
		float fSKILL_VAR = sSKILL_DATA.fBASIC_VAR;

		// 2006-03-28 Jgkim��ȹ�� ��û���� �ּ�ó��, ������ ���ĺ����
		// int nVAR = abs ( int(fSKILL_VAR*fPOWER) );
		// nVAR = nVAR - (int) ( nVAR*nRESIST/100.0f*fRESIST_G );
		// if ( nVAR<0 )	nVAR = 0;

		// //	Note : ������� ����ȴ�, ����� �ݿ���.
		// gdDamage.wLow += WORD ( nVAR + (gdDamage.wLow*wGRADE)/GLCONST_CHAR::fDAMAGE_GRADE_K );
		// gdDamage.wHigh += WORD ( nVAR + (gdDamage.wHigh*wGRADE)/GLCONST_CHAR::fDAMAGE_GRADE_K );
		
		
		// ��ų������ = �⺻��ų������*������ ���� �������ݿ���
		int nVAR = abs ( int(fSKILL_VAR*fPOWER) );

		// �ݿ���
		float fGrade = (float) wGRADE / GLCONST_CHAR::fDAMAGE_GRADE_K;

		// �ּ�/�ִ� ������ =  ��ų������ + (�ּ�/�ִ� ������) * �ݿ���
		gdDamage.dwLow  += DWORD (nVAR + ((float) gdDamage.dwLow  * fGrade));
		gdDamage.dwHigh += DWORD (nVAR + ((float) gdDamage.dwHigh * fGrade));
		
		// ��ü���װ� = ����� ���װ� / 100 * �Ӽ�����ġ �������
		float fResistTotal = (float) ((float) nRESIST * 0.01f * fRESIST_G);

		// �ּ�/�ִ� ������ = ������-(������*��ü���װ�)
		gdDamage.dwLow  -= (DWORD) ((float) gdDamage.dwLow  * fResistTotal);
		gdDamage.dwHigh -= (DWORD) ((float) gdDamage.dwHigh * fResistTotal);
		
		// �������� 0 ���� ������ 0 ���� �����Ѵ�.
		if (gdDamage.dwLow <0) gdDamage.dwLow  = 0;
		if (gdDamage.dwHigh<0) gdDamage.dwHigh = 0;
	}
	else
	{
		gdDamage.dwLow += m_sSUMITEM.gdDamage.dwLow;
		gdDamage.dwMax += m_sSUMITEM.gdDamage.dwMax;

		if ( ISLONGRANGE_ARMS() )	gdDamage.VAR_PARAM ( m_wSUM_SA );	//	��Ÿ� ����.
		else						gdDamage.VAR_PARAM ( m_wSUM_PA );	//	���� ����.
	}

	//	���ݷ�. ��ȭ�� �ݿ�.
	gdDamage.dwLow  = DWORD ( gdDamage.dwLow  * m_fDamageRate );
	gdDamage.dwHigh = DWORD ( gdDamage.dwHigh * m_fDamageRate );

	//	Note : �߰� ����ġ ����.
	int nExtFORCE = 0;
	int ndxLvl = nLEVEL - GETLEVEL();
	if ( ndxLvl > 0 )			nExtFORCE = int(RANDOM_POS*ndxLvl/10);

	//	Note : Critical �߻� Ȯ��.
	if ( ndxLvl > 5 )		ndxLvl = 5;
	if ( ndxLvl < -5 )		ndxLvl = -5;

	//	Note : Critical �߻� Ȯ��.
	int nPerHP = ((GETHP()*100)/GETMAXHP());
	if ( nPerHP <= 10 )	nPerHP = 10;
	int nPercentCri = 1000 / nPerHP - 10 + ndxLvl;
	//nPercentCri += (int)( ( m_sSUMITEM.fIncR_Critical + m_fCrit_Rate ) * 100 );
	nPercentCri += (int)( m_fCrit_Rate );//add critrate

	if ( nPercentCri > (int)GLCONST_CHAR::dwCRITICAL_MAX )		nPercentCri = GLCONST_CHAR::dwCRITICAL_MAX;
	if ( nPercentCri < 0 )	nPercentCri = 0;

	//	ũ��Ƽ�� �߻� ���� �Ǵ�.
	if ( nPercentCri > (RANDOM_POS*100) )	bCritical = true;

	//	����Ÿ�� �߻� ���� �Ǵ�
    if ( nCrushingBlow > GLCONST_CHAR::dwCRUSHING_BLOW_MAX )		nCrushingBlow = GLCONST_CHAR::dwCRUSHING_BLOW_MAX;	
	if ( nCrushingBlow < 0 )	nCrushingBlow = 0; //add blowrate
	if ( nCrushingBlow > (RANDOM_POS*100) )	bCrushingBlow = true;

	//	����� �ִ� �ּҰ� ������ ���� ����� ����.
	int  nDAMAGE_NOW = 0;
	nDAMAGE_NOW = int ( gdDamage.dwLow + (gdDamage.dwHigh-gdDamage.dwLow)*RANDOM_POS );
	int nDAMAGE_OLD = ( nDAMAGE_NOW + nExtFORCE );	//	����ġ �ջ�.

	//	���� ���� ����� ����.
	int nNetDAMAGE = int ( nDAMAGE_OLD*(1.0f-GLCONST_CHAR::fLOW_SEED_DAMAGE) - nDEFENSE );

	if ( nNetDAMAGE > 0 )	rResultDAMAGE = int ( nDAMAGE_OLD - nDEFENSE );
	else					rResultDAMAGE = int ( (nDAMAGE_OLD*GLCONST_CHAR::fLOW_SEED_DAMAGE)*RANDOM_POS );
	rResultDAMAGE = int(rResultDAMAGE*fSTATE_DAMAGE);

	//	�� ����� ������ �ݿ�.
	float fRATE = ( 1.0f - nDEFAULT_DEFENSE*nITEM_DEFENSE / GLCONST_CHAR::fDAMAGE_DEC_RATE );
	if ( fRATE > 1.0f )		fRATE = 1.0f;
	if ( fRATE < 0.0f )		fRATE = 0.0f;
	rResultDAMAGE = int(rResultDAMAGE*fRATE);

	if ( bCritical )	rResultDAMAGE = int ( rResultDAMAGE* m_fCrit_Dmg / 100 );

	if ( bCrushingBlow ) rResultDAMAGE = int ( rResultDAMAGE* m_fBlow_Dmg / 100 );

	//	������ ����
	if ( fDamageReduce > 0.0f ) 
	{
		int nDamageReduce = (int) ( ( (rResultDAMAGE * fDamageReduce) * nLEVEL ) / GLCONST_CHAR::wMAX_LEVEL );
		rResultDAMAGE -= nDamageReduce;
		
		if ( bPsyDamage ) dwDamageFlag += DAMAGE_TYPE_PSY_REDUCE;
		else dwDamageFlag += DAMAGE_TYPE_PSY_REDUCE;
	}


	//	������ �ݻ� ���
	if ( fDamageReflectionRate > 0.0f )
	{
		if ( fDamageReflectionRate > (RANDOM_POS*1) )
		{
			int nDamageReflection = (int) ( ( (rResultDAMAGE * fDamageReflection) * nLEVEL ) / GLCONST_CHAR::wMAX_LEVEL );
			
			if ( nDamageReflection > 0 )  
			{
				if ( bPsyDamage ) dwDamageFlag += DAMAGE_TYPE_PSY_REFLECTION;
				else dwDamageFlag += DAMAGE_TYPE_MAGIC_REFLECTION;
				
				// �ǰ��� ���忡�� �������� �ٽ� ������.
				STARGETID sActor(CROW_PC,dwGaeaID);
				pActor->DamageReflectionProc( nDamageReflection, sActor );
			}
		}
	}

	//	�ߵ� ��ų
	if ( sDefenseSkill.m_dwSkillID != NATIVEID_NULL() )
	{
		if ( sDefenseSkill.m_fRate > (RANDOM_POS*1) )
		{
			STARGETID sActor(CROW_PC,dwGaeaID);
			pActor->DefenseSkill( sDefenseSkill.m_dwSkillID, 
								  sDefenseSkill.m_wLevel, 
								  sActor );
		}
	}

	if ( rResultDAMAGE <= 1 )
	{
		bCritical = false;
		bCrushingBlow = false;
		rResultDAMAGE = 1;
	}

	if ( dwDivCount>1 )
	{
		rResultDAMAGE /= dwDivCount;
	}

	bShock = GLOGICEX::CHECKSHOCK ( GETLEVEL(), nLEVEL, rResultDAMAGE, bCritical );

	if ( bShock )			dwDamageFlag += DAMAGE_TYPE_SHOCK;
	if ( bCritical )		dwDamageFlag += DAMAGE_TYPE_CRITICAL;
	if ( bCrushingBlow )	dwDamageFlag += DAMAGE_TYPE_CRUSHING_BLOW;

	return dwDamageFlag;
}

// ������ ������ ����
DWORD GLCHARLOGIC::CALCDAMAGE_2004(
	int& rResultDAMAGE,
	const DWORD dwGaeaID,
	const STARGETID &cTargetID,
	const GLLandMan* pLandMan,
	const GLSKILL* pSkill,
	const DWORD dwskill_lev,
	const DWORD dwWeatherFlag,
	const dwDivCount)
{
	GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget ( pLandMan, cTargetID );
	if ( !pActor )	return DAMAGE_TYPE_NONE;

	int nDEFENSE = pActor->GetDefense ();
	int nDEFAULT_DEFENSE = pActor->GetBodyDefense ();
	int nITEM_DEFENSE = pActor->GetItemDefense ();

	int nLEVEL = pActor->GetLevel ();
	float fSTATE_DAMAGE = pActor->GETSTATE_DAMAGE ();
	const SRESIST &sRESIST = pActor->GETRESIST ();

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	WORD wGRADE = 0;
	const SITEMCUSTOM &sRHAND = GET_SLOT_ITEM(emRHand);
	if ( sRHAND.sNativeID != SNATIVEID(false) )
		wGRADE = sRHAND.GETGRADE(EMGRINDING_DAMAGE);

	//int nCrushingBlow = (int)( m_sSUMITEM.fIncR_CrushingBlow * 100 );
	int nCrushingBlow = (int)( m_fBlow_Rate );

	DWORD dwDamageFlag = DAMAGE_TYPE_NONE;
	bool bShock = false;
	bool bCrushingBlow = false;
	bool bCritical = false; 
	bool bPsyDamage = true;

	DAMAGE_SPEC	sDamageSpec = pActor->GetDamageSpec();
	float fDamageReduce = sDamageSpec.m_fPsyDamageReduce;
	float fDamageReflection = sDamageSpec.m_fPsyDamageReflection;
	float fDamageReflectionRate = sDamageSpec.m_fPsyDamageReflectionRate;

	DEFENSE_SKILL sDefenseSkill = pActor->GetDefenseSkill();

	GLPADATA gdDamage = m_gdDAMAGE_SKILL;
	if ( pSkill )
	{
		const SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[dwskill_lev];
		const SKILL::SSPEC &sSKILL_SPEC = pSkill->m_sAPPLY.sSPEC[dwskill_lev];

		//	Note : ��ų ���� ������ �Ҹ�Ǹ�, ������ ����ġ ����.
		//
		if ( sSKILL_DATA.wUSE_CHARMNUM > 0 )
		{
			SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
			if ( pItem )
			{
				GLPADATA &sDATA = pItem->sSuitOp.gdDamage;
				gdDamage.dwLow += sDATA.dwLow;
				gdDamage.dwHigh += sDATA.dwHigh;
			}
		}

		float fRESIST_G = 0;
		switch ( pSkill->m_sBASIC.emAPPLY )
		{
			//	���� ����.
		case SKILL::EMAPPLY_PHY_SHORT:
			gdDamage.dwLow += m_sSUMITEM.gdDamage.dwLow;
			gdDamage.dwMax += m_sSUMITEM.gdDamage.dwMax;
			
			gdDamage.VAR_PARAM ( m_wSUM_PA );
			fRESIST_G = GLCONST_CHAR::fRESIST_PHYSIC_G;
			break;

			//	��Ÿ� ����.
		case SKILL::EMAPPLY_PHY_LONG:
			gdDamage.dwLow += m_sSUMITEM.gdDamage.dwLow;
			gdDamage.dwMax += m_sSUMITEM.gdDamage.dwMax;

			gdDamage.VAR_PARAM ( m_wSUM_SA );
			fRESIST_G = GLCONST_CHAR::fRESIST_PHYSIC_G;
			
			fDamageReflection = 0.0f;
			fDamageReflectionRate = 0.0f;

			break;

			//	���� ����.
    	case SKILL::EMAPPLY_MAGIC:
			nDEFENSE = 0;						//	���� �����϶��� �Ϲ� ���� ���õ�.
			nDEFAULT_DEFENSE = 0;				//	���� �����϶��� �Ϲ� ���� ���õ�.
			nITEM_DEFENSE = 0;					//	���� �����϶��� �Ϲ� ���� ���õ�.
			gdDamage.VAR_PARAM ( m_wSUM_MA );
			fRESIST_G = GLCONST_CHAR::fRESIST_G;
			
			fDamageReduce = sDamageSpec.m_fMagicDamageReduce;
			fDamageReflection = sDamageSpec.m_fMagicDamageReflection;
			fDamageReflectionRate = sDamageSpec.m_fMagicDamageReflectionRate;
			bPsyDamage = false;
			break;

		default:
			GASSERT(0&&"CALCDAMAGE() ���� �������� ������� �ʴ� ��ų.");
			break;
		};

		//if ( pSkill->m_sAPPLY.emSPEC == EMSPECA_CRUSHING_BLOW )	nCrushingBlow += (int) ( sSKILL_SPEC.fVAR2 * 100 ) ;

		EMELEMENT emELMT(EMELEMENT_SPIRIT);
		if ( pSkill->m_sAPPLY.emELEMENT==EMELEMENT_ARM )
		{
			SITEM *pITEM = GET_ELMT_ITEM ();
			if ( pITEM )	emELMT = STATE_TO_ELEMENT(pITEM->sSuitOp.sBLOW.emTYPE);
		}
		else
		{
			emELMT = pSkill->m_sAPPLY.emELEMENT;
		}

		short nRESIST = sRESIST.GetElement ( emELMT );
		//if ( nRESIST>99 )	nRESIST = 99;
		if( nRESIST>GLCONST_CHAR::iRESIST_MAX ) nRESIST = GLCONST_CHAR::iRESIST_MAX;
		float fPOWER = GLOGICEX::WEATHER_ELEMENT_POW ( emELMT, dwWeatherFlag, pLandMan->IsWeatherActive() );

		//	��ų�� ������ ��꿡 ������ �ִ� ���� �⺻���� Ư������ �˻�.
		float fSKILL_VAR = sSKILL_DATA.fBASIC_VAR;

		int nVAR = abs ( int(fSKILL_VAR*fPOWER) );
		nVAR = nVAR - (int) ( nVAR*nRESIST*0.01f*fRESIST_G );
		if ( nVAR<0 )	nVAR = 0;

		//	Note : ������� ����ȴ�, ����� �ݿ���.
		gdDamage.dwLow += DWORD ( nVAR + (gdDamage.dwLow*wGRADE)/GLCONST_CHAR::fDAMAGE_GRADE_K );
		gdDamage.dwHigh += DWORD ( nVAR + (gdDamage.dwHigh*wGRADE)/GLCONST_CHAR::fDAMAGE_GRADE_K );
	}
	else
	{
		gdDamage.dwLow += m_sSUMITEM.gdDamage.dwLow;
		gdDamage.dwMax += m_sSUMITEM.gdDamage.dwMax;

		if ( ISLONGRANGE_ARMS() )	gdDamage.VAR_PARAM ( m_wSUM_SA );	//	��Ÿ� ����.
		else						gdDamage.VAR_PARAM ( m_wSUM_PA );	//	���� ����.
	}

	//	���ݷ�. ��ȭ�� �ݿ�.
	gdDamage.dwLow = DWORD ( gdDamage.dwLow * m_fDamageRate );
	gdDamage.dwHigh = DWORD ( gdDamage.dwHigh * m_fDamageRate );

	//	Note : �߰� ����ġ ����.
	int nExtFORCE = 0;
	int ndxLvl = nLEVEL - GETLEVEL();
	if ( ndxLvl > 0 )			nExtFORCE = int(RANDOM_POS*ndxLvl*0.1f);

	//	Note : Critical �߻� Ȯ��.
	if ( ndxLvl > 5 )		ndxLvl = 5;
	if ( ndxLvl < -5 )		ndxLvl = -5;

	//	Note : Critical �߻� Ȯ��.
	int nPerHP = ((GETHP()*100)/GETMAXHP());
	if ( nPerHP <= 10 )	nPerHP = 10;
	int nPercentCri = 1000 / nPerHP - 10 + ndxLvl;
	nPercentCri += (int)( m_fCrit_Rate );//add critrate
	if ( nPercentCri > (int)GLCONST_CHAR::dwCRITICAL_MAX )		nPercentCri = GLCONST_CHAR::dwCRITICAL_MAX;
	if ( nPercentCri < 0 )	nPercentCri = 0;

	//	ũ��Ƽ�� �߻� ���� �Ǵ�.
	if ( nPercentCri > (RANDOM_POS*100) )	bCritical = true;

	//	����Ÿ�� �߻� ���� �Ǵ�
    if ( nCrushingBlow > GLCONST_CHAR::dwCRUSHING_BLOW_MAX )		nCrushingBlow = GLCONST_CHAR::dwCRUSHING_BLOW_MAX;	
	if ( nCrushingBlow < 0 )	nCrushingBlow = 0;//add blowrate
	if ( nCrushingBlow > (RANDOM_POS*100) )	bCrushingBlow = true;

	//	����� �ִ� �ּҰ� ������ ���� ����� ����.
	int  nDAMAGE_NOW = 0;
	nDAMAGE_NOW = int ( gdDamage.dwLow + (gdDamage.dwHigh-gdDamage.dwLow)*RANDOM_POS );
	int nDAMAGE_OLD = ( nDAMAGE_NOW + nExtFORCE );	//	����ġ �ջ�.

	//	���� ���� ����� ����.
	int nNetDAMAGE = int ( nDAMAGE_OLD*(1.0f-GLCONST_CHAR::fLOW_SEED_DAMAGE) - nDEFENSE );

	if ( nNetDAMAGE > 0 )	rResultDAMAGE = int ( nDAMAGE_OLD - nDEFENSE );
	else					rResultDAMAGE = int ( (nDAMAGE_OLD*GLCONST_CHAR::fLOW_SEED_DAMAGE)*RANDOM_POS );
	rResultDAMAGE = int(rResultDAMAGE*fSTATE_DAMAGE);

	//	�� ����� ������ �ݿ�.
	float fRATE = ( 1.0f - nDEFAULT_DEFENSE*nITEM_DEFENSE / GLCONST_CHAR::fDAMAGE_DEC_RATE );
	if ( fRATE > 1.0f )		fRATE = 1.0f;
	if ( fRATE < 0.0f )		fRATE = 0.0f;
	rResultDAMAGE = int(rResultDAMAGE*fRATE);

	if ( bCritical )	rResultDAMAGE = int ( rResultDAMAGE* m_fCrit_Dmg / 100 );
	
	if ( bCrushingBlow ) rResultDAMAGE = int ( rResultDAMAGE* m_fBlow_Dmg / 100 );

	//	������ ����
	if ( fDamageReduce > 0.0f ) 
	{
		int nDamageReduce = (int) ( ( (rResultDAMAGE * fDamageReduce) * nLEVEL ) / GLCONST_CHAR::wMAX_LEVEL );
		rResultDAMAGE -= nDamageReduce;
		
		if ( bPsyDamage ) dwDamageFlag += DAMAGE_TYPE_PSY_REDUCE;
		else dwDamageFlag += DAMAGE_TYPE_MAGIC_REDUCE;
	}


	//	������ �ݻ� ���
	if ( fDamageReflectionRate > 0.0f )
	{
		if ( fDamageReflectionRate > (RANDOM_POS*1) )
		{
			int nDamageReflection = (int) ( ( (rResultDAMAGE * fDamageReflection) * nLEVEL ) / GLCONST_CHAR::wMAX_LEVEL );
			
			if ( nDamageReflection > 0 ) 
			{
				if ( bPsyDamage ) dwDamageFlag += DAMAGE_TYPE_PSY_REFLECTION;
				else dwDamageFlag += DAMAGE_TYPE_MAGIC_REFLECTION;		

				// �ǰ��� ���忡�� �������� �ٽ� ������.
				STARGETID sActor(CROW_PC,dwGaeaID);
				pActor->DamageReflectionProc( nDamageReflection, sActor );
			}
		}
	}

	//	�ߵ� ��ų
	if ( sDefenseSkill.m_dwSkillID != NATIVEID_NULL() )
	{
		if ( sDefenseSkill.m_fRate > (RANDOM_POS*1) )
		{
			STARGETID sActor(CROW_PC,dwGaeaID);
			pActor->DefenseSkill( sDefenseSkill.m_dwSkillID, 
								  sDefenseSkill.m_wLevel, 
								  sActor );
		}
	}

	if ( rResultDAMAGE <= 1 )
	{
		bCritical = false;
		bCrushingBlow = false;
		rResultDAMAGE = 1;
	}

	if ( dwDivCount>1 )
	{
		rResultDAMAGE /= dwDivCount;
	}

	bShock = GLOGICEX::CHECKSHOCK ( GETLEVEL(), nLEVEL, rResultDAMAGE, bCritical );

	if ( bShock )			dwDamageFlag += DAMAGE_TYPE_SHOCK;
	if ( bCritical )		dwDamageFlag += DAMAGE_TYPE_CRITICAL;
	if ( bCrushingBlow )	dwDamageFlag += DAMAGE_TYPE_CRUSHING_BLOW;

	return dwDamageFlag;
}

DWORD GLCHARLOGIC::RECEIVE_DAMAGE ( const DWORD wDamage )
{
	DWORD wOLD = m_sHP.dwNow;
	m_sHP.DECREASE ( wDamage );

	return (wOLD>m_sHP.dwNow) ? (wOLD-m_sHP.dwNow) : 0;
}

BOOL GLCHARLOGIC::RECEIVE_QITEMFACT ( const SNATIVEID &nidITEM )
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( nidITEM );
	if ( !pITEM )										return FALSE;
	if ( pITEM->sBasicOp.emItemType!=ITEM_QITEM )		return FALSE;
	if ( pITEM->sQuestionItem.emType==QUESTION_NONE )	return FALSE;

	ITEM::SQUESTIONITEM &sQUESTIONITEM = pITEM->sQuestionItem;

	switch ( sQUESTIONITEM.emType )
	{
	case QUESTION_SPEED_UP:
	case QUESTION_CRAZY:
	case QUESTION_ATTACK_UP:
	case QUESTION_EXP_UP:
	case QUESTION_LUCKY:
	case QUESTION_SPEED_UP_M:
	case QUESTION_MADNESS:
	case QUESTION_ATTACK_UP_M:
		m_sQITEMFACT.emType = sQUESTIONITEM.emType;
		m_sQITEMFACT.fTime = sQUESTIONITEM.fTime;
		m_sQITEMFACT.wParam1 = sQUESTIONITEM.wParam1;
		m_sQITEMFACT.wParam2 = sQUESTIONITEM.wParam2;
		break;
	};

	return TRUE;
}

void GLCHARLOGIC::RECEIVE_EVENTFACT_BEGIN ( const EMGM_EVENT_TYPE emType, const WORD wValue )
{
	m_sEVENTFACT.SetEVENT( emType, wValue );
}

void GLCHARLOGIC::RECEIVE_EVENTFACT_END ( const EMGM_EVENT_TYPE emType )
{
	m_sEVENTFACT.ResetEVENT( emType );
}

//	Note : ? �������� ��ȿ �ð��� ���Ž�Ų��.
//			���� �ڵ�� ��������� false, �׿� ���׿� true
//
bool GLCHARLOGIC::UPDATE_QITEMFACT ( float fElapsedTime )
{
	if( !m_sQITEMFACT.IsACTIVE() )		return true;

	m_sQITEMFACT.fTime -= fElapsedTime;
	if ( m_sQITEMFACT.fTime <= 0 )
	{
		m_sQITEMFACT.RESET();
		return false;
	}

	return true;
}

float GLCHARLOGIC::CHECK_QBOXTIME ( float fElapsedTime )//add qbox
{
	float fRemainingTime = 0.0f;

	if( !m_sQITEMFACT.IsACTIVE() )		return fRemainingTime;

	fRemainingTime = m_sQITEMFACT.fTime -= fElapsedTime;

	if ( m_sQITEMFACT.fTime <= 0 )
	{
		fRemainingTime = 0.0f;
	}

	if ( m_sQITEMFACT.emType == QUESTION_EXP_GET ||
		m_sQITEMFACT.emType == QUESTION_BOMB || 
		m_sQITEMFACT.emType == QUESTION_MOBGEN || 
		m_sQITEMFACT.emType == QUESTION_HEAL )
	{
		fRemainingTime = 0.0f;
	}

	return fRemainingTime;
}

void GLCHARLOGIC::UPDATE_MAX_POINT ( float fCONFT_POINT_RATE )
{
	EMCHARINDEX emCI = GETCHARINDEX ();										//	�ɸ����� ���� �ε���.
	const GLCONST_CHARCLASS &cCHARCONST = GLCONST_CHAR::cCONSTCLASS[emCI];	//	�ɸ����� �����.

	m_sHP.dwMax = DWORD ( m_sSUMSTATS.dwStr*cCHARCONST.fHP_STR + m_sSUMITEM.nHP + m_sSUM_PASSIVE.m_nHP );
	m_sHP.dwMax = DWORD ( m_sHP.dwMax * (1+m_sSUM_PASSIVE.m_fHP_RATE+m_fHP_RATE ) * fCONFT_POINT_RATE );
	if (m_sHP.dwMax > GLCONST_CHAR::m_sHP_MAX ) m_sHP.dwMax = GLCONST_CHAR::m_sHP_MAX;
    m_sHP.LIMIT();

	m_sMP.dwMax = DWORD ( m_sSUMSTATS.dwSpi*cCHARCONST.fMP_SPI + m_sSUMITEM.nMP + m_sSUM_PASSIVE.m_nMP );
	m_sMP.dwMax = DWORD ( m_sMP.dwMax * (1+m_sSUM_PASSIVE.m_fMP_RATE+m_fMP_RATE ) * fCONFT_POINT_RATE );
	if (m_sMP.dwMax > GLCONST_CHAR::m_sMP_MAX ) m_sMP.dwMax = GLCONST_CHAR::m_sMP_MAX;
	m_sMP.LIMIT();

	m_sSP.dwMax = DWORD ( m_sSUMSTATS.dwSta*cCHARCONST.fSP_STA + m_sSUMITEM.nSP + m_sSUM_PASSIVE.m_nSP );
	m_sSP.dwMax = DWORD ( m_sSP.dwMax * (1+m_sSUM_PASSIVE.m_fSP_RATE+m_fSP_RATE ) * fCONFT_POINT_RATE );
	if (m_sSP.dwMax > GLCONST_CHAR::m_sSP_MAX ) m_sSP.dwMax = GLCONST_CHAR::m_sSP_MAX;
	m_sSP.LIMIT();

	m_sCP.dwMax = DWORD ( GLCONST_CHAR::m_sCP_MAX + m_sSUMITEM.nCP + m_sSUM_PASSIVE.m_nCP ); //add cp
	m_sCP.dwMax = DWORD ( m_sCP.dwMax + m_fCP_RATE * fCONFT_POINT_RATE );
	m_sCP.LIMIT();

	m_fCP_GAIN = GLCONST_CHAR::fCP_RATE + m_sSUMITEM.fCP_GAIN + m_sSUM_PASSIVE.m_fCP_GAIN + m_fCP_SKILLGAIN; //add cp

	m_fCrit_Dmg = GLCONST_CHAR::dwCRITICAL_DAMAGE + m_sSUMITEM.fInc_Critical_Dmg + m_sSUM_PASSIVE.m_fCrit_Dmg + m_fCrit_Dmg_Skill; //add critdmg
	m_fBlow_Dmg = GLCONST_CHAR::dwCRUSHING_BLOW_DAMAGE + m_sSUMITEM.fInc_CrushingBlow_Dmg + m_sSUM_PASSIVE.m_fBlow_Dmg + m_fBlow_Dmg_Skill; //add blowdmg
	m_fCrit_Rate = m_sSUM_PASSIVE.m_fCrit_Rate + m_sSUMITEM.fIncR_Critical + m_fCrit_Rate_Skill;//add critrate
	m_fBlow_Rate = m_sSUM_PASSIVE.m_fBlow_Rate + m_sSUMITEM.fIncR_CrushingBlow + m_fBlow_Rate_Skill;//add blowrate

}

void GLCHARLOGIC::UPDATE_DATA ( float fTime, float fElapsedTime, BOOL bClient, float fCONFT_POINT_RATE )
{
	//	���� ��ų ������ ����.
	UPDATESKILLDELAY ( fElapsedTime );

	m_nDEFENSE_SKILL = m_nDEFENSE;
	m_gdDAMAGE_SKILL = m_gdDAMAGE;

	m_nSUM_HIT = m_nHIT;
	m_nSUM_AVOID = m_nAVOID;

	m_nSUM_PIERCE = m_sSUM_PASSIVE.m_nPIERCE;
	m_fSUM_TARRANGE = m_sSUM_PASSIVE.m_fTARRANGE;

	m_bSTATEBLOW = FALSE;
	m_fSTATE_MOVE = 1.0f;

	m_fSTATE_DELAY = 1.0f;

	m_fSTATE_DAMAGE = 1.0f;
	m_fSKILL_MOVE = 0.0f + m_sSUM_PASSIVE.m_fMOVEVELO;

	m_bSTATE_PANT = false;
	m_bSTATE_STUN = false;
	m_bTRANSFORM_EFF = false; //Transform Function 
	m_bINVISIBLE = false;
	m_bRECVISIBLE = false;

	m_dwHOLDBLOW = NULL;

	m_fDamageRate = 1.0f + m_sSUM_PASSIVE.m_fDAMAGE_RATE;
	m_fDefenseRate = 1.0f + m_sSUM_PASSIVE.m_fDEFENSE_RATE;

	m_sSUMRESIST_SKILL = m_sSUMRESIST;

	m_sDamageSpec.RESET();
	m_sDamageSpec = m_sSUM_PASSIVE.m_sDamageSpec;
	m_sDefenseSkill.RESET();

	m_fATTVELO = 0.0f;

	m_fSKILLDELAY = 0.0f;

	m_fEXP_RATE = 1;
	m_fGEN_RATE = 1;

	float fINCR_HP(m_fINCR_HP), fINCR_MP(m_fINCR_MP), fINCR_SP(m_fINCR_SP);

	int nSUM_PA(0), nSUM_SA(0), nSUM_MA(0);

	m_fHP_RATE = 0;
	m_fMP_RATE = 0;
	m_fSP_RATE = 0;
	m_fCP_RATE = 0; //add cp
	m_fCP_SKILLGAIN = 0; //add cp

	m_bSafeZone = false;

	m_bAntiPot = false; //add antipot
	m_bAntiSkill = false; //add antiskill

	m_fCrit_Dmg_Skill = 0;//add critdmgskill
	m_fBlow_Dmg_Skill = 0;//add blowdmgskill
	m_fCrit_Rate_Skill = 0;//add critrateskill
	m_fBlow_Rate_Skill = 0;//add blowrateskill

	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		SSKILLFACT &sSKEFF = m_sSKILLFACT[i];
		if ( sSKEFF.sNATIVEID==NATIVEID_NULL() )	continue;

		sSKEFF.fAGE -= fElapsedTime;

		//	Ŭ���̾�Ʈ�� �ƴ� ��� ���⼭ ��ų ����Ʈ�� ��Ȱ��ȭ, Ŭ���̾�Ʈ�� UpdateSkillEffect()���� ��.
		if ( !bClient && sSKEFF.fAGE <= 0.0f )		DISABLESKEFF(i);
		if ( GLCONST_CHAR::sSkillID_Transform[sSKEFF.sNATIVEID.wMainID][sSKEFF.sNATIVEID.wSubID] != NATIVEID_NULL() )
		{
			if ( GLCONST_CHAR::strTRANSFORM_SKIN[sSKEFF.sNATIVEID.wMainID][sSKEFF.sNATIVEID.wSubID].size() != 0 ) m_bTRANSFORM_EFF = true; //Transform Function 
		}

		switch ( sSKEFF.emTYPE )
		{
		case SKILL::EMFOR_VARHP:	fINCR_HP += sSKEFF.fMVAR;	break;
		case SKILL::EMFOR_VARMP:	fINCR_MP += sSKEFF.fMVAR;	break;
		case SKILL::EMFOR_VARSP:	fINCR_SP += sSKEFF.fMVAR;	break;
		case SKILL::EMFOR_VARAP:
			fINCR_HP += sSKEFF.fMVAR;
			fINCR_MP += sSKEFF.fMVAR;
			fINCR_SP += sSKEFF.fMVAR;
			break;

		case SKILL::EMFOR_DEFENSE:		m_nDEFENSE_SKILL += (int) sSKEFF.fMVAR;				break;
		case SKILL::EMFOR_HITRATE:		m_nSUM_HIT += int(sSKEFF.fMVAR);					break;
		case SKILL::EMFOR_AVOIDRATE:	m_nSUM_AVOID += int(sSKEFF.fMVAR);					break;
		case SKILL::EMFOR_VARDAMAGE:	m_gdDAMAGE_SKILL.VAR_PARAM ( int(sSKEFF.fMVAR) );	break;
		case SKILL::EMFOR_VARDEFENSE:	m_nDEFENSE_SKILL += int(sSKEFF.fMVAR);				break;
		case SKILL::EMFOR_PA:			nSUM_PA += int(sSKEFF.fMVAR);						break;
		case SKILL::EMFOR_SA:			nSUM_SA += int(sSKEFF.fMVAR);						break;
		case SKILL::EMFOR_MA:			nSUM_MA += int(sSKEFF.fMVAR);						break;
		case SKILL::EMFOR_HP_RATE:		m_fHP_RATE += sSKEFF.fMVAR;							break;
		case SKILL::EMFOR_MP_RATE:		m_fMP_RATE += sSKEFF.fMVAR;							break;
		case SKILL::EMFOR_SP_RATE:		m_fSP_RATE += sSKEFF.fMVAR;							break;
		case SKILL::EMFOR_RESIST:		m_sSUMRESIST_SKILL += int(sSKEFF.fMVAR);			break;

		case SKILL::EMFOR_CP_RATE:		m_fCP_RATE += sSKEFF.fMVAR;							break; //add cp
		case SKILL::EMFOR_CP_GAIN:		m_fCP_SKILLGAIN += sSKEFF.fMVAR;					break; //add cp
		};

		for(int _i = 0; _i < SKILL::MAX_LEVEL; _i++ )
		{
			switch ( sSKEFF.emADDON[_i] )
			{
			case EMIMPACTA_HITRATE:			m_nSUM_HIT += int(sSKEFF.fADDON_VAR[_i]);					break;
			case EMIMPACTA_AVOIDRATE:		m_nSUM_AVOID += int(sSKEFF.fADDON_VAR[_i]);					break;
			case EMIMPACTA_DAMAGE:			m_gdDAMAGE_SKILL.VAR_PARAM ( int(sSKEFF.fADDON_VAR[_i]) );	break;
			case EMIMPACTA_DEFENSE:			m_nDEFENSE_SKILL += int(sSKEFF.fADDON_VAR[_i]);				break;
			case EMIMPACTA_VARHP:			fINCR_HP += sSKEFF.fADDON_VAR[_i];							break;
			case EMIMPACTA_VARMP:			fINCR_MP += sSKEFF.fADDON_VAR[_i];							break;
			case EMIMPACTA_VARSP:			fINCR_SP += sSKEFF.fADDON_VAR[_i];							break;
			case EMIMPACTA_VARAP:
				fINCR_HP += sSKEFF.fADDON_VAR[_i];
				fINCR_MP += sSKEFF.fADDON_VAR[_i];
				fINCR_SP += sSKEFF.fADDON_VAR[_i];
				break;

			case EMIMPACTA_DAMAGE_RATE:		m_fDamageRate += sSKEFF.fADDON_VAR[_i];					break;
			case EMIMPACTA_DEFENSE_RATE:	m_fDefenseRate += sSKEFF.fADDON_VAR[_i];				break;		

			case EMIMPACTA_PA:				nSUM_PA += int(sSKEFF.fADDON_VAR[_i]);					break;
			case EMIMPACTA_SA:				nSUM_SA += int(sSKEFF.fADDON_VAR[_i]);					break;
			case EMIMPACTA_MA:				nSUM_MA += int(sSKEFF.fADDON_VAR[_i]);					break;
			case EMIMPACTA_HP_RATE:			m_fHP_RATE += sSKEFF.fADDON_VAR[_i];					break;
			case EMIMPACTA_MP_RATE:			m_fMP_RATE += sSKEFF.fADDON_VAR[_i];					break;
			case EMIMPACTA_SP_RATE:			m_fSP_RATE += sSKEFF.fADDON_VAR[_i];					break;
			case EMIMPACTA_RESIST:			m_sSUMRESIST_SKILL += int(sSKEFF.fADDON_VAR[_i]);		break;

			case EMIMPACTA_CP_RATE:			m_fCP_RATE += sSKEFF.fADDON_VAR[_i];					break; //add cp
			case EMIMPACTA_CP_GAIN:			m_fCP_SKILLGAIN += sSKEFF.fADDON_VAR[_i];				break; //add cp
			};

			switch ( sSKEFF.emSPEC[_i] )
			{
			case EMSPECA_NONBLOW:		m_dwHOLDBLOW = sSKEFF.dwSPECFLAG[_i];			break;
			case EMSPECA_PIERCE:		m_nSUM_PIERCE += int ( sSKEFF.fSPECVAR1[_i] );	break;
			case EMSPECA_TARRANGE:		m_fSUM_TARRANGE += sSKEFF.fSPECVAR1[_i];		break;
			case EMSPECA_MOVEVELO:		m_fSKILL_MOVE += sSKEFF.fSPECVAR1[_i];			break;
			case EMSPECA_INVISIBLE:		m_bINVISIBLE = true;							break;
			case EMSPECA_RECVISIBLE:	m_bRECVISIBLE = true;							break;
			case EMSPECA_ATTACKVELO:	m_fATTVELO += sSKEFF.fSPECVAR1[_i];				break;
			case EMSPECA_IMMUNE:break;

			case EMSPECA_ATTACK_INT:	m_gdDAMAGE_SKILL.VAR_PARAM (int (sSKEFF.fSPECVAR1[_i]));	break;
			case EMSPECA_ATTACK_RATE:	m_fDamageRate += sSKEFF.fSPECVAR1[_i];	break;
			case EMSPECA_DEF_INT:		m_nDEFENSE_SKILL += sSKEFF.fSPECVAR1[_i];	break;
			case EMSPECA_DEF_RATE:		m_fDefenseRate += sSKEFF.fSPECVAR1[_i];	break;
			case EMSPECA_ACCURACY_RATE:	m_nSUM_HIT += sSKEFF.fSPECVAR1[_i];	break;
			case EMSPECA_EVASION_RATE:	m_nSUM_AVOID += sSKEFF.fSPECVAR1[_i];	break;

			//Skill Delay recode 
			case EMSPECA_SKILLDELAY:
				{
					//m_fDelay += (100.0f + (-sSKEFF.fSPECVAR1[_i])) * 0.01f;
					float fDelay = sSKEFF.fSPECVAR1[_i];
					m_fSKILLDELAY += (-fDelay);			
				}
				break;	

			case EMSPECA_GETEXP:			m_fEXP_RATE += sSKEFF.fSPECVAR1[_i];		break;
			case EMSPECA_BLOW_DMG:	m_fBlow_Dmg_Skill += sSKEFF.fSPECVAR1[_i];			break; //add blowdmgskill
			case EMSPECA_CRIT_RATE:	m_fCrit_Rate_Skill += sSKEFF.fSPECVAR1[_i];			break; //add critrateskill
			case EMSPECA_CRIT_DMG:	m_fCrit_Dmg_Skill += sSKEFF.fSPECVAR1[_i];			break; //add critdmgskill
			case EMSPECA_CRUSHING_BLOW:	m_fBlow_Rate_Skill += sSKEFF.fSPECVAR2[_i];		break; //add Blowrateskill
			case EMSPECA_PSY_DAMAGE_REDUCE:
				if ( m_sDamageSpec.m_fPsyDamageReduce < sSKEFF.fSPECVAR1[_i] )
					m_sDamageSpec.m_fPsyDamageReduce = sSKEFF.fSPECVAR1[_i];
				break;
			
			case EMSPECA_MAGIC_DAMAGE_REDUCE:	
				if ( m_sDamageSpec.m_fMagicDamageReduce < sSKEFF.fSPECVAR1[_i] )
					m_sDamageSpec.m_fMagicDamageReduce = sSKEFF.fSPECVAR1[_i];		
				break;
			
			case EMSPECA_PSY_DAMAGE_REFLECTION:	
				if ( m_sDamageSpec.m_fPsyDamageReflection < sSKEFF.fSPECVAR1[_i] )
				{
					m_sDamageSpec.m_fPsyDamageReflection = sSKEFF.fSPECVAR1[_i];		
					m_sDamageSpec.m_fPsyDamageReflectionRate = sSKEFF.fSPECVAR2[_i];		
				}
				break;
			
			case EMSPECA_MAGIC_DAMAGE_REFLECTION:
				if ( m_sDamageSpec.m_fMagicDamageReflection < sSKEFF.fSPECVAR1[_i] )
				{
					m_sDamageSpec.m_fMagicDamageReflection = sSKEFF.fSPECVAR1[_i];	
					m_sDamageSpec.m_fMagicDamageReflectionRate = sSKEFF.fSPECVAR2[_i];
				}
				break;
			
			case EMSPECA_DEFENSE_SKILL_ACTIVE:
				{
					m_sDefenseSkill.m_dwSkillID = sSKEFF.dwNativeID[_i];
					m_sDefenseSkill.m_wLevel = sSKEFF.dwSPECFLAG[_i];
					m_sDefenseSkill.m_fRate = sSKEFF.fSPECVAR1[_i];
				}
				break;

			case EMSPECA_NOPOTION:			m_bAntiPot = true;								break;
			case EMSPECA_NOSKILL:			m_bAntiSkill = true;							break;
			case EMSPECA_STUN:
				m_fATTVELO -= 1.0f;
				m_bSTATE_STUN = true;
				break;
			case EMSPECA_CONTINUEDAMAGE:	
				{
					m_fDamageElapsed += fElapsedTime;
					if ( m_fDamageElapsed > 1.0f ) //Per Second
					{
						if ( sSKEFF.fSPECVAR2[_i] < 0 )
							m_sHP.INCREASE(-sSKEFF.fSPECVAR2[_i] );
						else
							m_sHP.DECREASE( sSKEFF.fSPECVAR2[_i] );
						m_fDamageElapsed = 0.0f;
					}
				}
				break;
			case EMSPECA_CURSE: break;
			//add new skill effects
			/*case EMSPECA_DAMAGE_RATE:
				{
					m_fDamageRate += sSKEFF.fSPECVAR1[_i];
				}
			break;
			case EMSPECA_DEF_INT:
				{
					m_nDEFENSE_SKILL += int(sSKEFF.fSPECVAR1[_i]); 
				}
			break;
			case EMSPECA_DEF_RATE:
				{
					m_fDefenseRate += sSKEFF.fSPECVAR1[_i]; 
				}
			break;
			case EMSPECA_HIT_INT:
			break;
			case EMSPECA_HIT_RATE:
			{
				m_nSUM_HIT += int(sSKEFF.fSPECVAR1[_i]);
			}
			break;
			case EMSPECA_AVOID_INT:
			break;
			case EMSPECA_AVOID_RATE:
			{
				m_nSUM_AVOID += int(sSKEFF.fSPECVAR1[_i]);
			}
			break;

			case EMSPECA_HP_INC:	{m_fHP_RATE += sSKEFF.fSPECVAR1[_i];}break;
			case EMSPECA_MP_INC:	{m_fMP_RATE += sSKEFF.fSPECVAR1[_i];}break;
			case EMSPECA_SP_INC:	{m_fSP_RATE += sSKEFF.fSPECVAR1[_i];}break;
			case EMSPECA_CP_INC:	{m_fCP_RATE += sSKEFF.fSPECVAR1[_i];}break;
			case EMSPECA_HP_REC_RATE:{fINCR_HP += sSKEFF.fSPECVAR1[_i];}break;
			case EMSPECA_MP_REC_RATE:{fINCR_MP  += sSKEFF.fSPECVAR1[_i];}break;
			case EMSPECA_SP_REC_RATE:{fINCR_SP  += sSKEFF.fSPECVAR1[_i];}break;
			case EMSPECA_HPMPSP_REC_RATE:
				{
					fINCR_HP  += sSKEFF.fSPECVAR1[_i];
					fINCR_MP  += sSKEFF.fSPECVAR1[_i];
					fINCR_SP  += sSKEFF.fSPECVAR1[_i];
				}
				break;
			case EMSPECA_RESISTANCE: {m_sSUMRESIST_SKILL += int(sSKEFF.fSPECVAR1[_i]);} break;
			case EMSPECA_MELEE_INT:	{nSUM_PA += int(sSKEFF.fSPECVAR1[_i]);}break;
			case EMSPECA_MISSILE_INT:{nSUM_SA += int(sSKEFF.fSPECVAR1[_i]);}break;
			case EMSPECA_MAGIC_INT:{nSUM_MA += int(sSKEFF.fSPECVAR1[_i]);}break;*/
			};
		}
	}

	for ( i=0; i<EMBLOW_MULTI; ++i )
	{
		SSTATEBLOW &sSTATEBLOW = m_sSTATEBLOWS[i];
		if ( sSTATEBLOW.emBLOW == EMBLOW_NONE )		continue;

		m_bSTATEBLOW = TRUE;
		sSTATEBLOW.fAGE -= fElapsedTime;
		if ( !bClient && sSTATEBLOW.fAGE <= 0.0f )	DISABLEBLOW(i);

		switch ( sSTATEBLOW.emBLOW )
		{
		case EMBLOW_NUMB:
			m_fSTATE_MOVE += sSTATEBLOW.fSTATE_VAR1;			//	�̵��ӵ� ����.
			m_fSTATE_DELAY += sSTATEBLOW.fSTATE_VAR2;			//	������ ����.
			break;

		case EMBLOW_STUN:
			m_fSTATE_MOVE = 0.0f;								//	����.
			m_bSTATE_PANT = true;
			m_bSTATE_STUN = true;
			break;

		case EMBLOW_STONE:
			m_fSTATE_MOVE += sSTATEBLOW.fSTATE_VAR1;			//	�̵��ӵ� ����.
			m_fIncHP += sSTATEBLOW.fSTATE_VAR2*fElapsedTime;
			break;

		case EMBLOW_BURN:
			m_fIncHP += sSTATEBLOW.fSTATE_VAR2*fElapsedTime;
			break;

		case EMBLOW_FROZEN:
			m_fSTATE_MOVE += sSTATEBLOW.fSTATE_VAR1;			//	�̵��ӵ� ����.
			m_fSTATE_DAMAGE += sSTATEBLOW.fSTATE_VAR2;			//	����� ����.
			break;

		case EMBLOW_MAD:
			m_nSUM_HIT = 30;
			m_nSUM_AVOID = 30;
			m_bSTATE_PANT = true;
			break;

		case EMBLOW_POISON:
			m_fIncHP += sSTATEBLOW.fSTATE_VAR2*fElapsedTime;
			break;

		case EMBLOW_CURSE:
			m_fIncHP += sSTATEBLOW.fSTATE_VAR2*fElapsedTime;
			m_fINCR_MP = 0.0f;
			break;
		};
	}

	if ( m_sQITEMFACT.IsACTIVE() )
	{
 		switch ( m_sQITEMFACT.emType )
		{
		case QUESTION_SPEED_UP:		m_fSKILL_MOVE += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_CRAZY:
			m_fSKILL_MOVE += (m_sQITEMFACT.wParam1/100.0f);
			m_fATTVELO += (m_sQITEMFACT.wParam2/100.0f);
			break;
		case QUESTION_ATTACK_UP:	m_fDamageRate += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_EXP_UP:		m_fEXP_RATE += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_LUCKY:		m_fGEN_RATE += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_SPEED_UP_M:	m_fSKILL_MOVE += (m_sQITEMFACT.wParam1/100.0f);		break;
		case QUESTION_MADNESS:
			m_fSKILL_MOVE += (m_sQITEMFACT.wParam1/100.0f);
			m_fATTVELO += (m_sQITEMFACT.wParam2/100.0f);
			break;
		case QUESTION_ATTACK_UP_M:	m_fDamageRate += (m_sQITEMFACT.wParam1/100.0f);		break;
		};
	}

	//	Memo :	? ������ �̺�Ʈ ����
	if( m_sEVENTFACT.IsACTIVE( EMGM_EVENT_SPEED ) )		m_fSKILL_MOVE += (m_sEVENTFACT.wSpeed/100.0f);
	if( m_sEVENTFACT.IsACTIVE( EMGM_EVENT_ASPEED ) )	m_fATTVELO += (m_sEVENTFACT.wASpeed/100.0f);
	if( m_sEVENTFACT.IsACTIVE( EMGM_EVENT_ATTACK ) )	m_fDamageRate += (m_sEVENTFACT.wAttack/100.0f);
	if( m_bBTGactivated )								m_fDamageRate = ( m_fDamageRate * GetBtgAddAttack() );//add btg attack


	// ���� ������ų ó��
	if ( m_sPETSKILLFACT.sNATIVEID !=NATIVEID_NULL() )
	{
		switch ( m_sPETSKILLFACT.emTYPE )
		{
		case SKILL::EMFOR_PET_HEAL:				//	������ HP ȸ�� �ӵ� ���
			fINCR_HP += m_sPETSKILLFACT.fMVAR;
			break;
		case SKILL::EMFOR_PET_SUPPROT:			//	������ HP, MP, SP ȸ���� ���
			fINCR_HP += m_sPETSKILLFACT.fMVAR;
			fINCR_MP += m_sPETSKILLFACT.fMVAR;
			fINCR_SP += m_sPETSKILLFACT.fMVAR;
			break;
		case SKILL::EMFOR_PET_BACKUP_ATK:		//	������ ���ݷ�(��) ���
			m_fDamageRate += m_sPETSKILLFACT.fMVAR/100.0f;
			break;
		case SKILL::EMFOR_PET_BACKUP_DEF:		//	������ ����(��) ���
			m_fDefenseRate += m_sPETSKILLFACT.fMVAR/100.0f;
			break;
		case SKILL::EMFOR_PET_BACKUP_HITRATE:
			m_nSUM_HIT += int(m_sPETSKILLFACT.fMVAR/100.0f);
			break;
		case SKILL::EMFOR_PET_BACKUP_AVOID:
			m_nSUM_AVOID += int(m_sPETSKILLFACT2.fMVAR/100.0f);
			break;
		};

		// ��ų ������Ÿ�� ����
		m_sPETSKILLFACT.fAGE -= fElapsedTime;
	}

	if ( m_sPETSKILLFACT2.sNATIVEID !=NATIVEID_NULL() )
	{
		switch ( m_sPETSKILLFACT2.emTYPE )
		{
		case SKILL::EMFOR_PET_HEAL:				//	������ HP ȸ�� �ӵ� ���
			fINCR_HP += m_sPETSKILLFACT2.fMVAR;
			break;
		case SKILL::EMFOR_PET_SUPPROT:			//	������ HP, MP, SP ȸ���� ���
			fINCR_HP += m_sPETSKILLFACT2.fMVAR;
			fINCR_MP += m_sPETSKILLFACT2.fMVAR;
			fINCR_SP += m_sPETSKILLFACT2.fMVAR;
			break;
		case SKILL::EMFOR_PET_BACKUP_ATK:		//	������ ���ݷ�(��) ���
			m_fDamageRate += m_sPETSKILLFACT2.fMVAR/100.0f;
			break;
		case SKILL::EMFOR_PET_BACKUP_DEF:		//	������ ����(��) ���
			m_fDefenseRate += m_sPETSKILLFACT2.fMVAR/100.0f;
			break;
		case SKILL::EMFOR_PET_BACKUP_HITRATE:
			m_nSUM_HIT += int(m_sPETSKILLFACT.fMVAR/100.0f);
			break;
		case SKILL::EMFOR_PET_BACKUP_AVOID:
			m_nSUM_AVOID += int(m_sPETSKILLFACT2.fMVAR/100.0f);
			break;
		};

		// ��ų ������Ÿ�� ����
		m_sPETSKILLFACT2.fAGE -= fElapsedTime;
	}

	// ���� ȿ�� ó��
	for( int i = 0; i < EMLANDEFFECT_MULTI; i++ )
	{
		SLANDEFFECT landEffect = m_sLandEffect[i];
		if( !landEffect.IsUse() ) continue;

		switch( landEffect.emLandEffectType ) 
		{
		case EMLANDEFFECT_ATK_SPEED: // ���ݼӵ�
			m_fATTVELO	  += landEffect.fValue;
			break;
		case EMLANDEFFECT_MOVE_SPEED: // �̵��ӵ�
			m_fSKILL_MOVE += landEffect.fValue;
			break;
		case EMLANDEFFECT_MP_RATE:	  // HP ��ȭ��
			m_fHP_RATE    += landEffect.fValue;
			break;
		case EMLANDEFFECT_HP_RATE:    // MP ��ȭ��
			m_fMP_RATE    += landEffect.fValue;
			break;
		case EMLANDEFFECT_RECOVER_RATE: // ȸ����
			fINCR_HP	  += landEffect.fValue;
			fINCR_MP	  += landEffect.fValue;
			fINCR_SP	  += landEffect.fValue;
			break;
		case EMLANDEFFECT_DAMAGE_RATE:	// ����ġ ��ȭ��
			m_fDamageRate += landEffect.fValue;
			break;
		case EMLANDEFFECT_DEFENSE_RATE:	// ���ġ ��ȭ��
			m_fDefenseRate += landEffect.fValue;
			break;
		case EMLANDEFFECT_RESIST_RATE:	// ���� ��ġ			
			m_sSUM_PASSIVE.m_sSUMRESIST.nFire += (int) landEffect.fValue;
			m_sSUM_PASSIVE.m_sSUMRESIST.nIce += (int) landEffect.fValue;
			m_sSUM_PASSIVE.m_sSUMRESIST.nElectric += (int) landEffect.fValue;
			m_sSUM_PASSIVE.m_sSUMRESIST.nPoison += (int) landEffect.fValue;
			m_sSUM_PASSIVE.m_sSUMRESIST.nSpirit += (int) landEffect.fValue;
			break;
		case EMLANDEFFECT_CANCEL_ALLBUFF: // ��� ���� ���
//			for ( i=0; i<EMBLOW_MULTI; ++i )		DISABLEBLOW ( i );
//			for ( i=0; i<SKILLFACT_SIZE; ++i )		DISABLESKEFF ( i );
			break;
		case EMLANDEFFECT_SAFE_ZONE:
			m_bSafeZone = true;
			break;
		}
	}


	//	Note : HP, MP, SP �ѷ� Ȯ��.
	//
	UPDATE_MAX_POINT ( fCONFT_POINT_RATE );

	//	����ġ ���ġ ����ġ ����.
	m_wSUM_PA = m_wPA + nSUM_PA;
	m_wSUM_SA = m_wSA + nSUM_SA;
	m_wSUM_MA = m_wMA + nSUM_MA;

	//	����. ��ȭ�� �ݿ�.
	m_nDEFENSE_SKILL = int ( m_nDEFENSE_SKILL * m_fDefenseRate );
	if ( m_nDEFENSE_SKILL < 0 )			m_nDEFENSE_SKILL = 1;

	m_sSUMRESIST_SKILL.LIMIT();

	//	���� �ӵ�. ( ���� skill �Ӽ� ����� �κп� state, passive skill �Ӽ� ����. )
	m_fATTVELO += m_fSTATE_MOVE + m_sSUM_PASSIVE.m_fATTVELO;

	m_fSTATE_DELAY += m_fSKILLDELAY + m_sSUM_PASSIVE.m_fSKILLDELAY;

	//	���� ���ɰŸ�.
	m_wSUM_ATTRANGE = m_wATTRANGE;

	//	MP �������� ������.
	if ( (m_fINCR_MP==0.0f) && (m_fIncMP>0) )		m_fIncMP = 0;





	//	��ų ȿ���� ����� ������ ����.
	m_gdDAMAGE_PHYSIC = m_gdDAMAGE_SKILL;
	m_gdDAMAGE_PHYSIC.dwLow += m_sSUMITEM.gdDamage.dwLow;
	m_gdDAMAGE_PHYSIC.dwMax += m_sSUMITEM.gdDamage.dwMax;

	if ( ISLONGRANGE_ARMS() )	m_gdDAMAGE_PHYSIC.VAR_PARAM ( m_wSUM_SA );	//	��Ÿ� ����.
	else						m_gdDAMAGE_PHYSIC.VAR_PARAM ( m_wSUM_PA );	//	���� ����.

	//	���ݷ�. ��ȭ�� �ݿ�.
	m_gdDAMAGE_PHYSIC.dwLow = DWORD ( m_gdDAMAGE_PHYSIC.dwLow * m_fDamageRate );
	m_gdDAMAGE_PHYSIC.dwHigh = DWORD ( m_gdDAMAGE_PHYSIC.dwHigh * m_fDamageRate );

	//	Note : ü�� ��ȭ.
	//
	float fElap = (fElapsedTime/GLCONST_CHAR::fUNIT_TIME);
	float fINC_HP = fElap* ( m_sHP.dwMax*fINCR_HP + GLCONST_CHAR::fHP_INC + m_sSUMITEM.fInc_HP );
	float fINC_MP = fElap* ( m_sMP.dwMax*fINCR_MP + GLCONST_CHAR::fMP_INC + m_sSUMITEM.fInc_MP );
	float fINC_SP = fElap* ( m_sSP.dwMax*fINCR_SP + GLCONST_CHAR::fSP_INC + m_sSUMITEM.fInc_SP );

	GLOGICEX::UPDATE_POINT ( m_sHP, m_fIncHP, fINC_HP, 1 );
	GLOGICEX::UPDATE_POINT ( m_sMP, m_fIncMP, fINC_MP, 0 );
	GLOGICEX::UPDATE_POINT ( m_sSP, m_fIncSP, fINC_SP, 0 );

	//add pk streak
	m_fPkTimer += fElapsedTime;
	if(m_fPkTimer >= 10.0f) 
	{
		m_fPkTimer = 0.0f;
		m_dwPkCounter = 0;
	}
	//add afk reward
	m_fGameTimer += fElapsedTime;
	m_fPremTimer += fElapsedTime;
	if( m_fGameTimer >= GLCONST_CHAR::fVoteTimer && GLCONST_CHAR::wVoteReward != 0)
	{
		m_fGameTimer = 30.0f;
		m_lnVoteP += GLCONST_CHAR::wVoteReward;
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DARKORANGE, ID2GAMEINTEXT("INC_WAR_POINTS"), GLCONST_CHAR::wVoteReward );
	}
	if( m_fPremTimer >= GLCONST_CHAR::fPremTimer && GLCONST_CHAR::wPremReward != 0 )
	{
		m_fPremTimer = 0.0f;
		m_lnPremP += GLCONST_CHAR::wPremReward;
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DARKORANGE, ID2GAMEINTEXT("INC_PREM_POINTS"), GLCONST_CHAR::wPremReward );
	}
}

float GLCHARLOGIC::GETATTVELO ()
{
	//	return m_fATTVELO<0.0f?0.0f:m_fATTVELO;
	float fATTVELO = m_fATTVELO + m_sSUMITEM.fIncR_AtkSpeed;
	return fATTVELO<0.0f?0.0f:fATTVELO;
}

float GLCHARLOGIC::GetBtgAddAttack () //add btg attack
{
	return m_fBTGAttackRate;
}

void GLCHARLOGIC::SetBtgAddAttack ( BOOL bEnabled ,float AddAttackRate )
{
	if (bEnabled)
	{
		m_bBTGactivated = true;
		m_fBTGAttackRate = AddAttackRate ;

		if ( m_fBTGAttackRate <= 1.0 )
		{
			m_fBTGAttackRate = 1.0;
		}
	}else{
		m_bBTGactivated = false;
		m_fBTGAttackRate = 1.0 ;
	}

}

float GLCHARLOGIC::GETMOVEVELO ()
{
	float fMOVE = m_fSTATE_MOVE + m_fSKILL_MOVE + m_fOPTION_MOVE + m_sSUMITEM.fIncR_MoveSpeed;
	return fMOVE<0.0f?0.0f:fMOVE;
}

float GLCHARLOGIC::GETATT_ITEM ()
{
	float fATTVELO = m_sSUMITEM.fInc_AtkSpeed / 100;
	return fATTVELO;
}
float GLCHARLOGIC::GETMOVE_ITEM ()
{
	float fMOVE = ( m_sSUMITEM.fInc_MoveSpeed / GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX].fRUNVELO);
	return fMOVE<0.0f?0.0f:fMOVE;
}

//	�ܼ��� ������ġ ������ �˻�.
BOOL GLCHARLOGIC::CHECKSLOT_ITEM ( SNATIVEID sNativeID, EMSLOT emSlot )
{
	if ( sNativeID==NATIVEID_NULL() )		return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )	return FALSE;

	const SITEM &sItem = *pItem;

	if( sItem.sBasicOp.emItemType!=ITEM_SUIT && 
		sItem.sBasicOp.emItemType!=ITEM_ARROW && 
		//add bullet
		sItem.sBasicOp.emItemType!=ITEM_BULLET && 

		sItem.sBasicOp.emItemType!=ITEM_CHARM && 
		sItem.sBasicOp.emItemType!=ITEM_ANTI_DISAPPEAR &&
		sItem.sBasicOp.emItemType!=ITEM_REVIVE && 
		sItem.sBasicOp.emItemType!=ITEM_VEHICLE )
		return FALSE;

	//	�ش� ���Կ� ���������� SUIT����.
	EMSUIT emSuit= SLOT_2_SUIT ( emSlot );
	if ( sItem.sSuitOp.emSuit != emSuit )	return FALSE;

	if ( sItem.sBasicOp.emItemType == ITEM_REVIVE ||
		 sItem.sBasicOp.emItemType == ITEM_ANTI_DISAPPEAR )
	{
		if ( emSlot != SLOT_NECK )
			return FALSE;
	}

	if ( sItem.sBasicOp.emItemType == ITEM_VEHICLE )
	{
		if ( emSlot != SLOT_VEHICLE ) return FALSE;
	}

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();
	
	if ( sItem.sBasicOp.emItemType==ITEM_ARROW || sItem.sBasicOp.emItemType==ITEM_CHARM )
	{
		if ( emSlot!=emLHand )
			return FALSE;
	}

	//add bullet
	if ( sItem.sBasicOp.emItemType==ITEM_BULLET )
	{
		if ( emSlot!=emLHand )
			return FALSE;
	}

	//	�տ� ��� ������ �Ѽտ��϶�.
	if ( emSuit==SUIT_HANDHELD )
	{
		//	��� �����.
		//
		if ( sItem.sSuitOp.IsBOTHHAND() )
		{
			if ( emSlot!=emRHand )											
				return FALSE;
		}
		//	�Ѽ� �����.
		//
		else
		{
			if ( sItem.sSuitOp.emHand!=HAND_BOTH )	//	��� ��� �����ϸ� ���� ����.
			{
				if ( sItem.sSuitOp.emHand==HAND_RIGHT && emSlot!=emRHand )	return FALSE;
				if ( sItem.sSuitOp.emHand==HAND_LEFT && emSlot!=emLHand )	return FALSE;
			}

			if ( sItem.sBasicOp.emItemType==ITEM_ARROW )
			{
				//	ȭ�� ����ÿ��� �����տ� Ȱ�� ������ ���� �ʴٸ� ���� �Ұ���.
				if ( !m_pITEMS[emRHand] || m_pITEMS[emRHand]->sSuitOp.emAttack!=ITEMATT_BOW )	
					return FALSE;	
			}
			//add bullet
			else if ( sItem.sBasicOp.emItemType==ITEM_BULLET )
			{
				//	���� ����ÿ��� �����տ� â�� ������ ���� �ʴٸ� ���� �Ұ���.
				if ( !m_pITEMS[emRHand] ) return FALSE;	
				if (m_pITEMS[emRHand]->sSuitOp.emAttack ==ITEMATT_DUAL_GUN ) return TRUE;
				else if (m_pITEMS[emRHand]->sSuitOp.emAttack ==ITEMATT_RAIL_GUN ) return TRUE;
				else if (m_pITEMS[emRHand]->sSuitOp.emAttack ==ITEMATT_PORTAL_GUN ) return TRUE;
				else return FALSE;	 
					
			}
			//
			else if ( sItem.sBasicOp.emItemType==ITEM_CHARM )
			{
				//	���� ����ÿ��� �����տ� â�� ������ ���� �ʴٸ� ���� �Ұ���.
				if ( !m_pITEMS[emRHand] || m_pITEMS[emRHand]->sSuitOp.emAttack!=ITEMATT_SPEAR )	
					return FALSE;
			}
		}
	}

	return TRUE;
}

//	�������� ������ �񿴴��� �˻��Ѵ�.
//	��, Ȱ�� ��� ���������� �޼տ� ȭ���� �����Ҽ� �ִ�.
BOOL GLCHARLOGIC::ISEMPTY_SLOT ( SNATIVEID sNativeID, EMSLOT emSlot )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )	return FALSE;

	EMSUIT emSuit= SLOT_2_SUIT ( emSlot );

	BOOL bEmptySlot = TRUE;
	if ( pItem->sSuitOp.emSuit==SUIT_HANDHELD )
	{
		EMSLOT emRHand = GetCurRHand();
		EMSLOT emLHand = GetCurLHand();

		//	�������� �ϴ� �������� ��յ����� ��.
		if ( pItem->sSuitOp.IsBOTHHAND() )
		{
			if ( m_pITEMS[emRHand] || m_pITEMS[emLHand] )
			{
				bEmptySlot = FALSE;
			}

			//	���� �������°� Ȱ�̶��. �޼տ� ȭ���� �־ ��. �� �������� ��� �־��.
			if ( pItem->sSuitOp.emAttack==ITEMATT_BOW )
			{
				if ( !m_pITEMS[emRHand] && m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emLHand]->sBasicOp.emItemType==ITEM_ARROW )
					{
						bEmptySlot = TRUE;
					}
				}
			}
			//add bullet
			if ( pItem->sSuitOp.emAttack==ITEMATT_DUAL_GUN )
			{
				if ( !m_pITEMS[emRHand] && m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emLHand]->sBasicOp.emItemType==ITEM_BULLET )
					{
						bEmptySlot = TRUE;
					}
				}
			}
			if ( pItem->sSuitOp.emAttack==ITEMATT_RAIL_GUN )
			{
				if ( !m_pITEMS[emRHand] && m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emLHand]->sBasicOp.emItemType==ITEM_BULLET )
					{
						bEmptySlot = TRUE;
					}
				}
			}
			if ( pItem->sSuitOp.emAttack==ITEMATT_PORTAL_GUN )
			{
				if ( !m_pITEMS[emRHand] && m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emLHand]->sBasicOp.emItemType==ITEM_BULLET )
					{
						bEmptySlot = TRUE;
					}
				}
			}
			//
			//	�������� ���� â �����϶� �޼տ� ������ �־ ��.
			if ( pItem->sSuitOp.emAttack==ITEMATT_SPEAR )
			{
				if ( !m_pITEMS[emRHand] && m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emLHand]->sBasicOp.emItemType==ITEM_CHARM )
					{
						bEmptySlot = TRUE;
					}
				}
			}
		}
		//	�������� �������� �Ѽյ����� ��.
		else
		{
			//	���� ���Ⱑ ����϶�.
			if ( m_pITEMS[emRHand] && m_pITEMS[emRHand]->sSuitOp.IsBOTHHAND() )
			{
				bEmptySlot = FALSE;
			}
			//	���� ���Ⱑ �Ѽ��϶�.
			else
			{
				if ( m_pITEMS[emSlot] )		bEmptySlot = FALSE;
			}

			//	�������� �������� ȭ���϶�.
			if ( pItem->sBasicOp.emItemType==ITEM_ARROW )
			{
				bEmptySlot = FALSE;
				if ( m_pITEMS[emRHand] && !m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_BOW )			bEmptySlot = TRUE;		
				}
			}
			//add bullet
			if ( pItem->sBasicOp.emItemType==ITEM_BULLET )
			{
				bEmptySlot = FALSE;
				if ( m_pITEMS[emRHand] && !m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_DUAL_GUN ||
						m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_RAIL_GUN||
						m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_PORTAL_GUN)		
						bEmptySlot = TRUE;
				}
			}
			//

			//	�������� �������� �����϶�.
			if ( pItem->sBasicOp.emItemType==ITEM_CHARM )
			{
				bEmptySlot = FALSE;
				if ( m_pITEMS[emRHand] && !m_pITEMS[emLHand] )
				{
					if ( m_pITEMS[emRHand]->sSuitOp.emAttack==ITEMATT_SPEAR )		bEmptySlot = TRUE;
				}
			}
		}
	}
	else if ( m_pITEMS[emSlot] )			bEmptySlot = FALSE;

	return bEmptySlot;
}

BOOL GLCHARLOGIC::ACCEPT_ITEM ( SNATIVEID sNativeID )
{
	if ( sNativeID==NATIVEID_NULL() )							return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )											return FALSE;

	const SITEM &sItem = *pItem;
	BOOL bArmor = (( sItem.sSuitOp.emSuit==SUIT_HANDHELD )&&sItem.sBasicOp.emItemType==ITEM_SUIT);
	WORD wAPT = bArmor ? (WORD) GLCONST_CHAR::dwACCEPT_LOWERSTATS : 0;

	if ( !(sItem.sBasicOp.dwReqCharClass&m_emClass) )			return FALSE;
	if ( !(sItem.sBasicOp.dwReqSchool&index2school(m_wSchool)) ) return FALSE;

	if ( sItem.sBasicOp.emReqBright!=BRIGHT_BOTH )
	{
		if ( GETBRIGHT()!=sItem.sBasicOp.emReqBright )			return FALSE;
	}

	if ( m_wSUM_PA < sItem.sBasicOp.wReqPA )					return FALSE;
	if ( m_wSUM_SA < sItem.sBasicOp.wReqSA )					return FALSE;
	//add reborn req
	if ( m_dwReborn < sItem.sBasicOp.dwRebornReq )					return FALSE;

	if ( m_wLevel+wAPT < sItem.sBasicOp.wReqLevelDW )			return FALSE;
	if ( sItem.sBasicOp.wReqLevelUP && m_wLevel > sItem.sBasicOp.wReqLevelUP )	return FALSE;
	if ( !m_sSUMSTATS.CHECK_REQ ( sItem.sBasicOp.sReqStats, wAPT ) )		return FALSE;
	
	if ( pItem->sSkillBookOp.sSkill_ID!=NATIVEID_NULL() )
	{
		if ( CHECKLEARNABLE_SKILL ( pItem->sSkillBookOp.sSkill_ID )!=EMSKILL_LEARN_OK )		return FALSE;
	}

	return TRUE;
}


BOOL GLCHARLOGIC::SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID )
{
	if ( sNativeID==NATIVEID_NULL() )							return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )											return FALSE;

	const SITEM &sItem = *pItem;
	BOOL bArmor = (( sItem.sSuitOp.emSuit==SUIT_HANDHELD )&&sItem.sBasicOp.emItemType==ITEM_SUIT);
	WORD wAPT = bArmor ? (WORD) GLCONST_CHAR::dwACCEPT_LOWERSTATS : 0;    

	if ( !(sItem.sBasicOp.dwReqCharClass&m_emClass) )			return FALSE;
	if ( !(sItem.sBasicOp.dwReqSchool&index2school(m_wSchool)) ) return FALSE;

	if ( m_wLevel+wAPT < sItem.sBasicOp.wReqLevelDW )			return FALSE;
	if ( sItem.sBasicOp.wReqLevelUP && m_wLevel > sItem.sBasicOp.wReqLevelUP )	return FALSE;	

	return TRUE;
}

BOOL GLCHARLOGIC::SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID )
{
	if ( sNativeID==NATIVEID_NULL() )							return FALSE;
	if ( sDisguiseID==NATIVEID_NULL() )							return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )											return FALSE;

	SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sDisguiseID );
	if ( pItem==NULL )											return FALSE;

	const SITEM &sItem = *pItem;
	const SITEM &sItemDisguise = *pItemDisguise;
	BOOL bArmor = (( sItem.sSuitOp.emSuit==SUIT_HANDHELD )&&sItem.sBasicOp.emItemType==ITEM_SUIT);
	WORD wAPT = bArmor ? (WORD) GLCONST_CHAR::dwACCEPT_LOWERSTATS : 0;

	//	Memo : �ڽ�Ƭ�� ���� �ڰݿ���� üũ�Ѵ�.
	//		�� ���� �������� ���밡�� �����̴�.
	if( !(sItem.sBasicOp.dwReqCharClass&m_emClass) ||
		!(sItemDisguise.sBasicOp.dwReqCharClass&m_emClass) )	return FALSE;
	if ( !(sItem.sBasicOp.dwReqSchool&index2school(m_wSchool)) ) return FALSE;

	if ( m_wLevel+wAPT < sItem.sBasicOp.wReqLevelDW )				return FALSE;
	if ( sItem.sBasicOp.wReqLevelUP && m_wLevel > sItem.sBasicOp.wReqLevelUP )					return FALSE;

	return TRUE;
}

BOOL GLCHARLOGIC::ACCEPT_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID )
{
	if ( sNativeID==NATIVEID_NULL() )							return FALSE;
	if ( sDisguiseID==NATIVEID_NULL() )							return FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pItem==NULL )											return FALSE;

	SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sDisguiseID );
	if ( pItem==NULL )											return FALSE;

	const SITEM &sItem = *pItem;
	const SITEM &sItemDisguise = *pItemDisguise;
	BOOL bArmor = (( sItem.sSuitOp.emSuit==SUIT_HANDHELD )&&sItem.sBasicOp.emItemType==ITEM_SUIT);
	WORD wAPT = bArmor ? (WORD) GLCONST_CHAR::dwACCEPT_LOWERSTATS : 0;

	//	Memo : �ڽ�Ƭ�� ���� �ڰݿ���� üũ�Ѵ�.
	//		�� ���� �������� ���밡�� �����̴�.
	if( !(sItem.sBasicOp.dwReqCharClass&m_emClass) ||
		!(sItemDisguise.sBasicOp.dwReqCharClass&m_emClass) )	return FALSE;
	if ( !(sItem.sBasicOp.dwReqSchool&index2school(m_wSchool)) ) return FALSE;

	if ( sItem.sBasicOp.emReqBright!=BRIGHT_BOTH )
	{
		if ( GETBRIGHT()!=sItem.sBasicOp.emReqBright )			return FALSE;
	}

	if ( m_wSUM_PA < sItem.sBasicOp.wReqPA )					return FALSE;
	if ( m_wSUM_SA < sItem.sBasicOp.wReqSA )					return FALSE;
	//add reborn req
	if ( m_dwReborn < sItem.sBasicOp.dwRebornReq )					return FALSE;

	if ( m_wLevel+wAPT < sItem.sBasicOp.wReqLevelDW )				return FALSE;
	if ( sItem.sBasicOp.wReqLevelUP && m_wLevel > sItem.sBasicOp.wReqLevelUP )					return FALSE;
	if ( !m_sSUMSTATS.CHECK_REQ ( sItem.sBasicOp.sReqStats, wAPT ) )		
		return FALSE;

	if ( pItem->sSkillBookOp.sSkill_ID!=NATIVEID_NULL() )
	{
		if ( CHECKLEARNABLE_SKILL ( pItem->sSkillBookOp.sSkill_ID )!=EMSKILL_LEARN_OK )		
			return FALSE;
	}

	return TRUE;
}

DWORD GLCHARLOGIC::CALC_ACCEPTP ( SNATIVEID sNativeID )
{
	DWORD dwATP = 0;
	if ( sNativeID==NATIVEID_NULL() )							return dwATP;
	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );
	if ( pITEM==NULL )											return dwATP;

	if ( m_sSUMSTATS.dwPow < pITEM->sBasicOp.sReqStats.dwPow )		dwATP += pITEM->sBasicOp.sReqStats.dwPow - m_sSUMSTATS.dwPow;
	if ( m_sSUMSTATS.dwStr < pITEM->sBasicOp.sReqStats.dwStr )		dwATP += pITEM->sBasicOp.sReqStats.dwStr - m_sSUMSTATS.dwStr;
	if ( m_sSUMSTATS.dwSpi < pITEM->sBasicOp.sReqStats.dwSpi )		dwATP += pITEM->sBasicOp.sReqStats.dwSpi - m_sSUMSTATS.dwSpi;
	if ( m_sSUMSTATS.dwDex < pITEM->sBasicOp.sReqStats.dwDex )		dwATP += pITEM->sBasicOp.sReqStats.dwDex - m_sSUMSTATS.dwDex;
	if ( m_sSUMSTATS.dwInt < pITEM->sBasicOp.sReqStats.dwInt )		dwATP += pITEM->sBasicOp.sReqStats.dwInt - m_sSUMSTATS.dwInt;
	if ( m_sSUMSTATS.dwSta < pITEM->sBasicOp.sReqStats.dwSta )		dwATP += pITEM->sBasicOp.sReqStats.dwSta - m_sSUMSTATS.dwSta;

	if ( m_wLevel < pITEM->sBasicOp.wReqLevelDW )					dwATP += pITEM->sBasicOp.wReqLevelDW - m_wLevel;

	return dwATP;
}

EMBEGINATTACK_FB GLCHARLOGIC::BEGIN_ATTACK ( WORD wStrikeNum )
{
	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	SITEM* pRHAND = GET_SLOT_ITEMDATA ( emRHand );
	SITEM* pLHAND = GET_SLOT_ITEMDATA ( emLHand );

	if ( pRHAND && pRHAND->sSuitOp.emAttack==ITEMATT_BOW )
	{
		if ( !pLHAND || pLHAND->sBasicOp.emItemType != ITEM_ARROW )			return EMBEGINA_ARROW;

		//	��밡�ɼ� ���� ��Ŵ.
		WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
		if ( wTurnNum >= wStrikeNum )	wTurnNum -= wStrikeNum;
		else							wTurnNum = 0;
	}
	if ( pRHAND && pRHAND->sSuitOp.emAttack==ITEMATT_DUAL_GUN )
	{
		if ( !pLHAND || pLHAND->sBasicOp.emItemType != ITEM_BULLET )			return EMBEGINA_BULLET;

		//	��밡�ɼ� ���� ��Ŵ.
		WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
		if ( wTurnNum >= wStrikeNum )	wTurnNum -= wStrikeNum;
		else							wTurnNum = 0;
	}
	if ( pRHAND && pRHAND->sSuitOp.emAttack==ITEMATT_RAIL_GUN )
	{
		if ( !pLHAND || pLHAND->sBasicOp.emItemType != ITEM_BULLET )			return EMBEGINA_BULLET;

		//	��밡�ɼ� ���� ��Ŵ.
		WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
		if ( wTurnNum >= wStrikeNum )	wTurnNum -= wStrikeNum;
		else							wTurnNum = 0;
	}
	if ( pRHAND && pRHAND->sSuitOp.emAttack==ITEMATT_PORTAL_GUN )
	{
		if ( !pLHAND || pLHAND->sBasicOp.emItemType != ITEM_BULLET )			return EMBEGINA_BULLET;

		//	��밡�ɼ� ���� ��Ŵ.
		WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
		if ( wTurnNum >= wStrikeNum )	wTurnNum -= wStrikeNum;
		else							wTurnNum = 0;
	}

	WORD wDisSP = GLCONST_CHAR::wBASIC_DIS_SP;
	if ( pRHAND )	wDisSP += pRHAND->sSuitOp.wReqSP;
	if ( pLHAND )	wDisSP += pLHAND->sSuitOp.wReqSP;

	//	SP ���� äũ.
	if ( m_sSP.dwNow < (wDisSP*wStrikeNum) )	return EMBEGINA_SP;

	return EMBEGINA_OK;
}

BOOL GLCHARLOGIC::VALID_LEVELUP () const
{
	// �ذ��δ� �ִ� ���� üũ�� �ٸ��� �Ѵ�.
	if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
	{
		if ( m_wLevel >= GLCONST_CHAR::wMAX_EXTREME_LEVEL )	return FALSE;
	}else{
		if ( m_wLevel >= GLCONST_CHAR::wMAX_LEVEL )	return FALSE;
	}
	return m_sExperience.lnNow >= GET_LEVELUP_EXP();
}

LONGLONG GLCHARLOGIC::GET_LEVELUP_EXP () const
{
	if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
	{
		return GLOGICEX::GLNEEDEXP2(GETLEVEL());
	}
	else
	{
		return GLOGICEX::GLNEEDEXP(GETLEVEL());
	}
}

void GLCHARLOGIC::LEVLEUP ( bool bInitNowExp )
{
	bool bMaxLevel = FALSE;
	if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
	{
		if( m_wLevel < GLCONST_CHAR::wMAX_EXTREME_LEVEL ) bMaxLevel = TRUE;
	}else{
		if( m_wLevel < GLCONST_CHAR::wMAX_LEVEL ) bMaxLevel = TRUE;
	}


	if ( /*m_wLevel < GLCONST_CHAR::wMAX_LEVEL*/bMaxLevel )
	{
		//	���� ����ġ ����.
		//GASSERT(m_sExperience.lnNow>=GET_LEVELUP_EXP());
		m_sExperience.lnNow -= GET_LEVELUP_EXP();

		//	������.
		++m_wLevel;

		//	������ ���ʽ� ����Ʈ
		if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
		{
			m_wStatsPoint += GLCONST_CHAR::wLVL_2ndSTATS_P;
			m_dwSkillPoint += GLCONST_CHAR::wLVL_2ndSKILL_P;
		}
		else
		{
			m_wStatsPoint += GLCONST_CHAR::wLVL_STATS_P;
			m_dwSkillPoint += GLCONST_CHAR::wLVL_SKILL_P;
		}

		//	��� ��ġ�� �ٽ� �ʱ�ȭ �Ѵ�.
		INIT_DATA(FALSE,FALSE,1.0f,bInitNowExp);

		//	ä���� ��� ȸ�� �����ش�.
		m_sHP.TO_FULL ();
		m_sMP.TO_FULL ();
		m_sSP.TO_FULL ();	
	}
}
//add pk
void GLCHARLOGIC::PKWINUP()
{
	m_dwPkWin += 1;
	//add pk streak
	m_dwPkCounter++;
	m_fPkTimer=0;
}

void GLCHARLOGIC::PKLOSSUP()
{
	m_dwPkLoss += 1;
	//add pk streak
	m_dwPkCounter = 0;
}
void GLCHARLOGIC::TDCOUNTSGNUC()
{
	m_dwTDCounterSGNUC = 1;

	if ( m_dwTDCounterPHXNUC != 0 ) 
	{
		m_dwTDCounterPHXNUC = 0;
	}

	if ( m_dwTDCounterMPNUC != 0 )
	{
		m_dwTDCounterMPNUC = 0;
	}
}
void GLCHARLOGIC::TDCOUNTSGTHE()
{
	m_dwTDCounterSGTHE = 1;

	if ( m_dwTDCounterPHXTHE != 0 ) 
	{
		m_dwTDCounterPHXTHE = 0;
	}

	if ( m_dwTDCounterMPTHE != 0 )
	{
		m_dwTDCounterMPTHE = 0;
	}
}
void GLCHARLOGIC::TDCOUNTSGFAC()
{
	m_dwTDCounterSGFAC = 1;

	if ( m_dwTDCounterPHXFAC != 0 ) 
	{
		m_dwTDCounterPHXFAC = 0; 
	}

	if ( m_dwTDCounterMPFAC != 0 )
	{
		m_dwTDCounterMPFAC = 0;
	}
}
void GLCHARLOGIC::TDCOUNTMPNUC()
{
	m_dwTDCounterMPNUC = 1;
	if ( m_dwTDCounterSGNUC != 0 ) 
	{
		m_dwTDCounterSGNUC = 0;
	}

	if ( m_dwTDCounterPHXNUC != 0 )
	{
		m_dwTDCounterPHXNUC = 0;
	}
}
void GLCHARLOGIC::TDCOUNTMPFAC()
{
	m_dwTDCounterMPFAC = 1;
	if ( m_dwTDCounterSGFAC != 0 ) 
	{
		m_dwTDCounterSGFAC = 0;
	}

	if ( m_dwTDCounterPHXFAC != 0 )
	{
		m_dwTDCounterPHXFAC = 0;
	}
}
void GLCHARLOGIC::TDCOUNTMPTHE()
{
	m_dwTDCounterMPTHE = 1;
	if ( m_dwTDCounterSGTHE != 0 ) 
	{
		m_dwTDCounterSGTHE = 0;
	}

	if ( m_dwTDCounterPHXTHE != 0 )
	{
		m_dwTDCounterPHXTHE = 0;
	}
}
void GLCHARLOGIC::TDCOUNTPHXNUC()
{
	m_dwTDCounterPHXNUC = 1;

	if ( m_dwTDCounterSGNUC != 0 ) 
	{
		m_dwTDCounterSGNUC = 0;
	}

	if ( m_dwTDCounterMPNUC != 0 )
	{
		m_dwTDCounterMPNUC = 0;
	}

}
void GLCHARLOGIC::TDCOUNTPHXTHE()
{
	m_dwTDCounterPHXTHE = 1;

	if ( m_dwTDCounterSGTHE != 0 ) 
	{
		m_dwTDCounterSGTHE = 0;
	}

	if ( m_dwTDCounterMPTHE != 0 )
	{
		m_dwTDCounterMPTHE = 0;
	}

}
void GLCHARLOGIC::TDCOUNTPHXFAC()
{
	m_dwTDCounterPHXFAC = 1;

	if ( m_dwTDCounterSGFAC != 0 ) 
	{
		m_dwTDCounterSGFAC = 0;
	}

	if ( m_dwTDCounterMPFAC != 0 )
	{
		m_dwTDCounterMPFAC = 0;
	}
}
void GLCHARLOGIC::TDTotalPHX()
{
	m_dwTDTotalPHX = m_dwTDCounterPHXNUC + m_dwTDCounterPHXFAC + m_dwTDCounterPHXTHE;
}
void GLCHARLOGIC::TDTotalSG()
{
	m_dwTDTotalSG = m_dwTDCounterSGNUC + m_dwTDCounterSGFAC + m_dwTDCounterSGTHE;
}
void GLCHARLOGIC::TDTotalMP()
{
	m_dwTDTotalMP = m_dwTDCounterMPNUC + m_dwTDCounterMPFAC + m_dwTDCounterMPTHE;
}
void GLCHARLOGIC::TDResetPHX()
{
	m_dwTDTotalPHX = 0;
}
void GLCHARLOGIC::TDResetSG()
{
	m_dwTDTotalSG = 0;
}
void GLCHARLOGIC::TDResetMP()
{
	m_dwTDTotalMP  = 0;
}
void GLCHARLOGIC::ShowRankName( BOOL bShowName ) //add pkrank
{
	m_bRankName = bShowName;
}
void GLCHARLOGIC::ShowRankMark( BOOL bShowMark ) //add pkrank
{
	m_bRankMark = bShowMark;
}
void GLCHARLOGIC::AllowBet( BOOL bAllowBet ) //add duel bet
{
	m_bAllowBet = bAllowBet;
}
void GLCHARLOGIC::CP_UP( DWORD dwRCLvl , WORD wRCNum ) //add cp
{
	float fRate =  dwRCLvl/wRCNum ;
	float fRateFinal =  fRate*m_fCP_GAIN ;

	m_sCP.dwNow += DWORD ( fRateFinal );
	m_sCP.LIMIT();
	//m_dwPkWin += 1; //remove this later
}

BOOL GLCHARLOGIC::DuelWinReward ( LONGLONG llLoss, WORD wType  )
{
	if ( wType == 1 )	m_lnMoney += llLoss ;
	else if ( wType == 2 )	m_lnVoteP += llLoss ;
	else if ( wType == 3 )	m_lnPremP += llLoss ;

	return TRUE;
}

BOOL GLCHARLOGIC::DuelLossReward ( LONGLONG llLoss, WORD wType  )
{
	if ( wType == 1 )
	{
		if  ( m_lnMoney < llLoss ){
			return FALSE;
		}else{
				m_lnMoney -= llLoss ;}
	}
	else if ( wType == 2 )
	{
		if  ( m_lnVoteP < llLoss ){
			return FALSE;
		}else{
				m_lnVoteP -= llLoss ;}
	}
	else if ( wType == 3 )
	{
		if  ( m_lnPremP < llLoss ){
			return FALSE;
		}else{
				m_lnPremP -= llLoss ;}
	}

	return TRUE;
}

void GLCHARLOGIC::STATSUP ( EMSTATS emStats )
{
	switch ( emStats )
	{
	case EMPOW:
		{
			if ( m_sStats.dwPow >= GLCONST_CHAR::dwPowMax )
			{}else{
				++m_sStats.dwPow;
				--m_wStatsPoint;	}
		}
		break;

	case EMSTR:
		{
			if ( m_sStats.dwStr >= GLCONST_CHAR::dwStrMax )
			{}else{
				++m_sStats.dwStr;
				--m_wStatsPoint;	}
		}
		break;

	case EMSPI: 
		{
			if ( m_sStats.dwSpi >= GLCONST_CHAR::dwSpiMax )
			{}else{
				++m_sStats.dwSpi; 
				--m_wStatsPoint;	}
		}
		break;

	case EMDEX: 
		{
			if ( m_sStats.dwDex >= GLCONST_CHAR::dwDexMax )
			{}else{
				++m_sStats.dwDex; 
				--m_wStatsPoint;	}
		}
		break;

	case EMINT:
		{
			if ( m_sStats.dwInt >= GLCONST_CHAR::dwIntMax )
			{}else{
				++m_sStats.dwInt; 
				--m_wStatsPoint;	}
		}
		break;

	case EMSTA: 
		{
			if ( m_sStats.dwSta >= GLCONST_CHAR::dwStaMax )
			{}else{
				++m_sStats.dwSta;
				--m_wStatsPoint;
			}
		}
		break;
	};

	//	�׿� STATE�� ����.
	//--m_wStatsPoint;

	//	��� ��ġ�� �ٽ� �ʱ�ȭ �Ѵ�.
	INIT_DATA(FALSE,FALSE);

}

void GLCHARLOGIC::STATSUP ( SCHARSTATS	sStats )
{
	m_sStats.dwPow += sStats.dwPow;
	m_sStats.dwStr += sStats.dwStr;
	m_sStats.dwSpi += sStats.dwSpi;
	m_sStats.dwDex += sStats.dwDex;
	m_sStats.dwInt += sStats.dwInt;
	m_sStats.dwSta += sStats.dwSta;

	m_wStatsPoint -= sStats.GetTotal();

	INIT_DATA(FALSE,FALSE);
}

//add addstats cmd
void GLCHARLOGIC::STATSUP_CMD( EMSTATS emStats, DWORD value ){

	switch ( emStats )
	{
	case EMPOW:
		{
			if ((m_sStats.dwPow + value) > GLCONST_CHAR::dwPowMax )
			{}else if( m_wStatsPoint >= value ){
				m_sStats.dwPow += value;
				m_wStatsPoint -= value;	}
		}
		break;

	case EMSTR:
		{
			if ((m_sStats.dwStr + value) > GLCONST_CHAR::dwStrMax )
			{}else if( m_wStatsPoint >= value ){
				m_sStats.dwStr += value;
				m_wStatsPoint -= value;	}
		}
		break;

	case EMSPI: 
		{
			if ((m_sStats.dwSpi + value) > GLCONST_CHAR::dwSpiMax )
			{}else if( m_wStatsPoint >= value ){
				m_sStats.dwSpi += value;
				m_wStatsPoint -= value;	}
		}
		break;

	case EMDEX: 
		{
			if ((m_sStats.dwDex + value) > GLCONST_CHAR::dwDexMax )
			{}else if( m_wStatsPoint >= value ){
				m_sStats.dwDex += value;
				m_wStatsPoint -= value;	}
		}
		break;

	case EMINT:
		{
			if ((m_sStats.dwInt + value) > GLCONST_CHAR::dwIntMax )
			{}else if( m_wStatsPoint >= value ){
				m_sStats.dwInt += value;
				m_wStatsPoint -= value;	}
		}
		break;

	case EMSTA: 
		{
			if ((m_sStats.dwSta + value) > GLCONST_CHAR::dwStaMax )
			{}else if( m_wStatsPoint >= value ){
				m_sStats.dwSta += value;
				m_wStatsPoint -= value;	}
		}
		break;


	};

	INIT_DATA(FALSE,FALSE);
}
// *****************************************************
// Desc: ��ų���� ����
// *****************************************************
bool GLCHARLOGIC::RESET_SKILL ()
{
	EMCHARINDEX emINDEX = CharClassToIndex(m_emClass);
	const SCHARDATA2& sCHARDATA = GLCONST_CHAR::GET_CHAR_DATA2(m_wSchool,emINDEX);

	//	Note : ����Ʈ���� ȹ���� ��ų ����Ʈ.
	//
	DWORD dwGiftSkill(0);

	GLQuestPlay::MAPQUEST& sQuestEnd = m_cQuestPlay.GetQuestEnd();
	GLQuestPlay::MAPQUEST_ITER pos = sQuestEnd.begin();
	GLQuestPlay::MAPQUEST_ITER end = sQuestEnd.end();
	for ( ; pos!=end; ++pos )
	{
		GLQUESTPROG *pPROG = (*pos).second;
		GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pPROG->m_sNID.dwID );
		if ( !pQUEST )	continue;

		dwGiftSkill += pQUEST->m_dwGiftSKILLPOINT;
	}

	//	Note : ��ų ����Ʈ �ʱ�ȭ.
	//
	if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
	{
		m_dwSkillPoint = sCHARDATA.m_dwSkillPoint + (m_wLevel-1)*GLCONST_CHAR::wLVL_2ndSKILL_P + dwGiftSkill;
	}
	else
	{
		m_dwSkillPoint = sCHARDATA.m_dwSkillPoint + (m_wLevel-1)*GLCONST_CHAR::wLVL_SKILL_P + dwGiftSkill;
	}

	//	Note : ��ų�� ����� ���� ���� ����.
	//
	m_wSKILLQUICK_ACT = 0;
	for ( int i=0; i<EMSKILLQUICK_SIZE; ++i )
	{
		m_sSKILLQUICK[i] = SNATIVEID(false);
	}


	//	Note : ������ ��ų �ʱ�ȭ.
	//

	m_ExpSkills.clear();
//	m_ExpSkills = sCHARDATA.m_ExpSkills;

	//	Note : ��� ��ġ�� �ٽ� �ʱ�ȭ �Ѵ�.
	//
	INIT_DATA(FALSE,FALSE);

	return true;
}

// *****************************************************
// Desc: �������� ����
// *****************************************************
bool GLCHARLOGIC::RESET_STATS ( const DWORD dwDIS )
{
	EMCHARINDEX emINDEX = CharClassToIndex(m_emClass);
	const SCHARDATA2& sCHARDATA = GLCONST_CHAR::GET_CHAR_DATA2(m_wSchool,emINDEX);

	// ��ü ���� ����Ʈ �ʱ�ȭ
	if ( dwDIS==UINT_MAX )
	{
		//	Note : ����Ʈ���� ȹ���� ���� ����Ʈ.
		//
		DWORD dwGiftStats(0);

		GLQuestPlay::MAPQUEST& sQuestEnd = m_cQuestPlay.GetQuestEnd();
		GLQuestPlay::MAPQUEST_ITER pos = sQuestEnd.begin();
		GLQuestPlay::MAPQUEST_ITER end = sQuestEnd.end();
		for ( ; pos!=end; ++pos )
		{
			GLQUESTPROG *pPROG = (*pos).second;
			GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pPROG->m_sNID.dwID );
			if ( !pQUEST )	continue;

			dwGiftStats += (DWORD) pQUEST->m_dwGiftSTATSPOINT;
		}

		if( m_CHARINDEX == GLCI_EXTREME_M || m_CHARINDEX == GLCI_EXTREME_W )
		{
			m_wStatsPoint = sCHARDATA.m_wStatsPoint + (m_wLevel-1)*GLCONST_CHAR::wLVL_2ndSTATS_P + dwGiftStats;
		}
		else
		{
			m_wStatsPoint = sCHARDATA.m_wStatsPoint + (m_wLevel-1)*GLCONST_CHAR::wLVL_STATS_P + dwGiftStats;
		}

		m_sStats.RESET();

		return true;
	}

	// ������ ��޿� ���� �ʱ�ȭ
	DWORD dwNOW_DIS(0);
	while(1)
	{
		if ( dwNOW_DIS>=dwDIS )		break;
		if ( m_sStats.IsZERO() )	break;

		// �� ���ݺ��� �ϳ��� ����
		for ( int i=0; i<EMSIZE; ++i )
		{
			if ( dwNOW_DIS>=dwDIS )		break;
			if ( m_sStats.IsZERO() )	break;

			DWORD &dwSTAT = m_sStats.GET((EMSTATS)i);
			if ( dwSTAT == 0 )			continue;

			--dwSTAT;
			++dwNOW_DIS;
		}
	}

	//	Note : ���� �ܿ� ���� ����Ʈ�� ����.
	m_wStatsPoint += dwNOW_DIS;

	//	Note : ��� ��ġ�� �ٽ� �ʱ�ȭ �Ѵ�.
	//
	INIT_DATA(FALSE,FALSE);

	return true;
}

bool GLCHARLOGIC::RESET_STATS_SKILL ( const DWORD dwDIS_STAT )
{
	//	Note : ���� ȣ�� ������ stats, skill���̿��� �Ѵ�.
	RESET_STATS ( dwDIS_STAT );
	RESET_SKILL();

	return true;
}

void GLCHARLOGIC::UPDATESKILLDELAY ( float fElapsedTime )
{
	DELAY_MAP_ITER iter_del;

	DELAY_MAP_ITER iter = m_SKILLDELAY.begin ();
	DELAY_MAP_ITER iter_end = m_SKILLDELAY.end ();

	for ( ; iter!=iter_end; )
	{
		float &fDelay = (*iter).second;
		iter_del = iter++;

		fDelay -= fElapsedTime;
		if ( fDelay <= 0.0f )	m_SKILLDELAY.erase ( iter_del );
	}
}

void GLCHARLOGIC::LEARN_SKILL ( SNATIVEID skill_id )
{
	//	Note : ��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )											return;

	//	Note : ����ġ ���ҽ�Ŵ.
	//
	SKILL::SLEARN_LVL &sSKILL_LEARN_LVL = pSkill->m_sLEARN.sLVL_STEP[0];
	if ( m_dwSkillPoint < sSKILL_LEARN_LVL.dwSKP )			return;

	if ( m_lnMoney < sSKILL_LEARN_LVL.dwMoneyReq )			return;
	if ( m_lnVoteP < sSKILL_LEARN_LVL.dwVotePoint )			return;
	if ( m_lnPremP < sSKILL_LEARN_LVL.dwPremPoint )			return;

	m_dwSkillPoint -= sSKILL_LEARN_LVL.dwSKP;

	m_lnMoney -= sSKILL_LEARN_LVL.dwMoneyReq;
	m_lnVoteP -= sSKILL_LEARN_LVL.dwVotePoint;
	m_lnPremP -= sSKILL_LEARN_LVL.dwPremPoint;

	//	Note : ��� ��ų�� �����.
	//
	m_ExpSkills.insert ( std::make_pair(skill_id.dwID,SCHARSKILL(skill_id,0)) );

	//	Note : ��ú� ��ų�� ��ȭ �Ҷ� �ʱ� ��ġ���� ��� �� ����Ѵ�.
	//
	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		INIT_DATA ( FALSE, FALSE );
	}
}

bool GLCHARLOGIC::LEARN_SKILL_QUEST ( SNATIVEID skill_id )
{
	//	Note : ��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )	return false;

	//	Note : ��� ��ų�� �����.
	//
	
	m_ExpSkills.insert ( std::make_pair(skill_id.dwID,SCHARSKILL(skill_id,0)) );
	//	Note : ��ú� ��ų�� ��ȭ �Ҷ� �ʱ� ��ġ���� ��� �� ����Ѵ�.
	//
	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		INIT_DATA ( FALSE, FALSE );
	}
	return true;
}

VOID GLCHARLOGIC::LVLUP_SKILL ( SNATIVEID skill_id, WORD wToLevel )
{
	//	Note : ��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )											return;

	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter==m_ExpSkills.end() )						return;

	SCHARSKILL &sSkill = (*learniter).second;

	//	Note : ����ġ ���ҽ�Ŵ.
	//
	SKILL::SLEARN_LVL &sSKILL_LEARN_LVL = pSkill->m_sLEARN.sLVL_STEP[wToLevel];
	if ( m_dwSkillPoint < sSKILL_LEARN_LVL.dwSKP )			return;

	if ( m_lnMoney < sSKILL_LEARN_LVL.dwMoneyReq )			return;
	if ( m_lnVoteP < sSKILL_LEARN_LVL.dwVotePoint )			return;
	if ( m_lnPremP < sSKILL_LEARN_LVL.dwPremPoint )			return;

	m_dwSkillPoint -= sSKILL_LEARN_LVL.dwSKP;

	m_lnMoney -= sSKILL_LEARN_LVL.dwMoneyReq;
	m_lnVoteP -= sSKILL_LEARN_LVL.dwVotePoint;
	m_lnPremP -= sSKILL_LEARN_LVL.dwPremPoint;

	//	Note : ��ų ���� ��.
	//
	++sSkill.wLevel;

	//	Note : ��ú� ��ų�� ��ȭ �Ҷ� �ʱ� ��ġ���� ��� �� ����Ѵ�.
	//
	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		INIT_DATA ( FALSE, FALSE );
	}
}

BOOL GLCHARLOGIC::ISLEARNED_SKILL ( SNATIVEID skill_id, WORD wLEVEL )
{
	//	Note : ��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )											return FALSE;

	//	Note : ���� ��ų���� �˻�.
	//
	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter==m_ExpSkills.end() )			return FALSE;

	SCHARSKILL &sSkill = (*learniter).second;
	if ( sSkill.wLevel <  wLEVEL )				return FALSE;

	return TRUE;
}

BOOL GLCHARLOGIC::ISMASTER_SKILL ( SNATIVEID skill_id )
{
	//	Note : ��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )												return FALSE;

	//	Note : ���� ��ų���� �˻�.
	//
	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter==m_ExpSkills.end() )							return FALSE;

	SCHARSKILL &sSkill = (*learniter).second;

	if ( DWORD(sSkill.wLevel+1)>=SKILL::MAX_LEVEL )				return TRUE;
	if ( DWORD(sSkill.wLevel+1)>=pSkill->m_sBASIC.dwMAXLEVEL )	return TRUE;

	return FALSE;
}

SCHARSKILL* GLCHARLOGIC::GETLEARNED_SKILL ( SNATIVEID skill_id )
{
	//	Note : ���� ��ų���� �˻�.
	//
	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter==m_ExpSkills.end() )			return NULL;

	return &((*learniter).second);
}

EMSKILL_LEARNCHECK GLCHARLOGIC::CHECKLEARNABLE_SKILL ( SNATIVEID skill_id )
{
	WORD wSKILL_LEVEL = 0;

	//	Note : ��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )								return EMSKILL_LEARN_UNKNOWN;

	//	Note : ���� ��ų���� �˻�.
	//
	SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
	if ( learniter!=m_ExpSkills.end() )
	{
		SCHARSKILL &sSkill = (*learniter).second;
		wSKILL_LEVEL = sSkill.wLevel + 1;			//	���� ��ų LEVEL

		if ( wSKILL_LEVEL == SKILL::MAX_LEVEL )				return EMSKILL_LEARN_MAX;
		if ( wSKILL_LEVEL == pSkill->m_sBASIC.dwMAXLEVEL )	return EMSKILL_LEARN_MAX;
	}
	
	SKILL::SLEARN &sSKILL_LEARN = pSkill->m_sLEARN;
	SKILL::SLEARN_LVL &sSKILL_LEARN_LVL = pSkill->m_sLEARN.sLVL_STEP[wSKILL_LEVEL];

	//	Note : ��ų �������� �˻�.
	//
	if ( !(m_emClass&sSKILL_LEARN.dwCLASS) )					return EMSKILL_LEARN_NOTCLASS;
	
	if ( sSKILL_LEARN.emBRIGHT!=BRIGHT_BOTH )
	{
		if ( GETBRIGHT()!=sSKILL_LEARN.emBRIGHT )				return EMSKILL_LEARN_NOTBRIGHT;
	}

	if ( m_dwSkillPoint < sSKILL_LEARN_LVL.dwSKP )				return EMSKILL_LEARN_NOTTERM;
	if ( WORD(GETLEVEL()) < sSKILL_LEARN_LVL.dwLEVEL )			return EMSKILL_LEARN_NOTTERM;
	if ( m_dwReborn < sSKILL_LEARN_LVL.dwREBORN )				return EMSKILL_LEARN_NOTTERM;
	if ( !m_sSUMSTATS.CHECK_REQ ( sSKILL_LEARN_LVL.sSTATS ) )	return EMSKILL_LEARN_NOTTERM;

	if ( m_lnMoney < sSKILL_LEARN_LVL.dwMoneyReq )				return EMSKILL_LEARN_NOTTERM;
	if ( m_lnVoteP < sSKILL_LEARN_LVL.dwVotePoint )				return EMSKILL_LEARN_NOTTERM;
	if ( m_lnPremP < sSKILL_LEARN_LVL.dwPremPoint )				return EMSKILL_LEARN_NOTTERM;

	//	�䱸��ų ���� �˻�.
	if ( NATIVEID_NULL()!=sSKILL_LEARN.sSKILL )
	{
		if ( !ISLEARNED_SKILL(sSKILL_LEARN.sSKILL,WORD(sSKILL_LEARN_LVL.dwSKILL_LVL)) )	return EMSKILL_LEARN_NOTTERM;
	}

	return EMSKILL_LEARN_OK;
}

void GLCHARLOGIC::SETACTIVESKILL ( SNATIVEID skill_id )
{
	m_idACTIVESKILL = skill_id;
}

EMSKILLCHECK GLCHARLOGIC::CHECHSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bNotLearn )
{
	//	Note : ���� ��ų���� �˻�.
	//
	if ( m_bAntiSkill ) return EMSKILL_NOTSKILL; //add antiskill

	SCHARSKILL sSkill;
	if ( bNotLearn )
	{
		if ( skill_id != m_sDefenseSkill.m_dwSkillID ) return EMSKILL_NOTLEARN;
		sSkill.sNativeID = skill_id;
		sSkill.wLevel = m_sDefenseSkill.m_wLevel;
	}
	else
	{
		SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
		if ( learniter==m_ExpSkills.end() )										return EMSKILL_NOTLEARN; // ��� ��ų�� �ƴ� ���.
		sSkill = (*learniter).second;

		//	Note : ��ų ������ Ÿ���� �������� ����.
		//
		DELAY_MAP_ITER delayiter = m_SKILLDELAY.find ( skill_id.dwID );
		if ( delayiter!=m_SKILLDELAY.end() )									return EMSKILL_DELAYTIME; // ��ų ������ �ð��� ������ ����.
	}

	//	Note : ��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id );
	if ( !pSkill )															return EMSKILL_UNKNOWN;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[sSkill.wLevel];
	

	//	Note : ����� �� �ִ� ��ų�������� �˻�.
	if ( pSkill->m_sBASIC.emROLE!=SKILL::EMROLE_NORMAL )					return EMSKILL_UNKNOWN;

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	//	Note : ��ų ���� �����ؾ� �ϴ� ������ ���� �˻�.
	//
	GLITEM_ATT emSKILL_LITEM = pSkill->m_sBASIC.emUSE_LITEM;
	GLITEM_ATT emSKILL_RITEM = pSkill->m_sBASIC.emUSE_RITEM;
	if ( emSKILL_LITEM!=ITEMATT_NOCARE )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( !pItem )														return EMSKILL_NOTITEM;

		if ( pItem->sSuitOp.emAttack != ITEMATT_FIST )
		{
			if ( !CHECHSKILL_ITEM(emSKILL_LITEM,pItem->sSuitOp.emAttack) )		return EMSKILL_NOTITEM;
		}
	}

	if ( emSKILL_RITEM!=ITEMATT_NOCARE )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emRHand);
		if ( !pItem )														return EMSKILL_NOTITEM;
		
		if ( pItem->sSuitOp.emAttack != ITEMATT_FIST )
		{
			if ( !CHECHSKILL_ITEM(emSKILL_RITEM,pItem->sSuitOp.emAttack) )		return EMSKILL_NOTITEM;
		}
		
	}

	//	Note : ȭ�� �Ҹ�� �Ҹ� ���� üũ.
	//		--> �Ҹ��� 2 ���δ� �Ѱ��� �����ִ� ��Ȳ? ���� ���� ���ڸ� �����ϰ� ��ų�� �ߵ��ǰ� ����.
	//
	if ( sSKILL_DATA.wUSE_ARROWNUM!=0 )
	{
		//old code
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( !pItem )														return EMSKILL_NOTARROW;

		if ( pItem->sBasicOp.emItemType != ITEM_ARROW )						return EMSKILL_NOTARROW;
	}

	if ( sSKILL_DATA.wUSE_BULLET!=0 )
	{
		//old code
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( !pItem )														return EMSKILL_NOTBULLET;

		if ( pItem->sBasicOp.emItemType != ITEM_BULLET )						return EMSKILL_NOTBULLET;
	}

	//	Note : ���� �Ҹ�� �Ҹ� ���� üũ.
	//		--> �Ҹ��� 2 ���δ� �Ѱ��� �����ִ� ��Ȳ? ���� ���� ���ڸ� �����ϰ� ��ų�� �ߵ��ǰ� ����.
	//
	if ( sSKILL_DATA.wUSE_CHARMNUM!=0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( !pItem )														return EMSKILL_NOTCHARM;

		if ( pItem->sBasicOp.emItemType != ITEM_CHARM )						return EMSKILL_NOTCHARM;

		//	������ �������� �ʴ´�. ���ڶ� ��� �ִ°� �ѵ������� �Ҹ��Ŵ.
	}

	//	Note : ���� ü�� �Ҹ� ����.
	if ( m_sHP.dwNow <= sSKILL_DATA.wUSE_HP*wStrikeNum )					return EMSKILL_NOTHP;
	if ( m_sMP.dwNow < sSKILL_DATA.wUSE_MP*wStrikeNum )						return EMSKILL_NOTMP;
	if ( m_sCP.dwNow < sSKILL_DATA.wUSE_CP*wStrikeNum )						return EMSKILL_NOTCP; //add cp

	//if ( m_sExperience.lnNow < sSKILL_DATA.wUSE_EXP*wStrikeNum )			return EMSKILL_NOTEXP;

	//	NEED : ���� ��Ƽ��ų�� �����Ǿ�����.
	//
	//sSKILL_DATA.wUSE_HP_PTY;
	//sSKILL_DATA.wUSE_MP_PTY;

	SITEM* pRHAND = GET_SLOT_ITEMDATA ( emRHand );
	SITEM* pLHAND = GET_SLOT_ITEMDATA ( emLHand );

	WORD wDisSP = sSKILL_DATA.wUSE_SP;
	if ( pRHAND )	wDisSP += pRHAND->sSuitOp.wReqSP;
	if ( pLHAND )	wDisSP += pLHAND->sSuitOp.wReqSP;

	if ( m_sSP.dwNow < wDisSP*wStrikeNum )									return EMSKILL_NOTSP;

	//	NEED : ���� ��Ƽ��ų�� �����Ǿ�����.
	//
	//sSKILL_DATA.wUSE_SP_PTY;

	return EMSKILL_OK;
}

void GLCHARLOGIC::ACCOUNTSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bServer )
{
	//	Note : ĳ���Ͱ� ��� ��ų ���� ������.
	// �ߵ���ų�� ��ų ��Ͽ� ����
	SCHARSKILL sSkill;

	if ( IsDefenseSkill() )
	{
		if ( skill_id != m_sDefenseSkill.m_dwSkillID ) return;
		sSkill.sNativeID = skill_id;
		sSkill.wLevel = m_sDefenseSkill.m_wLevel;
	}
	else
	{
		SKILL_MAP_ITER learniter = m_ExpSkills.find ( skill_id.dwID );
		if( learniter==m_ExpSkills.end() )										return;	// ��� ��ų�� �ƴ� ���.
		sSkill = (*learniter).second;
	}

	//	��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )															return;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[sSkill.wLevel];


	//	��ų ������ ���.
	float fDelayTime = GLOGICEX::SKILLDELAY(pSkill->m_sBASIC.dwGRADE,sSkill.wLevel,GETLEVEL(),sSKILL_DATA.fDELAYTIME);
	
	//	Note : �����̻��� ������ ����.
	fDelayTime = fDelayTime * m_fSTATE_DELAY;

	//	Note : ���������� �޽��� ������ ��ŭ ���ҽ��� �ش�.
	if ( bServer )		APPLY_MSGDELAY ( fDelayTime );

	m_SKILLDELAY.insert ( std::make_pair(skill_id.dwID,fDelayTime) );

	EMSLOT emLHand = GetCurLHand();

	//	ȭ�� �Ҹ�.
	if ( sSKILL_DATA.wUSE_ARROWNUM!= 0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( pItem )
		{
			//	ȭ�� �Ҹ��ϴ� ��ų������ ȭ������ �ݵ�� �־�� ������ ������ �������� ���� ������ ����.
			//	ȭ�� ���� ������ �������� �ʴ´�.
			WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
			if ( wTurnNum >= sSKILL_DATA.wUSE_ARROWNUM )	wTurnNum -= sSKILL_DATA.wUSE_ARROWNUM;
			else											wTurnNum = 0;
		}
	}

	if ( sSKILL_DATA.wUSE_BULLET!= 0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( pItem )
		{
			//	ȭ�� �Ҹ��ϴ� ��ų������ ȭ������ �ݵ�� �־�� ������ ������ �������� ���� ������ ����.
			//	ȭ�� ���� ������ �������� �ʴ´�.
			WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
			if ( wTurnNum >= sSKILL_DATA.wUSE_BULLET )	wTurnNum -= sSKILL_DATA.wUSE_BULLET;
			else											wTurnNum = 0;
		}
	}

	//	���� �Ҹ�.
	if ( sSKILL_DATA.wUSE_CHARMNUM!= 0 )
	{
		SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
		if ( pItem )
		{
			//	���� �Ҹ��ϴ� ��ų������ ȭ������ �ݵ�� �־�� ������ ������ �������� ���� ������ ����.
			//	���� ���� ������ �������� �ʴ´�.
			WORD &wTurnNum = m_PutOnItems[emLHand].wTurnNum;
			if ( wTurnNum >= sSKILL_DATA.wUSE_CHARMNUM )	wTurnNum -= sSKILL_DATA.wUSE_CHARMNUM;
			else											wTurnNum = 0;
		}
	}

	//	���� ü�� �Ҹ�.
	m_sHP.DECREASE ( sSKILL_DATA.wUSE_HP*wStrikeNum );
	m_sMP.DECREASE ( sSKILL_DATA.wUSE_MP*wStrikeNum );

	m_sCP.DECREASE ( sSKILL_DATA.wUSE_CP*wStrikeNum );//add cp

	//m_sExperience.DECREASE ( sSKILL_DATA.wUSE_EXP*wStrikeNum );
}


BOOL GLCHARLOGIC::DOGRINDING(	SITEMCUSTOM &sCusItem, 
								const SITEM *pHold, 
								bool &_bRESET, 
								bool &_bTERMINATE, 
								bool &_bNOFAIL, //add nofailups
								EMANTIDISAPPEAR &emANTIDISAPPEAR )
{
	bool bSUCCEED(false);
	_bRESET = false;
	_bTERMINATE = false;

	const EMGRINDING_TYPE emGRINDING = pHold->sGrindingOp.emTYPE;

	BYTE cGRADE = sCusItem.GETGRADE ( emGRINDING );
	
	if ( emGRINDING == EMGRINDING_DAMAGE || emGRINDING == EMGRINDING_DEFENSE )
	{
		if ( cGRADE >= GLCONST_CHAR::wGRADE_MAX )	return FALSE;
	}
	else
	{
		if ( cGRADE >= GLCONST_CHAR::wGRADE_MAX_REGI )	return FALSE;
	}
	
	

	//	4�ܰ� �̻���� ���� Ȯ�� ����.
	if ( cGRADE >= GRADE_NORMAL && !_bNOFAIL ) //add nofailups
	{
		//	���� Ȯ��.
		float fRATE = GLCONST_CHAR::fGRADE_RATE[cGRADE];
		BOOL bOK = RANDOM_GEN ( fRATE );
		if ( !bOK )
		{
			//	���� Ȯ��.
			float fRESET = GLCONST_CHAR::fGRADE_RESET_RATE[cGRADE];
			bOK = RANDOM_GEN ( fRESET );
			if ( bOK )
			{
				_bRESET = true;
				sCusItem.ResetGrind(emGRINDING);
			
				//	�ļ� Ȯ��.
				bool bTER = GLCONST_CHAR::bENCHANT_TERMINATE_ITEM && ( emGRINDING==EMGRINDING_DAMAGE || emGRINDING==EMGRINDING_DEFENSE );
				if ( bTER )
				{
					float fTERMINATE = GLCONST_CHAR::fGRADE_TERMINATE_RATE[cGRADE];
					bOK = RANDOM_GEN ( fTERMINATE );
					if ( bOK )
					{
						// �ҹ��ָ� ������ �ִٸ�
						if( emANTIDISAPPEAR == EMANTIDISAPPEAR_ON )
						{
							emANTIDISAPPEAR = EMANTIDISAPPEAR_USE; // �ҹ��ָ� ����ߴ�.
						}
						else
						{
							_bTERMINATE = true;		// �������� �Ҹ�Ǿ���.
						}
					}
				}
			}

			return FALSE;
		}
	}

	//	Note : ������ ����.
	//
	switch ( pHold->sGrindingOp.emTYPE )
	{
	case EMGRINDING_DAMAGE:			sCusItem.cDAMAGE++;			break;
	case EMGRINDING_DEFENSE:		sCusItem.cDEFENSE++;		break;
	case EMGRINDING_RESIST_FIRE:	sCusItem.cRESIST_FIRE++;	break;
	case EMGRINDING_RESIST_ICE:		sCusItem.cRESIST_ICE++;		break;
	case EMGRINDING_RESIST_ELEC:	sCusItem.cRESIST_ELEC++;	break;
	case EMGRINDING_RESIST_POISON:	sCusItem.cRESIST_POISON++;	break;
	case EMGRINDING_RESIST_SPIRIT:	sCusItem.cRESIST_SPIRIT++;	break;
	};

	return TRUE;
}

//	Note : �ɸ��Ͱ� �������� �����ϰ� �ִ��� �˻�.
//		���뺹��, �κ��丮���� �˻�.
//		��ħ���� �������� ��� full �� ���� ��츸 ������ ������ �Ǵ�.
//
BOOL GLCHARLOGIC::ISHAVEITEM ( SNATIVEID &sNID, DWORD *pNum )
{
	DWORD dwNum = 0;
	for ( int i=0; i<SLOT_TSIZE; ++i )
	{
		const SITEM* pITEM = GET_SLOT_ITEMDATA ( EMSLOT(i) );
		if ( !pITEM )								continue;

		const SITEMCUSTOM &sCUSTOM = GET_SLOT_ITEM ( EMSLOT(i) );
		if ( sCUSTOM.sNativeID==SNATIVEID(false) )	continue;

		if ( pITEM->sBasicOp.sNativeID==sNID )
		{
			if ( pITEM->sDrugOp.wPileNum == 1 )						dwNum++;
			else if ( pITEM->sDrugOp.wPileNum == sCUSTOM.wTurnNum )	dwNum++;
		}
	}

	dwNum += m_cInventory.CountPileItem ( sNID );

	if ( pNum )		*pNum = dwNum;
	return ( dwNum > 0 );
}

BOOL GLCHARLOGIC::ISREVIVE ()
{
	SITEM* pITEM = GET_SLOT_ITEMDATA(SLOT_NECK);
	if ( !pITEM )											return FALSE;
	if ( pITEM->sDrugOp.emDrug!=ITEM_DRUG_CALL_REVIVE )		return FALSE;

	return TRUE;
}

WORD GLCHARLOGIC::GETSKILLRANGE_TAR ( const GLSKILL &sSKILL ) const
{
	WORD dwRANGE = sSKILL.m_sBASIC.wTARRANGE;
	if ( sSKILL.m_sBASIC.emAPPLY==SKILL::EMAPPLY_PHY_LONG )		dwRANGE += (WORD) GETSUM_TARRANGE();
	return dwRANGE;
}

WORD GLCHARLOGIC::GETSKILLRANGE_APPLY ( const GLSKILL &sSKILL, const WORD dwLEVEL ) const
{
	const SKILL::CDATA_LVL &sDATA_LVL = sSKILL.m_sAPPLY.sDATA_LVL[dwLEVEL];

	WORD dwRANGE = sDATA_LVL.wAPPLYRANGE;
	if ( sSKILL.m_sBASIC.emAPPLY==SKILL::EMAPPLY_PHY_LONG )		dwRANGE += (WORD) GETSUM_TARRANGE();
	return dwRANGE;
}

//	Note : ���� pk ������ �˾ƺ���. ( UINT_MAX �ϰ�� pk �ش���� ����. )
DWORD GLCHARLOGIC::GET_PK_LEVEL ()
{
	if ( m_nBright >= 0 )	return UINT_MAX;

	DWORD dwLEVEL = 0;
	for ( dwLEVEL=0; dwLEVEL<GLCONST_CHAR::EMPK_STATE_LEVEL; ++dwLEVEL )
	{
		if ( GLCONST_CHAR::sPK_STATE[dwLEVEL].nPKPOINT <= m_nBright )		break;
	}

	if ( dwLEVEL>=GLCONST_CHAR::EMPK_STATE_LEVEL )	dwLEVEL = GLCONST_CHAR::EMPK_STATE_LEVEL-1;

	return dwLEVEL;
}


float GLCHARLOGIC::GET_PK_DECEXP_RATE ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return 100.0f;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].fPK_EXP_RATE;
}

std::string GLCHARLOGIC::GET_PK_NAME ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return GLCONST_CHAR::sPK_STATE[0].strNAME;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].strNAME;
}

DWORD GLCHARLOGIC::GET_PK_COLOR ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return GLCONST_CHAR::dwPK_NORMAL_NAME_COLOR;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].dwNAME_COLOR;
}

float GLCHARLOGIC::GET_PK_SHOP2BUY ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return 100.0f;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].fSHOP_2BUY_RATE;
}

float GLCHARLOGIC::GET_PK_SHOP2SALE ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )	return 100.0f;

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].fSHOP_2SALE_RATE;
}

DWORD GLCHARLOGIC::GET_PK_ITEMDROP_NUM ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	if ( dwLEVEL==UINT_MAX )
	{
		if ( GLCONST_CHAR::fPK_ITEM_DROP==0 )	return 0;
		return 1;
	}

	return GLCONST_CHAR::sPK_STATE[dwLEVEL].dwITEM_DROP_NUM;
}

float GLCHARLOGIC::GET_PK_ITEMDROP_RATE ()
{
	DWORD dwLEVEL = GET_PK_LEVEL();
	float fRATE = 0;

	if ( dwLEVEL==UINT_MAX )						fRATE = GLCONST_CHAR::fPK_ITEM_DROP;
	else											fRATE = GLCONST_CHAR::sPK_STATE[dwLEVEL].fITEM_DROP_RATE;

	if ( GLSchoolFreePK::GetInstance().IsON() )		fRATE += GLCONST_CHAR::fSCHOOL_FREE_PK_ITEM_DROP;

	return fRATE;
}

bool GLCHARLOGIC::ISPLAYKILLING ()
{
	return !m_mapPlayHostile.empty();
}

//	Note : �ڽ��� �������ΰ�?
bool GLCHARLOGIC::ISOFFENDER ()
{
	return ( m_nBright < GLCONST_CHAR::sPK_STATE[0].nPKPOINT );
}

bool GLCHARLOGIC::IS_HOSTILE_ACTOR ( DWORD dwCHARID )
{
	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.find ( dwCHARID );
	if ( pos!=m_mapPlayHostile.end() )
	{
		if( pos->second )
			return ( pos->second->bBAD == TRUE );
		else
			CDebugSet::ToLogFile( "GLCHARLOGIC::IS_HOSTILE_ACTOR, pos->second = NULL" );
	}

	return false;
}

//	Note : ������ ���. ( pk ���� ),	true : �ű� ��Ͻ�, false : ���� ��� ���Ž�.
bool GLCHARLOGIC::ADD_PLAYHOSTILE ( DWORD dwCHARID, BOOL bBAD )
{
	if ( m_dwCharID == dwCHARID )
	{
		CDebugSet::ToLogFile( "GLCHARLOGIC::ADD_PLAYHOSTILE, m_dwCharID == dwCHARID" );
		return false;
	}

	bool bResult = true;

	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.find ( dwCHARID );
	// �̹� ��ϵǾ� ������...
	if ( pos!=m_mapPlayHostile.end() )
	{
		bResult = false;

		//	Note : �ð��� ����. ���� bBAD�� �������� ����. ( ó�� �������� �� ����. )
		if( pos->second )
		{
			pos->second->fTIME = GLCONST_CHAR::fPK_JUSTNESS_TIME;
			return false;
		}
		else
		{
			// �̹� ��ϵǾ� �־����� ���� ��ȿ���� �ʾƼ� �����Ѵ�.
			CDebugSet::ToLogFile ("GLCHARLOGIC::ADD_PLAYHOSTILE, pos->second = NULL");
			m_mapPlayHostile.erase (pos);
		}
	}

	// ��ϵǾ� ���� �ʰų� ��ȿ���� ���� ���̾��� ������
	// �űԷ� ����Ѵ�.
	SPLAYHOSTILE* pHOSTILE = new SPLAYHOSTILE;
	pHOSTILE->bBAD = bBAD;
	pHOSTILE->fTIME = GLCONST_CHAR::fPK_JUSTNESS_TIME;
	m_mapPlayHostile.insert ( std::make_pair( dwCHARID, pHOSTILE ) );

	return bResult;
}

bool GLCHARLOGIC::DEL_PLAYHOSTILE ( DWORD dwCHARID )
{
	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.find ( dwCHARID );
	if ( pos!=m_mapPlayHostile.end() )
	{
		SAFE_DELETE( pos->second );
		m_mapPlayHostile.erase ( pos );
		return true;
	}
	else
	{	
		return false;
	}
}

void GLCHARLOGIC::DEL_PLAYHOSTILE_ALL ()
{
	for( MAPPLAYHOSTILE_ITER ci = m_mapPlayHostile.begin(); ci != m_mapPlayHostile.end(); ++ci )
		SAFE_DELETE( ci->second );
	m_mapPlayHostile.clear();
}

bool GLCHARLOGIC::IS_PLAYHOSTILE ( DWORD dwCHARID )
{
	MAPPLAYHOSTILE_ITER pos = m_mapPlayHostile.find ( dwCHARID );
	return ( pos != m_mapPlayHostile.end() );
}

EMSLOT GLCHARLOGIC::GetCurRHand()
{
	if( IsUseArmSub() ) return SLOT_RHAND_S;
	else				return SLOT_RHAND;
}

EMSLOT GLCHARLOGIC::GetCurLHand()
{
	if( IsUseArmSub() ) return SLOT_LHAND_S;
	else				return SLOT_LHAND;
}

BOOL GLCHARLOGIC::VALID_SLOT_ITEM ( EMSLOT _slot )							
{ 
	if( m_PutOnItems[_slot].sNativeID==NATIVEID_NULL() ) return FALSE;

	if( IsUseArmSub() )
	{
		if( _slot == SLOT_RHAND || _slot == SLOT_LHAND ) return FALSE;
	}
	else
	{
		if( _slot == SLOT_RHAND_S || _slot == SLOT_LHAND_S ) return FALSE;
	}

	return TRUE;
}

BOOL GLCHARLOGIC::ISLONGRANGE_ARMS ()							
{
	EMSLOT emRHand = GetCurRHand();
	//return m_pITEMS[emRHand] && ( m_pITEMS[emRHand]->sSuitOp.emAttack>ITEMATT_NEAR ); 
	if ( m_pITEMS[emRHand] && GLCONST_CHAR::bISLONGRANGE_ARMS )
	{
		switch ( m_pITEMS[emRHand]->sSuitOp.emAttack )
		{
		case ITEMATT_SWORD:
		case ITEMATT_SABER:
		case ITEMATT_SWORDSABER:
		case ITEMATT_DAGGER:
		case ITEMATT_SPEAR:
		case ITEMATT_STICK:
		case ITEMATT_GAUNT:
		case ITEMATT_BOW:
		case ITEMATT_THROW:
		case ITEMATT_DUAL_GUN:
		case ITEMATT_RAIL_GUN:
		case ITEMATT_PORTAL_GUN:
		case ITEMATT_SCYTHE:
		case ITEMATT_DUALSPEAR:
		case ITEMATT_SHURIKEN:
		case ITEMATT_FIST:
		case ITEMATT_WAND:
		case ITEMATT_CUBE:
		case ITEMATT_WHIP:
	    case ITEMATT_SHIELD:
	    case ITEMATT_HAMMER:
	    case ITEMATT_UMBRELLA:
			{
				return TRUE;
			}
		default:	return FALSE;
		};
	}
	else
	{
		return FALSE;
	}
}

BOOL GLCHARLOGIC::IsCurUseArm( EMSLOT emSlot )
{
	if( IsUseArmSub() )
	{
		if( emSlot == SLOT_RHAND || emSlot == SLOT_LHAND ) return FALSE;
	}
	else
	{
		if( emSlot == SLOT_RHAND_S || emSlot == SLOT_LHAND_S ) return FALSE;
	}

	return TRUE;
}


void GLCHARLOGIC::DISABLEALLLANDEFF()
{
	for( int i = 0; i < EMLANDEFFECT_MULTI; i++ )
	{
		m_sLandEffect[i].Init();
	}
}
void GLCHARLOGIC::ADDLANDEFF( SLANDEFFECT landEffect, int iNum )
{
	if( iNum >= EMLANDEFFECT_MULTI ) return;
	m_sLandEffect[iNum] = landEffect;
}
void GLCHARLOGIC::LEARN_SKILL_NONSCROLL ( SNATIVEID skill_id )
{
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )											return;

	SKILL::SLEARN_LVL &sSKILL_LEARN_LVL = pSkill->m_sLEARN.sLVL_STEP[0];
	if ( m_dwSkillPoint < sSKILL_LEARN_LVL.dwSKP )			return;

	SCHARSKILL* pCharSkill = GETLEARNED_SKILL( skill_id );
	if ( !pCharSkill ) 
	{
		if ( m_lnMoney < int(GLCONST_CHAR::lnSKILL_PREREQ[skill_id.wMainID][skill_id.wSubID])) return; // sSKILL_LEARN_LVL.dwMoneyReq )			return;
	}

	if ( m_lnVoteP < sSKILL_LEARN_LVL.dwVotePoint )			return;
	if ( m_lnPremP < sSKILL_LEARN_LVL.dwPremPoint )			return;

	m_dwSkillPoint -= sSKILL_LEARN_LVL.dwSKP;

	if ( !pCharSkill ) 
	{
		m_lnMoney -= int(GLCONST_CHAR::lnSKILL_PREREQ[skill_id.wMainID][skill_id.wSubID]); //sSKILL_LEARN_LVL.dwMoneyReq;
	}

	m_lnVoteP -= sSKILL_LEARN_LVL.dwVotePoint;
	m_lnPremP -= sSKILL_LEARN_LVL.dwPremPoint;

	m_ExpSkills.insert ( std::make_pair(skill_id.dwID,SCHARSKILL(skill_id,0)) );

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		INIT_DATA ( FALSE, FALSE );
	}
}
void GLCHARLOGIC::DISABLESKEFF(int i)
{
	m_sSKILLFACT[i].RESET();
}