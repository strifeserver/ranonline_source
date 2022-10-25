#include "StdAfx.h"
#include "BikeColorCard.h"

#include "../EngineUIlib/GUInterface/BasicProgressBar.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "GameTextControl.h"
#include "../EngineUIlib/GUInterface/UIEditBox.h"
#include "BasicLineBox.h"
#include "BasicTextButton.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CBikeColorCard::nLIMIT_NUMBER_CHAR = 2;
const int CBikeColorCard::nMAX_COLOR = 31;
const float CBikeColorCard::fRGB_RATIO = 8.225806f;

CBikeColorCard::CBikeColorCard ()
	: m_pRProgressBar(NULL)
	, m_pGProgressBar(NULL)
	, m_pBProgressBar(NULL)
	, m_pREditBox(NULL)
	, m_pGEditBox(NULL)
	, m_pBEditBox(NULL)
	, m_RGBControl(NULL)
	, m_RGBControl1(NULL)
	, m_RGBControl2(NULL)
	, m_RGBControl3(NULL)
	, m_RGBControl4(NULL)
	, m_RGBControl5(NULL)
	, m_nRDummy(0)
	, m_nGDummy(0)
	, m_nBDummy(0)
	, m_nR(0)
	, m_nG(0)
	, m_nB(0)
	, m_nR1(0)
	, m_nG1(0)
	, m_nB1(0)
	, m_nR2(0)
	, m_nG2(0)
	, m_nB2(0)
	, m_nR3(0)
	, m_nG3(0)
	, m_nB3(0)
	, m_nR4(0)
	, m_nG4(0)
	, m_nB4(0)
	, m_nR5(0)
	, m_nG5(0)
	, m_nB5(0)
	, m_nRBack(0)
	, m_nGBack(0)
	, m_nBBack(0)
	, m_nRBack1(0)
	, m_nGBack1(0)
	, m_nBBack1(0)
	, m_nRBack2(0)
	, m_nGBack2(0)
	, m_nBBack2(0)
	, m_nRBack3(0)
	, m_nGBack3(0)
	, m_nBBack3(0)
	, m_nRBack4(0)
	, m_nGBack4(0)
	, m_nBBack4(0)
	, m_nRBack5(0)
	, m_nGBack5(0)
	, m_nBBack5(0)
	, m_nRegR(0)
	, m_nRegG(0)
	, m_nRegB(0)
	, m_nRegR1(0)
	, m_nRegG1(0)
	, m_nRegB1(0)
	, m_nRegR2(0)
	, m_nRegG2(0)
	, m_nRegB2(0)
	, m_nRegR3(0)
	, m_nRegG3(0)
	, m_nRegB3(0)
	, m_nRegR4(0)
	, m_nRegG4(0)
	, m_nRegB4(0)
	, m_nRegR5(0)
	, m_nRegG5(0)
	, m_nRegB5(0)
	, m_bOKButton(FALSE)
	, nPartSelect(0)
{
}

CBikeColorCard::~CBikeColorCard ()
{
}

