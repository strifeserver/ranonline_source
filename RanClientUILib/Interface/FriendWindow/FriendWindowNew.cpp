#include "StdAfx.h"
#include "FriendWindowNew.h"

#include "../BasicTextButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFriendWindowNew::CFriendWindowNew ()
{
}

CFriendWindowNew::~CFriendWindowNew ()
{
}

CBasicTextButton*  CFriendWindowNew::CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE23;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButtonEx ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText );
	RegisterControl ( pTextButton );
	return pTextButton;
}

void CFriendWindowNew::CreateSubControl ()
{
	m_pBackGround = new CUIControl;
	m_pBackGround->CreateSub ( this, "RNFRIEND_WINDOW_WHITE", UI_FLAG_DEFAULT );	
	m_pBackGround->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pBackGround );
	
	m_pButtonFriend = CreateTextButton23 ( "RNFRIEND_FRIEND_BUTTON", BUTTON_FRIEND, "Friend" );
	m_pButtonBlock  = CreateTextButton23 ( "RNFRIEND_BLOCK_BUTTON", BUTTON_BLOCK, "Block" );
	SetPage(1);
}

void CFriendWindowNew::SetPage(int nIndex)
{
	m_pButtonFriend->SetFlip(FALSE);
	m_pButtonBlock->SetFlip(FALSE);

	switch(nIndex)
	{
	case 1: m_pButtonFriend->SetFlip(TRUE); break;
	case 2: m_pButtonBlock->SetFlip(TRUE);	break;
	}
}
void CFriendWindowNew::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
	switch(ControlID)
	{
	case BUTTON_FRIEND:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP ) SetPage(1);
			}
		}
		break;
	case BUTTON_BLOCK:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP ) SetPage(2);
			}
		}
		break;
	}
}