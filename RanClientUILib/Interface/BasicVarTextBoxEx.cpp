#include "StdAfx.h"

#include "./BasicVarTextBoxEx.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "../EngineUIlib/GUInterface/BasicButton.h"
#include "./BasicLineBoxEx.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "../EngineUIlib/GUInterface/UIDebugSet.h"
#include "ItemImage.h"
#include "GLItemMan.h"
#include "GLItem.h"
#include "GLGaeaClient.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	float	CBasicVarTextBoxEx::fMOUSEPOINT_GAP = 20.0f;

CBasicVarTextBoxEx::CBasicVarTextBoxEx () :
	m_pTextBox ( NULL ),
	m_pTextBoxBig ( NULL ),
	m_pLineBox ( NULL ),
	m_pFont ( NULL ),
	m_pSelfDummy ( NULL ),
	m_pCloseButton ( NULL ),
	m_pItemImageRender ( NULL ),
	m_vMousePointGap (fMOUSEPOINT_GAP,fMOUSEPOINT_GAP),
	m_bBLOCK_MOUSETRACKING ( false )
{
}

CBasicVarTextBoxEx::~CBasicVarTextBoxEx ()
{
}

CItemImage*	CBasicVarTextBoxEx::CreateItemImage ( const char* szControl, UIGUID controlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_XSIZE|UI_FLAG_YSIZE, controlID );
	pItemImage->CreateSubControl ();
//	pItemImage->SetUseRender ( TRUE );
//	pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( pItemImage );

	return pItemImage;
}
void CBasicVarTextBoxEx::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );
	m_pFont12 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12, _DEFAULT_FONT_FLAG );

	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_VAR_LINE_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxDialogueDarkGray ( "BASIC_VAR_LINE_BOX" );
	pLineBox->SetNoUpdate ( true );
	//pLineBox->SetDiffuse (  D3DCOLOR_ARGB(0xFF,0x00,0x00,0x00) );
	RegisterControl ( pLineBox );
	m_pLineBox = pLineBox;

	CUIControl* pSelfDummy = new CUIControl;
	pSelfDummy->CreateSub ( this, "BASIC_VAR_LINE_BOX" );
	pSelfDummy->SetVisibleSingle ( FALSE );
	RegisterControl ( pSelfDummy );
	m_pSelfDummy = pSelfDummy;

	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, "ITEM_IMAGE_RENDER" );
	pItemImage->CreateSubControl ();
	pItemImage->SetUseRender ( TRUE );
	pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( pItemImage );
    m_pItemImageRender = pItemImage;
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, "VAR_TEXT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pTextBox->SetFont ( m_pFont );
	pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	pTextBox->SetNoUpdate ( true );
	RegisterControl ( pTextBox );
	m_pTextBox = pTextBox;

	CBasicTextBox* pTextBoxBig = new CBasicTextBox;
	pTextBoxBig->CreateSub ( this, "VAR_TEXT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pTextBoxBig->SetFont ( m_pFont12 );
	pTextBoxBig->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	pTextBox->SetNoUpdate ( true );
	RegisterControl ( pTextBoxBig );
	m_pTextBoxBig = pTextBoxBig;

	m_pCloseButton = new CBasicButton;
	m_pCloseButton->CreateSub ( this, "INFO_DISPLAY_LINK_CLOSE" , UI_FLAG_DEFAULT, BUTTON_CLOSE );
	m_pCloseButton->CreateFlip ( "INFO_DISPLAY_LINK_CLOSE_F", CBasicButton::CLICK_FLIP );
	m_pCloseButton->CreateMouseOver( "INFO_DISPLAY_LINK_CLOSE_OVER" );
	m_pCloseButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pCloseButton );
}

void CBasicVarTextBoxEx::SetTextAlign ( int nALIGN )
{
	m_pTextBox->SetTextAlign ( nALIGN );
}

void CBasicVarTextBoxEx::ClearText ()
{
	if ( m_pTextBox ) m_pTextBox->ClearText ();
	if ( m_pTextBoxBig ) m_pTextBoxBig->ClearText ();
	//	박스 크기 최소화
}

int CBasicVarTextBoxEx::AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddTextNoSplit ( strText, dwColor );

	return -1;
}

int CBasicVarTextBoxEx::AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBoxBig ) return m_pTextBoxBig->AddTextNoSplitBig ( strText, dwColor );
	 m_pTextBoxBig->SetFont ( m_pFont12 );

	return -1;
}

