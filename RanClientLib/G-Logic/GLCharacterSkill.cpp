#include "stdafx.h"

#include "../EngineUILib/GUInterface/Cursor.h"
#include "DxInputDevice.h"
#include "editmeshs.h"
#include "DxMethods.h"
#include "DxViewPort.h"
#include "DxEffectMan.h"
#include "DxShadowMap.h"
#include "EditMeshs.h"
#include "GLogicData.h"
#include "GLItemMan.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "stl_Func.h"
#include "DxEffGroupPlayer.h"
#include "DxEffCharData.h"

#include "../RanClientUILib/Interface/UITextControl.h"
#include "../RanClientUILib/Interface/GameTextControl.h"
#include "../RanClientUILib/Interface/InnerInterface.h"
#include "../RanClientUILib/Interface/BasicGameMenu.h"

#include "GLFactEffect.h"
#include "GLPartyClient.h"
#include "GLStrikeM.h"
#include "GLCrowData.h"

#include "GLCharacter.h"
//add abl
#include "../EngineLib/Meshs/DxSkinPieceData.h"
#include "../EngineLib/Meshs/DxSkinCharPiece.h"
#include "../EngineLib/Meshs/DxSkinPieceRootData.h"

#include "RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLCharacter::StartSkillProc ()
{
	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLAND && pLAND->IsPeaceZone() )	return;

	if ( IsSTATE(EM_ACT_PEACEMODE) )
	{
		ReqTogglePeaceMode ();
	}

	DoPASSIVITY ( TRUE );

	m_nattSTEP = 0;
	m_fattTIMER = 0.0f;

	SETACTIVESKILL ( m_sActiveSkill );

	//	Note : Ω∫≈≥ ¡§∫∏ ∞°¡Æø».
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_idACTIVESKILL.wMainID, m_idACTIVESKILL.wSubID );
	if ( !pSkill ) return;

	bool bInvi(false);
	if ( pSkill->m_sBASIC.emIMPACT_SIDE != SIDE_ENERMY )
	{
		SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( pSkill->m_sBASIC.sNATIVEID );	
		if ( pCharSkill )
		{
			WORD wLevel = pCharSkill->wLevel;
			for(int a = 0; a < SKILL::MAX_LEVEL; a++)
			{
				if ( pSkill->m_sAPPLY.sSPECS[a].emSPEC == EMSPECA_INVISIBLE )
				{
					if ( pSkill->m_sAPPLY.sSPECS[a].sSPEC[wLevel].fVAR1 != 0.0f )
					{
						bInvi = true;
						break;
					}
				}
			}
		}
	}


	m_vDir = UpdateSkillDirection ( m_vPos, m_vDir, m_idACTIVESKILL, m_vTARPOS, m_sTARIDS );

	m_emANIMAINSKILL = pSkill->m_sEXT_DATA.emANIMTYPE;
	m_emANISUBSKILL = pSkill->m_sEXT_DATA.emANISTYPE;

	PANIMCONTNODE pAnicont = m_pSkinChar->GETANI ( m_emANIMAINSKILL, m_emANISUBSKILL );
	if ( !pAnicont ) return;
	WORD wStrikeNum = pAnicont->pAnimCont->m_wStrikeCount;

	ACCOUNTSKILL ( m_idACTIVESKILL, 1 );


	STARGETID sTARG(GETCROW(),m_dwGaeaID);

	//	Note : Ω∫≈≥ Ω√¿€∞˙ µøΩ√ø° ≥™≈∏≥™¥¬ ¿Ã∆—∆Æ. πﬂµø.
	//
	EMELEMENT emELMT = ( pSkill->m_sAPPLY.emELEMENT==EMELEMENT_ARM ) ? GET_ITEM_ELMT() : (EMELEMENT_SPIRIT);

	if ( pSkill->m_sEXT_DATA.emSELFBODY==SKILL::EMTIME_FIRST )
		DxEffGroupPlayer::GetInstance().NewEffBody ( pSkill->m_sEXT_DATA.GETSELFBODY(emELMT), &sTARG, &m_vDir );

	EMSLOT emRHand = GetCurRHand();

	SANIMSTRIKE sStrike;
	sStrike.m_emPiece = VALID_SLOT_ITEM(emRHand) ? PIECE_RHAND : PIECE_GLOVE;
	sStrike.m_emEffect = EMSF_TARGET;
	sStrike.m_dwFrame = 0;

	if ( pSkill->m_sEXT_DATA.emSELFZONE01==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE01_POS, pSkill->m_sEXT_DATA.GETSELFZONE01(emELMT), &sTARG );

	if ( pSkill->m_sEXT_DATA.emSELFZONE02==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE02_POS, pSkill->m_sEXT_DATA.GETSELFZONE02(emELMT), &sTARG );

	if ( pSkill->m_sEXT_DATA.emSELFZONE03==SKILL::EMTIME_FIRST )
		SK_EFF_SELFZONE ( sStrike, pSkill->m_sEXT_DATA.emSELFZONE03_POS, pSkill->m_sEXT_DATA.GETSELFZONE03(emELMT), &sTARG );
	if( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SPEC )
	{
		EMELEMENT emELMT = ( pSkill->m_sAPPLY.emELEMENT==EMELEMENT_ARM ) ? GET_ITEM_ELMT() : (EMELEMENT_SPIRIT);

		for ( WORD i=0; i<m_wTARNUM; ++i )
		{
			STARGETID sTARGET;
			sTARGET.emCrow = m_sTARIDS[i].GETCROW();
			sTARGET.dwID = m_sTARIDS[i].GETID();

			if ( !GLGaeaClient::GetInstance().IsVisibleCV(sTARGET) )	continue;
			sTARGET.vPos = GLGaeaClient::GetInstance().GetTargetPos(sTARGET);

			if ( pSkill->m_sEXT_DATA.emTARGZONE01==SKILL::EMTIME_FIRST )
				SK_EFF_TARZONE ( sTARGET, pSkill->m_sEXT_DATA.emTARGZONE01_POS, pSkill->m_sEXT_DATA.GETTARGZONE01(emELMT) );

			if ( pSkill->m_sEXT_DATA.emTARGZONE02==SKILL::EMTIME_FIRST )
				SK_EFF_TARZONE ( sTARGET, pSkill->m_sEXT_DATA.emTARGZONE02_POS, pSkill->m_sEXT_DATA.GETTARGZONE02(emELMT) );
			//add new official feat
			if ( pSkill->m_sEXT_DATA.emTARGZONE03==SKILL::EMTIME_FIRST )
				SK_EFF_TARZONE ( sTARGET, pSkill->m_sEXT_DATA.emTARGZONE03_POS, pSkill->m_sEXT_DATA.GETTARGZONE03(emELMT) );
		}
	}

	//	Note : Ω∫≈≥ ∏ﬁΩ√¡ˆ πﬂª˝.
	//
	GLMSG::SNETPC_REQ_SKILL NetMsg;
	NetMsg.skill_id = m_idACTIVESKILL;
	NetMsg.vTARPOS = m_vTARPOS;

	if ( IsDefenseSkill() )	NetMsg.bDefenseSkill = true;
	
	bool bPKMODE(false);
	for ( WORD i=0; i<m_wTARNUM; ++i )
	{
		STARGETID sTARGET;
		sTARGET.emCrow = m_sTARIDS[i].GETCROW();
		sTARGET.dwID = m_sTARIDS[i].GETID();

		NetMsg.ADDTARGET ( m_sTARIDS[i] );

		BOOL bCONFT = ISCONFRONT_TAR(sTARGET);
		if ( sTARGET.emCrow==CROW_PC && !bCONFT )	bPKMODE = true;
	}


	NETSENDTOFIELD ( &NetMsg );
	if ( m_bINVISIBLE ) 
	{
		for( int _i = 0; _i < EMSKILLCLASS_NSIZE; _i++ )
		{
			for( int _j = 0; _j < GLSkillMan::MAX_CLASSSKILL; _j++ )
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( _i, _j );
				if ( !pSkill ) continue;

				for ( int _k = 0; _k < SKILLFACT_SIZE; _k++ )
				{
					if ( m_sSKILLFACT[_k].sNATIVEID == pSkill->m_sBASIC.sNATIVEID )
					{
						ReqDisableSkill( _k, pSkill->m_sBASIC.sNATIVEID );
						SetSTATE(EM_ACT_RUN);
						CBasicGameMenu * pGameMenu = CInnerInterface::GetInstance().GetGameMenu();
						if( pGameMenu ) pGameMenu->SetFlipRunButton( FALSE );

						m_actorMove.SetMaxSpeed ( GetMoveVelo() );
						PGLPETCLIENT pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
						if ( pMyPet->IsVALID () ) pMyPet->SetMoveState ( IsSTATE(EM_ACT_RUN) );

						GLMSG::SNETPC_MOVESTATE NetMsg;
						NetMsg.dwActState = m_dwActState;

						NETSENDTOFIELD ( (NET_MSG_GENERIC*) &NetMsg );
					}
				}
			}
		}
	}
}

