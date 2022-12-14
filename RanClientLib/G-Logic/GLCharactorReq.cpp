#include "stdafx.h"

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
#include "GLPartyClient.h"
#include "GLBusStation.h"
#include "GLBusData.h"
#include "GLTaxiStation.h"
#include "RANPARAM.h"
#include "GLMaplist.h"
#include "GLFriendClient.h"
#include "RanFilter.h"
#include "DxClubMan.h"
#include "GLFactEffect.h"
#include "GLQUEST.h"
#include "GLQUESTMAN.h"
#include "GLCharacter.h"
#include "GLItemMixMan.h"
#include "GLItem.h"

#include "../RanClientUILib/Interface/UITextControl.h"
#include "../RanClientUILib/Interface/GameTextControl.h"
#include "../RanClientUILib/Interface/InnerInterface.h"
#include "../RanClientUILib/Interface/InventoryPage.h"
#include "../RanClientUILib/Interface/InventoryWindow.h"
#include "../RanClientUILib/Interface/BasicGameMenu.h"
#include "../RanClientUILib/Interface/QBoxButton.h"
#include "../RanClientUILib/Interface/ModalWindow.h"
#include "../RanClientUILib/Interface/ModalCallerID.h"
#include "../RanClientUILib/Interface/ItemShopIconMan.h"
#include "../RanClientUILib/Interface/PetskinMixImage.h"
#include "../RanClientUILib/Interface/MapWindow/LargeMapWindow.h"

#include "../enginelib/Common/StringUtils.h"

//#include "./ObserverNotifyID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLCharacter::ReqToggleRun ()
{
	if ( IsSTATE(EM_ACT_RUN) )
	{
		ReSetSTATE(EM_ACT_RUN);
		CBasicGameMenu * pGameMenu = CInnerInterface::GetInstance().GetGameMenu();
		if( pGameMenu ) pGameMenu->SetFlipRunButton( FALSE );
	}
	else
	{
		SetSTATE(EM_ACT_RUN);
		CBasicGameMenu * pGameMenu = CInnerInterface::GetInstance().GetGameMenu();
		if( pGameMenu ) pGameMenu->SetFlipRunButton( TRUE );
	}

	m_actorMove.SetMaxSpeed ( GetMoveVelo() );

	PGLPETCLIENT pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
	if ( pMyPet->IsVALID () )
	{
		pMyPet->SetMoveState ( IsSTATE(EM_ACT_RUN) );
	}

	//	Note : ?????? ????.
	//
	GLMSG::SNETPC_MOVESTATE NetMsg;
	NetMsg.dwActState = m_dwActState;

	NETSENDTOFIELD ( (NET_MSG_GENERIC*) &NetMsg );
}

void GLCharacter::ReqTogglePeaceMode ()
{
	if ( IsSTATE(EM_ACT_PEACEMODE) && !m_bVehicle )	ReSetSTATE(EM_ACT_PEACEMODE);
	else											SetSTATE(EM_ACT_PEACEMODE);

	m_fIdleTime = 0.0f;

	//	Note : ?????? ????.
	//
	GLMSG::SNETPC_ACTSTATE NetMsg;
	NetMsg.dwActState = m_dwActState;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

void GLCharacter::ReqToggleBooster () //add bike booster
{
	if ( IsSTATE(EM_ACT_BOOSTER) /*&& m_bVehicle */)	ReSetSTATE(EM_ACT_BOOSTER);
	else  if ( m_bVehicle )	SetSTATE( EM_ACT_BOOSTER );

	GLMSG::SNETPC_ACTSTATE NetMsg;
	NetMsg.dwActState = m_dwActState;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

void GLCharacter::ReqVisibleNone ()
{
	SetSTATE(EM_REQ_VISIBLENONE);

	//	Note : (????????????) ?????? ????.
	//
	GLMSG::SNETPC_ACTSTATE NetMsg;
	NetMsg.dwActState = m_dwActState;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

void GLCharacter::ReqVisibleOff ()
{
	SetSTATE(EM_REQ_VISIBLEOFF);

	//	Note : (????????????) ?????? ????.
	//
	GLMSG::SNETPC_ACTSTATE NetMsg;
	NetMsg.dwActState = m_dwActState;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

void GLCharacter::ReqVisibleOn ()
{
	ReSetSTATE(EM_REQ_VISIBLENONE);
	ReSetSTATE(EM_REQ_VISIBLEOFF);

	//	Note : (????????????) ?????? ????.
	//
	GLMSG::SNETPC_ACTSTATE NetMsg;
	NetMsg.dwActState = m_dwActState;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

// *****************************************************
// Desc: ???? ?????? ????
// *****************************************************
void GLCharacter::ReqGateOut ()
{
	if ( IsSTATE(EM_REQ_GATEOUT) )							return;
	if ( IsSTATE(EM_ACT_WAITING) )							return;

	DWORD dwGateID = DetectGate ();
	if ( dwGateID==UINT_MAX )								return;

	PLANDMANCLIENT pLandMClient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLandMClient )								return;

	DxLandGateMan *pLandGateMan = &pLandMClient->GetLandGateMan();
	if ( !pLandGateMan )								return;

	PDXLANDGATE pLandGate = pLandGateMan->FindLandGate ( dwGateID );
	if ( !pLandGate )									return;

	SNATIVEID sMapID = pLandGate->GetToMapID();

	SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( sMapID );
	if ( !pMapNode )									return;


	//	TODO :  +, quest, ?????? ?????? ???? ????, ???????????????? ???????? ????. ???????? ?????? ??????.
	//
	//	TODO : ???? ???? ?????? ?????????? ?????? ???? ???????? ???????? ?????? ?? ????
	//		?????? ?????? ?????? ???????? ???????????????? ???????? ????.
	//

	EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
	
	GLLevelFile cLevelFile;
	BOOL bOk = cLevelFile.LoadFile ( pMapNode->strFile.c_str(), TRUE, NULL );
	if ( !bOk )											return;

	SLEVEL_REQUIRE* pRequire = cLevelFile.GetLevelRequire ();
	emReqFail = pRequire->ISCOMPLETE ( this );
	if ( emReqFail != EMREQUIRE_COMPLETE )
	{
		CInnerInterface &cINTERFACE = CInnerInterface::GetInstance();
		switch ( emReqFail )
		{
		case EMREQUIRE_LEVEL:
			{
				if( pRequire->m_signLevel == EMSIGN_FROMTO )
				{
					cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
						ID2GAMEINTEXT("EMREQUIRE_LEVEL2"),
						pRequire->m_wLevel,
						pRequire->m_wLevel2 );
				}else{
					std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signLevel].c_str());
					
					if( RANPARAM::emSERVICE_TYPE == EMSERVICE_THAILAND )
					{
						cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
													ID2GAMEINTEXT("EMREQUIRE_LEVEL"),
													strSIGN.c_str(),
													pRequire->m_wLevel );
					}
					else
					{
						cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
													ID2GAMEINTEXT("EMREQUIRE_LEVEL"),
													pRequire->m_wLevel,
													strSIGN.c_str() );
					}
				}
			}
			break;

		case EMREQUIRE_ITEM:
			{
				SITEM *pItem = GLItemMan::GetInstance().GetItem ( pRequire->m_sItemID );
				if ( pItem )
				{
					cINTERFACE.PrintMsgTextDlg
					(
						NS_UITEXTCOLOR::DISABLE,
						ID2GAMEINTEXT("EMREQUIRE_ITEM"),
						pItem->GetName()
					);
				}
			}
			break;

		case EMREQUIRE_SKILL:
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pRequire->m_sSkillID );
				if ( pSkill )
				{
					cINTERFACE.PrintMsgTextDlg
					(
						NS_UITEXTCOLOR::DISABLE,
						ID2GAMEINTEXT("EMREQUIRE_SKILL"),
						pSkill->GetName()
					);
				}
			}
			break;

		case EMREQUIRE_LIVING:
			{
				std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signLiving].c_str());
				cINTERFACE.PrintMsgTextDlg
				(
					NS_UITEXTCOLOR::DISABLE,
					ID2GAMEINTEXT("EMREQUIRE_LIVING"),
					pRequire->m_nLiving,
					strSIGN.c_str()
				);
			}
			break;

		case EMREQUIRE_BRIGHT:
			{
				std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signBright].c_str());
				cINTERFACE.PrintMsgTextDlg
				(
					NS_UITEXTCOLOR::DISABLE,
					ID2GAMEINTEXT("EMREQUIRE_BRIGHT"),
					pRequire->m_nBright,
					strSIGN.c_str()
				);
			}
			break;

		case EMREQUIRE_QUEST_COM:
			{
				CString strQUEST = "quest";
				GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pRequire->m_sComQuestID.dwID );
				if ( pQUEST )		strQUEST = pQUEST->GetTITLE();

				cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQUIRE_QUEST_COM"), strQUEST.GetString() );
			}
			break;

		case EMREQUIRE_QUEST_ACT:
			{
				CString strQUEST = "quest";
				GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pRequire->m_sActQuestID.dwID );
				if ( pQUEST )		strQUEST = pQUEST->GetTITLE();

				cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQUIRE_QUEST_ACT"), strQUEST.GetString() );
			}
			break;

		default:
			cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("MAP_REQUIRE_FAIL") );
			break;
		};

		//	Note : ???? ???? ?????? ???? ????. GM level ?????? ???? ???? ????.
		//
		if ( m_dwUserLvl < USER_GM3 )	return;
	}


	if( pMapNode->bInstantMap )
	{
		//	Note : ???????? ?? ???? ????
		//
		GLMSG::SNETREQ_CREATE_INSTANT_MAP_REQ NetMsg;
		NetMsg.dwGateID = dwGateID;
		NETSENDTOFIELD ( &NetMsg );
		SetSTATE(EM_REQ_GATEOUT);
		return;
	}

	//	Note : ???? ?????? ???? ?????? ????.
	//
	GLMSG::SNETREQ_GATEOUT_REQ NetMsg;
	NetMsg.dwGateID = dwGateID;
	NETSENDTOFIELD ( &NetMsg );

	SetSTATE(EM_REQ_GATEOUT);

}

bool GLCharacter::ReqGESTURE ( int nMOTION, bool bCOMMAND )
{
	//	Note : ???? ?????? ???? ?????? ?????? ???????? ?? ?? ????.
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	BOOL bPeaceZone = pLand ? pLand->IsPeaceZone() : FALSE;

	//	Note : ???????? ???????? ???????? ????. ?????????? ????.
	if ( bCOMMAND )
	{
		if ( !bPeaceZone && !IsSTATE(EM_ACT_PEACEMODE) )
		{
			if ( IsACTION(GLAT_IDLE) )		ReqTogglePeaceMode();
		}
	}

	if ( !bPeaceZone && IsSTATE(EM_ACT_PEACEMODE) )
	{
		bPeaceZone = TRUE;
	}

	if ( !bPeaceZone )	return false;

	// ???? ?????????? ?????? ????
	if ( m_bVehicle ) return true;

	//	Note : ???? ?????? ???????????? ?????? ????.
	PANIMCONTNODE pNODE = m_pSkinChar->GETANI ( AN_GESTURE, EMANI_SUBTYPE(nMOTION) );
	if ( !pNODE )			return false;

	//	Note : ?????? ?????????? ????.
	if ( !IsValidBody() )	return false;

	//	Note : ???????? ????.
	m_dwANISUBGESTURE = (DWORD) nMOTION;
	TurnAction(GLAT_TALK);

	//	Note : ?????? ?????? ????.
	GLMSG::SNETPC_REQ_GESTURE NetMsg;
	NetMsg.dwID = m_dwANISUBGESTURE;
	NETSENDTOFIELD ( &NetMsg );

	return true;
}

inline bool GLCharacter::IsInsertToInven ( PITEMCLIENTDROP pItemDrop )
{
	GASSERT(pItemDrop&&"GLChar::IsItemToInven()");
	if ( !pItemDrop )	return false;

	SITEM *pItem = GLItemMan::GetInstance().GetItem(pItemDrop->sItemClient.sNativeID);
	if ( !pItem )		return false;

	if ( pItem->ISPILE() )
	{
		WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
		WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

		//	???? ???????? ????.
		WORD wPILENUM = pItem->sDrugOp.wPileNum;
		SNATIVEID sNID = pItem->sBasicOp.sNativeID;

		//	Note : ???? ?????? ????????. ( ??????. )
		//
		WORD wREQINSRTNUM = ( pItemDrop->sItemClient.wTurnNum );

		BOOL bITEM_SPACE = TRUE;
#if defined(VN_PARAM) //vietnamtest%%%
		if( m_dwVietnamGainType == GAINTYPE_EMPTY )
		{
			bITEM_SPACE = m_cVietnamInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, wINVENX, wINVENY );
		}else{
			bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, wINVENX, wINVENY );
		}		
#else
		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, wINVENX, wINVENY );
#endif
		if ( !bITEM_SPACE )		return false;
	}
	else
	{
		WORD wPosX, wPosY;
		BOOL bOk = TRUE;
#if defined(VN_PARAM) //vietnamtest%%%
		if( m_dwVietnamGainType == GAINTYPE_EMPTY )
		{
			bOk = m_cVietnamInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		}else{
			bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		}
#else
		bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
#endif
		if ( !bOk )				return false;
	}

	return true;
}

//	Note : ???? ??????(??) ??????.
HRESULT GLCharacter::ReqFieldTo ( const STARGETID &sTargetID, bool bPet )
{
	if ( VALID_HOLD_ITEM () )					return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;	

	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLAND )	return E_FAIL;

	//if ( sTargetID.emCrow==CROW_ITEM )
	//{
	//	if ( pLAND->ISITEM_PICKDELAY ( sTargetID.dwID ) )	return E_FAIL;
	//}
	//else if ( sTargetID.emCrow==CROW_MONEY )
	//{
	//	if ( pLAND->ISMONEY_PICKDELAY ( sTargetID.dwID ) )	return E_FAIL;
	//}

	//BOOL bInventoryOpen = FALSE;
	//bInventoryOpen = CInnerInterface::GetInstance().IsInventoryWindowOpen ();
	//if ( sTargetID.emCrow==CROW_ITEM && bInventoryOpen )
	//{
	//	//	?????? ????.
	//	GLMSG::SNETPC_REQ_FIELD_TO_HOLD NetMsg;
	//	NetMsg.dwGlobID = sTargetID.dwID;
	//	NETSENDTOFIELD ( &NetMsg );

	//	//	???? ?????? ???? ????.
	//	pLAND->SETITEM_PICKDELAY ( sTargetID.dwID );
	//}
	//else
	{
		//	Note : ?????? ???????? ?????????? ???? ??????.
		//
		// ????????
		if ( !IsValidBody() )	return E_FAIL;

		//	???? ????
		const D3DXVECTOR3 &vTarPos = sTargetID.vPos;

		//	???? ????
		D3DXVECTOR3 vPos;

		if ( bPet )	
		{
			GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
			if ( pMyPet->IsVALID() )	vPos = pMyPet->GetPosition();
		}	
		else vPos = m_vPos;

		D3DXVECTOR3 vDistance = vPos - vTarPos;
		float fDistance = D3DXVec3Length ( &vDistance );

		WORD wTarBodyRadius = 4;
		WORD wTakeRange = wTarBodyRadius + GETBODYRADIUS() + 2;
		WORD wTakeAbleDis = wTakeRange + 15;

		if ( fDistance>wTakeAbleDis )
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAKE_FB_DISTANCE") );			
			return E_FAIL;
		}

		if ( sTargetID.emCrow==CROW_ITEM )
		{
			PITEMCLIENTDROP pItemDrop = pLAND->GetItem ( sTargetID.dwID );
			if ( !pItemDrop )		return E_FAIL;

			if( CInnerInterface::GetInstance().GetQBoxButton()->GetQBoxEnable() == FALSE )
			{
				SITEM *pItem = GLItemMan::GetInstance().GetItem(pItemDrop->sItemClient.sNativeID);
				if( pItem != NULL && pItem->sBasicOp.emItemType==ITEM_QITEM )
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("QBOX_OPTION_DISABLE_MSG") );
					return E_FAIL;
				}
			}

			//	Note : ?????? ?????? ?????? ????.
			//
			BOOL bOk = IsInsertToInven ( pItemDrop );
			if ( !bOk )
			{
#if defined(VN_PARAM) //vietnamtest%%%
				if( m_dwVietnamGainType == GAINTYPE_EMPTY )	return E_FAIL;
#endif
				//	?????? ????????.
				//	?????? ????.
				GLMSG::SNETPC_REQ_FIELD_TO_HOLD NetMsg;
				NetMsg.dwGlobID = sTargetID.dwID;
				NETSENDTOFIELD ( &NetMsg );
				return S_OK;
			}

			//	???? ?????? ???? ????.
			// pLAND->SETITEM_PICKDELAY ( sTargetID.dwID );
		}
		else if ( sTargetID.emCrow==CROW_MONEY )
		{
			PMONEYCLIENTDROP pMoney = GLGaeaClient::GetInstance().GetActiveMap()->GetMoney ( sTargetID.dwID );
			if ( !pMoney )		return E_FAIL;

			//	???? ?????? ???? ????.
			// pLAND->SETMONEY_PICKDELAY ( sTargetID.dwID );
		}

		//	?????? ????.
		GLMSG::SNETPC_REQ_FIELD_TO_INVEN NetMsg;
		NetMsg.emCrow = sTargetID.emCrow;
		NetMsg.dwID = sTargetID.dwID;
		NetMsg.bPet	= bPet;
		NETSENDTOFIELD ( &NetMsg );
	}

	return S_OK;
}

//	Note : ???????? ?????? ????, ??????, ????????, ??????.
HRESULT GLCharacter::ReqInvenTo ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );

	//	Note : ???????? ??????????.
	//
	if ( GLTradeClient::GetInstance().Valid() )
	{
		if ( pInvenItem )	GLTradeClient::GetInstance().SetPreItem ( SINVEN_POS( pInvenItem->wPosX, pInvenItem->wPosY ) );
		else				GLTradeClient::GetInstance().ReSetPreItem ();
		return S_OK;
	}

	if( ValidRebuildOpen() || ValidGarbageOpen() || ValidItemMixOpen() )	// ITEMREBUILD_MARK
	{
		if( m_sPreInventoryItem.wPosX == wPosX && m_sPreInventoryItem.wPosY == wPosY )
		{
			m_sPreInventoryItem.RESET();
		}
		else
		{
			if( pInvenItem )
				m_sPreInventoryItem.SET( wPosX, wPosY );
			else
				m_sPreInventoryItem.RESET();
		}
		return S_OK;
	}

	if ( !VALID_HOLD_ITEM () && !pInvenItem )	return E_FAIL;

	if ( VALID_HOLD_ITEM () && pInvenItem )
	{
#if defined(VN_PARAM) //vietnamtest%%%
		const SITEMCUSTOM& sCustom = GET_HOLD_ITEM();
		
		if ( !sCustom.bVietnamGainItem )
#endif
		{
			GLMSG::SNETPC_REQ_INVEN_EX_HOLD NetMsg;
			NetMsg.wPosX = pInvenItem->wPosX;
			NetMsg.wPosY = pInvenItem->wPosY;
			NETSENDTOFIELD ( &NetMsg );
		}
	}
	else if ( pInvenItem )
	{
		GLMSG::SNETPC_REQ_INVEN_TO_HOLD NetMsg;
		NetMsg.wPosX = pInvenItem->wPosX;
		NetMsg.wPosY = pInvenItem->wPosY;
		NETSENDTOFIELD ( &NetMsg );
	}
	else if ( VALID_HOLD_ITEM () )
	{
#if defined(VN_PARAM) //vietnamtest%%%
		const SITEMCUSTOM& sCustom = GET_HOLD_ITEM();
		
		if ( !sCustom.bVietnamGainItem || ( sCustom.bVietnamGainItem && m_dwVietnamInvenCount > 0 ) )
#endif
		{
			//	Note : ?????? ???????? ?????????? ???? ??????.
			//
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
			GASSERT(pItem&&"?????? ???????? ???????? ????");

			BOOL bOk = m_cInventory.IsInsertable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
			if ( !bOk )
			{
				//	?????? ????????.
				return E_FAIL;
			}

			GLMSG::SNETPC_REQ_HOLD_TO_INVEN NetMsg;
			NetMsg.wPosX = wPosX;
			NetMsg.wPosY = wPosY;
#if defined(VN_PARAM) //vietnamtest%%%
			NetMsg.bUseVietnamInven = sCustom.bVietnamGainItem;
#else
			NetMsg.bUseVietnamInven = FALSE;
#endif
			NETSENDTOFIELD ( &NetMsg );

		}
#if defined(VN_PARAM) //vietnamtest%%%
		else
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_VIETNAM_ITEMGET_FAILED") );
			return E_FAIL;		
		}
#endif
	}

	return S_OK;
}

//	Note : ?????? ???? ???? ???????? ?????? ????, ??????, ????????, ??????.
HRESULT GLCharacter::ReqVNInvenTo ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )							return E_FAIL;
	if ( ValidWindowOpen() )						return E_FAIL;	

	SINVENITEM* pInvenItem = m_cVietnamInventory.FindPosItem ( wPosX, wPosY );
	if ( !VALID_HOLD_ITEM () && !pInvenItem )		return E_FAIL;


	const SITEMCUSTOM& sCustom = GET_HOLD_ITEM();


	if ( VALID_HOLD_ITEM () && pInvenItem )
	{
		if ( sCustom.bVietnamGainItem )
		{
			GLMSG::SNETPC_REQ_VNGAIN_EX_HOLD NetMsg;
			NetMsg.wPosX = pInvenItem->wPosX;
			NetMsg.wPosY = pInvenItem->wPosY;
			NETSENDTOFIELD ( &NetMsg );
		}
	}
	else if ( pInvenItem )
	{
		GLMSG::SNETPC_REQ_VNGAIN_TO_HOLD NetMsg;
		NetMsg.wPosX = pInvenItem->wPosX;
		NetMsg.wPosY = pInvenItem->wPosY;
		NETSENDTOFIELD ( &NetMsg );
	}
	else if ( VALID_HOLD_ITEM () && sCustom.bVietnamGainItem )
	{
		//	Note : ?????? ???????? ?????????? ???? ??????.
		//
		SITEM* pItem = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
		GASSERT(pItem&&"?????? ???????? ???????? ????");

		BOOL bOk = m_cVietnamInventory.IsInsertable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )
		{
			//	?????? ????????.
			return E_FAIL;
		}

		GLMSG::SNETPC_REQ_HOLD_TO_VNGAIN NetMsg;
		NetMsg.wPosX = wPosX;
		NetMsg.wPosY = wPosY;

		NETSENDTOFIELD ( &NetMsg );
	}
	return S_OK;
}

//	Note : ?????? ???????????? ?????? ???????? ???????? ???? ????
HRESULT GLCharacter::ReqVietemInvenTo (WORD wPosX, WORD wPosY )
{

	if( m_dwVietnamInvenCount <= 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_VIETNAM_ITEMGET_FAILED") );
		return E_FAIL;
	}

	SINVENITEM* pInvenItem = m_cVietnamInventory.FindPosItem ( wPosX, wPosY );

	if ( !pInvenItem )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )			return E_FAIL;

	WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
	WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

	//	Note : ?????? ???? ?????? ?????? ????.
	//
	//BOOL bITEM_SPACE(FALSE);
	//if ( pItem->ISPILE() )
	//{
	//	//	???? ???????? ????.
	//	WORD wPILENUM = pItem->sDrugOp.wPileNum;
	//	WORD wREQINSRTNUM = ( wBuyNum * pItem->GETAPPLYNUM() );
	//	bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sBUYNID, wPILENUM, wINVENX, wINVENY );
	//}
	//else
	//{
	//	GASSERT(wBuyNum==1&&"?????? ???????? ???????? 1?????? ???? ??????????.");

	//	//	???? ???????? ????.
	//	WORD wInsertPosX(0), wInsertPosY(0);
	//	bITEM_SPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );
	//}

	WORD wInsertPosX(0), wInsertPosY(0);
	BOOL bITEM_SPACE(FALSE);
	bITEM_SPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );

	if ( !bITEM_SPACE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHARGED_ITEM_GET_FB_NOINVEN") );
		return E_FAIL;
	}

	//	???? ???? ??????.
	GLMSG::SNETPC_REQ_VNINVEN_TO_INVEN NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ?????? ?????????? ?????? ???? ????
HRESULT GLCharacter::ReqVNInveReset ()
{
	m_cVietnamInventory.DeleteItemAll();

	GLMSG::SNETPC_REQ_VNGAIN_INVEN_RESET NetMsg;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

// *****************************************************
// Desc: ?????? ?????? ????
// *****************************************************
HRESULT GLCharacter::ReqGrinding ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )			return E_FAIL;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )	return S_FALSE;

	if ( pHold->sBasicOp.emItemType != ITEM_GRINDING )	return S_FALSE;

	BOOL bGrinding = pItem->sBasicOp.emItemType==ITEM_SUIT && pItem->sSuitOp.wReModelNum>0;
	if ( !bGrinding )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_NOITEM") );
		return S_FALSE;
	}

	if ( pHold->sGrindingOp.emTYPE == EMGRINDING_DAMAGE || pHold->sGrindingOp.emTYPE == EMGRINDING_DEFENSE )
	{
		if ( pInvenItem->sItemCustom.GETGRADE(pHold->sGrindingOp.emTYPE)>=GLCONST_CHAR::wGRADE_MAX )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_MAX") );
			return S_FALSE;
		}
	}
	else
	{
		if ( pInvenItem->sItemCustom.GETGRADE(pHold->sGrindingOp.emTYPE)>=GLCONST_CHAR::wGRADE_MAX_REGI )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_MAX") );
			return S_FALSE;
		}
	}



	//	Note : ?????? ????, ???? ???????? ???? ???? ??????
	//
	
	BYTE cGrade = 0;

	cGrade = pInvenItem->sItemCustom.GETGRADE(pHold->sGrindingOp.emTYPE);

	if ( cGrade >= GRADE_HIGH && pHold->sGrindingOp.emGRINDER_TYPE != EMGRINDER_TOP )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_NOT_BEST") );
		return S_FALSE;
	}
	else if ( cGrade >=GRADE_NORMAL && pHold->sGrindingOp.emGRINDER_TYPE < EMGRINDER_HIGH )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_NOT_HIGH") );
		return S_FALSE;
	}


	// ?????? ?? ?????? ?????? ???????? ???????? ?????? ????
	if ( cGrade >= GRADE_HIGH && pItem->sGrindingOp.emGRINDER_TYPE != EMGRINDER_TOP )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_NOT_BESTITEM") );
		return S_FALSE;
	}

	//	Note : ?????? ???? ????
	if ( cGrade >= GRADE_HIGH )
	{
		if ( GLCONST_CHAR::wUSE_GRADE_NUM[cGrade-GRADE_HIGH] > GET_SLOT_ITEM(SLOT_HOLD).wTurnNum )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_NOT_NUM"), GLCONST_CHAR::wUSE_GRADE_NUM[cGrade-GRADE_HIGH]  );
			return S_FALSE;
		}

	}

	//Note : ???? ???????? ?????? GRADE_NORMAL ?? ???? ???? ??????.
	//
	//if ( pInvenItem->sItemCustom.GETGRADE(pHold->sGrindingOp.emTYPE)<GRADE_NORMAL && pHold->sGrindingOp.bHIGH )
	//{
	//	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_NOT_LOW") );
	//	return S_FALSE;
	//}

	if ( pItem->sSuitOp.gdDamage == GLPADATA(0,0) )
	{
		if ( pHold->sGrindingOp.emCLASS != EMGRINDING_CLASS_CLOTH )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_DEFCLASS") );
			return S_FALSE;
		}
	}
	else
	{
		if ( pHold->sGrindingOp.emCLASS != EMGRINDING_CLASS_ARM )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("GRINDING_DEFCLASS") );
			return S_FALSE;
		}
	}

	//	Note : ?????? ???? ???? ????.
	//
	GLMSG::SNET_INVEN_GRINDING NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqDisguise ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )			return E_FAIL;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )	return S_FALSE;

	if ( !pHold->sBasicOp.IsDISGUISE() )	return S_FALSE;

	if ( pItem->sBasicOp.IsDISGUISE() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_FAIL") );
		return S_FALSE;
	}

	if ( ( pHold->sBasicOp.dwReqCharClass & pItem->sBasicOp.dwReqCharClass ) == NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_DEFSUIT") );
		return S_FALSE;
	}
	
	if ( pHold->sBasicOp.emItemType != ITEM_SUIT )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_NOTSUIT") );
		return S_FALSE;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_SUIT )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_NOTSUIT") );
		return S_FALSE;
	}

	if ( pHold->sSuitOp.emSuit != pItem->sSuitOp.emSuit )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_DEFSUIT") );
		return S_FALSE;
	}

	if ( pInvenItem->sItemCustom.nidDISGUISE!=SNATIVEID(false) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_ALREADY") );
		return S_FALSE;
	}

	GLMSG::SNET_INVEN_DISGUISE	NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqCleanser ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )			return E_FAIL;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )	return S_FALSE;

	//if ( pHold->sBasicOp.emItemType!=ITEM_CLEANSER )
	//{
	//	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CLEANSER_FB_NOCLEANSER") );
	//	return S_FALSE;
	//}

	if ( pInvenItem->sItemCustom.nidDISGUISE==SNATIVEID(false) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CLEANSER_FB_NONEED") );
		return S_FALSE;
	}

	GLMSG::SNET_INVEN_CLEANSER	NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqCharCard ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )			return E_FAIL;

	//if ( pItem->sBasicOp.emItemType!=ITEM_CHARACTER_CARD )	return E_FAIL;

	GLMSG::SNET_INVEN_CHARCARD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ???? ???? ????.