CBasicTextBox* CBikeColorCard::CreateStaticControl( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

void CBikeColorCard::CreateSubControl ()
{
	CD3DFontPar * pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9 );

	CreateControl( "BIKECOLORCARD_TITLE_BACK_L" );
	CreateControl( "BIKECOLORCARD_TITLE_BACK_R" );
	CBasicTextBox * pTextBox = CreateStaticControl( "BIKECOLORCARD_TITLE", pFont, TEXT_ALIGN_CENTER_X );
	pTextBox->AddText( ID2GAMEINTEXT("BIKECOLORCARD_TITLE") );

	{ // Note : R,G,B Static
		CUIControl * pUIControl = new CUIControl;
		pUIControl->CreateSub( this, "BIKECOLORCARD_R" );
		RegisterControl( pUIControl );

		pUIControl = new CUIControl;
		pUIControl->CreateSub( this, "BIKECOLORCARD_G" );
		RegisterControl( pUIControl );

		pUIControl = new CUIControl;
		pUIControl->CreateSub( this, "BIKECOLORCARD_B" );
		RegisterControl( pUIControl );
	}

	{	// Note : 프로그래스바
		CBasicProgressBar* pProgressBar( NULL );
		CUIControl*	pDummyBar( NULL );

		pProgressBar = new CBasicProgressBar;
		pProgressBar->CreateSub ( this, "BIKECOLORCARD_R_PROGRESS" );
		pProgressBar->CreateOverImage ( "BIKECOLORCARD_R_PROGRESS_OVERIMAGE" );
		RegisterControl ( pProgressBar );
		m_pRProgressBar = pProgressBar;

		pDummyBar = new CUIControl;
		pDummyBar->CreateSub ( this, "BIKECOLORCARD_R_PROGRESS_DUMMY", UI_FLAG_DEFAULT, BIKECOLORCARD_R_PROGRESS );
		RegisterControl ( pDummyBar );

		pProgressBar = new CBasicProgressBar;
		pProgressBar->CreateSub ( this, "BIKECOLORCARD_G_PROGRESS" );
		pProgressBar->CreateOverImage ( "BIKECOLORCARD_G_PROGRESS_OVERIMAGE" );
		RegisterControl ( pProgressBar );
		m_pGProgressBar = pProgressBar;

		pDummyBar = new CUIControl;
		pDummyBar->CreateSub ( this, "BIKECOLORCARD_G_PROGRESS_DUMMY", UI_FLAG_DEFAULT, BIKECOLORCARD_G_PROGRESS );
		RegisterControl ( pDummyBar );

		pProgressBar = new CBasicProgressBar;
		pProgressBar->CreateSub ( this, "BIKECOLORCARD_B_PROGRESS" );
		pProgressBar->CreateOverImage ( "BIKECOLORCARD_B_PROGRESS_OVERIMAGE" );
		RegisterControl ( pProgressBar );
		m_pBProgressBar = pProgressBar;		

		pDummyBar = new CUIControl;
		pDummyBar->CreateSub ( this, "BIKECOLORCARD_B_PROGRESS_DUMMY", UI_FLAG_DEFAULT, BIKECOLORCARD_B_PROGRESS );
		RegisterControl ( pDummyBar );
	}

	{ // Note : 입력 에디터 박스
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxEditBox( "BIKECOLORCARD_R_EDITBOX_BACK" );
		RegisterControl( pBasicLineBox );

		CUIEditBox* pEditBox = new CUIEditBox;
		pEditBox->CreateSub( this, "BIKECOLORCARD_R_EDITBOX", UI_FLAG_XSIZE | UI_FLAG_RIGHT, BIKECOLORCARD_R_EDITBOX );
		pEditBox->CreateCarrat( "MODAL_CARRAT", TRUE, UINT_MAX );
		pEditBox->SetLimitInput( nLIMIT_NUMBER_CHAR );
		pEditBox->SetFont( pFont );
		pEditBox->DoUSE_NUMBER( true );
		RegisterControl( pEditBox );
		m_pREditBox = pEditBox;

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxEditBox( "BIKECOLORCARD_G_EDITBOX_BACK" );
		RegisterControl( pBasicLineBox );

		pEditBox = new CUIEditBox;
		pEditBox->CreateSub( this, "BIKECOLORCARD_G_EDITBOX", UI_FLAG_XSIZE | UI_FLAG_RIGHT, BIKECOLORCARD_G_EDITBOX );
		pEditBox->CreateCarrat( "MODAL_CARRAT", TRUE, UINT_MAX );
		pEditBox->SetLimitInput( nLIMIT_NUMBER_CHAR );
		pEditBox->SetFont( pFont );	
		pEditBox->DoUSE_NUMBER( true );
		RegisterControl( pEditBox );
		m_pGEditBox = pEditBox;

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub( this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxEditBox( "BIKECOLORCARD_B_EDITBOX_BACK" );
		RegisterControl( pBasicLineBox );

		pEditBox = new CUIEditBox;
		pEditBox->CreateSub( this, "BIKECOLORCARD_B_EDITBOX", UI_FLAG_XSIZE | UI_FLAG_RIGHT, BIKECOLORCARD_B_EDITBOX );
		pEditBox->CreateCarrat( "MODAL_CARRAT", TRUE, UINT_MAX );
		pEditBox->SetLimitInput( nLIMIT_NUMBER_CHAR );
		pEditBox->SetFont( pFont );
		pEditBox->DoUSE_NUMBER( true );
		RegisterControl( pEditBox );
		m_pBEditBox = pEditBox;
	}

	{ // Note : 버튼
		CBasicTextButton* pButton = new CBasicTextButton;
		pButton->CreateSub( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, BIKECOLORCARD_OK );
		pButton->CreateBaseButton( "BIKECOLORCARD_OK", CBasicTextButton::SIZE14, 
									CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD( "MODAL_BUTTON", 0 ) );
		pButton->SetShortcutKey( DIK_RETURN, DIK_NUMPADENTER );
		RegisterControl( pButton );

		pButton = new CBasicTextButton;
		pButton->CreateSub( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, BIKECOLORCARD_CANCEL );
		pButton->CreateBaseButton( "BIKECOLORCARD_CANCEL", CBasicTextButton::SIZE14, 
									CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD( "MODAL_BUTTON", 1 ) );
		pButton->SetShortcutKey( DIK_ESCAPE );
		RegisterControl( pButton );
	}

	{ // Note : 색상 변경 창
		CUIControl * pControl = new CUIControl;
		pControl->CreateSub( this, "BIKECOLORCARD_RGB_BACK" );
		RegisterControl( pControl );

		pControl = new CUIControl;
		pControl->CreateSub( this, "BIKECOLORCARD_RGB", UI_FLAG_XSIZE, BIKECOLOR_PART_SEL0 );
		pControl->SetUseRender( TRUE );
		RegisterControl( pControl );
		m_RGBControl = pControl;
	}
	{ // Note : 색상 변경 창
		CUIControl * pControl1 = new CUIControl;
		pControl1->CreateSub( this, "BIKECOLORCARD_RGB_BACK1" );
		RegisterControl( pControl1 );

		pControl1 = new CUIControl;
		pControl1->CreateSub( this, "BIKECOLORCARD_RGB1", UI_FLAG_XSIZE, BIKECOLOR_PART_SEL1 );
		pControl1->SetUseRender( TRUE );
		RegisterControl( pControl1 );
		m_RGBControl1 = pControl1;
	}
	{ // Note : 색상 변경 창
		CUIControl * pControl2 = new CUIControl;
		pControl2->CreateSub( this, "BIKECOLORCARD_RGB_BACK2" );
		RegisterControl( pControl2 );

		pControl2 = new CUIControl;
		pControl2->CreateSub( this, "BIKECOLORCARD_RGB2", UI_FLAG_XSIZE, BIKECOLOR_PART_SEL2 );
		pControl2->SetUseRender( TRUE );
		RegisterControl( pControl2 );
		m_RGBControl2 = pControl2;
	}
	{ // Note : 색상 변경 창
		CUIControl * pControl3 = new CUIControl;
		pControl3->CreateSub( this, "BIKECOLORCARD_RGB_BACK3" );
		RegisterControl( pControl3 );

		pControl3 = new CUIControl;
		pControl3->CreateSub( this, "BIKECOLORCARD_RGB3", UI_FLAG_XSIZE, BIKECOLOR_PART_SEL3 );
		pControl3->SetUseRender( TRUE );
		RegisterControl( pControl3 );
		m_RGBControl3 = pControl3;
	}
	{ // Note : 색상 변경 창
		CUIControl * pControl4 = new CUIControl;
		pControl4->CreateSub( this, "BIKECOLORCARD_RGB_BACK4" );
		RegisterControl( pControl4 );

		pControl4 = new CUIControl;
		pControl4->CreateSub( this, "BIKECOLORCARD_RGB4", UI_FLAG_XSIZE, BIKECOLOR_PART_SEL4 );
		pControl4->SetUseRender( TRUE );
		RegisterControl( pControl4 );
		m_RGBControl4 = pControl4;
	}
	{ // Note : 색상 변경 창
		CUIControl * pControl5 = new CUIControl;
		pControl5->CreateSub( this, "BIKECOLORCARD_RGB_BACK5" );
		RegisterControl( pControl5 );

		pControl5 = new CUIControl;
		pControl5->CreateSub( this, "BIKECOLORCARD_RGB5", UI_FLAG_XSIZE, BIKECOLOR_PART_SEL5 );
		pControl5->SetUseRender( TRUE );
		RegisterControl( pControl5 );
		m_RGBControl5 = pControl5;
	}
}

BOOL CBikeColorCard::UpdateProgressBar( CBasicProgressBar * pBar, int x )
{
	if( pBar && pBar->IsExclusiveSelfControl() )
	{
		const float fSize = pBar->GetGlobalPos ().sizeX;
		const float fPos = float(x) - pBar->GetGlobalPos ().left;
		const float fPercent = fPos / fSize;
		pBar->SetPercent ( fPercent );

		return TRUE;
	}

	return FALSE;
}

void CBikeColorCard::UpdateProgressBarToEditBox( CBasicProgressBar * pBar, CUIEditBox * pEditBox, INT & nColorVal )
{
	int nVal(0);
	float fPercent = pBar->GetPercent();
	if( fPercent > 0 )	nVal = (int)(nMAX_COLOR * fPercent);

	CString strTemp;
	strTemp.Format( "%d", nVal );

	pEditBox->ClearEdit();
	pEditBox->SetEditString( strTemp );

	nColorVal = nVal;
}

BOOL CBikeColorCard::UpdateEditBoxToProgressBar( CUIEditBox * pEditBox, CBasicProgressBar * pBar, INT & nColorVal )
{
	if( pEditBox && pEditBox->IsBegin() )
	{
		const CString & strRVal = pEditBox->GetEditString();
		nColorVal = _ttoi( strRVal.GetString() );
		if( nColorVal > nMAX_COLOR )
		{
			nColorVal = nMAX_COLOR;

			pEditBox->ClearEdit();
			pEditBox->SetEditString( CString("31") );
		}

		float fPercent(0);
		if( nColorVal > 0 )	fPercent = nColorVal / float(nMAX_COLOR);
		pBar->SetPercent( fPercent );

		return TRUE;
	}

	return FALSE;
}

void CBikeColorCard::Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if( bFirstControl )
	{
		BOOL bChange( FALSE );

		if( UpdateProgressBar( m_pRProgressBar, x ) )
		{
			switch(nPartSelect)
			{
				case 1: UpdateProgressBarToEditBox( m_pRProgressBar, m_pREditBox, m_nR1 );	break;
				case 2: UpdateProgressBarToEditBox( m_pRProgressBar, m_pREditBox, m_nR2 );	break;
				case 3: UpdateProgressBarToEditBox( m_pRProgressBar, m_pREditBox, m_nR3 );	break;
				case 4: UpdateProgressBarToEditBox( m_pRProgressBar, m_pREditBox, m_nR4 );	break;
				case 5: UpdateProgressBarToEditBox( m_pRProgressBar, m_pREditBox, m_nR5 );	break;
				default: UpdateProgressBarToEditBox( m_pRProgressBar, m_pREditBox, m_nR );	break;
			}
			
			bChange = TRUE;
		}
		else if( UpdateProgressBar( m_pGProgressBar, x ) )
		{
			switch(nPartSelect)
			{
				case 1: UpdateProgressBarToEditBox( m_pGProgressBar, m_pGEditBox, m_nG1 );	break;
				case 2: UpdateProgressBarToEditBox( m_pGProgressBar, m_pGEditBox, m_nG2 );	break;
				case 3: UpdateProgressBarToEditBox( m_pGProgressBar, m_pGEditBox, m_nG3 );	break;
				case 4: UpdateProgressBarToEditBox( m_pGProgressBar, m_pGEditBox, m_nG4 );	break;
				case 5: UpdateProgressBarToEditBox( m_pGProgressBar, m_pGEditBox, m_nG5 );	break;
				default: UpdateProgressBarToEditBox( m_pGProgressBar, m_pGEditBox, m_nG );	break;
			}

			bChange = TRUE;
		}
		else if( UpdateProgressBar( m_pBProgressBar, x ) )
		{
			switch(nPartSelect)
			{
				case 1: UpdateProgressBarToEditBox( m_pBProgressBar, m_pBEditBox, m_nB1 );	break;
				case 2: UpdateProgressBarToEditBox( m_pBProgressBar, m_pBEditBox, m_nB2 );	break;
				case 3: UpdateProgressBarToEditBox( m_pBProgressBar, m_pBEditBox, m_nB3 );	break;
				case 4: UpdateProgressBarToEditBox( m_pBProgressBar, m_pBEditBox, m_nB4 );	break;
				case 5: UpdateProgressBarToEditBox( m_pBProgressBar, m_pBEditBox, m_nB5 );	break;
				default: UpdateProgressBarToEditBox( m_pBProgressBar, m_pBEditBox, m_nB );	break;
			}

			bChange = TRUE;
		}
		
		switch(nPartSelect)
		{
			case 1: 
				{
					if( UpdateEditBoxToProgressBar( m_pREditBox, m_pRProgressBar, m_nR1 ) )		bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pGEditBox, m_pGProgressBar, m_nG1 ) )	bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pBEditBox, m_pBProgressBar, m_nB1 ) )	bChange = TRUE;
				}
				break;
			case 2: 
				{
					if( UpdateEditBoxToProgressBar( m_pREditBox, m_pRProgressBar, m_nR2 ) )		bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pGEditBox, m_pGProgressBar, m_nG2 ) )	bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pBEditBox, m_pBProgressBar, m_nB2 ) )	bChange = TRUE;
				}
				break;
			case 3: 
				{
					if( UpdateEditBoxToProgressBar( m_pREditBox, m_pRProgressBar, m_nR3 ) )		bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pGEditBox, m_pGProgressBar, m_nG3 ) )	bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pBEditBox, m_pBProgressBar, m_nB3 ) )	bChange = TRUE;
				}
				break;
			case 4: 
				{
					if( UpdateEditBoxToProgressBar( m_pREditBox, m_pRProgressBar, m_nR4 ) )		bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pGEditBox, m_pGProgressBar, m_nG4 ) )	bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pBEditBox, m_pBProgressBar, m_nB4 ) )	bChange = TRUE;
				}
				break;
			case 5:
				{
					if( UpdateEditBoxToProgressBar( m_pREditBox, m_pRProgressBar, m_nR5 ) )		bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pGEditBox, m_pGProgressBar, m_nG5 ) )	bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pBEditBox, m_pBProgressBar, m_nB5 ) )	bChange = TRUE;
				}
				break;
			default: 
				{
					if( UpdateEditBoxToProgressBar( m_pREditBox, m_pRProgressBar, m_nR ) )		bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pGEditBox, m_pGProgressBar, m_nG ) )	bChange = TRUE;
					else if( UpdateEditBoxToProgressBar( m_pBEditBox, m_pBProgressBar, m_nB ) )	bChange = TRUE;
				}	
				break;
		}

		

		if( bChange )
		{
			RegulateRGB();
			m_RGBControl->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR, m_nRegG, m_nRegB ) );
			m_RGBControl1->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR1, m_nRegG1, m_nRegB1 ) );
			m_RGBControl2->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR2, m_nRegG2, m_nRegB2 ) );
			m_RGBControl3->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR3, m_nRegG3, m_nRegB3 ) );
			m_RGBControl4->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR4, m_nRegG4, m_nRegB4 ) );
			m_RGBControl5->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR5, m_nRegG5, m_nRegB5 ) );

			WORD wTemp = m_nR << 10	| m_nG << 5 | m_nB;
			WORD wTemp1 = m_nR1 << 10	| m_nG1 << 5 | m_nB1;
			WORD wTemp2 = m_nR2 << 10	| m_nG2 << 5 | m_nB2;
			WORD wTemp3 = m_nR3 << 10	| m_nG3 << 5 | m_nB3;
			WORD wTemp4 = m_nR4 << 10	| m_nG4 << 5 | m_nB4;
			WORD wTemp5 = m_nR5 << 10	| m_nG5 << 5 | m_nB5;

			GLGaeaClient::GetInstance().GetCharacter()->BikeColorChange( wTemp, wTemp1, wTemp2, wTemp3, wTemp4, wTemp5 );
		}
	}
}