// *****************************************************
// Desc: Ω∫≈≥πﬂµø∞¸∑√ √≥∏Æ
// *****************************************************
void GLCharacter::SkillReaction ( const STARGETID &sTargetID, const DWORD dwMR, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo )
{
	if ( m_bSTATE_STUN )	return;

	if ( m_bTRANSFORM_EFF ) return; //Transform Function 
	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLAND && pLAND->IsPeaceZone() )	return;

	//	Note : ∏Æø¢º« √Îº“.
	SREACTION sREC_TEMP = ( m_sREACTION.sTARID == sTargetID ) ? m_sREACTION : SREACTION();
	m_sREACTION.RESET();

	m_idACTIVESKILL = NATIVEID_NULL();

	//	Note : ¿Õ»˘ Ω∫≈≥¿Œ¡ˆ ∞ÀªÁ.
	//
	if ( !ISLEARNED_SKILL ( m_sActiveSkill ) && !IsDefenseSkill() )	return;

	const D3DXVECTOR3 &vTarPos = sTargetID.vPos;

	float fMoveDist = FLT_MAX;
	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );	// ≥ª∞° ¿Ãµø«— ∞≈∏Æ
	if ( IsACTION(GLAT_MOVE) )	fMoveDist = D3DXVec3Length ( &D3DXVECTOR3(m_sTargetID.vPos-vTarPos) ); 

	//	Note : Ω∫≈≥ ¡§∫∏ ∞°¡Æø».
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sActiveSkill.wMainID, m_sActiveSkill.wSubID );
	if ( !pSkill )								return;

	EMIMPACT_TAR emTARGET = pSkill->m_sBASIC.emIMPACT_TAR;

	if ( emTARGET==TAR_SPEC || emTARGET==TAR_SELF_TOSPEC )
	{
		GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID.emCrow, sTargetID.dwID );
		if ( pTarget )
		{
			WORD wAttackRange = pTarget->GetBodyRadius() + GETBODYRADIUS() + GETSKILLRANGE_TAR(*pSkill) + 2;
			WORD wAttackAbleDis = wAttackRange + 2;

			// ∞¯∞›¿Ã ∞°¥…«œ¡ˆ æ ¿ª ∞ÊøÏ
			if ( fDist>wAttackAbleDis )
			{
				// ≈∏∞Ÿ¿Ã øÚ¡˜¿Œ ∞≈∏Æ∞° ∞¯∞› ∞°¥…∞≈∏Æ∫∏¥Ÿ ¥ı ∏÷∏È ¥ŸΩ√ ≈∏∞Ÿ¡§∫∏ ∞ªΩ≈
				if ( fMoveDist > wAttackAbleDis )
				{
					//	Note : ¿Ãµø¡ˆ¡° ¡ˆ¡§.
					//
					D3DXVECTOR3 vPos = vTarPos - m_vPos;
					D3DXVec3Normalize ( &vPos, &vPos );

					bMove = TRUE;
					vMoveTo = vTarPos - vPos*wAttackRange;
					vMoveTo.y = vTarPos.y;

					//	Note : ∏Æø¢º« µÓ∑œ.
					//
					m_sREACTION.emREACTION = REACT_SKILL;
					m_sREACTION.bCONTINUE = bcontinue;
					m_sREACTION.sTARID = sTargetID;
					m_sREACTION.vMARK_POS = vMoveTo;
				}
				// æ∆¥œ∏È øπ¿¸ ¡§∫∏∑Œ ∞ªΩ≈
				else
				{
					m_sREACTION = sREC_TEMP;
				}

				return;
			}
		}
	}

	//	¡ˆ«¸∞˙ √Êµπ«œ¥¬¡ˆ ∞ÀªÁ.
	//	
	if ( emTARGET!=TAR_SELF && sTargetID.dwID!=EMTARGET_NULL )
	{
		D3DXVECTOR3 vPos1 = GetPosition();	vPos1.y += 15.0f;
		D3DXVECTOR3 vPos2 = vTarPos;		vPos2.y += 15.0f;
		
		BOOL bCollision = TRUE;
		D3DXVECTOR3 vCollision;
		LPDXFRAME pDxFrame = NULL;

		DxLandMan *pLandMan = GLGaeaClient::GetInstance().GetActiveMap()->GetLandMan();
		pLandMan->IsCollision ( vPos1, vPos2, vCollision, bCollision, pDxFrame, FALSE );
		if ( bCollision )						return;
	}

	PANIMCONTNODE pAnicont = m_pSkinChar->GETANI ( pSkill->m_sEXT_DATA.emANIMTYPE, pSkill->m_sEXT_DATA.emANISTYPE );
	if ( !pAnicont )
	{
		CDebugSet::ToLogFile ( "Ω∫≈≤ ø°¥œ∏ﬁ¿Ãº«¿Ã ¡∏¡¶«œ¡ˆ æ Ω¿¥œ¥Ÿ. ANIM %d, ANIS %d", pSkill->m_sEXT_DATA.emANIMTYPE, pSkill->m_sEXT_DATA.emANISTYPE );
		return;
	}

	WORD wStrikeNum = pAnicont->pAnimCont->m_wStrikeCount;

	//	Note : Ω∫≈≥ ºˆ«‡ ∞°¥… ∞ÀªÁ.
	//
	EMSKILLCHECK emSkillCheck = GLCHARLOGIC::CHECHSKILL ( m_sActiveSkill, 1, IsDefenseSkill() );
	if ( emSkillCheck!=EMSKILL_OK && emSkillCheck!=EMSKILL_NOTSP)
	{
		if ( dwMR&(DXKEY_UP) )
		{
			switch ( emSkillCheck )
			{
			case EMSKILL_DELAYTIME:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_DELAYTIME") );
				break;

			case EMSKILL_NOTITEM:
				{	

					if ( pSkill->m_sBASIC.emUSE_LITEM!=ITEMATT_NOCARE && pSkill->m_sBASIC.emUSE_RITEM!=ITEMATT_NOCARE )
					{
						CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTITEM_2"),
							COMMENT::ITEMATTACK[pSkill->m_sBASIC.emUSE_LITEM].c_str(), COMMENT::ITEMATTACK[pSkill->m_sBASIC.emUSE_RITEM].c_str() );
					}
					else if ( pSkill->m_sBASIC.emUSE_LITEM!=ITEMATT_NOCARE || pSkill->m_sBASIC.emUSE_RITEM!=ITEMATT_NOCARE )
					{
						std::string strITEM;
						if ( pSkill->m_sBASIC.emUSE_LITEM!=ITEMATT_NOCARE )			strITEM = COMMENT::ITEMATTACK[pSkill->m_sBASIC.emUSE_LITEM].c_str();
						if ( pSkill->m_sBASIC.emUSE_RITEM!=ITEMATT_NOCARE )			strITEM = COMMENT::ITEMATTACK[pSkill->m_sBASIC.emUSE_RITEM].c_str();

						if ( pSkill->m_sLEARN.sWEAPON.wMainID != 65535 && 
							 pSkill->m_sLEARN.sWEAPON.wSubID != 65535 )				strITEM += " / Fist";
						if ( strITEM.empty() )		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTITEM") );
						CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTITEM_1"), strITEM.c_str() );
					}
				}
				break;

			case EMSKILL_NOTARROW:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTITEM_ARROW") );
				break;
				//add bullet
			case EMSKILL_NOTBULLET:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTITEM_BULLET") );
				break;
				//

			case EMSKILL_NOTCHARM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTITEM_CHARM") );
				break;

			case EMSKILL_NOTHP:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTHP") );
				break;

			case EMSKILL_NOTMP:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTMP") );
				break;

			case EMSKILL_NOTCP:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTCP") );
				break;

			case EMSKILL_NOTSP:
				GASSERT(0&&"SP∞° ∫Œ¡∑«œ¥ı∂Ûµµ Ω∫≈≥¿∫ πﬂµøµ«æﬂ «’¥œ¥Ÿ.");
				break;

			case EMSKILL_NOTEXP:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTEXP") );
				break;

			case EMSKILL_NOTSKILL: //add antiskill
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_NOTSKILL") );
				break;

			default:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILL_DOFAIL") );
				break;
			};
		}
		return;
	}

	//	Note : ≈¨∏Ø¡ˆ¡° √£±‚.
	//
	D3DXVECTOR3 *pvTarPos = NULL;
	D3DXVECTOR3 vTargetPt, vFromPt;
	vFromPt = DxViewPort::GetInstance().GetFromPt ();
	BOOL bOk = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vTargetPt );
	if ( bOk )
	{
		D3DXVECTOR3 vCollisionPos;
		DWORD dwCollisionID;
		BOOL bCollision;
		GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh()->IsCollision
		(
			vFromPt,
			vTargetPt,
			vCollisionPos,
			dwCollisionID,
			bCollision
		);
		if ( bCollision )	pvTarPos = &vCollisionPos;
	}

	//	Note : Skill ∏Ò«•∏¶ º±≈√«‘.
	//
	BOOL bOK = SelectSkillTarget ( sTargetID, pvTarPos );
	if ( !bOK )
	{
		//	∏Ò«•π∞¿Ã æ¯∞≈≥™ ∏Ò«• ¡ˆ¡°¿Ã ¿Ø»ø«œ¡ˆ æ ¿ª∂ß.
		if ( dwMR&(DXKEY_UP) )		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("NOTARGET_SKILL") );
		return;
	}


	//	Note : πÊ«‚ »∏¿¸¿Ã « ø‰«“ ∞ÊøÏ πÊ«‚¿ª ∆∞¥Ÿ.
	//
	if ( emTARGET==TAR_SPEC || emTARGET==TAR_SELF_TOSPEC )
	{
		if ( m_sTARIDS[0].wID != USHRT_MAX )
		{
			STARGETID sRotateTarID ( static_cast<EMCROW>(m_sTARIDS[0].wCrow), static_cast<DWORD>(m_sTARIDS[0].wID) );
			GLCOPY* pTarget = GLGaeaClient::GetInstance().GetCopyActor ( sRotateTarID );
			if ( pTarget )
			{
				D3DXVECTOR3 vDirection = pTarget->GetPosition() - m_vPos;
				D3DXVec3Normalize ( &vDirection, &vDirection );
				m_vDir = vDirection;
			}
		}

		//	Note : «ˆ¡¶ ¿ßƒ°øÕ πÊ«‚¿∏∑Œ Transform ∏ﬁ∆Æ∏ØΩ∫ ∞ËªÍ.
		//
		D3DXMATRIX matTrans, matYRot;
		D3DXMatrixTranslation ( &matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
		float fThetaY = DXGetThetaYFromDirection ( m_vDir, m_vDirOrig );
		D3DXMatrixRotationY ( &matYRot, fThetaY );

		m_matTrans = matYRot * matTrans;
	}
	else if ( emTARGET==TAR_ZONE_SPEC )
	{
		D3DXVECTOR3 vDirection = m_vTARPOS - m_vPos;
		D3DXVec3Normalize ( &vDirection, &vDirection );
		m_vDir = vDirection;
	}
	else if ( emTARGET==TAR_SELF )
	{
		//	Note : πÊ«‚ »∏¿¸ « ø‰ æ¯¿Ω.
	}

	//	Note : Ω∫≈≥¿Ã πﬂµøµ .
	//
	
	TurnAction ( GLAT_SKILL );

	if ( bcontinue )
	{
		m_sREACTION.emREACTION = REACT_SKILL;
		m_sREACTION.bCONTINUE = true;
		m_sREACTION.sTARID = sTargetID;
		m_sREACTION.vMARK_POS = m_vPos;
	}

	return;
}