HRESULT GLCharacter::ReqStorageCard ( WORD wPosX, WORD wPosY, WORD wSTORAGE )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )					return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGECARD_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_STORAGE_CARD )
	//{
	//	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGECARD_FB_NOITEM") );
	//	return E_FAIL;
	//}

	if ( wSTORAGE < 1 || wSTORAGE>=(EMSTORAGE_CHANNEL-1) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGECARD_FB_INVNUM") );
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_STORAGECARD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.wSTORAGE = wSTORAGE;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqInvenLineCard ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_INVENLINE_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_INVEN_CARD )
	//{
	//	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_INVENLINE_FB_NOITEM") );
	//	return E_FAIL;
	//}

	if ( m_wINVENLINE >= (EM_INVENSIZE_Y-EM_INVEN_DEF_SIZE_Y-1) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_INVENLINE_FB_MAXLINE") );
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_INVENLINE NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqRemodelOpenCard ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if ( ValidRebuildOpen() )					return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REMODELOPEN_FB_NOITEM") );
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_REMODELOPEN NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqGabargeOpenCard ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REMODELOPEN_FB_NOITEM") );
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_GARBAGEOPEN NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

void GLCharacter::ReqGarbageOpenInven()
{
	GLMSG::SNET_INVEN_GARBAGEOPEN NetMsg;
	NetMsg.bInven = true;
	NETSENDTOFIELD ( &NetMsg );
}

HRESULT GLCharacter::ReqStorageOpenCard ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	// ?????? ?????????? ???? ?????????? ?????? ?? ????.
	if ( CInnerInterface::GetInstance().IsStorageWindowOpen () ) return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGEOPEN_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_STORAGE_CONNECT )
	//{
	//	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGEOPEN_FB_NOITEM") );
	//	return E_FAIL;
	//}

	GLMSG::SNET_INVEN_STORAGEOPEN NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqStorageCloseCard()
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;

	GLMSG::SNET_INVEN_STORAGECLOSE NetMsg;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqPremiumSet ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_PREMIUMSET_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_PREMIUMSET )
	//{
	//	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_PREMIUMSET_FB_NOITEM") );
	//	return E_FAIL;
	//}

	GLMSG::SNET_INVEN_PREMIUMSET NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqInvenHairChange ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_HAIR )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_NOITEM") );
	//	return E_FAIL;
	//}

	GLMSG::SNETPC_INVEN_HAIR_CHANGE NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

// *****************************************************
// Desc: ???? ?????? ????
// *****************************************************
HRESULT GLCharacter::ReqInvenHairStyleChange ( WORD wHairStyle )
{
	GLMSG::SNETPC_INVEN_HAIRSTYLE_CHANGE NetMsg;
	NetMsg.wPosX	  = m_wInvenPosX1;
	NetMsg.wPosY	  = m_wInvenPosY1;
	NetMsg.wHairStyle = wHairStyle;
	NETSENDTOFIELD ( &NetMsg );

	m_wInvenPosX1 = 0;
	m_wInvenPosY1 = 0;

	return S_OK;
}

// *****************************************************
// Desc: ???? ?????? ????
// *****************************************************
HRESULT GLCharacter::InvenHairStyleChange ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_HAIR_STYLE )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_NOITEM") );
	//	return E_FAIL;
	//}

	CInnerInterface::GetInstance().ShowGroupFocus( HAIRSTYLECARD_WINDOW );

	m_wInvenPosX1 = wPosX;
	m_wInvenPosY1 = wPosY;

	return S_OK;
}

// *****************************************************
// Desc: ???? ???? ????
// *****************************************************
HRESULT GLCharacter::ReqInvenHairColorChange ( WORD wHairColor )
{
	GLMSG::SNETPC_INVEN_HAIRCOLOR_CHANGE NetMsg;
	NetMsg.wPosX	  = m_wInvenPosX2;
	NetMsg.wPosY	  = m_wInvenPosY2;
	NetMsg.wHairColor = wHairColor;
	NETSENDTOFIELD ( &NetMsg );

	m_wInvenPosX2 = 0;
	m_wInvenPosY2 = 0;

	return S_OK;
}

// *****************************************************
// Desc: ???? ???? ????
// *****************************************************
HRESULT GLCharacter::InvenHairColorChange ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_HAIR_COLOR )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_NOITEM") );
	//	return E_FAIL;
	//}

	DoModal( ID2GAMEINTEXT("MODAL_HAIRCOLOR_INFO"), MODAL_INFOMATION, OK, MODAL_HAIRCOLOR_INFO );

	m_wInvenPosX2 = wPosX;
	m_wInvenPosY2 = wPosY;

	return S_OK;
}

HRESULT GLCharacter::InvenBikeColorChange ( WORD wPosX, WORD wPosY )//add bike color
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_VEHICLE_CHANGE_FB_NOITEM") );
		return E_FAIL;
	}

	if ( !m_bVehicle )	
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_VEHICLE_CHANGE_FB_NOVEHICLE") );
		return E_FAIL;
	}

	DoModal( ID2GAMEINTEXT("MODAL_BIKECOLOR_INFO"), MODAL_INFOMATION, OK, MODAL_BIKECOLOR_INFO );

	m_wInvenPosX3 = wPosX;
	m_wInvenPosY3 = wPosY;

	return S_OK;
}

HRESULT GLCharacter::ReqInvenBikeColorChange ( WORD wBikeColor, WORD wBikeColor1, WORD wBikeColor2, WORD wBikeColor3, WORD wBikeColor4, WORD wBikeColor5 )
{
	GLMSG::SNETPC_INVEN_BIKECOLOR_CHANGE NetMsg;
	NetMsg.wPosX	  = m_wInvenPosX3;
	NetMsg.wPosY	  = m_wInvenPosY3;
	NetMsg.wBikeColor = wBikeColor;
	NetMsg.wBikeColor1	= wBikeColor1;
	NetMsg.wBikeColor2	= wBikeColor2;
	NetMsg.wBikeColor3	= wBikeColor3;
	NetMsg.wBikeColor4	= wBikeColor4;
	NetMsg.wBikeColor5	= wBikeColor5;

	NETSENDTOFIELD ( &NetMsg );

	m_wInvenPosX3 = 0;
	m_wInvenPosY3 = 0;

	return S_OK;
}

//	Note : ?????????? ????.
HRESULT GLCharacter::ReqInvenFaceChange ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_FACE_CHANGE_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_FACE )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_FACE_CHANGE_FB_NOITEM") );
	//	return E_FAIL;
	//}

	GLMSG::SNETPC_INVEN_FACE_CHANGE NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

// *****************************************************
// Desc: ???? ?????? ????
// *****************************************************
HRESULT GLCharacter::ReqInvenFaceStyleChange ( WORD wFaceStyle )
{
	GLMSG::SNETPC_INVEN_FACESTYLE_CHANGE NetMsg;
	NetMsg.wPosX	  = m_wInvenPosX1;
	NetMsg.wPosY	  = m_wInvenPosY1;
	NetMsg.wFaceStyle = wFaceStyle;
	NETSENDTOFIELD ( &NetMsg );

	m_wInvenPosX1 = 0;
	m_wInvenPosY1 = 0;

	return S_OK;
}

// *****************************************************
// Desc: ???? ?????? ????
// *****************************************************
HRESULT GLCharacter::InvenFaceStyleChange ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_FACE_CHANGE_FB_NOITEM") );
		return E_FAIL;
	}

	CInnerInterface::GetInstance().ShowGroupFocus( FACE_CHANGE_WINDOW );

	m_wInvenPosX1 = wPosX;
	m_wInvenPosY1 = wPosY;	

	return S_OK;
}

HRESULT GLCharacter::InvenGenderChange ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_NOITEM") );
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_GENDER_CHANGE )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_ITEMTYPE") );
		return E_FAIL;
	}

	//fix change gender
	//if ( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_EXTREME_W )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_BADCLASS") );
	//	return E_FAIL;
	//}

	DoModal( ID2GAMEINTEXT("MODAL_GENDER_CHANGE"),  MODAL_INFOMATION, YESNO, MODAL_GENDER_CHANGE );	

	m_wInvenPosX1 = wPosX;
	m_wInvenPosY1 = wPosY;

	return S_OK;
}

HRESULT GLCharacter::ReqInvenGenderChange ( WORD wFace, WORD wHair )
{
	GLMSG::SNETPC_INVEN_GENDER_CHANGE NetMsg;
	NetMsg.wPosX = m_wInvenPosX1;
	NetMsg.wPosY = m_wInvenPosY1;
	NetMsg.wFace = wFace;	
	NetMsg.wHair = wHair;
	NETSENDTOFIELD ( &NetMsg );

	m_wInvenPosX1 = 0;
	m_wInvenPosY1 = 0;

	return S_OK;
}


HRESULT GLCharacter::InvenRename ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_NOITEM") );
		return E_FAIL;
	}

	//if ( pItem->sBasicOp.emItemType!=ITEM_RENAME )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_BADITEM") );
	//	return E_FAIL;
	//}

	DoModal ( ID2GAMEINTEXT("INVEN_CHAR_RENAME"), MODAL_INPUT, EDITBOX, MODAL_CHAR_RENAME);

	m_wInvenPosX1 = wPosX;
	m_wInvenPosY1 = wPosY;

	return S_OK;
}

HRESULT GLCharacter::ReqInvenRename ( const char* szCharName )
{
	if( !szCharName )
		return S_FALSE;

	CString strTEMP( szCharName );

#ifdef TH_PARAM	
	// ?????? ???? ???? ????

	if ( !m_pCheckString ) return S_FALSE;

	if ( !m_pCheckString(strTEMP) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_THAICHAR_ERROR"));
		return S_FALSE;
	}
#endif

#ifdef VN_PARAM
	// ?????? ???? ???? ???? 
	if( STRUTIL::CheckVietnamString( strTEMP ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_VNCHAR_ERROR"));
		return S_FALSE;
	}

#endif 


	BOOL bFILTER0 = STRUTIL::CheckString( strTEMP );
	BOOL bFILTER1 = CRanFilter::GetInstance().NameFilter( strTEMP );
	if ( bFILTER0 || bFILTER1 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEEXTEXT("CHARACTER_BADNAME") );
		return S_FALSE;
	}

	GLMSG::SNETPC_INVEN_RENAME NetMsg;
	NetMsg.wPosX = m_wInvenPosX1;
	NetMsg.wPosY = m_wInvenPosY1;
	StringCchCopy ( NetMsg.szName, CHAR_SZNAME, szCharName );
	NETSENDTOFIELD ( &NetMsg );

	m_wInvenPosX1 = 0;
	m_wInvenPosY1 = 0;
	return S_OK;
}

//	Note : ???? ???? ????.
HRESULT GLCharacter::ResetSkillStats ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )						return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RESET_SKST_FB_NOITEM") );
		return E_FAIL;
	}

	DoModal( ID2GAMEINTEXT("MODAL_RESET_SKILLSTATS"),  MODAL_INFOMATION, YESNO, MODAL_RESET_SKILLSTATS );

	m_wInvenPosX1 = wPosX;
	m_wInvenPosY1 = wPosY;

	return S_OK;
}

HRESULT GLCharacter::ReqResetSkillStats ()
{
	GLMSG::SNET_INVEN_RESET_SKST NetMsg;
	NetMsg.wPosX = m_wInvenPosX1;
	NetMsg.wPosY = m_wInvenPosY1;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ?????? ??????, ?????? ???? ???? ????
HRESULT GLCharacter::ReqInvenVietnamGet ( WORD wPosX, WORD wPosY, bool bGetExp )
{
//	if( m_dwVietnamGainType == GAINTYPE_EMPTY ) return E_FAIL;
//	if( m_dwVietnamGainType == GAINTYPE_HALF ) return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		if( bGetExp )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_VIETNAM_EXPGET_FB_NOITEM") );
		}else{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_VIETNAM_ITEMGET_FB_NOITEM") );
		}
		return E_FAIL;
	}

	GLMSG::SNETPC_INVEN_VIETNAM_INVENGET NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.bGetExp = bGetExp;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqDetectWPE ( BOOL bDetect, DWORD dwCharID )
	{
 		 if ( m_dwCharID!=dwCharID ) return S_FALSE;
  		if ( !bDetect )  return S_FALSE;
  		if  ( bDetect ) m_bDisconnect = TRUE;

 		 return S_OK;
		}

//	Note : ???????? ?????? ???????? ( ??????, ?????????? ?? ).
HRESULT GLCharacter::ReqInvenDrug ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;	

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;	

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )		return E_FAIL;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );	

	if ( pItem->sBasicOp.emItemType == ITEM_CURE )
	{
		//	Note : pk ?????? ?????? ???? ?????? ???? ???????? ?????? ??????.
		//
		PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap ();
		if ( pLandManClient && !pLandManClient->IsInstantMap() )
		{
			if ( pItem->sDrugOp.emDrug == ITEM_DRUG_HP || 
				pItem->sDrugOp.emDrug == ITEM_DRUG_HP_MP || 
				pItem->sDrugOp.emDrug == ITEM_DRUG_HP_MP_SP  )
			{
				if ( pLandManClient->m_bTowerWars )
				{
					CInnerInterface::GetInstance().PrintMsgText ( 
									NS_UITEXTCOLOR::NEGATIVE, "Unable to use this item" );
					return E_FAIL;
				}
			}
		}

		// ???? ???? ????
		if ( !SIMPLE_CHECK_ITEM( pItem->sBasicOp.sNativeID ) ) return S_FALSE;

		DWORD dwPK_LEVEL = GET_PK_LEVEL();
		if ( dwPK_LEVEL != UINT_MAX && dwPK_LEVEL>GLCONST_CHAR::dwPK_DRUG_ENABLE_LEVEL )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DRUG_PK_LEVEL") );
			return E_FAIL;
		}

		if ( m_sCONFTING.IsFIGHTING() && !m_sCONFTING.IsRECOVE() )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("CONFRONT_RECOVE"), 0 );
			return E_FAIL;
		}

		if ( CheckCoolTime( pItem->sBasicOp.sNativeID ) ) return E_FAIL;

		if ( pItem->sDrugOp.emDrug!=ITEM_DRUG_NUNE )
		{
			switch ( pItem->sDrugOp.emDrug )
			{
			case ITEM_DRUG_HP:
			{	// 9/13/2016 - Anti  Auto Pots System - Eduj
				if ( !m_bUsePots ) m_bUsePots = TRUE;
				if ( m_bUsePots ) m_dwTickCount++;
				if ( m_bDisablePots )
				{
				  if ( !m_bDetectOnce )
				      {
		                CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, "Force Disconnection After 10sec." );
                        m_bDetectOnce = TRUE;
		              }
		              return S_FALSE; 
				}
				if ( m_sHP.dwMax == m_sHP.dwNow )		return S_FALSE; 
			}
				break;
			case ITEM_DRUG_MP:
				if ( m_sMP.dwMax == m_sMP.dwNow )		return S_FALSE;
				break;

			case ITEM_DRUG_SP:
				if ( m_sSP.dwMax == m_sSP.dwNow )		return S_FALSE;
				break;

			case ITEM_DRUG_HP_MP:
				{// 9/13/2016 - Anti  Auto Pots System - Eduj
				if ( !m_bUsePots ) m_bUsePots = TRUE;
				if ( m_bUsePots ) m_dwTickCount++;
				if ( m_bDisablePots )
				{
				  if ( !m_bDetectOnce )
				      {
		                CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, "Force Disconnection After 10sec." );
                        m_bDetectOnce = TRUE;
		              }
		              return S_FALSE; 
				}
				return S_FALSE; 
				if ( m_sHP.dwMax==m_sHP.dwNow && m_sMP.dwMax==m_sMP.dwNow )		return S_FALSE;
				}
				break;
			case ITEM_DRUG_MP_SP:
				if ( m_sMP.dwMax==m_sMP.dwNow && m_sSP.dwMax==m_sSP.dwNow )		return S_FALSE;
				break;

			case ITEM_DRUG_HP_MP_SP:
				{// 9/13/2016 - Anti  Auto Pots System - Eduj
				if ( !m_bUsePots ) m_bUsePots = TRUE;
				if ( m_bUsePots ) m_dwTickCount++;
				if ( m_bDisablePots )
				{
				  if ( !m_bDetectOnce )
				      {
		                CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, "Force Disconnection After 10sec." );
                        m_bDetectOnce = TRUE;
		              }
		              return S_FALSE; 
				}
				if ( m_sHP.dwMax==m_sHP.dwNow && m_sMP.dwMax==m_sMP.dwNow
					&& m_sSP.dwMax==m_sSP.dwNow )		return S_FALSE;
				}
				break;

			case ITEM_DRUG_CURE:
				if ( !ISSTATEBLOW() )				return S_FALSE;
				break;

			case ITEM_DRUG_HP_CURE:
				{// 9/13/2016 - Anti  Auto Pots System - Eduj
				if ( !m_bUsePots ) m_bUsePots = TRUE;
				if ( m_bUsePots ) m_dwTickCount++;
				if ( m_bDisablePots )
				{
				  if ( !m_bDetectOnce )
				      {
		                CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, "Force Disconnection After 10sec." );
                        m_bDetectOnce = TRUE;
		              }
		              return S_FALSE; 
				}
				if ( m_sHP.dwMax == m_sHP.dwNow && !ISSTATEBLOW() ) 	return S_FALSE;
				}
				break;

			case ITEM_DRUG_HP_MP_SP_CURE:
				{// 9/13/2016 - Anti  Auto Pots System - Eduj
				if ( !m_bUsePots ) m_bUsePots = TRUE;
				if ( m_bUsePots ) m_dwTickCount++;
				if ( m_bDisablePots )
				{
				  if ( !m_bDetectOnce )
				      {
		                CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, "Force Disconnection After 10sec." );
                        m_bDetectOnce = TRUE;
		              }
		              return S_FALSE; 
				}
				if ( m_sHP.dwMax==m_sHP.dwNow && m_sMP.dwMax==m_sMP.dwNow
					&& m_sSP.dwMax==m_sSP.dwNow && !ISSTATEBLOW() ) 		return S_FALSE;
				}
				break;
			};

			GLMSG::SNETPC_REQ_INVENDRUG NetMsg;
			NetMsg.wPosX = wPosX;
			NetMsg.wPosY = wPosY;

			NETSENDTOFIELD ( &NetMsg );

			return S_OK;
		}

  }

	// ???? ???????? ?????????? ???????? ???? ??????
	if ( !pHold )
	{

		// ???? ???? ????
		if ( !SIMPLE_CHECK_ITEM( pItem->sBasicOp.sNativeID ) ) return S_FALSE;

		if ( CheckCoolTime( pItem->sBasicOp.sNativeID ) ) return S_FALSE;

		if ( pItem->sBasicOp.emItemType == ITEM_SKILL )
		{
			ReqInvenSkill ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_RECALL )
		{
			ReqReCall ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_TELEPORT_CARD )
		{
			ReqTeleport ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_BOX )
		{
			ReqBoxOpen ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_RANDOMITEM )
		{
			ReqRandumBoxOpen ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_CHARACTER_CARD )
		{
			ReqCharCard ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_STORAGE_CARD )
		{
			//	Note : ???????????? ?????? ???? ????????.
			CInnerInterface::GetInstance().SetStorageChargeOpen ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_INVEN_CARD )
		{
			ReqInvenLineCard ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_STORAGE_CONNECT )
		{
			ReqStorageOpenCard ( wPosX, wPosY );
		}
		else if( pItem->sBasicOp.emItemType == ITEM_REMODEL )
		{
			ReqRemodelOpenCard ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType == ITEM_GARBAGE_CARD )
		{
			ReqGabargeOpenCard ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_PREMIUMSET )
		{
			ReqPremiumSet ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_SKP_STAT_RESET )
		{
			ResetSkillStats ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_PRIVATEMARKET )
		{
			// ???????? ?????? ???? ???? ????
			if ( !IsACTION ( GLAT_IDLE ) || m_sREACTION.ISVALID() )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_OPEN_FB_NOIDLE") );
				return S_FALSE;
			}

			m_wPMPosX = (wPosX);
			m_wPMPosY = (wPosY);

			//	Note : ???????????? ???? ???? ????.
			CInnerInterface::GetInstance().SetPrivateMarketMake ();
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_HAIR )
		{
			ReqInvenHairChange ( wPosX, wPosY );
		}

		else if ( pItem->sBasicOp.emItemType==ITEM_HAIR_COLOR )
		{
			InvenHairColorChange ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_BIKE_COLOR )//add bike color
		{
			InvenBikeColorChange ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_HAIR_STYLE )
		{
			InvenHairStyleChange ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_FACE )
		{
			ReqInvenFaceChange( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_FACE_STYLE )
		{
			InvenFaceStyleChange( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_TAXI_CARD )
		{
			InvenUseTaxiCard( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_NPC_RECALL )
		{
			InvenUseNpcRecall( wPosX, wPosY );
		}			
		else if ( pItem->sBasicOp.emItemType==ITEM_GENDER_CHANGE )
		{
			InvenGenderChange ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_RENAME )
		{
			InvenRename ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType ==ITEM_VIETNAM_EXPGET )
		{
			ReqInvenVietnamGet ( wPosX, wPosY, TRUE );
		}
		else if ( pItem->sBasicOp.emItemType ==ITEM_VIETNAM_ITEMGET )
		{
			ReqInvenVietnamGet ( wPosX, wPosY, FALSE );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_PET_RENAME )
		{
			GLGaeaClient::GetInstance().GetPetClient()->ReqInputName ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_PET_COLOR )
		{
			GLGaeaClient::GetInstance().GetPetClient()->ReqInputColor ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_PET_STYLE )
		{
			GLGaeaClient::GetInstance().GetPetClient()->ReqInputStyle ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_PET_SKILL )
		{
			GLGaeaClient::GetInstance().GetPetClient()->ReqLearnSkill ( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_SUMMON_CARD )
		{
			GLGaeaClient::GetInstance().ReqUseSummonCard( wPosX, wPosY );
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_DUAL_PETCARD)
		{
			GLGaeaClient::GetInstance().GetPetClient()->ReqDualSkillPet(wPosX, wPosY);
		}
		else if ( pItem->sBasicOp.emItemType==ITEM_PET_CARD )
		{
			// ???? ???????? ??????
			if ( m_fPetDelay >= 5.0f )	//add pet delay by CNDev
			{
				if ( GLGaeaClient::GetInstance().GetPetClient()->IsVALID() )
				{
					GLMSG::SNETPET_REQ_UNUSEPETCARD NetMsg;
					NetMsg.dwGUID = GLGaeaClient::GetInstance().GetPetClient()->m_dwGUID;
					NetMsg.dwPetID = pInvenItem->sItemCustom.dwPetID;
					NETSENDTOFIELD ( &NetMsg );
				}
				else 
				{
					ReqUsePetCard ( wPosX, wPosY );
					m_wInvenPosX1 = wPosX;
					m_wInvenPosY1 = wPosY;
				}

				m_fPetDelay = 0.0f;
			}else{
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_PET_DELAY") );
			}

			return S_OK;
		}
	}
	// ???? ?????? ?????? ????
	else	
	{

		// ???? ???? ???? ?????? ????
		if ( pHold->sBasicOp.IsDISGUISE() )
		{
			ReqDisguise ( wPosX, wPosY );
			return S_OK;
		}

		// ???? ???? ????
		if ( !SIMPLE_CHECK_ITEM( pHold->sBasicOp.sNativeID ) ) return S_FALSE;

		if ( CheckCoolTime( pHold->sBasicOp.sNativeID ) ) return S_FALSE;

		// ???????????? ????????
		if ( pHold->sBasicOp.emItemType == ITEM_PET_REVIVE )
		{
			if ( pItem->sBasicOp.emItemType!=ITEM_PET_CARD )	return S_FALSE;
			
			m_wInvenPosX1 = wPosX;
			m_wInvenPosY1 = wPosY;
			
			m_sTempItemCustom = GET_HOLD_ITEM ();
			RELEASE_HOLD_ITEM ();

			// ?????? ???????????????? ??????.
			if ( !CInnerInterface::GetInstance().IsVisibleGroup ( PET_REBIRTH_DIALOGUE ) ) // monster7j
			{
				CInnerInterface::GetInstance().ShowGroupFocus ( PET_REBIRTH_DIALOGUE );
			}
		}
		else if ( pHold->sBasicOp.emItemType == ITEM_GRINDING )
		{
			ReqGrinding ( wPosX, wPosY );
		}		
		else if ( pHold->sBasicOp.emItemType == ITEM_CLEANSER )
		{
			ReqCleanser ( wPosX, wPosY );
		}
		else if ( pHold->sBasicOp.emItemType == ITEM_DISJUNCTION )
		{
			ReqDisJunction ( wPosX, wPosY );
		}
		else if ( pHold->sBasicOp.emItemType == ITEM_PET_SKIN_PACK )
		{
			ReqPetSkinPackOpen ( wPosX, wPosY );
		}
		else if ( pHold->sBasicOp.emItemType == ITEM_PET_FOOD )
		{
			GLGaeaClient::GetInstance().GetPetClient()->ReqGiveFood ( wPosX, wPosY );
		}
		else if ( pHold->sBasicOp.emItemType == ITEM_VEHICLE_OIL )
		{
			ReqVehicleGiveBattery ( wPosX, wPosY );
		}
	}

	return S_OK;
}

DWORD GLCharacter::GetAmountActionQ ( WORD wSLOT )
{
	if ( GLTradeClient::GetInstance().Valid() )	return 0;
	if ( EMACTIONQUICK_SIZE <= wSLOT )		return 0;
	
	const SACTION_SLOT &sACTION = m_sACTIONQUICK[wSLOT];

	return m_cInventory.CountTurnItem ( sACTION.sNID );
}

HRESULT GLCharacter::ReqSchoolWarParticipate ()
{
	if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) ||  m_sPMarket.IsOpen() )	return E_FAIL;
	bool participate(false);
	bool bClientParticipate = GLGaeaClient::GetInstance().m_bParticipate;
	//float ftimeBefore = GLGaeaClient::GetInstance().m_fNextWarTimer;
	//int nMin = (int)ftimeBefore/60;
	//if ( nMin < 2 )
	{
		if ( m_wLevel > 259 )
		{
			if ( !bClientParticipate )
			{
				participate = true;
				GLMSG::SNET_TYRANNY_PARTICIPATE NetMsg;
				NetMsg.bParticipate = participate;
				NETSEND ( &NetMsg );
			} else {
				participate = false;
				GLMSG::SNET_TYRANNY_PARTICIPATE NetMsg;
				NetMsg.bParticipate = participate;
				NETSEND ( &NetMsg );
			}
		}
		else
		{
			return E_FAIL;
			CInnerInterface::GetInstance().PrintConsoleText("You must be Lv.260 to participate.");
		}
	}
	return S_OK;
}
//	Note : ???? ???????? ???? ???? ????. ( ?????? ???? ?????? ).
HRESULT GLCharacter::ReqActionQ ( WORD wSLOT )
{
	HRESULT hr = S_OK;
	if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	if ( EMACTIONQUICK_SIZE <= wSLOT )		return E_FAIL;
	
	const SACTION_SLOT &sACTION = m_sACTIONQUICK[wSLOT];
	if ( !sACTION.VALID() )	return E_FAIL;

	switch ( sACTION.wACT )
	{
	case EMACT_SLOT_DRUG:
		{
			SINVENITEM* pInvenItem = m_cInventory.FindItem ( sACTION.sNID );
			if ( !pInvenItem )						return E_FAIL;

			SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sACTION.sNID );
			if ( !pITEM )							return E_FAIL;

			if ( pITEM->sBasicOp.emItemType != ITEM_CURE && 
				 pITEM->sBasicOp.emItemType != ITEM_TELEPORT_CARD &&		
				 pITEM->sBasicOp.emItemType != ITEM_RECALL /*&& 
				 pITEM->sBasicOp.emItemType != ITEM_PET_CARD*/ )
				 // Disabled to avoid crash on Field Server
				 // 08-14-2013 11:25 PM
				return E_FAIL;

			hr = ReqInvenDrug ( pInvenItem->wPosX, pInvenItem->wPosY );
			if ( FAILED(hr) )						return hr;
		}
		break;
	};

	return S_OK;
}

