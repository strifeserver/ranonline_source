#include "StdAfx.h"
#include "AdminMessageDisplay.h"
#include "BasicChat.h"
#include "BasicChatRightBody.h"
#include "BasicInfoView.h"
#include "BasicSkillTray.h"
#include "BasicSkillTrayNew.h"
#include "BlockProgramAlarm.h"
#include "ClubWindow.h"
#include "ConftModalWindow.h"
#include "DxViewPort.h"
#include "FriendWindow.h"
#include "GameTextControl.h"
#include "GLGaeaClient.h"
#include "HeadChatDisplayMan.h"
#include "InnerInterface.h"
#include "ItemSlot.h"
#include "MiniMap.h"
#include "ModalWindow.h"
#include "PartyModalWindow.h"
#include "PrivateMarketSellWindow.h"
#include "PrivateMarketShowMan.h"
#include "RANPARAM.h"
#include "RebirthDialogue.h"
#include "ForceRebirth.h"
#include "SkillWindowToTray.h"
#include "SubMasterSet.h"
#include "UILeftTopGroup.h"
#include "UITextControl.h"
#include "WaitServerDialogue.h"
#include "RanFilter.h"
#include "BasicQuickSkillWindow.h"
#include "BasicQuickSkillWindowNew.h"
#include "SkillTrayTabNew.h"
#include "CharacterWindow/CharacterWindowCharWearEx.h"
#include "CharacterWindow/CharacterWindowCharWear.h"
#include "CharacterWindow/CharacterWindowVehicle.h"
#include "CharacterWindow/CharacterWindowCharAdditionalInfo.h"
#include "BasicVarTextBoxEx.h"
#include "InventoryWindow.h"
#include "CharacterWindow/CharacterWindowPet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CInnerInterface::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{	
	BOOL bMOUSEIN_LBUP = FALSE;
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		switch ( ControlID )
		{
		case ITEM_MOVE:
		case ADMIN_MESSAGE_DISPLAY:
		case DAMAGE_MAN:
		case MAPMOVE_DISPLAY:
		case CONFT_DISPLAY_MAN:
		case HEADCHAT_MAN:
		case NAME_DISPLAY_MAN:		
		case LEFTTOP_CONTROL_GROUP:
		case SKILL_TIME_DISPLAY:
		case ACADEMY_CONFT_DISPLAY:		
		case SIMPLE_HP:
		case SIMPLE_MESSAGE_MAN:		
		case QUESTION_ITEM_DISPLAY:
		case CONFT_CONFIRM:
		case WARNING_MSG_WINDOW:
		case QUEST_HELPER_DISPLAY:
		case ITEM_SHOP_ICON_MAN:
		case CDM_RANKING_DISPLAY:
		case INFO_CTF_DISPLAY:
			break;

		case MINIMAP:
			{
				if ( UIMSG_MOUSEIN_FULLSCREENBUTTON & dwMsg )
				{
					SetCharMoveBlock ();
				}
			}
			break;

		case WAITSERVER_DISPLAY:
			{
				int nTYPE = m_pWaitServerDisplay->GetActionTYPE();
				if ( nTYPE == WAITSERVER_TOFRIEND )
				{
					SetCharMoveBlock ();
				}

				if ( UIMSG_MOUSEIN_OKBUTTON & dwMsg )
				{
					SetCharMoveBlock ();
				}
			}
			break;			

		case BLOCK_PROGRAM_ALARM:
			{
				if ( UIMSG_MOUSEIN_ALARMBUTTON & dwMsg )
				{
					SetCharMoveBlock ();
				}
			}
			break;

		case INFO_DISPLAY_LINK:
			{
				if ( UIMSG_UIVARTEXT_BUTTON_CLOSE_CLICK & dwMsg )
				{
					CLEAR_INFO_LINK ();
				}

				if ( UIMSG_UIVARTEXT_BUTTON_CLOSE_MOUSEIN & dwMsg )
				{
					SetCharMoveBlock ();
				}
			}break;
		case BASIC_CHAT_BOX:
			{
				if ( UIMSG_MOUSEIN_BLOCK_CHARACTER_MOVE & dwMsg )
				{
					SetCharMoveBlock ();
				}

				if ( RANPARAM::bMOVABLE_ON_CHAT )
				{
					//	m_bFirstControl?? ????????,
					//	???? ????????, TranslateMessage.. ?? ?? ???? ??????
					//	???????? ??????.
					//	????, ?????????????? ???????? ?? ????,
					//	m_bFirstControl?? ??????????????,
					//	?????? VisibleTest?? ???? ?????????? ???? ?????? ????????.
					//	????????, ???????? ?????? ?????????? ??????????????
					//	?????????? ????.
					if( IsOpenWindowToMoveBlock() )
					{
						SetCharMoveBlock ();
					}
				}
				else
				{
					SetCharMoveBlock ();
				}
			}
			break;

		default:
			{
				SetCharMoveBlock ();
				SetFirstItemSlot ();
			}
		}

		if ( CHECK_LB_UP_LIKE ( dwMsg ) )
		{
			bMOUSEIN_LBUP = TRUE;
		}
	}

	if ( MODAL_WINDOW <= ControlID ) //	NOTE :????????
	{
		// MEMO : ???? ?????? ???? ???? ?????? ???? ??????. ???? ????...
		//CModalWindow* pModalWindow = (CModalWindow*)FindControl( ControlID );
		if ( m_pModalWindow )
		{
			UIGUID CallerID = m_pModalWindow->GetCallerID ();
			ModalMsgProcess ( CallerID, dwMsg );

			if ( !m_pModalWindow->IsUseModaless () )
				SetCharMoveBlock ();
		}
		
		return ;
	}
	
	switch ( ControlID )
	{
	case LEFTTOP_CONTROL_GROUP:
		{
			if ( UIMSG_MOUSE_IN & dwMsg )
			{
				SetCharMoveBlock ();
			}
		}
		break;


	case GAME_MENU_OPEN_BUTTON:
		{
			if ( bMOUSEIN_LBUP )
			{
				HideGroup ( ControlID );
				ShowGroupBottom ( GAME_MENU, true );
			}
		}
		break;

	case MINIPARTY_OPEN:
		{
			if ( bMOUSEIN_LBUP )
			{
				HideGroup ( ControlID );
				ShowGroupFocus ( MINIPARTY_WINDOW );
			}
		}
		break;

	case QUEST_ALARM:
		{
			if ( bMOUSEIN_LBUP )
			{
				HideGroup ( ControlID );

//				#ifdef CH_PARAM // ???? ?????????? ????
//					UpdatePotionTrayPosition();
//				#endif

				if ( NATIVEID_NULL().dwID == m_dwEventQuestID )
				{
					ShowGroupFocus ( QUEST_WINDOW );
					REFRESH_QUEST_WINDOW ();					
				}
				else
				{
					SetQuestWindowOpen ( m_dwEventQuestID );
				}
			}
		}
		break;

	case QUICK_SKILL_TRAY_TAB_WINDOW:
	case QUICK_SKILL_TRAY_TAB_NEW_WINDOW:
		{
			CSkillWindowToTray* pSkillWindowToTray = GetSkillWindowToTray ();
			if ( pSkillWindowToTray )
			{
				pSkillWindowToTray->SetUseSnap ( (dwMsg & UIMSG_MOUSEIN_SKILLSLOT) ? TRUE : FALSE );
			}
		}
		break;

	case TRADE_WINDOW:
	case TRADEINVENTORY_WINDOW:
	case MARKET_WINDOW:	
	case STORAGE_WINDOW:
	case CLUB_STORAGE_WINDOW:
	//case ITEMBANK_WINDOW:
		{			
			BOOL bSnap = (dwMsg & UIMSG_MOUSEIN_NEWWEARSLOT) || (dwMsg & UIMSG_MOUSEIN_ITEMSLOT);
			if ( bSnap ) SetSnapItem ();

			SetCharMoveBlock ();
		}
		break;
	case PET_WINDOW:
	case INVENTORY_WINDOW:
	case CHARACTER_WINDOW:
	case CHARACTER_ADDITIONAL_WINDOW:
		{
			BOOL bSnap =  (dwMsg & UIMSG_MOUSEIN_NEWWEARSLOT) || (dwMsg & UIMSG_MOUSEIN_NEWWEARSLOTEX) || (dwMsg & UIMSG_MOUSEIN_VEHICLE_NEWWEARSLOT) ||
							(dwMsg & UIMSG_MOUSEIN_NEWWEARSLOT) ||  (dwMsg & UIMSG_MOUSEIN_NEWWEARSLOTEX) || (dwMsg & UIMSG_MOUSEIN_ITEMSLOT);
			if ( bSnap ) SetSnapItem ();
			int nSTATE = GetInventoryWindow()->GetSTATE();
			if (nSTATE == 1)

			{
				SetCharMoveBlock ();
			}
		}
		break;

	case DIALOGUE_WINDOW:
		{
			SetCharMoveBlock ();
		}
		break;

	case CONFT_MODAL_WINDOW:
		{			
			if ( m_pConftModalWindow )
			{
				UIGUID CallerID = m_pConftModalWindow->GetCallerID ();
				ModalMsgProcess ( CallerID, dwMsg );
			}
//			SetCharMoveBlock ();
		}
		break;

	case PARTY_MODAL_WINDOW:
		{			
			if ( m_pPartyModalWindow )
			{
				UIGUID CallerID = m_pPartyModalWindow->GetCallerID ();
				ModalMsgProcess ( CallerID, dwMsg );
			}
//			SetCharMoveBlock ();
		}
		break;
	
		/*
	case PET_REBIRTH_DIALOGUE: //Monster7j
		{
			if(m_pPetRebirthDialogue)
			{

			}
		}
		break;
		*/
	case REBIRTH_DIALOGUE:
	case REBIRTH_FORCE:
		{
			if ( m_pRebirthDialogue )
			{
				const UIGUID cID = m_pRebirthDialogue->GetCallerID ();
				ModalMsgProcess ( cID, dwMsg );
			}
			if ( m_pForceRebirth )
			{
				const UIGUID cID = m_pForceRebirth->GetCallerID ();
				ModalMsgProcess ( cID, dwMsg );
			}

			SetCharMoveBlock ();
		}
		break;

	case PRIVATE_MARKET_SELL_WINDOW:
		{
			if ( m_pPrivateMarketSellWindow )
			{
				const UIGUID& CallerID = m_pPrivateMarketSellWindow->GetCallerID ();
				ModalMsgProcess ( CallerID, dwMsg );
			}
		}
		break;

	case PRIVATE_MARKET_SHOW_MAN:
		{
			if ( UIMSG_MOUSEIN_MARKET_ADVERTISE_LBDNUP & dwMsg )
			{
				SetCharMoveBlock ();
			}
		}
		break;

	case CLUB_WINDOW:
		{
			if( UIMSG_MOUSEIN_SUBMASTER_MAKE & dwMsg )
			{
				if( !IsVisibleGroup( SUBMASTER_MAKE ) )
				{
					if( m_pSubMasterSet )
					{
						// ???? ?????? ???????? ?????????? ?????? ???????? ?????????????? ??????.
						DWORD dwCharID = m_pClubWindow->GetClubMemberID();
						CString strName = m_pClubWindow->GetClubMemberName();
						m_pSubMasterSet->SetSubMasterName( strName );
						m_pSubMasterSet->LoadCurrentOption( dwCharID );
						ShowGroupFocus( SUBMASTER_MAKE, true );
					}
				}
			}
			else if ( UIMSG_ADDNAME_TO_CHATBOX_FROM_CLUBWINDOW & dwMsg )
			{
				GetChat()->BEGIN_PRIVATE_CHAT ( m_pClubWindow->GetClubMemberName() );
				ShowGroupFocus ( BASIC_CHAT_BOX );
			}
		}
		break;

	case FRIEND_WINDOW:
		{
			if( UIMSG_ADDNAME_TO_CHATBOX & dwMsg )
			{
				GetChat()->BEGIN_PRIVATE_CHAT ( m_pFriendWindow->GET_FRIEND_NAME () );
				m_pChat->REFRESH_FRIEND_LIST(); // ???????? ???????????? ???? ????????.
				ShowGroupFocus ( BASIC_CHAT_BOX );
			}
			else if( UIMSG_EDIT_END & dwMsg )
			{
				m_pFriendWindow->EDIT_END ();
			}
			else if( UIMSG_RECEIVE_NOTE_VISIBLE & dwMsg )
			{
				if( !IsVisibleGroup( RECEIVE_NOTE_WINDOW ) )
				{
					//if( m_pReceiveNoteWindow )
					//{
						ShowGroupFocus( RECEIVE_NOTE_WINDOW, true );
					//}
				}
			}
			else if( UIMSG_WRITE_NOTE_VISIBLE & dwMsg )
			{
#if defined(KRT_PARAM)
				if( !IsVisibleGroup( SMS_SEND_WINDOW ) )
				{
					ShowGroupFocus( SMS_SEND_WINDOW, true );
				}
#else
				if( !IsVisibleGroup( WRITE_NOTE_WINDOW ) )
				{
					ShowGroupFocus( WRITE_NOTE_WINDOW, true );
				}
#endif
			}
		}
		break;

	case BASIC_INFO_VIEW:
		{
			if( UIMSG_BASICINFOVIEW_LBDUP & dwMsg )
			{
				BasicInfoViewDoubleClick();
			}
		}
		break;
	}
}