void GLCharacter::SKT_EFF_HOLDOUT ( STARGETID sTarget, DWORD dwDamageFlag )
{
	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		SSKILLFACT &sSKEFF = m_sSKILLFACT[i];
		if ( sSKEFF.sNATIVEID==NATIVEID_NULL() )	continue;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSKEFF.sNATIVEID.wMainID, sSKEFF.sNATIVEID.wSubID );
		if ( !pSkill )								continue;

		if ( !pSkill->m_sEXT_DATA.strHOLDOUT.empty() )
		{
			for(int a=0; a < SKILL::MAX_LEVEL; a++)
			{
				if ( (pSkill->m_sAPPLY.sSPECS[a].emSPEC == EMSPECA_PSY_DAMAGE_REDUCE && 
					!( dwDamageFlag & DAMAGE_TYPE_PSY_REDUCE) ) )
				continue;
			
				if ( (pSkill->m_sAPPLY.sSPECS[a].emSPEC == EMSPECA_MAGIC_DAMAGE_REDUCE && 
					!( dwDamageFlag & DAMAGE_TYPE_MAGIC_REDUCE) ) )
					continue;

				if ( (pSkill->m_sAPPLY.sSPECS[a].emSPEC == EMSPECA_PSY_DAMAGE_REFLECTION && 
					!( dwDamageFlag & DAMAGE_TYPE_PSY_REFLECTION) ) )
					continue;

				if ( (pSkill->m_sAPPLY.sSPECS[a].emSPEC == EMSPECA_MAGIC_DAMAGE_REFLECTION && 
					!( dwDamageFlag & DAMAGE_TYPE_MAGIC_REFLECTION) ) )
					continue;
			}

			const char* szZONE_EFF = pSkill->m_sEXT_DATA.strHOLDOUT.c_str();

			D3DXVECTOR3 vDIR = sTarget.vPos - m_vPos;

			D3DXVECTOR3 vDIR_ORG(1,0,0);
			float fdir_y = DXGetThetaYFromDirection ( vDIR, vDIR_ORG );

			D3DXMATRIX matTrans;
			D3DXMatrixRotationY ( &matTrans, fdir_y );
			matTrans._41 = m_vPos.x;
			matTrans._42 = m_vPos.y;
			matTrans._43 = m_vPos.z;

			//	Note : ¿⁄±‚ ¿ßƒ° ¿Ã∆Â∆Æ πﬂª˝Ω√≈¥.
			DxEffGroupPlayer::GetInstance().NewEffGroup ( szZONE_EFF, matTrans, &sTarget );
		}
	}
}