//	Note : ???? ?????? ????. ( ???? ??????????. )
HRESULT GLCharacter::ReqInvenSkill ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )					return E_FAIL;
	//if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;
	//if( ValidRebuildOpen() )	return E_FAIL;	// ITEMREBUILD_MARK

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_SKILL )	return E_FAIL;

	SNATIVEID sSKILL_ID = pItem->sSkillBookOp.sSkill_ID;

	if ( ISLEARNED_SKILL(sSKILL_ID) )
	{
		//	???? ?????? ????.
		return E_FAIL;
	}

	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
	{
		//	???? ???? ???? ?????? ???????? ????????.
		return E_FAIL;
	}

	//	Note : ???? ?????? ??????????.
	//
	GLMSG::SNETPC_REQ_LEARNSKILL NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqSkillUp ( const SNATIVEID skill_id )
{
	if ( !IsValidBody() )	return E_FAIL;

	EMSKILL_LEARNCHECK emSKILL_LVLUPCHECK = EMSKILL_LEARN_UNKNOWN;
	SCHARSKILL* pCHARSKILL = GETLEARNED_SKILL(skill_id);
	if ( !pCHARSKILL )
	{
		//	?????????? ???? ?? ?? ???? ????.
		return E_FAIL;
	}

	emSKILL_LVLUPCHECK = CHECKLEARNABLE_SKILL(skill_id);
	if ( emSKILL_LVLUPCHECK!=EMSKILL_LEARN_OK )
	{
		//	???? ?????? ??????????.
		return E_FAIL;
	}

	GLMSG::SNETPC_REQ_SKILLUP NetMsg;
	NetMsg.skill_id = skill_id;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqReCall ( WORD wPosX, WORD wPosY )
{
	//	?????? ???? ?????? ???? ????.
	//if ( !IsValidBody() )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_CONDITION") );
	//	return E_FAIL;
	//}
	
	//	???????? ????.
	//if ( GLTradeClient::GetInstance().Valid() )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_CONDITION") );
	//	return E_FAIL;
	//}

	//	???? ?????? ????.
	if ( m_sCONFTING.IsCONFRONTING() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_CONDITION") );
		return E_FAIL;
	}

	//	Note : pk ?????? ?????? ???? ?????? ???? ???? ?????? ?????? ??????.
	//
	DWORD dwPK_LEVEL = GET_PK_LEVEL();
	if ( dwPK_LEVEL != UINT_MAX && dwPK_LEVEL>GLCONST_CHAR::dwPK_RECALL_ENABLE_LEVEL )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_PK_LEVEL") );
		return E_FAIL;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_ITEM") );
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_RECALL )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_ITEM") );
		return E_FAIL;
	}

	PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap ();
	if ( pLandManClient && !pLandManClient->IsInstantMap() )
	{
		if ( pLandManClient->m_bTowerWars )
		{
			CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, "Unable to use this item" );
			return E_FAIL;
		}
	}
	if ( pItem->sDrugOp.emDrug!=ITEM_DRUG_CALL_SCHOOL &&
		pItem->sDrugOp.emDrug!=ITEM_DRUG_CALL_REGEN &&
		pItem->sDrugOp.emDrug!=ITEM_DRUG_CALL_LASTCALL )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_ITEM") );
		return E_FAIL;
	}

	if ( pItem->sDrugOp.emDrug==ITEM_DRUG_CALL_SCHOOL && pItem->sDrugOp.emDrug==ITEM_DRUG_CALL_REGEN && pItem->sDrugOp.emDrug==ITEM_DRUG_CALL_LASTCALL )// Added by JADev - Anti SP, BP During War
	{
		PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap ();
			if ( !pLandManClient )						return E_FAIL;

			GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
			GLMapList::FIELDMAP_ITER iter = MapsList.find ( pLandManClient->GetMapID ().dwID );
			if ( iter==MapsList.end() )					return E_FAIL;

			const SMAPNODE *pMapNode = &(*iter).second;

			// ?????????????? ????
			if ( !pMapNode->bCardActivity )
			{
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_RECALL_FB_NOUSE_CARD") );// Added by JADev - Anti SP, BP During War
				return S_OK;
			}
	}
	if( ValidRebuildOpen() )	// ITEMREBUILD_MARK
		CInnerInterface::GetInstance().CloseItemRebuildWindow();

	if( ValidGarbageOpen() )	// ??????
		CInnerInterface::GetInstance().CloseItemGarbageWindow();

	if ( ValidItemMixOpen() )
		CInnerInterface::GetInstance().CloseItemMixWindow();

	GLMSG::SNETPC_REQ_INVEN_RECALL NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

// ?????? ???? ???? - ?????????? ????
HRESULT GLCharacter::ReqTeleport ( WORD wPosX, WORD wPosY )
{	
	//	???? ?????? ????.
	if ( m_sCONFTING.IsCONFRONTING() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_CONDITION") );
		return E_FAIL;
	}

	//	Note : pk ?????? ?????? ???? ?????? ???? ???? ?????? ?????? ??????.
	//
	DWORD dwPK_LEVEL = GET_PK_LEVEL();
	if ( dwPK_LEVEL != UINT_MAX && dwPK_LEVEL>GLCONST_CHAR::dwPK_RECALL_ENABLE_LEVEL )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_PK_LEVEL") );
		return E_FAIL;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_ITEM") );
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_TELEPORT_CARD )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_ITEM") );
		return E_FAIL;
	}

	if ( pItem->sDrugOp.emDrug!=ITEM_DRUG_CALL_TELEPORT )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_ITEM") );
		return E_FAIL;
	}

	if ( ValidRebuildOpen() )	// ITEMREBUILD_MARK
		CInnerInterface::GetInstance().CloseItemRebuildWindow();

	if ( ValidGarbageOpen() )	// ??????
		CInnerInterface::GetInstance().CloseItemGarbageWindow();

	if ( ValidItemMixOpen() )
		CInnerInterface::GetInstance().CloseItemMixWindow();

	GLMSG::SNETPC_REQ_INVEN_TELEPORT NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;

}

HRESULT GLCharacter::ReqBoxOpen ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )	return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOITEM") );
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_BOX )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOBOX") );
		return E_FAIL;
	}

	if ( !pItem->sBox.VALID() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_EMPTY") );
		return E_FAIL;
	}

	//	Note : ?????? ???? ???? ????.
	//
	GLInventory cInvenTemp;
	cInvenTemp.SetAddLine ( m_cInventory.GETAddLine(), true );
	cInvenTemp.Assign ( m_cInventory );

	for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		SITEMCUSTOM sCUSTOM;
		sCUSTOM.sNativeID = pItem->sBox.sITEMS[i].nidITEM;
		if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
		if ( !pITEM )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_INVALIDITEM") );
			return E_FAIL;
		}

		BOOL bOK = cInvenTemp.InsertItem ( sCUSTOM );
		if ( !bOK )
		{
			//	Note : ?????? ?????? ???? ?????? ??????.
			//
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOTINVEN") );
			return E_FAIL;
		}
	}

	//	Note : ?????? ???? ?????? ????.
	//
	GLMSG::SNET_INVEN_BOXOPEN NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqGMItem ( SNATIVEID sItemID ,WORD wNum ,WORD wPass) //add itemcmd
{

	if ( wPass != GLCONST_CHAR::wGMItemPass )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_PASS") );
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_NOTITEM") );
		return E_FAIL;
	}

	if ( wNum >= GLCONST_CHAR::wGMItemMax )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_MAX") );
		return E_FAIL;
	}

	GLInventory cInvenTemp;
	cInvenTemp.SetAddLine ( m_cInventory.GETAddLine(), true );
	cInvenTemp.Assign ( m_cInventory );

	for ( int i=0; i< wNum; ++i )
	{
		SITEMCUSTOM sCUSTOM;
		sCUSTOM.sNativeID = pItem->sBasicOp.sNativeID;

		if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
		if ( !pITEM )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_FAIL") );
			return E_FAIL;
		}

		BOOL bOK = cInvenTemp.InsertItem ( sCUSTOM );
		if ( !bOK )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_INFAIL") );
			return E_FAIL;
		}
	}

	GLMSG::SNET_INVEN_GMITEM NetMsg;
	NetMsg.sItemID = sItemID;
	NetMsg.wNum = wNum;
	NetMsg.wPass = wPass;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqRandumBoxOpen ( WORD wPosX, WORD wPosY )
{
	//if ( !IsValidBody() )	return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_FAIL") );
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_RANDOMITEM )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_FAIL") );
		return E_FAIL;
	}

	if ( !pItem->sRandomBox.VALID() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_EMPTY") );
		return E_FAIL;
	}

	//	Note : ?????? ???? ?????? ????.
	//
	GLMSG::SNET_INVEN_RANDOMBOXOPEN NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqPetSkinPackOpen ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )	return S_FALSE;

	if ( pHold->sBasicOp.emItemType != ITEM_PET_SKIN_PACK )	return S_FALSE;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_FAIL") );
		return E_FAIL;
	}


	if ( !pHold->sPetSkinPack.VALID() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_EMPTY") );
		return E_FAIL;
	}

	// ???? ???? ???? ???? ???? ????
	GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
	if ( pMyPet == NULL || !pMyPet->IsVALID () )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_PET_FAIL") );
		return E_FAIL;
	}

	if( pInvenItem->sItemCustom.dwPetID != pMyPet->m_dwPetID )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_DIFFER_CARD_FAIL") );
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_PET_CARD )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_PETCARD_FAIL") );
		return E_FAIL;
	}

	if( !CInnerInterface::GetInstance().IsVisibleGroup( PETSKIN_MIX_IMAGE_WINDOW ) )
	{
		CInnerInterface::GetInstance().GetPetSkinMixImage()->SetItemData( wPosX, wPosY, pHold->sBasicOp.sNativeID );
		CInnerInterface::GetInstance().ShowGroupFocus( PETSKIN_MIX_IMAGE_WINDOW );
	}

	
	//	Note : ?????? ???? ?????? ????.
	//
	/*GLMSG::SNETPET_SKINPACKOPEN NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );*/
	return S_OK;
}

HRESULT GLCharacter::ReqDisJunction ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )	return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_DISJUNCTION_FB_FAIL") );
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )	return S_FALSE;

	if ( pHold->sBasicOp.emItemType!=ITEM_DISJUNCTION )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_DISJUNCTION_FB_FAIL") );
		return S_FALSE;
	}

	if ( pInvenItem->sItemCustom.nidDISGUISE==SNATIVEID(false) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_DISJUNCTION_FB_NONEED") );
		return S_FALSE;
	}

	//	Note : ?????? ?????? ???? ????.
	//
	GLMSG::SNET_INVEN_DISJUNCTION NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ???????? ?????????? ????????.
HRESULT GLCharacter::ReqSlotTo ( EMSLOT emSlot )
{
	if ( !VALID_HOLD_ITEM() && !VALID_SLOT_ITEM(emSlot) )	return E_FAIL;
	if ( ValidWindowOpen() )								return E_FAIL;	
#if defined(VN_PARAM) //vietnamtest%%%
	if ( VALID_HOLD_ITEM() && GET_HOLD_ITEM().bVietnamGainItem )	return E_FAIL;
#endif


	//	Note : ?????????? ???? ???? ???? ???? ?????? ???? ?? ?? ?????? ??.
	//
	if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) )
	{
		return E_FAIL;
	}


	//	SLOT <-> HOLD ( ???? ?? ???? )
	bool bEMPTY_SLOT = !ISEMPTY_SLOT(GET_HOLD_ITEM().sNativeID,emSlot);
	if ( VALID_HOLD_ITEM() && bEMPTY_SLOT )
	{
		BOOL bOk;
		bOk = ACCEPT_ITEM ( GET_HOLD_ITEM().sNativeID );
		if ( !bOk )
		{
			//	???????? ????.
			return E_FAIL;
		}

		bOk = CHECKSLOT_ITEM ( GET_HOLD_ITEM().sNativeID, emSlot );
		if ( !bOk )
		{
			//	?????????? ???? ????????.
			return E_FAIL;
		}

		SITEM *pItem = GLItemMan::GetInstance().GetItem(GET_HOLD_ITEM().sNativeID);

		if ( pItem && pItem->sSuitOp.emSuit==SUIT_HANDHELD && pItem->sSuitOp.IsBOTHHAND() )
		{
			EMSLOT emRHand = GetCurRHand();
			EMSLOT emLHand = GetCurLHand();

			SITEM *pItem1=NULL, *pItem2=NULL;
			if ( VALID_SLOT_ITEM(emLHand) && VALID_SLOT_ITEM(emRHand) )
			{
				pItem1 = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emLHand).sNativeID);
				pItem2 = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emRHand).sNativeID);

				WORD wInvenPosX, wInvenPosY;
				bOk = m_cInventory.FindInsrtable ( pItem1->sBasicOp.wInvenSizeX, pItem1->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
				if ( !bOk )
				{
					//	?????????? ???? ?????? ????.
					return E_FAIL;
				}
			}
		}
		//add ownership
		const SITEMCUSTOM& sHoldItem = GET_HOLD_ITEM();

		DWORD dwOwner = sHoldItem.dwOwnerID;
		DWORD dwOwnerID =m_dwCharID;

		
		if ( pItem->sBasicOp.IsOwner() )
		{
			if ( !( dwOwnerID == dwOwner)) return E_FAIL;
		}
		
		//

		GLMSG::SNETPC_REQ_SLOT_EX_HOLD NetMsg;
		NetMsg.emSlot = emSlot;
		NETSENDTOFIELD ( &NetMsg );
	}
	//	SLOT -> HOLD ( ???? )
	else if ( VALID_SLOT_ITEM(emSlot) )
	{
		GLMSG::SNETPC_REQ_SLOT_TO_HOLD NetMsg;
		NetMsg.emSlot = emSlot;
		NETSENDTOFIELD ( &NetMsg );
	}
	//	SLOT <- HOLD ( ???? )
	else if ( VALID_HOLD_ITEM() )
	{
		BOOL bOk;
		bOk = ACCEPT_ITEM ( GET_HOLD_ITEM().sNativeID );
		if ( !bOk )
		{
			//	???????? ????.
			return E_FAIL;
		}

		bOk = CHECKSLOT_ITEM ( GET_HOLD_ITEM().sNativeID, emSlot );
		if ( !bOk )
		{
			//	?????????? ???? ????????.
			return E_FAIL;
		}

		//add ownership
		const SITEMCUSTOM& sHoldItem = GET_HOLD_ITEM();
		SITEM *pItem = GLItemMan::GetInstance().GetItem(GET_HOLD_ITEM().sNativeID);

		DWORD dwOwner = sHoldItem.dwOwnerID;
		DWORD dwOwnerID =m_dwCharID;

		if ( pItem->sBasicOp.IsOwner() )
		{
			if ( !( dwOwnerID == dwOwner)) return E_FAIL;
		}
		//

		GLMSG::SNETPC_REQ_HOLD_TO_SLOT NetMsg;
		NetMsg.emSlot = emSlot;
		NETSENDTOFIELD ( &NetMsg );		

	}

	return S_OK;
}


//add itemcolor
// slot of item selected , position of item in X, position of item in X
HRESULT GLCharacter::ReqSlotDrug ( EMSLOT emSlot )
{
	if ( !VALID_HOLD_ITEM() && !VALID_SLOT_ITEM(emSlot) )	return E_FAIL;
	if ( ValidWindowOpen() )								return E_FAIL;	

	if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) )	return E_FAIL;

	//get the hold item
	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );	
	if ( !pHold )	return E_FAIL;

	//get the item in slot
	SITEM *pSlot = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emSlot).sNativeID);
	if ( !pSlot )	return E_FAIL;

	//check if hold item is in cooldown
	if ( CheckCoolTime( pHold->sBasicOp.sNativeID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("DRUG_ITEM_COOLTIME") );
		return E_FAIL;
	}

	if ( pHold )
	{
		//item type color
		if ( pHold->sBasicOp.emItemType == ITEM_ITEM_COLOR )
		{
		  if (   pSlot->sSuitOp.emSuit != SUIT_HEADGEAR && 
				 pSlot->sSuitOp.emSuit != SUIT_UPPER && 
				 pSlot->sSuitOp.emSuit != SUIT_LOWER && 
				 pSlot->sSuitOp.emSuit != SUIT_HAND &&		
				 pSlot->sSuitOp.emSuit != SUIT_FOOT  )  				
	        {//check if the item in the slot can be colored

				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("ITEM_CANNOT_COLOR") );
			}
			else
			{
				ReqSlotColor ( emSlot );
			}	
		}
	}
	
	return S_OK;
}

HRESULT GLCharacter::ReqSlotColor ( EMSLOT emSlot )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;

	//get item in slot
	SITEM *pSlot = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emSlot).sNativeID);
	if ( !pSlot )	return E_FAIL;

	//get hold item
	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )	return E_FAIL;

	//check if hold item is itemcolor card
	if ( pHold->sBasicOp.emItemType != ITEM_ITEM_COLOR ) return E_FAIL;

	//check if the item in the slot can be colored
	 if (   pSlot->sSuitOp.emSuit != SUIT_HEADGEAR && 
		    pSlot->sSuitOp.emSuit != SUIT_UPPER && 
		    pSlot->sSuitOp.emSuit != SUIT_LOWER && 
			pSlot->sSuitOp.emSuit != SUIT_HAND &&		
			pSlot->sSuitOp.emSuit != SUIT_FOOT  )  
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("ITEM_CANNOT_COLOR") );
		return E_FAIL;
	}

	//check inventory for vacant
	bool bOK;
	WORD wInvenPosX, wInvenPosY;
	bOK = m_cInventory.FindInsrtable ( pHold->sBasicOp.wInvenSizeX, pHold->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
	if ( !bOK )
	{//fail if no vacant
		return E_FAIL;
	}
	//put back the color card in inventory
	ReqInvenTo(wInvenPosX,wInvenPosY);

	//remember the slot to modify color
	m_ColorSlot = emSlot;
	// do color modal
	DoModal( ID2GAMEINTEXT("MODAL_ITEMCOLOR_INFO"), MODAL_INFOMATION, OK, MODAL_ITEMCOLOR_INFO );
	//remember the inventory position for deletion later
	//must prevent player from moving items in inventory while item color card is on
	m_wInvenPosX4 = wInvenPosX;
	m_wInvenPosY4 = wInvenPosY;

	return S_OK;
}

HRESULT GLCharacter::ReqInvenItemColorChange ( WORD wItemColor )//add itemcolor
{
	GLMSG::SNETPC_SLOT_ITEMCOLOR_CHANGE NetMsg;
	NetMsg.wPosX	  = m_wInvenPosX4;
	NetMsg.wPosY	  = m_wInvenPosY4;
	NetMsg.wItemColor = wItemColor;
	NetMsg.emSlot     = m_ColorSlot;
	NETSENDTOFIELD ( &NetMsg );

	m_wInvenPosX4 = 0;
	m_wInvenPosY4 = 0;
	m_ColorSlot = SLOT_TSIZE;
	return S_OK;
}

HRESULT GLCharacter::ReqSlotChange()
{
	//	Note : ???? ???? ???? ???? ?????? ???? ?? ?? ?????? ??.
	//
	if( IsACTION(GLAT_SKILL) )
	{
		return E_FAIL;
	}


	GLMSG::SNETPC_REQ_SLOT_CHANGE NetMsg;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ?????? ?????? ????.
HRESULT GLCharacter::ReqHoldToField ( const D3DXVECTOR3 &vPos )
{
	if ( !VALID_HOLD_ITEM () )					return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;	

	const SITEMCUSTOM& sHoldItem = GET_HOLD_ITEM();
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sHoldItem.sNativeID );
	if ( !pItem )				return E_FAIL;

	//	????????
	if ( !pItem->sBasicOp.IsTHROW() )	return E_FAIL;


	// ????????????
	if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD )
	{
		GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
		if ( pMyPet->IsVALID () && sHoldItem.dwPetID == pMyPet->m_dwPetID )
		{
			return E_FAIL;
		}
	}	

	GLMSG::SNETPC_REQ_HOLD_TO_FIELD NetMsg;
	NetMsg.vPos = vPos;
#if defined(VN_PARAM) //vietnamtest%%%
	if ( sHoldItem.bVietnamGainItem )
	{
		NetMsg.bVietnamItem = sHoldItem.bVietnamGainItem;
	}
#endif

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqHoldToFieldFireCracker ( const D3DXVECTOR3 &vPos )
{
	if ( !VALID_HOLD_ITEM () )					return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;	

	SITEM* pITEM = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pITEM )							return E_FAIL;

	if ( pITEM->sBasicOp.emItemType!=ITEM_FIRECRACKER )		return E_FAIL;
	if ( pITEM->sBasicOp.strTargetEffect.empty() )			return E_FAIL;

	GLMSG::SNETPC_REQ_FIRECRACKER NetMsg;
	NetMsg.vPOS = vPos;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

bool GLCharacter::IsNpcPileItem ( SNATIVEID sNID_NPC, DWORD dwChannel, WORD wPosX, WORD wPosY )
{
	if ( ValidWindowOpen() )					return false;	

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( sNID_NPC );

	//	???? NPC?? ???????? ????????.
	if ( !pCrowData )								return false;

	if ( pCrowData->GetSaleNum() <= dwChannel )		return false;

	GLInventory *pInven = pCrowData->GetSaleInven ( dwChannel );
	if ( !pInven )									return false;

	SINVENITEM* pSaleItem = pInven->FindPosItem ( wPosX, wPosY );
	if ( !pSaleItem )								return false;
		
	SNATIVEID sBUYNID = pSaleItem->sItemCustom.sNativeID;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sBUYNID );

	return ( pItem->ISPILE() != FALSE );
}

bool GLCharacter::IsRestartPossible()
{
	GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
	GLMapList::FIELDMAP_ITER iter = MapsList.find ( GLGaeaClient::GetInstance().GetActiveMap ()->GetMapID ().dwID );
	if ( iter==MapsList.end() )					return FALSE;

	const SMAPNODE *pMapNode = &(*iter).second;

	return !pMapNode->bRestart;
}

void GLCharacter::SendHackingMSG ()
{
	GLMSG::SNET_REQ_SERVERTEST NetMsg;
	NETSENDTOFIELD ( &NetMsg );
}

void GLCharacter::SendHackingMSG1 ()
{
}

// *****************************************************
// Desc: ???????? ??????????/????????
// *****************************************************
HRESULT GLCharacter::ReqNpcTo ( SNATIVEID sNID_NPC, DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wBuyNum )
{
	if ( ValidWindowOpen() )						return E_FAIL;	

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( sNID_NPC );

	if ( !pCrowData )
	{
		//	???? NPC?? ???????? ????????.
		return E_FAIL;
	}

	//	???????? ???? ????.
	if ( VALID_HOLD_ITEM() )
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );

		//	????????
		if ( !pItem->sBasicOp.IsSALE() )
		{
			//	???? ???????? ????.
			return E_FAIL;
		}

		if ( GET_HOLD_ITEM().IsGM_GENITEM() )
		{
			//	???? ???????? ????.
			return E_FAIL;
		}

		// ????????????
		if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD )
		{
			const SITEMCUSTOM& sHoldItem = GET_HOLD_ITEM();
			GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
			if ( pMyPet->IsVALID () && sHoldItem.dwPetID == pMyPet->m_dwPetID )
			{
				return E_FAIL;
			}
		}

		//	???? ??????.
		GLMSG::SNETPC_REQ_SALE_TO_NPC NetMsg;
		NetMsg.sNID = sNID_NPC;
		NetMsg.dwNPCID = m_dwNPCID;
		NETSENDTOFIELD ( &NetMsg );
	}
	//	?????? ???? ????.
	else
	{
		GASSERT(wBuyNum>0&&"?????? ???? ???? ?????? 1?? ?????????? ????.");

		if ( pCrowData->GetSaleNum() <= dwChannel )		return E_FAIL;

		GLInventory *pInven = pCrowData->GetSaleInven ( dwChannel );
		if ( !pInven )									return E_FAIL;

		SINVENITEM* pSaleItem = pInven->FindPosItem ( wPosX, wPosY );
		if ( !pSaleItem )
		{
			//	?????? ???? ???????? ????????.
			return E_FAIL;
		}
		
		SNATIVEID sBUYNID = pSaleItem->sItemCustom.sNativeID;

		SITEM* pItem = GLItemMan::GetInstance().GetItem ( sBUYNID );

		LONGLONG dwPrice = pCrowData->GetNpcSellPrice(pItem->sBasicOp.sNativeID.dwID);
		//add money 2
		//no tax for vot item
		if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
		{
			dwPrice = pItem->sBasicOp.dwBuyPrice;
		}
		//add money 3
		//no tax for premium item
		else if (pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
		{
			dwPrice = pItem->sBasicOp.dwBuyPrice;
		}
		//add contributionpoint by CNDev
		//no tax for contributionpoint item
		else if (pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
		{
			dwPrice = pItem->sBasicOp.dwBuyPrice;
		}
		else
		{	
			if( dwPrice == 0 )
			{
				dwPrice = pItem->sBasicOp.dwBuyPrice;
			}
		}

		//add money 2
		//price check for vote item
		if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) 
		{
			if ( m_lnVoteP < (LONGLONG)dwPrice*wBuyNum )
			{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOMONEY_2") );
			return E_FAIL;
			}
		}
		//add money 3
		//price check for premium item
		else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
		{
			if ( m_lnPremP < (LONGLONG)dwPrice*wBuyNum )
			{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOMONEY_3") );
			return E_FAIL;
			}
		}
		//add contributionpoint by CNDev
		else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
		{
			if ( m_lnContribP < (LONGLONG)dwPrice*wBuyNum )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOMONEY_4") );
				return E_FAIL;
			}
		}
		//price check for gold
		else 
		{
			if ( m_lnMoney < (LONGLONG)dwPrice*wBuyNum )
			{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOMONEY") );
			return E_FAIL;
			}
		}


		

		WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
		WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

		//	Note : ?????? ???? ?????? ?????? ????.
		//
		BOOL bITEM_SPACE(FALSE);
		if ( pItem->ISPILE() )
		{
			//	???? ???????? ????.
			WORD wPILENUM = pItem->sDrugOp.wPileNum;
			WORD wREQINSRTNUM = ( wBuyNum * pItem->GETAPPLYNUM() );
			bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sBUYNID, wPILENUM, wINVENX, wINVENY );
		}
		else
		{
			GASSERT(wBuyNum==1&&"?????? ???????? ???????? 1?????? ???? ??????????.");

			//	???? ???????? ????.
			WORD wInsertPosX(0), wInsertPosY(0);
			bITEM_SPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );
		}

		if ( !bITEM_SPACE )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOSPACE") );
			return E_FAIL;
		}

		//	???? ???? ??????.
		GLMSG::SNETPC_REQ_BUY_FROM_NPC NetMsg;
		NetMsg.sNID = sNID_NPC;
		NetMsg.dwNPCID = m_dwNPCID;
		NetMsg.dwChannel = dwChannel;
		NetMsg.wPosX = pSaleItem->wPosX;
		NetMsg.wPosY = pSaleItem->wPosY;
		NetMsg.wBuyNum = wBuyNum;

		NETSENDTOFIELD ( &NetMsg );
	}

	return S_OK;
}

// *****************************************************
// Desc: ???? ?????? ???? DB???? ????????
// *****************************************************
HRESULT GLCharacter::ReqItemBankInfo ()
{
	GLMSG::SNET_GET_CHARGEDITEM_FROMDB NetMsg;
	NetMsg.dwCharID = m_dwCharID;
	StringCchCopy ( NetMsg.szUID, USR_ID_LENGTH+1, m_szUID );

	NETSENDTOFIELD ( &NetMsg );

	m_cInvenCharged.DeleteItemAll ();
	m_mapChargedKey.clear();

	return S_OK;
}

