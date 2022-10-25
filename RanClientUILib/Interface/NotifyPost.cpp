#include "StdAfx.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"
#include "InnerInterface.h"
#include ".\notifypost.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "CharacterEnum.h"



CNotifyPost::CNotifyPost(void)
{
}

CNotifyPost::~CNotifyPost(void)
{
}
void CNotifyPost::CreateSubControl ()
{
	
	m_pNotifyButton = new CBasicButton;
	m_pNotifyButton->CreateSub ( this,"POSTNOTIFY_BUTTON_IMAGE_DEFAULT", UI_FLAG_DEFAULT, NOTIFY_BUTTON );
	m_pNotifyButton->SetUseGlobalAction ( TRUE );
	m_pNotifyButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pNotifyButton );
}


void CNotifyPost::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );	

	m_pNotifyButton->SetVisibleSingle ( TRUE );
}

void CNotifyPost::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case	NOTIFY_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Not Yet Used", NS_UITEXTCOLOR::WHITE  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
			}
		}
		break;
	}
}