void GLCharacter::SK_EFF_SELFZONE ( const SANIMSTRIKE &sStrikeEff, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF, STARGETID *pTarget )
{
	D3DXMATRIX matTrans;

	//	Ω∫∆Æ∂Û¿Ã≈© ¡ˆ¡°ø°º≠.
	BOOL bTRANS(FALSE);
	if ( emPOS == SKILL::EMPOS_STRIKE )
	{
		//	≈∏∞Ÿ¿« ¿ßƒ°.
		D3DXVECTOR3 vTARPOS = m_vPos + m_vDir * 60.0f;

		//	≈∏∞› ¿ßƒ°∏¶ æÀæ∆≥ø.
		STRIKE::SSTRIKE sStrike;

		if ( m_pSkinChar->m_pSkinCharPiece[PIECE_RHAND] ) //fix abl strike
		{
			STRIKE::CSELECTOR Selector ( vTARPOS, m_pSkinChar->m_pSkinCharPiece[PIECE_RHAND]->m_PartArray );
			BOOL bOk = Selector.SELECT ( sStrikeEff, sStrike ,true);
			if ( bOk )
			{
				bTRANS = TRUE;

				D3DXMatrixTranslation ( &matTrans, sStrike.vPos.x, sStrike.vPos.y, sStrike.vPos.z );
			}
		}
		else if ( m_pSkinChar->m_pSkinCharPiece[PIECE_REBOOT_RHAND] ) //fix abl strike
		{
			STRIKE::CSELECTOR Selector ( vTARPOS, m_pSkinChar->m_pSkinCharPiece[PIECE_REBOOT_RHAND]->m_PartArray );
			BOOL bOk = Selector.SELECT ( sStrikeEff, sStrike ,true);
			if ( bOk )
			{
				bTRANS = TRUE;

				D3DXMatrixTranslation ( &matTrans, sStrike.vPos.x, sStrike.vPos.y, sStrike.vPos.z );
			}
		}
		else
		{
			STRIKE::CSELECTOR Selector ( vTARPOS, m_pSkinChar->m_PartArray );
			BOOL bOk = Selector.SELECT ( sStrikeEff, sStrike ,false);
			if ( bOk )
			{
				bTRANS = TRUE;

				D3DXMatrixTranslation ( &matTrans, sStrike.vPos.x, sStrike.vPos.y, sStrike.vPos.z );
			}
		}
		
	}

	//	¡ˆ∏È±‚¡ÿ ∏Ò«•¡°ø° »∏¿¸ º”º∫ ≥÷æÓº≠.
	if ( !bTRANS )
	{
		bTRANS = TRUE;

		D3DXMATRIX matYRot;
		D3DXMatrixRotationY ( &matYRot, D3DX_PI/2.0f );
		matTrans = matYRot * m_matTrans;
	}

	//	Note : ¿⁄±‚ ¿ßƒ° ¿Ã∆Â∆Æ πﬂª˝Ω√≈¥.
	DxEffGroupPlayer::GetInstance().NewEffGroup ( szZONE_EFF, matTrans, pTarget );
}

