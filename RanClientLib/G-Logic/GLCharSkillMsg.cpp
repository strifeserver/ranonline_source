#include "stdafx.h"
#include "./GLChar.h"

#include "./GLGaeaServer.h"
#include "GLGaeaClient.h"
#include "./GLSchoolFreePK.h"
#include "GLClubDeathMatch.h"
#include "GLSchoolWars.h"
#include "GLRoyalRumble.h"

#include "GLTowerWars/GLTowerWars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Activate Skill w/o Scroll
HRESULT GLChar::MsgReqLearnSkillNonInven ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN *pNetMsg = (GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN *) nmg;

	SNATIVEID sSKILL_ID = pNetMsg->sSKILLID;

	if ( ISLEARNED_SKILL(sSKILL_ID) )
	{
		GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB NetMsgFB;
		NetMsgFB.skill_id = sSKILL_ID;
		NetMsgFB.emCHECK = EMSKILL_LEARN_ALREADY;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
	{
		GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB	NetMsgFB;
		NetMsgFB.skill_id = sSKILL_ID;
		NetMsgFB.emCHECK = emSKILL_LEARNCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	LEARN_SKILL_NONSCROLL(sSKILL_ID);

	GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB	NetMsgFB;
	NetMsgFB.skill_id = sSKILL_ID;
	NetMsgFB.emCHECK = EMSKILL_LEARN_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;
	NetMsgSkp.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

	//Update Any money type
	GLMSG::SNETPC_UPDATE_MONEYTYPE NetMsgMoney;
	NetMsgMoney.lnGold = m_lnMoney;
	NetMsgMoney.lnPremP = m_lnPremP;
	NetMsgMoney.lnVoteP = m_lnVoteP;
	NetMsgMoney.lnContribP = m_lnContribP; //add contributionpoint by CNDev
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgMoney );
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSKILL_ID );
	if ( !pSkill ) return E_FAIL;

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;
		NetMsgBrd.sSKILL_DATA = m_sSUM_PASSIVE;
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

	return S_OK;
}
HRESULT GLChar::MsgReqLearnSkill ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_LEARNSKILL *pNetMsg = (GLMSG::SNETPC_REQ_LEARNSKILL *) nmg;

	SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )	return E_FAIL;

	if ( CheckCoolTime( pInvenItem->sItemCustom.sNativeID ) )	return S_FALSE;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_SKILL )	return E_FAIL;

	SNATIVEID sSKILL_ID = pItem->sSkillBookOp.sSkill_ID;

	if ( ISLEARNED_SKILL(sSKILL_ID) )
	{
		//	???? ?????? ????.
		GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
		NetMsgFB.skill_id = sSKILL_ID;
		NetMsgFB.emCHECK = EMSKILL_LEARN_ALREADY;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
	{
		//	???? ???? ???? ?????? ???????? ????????.
		GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
		NetMsgFB.skill_id = sSKILL_ID;
		NetMsgFB.emCHECK = emSKILL_LEARNCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	//	Note : ???? ??????.
	//
	LEARN_SKILL(sSKILL_ID);

	//	Note : ???? ???????? ???? ?????? ???? ????.
	//
	QuestStartFromGetSKILL ( sSKILL_ID );

	//	Note : ?????? ?????? ????.
	//
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY, true );

	//	???? ???? ????.
	GLMSG::SNETPC_REQ_LEARNSKILL_FB	NetMsgFB;
	NetMsgFB.skill_id = sSKILL_ID;
	NetMsgFB.emCHECK = EMSKILL_LEARN_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	???? ?????? ?????? ?????? ????????.
	GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;
	NetMsgSkp.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

	//Update Any money type
	GLMSG::SNETPC_UPDATE_MONEYTYPE NetMsgMoney;
	NetMsgMoney.lnGold = m_lnMoney;
	NetMsgMoney.lnPremP = m_lnPremP;
	NetMsgMoney.lnVoteP = m_lnVoteP;
	NetMsgMoney.lnContribP = m_lnContribP; //add contributionpoint by CNDev
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgMoney );
	//	Note : passive skill ?? ???? ?????? ???????? ???????? ?????????????? ??????.
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSKILL_ID );
	if ( !pSkill ) return E_FAIL;

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;
		NetMsgBrd.sSKILL_DATA = m_sSUM_PASSIVE;
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqSkillUp ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_SKILLUP *pNetMsg = (GLMSG::SNETPC_REQ_SKILLUP *) nmg;

	EMSKILL_LEARNCHECK emSKILL_LVLUPCHECK = EMSKILL_LEARN_UNKNOWN;
	SCHARSKILL* pCHARSKILL = GETLEARNED_SKILL(pNetMsg->skill_id);
	if ( !pCHARSKILL )
	{
		//	?????????? ???? ?? ?? ???? ????.
		return E_FAIL;
	}

	emSKILL_LVLUPCHECK = CHECKLEARNABLE_SKILL(pNetMsg->skill_id);
	if ( emSKILL_LVLUPCHECK!=EMSKILL_LEARN_OK )
	{
		//	???? ?????? ??????????. FB ??????.
		GLMSG::SNETPC_REQ_SKILLUP_FB	NetMsgFB;
		NetMsgFB.sSkill.sNativeID = pNetMsg->skill_id;
		NetMsgFB.emCHECK = emSKILL_LVLUPCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	//	Note : ???? LEVEL UP.
	//
	WORD wToLevel = pCHARSKILL->wLevel + 1;
	LVLUP_SKILL ( pNetMsg->skill_id, wToLevel );

	//	????. FB ??????.
	GLMSG::SNETPC_REQ_SKILLUP_FB	NetMsgFB;
	
	NetMsgFB.sSkill.sNativeID = pNetMsg->skill_id;
	NetMsgFB.sSkill.wLevel = wToLevel;
	NetMsgFB.emCHECK = EMSKILL_LEARN_OK;

	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//	???? ?????? ?????? ?????? ????????.
	GLMSG::SNETPC_UPDATE_SKP NetMsgSkp;
	NetMsgSkp.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgSkp );

	//Update Any money type
	GLMSG::SNETPC_UPDATE_MONEYTYPE NetMsgMoney;
	NetMsgMoney.lnGold = m_lnMoney;
	NetMsgMoney.lnPremP = m_lnPremP;
	NetMsgMoney.lnVoteP = m_lnVoteP;
	NetMsgMoney.lnContribP = m_lnContribP; //add contributionpoint by CNDev
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgMoney );
	//	Note : passive skill ?? ???? ?????? ???????? ???????? ?????????????? ??????.
	//
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pNetMsg->skill_id );
	if ( !pSkill ) return E_FAIL;

	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
	{
		GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
		NetMsgBrd.dwGaeaID = m_dwGaeaID;
		NetMsgBrd.sSKILL_DATA = m_sSUM_PASSIVE;
		SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqSkill ( NET_MSG_GENERIC* nmg )
{
	static bool bFirst = true;
	//static int nCnt = 0;

	//CONSOLEMSG_WRITE( "MsgReqSkill Start" );

	if( GLGaeaServer::GetInstance().m_bEmptyMsg )						return S_OK;
	if ( !IsValidBody() )												return E_FAIL;
	if ( m_pLandMan && m_pLandMan->IsPeaceZone() )						return E_FAIL;

#if !defined(KR_PARAM) && !defined(KRT_PARAM)

	// ?????? ????
	if ( m_fSkillDelay < 0.3f )											return E_FAIL;  // 05.11.30  0.5f -> 0.3f

#endif

	bool bFreePKMap		= m_pLandMan->IsFreePK();
	bool bGuidBattleMap = m_pLandMan->m_bGuidBattleMap;
	bool bBRIGHTEVENT   = GLGaeaServer::GetInstance().IsBRIGHTEVENT();

	//
	//juver battle royale
	bool bRedZone = m_pLandMan->IsRedZone();
	bool bRoyalRumble = false;

	GLMSG::SNETPC_REQ_SKILL *pNetMsg = (GLMSG::SNETPC_REQ_SKILL *) nmg;
	SNATIVEID skill_id = pNetMsg->skill_id;

	// ???? ?????????? ???? ???? ???????? ????
	if ( pNetMsg->bDefenseSkill )
	{
		if ( pNetMsg->skill_id != m_sDefenseSkill.m_dwSkillID ) return E_FAIL;
		if ( !m_bDefenseSkill ) return E_FAIL;
	}

	//	???? ???? ??????.
	const PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )	return E_FAIL;
	const SKILL::SEXT_DATA &sEXT_DATA = pSkill->m_sEXT_DATA;
	bool benermy = ( SIDE_ENERMY==pSkill->m_sBASIC.emIMPACT_SIDE );

	//	Note : ???? ????.
	//
	m_TargetID.vPos = pNetMsg->vTARPOS;
	m_TargetID.emCrow = pNetMsg->sTARIDS[0].GETCROW();
	m_TargetID.dwID = pNetMsg->sTARIDS[0].GETID();

	WORD wSkillRange = GETSKILLRANGE_TAR(*pSkill);	

	// ???? ?????? ???????????? ???? ????
	if ( m_sCONFTING.IsPOWERFULTIME () ) return E_FAIL;
	
	//	Note : ?????? ?????? ????.
	//
	GLACTOR* pTARGET = NULL;
	for ( WORD i=0, j=0; i<pNetMsg->wTARNUM && j<EMTARGET_NET; ++i )
	{
		//	Note : ?????? ????.
		//
		STARGETID sTARID(pNetMsg->sTARIDS[i].GETCROW(),pNetMsg->sTARIDS[i].GETID());
		pTARGET = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTARID );
		if ( !pTARGET )	continue;

		// ?????????? ????
		if ( benermy )
		{

#if !defined(KR_PARAM) && !defined(KRT_PARAM) && !defined(JP_PARAM)

			// ???????? ???? ????
			D3DXVECTOR3 vTarPos = pTARGET->GetPosition();
			float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-vTarPos) );
			WORD wAttackRange = pTARGET->GetBodyRadius() + GETBODYRADIUS() + wSkillRange + 7;
			if ( ISLONGRANGE_ARMS() )	 wAttackRange += (WORD) GETSUM_TARRANGE();
			WORD wAttackAbleDis = wAttackRange + 20;

			// ???? ?????????? ???? ?????? ????
			if ( fDist > wAttackAbleDis )
			{
				//	Note : ????????.
				GLMSG::SNETPC_ATTACK_AVOID NetMsg;
				NetMsg.emTarCrow	= sTARID.emCrow;
				NetMsg.dwTarID		= sTARID.dwID;

				GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

				//	Note : ???? ???????????????? ?????? ????.
				//
				GLMSG::SNETPC_ATTACK_AVOID_BRD NetMsgBrd;
				NetMsgBrd.dwGaeaID	= m_dwGaeaID;
				NetMsgBrd.emTarCrow	= sTARID.emCrow;
				NetMsgBrd.dwTarID	= sTARID.dwID;

				SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
				continue;
			}

