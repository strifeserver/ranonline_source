#include "StdAfx.h"
#include "./InnerInterface.h"

#include "AdminMessageDisplay.h"
#include "AcademyConftDisplay.h"
#include "BasicChatRightBody.h"
#include "BasicVarTextBox.h"
#include "BasicChat.h"
#include "BasicQuickSkillSlot.h"
#include "BasicGameMenu.h"
#include "BasicHWOptionWindow.h"
#include "BasicEscMenu.h"
#include "BasicInfoView.h"
#include "BonusTimeGauge.h"
#include "BonusTimeDisplay.h"
#include "BusWindow.h"
#include "BlockProgramAlarm.h"
#include "ClubWindow.h"
#include "ClubMake.h"
#include "ConftDisplayMan.h"
#include "ConftConfirm.h"
#include "ConftModalWindow.h"
#include "CommonWeb.h"
//#include "CharacterWindow.h"
#include "CharacterWindow/CharacterWindow.h"

#include "ChatMacroWindow.h"
#include "CountMsg.h"
#include "DamageDisplayMan.h"
#include "DialogueWindow.h"
#include "DxViewPort.h"
#include "DxGlobalStage.h"
#include "DxParamSet.h"
#include "EscMenuOpen.h"
#include "QBoxButton.h"
#include "FriendWindow.h"
#include "GameTextControl.h"
#include "GambleBox.h"
#include "GambleSelectBox.h"
#include "GambleAgainBox.h"
#include "GambleResultOddBox.h"
#include "GambleResultEvenBox.h"
#include "GambleAnimationBox.h"
#include "PetskinMixImage.h"
#include "GLGaeaClient.h"
#include "GLPartyClient.h"
#include "GLQuest.h"
#include "HeadChatDisplayMan.h"
#include "HairColorCard.h"
#include "HairStyleCard.h"
#include "HelpWindow.h"
#include "HelpWindowWeb.h"
#include "InventoryWindow.h"
#include "VNGainSysInventory.h"
#include "VNGainSysGauge.h"
#include "ItemBankWindow.h"
#include "ItemShopWindow.h"
#include "ItemGarbage.h"
#include "ItemMove.h"
#include "ItemShopIconMan.h"
#include "ItemShopWindowWeb.h"
#include "ItemRebuild.h"	// ITEMREBUILD_MARK
#include "KeySettingWindow.h"
#include "LargeMapWindow.h"
#include "MarketWindow.h"
#include "MiniMap.h"
#include "ModalWindow.h"
#include "ModalCallerID.h"
#include "MiniPartyWindow.h"
#include "MapMoveDisplay.h"
#include "NameDisplayMan.h"
#include "PrivateMarketWindow.h"
#include "PrivateMarketSellWindow.h"
#include "PrivateMarketMake.h"
#include "PrivateMarketShowMan.h"
#include "PtoPWindow.h"
#include "PartyWindow.h"
#include "PartyModalWindow.h"
#include "PetRebirthDialogue.h"//Monster7j
#include "PetColorCard.h"
#include "PetStyleCard.h"
#include "PetWindow.h"
#include "PetSkillWindow.h"
#include "QuestionItemDisplay.h"
#include "QuestWindow.h"
#include "QuestAlarm.h"
#include "QuestHelper.h"
#include "RebuildInventoryWindow.h"
#include "RebirthDialogue.h"
#include "ReceiveNoteWindow.h"
#include "SimpleHP.h"
#include "SimpleMessageMan.h"
#include "SystemMessageWindow.h"
#include "StorageWindow.h"
#include "StorageChargeCard.h"
#include "SkillTimeDisplay.h"
#include "SkillTrayTab.h"
#include "SkillWindow.h"
#include "SkillWindowToTray.h"
#include "SubMasterSet.h"
#include "SMSSendWindow.h"
#include "TradeWindow.h"
#include "TradeInventoryWindow.h"
#include "TargetInfoDisplay.h"
#include "TargetInfoDisplayNpc.h"
#include "TargetInfoDisplayPlayer.h"
#include "UIInfoLoader.h"
#include "UILeftTopGroup.h"
#include "WaitServerDialogue.h"
#include "WeaponDisplay.h"
#include "PetDisplay.h" //add petimage
#include "VehicleDisplay.h" //add vehicleimage
#include "WriteNoteWindow.h"
#include "WarningMsg.h"
#include "VehicleWindow.h"
#include "GenderChangeWindow.h"
#include "ThaiCCafeMark.h"
#include "GarbageInventoryWindow.h"
#include "ShopItemSearchWindow.h"
#include "ItemSearchResultWindow.h"
#include "SummonWindow.h"
#include "SummonPosionDisplay.h"
#include "AttenDanceBookWindow.h"
#include "FaceStyleCard.h"
#include "MapRequireCheck.h"
#include "ClubBattleModalWindow.h"
#include "TaxiWindow.h"
#include "ItemMixWindow.h"
#include "ItemMixInventoryWindow.h"
#include "GatherGauge.h"
#include "CdmRankingDisplay.h"

#include "QBoxTimeUnit.h" //add qbox
#include "PetTimeUnit.h" //add petimage
#include "VehicleTimeUnit.h" //add vehicleimage

#include "NotifyRecord.h"//add notify
#include "NotifyPost.h"//add notify
#include "NotifyCompete.h"//add notify

#include "BikeColorCard.h" //add bike color
#include "ItemColorCard.h" //add itemcolor

#include "ItemPreview.h"//add itempreview

#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../EngineSoundLib/DxSound/DxSoundLib.h"
#include "../EngineUiLib/GUInterface/BasicTextBox.h"
#include "ItemPreviewWindow/ItemPreviewWindow.h"
#include "PartyDisplay.h"
#include "CrowTargetInfo.h"
#include "CrowTargetInfoNpc.h"
#include "CrowTargetInfoPlayer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CInnerInterface::WAITSERVER_DIALOGUE_OPEN ( const CString& strMessage, const int nAction, const float fTimer )
{
	if ( IsVisibleGroup ( WAITSERVER_DISPLAY ) ) return ;

	ShowGroupFocus ( WAITSERVER_DISPLAY );
	m_pWaitServerDisplay->SetDisplayMessage ( strMessage, nAction, fTimer );
}

void CInnerInterface::WAITSERVER_DIALOGUE_CLOSE ()
{
	HideGroup ( WAITSERVER_DISPLAY );
}

void CInnerInterface::SetMiniMapInfo ( GLMapAxisInfo &sInfo, CString strMapName )
{
	if ( m_pMiniMap && m_pLargeMapWindow )
	{
		m_pMiniMap->SetMapAxisInfo ( sInfo, strMapName );
		m_pLargeMapWindow->SetMapAxisInfo ( sInfo, strMapName );
	}
}

const char* CInnerInterface::MakeString ( const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 256;

	va_list argList;
	static	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	return szBuffer;
}

void CInnerInterface::CloseAllWindow ()
{	
	CUIFocusContainer::UICONTROL_FOCUSLIST list = GetFocusList ()->GetFocusList ();
	CUIFocusContainer::UICONTROL_FOCUSLIST_CITER citer = list.begin ();
	CUIFocusContainer::UICONTROL_FOCUSLIST_CITER citer_end = list.end ();
	for ( ; citer != citer_end; ++citer )
	{
		UIGUID cID = citer->cID;

		if ( !IsVisibleGroup ( cID ) ) continue;

		if ( cID == INVENTORY_WINDOW || cID == MARKET_WINDOW ||
			cID == STORAGE_WINDOW || cID == CLUB_STORAGE_WINDOW )
		{
			SetMarketWindowClose ();
			SetStorageWindowClose ();
			SetClubStorageWindowClose ();
		}
		else if ( cID == TRADEINVENTORY_WINDOW )
		{
			if ( IsVisibleGroup ( TRADE_WINDOW ) ) SetTradeWindowCloseReq ();
			else if ( IsVisibleGroup ( PRIVATE_MARKET_WINDOW ) ) SetPrivateMarketClose ();
		}
		else if ( cID == TRADE_WINDOW )
		{							
			SetTradeWindowCloseReq ();
		}
		else if ( cID == MINIPARTY_WINDOW )
		{
			ShowGroupBottom ( MINIPARTY_OPEN );

//			#ifdef CH_PARAM // �߱� �������̽� ����
//				UpdatePotionTrayPosition();
//			#endif
		}
		else if ( cID == PRIVATE_MARKET_WINDOW )
		{
			SetPrivateMarketClose ();
		}
		else if( cID == ITEM_REBUILD_WINDOW || cID == REBUILDINVENTORY_WINDOW )	// ITEMREBUILD_MARK
		{
			CloseItemRebuildWindow();
		}
		else if( cID == ITEM_GARBAGE_WINDOW || cID == GARBAGEINVENTORY_WINDOW )	// ������
		{
			CloseItemGarbageWindow();
		}
		else if( cID == ITEM_MIX_WINDOW || cID == ITEM_MIX_INVEN_WINDOW )	// ������ ����
		{
			CloseItemMixWindow();
		}

		// ������� �ȵǴ� ��Ʈ���� ������ ��Ʈ���� �����.
		if ( cID != BASIC_CHAT_BOX && cID != BASIC_INFO_VIEW)
		{
#if defined(VN_PARAM) //vietnamtest%%%
			if( cID != VNGAINSYS_GAUGE_WINDOW )
#endif
			HideGroup ( citer->cID );
		}
	}
	
	m_bPartyStateBack = FALSE;
}

void	CInnerInterface::SetPtoPWindowOpen ( PGLCHARCLIENT pCharClient, DWORD dwGaeaID, GLCLUB & sClub )
{
	GASSERT( pCharClient );

	D3DXVECTOR3 *pPos = GLGaeaClient::GetInstance().FindCharPos( pCharClient->GetName() );
	if ( pPos )
	{
		D3DXVECTOR3 vPos = *pPos;
		vPos.y += pCharClient->GetBodyHeight();

		D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );

		const UIRECT& rcOriginPos = GetPtoPWindow ()->GetGlobalPos ();
		int nPosX = (int)( vScreen.x - ( rcOriginPos.sizeX * 0.5f ));
		int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );
		
		GetPtoPWindow ()->SetPtoPMenu ( pCharClient, dwGaeaID, sClub );	
		GetPtoPWindow ()->SetGlobalPos ( D3DXVECTOR2( float(nPosX), float(nPosY) ) );
		ShowGroupFocus ( PTOPMENU_WINDOW );
	}
}

void CInnerInterface::SetTradeWindowOpen ( CString szPlayerName, DWORD dwTargetID )
{
	if ( IsVisibleGroup ( TRADE_WINDOW ) ) return ;

	GetTradeWindow ()->SetTradeInfo ( szPlayerName, dwTargetID );
	CloseAllWindow ();	

	const long lResolution = GetResolution ();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	{
		const UIRECT& rcTradeWindow = GetTradeWindow()->GetGlobalPos ();

		D3DXVECTOR2 vPos;
		vPos.x = ((X_RES) / 2.0f) - rcTradeWindow.sizeX;
		vPos.y = (Y_RES - rcTradeWindow.sizeY) / 2.0f;
		GetTradeWindow()->SetGlobalPos ( vPos );
	}

	{
		const UIRECT& rcTradeInventoryWindow = GetTradeInventoryWindow()->GetGlobalPos ();

		D3DXVECTOR2 vPos;
		vPos.x = (X_RES) / 2.0f;
		vPos.y = (Y_RES - rcTradeInventoryWindow.sizeY) / 2.0f;
		GetTradeInventoryWindow()->SetGlobalPos ( vPos );
	}

	ShowGroupFocus ( TRADE_WINDOW );
	ShowGroupFocus ( TRADEINVENTORY_WINDOW );
}

void CInnerInterface::SetDamage ( D3DXVECTOR3 vPos, int nDamage, DWORD dwDamageFlag, BOOL bAttack )
{
	//	ī�޶� ������ �����ȿ� ������ ��ġ�� �ִ��� �˻�.
	//
	const CLIPVOLUME &CV = DxViewPort::GetInstance().GetClipVolume();
	BOOL bOK = COLLISION::IsCollisionVolume(CV,vPos,vPos);
	if ( !bOK )				return;
	
	//	������ �������� �κп��� �������� ��ġ�ϴ��� �˻�.
	//
	PLANDMANCLIENT pLandClient = GLGaeaClient::GetInstance().GetActiveMap();
	GASSERT(pLandClient&&"GLGaeaClient::GetInstance().GetActiveMap()");
	if ( !pLandClient )		return;

	DxLandMan *pLandMan = pLandClient->GetLandMan();
	GASSERT(pLandMan&&"pLandClient->GetLandMan()");
	if ( !pLandMan )		return;

	D3DXVECTOR3 vPoint1 = DxViewPort::GetInstance().GetFromPt();
	D3DXVECTOR3 vPoint2 = vPos;
	D3DXVECTOR3 vCollision;
	BOOL bCollision(FALSE);
	LPDXFRAME pDxFrame;
	pLandMan->IsCollision( vPoint1, vPoint2, vCollision, bCollision, pDxFrame, TRUE );
	if ( bCollision )		return;


	D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );
	const UIRECT& rcOriginPos = m_pDamageDisplayMan->GetGlobalPos ();

	//	Note : ������ �¿�� ������� ��������.
	//	20 ������ ������ ����
    int nPosX = (int)( vScreen.x - ( rcOriginPos.sizeX * 0.5f ));
	int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );

	static const int nMAXINTERVAL = 20;
	static const int nBASE = int(nMAXINTERVAL / 2);

	int xInterval = rand()%nMAXINTERVAL - nBASE;
	int yInterval = rand()%nMAXINTERVAL - nBASE;
	nPosX += xInterval;
	nPosY += yInterval;

	long lResolution = CUIMan::GetResolution ();
	WORD X_RES = HIWORD(lResolution);
	WORD Y_RES = LOWORD(lResolution);
	if ( ( nPosX < 0.0f ) || ( nPosY < 0.0f )  || 
		( float(X_RES) < nPosX )  || ( float(Y_RES) < nPosY ) )
	{
		return ;
	}

	m_pDamageDisplayMan->SetDamage ( nPosX, nPosY, nDamage, dwDamageFlag, bAttack );
}

void CInnerInterface::SetTargetInfo ( STARGETID sTargetID )
{
	CString strName;
	D3DCOLOR dwColor = NS_UITEXTCOLOR::DEFAULT;
	GLPWDATA sHP; //original gldwdata
	//WORD wSchool = CTargetInfoDisplay::NODATA;
	PGLCROWCLIENT pCrow = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( sTargetID.dwID );


	GLCOPY *pCOPY = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pCOPY ) return;

	strName = pCOPY->GetName();
	sHP = pCOPY->GetHp();
	
	if ( pCOPY->GetCrow() == CROW_MOB || pCOPY->GetCrow() == CROW_SUMMON )
	{
		dwColor = GLGaeaClient::GetInstance().GetMobNameColor ( sTargetID.dwID );
		int fPercent2 =  (float(sHP.dwNow) / float(sHP.dwMax)) * 100;
		if ( m_pCrowTargetInfo )	m_pCrowTargetInfo->SetTargetInfo ( sHP.dwNow, sHP.dwMax, fPercent2, pCrow->GETLEVEL(), strName);
		ShowGroupBottom ( CROW_TARGET_INFO );
	}
	else if ( pCOPY->GetCrow () == CROW_PC )
	{
		HideGroup ( CROW_TARGET_INFO );
		bool bBRIGHTEVENT   = GLGaeaClient::GetInstance().IsBRIGHTEVENT();
		bool bSCHOOL_FREEPK = GLGaeaClient::GetInstance().IsSchoolFreePk();
		bool bGuidBattleMap = false;

		//juver battle royale
		bool bRedZone = false;
	
		PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
		if ( pLand )
		{
			bGuidBattleMap = ( pLand->m_bClubBattle || pLand->m_bClubDeathMatch );

			bRedZone = ( pLand->IsRedZone() );
		}

		//	��� �Ǵ� PK�� �ƴϸ�, �Ǹ� ǥ������ �ʴ´�.
		if ( !GLGaeaClient::GetInstance().GetCharacter()->ISCONFRONT_TAR ( sTargetID ) &&
			!GLGaeaClient::GetInstance().GetCharacter()->IS_PLAYHOSTILE ( pCOPY->GetCharID() ) )
		{
			if ( !(bBRIGHTEVENT || bSCHOOL_FREEPK || bGuidBattleMap) )
			{
				//	��ǥ�ø� ���� ( �ƽø� )
				sHP.TO_FULL ();
			}
			if ( !(bBRIGHTEVENT || bSCHOOL_FREEPK || bRedZone) )
			{
				sHP.TO_FULL ();
			}
		}

		//	�̸� �� (PK_COLOR)
		PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTargetID.dwID );
		if ( !pCHAR ) return ;
		dwColor = pCHAR->GET_PK_COLOR ();
		CString strLevel, strCombine, strReborn, strParty, strGuild, strPartyJoined, strPartyM, strPartyNew, strPartyMember;
		//strHp.Format ( "%d", pCHAR->GetHp());
		//strReborn.Format ( "%d", pCHAR->GetReborn());
		strCombine.Format("%s",COMMENT::CHARCLASS[CharClassToIndex(pCHAR->GetClass())].c_str());
		strPartyJoined = "Already joined a Party";//New Target Info for Party by CNDev
		strPartyMember = "PartyMember";
		strPartyM = "PartyMaster";
		//strGuildMaster = "GuildMaster";
		if(pCHAR->IsPartyMem() == true){
		strParty.Format ( "%s", strPartyJoined );
		}else{
		strParty.Format ( "None");
		}
		if(pCHAR->IsPartyMaster() == true){
		strPartyNew.Format ( "%s", strPartyM );
		}else if(pCHAR->IsPartyMem() == true){
			strPartyNew.Format ( "%s",strPartyMember );
		}
		if(pCHAR->GetClubName() != ""){
		strGuild.Format ( "%s", pCHAR->GetClubName());
		}else{
			strGuild.Format ( "None");
		}
		int fPercent3 =  (float(sHP.dwNow) / float(sHP.dwMax)) * 100;
		if ( m_pCrowTargetInfoPlayer ) m_pCrowTargetInfoPlayer->SetTargetInfo ( sHP.dwNow, sHP.dwMax, fPercent3,pCHAR->GetSchool(), pCHAR->GetReborn(), strCombine, pCHAR->GetLevel(), strPartyMember, strPartyM, strPartyNew, strPartyJoined, strParty, strGuild, strName );
		ShowGroupBottom ( CROW_TARGET_INFO_PLAYER );
	}

	float fPercent = float(sHP.dwNow) / float(sHP.dwMax);
	if ( m_pTargetInfoDisplay ) m_pTargetInfoDisplay->SetTargetInfo ( fPercent, strName, dwColor );

	//	NOTE
	//		������ ����
	{
		// Note : �̸�ǥ�� ��������
		D3DXVECTOR3 vPos = pCOPY->GetPosBodyHeight();
		D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

		const UIRECT& rcOriginPos = m_pTargetInfoDisplay->GetGlobalPos ();
		int nPosX = (int)( floor(vScreen.x - ( rcOriginPos.sizeX * 0.5f )));
		int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );
		if ( m_pTargetInfoDisplay )
		{
			//if ( abs ( rcOriginPos.left - nPosX ) > 1 || abs ( rcOriginPos.top - nPosY ) > 1 )
				m_pTargetInfoDisplay->SetGlobalPos ( D3DXVECTOR2 ( float ( nPosX ), float ( nPosY ) ) );
		}
		ShowGroupBottom ( TARGETINFO_DISPLAY );

		//	�ߺ� �̸� �����
		if ( m_pNameDisplayMan )	m_pNameDisplayMan->VisibleTargetInfoCtrl ( sTargetID.emCrow, sTargetID.dwID, FALSE );
	}
}
void CInnerInterface::SetTargetInfoNpc ( STARGETID sTargetID ) //Target Info for NPC
{
	CString strName;
	D3DCOLOR dwColor = NS_UITEXTCOLOR::DEFAULT;
	GLPWDATA sHP; 
		PGLCROWCLIENT pCrow = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( sTargetID.dwID );


	GLCOPY *pCOPY = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pCOPY ) return;

	strName = pCOPY->GetName();
	sHP = pCOPY->GetHp();
	
	if ( pCOPY->GetCrow() == CROW_NPC )
	{
		dwColor = GLGaeaClient::GetInstance().GetMobNameColor ( sTargetID.dwID );
	}


	float fPercent = float(sHP.dwNow) / float(sHP.dwMax);
	if ( m_pCrowTargetInfoNpc )	m_pCrowTargetInfoNpc->SetTargetInfo ( strName );
	if ( m_pTargetInfoDisplayNpc ) m_pTargetInfoDisplayNpc->SetTargetInfoNpc ( fPercent, strName, dwColor );

	//	NOTE
	//		??? ??
	{
		// Note : ???? ????
		D3DXVECTOR3 vPos = pCOPY->GetPosBodyHeight();
		D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

		const UIRECT& rcOriginPos = m_pTargetInfoDisplayNpc->GetGlobalPos ();
		int nPosX = (int)( floor(vScreen.x - ( rcOriginPos.sizeX * 0.5f )));
		int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );
		if ( m_pTargetInfoDisplayNpc )
		{
			//if ( abs ( rcOriginPos.left - nPosX ) > 1 || abs ( rcOriginPos.top - nPosY ) > 1 )
				m_pTargetInfoDisplayNpc->SetGlobalPos ( D3DXVECTOR2 ( float ( nPosX ), float ( nPosY ) ) );
		}
		ShowGroupBottom ( TARGETINFO_DISPLAY_NPC );
		ShowGroupBottom ( CROW_TARGET_INFO_NPC );

		//	?? ?? ???
		if ( m_pNameDisplayMan )	m_pNameDisplayMan->VisibleTargetInfoCtrl ( sTargetID.emCrow, sTargetID.dwID, FALSE );
	}
}