void GLCharacter::SK_EFF_TARZONE ( const STARGETID &_sTarget, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF )
{
	D3DXMATRIX matTrans;
	STARGETID sTarget = _sTarget;
	D3DXVECTOR3 vDir = m_vPos - sTarget.vPos;

	//	Ω∫∆Æ∂Û¿Ã≈© ¡ˆ¡°ø°º≠.
	BOOL bTRANS(FALSE);
	if ( emPOS == SKILL::EMPOS_STRIKE )
	{
		DxSkinChar* pSkin = GLGaeaClient::GetInstance().GetSkinChar ( sTarget );

		//	≈∏∞Ÿ¿« ¿ßƒ°.
		D3DXVECTOR3 vTARPOS ( m_vPos.x, m_vPos.y+15, m_vPos.z );;

		//	≈∏∞› ¿ßƒ°∏¶ æÀæ∆≥ø.
		IMPACT_ARROW::SSTRIKE sStrike;
		IMPACT_ARROW::CSELECTOR Selector ( vTARPOS, pSkin->m_PartArray );
		BOOL bOk = Selector.SELECT ( sStrike );
		if ( bOk )
		{
			bTRANS = TRUE;

			D3DXMatrixTranslation ( &matTrans, sStrike.vPos.x, sStrike.vPos.y, sStrike.vPos.z );
			sTarget.vPos = vTARPOS;
		}
	}

	//	¡ˆ∏È±‚¡ÿ ∏Ò«•¡°.
	if ( !bTRANS )
	{
		bTRANS = TRUE;
		D3DXMatrixTranslation ( &matTrans, sTarget.vPos.x, sTarget.vPos.y, sTarget.vPos.z );
	}

	//	Note : ∏Ò«• ¿ßƒ° ¿Ã∆Â∆Æ πﬂª˝Ω√≈¥.
	DxEffGroupPlayer::GetInstance().NewEffGroup ( szZONE_EFF, matTrans, &sTarget );
}

void GLCharacter::SK_EFF_TARG ( const PGLSKILL pSkill, const SANIMSTRIKE &sStrikeEff, const STARGETID &_sTarget )
{
	D3DXMATRIX matTrans;
	STARGETID sTarget = _sTarget;
	D3DXVECTOR3 vDir = m_vPos - sTarget.vPos;

	//	Ω∫∆Æ∂Û¿Ã≈© ¡ˆ¡°ø°º≠.
	BOOL bTRANS(FALSE);
	if ( pSkill->m_sEXT_DATA.emTARG_POSA == SKILL::EMPOS_STRIKE )
	{
		//	≈∏∞Ÿ¿« ¿ßƒ°.
		D3DXVECTOR3 vSTRPOS ( sTarget.vPos.x, sTarget.vPos.y+15, sTarget.vPos.z );;

		D3DXVECTOR3 vDir = vSTRPOS - m_vPos;
		float fLength = D3DXVec3Length ( &vDir );

		D3DXVec3Normalize ( &vDir, &vDir );
		D3DXVECTOR3 vSTRPAR = m_vPos + vDir*fLength * 10.0f;

		//	Ω∫∆Æ∂Û¿Ã≈© ¿ßƒ°∏¶ æÀæ∆≥ø.
		STRIKE::SSTRIKE sStrike;
		if ( m_pSkinChar->m_pSkinCharPiece[PIECE_RHAND] ) //fix abl strike
		{

			STRIKE::CSELECTOR Selector ( vSTRPAR, m_pSkinChar->m_pSkinCharPiece[PIECE_RHAND]->m_PartArray );
			BOOL bOk = Selector.SELECT ( sStrikeEff, sStrike ,true);
			if ( bOk )
			{
				bTRANS = TRUE;
				D3DXMatrixTranslation ( &matTrans, sStrike.vPos.x, sStrike.vPos.y, sStrike.vPos.z );
			}
		}
		else if ( m_pSkinChar->m_pSkinCharPiece[PIECE_REBOOT_RHAND] ) //fix abl strike
		{

			STRIKE::CSELECTOR Selector ( vSTRPAR, m_pSkinChar->m_pSkinCharPiece[PIECE_REBOOT_RHAND]->m_PartArray );
			BOOL bOk = Selector.SELECT ( sStrikeEff, sStrike ,true);
			if ( bOk )
			{
				bTRANS = TRUE;
				D3DXMatrixTranslation ( &matTrans, sStrike.vPos.x, sStrike.vPos.y, sStrike.vPos.z );
			}
		}
		else
		{
			STRIKE::CSELECTOR Selector ( vSTRPAR, m_pSkinChar->m_PartArray );
			BOOL bOk = Selector.SELECT ( sStrikeEff, sStrike ,false);
			if ( bOk )
			{
				bTRANS = TRUE;
				D3DXMatrixTranslation ( &matTrans, sStrike.vPos.x, sStrike.vPos.y, sStrike.vPos.z );
			}
		}
	}
	
	//	¡ˆ∏È±‚¡ÿ »∏¿¸ º”º∫ ≥÷æÓº≠.
	if ( !bTRANS )
	{
		bTRANS = TRUE;

		D3DXMATRIX matYRot;
		D3DXMatrixRotationY ( &matYRot, D3DX_PI/2.0f );
		matTrans = matYRot * m_matTrans;
	}

	//	∏Ò«• ¡ˆ¡° º±≈√
	BOOL bIMPACT(FALSE);
	if ( pSkill->m_sEXT_DATA.emTARG_POSB == SKILL::EMPOS_STRIKE )
	{
		DxSkinChar* pSkin = GLGaeaClient::GetInstance().GetSkinChar ( sTarget );
		if ( !pSkin )	return;

		//	≈∏∞Ÿ¿« ¿ßƒ°.
		D3DXVECTOR3 vTARPOS ( m_vPos.x, m_vPos.y+15, m_vPos.z );;

		//	≈∏∞› ¿ßƒ°∏¶ æÀæ∆≥ø.
		IMPACT_ARROW::SSTRIKE sImpact;
		IMPACT_ARROW::CSELECTOR Selmpact ( vTARPOS, pSkin->m_PartArray );
		BOOL bOk = Selmpact.SELECT ( sImpact );
		if ( bOk )
		{
			bIMPACT = TRUE;
			sTarget.vPos = sImpact.vPos;
		}
	}

	if ( !bIMPACT )
	{
		sTarget.vPos = sTarget.vPos;
		sTarget.vPos.y += 15.0f;
	}

//	D3DXVECTOR3 vDir = sTarget.vPos - m_vPos;
	vDir.y = 0.f;
	D3DXVec3Normalize ( &vDir, &vDir );

	EMELEMENT emELMT = ( pSkill->m_sAPPLY.emELEMENT==EMELEMENT_ARM ) ? GET_ITEM_ELMT() : (EMELEMENT_SPIRIT);
	DxEffSingleGroup* pEffSingleG = DxEffGroupPlayer::GetInstance().NewEffGroup ( pSkill->m_sEXT_DATA.GETTARG(emELMT), matTrans, &sTarget );
	if ( !pEffSingleG )		return;

	//	Note : ∏Ò«• ¿ßƒ° ¿Ã∆Â∆Æ.
	if ( pSkill->m_sEXT_DATA.emTARGZONE01==SKILL::EMTIME_TARG_OVR )
		pEffSingleG->AddEffAfter ( EFFASINGLE, pSkill->m_sEXT_DATA.GETTARGZONE01(emELMT) );

	if ( pSkill->m_sEXT_DATA.emTARGZONE02==SKILL::EMTIME_TARG_OVR )
		pEffSingleG->AddEffAfter ( EFFASINGLE, pSkill->m_sEXT_DATA.GETTARGZONE02(emELMT) );
	//add new official feat
	if ( pSkill->m_sEXT_DATA.emTARGZONE03==SKILL::EMTIME_TARG_OVR )
		pEffSingleG->AddEffAfter ( EFFASINGLE, pSkill->m_sEXT_DATA.GETTARGZONE03(emELMT) );

	//	Note : ∏Ò«• ∏ˆ ¿Ã∆Â∆Æ.
	if ( pSkill->m_sEXT_DATA.emTARGBODY01==SKILL::EMTIME_TARG_OVR )
		pEffSingleG->AddEffAfter ( EFFABODY, pSkill->m_sEXT_DATA.GETTARGBODY01(emELMT) );

	if ( pSkill->m_sEXT_DATA.emTARGBODY02==SKILL::EMTIME_TARG_OVR )
		pEffSingleG->AddEffAfter ( EFFABODY, pSkill->m_sEXT_DATA.GETTARGBODY02(emELMT) );
}