HRESULT GLCharacter::ReqChargedItemTo ( WORD wPosX, WORD wPosY )
{
	SINVENITEM *pINVENITEM = m_cInvenCharged.GetItem ( wPosX, wPosY );
	if ( !pINVENITEM )						return E_FAIL;

	SNATIVEID nidPOS(wPosX,wPosY);
	MAPSHOP_KEY_ITER iter = m_mapChargedKey.find ( nidPOS.dwID );
	if ( m_mapChargedKey.end()==iter )		return E_FAIL;

	std::string strPurKey = (*iter).second;
	
	//	Note : ?????? ???????? ????.
	//
	GLMSG::SNET_CHARGED_ITEM_GET NetMsg;
	NetMsg.dwID = nidPOS.dwID;
	StringCchCopy ( NetMsg.szPurKey, PURKEY_LENGTH+1, strPurKey.c_str() );
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

void GLCharacter::ReqLevelUp ()
{
	GLMSG::SNETPC_REQ_LEVELUP NetMsg;
	NETSENDTOFIELD ( &NetMsg );
}

void GLCharacter::ReqStatsUp ( SCHARSTATS sStats )
{
	if ( m_wStatsPoint==0 )	return;
	if ( sStats.GetTotal() >  m_wStatsPoint )	return;

	GLMSG::SNETPC_REQ_STATSUP NetMsg;
	NetMsg.sStats = sStats;
	NETSENDTOFIELD ( &NetMsg );
}

//add addstats cmd
void GLCharacter::ReqStatsUpCmd( EMSTATS emStats, DWORD value )
{
	if ( m_wStatsPoint < value){
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, "Adding stats Failed! not enough stat points." );
		return;
	}

	switch( emStats )
	{
		case EMPOW: CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GREENYELLOW, "Adding %d Pow. Success!", value );	break;
		case EMSTR: CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GREENYELLOW, "Adding %d Vit. Success!", value );	break;
		case EMSPI: CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GREENYELLOW, "Adding %d Int. Success!", value );	break;
		case EMDEX: CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GREENYELLOW, "Adding %d Dex. Success!", value );	break;
		case EMSTA: CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GREENYELLOW, "Adding %d Agi. Success!", value );	break;
		
	}

	GLMSG::SNETPC_REQ_STATSUPCMD NetMsg;
	NetMsg.emStats = emStats;
	NetMsg.dwValue = value;
	NETSENDTOFIELD ( &NetMsg );
}
void GLCharacter::ReqDisableSkill( DWORD dwSKILL, SNATIVEID sKILLID )
{
	if ( dwSKILL > SKILLFACT_SIZE ) return;

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sKILLID.wMainID, sKILLID.wSubID );
	if ( !pSkill ) return;
	if ( pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ENERMY ) return;

	GLMSG::SNETPC_REQ_DISABLESKILLEFF NetMsg;
	NetMsg.dwSKILL = dwSKILL;
	NETSENDTOFIELD ( &NetMsg );
}
void GLCharacter::ReqRankName ( BOOL bRankName ) //add pkrank
{
	GLMSG::SNETPC_REQ_RANKNAME NetMsg;
	NetMsg.bRankName = bRankName;
	NETSENDTOFIELD ( &NetMsg );
}
void GLCharacter::ReqRankMark ( BOOL bRankMark ) //add pkrank
{
	GLMSG::SNETPC_REQ_RANKMARK NetMsg;
	NetMsg.bRankMark = bRankMark;
	NETSENDTOFIELD ( &NetMsg );
}

void GLCharacter::ReqAllowBet ( BOOL bAllowBet ) //add duel bet
{
	GLMSG::SNETPC_REQ_ALLOW_BET NetMsg;
	NetMsg.bAllowBet = bAllowBet;
	NETSENDTOFIELD ( &NetMsg );
}

HRESULT GLCharacter::ReqSkillQuickSet ( const SNATIVEID skill_id, const WORD wSLOT )
{
	if ( EMSKILLQUICK_SIZE <= wSLOT )	return E_FAIL;

	//	Note : ???? ?????? ???? ???? ??????.
	if ( !ISLEARNED_SKILL(skill_id) )	return E_FAIL;

	GLMSG::SNETPC_REQ_SKILLQUICK_SET NetMsg;
	NetMsg.wSLOT = wSLOT;
	NetMsg.skill_id = skill_id;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqSkillQuickReSet ( const WORD wSLOT )
{
	if ( EMSKILLQUICK_SIZE <= wSLOT )	return E_FAIL;

	//	Note : ???????????? ?????? ??????.
	//m_sSKILLQUICK[wSLOT] = NATIVEID_NULL();

	GLMSG::SNETPC_REQ_SKILLQUICK_RESET NetMsg;
	NetMsg.wSLOT = wSLOT;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ?????? ?????? ????/????.
HRESULT GLCharacter::ReqItemQuickSet ( const WORD wSLOT )
{
	if ( EMACTIONQUICK_SIZE <= wSLOT )	return E_FAIL;
	if ( !VALID_HOLD_ITEM() )			return S_FALSE;

	const SITEMCUSTOM& sITEM = GET_HOLD_ITEM ();
	
#if defined(VN_PARAM) //vietnamtest%%%
	if ( sITEM.bVietnamGainItem ) return S_FALSE;
#endif

	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sITEM.sNativeID );
	if ( !pITEM )						return S_FALSE;

	if ( pITEM->sBasicOp.emItemType != ITEM_CURE &&
		 pITEM->sBasicOp.emItemType != ITEM_RECALL &&
		 pITEM->sBasicOp.emItemType != ITEM_TELEPORT_CARD /*&& 
		 pITEM->sBasicOp.emItemType != ITEM_PET_CARD*/ )
			// Disabled to avoid crash on Field Server
			// 08-14-2013 11:25 PM
		 return S_FALSE;

	//	Note : ???????????? ?????? ???? ??????.
	//m_sACTIONQUICK[wSLOT].wACT = EMACT_SLOT_DRUG;
	//m_sACTIONQUICK[wSLOT].sNID = sITEM.sNativeID;

	//	Note : ?????? ?????? ?????? ???? ????.
	GLMSG::SNETPC_REQ_ACTIONQUICK_SET NetMsg;
	NetMsg.wSLOT = wSLOT;
	NetMsg.wACT = EMACT_SLOT_DRUG;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqItemQuickReSet ( const WORD wSLOT )
{
	if ( EMACTIONQUICK_SIZE <= wSLOT )	return E_FAIL;

	//m_sACTIONQUICK[wSLOT].RESET ();

	GLMSG::SNETPC_REQ_ACTIONQUICK_RESET NetMsg;
	NetMsg.wSLOT = wSLOT;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ???? ?????? ????/????.
HRESULT GLCharacter::ReqSkillRunSet ( const WORD wSLOT )
{
	if ( EMSKILLQUICK_SIZE <= wSLOT )	return E_FAIL;

	const SNATIVEID &skill_id = m_sSKILLQUICK[wSLOT];
	if ( skill_id==NATIVEID_NULL() )	return E_FAIL;

	//	Note : ???? ?????? ???? ???? ??????.
	if ( !ISLEARNED_SKILL(skill_id) )	return E_FAIL;

	m_sRunSkill = skill_id;

	GLMSG::SNETPC_REQ_SKILLQUICK_ACTIVE NetMsg;
	NetMsg.wSLOT = wSLOT;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqSkillRunReSet ()
{
	m_sRunSkill = NATIVEID_NULL();
	return S_OK;
}

// *****************************************************
// Desc: ?????? ???? ???? ???? ( ???? ?????? )
// *****************************************************
HRESULT GLCharacter::ReqGetStorage ( DWORD dwChannel, DWORD dwNPCID = 0 )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( IsVALID_STORAGE(dwChannel) )			return S_OK;

	//	Note : ?????? ????.
	//
	GLMSG::SNETPC_REQ_GETSTORAGE	NetMsg;
	NetMsg.dwChannel = dwChannel;
	NetMsg.dwNPCID   = m_dwNPCID;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}


// *****************************************************
// Desc: ???? ?????? ????, ??????, ????????, ??????.
// *****************************************************
HRESULT GLCharacter::ReqStorageTo ( DWORD dwChannel, WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )							return E_FAIL;
	if ( !IsVALID_STORAGE(dwChannel) )				return S_OK;
	if ( ValidWindowOpen() )						return E_FAIL;	

	SINVENITEM* pInvenItem = m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !VALID_HOLD_ITEM () && !pInvenItem )		return E_FAIL;


	if ( VALID_HOLD_ITEM() ) 
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
		if ( !pITEM )		return false;
		
		//	????????
		if (GLCONST_CHAR::ENABLE_LOCKER_CONTROL)
		{
			if ( !pITEM->sBasicOp.IsLocker() )		return FALSE;
		}
		else
		{
			if ( !pITEM->sBasicOp.IsEXCHANGE() )		return FALSE;
		}
	}


	bool bKEEP = IsKEEP_STORAGE(dwChannel);

	if ( VALID_HOLD_ITEM () && pInvenItem )
	{
#if defined(VN_PARAM) //vietnamtest%%%
		if ( !GET_HOLD_ITEM().bVietnamGainItem  )
#endif

		{
			if ( !bKEEP )
			{
				//	Note : ???????? ???? ?? ???? ????..
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("STORAGE_SPAN_END") );
				return E_FAIL;
			}

			GLMSG::SNETPC_REQ_STORAGE_EX_HOLD NetMsg;
			NetMsg.dwChannel = dwChannel;
			NetMsg.dwNPCID = m_dwNPCID;
			NetMsg.wPosX = pInvenItem->wPosX;
			NetMsg.wPosY = pInvenItem->wPosY;
			NETSENDTOFIELD ( &NetMsg );
		}
	}
	else if ( pInvenItem )
	{
		GLMSG::SNETPC_REQ_STORAGE_TO_HOLD NetMsg;
		NetMsg.dwChannel = dwChannel;
		NetMsg.dwNPCID = m_dwNPCID;
		NetMsg.wPosX = pInvenItem->wPosX;
		NetMsg.wPosY = pInvenItem->wPosY;
		NETSENDTOFIELD ( &NetMsg );
	}
	else if ( VALID_HOLD_ITEM () )
	{
#if defined(VN_PARAM) //vietnamtest%%%
		if ( !GET_HOLD_ITEM().bVietnamGainItem  )
#endif

		{

			if ( !bKEEP )
			{
				//	Note : ???????? ???? ?? ???? ????..
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("STORAGE_SPAN_END") );
				return E_FAIL;
			}

			//	Note : ?????? ???????? ?????????? ???? ??????.
			//
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
			GASSERT(pItem&&"?????? ???????? ???????? ????");

			BOOL bOk = m_cStorage[dwChannel].IsInsertable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
			if ( !bOk )
			{
				//	?????? ????????.
				return E_FAIL;
			}

			GLMSG::SNETPC_REQ_HOLD_TO_STORAGE NetMsg;
			NetMsg.dwChannel = dwChannel;
			NetMsg.dwNPCID = m_dwNPCID;
			NetMsg.wPosX = wPosX;
			NetMsg.wPosY = wPosY;

			NETSENDTOFIELD ( &NetMsg );
		}
	}

	return S_OK;
}


// *****************************************************
// Desc: ???? ?????? ????. ( ???? ??????????. )
// *****************************************************
HRESULT GLCharacter::ReqStorageSkill ( DWORD dwChannel, WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( !IsVALID_STORAGE(dwChannel) )			return S_OK;
	if ( ValidWindowOpen() )					return E_FAIL;	

	SINVENITEM* pInvenItem = m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_SKILL )	return E_FAIL;

	SNATIVEID sSKILL_ID = pItem->sSkillBookOp.sSkill_ID;

	if ( ISLEARNED_SKILL(sSKILL_ID) )
	{
		//	???? ?????? ????.
		return E_FAIL;
	}

	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
	{
		//	???? ???? ???? ?????? ???????? ????????.
		return E_FAIL;
	}

	//	Note : ???? ?????? ??????????.
	//
	GLMSG::SNETPC_REQ_LEARNSKILL_STORAGE NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.dwNPCID = m_dwNPCID;
	
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

// *****************************************************
// Desc: ???? ?????? ???????? ( ??????, ?????????? ?? ).
// *****************************************************
HRESULT GLCharacter::ReqStorageDrug ( DWORD dwChannel, WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( !IsVALID_STORAGE(dwChannel) )			return S_OK;
	if ( ValidWindowOpen() )					return E_FAIL;	

	SINVENITEM* pInvenItem = m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	if ( CheckCoolTime( pInvenItem->sItemCustom.sNativeID ) ) return E_FAIL;


	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )			return E_FAIL;

	// ???? ???? ????
	if ( !SIMPLE_CHECK_ITEM( pItem->sBasicOp.sNativeID ) ) return S_FALSE;

	if ( pItem->sBasicOp.emItemType == ITEM_CURE )
	{
		//	Note : pk ?????? ?????? ???? ?????? ???? ???????? ?????? ??????.
		//
		DWORD dwPK_LEVEL = GET_PK_LEVEL();
		if ( dwPK_LEVEL != UINT_MAX && dwPK_LEVEL>GLCONST_CHAR::dwPK_DRUG_ENABLE_LEVEL )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DRUG_PK_LEVEL") );
			return E_FAIL;
		}

		if ( m_sCONFTING.IsFIGHTING() && !m_sCONFTING.IsRECOVE() )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("CONFRONT_RECOVE"), 0 );
			return E_FAIL;
		}

		if ( pItem->sDrugOp.emDrug!=ITEM_DRUG_NUNE )
		{
			switch ( pItem->sDrugOp.emDrug )
			{
			case ITEM_DRUG_HP:
				if ( m_sHP.dwMax == m_sHP.dwNow )		return S_FALSE;
				break;

			case ITEM_DRUG_MP:
				if ( m_sMP.dwMax == m_sMP.dwNow )		return S_FALSE;
				break;

			case ITEM_DRUG_SP:
				if ( m_sSP.dwMax == m_sSP.dwNow )		return S_FALSE;
				break;

			case ITEM_DRUG_CP:
				if ( m_sCP.dwMax == m_sCP.dwNow )		return S_FALSE; //add cp
				break;

			case ITEM_DRUG_HP_MP:
				if ( m_sHP.dwMax==m_sHP.dwNow && m_sMP.dwMax==m_sMP.dwNow )		return S_FALSE;
				break;
			case ITEM_DRUG_MP_SP:
				if ( m_sMP.dwMax==m_sMP.dwNow && m_sSP.dwMax==m_sSP.dwNow )		return S_FALSE;
				break;

			case ITEM_DRUG_HP_MP_SP:
				if ( m_sHP.dwMax==m_sHP.dwNow && m_sMP.dwMax==m_sMP.dwNow
					&& m_sSP.dwMax==m_sSP.dwNow )		return S_FALSE;
				break;

			case ITEM_DRUG_CURE:
				if ( !ISSTATEBLOW() )				return S_FALSE;
				break;

			case ITEM_DRUG_HP_CURE:
				if ( m_sHP.dwMax == m_sHP.dwNow && !ISSTATEBLOW() )		return S_FALSE;
				break;

			case ITEM_DRUG_HP_MP_SP_CURE:
				if ( m_sHP.dwMax==m_sHP.dwNow && m_sMP.dwMax==m_sMP.dwNow
					&& m_sSP.dwMax==m_sSP.dwNow && !ISSTATEBLOW() )		return S_FALSE;
				break;
			};

			GLMSG::SNETPC_REQ_STORAGEDRUG NetMsg;
			NetMsg.wPosX = wPosX;
			NetMsg.wPosY = wPosY;
			NetMsg.dwNPCID = m_dwNPCID;

			NETSENDTOFIELD ( &NetMsg );
		}
	}
	else if ( pItem->sBasicOp.emItemType ==ITEM_SKILL )
	{
		ReqStorageSkill ( dwChannel, wPosX, wPosY );
	}
	else if ( pItem->sBasicOp.emItemType ==ITEM_PET_SKILL )
	{
		GLGaeaClient::GetInstance().GetPetClient()->ReqLearnSkill ( dwChannel, wPosX, wPosY );
	}
	
	return S_OK;
}

// *****************************************************
// Desc: ???? ?? ????.
// *****************************************************
HRESULT GLCharacter::ReqStorageSaveMoney ( LONGLONG lnMoney )
{
//#if !defined(KR_PARAM) && !defined(KRT_PARAM)
//	if( m_lnMoney%UINT_MAX < lnMoney )			return E_FAIL;
//#endif
	if ( m_lnMoney < lnMoney )					return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;	

	GLMSG::SNETPC_REQ_STORAGE_SAVE_MONEY NetMsg;
	NetMsg.lnMoney = lnMoney;
	NetMsg.dwNPCID = m_dwNPCID;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

// *****************************************************
// Desc: ???? ?? ??????.
// *****************************************************
HRESULT GLCharacter::ReqStorageDrawMoney ( LONGLONG lnMoney )
{
//#if !defined(KR_PARAM) && !defined(KRT_PARAM)
//	if( m_lnStorageMoney%UINT_MAX < lnMoney )	return E_FAIL;
//#endif
	if ( m_lnStorageMoney < lnMoney )			return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;	

	GLMSG::SNETPC_REQ_STORAGE_DRAW_MONEY NetMsg;
	NetMsg.lnMoney = lnMoney;
	NetMsg.dwNPCID = m_dwNPCID;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ?? ?????? ????.
HRESULT GLCharacter::ReqMoneyToField ( LONGLONG lnMoney )
{
	if ( !GLCONST_CHAR::bMONEY_DROP2FIELD )		return E_FAIL;
	if ( lnMoney == 0 )							return E_FAIL;
//#if !defined(KR_PARAM) && !defined(KRT_PARAM)
//	if ( m_lnMoney%UINT_MAX < lnMoney )			return E_FAIL;
//#endif
	if ( m_lnMoney < lnMoney )					return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;	

	D3DXVECTOR3 vDir(1,0,0);
	BOOL bCollision = FALSE;
	DWORD dwCollisionID;
	D3DXVECTOR3 vCollisionPos(0,0,0);

	srand(GetTickCount());
	float fBaseY = (rand()%32) * (D3DX_PI*2)/32.0f;

	for ( float fRotY = 0.0f; fRotY<D3DX_PI*2; fRotY += 0.2f )
	{
		D3DXMATRIX matRotY;
		D3DXMatrixRotationY ( &matRotY, fRotY+fBaseY );
		D3DXVec3TransformCoord ( &vDir, &vDir, &matRotY );

		D3DXVec3Normalize ( &vDir, &vDir );
		D3DXVECTOR3 vDropPos = GetPosition() + vDir*float(GLCONST_CHAR::wBODYRADIUS+12);

		GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh()->IsCollision
		(
			vDropPos + D3DXVECTOR3(0,+5,0),
			vDropPos + D3DXVECTOR3(0,-5,0),
			vCollisionPos,
			dwCollisionID,
			bCollision
		);
	}

	if ( !bCollision )
	{
		GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh()->IsCollision
		(
			GetPosition() + D3DXVECTOR3(0,+5,0),
			GetPosition() + D3DXVECTOR3(0,-5,0),
			vCollisionPos,
			dwCollisionID,
			bCollision
		);
	}

	if ( bCollision )
	{
		GLMSG::SNETPC_REQ_MONEY_TO_FIELD NetMsg;
		NetMsg.vPos = vCollisionPos;
		NetMsg.lnMoney = lnMoney;
		NETSENDTOFIELD ( &NetMsg );
	}

	return S_OK;
}

//	Note : ?????? ???? ????.
//HRESULT GLCharacter::ReqTradeMoney ( LONGLONG lnMoney )
//{
//	if ( !GLTradeClient::GetInstance().Valid () )	return E_FAIL;
//
//	//	Note : ???? ?????? ???????? ????.
//	//
//	if ( lnMoney > m_lnMoney )		return E_FAIL;
//	
//	//	Note : ?????? ???? ???? MSG.
//	//
//	GLMSG::SNET_TRADE_MONEY NetMsg;
//	NetMsg.lnMoney = lnMoney;
//	NETSENDTOFIELD(&NetMsg);
//
//	return S_OK;
//}

//	Note : ?????? ?????? ????/????/????.
HRESULT GLCharacter::ReqTradeBoxTo ( WORD wPosX, WORD wPosY )
{
	if ( !GLTradeClient::GetInstance().Valid () )	return E_FAIL;

	SINVENITEM* pTradeItem = GLTradeClient::GetInstance().FindPosItem ( wPosX, wPosY );	//	???? ?????? ???????? ?????? ???????? ????.
	SINVEN_POS sPreTradeItem = GLTradeClient::GetInstance().GetPreItem();					//	???? ???? ???????? ????.

	//	Note : ???? ?????? ???? ?????? ???????? ???? ?????????? ???? ???????? ???? ????.
	//
	if ( sPreTradeItem.VALID() && pTradeItem )
	{
		//	?????????? ???? ??????.
		SINVENITEM* pResistItem = m_cInventory.GetItem ( sPreTradeItem.wPosX, sPreTradeItem.wPosY );
		if ( !pResistItem )		return E_FAIL;

		//	???? ???????? ?????? ????.
		SINVENITEM* pAlready = GLTradeClient::GetInstance().FindItemBack ( pResistItem->wBackX, pResistItem->wBackY );
		if ( pAlready )
		{
			GLTradeClient::GetInstance().ReSetPreItem();
			return E_FAIL;
		}

		SITEM *pItem = GLItemMan::GetInstance().GetItem ( pResistItem->sItemCustom.sNativeID );
		if ( !pItem )			return E_FAIL;
		//
		//	????????
		if ( !pItem->sBasicOp.IsEXCHANGE() ) return E_FAIL;


		if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD )
		{
			// ???????? ???????? ?????????? ???? ?????? ???? ??????
			GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
			if ( pMyPet->IsVALID () && pResistItem->sItemCustom.dwPetID == pMyPet->m_dwPetID )
			{
				return E_FAIL;
			}
		}

		if ( pItem->sBasicOp.emItemType == ITEM_VEHICLE && pResistItem->sItemCustom.dwVehicleID != 0 )
		{
			return E_FAIL;
		}

		//	Note : ???? ???????? ?????? ???????? ???????? ?????? ???? ?? ?? ?????? ????.
		//
		BOOL bOk = GLTradeClient::GetInstance().IsExInsertable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )				return FALSE;

		//	Note : ???? ???????? ?????????? ????.
		//
		GLMSG::SNET_TRADE_ITEM_REMOVE NetMsgReMove;
		NetMsgReMove.wPosX = pTradeItem->wPosX;
		NetMsgReMove.wPosY = pTradeItem->wPosY;
		NETSENDTOFIELD(&NetMsgReMove);

		//	Note : ?????? ???????? ???????? ????.
		GLMSG::SNET_TRADE_ITEM_REGIST NetMsgRegist;
		NetMsgRegist.wPosX = wPosX;
		NetMsgRegist.wPosY = wPosY;
		NetMsgRegist.wInvenX = sPreTradeItem.wPosX;
		NetMsgRegist.wInvenY = sPreTradeItem.wPosY;
		NETSENDTOFIELD(&NetMsgRegist);

		GLTradeClient::GetInstance().ReSetPreItem();
	}
	//	Note : ?????? ???????? ???? ????.
	//
	else if ( sPreTradeItem.VALID() )
	{
		//	?????????? ???? ??????.
		SINVENITEM* pResistItem = m_cInventory.GetItem ( sPreTradeItem.wPosX, sPreTradeItem.wPosY );
		if ( !pResistItem )		return E_FAIL;

		//	???? ???????? ?????? ????.
		SINVENITEM* pAlready = GLTradeClient::GetInstance().FindItemBack ( sPreTradeItem.wPosX, sPreTradeItem.wPosY );
		if ( pAlready )
		{
			GLTradeClient::GetInstance().ReSetPreItem();
			return E_FAIL;
		}

		//	Note : ?????????? ???? ?? ?? ?????? ????.
		//
		SITEM *pItem = GLItemMan::GetInstance().GetItem ( pResistItem->sItemCustom.sNativeID );
		if ( !pItem )			return E_FAIL;

		//	????????
		if ( !pItem->sBasicOp.IsEXCHANGE() ) return E_FAIL;

		if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD )
		{
			// ???????? ???????? ?????????? ???? ?????? ???? ??????
			const SITEMCUSTOM& sHold = GET_HOLD_ITEM();
			GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
			if ( pMyPet->IsVALID () && pResistItem->sItemCustom.dwPetID == pMyPet->m_dwPetID )
			{
				return E_FAIL;
			}
		}

		if ( pItem->sBasicOp.emItemType == ITEM_VEHICLE && pResistItem->sItemCustom.dwVehicleID != 0 )
		{
			return E_FAIL;
		}

		BOOL bOk = GLTradeClient::GetInstance().IsInsertable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )				return E_FAIL;

		//	Note : ?????? ???????? ???????? ????.
		GLMSG::SNET_TRADE_ITEM_REGIST NetMsgRegist;
		NetMsgRegist.wPosX = wPosX;
		NetMsgRegist.wPosY = wPosY;
		NetMsgRegist.wInvenX = sPreTradeItem.wPosX;
		NetMsgRegist.wInvenY = sPreTradeItem.wPosY;
		NETSENDTOFIELD(&NetMsgRegist);

		GLTradeClient::GetInstance().ReSetPreItem();
	}
	//	Note : ?????????? ?????? ???????? ????????.
	//
	else if ( pTradeItem )
	{
		GLMSG::SNET_TRADE_ITEM_REMOVE NetMsgReMove;
		NetMsgReMove.wPosX = pTradeItem->wPosX;
		NetMsgReMove.wPosY = pTradeItem->wPosY;
		NETSENDTOFIELD(&NetMsgReMove);
	}
	
	return S_OK;
}

//	Note : ???? ????.
HRESULT GLCharacter::ReqTradeAgree ()
{
	if ( !GLTradeClient::GetInstance().Valid () )	return E_FAIL;

	if ( !GLTradeClient::GetInstance().IsAgreeAble() )
	{
		//	Note : ?????? ???? ?????? ?? ?? ????????. X`s ???? ???? ???? ???? ????????.
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TRADE_AGREE_TIME") );
		return E_FAIL;
	}

	//	Note : ?????? ?????? ?????????? ?????? ?????? ?????? ????.
	//
	if ( !IsVaildTradeInvenSpace () )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TRADE_AGREE_FAIL_MYINVEN") );
		return E_FAIL;
	}

	GLMSG::SNET_TRADE_AGREE NetMsg;
	NETSENDTOFIELD(&NetMsg);
	
	return S_OK;
}

