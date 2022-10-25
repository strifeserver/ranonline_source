#include "StdAfx.h"
#include "NotifyRequestIcon.h"
#include "NotifyRequestWindow.h"
#include "../InnerInterface.h"
#include "../InnerInterfaceGuid.h"

const float CNotifyRequestIcon::fBLINK_TIME_LOOP = 0.2f;

CNotifyRequestIcon::CNotifyRequestIcon(void) 
	: m_pAlarmBlink ( NULL )
	, m_fBLINK_TIME ( 0.0f )
	, m_bAlarm ( false )
{
}

CNotifyRequestIcon::~CNotifyRequestIcon(void)
{
}

void CNotifyRequestIcon::CreateSubControl ()
{
	{
		CUIControl* pControl = new CUIControl;
		pControl->CreateSub ( this, "NOTIFY_REQUEST_ICON_DEFAULT", UI_FLAG_DEFAULT, NOTIFY_REQUEST_ICON_DEFAULT );
		pControl->SetVisibleSingle ( TRUE );
		RegisterControl ( pControl );
	}

	{
		CUIControl* pControl = new CUIControl;
		pControl->CreateSub ( this, "NOTIFY_REQUEST_ICON_BLINK" );
		pControl->SetVisibleSingle ( FALSE );
		RegisterControl ( pControl );
		m_pAlarmBlink = pControl;
	}
}

void CNotifyRequestIcon::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case NOTIFY_REQUEST_ICON_DEFAULT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				if ( !CInnerInterface::GetInstance().IsVisibleGroup( NOTIFY_REQUEST_WINDOW ))
				{
					CInnerInterface::GetInstance().SetDefaultPosInterface( NOTIFY_REQUEST_WINDOW );
					CInnerInterface::GetInstance().ShowGroupFocus( NOTIFY_REQUEST_WINDOW );
					CInnerInterface::GetInstance().GetNotifyRequestWindow()->LoadRequest();

					m_pAlarmBlink->SetVisibleSingle ( FALSE );
					m_bAlarm = false;	
				}
				else
				{
					CInnerInterface::GetInstance().HideGroup( NOTIFY_REQUEST_WINDOW );
				}
			}
		}
		break;
	}
}

void CNotifyRequestIcon::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_bAlarm )
	{		
		m_fBLINK_TIME += fElapsedTime;
		if ( fBLINK_TIME_LOOP <= m_fBLINK_TIME )
		{
			BOOL bVisible = m_pAlarmBlink->IsVisible ();
			m_pAlarmBlink->SetVisibleSingle ( !bVisible );
			m_fBLINK_TIME = 0.0f;
		}
	}
}

void CNotifyRequestIcon::SET_ALARM()
{
	if ( CInnerInterface::GetInstance().IsVisibleGroup( NOTIFY_REQUEST_WINDOW ))
		CInnerInterface::GetInstance().HideGroup( NOTIFY_REQUEST_WINDOW );

	m_bAlarm = true;
}