void GLCharacter::SkillEffect ( const SANIMSTRIKE &sStrikeEff )
{
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_idACTIVESKILL.wMainID, m_idACTIVESKILL.wSubID );
	if ( !pSkill )							return;

	D3DXMATRIX matTrans;
	using namespace SKILL;

	EMELEMENT emELMT = ( pSkill->m_sAPPLY.emELEMENT==EMELEMENT_ARM ) ? GET_ITEM_ELMT() : (EMELEMENT_SPIRIT);

	//	Note : ≈∏∞Ÿ ¿Ã∆Â∆Æ∞° ¡ˆ∏È¿ª «‚«ÿ πﬂªÁµ«¥¬ ∞ÊøÏ.
	//
	if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_ZONE_SPEC )
	{
		if ( pSkill->m_sEXT_DATA.emTARG == EMTIME_IMPACT )
		{
			D3DXMatrixTranslation ( &matTrans, m_vTARPOS.x, m_vTARPOS.y, m_vTARPOS.z );
			DxEffSingleGroup* pEffSingleG = DxEffGroupPlayer::GetInstance().NewEffGroup ( pSkill->m_sEXT_DATA.GETSELFZONE01(emELMT), matTrans );
			if ( pEffSingleG )
			{
				if ( pSkill->m_sEXT_DATA.emTARGZONE01==EMTIME_TARG_OVR )
					pEffSingleG->AddEffAfter ( EFFASINGLE, pSkill->m_sEXT_DATA.GETTARGZONE01(emELMT) );

				if ( pSkill->m_sEXT_DATA.emTARGZONE02==EMTIME_TARG_OVR )
					pEffSingleG->AddEffAfter ( EFFASINGLE, pSkill->m_sEXT_DATA.GETTARGZONE02(emELMT) );
				if ( pSkill->m_sEXT_DATA.emTARGZONE03==EMTIME_TARG_OVR )
					pEffSingleG->AddEffAfter ( EFFASINGLE, pSkill->m_sEXT_DATA.GETTARGZONE03(emELMT) );
			}
		}

		if ( pSkill->m_sEXT_DATA.emTARGZONE01==EMTIME_IMPACT )
			DxEffGroupPlayer::GetInstance().NewEffGroup ( pSkill->m_sEXT_DATA.GETTARGZONE01(emELMT), matTrans );

		if ( pSkill->m_sEXT_DATA.emTARGZONE02==EMTIME_IMPACT )
			DxEffGroupPlayer::GetInstance().NewEffGroup ( pSkill->m_sEXT_DATA.GETTARGZONE02(emELMT), matTrans );
		if ( pSkill->m_sEXT_DATA.emTARGZONE03==EMTIME_IMPACT )
			DxEffGroupPlayer::GetInstance().NewEffGroup ( pSkill->m_sEXT_DATA.GETTARGZONE03(emELMT), matTrans );
	}
	//	Note : ≈∏∞Ÿ ¿Ã∆Â∆Æ∞° ¡ˆ∏È¿ª «‚«ÿ πﬂªÁµ«¥¬ ∞ÊøÏ.
	//		¿⁄±‚ ¿ßƒ°ø°º≠ ¥ÎªÛ ¿ßƒ°±Ó¡ˆ. ( »≠ªÏ ∞¸≈Î )
	else if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF_TOSPEC )
	{
		if ( m_wTARNUM == 0 )	return;

		STARGETID sTarget(static_cast<EMCROW>(m_sTARIDS[m_wTARNUM-1].wCrow),static_cast<DWORD>(m_sTARIDS[m_wTARNUM-1].wID));
		if ( !GLGaeaClient::GetInstance().IsVisibleCV(sTarget) )		return;
		sTarget.vPos = GLGaeaClient::GetInstance().GetTargetPos(sTarget);

		D3DXVECTOR3 vDir = sTarget.vPos - m_vPos;
		vDir.y = 0.f;
		D3DXVec3Normalize ( &vDir, &vDir );

		//	Note : ∏Ò«• ¡ˆ«‚ ¿Ã∆Â∆Æø° Ω«æÓ ∫∏≥ª¥¬ ¿Ã∆Â∆Æ.
		//		( Ω«æÓ ∫∏≥æ∂ß¥¬ ∏Ò«• ¿ßƒ° ¿Ã∆Â∆Æ¿« ¿ßƒ°¥¬ ¿⁄µø¿∏∑Œ ¡ˆ«‚ ¿Ã∆—∆Æ¿« ¡æ∑· ¡ˆ¡°¿∏∑Œ ∞Ì¡§. )
		if ( pSkill->m_sEXT_DATA.VALIDTARG(emELMT) )
		{
			SK_EFF_TARG ( pSkill, sStrikeEff, sTarget );
		}
		//	¥‹µ∂¿∏∑Œ ∏Ò«• ¡ˆ¡°ø° πﬂª˝.
		else
		{
			//	Note : ∏Ò«• ¿ßƒ° ¿Ã∆Â∆Æ.
			if ( pSkill->m_sEXT_DATA.emTARGZONE01==EMTIME_IMPACT )
				SK_EFF_TARZONE ( sTarget, pSkill->m_sEXT_DATA.emTARGZONE01_POS, pSkill->m_sEXT_DATA.GETTARGZONE01(emELMT) );

			if ( pSkill->m_sEXT_DATA.emTARGZONE02==EMTIME_IMPACT )
				SK_EFF_TARZONE ( sTarget, pSkill->m_sEXT_DATA.emTARGZONE02_POS, pSkill->m_sEXT_DATA.GETTARGZONE02(emELMT) );

			if ( pSkill->m_sEXT_DATA.emTARGZONE03==EMTIME_IMPACT )
				SK_EFF_TARZONE ( sTarget, pSkill->m_sEXT_DATA.emTARGZONE03_POS, pSkill->m_sEXT_DATA.GETTARGZONE03(emELMT) );


			//	Note : ∏Ò«• ∏ˆ ¿Ã∆Â∆Æ.
			if ( pSkill->m_sEXT_DATA.emTARGBODY01==EMTIME_IMPACT )
				DxEffGroupPlayer::GetInstance().NewEffBody ( pSkill->m_sEXT_DATA.GETTARGBODY01(emELMT), &sTarget, &vDir );
			if ( pSkill->m_sEXT_DATA.emTARGBODY02==EMTIME_IMPACT )
				DxEffGroupPlayer::GetInstance().NewEffBody ( pSkill->m_sEXT_DATA.GETTARGBODY02(emELMT), &sTarget, &vDir );
		}
	}
	//	Note : ≈∏∞Ÿ ¿Ã∆Â∆Æ∞° ¿Ø¥÷¿ª «‚«ÿ πﬂªÁµ«¥¬ ∞ÊøÏ.
	//
	else
	{
		//	Note : ∏Ò«• ¿Ø¥÷¿Ã ¿÷¥¬ ∞ÊøÏ.
		//
		if ( m_wTARNUM )
		{
			for ( WORD i=0; i<m_wTARNUM; ++i )
			{
				STARGETID sTarget(static_cast<EMCROW>(m_sTARIDS[i].wCrow),static_cast<DWORD>(m_sTARIDS[i].wID));
				if ( !GLGaeaClient::GetInstance().IsVisibleCV(sTarget) )	continue;
				sTarget.vPos = GLGaeaClient::GetInstance().GetTargetPos(sTarget);

				D3DXVECTOR3 vDir = sTarget.vPos - m_vPos;
				vDir.y = 0.f;
				D3DXVec3Normalize ( &vDir, &vDir );

				//	Note : ∏Ò«• ¡ˆ«‚ ¿Ã∆Â∆Æø° Ω«æÓ ∫∏≥ª¥¬ ¿Ã∆Â∆Æ.
				//		( Ω«æÓ ∫∏≥æ∂ß¥¬ ∏Ò«• ¿ßƒ° ¿Ã∆Â∆Æ¿« ¿ßƒ°¥¬ ¿⁄µø¿∏∑Œ ¡ˆ«‚ ¿Ã∆—∆Æ¿« ¡æ∑· ¡ˆ¡°¿∏∑Œ ∞Ì¡§. )
				if ( pSkill->m_sEXT_DATA.VALIDTARG(emELMT) )
				{
					SK_EFF_TARG ( pSkill, sStrikeEff, sTarget );
				}
				//	¥‹µ∂¿∏∑Œ ∏Ò«• ¡ˆ¡°ø° πﬂª˝.
				else
				{
					//	Note : ∏Ò«• ¿ßƒ° ¿Ã∆Â∆Æ.
					if ( pSkill->m_sEXT_DATA.emTARGZONE01==EMTIME_IMPACT )
						SK_EFF_TARZONE ( sTarget, pSkill->m_sEXT_DATA.emTARGZONE01_POS, pSkill->m_sEXT_DATA.GETTARGZONE01(emELMT) );

					if ( pSkill->m_sEXT_DATA.emTARGZONE02==EMTIME_IMPACT )
						SK_EFF_TARZONE ( sTarget, pSkill->m_sEXT_DATA.emTARGZONE02_POS, pSkill->m_sEXT_DATA.GETTARGZONE02(emELMT) );

					if ( pSkill->m_sEXT_DATA.emTARGZONE03==EMTIME_IMPACT )
						SK_EFF_TARZONE ( sTarget, pSkill->m_sEXT_DATA.emTARGZONE03_POS, pSkill->m_sEXT_DATA.GETTARGZONE03(emELMT) );

					//	Note : ∏Ò«• ∏ˆ ¿Ã∆Â∆Æ.
					if ( pSkill->m_sEXT_DATA.emTARGBODY01==EMTIME_IMPACT )
						DxEffGroupPlayer::GetInstance().NewEffBody ( pSkill->m_sEXT_DATA.GETTARGBODY01(emELMT), &sTarget, &vDir );
					if ( pSkill->m_sEXT_DATA.emTARGBODY02==EMTIME_IMPACT )
						DxEffGroupPlayer::GetInstance().NewEffBody ( pSkill->m_sEXT_DATA.GETTARGBODY02(emELMT), &sTarget, &vDir );
				}

				if ( pSkill->m_sEXT_DATA.bTARG_ONE )	break;
			}
		}
	}


	STARGETID sTARG(GETCROW(),m_dwGaeaID);

	if ( pSkill->m_sEXT_DATA.emSELFZONE01 == EMTIME_IMPACT )
		SK_EFF_SELFZONE ( sStrikeEff, pSkill->m_sEXT_DATA.emSELFZONE01_POS, pSkill->m_sEXT_DATA.GETSELFZONE01(emELMT), &sTARG );

	if ( pSkill->m_sEXT_DATA.emSELFZONE02 == EMTIME_IMPACT )
		SK_EFF_SELFZONE ( sStrikeEff, pSkill->m_sEXT_DATA.emSELFZONE02_POS, pSkill->m_sEXT_DATA.GETSELFZONE02(emELMT), &sTARG );

	if ( pSkill->m_sEXT_DATA.emSELFZONE03 == EMTIME_IMPACT )
		SK_EFF_SELFZONE ( sStrikeEff, pSkill->m_sEXT_DATA.emSELFZONE03_POS, pSkill->m_sEXT_DATA.GETSELFZONE03(emELMT), &sTARG );
	if ( pSkill->m_sEXT_DATA.emSELFBODY == EMTIME_IMPACT )
		DxEffGroupPlayer::GetInstance().NewEffBody ( pSkill->m_sEXT_DATA.GETSELFBODY(emELMT), &sTARG, &m_vDir );
}