//	Note : ???? ????.
HRESULT GLCharacter::ReqTradeCancel ()
{
	if ( !GLTradeClient::GetInstance().Valid () )	return E_FAIL;
	
	GLMSG::SNET_TRADE_CANCEL NetMsg;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqReGenGate ( DWORD dwNpcID )
{
	GLMSG::SNETPC_REQ_REGEN_GATE NetMsg;

	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	PGLCROWCLIENT pCrow = pLandMan->GetCrow ( dwNpcID );
	if ( !pCrow )										goto _REQ_FAIL;
	if ( pCrow->GETCROW() != CROW_NPC )					goto _REQ_FAIL;

	//	Note : ???????? ???? ????.
	//
	NetMsg.dwNpcID = dwNpcID;
	NETSEND(&NetMsg);

	return S_OK;

_REQ_FAIL:
	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREGEN_GATE_FAIL") );
	return E_FAIL;
}

// *****************************************************
// Desc: ????????
// *****************************************************
void GLCharacter::ReqReBirth ()
{
	if ( !IsACTION(GLAT_DIE) )			return;

	//	?????????? ??????.
	m_dwWAIT = 0;
	SetSTATE(EM_ACT_DIE);

	//	???? ????.
	GLMSG::SNETPC_REQ_REBIRTH NetMsg;
	NetMsg.bRegenEntryFailed = FALSE;

	//  ???? ???????? ???? ?????? ???? ?????? ??????????.
	if ( m_dwUserLvl < USER_GM3 )	
	{
		SMAPNODE *pNODE = GLGaeaClient::GetInstance().FindMapNode(m_sStartMapID);
		if ( pNODE )
		{
			EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
			GLLevelFile cLevelFile;
			BOOL bOk = cLevelFile.LoadFile ( pNODE->strFile.c_str(), TRUE, NULL );
			if( bOk )
			{
				SLEVEL_REQUIRE* pRequire = cLevelFile.GetLevelRequire ();
				emReqFail = pRequire->ISCOMPLETE ( this );

				if ( emReqFail != EMREQUIRE_COMPLETE )
				{
					m_sStartMapID = GLCONST_CHAR::nidSTARTMAP[m_wSchool];
					m_dwStartGate = GLCONST_CHAR::dwSTARTGATE[m_wSchool];
					m_vStartPos   = D3DXVECTOR3(0.0f,0.0f,0.0f);
					NetMsg.bRegenEntryFailed = TRUE;
				}
			}
		}
	}


	NETSEND ( &NetMsg );

	// ???????? ?????????? ???????? ???????? ????
	if ( GLCONST_CHAR::bBATTLEROYAL )
	{
		ReSetSTATE(EM_REQ_VISIBLEOFF);

		//	Note : (????????????) ?????? ????.
		//
		GLMSG::SNETPC_ACTSTATE NetMsg;
		NetMsg.dwActState = m_dwActState;
		NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
	}
}

// *****************************************************
// Desc: ???????? (?????? ????)
// *****************************************************
HRESULT GLCharacter::ReqReGenRevive ()
{
	if ( !IsACTION(GLAT_DIE) )			return E_FAIL;
	if ( !ISREVIVE () )					return E_FAIL;

	//	?????????? ??????.
	m_dwWAIT = 0;
	SetSTATE(EM_ACT_DIE);

	// ?????? ???? ????
	GLMSG::SNETPC_REQ_REVIVE NetMsg;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

// ?????? ?????? ????????.
HRESULT GLCharacter::ReqRecoveryExp ()
{
// ??????????_????_Normal
#if defined( _RELEASED ) || defined ( KRT_PARAM ) || defined ( KR_PARAM ) 
	if ( !IsACTION(GLAT_DIE) )			return E_FAIL;

	//	?????????? ??????.
	m_dwWAIT = 0;
	SetSTATE(EM_ACT_DIE);

	// ?????????? ???? ????
	GLMSG::SNETPC_REQ_RECOVERY NetMsg;
	NETSENDTOFIELD(&NetMsg);
#endif
	
	return S_OK;
}

// ?????? ?????? ???????? Npc
HRESULT GLCharacter::ReqRecoveryExpNpc ( DWORD dwNpcID )
{
// ??????????_????_Npc
#if defined( _RELEASED ) || defined ( KRT_PARAM ) || defined ( KR_PARAM ) || defined ( TH_PARAM ) || defined ( MYE_PARAM ) || defined ( MY_PARAM ) || defined ( PH_PARAM ) || defined ( CH_PARAM ) || defined ( TW_PARAM ) || defined ( HK_PARAM ) || defined ( GS_PARAM )
	GLMSG::SNETPC_REQ_RECOVERY_NPC NetMsg;
	NetMsg.dwNPCID = dwNpcID;
	NETSENDTOFIELD(&NetMsg);
#endif
	return S_OK;
}


//  Note : ?????? ???????? ??????.
HRESULT GLCharacter::ReqGetReExp ()
{
// ??????????_????_Normal
#if defined( _RELEASED ) || defined ( KRT_PARAM ) || defined ( KR_PARAM )

	if ( !IsACTION(GLAT_DIE) )			return E_FAIL;	

	// ?????? ???????? ????
	GLMSG::SNETPC_REQ_GETEXP_RECOVERY NetMsg;
	NETSENDTOFIELD(&NetMsg);
#endif

	return S_OK;
}

HRESULT GLCharacter::ReqGetReExpNpc ( DWORD dwNpcID )
{
// ??????????_????_Npc
#if defined( _RELEASED ) || defined ( KRT_PARAM ) || defined ( KR_PARAM ) || defined ( TH_PARAM ) || defined ( MYE_PARAM ) || defined ( MY_PARAM ) || defined ( PH_PARAM ) || defined ( CH_PARAM ) || defined ( TW_PARAM ) || defined ( HK_PARAM ) || defined ( GS_PARAM )
	// ?????? ???????? ????
	GLMSG::SNETPC_REQ_GETEXP_RECOVERY_NPC NetMsg;
	NetMsg.dwNPCID = dwNpcID;
	NETSENDTOFIELD(&NetMsg);
#endif

	return S_OK;
}


HRESULT GLCharacter::ReqCure ( DWORD dwNpcID, DWORD dwGlobalID )
{
	GLMSG::SNETPC_REQ_CURE NetMsg;

	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	PGLCROWCLIENT pCrow = pLandMan->GetCrow ( dwNpcID );

	if ( !pCrow )										goto _REQ_FAIL;
	if ( pCrow->GETCROW() != CROW_NPC )					goto _REQ_FAIL;

	//	Note : ???? ????.
	NetMsg.dwNpcID = dwNpcID;
	NetMsg.dwGlobalID = dwGlobalID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;

_REQ_FAIL:
	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREGEN_CURE_FAIL") );

	return S_OK;
}

//	Note : ?????? ???? ????. ( stats, skill )
//
HRESULT GLCharacter::ReqCharReset ( DWORD dwNpcID )
{
	EMREGEN_CHARRESET_FB emFB = EMREGEN_CHARRESET_FAIL;
	WORD wPosX(0), wPosY(0);
	GLMSG::SNETPC_REQ_CHARRESET NetMsg;

	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	PGLCROWCLIENT pCrow = pLandMan->GetCrow ( dwNpcID );

	if ( !pCrow )										goto _REQ_FAIL;
	if ( pCrow->GETCROW() != CROW_NPC )					goto _REQ_FAIL;


	bool bITEM = m_cInventory.GetCharResetItem ( wPosX, wPosY );
	if ( !bITEM )
	{
		emFB = EMREGEN_CHARRESET_ITEM_FAIL;
		goto _REQ_FAIL;
	}

	//	Note : ?????? ???? ????.
	NetMsg.dwNpcID = dwNpcID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;

_REQ_FAIL:
	
	switch ( emFB )
	{
	case EMREGEN_CHARRESET_FAIL:
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREGEN_CHARRESET_FAIL") );
		break;

	case EMREGEN_CHARRESET_ITEM_FAIL:
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREGEN_CHARRESET_ITEM_FAIL") );
		break;
	};

	return S_OK;
}

//	Note : NPC?? item?? ???? A:npc???? ????????, b:npc?????? ??????.
HRESULT GLCharacter::ReqItemTrade ( DWORD dwNpcID, DWORD dwGlobalID, DWORD *pDwA_NID, DWORD dwB_NID )
{
	EMNPC_ITEM_TRADE_FB emFB = EMNPC_ITEM_TRADE_FAIL;
	WORD wPosX(0), wPosY(0);
	GLMSG::SNETPC_REQ_NPC_ITEM_TRADE NetMsg;

	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	PGLCROWCLIENT pCrow = pLandMan->GetCrow ( dwNpcID );

	if ( !pCrow )										goto _REQ_FAIL;
	if ( pCrow->GETCROW() != CROW_NPC )					goto _REQ_FAIL;

	// Need Add New Item Con.
	BOOL bOK[] = { FALSE, FALSE, FALSE, FALSE, FALSE };
	bOK[0] = ISHAVEITEM ( SNATIVEID(pDwA_NID[0]) );
	BYTE i;
	// MAX_NEEDITEM_COUNT 5
	for( i = 1; i < 5; i++ )
	{
		if( pDwA_NID[i] == UINT_MAX )
			bOK[i] = TRUE;
		else
			bOK[i] = ISHAVEITEM ( SNATIVEID(pDwA_NID[i]) );	
	}

	if( bOK[0] == FALSE || bOK[1] == FALSE || bOK[2] == FALSE || bOK[3] == FALSE || bOK[4] == FALSE )
	{
		emFB = EMNPC_ITEM_TRADE_ITEM_FAIL;
		goto _REQ_FAIL;
	}

	//	Note : ?????? ???? ????.
	//
	NetMsg.dwNpcID    = dwNpcID;
	// MAX_NEEDITEM_COUNT 5
	for( i = 0; i < 5; i++ )
	{
		NetMsg.dwA_NID[i]    = pDwA_NID[i];
	}
	NetMsg.dwB_NID    = dwB_NID;
	NetMsg.dwGlobalID = dwGlobalID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;

_REQ_FAIL:
	switch ( emFB )
	{
	case EMNPC_ITEM_TRADE_FAIL:
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_TRADE_FAIL") );
		break;

	case EMNPC_ITEM_TRADE_ITEM_FAIL:
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_TRADE_ITEM_FAIL") );
		break;
	};

	return S_OK;
}

// *****************************************************
// Desc: ???????? ????(????????????)
// *****************************************************
HRESULT	GLCharacter::Req2Friend ( const char *szNAME )
{
	SFRIEND* pFRIEND = GLFriendClient::GetInstance().GetFriend ( szNAME );
	if ( !pFRIEND )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM2FRIEND_FB_FAIL") );
		return S_FALSE;
	}

	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLAND->m_bTowerWars )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, "Disable" );
		return E_FAIL;
	}
	SINVENITEM *pINVENITEM = m_cInventory.FindItem ( ITEM_2FRIEND );
	if ( !pINVENITEM )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM2FRIEND_FB_NO_ITEM") );
		return S_FALSE;
	}

	//	Note : ???????? ???? ????.
	GLMSG::SNETPC_2_FRIEND_REQ NetMsg;
	StringCchCopy ( NetMsg.szFRIEND_NAME, CHAR_SZNAME, pFRIEND->szCharName );
	NetMsg.wItemPosX = pINVENITEM->wPosX;
	NetMsg.wItemPosY = pINVENITEM->wPosY;
	NETSEND ( &NetMsg);

	return S_OK;
}

bool GLCharacter::IsInvenSplitItem ( WORD wPosX, WORD wPosY, bool bVietnamInven )
{
	SINVENITEM* pInvenItem = NULL;
	if( bVietnamInven )
	{
		pInvenItem = m_cVietnamInventory.FindPosItem ( wPosX, wPosY );
	}else{
		pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	}
	if ( !pInvenItem )	return false;

	//	Note : ?????? ???? ????????.
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	GASSERT(pItem&&"?????? ???????? ???????? ????");

	bool bSPLIT(false);
	bSPLIT = ( pItem->ISINSTANCE() );
	if ( !bSPLIT )					return FALSE;
	bSPLIT = ( pInvenItem->sItemCustom.wTurnNum>1 );

	return bSPLIT;
}

bool GLCharacter::IsStorageSplitItem ( DWORD dwChannel, WORD wPosX, WORD wPosY )
{
	GASSERT(EMSTORAGE_CHANNEL>dwChannel);
	if ( EMSTORAGE_CHANNEL<=dwChannel )	return false;

	SINVENITEM* pInvenItem = m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return false;

	//	Note : ?????? ???? ????????.
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	GASSERT(pItem&&"?????? ???????? ???????? ????");

	bool bSPLIT(false);
	bSPLIT = ( pItem->ISPILE() );
	if ( !bSPLIT )					return FALSE;
	bSPLIT = ( pInvenItem->sItemCustom.wTurnNum>1 );

	return bSPLIT;
}

//	Note : ???????? - ???? ?????? ????.
HRESULT GLCharacter::ReqInvenSplit ( WORD wPosX, WORD wPosY, WORD wSplitNum )
{
	if ( !IsInvenSplitItem(wPosX,wPosY) )	return E_FAIL;
	
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )									return E_FAIL;

	if ( pInvenItem->sItemCustom.wTurnNum <= wSplitNum )
	{
		return E_FAIL;
	}

	//	Note : ?????? ???? ?????? ????.
	GLMSG::SNETPC_REQ_INVEN_SPLIT NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.wSplit = wSplitNum;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

// *****************************************************
// Desc: ???? - ???? ?????? ????.
// *****************************************************
HRESULT GLCharacter::ReqStorageSplit ( DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wSplitNum )
{
	if ( !IsStorageSplitItem(dwChannel,wPosX,wPosY) )	return E_FAIL;

	SINVENITEM* pInvenItem = m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return false;
	
	if ( pInvenItem->sItemCustom.wTurnNum <= wSplitNum )
	{
		return E_FAIL;
	}

	//	Note : ?????? ???? ?????? ????.
	GLMSG::SNETPC_REQ_STORAGE_SPLIT NetMsg;
	NetMsg.dwChannel = dwChannel;
	NetMsg.dwNPCID = m_dwNPCID;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.wSplit = wSplitNum;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

//	Note : ???? ????.
//
HRESULT GLCharacter::ReqConflict ( DWORD dwID, const SCONFT_OPTION &sOption )
{
	if ( !IsValidBody() )					return E_FAIL;

	PGLCHARCLIENT pChar = GLGaeaClient::GetInstance().GetChar ( dwID );
	if ( !pChar )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_FAIL") );
		return E_FAIL;
	}

	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	if ( pLAND->m_bTowerWars )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, "Disable" );
		return E_FAIL;
	}
	if ( GLGaeaClient::GetInstance().GetActiveMap()->IsPeaceZone() )
	{
		//	Note : ???? ?????????? ?????? ????????????.
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_PEACE") );
		return E_FAIL;
	}
	
	if ( !sOption.VALID_OPT() )	
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_OPTION") );
		return E_FAIL;
	}

	if ( m_sCONFTING.IsCONFRONTING() )
	{
		//	Note : ???? ???????????? ???? ?????? ????????????.
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_ALREADY_ME") );
		return E_FAIL;
	}

	GLPARTY_CLIENT* pParty = GLPartyClient::GetInstance().FindMember ( dwID );
	if ( pParty )
	{
		//	Note : ???? ?????? ???????????? ???? ????????????.
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_PARTY") );
		return E_FAIL;
	}

	//	Note : ???? ?????? ????.
	if ( IsClubMaster() && pChar->IsClubMaster() )
	{
		GLMSG::SNETPC_REQ_CONFRONT NetMsg;
		NetMsg.emTYPE = EMCONFT_GUILD;
		NetMsg.dwID = dwID;
		NetMsg.sOption = sOption;
		NETSEND(&NetMsg);
	}
	//	Note : ???? ?????? ????.
	else 
	if ( IsPartyMaster() && pChar->IsPartyMaster() )
	{
		GLMSG::SNETPC_REQ_CONFRONT NetMsg;
		NetMsg.emTYPE = EMCONFT_PARTY;
		NetMsg.dwID = dwID;
		NetMsg.sOption = sOption;
		NETSEND(&NetMsg);
	}
	//	Note : ???? ?????? ????.
	else
	{
		if ( IsPartyMem() )
		{
			//	Note : ???????? '????????'?? ?????? ?? ????????.
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_MENOTMASTER") );
			return E_FAIL;
		}

		GLMSG::SNETPC_REQ_CONFRONT NetMsg;
		NetMsg.emTYPE = EMCONFT_ONE;
		NetMsg.dwID = dwID;
		NetMsg.sOption = sOption;
		NETSEND(&NetMsg);
	}

	return S_OK;
}

void GLCharacter::ReqSkillReaction ( STARGETID sTID )
{
	if ( IsACTION(GLAT_SKILL) || IsACTION(GLAT_ATTACK) )	return;

	BOOL bMove(FALSE);
	D3DXVECTOR3 vMoveTo;

	m_sActiveSkill = m_sRunSkill;

	PGLSKILL pRunSkill = GLSkillMan::GetInstance().GetData ( m_sActiveSkill );
	if ( !pRunSkill )										return;
	if ( pRunSkill->m_sBASIC.emIMPACT_SIDE == SIDE_ENERMY )	return;

	SetDefenseSkill( false );

	GLCOPY* pCOPY = GLGaeaClient::GetInstance().GetCopyActor(sTID);
	if ( !pCOPY )	return;

	sTID.vPos = pCOPY->GetPosition();
	SkillReaction ( sTID, DXKEY_UP, false, bMove, vMoveTo );

	//	Note : Reaction ???? ?????? ?????? ????.
	//
	if ( bMove )
	{
		ActionMoveTo ( 0.0f, vMoveTo+D3DXVECTOR3(0,+5,0), vMoveTo+D3DXVECTOR3(0,-5,0), FALSE, TRUE );
	}
}

HRESULT GLCharacter::ReqQuestStart ( DWORD dwNpcID, DWORD dwTalkID, DWORD dwQUESTID )
{
	//	Note : ?????? ?????? ???? ????.
	//
	GLQUEST* pQUEST = GLQuestMan::GetInstance().Find ( dwQUESTID );
	if ( !pQUEST )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMQUEST_START_FB_FAIL") );
		return S_FALSE;
	}

	//	???? ???????? ?????? ???? ????.
	GLQUESTPROG* pQUEST_PROG = m_cQuestPlay.FindProc ( dwQUESTID );
	if ( pQUEST_PROG )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMQUEST_START_FB_ALREADY") );
		return S_FALSE;
	}

	//	???? ?????? ?????????? ????.
	GLQUESTPROG* pQUEST_END = m_cQuestPlay.FindEnd ( dwQUESTID );
	if ( pQUEST_END )
	{
		if ( !pQUEST_END->m_bCOMPLETE )
		{
			//	????(????)?? ???????? ???? ???? ???????? ????.
			if ( !pQUEST->IsAGAIN() )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMQUEST_START_FB_DONAGAIN") );
				return S_FALSE;
			}
		}
		else
		{
			//	?????? ???? ???????? ????.
			if ( !pQUEST->IsREPEAT() )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMQUEST_START_FB_DONREPEAT") );
				return S_FALSE;
			}
		}
	}

	if ( m_lnMoney < pQUEST->m_dwBeginMoney )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMQUEST_START_FB_NEEDMONEY") );
		return S_FALSE;
	}

	//pQUEST->m_dwBeginPartyMemNum;

	//	Note : quest ???? ?????? ????.
	//
	GLMSG::SNETPC_REQ_QUEST_START NetMsg;
	NetMsg.dwNpcID = dwNpcID;
	NetMsg.dwTalkID = dwTalkID;
	NetMsg.dwQuestID = dwQUESTID;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqQuestStepNpcTalk ( DWORD dwNpcID, DWORD dwTalkID, DWORD dwQUESTID, DWORD dwQUESTSTEP )
{
	//	Note : ?????? ?????? ???? ????.
	//
	GLQUEST* pQUEST = GLQuestMan::GetInstance().Find ( dwQUESTID );
	if ( !pQUEST )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMQUEST_START_FB_FAIL") );
		return S_FALSE;
	}

	//	???????? ?????? ???? ????.
	GLQUESTPROG* pQUEST_PROG = m_cQuestPlay.FindProc ( dwQUESTID );
	if ( !pQUEST_PROG )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMQUEST_START_FB_ALREADY") );
		return S_FALSE;
	}

	//	Note : quest ???? ?????? ????.
	//
	GLMSG::SNET_QUEST_STEP_NPC_TALK NetMsg;
	NetMsg.dwNpcID = dwNpcID;
	NetMsg.dwTalkID = dwTalkID;
	NetMsg.dwQUESTID = dwQUESTID;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqQuestGiveUp ( DWORD dwQUESTID )
{
	//	Note : ?????? ?????? ???? ????.
	//
	GLQUEST* pQUEST = GLQuestMan::GetInstance().Find ( dwQUESTID );
	if ( !pQUEST )			return S_FALSE;

	//	???????? ?????? ???? ????.
	GLQUESTPROG* pQUEST_PROG = m_cQuestPlay.FindProc ( dwQUESTID );
	if ( !pQUEST_PROG )		return S_FALSE;

	if ( !pQUEST->IsGIVEUP() )		return S_FALSE;

	//	Note : quest ???? ?????? ????.
	//
	GLMSG::SNET_QUEST_PROG_GIVEUP NetMsg;
	NetMsg.dwQUESTID = dwQUESTID;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqQuestREADINGReset ( DWORD dwQUESTID )
{
	//	Note : ?????? ?????? ???? ????.
	//
	GLQUEST* pQUEST = GLQuestMan::GetInstance().Find ( dwQUESTID );
	if ( !pQUEST )			return S_FALSE;

	//	???????? ?????? ???? ????.
	GLQUESTPROG* pQUEST_PROG = m_cQuestPlay.FindProc ( dwQUESTID );
	if ( !pQUEST_PROG )		return S_FALSE;

	if ( !pQUEST_PROG->IsReqREADING() )		return S_FALSE;
	pQUEST_PROG->ResetReqREADING ();

	//	Note : quest ???????? ?????? ????.
	//
	GLMSG::SNETPC_QUEST_PROG_READ NetMsg;
	NetMsg.dwQID = dwQUESTID;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqQuestComplete ( DWORD dwQUESTID )
{
	//	Note : ?????? ?????? ???? ????.
	//
	GLQUEST* pQUEST = GLQuestMan::GetInstance().Find ( dwQUESTID );
	if ( !pQUEST )			return S_FALSE;

	//	???????? ?????? ???? ????.
	GLQUESTPROG* pQUEST_PROG = m_cQuestPlay.FindProc ( dwQUESTID );
	if ( !pQUEST_PROG )		return S_FALSE;

	if ( !pQUEST_PROG->CheckCOMPLETE() )		return S_FALSE;

	//	Note : quest ???? ????.
	//
	GLMSG::SNETPC_REQ_QUEST_COMPLETE NetMsg;
	NetMsg.dwQID = dwQUESTID;

	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqBusStation ( DWORD dwNpcID, DWORD dwSTATION )
{
	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	PGLCROWCLIENT pCrow = pLandMan->GetCrow ( dwNpcID );

	if ( !pCrow )										return S_OK;
	if ( pCrow->GETCROW() != CROW_NPC )					return S_OK;

	//	Note : ?????? id?? ???????? ????.
	//
	SSTATION* pSTATION = GLBusStation::GetInstance().GetStation ( dwSTATION );
	if ( !pSTATION )									return S_OK;

	SMAPNODE *pNODE = GLGaeaClient::GetInstance().FindMapNode(SNATIVEID(pSTATION->dwMAPID));
	if ( !pNODE )										return S_OK;
	
	//	Note : pk ?????? ?????? ???? ?????? ???? ???? ?????? ??????.
	//
	//DWORD dwPK_LEVEL = GET_PK_LEVEL();
	//if ( dwPK_LEVEL != UINT_MAX && dwPK_LEVEL>GLCONST_CHAR::dwPK_RECALL_ENABLE_LEVEL )
	//{
	//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMBUS_TAKE_PK_LEVEL") );
	//	return E_FAIL;
	//}

	EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);

	GLLevelFile cLevelFile;
	BOOL bOk = cLevelFile.LoadFile ( pNODE->strFile.c_str(), TRUE, NULL );
	if ( !bOk )											return S_OK;

	SLEVEL_REQUIRE* pRequire = cLevelFile.GetLevelRequire ();
	emReqFail = pRequire->ISCOMPLETE ( this );
	if ( emReqFail != EMREQUIRE_COMPLETE )
	{
		CInnerInterface &cINTERFACE = CInnerInterface::GetInstance();
		switch ( emReqFail )
		{
		case EMREQUIRE_LEVEL:
			{
				if( pRequire->m_signLevel == EMSIGN_FROMTO )
				{
					cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
						ID2GAMEINTEXT("EMREQUIRE_LEVEL2"),
						pRequire->m_wLevel,
						pRequire->m_wLevel2 );
				}else{
					std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signLevel].c_str());

					if( RANPARAM::emSERVICE_TYPE == EMSERVICE_THAILAND )
					{
						cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
							ID2GAMEINTEXT("EMREQUIRE_LEVEL"),
							strSIGN.c_str(),
							pRequire->m_wLevel );
					}
					else
					{
						cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
							ID2GAMEINTEXT("EMREQUIRE_LEVEL"),
							pRequire->m_wLevel,
							strSIGN.c_str() );
					}
				}
			}
			break;

		case EMREQUIRE_ITEM:
			{
				SITEM *pItem = GLItemMan::GetInstance().GetItem ( pRequire->m_sItemID );
				if ( pItem )
				{
					cINTERFACE.PrintMsgTextDlg
					(
						NS_UITEXTCOLOR::DISABLE,
						ID2GAMEINTEXT("EMREQUIRE_ITEM"),
						pItem->GetName()
					);
				}
			}
			break;

		case EMREQUIRE_SKILL:
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pRequire->m_sSkillID );
				if ( pSkill )
				{
					cINTERFACE.PrintMsgTextDlg
					(
						NS_UITEXTCOLOR::DISABLE,
						ID2GAMEINTEXT("EMREQUIRE_SKILL"),
						pSkill->GetName()
					);
				}
			}
			break;

		case EMREQUIRE_LIVING:
			{
				std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signLiving].c_str());
				cINTERFACE.PrintMsgTextDlg
				(
					NS_UITEXTCOLOR::DISABLE,
					ID2GAMEINTEXT("EMREQUIRE_LIVING"),
					pRequire->m_nLiving,
					strSIGN.c_str()
				);
			}
			break;

		case EMREQUIRE_BRIGHT:
			{
				std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signBright].c_str());
				cINTERFACE.PrintMsgTextDlg
				(
					NS_UITEXTCOLOR::DISABLE,
					ID2GAMEINTEXT("EMREQUIRE_BRIGHT"),
					pRequire->m_nBright,
					strSIGN.c_str()
				);
			}
			break;

		case EMREQUIRE_QUEST_COM:
			{
				CString strQUEST = "quest";
				GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pRequire->m_sComQuestID.dwID );
				if ( pQUEST )		strQUEST = pQUEST->GetTITLE();

				cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQUIRE_QUEST_COM"), strQUEST.GetString() );
			}
			break;

		case EMREQUIRE_QUEST_ACT:
			{
				CString strQUEST = "quest";
				GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pRequire->m_sComQuestID.dwID );
				if ( pQUEST )		strQUEST = pQUEST->GetTITLE();

				cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQUIRE_QUEST_ACT"), strQUEST.GetString() );
			}
			break;

		default:
			cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("MAP_REQUIRE_FAIL") );
			break;
		};

		//	Note : ???? ???? ?????? ???? ????. GM level ?????? ???? ???? ????.
		//
		if ( m_dwUserLvl < USER_GM3 )	return S_OK;
	}

	//	Note : ???? ?????? ?????? ????.
	//
	SINVENITEM* pITEM_TICKET = m_cInventory.FindItem ( ITEM_TICKET );
	if ( !pITEM_TICKET )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMBUS_TAKE_TICKET") );
		return S_OK;
	}

	//	Note : ???? ????.
	//
	GLMSG::SNETPC_REQ_BUS NetMsg;
	NetMsg.wPosX = pITEM_TICKET->wPosX;
	NetMsg.wPosY = pITEM_TICKET->wPosY;
	NetMsg.dwNPC_ID = dwNpcID;
	NetMsg.dwSTATION_ID = dwSTATION;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

	//	Note : ???? ???? ????.