void	CInnerInterface::DisplayChatMessage ( int nType, const char *szName, const char *szMsg )
{
	SITEMLINK sITEM;
	DisplayChatMessage ( nType, szName, szMsg, sITEM );
}

void	CInnerInterface::DisplayChatMessage ( int nType, const char *szName, const char *szMsg, SITEMLINK sLINK  )
{
	if ( m_pChat )
	{
		CString strCombine;
		CString strTemp = szMsg;

		if ( CRanFilter::GetInstance().ChatFilter( strTemp ) )
			strTemp = CRanFilter::GetInstance().GetProverb();

		
		switch ( nType )
		{
		case CHAT_TYPE_GLOBAL:		// ?????? ?????? ??????
			{
				strCombine.Format ( "[%s]:%s", ID2GAMEWORD("ADMIN_NAME"), strTemp );	
				m_pChat->AddStringToChatEx ( strCombine, CHAT_SYSTEM, &sLINK );
			}
			break;
			//add staffchat
/*		case CHAT_TYPE_GLOBAL_STAFF:		// ?????? ?????? ??????
			{
				strCombine.Format ( "[%s]:%s", ID2GAMEWORD("ADMIN_NAME_STAFF"), strTemp );	
				m_pChat->AddStringToChatEx ( strCombine, CHAT_SYSTEM );
			}
			break;*/

		case CHAT_TYPE_NORMAL:		// ?????? ???? ??????
			{
				strCombine.Format ( "[%s]:%s", szName, strTemp );	
				m_pChat->AddStringToChatEx ( strCombine, CHAT_NORMAL, &sLINK );
			}
			break;

		case CHAT_TYPE_PARTY:		// ?????????? ???????? ??????
			{
				strCombine.Format ( "[%s]:%s", szName, strTemp );	
				m_pChat->AddStringToChatEx ( strCombine, CHAT_PARTY, &sLINK );
			}
			break;

		case CHAT_TYPE_PRIVATE:		// ???????? ??????, ????, ??????
			{
				m_pChat->AddChat ( szName, strTemp, CHAT_PRIVATE, &sLINK );
			}
			break;

        case CHAT_TYPE_GUILD:		// ?????????? ???????? ??????
			{
				strCombine.Format ( "[%s]:%s", szName, strTemp );	
				m_pChat->AddStringToChatEx ( strCombine, CHAT_GUILD, &sLINK );
			}
			break;

		case CHAT_TYPE_LOUDSPEAKER:
			{
				strCombine.Format ( "[%s]:%s", szName, strTemp );	
				m_pChat->AddStringToChatEx ( strCombine, CHAT_TOALL, &sLINK );
			}
			break;

		case CHAT_TYPE_ALLIANCE:
			{
				strCombine.Format ( "[%s]:%s", szName, strTemp );	
				m_pChat->AddStringToChatEx ( strCombine, CHAT_ALLIANCE, &sLINK );
			}
			break;
		
		case CHAT_TYPE_CTRL_GLOBAL:
			{
				strCombine.Format ( "[%s]:%s", ID2GAMEWORD("NOTIFY_NAME"), strTemp );	
				m_pChat->AddStringToChatEx ( strCombine, CHAT_SYSTEM, &sLINK );
			}
			break;

		default:
			GASSERT( FALSE );
			break;
		};

		D3DCOLOR dwIDColor = NS_UITEXTCOLOR::IDCOLOR;
		switch ( nType )
		{
		case CHAT_TYPE_GLOBAL:
			{
				if ( m_pAdminMessageDisplay )
				{
					if ( !IsVisibleGroup ( ADMIN_MESSAGE_DISPLAY ) )
						strCombine.Format ( "[%s]%s", ID2GAMEWORD("ADMIN_NAME"), strTemp );	
					else
						strCombine.Format ( "%s", strTemp );	

					m_pAdminMessageDisplay->AddText ( strCombine, NS_UITEXTCOLOR::ADMIN_COLOR );

					// ???? ?????? ???? AddText?? ?????? ????.
					/*int i, nSize = strlen(szMsg);
					for( i = 0; i < nSize; i++ )
					{
						if( szMsg[i] == '\r' ) continue;
						if( szMsg[i] == '\n' )
						{
							if( strCombine.GetLength() != 0 )
								m_pAdminMessageDisplay->AddText ( strCombine, NS_UITEXTCOLOR::ADMIN_COLOR );
							strCombine = "";
						}else{
							strCombine += szMsg[i];
						}
					}*/
					//m_pAdminMessageDisplay->AddText ( strCombine, NS_UITEXTCOLOR::ADMIN_COLOR );
					
					ShowGroupTop ( ADMIN_MESSAGE_DISPLAY );					
				}
			}
			break;
			//add staffchat
		case CHAT_TYPE_GLOBAL_STAFF:
			{
				if ( m_pAdminMessageDisplay )
				{
					if ( !IsVisibleGroup ( ADMIN_MESSAGE_DISPLAY ) )
						strCombine.Format ( "[%s]%s", ID2GAMEWORD("ADMIN_NAME_STAFF"), strTemp );	
					else
						strCombine.Format ( "%s", strTemp );	

					m_pAdminMessageDisplay->AddText ( strCombine, NS_UITEXTCOLOR::PALEGREEN );
					
					ShowGroupTop ( ADMIN_MESSAGE_DISPLAY );					
				}
			}
			break;

		case CHAT_TYPE_NORMAL:
		case CHAT_TYPE_PARTY:
		case CHAT_TYPE_GUILD:
		case CHAT_TYPE_ALLIANCE:
//		case CHAT_TYPE_PRIVATE:		
			{
				if ( m_pHeadChatDisplayMan )
				{
					D3DXVECTOR3 *pPos = GLGaeaClient::GetInstance().FindCharHeadPos( szName );
					if ( pPos )
					{
						D3DXVECTOR3 vPos = *pPos;
						D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );

						int nPosX = (int)( vScreen.x);
						int nPosY = (int)( vScreen.y);

						DWORD dwIDColor = NS_UITEXTCOLOR::IDCOLOR;

						////	???? ?? (PK_COLOR)
						//PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( sTargetID.dwID );
						//if ( !pCHAR ) return ;
						//dwColor = pCHAR->GET_PK_COLOR ();

						if ( IsPartyMember ( szName ) ) dwIDColor = NS_UITEXTCOLOR::GREENYELLOW;
						D3DXVECTOR2 vCharPos = D3DXVECTOR2 ( float(nPosX), float(nPosY ) );
						m_pHeadChatDisplayMan->AddChat ( szName, dwIDColor, strTemp, NS_UITEXTCOLOR::DEFAULT, vCharPos );
					}
				}
			}
			break;

		}
	}
}

