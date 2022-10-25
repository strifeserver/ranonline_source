#include "StdAfx.h"
#include "./CharacterWindow.h"

#include "../EngineUiLib/GUInterface/BasicTextBox.h"
#include "../EngineUiLib/GUInterface/BasicProgressBar.h"
#include "../EngineUiLib/GUInterface/BasicButton.h"
#include "../EngineLib/DxCommon/d3dfont.h"

#include "../BasicTextButton.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../RanClientLib/G-Logic/GLGaeaClient.h"
#include "../InnerInterface.h"
#include "CharacterWindowChar.h"
#include "CharacterWindowVehicle.h"
//#include "CharacterWindowPet.h"
#include "../BasicLineBox.h"
#include "../BasicLineBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterWindow::CCharacterWindow () :
	nActivePage(0)
	, m_pPageChar( NULL )
	, m_pPageVehicle( NULL )
	, m_pPagePet( NULL )
	, m_pBackGround( NULL )
	, m_pButtonChar( NULL )
	, m_pButtonVehicle( NULL )
	, m_pButtonPet( NULL )
	, m_pAddInfoButtonL(NULL)
	, m_pAddInfoButtonR(NULL)
{
}

CCharacterWindow::~CCharacterWindow ()
{
}

void CCharacterWindow::CreateSubControl ()
{
/*	m_pBackGround = new CUIControl;
	m_pBackGround->CreateSub ( this, "CHARACTER_WINDOW_BACKGROUND", UI_FLAG_DEFAULT );	
	m_pBackGround->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pBackGround );*/

	{	// Ðen Xám Ðen Background
		CBasicLineBoxEx* pBasicLineBoxEx0 = new CBasicLineBoxEx;
		pBasicLineBoxEx0->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_DENXAM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx0->CreateBaseBoxDenXamBox ( "CHARACTER_WINDOW_BACKGROUND" );
		RegisterControl ( pBasicLineBoxEx0 );
	}
	
	m_pButtonChar= CreateTextButton23 ( "CHARACTER_WINDOW_CHAR_TAP", BUTTON_CHARACTER, (char*)ID2GAMEWORD ( "CHARACTER_TAP_NAME", 0 ) );
	m_pButtonChar->SetVisibleSingle ( FALSE );
	m_pButtonVehicle = CreateTextButton23 ( "CHARACTER_WINDOW_VEHICLE_TAP", BUTTON_VEHICLE, (char*)ID2GAMEWORD ( "CHARACTER_TAP_NAME", 1 ) );
	m_pButtonVehicle->SetVisibleSingle ( FALSE );
//	m_pButtonPet = CreateTextButton23 ( "CHARACTER_WINDOW_NEW_PET_TAP", BUTTON_PET, (char*)ID2GAMEWORD ( "CHARACTER_TAP_NAME", 2 ) );

	m_pPageChar = new CCharacterWindowChar;
	m_pPageChar->CreateSub ( this, "CHARACTER_WINDOW_REGION", UI_FLAG_YSIZE, PAGE_CHARACTER );
	m_pPageChar->CreateSubControl ();
	RegisterControl ( m_pPageChar );



	m_pPageVehicle = new CCharacterWindowVehicle;
	m_pPageVehicle->CreateSub ( this, "CHARACTER_WINDOW_REGION", UI_FLAG_YSIZE, PAGE_VEHICLE );
	m_pPageVehicle->CreateSubControl ();
	RegisterControl ( m_pPageVehicle );

/*	m_pPagePet = new CCharacterWindowPet;
	m_pPagePet->CreateSub ( this, "CHARACTER_WINDOW_REGION", UI_FLAG_YSIZE, PAGE_PET );
	m_pPagePet->CreateSubControl ();
	RegisterControl ( m_pPagePet );*/
	
	CUIControl*	pChannelBack = new CUIControl;
	pChannelBack->CreateSub ( this, "FLAG_VIETNAM", UI_FLAG_BOTTOM );
	pChannelBack->SetProtectSizePoint ();
	pChannelBack->SetNoUpdate ( true );
	RegisterControl ( pChannelBack );
	
	m_pAddInfoButtonL = new CBasicButton;
	m_pAddInfoButtonL->CreateSub ( this, "ADDITIONAL_L_BUTTON", UI_FLAG_DEFAULT, ADDITIONAL_BUTTON );
	m_pAddInfoButtonL->CreateFlip ( "ADDITIONAL_L_BUTTON_F", CBasicButton::RADIO_FLIP );
	m_pAddInfoButtonL->CreateMouseOver( "ADDITIONAL_IMAGE" );
	m_pAddInfoButtonL->SetUseGlobalAction ( TRUE );
	RegisterControl ( m_pAddInfoButtonL );
	
	m_pAddInfoButtonR = new CBasicButton;
	m_pAddInfoButtonR->CreateSub ( this, "ADDITIONAL_R_BUTTON", UI_FLAG_DEFAULT, ADDITIONAL_BUTTON );
	m_pAddInfoButtonR->CreateFlip ( "ADDITIONAL_R_BUTTON_F", CBasicButton::RADIO_FLIP );
	m_pAddInfoButtonR->CreateMouseOver( "ADDITIONAL_IMAGE" );
	m_pAddInfoButtonR->SetUseGlobalAction ( TRUE );
	m_pAddInfoButtonR->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pAddInfoButtonR );
}