HRESULT GLCharacter::ReqTaxiStation ( WORD wPosX, WORD wPosY, int nSelectMap, int nSelectStop )
{
	//	Note : ?????? id?? ???????? ????.
	//
	STAXI_MAP* pTaxiMap = GLTaxiStation::GetInstance().GetTaxiMap ( nSelectMap );
	if ( !pTaxiMap )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_MAPFAIL") );
		return S_OK;
	}

	STAXI_STATION* pStation = pTaxiMap->GetStation( nSelectStop );
	if ( !pStation )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_STATIONFAIL") );
		return S_OK;
	}

	SMAPNODE *pNODE = GLGaeaClient::GetInstance().FindMapNode(SNATIVEID(pStation->dwMAPID));
	if ( !pNODE )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_MAPFAIL") );
		return S_OK;
	}

	EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);

	GLLevelFile cLevelFile;
	BOOL bOk = cLevelFile.LoadFile ( pNODE->strFile.c_str(), TRUE, NULL );
	if ( !bOk )											return S_OK;

	SLEVEL_REQUIRE* pRequire = cLevelFile.GetLevelRequire ();
	emReqFail = pRequire->ISCOMPLETE ( this );
	if ( emReqFail != EMREQUIRE_COMPLETE )
	{
		CInnerInterface &cINTERFACE = CInnerInterface::GetInstance();
		switch ( emReqFail )
		{
		case EMREQUIRE_LEVEL:
			{
				if( pRequire->m_signLevel == EMSIGN_FROMTO )
				{
					cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
						ID2GAMEINTEXT("EMREQUIRE_LEVEL2"),
						pRequire->m_wLevel,
						pRequire->m_wLevel2 );
				}else{
					std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signLevel].c_str());

					if( RANPARAM::emSERVICE_TYPE == EMSERVICE_THAILAND )
					{
						cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
							ID2GAMEINTEXT("EMREQUIRE_LEVEL"),
							strSIGN.c_str(),
							pRequire->m_wLevel );
					}
					else
					{
						cINTERFACE.PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, 
							ID2GAMEINTEXT("EMREQUIRE_LEVEL"),
							pRequire->m_wLevel,
							strSIGN.c_str() );
					}
				}
			}
			break;

		case EMREQUIRE_ITEM:
			{
				SITEM *pItem = GLItemMan::GetInstance().GetItem ( pRequire->m_sItemID );
				if ( pItem )
				{
					cINTERFACE.PrintMsgTextDlg
					(
						NS_UITEXTCOLOR::DISABLE,
						ID2GAMEINTEXT("EMREQUIRE_ITEM"),
						pItem->GetName()
					);
				}
			}
			break;

		case EMREQUIRE_SKILL:
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pRequire->m_sSkillID );
				if ( pSkill )
				{
					cINTERFACE.PrintMsgTextDlg
					(
						NS_UITEXTCOLOR::DISABLE,
						ID2GAMEINTEXT("EMREQUIRE_SKILL"),
						pSkill->GetName()
					);
				}
			}
			break;

		case EMREQUIRE_LIVING:
			{
				std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signLiving].c_str());
				cINTERFACE.PrintMsgTextDlg
				(
					NS_UITEXTCOLOR::DISABLE,
					ID2GAMEINTEXT("EMREQUIRE_LIVING"),
					pRequire->m_nLiving,
					strSIGN.c_str()
				);
			}
			break;

		case EMREQUIRE_BRIGHT:
			{
				std::string strSIGN = ID2GAMEINTEXT(COMMENT::CDT_SIGN_ID[pRequire->m_signBright].c_str());
				cINTERFACE.PrintMsgTextDlg
				(
					NS_UITEXTCOLOR::DISABLE,
					ID2GAMEINTEXT("EMREQUIRE_BRIGHT"),
					pRequire->m_nBright,
					strSIGN.c_str()
				);
			}
			break;

		case EMREQUIRE_QUEST_COM:
			{
				CString strQUEST = "quest";
				GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pRequire->m_sComQuestID.dwID );
				if ( pQUEST )		strQUEST = pQUEST->GetTITLE();

				cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQUIRE_QUEST_COM"), strQUEST.GetString() );
			}
			break;

		case EMREQUIRE_QUEST_ACT:
			{
				CString strQUEST = "quest";
				GLQUEST *pQUEST = GLQuestMan::GetInstance().Find ( pRequire->m_sComQuestID.dwID );
				if ( pQUEST )		strQUEST = pQUEST->GetTITLE();

				cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQUIRE_QUEST_ACT"), strQUEST.GetString() );
			}
			break;

		default:
			cINTERFACE.PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("MAP_REQUIRE_FAIL") );
			break;
		};

		//	Note : ???? ???? ?????? ???? ????. GM level ?????? ???? ???? ????.
		//
		if ( m_dwUserLvl < USER_GM3 )	return S_OK;
	}

	//	Note : ???? ?????? ?????? ????.
	//
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType != ITEM_TAXI_CARD )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_TICKET") );
		return E_FAIL;
	}

	LONGLONG lnCharge = GetCalcTaxiCharge( nSelectMap, nSelectStop );

	if ( m_lnMoney < lnCharge )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_MONEY") );
		return S_FALSE;
	}

	//	Note : ???? ????.
	//
	GLMSG::SNETPC_REQ_TAXI NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.dwSelectMap = nSelectMap;
	NetMsg.dwSelectStop = nSelectStop;
	NetMsg.dwGaeaID = m_dwGaeaID;
	NETSEND ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqLoudSpeaker ( const char* szChat, SITEMLINK* pItemLink )
{
	WORD wPosX(0), wPosY(0);

	if ( m_dwUserLvl < USER_GM3 )
	{
		SINVENITEM* pINVENITEM = m_cInventory.FindItem ( ITEM_LOUDSPEAKER );
		if ( !pINVENITEM )
		{
			//	Note : ?????? ???????? ???????? ????????.
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAT_LOUDSPEAKER_NOITEM") );
			return S_OK;
		}

		if ( m_sPMarket.IsOpen() ) 
		{
			DWORD wTurn = m_cInventory.CountTurnItem( pINVENITEM->sItemCustom.sNativeID );
			DWORD wMarketTurn = m_sPMarket.GetItemTurnNum( pINVENITEM->sItemCustom.sNativeID ) ;

			if ( wTurn <= wMarketTurn ) 
			{
				//	Note : ?????? ???????? ???????? ????????.
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAT_LOUDSPEAKER_NOITEM") );
				return S_OK;
			}
		}

		wPosX = pINVENITEM->wPosX;
		wPosY = pINVENITEM->wPosY;
	}

	//	Note : ?????? ???? ????.
	//
	GLMSG::SNETPC_CHAT_LOUDSPEAKER NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	if ( pItemLink )
		NetMsg.sITEMLINK = *pItemLink;
	StringCchCopy ( NetMsg.szMSG, CHAT_MSG_SIZE+1, szChat );
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqPMarketTitle ( const char* szTitle )
{
	if ( m_sPMarket.IsOpen() )	return S_FALSE;

	//	Note : ???????? ??????.
	m_sPMarket.DoMarketClose();

	//	Note : ?????? ???? ????.
	//
	GLMSG::SNETPC_PMARKET_TITLE NetMsg;
	StringCchCopy ( NetMsg.szPMarketTitle, CHAT_MSG_SIZE+1, szTitle );
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqPMarketRegItem ( WORD wPosX, WORD wPosY, LONGLONG dwMoney, DWORD dwNum )
{
	if ( m_sPMarket.IsOpen() )	return S_FALSE;

	SINVENITEM *pINVENITEM = m_cInventory.GetItem ( wPosX, wPosY );
	if ( !pINVENITEM )			return S_FALSE;

	//	Note : ?????? ?? ???? ?????? ???????? ????????.
	if ( m_sPMarket.GetItemNum() >= GLPrivateMarket::EMMAX_SALE_NUM )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_MAXNUM") );
		return S_FALSE;
	}

	SNATIVEID nidITEM = pINVENITEM->sItemCustom.sNativeID;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( nidITEM );
	if ( !pITEM )				return S_FALSE;

	//	????????
	if ( !pITEM->sBasicOp.IsEXCHANGE() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( 
			NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_NOSALE") );
		return S_FALSE;
	}

	
	// ???????????? ???? ?????? ???? ?????? ?????? ?????? ?? ????.
	if ( pITEM->sBasicOp.emItemType == ITEM_PET_CARD )
	{
		// ???????? ???????? ?????????? ???? ?????? ???? ??????
		GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
		if ( pMyPet->IsVALID () && pINVENITEM->sItemCustom.dwPetID == pMyPet->m_dwPetID )
		{
			return E_FAIL;
		}
	}

	if ( pITEM->sBasicOp.emItemType == ITEM_VEHICLE && pINVENITEM->sItemCustom.dwVehicleID != 0 )
	{
		return E_FAIL;
	}

	//	Note : ???? ?????? ?????????? ????.
	bool bREGPOS = m_sPMarket.IsRegInvenPos ( SNATIVEID(wPosX,wPosY) );
	if ( bREGPOS )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_REGITEM") );
		return S_FALSE;
	}

	//	Note : ???? ?????? ???? ?????? ?????? ???????? ???? ???????? ?????? ????.
	if ( pITEM->ISPILE() )
	{
		bool bREG = m_sPMarket.IsRegItem ( nidITEM );
		if ( bREG )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_REGITEM") );
			return S_FALSE;
		}

		//	Note : ???????? ???? ???? ???? ???? ????.
		DWORD dwTURN = m_cInventory.CountTurnItem ( nidITEM );
		if ( dwNum >= dwTURN )
		{
			dwNum = dwTURN;
		}
	}

	if ( !pITEM->ISPILE() )
	{
		dwNum = 1;
	}
	
	SNATIVEID sSALEPOS;
	bool bPOS = m_sPMarket.FindInsertPos ( nidITEM, sSALEPOS );
	if ( !bPOS )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_MAXNUM") );
		return S_FALSE;
	}

	GLMSG::SNETPC_PMARKET_REGITEM NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.dwMoney = dwMoney;
	NetMsg.dwNum = dwNum;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqPMarketDisItem ( WORD wPosX, WORD wPosY )
{
	if ( m_sPMarket.IsOpen() )	return S_FALSE;

	SNATIVEID sSALEPOS(wPosX,wPosY);
	const SSALEITEM* pSALEITEM = m_sPMarket.GetItem ( sSALEPOS );
	if ( !pSALEITEM )		return S_FALSE;

	GLMSG::SNETPC_PMARKET_DISITEM NetMsg;
	NetMsg.sSALEPOS = sSALEPOS;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqPMarketOpen ()
{
	if ( m_sPMarket.IsOpen() )		return S_FALSE;

	//	Note : ?????????? ?????? ???????? 
	if ( m_sPMarket.GetItemNum() == 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_OPEN_FB_EMPTY") );
		return S_FALSE;
	}

	if ( m_wPMPosX==USHRT_MAX )		return S_FALSE;
	if ( m_wPMPosY==USHRT_MAX )		return S_FALSE;

	//	Note : ???????? ???? ????.
	GLMSG::SNETPC_PMARKET_OPEN NetMsg;
	NetMsg.wPosX = m_wPMPosX;
	NetMsg.wPosY = m_wPMPosY;
	NETSENDTOFIELD ( &NetMsg );

	m_wPMPosX = (USHRT_MAX);
	m_wPMPosY = (USHRT_MAX);

	return S_OK;
}

HRESULT GLCharacter::ReqPMarketClose ()
{
	if ( !m_sPMarket.IsOpen() )		return S_FALSE;

	m_sPMarket.DoMarketClose();

	GLMSG::SNETPC_PMARKET_CLOSE NetMsg;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqPMarketInfo ( DWORD dwGaeaID )
{
	PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar ( dwGaeaID );
	if ( !pCLIENT )					return S_FALSE;

	bool bOPEN = pCLIENT->m_sPMarket.IsOpen();
	if ( !bOPEN )
	{
		return S_FALSE;
	}

	//	Note : ???? ???? ?????? ???? ?????? ????.
	pCLIENT->m_sPMarket.DoMarketInfoRelease();

	//	Note : ???? ???? ????.
	GLMSG::SNETPC_PMARKET_ITEM_INFO NetMsg;
	NetMsg.dwGaeaID = dwGaeaID;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqPMarketInfoRelease ( DWORD dwGaeaID )
{
	PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar ( dwGaeaID );
	if ( !pCLIENT )					return S_FALSE;

	pCLIENT->m_sPMarket.DoMarketInfoRelease();

	// ???????? ?????????? ?????? ???? ?????? ?????? ?????? ??????????.
	m_mapPETCardInfoTemp.clear();
	m_mapVEHICLEItemInfoTemp.clear();

	return S_OK;
}

HRESULT GLCharacter::ReqPMarketBuy ( DWORD dwGaeaID, WORD wPosX, WORD wPosY, DWORD dwNum )
{
	PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar ( dwGaeaID );
	if ( !pCLIENT )					return S_FALSE;

	SNATIVEID sSALEPOS(wPosX,wPosY);

	const SSALEITEM* pSALEITEM = pCLIENT->m_sPMarket.GetItem ( sSALEPOS );
	if ( !pSALEITEM )				return S_FALSE;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pSALEITEM->sITEMCUSTOM.sNativeID );
	if ( !pITEM )					return S_FALSE;

	if ( pSALEITEM->bSOLD )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_SOLD") );
		return S_FALSE;
	}

	if ( !pITEM->ISPILE() )
	{
		dwNum = 1;
	}

	if ( pSALEITEM->dwNUMBER < dwNum )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_NUM") );
		return S_FALSE;
	}

	if ( m_lnMoney < (dwNum*pSALEITEM->llPRICE) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_LOWMONEY") );
		return S_FALSE;
	}

	WORD wINVENX = pITEM->sBasicOp.wInvenSizeX;
	WORD wINVENY = pITEM->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);
	if ( pITEM->ISPILE() )
	{
		WORD wPILENUM = pITEM->sDrugOp.wPileNum;
		SNATIVEID sNID = pITEM->sBasicOp.sNativeID;
		WORD wREQINSRTNUM = (WORD) dwNum;

		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, wINVENX, wINVENY );
	}
	else
	{
		WORD wPosX(0), wPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wPosX, wPosY );
	}

	if ( !bITEM_SPACE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_NOINVEN") );
		return E_FAIL;
	}

	GLMSG::SNETPC_PMARKET_BUY NetMsg;
	NetMsg.dwGaeaID = dwGaeaID;
	NetMsg.dwNum = dwNum;
	NetMsg.sSALEPOS = sSALEPOS;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqClubNew ( DWORD dwNpcID, const char* szClubName )
{
	if  ( szClubName==NULL )	return S_FALSE;

	if ( m_dwGuild!=CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_ALREADY") );
		return S_FALSE;
	}

	if ( !GLPartyClient::GetInstance().GetMaster() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( GLPartyClient::GetInstance().GetMemberNum() < GLCONST_CHAR::dwCLUB_PARTYNUM )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_NOTMEMBER"), GLCONST_CHAR::dwCLUB_PARTYNUM );
		return S_FALSE;
	}

	if ( !szClubName || (strlen(szClubName) == 0) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_FAIL") );
		return S_FALSE;
	}

	if ( m_wLevel < GLCONST_CHAR::sCLUBRANK[0].m_dwMasterLvl )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_MASTER_LVL"), GLCONST_CHAR::sCLUBRANK[0].m_dwMasterLvl );
		return S_FALSE;
	}

	if ( m_nLiving < int ( GLCONST_CHAR::sCLUBRANK[0].m_dwLivingPoint ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_AVER_LIVING"), GLCONST_CHAR::sCLUBRANK[0].m_dwLivingPoint );
		return S_FALSE;
	}

	if ( m_lnMoney < GLCONST_CHAR::sCLUBRANK[0].m_dwPay )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_NOTMONEY"), GLCONST_CHAR::sCLUBRANK[0].m_dwPay );
		return S_FALSE;
	}


	CString strTEMP( szClubName ); 

#ifdef TH_PARAM
	// ?????? ???? ???? ????

	if ( !m_pCheckString ) return S_FALSE;

	if ( !m_pCheckString(strTEMP) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_THAICHAR_ERROR"));
		return S_FALSE;
	}
#endif

#ifdef VN_PARAM
	// ?????? ???? ???? ???? 
	if( STRUTIL::CheckVietnamString( strTEMP ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_VNCHAR_ERROR"));
		return S_FALSE;
	}

#endif 

	BOOL bFILTER0 = STRUTIL::CheckString( strTEMP );;
	BOOL bFILTER1 = CRanFilter::GetInstance().NameFilter ( szClubName );
	if( bFILTER0 || bFILTER1 || ( strTEMP != szClubName ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NEW_FB_BADNAME"), szClubName );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_NEW NetMsg;
	NetMsg.dwNpcID = dwNpcID;
	StringCchCopy ( NetMsg.szClubName, CHAR_SZNAME, szClubName );
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqClubRank ( DWORD dwNpcID )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( m_sCLUB.m_dwRank >= (GLCONST_CHAR::dwMAX_CLUBRANK-1) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_RANK_FB_MAX") );
		return S_FALSE;
	}

	DWORD dwRANK = m_sCLUB.m_dwRank + 1;

	if ( m_wLevel < GLCONST_CHAR::sCLUBRANK[dwRANK].m_dwMasterLvl )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_RANK_FB_MASTER_LVL"), GLCONST_CHAR::sCLUBRANK[dwRANK].m_dwMasterLvl );
		return S_FALSE;
	}

	if ( m_nLiving < int ( GLCONST_CHAR::sCLUBRANK[dwRANK].m_dwLivingPoint ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_RANK_FB_AVER_LIVING"), GLCONST_CHAR::sCLUBRANK[dwRANK].m_dwLivingPoint );
		return S_FALSE;
	}

	if ( m_lnMoney < GLCONST_CHAR::sCLUBRANK[dwRANK].m_dwPay )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_RANK_FB_NOTMONEY"), GLCONST_CHAR::sCLUBRANK[dwRANK].m_dwPay );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_RANK NetMsg;
	NetMsg.dwNpcID = dwNpcID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqClubDissolution ()
{
	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( m_sCLUB.IsAlliance() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_DIS_FB_ALLIANCE") );
		return S_FALSE;
	}

	if ( m_sCLUB.GetAllBattleNum() > 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_DIS_FB_CLUBBATTLE") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_DISSOLUTION NetMsg;
	NETSEND(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqClubDissolutionCancel ()
{
	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_DISSOLUTION NetMsg;
	NetMsg.bCANCEL = true;
	NETSEND(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqClubJoin ( DWORD dwGaeaID )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMemberFlgJoin(m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( m_sCLUB.GetAllBattleNum() > 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_MEMBER_REQ_FB_CLUBBATTLE") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_MEMBER_REQ NetMsg;
	NetMsg.dwGaeaID = dwGaeaID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqClubJoinAns ( DWORD dwMaster, bool bOK )
{
	GLMSG::SNET_CLUB_MEMBER_REQ_ANS NetMsg;
	NetMsg.dwMaster = dwMaster;
	NetMsg.bOK = bOK;
	NETSENDTOFIELD(&NetMsg);
	
	return S_OK;
}

HRESULT GLCharacter::ReqClubMemberDel ( DWORD dwMember )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMemberFlgKick(m_dwCharID) )
	{
		return S_FALSE;
	}

	if ( m_sCLUB.m_dwMasterID==dwMember )
	{
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_MEMBER_DEL NetMsg;
	NetMsg.dwMember = dwMember;
	NETSEND(&NetMsg);
	
	return S_OK;
}

	//	Note : ???? ?????? ???? ????
HRESULT GLCharacter::ReqClubAuthority ( DWORD dwMember )
{
	if ( m_dwGuild==CLUB_NULL )	return S_FALSE;
	if ( !m_sCLUB.IsMaster( m_dwCharID ) )	return S_FALSE;
	if ( m_sCLUB.m_dwMasterID==dwMember )	return S_FALSE;

	//	?????? ????
	GLCLUBMEMBER* pMember = m_sCLUB.GetMember( dwMember );
	if ( !pMember )	return S_FALSE;

	//	???? ???? ????
	if ( !pMember->bONLINE ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_AUTHORITY_REQ_FB_NOONLINE") );
		return S_FALSE;
	}

	//	???? ????
	if ( m_sCLUB.IsAlliance() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_AUTHORITY_REQ_FB_ALLIANCE") );
		return S_FALSE;
	}

	//	???????? ????
	if ( m_sCLUB.GetAllBattleNum() > 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_AUTHORITY_REQ_FB_CLUBBATTLE") );
		return S_FALSE;
	}

    //	???? ????
	if ( m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_AUTHORITY_REQ_FB_CONFING") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_AUTHORITY_REQ NetMsg;
	NetMsg.dwMember = dwMember;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ?????? ???? ???? ????
HRESULT GLCharacter::ReqClubAuthorityAns ( bool bOK )
{
	GLMSG::SNET_CLUB_AUTHORITY_REQ_ANS NetMsgAns;
	NetMsgAns.bOK = bOK;
	NETSEND(&NetMsgAns);

	return S_OK;
}

HRESULT GLCharacter::ReqClubMarkInfo ( DWORD dwClubID, DWORD dwMarkVer )
{
	if ( dwMarkVer==0 )					return S_FALSE;

	//	Note : ?????????? ?????? ????.
	//
	DWORD dwServerID = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
	bool bVALID = DxClubMan::GetInstance().IsValidData ( dwServerID, dwClubID, dwMarkVer );
	if ( bVALID )						return S_FALSE;

	//	Note : ???? ???? ???? ????.
	GLMSG::SNET_CLUB_MARK_INFO NetMsgInfo;
	NetMsgInfo.dwClubID = dwClubID;
	NETSEND(&NetMsgInfo);

	return S_OK;
}

HRESULT GLCharacter::ReqClubMarkChange ( const char* szFileName )
{
	if ( !szFileName )	return E_FAIL;

	// Note : ???????? ????????.
	LPDWORD pMarkColor(NULL);
	BOOL bOK = DxClubMan::GetInstance().LoadClubMark ( szFileName, pMarkColor );
	if ( !bOK )
	{
		CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_LOADMARK_FAIL"), szFileName );
		return E_FAIL;
	}

	//	Note : ???? ???? ???? ????.
	GLMSG::SNET_CLUB_MARK_CHANGE NetMsgChange;
	memcpy ( NetMsgChange.aryMark, pMarkColor, sizeof(DWORD)*EMCLUB_MARK_SX*EMCLUB_MARK_SY);
	NETSEND(&NetMsgChange);

	return S_OK;
}

HRESULT GLCharacter::ReqClubNick ( const char* szNickName )
{
	if ( !szNickName )					return S_FALSE;
	//if ( strlen(szNickName)==0 )		return S_FALSE;


	CString strTEMP = szNickName;

#ifdef TH_PARAM
	// ?????? ???? ???? ????

	if ( !m_pCheckString ) return S_FALSE;

	if ( !m_pCheckString(strTEMP) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_MEMBER_NICK_FB_THAICHAR_ERROR"));
		return S_FALSE;
	}
#endif

#ifdef VN_PARAM
	// ?????? ???? ???? ???? 
	if( STRUTIL::CheckVietnamString( strTEMP ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_MEMBER_NICK_FB_VNCHAR_ERROR"));
		return S_FALSE;
	}

#endif 
	
	BOOL bFILTER0 = STRUTIL::CheckString( strTEMP );
	BOOL bFILTER1 = CRanFilter::GetInstance().NameFilter ( szNickName );
	if ( bFILTER0 || bFILTER1 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_MEMBER_NICK_FB_BADNAME") );
		return S_FALSE;
	}

	//	Note : ???? ???? ????.
	GLMSG::SNET_CLUB_MEMBER_NICK NetMsg;
	StringCchCopy ( NetMsg.szNick, CHAR_SZNAME, szNickName );
	NETSEND(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqClubSecede ()
{
	if ( m_dwGuild==CLUB_NULL )
	{
		return S_FALSE;
	}

	//	Note : ???????? ?????? ???? ????.
	if ( m_sCLUB.IsMaster(m_dwCharID) )
	{
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_MEMBER_SECEDE NetMsg;
	NETSEND(&NetMsg);
	
	return S_OK;
}
HRESULT GLCharacter::ReqDetectAP ( BOOL bDetect, DWORD dwCharID ) //add autopots
{

	GLMSG::SNET_AP_DT NetMsg;
	NetMsg.bDetect = bDetect;
	NetMsg.dwCharID = dwCharID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}	
HRESULT GLCharacter::ReqCDCertify (DWORD dwNpcID )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCDCERTIFY_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMemberFlgCDCertify(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCDCERTIFY_NOTMASTER") );
		return S_FALSE;
	}

	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLand )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCDCERTIFY_FAIL") );
		return S_FALSE;
	}

	PGLCROWCLIENT pCROW = pLand->GetCrow(dwNpcID);
	if ( !pCROW )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCDCERTIFY_FAIL") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_CD_CERTIFY NetMsg;
	NetMsg.dwNpcID = dwNpcID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqGuidCommission ( DWORD dwNPCID, float fRATE )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMGUIDCOMMISSION_FB_NOTCLUB") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMGUIDCOMMISSION_FB_NOTCLUB") );
		return S_FALSE;
	}

	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLand )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMGUIDCOMMISSION_FB_FAIL") );
		return S_FALSE;
	}

	PGLCROWCLIENT pCROW = pLand->GetCrow(dwNPCID);
	if ( !pCROW )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMGUIDCOMMISSION_FB_FAIL") );
		return S_FALSE;
	}

	if ( fRATE < 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMGUIDCOMMISSION_FB_RANGE") );
		return S_FALSE;
	}

	if ( fRATE > GLCONST_CHAR::fMAX_COMMISSION )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMGUIDCOMMISSION_FB_RANGE") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_GUID_COMMISSION NetMsg;
	NetMsg.fCommission = fRATE;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ????.
HRESULT GLCharacter::ReqClubNotice ( const char* szClubNotice )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NOTICE_FB_FAIL") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMemberFlgNotice(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_NOTICE_FB_NOTMATER") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_NOTICE_REQ NetMsg;
	StringCchCopy ( NetMsg.szNotice, EMCLUB_NOTICE_LEN+1, szClubNotice );
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ????.
HRESULT GLCharacter::ReqClubSubMaster ( DWORD dwCharID, DWORD dwClubFlag )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUBSUBMASTER_FB_FAIL") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUBSUBMASTER_FB_NOTMASTER") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_SUBMASTER NetMsg;
	NetMsg.dwCharID = dwCharID;
	NetMsg.dwFlags = dwClubFlag;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ????.
HRESULT GLCharacter::ReqClubAlliance ( DWORD dwGaeaID )
{
	PGLCHARCLIENT pCHAR_REQ = GLGaeaClient::GetInstance().GetChar ( dwGaeaID );
	if ( !pCHAR_REQ )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_REQ_FB_FAIL") );
		return S_FALSE;
	}

	DWORD dwCharID = pCHAR_REQ->GetCharData().dwCharID;

	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_REQ_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_REQ_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !pCHAR_REQ->IsClubMaster() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_REQ_FB_TARNOTMASTER") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsAlliance() && !m_sCLUB.IsChief() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_REQ_FB_NOTCHIEF") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsRegDissolution() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_REQ_FB_DISSOLUTION") );
		return S_FALSE;
	}	

	if ( m_sCLUB.GetAllBattleNum() > 0  )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_REQ_FB_CLUBBATTLE") );
		return S_FALSE;
	}	
	
	GLMSG::SNET_CLUB_ALLIANCE_REQ NetMsg;
	NetMsg.dwCharID = dwCharID;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqClubAllianceAns ( DWORD dwChiefCharID, bool bOK )
{
	GLMSG::SNET_CLUB_ALLIANCE_REQ_ANS NetMsgAns;
	NetMsgAns.dwChiefCharID = dwChiefCharID;
	NetMsgAns.bOK = bOK;
	NETSEND(&NetMsgAns);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqClubAllianceSec ()
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_SEC_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_SEC_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsAlliance() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_SEC_FB_ALLIANCE") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsChief() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_SEC_FB_FAIL") );
		return S_FALSE;
	}

	if ( m_sCLUB.GetAllBattleNum() > 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_SEC_FB_BATTLE") );
		return S_FALSE;		
	}

	GLMSG::SNET_CLUB_ALLIANCE_SEC_REQ NetMsg;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqClubAllianceDel ( DWORD dwClubID )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DEL_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DEL_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsAlliance() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DEL_FB_NOTCHIEF") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsChief() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DEL_FB_NOTCHIEF") );
		return S_FALSE;
	}

	if ( m_sCLUB.GetAllBattleNum() > 0 ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DEL_FB_BATTLE") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_ALLIANCE_DEL_REQ NetMsg;
	NetMsg.dwDelClubID = dwClubID;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqClubAllianceDis ()
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DIS_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DIS_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsAlliance() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DIS_FB_NOTCHIEF") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsChief() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DIS_FB_NOTCHIEF") );
		return S_FALSE;
	}

	if ( m_sCLUB.GetAllBattleNum() > 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_ALLIANCE_DIS_FB_BATTLE") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_ALLIANCE_DIS_REQ NetMsg;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ????.
HRESULT GLCharacter::ReqClubBattle ( DWORD dwGaeaID, DWORD dwTime )
{

	if ( !GLCONST_CHAR::bCLUB_BATTLE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_FAIL") );
		return S_FALSE;
	}

	PGLCHARCLIENT pCHAR_REQ = GLGaeaClient::GetInstance().GetChar ( dwGaeaID );
	if ( !pCHAR_REQ )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_FAIL") );
		return S_FALSE;
	}

	if ( m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_CONFT") );
		return S_FALSE;		
	}

	DWORD dwCharID = pCHAR_REQ->GetCharData().dwCharID;

	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !pCHAR_REQ->IsClubMaster() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_TARNOTMASTER") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsAllianceGuild( pCHAR_REQ->GETCLUBID() ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_ALLIANCE") );
			
		return S_FALSE;
	}

	if ( m_sCLUB.IsRegDissolution() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_DISSOLUTION") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsBattle( pCHAR_REQ->GETCLUBID() ) || m_sCLUB.IsBattleReady( pCHAR_REQ->GETCLUBID() ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_ALREADY") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsBattleAlliance( pCHAR_REQ->GETALLIANCEID() )  || m_sCLUB.IsBattleReady( pCHAR_REQ->GETALLIANCEID() )  )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_ALREADY2") );
		return S_FALSE;
	}

	if ( dwTime < GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE
					, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_TIMEMIN")
					, GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN / 60
					, GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN % 60);
		return S_FALSE;
	}

	if ( dwTime > GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE
					, ID2GAMEINTEXT("EMCLUB_BATTLE_REQ_FB_TIMEMAX")
					, GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX / 60
					, GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX % 60);
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_BATTLE_REQ NetMsg;
	NetMsg.dwCharID = dwCharID;
	NetMsg.dwBattleTime = dwTime;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ????.