void CBikeColorCard::TranslateProgressBarMsg( CBasicProgressBar * pBar, DWORD dwMsg )
{
	if( !pBar ) return;

	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if ( CHECK_LB_DOWN_LIKE ( dwMsg ) )
		{			
			pBar->SetExclusiveControl();
		}
		else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
		{
			pBar->ResetExclusiveControl();
		}
	}
	else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
	{									
		pBar->ResetExclusiveControl();
	}
}

void CBikeColorCard::TranslateEditBoxMsg( CUIEditBox * pEditBox, DWORD dwMsg )
{
	if( !pEditBox ) return;

	if( CHECK_MOUSE_IN_LBDOWNLIKE( dwMsg ) )
	{
		pEditBox->BeginEdit();
	}
	else if( CHECK_LB_DOWN_LIKE( dwMsg ) )
	{
		pEditBox->EndEdit();
	}
}

void CBikeColorCard::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case BIKECOLORCARD_R_PROGRESS: TranslateProgressBarMsg( m_pRProgressBar, dwMsg ); break;
	case BIKECOLORCARD_G_PROGRESS: TranslateProgressBarMsg( m_pGProgressBar, dwMsg ); break;
	case BIKECOLORCARD_B_PROGRESS: TranslateProgressBarMsg( m_pBProgressBar, dwMsg ); break;

	case BIKECOLORCARD_R_EDITBOX: TranslateEditBoxMsg( m_pREditBox, dwMsg ); break;
	case BIKECOLORCARD_G_EDITBOX: TranslateEditBoxMsg( m_pGEditBox, dwMsg ); break;
	case BIKECOLORCARD_B_EDITBOX: TranslateEditBoxMsg( m_pBEditBox, dwMsg ); break;
	
	case BIKECOLOR_PART_SEL0:	if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )		nPartSelect=0;		InitUIControlDummy(); break;
	case BIKECOLOR_PART_SEL1:	if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )		nPartSelect=1;		InitUIControlDummy(); break;
	case BIKECOLOR_PART_SEL2:	if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )		nPartSelect=2;		InitUIControlDummy(); break;
	case BIKECOLOR_PART_SEL3:	if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )		nPartSelect=3;		InitUIControlDummy(); break;
	case BIKECOLOR_PART_SEL4:	if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )		nPartSelect=4;		InitUIControlDummy(); break;
	case BIKECOLOR_PART_SEL5:	if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )		nPartSelect=5;		InitUIControlDummy(); break;

	case BIKECOLORCARD_OK:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				m_bOKButton = TRUE;
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}
		break;

	case BIKECOLORCARD_CANCEL:
	case ET_CONTROL_BUTTON:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );
}