void CInnerInterface::SetTargetInfoPlayer ( STARGETID sTargetID ) //Target Info for PC
{
	CString strName;
	D3DCOLOR dwColor = NS_UITEXTCOLOR::DEFAULT;
	GLPWDATA sHP; 

	PGLCROWCLIENT pCrow = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( sTargetID.dwID );
	PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTargetID.dwID );
	CString strLevel, strCombine, /*strReborn,*/ strParty, strPartyJoined, strGuild, strPartyM, strPartyNew, strPartyMember;
	//strHp.Format ( "%d", pCHAR->GetHp());
	//strReborn.Format ( "%d", pCHAR->GetReborn());
	strCombine.Format("%s",COMMENT::CHARCLASS[CharClassToIndex(pCHAR->GetClass())].c_str());
	strPartyJoined = "Already joined a Party";//New Target Info for Party by CNDev
		strPartyMember = "PartyMember";
		strPartyM = "PartyMaster";
//		strGuildMaster = "GuildMaster";
		if(pCHAR->IsPartyMem() == true){
		strParty.Format ( "%s", strPartyJoined );
		}else{
		strParty.Format ( "None");
		}
		if(pCHAR->IsPartyMaster() == true){
		strPartyNew.Format ( "%s", strPartyM );
		}else if(pCHAR->IsPartyMem() == true){
			strPartyNew.Format ( "%s",strPartyMember );
		}
		if(pCHAR->GetClubName() != ""){
		strGuild.Format ( "%s", pCHAR->GetClubName());
		}else{
			strGuild.Format ( "None");
		}
	GLCOPY *pCOPY = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pCOPY ) return;

	strName = pCOPY->GetName();
	sHP = pCOPY->GetHp();
	
	if ( pCOPY->GetCrow () == CROW_PC )
	{
		bool bBRIGHTEVENT   = GLGaeaClient::GetInstance().IsBRIGHTEVENT();
		bool bSCHOOL_FREEPK = GLGaeaClient::GetInstance().IsSchoolFreePk();
		bool bGuidBattleMap = false;

		//juver battle royale
		bool bRedZone = false;
	
		PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
		if ( pLand )
		{
			bGuidBattleMap = ( pLand->m_bClubBattle || pLand->m_bClubDeathMatch );

			bRedZone = ( pLand->IsRedZone() );
		}

		//	?? ?? PK? ???, ?? ???? ???.
		if ( !GLGaeaClient::GetInstance().GetCharacter()->ISCONFRONT_TAR ( sTargetID ) &&
			!GLGaeaClient::GetInstance().GetCharacter()->IS_PLAYHOSTILE ( pCOPY->GetCharID() ) )
		{
			if ( !(bBRIGHTEVENT || bSCHOOL_FREEPK || bGuidBattleMap) )
			{
				//	???? ?? ( ??? )
				sHP.TO_FULL ();
			}
			if ( !(bBRIGHTEVENT || bSCHOOL_FREEPK || bRedZone) )
			{
				sHP.TO_FULL ();
			}
		}

		//	?? ? (PK_COLOR)
		if ( !pCHAR ) return ;
		dwColor = pCHAR->GET_PK_COLOR ();
	}


	float fPercent = float(sHP.dwNow) / float(sHP.dwMax);
	int fPercent3 =  (float(sHP.dwNow) / float(sHP.dwMax)) * 100;
	if ( m_pCrowTargetInfoPlayer ) m_pCrowTargetInfoPlayer->SetTargetInfo ( sHP.dwNow, sHP.dwMax, fPercent3,pCHAR->GetSchool(), pCHAR->GetReborn(), strCombine, pCHAR->GetLevel(),strPartyMember, strPartyM, strPartyNew, strPartyJoined, strParty, strGuild, strName );
	if ( m_pTargetInfoDisplayPlayer ) m_pTargetInfoDisplayPlayer->SetTargetInfoPlayer ( fPercent, strName, dwColor );

	//	NOTE
	//		??? ??
	{
		// Note : ???? ????
		D3DXVECTOR3 vPos = pCOPY->GetPosBodyHeight();
		D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

		const UIRECT& rcOriginPos = m_pTargetInfoDisplayPlayer->GetGlobalPos ();
		int nPosX = (int)( floor(vScreen.x - ( rcOriginPos.sizeX * 0.5f )));
		int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );
		if ( m_pTargetInfoDisplayPlayer )
		{
			//if ( abs ( rcOriginPos.left - nPosX ) > 1 || abs ( rcOriginPos.top - nPosY ) > 1 )
				m_pTargetInfoDisplayPlayer->SetGlobalPos ( D3DXVECTOR2 ( float ( nPosX ), float ( nPosY ) ) );
		}
		ShowGroupBottom ( TARGETINFO_DISPLAY_PLAYER );
		ShowGroupBottom ( CROW_TARGET_INFO_PLAYER );

		//	?? ?? ???
		if ( m_pNameDisplayMan )	m_pNameDisplayMan->VisibleTargetInfoCtrl ( sTargetID.emCrow, sTargetID.dwID, FALSE );
	}
}
void CInnerInterface::SetTargetInfoCrow ( STARGETID sTargetID ) //Crow Dispaly Info for Mobs
{
	CString strName;
	GLPWDATA sHP;

	bool bOk(FALSE);
	GLCOPY *pCOPY = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pCOPY ) return;

	strName = pCOPY->GetName();
	sHP = pCOPY->GetHp();

	if ( pCOPY->GetCrow() == CROW_MOB || pCOPY->GetCrow() == CROW_SUMMON || pCOPY->GetCrow() == CROW_NPC )
	{
		PGLCROWCLIENT pCrow = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( sTargetID.dwID );
		if ( !pCrow ) return;
		
		int fPercent2 =  (float(sHP.dwNow) / float(sHP.dwMax)) * 100;
		if ( m_pCrowTargetInfo )	m_pCrowTargetInfo->SetTargetInfo ( sHP.dwNow, sHP.dwMax, fPercent2, pCrow->GETLEVEL(), strName);

		bOk = TRUE;
	}

	if ( bOk ) ShowGroupTop ( CROW_TARGET_INFO );
}
void CInnerInterface::SetTargetInfoCrowNpc ( STARGETID sTargetID ) //Crow Dispaly Info for Npc
{
	CString strName;

	bool bOk(FALSE);
	GLCOPY *pCOPY = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pCOPY ) return;

	strName = pCOPY->GetName();

	if ( pCOPY->GetCrow() == CROW_MOB || pCOPY->GetCrow() == CROW_SUMMON || pCOPY->GetCrow() == CROW_NPC )
	{
		PGLCROWCLIENT pCrow = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( sTargetID.dwID );
		if ( !pCrow ) return;
		
		if ( m_pCrowTargetInfoNpc )	m_pCrowTargetInfoNpc->SetTargetInfo ( strName );

		bOk = TRUE;
	}

	if ( bOk ) ShowGroupTop ( CROW_TARGET_INFO_NPC );
}
void CInnerInterface::SetTargetInfoCrowPlayer ( STARGETID sTargetID ) //Crow Dispaly Info for PC
{
	CString strName;
	GLPWDATA sHP; 
	PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTargetID.dwID );
	CString strLevel, strCombine, /*strReborn,*/ strPartyJoined, strParty, strGuild, strPartyM, strPartyNew,strPartyMember;
	strLevel.Format ( "%d", pCHAR->GetLevel());
//	strReborn.Format ( "%d", pCHAR->GetReborn());
	//strHp.format ( "%d", pCHAR->GetHp());
	strCombine.Format("%s",COMMENT::CHARCLASS[CharClassToIndex(pCHAR->GetClass())].c_str());
	strPartyJoined = "Already joined a Party";//New Target Info for Party by CNDev
	strPartyMember = "PartyMember";
		strPartyM = "PartyMaster";
//		strGuildMaster = "GuildMaster";
		if(pCHAR->IsPartyMem() == true){
		strParty.Format ( "%s", strPartyJoined );
		}else{
		strParty.Format ( "None");
		}
		if(pCHAR->IsPartyMaster() == true){
		strPartyNew.Format ( "%s", strPartyM );
		}else if(pCHAR->IsPartyMem() == true){
			strPartyNew.Format ( "%s",strPartyMember );
		}
		if(pCHAR->GetClubName() != ""){
		strGuild.Format ( "%s", pCHAR->GetClubName());
		}else{
			strGuild.Format ( "None");
		}

	bool bOk(FALSE);
	GLCOPY *pCOPY = GLGaeaClient::GetInstance().GetCopyActor ( sTargetID );
	if ( !pCOPY ) return;

	strName = pCOPY->GetName();

	if ( pCOPY->GetCrow() == CROW_PC )
	{
		PGLCROWCLIENT pCrow = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( sTargetID.dwID );
		if ( !pCrow ) return;
	int fPercent3 =  (float(sHP.dwNow) / float(sHP.dwMax)) * 100;		
	if ( m_pCrowTargetInfoPlayer ) m_pCrowTargetInfoPlayer->SetTargetInfo ( sHP.dwNow, sHP.dwMax, fPercent3,pCHAR->GetSchool(), pCHAR->GetReborn(), strCombine, pCHAR->GetLevel(),strPartyMember, strPartyM, strPartyNew, strPartyJoined, strParty, strGuild, strName );

		bOk = TRUE;
	}

	if ( bOk ) ShowGroupTop ( CROW_TARGET_INFO_PLAYER );
}

void CInnerInterface::SetMarketWindowOpen ( SNATIVEID sNativeID ) 
{
	if ( m_pMarketWindow )
	{
		m_pMarketWindow->InitMarket ( sNativeID );
		ShowGroupFocus ( MARKET_WINDOW );
	}
}

void CInnerInterface::SetStorageWindowOpen ( DWORD dwNPCID = 0 )
{
	if ( m_pStorageWindow )
	{
		m_pStorageWindow->InitStorage ( dwNPCID );
		ShowGroupFocus ( STORAGE_WINDOW );
	}
}

void CInnerInterface::SetBusWindowOpen ( const DWORD dwGaeaID, SNpcTalk* pNpcTalk )
{
	if ( m_pBusWindow )
	{
		if ( !m_pBusWindow->SetBusData ( dwGaeaID, pNpcTalk ) )
		{
			return ;
		}

		ShowGroupFocus ( BUS_WINDOW, true );
	}
}

void CInnerInterface::SetDialogueWindowOpen ( STARGETID sCrowID, GLCHARLOGIC* pCHAR )
{
	if ( m_pDialogueWindow )
	{
		//	TO DO :
		//		������ �߰� �Ǵ���, �� �Լ��� ��ġ�� �ȴ�.
		//		����, ���� �뼱�� ǥ���ؾ��ϴ� ������,		
		//		��ȭ�� ǥ���ؾ��ϴ� �������� ���⿡�� �����ؾ��Ѵ�.

		//	NOTE :
		//		sCrowID�� '����'����, '�Ϲ� NPC'���� Ȯ���� �� �ִ�.
		if ( !m_pDialogueWindow->SetDialogueData ( sCrowID, pCHAR ) )
		{
//			GASSERT ( 0 && "����! npc���� �б� ����!" );
			return;
		}
		ShowGroupFocus ( DIALOGUE_WINDOW );
	}
}

BOOL CInnerInterface::PrintMsgText ( D3DCOLOR dwColor, const char* szFormat, ... )
{	
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		GetChat()->AddStringToSystemMessage ( szBuffer, dwColor );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CInnerInterface::PrintConsoleText ( const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		GetChat()->AddStringToChatEx ( szBuffer, CHAT_SYSTEM );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL CInnerInterface::PrintMsgTextDlg ( D3DCOLOR dwColor, const char* szFormat, ... )
{	
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		GetChat()->AddStringToSystemMessage ( szBuffer, dwColor );
		m_pSystemMessageWindow->SetSystemMessage ( szBuffer, dwColor );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CInnerInterface::PrintConsoleTextDlg ( const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	if ( GetChat() )
	{
		GetChat()->AddStringToChatEx ( szBuffer, CHAT_SYSTEM );
		m_pSystemMessageWindow->SetSystemMessage ( szBuffer, NS_UITEXTCOLOR::HOTPINK );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CInnerInterface::PrintMsgDlg ( D3DCOLOR dwColor, const char* szFormat, ... )
{
	GASSERT( szFormat );

	const int MAX_STRLENGTH = 512;

	va_list argList;
	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	GASSERT( m_pSystemMessageWindow );
	if( m_pSystemMessageWindow )
	{
		m_pSystemMessageWindow->SetSystemMessage( szBuffer, dwColor );
		return TRUE;
	}
	else return FALSE;
}

void CInnerInterface::SetTradeWindowClose ()
{
	HideGroup ( TRADE_WINDOW );
	HideGroup ( TRADEINVENTORY_WINDOW );
}

void	CInnerInterface::SetTradeWindowCloseReq ()
{
	//	��� ��ư ����
	//
	GLMSG::SNET_TRADE_CANCEL NetMsg;
	NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
}

void	CInnerInterface::SetFightBegin ( const int nIndex )
{
	GetConftDisplayMan()->SetFightBegin ( nIndex );
	ShowGroupTop ( CONFT_DISPLAY_MAN );
}

void	CInnerInterface::SetFightEnd ( const int nResult )
{
	GetConftDisplayMan()->SetFightEnd ( nResult );
	ShowGroupTop ( CONFT_DISPLAY_MAN );
}

void	CInnerInterface::SetStorageWindowClose ()
{
	HideGroup ( STORAGE_WINDOW );
	HideGroup ( INVENTORY_WINDOW );
}

void	CInnerInterface::SetClubStorageWindowOpen ()
{
	if ( m_pClubStorageWindow )
	{
		m_pClubStorageWindow->InitStorage ();
		ShowGroupFocus ( CLUB_STORAGE_WINDOW );
	}
}

void	CInnerInterface::SetClubStorageWindowClose ()
{
	HideGroup ( CLUB_STORAGE_WINDOW );
	HideGroup ( INVENTORY_WINDOW );
}

void	CInnerInterface::SetMarketWindowClose ()
{
	HideGroup ( MARKET_WINDOW );
	HideGroup ( INVENTORY_WINDOW );
}

void CInnerInterface::CLEAR_TEXT ()
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
		m_pInfoDisplayEx->ClearText ();
	}
	else
	{
		m_pInfoDisplay->ClearText ();	
	}
}

void CInnerInterface::RESET_INFO ()
{
    NS_SKILLINFO::RESET ();
	NS_ITEMINFO::RESET ();
	m_strGeneralInfoBack.Empty ();
	if( m_pInfoDisplay ) m_pInfoDisplay->ClearText ();
	m_sItemInfoTemp = SITEMCUSTOM();
}

void CInnerInterface::ADDTEXT_NOSPLIT ( const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
		m_pInfoDisplayEx->AddTextNoSplit ( strText, dwColor );
	}
	else
	{
		m_pInfoDisplay->AddTextNoSplit ( strText, dwColor );
	}
}
//ItemImageRender by BlackCatYB
void CInnerInterface::ADDTEXT_NOSPLIT2( SNATIVEID sICONINDEX, const char* szTexture )
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
		m_pInfoDisplayEx->ResetSkill ();
		m_pInfoDisplayEx->SetItem ( sICONINDEX, szTexture );
	}
	else
	{
		m_pInfoDisplay->ResetSkill ();
		m_pInfoDisplay->SetItem ( sICONINDEX, szTexture );
	}
}
//ItemImageRender by BlackCatYB
void CInnerInterface::ADDTEXT_NOSPLIT3()
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
		m_pInfoDisplayEx->ResetItem ();
	}
	else
	{
		m_pInfoDisplay->ResetItem ();
	}
}
//SkillImageRender by BlackCatYB
void CInnerInterface::ADDTEXT_NOSPLIT4( SNATIVEID sNativeID )
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
		m_pInfoDisplayEx->ResetItem ();
//		m_pInfoDisplayEx->ResetIcon ();
		m_pInfoDisplayEx->SetSkill ( sNativeID );
	}
	else
	{
		m_pInfoDisplay->ResetItem ();
//		m_pInfoDisplay->ResetIcon ();
		m_pInfoDisplay->SetSkill ( sNativeID );
	}
}
//SkillImageRender by BlackCatYB
void CInnerInterface::ADDTEXT_NOSPLIT5()
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
		m_pInfoDisplayEx->ResetSkill ();
	}
	else
	{
		m_pInfoDisplay->ResetSkill ();
	}
}/*
//IconResistant by BlackCatYB
void CInnerInterface::ADDTEXT_NOSPLIT6()
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
		m_pInfoDisplayEx->ResetSkill ();
		m_pInfoDisplayEx->SetIcon ();
	}
	else
	{
		m_pInfoDisplay->ResetSkill ();
		m_pInfoDisplay->SetIcon ();
	}
}
//IconResistant by BlackCatYB
void CInnerInterface::ADDTEXT_NOSPLIT7()
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
//		m_pInfoDisplayEx->ResetIcon ();
	}
	else
	{
		m_pInfoDisplay->ResetIcon ();
	}
}*/

void CInnerInterface::ADDTEXT_LONGESTLINE_SPLIT ( const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_bITEM_INFO_EX_DISPLAY_MODE )
	{
		m_pInfoDisplayEx->AddTextLongestLineSplit ( strText, dwColor );
	}
	else
	{
		m_pInfoDisplay->AddTextLongestLineSplit ( strText, dwColor );
	}
}

void CInnerInterface::SHOW_ITEM_INFO ( SITEMCUSTOM &sItemCustom, BOOL bShopOpen, BOOL bInMarket, BOOL bInPrivateMarket, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID /* = SNATIVEID */ )
{	
	if ( IsMouseInControl () ) return ;

	//	NOTE
	//		�̹� �Ѹ��� �ִٴ� ����
	//		�ٸ� â���� ���� ����Ÿ�� ���� �ִٴ� �̾߱��̴�.
	if ( IsVisibleGroup ( INFO_DISPLAY ) ) return ;
	if ( m_etInfoType != ET_ITEM_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_ITEM_INFO;
	}

	NS_ITEMINFO::LOAD ( sItemCustom, bShopOpen, bInMarket, bInPrivateMarket, FALSE, wPosX, wPosY, sNpcNativeID );
	m_sItemInfoTemp = sItemCustom;
	ShowGroupTop ( INFO_DISPLAY );

	m_bUSING_INFO_DISPLAY = true;

	//	�� ���� ( �ڱⰡ ���� �����ϰ� �ִ� �����۰� ���콺 ������ �������� ������� )
	BOOL bTRADE = IsVisibleGroup ( TRADE_WINDOW );
	if ( bTRADE || bInMarket || bInPrivateMarket )
	{
		for( int nSLOT = SLOT_HEADGEAR; nSLOT < SLOT_NSIZE_S_2; ++nSLOT )
		{			
			if ( GLGaeaClient::GetInstance().GetCharacter()->CHECKSLOT_ITEM ( sItemCustom.sNativeID, (EMSLOT) nSLOT ) )
			{
				SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_SLOT_ITEM ( (EMSLOT) nSLOT );
				if ( sItemCustom.sNativeID == NATIVEID_NULL () ) return ;

				m_bITEM_INFO_EX_DISPLAY_MODE = true;
				{
					//	ITEM ���� �̱�, ������ -1�� �ǹ̾��� ����
					//
					NS_ITEMINFO::LOAD ( sItemCustom, FALSE, FALSE, FALSE, TRUE, wPosX, wPosY, sNpcNativeID );
				}
				m_bITEM_INFO_EX_DISPLAY_MODE = false;

				ShowGroupTop ( INFO_DISPLAY_EX );
			}
		}		
	}
}