#endif

		}

		bool bREACTIONALBE  = IsReActionable( pTARGET, benermy );			
		if ( !bREACTIONALBE )												continue;

		bool bSCHOOL_FREEPK = GLSchoolFreePK::GetInstance().IsON();
		//if ( GetSchool() == pTARGET->GetSchool() )	bSCHOOL_FREEPK = false;

		if ( GetSchool() == pTARGET->GetSchool() )
		{
			if ( bRedZone )
			{
				bSCHOOL_FREEPK = true;
			}
			if ( bRoyalRumble )
			{
				bSCHOOL_FREEPK = true;
			}
			else
			{
				bSCHOOL_FREEPK = false;
			}
		}
		///

		//	Note : ?????? ???? pk?? ???? ???? ???????? ???? ?????? ????.
		for(int a= 0; a < SKILL::MAX_LEVEL; a++)
		{
			if ( pSkill->m_sAPPLY.sSPECS[a].emSPEC == EMSPECA_REBIRTH && bSCHOOL_FREEPK ) continue;
		}	

		if ( pTARGET->GetCrow() == CROW_PC )
		{
			PGLCHAR pCHAR = GLGaeaServer::GetInstance().GetChar ( sTARID.dwID );
			if ( pCHAR )
			{
				// ?????? ?????? ???????? ?????? ?????? ???????? ???? ????.
				if( pSkill->m_sBASIC.sNATIVEID.wMainID == 14 && pSkill->m_sBASIC.sNATIVEID.wSubID == 15 && pCHAR->m_bNon_Rebirth == TRUE )
				{
					//	Note : ???? ???? ???? FB ??????.
					{
						GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
						strcpy( NetMsgFB.szName, pCHAR->m_szName );
						NetMsgFB.emSKILL_FB = EMSKILL_NOTREBIRTH;
						GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
					}
					{
						GLMSG::SNET_MSG_REQ_SKILL_REVIVEL_FAILED NetMsgFB;
						strcpy( NetMsgFB.szName, m_szName );
						GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );
					}
					return E_FAIL;
				}
				// ???? ?????? ???????????? ???? ????
				if ( pCHAR->m_sCONFTING.IsPOWERFULTIME () )					continue;

				bool bClubBattle = false;
				bool bClubDeathMatch = false;
				bool bSchoolWars = false;
				bool bTowerWars = false;

				if ( m_pLandMan->IsClubBattleZone() )
				{
					GLClubMan &cClubMan = m_pGLGaeaServer->GetClubMan();
					GLCLUB *pMyClub = cClubMan.GetClub ( m_dwGuild );
					GLCLUB *pTarClub = cClubMan.GetClub ( pCHAR->m_dwGuild );

					if ( pMyClub && pTarClub )
					{
						bool bClub = pMyClub->IsBattle( pCHAR->m_dwGuild );
						bool bAlliance = pMyClub->IsBattleAlliance ( pTarClub->m_dwAlliance );

						bClubBattle = ( bClub || bAlliance );
					}
				}

				if ( m_pLandMan->m_bClubDeathMatchMap )
				{
					GLClubDeathMatch* pCDM = GLClubDeathMatchFieldMan::GetInstance().Find( m_pLandMan->m_dwClubMapID );
					if ( pCDM && pCDM->IsBattle() ) bClubDeathMatch = true;	
				}

				if ( m_pLandMan->m_bSchoolWarsMap )
				{
					GLSchoolWars* pSW = GLSchoolWarsFieldMan::GetInstance().Find( m_pLandMan->m_dwSchoolWarMapID );
					if ( pSW && pSW->IsBattle() ) bSchoolWars = true;	
				}

				if ( m_pLandMan->m_bRoyalRumbleMap )
				{
					GLRoyalRumble* pRR = GLRoyalRumbleFieldMan::GetInstance().Find( m_pLandMan->m_dwRoyalRumbleMapID );
					if ( pRR && pRR->IsBattle() ) bRoyalRumble = true;	
				}


				if ( m_pLandMan->m_bTowerWarsMap )
				{
					GLTowerWars* pSW = GLTowerWarsFieldMan::GetInstance().Find( m_pLandMan->m_dwTowerWarsMapID );
					if ( pSW && pSW->IsBattle() ) bTowerWars = true;	
				}

				if ( !(bGuidBattleMap|| bClubDeathMatch || bSchoolWars || bTowerWars || bSCHOOL_FREEPK||bFreePKMap||bBRIGHTEVENT||IsConflictTarget(pTARGET) || bClubBattle ) )
				{
					//	Note : ???????? ???????? ?????? ???????? ?????? ???????? ?????????? 
					//		?????? ?????????? ?? ????.
					if ( !IS_PLAYHOSTILE(pCHAR->m_dwCharID) && !pCHAR->ISOFFENDER() && benermy )
					{
						int nBRIGHT(GLCONST_CHAR::nPK_TRY_BRIGHT_POINT), nLIFE(GLCONST_CHAR::nPK_TRY_LIVING_POINT);

						//	Note : ???????? ????.
						m_nBright += nBRIGHT;

						GLMSG::SNETPC_UPDATE_BRIGHT NetMsg;
						NetMsg.nBright = m_nBright;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

						GLMSG::SNETPC_UPDATE_BRIGHT_BRD NetMsgBrd;
						NetMsgBrd.dwGaeaID = m_dwGaeaID;
						NetMsgBrd.nBright = m_nBright;
						SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

						//	Note : ???????? ????.
						m_nLiving += nLIFE;

						GLMSG::SNETPC_UPDATE_LP NetMsgLp;
						NetMsgLp.nLP = m_nLiving;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgLp);

						//	Note : ???????? ???? ????.
						GLITEMLMT::GetInstance().ReqAction
						(
							m_dwCharID,					//	??????.
							EMLOGACT_HOSTILE,			//	????.
							ID_CHAR, pCHAR->m_dwCharID,	//	??????.
							0,							//	exp
							nBRIGHT,					//	bright
							nLIFE,						//	life
							0							//	money
						);
					}
				}

				if ( benermy && !IsConflictTarget(pTARGET) )
				{
					//	Note : ???? ?????????? ?????? ????. ( pk )
					AddPlayHostile ( pCHAR->m_dwCharID, TRUE, bClubBattle );

					//	Note : ???? ?????????? ?????? ????. ( PK )
					pCHAR->AddPlayHostile ( m_dwCharID, FALSE, bClubBattle );

					STARGETID sTargetID = STARGETID ( CROW_PC, m_dwGaeaID, m_vPos );
					
					
					// ???????? ?????????? ???? ???? ?????????? ????
					PGLPETFIELD pEnemyPet = GLGaeaServer::GetInstance().GetPET ( pCHAR->m_dwPetGUID );
					if ( pEnemyPet && pEnemyPet->IsValid () && !pEnemyPet->IsSTATE ( EM_PETACT_ATTACK ) )
					{
						D3DXVECTOR3 vOwnerPos, vDist;
						float fDist;
						vOwnerPos = pCHAR->GetPosition ();
						vDist = pEnemyPet->m_vPos - vOwnerPos;
						fDist = D3DXVec3Length(&vDist);

						// ???????? ???? ?????? 
						if ( fDist <= GLCONST_PET::fWalkArea )
						{
							pEnemyPet->ReSetAllSTATE ();
							pEnemyPet->SetSTATE ( EM_PETACT_ATTACK );

							GLMSG::SNETPET_ATTACK NetMsg;
							NetMsg.sTarID = sTargetID;
							GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsg );
							
							GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
							NetMsgBRD.dwGUID = pEnemyPet->m_dwGUID;
							NetMsgBRD.sTarID = sTargetID;
							pCHAR->SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
						}

					}

					// ???????? Summon Attack ????
					PGLSUMMONFIELD pEnemySummon = GLGaeaServer::GetInstance().GetSummon ( pCHAR->m_dwSummonGUID );
					if ( pEnemySummon && pEnemySummon->IsValid () && !pEnemySummon->IsSTATE ( EM_SUMMONACT_ATTACK ) && 
						pEnemySummon->GetAttackTarget( sTargetID ) )
					{
						/*D3DXVECTOR3 vOwnerPos, vDist;
						float fDist;
						vOwnerPos = pCHAR->GetPosition ();
						vDist = pEnemySummon->m_vPos - vOwnerPos;
						fDist = D3DXVec3Length(&vDist);

						if ( fDist <= pEnemySummon->m_fWalkArea )*/
						{
							pEnemySummon->SetAttackTarget( sTargetID );
						}
					}

					PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
					if ( pMyPet && pMyPet->IsValid () && !pMyPet->IsSTATE ( EM_PETACT_ATTACK ) )
					{
						D3DXVECTOR3 vDist;
						float fDist;
						vDist = pMyPet->m_vPos - m_vPos;
						fDist = D3DXVec3Length(&vDist);

						// ???????? ???? ?????? 
						if ( fDist <= GLCONST_PET::fWalkArea )
						{
							pMyPet->ReSetAllSTATE ();
							pMyPet->SetSTATE ( EM_PETACT_ATTACK );

							GLMSG::SNETPET_ATTACK NetMsg;
							NetMsg.sTarID = m_TargetID;
							GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
							
							GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
							NetMsgBRD.dwGUID = pMyPet->m_dwGUID;
							NetMsgBRD.sTarID = m_TargetID;
							SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
						}
					}
				}
			}

		}

		//	Note : ?????? ???? ???????? ????.
		//		( ???????? ???????? ?????? ?????? ???? ???? ???? ????. )
		m_sTARIDS[j++] = pNetMsg->sTARIDS[i];
	}

	m_wTARNUM = j;
	if ( m_wTARNUM == 0 )
	{
		//	Note : ????????.
		GLMSG::SNETPC_ATTACK_DAMAGE NetMsg;
		NetMsg.emTarCrow		= m_TargetID.emCrow;
		NetMsg.dwTarID			= m_TargetID.dwID;
		NetMsg.nDamage			= 0;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, (NET_MSG_GENERIC*) &NetMsg );

		return E_FAIL;
	}

	//	Note : ???? ???? ???? ????.
	//
	GLCONST_CHARCLASS &ConstCharClass = GLCONST_CHAR::cCONSTCLASS[m_CHARINDEX];
	VECANIATTACK &vecAniAttack = ConstCharClass.m_ANIMATION[sEXT_DATA.emANIMTYPE][sEXT_DATA.emANISTYPE];
	if ( vecAniAttack.empty() )
	{
		// ???????? ???????????????? ????????.
		DEBUGMSG_WRITE ( _T("Can't find skill animation. char[%d] animation[%d][%d]"),
			             m_CHARINDEX,
						 sEXT_DATA.emANIMTYPE,
						 sEXT_DATA.emANISTYPE );
		return E_FAIL;
	}

	const SANIATTACK &sAniAttack = vecAniAttack[0];
	WORD wStrikeNum = sAniAttack.m_wDivCount;
	if ( wStrikeNum==0 )
	{
		// ???? ???????????? ???? ?????? ???????? ??????????
		DEBUGMSG_WRITE ( _T("Can't find target position of skill animation. char[%d] animation[%d][%d]"),
			             m_CHARINDEX,
						 sEXT_DATA.emANIMTYPE,
						 sEXT_DATA.emANISTYPE );
		return E_FAIL;
	}

	EMSKILLCHECK emCHECK = GLCHARLOGIC::CHECHSKILL ( skill_id, 1, pNetMsg->bDefenseSkill );
	if ( emCHECK != EMSKILL_OK && emCHECK != EMSKILL_NOTSP )
	{
		//	Note : ???? ???? ???? FB ??????.
		GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
		NetMsgFB.emSKILL_FB = emCHECK;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	//	Note : SKILL ????.
	//
	SETACTIVESKILL ( skill_id );
	SetDefenseSkill( pNetMsg->bDefenseSkill );
	if ( pNetMsg->bDefenseSkill ) m_bDefenseSkill = false;

	m_SKILLMTYPE = sEXT_DATA.emANIMTYPE;
	m_SKILLSTYPE = sEXT_DATA.emANISTYPE;

	BOOL bLowSP =  (emCHECK==EMSKILL_NOTSP) ? TRUE : FALSE;
	PreStrikeProc ( TRUE, bLowSP );

	TurnAction ( GLAT_SKILL );

	// ?????? ?????????? ???????? ???????? ??????
	m_fSkillDelay = 0.0f;
	
	//	Note : ???? ?????? ???????? ????????.
	//
	GLCHARLOGIC::ACCOUNTSKILL ( skill_id, 1, true );


	//	Note : SKILL ???? ?????? - ?????? ???? Char???? ?????? Msg?? ????.
	//
	WORD wLevel;
	if ( pNetMsg->bDefenseSkill )	wLevel = m_sDefenseSkill.m_wLevel;
	else wLevel = GETLEARNED_SKILL(pNetMsg->skill_id)->wLevel;

	GLMSG::SNETPC_REQ_SKILL_BRD NetMsgBRD;
	NetMsgBRD.emCrow = GETCROW();
	NetMsgBRD.dwID = m_dwGaeaID; //- ?????? ????. 
	NetMsgBRD.skill_id = pNetMsg->skill_id;
	NetMsgBRD.wLEVEL = wLevel; //- ???? ???? ????.
	NetMsgBRD.vTARPOS = pNetMsg->vTARPOS;
	for ( WORD i=0; i<m_wTARNUM; ++i )		NetMsgBRD.ADDTARGET ( m_sTARIDS[i] );

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );



	//	Note : ???? ???? ???? FB ??????.
	GLMSG::SNETPC_REQ_SKILL_FB NetMsgFB;
	NetMsgFB.emSKILL_FB = EMSKILL_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	//CONSOLEMSG_WRITE( "MsgReqSkill End [%u]", skill_id.dwID );

	// ???? ???????? ???? ?????????? ????
	if ( m_TargetID.emCrow==CROW_MOB )
	{
		PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
		if ( pMyPet && pMyPet->IsValid () && !pMyPet->IsSTATE ( EM_PETACT_ATTACK ) )
		{
			D3DXVECTOR3 vDist;
			float fDist;
			vDist = pMyPet->m_vPos - m_vPos;
			fDist = D3DXVec3Length(&vDist);

			// ???????? ???? ?????? 
			if ( fDist <= GLCONST_PET::fWalkArea )
			{
				GLMSG::SNETPET_ATTACK NetMsg;
				NetMsg.sTarID = m_TargetID;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
				
				GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
				NetMsgBRD.dwGUID = m_dwPetGUID;
				NetMsgBRD.sTarID = m_TargetID;
				SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
			}
		}
	}

	return S_OK;
}