BOOL GLCharacter::SkillProc ( float fElapsedTime )
{
	m_fattTIMER += fElapsedTime * GLCHARLOGIC::GETATTVELO();
	m_fattTIMER += GLCHARLOGIC::GETATT_ITEM();

	GASSERT(m_pSkinChar->GETCURANIMNODE());

	PANIMCONTNODE pAnicontNode = m_pSkinChar->GETCURANIMNODE();
	GASSERT(pAnicontNode);
	PSANIMCONTAINER pAnimCont = pAnicontNode->pAnimCont;
	
	if ( pAnimCont->m_wStrikeCount == 0 )
	{
		return TRUE;		// øπ¿¸ø°¥¬ FALSE ø©º≠ IDLE ªÛ≈¬∑Œ πŸ≤Óæ˙¥Ÿ. ¡ˆ±›¿∫ TRUE∏¶ ¡÷æÓº≠ ªÛ≈¬∫Ø»≠∏¶ æ»¡ÿ¥Ÿ.
	}

	int nTotalKeys = int(pAnimCont->m_dwETime) - int(pAnimCont->m_dwSTime);
	int nThisKey = int(pAnimCont->m_dwSTime) + int(m_fattTIMER*UNITANIKEY_PERSEC);

	int nStrikeKey = (int) pAnimCont->m_sStrikeEff[m_nattSTEP].m_dwFrame;
	if ( m_nattSTEP < pAnimCont->m_wStrikeCount )
	{
		if ( nThisKey>=nStrikeKey )
		{
			//	Note : Ω∫≈≥ ¿Ã∆Â∆Æ πﬂµø.
			//
			SkillEffect ( pAnimCont->m_sStrikeEff[m_nattSTEP] );
		
			m_nattSTEP++;
		}
	}

	if ( nThisKey >= int(pAnimCont->m_dwETime) )
	{
		if ( pAnimCont->m_dwFlag&ACF_LOOP )
		{
			m_fattTIMER = 0.0f;
			return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL GLCharacter::RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, const DWORD dwSELECT )
{
	//	Ω∫≈≥ ¡§∫∏ ∞°¡Æø».
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )				return FALSE;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[wlevel];

	BOOL bHOLD = FALSE;
	SSKILLFACT sSKILLEF;
	switch ( pSkill->m_sAPPLY.emBASIC_TYPE )
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
	case SKILL::EMFOR_RESIST:
	case SKILL::EMFOR_CP_RATE: //add cp
	case SKILL::EMFOR_CP_GAIN: //add cp
		bHOLD = TRUE;
		sSKILLEF.emTYPE = pSkill->m_sAPPLY.emBASIC_TYPE;
		sSKILLEF.fMVAR = sSKILL_DATA.fBASIC_VAR;
		break;
	};

	int _nIndex = 0;
	for(int a = 0; a < SKILL::MAX_LEVEL; a++ )
	{
		if ( pSkill->m_sAPPLY.sADDONS[a].emADDON != EMIMPACTA_NONE )
		{
			bHOLD = TRUE;
			sSKILLEF.emADDON[_nIndex] = pSkill->m_sAPPLY.sADDONS[a].emADDON;
			sSKILLEF.fADDON_VAR[_nIndex] = pSkill->m_sAPPLY.sADDONS[a].fADDON_VAR[wlevel];
			_nIndex += 1;
		}
	}

	int nIndex = 0;
	for(int a = 0; a < SKILL::MAX_LEVEL; a++)
	{
		switch ( pSkill->m_sAPPLY.sSPECS[a].emSPEC )
		{
		case EMSPECA_REFDAMAGE:
		case EMSPECA_NONBLOW:
		case EMSPECA_PIERCE:
		case EMSPECA_TARRANGE:
		case EMSPECA_MOVEVELO:
		case EMSPECA_ATTACKVELO:
		case EMSPECA_SKILLDELAY:
		case EMSPECA_PSY_DAMAGE_REDUCE:
		case EMSPECA_MAGIC_DAMAGE_REDUCE:
		case EMSPECA_PSY_DAMAGE_REFLECTION:
		case EMSPECA_MAGIC_DAMAGE_REFLECTION:
		case EMSPECA_DEFENSE_SKILL_ACTIVE:
		case EMSPECA_NOPOTION: //add antipot
		case EMSPECA_NOSKILL: //add antiskill
		case EMSPECA_RECVISIBLE: //add invi-anti
		case EMSPECA_INVISIBLE: //add invi-anti
		case EMSPECA_STUN:
		case EMSPECA_IMMUNE:
		case EMSPECA_CONTINUEDAMAGE:
		case EMSPECA_CURSE:
		case EMSPECA_GETEXP:
		case EMSPECA_ATTACK_INT:
		case EMSPECA_ATTACK_RATE:
		case EMSPECA_DEF_INT:
		case EMSPECA_DEF_RATE:
		case EMSPECA_ACCURACY_RATE:
		case EMSPECA_EVASION_RATE:
			bHOLD = TRUE;
			sSKILLEF.emSPEC[nIndex] = pSkill->m_sAPPLY.sSPECS[a].emSPEC;
			sSKILLEF.fSPECVAR1[nIndex] = pSkill->m_sAPPLY.sSPECS[a].sSPEC[wlevel].fVAR1;
			sSKILLEF.fSPECVAR2[nIndex] = pSkill->m_sAPPLY.sSPECS[a].sSPEC[wlevel].fVAR2;
			sSKILLEF.dwSPECFLAG[nIndex] = pSkill->m_sAPPLY.sSPECS[a].sSPEC[wlevel].dwFLAG;
			sSKILLEF.dwNativeID[nIndex] = pSkill->m_sAPPLY.sSPECS[a].sSPEC[wlevel].dwNativeID;
			nIndex++;
			break;
		};
	}

	//	Note : ¡ˆº”«¸ Ω∫≈≥¿œ ∞ÊøÏ √÷¿˚ ΩΩ∑‘¿ª √£æ∆º≠ Ω∫≈≥ »ø∞˙∏¶ ≥÷æÓ¡‹.
	//
	if ( bHOLD )
	{
		sSKILLEF.sNATIVEID	= skill_id;
		sSKILLEF.wLEVEL		= wlevel;
		sSKILLEF.fAGE		= sSKILL_DATA.fLIFE;

		m_sSKILLFACT[dwSELECT] = sSKILLEF;
		DoTransform(pSkill); //Transform Function 

		return TRUE;
	}
	GASSERT ( 0 && "√©‚Ä∫‚Äì√¶¬®‚Äú √®¬ù¬∂√©‚Äò‚Äô√®‚Äì‚Äò√ßÀÜ¬æ√©‚Ñ¢‚Ä∫ √•¬∂¬∏√Ø¬†‚Ä∞√Ø¬ù¬º?E√•¬Ω≈†√¶‚Äú¬†." );
	return FALSE;
}