void CInnerInterface::SHOW_SKILL_INFO ( SNATIVEID sNativeID, const BOOL bNextLevel )
{
	if ( IsMouseInControl () ) return ;

	if ( IsVisibleGroup ( INFO_DISPLAY ) ) return ;
	if ( m_etInfoType != ET_SKILL_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_SKILL_INFO;
	}

	NS_SKILLINFO::LOAD ( sNativeID, bNextLevel );
	ShowGroupTop ( INFO_DISPLAY );

	m_bUSING_INFO_DISPLAY = true;
}

bool CInnerInterface::BEGIN_COMMON_LINEINFO_MULTI ()
{
	if ( IsMouseInControl () ) return false;
	if ( IsVisibleGroup ( INFO_DISPLAY ) ) return false;
	if ( m_etInfoType != ET_GENERAL_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_GENERAL_INFO;
	}

	m_strGeneralInfoBack.Empty ();
	m_pInfoDisplay->ClearText ();

	m_bUSING_INFO_DISPLAY = true;

	return true;
}

bool CInnerInterface::ADD_COMMON_LINEINFO_MULTI ( const CString& strText, D3DCOLOR dwColor )
{
	if ( IsVisibleGroup ( INFO_DISPLAY ) ) return false;

	m_pInfoDisplay->AddTextNoSplit ( strText, dwColor );

	return true;
}

void CInnerInterface::END_COMMON_LINEINFO_MULTI ()
{
	ShowGroupTop ( INFO_DISPLAY );
}

void CInnerInterface::SHOW_COMMON_LINEINFO ( const CString& strText, D3DCOLOR dwColor )
{
	if ( IsMouseInControl () ) return ;
	if ( IsVisibleGroup ( INFO_DISPLAY ) ) return ;		
	if ( m_etInfoType != ET_GENERAL_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_GENERAL_INFO;
	}

	if ( !strText.GetLength() ) return ;

	if ( m_strGeneralInfoBack != strText )
	{
		m_pInfoDisplay->ClearText ();
		m_pInfoDisplay->AddTextNoSplit ( strText, dwColor );
		m_pInfoDisplay->ResetItem ();
		m_pInfoDisplay->ResetSkill ();
	//	m_pInfoDisplay->ResetIcon ();
		m_strGeneralInfoBack = strText;
	}

	ShowGroupTop ( INFO_DISPLAY );

	m_bUSING_INFO_DISPLAY = true;
}

BOOL CInnerInterface::IsPartyMember ( const char* szName )
{
	GLPARTY_CLIENT* pMaster = GLPartyClient::GetInstance().GetMaster();
	if ( pMaster )
	{
		//	�̸� ��

		//	NOTE
		//		�������̸�
		if ( !strcmp ( pMaster->m_szName, szName ) )
		{
			return TRUE;
		}
		else
		{
			//	NOTE
			//		����̸�
			DWORD nMEMBER_NUM = GLPartyClient::GetInstance().GetMemberNum();
			if( nMEMBER_NUM > 1 )
			{
				nMEMBER_NUM -= 1; // Note : �����ʹ� ����
				for ( DWORD i = 0; i < nMEMBER_NUM; i++ )
				{
					GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember ( i );			
					if ( pMember )
					{
						if ( !strcmp ( pMember->m_szName, szName ) )
						{						
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

void CInnerInterface::ClearNameList ()
{
	if ( m_pNameDisplayMan ) m_pNameDisplayMan->ClearList ();
}

void CInnerInterface::SetAcademyFightBegin ( const int& nLMARK, const int& nLNUMBER, const int& nRMARK, const int& nRNUMBER )
{
	m_pAcademyConftDisplay->SET_MARK_L ( nLMARK );
	m_pAcademyConftDisplay->SET_MARK_R ( nRMARK );
	m_pAcademyConftDisplay->SET_NUMBER_L ( nLNUMBER );
	m_pAcademyConftDisplay->SET_NUMBER_R ( nRNUMBER );
	m_pAcademyConftDisplay->UPDATE_ALPHA ( USHRT_MAX );

	ShowGroupTop ( ACADEMY_CONFT_DISPLAY, true );

	m_bACADEMY_FIGHT_READY = true;
}

void CInnerInterface::SetAcademyUpdateNumber ( const int& nLNUMBER, const int& nRNUMBER )
{
	const int nLNUMBER_BACK = m_pAcademyConftDisplay->GET_NUMBER_L ();
	const int nRNUMBER_BACK = m_pAcademyConftDisplay->GET_NUMBER_R ();
	bool bUPDATE = false;

	if ( nLNUMBER_BACK != nLNUMBER )
	{
		m_pAcademyConftDisplay->SET_NUMBER_L ( nLNUMBER );
		bUPDATE = true;
	}
	if ( nRNUMBER_BACK != nRNUMBER )
	{
		m_pAcademyConftDisplay->SET_NUMBER_R ( nRNUMBER );
		bUPDATE = true;
	}

	if ( bUPDATE || m_bACADEMY_FIGHT_READY ) m_pAcademyConftDisplay->UPDATED_NUMBER ();

	m_bACADEMY_FIGHT_READY = false;
}

void CInnerInterface::SetAcademyFightEnd ()
{
	HideGroup ( ACADEMY_CONFT_DISPLAY, true );
}

void CInnerInterface::SetQuestWindowOpen ( DWORD dwQuestID )
{
	if ( NATIVEID_NULL().dwID != m_dwEventQuestID )
	{
		GLGaeaClient::GetInstance().GetCharacter()->ReqQuestREADINGReset ( m_dwEventQuestID );
		m_dwEventQuestID = NATIVEID_NULL().dwID;
	}

	ShowGroupFocus ( QUEST_WINDOW );
	m_pQuestWindow->AUTOSELECT_PAGE ( dwQuestID );	
}

void CInnerInterface::REFRESH_QUEST_WINDOW ()
{
	if ( IsVisibleGroup ( QUEST_WINDOW ) )m_pQuestWindow->REFRESH_QUEST_WINDOW ();	
}

void CInnerInterface::REFRESH_CLUB_LIST ()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) 
	{
		m_pClubWindow->LoadClubMemberList ();
	}
}

void CInnerInterface::REFRESH_CLUB_ALLIANCE_LIST()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) m_pClubWindow->LoadClubAllianceList ();
}

void CInnerInterface::REFRESH_CLUB_BATTLE_LIST()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) m_pClubWindow->LoadClubBattleList ();
}

void CInnerInterface::REFRESH_CLUB_STATE ()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) m_pClubWindow->RefreshState ();
}

void CInnerInterface::REFRESH_CLUB_NOTICE()
{
	if ( IsVisibleGroup ( CLUB_WINDOW ) ) m_pClubWindow->SetAnnouncement();
}

void CInnerInterface::REFRESH_ATTENDBOOK()
{
	if ( m_pAttendanceBookWindow ) m_pAttendanceBookWindow->RefreshAttend();
}

void CInnerInterface::SHOW_ITEM_INFO_SIMPLE ( SITEMCUSTOM &sItemCustom )
{
	if ( IsMouseInControl () ) return ;

	//	NOTE
	//		�̹� �Ѹ��� �ִٴ� ����
	//		�ٸ� â���� ���� ����Ÿ�� ���� �ִٴ� �̾߱��̴�.
	if ( IsVisibleGroup ( INFO_DISPLAY ) ) return ;
	if ( m_etInfoType != ET_ITEM_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_ITEM_INFO;
	}

	NS_ITEMINFO::LOAD_SIMPLE ( sItemCustom );
	m_sItemInfoTemp = sItemCustom;
	ShowGroupTop ( INFO_DISPLAY );

	m_bUSING_INFO_DISPLAY = true;
}

void	CInnerInterface::UpdateStateQuestAlarm ()
{
	if ( !IsVisibleGroup ( QUEST_WINDOW ) )	//	����Ʈâ�� �ݱ� ����
	{
		GLQuestPlay& cQuestPlay = GLGaeaClient::GetInstance().GetCharacter()->m_cQuestPlay;
		if ( !cQuestPlay.GetQuestProc ().empty () )	//	�������� ����Ʈ�� �����Ѵ�.
		{
			if ( !IsVisibleGroup ( QUEST_ALARM ) )
			{
				ShowGroupBottom ( QUEST_ALARM );	//	����Ʈ ������ ǥ��
			}
			
			DWORD dwQuestID = NATIVEID_NULL().dwID;			
			if ( cQuestPlay.GetReqREADING ( dwQuestID ) )	//	����Ʈ �̺�Ʈ �߻�
			{
				GLQUESTPROG* pQuestProg = cQuestPlay.FindProc ( dwQuestID );
				if ( !pQuestProg ) return ;				
				const DWORD dwEventQuestStep = pQuestProg->m_dwSTEP;

				if ( m_dwEventQuestID != dwQuestID || dwEventQuestStep != m_dwEventQuestStep )
				{
					DxSoundLib::GetInstance()->PlaySound ( "QuestAlarm" );
					m_dwEventQuestID = dwQuestID;
					m_dwEventQuestStep = dwEventQuestStep;
					
					RESET_QUEST_HELPER( dwQuestID );
				}

				m_pQuestAlarm->SET_ALARM ();
//				m_pQuestWindow->SetQuestID ( m_dwEventQuestID );

				return ;
			}
		}
		else
		{
			HideGroup ( QUEST_ALARM );
		}
	}
	else
	{
		GLQuestPlay& cQuestPlay = GLGaeaClient::GetInstance().GetCharacter()->m_cQuestPlay;
		if ( !cQuestPlay.GetQuestProc ().empty () )	//	�������� ����Ʈ�� �����Ѵ�.
		{
			DWORD dwQuestID = NATIVEID_NULL().dwID;
			if ( cQuestPlay.GetReqREADING ( dwQuestID ) )	//	����Ʈ �̺�Ʈ �߻�
			{
				GLQUESTPROG* pQuestProg = cQuestPlay.FindProc ( dwQuestID );
				if ( !pQuestProg ) return ;				
				const DWORD dwEventQuestStep = pQuestProg->m_dwSTEP;

				if ( m_dwEventQuestID != dwQuestID || dwEventQuestStep != m_dwEventQuestStep )
				{
					DxSoundLib::GetInstance()->PlaySound ( "QuestAlarm" );
					m_dwEventQuestID = dwQuestID;
					m_dwEventQuestStep = dwEventQuestStep;

					RESET_QUEST_HELPER( dwQuestID );
				}

				//	����Ʈ �������� �ƴϰ�.
				if ( CQuestWindow::QUEST_LIST != m_pQuestWindow->GET_SHOW_PAGE () )
				{
					//	�̺�Ʈ �߻��� ID�� ���� ���� �ִ� ����Ʈ�� ID�� �ٸ����
					if ( m_dwEventQuestID != m_pQuestWindow->GetQuestID () )
					{
						m_pQuestAlarm->SET_ALARM ();
//						m_pQuestWindow->SetQuestID ( m_dwEventQuestID );
						
						if ( !IsVisibleGroup ( QUEST_ALARM ) )
						{
							ShowGroupBottom ( QUEST_ALARM );	//	����Ʈ ������ ǥ��
						}

						return ;
					}
				}

				GLGaeaClient::GetInstance().GetCharacter()->ReqQuestREADINGReset ( m_dwEventQuestID );					
				m_dwEventQuestID = NATIVEID_NULL().dwID;				
			}
		}

		HideGroup ( QUEST_ALARM );
	}

//#ifdef CH_PARAM // �߱� �������̽� ����
//	UpdatePotionTrayPosition();
//#endif
}

void CInnerInterface::DOMODAL_ADD_FRIEND_REQ ( const CString& strName )
{
	if ( IsVisibleGroup ( MODAL_WINDOW ) )
	{
		return ;
	}

	m_strFriendName = strName;
	CString strCombine = MakeString ( ID2GAMEINTEXT("ADD_FRIEND_REQ"), strName );	
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_ADD_FRIEND_LURE_ANS );
}

void CInnerInterface::REFRESH_FRIEND_LIST ()
{
	if ( !IsVisibleGroup ( FRIEND_WINDOW ) ) return ;

	//	��� ���� ( �߰�, ����... �׿� )
	m_pFriendWindow->LoadFriendList ();
	m_pChat->REFRESH_FRIEND_LIST ();
}

void CInnerInterface::REFRESH_FRIEND_STATE()
{
	if ( !IsVisibleGroup ( FRIEND_WINDOW ) ) return ;

	//	���� ���� ( ��/�������� )
	m_pFriendWindow->LoadFriendList ();
}

void CInnerInterface::OPEN_MODAL ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID, BOOL bHide )
{
	const int nMODAL_NEWID = CModalWindow::MAKE_MODAL_NEWID () + MODAL_WINDOW;	

	CModalWindow* pModalWindow = new CModalWindow;
	pModalWindow->Create ( nMODAL_NEWID, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
	pModalWindow->CreateSubControl ();
	pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
	pModalWindow->DoModal ( strText, nModalTitle, nModalType, CallerID );
	pModalWindow->SetHide ( bHide );
	pModalWindow->m_bTEST_MULTI = true;
	pModalWindow->InitDeviceObjects ( m_pd3dDevice );
	pModalWindow->RestoreDeviceObjects ( m_pd3dDevice );
	RegisterControl ( pModalWindow );	

	ShowGroupFocus ( nMODAL_NEWID );
}

void CInnerInterface::OPEN_MODALESS ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID, BOOL bHide )
{
	const int nMODAL_NEWID = CModalWindow::MAKE_MODAL_NEWID () + MODAL_WINDOW;

	CModalWindow* pModalWindow = new CModalWindow;
	pModalWindow->Create ( nMODAL_NEWID, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
	pModalWindow->CreateSubControl ();
	pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
	pModalWindow->DoModal ( strText, nModalTitle, nModalType, CallerID );
	pModalWindow->SetHide ( bHide );
	pModalWindow->m_bTEST_MULTI = true;
	pModalWindow->SetUseModaless ( true );
	pModalWindow->InitDeviceObjects ( m_pd3dDevice );
	pModalWindow->RestoreDeviceObjects ( m_pd3dDevice );
	RegisterControl ( pModalWindow );	

	ShowGroupFocus ( nMODAL_NEWID );
}

void CInnerInterface::CLOSE_MODAL ( const UIGUID cID, bool bMakeMsg )
{		
	CUIControl* pControl = GetFocusControl ();	
	if ( pControl && pControl->GetWndID () != cID )
	{
		if ( !((CModalWindow*)pControl)->IsUseModaless () )
		{
			GASSERT ( 0 && "����, ��� ������ ���������� �ʽ��ϴ�." );
		}
	}
	HideGroup ( cID, bMakeMsg );
}

void	CInnerInterface::UpdateStateSimpleHP ()
{
	if ( DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP )
	{
		if ( !IsVisibleGroup ( SIMPLE_HP ) )
			ShowGroupBottom ( SIMPLE_HP );
	}
	else
	{
		if ( IsVisibleGroup ( SIMPLE_HP ) )
			HideGroup ( SIMPLE_HP );
	}

	if ( IsVisibleGroup ( SIMPLE_HP ) )
	{
		const long lResolution = GetResolution ();
		WORD X_RES = HIWORD ( lResolution );
		WORD Y_RES = LOWORD ( lResolution );

		D3DXVECTOR3 vPos = GLGaeaClient::GetInstance().GetCharacter()->GetPosition ();	

		//	NOTE
		//		������ ����
		{
			vPos.y -= 2.0f;
			D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

			const UIRECT& rcOriginPos = m_pSimpleHP->GetGlobalPos ();
			D3DXVECTOR2 vNewPos;
			vNewPos.x = (X_RES - rcOriginPos.sizeX) / 2.0f;
			vNewPos.y = ( vScreen.y - rcOriginPos.sizeY );

			if ( fabs ( rcOriginPos.left - vNewPos.x ) > 1.0f ||
				fabs ( rcOriginPos.top - vNewPos.y ) > 1.0f ) 

			m_pSimpleHP->SetGlobalPos ( vNewPos );			
		}
	}


	GLSummonClient* pSummon = GLGaeaClient::GetInstance().GetSummonClient ();
	if( pSummon && pSummon->IsVALID() )
	{
		if ( !IsVisibleGroup ( SUMMON_HP ) )
		ShowGroupBottom ( SUMMON_HP );

		D3DXVECTOR3 vPos = pSummon->GetPosition();

		vPos.y -= 2.0f;
		D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

		const UIRECT& rcOriginPos = m_pSummonHP->GetGlobalPos ();
		D3DXVECTOR2 vNewPos;
		vNewPos.x = vScreen.x - (rcOriginPos.sizeX/2.0f);
		vNewPos.y = ( vScreen.y - rcOriginPos.sizeY );

		if ( fabs ( rcOriginPos.left - vNewPos.x ) > 1.0f ||
			fabs ( rcOriginPos.top - vNewPos.y ) > 1.0f ) 

		m_pSummonHP->SetGlobalPos ( vNewPos );			
	}

}

void	CInnerInterface::SetLottoOpen ( const DWORD& dwGaeaID )
{
	CString strCombine = MakeString ( ID2GAMEINTEXT("UI_LOTTO"), "2004�� 7�� 2����" );	
	DoModal ( strCombine, MODAL_INPUT, MODAL_EDITBOX_LOTTERY, MODAL_LOTTO );	
}

void	CInnerInterface::SetStorageChargeOpen ( const WORD& wPosX, const WORD& wPosY )
{
	if ( m_pStorageChargeCard )
	{
		ShowGroupFocus ( STORAGE_CHARGE_CARD );
		m_pStorageChargeCard->InitStorageChargeCard ( wPosX, wPosY );
	}
}



void	CInnerInterface::SetItemShopInfo ()
{     
	if ( m_pItemShopWindow )
	{
		ShowGroupFocus ( ITEMSHOP_WINDOW );
		m_pItemShopWindow->InitItemShop (0,0);
	}
}

void	CInnerInterface::SetItemShopWindowOpen ()
{

	if( m_fItemShopDelay >= 5.0f )
	{
		m_fItemShopDelay = 0.0f;
	}else{
		PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("ITEM_SHOP_DELAY") );
		return;
	}

	ClearItemShop ();

	// ���⼭ ������ ��������� ���� ��û
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
	if ( pCharacter )
	{
		pCharacter->ReqItemShopInfo ();
		pCharacter->ReqRetrievePoints ();
	}
}



void	CInnerInterface::SetItemBankInfo ()
{     
	if ( m_pInventoryWindow )
	{
		ShowGroupFocus ( INVENTORY_WINDOW );
		m_pInventoryWindow->INVENTORY_BANK();
		m_pInventoryWindow->InitItemBank();
		state = 1;
	
	}
}

void	CInnerInterface::SetItemBankWindowOpen ()
{

	if( m_fItemBankDelay >= 5.0f )
	{
		m_fItemBankDelay = 0.0f;
	}else{
		PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("ITEM_BANK_DELAY") );
		return;
	}

	ClearItemBank ();

	// ���⼭ ������ ��������� ���� ��û
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
	if ( pCharacter )
	{
		pCharacter->ReqItemBankInfo ();
	}

	SetItemBankInfo ();
}

void	CInnerInterface::SetVNGainSysInfo ()
{     
	if ( m_pVNGainSysInventory )
	{
		ShowGroupFocus ( VNGAINSYS_WINDOW );
		m_pVNGainSysInventory->InitVNGainSys();
	}
}

void	CInnerInterface::SetVNGainSysWindowOpen ()
{
	if ( m_bFirstVNGainSysCall )
	{
		// ���⼭ ������ ��������� ���� ��û
		//GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
		/*if ( pCharacter )
		{
			pCharacter->ReqVNGainSysInfo ();
		}*/
		m_bFirstVNGainSysCall = false;
	}

	SetVNGainSysInfo ();
}
	