int	CBasicVarTextBoxEx::SetTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->SetTextNoSplit ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxEx::SetText( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) m_pTextBox->SetText( strText, dwColor );
}

void CBasicVarTextBoxEx::AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_pTextBox ) m_pTextBox->AddString ( nIndex, strText, dwColor );
}

int CBasicVarTextBoxEx::AddText ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddText ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxEx::SetUseOverColor ( bool bUseColor )
{
	m_pTextBox->SetUseOverColor ( (bUseColor)?TRUE:FALSE );
}

void CBasicVarTextBoxEx::SetOverColor ( const D3DCOLOR& dwColor )
{
	int nIndex = m_pTextBox->GetCount () - 1;
	m_pTextBox->SetOverColor ( nIndex, dwColor );
}

/*void CBasicVarTextBoxEx::RePosControl ( int x, int y )
{
	const UIRECT& rcTextBoxLocalPos = m_pTextBox->GetLocalPos ();

	const UIRECT& rcLocalPosDummy = m_pSelfDummy->GetLocalPos ();
	const UIRECT& rcOriginPos = GetLocalPos ();

	AlignSubControl ( rcOriginPos, rcLocalPosDummy );

	if ( m_pTextBox )
	{
		int nTotalLine = m_pTextBox->GetTotalLine ();
		const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
		const float fTextBoxGapX = (float)m_pTextBox->GetLongestLine () - rcTextBoxLocalPos.sizeX;
		const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;
		
		UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
			(rcOriginPos.sizeX + 10.0f) + (fTextBoxGapX + 10.0f), rcOriginPos.sizeY + fTextBoxGapY );

		AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

		SetGlobalPos ( rcLocalNewPos );		

		if ( m_pCloseButton )
		{
			const UIRECT& rcOriginPos1 = m_pTextBox->GetLocalPos();
			const UIRECT& rcOriginPos2 = m_pCloseButton->GetLocalPos();
			float fx = rcOriginPos1.right - rcOriginPos2.sizeX;
			float fy = rcOriginPos1.top;
			m_pCloseButton->SetLocalPos( D3DXVECTOR2 ( fx , fy ));
		}
	}
}*/

void CBasicVarTextBoxEx::RePosControl ( int x, int y )
{
	const float fTextBoxGapX_ = (float)m_pTextBox->GetLongestLine ();
	const float fTextBoxGapXName_ = (float)m_pTextBoxBig->GetLongestLine ();

	if ( fTextBoxGapXName_ > fTextBoxGapX_ )
	{
		const UIRECT& rcTextBoxLocalPos = m_pTextBoxBig->GetLocalPos ();
		const UIRECT& rcLocalPosDummy = m_pSelfDummy->GetLocalPos ();
		const UIRECT& rcOriginPos = GetLocalPos ();

		AlignSubControl ( rcOriginPos, rcLocalPosDummy );

		if ( m_pTextBoxBig )
		{
			int nTotalLine = m_pTextBox->GetTotalLine ();
			const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
			const float fTextBoxGapX = (float)m_pTextBoxBig->GetLongestLine () - rcTextBoxLocalPos.sizeX;
			const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;
			
			//UIRECT rcLocalNewPos = UIRECT ( 387/*float(x)*/ /*+ m_vMousePointGap.x*/, 529/*float(y)*/ /*+ m_vMousePointGap.y*/,
			//	rcOriginPos.sizeX + fTextBoxGapX, rcOriginPos.sizeY + fTextBoxGapY );

			UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
			(rcOriginPos.sizeX + 5.0f) + (fTextBoxGapX + 5.0f), rcOriginPos.sizeY + fTextBoxGapY );

			AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

			SetGlobalPos ( rcLocalNewPos );	

			if ( m_pCloseButton )
			{
				const UIRECT& rcOriginPos1 = m_pTextBoxBig->GetLocalPos();
				const UIRECT& rcOriginPos2 = m_pCloseButton->GetLocalPos();
				float fx = rcOriginPos1.right - rcOriginPos2.sizeX;
				float fy = rcOriginPos1.top;
				m_pCloseButton->SetLocalPos( D3DXVECTOR2 ( fx , fy ));
			}
		}
	}
	else
	{
		const UIRECT& rcTextBoxLocalPos = m_pTextBox->GetLocalPos ();
		const UIRECT& rcLocalPosDummy = m_pSelfDummy->GetLocalPos ();
		const UIRECT& rcOriginPos = GetLocalPos ();

		AlignSubControl ( rcOriginPos, rcLocalPosDummy );

		if ( m_pTextBox )
		{
			int nTotalLine = m_pTextBox->GetTotalLine ();
			const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
			const float fTextBoxGapX = (float)m_pTextBox->GetLongestLine () - rcTextBoxLocalPos.sizeX;
			const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;
			
			//UIRECT rcLocalNewPos = UIRECT ( 387/*float(x)*/ /*+ m_vMousePointGap.x*/, 529/*float(y)*/ /*+ m_vMousePointGap.y*/,
			//	rcOriginPos.sizeX + fTextBoxGapX, rcOriginPos.sizeY + fTextBoxGapY );

			UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
			(rcOriginPos.sizeX + 5.0f) + (fTextBoxGapX + 5.0f), rcOriginPos.sizeY + fTextBoxGapY );

			AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

			SetGlobalPos ( rcLocalNewPos );	

			if ( m_pCloseButton )
			{
				const UIRECT& rcOriginPos1 = m_pTextBoxBig->GetLocalPos();
				const UIRECT& rcOriginPos2 = m_pCloseButton->GetLocalPos();
				float fx = rcOriginPos1.right - rcOriginPos2.sizeX;
				float fy = rcOriginPos1.top;
				m_pCloseButton->SetLocalPos( D3DXVECTOR2 ( fx , fy ));
			}
		}
	}
}

