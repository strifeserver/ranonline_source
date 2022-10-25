#include "StdAfx.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"
#include "InnerInterface.h"
#include ".\notifyrecord.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "CharacterEnum.h"



CNotifyRecord::CNotifyRecord(void)
{
}

CNotifyRecord::~CNotifyRecord(void)
{
}
void CNotifyRecord::CreateSubControl ()
{
	
	m_pNotifyButton = new CBasicButton;
	m_pNotifyButton->CreateSub ( this,"STUDENTRECORD_NOTIFY_BUTTON_IMAGE", UI_FLAG_DEFAULT, NOTIFY_BUTTON );
	m_pNotifyButton->SetUseGlobalAction ( TRUE );
	m_pNotifyButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pNotifyButton );
}


void CNotifyRecord::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );	

	m_pNotifyButton->SetVisibleSingle ( TRUE );
}

void CNotifyRecord::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case	NOTIFY_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Attendance Record", NS_UITEXTCOLOR::WHITE  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
			if ( CInnerInterface::GetInstance().IsVisibleGroup( ATTENDANCE_BOOK_WINDOW ) )		CInnerInterface::GetInstance().HideGroup ( ATTENDANCE_BOOK_WINDOW );
			else	CInnerInterface::GetInstance().ShowGroupFocus( ATTENDANCE_BOOK_WINDOW );

			}
		}
		break;
	}
}