void CInnerInterface::OpenItemRebuildWindow()	// ITEMREBUILD_MARK
{
	if( !m_pItemRebuildWindow || !m_pRebuildInventoryWindow )
		return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	// RebuildInventory Window
	const UIRECT& rcInvenWindow = m_pRebuildInventoryWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pRebuildInventoryWindow->SetGlobalPos( vPos );
	ShowGroupFocus( REBUILDINVENTORY_WINDOW );

	// ItemRebuild Window
	const UIRECT& rcRebuildWindow = m_pItemRebuildWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcRebuildWindow.sizeX;
	m_pItemRebuildWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_REBUILD_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildOpen();
}

void CInnerInterface::CloseItemRebuildWindow()
{
	HideGroup( REBUILDINVENTORY_WINDOW );
	HideGroup( ITEM_REBUILD_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildClose();
}

void CInnerInterface::OpenItemGarbageWindow()	// ������
{
	if( !m_pItemGarbageWindow || !m_pGarbageInventoryWindow )
		return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	// RebuildInventory Window
	const UIRECT& rcInvenWindow = m_pGarbageInventoryWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pGarbageInventoryWindow->SetGlobalPos( vPos );
	ShowGroupFocus( GARBAGEINVENTORY_WINDOW );

	// ItemRebuild Window
	const UIRECT& rcRebuildWindow = m_pItemGarbageWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcRebuildWindow.sizeX;
	m_pItemGarbageWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_GARBAGE_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqGarbageOpen();
}

void CInnerInterface::CloseItemGarbageWindow()
{
	HideGroup( GARBAGEINVENTORY_WINDOW );
	HideGroup( ITEM_GARBAGE_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ReqGarbageClose();
}

void CInnerInterface::SetPetDisplay()
{
	ShowGroupBottom( PET_DISPLAY );
	ShowGroupBottom( PET_TIME_DISPLAY );
}

void CInnerInterface::ResetPetDisplay()
{
	HideGroup( PET_DISPLAY );
	HideGroup( PET_TIME_DISPLAY );
}
void CInnerInterface::SetVehicleDisplay()
{
	ShowGroupBottom( VEHICLE_DISPLAY );
	ShowGroupBottom( VEHICLE_TIME_DISPLAY );
}

void CInnerInterface::ResetVehicleDisplay()
{
	HideGroup( VEHICLE_DISPLAY );
	HideGroup( VEHICLE_TIME_DISPLAY );
}

void CInnerInterface::OpenItemMixWindow( DWORD dwNpcID )
{
	if( !m_pItemMixWindow || !m_pItemMixInvenWindow )
		return;

	CloseAllWindow();

	const long lResolution = GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );
	D3DXVECTOR2 vPos;

	const UIRECT& rcInvenWindow = m_pItemMixInvenWindow->GetGlobalPos();
	vPos.x = ( X_RES ) / 2.0f;
	vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
	m_pItemMixInvenWindow->SetGlobalPos( vPos );
	ShowGroupFocus( ITEM_MIX_INVEN_WINDOW );

	// ItemRebuild Window
	const UIRECT& rcRebuildWindow = m_pItemMixWindow->GetGlobalPos();
	vPos.x = ( X_RES / 2.0f ) - rcRebuildWindow.sizeX;
	m_pItemMixWindow->SetGlobalPos( vPos );
	m_pItemMixWindow->m_dwNpcID = dwNpcID;
	ShowGroupFocus( ITEM_MIX_WINDOW );
}

void CInnerInterface::CloseItemMixWindow()
{
	HideGroup( ITEM_MIX_INVEN_WINDOW );
	HideGroup( ITEM_MIX_WINDOW );

	GLGaeaClient::GetInstance().GetCharacter()->ResetItemMix();
}

void	CInnerInterface::REFRESH_ITEMBANK ()
{
	if ( m_pInventoryWindow )
	{		
		m_pInventoryWindow->InitItemBank ();
	}
}

void	CInnerInterface::REFRESH_VNGAINSYSTEM ()
{
	if ( m_pVNGainSysInventory )
	{		
		m_pVNGainSysInventory->InitVNGainSys();
	}
}

void	CInnerInterface::SetInventorySlotViewSize ( const int& nSLOT_VIEW_SIZE )
{
	if ( m_pInventoryWindow )
	{
		m_pInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );
		m_pTradeInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );
		m_pRebuildInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );	// ITEMREBUILD_MARK
	}
}

int CInnerInterface::GetInventorySlotViewSize () const
{
	if ( m_pInventoryWindow )
	{
		return m_pInventoryWindow->GetOneViewSlot ();
	}

	GASSERT ( 0 && "�κ��丮 ����� �� �� �����ϴ�." );
	return 0;
}

HRESULT CInnerInterface::FinalCleanup ()
{
	HRESULT hr = S_OK;

	hr = CUIMan::FinalCleanup ();
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

void CInnerInterface::SetPrivateMarketOpen ( const bool& bOPENER, const DWORD& dwGaeaID )
{
	if ( IsVisibleGroup ( PRIVATE_MARKET_WINDOW ) ) return ;

	if ( m_pPrivateMarketWindow )
	{
		m_pPrivateMarketWindow->InitPrivateMarket ( bOPENER, dwGaeaID );
		m_pTradeInventoryWindow->SetOPENER ( bOPENER );
        
		const BOOL bTemp_STATEBACK = m_bPartyStateBack;
		CloseAllWindow ();

		//	NOTE
		//			��Ƽ�� �� ���¿��� �̴���Ƽâ�� �ٿ��ΰ�,
		//			���λ����� ���� ���...
		//			�̴���Ƽâ�� �ٽ� ������.
		//			������ CloseAllWindow()�� ȣ���ϸ�,
		//			��� â�� �ݰ� �ǰ�, ��Ƽ����(m_bPartyStateBack)�� �����ϰ� �Ǵµ�
		//			�̷��� �� ���, ���ο� ��Ƽ�� �����Ȱ����� ó���ؼ�
		//			�̴���Ƽâ�� �ٽ� ���� �Ǵ� ���̴�.
		//			����, CloseAllWindow()������ m_bPartyStateBack�� true���ٸ�,
		//			���Ŀ��� true�� �������� �ش�.
		m_bPartyStateBack = bTemp_STATEBACK;
		//////////////////////////////////////////////////////////////////

		const long lResolution = GetResolution ();
		WORD X_RES = HIWORD ( lResolution );
		WORD Y_RES = LOWORD ( lResolution );
		{
			const UIRECT& rcPrivateMarketWindow = m_pPrivateMarketWindow->GetGlobalPos ();

			D3DXVECTOR2 vPos;
			vPos.x = ((X_RES) / 2.0f) - rcPrivateMarketWindow.sizeX;
			vPos.y = (Y_RES - rcPrivateMarketWindow.sizeY) / 2.0f;
			m_pPrivateMarketWindow->SetGlobalPos ( vPos );
		}

		{
			const UIRECT& rcTradeInventoryWindow = GetTradeInventoryWindow()->GetGlobalPos ();

			D3DXVECTOR2 vPos;
			vPos.x = (X_RES) / 2.0f;
			vPos.y = (Y_RES - rcTradeInventoryWindow.sizeY) / 2.0f;
			GetTradeInventoryWindow()->SetGlobalPos ( vPos );
		}

		ShowGroupFocus ( PRIVATE_MARKET_WINDOW );
		ShowGroupFocus ( TRADEINVENTORY_WINDOW );
	}
}

void CInnerInterface::GetPrivateMarketInfo ( bool& bOPENER, DWORD& dwGaeaID )
{	
	bOPENER = m_pPrivateMarketWindow->IsOPENER ();
	dwGaeaID = m_pPrivateMarketWindow->GetPrivateMarketID ();;
}

void CInnerInterface::SetPrivateMarketClose ()
{
	HideGroup ( PRIVATE_MARKET_WINDOW );
	HideGroup ( TRADEINVENTORY_WINDOW );
    
	if ( m_pPrivateMarketWindow->IsOPENER () )
		GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketClose ();
	else
		GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketInfoRelease ( m_pPrivateMarketWindow->GetPrivateMarketID () );
}

void CInnerInterface::MODAL_PRIVATE_MARKET_SELLITEM ( const bool& bCOUNTABLE, const WORD& wPosX, const WORD& wPosY )
{
	if ( m_pPrivateMarketSellWindow )
	{
		m_pPrivateMarketSellWindow->SetType ( bCOUNTABLE );
		m_pPrivateMarketSellWindow->SetCallerID ( MODAL_PRIVATE_MARKET_SELL );
		m_pPrivateMarketSellWindow->SetItemIndex ( wPosX, wPosY );
		ShowGroupFocus ( PRIVATE_MARKET_SELL_WINDOW, true );
	}
}

const	CString& CInnerInterface::GetPrivateMarketTitle ()
{
	return m_pPrivateMarketMake->GetTITLE ();
}

void	CInnerInterface::SetPrivateMarketMake ()
{	
	if( !IsVisibleGroup ( PRIVATE_MARKET_MAKE ) )
	{
		if ( m_pPrivateMarketMake )
		{
			m_pPrivateMarketMake->Init ();
		}
	}

	ShowGroupFocus ( PRIVATE_MARKET_MAKE, true ); // 
}

void	CInnerInterface::SetClubMake ( const DWORD& dwNpcID )
{
	if ( !IsVisibleGroup ( CLUB_MAKE ) )
	{
		if ( m_pClubMake )
		{
			m_pClubMake->Init ( dwNpcID );
			ShowGroupFocus ( CLUB_MAKE, true );
		}
	}	
}

CString CInnerInterface::GetdwKeyToString(int dwKey)
{
	return GetKeySettingWindow()->GetdwKeyToString(dwKey);
}

void 	CInnerInterface::SetShotcutText ( DWORD nID, CString& strTemp )
{
	if( nID >= 0 && nID < QUICK_SLOT_NUM )
	{
		m_pUILeftTopGroup->SetShotcutText( nID, strTemp );
	}
	else if( nID >= QUICK_SLOT_NUM && nID < (QUICK_SLOT_NUM + QUICK_SKILL_NUM) )
	{
		m_pSkillTrayTab->SetShotcutText( nID - ( QUICK_SLOT_NUM) , strTemp);
	}
	else if( nID >= (QUICK_SLOT_NUM + QUICK_SKILL_NUM) && 
			  nID < (QUICK_SLOT_NUM + QUICK_SKILL_NUM + MENU_SHOTCUT_NUM ))
	{
		int ranIndex = RANPARAM::KeySettingToRanparam[nID- (QUICK_SLOT_NUM + QUICK_SKILL_NUM) ];		
		int menuIndex;
		
		for (int i = 0; i < BASIC_MENU_NUM; ++i)
		{
			menuIndex = RANPARAM::BasicMenuToRanparam[i];
			if( ranIndex == menuIndex )
			{
				m_pGameMenu->SetShotcutText( i , strTemp);
				return;
			}
		}		
	}
}

void CInnerInterface::GetShotCutKey(){
	m_pKeySettingWindow->GetShotCutKey();
}

void CInnerInterface::GetChatMacro(){
	m_pChatMacroWindow->GetChatMacro();
}
void CInnerInterface::AddChatMacro(int nIndex)
{
	GetChat()->AddChatMacro (RANPARAM::ChatMacro[nIndex]);
}

void CInnerInterface::ChatLog( bool bChatLogt, int nChatLogType )
{
	GetChat()->ChatLog( bChatLogt, nChatLogType );
}

CString CInnerInterface::GET_RECORD_CHAT()
{
	if( !GetChat() ) return NULL;
	return GetChat()->GET_RECORD_CHAT();
}

void CInnerInterface::DOMODAL_CLUB_JOIN_ASK ( const DWORD& dwMasterID, const CString& strClubName, const CString& strClubMaster )
{
	m_dwClubMasterID = dwMasterID;
	
	CString strCombine = MakeString ( ID2GAMEINTEXT("CLUB_JOIN_ASK"), strClubName, strClubMaster );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_JOIN_ASK );
}

void CInnerInterface::DOMODAL_UNION_JOIN_ASK ( const DWORD& dwMasterID, const CString& strClubMaster )
{
	m_dwClubMasterID = dwMasterID;

	CString strCombine = MakeString ( ID2GAMEINTEXT("UNION_JOIN_ASK"), strClubMaster );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_UNION_JOIN_ASK );
}

void CInnerInterface::DOMODAL_CLUB_BATTLE_ASK ( const DWORD& dwMasterID, const CString& strClubMaster, DWORD dwBattleTime, bool bAlliance )
{
	m_dwClubMasterID = dwMasterID;

	if ( bAlliance ) 
	{
		CString strCombine = MakeString ( ID2GAMEINTEXT("ALLIANCE_BATTLE_ASK"), strClubMaster, dwBattleTime/60, dwBattleTime%60 );
		DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_ALLIANCE_BATTLE_ASK );
	}
	else
	{
		CString strCombine = MakeString ( ID2GAMEINTEXT("CLUB_BATTLE_ASK"), strClubMaster, dwBattleTime/60, dwBattleTime%60 );
		DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_BATTLE_ASK );
	}
}

void CInnerInterface::DOMODAL_CLUB_AUTHORITY_ASK( const CString& strClubName )
{
	CString strCombine = MakeString ( ID2GAMEINTEXT("CLUB_AUTHORITY_MEMBER_ASK"), strClubName );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_AUTHORITY_ASK );
}

void CInnerInterface::DOMODAL_CLUB_BATTLE_ARMISTICE_ASK( const DWORD& dwClubID, const CString& strClubName, bool bAlliance )
{
	// �ӽ� ����
	m_dwClubMasterID = dwClubID;
	CString strCombine;
	
	if ( bAlliance ) 
	{
		strCombine = MakeString ( ID2GAMEINTEXT("ALLIANCE_BATTLE_ARMISTICE_ASK"), strClubName );
		DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_ALLIANCE_BATTLE_ARMISTICE_ASK );
	}
	else 
	{
		strCombine = MakeString ( ID2GAMEINTEXT("CLUB_BATTLE_ARMISTICE_ASK"), strClubName );
		DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_BATTLE_ARMISTICE_ASK );
	}
}

bool CInnerInterface::SET_QUESTION_ITEM_ID ( int nID )
{
	UIGUID cID = NO_ID;
	switch ( nID )
	{
	case QUESTION_SPEED_UP:		cID = CQuestionItemDisplay::SPEED_UP;		break;
	case QUESTION_CRAZY:		cID = CQuestionItemDisplay::CRAZY_TIME;		break;
	case QUESTION_ATTACK_UP:	cID = CQuestionItemDisplay::POWER_UP;		break;
	case QUESTION_EXP_UP:		cID = CQuestionItemDisplay::EXP_TIME;		break;
	case QUESTION_EXP_GET:		cID = CQuestionItemDisplay::EXP_GET;		break;
	case QUESTION_LUCKY:		cID = CQuestionItemDisplay::LUCKY;			break;
	case QUESTION_BOMB:			cID = CQuestionItemDisplay::BOMB;			break;
	case QUESTION_MOBGEN:		cID = CQuestionItemDisplay::OH_NO;			break;
	case QUESTION_SPEED_UP_M:	cID = CQuestionItemDisplay::SPEED_UP_M;		break;
	case QUESTION_MADNESS:		cID = CQuestionItemDisplay::MADNESS;		break;
	case QUESTION_ATTACK_UP_M:	cID = CQuestionItemDisplay::ATTACK_UP_M;	break;
	case QUESTION_HEAL:			cID = CQuestionItemDisplay::HEAL;			break;
	}

	UIGUID ImageID = NO_ID;
	switch ( nID )
	{
	case QUESTION_SPEED_UP:		ImageID = CQBoxButton::QBOX_SPEED_UP;		break;
	case QUESTION_CRAZY:		ImageID = CQBoxButton::QBOX_CRAZY_TIME;		break;
	case QUESTION_ATTACK_UP:	ImageID = CQBoxButton::QBOX_POWER_UP;		break;
	case QUESTION_EXP_UP:		ImageID = CQBoxButton::QBOX_EXP_TIME;		break;
	case QUESTION_LUCKY:		ImageID = CQBoxButton::QBOX_LUCKY;			break;
	case QUESTION_SPEED_UP_M:	ImageID = CQBoxButton::QBOX_SPEED_UP_M;		break;
	case QUESTION_MADNESS:		ImageID = CQBoxButton::QBOX_MADNESS;		break;
	case QUESTION_ATTACK_UP_M:	ImageID = CQBoxButton::QBOX_ATTACK_UP_M;	break;
	}

	if ( cID == NO_ID ) return false;

	ShowGroupTop ( QUESTION_ITEM_DISPLAY );
	m_pQuestionItemDisplay->START ( cID );
	m_pQBoxButton->Start( ImageID );

	if ( ImageID !=1 ||  ImageID !=0 )	ShowGroupBottom ( QBOX_TIME_DISPLAY );
	else	HideGroup ( QBOX_TIME_DISPLAY );

	return true;
}

bool	CInnerInterface::SET_KEEP_QUESTION_ITEM_ID ( int nID )
{
	UIGUID cID = NO_ID;
	switch ( nID )
	{
	case QUESTION_SPEED_UP:		cID = CQuestionItemDisplay::SPEED_UP;		break;
	case QUESTION_CRAZY:		cID = CQuestionItemDisplay::CRAZY_TIME;		break;
	case QUESTION_ATTACK_UP:	cID = CQuestionItemDisplay::POWER_UP;		break;
	case QUESTION_EXP_UP:		cID = CQuestionItemDisplay::EXP_TIME;		break;
	case QUESTION_EXP_GET:		cID = CQuestionItemDisplay::EXP_GET;		break;
	case QUESTION_LUCKY:		cID = CQuestionItemDisplay::LUCKY;			break;
	case QUESTION_BOMB:			cID = CQuestionItemDisplay::BOMB;			break;
	case QUESTION_MOBGEN:		cID = CQuestionItemDisplay::OH_NO;			break;
	case QUESTION_SPEED_UP_M:	cID = CQuestionItemDisplay::SPEED_UP_M;		break;
	case QUESTION_MADNESS:		cID = CQuestionItemDisplay::MADNESS;		break;
	case QUESTION_ATTACK_UP_M:	cID = CQuestionItemDisplay::ATTACK_UP_M;	break;
	case QUESTION_HEAL:			cID = CQuestionItemDisplay::HEAL;			break;
	}

	UIGUID ImageID = NO_ID;
	switch ( nID )
	{
	case QUESTION_SPEED_UP:		ImageID = CQBoxButton::QBOX_SPEED_UP;		break;
	case QUESTION_CRAZY:		ImageID = CQBoxButton::QBOX_CRAZY_TIME;		break;
	case QUESTION_ATTACK_UP:	ImageID = CQBoxButton::QBOX_POWER_UP;		break;
	case QUESTION_EXP_UP:		ImageID = CQBoxButton::QBOX_EXP_TIME;		break;
	case QUESTION_LUCKY:		ImageID = CQBoxButton::QBOX_LUCKY;			break;
	case QUESTION_SPEED_UP_M:	ImageID = CQBoxButton::QBOX_SPEED_UP_M;		break;
	case QUESTION_MADNESS:		ImageID = CQBoxButton::QBOX_MADNESS;		break;
	case QUESTION_ATTACK_UP_M:	ImageID = CQBoxButton::QBOX_ATTACK_UP_M;	break;
	}

	if ( cID == NO_ID ) return false;

	RESET_KEEP_QUESTION_ITEM();

	ShowGroupTop ( QUESTION_ITEM_DISPLAY );
	m_pQuestionItemDisplay->KEEP_START ( cID );

	m_pQBoxButton->Change( ImageID ); //add qbox

	if ( ImageID !=1 ||  ImageID !=0 )	ShowGroupBottom ( QBOX_TIME_DISPLAY );
	else	HideGroup ( QBOX_TIME_DISPLAY );

	return true;
}

void	CInnerInterface::RESET_KEEP_QUESTION_ITEM ()
{
	if( m_pQuestionItemDisplay )
	{
		m_pQuestionItemDisplay->KEEP_STOP ();
	}

	if( m_pQBoxButton )
	{
		m_pQBoxButton->Stop();
	}
	HideGroup ( QBOX_TIME_DISPLAY );
}