HRESULT GLCharacter::ReqAllianceBattle ( DWORD dwGaeaID, DWORD dwTime )
{

	DWORD dwCharID;
	PGLCHARCLIENT pCHAR_REQ;

	if ( !GLCONST_CHAR::bCLUB_BATTLE || !GLCONST_CHAR::bCLUB_BATTLE_ALLIANCE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_FAIL") );
		return S_FALSE;
	}

	pCHAR_REQ = GLGaeaClient::GetInstance().GetChar ( dwGaeaID );
	if ( !pCHAR_REQ )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_FAIL") );
		return S_FALSE;
	}

	if ( m_sCONFTING.emTYPE != EMCONFT_NONE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_CONFT") );
		return S_FALSE;		
	}

	dwCharID = pCHAR_REQ->GetCharData().dwCharID;

	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !pCHAR_REQ->IsClubMaster() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_TARNOTMASTER") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsAllianceGuild( pCHAR_REQ->GETCLUBID() ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_ALLIANCE") );
			
		return S_FALSE;
	}

	if ( !pCHAR_REQ->IsAllianceMaster() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_TARNOTMASTER") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsRegDissolution() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_DISSOLUTION") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsBattle( pCHAR_REQ->GETCLUBID() ) || m_sCLUB.IsBattleReady( pCHAR_REQ->GETCLUBID() ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_ALREADY") );
		return S_FALSE;
	}

	if ( m_sCLUB.IsBattleAlliance( pCHAR_REQ->GETALLIANCEID() ) || m_sCLUB.IsBattleReady( pCHAR_REQ->GETALLIANCEID() ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_ALREADY2") );
		return S_FALSE;
	}

	if ( dwTime < GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE
					, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_TIMEMIN")
					, GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN / 60
					, GLCONST_CHAR::dwCLUB_BATTLE_TIMEMIN % 60);
		return S_FALSE;
	}

	if ( dwTime > GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE
					, ID2GAMEINTEXT("EMALLIANCE_BATTLE_REQ_FB_TIMEMAX")
					, GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX / 60
					, GLCONST_CHAR::dwCLUB_BATTLE_TIMEMAX % 60);
		return S_FALSE;
	}

	GLMSG::SNET_ALLIANCE_BATTLE_REQ NetMsg;
	NetMsg.dwCharID = dwCharID;
	NetMsg.dwBattleTime = dwTime;
	NETSEND(&NetMsg);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqClubBattleAns ( DWORD dwChiefCharID, bool bOK )
{

	GLMSG::SNET_CLUB_BATTLE_REQ_ANS NetMsgAns;
	NetMsgAns.dwClubCharID = dwChiefCharID;
	NetMsgAns.bOK = bOK;
	NETSEND(&NetMsgAns);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqAllianceBattleAns ( DWORD dwChiefCharID, bool bOK )
{

	GLMSG::SNET_ALLIANCE_BATTLE_REQ_ANS NetMsgAns;
	NetMsgAns.dwClubCharID = dwChiefCharID;
	NetMsgAns.bOK = bOK;
	NETSEND(&NetMsgAns);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqClubBattleArmistice( DWORD dwCLUBID )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_ARMISTICE_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_ARMISTICE_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsBattle( dwCLUBID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_ARMISTICE_FB_NOBATTLE") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsBattleStop( dwCLUBID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_ARMISTICE_FB_DISTIME") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ NetMsg;
	NetMsg.dwClubID = dwCLUBID;
	NETSEND(&NetMsg);	

	return S_OK;
}

HRESULT GLCharacter::ReqAllianceBattleArmistice( DWORD dwCLUBID )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_ARMISTICE_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_ARMISTICE_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsAlliance() || !m_sCLUB.IsChief() ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_ARMISTICE_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsBattleAlliance( dwCLUBID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_ALLIANCE_ARMISTICE_FB_NOBATTLE") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsBattleStop( dwCLUBID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_ARMISTICE_FB_DISTIME") );
		return S_FALSE;
	}

	GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ NetMsg;
	NetMsg.dwClubID = dwCLUBID;
	NETSEND(&NetMsg);	

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqClubBattleArmisticeAns( DWORD dwCLUBID, bool bOK )
{	
	GLMSG::SNET_CLUB_BATTLE_ARMISTICE_REQ_ANS NetMsgAns;
	NetMsgAns.dwClubID = dwCLUBID;
	NetMsgAns.bOK = bOK;
	NETSEND(&NetMsgAns);

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqAllianceBattleArmisticeAns( DWORD dwCLUBID, bool bOK )
{	
	GLMSG::SNET_ALLIANCE_BATTLE_ARMISTICE_REQ_ANS NetMsgAns;
	NetMsgAns.dwClubID = dwCLUBID;
	NetMsgAns.bOK = bOK;
	NETSEND(&NetMsgAns);

	return S_OK;
}


//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqClubBattleSubmission ( DWORD dwCLUBID )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_SUBMISSION_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_SUBMISSION_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsBattle( dwCLUBID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_SUBMISSION_FB_NOBATTLE") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsBattleStop( dwCLUBID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMCLUB_BATTLE_SUBMISSION_FB_DISTIME") );
		return S_FALSE;
	}

	GLMSG::SNET_CLUB_BATTLE_SUBMISSION_REQ NetMsg;
	NetMsg.dwClubID = dwCLUBID;
	NETSEND(&NetMsg);	

	return S_OK;
}

//	Note : ???? ???? ???? ????.
HRESULT GLCharacter::ReqAllianceBattleSubmission ( DWORD dwCLUBID )
{
	if ( m_dwGuild==CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_SUBMISSION_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsMaster(m_dwCharID) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_SUBMISSION_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsAlliance() || !m_sCLUB.IsChief() ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_SUBMISSION_FB_NOTMASTER") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsBattleAlliance( dwCLUBID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_SUBMISSION_FB_NOBATTLE") );
		return S_FALSE;
	}

	if ( !m_sCLUB.IsBattleStop( dwCLUBID ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMALLIANCE_BATTLE_SUBMISSION_FB_DISTIME") );
		return S_FALSE;
	}

	GLMSG::SNET_ALLIANCE_BATTLE_SUBMISSION_REQ NetMsg;
	NetMsg.dwClubID = dwCLUBID;
	NETSEND(&NetMsg);	

	return S_OK;
}

bool GLCharacter::IsKEEP_CLUB_STORAGE ( DWORD dwCHANNEL )
{
	if ( m_sCLUB.m_dwID==CLUB_NULL )		return false;
	if ( m_sCLUB.m_dwMasterID!=m_dwCharID )	return false;

	if ( !m_sCLUB.m_bVALID_STORAGE )		return false;

	if ( m_sCLUB.m_dwRank < dwCHANNEL )		return false;

	return true;
}

//	Note : ???? ?????? ?????? ????.
HRESULT GLCharacter::ReqGetClubStorage ()
{
	if ( !IsValidBody() )					return S_FALSE;
	if ( m_sCLUB.m_dwID==CLUB_NULL )		return S_FALSE;
	if ( m_sCLUB.m_dwMasterID!=m_dwCharID )	return S_FALSE;

	//	Note : ???? ???? ?????? ?????????? ????.
	if ( !m_sCLUB.m_bVALID_STORAGE )
	{
		//	Note : ?????? ????.
		//		?? ?????? ???? ?? ?????????? ?????? ???? ?????? ????????.
		GLMSG::SNET_CLUB_GETSTORAGE	NetMsg;
		NETSENDTOFIELD ( &NetMsg );
	}
	else
	{
		//	Note : ?????? ???? ???? ????.
		GLMSG::SNET_CLUB_INCOME_RENEW NetMsgReNew;
		NETSENDTOFIELD ( &NetMsgReNew );
	}

	return S_OK;
}

//	Note : ???? ?????? ????, ??????, ????????, ??????.
HRESULT GLCharacter::ReqClubStorageTo ( DWORD dwChannel, WORD wPosX, WORD wPosY )
{
	GASSERT(MAX_CLUBSTORAGE>dwChannel);
	if ( MAX_CLUBSTORAGE<=dwChannel )	return false;

	if ( !IsValidBody() )						return S_FALSE;
	if ( ValidWindowOpen() )					return S_FALSE;	

	if ( !IsKEEP_CLUB_STORAGE(dwChannel) )			return S_FALSE;

	//	???? ???? ????
	if ( VALID_HOLD_ITEM () )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
		if ( !pITEM )		return S_FALSE;
	
		if ( !pITEM->sBasicOp.IsEXCHANGE() )		return S_FALSE;
	}

	SINVENITEM* pInvenItem = m_sCLUB.m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !VALID_HOLD_ITEM () && !pInvenItem )		return E_FAIL;

	if ( VALID_HOLD_ITEM () && pInvenItem )
	{
#if defined(VN_PARAM) //vietnamtest%%%
		if ( GET_HOLD_ITEM().bVietnamGainItem  )	return E_FAIL;
#endif
		GLMSG::SNET_CLUB_STORAGE_EX_HOLD NetMsg;
		NetMsg.dwChannel = dwChannel;
		NetMsg.wPosX = pInvenItem->wPosX;
		NetMsg.wPosY = pInvenItem->wPosY;
		NETSENDTOFIELD ( &NetMsg );
	}
	else if ( pInvenItem )
	{
		GLMSG::SNET_CLUB_STORAGE_TO_HOLD NetMsg;
		NetMsg.dwChannel = dwChannel;
		NetMsg.wPosX = pInvenItem->wPosX;
		NetMsg.wPosY = pInvenItem->wPosY;
		NETSENDTOFIELD ( &NetMsg );
	}
	else if ( VALID_HOLD_ITEM () )
	{
#if defined(VN_PARAM) //vietnamtest%%%
		if ( GET_HOLD_ITEM().bVietnamGainItem  )	return E_FAIL;
#endif		

		//	Note : ?????? ???????? ?????????? ???? ??????.
		//
		SITEM* pItem = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
		GASSERT(pItem&&"?????? ???????? ???????? ????");

		BOOL bOk = m_sCLUB.m_cStorage[dwChannel].IsInsertable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )
		{
			//	?????? ????????.
			return E_FAIL;
		}

		GLMSG::SNET_CLUB_HOLD_TO_STORAGE NetMsg;
		NetMsg.dwChannel = dwChannel;
		NetMsg.wPosX = wPosX;
		NetMsg.wPosY = wPosY;

		NETSENDTOFIELD ( &NetMsg );
	}

	return S_OK;
}

bool GLCharacter::IsClubStorageSplitItem ( DWORD dwChannel, WORD wPosX, WORD wPosY )
{
	GASSERT(MAX_CLUBSTORAGE>dwChannel);
	if ( MAX_CLUBSTORAGE<=dwChannel )	return false;

	if ( !IsValidBody() )				return false;
	if ( ValidWindowOpen() )			return false;		
	
	if ( !IsKEEP_CLUB_STORAGE(dwChannel) )			return false;

	SINVENITEM* pInvenItem = m_sCLUB.m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return false;

	//	Note : ?????? ???? ????????.
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	GASSERT(pItem&&"?????? ???????? ???????? ????");

	bool bSPLIT(false);
	bSPLIT = ( pItem->ISPILE() );
	if ( !bSPLIT )					return false;
	bSPLIT = ( pInvenItem->sItemCustom.wTurnNum>1 );

	return bSPLIT;
}

//	Note : ???? - ???? ?????? ????.
HRESULT GLCharacter::ReqClubStorageSplit ( DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wSplitNum )
{
	if ( !IsClubStorageSplitItem(dwChannel,wPosX,wPosY) )	return E_FAIL;

	SINVENITEM* pInvenItem = m_sCLUB.m_cStorage[dwChannel].FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return false;
	
	if ( pInvenItem->sItemCustom.wTurnNum <= wSplitNum )
	{
		return E_FAIL;
	}

	//	Note : ?????? ???? ?????? ????.
	GLMSG::SNET_CLUB_STORAGE_SPLIT NetMsg;
	NetMsg.dwChannel = dwChannel;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NetMsg.wSplit = wSplitNum;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

//	Note : ???? ?? ????.
HRESULT GLCharacter::ReqClubStorageSaveMoney ( LONGLONG lnMoney )
{
	if( m_sCLUB.m_dwID==CLUB_NULL )				return E_FAIL;
	if( m_sCLUB.m_dwMasterID!=m_dwCharID )		return E_FAIL;
	if( !m_sCLUB.m_bVALID_STORAGE )				return E_FAIL;

//#if !defined(KR_PARAM) && !defined(KRT_PARAM)
//	if( m_lnMoney%UINT_MAX < lnMoney )			return E_FAIL;
//#endif
	if ( m_lnMoney < lnMoney )					return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;	

	GLMSG::SNET_CLUB_STORAGE_SAVE_MONEY NetMsg;
	NetMsg.lnMoney = lnMoney;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//	Note : ???? ?? ??????.
HRESULT GLCharacter::ReqClubStorageDrawMoney ( LONGLONG lnMoney )
{
	if ( m_sCLUB.m_dwID==CLUB_NULL )			return E_FAIL;
	if ( m_sCLUB.m_dwMasterID!=m_dwCharID )		return E_FAIL;

	if ( !m_sCLUB.m_bVALID_STORAGE )			return E_FAIL;

//#if !defined(KR_PARAM) && !defined(KRT_PARAM)
//	if ( m_sCLUB.m_lnStorageMoney%UINT_MAX < lnMoney )	return E_FAIL;
//#endif
	if ( m_sCLUB.m_lnStorageMoney < lnMoney )	return E_FAIL;
	if ( ValidWindowOpen() )					return E_FAIL;

	GLMSG::SNET_CLUB_STORAGE_DRAW_MONEY NetMsg;
	NetMsg.lnMoney = lnMoney;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqGarbageOpen()
{
	InitGarbageData();
	OpenGarbage();
	
    return S_OK;
}

HRESULT GLCharacter::ReqGarbageMoveItem()
{
	if( m_sPreInventoryItem.VALID() )
	{
		// ???? ?? ???????? ???? ???????????? ???? ?? ????
		SITEMCUSTOM sPreItem = GET_PREHOLD_ITEM();
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sPreItem.sNativeID );

		// ????????????
		if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD )
		{
			GLPetClient* pMyPet = GLGaeaClient::GetInstance().GetPetClient ();
			if ( pMyPet && pMyPet->IsVALID () && sPreItem.dwPetID == pMyPet->m_dwPetID )
			{
				m_sPreInventoryItem.RESET();
				return E_FAIL;
			}
		}

        // disabled garbage basicop on item to make all item default deletable by garbage item
		//if( pItem && pItem->sBasicOp.IsGarbage() )
		//{
			m_sGarbageItem.SET( m_sPreInventoryItem.wPosX, m_sPreInventoryItem.wPosY );
		//}
		
		m_sPreInventoryItem.RESET();
	}
	
	return S_OK;
}

HRESULT GLCharacter::ReqGarbageResult()
{
	
	if( !m_sGarbageItem.VALID() )
		return S_FALSE;

	GLMSG::SNET_GARBAGE_RESULT NetMsg;

	NetMsg.wPosX = m_sGarbageItem.wPosX;
	NetMsg.wPosY = m_sGarbageItem.wPosY;
	
	NETSENDTOFIELD( &NetMsg );
	
	return S_OK;
}

HRESULT GLCharacter::ReqGarbageClose()
{
	InitGarbageData();
	CloseGarbage();

	return S_OK;
}

const SITEMCUSTOM& GLCharacter::GET_GARBAGE_ITEM()	// ITEMREBUILD_MARK
{
	static SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();

	if( !m_sGarbageItem.VALID() )
		return sItemCustom;

	SINVENITEM* pResistItem = m_cInventory.GetItem( m_sGarbageItem.wPosX, m_sGarbageItem.wPosY );
	if( !pResistItem )
		return sItemCustom;

	sItemCustom = pResistItem->sItemCustom;

	return sItemCustom;
}

VOID GLCharacter::InitGarbageData()
{
	m_sGarbageItem.RESET();
	m_sPreInventoryItem.RESET();
}

HRESULT GLCharacter::ReqUsePetCard ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;

	// ?????????? ???? ?????? ?? ????.
	if ( m_sCONFTING.IsCONFRONTING() )			return E_FAIL;

	PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap ();
	if ( !pLandManClient )						return E_FAIL;

	GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
	GLMapList::FIELDMAP_ITER iter = MapsList.find ( pLandManClient->GetMapID ().dwID );
	if ( iter==MapsList.end() )					return E_FAIL;

	const SMAPNODE *pMapNode = &(*iter).second;

	// ?????????????? ????
	if ( !pMapNode->bPetActivity )
	{
		CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_INVALIDZONE") );
		return E_FAIL;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		// ?????? ????
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		// ?????? ????
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PET_CARD )
	{
		// ?????? ??????
		return E_FAIL;
	}
	
	m_llPetCardGenNum	  = pInvenItem->sItemCustom.lnGenNum;
	m_sPetCardNativeID    = pInvenItem->sItemCustom.sNativeID;
	m_cPetCardGenType     = pInvenItem->sItemCustom.cGenType;

	GLMSG::SNETPET_REQ_USEPETCARD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqReGenPet ()
{
	if ( !IsValidBody() )						return E_FAIL;

	PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap ();
	if ( !pLandManClient )						return E_FAIL;

	GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
	GLMapList::FIELDMAP_ITER iter = MapsList.find ( pLandManClient->GetMapID ().dwID );
	if ( iter==MapsList.end() )					return E_FAIL;

	const SMAPNODE *pMapNode = &(*iter).second;

	if ( !pMapNode->bPetActivity )				return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.FindItemByGenNumber ( m_llPetCardGenNum, m_sPetCardNativeID, m_cPetCardGenType );
	if ( !pInvenItem ) return E_FAIL;
	
	GLMSG::SNETPET_REQ_USEPETCARD NetMsg;
	NetMsg.wPosX = pInvenItem->wPosX;
	NetMsg.wPosY = pInvenItem->wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

// *****************************************************
// Desc: DB???? ?????? ???? ???? ????
// *****************************************************
HRESULT	GLCharacter::ReqPetReviveInfo ()
{
	GLMSG::SNETPET_REQ_PETREVIVEINFO NetMsg;
	NetMsg.dwCharID = m_dwCharID;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

// *****************************************************
// Desc: DB???? ?????? ?? ????????
// *****************************************************
HRESULT GLCharacter::ReqPetRevive ( DWORD dwPetID )
{
	GLMSG::SNETPET_REQ_REVIVE NetMsg;
	NetMsg.dwPetID = dwPetID;
	NetMsg.wPosX   = m_wInvenPosX1;
	NetMsg.wPosY   = m_wInvenPosY1;
	NETSENDTOFIELD ( &NetMsg );	

	return S_OK;
}

// *****************************************************
// Desc: ???????? ???? ????
// *****************************************************
HRESULT	GLCharacter::ReqPetCardInfo ()
{
	GLInventory::CELL_MAP* pMapItem = m_cInventory.GetItemList();
	GLInventory::CELL_MAP_CITER iter = pMapItem->begin();
	GLInventory::CELL_MAP_CITER iter_end = pMapItem->end();
	for ( ; iter!=iter_end; ++iter )
	{
		SINVENITEM* pInvenItem = (*iter).second;
		SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
		if ( pItem && pItem->sBasicOp.emItemType == ITEM_PET_CARD && pInvenItem->sItemCustom.dwPetID != 0 )
		{
			GLMSG::SNETPET_REQ_PETCARDINFO NetMsg;
			NetMsg.dwPetID = pInvenItem->sItemCustom.dwPetID;
			NETSENDTOFIELD ( &NetMsg );
		}
	}

	for ( WORD i = 0; i < EMSTORAGE_CHANNEL; ++i )
	{
		if ( m_bStorage[i] )
		{
			GLInventory::CELL_MAP* pMapItem = m_cStorage[i].GetItemList();
			GLInventory::CELL_MAP_CITER iter = pMapItem->begin();
			GLInventory::CELL_MAP_CITER iter_end = pMapItem->end();
			for ( ; iter!=iter_end; ++iter )
			{
				SINVENITEM* pInvenItem = (*iter).second;
				SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
				if ( pItem && pItem->sBasicOp.emItemType == ITEM_PET_CARD && pInvenItem->sItemCustom.dwPetID != 0  )
				{
					GLMSG::SNETPET_REQ_PETCARDINFO NetMsg;
					NetMsg.dwPetID = pInvenItem->sItemCustom.dwPetID;
					NETSENDTOFIELD ( &NetMsg );
				}
			}
		}
	}
	
	return S_OK;
}

HRESULT GLCharacter::ReqSetPhoneNumber ( const TCHAR * szPhoneNumber )
{
	if( !szPhoneNumber )	return S_FALSE;

	GLMSG::SNETPC_PHONE_NUMBER NetMsg;

	StringCchCopy ( NetMsg.szPhoneNumber, SMS_RECEIVER, szPhoneNumber );

	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqSendSMS( int nCharID, const TCHAR * szPhoneNumber, const TCHAR * szSmsMsg )
{
	if( !szPhoneNumber )	return S_FALSE;
	if( !szSmsMsg )			return S_FALSE;

	SINVENITEM *pINVENITEM = m_cInventory.FindItem ( ITEM_SMS );
	if ( !pINVENITEM )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMSMS_FB_NO_ITEM") );
		return S_FALSE;
	}

	GLMSG::SNETPC_SEND_SMS NetMsg;

	NetMsg.dwReceiveChaNum = nCharID;
	StringCchCopy ( NetMsg.szPhoneNumber, SMS_RECEIVER, szPhoneNumber );
	StringCchCopy ( NetMsg.szSmsMsg, SMS_LENGTH, szSmsMsg );
	NetMsg.wItemPosX = pINVENITEM->wPosX;
	NetMsg.wItemPosY = pINVENITEM->wPosY;

	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqMGameOddEvenBatting( UINT uiBattingMoney )
{
	// ???? ???? ???? ?????? ????????. ???????????? ????
	if( uiBattingMoney > MINIGAME_ODDEVEN::uiMaxBattingMoney )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, 
														ID2GAMEINTEXT( "EMMGAME_ODDEVEN_FB_MAXBATTING" ), 
														MINIGAME_ODDEVEN::uiMaxBattingMoney );
		return S_FALSE;
	}

	// ?????????? ????????. ???????????? ????
	if( uiBattingMoney > m_lnMoney )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMMGAME_ODDEVEN_FB_MONEY_FAIL" ) );
		return S_FALSE;
	}

	// ???? ?????? ???????? ?????? ????????.
	GLMSG::SNETPC_MGAME_ODDEVEN NetMsg;
	NetMsg.dwNPCID = m_dwNPCID;
	NetMsg.emEvent = EMMGAME_ODDEVEN_OK;
	NetMsg.uiBattingMoney = uiBattingMoney;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqMGameOddEvenAgain()
{
	// ???????? ???? ?????? ????

	GLMSG::SNETPC_MGAME_ODDEVEN NetMsg;
	NetMsg.dwNPCID = m_dwNPCID;
	NetMsg.emEvent = EMMGAME_ODDEVEN_AGAIN_OK;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqMGameOddEvenCancel()
{
	// ?????? ???? ?????? ??????.
	GLMSG::SNETPC_MGAME_ODDEVEN NetMsg;
	NetMsg.dwNPCID = m_dwNPCID;
	NetMsg.emEvent = EMMGAME_ODDEVEN_CANCEL;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqMGameOddEvenSelect( BOOL bOdd )
{
	// ???????? ????, ?????? ????
	// bOdd?? TRUE?? ????, FALSE?? ????
	GLMSG::SNETPC_MGAME_ODDEVEN NetMsg;
	NetMsg.dwNPCID = m_dwNPCID;
	NetMsg.emEvent = EMMGAME_ODDEVEN_SELECT;

	if( bOdd )	NetMsg.emCase = EMMGAME_ODDEVEN_ODD;
	else		NetMsg.emCase = EMMGAME_ODDEVEN_EVEN;

	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqMGameOddEvenShuffle()
{
	GLMSG::SNETPC_MGAME_ODDEVEN NetMsg;
	NetMsg.dwNPCID = m_dwNPCID;
	NetMsg.emEvent = EMMGAME_ODDEVEN_SHUFFLE;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqMGameOddEvenFinish()
{
	GLMSG::SNETPC_MGAME_ODDEVEN NetMsg;
	NetMsg.dwNPCID = m_dwNPCID;
	NetMsg.emEvent = EMMGAME_ODDEVEN_FINISH;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqFriendWindowOpen( bool bOpen )
{
	GLMSG::SNETPC_REQ_FRIEND_CLUB_OPEN NetMsg;
	NetMsg.emTYPE = EMFRIEND_WINDOW;
	NetMsg.bOpen = bOpen;

	NETSEND( &NetMsg );
	
	return S_OK;
}

HRESULT GLCharacter::ReqClubInfoUpdate( bool bUpdate )
{
	if ( !bUpdate ) 
	{
		if ( CInnerInterface::GetInstance().IsVisibleGroup( CLUB_WINDOW ) || 
			 CInnerInterface::GetInstance().IsVisibleGroup( LARGEMAP_WINDOW ) ) 
			return S_OK;
	}

	GLMSG::SNETPC_REQ_FRIEND_CLUB_OPEN NetMsg;
	NetMsg.emTYPE = EMCLUB_WINDOW;
	NetMsg.bOpen = bUpdate;

	NETSEND( &NetMsg );

	return S_OK;
}


HRESULT GLCharacter::SetVehicle ( bool bActive )
{
	// ???????? ???? ?????? ?????? ???????? ?????? ????.
 	if ( bActive )
	{
		if ( m_bVehicle ) return E_FAIL;
		m_bVehicle = TRUE;
		m_bReqVehicle = FALSE;
		
		int emType = m_sVehicle.m_emTYPE ;

		if ( emType == VEHICLE_TYPE_BOARD ) //modify vehicle anim
		{
			m_emANISUBTYPE = (EMANI_SUBTYPE) (AN_SUB_HOVERBOARD ) ;  
		}

		if ( !IsSTATE(EM_ACT_PEACEMODE) ) ReqTogglePeaceMode();

		// ???? ?????? ????
		STARGETID sTargetID(CROW_PC, m_dwGaeaID, m_vPos);
		DxEffGroupPlayer::GetInstance().NewEffGroup
		(
			GLCONST_CHAR::strVEHICLE_GEN_EFFECT.c_str(),
			m_matTrans,
			&sTargetID
		);

		SITEMCUSTOM& sItemCustom = m_PutOnItems[SLOT_VEHICLE];

		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
		if ( !pItem ) return E_FAIL;

		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::IDCOLOR, ID2GAMEINTEXT("VEHICLE_SET_FB_OK"), pItem->GetName() );

		DxViewPort::GetInstance().SetVehicleCamera();

		CInnerInterface::GetInstance().SetVehicleDisplay();//add vehicleimage

	}
	else
	{
		if ( !m_bVehicle )	return E_FAIL;

		m_bVehicle = FALSE;		
		m_bReqVehicle = FALSE;

		EMSLOT emRHand = GetCurRHand();
		EMSLOT emLHand = GetCurLHand();
		m_emANISUBTYPE = CHECK_ANISUB ( m_pITEMS[emRHand], m_pITEMS[emLHand]  );

		SITEMCUSTOM& sItemCustom = m_PutOnItems[SLOT_VEHICLE];
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
		if ( pItem ) CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::IDCOLOR, ID2GAMEINTEXT("VEHICLE_RESET_FB_OK"),pItem->GetName() );
		
		DxViewPort::GetInstance().SetGameCamera();

		CInnerInterface::GetInstance().ResetVehicleDisplay();//add vehicleimage
	}


	/*Disable remove buff and Qitem when use vehicle
	// ???? ???? ????
	for ( int i = 0; i < SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
		
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData( m_sSKILLFACT[i].sNATIVEID );

		if ( pSkill && pSkill->m_sBASIC.emIMPACT_SIDE != SIDE_ENERMY )
		{
			FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT[i].sNATIVEID );
			DISABLESKEFF( i );
		}	
	}	

	// ???? ?????? ????
	m_sQITEMFACT.RESET ();
	CInnerInterface::GetInstance().RESET_KEEP_QUESTION_ITEM ();
	*/

	GLCHARLOGIC::INIT_DATA ( FALSE, FALSE );
	UpdateSuit( TRUE );
	ReSelectAnimation();

	return S_OK;

}

HRESULT GLCharacter::ReqSetVehicle(  bool bActive )
{
	// ?????? ??????
	if ( bActive )
	{
		// ?????????? ?????? ?????? ?????? ????.
		if ( m_sCONFTING.IsCONFRONTING() )			return E_FAIL;

		if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL)  || 
			 IsACTION(GLAT_FALLING) || IsACTION(GLAT_TALK) || 
			 IsACTION(GLAT_GATHERING) )	return E_FAIL;

		if ( !m_sVehicle.IsActiveValue() )			return E_FAIL;

		// ?? ???? 
		PLANDMANCLIENT pLandManClient = GLGaeaClient::GetInstance().GetActiveMap ();
		if ( !pLandManClient )						return E_FAIL;

		GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
		GLMapList::FIELDMAP_ITER iter = MapsList.find ( pLandManClient->GetMapID ().dwID );
		if ( iter==MapsList.end() )					return E_FAIL;

		const SMAPNODE *pMapNode = &(*iter).second;

		// ?????????????? ????
		if ( !pMapNode->bVehicleActivity )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_MAP_FAIL") );
			return S_OK;
		}


		// ?????? ???? ???? ???? 		
		SITEMCUSTOM& sItemCostom = m_PutOnItems[SLOT_VEHICLE];
		if ( sItemCostom.sNativeID == NATIVEID_NULL() )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_NO_ITEM") );
			return E_FAIL;
		}

		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCostom.sNativeID );
		if ( !pItem || pItem->sBasicOp.emItemType != ITEM_VEHICLE )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_NO_ITEM") );
			return E_FAIL;		
		}

		if ( m_sVehicle.IsNotEnoughFull() )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_NOTENOUGH_OIL") );
			return E_FAIL;	
		}

		m_sVehicleNativeID  = sItemCostom.sNativeID; //add vehicleimage

		m_bReqVehicle = TRUE;
		
		
		// ?????? ????
		GLMSG::SNETPC_ACTIVE_VEHICLE NetMsg;
		NetMsg.bActive = true;

		NETSENDTOFIELD( &NetMsg );

		return S_OK;
		
	}
	// ?????? ????????
	else 
	{
		if ( !m_sVehicle.IsActiveValue() )			return E_FAIL;

		if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) || IsACTION(GLAT_FALLING) )	return E_FAIL;

		// ?????? ???? ???? ???? 
		SITEMCUSTOM& sItemCostom = m_PutOnItems[SLOT_VEHICLE];
		if ( sItemCostom.sNativeID == NATIVEID_NULL() )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_NO_ITEM") );
			return E_FAIL;
		}

		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCostom.sNativeID );
		if ( !pItem || pItem->sBasicOp.emItemType != ITEM_VEHICLE )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_NO_ITEM") );
			return E_FAIL;		
		}

		// ?????? ????
		GLMSG::SNETPC_ACTIVE_VEHICLE NetMsg;
		NetMsg.bActive = false;

		m_bReqVehicle = TRUE;

		NETSENDTOFIELD( &NetMsg );

		return S_OK;		
		
	}

	return S_OK;
}

