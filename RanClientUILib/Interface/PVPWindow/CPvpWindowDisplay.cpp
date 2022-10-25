#include "StdAfx.h"
#include "CPvpWindowDisplay.h"
#include "../BasicLineBoxEx.h"
#include "GLGaeaClient.h"
#include "../InnerInterface.h"
#include "../BasicTextButton.h"
#include "../GameTextControl.h"
#include "../../EngineLib/DxCommon/DxFontMan.h"
#include "../../EngineLib/DxCommon/d3dfont.h"

#include "../EngineUILib/GUInterface/BasicTextBox.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPvpWindowDisplay::CPvpWindowDisplay () :
	m_nActivePage(0),
	m_pBackGround( NULL ),
	m_pButtonSchoolWarPage( NULL ),
	//m_pPageSchoolWar( NULL ),
	m_pTimeTextNext( NULL ),
	m_pButtonClose( NULL ),
	m_pButtonParticipate ( NULL ),
	m_pLineBoxUpper( NULL ),
	m_pLineBoxBottom( NULL ),
	m_pLineBoxTopWhite ( NULL ),
	m_pTextProgress ( NULL ),
	m_pTextRegister ( NULL ),
	m_pLineBoxBottomWhite ( NULL )
{
	
}

CPvpWindowDisplay::~CPvpWindowDisplay ()
{
}

CBasicTextButton*  CPvpWindowDisplay::CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE23;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButtonEx ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText );
	RegisterControl ( pTextButton );
	return pTextButton;
}

CBasicTextButton* CPvpWindowDisplay::CreateTextButton ( char* szButton, UIGUID ControlID , char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE19;
	CBasicTextButton* pButton = new CBasicTextButton;
	pButton->CreateSub( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, ControlID );
	pButton->CreateBaseButton( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText );
	RegisterControl( pButton );

	return pButton;
}

void CPvpWindowDisplay::CreateSubControl ()
{	

	m_pLineBoxTop = new CBasicLineBoxEx;
	m_pLineBoxTop->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxTop->CreateBaseBoxDialogueWhiteBack ( "PVP_LINEBOX_TOP" );
	m_pLineBoxTop->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxTop );
	
	m_pLineBoxTopWhite = new CBasicLineBoxEx;
	m_pLineBoxTopWhite->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxTopWhite->CreateBaseBoxDialogueWhiteBack ( "PVP_LINEBOX_TOP_WHITE" );
	m_pLineBoxTopWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxTopWhite );
	
	m_pLineBoxBottomWhite = new CBasicLineBoxEx;
	m_pLineBoxBottomWhite->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxBottomWhite->CreateBaseBoxDialogueWhiteBack ( "PVP_LINEBOX_BOTTOM_WHITE" );
	m_pLineBoxBottomWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxBottomWhite );

	m_pLineBoxUpper = new CBasicLineBoxEx;
	m_pLineBoxUpper->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxUpper->CreateBaseBoxESCMenu ( "PVP_LINEBOX_UPPER" );
	m_pLineBoxUpper->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxUpper );
	
	m_pLineBoxBottom = new CBasicLineBoxEx;
	m_pLineBoxBottom->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxBottom->CreateBaseBoxESCMenu ( "PVP_LINEBOX_BOTTOM" );
	m_pLineBoxBottom->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxBottom );

//	m_pButtonSchoolWarPage = CreateTextButton23 ( "PVP_BUTTON_PAGE", SCHOOLWAR_BUTTON_PAGE, (char*)ID2GAMEWORD ( "PVP_WINDOW_TEXTS", 1 ) );


	m_pButtonClose = CreateTextButton( "PVP_BUTTON_CLOSE", SCHOOLWAR_BUTTON_CLOSE, (char*)ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT",2)); 

	m_pButtonParticipate = CreateTextButton( "SCHOOLWAR_BUTTON_PARTICIPATE", SCHOOLWAR_BUTTON_PARTICIPATE, (char*)ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT",0)); 

	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;

	CString strText[16] = 
	{
		"PVP_TEXT_INFO",
		"PVP_TEXT_GOAL",
		"PVP_TEXT_MATCH",
		"PVP_TEXT_PLAYERS",
		"PVP_TEXT_LEVEL",
		"PVP_TEXT_REWARD1",
		"PVP_TEXT_NEXT_BATTLE",
		"PVP_TEXT_START_TIME",
		"PVP_TEXT_END_TIME",
		"PVP_TEXT_PROGRESS",
		"PVP_TEXT_REGISTRATION",
		"PVP_TEXT_PREVIOUS",
		"PVP_TEXT_DURATION",
		"PVP_TEXT_WINNER1",
		"PVP_TEXT_REWARD2",
		"PVP_TEXT_WINNER2"
	};

	

	CString WinLose[2] = 
	{
	
		"PVP_TEXT_WIN",
		"PVP_TEXT_LOSE"
	};

	for(int i = 0; i < 2; i++ )
	{
		m_pWinLose[i] = CreateStaticControl( (char*)WinLose[i].GetString(), pFont, NS_UITEXTCOLOR::YELLOW, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
		m_pWinLose[i]->SetText( ID2GAMEWORD("PVP_TEXT_WINLOSE",i), NS_UITEXTCOLOR::YELLOW );
	}
	for(int i = 0; i < 16; i++ )
	{
		m_pText[i] = CreateStaticControl( (char*)strText[i].GetString(), pFont, dwColor, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
		m_pText[i]->SetText( ID2GAMEWORD("PVP_TEXT_STATIC",i), dwColor );
	}

	CString imgSchool[3] = 
	{
		"PVP_SCHOOL_MP",
		"PVP_SCHOOL_SG",
		"PVP_SCHOOL_PHX"
	};

	CString imgSchoolWin[3] = 
	{
		"PVP_SCHOOL_SG_WIN",
		"PVP_SCHOOL_MP_WIN",
		"PVP_SCHOOL_PHX_WIN"
	};

	CString strBuffImg[3] = 
	{
		"PVP_SCHOOL_MAIN_BUFF",
		"PVP_SCHOOL_GOLDITEM_BUFF",
		"PVP_SCHOOL_EXP_BUFF"
	};

	for(int i = 0; i < 3; i++ )
	{
		m_pSchoolImg[i] = new CUIControl;
		m_pSchoolImg[i]->CreateSub ( this, (char*)imgSchool[i].GetString() );	
		m_pSchoolImg[i]->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pSchoolImg[i] );

		CUIControl* pSchoolBuff = new CUIControl;
		pSchoolBuff->CreateSub ( this, (char*)strBuffImg[i].GetString() );	
		pSchoolBuff->SetVisibleSingle ( TRUE );
		RegisterControl ( pSchoolBuff );
	}

	for(int i = 0; i < 3; i++ )
	{
		m_pSchoolWinImg[i] = new CUIControl;
		m_pSchoolWinImg[i]->CreateSub ( this, (char*)imgSchoolWin[i].GetString() );	
		m_pSchoolWinImg[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolWinImg[i] );
	}

	m_pSchoolWarImage = new CUIControl;
	m_pSchoolWarImage->CreateSub ( this, "SCHOOL_WAR_MAP_IMAGE" );	
	m_pSchoolWarImage->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pSchoolWarImage );

	m_pTimeTextNext	= CreateStaticControl( "INFO_CTF_DISPLAY_TIME_NEXT_TEXT2", pFont,dwColor, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRegister = CreateStaticControl( "INFO_CTF_DISPLAY_REGISTER_TEXT", pFont,dwColor, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextProgress = CreateStaticControl( "INFO_CTF_DISPLAY_PROGRESS_TEXT", pFont,dwColor, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pTimeTextDuration	= CreateStaticControl( "INFO_CTF_DISPLAY_TIME_NEXT_DURATION", pFont,dwColor, TEXT_ALIGN_LEFT );
	m_pTimeTextEnd	= CreateStaticControl( "INFO_CTF_DISPLAY_TIME_END_TEXT2", pFont,dwColor, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pWinnerText	= CreateStaticControl( "INFO_CTF_DISPLAY_WINNER_NAME", pFont,dwColor, TEXT_ALIGN_LEFT );

}
void CPvpWindowDisplay::SetWarTime(float fElapsedTime)
{
	CString strCombine,strProgress,strRegister;
	int nSec = (int)fElapsedTime%60;
	int nMin = (int)fElapsedTime/60;
	int nHour = (int)(fElapsedTime/60)/60;
	
	m_pTimeTextNext->SetOneLineText( "--:--", NS_UITEXTCOLOR::BRIGHTGREEN );
	strCombine.Format("After %02dm : %02ds", nMin, nSec );
     m_pTextProgress->SetOneLineText( strCombine, NS_UITEXTCOLOR::WHITE );

	 
	 if ( nMin < 5 && !GLGaeaClient::GetInstance().m_bTowerWars )
	 {
		 strRegister.Format( "Possible" );
		 m_pTextRegister->SetOneLineText( strRegister, NS_UITEXTCOLOR::LIME );
	 }else if ( GLGaeaClient::GetInstance().m_bTowerWars )
	 {
		 strRegister.Format( "Not Possible" );
		 m_pTextRegister->SetOneLineText( strRegister, NS_UITEXTCOLOR::DISABLE );
		float fTime = GLGaeaClient::GetInstance().m_fTowerWarsTimer;
		int nMin2 = (int)fTime/60;
		int nHour2 = (int)(fTime/60)/60;
		int nSec2 = (int)fTime%60;
		 strProgress.Format("%02dm : %02ds", nMin2, nSec2 );
		m_pTextProgress->SetOneLineText( strProgress, NS_UITEXTCOLOR::WHITE );

	 }else
	 {
		strRegister.Format( "Not Possible" );
		 m_pTextRegister->SetOneLineText( strRegister, NS_UITEXTCOLOR::DISABLE );
	 }
}
void CPvpWindowDisplay::SetWinImg( int nWin )
{
	for(int i = 0; i < 3; i++ )
	{
		m_pSchoolWinImg[i]->SetVisibleSingle ( FALSE );
	}
	m_pSchoolWinImg[nWin]->SetVisibleSingle ( TRUE );
}
void CPvpWindowDisplay::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( CTF_RANKING_DISPLAY );
			}
		}break;
	case SCHOOLWAR_BUTTON_PARTICIPATE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				WORD wLevel = GLGaeaClient::GetInstance().GetCharacter()->m_wLevel;
				if ( wLevel >= 230  )
				{
					bool Participate = GLGaeaClient::GetInstance().m_bParticipate;
					if ( Participate )
					{
						m_pButtonParticipate->SetOneLineText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT",0),NS_UITEXTCOLOR::WHITE);
					}
					else
					{
						m_pButtonParticipate->SetOneLineText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT",1),NS_UITEXTCOLOR::WHITE);
					}

					GLGaeaClient::GetInstance().GetCharacter()->ReqSchoolWarParticipate();
				}
				else
				{
					CInnerInterface::GetInstance().PrintConsoleText("Your level must be greater than 230 to proceed.");
				}
			}
		}break;
	case ET_CONTROL_BUTTON:
	case SCHOOLWAR_BUTTON_CLOSE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup ( GetWndID () );
			}
		}break;