void CBikeColorCard::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		InitUIControl();

		GLGaeaClient::GetInstance().GetCharacter()->BikeColorInitData();
	}
	else
	{
		if( m_bOKButton )
		{
			WORD wTemp = m_nR << 10	| m_nG << 5 | m_nB;
			WORD wTemp1 = m_nR1 << 10	| m_nG1 << 5 | m_nB1;
			WORD wTemp2 = m_nR2 << 10	| m_nG2 << 5 | m_nB2;
			WORD wTemp3 = m_nR3 << 10	| m_nG3 << 5 | m_nB3;
			WORD wTemp4 = m_nR4 << 10	| m_nG4 << 5 | m_nB4;
			WORD wTemp5 = m_nR5 << 10	| m_nG5 << 5 | m_nB5;

			GLGaeaClient::GetInstance().GetCharacter()->ReqInvenBikeColorChange( wTemp, wTemp1, wTemp2, wTemp3, wTemp4, wTemp5 );
			
			m_bOKButton = FALSE;
		}
		else
		{
			WORD wTemp = m_nRBack << 10	| m_nGBack << 5 | m_nBBack;
			WORD wTemp1 = m_nRBack1 << 10	| m_nGBack1 << 5 | m_nBBack1;
			WORD wTemp2 = m_nRBack2 << 10	| m_nGBack2 << 5 | m_nBBack2;
			WORD wTemp3 = m_nRBack3 << 10	| m_nGBack3 << 5 | m_nBBack3;
			WORD wTemp4 = m_nRBack4 << 10	| m_nGBack4 << 5 | m_nBBack4;
			WORD wTemp5 = m_nRBack5 << 10	| m_nGBack5 << 5 | m_nBBack5;

			GLGaeaClient::GetInstance().GetCharacter()->BikeColorChange( wTemp, wTemp1, wTemp2, wTemp3, wTemp4, wTemp5 );
		}

		ClearEditBox();

		GLGaeaClient::GetInstance().GetCharacter()->BikeColorEnd();
	}
}

