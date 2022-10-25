#include "StdAfx.h"
#include "ConftModalWindow.h"
#include "../EngineUIlib/GUInterface/BasicButton.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "BasicComboBox.h"
#include "BasicComboBoxRollOver.h"
#include "BasicTextButton.h"
#include "InnerInterface.h"
#include "RANPARAM.h"
#include "GLogicData.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"

#include "../EngineUILib/GUInterface/UIEditBox.h"
#include "../EngineUILib/GUInterface/UIEditBoxMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CConftModalWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case CONFT_MODAL_USABLEHP_COMBO_OPEN:
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{
						m_pComboBoxUsableHPRollOver->SetVisibleSingle ( TRUE );
						m_pComboBoxUsableHPRollOver->SetScrollPercent ( 0.0f );
						m_pComboBoxHPStateMyRollOver->SetVisibleSingle ( FALSE );					
						m_pComboBoxHPStateHisRollOver->SetVisibleSingle ( FALSE );
						m_pComboBoxBetTypeRollOver->SetVisibleSingle ( FALSE );

						m_RollOverID = CONFT_MODAL_USABLEHP_COMBO_ROLLOVER;
						m_bFirstLBUP = TRUE;
					}
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "DUEL_MODAL_DESC", 0 ), NS_UITEXTCOLOR::DEFAULT );
				}	
			}
		}
		break;

	case CONFT_MODAL_USABLEHP_COMBO_ROLLOVER:
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
				{	
					DWORD dwMsg = GetMessageEx ();
					dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
					ResetMessageEx ();
					AddMessageEx ( dwMsg );
					break;
				}

				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{				
					int nIndex = m_pComboBoxUsableHPRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return ;
					RANPARAM::wCONFT_RECOVER = GLCONST_CHAR::wCONFT_RC_TYPE[nIndex];
					LoadUsableHP ();
				}
			}
		}
		break;

	case CONFT_MODAL_HPSTATEMY_COMBO_OPEN:
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{
						m_pComboBoxUsableHPRollOver->SetVisibleSingle ( FALSE );
						m_pComboBoxHPStateHisRollOver->SetVisibleSingle ( FALSE );
						m_pComboBoxHPStateMyRollOver->SetVisibleSingle ( TRUE );
						m_pComboBoxHPStateMyRollOver->SetScrollPercent ( 0.0f );
						m_pComboBoxBetTypeRollOver->SetVisibleSingle ( FALSE );
						
						m_RollOverID = CONFT_MODAL_HPSTATEMY_COMBO_ROLLOVER;
						m_bFirstLBUP = TRUE;
					}
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "DUEL_MODAL_DESC", 1 ), NS_UITEXTCOLOR::DEFAULT );
				}
			}
		}
		break;

	case CONFT_MODAL_HPSTATEMY_COMBO_ROLLOVER:
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
				{	
					DWORD dwMsg = GetMessageEx ();
					dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
					ResetMessageEx ();
					AddMessageEx ( dwMsg );
					break;
				}

				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					if ( m_bReqModal )
					{
						int nIndex = m_pComboBoxHPStateMyRollOver->GetSelectIndex ();
						if ( nIndex < 0 ) return ;
						RANPARAM::fCONFT_HP_RATE = GLCONST_CHAR::fCONFT_STATE_TYPE[nIndex];
					}

					LoadHPStateMy ();
				}
			}
		}
		break;

	case CONFT_MODAL_HPSTATEHIS_COMBO_OPEN:
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{
						m_pComboBoxUsableHPRollOver->SetVisibleSingle ( FALSE );
						m_pComboBoxHPStateMyRollOver->SetVisibleSingle ( FALSE );
						m_pComboBoxHPStateHisRollOver->SetVisibleSingle ( TRUE );
						m_pComboBoxHPStateHisRollOver->SetScrollPercent ( 0.0f );
						m_pComboBoxBetTypeRollOver->SetVisibleSingle ( FALSE );
						
						m_RollOverID = CONFT_MODAL_HPSTATEHIS_COMBO_ROLLOVER;
						m_bFirstLBUP = TRUE;
					}
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "DUEL_MODAL_DESC", 2 ), NS_UITEXTCOLOR::DEFAULT );
				}
			}
		}
		break;

	case CONFT_MODAL_HPSTATEHIS_COMBO_ROLLOVER:
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
				{				
					DWORD dwMsg = GetMessageEx ();
					dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
					ResetMessageEx ();
					AddMessageEx ( dwMsg );
					break;
				}

				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					if ( m_bReqModal )
					{
						int nIndex = m_pComboBoxHPStateHisRollOver->GetSelectIndex ();
						if ( nIndex < 0 ) return ;
						RANPARAM::fCONFT_TAR_HP_RATE = GLCONST_CHAR::fCONFT_STATE_TYPE[nIndex];
					}

					LoadHPStateHis ();
				}
			}
		}
		break;

	//case CONFT_MODAL_RANGELIMIT_BUTTON:
	//	{
	//		if ( m_bReqModal )	//	신청시에만 동작
	//		{
	//			if ( CHECK_MOUSE_IN ( dwMsg ) )
	//			{
	//				if ( dwMsg & UIMSG_LB_DOWN )
	//				{
	//					if ( m_pRangeLimitButton )
	//					{
	//						RANPARAM::bCONFT_BOUND = !RANPARAM::bCONFT_BOUND;
	//						LoadRangeLimit ();
	//					}
	//				}
	//			}
	//		}
	//	}
	//	break;

	case MODAL_CONFT_BET_VAL_EDIT: //add duel bet
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{

						if ( bNowEdit ) 
						{
							bNowEdit = false;
							m_pBetValue->EndEdit();
							m_pEditVal->SetOneLineText ("Edit");
						}
						else if ( !bNowEdit ) 
						{
							if ( RANPARAM::wBetType != 0 )
							{
								bNowEdit = true;
								m_pBetValue->BeginEdit();
								m_pEditVal->SetOneLineText ("Done");
							}
						}
					}
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "DUEL_MODAL_DESC", 3 ), NS_UITEXTCOLOR::DEFAULT );
				}
			}
		}
		break;

	case CONFT_MODAL_BET_COMBO_OPEN: //add duel bet
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( CHECK_MOUSE_IN ( dwMsg ) )
				{
					if ( dwMsg & UIMSG_LB_DOWN )
					{
						m_pComboBoxUsableHPRollOver->SetVisibleSingle ( FALSE );
						m_pComboBoxHPStateMyRollOver->SetVisibleSingle ( FALSE );
						m_pComboBoxHPStateHisRollOver->SetVisibleSingle ( FALSE );
						
						m_pComboBoxBetTypeRollOver->SetVisibleSingle ( TRUE );
						m_pComboBoxBetTypeRollOver->SetScrollPercent ( 0.0f );
						
						m_RollOverID = CONFT_MODAL_BET_COMBO_ROLLOVER;
						m_bFirstLBUP = TRUE;
					}
					CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "DUEL_MODAL_DESC", 4 ), NS_UITEXTCOLOR::DEFAULT );
				}
			}
		}
		break;

	case CONFT_MODAL_BET_COMBO_ROLLOVER://add duel bet
		{
			if ( m_bReqModal )	//	신청시에만 동작
			{
				if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
				{				
					DWORD dwMsg = GetMessageEx ();
					dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
					ResetMessageEx ();
					AddMessageEx ( dwMsg );
					break;
				}

				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					if ( m_bReqModal )
					{
						int nIndex = m_pComboBoxBetTypeRollOver->GetSelectIndex ();
						if ( nIndex < 0 ) return ;
						RANPARAM::wBetType = nIndex ;
					}

					LoadBetType ();
				}
			}
		}
		break;

	case MODAL_CONFT_YES:
	case MODAL_CONFT_OK:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//	메시지
				//
				if ( m_bReqModal )
				{
					LONGLONG llMoney = GLGaeaClient::GetInstance().GetCharacter ()->m_lnMoney;
					LONGLONG llVoteP = GLGaeaClient::GetInstance().GetCharacter ()->m_lnVoteP;
					LONGLONG llPremP = GLGaeaClient::GetInstance().GetCharacter ()->m_lnPremP;
					ULONGLONG llDuelVal =  GetDuelBet();
					CString strTempMsg = "";
					CString strTempMsg2 = "";

					if ( RANPARAM::wBetType != 0 )
					{
						if ( llDuelVal == 0  ) 
						{
							strTempMsg2.Format ("0 Bet Entered. Please Select Bet Type 'None' if you dont want to BET");
							CInnerInterface::GetInstance().PrintConsoleText ( strTempMsg2.GetString() );
							break;
						}
					}

					if ( RANPARAM::wBetType == 1 )
					{
						if ( llDuelVal >= GLCONST_CHAR::llMaxDuelBetMoney ) 
						{
							llDuelVal = GLCONST_CHAR::llMaxDuelBetMoney;
							strTempMsg.Format ("Maximum Duel Bet Money is:%I64u",GLCONST_CHAR::llMaxDuelBetMoney );
							CInnerInterface::GetInstance().PrintConsoleText ( strTempMsg.GetString() );	
						}

						if ( llDuelVal > llMoney ) 
						{
							strTempMsg2.Format ("Not Enough Money to Bet Money:%I64d",llMoney);
							CInnerInterface::GetInstance().PrintConsoleText ( strTempMsg2.GetString() );
							break;
						}
					}
					else if ( RANPARAM::wBetType == 2 )
					{
						if ( llDuelVal >= GLCONST_CHAR::llMaxDuelBetVoteP ) 
						{
							llDuelVal = GLCONST_CHAR::llMaxDuelBetVoteP;
							strTempMsg.Format ("Maximum Duel Bet Vote-Points is:%I64u",GLCONST_CHAR::llMaxDuelBetVoteP );
							CInnerInterface::GetInstance().PrintConsoleText ( strTempMsg.GetString() );	
						}

						if ( llDuelVal > llVoteP ) 
						{
							strTempMsg2.Format ("Not Enough Vote-Points to Bet Vote-Points:%I64d",llVoteP);
							CInnerInterface::GetInstance().PrintConsoleText ( strTempMsg2.GetString() );
							break;
						}
					}
					else if ( RANPARAM::wBetType == 3 )
					{
						if ( llDuelVal >= GLCONST_CHAR::llMaxDuelBetPremP ) 
						{
							llDuelVal = GLCONST_CHAR::llMaxDuelBetPremP;
							strTempMsg.Format ("Maximum Duel Bet Premium-Points is:%I64u",GLCONST_CHAR::llMaxDuelBetPremP );
							CInnerInterface::GetInstance().PrintConsoleText ( strTempMsg.GetString() );	
						}

						if ( llDuelVal > llPremP ) 
						{
							strTempMsg2.Format ("Not Enough Premium-Points to Bet Premium-Points:%I64d",llPremP);
							CInnerInterface::GetInstance().PrintConsoleText ( strTempMsg2.GetString() );
							break;
						}
					}
					RANPARAM::llConft_Value = llDuelVal;	
				}

				AddMessageEx ( UIMSG_MODAL_OK );

				bNowEdit = false;
				m_pBetValue->EndEdit();
				m_pEditVal->SetOneLineText ("Edit");

				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
			}
		}
		break;

	case MODAL_CONFT_NO:
	case MODAL_CONFT_CANCEL:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//	메시지
				//
				AddMessageEx ( UIMSG_MODAL_CANCEL );
				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
				

				bNowEdit = false;
				m_pBetValue->EndEdit();
				m_pEditVal->SetOneLineText ("Edit");
			}
		}
		break;
	}
}