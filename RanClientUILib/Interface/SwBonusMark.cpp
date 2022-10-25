#include "StdAfx.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../EngineUIlib/GUInterface/BasicProgressBar.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "BasicLineBoxEx.h"
#include "InnerInterface.h"
#include "SwBonusMark.h"
#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSwBonusMark::CSwBonusMark()
	: m_pImage( NULL )
{
}

CSwBonusMark::~CSwBonusMark()
{
}

void CSwBonusMark::CreateSubControl ()
{
	m_pImage =  new CBasicButton;
	m_pImage->CreateSub ( this, "SW_BONUS_IMAGE", UI_FLAG_DEFAULT, SWIconOnMouse );	
	m_pImage->SetUseGlobalAction ( TRUE );
	RegisterControl ( m_pImage );
}

void CSwBonusMark::SetSwBonus( bool isWinner )
{
	if ( isWinner )		m_pImage->SetVisibleSingle( TRUE );
	else				m_pImage->SetVisibleSingle( FALSE );
}
void CSwBonusMark::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
switch ( ControlID )
	{
	case SWIconOnMouse:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Exp,Drop,Gold Rate Bonus: x2", NS_UITEXTCOLOR::GOLD  );
			}
		}
		break;
	}
}