void CBikeColorCard::InitUIControl()
{
	const WORD & wBIKECOLOR = GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.m_wColor;
	const WORD & wBIKECOLOR1 = GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.m_wColor1;
	const WORD & wBIKECOLOR2 = GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.m_wColor2;
	const WORD & wBIKECOLOR3 = GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.m_wColor3;
	const WORD & wBIKECOLOR4 = GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.m_wColor4;
	const WORD & wBIKECOLOR5 = GLGaeaClient::GetInstance().GetCharacter()->m_sVehicle.m_wColor5;
	
	m_nRBack = m_nR = (wBIKECOLOR & 0x7c00) >> 10;
	m_nGBack = m_nG = (wBIKECOLOR & 0x3e0) >> 5;
	m_nBBack = m_nB = (wBIKECOLOR & 0x1f);

	m_nRBack1 = m_nR1 = (wBIKECOLOR1 & 0x7c00) >> 10;
	m_nGBack1 = m_nG1 = (wBIKECOLOR1 & 0x3e0) >> 5;
	m_nBBack1 = m_nB1 = (wBIKECOLOR1 & 0x1f);

	m_nRBack2 = m_nR2 = (wBIKECOLOR2 & 0x7c00) >> 10;
	m_nGBack2 = m_nG2 = (wBIKECOLOR2 & 0x3e0) >> 5;
	m_nBBack2 = m_nB2 = (wBIKECOLOR2 & 0x1f);

	m_nRBack3 = m_nR3 = (wBIKECOLOR3 & 0x7c00) >> 10;
	m_nGBack3 = m_nG3 = (wBIKECOLOR3 & 0x3e0) >> 5;
	m_nBBack3 = m_nB3 = (wBIKECOLOR3 & 0x1f);

	m_nRBack4 = m_nR4 = (wBIKECOLOR4 & 0x7c00) >> 10;
	m_nGBack4 = m_nG4 = (wBIKECOLOR4 & 0x3e0) >> 5;
	m_nBBack4 = m_nB4 = (wBIKECOLOR4 & 0x1f);

	m_nRBack5 = m_nR5 = (wBIKECOLOR5 & 0x7c00) >> 10;
	m_nGBack5 = m_nG5 = (wBIKECOLOR5 & 0x3e0) >> 5;
	m_nBBack5 = m_nB5 = (wBIKECOLOR5 & 0x1f);

	CString strTemp;

	switch(nPartSelect)
	{
		case 1: m_nRDummy=m_nR1; m_nGDummy=m_nG1; m_nBDummy=m_nB1;	break;
		case 2: m_nRDummy=m_nR2; m_nGDummy=m_nG2; m_nBDummy=m_nB2;	break;
		case 3: m_nRDummy=m_nR3; m_nGDummy=m_nG3; m_nBDummy=m_nB3;	break;
		case 4: m_nRDummy=m_nR4; m_nGDummy=m_nG4; m_nBDummy=m_nB4;	break;
		case 5: m_nRDummy=m_nR5; m_nGDummy=m_nG5; m_nBDummy=m_nB5;	break;
		default: m_nRDummy=m_nR; m_nGDummy=m_nG; m_nBDummy=m_nB;	break;
	}

	strTemp.Format( "%d", m_nRDummy );
	m_pREditBox->SetEditString( strTemp );

	strTemp.Format( "%d", m_nGDummy );
	m_pGEditBox->SetEditString( strTemp );

	strTemp.Format( "%d", m_nBDummy );
	m_pBEditBox->SetEditString( strTemp );

	float fPercent(0);
	if( m_nRDummy > 0 )	fPercent = m_nRDummy / float(nMAX_COLOR);
	m_pRProgressBar->SetPercent( fPercent );

	fPercent = 0;
	if( m_nGDummy > 0 )	fPercent = m_nGDummy / float(nMAX_COLOR);
	m_pGProgressBar->SetPercent( fPercent );

	fPercent = 0;
	if( m_nBDummy > 0 )	fPercent = m_nBDummy / float(nMAX_COLOR);
	m_pBProgressBar->SetPercent( fPercent );

	RegulateRGB();
	m_RGBControl->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR, m_nRegG, m_nRegB ) );
	m_RGBControl1->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR1, m_nRegG1, m_nRegB1 ) );
	m_RGBControl2->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR2, m_nRegG2, m_nRegB2 ) );
	m_RGBControl3->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR3, m_nRegG3, m_nRegB3 ) );
	m_RGBControl4->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR4, m_nRegG4, m_nRegB4 ) );
	m_RGBControl5->SetDiffuse( D3DCOLOR_ARGB( 255, m_nRegR5, m_nRegG5, m_nRegB5 ) );
}