HRESULT GLCharacter::ReqVehicleUpdate()
{
	if ( m_sVehicle.IsActiveValue() ) return E_FAIL;

	// ?????? ???? ???? ???? 
	SITEMCUSTOM& sItemCostom = m_PutOnItems[SLOT_VEHICLE];
	if ( sItemCostom.sNativeID == NATIVEID_NULL() )
	{
		return E_FAIL;
	}

	GLMSG::SNETPC_GET_VEHICLE NetMsg;
	NetMsg.nItemID = sItemCostom.sNativeID;

	NETSENDTOFIELD( &NetMsg );

	return S_OK;
}

void GLCharacter::ReqVehicleChangeAccessory( EMSUIT emSUIT )
{
	if ( GLTradeClient::GetInstance().Valid() )		return;

	//	Note : ?????????? ???? ???? ???? ???? ?????? ???? ?? ?? ?????? ??.
	//
	if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) ) return;

	SITEMCUSTOM sHoldItemCustom = GET_HOLD_ITEM ();
	SITEMCUSTOM	sSlotItemCustom = m_sVehicle.GetSlotitembySuittype ( emSUIT );

	//	SLOT <-> HOLD
	if ( sHoldItemCustom.sNativeID != NATIVEID_NULL() && sSlotItemCustom.sNativeID != NATIVEID_NULL() )
	{
		SITEM* pHoldItem = GLItemMan::GetInstance().GetItem ( sHoldItemCustom.sNativeID );
		if ( !pHoldItem ) 
		{
			// ???? ????
			return;
		}

		if ( !m_sVehicle.CheckSlotItem( pHoldItem->sBasicOp.sNativeID, emSUIT ) ) return;
		
		SITEM* pSlotItem = GLItemMan::GetInstance().GetItem ( sSlotItemCustom.sNativeID );
		if ( !pSlotItem ) 
		{
			// ???? ????
			return;
		}

		if ( !GLGaeaClient::GetInstance().GetCharacter()->ACCEPT_ITEM ( sHoldItemCustom.sNativeID ) )
		{
			return;
		}

		// ?????? ??????
		if ( pHoldItem->sSuitOp.emSuit != pSlotItem->sSuitOp.emSuit ) return;

		GLMSG::SNET_VEHICLE_REQ_SLOT_EX_HOLD NetMsg;
		NetMsg.emSuit = emSUIT;
		NETSENDTOFIELD ( &NetMsg );
	}
	// HOLD -> SLOT
	else if ( sHoldItemCustom.sNativeID != NATIVEID_NULL() )
	{
		SITEM* pHoldItem = GLItemMan::GetInstance().GetItem ( sHoldItemCustom.sNativeID );
		if ( !pHoldItem ) 
		{
			// ???? ????
			return;
		}

		if ( !m_sVehicle.CheckSlotItem( pHoldItem->sBasicOp.sNativeID, emSUIT ) ) return;

		if ( !GLGaeaClient::GetInstance().GetCharacter()->ACCEPT_ITEM ( sHoldItemCustom.sNativeID ) )
		{
			return;
		}

		// ?????? ??????
		if ( pHoldItem->sSuitOp.emSuit != emSUIT ) return;

		GLMSG::SNET_VEHICLE_REQ_HOLD_TO_SLOT NetMsg;
		NetMsg.emSuit = emSUIT;
		NETSENDTOFIELD ( &NetMsg );
	}
	// SLOT -> HOLD
	else if ( sSlotItemCustom.sNativeID != NATIVEID_NULL() )
	{
		GLMSG::SNET_VEHICLE_REQ_SLOT_TO_HOLD NetMsg;
		NetMsg.emSuit = emSUIT;
		NETSENDTOFIELD ( &NetMsg );
	}

	return;

}



void GLCharacter::ReqVehicleRemoveSlotItem( EMSUIT emSUIT )
{
	WORD wPosX(0), wPosY(0);
	SITEMCUSTOM	sSlotItemCustom = m_sVehicle.GetSlotitembySuittype ( emSUIT );

	SITEM* pSlotItem = GLItemMan::GetInstance().GetItem ( sSlotItemCustom.sNativeID );
	if ( !pSlotItem ) 
	{
		// ???? ????
		return;
	}

	BOOL bOk = m_cInventory.FindInsrtable ( pSlotItem->sBasicOp.wInvenSizeX, pSlotItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
	if ( !bOk )
	{
		//	?????? ????????.
		return;
	}

	GLMSG::SNET_VEHICLE_REQ_REMOVE_SLOTITEM NetMsg;
	NetMsg.emSuit = emSUIT;

	NETSENDTOFIELD ( &NetMsg );
}

void GLCharacter::ReqVehicleGiveBattery( WORD wPosX, WORD wPosY )
{
	// ?????? ?????????? ?????? ?? ?? ????.
	//if ( !IsVALID() )							return;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem ) return;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) return;

	// ?????? ?????? ???? ????
	if ( pItem->sBasicOp.emItemType != ITEM_VEHICLE )	return;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold ) return;

	// ???? ???? ????
	if ( pHold->sBasicOp.emItemType != ITEM_VEHICLE_OIL )	return;

	// ?????? ?????? ?????? ????
	if ( pInvenItem->sItemCustom.dwVehicleID == 0 ) return;



	SVEHICLEITEMINFO sVehicle;


	if ( !DxGlobalStage::GetInstance().IsEmulator() )
	{
		// ?????? ?????? ?????? ?????? ???? ??????????.
		VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pInvenItem->sItemCustom.dwVehicleID );
		if ( iter==m_mapVEHICLEItemInfo.end() ) return;
		sVehicle = (*iter).second;
	}
	else
	{
		sVehicle.m_nFull  = m_sVehicle.m_nFull;
		sVehicle.m_emTYPE = m_sVehicle.m_emTYPE;
	}

	// ?????? ?????? ??????
	if ( sVehicle.m_nFull >= 1000 ) return;


	GLMSG::SNET_VEHICLE_REQ_GIVE_BATTERY NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;

	NETSENDTOFIELD ( &NetMsg );
}

void GLCharacter::ReqVehicleInvenUpdate()
{
	GLInventory::CELL_MAP* pMapItem = m_cInventory.GetItemList();
	GLInventory::CELL_MAP_CITER iter = pMapItem->begin();
	GLInventory::CELL_MAP_CITER iter_end = pMapItem->end();
	for ( ; iter!=iter_end; ++iter )
	{
		SINVENITEM* pInvenItem = (*iter).second;
		SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
		if ( pItem && pItem->sBasicOp.emItemType == ITEM_VEHICLE && pInvenItem->sItemCustom.dwVehicleID != 0 )
		{
			GLMSG::SNET_VEHICLE_REQ_ITEM_INFO NetMsg;
			NetMsg.dwVehicleID = pInvenItem->sItemCustom.dwVehicleID;
			NETSENDTOFIELD ( &NetMsg );
		}
	}

	// ?????? ???? ???? ???? 
	SITEMCUSTOM& sItemCustom = m_PutOnItems[SLOT_VEHICLE];
	if ( sItemCustom.sNativeID != NATIVEID_NULL() )
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		if ( pItem && pItem->sBasicOp.emItemType == ITEM_VEHICLE && sItemCustom.dwVehicleID != 0 )
		{
			GLMSG::SNET_VEHICLE_REQ_ITEM_INFO NetMsg;
			NetMsg.dwVehicleID = sItemCustom.dwVehicleID;
			NETSENDTOFIELD ( &NetMsg );
		}
	}

	return;

}

void GLCharacter::ReqNonRebirth( BOOL bNonRebirth )
{
	GLMSG::SNET_NON_REBIRTH_REQ NetMsg;
	NetMsg.bNon_Rebirth = bNonRebirth;
	NETSENDTOFIELD ( &NetMsg );
}

void GLCharacter::ReqQBoxEnableState( bool bQboxEnable )
{
	GLMSG::SNET_QBOX_OPTION_REQ_AG NetMsg;
	NetMsg.bQBoxEnable = bQboxEnable;
	NETSEND ( &NetMsg );
}

void GLCharacter::ReqItemShopOpen( bool bOpen )		// ItemShop Open/Close ????
{
//#if defined ( JP_PARAM ) || defined ( _RELEASED)	// JAPAN Item Shop //add itemshop icon
	
	if ( m_bItemShopOpen == bOpen )	return;
	
	GLMSG::SNETPC_OPEN_ITEMSHOP	NetMsg;
	NetMsg.bOpen = bOpen;
	NETSENDTOFIELD( &NetMsg );
	
	m_bItemShopOpen = bOpen;
	
	CItemShopIconMan* pShopIconMan = CInnerInterface::GetInstance().GetItemShopIconMan();
	if ( !pShopIconMan ) return;

	if ( m_bItemShopOpen )
	{
		pShopIconMan->ADD_SHOP_ICON( m_dwGaeaID );
	}
	else
	{
		pShopIconMan->DEL_SHOP_ICON( m_dwGaeaID );
	}

//#endif
	return;
}

HRESULT GLCharacter::ReqAttendList ( bool bDay )
{
	if ( m_bReqAttendList && !bDay )	return S_OK;

	m_bReqAttendList = true;

	//	Note : ?????? ????.
	GLMSG::SNETPC_REQ_ATTENDLIST NetMsg;
	NETSEND(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqAttendance()
{
	//	???????????? ????
	CTime cCurTime = GLGaeaClient::GetInstance().GetCurrentTime();
	CTime cDayTime( cCurTime.GetYear(), cCurTime.GetMonth(), cCurTime.GetDay(),0,0,0 );
	CTime cLoginTime;
	if ( m_tAttendLogin < cDayTime.GetTime()  ) cLoginTime = cDayTime;
	else cLoginTime = m_tAttendLogin;

	CTimeSpan cGameTime = cCurTime - cLoginTime;
	if ( cGameTime.GetTotalMinutes() < m_dwAttendTime ) 
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ATTEND_FB_ATTENTIME"),m_dwAttendTime );
		return S_FALSE;
	}

	//	Note : ?????? ????.
	GLMSG::SNETPC_REQ_ATTENDANCE NetMsg;
	NETSEND(&NetMsg);

	return S_OK;
}


HRESULT GLCharacter::InvenUseTaxiCard( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_TICKET") );
		return E_FAIL;
	}

	CInnerInterface::GetInstance().OPEN_TAXI_WINDOW( wPosX, wPosY );

	return S_OK;
}

HRESULT GLCharacter::InvenUseNpcRecall( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_NPC_RECALL_FB_NOITEM") );
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_NPC_RECALL_FB_NOITEM") );
		return E_FAIL;
	}


	//	???? ????

	GLMSG::SNET_INVEN_NPC_RECALL NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqItemMix( DWORD dwNpcID )
{
	SINVENITEM	sInvenItem[ITEMMIX_ITEMNUM];
	for ( int i =0; i < ITEMMIX_ITEMNUM; ++i )
	{
		sInvenItem[i].sItemCustom = GET_ITEM_MIX( i );
		sInvenItem[i].wPosX = m_sItemMixPos[i].wPosX;
		sInvenItem[i].wPosY = m_sItemMixPos[i].wPosY;
	}

	GLItemMixMan::GetInstance().SortInvenItem( sInvenItem );	

	ITEM_MIX sItemMix;
	for( int i = 0; i < ITEMMIX_ITEMNUM; ++i ) 
	{
		sItemMix.sMeterialItem[i].sNID = GET_ITEM_MIX( i ).sNativeID;
		
		if( sItemMix.sMeterialItem[i].sNID != NATIVEID_NULL() )
			sItemMix.sMeterialItem[i].nNum = GET_ITEM_MIX( i ).wTurnNum;
	}

	GLItemMixMan::GetInstance().SortMeterialItem( sItemMix );
	const ITEM_MIX* pItemMix = GLItemMixMan::GetInstance().GetItemMix( sItemMix );
	
	if ( !pItemMix ) 
	{
		//	???? ?????? ????
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMITEM_MIX_FB_NOMIX") );
		CInnerInterface::GetInstance().SetItemMixResult( ID2GAMEINTEXT("EMITEM_MIX_FB_NOMIX") );
		return E_FAIL;
	}

	if ( m_lnMoney < sItemMix.dwPrice ) 
	{
		//	???? ????
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMITEM_MIX_FB_NOMONEY") );
		CInnerInterface::GetInstance().SetItemMixResult( ID2GAMEINTEXT("EMITEM_MIX_FB_NOMONEY") );
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_ITEM_MIX sNetMsg;

	sNetMsg.dwKey = pItemMix->dwKey;
	for ( int i = 0; i < ITEMMIX_ITEMNUM; ++i )
	{
		sNetMsg.sInvenPos[i].wPosX = sInvenItem[i].wPosX;
		sNetMsg.sInvenPos[i].wPosY = sInvenItem[i].wPosY;
	}
	sNetMsg.dwNpcID = dwNpcID;

	NETSENDTOFIELD( &sNetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqGathering( const STARGETID& sTargetID )
{
	PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLAND )	return E_FAIL;

	{
		//	Note : ?????? ???????? ?????????? ???? ??????.
		//
		// ????????
		if ( !IsValidBody() )	return E_FAIL;
		if ( IsACTION( GLAT_GATHERING ) )	return E_FAIL;

		//	???? ????
		const D3DXVECTOR3 &vTarPos = sTargetID.vPos;

		//	???? ????
		D3DXVECTOR3 vPos;
		vPos = m_vPos;

		D3DXVECTOR3 vDistance = vPos - vTarPos;
		float fDistance = D3DXVec3Length ( &vDistance );

		WORD wTarBodyRadius = 4;
		WORD wGatherRange = wTarBodyRadius + GETBODYRADIUS() + 2;
		WORD wGatherAbleDis = wGatherRange + 10;

		if ( fDistance>wGatherAbleDis )
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_FB_DISTANCE") );			
			return E_FAIL;
		}

		if ( sTargetID.emCrow!=CROW_MATERIAL )	
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_FB_NOTTYPE") );			
			return E_FAIL;
		}
		
		PGLMATERIALCLIENT pMaterial = pLAND->GetMaterial( sTargetID.dwID );
		if ( !pMaterial )
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMGATHER_FB_NOCROW") );			
			return E_FAIL;		
		}

		//	?????? ????.
		GLMSG::SNETPC_REQ_GATHERING NetMsg;
		NetMsg.dwTargetID = sTargetID.dwID;
		NETSENDTOFIELD ( &NetMsg );
	}

	return S_OK;
}

HRESULT GLCharacter::ReqCancelGathering()
{

	CInnerInterface::GetInstance().HideGroup( GATHER_GAUGE );
	return S_OK;
}
HRESULT GLCharacter::ReqNonInvenSkill ( SNATIVEID sSKILLID ) //Activate Skill w/o Scroll
{
	SNATIVEID sSKILL_ID = sSKILLID;

	if ( ISLEARNED_SKILL(sSKILL_ID) )
	{
		return E_FAIL;
	}

	EMSKILL_LEARNCHECK emSKILL_LEARNCHECK = CHECKLEARNABLE_SKILL(sSKILL_ID);
	if ( emSKILL_LEARNCHECK!=EMSKILL_LEARN_OK )
	{
		return E_FAIL;
	}

	GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN NetMsg;
	NetMsg.sSKILLID = sSKILLID;
	
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqEquipItem ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	EMITEM_TYPE emItemType = pItemData->sBasicOp.emItemType;
	EMSLOT J;				

	if ( pInvenItem->sItemCustom.sNativeID != NATIVEID_NULL()) 
	{
		if (pItemData)
		{
			switch ( pItemData->sSuitOp.emSuit )
			{
						case SUIT_HEADGEAR:	 J = SLOT_HEADGEAR;	break;
						case SUIT_UPPER:     J = SLOT_UPPER;	break;
						case SUIT_LOWER:	 J = SLOT_LOWER;	break;
						case SUIT_HAND:		 J = SLOT_HAND;		break;
						case SUIT_FOOT:		 J = SLOT_FOOT;		break;
						case SUIT_NECK:		 J = SLOT_NECK;		break;
						case SUIT_WRIST:	 J = SLOT_WRIST;	break;
						case SUIT_VEHICLE:	 J = SLOT_VEHICLE;	break;
						case SUIT_WAISTBAND: J = SLOT_WAIST;	break;
						case SUIT_ORNAMENT:	 J = SLOT_REARRING;	break;
                        case SUIT_EARRING:	 J = SLOT_LEARRING;break;
				case SUIT_HANDHELD:	
				if ( emItemType == ITEM_ARROW || emItemType == ITEM_CHARM || emItemType == ITEM_BULLET )
				{
					if(GLGaeaClient::GetInstance().GetCharacter()->IsUseArmSub()) J = SLOT_LHAND_S;
					else J = SLOT_LHAND;
				}
				else
				{
					if(GLGaeaClient::GetInstance().GetCharacter()->IsUseArmSub()) J = SLOT_RHAND_S;
					else J = SLOT_RHAND;
				}
				break;
				case SUIT_FINGER:	
				{
					if ( m_bRingSwap )
					{
						m_bRingSwap = FALSE;
						J = SLOT_RFINGER;
					}
					else
					{
						J = SLOT_LFINGER;
						m_bRingSwap = TRUE;
					}
				}
				break;
				case SUIT_ACCESSORIES:	
				{
					
					if ( m_bRingSwap )
					{
						m_bRingSwap = FALSE;
						J = SLOT_RACCESSORY;
					}
					else
					{
						J = SLOT_LACCESSORY;
						m_bRingSwap = TRUE;
					}
				}
				break;
			};
		}					
	}

	if ( !VALID_HOLD_ITEM () && !pInvenItem )
		return E_FAIL;

	if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) ||  m_sPMarket.IsOpen() )
		return E_FAIL;

	BOOL bOk;
	bOk = ACCEPT_ITEM ( pInvenItem->sItemCustom.sNativeID );
	if ( !bOk )
		return E_FAIL;

	bOk = CHECKSLOT_ITEM ( pInvenItem->sItemCustom.sNativeID, J );
	if ( !bOk )
		return E_FAIL;


	if ( GLTradeClient::GetInstance().Valid() )
	{
		if ( pInvenItem )	GLTradeClient::GetInstance().SetPreItem ( SINVEN_POS( pInvenItem->wPosX, pInvenItem->wPosY ) );
		else				GLTradeClient::GetInstance().ReSetPreItem ();

		return S_OK;
	}

	if( ValidRebuildOpen() || ValidGarbageOpen() || ValidItemMixOpen() )
	{
		if( m_sPreInventoryItem.wPosX == wPosX && m_sPreInventoryItem.wPosY == wPosY ) m_sPreInventoryItem.RESET();
		else
		{
			if( pInvenItem )	m_sPreInventoryItem.SET( wPosX, wPosY );
			else				m_sPreInventoryItem.RESET();
		}
	
		return S_OK;
	}

	GLMSG::SNETPC_REQ_INVEN_TO_HOLD NetMsg;
	NetMsg.wPosX = pInvenItem->wPosX;
	NetMsg.wPosY = pInvenItem->wPosY;
	NETSENDTOFIELD ( &NetMsg );
	
	if ( J == SLOT_RHAND ) J = GetCurRHand();
	if ( J == SLOT_LHAND ) J = GetCurLHand();

	BOOL bEMPTY_SLOT = ISEMPTY_SLOT(pInvenItem->sItemCustom.sNativeID,J);

	if (!bEMPTY_SLOT)
	{
		GLMSG::SNETPC_REQ_SLOT_EX_HOLD NetMsgEquip;
		NetMsgEquip.emSlot = J;
		NETSENDTOFIELD ( &NetMsgEquip );
				
		GLMSG::SNETPC_REQ_HOLD_TO_INVEN NetMsgToInven;
		NetMsgToInven.wPosX = wPosX;
		NetMsgToInven.wPosY = wPosY;

#if defined(VN_PARAM)
		NetMsgToInven.bUseVietnamInven = sCustom.bVietnamGainItem;
#else
		NetMsgToInven.bUseVietnamInven = FALSE;
#endif
		NETSENDTOFIELD ( &NetMsgToInven );
	}
	else
	{
		GLMSG::SNETPC_REQ_HOLD_TO_SLOT NetMsgEquip;
		NetMsgEquip.emSlot = J;
		NETSENDTOFIELD ( &NetMsgEquip );	
	}

	return S_OK;
}

HRESULT GLCharacter::ReqUnEquipItem ( EMSLOT emSlot )
{
	if ( !VALID_HOLD_ITEM() && !VALID_SLOT_ITEM(emSlot) )							return E_FAIL;
	if ( ValidWindowOpen() )														return E_FAIL;	
	if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) ||  m_sPMarket.IsOpen() )	return E_FAIL;

	bool bOk(false);
	WORD X,Y;

	for (int y=0; y<EM_INVENSIZE_Y; ++y ) 
	{
		if (bOk) break;

		for ( int x = 0; x < EM_INVENSIZE_X; ++x ) 
		{
			if ( m_cInventory.IsInsertable ( 1,1,x,y))
			{
				Y = y;
				X = x;
				bOk = true;
				break;
			}
		}
	}

	GLMSG::SNETPC_REQ_SLOT_TO_HOLD NetMsg;
	NetMsg.emSlot = emSlot;
	NETSENDTOFIELD ( &NetMsg );

	GLMSG::SNETPC_REQ_SLOT_TO_HOLD NetMsgEquip;
	NetMsgEquip.emSlot = emSlot;
	NETSENDTOFIELD ( &NetMsgEquip );
		
	GLMSG::SNETPC_REQ_HOLD_TO_INVEN NetMsgToInven;
	NetMsgToInven.wPosX = X;
	NetMsgToInven.wPosY = Y;
#if defined(VN_PARAM)
	NetMsgToInven.bUseVietnamInven = sCustom.bVietnamGainItem;
#else
	NetMsgToInven.bUseVietnamInven = FALSE;
#endif

	NETSENDTOFIELD ( &NetMsgToInven );

	return S_OK;
}

void GLCharacter::ReqRetrieveMobInMap()
{
	CInnerInterface::GetInstance().GetLargeMapWindow()->ClearMobList();

	GLMSG::SNETPC_REQ_CROWLIST_FLD NetMsg;
	NetMsg.dwGaeaID = m_dwGaeaID;
	NETSEND ( &NetMsg );
}

HRESULT GLCharacter::ReqRebuildOpen() //sealed card NaJDeV
{
	InitRebuildData();
	OpenRebuild();

	GLMSG::SNET_REBUILD_RESULT NetMsg;

	NetMsg.emResult = EMREBUILD_RESULT_OPEN;

	NETSENDTOFIELD( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqRebuildMoveItem() //sealed card NaJDeV
{
	GLMSG::SNET_REBUILD_MOVE_ITEM NetMsg;

	if( m_sPreInventoryItem.VALID() )
	{
		SITEMCUSTOM sPreItem = GET_PREHOLD_ITEM();
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sPreItem.sNativeID );
		if( pItem && pItem->sBasicOp.emItemType == ITEM_SUIT )
		{
			if( strlen( pItem->sRandomOpt.szNAME ) > 3 )
			{
				NetMsg.wPosX = m_sPreInventoryItem.wPosX;
				NetMsg.wPosY = m_sPreInventoryItem.wPosY;
			}
		}
		m_sPreInventoryItem.RESET();
	}

	NETSENDTOFIELD( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqRebuildResult( SITEMSEALDATA pData1, SITEMSEALDATA pData2 ) //sealed card NaJDeV
{
	if( !m_sRebuildItem.VALID() )
		return S_FALSE;

	if( m_i64RebuildInput != m_i64RebuildCost || m_lnMoney < m_i64RebuildCost )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREBUILD_RESULT_MONEY" ) );
		return E_FAIL;
	}

	GLMSG::SNET_REBUILD_RESULT NetMsg;
	NetMsg.dwNPCID = m_dwNPCID;
	NetMsg.emResult = EMREBUILD_RESULT_SUCCESS;
	NetMsg.Data1 = pData1;
	NetMsg.Data2 = pData2;
	NETSENDTOFIELD( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqRebuildInputMoney( LONGLONG i64InputMoney ) //sealed card NaJDeV
{
	GLMSG::SNET_REBUILD_INPUT_MONEY NetMsg;

	NetMsg.i64InputMoney = (LONGLONG)max( 0, i64InputMoney );
	NetMsg.i64InputMoney = (LONGLONG)min( NetMsg.i64InputMoney, m_lnMoney );

	NETSENDTOFIELD( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqRebuildClose() //sealed card NaJDeV
{
	InitRebuildData();

	CloseRebuild();

	GLMSG::SNET_REBUILD_RESULT NetMsg;

	NetMsg.emResult = EMREBUILD_RESULT_CLOSE;

	NETSENDTOFIELD( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::ReqRebuildMoveSeal() //sealed card NaJDeV
{
	GLMSG::SNET_REBUILD_MOVE_SEAL NetMsg;

	if( m_sPreInventoryItem.VALID() )
	{
		SITEMCUSTOM sCustom = GET_REBUILD_ITEM();
		if ( !sCustom.sNativeID.IsValidNativeID() )
		{
			CInnerInterface::GetInstance().PrintConsoleText( "Insert item first and then try again." );
			m_sPreInventoryItem.RESET();
			return E_FAIL;
		}

		SITEMCUSTOM sPreItem = GET_PREHOLD_ITEM();
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sPreItem.sNativeID );
		if( pItem && pItem->sBasicOp.emItemType == ITEM_SEALED_CARD )
		{
			NetMsg.wPosX = m_sPreInventoryItem.wPosX;
			NetMsg.wPosY = m_sPreInventoryItem.wPosY;
		}
		
		m_sPreInventoryItem.RESET();
	}
			
	NETSENDTOFIELD( &NetMsg );

	return S_OK;
}
