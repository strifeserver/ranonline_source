#include "StdAfx.h"
#include "NotifyRequestWindow.h"
#include "../InnerInterface.h"
#include "../InnerInterfaceGuid.h"
#include "../BasicTextBoxEx.h"
#include "../BasicScrollBarEx.h"
#include "../GameTextControl.h"
#include "../UITextControl.h"
#include "../ModalCallerID.h"
#include "../PartyWindow.h"
#include "../PartyModalWindow.h"
#include "../ModalWindow.h"
#include "../BasicLineBox.h"
#include "../UITextControl.h"

#include "../EngineLib/DxCommon/DxFontMan.h"

#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../EngineUILib/GUInterface/BasicScrollThumbFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNotifyRequestWindow::CNotifyRequestWindow ()
	: m_pBackGround( NULL )
	, m_pListText( NULL )
	, m_pListScrollBar( NULL )
	, m_pHeaderText( NULL )
{
}

CNotifyRequestWindow::~CNotifyRequestWindow ()
{
}

void CNotifyRequestWindow::CreateSubControl ()
{
	{
		m_pBackGround = new CUIControl;
		m_pBackGround->CreateSub ( this, "REQUEST_LIST_BACKGROUND", UI_FLAG_DEFAULT );	
		m_pBackGround->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pBackGround );
	}

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "REQUEST_LIST_LINEBOX" );
	RegisterControl ( pBasicLineBox );		

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
	pTextBox->CreateSub ( this, "REQUEST_LIST_TEXTBOX", UI_FLAG_DEFAULT, REQUEST_LIST_TEXTBOX );
	pTextBox->SetFont ( pFont9 );		
	pTextBox->SetLineInterval ( 3.0f );
	pTextBox->SetSensitive ( true );
	pTextBox->SetLimitLine ( 10000 );
	RegisterControl ( pTextBox );
	m_pListText = pTextBox;

	int nTotalLine = pTextBox->GetVisibleLine ();

	CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
	pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, REQUEST_LIST_SCROLLBAR );
	pScrollBar->CreateBaseScrollBar ( "REQUEST_LIST_SCROLLBAR" );
	pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( pScrollBar );
	m_pListScrollBar = pScrollBar;

	CBasicTextBox* pText = new CBasicTextBox;
	pText->CreateSub ( this, "REQUEST_LIST_HEADER_TEXT" );
	pText->SetFont ( pFont9 );
	pText->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( pText );
	m_pHeaderText = pText;
}

void CNotifyRequestWindow::LoadRequest()
{
	m_pListText->ClearText();

	for (std::map<DWORD, REQUEST_LIST>::iterator it=mapReq.begin(); it!=mapReq.end(); ++it)
	{
		REQUEST_LIST rList = it->second;

		static CString strKeyword[] =
		{
			"REQUEST_LIST_FRIEND",
			"REQUEST_LIST_PARTY",
			"REQUEST_LIST_GUILD",
			"REQUEST_LIST_ALLIANCE",
			"REQUEST_LIST_TRADE"
		};

		CString strText;

		if ( rList.wType == 0 )
			strText.Format( "[%d-%d-%d] Friend Request from %s", rList.reqDate.nYear, rList.reqDate.nMonth, rList.reqDate.nDay, rList.strName );
		else if ( rList.wType == 1 )
			strText.Format( "[%d-%d-%d] %s Party Request", rList.reqDate.nYear, rList.reqDate.nMonth, rList.reqDate.nDay, rList.strName );
		else if ( rList.wType == 2 )
			strText.Format( "[%d-%d-%d] %s Guild Request", rList.reqDate.nYear, rList.reqDate.nMonth, rList.reqDate.nDay, rList.strClubName );
		else if ( rList.wType == 3 )
			strText.Format( "[%d-%d-%d] %s Alliance Request", rList.reqDate.nYear, rList.reqDate.nMonth, rList.reqDate.nDay, rList.strClubMaster );

		int nIndex = m_pListText->AddText ( strText, D3DCOLOR_ARGB(255,118,238,198) );	
		//m_pListText->SetTextImage ( nIndex, strKeyword[rList.wType] );	
		m_pListText->SetTextData ( nIndex, it->first );
	}
	
	m_pListText->SetCurLine ( 0 );

	const int nTotal = m_pListText->GetCount ();
	const int nViewPerPage = m_pListText->GetVisibleLine ();	
	m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
	m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
	
	CString strText;
	strText.Format( "%s(%d)", ID2GAMEWORD("REQUEST_LIST_HEADER_STATIC_TEXT", 0 ), GetRequestSize() );
	m_pHeaderText->SetOneLineText( strText, NS_UITEXTCOLOR::GOLD );
}

void CNotifyRequestWindow::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( NOTIFY_REQUEST_WINDOW );
			}
		}
		break;
	case REQUEST_LIST_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				const int nIndex = m_pListText->GetSelectPos ();
				if ( nIndex < 0 || m_pListText->GetCount () <= nIndex ) 
					return;

				m_pListText->SetUseOverColor ( TRUE );
				m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::DARKORANGE );
				
				if ( UIMSG_LB_DUP & dwMsg )
				{
					DWORD dwID = m_pListText->GetTextData ( nIndex );
					for (std::map<DWORD, REQUEST_LIST>::iterator it=mapReq.begin(); it!=mapReq.end(); ++it)
					{
						if ( dwID != it->first ) continue;

						REQUEST_LIST rList = it->second;
						if ( rList.wType == 0 )			//Friend
						{
							CInnerInterface::GetInstance().SetFriendName2( rList.strName );
							CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("ADD_FRIEND_REQ"), rList.strName );	
							DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_ADD_FRIEND_LURE_ANS );
						}
						else if ( rList.wType == 1 )	//Party
						{
							CInnerInterface::GetInstance().SetPartyOption( rList.sPartyOption );
							CInnerInterface::GetInstance().GetPartyWindow()->SetPartyInfo ( rList.dwPartyID, rList.dwMasterID );

							CString strTemp;
							strTemp = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("PARTY_TAR"), rList.strName );
							DoPartyModal ( strTemp, MODAL_PARTY_TAR );
						}
						else if ( rList.wType == 2 )	//Guild
						{
							CInnerInterface::GetInstance().SetClubMasterID( rList.dwClubMasterID );
							CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("CLUB_JOIN_ASK"), rList.strClubName, rList.strClubMaster );
							DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_CLUB_JOIN_ASK );
						}
						else if ( rList.wType == 3 )	//Alliance
						{
							CInnerInterface::GetInstance().SetClubMasterID( rList.dwClubMasterID );
							CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("UNION_JOIN_ASK"), rList.strClubMaster );
							DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_UNION_JOIN_ASK );
						}

						mapReq.erase(dwID);
						CInnerInterface::GetInstance().HideGroup( NOTIFY_REQUEST_WINDOW );
					}
				}
			}
		}
		break;
	}
}

void CNotifyRequestWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListText->SetUseOverColor ( FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

		const int nTotalLine = m_pListText->GetTotalLine ();
		const int nLinePerOneView = m_pListText->GetVisibleLine ();
		CDebugSet::ToView ( 1, 10, "%d", nLinePerOneView );
		pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pListText->SetCurLine ( nPos );			
		}
	}
}

void CNotifyRequestWindow::RemoveAllPartyReq()
{
	for (std::map<DWORD, REQUEST_LIST>::iterator it=mapReq.begin(); it!=mapReq.end(); ++it)
	{
		REQUEST_LIST rList = it->second;
		if ( rList.wType == 1 )
			mapReq.erase(it->first);
	}
}