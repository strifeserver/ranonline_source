#include "StdAfx.h"
#include "MiniPartySlot.h"
#include "../../EngineLib/DxCommon/DxFontMan.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "BasicLineBox.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "BasicLineBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMiniPartySlot::CMiniPartySlot () :
	m_pPlayerName ( NULL ),
	m_bMaster ( FALSE )
{
	//add class
	for ( int i = 0; i < GLCI_NUM_2012; i++ )
	{
		m_pClass[i] = NULL;
	}

	for ( int i = 0; i < 3; i++ )
	{
		m_pSchool[i] = NULL;	
	}

	memset( m_pNumber, 0, sizeof(m_pNumber) );
}

CMiniPartySlot::~CMiniPartySlot ()
{
}

void CMiniPartySlot::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	int nTextAlign = TEXT_ALIGN_CENTER_X;
	int nTextAlignRight = TEXT_ALIGN_RIGHT;
	D3DCOLOR dwFontColor = NS_UITEXTCOLOR::DEFAULT;
	D3DCOLOR dwNameColor = NS_UITEXTCOLOR::PARTYNAME;


	CBasicLineBox* pLineBoxCombo = new CBasicLineBox;
	pLineBoxCombo->CreateSub ( this, "BASIC_LINE_BOX_MINIPARTY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxCombo->CreateBaseBoxMiniParty ( "MINIPARTY_SLOT_BACK" );
	RegisterControl ( pLineBoxCombo );


	{
		//add class
		CString strClassKeyword[GLCI_NUM_2012] = 
		{
			"MINIPARTY_SLOT_CLASS_FIGHTER_M",
			"MINIPARTY_SLOT_CLASS_ARMS_M",
			"MINIPARTY_SLOT_CLASS_ARCHER_W",
			"MINIPARTY_SLOT_CLASS_SPIRIT_W",
			"MINIPARTY_SLOT_CLASS_EXTREME_M",
			"MINIPARTY_SLOT_CLASS_EXTREME_W",
			"MINIPARTY_SLOT_CLASS_FIGHTER_W",
			"MINIPARTY_SLOT_CLASS_ARMS_W",
			"MINIPARTY_SLOT_CLASS_ARCHER_M",
			"MINIPARTY_SLOT_CLASS_SPIRIT_M",
			"MINIPARTY_SLOT_CLASS_SCIENCE_M",
			"MINIPARTY_SLOT_CLASS_SCIENCE_W",
			"MINIPARTY_SLOT_CLASS_ASSASIN_M",
			"MINIPARTY_SLOT_CLASS_ASSASIN_W",
			"MINIPARTY_SLOT_CLASS_TESTING_M",
			"MINIPARTY_SLOT_CLASS_TESTING_W",
		};

		//add class
		for ( int i = 0; i < GLCI_NUM_2012; i++ )
		{
			m_pClass[i] = CreateControl ( strClassKeyword[i].GetString() );
		}
	}

	{//Add School
		CString strSchool[3] = 
		{
			"MINIPARTY_SLOT_SCHOOL0",
			"MINIPARTY_SLOT_SCHOOL1",
			"MINIPARTY_SLOT_SCHOOL2"
		};

		for ( int i = 0; i < 3; i++ )
		{
			m_pSchool[i] = CreateControl ( strSchool[i].GetString() );
		}
	}

	{
		CBasicLineBox* pLineBox1 = new CBasicLineBox;
		pLineBox1->CreateSub ( this, "BASIC_LINE_BOX_MINIPARTY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pLineBox1->CreateBaseBoxMiniParty ( "MINIPARTY_BACK" );
		RegisterControl ( pLineBox1 );
	}

	{
		m_pMaster = CreateControl ( "MINIPARTY_NUMBER_M" );
		m_pNumberSlot = CreateStaticControl ( "MINIPARTY_NUMBER", pFont9, NS_UITEXTCOLOR::WHITE, nTextAlign );
	}

	m_pPlayerName = CreateStaticControl ( "MINIPARTY_SLOT_PLAYER_NAME", pFont9, NS_UITEXTCOLOR::WHITE, nTextAlignRight );
}

CUIControl*	CMiniPartySlot::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextBox* CMiniPartySlot::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}

void CMiniPartySlot::SetNumber ( int nSlotNumber )
{
	if ( nSlotNumber < 0 && MAXPARTY <= nSlotNumber )
	{
		GASSERT ( 0 && "?????? ?????? ??? ?????." );
		return ;
	}

	m_pMaster->SetVisibleSingle ( FALSE );
	m_pNumberSlot->SetVisibleSingle ( FALSE );

	if ( nSlotNumber == 0 )
	{
		m_pMaster->SetVisibleSingle ( TRUE );
		m_pNumberSlot->SetVisibleSingle ( FALSE );
	}
	else
	{
		m_pNumberSlot->SetVisibleSingle ( TRUE );
	
		CString strNum;
		strNum.Format ( "%d", nSlotNumber );

		m_pNumberSlot->AddText ( strNum, NS_UITEXTCOLOR::WHITE );
	}
}

void CMiniPartySlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case HP_BAR:
		{
			if ( CHECK_MOUSEIN_RBUPLIKE ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_MOUSEIN_RB_EVENT );
			}
		}
		break;
	}
}
void CMiniPartySlot::SetClass ( int nClassType )
{
	//add class
	if ( nClassType < GLCI_FIGHTER_M && GLCI_TESTING_W < nClassType )
	{
		GASSERT ( 0 && "?????? ?????? ??? ?????." );
		return ;
	}
	//add class
	for ( int i = 0; i < GLCI_NUM_2012; i++ )
	{
		m_pClass[i]->SetVisibleSingle ( FALSE );
	}

	m_pClass[nClassType]->SetVisibleSingle ( TRUE );
}

void CMiniPartySlot::SetSchool ( int nSchoolType )
{
	//add class
	for ( int i = 0; i < 3; i++ )
	{
		m_pSchool[i]->SetVisibleSingle ( FALSE );
	}

	m_pSchool[nSchoolType]->SetVisibleSingle ( TRUE );
}