void CInnerInterface::SetQBoxTime ( float fTime) //add qbox
{
	m_pQBoxTimeUnit->SetTime ( fTime );

}
void CInnerInterface::UpdateQBoxTime ( float fRTime )//add qbox
{
	m_pQBoxTimeUnit->SetLeftTime ( fRTime );
}


void	CInnerInterface::SET_CONFT_CONFIRM_VISIBLE ( bool bVisible )
{
	if ( bVisible )	ShowGroupTop( CONFT_CONFIRM );
	else			HideGroup( CONFT_CONFIRM );
}

void	CInnerInterface::SET_CONFT_CONFIRM_PERCENT ( float fPercent )
{
	if( m_pConftConfirm )
	{
		m_pConftConfirm->SetPercent ( fPercent );
	}
}

HRESULT CInnerInterface::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	TEXTURE_PRE_LOAD ( pd3dDevice );

	{

		m_pKeySettingWindow = new CKeySettingWindow;
		m_pKeySettingWindow->Create ( KEY_SETTING_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pKeySettingWindow->CreateBaseWindowBlack ( "KEY_SETTING_WINDOW", (char*)ID2GAMEWORD("KEYSETTING_WINDOW_NAME_STATIC") );
		m_pKeySettingWindow->CreateSubControl ();
		m_pKeySettingWindow->SetControlNameEx ( "����Ű����â" );
		m_pKeySettingWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pKeySettingWindow, true);
		ShowGroupFocus ( KEY_SETTING_WINDOW );
	}

	{

		m_pChatMacroWindow = new CChatMacroWindow;
		m_pChatMacroWindow->Create ( CHATMACRO_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pChatMacroWindow->CreateBaseWindowBlack ( "CHATMACRO_WINDOW", (char*)ID2GAMEWORD("CHATMACRO_WINDOW_NAME_STATIC") );
		m_pChatMacroWindow->CreateSubControl ();
		m_pChatMacroWindow->SetControlNameEx ( "ä�ø�ũ��" );
		m_pChatMacroWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pChatMacroWindow, true );
		ShowGroupFocus ( CHATMACRO_WINDOW );
		
	}

	{
		m_pNameDisplayMan = new CNameDisplayMan;
		m_pNameDisplayMan->Create ( NAME_DISPLAY_MAN, "NAME_DISPLAY_MAN" );
		RegisterControl ( m_pNameDisplayMan );
		ShowGroupBottom ( NAME_DISPLAY_MAN );
		HideGroup( NAME_DISPLAY_MAN );

		m_pPrivateMarketShowMan = new CPrivateMarketShowMan;
		m_pPrivateMarketShowMan->Create ( PRIVATE_MARKET_SHOW_MAN, "NAME_DISPLAY_MAN" );
		RegisterControl ( m_pPrivateMarketShowMan );
		ShowGroupBottom ( PRIVATE_MARKET_SHOW_MAN );
	}

	{
		m_pTargetInfoDisplay = new CTargetInfoDisplay;
		m_pTargetInfoDisplay->Create ( TARGETINFO_DISPLAY, "TARGETINFO_DISPLAY" );
		m_pTargetInfoDisplay->CreateSubControl ();
		RegisterControl ( m_pTargetInfoDisplay );
		ShowGroupBottom ( TARGETINFO_DISPLAY );
		HideGroup( TARGETINFO_DISPLAY );
	}
	{
		m_pTargetInfoDisplayNpc = new CTargetInfoDisplayNpc;
		m_pTargetInfoDisplayNpc->Create ( TARGETINFO_DISPLAY_NPC, "TARGETINFO_DISPLAY_NPC" );
		m_pTargetInfoDisplayNpc->CreateSubControl ();
		RegisterControl ( m_pTargetInfoDisplayNpc );
		ShowGroupBottom ( TARGETINFO_DISPLAY_NPC );
		HideGroup( TARGETINFO_DISPLAY_NPC );
	}

	{
		m_pTargetInfoDisplayPlayer = new CTargetInfoDisplayPlayer;
		m_pTargetInfoDisplayPlayer->Create ( TARGETINFO_DISPLAY_PLAYER, "TARGETINFO_DISPLAY_PLAYER" );
		m_pTargetInfoDisplayPlayer->CreateSubControl ();
		RegisterControl ( m_pTargetInfoDisplayPlayer );
		ShowGroupBottom ( TARGETINFO_DISPLAY_PLAYER );
		HideGroup( TARGETINFO_DISPLAY_PLAYER );
	}

	{
		m_pDamageDisplayMan = new CDamageDisplayMan;
		m_pDamageDisplayMan->Create ( DAMAGE_MAN, "DAMAGE_MAN" );
		m_pDamageDisplayMan->CreateSubControl ();
		RegisterControl ( m_pDamageDisplayMan );
		ShowGroupBottom ( DAMAGE_MAN );
	}

	{
        m_pHeadChatDisplayMan = new CHeadChatDisplayMan;
		m_pHeadChatDisplayMan->Create ( HEADCHAT_MAN, "HEADCHAT_MAN" );
		RegisterControl ( m_pHeadChatDisplayMan );
		ShowGroupBottom ( HEADCHAT_MAN );
	}

	{
		m_pUILeftTopGroup = new CUILeftTopGroup;
		m_pUILeftTopGroup->CreateEx( LEFTTOP_CONTROL_GROUP, "LEFTTOP_CONTROL_GROUP" );
		m_pUILeftTopGroup->CreateSubControl ();
		RegisterControl ( m_pUILeftTopGroup );
		ShowGroupBottom ( LEFTTOP_CONTROL_GROUP );
	}

	{

		const EMCHARCLASS emCharClass = DxGlobalStage::GetInstance().GetGameStage()->GetCharJoinData().m_CharData2.m_emClass;
		const int nClassType = CharClassToIndex ( emCharClass );

		CSkillTimeDisplay* pSkillTimeDisplay = new CSkillTimeDisplay;	

		// �ذ��ο� �Ϲ� ĳ���͸� �����ؼ� ����UI ��ġ ����
	//	if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
	//	{
			pSkillTimeDisplay->Create ( SKILL_TIME_DISPLAY, "SKILL_TIME_DISPLAY_EXTREME" );
	//	}
	//	else
	//	{
	//		pSkillTimeDisplay->Create ( SKILL_TIME_DISPLAY, "SKILL_TIME_DISPLAY" );
	//	}
		
		pSkillTimeDisplay->CreateSubControl ();
		RegisterControl ( pSkillTimeDisplay );
		ShowGroupBottom ( SKILL_TIME_DISPLAY );


		// �ذ��� ���� ǥ�� UI 
		//if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
		//{
			CWeaponDisplay* pWeaponDisplay = new CWeaponDisplay;
			pWeaponDisplay->Create ( WEAPON_DISPLAY, "WEAPON_DISPLAY" );
			pWeaponDisplay->CreateSubControl ();
			RegisterControl ( pWeaponDisplay );
			ShowGroupBottom ( WEAPON_DISPLAY );
		//}

	}

	{ //add petimage
		CPetDisplay* pPetDisplay = new CPetDisplay;
		pPetDisplay->Create ( PET_DISPLAY, "PET_HUNGRY_ICON_BOX", UI_FLAG_RIGHT | UI_FLAG_BOTTOM  );
		pPetDisplay->CreateSubControl ();
		RegisterControl ( pPetDisplay );
		ShowGroupBottom ( PET_DISPLAY );
		HideGroup ( PET_DISPLAY );

		m_pPetTimeUnit = new CPetTimeUnit;
		m_pPetTimeUnit->Create( PET_TIME_DISPLAY, "PET_HUNGRY_ICON_GAUGE", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pPetTimeUnit->CreateSubControl ();
		RegisterControl ( m_pPetTimeUnit );
		ShowGroupBottom ( PET_TIME_DISPLAY );
		HideGroup ( PET_TIME_DISPLAY );
	}

	{ //add vehicleimage
		CVehicleDisplay* pVehicleDisplay = new CVehicleDisplay;
		pVehicleDisplay->Create ( VEHICLE_DISPLAY, "VEHICLE_POWER_ICON_BOX", UI_FLAG_RIGHT | UI_FLAG_BOTTOM  );
		pVehicleDisplay->CreateSubControl ();
		RegisterControl ( pVehicleDisplay );
		ShowGroupBottom ( VEHICLE_DISPLAY );
		HideGroup ( VEHICLE_DISPLAY );

		m_pVehicleTimeUnit = new CVehicleTimeUnit;
		m_pVehicleTimeUnit->Create( VEHICLE_TIME_DISPLAY, "VEHICLE_POWER_ICON_GAUGE", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pVehicleTimeUnit->CreateSubControl ();
		RegisterControl ( m_pVehicleTimeUnit );
		ShowGroupBottom ( VEHICLE_TIME_DISPLAY );
		HideGroup ( VEHICLE_TIME_DISPLAY );
	}

	{ //add notify
		CNotifyPost* pNotifyPost = new CNotifyPost;
		pNotifyPost->Create ( NOTIFY_POST, "POSTNOTIFY_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM  );
		pNotifyPost->CreateSubControl ();
		RegisterControl ( pNotifyPost );
		ShowGroupBottom ( NOTIFY_POST );

		CNotifyRecord* pNotifyRecord = new CNotifyRecord;
		pNotifyRecord->Create ( NOTIFY_RECORD, "STUDENTRECORD_NOTIFY_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM  );
		pNotifyRecord->CreateSubControl ();
		RegisterControl ( pNotifyRecord );
		ShowGroupBottom ( NOTIFY_RECORD );

		CNotifyCompete* pNotifyCompete = new CNotifyCompete;
		pNotifyCompete->Create ( NOTIFY_COMPETE, "COMPETITION_NOTIFY_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM  );
		pNotifyCompete->CreateSubControl ();
		RegisterControl ( pNotifyCompete );
		ShowGroupBottom ( NOTIFY_COMPETE );
	}
	{ //CrowTargetInfo by NjD
			m_pCrowTargetInfo = new CCrowTargetInfo;
			m_pCrowTargetInfo->Create ( CROW_TARGET_INFO, "RNCROW_TARGET_INFO", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
			m_pCrowTargetInfo->CreateSubControl ();
			RegisterControl ( m_pCrowTargetInfo );
			ShowGroupBottom ( CROW_TARGET_INFO );
			HideGroup( CROW_TARGET_INFO );
	}
	{ //CrowTargetInfo by NjD
			m_pCrowTargetInfoNpc = new CCrowTargetInfoNpc;
			m_pCrowTargetInfoNpc->Create ( CROW_TARGET_INFO_NPC, "RNCROW_TARGET_INFO_NPC", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
			m_pCrowTargetInfoNpc->CreateSubControl ();
			RegisterControl ( m_pCrowTargetInfoNpc );
			ShowGroupBottom ( CROW_TARGET_INFO_NPC );
			HideGroup( CROW_TARGET_INFO_NPC );
	}
	{ //CrowTargetInfo by NjD
			m_pCrowTargetInfoPlayer = new CCrowTargetInfoPlayer;
			m_pCrowTargetInfoPlayer->Create ( CROW_TARGET_INFO_PLAYER, "RNCROW_TARGET_INFO_PLAYER", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
			m_pCrowTargetInfoPlayer->CreateSubControl ();
			RegisterControl ( m_pCrowTargetInfoPlayer );
			ShowGroupBottom ( CROW_TARGET_INFO_PLAYER );
			HideGroup( CROW_TARGET_INFO_PLAYER );
	}
	{
		m_pMiniMap = new CMiniMap;
		m_pMiniMap->Create ( MINIMAP, "BASIC_MINIMAP", UI_FLAG_RIGHT );
		m_pMiniMap->CreateSubControl ();
		RegisterControl ( m_pMiniMap );
		ShowGroupBottom ( MINIMAP );
	}

	{
		m_pQuestHelper = new CQuestHelper;
		m_pQuestHelper->Create ( QUEST_HELPER_DISPLAY, "QUEST_HELPER_DISPLAY", UI_FLAG_RIGHT );
		m_pQuestHelper->CreateSubControl ();
		RegisterControl ( m_pQuestHelper );
		ShowGroupBottom ( QUEST_HELPER_DISPLAY );
	}

	{
		m_pCdmRankingDisplay = new CCdmRankingDisplay;
		m_pCdmRankingDisplay->Create ( CDM_RANKING_DISPLAY, "CDM_RANKING_DISPLAY", UI_FLAG_RIGHT );
		m_pCdmRankingDisplay->CreateSubControl ();
		RegisterControl ( m_pCdmRankingDisplay );
		ShowGroupBottom ( CDM_RANKING_DISPLAY );	
		HideGroup( CDM_RANKING_DISPLAY );

	}

	{
		CBasicButton* pMiniPartyOpen = new CBasicButton;
		pMiniPartyOpen->Create ( MINIPARTY_OPEN, "MINIPARTY_OPEN", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
//#ifdef CH_PARAM // �߱� �������̽� ����
//		pMiniPartyOpen->CreateMouseOver( "MINIPARTY_OPEN_F" );
//#else
		pMiniPartyOpen->CreateFlip ( "MINIPARTY_OPEN_F", CBasicButton::CLICK_FLIP );
		pMiniPartyOpen->SetUseDynamic ( TRUE );
//#endif
		pMiniPartyOpen->SetUseGlobalAction ( TRUE );
		RegisterControl ( pMiniPartyOpen );
		ShowGroupBottom ( MINIPARTY_OPEN );

		m_pQuestAlarm = new CQuestAlarm;
		m_pQuestAlarm->Create ( QUEST_ALARM, "QUEST_ALARM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pQuestAlarm->CreateSubControl ();
		RegisterControl ( m_pQuestAlarm );
		ShowGroupBottom ( QUEST_ALARM );
		HideGroup ( QUEST_ALARM );

		CEscMenuOpen* pEscMenuOpen = new CEscMenuOpen;
		pEscMenuOpen->Create ( ESCMENU_OPEN, "ESCMENU_OPEN", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		pEscMenuOpen->CreateSubControl ();
		RegisterControl ( pEscMenuOpen );		
		ShowGroupBottom ( ESCMENU_OPEN );

		m_pQBoxButton = new CQBoxButton;
		m_pQBoxButton->Create ( QBOX_BUTTON, "QBOX_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pQBoxButton->CreateSubControl ();
		RegisterControl ( m_pQBoxButton );		
		ShowGroupBottom ( QBOX_BUTTON );

		//add qbox
		m_pQBoxTimeUnit = new CQBoxTimeUnit;
		m_pQBoxTimeUnit->Create( QBOX_TIME_DISPLAY, "QBOX_GAUGE_BOX", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pQBoxTimeUnit->CreateSubControl ();
		RegisterControl ( m_pQBoxTimeUnit );
		ShowGroupBottom ( QBOX_TIME_DISPLAY );
		HideGroup ( QBOX_TIME_DISPLAY );


// #ifndef CH_PARAM // �߱� �������̽� ����
		CBasicButton* pGameMenuOpen = new CBasicButton;
		pGameMenuOpen->Create ( GAME_MENU_OPEN_BUTTON, "GAME_MENU_OPEN_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		pGameMenuOpen->CreateFlip ( "GAME_MENU_OPEN_BUTTON_F", CBasicButton::MOUSEIN_FLIP );
		pGameMenuOpen->SetUseGlobalAction ( TRUE );
		RegisterControl ( pGameMenuOpen );
		ShowGroupBottom ( GAME_MENU_OPEN_BUTTON );
		HideGroup( GAME_MENU_OPEN_BUTTON );
// #endif

		m_pGameMenu = new CBasicGameMenu;
		m_pGameMenu->Create ( GAME_MENU, "GAME_MENU", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pGameMenu->CreateSubControl ();
		RegisterControl ( m_pGameMenu );
		ShowGroupBottom ( GAME_MENU );
		{
			// ���� ĳ������ �޸��� ��带 �����Ѵ�.
			BOOL bRun = GLGaeaClient::GetInstance().GetCharacter()->IsRunMode();
			m_pGameMenu->SetFlipRunButton( bRun );
		}
	}

	{
		m_pBasicQuickSkillSlot = new CBasicQuickSkillSlot;
		m_pBasicQuickSkillSlot->Create ( BASIC_QUICK_SKILL_SLOT, "BASIC_QUICK_SKILL_SLOT" );
		m_pBasicQuickSkillSlot->CreateSubControl ();
		m_pBasicQuickSkillSlot->SetUseSkillImageTwinkle( false ); // ��ų ����� �����̴� �׼��� ������� �ʽ��ϴ�.
		RegisterControl ( m_pBasicQuickSkillSlot );
		ShowGroupBottom ( BASIC_QUICK_SKILL_SLOT );

// #ifndef CH_PARAM // �߱� �������̽� ����
		CBasicButton* pButton = new CBasicButton;
		pButton->Create ( QUICK_SKILL_TRAY_OPEN_BUTTON, "QUICK_SKILL_TRAY_OPEN_BUTTON" );
		pButton->CreateFlip ( "QUICK_SKILL_TRAY_OPEN_BUTTON_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		RegisterControl ( pButton );
		ShowGroupBottom ( QUICK_SKILL_TRAY_OPEN_BUTTON );
		HideGroup ( QUICK_SKILL_TRAY_OPEN_BUTTON );
//#endif

		m_pSkillTrayTab = new CSkillTrayTab;
		m_pSkillTrayTab->CreateEx( QUICK_SKILL_TRAY_TAB_WINDOW, "QUICK_SKILL_TRAY_TAB_WINDOW" );
		m_pSkillTrayTab->CreateSubControl();
		RegisterControl( m_pSkillTrayTab );
		ShowGroupBottom ( QUICK_SKILL_TRAY_TAB_WINDOW );
	}

	{
		m_pChat = new CBasicChat;
		m_pChat->Create( BASIC_CHAT_BOX, "BASIC_CHAT_MINBOX", UI_FLAG_BOTTOM );
		m_pChat->CreateSubControl();
		m_pChat->SetProtectSizePoint();

		// ���� ä�� �ڽ��� ũ�⸦ �ּ� ����� �ƴ϶� ������ ������� �������� �մϴ�.
		{
			WORD wAlignFlag = m_pChat->GetAlignFlag ();
			m_pChat->SetAlignFlag ( UI_FLAG_YSIZE );

			CUIControl TempControl;
			TempControl.Create ( 1, "BASIC_CHAT_BOX" );
			const UIRECT& rcParentOldPos = m_pChat->GetLocalPos ();
			const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
			m_pChat->AlignSubControl ( rcParentOldPos, rcParentNewPos );
			m_pChat->SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );

			m_pChat->SetAlignFlag ( wAlignFlag );
		}		
		RegisterControl ( m_pChat );
		ShowGroupFocus ( BASIC_CHAT_BOX );
	}

	{
		m_pCharacterWindow = new CCharacterWindow;
		m_pCharacterWindow->Create ( CHARACTER_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//m_pCharacterWindow->CreateBaseWindowLightGray ( "RNCHARACTER_WINDOW_MAIN", (char*)ID2GAMEWORD("CHARACTER_NAME_STATIC") );
		m_pCharacterWindow->CreateBaseWindowLightGray ( "CHARACTER_WINDOW_MAIN", (char*)ID2GAMEWORD("CHARACTER_NAME_STATIC") );
		m_pCharacterWindow->CreateSubControl ();
		m_pCharacterWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pCharacterWindow, true );
		ShowGroupFocus ( CHARACTER_WINDOW );
	}

	{
		CBasicHWOptionWindow* pOptionWindow = new CBasicHWOptionWindow;
		pOptionWindow->Create ( OPTION_HW_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pOptionWindow->CreateBaseWindowBlack ( "HWOPTION_WINDOW", (char*)ID2GAMEWORD("HWOPTION_WINDOW_NAME_STATIC") );
		pOptionWindow->CreateSubControl ();
		pOptionWindow->SetControlNameEx ( "Game Option" );
		pOptionWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( pOptionWindow, true );
		ShowGroupFocus ( OPTION_HW_WINDOW );
	}

	{
		CSkillWindow* pSkillWindow = new CSkillWindow;
		pSkillWindow->Create ( SKILL_WINDOW,  "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pSkillWindow->CreateBaseWindowBlack ( "SKILL_WINDOW", (char*)ID2GAMEWORD("SKILL_NAME_STATIC") );
		pSkillWindow->CreateSubControl ();
		pSkillWindow->SetControlNameEx ( "Skill Window" );
		pSkillWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( pSkillWindow, true );
		ShowGroupFocus ( SKILL_WINDOW );
	}

	{
		CBasicEscMenu* pEscMenu = new CBasicEscMenu;
		pEscMenu->Create ( ESC_MENU, "ESC_MENU", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pEscMenu->CreateSubControl ();
		RegisterControl ( pEscMenu );
		ShowGroupFocus ( ESC_MENU );
	}

	{
		m_pPartyWindow = new CPartyWindow;
		m_pPartyWindow->Create ( PARTY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPartyWindow->CreateBaseWindowBlack ( "PARTY_WINDOW", (char*)ID2GAMEWORD("PARTY_NAME_STATIC") );
		m_pPartyWindow->CreateSubControl ();
		m_pPartyWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pPartyWindow, true );
		ShowGroupFocus ( PARTY_WINDOW );
	}

	{
		m_pMarketWindow = new CMarketWindow;
		m_pMarketWindow->Create ( MARKET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pMarketWindow->CreateBaseWindowBlack ( "MARKET_WINDOW", (char*)ID2GAMEWORD("MARKET_NAME_STATIC") );
		m_pMarketWindow->CreateSubControl ();
		m_pMarketWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_LEFT );
		RegisterControl ( m_pMarketWindow, true );
		ShowGroupFocus ( MARKET_WINDOW );
	}

	{	//	�κ��丮
		m_pInventoryWindow = new CInventoryWindow;
		m_pInventoryWindow->Create ( INVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInventoryWindow->CreateBaseWindowLightGray ( "INVENTORY_WINDOW_RENEW", (char*)ID2GAMEWORD("INVENTORY_NAME_STATIC") );
		m_pInventoryWindow->CreateSubControl ();
		m_pInventoryWindow->INVENTORY();
		m_pInventoryWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pInventoryWindow, true );
		ShowGroupFocus ( INVENTORY_WINDOW );
	}
	{
    m_pPartyDisplay = new CPartyDisplay;
    m_pPartyDisplay->Create( PARTY_DISPLAY, "PARTY_DISPLAY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
    m_pPartyDisplay->CreateSubControl();
    RegisterControl ( m_pPartyDisplay, true );
    ShowGroupBottom ( PARTY_DISPLAY );
	}
	{	//	�ŷ�
		m_pTradeWindow = new CTradeWindow;
		m_pTradeWindow->Create ( TRADE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTradeWindow->CreateBaseWindowBlack ( "TRADE_WINDOW", (char*)ID2GAMEWORD("TRADE_NAME_STATIC") );
		m_pTradeWindow->CreateSubControl ();
		m_pTradeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pTradeWindow, true );
		ShowGroupFocus ( TRADE_WINDOW );

		m_pTradeInventoryWindow = new CTradeInventoryWindow;
		m_pTradeInventoryWindow->Create ( TRADEINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTradeInventoryWindow->CreateBaseWindowBlack ( "TRADEINVENTORY_WINDOW", (char*)ID2GAMEWORD("TRADEINVENTORY_NAME_STATIC") );
		m_pTradeInventoryWindow->CreateSubControl ();
		m_pTradeInventoryWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pTradeInventoryWindow, true );
		ShowGroupFocus ( TRADEINVENTORY_WINDOW );
		HideGroup ( TRADEINVENTORY_WINDOW );
	}

	{	//	â��
		m_pStorageWindow = new CStorageWindow;		
		m_pStorageWindow->Create ( STORAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pStorageWindow->CreateBaseWindowBlack ( "STORAGE_WINDOW", (char*)ID2GAMEWORD("STORAGE_NAME_STATIC") );
		m_pStorageWindow->CreateSubControl ();
		m_pStorageWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_LEFT );
		RegisterControl ( m_pStorageWindow, true );
		ShowGroupFocus ( STORAGE_WINDOW );
	}

	{	//	â��
		m_pClubStorageWindow = new CClubStorageWindow;		
		m_pClubStorageWindow->Create ( CLUB_STORAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubStorageWindow->CreateBaseWindowBlack ( "STORAGE_WINDOW", (char*)ID2GAMEWORD("CLUB_STORAGE_NAME_STATIC") );
		m_pClubStorageWindow->CreateSubControl ();
		m_pClubStorageWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_LEFT );
		RegisterControl ( m_pClubStorageWindow, true );
		ShowGroupFocus ( CLUB_STORAGE_WINDOW );
	}


	{	//	�̴� ��Ƽ
		CMiniPartyWindow* pMiniPartyWindow = new CMiniPartyWindow;
		pMiniPartyWindow->Create ( MINIPARTY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pMiniPartyWindow->CreateBaseWindowMiniParty ( "MINIPARTY_WINDOW", (char*)ID2GAMEWORD("MINIPARTY_NAME_STATIC") );
		pMiniPartyWindow->CreateSubControl ();
		pMiniPartyWindow->SetAlignFlag ( UI_FLAG_BOTTOM | UI_FLAG_RIGHT );
		RegisterControl ( pMiniPartyWindow, true );
		ShowGroupFocus ( MINIPARTY_WINDOW );
	}

	{	//	���
//		if ( 1 || "�׽�Ʈ" )
		{
			m_pModalWindow = new CModalWindow;
			m_pModalWindow->Create ( MODAL_WINDOW, "BASIC_MODAL_WINDOW", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			m_pModalWindow->CreateSubControl ();
			RegisterControl ( m_pModalWindow );
			ShowGroupFocus ( MODAL_WINDOW );
		}
//		else
//		{
//			m_pModalWindow = new CModalWindow;
//			m_pModalWindow->Create ( MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
//			m_pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
//			m_pModalWindow->CreateSubControl ();
//			m_pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
//			RegisterControl ( m_pModalWindow );
////			m_pModalWindow[i] = pModalWindow;
//			ShowGroupFocus ( MODAL_WINDOW );
//
////			m_nModalWindowIndex = 0;
//		}
	}

	{
		m_pPtoPWindow = new CPtoPWindow;		
		m_pPtoPWindow->Create ( PTOPMENU_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
//		m_pPtoPWindow->Create ( PTOPMENU_WINDOW, "PTOPMENU_WINDOW" );		
		m_pPtoPWindow->CreateSubControl ();
		m_pPtoPWindow->ResetAlignFlag ();
		RegisterControl ( m_pPtoPWindow );
		ShowGroupFocus ( PTOPMENU_WINDOW );
	}

	{
		m_pDialogueWindow = new CDialogueWindow;
		m_pDialogueWindow->Create ( DIALOGUE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pDialogueWindow->CreateBaseWindowBlack ( "DIALOGUE_WINDOW", (char*)ID2GAMEWORD("DIALOGUE_WINDOW_STATIC") );
		m_pDialogueWindow->CreateSubControl ();
		m_pDialogueWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pDialogueWindow, true );
		ShowGroupFocus ( DIALOGUE_WINDOW );
	}
	
	{	//	��ų �ű��
		m_pSkillWindowToTray = new CSkillWindowToTray;
		m_pSkillWindowToTray->Create ( SKILL_WINDOW_TO_TRAY, "SKILL_WINDOW_TO_TRAY" );
		m_pSkillWindowToTray->CreateSubControl ();
		RegisterControl ( m_pSkillWindowToTray );
		ShowGroupTop ( SKILL_WINDOW_TO_TRAY );
	}

	{
		m_pItemMove = new CItemMove;
		m_pItemMove->Create ( ITEM_MOVE, "ITEM_MOVE" );
		m_pItemMove->CreateSubControl ();
		RegisterControl ( m_pItemMove );
		ShowGroupTop ( ITEM_MOVE );
	}

	{
		m_pInfoDisplay = new CBasicVarTextBox;
		m_pInfoDisplay->Create ( INFO_DISPLAY, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplay->CreateSubControl ();
		m_pInfoDisplay->SetVisibleSingle ( FALSE );
		m_pInfoDisplay->SetTextAlign( TEXT_ALIGN_LEFT );
		RegisterControl ( m_pInfoDisplay );
		ShowGroupTop ( INFO_DISPLAY );
	}

	{
		m_pInfoDisplayEx = new CBasicVarTextBox;
		m_pInfoDisplayEx->Create ( INFO_DISPLAY_EX, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplayEx->CreateSubControl ();
		m_pInfoDisplayEx->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfoDisplayEx );
		ShowGroupTop ( INFO_DISPLAY_EX );
	}

	{
		m_pAdminMessageDisplay = new CAdminMessageDisplay;
		m_pAdminMessageDisplay->Create ( ADMIN_MESSAGE_DISPLAY, "ADMIN_MESSAGE_DISPLAY" );
		m_pAdminMessageDisplay->CreateSubControl ();		
		RegisterControl ( m_pAdminMessageDisplay );
		ShowGroupTop ( ADMIN_MESSAGE_DISPLAY );

		CSimpleMessageMan*	pSimpleMessageMan = new CSimpleMessageMan;
		pSimpleMessageMan->Create ( SIMPLE_MESSAGE_MAN, "SIMPLE_MESSAGE_MAN" );
		pSimpleMessageMan->CreateSubControl ();		
		RegisterControl ( pSimpleMessageMan );
		ShowGroupBottom ( SIMPLE_MESSAGE_MAN );
		HideGroup ( SIMPLE_MESSAGE_MAN );
	}

	{	//	��� ��û ���
		m_pConftModalWindow = new CConftModalWindow;
		m_pConftModalWindow->Create ( CONFT_MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		m_pConftModalWindow->CreateBaseModal ( "CONFT_MODAL_WINDOW");
		m_pConftModalWindow->CreateSubControl ();
		m_pConftModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pConftModalWindow->LoadDefaultCondition ();
		RegisterControl ( m_pConftModalWindow );
		ShowGroupFocus ( CONFT_MODAL_WINDOW );
	}

	{	//	��Ƽ ��û ���
		m_pPartyModalWindow = new CPartyModalWindow;
		m_pPartyModalWindow->Create ( PARTY_MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		m_pPartyModalWindow->CreateBaseModal ( "PARTY_MODAL_WINDOW");
		m_pPartyModalWindow->CreateSubControl ();
		m_pPartyModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPartyModalWindow->LoadDefaultCondition ();
		RegisterControl ( m_pPartyModalWindow );
		ShowGroupFocus ( PARTY_MODAL_WINDOW );
	}

	{	//	���� �ȱ� ���� ���
		m_pPrivateMarketSellWindow = new CPrivateMarketSellWindow;
		m_pPrivateMarketSellWindow->Create ( PRIVATE_MARKET_SELL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		m_pPrivateMarketSellWindow->CreateBaseModal ( "PRIVATE_MARKET_SELL_WINDOW");
		m_pPrivateMarketSellWindow->CreateSubControl ();
		m_pPrivateMarketSellWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
//		m_pPrivateMarketSellWindow->LoadDefaultCondition ();
		RegisterControl ( m_pPrivateMarketSellWindow );
		ShowGroupFocus ( PRIVATE_MARKET_SELL_WINDOW );
	}

	{	//	�� �̵�
		m_pMapMoveDisplay = new CMapMoveDisplay;
		m_pMapMoveDisplay->Create ( MAPMOVE_DISPLAY, "BASIC_MAPMOVE_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pMapMoveDisplay->CreateSubControl ();
		RegisterControl ( m_pMapMoveDisplay );
		ShowGroupFocus ( MAPMOVE_DISPLAY );

		m_pMapMoveDisplayDummy = new CUIControl;
		m_pMapMoveDisplayDummy->Create ( MAPMOVE_DISPLAY_DUMMY, "BASIC_MAPMOVE_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        RegisterControl ( m_pMapMoveDisplayDummy );

		m_pSystemMessageWindow = new CSystemMessageWindow;
		m_pSystemMessageWindow->Create ( SYSTEM_MESSAGE_WINDOW, "BASIC_SYSTEM_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
		m_pSystemMessageWindow->CreateSubControl ();
		RegisterControl ( m_pSystemMessageWindow );
		ShowGroupTop ( SYSTEM_MESSAGE_WINDOW );
		HideGroup ( SYSTEM_MESSAGE_WINDOW );

		m_pSystemMessageWindowDummy = new CUIControl;
		m_pSystemMessageWindowDummy->Create ( SYSTEM_MESSAGE_WINDOW_DUMMY, "BASIC_SYSTEM_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
        RegisterControl ( m_pSystemMessageWindowDummy );
	}

	{	
		m_pBlockProgramAlarm = new CBlockProgramAlarm;
		m_pBlockProgramAlarm->Create ( BLOCK_PROGRAM_ALARM, "BLOCK_PROGRAM_ALARM" );		
		m_pBlockProgramAlarm->CreateSubControl ();
		m_pBlockProgramAlarm->ResetAlignFlag ();
		RegisterControl ( m_pBlockProgramAlarm, true );
		ShowGroupFocus ( BLOCK_PROGRAM_ALARM );

		m_pBlockProgramAlarmDummy = new CUIControl;
		m_pBlockProgramAlarmDummy->Create ( BLOCK_PROGRAM_ALARM_DUMMY, "BLOCK_PROGRAM_ALARM", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        RegisterControl ( m_pBlockProgramAlarmDummy );
	}

	{		
		m_pWaitServerDisplay = new CWaitServerDialogue;
		m_pWaitServerDisplay->Create ( WAITSERVER_DISPLAY, "WAITSERVER_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pWaitServerDisplay->CreateSubControl ();
		RegisterControl ( m_pWaitServerDisplay );
		ShowGroupFocus ( WAITSERVER_DISPLAY );
	}

	{	//	��� ī��Ʈ �Ŵ���
		m_pConftDisplayMan = new CConftDisplayMan;
		m_pConftDisplayMan->Create ( CONFT_DISPLAY_MAN, "CONFT_DISPLAY_MAN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pConftDisplayMan->CreateSubControl ();
		RegisterControl ( m_pConftDisplayMan );
	}

	{
		// WebBrowser �����Ǿ� ������ CHelpWindowWeb ����, ������ CHelpWindow ���� ( ���� )
		if( CCommonWeb::Get()->GetCreate() )
		{
			CHelpWindowWeb* pHelpWindow = new CHelpWindowWeb;
			pHelpWindow->Create ( HELP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pHelpWindow->CreateBaseWindowBlack ( "HELP_WINDOW", (char*)ID2GAMEWORD("HELP_WINDOW_STATIC") );
			pHelpWindow->CreateWeb ();
			pHelpWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			RegisterControl ( pHelpWindow, true );
		}
		else
		{
			CHelpWindow* pHelpWindow = new CHelpWindow;
			pHelpWindow->Create ( HELP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pHelpWindow->CreateBaseWindowBlack ( "HELP_WINDOW", (char*)ID2GAMEWORD("HELP_WINDOW_STATIC") );
			pHelpWindow->CreateSubControl ();
			pHelpWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			RegisterControl ( pHelpWindow, true );
		}
		ShowGroupFocus ( HELP_WINDOW );
	}

//#if defined ( CH_PARAM ) || defined ( TH_PARAM ) || defined( PH_PARAM ) || defined ( JP_PARAM ) || defined( _RELEASED ) // �ʸ��� �����ۼ�
	{
		if( CCommonWeb::Get()->GetCreate() )
		{
			CItemShopWindowWeb* pItemShopWindow = new CItemShopWindowWeb;
			pItemShopWindow->Create ( ITEMSHOP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pItemShopWindow->CreateBaseWindowBlack ( "ITEMSHOP_WINDOW", (char*)ID2GAMEWORD("ITEMSHOP_WINDOW_STATIC") );
			pItemShopWindow->CreateWeb ();
			pItemShopWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			RegisterControl ( pItemShopWindow, true );			
		}
		ShowGroupFocus ( ITEMSHOP_WINDOW );
	}
//#endif

	{		
		/*m_pQuestWindow = new CQuestWindow;
		m_pQuestWindow->Create ( QUEST_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pQuestWindow->CreateBaseWindowBlack ( "QUEST_WINDOW", (char*)ID2GAMEWORD("QUEST_WINDOW_STATIC") );
		m_pQuestWindow->CreateSubControl ();
		m_pQuestWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );		
		RegisterControl ( m_pQuestWindow, true );
		ShowGroupFocus ( QUEST_WINDOW );*/
		m_pQuestWindow = new CQuestWindow;
		m_pQuestWindow->Create ( QUEST_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pQuestWindow->CreateBaseWindowBlack ( "QUEST_WINDOW", (char*)ID2GAMEWORD("QUEST_WINDOW_STATIC") );
		m_pQuestWindow->CreateSubControl ();
		m_pQuestWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );		
		RegisterControl ( m_pQuestWindow, true );
		ShowGroupFocus ( QUEST_WINDOW );
	}

	{
		m_pSimpleHP = new CSimpleHP;
		m_pSimpleHP->Create ( SIMPLE_HP, "SIMPLE_HP" );
		m_pSimpleHP->SetUseSummonHP(FALSE);
		m_pSimpleHP->CreateSubControl ();
		RegisterControl ( m_pSimpleHP );
		ShowGroupBottom ( SIMPLE_HP );
		HideGroup ( SIMPLE_HP );


		m_pSummonHP = new CSimpleHP;
		m_pSummonHP->Create ( SUMMON_HP, "SIMPLE_HP" );
		m_pSummonHP->SetUseSummonHP(TRUE);
		m_pSummonHP->CreateSubControl ();
		RegisterControl ( m_pSummonHP );
		ShowGroupBottom ( SUMMON_HP );
		HideGroup ( SUMMON_HP );


		m_pBasicInfoView = new CBasicInfoView;
		m_pBasicInfoView->Create ( BASIC_INFO_VIEW, "BASIC_INFO_VIEW" );
		m_pBasicInfoView->CreateSubControl ();	
		RegisterControl ( m_pBasicInfoView, true );	
		ShowGroupFocus ( BASIC_INFO_VIEW );
	}

	{
		m_pBonusTimeGauge = new CBonusTimeGauge;
		m_pBonusTimeGauge->Create ( BONUSTIME_EVENT_GAUGE, "BONUSTIME_EVENT_GAUGE", UI_FLAG_RIGHT );
		m_pBonusTimeGauge->CreateSubControl ();
		RegisterControl ( m_pBonusTimeGauge );
		ShowGroupBottom ( BONUSTIME_EVENT_GAUGE );
		HideGroup ( BONUSTIME_EVENT_GAUGE );

		m_pBonusTimeDisplay = new CBonusTimeDisplay;
		m_pBonusTimeDisplay->Create ( BONUSTIME_EVENT_DISPLAY, "BONUSTIME_EVENT_DISPLAY");
		m_pBonusTimeDisplay->CreateSubControl ();
		RegisterControl ( m_pBonusTimeDisplay );
		ShowGroupTop ( BONUSTIME_EVENT_DISPLAY );
		HideGroup( BONUSTIME_EVENT_DISPLAY );
	}


	{
		m_pFriendWindow = new CFriendWindow;
		m_pFriendWindow->Create ( FRIEND_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pFriendWindow->CreateBaseWindowLightGray ( "RNFRIEND_WINDOW", (char*)ID2GAMEWORD("FRIEND_WINDOW_NAME_STATIC") );
		m_pFriendWindow->CreateSubControl ();
		m_pFriendWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pFriendWindow, true );
		ShowGroupFocus ( FRIEND_WINDOW );

		m_pClubWindow = new CClubWindow;
		m_pClubWindow->Create ( CLUB_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubWindow->CreateBaseWindowBlack ( "CLUB_WINDOW", (char*)ID2GAMEWORD("CLUB_WINDOW_NAME_STATIC") );
		m_pClubWindow->CreateSubControl ();
		m_pClubWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );		
		RegisterControl ( m_pClubWindow, true );
		ShowGroupFocus ( CLUB_WINDOW );
	}

	{
		m_pSubMasterSet = new CSubMasterSet;
		m_pSubMasterSet->Create( SUBMASTER_MAKE, "SUBMASTER_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pSubMasterSet->CreateSubControl();
		RegisterControl ( m_pSubMasterSet );
		ShowGroupFocus ( SUBMASTER_MAKE );
	}

	{
		CReceiveNoteWindow* pReceiveNote = new CReceiveNoteWindow;
		pReceiveNote->Create( RECEIVE_NOTE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pReceiveNote->CreateBaseWindowBlack( "RECEIVE_NOTE_WINDOW", (char*)ID2GAMEWORD("FRIEND_TAB_BUTTON", 5) );
		pReceiveNote->CreateSubControl();
		pReceiveNote->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
		RegisterControl( pReceiveNote );
		ShowGroupFocus ( RECEIVE_NOTE_WINDOW );
	}

	{
		CWriteNoteWindow* pWirteNote = new CWriteNoteWindow;
		pWirteNote->Create( WRITE_NOTE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pWirteNote->CreateBaseWindowBlack( "WRITE_NOTE_WINDOW", (char*)ID2GAMEWORD("FRIEND_TAB_BUTTON", 4) );
		pWirteNote->CreateSubControl();
		pWirteNote->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
		RegisterControl( pWirteNote );
		ShowGroupFocus ( WRITE_NOTE_WINDOW );
	}

#if defined(KRT_PARAM)
	{
		CSMSSendWindow* pSMSSendWindow = new CSMSSendWindow;
		pSMSSendWindow->Create( SMS_SEND_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pSMSSendWindow->CreateBaseWindowBlack( "SMS_SEND_WINDOW", (char*)ID2GAMEWORD("FRIEND_BUTTON", 12) );
		pSMSSendWindow->CreateSubControl();
		pSMSSendWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
		RegisterControl( pSMSSendWindow );
		ShowGroupFocus ( SMS_SEND_WINDOW );
	}
#endif

	{
		CHairColorCard * pHairColorCard = new CHairColorCard;
		pHairColorCard->Create( HAIRCOLORCARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pHairColorCard->CreateBaseWindowBlack( "HAIRCOLORCARD_WINDOW", NULL );
		pHairColorCard->CreateSubControl();
		pHairColorCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		{	// Note : ���� ������ Ÿ��Ʋ�� ���⼭ �Է��ߴ�.
			// Ÿ��Ʋ �Է��� ������ ����� ����ǹǷ�
			// 80�� �Ѵ� ��Ʈ���� ©���� ������ �����.
			// �׷��� �����찡 �������� �� �Ŀ� Ÿ��Ʋ�� �����ϸ� ���������� ��µȴ�.
			pHairColorCard->SetTitleName( (char*)ID2GAMEWORD("HAIRCOLORCARD_WINDOW_TITLE") );
		}
		RegisterControl( pHairColorCard );
		//ShowGroupFocus( HAIRCOLORCARD_WINDOW );

		CPetColorCard * pPetColorCard = new CPetColorCard;
		pPetColorCard->Create( PETCOLORCARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pPetColorCard->CreateBaseWindowBlack( "HAIRCOLORCARD_WINDOW", NULL );
		pPetColorCard->CreateSubControl();
		pPetColorCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pPetColorCard->SetTitleName( (char*)ID2GAMEWORD("PETCOLORCARD_WINDOW_TITLE") );
		RegisterControl( pPetColorCard );
		//ShowGroupFocus( PETCOLORCARD_WINDOW );

		CBikeColorCard * pBikeColorCard = new CBikeColorCard;
		pBikeColorCard->Create( BIKECOLORCARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBikeColorCard->CreateBaseWindowBlack( "HAIRCOLORCARD_WINDOW", NULL );
		pBikeColorCard->CreateSubControl();
		pBikeColorCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pBikeColorCard->SetTitleName( (char*)ID2GAMEWORD("BIKECOLORCARD_WINDOW_TITLE") );
		RegisterControl( pBikeColorCard );
		//ShowGroupFocus( PETCOLORCARD_WINDOW );

		//add itemcolor
		CItemColorCard * pItemColorCard = new CItemColorCard;
		pItemColorCard->Create( ITEMCOLORCARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pItemColorCard->CreateBaseWindowBlack( "HAIRCOLORCARD_WINDOW", NULL );
		pItemColorCard->CreateSubControl();
		pItemColorCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pItemColorCard->SetTitleName( (char*)ID2GAMEWORD("ITEMCOLORCARD_WINDOW_TITLE") );
		RegisterControl( pItemColorCard );

	}
	
	{
		CHairStyleCard * pHairStyleCard = new CHairStyleCard;
		pHairStyleCard->Create( HAIRSTYLECARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pHairStyleCard->CreateBaseWindowBlack( "HAIRSTYLECARD_WINDOW", NULL );
		pHairStyleCard->CreateSubControl();
		pHairStyleCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		{	// Note : ���� ������ Ÿ��Ʋ�� ���⼭ �Է��ߴ�.
			// Ÿ��Ʋ �Է��� ������ ����� ����ǹǷ�
			// 80�� �Ѵ� ��Ʈ���� ©���� ������ �����.
			// �׷��� �����찡 �������� �� �Ŀ� Ÿ��Ʋ�� �����ϸ� ���������� ��µȴ�.
			pHairStyleCard->SetTitleName( (char*)ID2GAMEWORD("HAIRSTYLECARD_WINDOW_TITLE") );
		}
		RegisterControl( pHairStyleCard );

		CGenderChangeWindow * pGenderChangeWindow = new CGenderChangeWindow;
		pGenderChangeWindow->Create( GENDER_CHANGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pGenderChangeWindow->CreateBaseWindowBlack( "GENDER_CHANGE_WINDOW", NULL );
		pGenderChangeWindow->CreateSubControl();
		pGenderChangeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		{	
			pGenderChangeWindow->SetTitleName( (char*)ID2GAMEWORD("GENDER_CHANGE_WINDOW_TITLE") );
		}
		RegisterControl( pGenderChangeWindow );
		

		CPetStyleCard * pPetStyleCard = new CPetStyleCard;
		pPetStyleCard->Create( PETSTYLECARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pPetStyleCard->CreateBaseWindowBlack( "HAIRSTYLECARD_WINDOW", NULL );
		pPetStyleCard->CreateSubControl();
		pPetStyleCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pPetStyleCard->SetTitleName( (char*)ID2GAMEWORD("PETSTYLECARD_WINDOW_TITLE") );
		RegisterControl( pPetStyleCard );
		//ShowGroupFocus( PETSTYLECARD_WINDOW );
	}

	{
		m_pLargeMapWindow = new CLargeMapWindow;
		m_pLargeMapWindow->Create ( LARGEMAP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pLargeMapWindow->CreateBaseWindowBlack ( "LARGEMAP_WINDOW", (char*)ID2GAMEWORD("LARGEMAP_WINDOW_STATIC") );
		m_pLargeMapWindow->CreateSubControl ();
		m_pLargeMapWindow->SetProtectSizePoint ();
		m_pLargeMapWindow->SetAlignFlag ( UI_FLAG_RIGHT | UI_FLAG_TOP );
		RegisterControl ( m_pLargeMapWindow, true );
		ShowGroupFocus ( LARGEMAP_WINDOW );
	}

	{	// ITEMREBUILD_MARK
		m_pItemRebuildWindow = new CItemRebuild;
		m_pItemRebuildWindow->Create( ITEM_REBUILD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemRebuildWindow->CreateBaseWindowBlack( "ITEM_REBUILD_WINDOW", NULL );
		m_pItemRebuildWindow->CreateSubControl();
		m_pItemRebuildWindow->ResetAlignFlag();
		m_pItemRebuildWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemRebuildWindow->SetTitleName( ID2GAMEINTEXT("ITEMREBUILD_WINDOW_TITLE") );
		RegisterControl( m_pItemRebuildWindow, true );
		ShowGroupFocus( ITEM_REBUILD_WINDOW );
	}

	{
		m_pRebuildInventoryWindow = new CRebuildInventoryWindow;
		m_pRebuildInventoryWindow->Create( REBUILDINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pRebuildInventoryWindow->CreateBaseWindowBlack( "REBUILDINVENTORY_WINDOW", NULL );
		m_pRebuildInventoryWindow->CreateSubControl();
		m_pRebuildInventoryWindow->ResetAlignFlag();
		m_pRebuildInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pRebuildInventoryWindow->SetTitleName( ID2GAMEINTEXT("REBUILDINVENTORY_WINDOW_TITLE") );
		RegisterControl( m_pRebuildInventoryWindow, true );
		ShowGroupFocus( REBUILDINVENTORY_WINDOW );
		HideGroup( REBUILDINVENTORY_WINDOW );
	}

	{	// ITEMREBUILD_MARK
		m_pItemGarbageWindow = new CItemGarbage;
		m_pItemGarbageWindow->Create( ITEM_GARBAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemGarbageWindow->CreateBaseWindowBlack( "ITEM_GARBAGE_WINDOW", NULL );
		m_pItemGarbageWindow->CreateSubControl();
		m_pItemGarbageWindow->ResetAlignFlag();
		m_pItemGarbageWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemGarbageWindow->SetTitleName( ID2GAMEINTEXT("ITEMGARBAGE_WINDOW_TITLE") );
		RegisterControl( m_pItemGarbageWindow, true );
		ShowGroupFocus( ITEM_GARBAGE_WINDOW );
	}

	{
		m_pGarbageInventoryWindow = new CGarbageInventoryWindow;
		m_pGarbageInventoryWindow->Create( GARBAGEINVENTORY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pGarbageInventoryWindow->CreateBaseWindowBlack( "GARBAGEINVENTORY_WINDOW", NULL );
		m_pGarbageInventoryWindow->CreateSubControl();
		m_pGarbageInventoryWindow->ResetAlignFlag();
		m_pGarbageInventoryWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pGarbageInventoryWindow->SetTitleName( ID2GAMEINTEXT("GARBAGEINVENTORY_WINDOW_TITLE") );
		RegisterControl( m_pGarbageInventoryWindow, true );
		ShowGroupFocus( GARBAGEINVENTORY_WINDOW );
		HideGroup( GARBAGEINVENTORY_WINDOW );
	}

	

	{
	
		m_pPetRebirthDialogue = new CPetRebirthDialogue;	// Monster7j
		m_pPetRebirthDialogue->Create ( PET_REBIRTH_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pPetRebirthDialogue->CreateBaseWindowBlack ( "PET_REBIRTH_DIALOGUE", (char*)ID2GAMEWORD("PET_REBIRTH_NAME_STATIC") );
		m_pPetRebirthDialogue->CreateSubControl ();
		RegisterControl ( m_pPetRebirthDialogue ,true);
		ShowGroupFocus ( PET_REBIRTH_DIALOGUE );
	}

	{
		m_pGambleBox = new CGambleBox;	// Monster7j
		m_pGambleBox->Create ( GAMBLE_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pGambleBox->CreateBaseWindowBlack ( "GAMBLE_DIALOGUE", (char*)ID2GAMEWORD("GAMBLE_NAME_STATIC") );
		m_pGambleBox->CreateSubControl ();
		RegisterControl ( m_pGambleBox ,true);
		ShowGroupFocus ( GAMBLE_DIALOGUE );
	}
	
	{
		m_pGambleSelectBox = new CGambleSelectBox;	// Monster7j
		m_pGambleSelectBox->Create ( GAMBLE_SELECT_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pGambleSelectBox->CreateBaseWindowBlack ( "GAMBLE_SELECT_DIALOGUE", (char*)ID2GAMEWORD("GAMBLE_NAME_STATIC") );
		m_pGambleSelectBox->CreateSubControl ();
		RegisterControl ( m_pGambleSelectBox ,true);
		ShowGroupFocus ( GAMBLE_SELECT_DIALOGUE );
	}

	{
		m_pGambleAgainBox = new CGambleAgainBox;	// Monster7j
		m_pGambleAgainBox->Create ( GAMBLE_AGAIN_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pGambleAgainBox->CreateBaseWindowBlack ( "GAMBLE_AGAIN_DIALOGUE", (char*)ID2GAMEWORD("GAMBLE_NAME_STATIC") );
		m_pGambleAgainBox->CreateSubControl ();
		RegisterControl ( m_pGambleAgainBox ,true);
		ShowGroupFocus ( GAMBLE_AGAIN_DIALOGUE );
	}
	
	{
		m_pGambleAnimationBox = new CGambleAnimationBox;
		m_pGambleAnimationBox->CreateEx( GAMBLE_ANIMATION_DIALOGUE, "GAMBLE_ANIMATION_DIALOGUE" );
		m_pGambleAnimationBox->CreateSubControl();
		RegisterControl( m_pGambleAnimationBox );
		ShowGroupFocus ( GAMBLE_ANIMATION_DIALOGUE );
	}

	{
		m_pPetSkinMixImage = new CPetSkinMixImage;
		m_pPetSkinMixImage->CreateEx( PETSKIN_MIX_IMAGE_WINDOW, "PETSKIN_MIX_IMAGE_WINDOW" );
		m_pPetSkinMixImage->CreateSubControl();
		m_pPetSkinMixImage->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl( m_pPetSkinMixImage );
		ShowGroupFocus ( PETSKIN_MIX_IMAGE_WINDOW );
	}

	{
		m_pGambleResultOddBox = new CGambleResultOddBox;
		m_pGambleResultOddBox->CreateEx( GAMBLE_RESULT_ODD_DIALOGUE, "GAMBLE_RESULT_ODD_DIALOGUE" );
		m_pGambleResultOddBox->CreateSubControl();
		RegisterControl( m_pGambleResultOddBox );
		ShowGroupFocus ( GAMBLE_RESULT_ODD_DIALOGUE );
	}

	{
		m_pGambleResultEvenBox = new CGambleResultEvenBox;
		m_pGambleResultEvenBox->CreateEx( GAMBLE_RESULT_EVEN_DIALOGUE, "GAMBLE_RESULT_EVEN_DIALOGUE" );
		m_pGambleResultEvenBox->CreateSubControl();
		RegisterControl( m_pGambleResultEvenBox );
		ShowGroupFocus ( GAMBLE_RESULT_EVEN_DIALOGUE );
	}

	{
		m_pBusWindow = new CBusWindow;
		m_pBusWindow->Create ( BUS_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBusWindow->CreateBaseWindowBlack ( "BUS_WINDOW", (char*)ID2GAMEWORD("BUS_WINDOW_NAME_STATIC") );
		//m_pBusWindow->CreateBaseWindowBlack ( "BUS_WINDOW", (char*)ID2GAMEWORD("BUS_WINDOW_NAME_STATIC") );
		m_pBusWindow->CreateSubControl ();
		m_pBusWindow->ResetAlignFlag ();
		RegisterControl ( m_pBusWindow, true );
		ShowGroupFocus ( BUS_WINDOW );
	}

	{
		m_pAcademyConftDisplay = new CAcademyConftDisplay;
		m_pAcademyConftDisplay->Create ( ACADEMY_CONFT_DISPLAY, "ACADEMY_CONFT_DISPLAY", UI_FLAG_CENTER_X );
		m_pAcademyConftDisplay->CreateSubControl ();
		RegisterControl ( m_pAcademyConftDisplay );
		ShowGroupTop ( ACADEMY_CONFT_DISPLAY );
		HideGroup ( ACADEMY_CONFT_DISPLAY );
	}

	{	//	��������� ����
		//m_pItemBankWindow = m_pInventoryWindow-;		
		//m_pItemBankWindow->Create ( ITEMBANK_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//m_pItemBankWindow->CreateBaseWindowLightGray ( "ITEMBANK_WINDOW", (char*)ID2GAMEWORD("INVENTORY_NAME_STATIC") );
		//m_pItemBankWindow->CreateSubControl ();
		//m_pItemBankWindow->INVENTORY_BANK();
		//m_pItemBankWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		//RegisterControl ( m_pItemBankWindow, true );
		//ShowGroupFocus ( ITEMBANK_WINDOW );	
	}

	

#if defined(VN_PARAM) //vietnamtest%%%
	{	//	��Ʈ�� Ž�� ���� �κ��丮
		m_pVNGainSysInventory = new CVNGainSysInventory;		
		m_pVNGainSysInventory->Create ( VNGAINSYS_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pVNGainSysInventory->CreateBaseWindowBlack ( "VNGAINSYS_WINDOW", (char*)ID2GAMEWORD("VNGAINSYSTEM_NAME_STATIC") );
		m_pVNGainSysInventory->CreateSubControl ();
		m_pVNGainSysInventory->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pVNGainSysInventory, true );
		ShowGroupFocus ( VNGAINSYS_WINDOW );
//		HideGroup ( VNGAINSYS_WINDOW );
	}

	{	//	��Ʈ�� Ž�� ���� ������
		m_pVNGainSysGauge = new CVNGainSysGauge;		
		m_pVNGainSysGauge->Create ( VNGAINSYS_GAUGE_WINDOW, "VNGAINSYS_GAUGE_WINDOW", UI_FLAG_RIGHT );
		m_pVNGainSysGauge->CreateSubControl ();
		RegisterControl ( m_pVNGainSysGauge );
		ShowGroupBottom ( VNGAINSYS_GAUGE_WINDOW );
//		ShowGroupFocus ( VNGAINSYS_GAUGE_WINDOW );
	}
#endif

	
	{	
		m_pPrivateMarketWindow = new CPrivateMarketWindow;		
		m_pPrivateMarketWindow->Create ( PRIVATE_MARKET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPrivateMarketWindow->CreateBaseWindowBlack ( "PRIVATE_MARKET_WINDOW", (char*)ID2GAMEWORD("PRIVATE_MARKET_NAME_STATIC") );
		m_pPrivateMarketWindow->CreateSubControl ();
		m_pPrivateMarketWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl ( m_pPrivateMarketWindow, true );
		ShowGroupFocus ( PRIVATE_MARKET_WINDOW );
		HideGroup ( PRIVATE_MARKET_WINDOW );
	}

	{
		m_pStorageChargeCard = new CStorageChargeCard;
		m_pStorageChargeCard->Create ( STORAGE_CHARGE_CARD, "STORAGE_CHARGE_CARD", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pStorageChargeCard->CreateSubControl ();
		RegisterControl ( m_pStorageChargeCard );
		ShowGroupFocus ( STORAGE_CHARGE_CARD );
	}

	{
		m_pRebirthDialogue = new CRebirthDialogue;
		m_pRebirthDialogue->Create ( REBIRTH_DIALOGUE, "REBIRTH_DIALOGUE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pRebirthDialogue->CreateSubControl ();
		RegisterControl ( m_pRebirthDialogue );
		ShowGroupFocus ( REBIRTH_DIALOGUE );
	}

	{
		CWarningMsg* pWarningMsg = new CWarningMsg;
		pWarningMsg->CreateEx( WARNING_MSG_WINDOW, "WARNING_MSG_WINDOW" );
		pWarningMsg->CreateSubControl();
		RegisterControl( pWarningMsg );
	}

	{
		m_pCountMsgWindow = new CCountMsg;
		m_pCountMsgWindow->Create( COUNT_MSG_WINDOW, "COUNT_MSG_WINDOW", UI_FLAG_CENTER_Y );
		m_pCountMsgWindow->CreateSubControl();
		RegisterControl( m_pCountMsgWindow );
	}

	{
		m_pPrivateMarketMake = new CPrivateMarketMake;
		m_pPrivateMarketMake->Create ( PRIVATE_MARKET_MAKE, "PRIVATE_MARKET_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPrivateMarketMake->CreateSubControl ();
		RegisterControl ( m_pPrivateMarketMake );
		ShowGroupFocus ( PRIVATE_MARKET_MAKE );		

		m_pClubMake = new CClubMake;
		m_pClubMake->Create ( CLUB_MAKE, "PRIVATE_MARKET_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pClubMake->CreateSubControl ();
		RegisterControl ( m_pClubMake );
		ShowGroupFocus ( CLUB_MAKE );		
	}

	{
		m_pQuestionItemDisplay = new CQuestionItemDisplay;		
		m_pQuestionItemDisplay->Create ( QUESTION_ITEM_DISPLAY, "QUESTION_ITEM_DISPLAY" );
		m_pQuestionItemDisplay->CreateSubControl ();
		RegisterControl ( m_pQuestionItemDisplay );
		ShowGroupTop ( QUESTION_ITEM_DISPLAY );
	}

	{
		m_pConftConfirm = new CConftConfirm;
		m_pConftConfirm->Create ( CONFT_CONFIRM, "CONFT_CONFIRM", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pConftConfirm->CreateSubControl ();
		RegisterControl ( m_pConftConfirm );
		ShowGroupTop ( CONFT_CONFIRM );
		HideGroup ( CONFT_CONFIRM );
	}

	{	//	��
		m_pPetWindow = new CPetWindow;
		m_pPetWindow->Create ( PET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPetWindow->CreateBaseWindowLightGray ( "PET_WINDOW", (char*)ID2GAMEWORD("PET_NAME_STATIC") );
		m_pPetWindow->CreateSubControl ();
		m_pPetWindow->SetAlignFlag ( UI_FLAG_CENTER_Y );
		RegisterControl ( m_pPetWindow, true );
		ShowGroupFocus ( PET_WINDOW );

		CPetSkillWindow* pPetSkillWindow = new CPetSkillWindow;
		pPetSkillWindow->Create ( PET_SKILL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pPetSkillWindow->CreateBaseWindowBlack ( "PET_SKILL_WINDOW", (char*)ID2GAMEWORD("PET_NAME_STATIC", 1) );
		pPetSkillWindow->CreateSubControl ();
		pPetSkillWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		RegisterControl( pPetSkillWindow, true );
		ShowGroupFocus( PET_SKILL_WINDOW );
	}


	{	//  ��ȯ��
		m_pSummonWindow = new CSummonWindow;
		m_pSummonWindow->Create ( SUMMON_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pSummonWindow->CreateBaseWindowBlack( "SUMMON_WINDOW", (char*)ID2GAMEWORD("SUMMON_WINDOW_STATIC",0 ) );
		m_pSummonWindow->CreateSubControl ();
		m_pSummonWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pSummonWindow, true );
		ShowGroupFocus ( SUMMON_WINDOW );

		// ��ȯ�� ���� ǥ��
		CSummonPosionDisplay* pSummonPosionDisplay = new CSummonPosionDisplay;
		pSummonPosionDisplay->Create ( SUMMON_POSION_DISPLAY, "SUMMON_POSION_DISPLAY" );
		pSummonPosionDisplay->CreateSubControl ();
		RegisterControl ( pSummonPosionDisplay );
		ShowGroupBottom ( SUMMON_POSION_DISPLAY );

	}

	
	{	// Ż�� 
		
		//m_pVehicleWindow = new CVehicleWindow;
		//m_pVehicleWindow->Create ( VEHICLE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//m_pVehicleWindow->CreateBaseWindowLightGray ( "VEHICLE_WINDOW", (char*)ID2GAMEWORD("VEHICLE_NAME_STATIC",0 ) );
		//m_pVehicleWindow->CreateSubControl ();
		//m_pVehicleWindow->SetAlignFlag ( UI_FLAG_CENTER_Y );
		//RegisterControl ( m_pVehicleWindow, true );
		//ShowGroupFocus ( VEHICLE_WINDOW );

	}


	{	// ������ �˻�
		m_pShopItemSearchWindow = new CShopItemSearchWindow;
		m_pShopItemSearchWindow->Create ( ITEM_SHOP_SEARCH_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pShopItemSearchWindow->CreateBaseWindowBlack ( "ITEM_SHOP_SEARCH_WINDOW", (char*)ID2GAMEWORD("ITEM_SHOP_SEARCH_STATIC",0 ) );
		m_pShopItemSearchWindow->CreateSubControl ();
		m_pShopItemSearchWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pShopItemSearchWindow, true );
		ShowGroupFocus ( ITEM_SHOP_SEARCH_WINDOW );
	
		// ������ �˻� ���
		m_pItemSearchResultWindow = new CItemSearchResultWindow;
		m_pItemSearchResultWindow->Create ( ITEM_SEARCH_RESULT_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemSearchResultWindow->CreateBaseWindowBlack ( "ITEM_SEARCH_RESULT_WINDOW", (char*)ID2GAMEWORD("ITEM_SHOP_SEARCH_STATIC",0 ) );
		m_pItemSearchResultWindow->CreateSubControl ();
		m_pItemSearchResultWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pItemSearchResultWindow, true );
		ShowGroupFocus ( ITEM_SEARCH_RESULT_WINDOW );
	}


	{
		m_pBasicInfoViewDummy = new CUIControl;
        m_pBasicInfoViewDummy->Create ( NO_ID, "BASIC_INFO_VIEW" );
		RegisterControl ( m_pBasicInfoViewDummy );
		m_pBasicInfoViewDummy->SetVisibleSingle( FALSE );

		m_pLeftTopGroupDummy = new CUIControl;
//#ifndef CH_PARAM // �߱� �������̽� ����
		m_pLeftTopGroupDummy->Create( NO_ID, "LEFTTOP_CONTROL_GROUP" );
//#else
//		m_pLeftTopGroupDummy->Create( NO_ID, "BASIC_QUICK_SKILL_SLOT" );
//#endif
		RegisterControl ( m_pLeftTopGroupDummy );
		m_pLeftTopGroupDummy->SetVisibleSingle( FALSE );

	}

/* // �߱� �������̽� ����
#ifdef CH_PARAM
		m_pBasicPotionTrayDummy = new CUIControl;
		m_pBasicPotionTrayDummy->CreateEx( NO_ID, "LEFTTOP_CONTROL_GROUP" );
		RegisterControl ( m_pBasicPotionTrayDummy );
		m_pBasicPotionTrayDummy->SetVisibleSingle( FALSE );

		m_pQuestAlarmDummy = new CUIControl;
		m_pQuestAlarmDummy->Create( NO_ID, "QUEST_ALARM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		RegisterControl( m_pQuestAlarmDummy );
		m_pQuestAlarmDummy->SetVisibleSingle( FALSE );
#endif
*/

#if defined( TH_PARAM ) || defined( MYE_PARAM ) || defined(MY_PARAM)
	{	// �±� ���̹����� ��ũ
		m_pThaiCCafeMark = new CThaiCCafeMark;		
		m_pThaiCCafeMark->Create ( THAI_CCAFE_MARK, "THAI_CCAFE_MARK", UI_FLAG_RIGHT );
		m_pThaiCCafeMark->CreateSubControl ();
		RegisterControl ( m_pThaiCCafeMark );
		ShowGroupBottom ( THAI_CCAFE_MARK );	
	}
#endif

//#if defined ( JP_PARAM ) || defined ( _RELEASED ) //add itemshop icon
	{
		// ItemShopIcon
		m_pItemShopIconMan = new CItemShopIconMan;
		m_pItemShopIconMan->Create ( ITEM_SHOP_ICON_MAN, "ITEM_SHOP_ICON_MAN" );		
		RegisterControl ( m_pItemShopIconMan );
		ShowGroupBottom ( ITEM_SHOP_ICON_MAN );
	}
//#endif 

	{
		m_pAttendanceBookWindow = new CAttendanceBookWindow;
		m_pAttendanceBookWindow->Create ( ATTENDANCE_BOOK_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pAttendanceBookWindow->CreateBaseWindowBlack ( "ATTENDANCE_BOOK_WINDOW", (char*)ID2GAMEWORD("ATTENDANCE_BOOK", 0) );
		m_pAttendanceBookWindow->CreateSubControl ();
		m_pAttendanceBookWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		RegisterControl ( m_pAttendanceBookWindow, true );
		ShowGroupFocus ( ATTENDANCE_BOOK_WINDOW );	
	}


	{
		CFaceStyleCard * pFaceChangeWindow = new CFaceStyleCard;
		pFaceChangeWindow->Create( FACE_CHANGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pFaceChangeWindow->CreateBaseWindowBlack( "FACESTYLECARD_WINDOW", NULL );
		pFaceChangeWindow->CreateSubControl();
		pFaceChangeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pFaceChangeWindow->SetTitleName( (char*)ID2GAMEWORD("FACE_CHANGE_WINDOW_TITLE") );
		
		RegisterControl( pFaceChangeWindow );
	}

	{
		m_pMapRequireCheckWindow = new CMapRequireCheck;
		m_pMapRequireCheckWindow->Create ( MAP_REQUIRE_CHECK, "MAP_REQUIRE_CHECK_DIALOGUE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pMapRequireCheckWindow->CreateSubControl ();
		RegisterControl ( m_pMapRequireCheckWindow );
		ShowGroupFocus ( MAP_REQUIRE_CHECK );
	}


	{
		m_pClubBattleModalWindow = new CClubBattleModal;
		m_pClubBattleModalWindow->Create( CLUB_BATTLE_MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubBattleModalWindow->CreateBaseWindowBlack( "CLUB_BATTLE_MODAL_WINDOW", NULL );
		m_pClubBattleModalWindow->CreateSubControl();
		m_pClubBattleModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pClubBattleModalWindow->SetTitleName( (char*)ID2GAMEWORD("CLUB_BATTLE_MODAL_WINDOW_TITLE") );
		
		RegisterControl( m_pClubBattleModalWindow );

	}

	{
		m_pTaxiWindow = new CTaxiWindow;
		m_pTaxiWindow->Create ( TAXI_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTaxiWindow->CreateBaseWindowBlack ( "TAXI_WINDOW", (char*)ID2GAMEWORD("TAXI_WINDOW_NAME_STATIC") );
		m_pTaxiWindow->CreateSubControl ();
		m_pTaxiWindow->ResetAlignFlag ();
		RegisterControl ( m_pTaxiWindow, true );
		ShowGroupFocus ( TAXI_WINDOW );
	}

	{	// ITEMREBUILD_MARK
		m_pItemMixWindow = new CItemMixWindow;
		m_pItemMixWindow->Create( ITEM_MIX_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemMixWindow->CreateBaseWindowBlack( "ITEM_MIX_WINDOW", NULL );
		m_pItemMixWindow->CreateSubControl();
		m_pItemMixWindow->ResetAlignFlag();
		m_pItemMixWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemMixWindow->SetTitleName( ID2GAMEINTEXT("ITEM_MIX_WINDOW_TITLE") );
		RegisterControl( m_pItemMixWindow, true );
		ShowGroupFocus( ITEM_MIX_WINDOW );
	}

	{
		m_pItemMixInvenWindow = new CItemMixInvenWindow;
		m_pItemMixInvenWindow->Create( ITEM_MIX_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemMixInvenWindow->CreateBaseWindowBlack( "ITEM_MIX_INVEN_WINDOW", NULL );
		m_pItemMixInvenWindow->CreateSubControl();
		m_pItemMixInvenWindow->ResetAlignFlag();
		m_pItemMixInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemMixInvenWindow->SetTitleName( ID2GAMEINTEXT("ITEM_MIX_INVEN_WINDOW_TITLE") );
		RegisterControl( m_pItemMixInvenWindow, true );
		ShowGroupFocus( ITEM_MIX_INVEN_WINDOW );
		HideGroup( ITEM_MIX_INVEN_WINDOW );
	}

	{
		m_pGatherGauge = new CGatherGauge;
		m_pGatherGauge->Create ( GATHER_GAUGE, "GATHER_GAUGE", UI_FLAG_CENTER_X );
		m_pGatherGauge->CreateSubControl ();
		RegisterControl ( m_pGatherGauge );
		ShowGroupBottom ( GATHER_GAUGE );
		HideGroup ( GATHER_GAUGE );
	}
	{
		//add itempreview
		m_pItemPreview = new CItemPreview;
		m_pItemPreview->Create ( ITEM_PREVIEW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemPreview->CreateBaseWindowLightGray( "ITEMPREVIEW_WINDOW", (char*)ID2GAMEWORD("ITEM_PREVIEW_TITLE") );
		m_pItemPreview->CreateSubControl ();
		m_pItemPreview->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemPreview->SetProtectSizePoint ();
		RegisterControl ( m_pItemPreview , true );
		ShowGroupFocus ( ITEM_PREVIEW );
	}

	{
		m_pItemPreviewWindow = new CItemPreviewWindow;
		m_pItemPreviewWindow->Create( ITEMPREVIEW_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemPreviewWindow->CreateBaseWindowLightGray( "ITEMPREVIEW_WINDOW", NULL );
		m_pItemPreviewWindow->CreateSubControl ();
		m_pItemPreviewWindow->ResetAlignFlag ();
		m_pItemPreviewWindow->SetTitleName( ID2GAMEWORD("ITEMPREVIEW_TITLE", 0 ) );
		RegisterControl ( m_pItemPreviewWindow );
		ShowGroupFocus ( ITEMPREVIEW_WINDOW );
		HideGroup ( ITEMPREVIEW_WINDOW );
	}

	CloseAllWindow ();

	DxSoundLib::GetInstance()->CreateSound ( "QuestAlarm", ID2GAMEWORD("QUEST_ALARM"), SFX_SOUND );

	return CUIMan::InitDeviceObjects ( pd3dDevice );
}

HRESULT CInnerInterface::DeleteDeviceObjects ()
{
	GLCharacter *pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if( pCharacter ) pCharacter->UPDATE_RECORD_CHAT( TRUE );

	TEXTURE_PRE_UNLOAD ();

	ResetControl();

	return CUIMan::DeleteDeviceObjects ();
}

static LPDIRECT3DTEXTUREQ* ppPRELOAD_TEXTURE = NULL;
HRESULT	CInnerInterface::TEXTURE_PRE_LOAD ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	CString strTemp = ID2GAMEWORD("SKILL_TEXTURE",0);
	int nCOUNT = atoi(strTemp);

	if ( nCOUNT < 1 ) return S_FALSE;

	ppPRELOAD_TEXTURE = new LPDIRECT3DTEXTUREQ[nCOUNT];

	for ( int i = 0; i < nCOUNT; ++i )
	{
		CString strKeyword = ID2GAMEWORD("SKILL_TEXTURE",i+1);
		TextureManager::LoadTexture( strKeyword, pd3dDevice, ppPRELOAD_TEXTURE[i], 0, 0 );
	}

	return S_OK;
}

HRESULT	CInnerInterface::TEXTURE_PRE_UNLOAD ()
{
	HRESULT hr;
	CString strTemp = ID2GAMEWORD("SKILL_TEXTURE",0);
	int nCOUNT = atoi(strTemp);

	if ( nCOUNT < 1 ) return S_FALSE;
	
	for ( int i = 0; i < nCOUNT; ++i )
	{
		CString strKeyword = ID2GAMEWORD("SKILL_TEXTURE",i+1);
		// MEMO : ReleaseTexture���� m_pTexture�� NULL�� �����.
		hr = TextureManager::ReleaseTexture ( strKeyword, ppPRELOAD_TEXTURE[i] );
	}

	SAFE_DELETE_ARRAY ( ppPRELOAD_TEXTURE );

	return S_OK;
}

void CInnerInterface::ADD_FRIEND_NAME_TO_EDITBOX( const CString& strName )
{
	if( m_pFriendWindow ) 
		m_pFriendWindow->ADD_FRIEND_NAME_TO_EDITBOX( strName );
}

void CInnerInterface::ADD_FRIEND( const CString& strName )
{
	if( m_pFriendWindow )
		m_pFriendWindow->ADD_FRIEND( strName );
}

void CInnerInterface::FRIEND_LIST( CONST CString & strName, bool bOnline )
{
	if( m_pChat )
		m_pChat->FRIEND_LIST( strName, bOnline );
}

void CInnerInterface::UpdateClubBattleTime( float fClubBattleTime )
{
	if( m_pMiniMap )
		m_pMiniMap->UpdateClubTime( fClubBattleTime );
}

bool CInnerInterface::IsCHAT_BEGIN()
{
	if( m_pChat )
		return m_pChat->IsCHAT_BEGIN();

	return false;
}

void CInnerInterface::ClearItemBank()
{
	if( m_pInventoryWindow )
		m_pInventoryWindow->ClearItemBank();
}

void CInnerInterface::ClearItemShop()
{
	if( m_pItemShopWindow )
		m_pItemShopWindow->ClearItemShop();
}

void CInnerInterface::ClearVNGainSys()
{
	if( m_pVNGainSysInventory )
		m_pVNGainSysInventory->ClearVNGainSys();
}

void CInnerInterface::DisableMinimapTarget()
{
	if( m_pLargeMapWindow )
		m_pLargeMapWindow->VISIBLE_TARGET_POS();
}

void CInnerInterface::SET_COUNT_MSG( INT nCount )
{
	if( m_pCountMsgWindow )
	{
		m_pCountMsgWindow->SetCount( nCount );
		ShowGroupTop( COUNT_MSG_WINDOW );
	}
}

void CInnerInterface::SetDefaultPosInterface(UIGUID ControlID)
{
	CUIControl * pControl = FindControl ( ControlID );

	if( pControl )
	{
		pControl->RestoreDeviceObjects(m_pd3dDevice);
	}
}

void CInnerInterface::ReqToggleRun()
{
	GLGaeaClient::GetInstance().GetCharacter()->ReqToggleRun ();
}

VOID CInnerInterface::SET_VNGAINTYPE_GAUGE( int nPos, int nLimit )
{
	m_pVNGainSysGauge->SetGauge( nPos, nLimit );
}


void CInnerInterface::BONUS_TIME_EVENT_START( bool bCharging )
{
	m_pBonusTimeGauge->BONUS_TIME_EVENT_START( bCharging );
}
void CInnerInterface::BONUS_TIME_EVENT_END()
{
	m_pBonusTimeGauge->BONUS_TIME_EVENT_END();
}
void CInnerInterface::BONUS_TIME_BUSTER_START()
{
	m_pBonusTimeGauge->BONUS_TIME_BUSTER_START();
	ShowGroupTop( BONUSTIME_EVENT_DISPLAY );
	m_pBonusTimeDisplay->START();
}

void CInnerInterface::BONUS_TIME_BUSTER_END()
{
	m_pBonusTimeGauge->BONUS_TIME_BUSTER_END();
	ShowGroupTop( BONUSTIME_EVENT_DISPLAY );
	m_pBonusTimeDisplay->STOP();
}

void CInnerInterface::SET_QUEST_HELPER( DWORD dwQuestID )
{
	m_pQuestHelper->SetQuestID( dwQuestID );

	if ( IsVisibleGroup(CDM_RANKING_DISPLAY) )
	{
		HideGroup( QUEST_HELPER_DISPLAY );
	}

}

void CInnerInterface::RESET_QUEST_HELPER( DWORD dwQuestID )
{
	m_pQuestHelper->ReSetQuestID( dwQuestID );
}

void CInnerInterface::SetThaiCCafeClass( DWORD dwClass )
{
	if ( m_pThaiCCafeMark )	m_pThaiCCafeMark->SetClass( dwClass );
}

void CInnerInterface::DoBattleModal( const CString strClubName, bool bAlliance )
{
	if ( !m_pClubBattleModalWindow ) return;
	
    m_pClubBattleModalWindow->SetClubName( strClubName );
	m_pClubBattleModalWindow->SetAlliance( bAlliance );
	ShowGroupFocus( CLUB_BATTLE_MODAL_WINDOW );
}

BOOL CInnerInterface::ReqClubBattle( DWORD dwTime, bool bAlliance )
{
	DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();

	if ( bAlliance ) 
		return GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattle( dwGaeaID, dwTime );
	else 
		return GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattle( dwGaeaID, dwTime );
}

void CInnerInterface::OPEN_TAXI_WINDOW( WORD wPosX, WORD wPosY )
{
	if ( !m_pTaxiWindow ) return;
	m_pTaxiWindow->SetInvenPos( wPosX, wPosY );
	ShowGroupFocus( TAXI_WINDOW );
}

//jdev help
void CInnerInterface::CreateHelp()
{
	CHelpWindowWeb* pHelpWindow = new CHelpWindowWeb;
	pHelpWindow->Create ( HELP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pHelpWindow->CreateBaseWindowBlack ( "HELP_WINDOW", (char*)ID2GAMEWORD("HELP_WINDOW_STATIC") );
	pHelpWindow->CreateWeb ();
	pHelpWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
	RegisterControl ( pHelpWindow, true );		
}
//add itempreview
void CInnerInterface::ItemPreview(int nType ,SNATIVEID sItemId )
{
	ShowGroupFocus( ITEM_PREVIEW );			
}

void CInnerInterface::SHOW_COMMON_LINEINFO_SPLIT ( const CString& strText, D3DCOLOR dwColor )
{
	if ( IsMouseInControl () ) return ;
	if ( IsVisibleGroup ( INFO_DISPLAY ) ) return ;		
	if ( m_etInfoType != ET_GENERAL_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_GENERAL_INFO;
	}

	if ( !strText.GetLength() ) return ;

	if ( m_strGeneralInfoBack != strText )
	{	
		m_pInfoDisplay->ClearText ();

		std::string s = strText.GetString();
		std::string delimiter = ".";

		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) 
		{
			token = s.substr(0, pos);
			std::cout << token << std::endl;
			m_pInfoDisplay->AddTextNoSplit ( token.c_str(), dwColor );
			s.erase(0, pos + delimiter.length());
		}
		m_strGeneralInfoBack = strText;
	}

	ShowGroupTop ( INFO_DISPLAY );

	m_bUSING_INFO_DISPLAY = true;
}