/*	case SCHOOLWAR_BUTTON_PAGE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	OpenPage( 1 );
		}break;*/

	/*case SCHOOLWAR_RANKING_BUTTON_RANK_SG:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	OpenPage( 2 );
		}break;
	case SCHOOLWAR_RANKING_BUTTON_RANK_MP:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	OpenPage( 3 );
		}break;
	case SCHOOLWAR_RANKING_BUTTON_RANK_PHX:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )	OpenPage( 4 );
		}break;*/
	}

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
}

void CPvpWindowDisplay::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		OpenPage( 1 );
	}
}



void CPvpWindowDisplay::OpenPage( int nPage )
{
	if ( m_pButtonSchoolWarPage )	m_pButtonSchoolWarPage->SetFlip ( FALSE );
	
	switch( nPage )
	{
	case 1:
		{
			if ( m_pButtonSchoolWarPage )	m_pButtonSchoolWarPage->SetFlip ( TRUE );

		
		}break;
	/*case 2:
	case 3:
	case 4:
		{
			m_pButtonRank[nPage-2]->SetFlip ( TRUE );

			if ( m_pPageRank )	
				m_pPageRank->RefreshRanking(nPage-2);
		}
		break;*/
	};
}

CBasicTextBox* CPvpWindowDisplay::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}

void CPvpWindowDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	for(int i = 0; i < 3; i++)
	{
		m_pSchoolWinImg[i]->SetVisibleSingle( FALSE );
	}

	CString strRegister, strCombine;
	bool bTowerWars = GLGaeaClient::GetInstance().IsTowerWars();
	if ( !bTowerWars && GLGaeaClient::GetInstance().GetCharacter()->m_wLevel >= 230 )
	{
		 strRegister.Format( "Possible" );
		 m_pTextRegister->SetOneLineText( strRegister, NS_UITEXTCOLOR::LIME );
	}
	else
	{
		strRegister.Format( "Not Possible" );
		m_pTextRegister->SetOneLineText( strRegister, NS_UITEXTCOLOR::DISABLE );
	}

	if( bTowerWars )
	{
		float fRemainingTime = GLGaeaClient::GetInstance().m_fTowerWarsTimer;
		int nSec = (int)fRemainingTime%60;
		int nMin = (int)fRemainingTime/60;
		
		strCombine.Format("After %02dm : %02ds", nMin, nSec );
	}
	else
	{
		int nWin = GLGaeaClient::GetInstance().m_wSwSchool;
		strCombine.Format("--:--");
	}

	WORD wSchool = GLGaeaClient::GetInstance().m_pTWRecord.wSchool;
	if(wSchool != MAX_SCHOOL)
		m_pSchoolWinImg[wSchool]->SetVisibleSingle( TRUE );

	m_pTextProgress->SetOneLineText( strCombine, NS_UITEXTCOLOR::WHITE );

	
	float fDurationTime = GLGaeaClient::GetInstance().m_pTWRecord.fElapsedTime;
	int nSec2 = (int)fDurationTime%60;
	int nMin2 = (int)fDurationTime/60;
	
	if ( int(fDurationTime) == 0 )
		strCombine.Format("--:--");
	else
		strCombine.Format("%02dm : %02ds", nMin2, nSec2 );

	m_pTimeTextDuration->SetOneLineText( strCombine, NS_UITEXTCOLOR::WHITE );
	m_pWinnerText->SetOneLineText( GLGaeaClient::GetInstance().m_pTWRecord.strName, NS_UITEXTCOLOR::WHITE );

	{
		m_pTimeTextNext->SetOneLineText( "00:00", NS_UITEXTCOLOR::WHITE );
		m_pTimeTextEnd->SetOneLineText( "00:30", NS_UITEXTCOLOR::WHITE );
	}
}