void CBikeColorCard::InitUIControlDummy()
{
	CString strTemp;

	switch(nPartSelect)
	{
		case 1: m_nRDummy=m_nR1; m_nGDummy=m_nG1; m_nBDummy=m_nB1;	break;
		case 2: m_nRDummy=m_nR2; m_nGDummy=m_nG2; m_nBDummy=m_nB2;	break;
		case 3: m_nRDummy=m_nR3; m_nGDummy=m_nG3; m_nBDummy=m_nB3;	break;
		case 4: m_nRDummy=m_nR4; m_nGDummy=m_nG4; m_nBDummy=m_nB4;	break;
		case 5: m_nRDummy=m_nR5; m_nGDummy=m_nG5; m_nBDummy=m_nB5;	break;
		default: m_nRDummy=m_nR; m_nGDummy=m_nG; m_nBDummy=m_nB;	break;
	}

	strTemp.Format( "%d", m_nRDummy );
	m_pREditBox->SetEditString( strTemp );

	strTemp.Format( "%d", m_nGDummy );
	m_pGEditBox->SetEditString( strTemp );

	strTemp.Format( "%d", m_nBDummy );
	m_pBEditBox->SetEditString( strTemp );

	float fPercent(0);
	if( m_nRDummy > 0 )	fPercent = m_nRDummy / float(nMAX_COLOR);
	m_pRProgressBar->SetPercent( fPercent );

	fPercent = 0;
	if( m_nGDummy > 0 )	fPercent = m_nGDummy / float(nMAX_COLOR);
	m_pGProgressBar->SetPercent( fPercent );

	fPercent = 0;
	if( m_nBDummy > 0 )	fPercent = m_nBDummy / float(nMAX_COLOR);
	m_pBProgressBar->SetPercent( fPercent );
}