void CBasicVarTextBoxEx::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	SetUseOverColor ( false );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( !m_bBLOCK_MOUSETRACKING ) RePosControl ( x, y );	
}

void CBasicVarTextBoxEx::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case BUTTON_CLOSE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_UP & dwMsg )
				{
					AddMessageEx ( UIMSG_UIVARTEXT_BUTTON_CLOSE_CLICK );
				}

				AddMessageEx ( UIMSG_UIVARTEXT_BUTTON_CLOSE_MOUSEIN );
			}
		}break;
	};

	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CBasicVarTextBoxEx::SetMousePointGap ( D3DXVECTOR2 vGap )
{
	m_vMousePointGap = vGap;
}

float CBasicVarTextBoxEx::GetLongestLine ()
{
	if ( m_pTextBox )
	{
		return (float)m_pTextBox->GetLongestLine ();
	}
	return 0.0f;
}

void CBasicVarTextBoxEx::AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
{
	float fLongestLine = GetLongestLine ();
	
	NS_UITEXTCONTROL::MULTILINETEXT& MLTextSet =
		NS_UITEXTCONTROL::GetMLTextWithoutCRLF ( strText, fLongestLine, m_pFont );

	for ( int i = 0; i < (int)MLTextSet.size(); ++i )
	{
		CString strTemp = MLTextSet[i].strLine;
		if ( strTemp.GetLength() )
		{
			AddTextNoSplit ( strTemp, /*NS_UITEXTCOLOR::DEFAULT*/ dwColor );		
		}
	}	
}

int	CBasicVarTextBoxEx::GetCount ()
{
	return m_pTextBox->GetCount ();
}

void CBasicVarTextBoxEx::SetLineInterval ( const float fLineInterval )
{
	m_pTextBox->SetLineInterval ( fLineInterval );
}

void CBasicVarTextBoxEx::SetBlockMouseTracking ( bool bBlock )
{
	m_bBLOCK_MOUSETRACKING = bBlock;
}

void CBasicVarTextBoxEx::SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sICONINDEX );
	if ( !pItem ) m_pItemImageRender->SetVisibleSingle ( FALSE );

	if ( m_pItemImageRender )
	{
		m_pItemImageRender->SetItem( sICONINDEX, szTexture );
		m_pItemImageRender->SetVisibleSingle ( TRUE );
	}
}

void CBasicVarTextBoxEx::ResetItemRender ()
{
	if ( m_pItemImageRender )
	{
		m_pItemImageRender->ResetItem();
		m_pItemImageRender->SetVisibleSingle ( FALSE );
	}
}

void CBasicVarTextBoxEx::ShowCloseButton( bool bSHOW )
{
	if ( m_pCloseButton )
		m_pCloseButton->SetVisibleSingle( bSHOW );
}