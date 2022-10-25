#include "StdAfx.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"
#include "InnerInterface.h"
#include ".\ItemCompound.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "CharacterEnum.h"



CItemCompound::CItemCompound(void)
{
}

CItemCompound::~CItemCompound(void)
{
}
void CItemCompound::CreateSubControl ()
{
	
	m_pCompoundButton = new CBasicButton;
	m_pCompoundButton->CreateSub ( this,"COMPOUND_BUTTON_IMAGE", UI_FLAG_DEFAULT, COMPOUND_BUTTON );
	m_pCompoundButton->SetUseGlobalAction ( TRUE );
	m_pCompoundButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pCompoundButton );
}


void CItemCompound::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );	

	m_pCompoundButton->SetVisibleSingle ( TRUE );
}

void CItemCompound::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case	COMPOUND_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Item Compound", NS_UITEXTCOLOR::WHITE  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
			if ( CInnerInterface::GetInstance().IsVisibleGroup( ITEM_MIX_WINDOW ) )		CInnerInterface::GetInstance().HideGroup ( ITEM_MIX_WINDOW );
			else	CInnerInterface::GetInstance().ShowGroupFocus( ITEM_MIX_WINDOW );
			}
		}
		break;
	}
}
