#include "StdAfx.h"
#include "ChatOptionWindow.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "BasicChatLeftBar.h"
#include "BasicChatRightBody.h"
#include "../EngineUILib/GUInterface/UIMan.h"
#include "ChatShowFlag.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChatOptionWindow::CChatOptionWindow()
	: m_bCheckButton( FALSE )
{
}

CChatOptionWindow::~CChatOptionWindow()
{
}

void CChatOptionWindow::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _BOLD_FONT, 9, _DEFAULT_FONT_FLAG );
	
	CBasicChatRightBody* pBasicChatRightBody = new CBasicChatRightBody;
    pBasicChatRightBody->CreateSub ( this, "CHAT_RIGHT_BODY", UI_FLAG_YSIZE, RIGHT_BODY );
	pBasicChatRightBody->CreateSubControl ( pFont9 );
	pBasicChatRightBody->SetProtectSizePoint ();
	RegisterControl ( pBasicChatRightBody );
	m_pBasicChatRightBody = pBasicChatRightBody;
	
	CBasicChatLeftBar* pBasicChatLeftBar = new CBasicChatLeftBar;
	pBasicChatLeftBar->CreateSub ( this, "CHAT_LEFT_BAR", UI_FLAG_YSIZE, LEFT_BAR );
	pBasicChatLeftBar->CreateSubControl ();
	pBasicChatLeftBar->SetProtectSizePoint ();
	RegisterControl ( pBasicChatLeftBar );
	m_pBasicChatLeftBar = pBasicChatLeftBar;

	CUIChatShowFlag* pUIChatShowFlag = new CUIChatShowFlag;
	pUIChatShowFlag->CreateSub ( this, "CHAT_SHOW_FLAG", UI_FLAG_BOTTOM, CHAT_SHOW_FLAG );
	pUIChatShowFlag->CreateSubControl ();
	pUIChatShowFlag->SetProtectSizePoint ();
	pUIChatShowFlag->SetVisibleSingle ( FALSE );
	RegisterControl ( pUIChatShowFlag );
	m_pUIChatShowFlag = pUIChatShowFlag;

	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, "CHAT_FLAG_BUTTON", UI_FLAG_BOTTOM, CHAT_FLAG_BUTTON );
	pButton->CreateFlip ( "CHAT_FLAG_BUTTON_F", CBasicButton::CLICK_FLIP );
	pButton->CreateMouseOver ( "CHAT_FLAG_BUTTON_F" );
	pButton->SetUseGlobalAction ( TRUE );
	RegisterControl ( pButton );
	m_pChatShowFlagButton = pButton;

}

void CChatOptionWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pBasicChatRightBody->CLASSIFY_CHATTYPE();
	int nChatType = m_pBasicChatRightBody->GetCHATTYPE ();
	switch ( nChatType )
	{
	case CHAT_NORMAL:	m_pBasicChatLeftBar->DO_CHAT_STATE( 0 ); break;
	case CHAT_PRIVATE:	m_pBasicChatLeftBar->DO_CHAT_STATE( 1 ); break;
	case CHAT_PARTY:	m_pBasicChatLeftBar->DO_CHAT_STATE( 2 ); break;
	case CHAT_TOALL:	m_pBasicChatLeftBar->DO_CHAT_STATE( 3 ); break;
	case CHAT_GUILD:	m_pBasicChatLeftBar->DO_CHAT_STATE( 4 ); break;
	case CHAT_ALLIANCE:	m_pBasicChatLeftBar->DO_CHAT_STATE( 5 ); break;
	}
}

void CChatOptionWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case CHAT_SHOW_FLAG:
		{
			m_pBasicChatRightBody->SetChatFlagState ( CHAT_TOALL );	//	리셋
			if ( UIMSG_CHAT_NORMAL	 & dwMsg ) m_pBasicChatRightBody->AddChatFlagState ( CHAT_NORMAL );
			if ( UIMSG_CHAT_PRIVATE  & dwMsg ) m_pBasicChatRightBody->AddChatFlagState ( CHAT_PRIVATE );
			if ( UIMSG_CHAT_PARTY    & dwMsg ) m_pBasicChatRightBody->AddChatFlagState ( CHAT_PARTY );
			if ( UIMSG_CHAT_CLUB     & dwMsg ) m_pBasicChatRightBody->AddChatFlagState ( CHAT_GUILD );
			if ( UIMSG_CHAT_ALLIANCE & dwMsg ) m_pBasicChatRightBody->AddChatFlagState ( CHAT_ALLIANCE );
			if ( UIMSG_CHAT_SYSTEM   & dwMsg ) m_pBasicChatRightBody->AddChatFlagState ( CHAT_SYSTEM );

			if ( !CHECK_MOUSE_IN ( dwMsg ) )
			{
				if (UIMSG_LB_DOWN & dwMsg )
				{
					if ( m_pUIChatShowFlag->IsVisible () )
					{
						//m_ActionMsgQ.PostUIMessage ( CHAT_SHOW_FLAG, UIMSG_INVISIBLE  );
						m_pUIChatShowFlag->SetVisibleSingle( FALSE );
						m_pBasicChatRightBody->SetVisibleChatButton( TRUE );
						m_bCheckButton = TRUE;
					}
				}
			}
			else
			{
				//	캐릭터 움직임 막음
//				AddMessageEx ( UIMSG_MOUSEIN_BLOCK_CHARACTER_MOVE );
			}
		}
		break;

	case CHAT_FLAG_BUTTON:
		{
/*			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_DOWN & dwMsg )
				{
					if ( !m_pUIChatShowFlag->IsVisible () && !m_bCheckButton )
					{
						//m_ActionMsgQ.PostUIMessage ( CHAT_SHOW_FLAG, UIMSG_VISIBLE );
						m_pBasicChatRightBody->SetVisibleChatButton( FALSE );
						m_pUIChatShowFlag->SetVisibleSingle( TRUE );
					}

					m_bCheckButton = FALSE;
				}
			}*/
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( CHAT_OPTION_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( CHAT_OPTION_WINDOW );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( CHAT_OPTION_WINDOW );
					}
				}
			}
		}	
		break;
	}

	return ;
}