void CBikeColorCard::RegulateRGB()
{
	m_nRegR = int(m_nR * fRGB_RATIO);
	m_nRegG = int(m_nG * fRGB_RATIO);
	m_nRegB = int(m_nB * fRGB_RATIO);

	m_nRegR1 = int(m_nR1 * fRGB_RATIO);
	m_nRegG1 = int(m_nG1 * fRGB_RATIO);
	m_nRegB1 = int(m_nB1 * fRGB_RATIO);

	m_nRegR2 = int(m_nR2 * fRGB_RATIO);
	m_nRegG2 = int(m_nG2 * fRGB_RATIO);
	m_nRegB2 = int(m_nB2 * fRGB_RATIO);

	m_nRegR3 = int(m_nR3 * fRGB_RATIO);
	m_nRegG3 = int(m_nG3 * fRGB_RATIO);
	m_nRegB3 = int(m_nB3 * fRGB_RATIO);

	m_nRegR4 = int(m_nR4 * fRGB_RATIO);
	m_nRegG4 = int(m_nG4 * fRGB_RATIO);
	m_nRegB4 = int(m_nB4 * fRGB_RATIO);

	m_nRegR5 = int(m_nR5 * fRGB_RATIO);
	m_nRegG5 = int(m_nG5 * fRGB_RATIO);
	m_nRegB5 = int(m_nB5 * fRGB_RATIO);
}

void CBikeColorCard::ClearEditBox()
{
	m_pREditBox->ClearEdit();
	m_pREditBox->EndEdit();

	m_pGEditBox->ClearEdit();
	m_pGEditBox->EndEdit();

	m_pBEditBox->ClearEdit();
	m_pBEditBox->EndEdit();
}