HRESULT GLChar::MsgReqSkillCancel ( NET_MSG_GENERIC* nmg )
{
	//CONSOLEMSG_WRITE( "MsgReqSkillCancel" );



	//	Note : ???? ????.
	if ( IsACTION(GLAT_SKILL) )		TurnAction ( GLAT_IDLE );

	//	Note : ?????? ???? Char???? ?????? Msg?? ????.
	//
	GLMSG::SNETPC_SKILL_CANCEL_BRD NetMsgBRD;
	NetMsgBRD.dwGaeaID = m_dwGaeaID;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQSet ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_SET *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_SET *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : ???? ?????? ???? ???? ??????.
	if ( !ISLEARNED_SKILL(pNetMsg->skill_id) )	return E_FAIL;

	//	Note : ?????? ??????.
	m_sSKILLQUICK[pNetMsg->wSLOT] = pNetMsg->skill_id;

	//	Note : ???????????? ????.
	GLMSG::SNETPC_REQ_SKILLQUICK_FB NetMsgFB;
	NetMsgFB.wSLOT = pNetMsg->wSLOT;
	NetMsgFB.skill_id = m_sSKILLQUICK[pNetMsg->wSLOT];
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQReSet ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_RESET *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_RESET *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : ?????? ??????.
	m_sSKILLQUICK[pNetMsg->wSLOT] = NATIVEID_NULL();

	//	Note : ???????????? ????.
	GLMSG::SNETPC_REQ_SKILLQUICK_FB NetMsgFB;
	NetMsgFB.wSLOT = pNetMsg->wSLOT;
	NetMsgFB.skill_id = m_sSKILLQUICK[pNetMsg->wSLOT];
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;
}

HRESULT GLChar::MsgReqSkillQSetActive ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SKILLQUICK_ACTIVE *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_ACTIVE *) nmg;

	if ( EMSKILLQUICK_SIZE <= pNetMsg->wSLOT )	return E_FAIL;

	//	Note : ???????? ???? ?????? ????.
	m_wSKILLQUICK_ACT = pNetMsg->wSLOT;

	return S_OK;
}