CBasicTextButton*  CCharacterWindow::CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE23;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText ,_DEFAULT_FONT_FLAG );
	RegisterControl ( pTextButton );
	return pTextButton;
}

void CCharacterWindow::SetGlobalPos(const D3DXVECTOR2& vPos)
{
	CUIGroup::SetGlobalPos( vPos );
}

void CCharacterWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ET_CONTROL_BUTTON:
			{
				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					CInnerInterface::GetInstance().HideGroup ( CHARACTER_WINDOW );
					CInnerInterface::GetInstance().HideGroup ( CHARACTER_ADDITIONAL_WINDOW );
				}
				break;
			}
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( CHARACTER_ADDITIONAL_WINDOW );
			}
		}
		break;
	case BUTTON_CHARACTER:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
			{
				if ( nActivePage == CHARPAGE_CHARACTER )	return;
				OpenPage( CHARPAGE_CHARACTER );
			}
		}break;
	case BUTTON_VEHICLE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
			{
				if ( GLTradeClient::GetInstance().Valid() ) return;
				if ( nActivePage == CHARPAGE_VEHICLE )	return;
				OpenPage( CHARPAGE_VEHICLE );
			}
		}break;
	case ADDITIONAL_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( CHARACTER_ADDITIONAL_WINDOW ) )
					{
						CInnerInterface::GetInstance().ShowGroupFocus ( CHARACTER_ADDITIONAL_WINDOW );
						m_pAddInfoButtonR->SetVisibleSingle ( TRUE );
//						m_pAddInfoButtonL->SetVisibleSingle ( FALSE );
					}
					else
					{
						CInnerInterface::GetInstance().HideGroup ( CHARACTER_ADDITIONAL_WINDOW );
//						m_pAddInfoButtonL->SetVisibleSingle ( TRUE );
						m_pAddInfoButtonR->SetVisibleSingle ( FALSE );
					}
				}
			}
		}	
		break;
/*	case ADDITIONAL_R_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_UP )
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup ( CHARACTER_ADDITIONAL_WINDOW ) )
					{
						CInnerInterface::GetInstance().HideGroup ( CHARACTER_ADDITIONAL_WINDOW );
						m_pAddInfoButtonL->SetVisibleSingle ( TRUE );
						m_pAddInfoButtonR->SetVisibleSingle ( FALSE );
					}
				}
			}
		}	
		break;*/
/*	case BUTTON_PET:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	
			{
				if ( GLTradeClient::GetInstance().Valid() ) return;
				if ( nActivePage == CHARPAGE_PET )	return;
				OpenPage( CHARPAGE_PET );
			}
		}break;*/
	}
}


void CCharacterWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCharacterWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindow::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage( 0 );
	}
	else
	{
		if ( m_pPageChar )
			m_pPageChar->ClearRender();

		if ( m_pPageVehicle )
			m_pPageVehicle->ClearRender();

/*		if ( m_pPagePet )
			m_pPagePet->ClearRender();*/
	}
}

void CCharacterWindow::OpenPage( int nPage )
{
	const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();

	//Add Character Window Title Name by JADev
	CString strCombineName;
	strCombineName.Format ( "[%s] Character Info", sCharData.m_szName );
	SetTitleName ( strCombineName ); 

	m_pButtonChar->SetFlipYellow ( FALSE );
	m_pButtonVehicle->SetFlipYellow ( FALSE );
//	m_pButtonPet->SetFlipBlack ( FALSE );


	m_pPageChar->SetVisibleSingle ( FALSE );
	m_pPageVehicle->SetVisibleSingle ( FALSE );
//	m_pPagePet->SetVisibleSingle ( FALSE );

	switch( nPage )
	{
	case CHARPAGE_CHARACTER:
		{
			m_pPageChar->SetVisibleSingle ( TRUE );
			m_pButtonChar->SetFlipYellow ( TRUE );
			nActivePage = nPage;
		}break;
	case CHARPAGE_VEHICLE:
		{
			m_pPageVehicle->SetVisibleSingle ( TRUE );
			m_pButtonVehicle->SetFlipYellow ( TRUE );
			nActivePage = nPage;
		}break;
/*	case CHARPAGE_PET:
		{
			m_pPagePet->SetVisibleSingle ( TRUE );
			m_pButtonPet->SetFlipBlack ( TRUE );
			nActivePage = nPage;
		}break;*/

	};
}

void CCharacterWindow::SetArmSwapTabButton( BOOL bArmSub )
{
	if( m_pPageChar )
	{
		m_pPageChar->SetArmSwapTabButton( bArmSub );
	}
}

void CCharacterWindow::RefreshVehiclePage()
{
	if( m_pPageVehicle )
	{
		if ( m_pPageVehicle->IsVisible() )
		{
			m_pPageVehicle->InitVehicle();
		}
	}
}
/*
void CCharacterWindow::SetSkillcombo()
{
	if( m_pPagePet )
	{
		if ( m_pPagePet->IsVisible() )
		{
			m_pPagePet->SetSkillCombo();
		}
	}
}*/