//void CInnerInterface::TranslateActionMsgQ ( UIGUID ControlID, DWORD dwMsg )
//{
//	CUIMan::TranslateActionMsgQ ( ControlID, dwMsg );
//
//	switch ( ControlID )
//	{
//	case FRIEND_WINDOW:
//		{
//			if ( UIMSG_ADDNAME_TO_CHATBOX & dwMsg )
//			{
//				GetChat()->BEGIN_PRIVATE_CHAT ( m_pFriendWindow->GET_FRIEND_NAME () );
//				m_pChat->REFRESH_FRIEND_LIST(); // ???????? ???????????? ???? ????????.
//				ShowGroupFocus ( BASIC_CHAT_BOX );
//			}
//			else if ( UIMSG_EDIT_END & dwMsg )
//			{
//				m_pFriendWindow->EDIT_END ();
//			}
//		}
//		break;		
//
//	case CLUB_WINDOW:
//		{
//			if ( UIMSG_ADDNAME_TO_CHATBOX_FROM_CLUBWINDOW & dwMsg )
//			{
//				GetChat()->BEGIN_PRIVATE_CHAT ( m_pClubWindow->GetClubMemberName() );
//				ShowGroupFocus ( BASIC_CHAT_BOX );
//			}
//		}
//		break;	
//	}
//
//	if( MODAL_WINDOW <= ControlID ) // MEMO : ???????? ????????...
//	{
//		if( dwMsg & UIMSG_INVISIBLE )
//		{
//			CModalWindow* pModalWindow = (CModalWindow*) FindControl ( ControlID );
//			if ( pModalWindow && pModalWindow->m_bTEST_MULTI )
//			{
//				GetFocusList()->EraseControl ( ControlID );
//				DeleteControl ( ControlID );
//			}
//		}
//	}
//}