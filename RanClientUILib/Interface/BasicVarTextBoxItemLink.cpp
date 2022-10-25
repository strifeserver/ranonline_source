#include "StdAfx.h"

#include "./BasicVarTextBoxItemLink.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
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

const	float	CBasicVarTextBoxItemLink::fMOUSEPOINT_GAP = 20.0f;

CBasicVarTextBoxItemLink::CBasicVarTextBoxItemLink () :
	m_pTextBox ( NULL ),
	m_pTextBoxName ( NULL ),
	m_pLineBox ( NULL ),
	m_pFont ( NULL ),
	//m_pFont ( NULL ),
	m_pSelfDummy ( NULL ),
	m_pItemImageRender ( NULL ),
	m_vMousePointGap (fMOUSEPOINT_GAP,fMOUSEPOINT_GAP),
	m_bBLOCK_MOUSETRACKING ( false ),
	m_pElecIcon(NULL),
	m_pFireIcon(NULL),
	m_pIceIcon(NULL),
	m_pPoiIcon(NULL),
	m_pSpiIcon(NULL)

{
}

CBasicVarTextBoxItemLink::~CBasicVarTextBoxItemLink ()
{
}

CItemImage*	CBasicVarTextBoxItemLink::CreateItemImage ( const char* szControl, UIGUID controlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_XSIZE|UI_FLAG_YSIZE, controlID );
	pItemImage->CreateSubControl ();
//	pItemImage->SetUseRender ( TRUE );
//	pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( pItemImage );

	return pItemImage;
}

void CBasicVarTextBoxItemLink::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	m_pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12, _DEFAULT_FONT_FLAG );

	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_VAR_LINE_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxDialogueDarkGray ( "BASIC_VAR_LINE_BOX" );
	pLineBox->SetNoUpdate ( true );
//	pLineBox->SetDiffuse (  D3DCOLOR_ARGB(190,0xFF,0xFF,0xFF) );
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
	pTextBox->SetTextAlign ( TEXT_ALIGN_LEFT );
	pTextBox->SetNoUpdate ( true );
	RegisterControl ( pTextBox );
	m_pTextBox = pTextBox;


	CBasicTextBox* pTextBoxName = new CBasicTextBox;
	pTextBoxName->CreateSub ( this, "VAR_TEXT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pTextBoxName->SetFont ( m_pFont10 );
	pTextBoxName->SetTextAlign ( TEXT_ALIGN_LEFT );
	pTextBoxName->SetNoUpdate ( true );
	RegisterControl ( pTextBoxName );
	m_pTextBoxName = pTextBoxName;

	{
		CString strItemBox[ITEM::SBOX::ITEM_SIZE] = 
		{
			"ITEM_BOX_IMAGE0",
			"ITEM_BOX_IMAGE1",
			"ITEM_BOX_IMAGE2",
			"ITEM_BOX_IMAGE3",
			"ITEM_BOX_IMAGE4",
			"ITEM_BOX_IMAGE5",
			"ITEM_BOX_IMAGE6",
			"ITEM_BOX_IMAGE7",
			"ITEM_BOX_IMAGE8",
			"ITEM_BOX_IMAGE9",
		};

		for ( int i = 0; i < ITEM::SBOX::ITEM_SIZE; i++ )
		{
			m_pItemImage[i] = CreateItemImage ( strItemBox[i].GetString (), ITEM_IMAGE0+i );
		}
	}

	{
		m_pElecIcon = CreateControl ( "ELEC_ITEM_IMAGE" );
		m_pFireIcon = CreateControl ( "FIRE_ITEM_IMAGE" );
		m_pIceIcon = CreateControl  ( "ICE_ITEM_IMAGE"  );
		m_pPoiIcon = CreateControl  ( "POI_ITEM_IMAGE"  );
		m_pSpiIcon = CreateControl  ( "SPI_ITEM_IMAGE"  );

		m_pElecIcon->SetVisibleSingle ( FALSE );
		m_pFireIcon->SetVisibleSingle ( FALSE );
		m_pIceIcon->SetVisibleSingle ( FALSE );
		m_pPoiIcon->SetVisibleSingle ( FALSE );
		m_pSpiIcon->SetVisibleSingle ( FALSE );
	}
}

void CBasicVarTextBoxItemLink::SetTextAlign ( int nALIGN )
{
	m_pTextBox->SetTextAlign ( nALIGN );
}

void CBasicVarTextBoxItemLink::ClearText ()
{
	if ( m_pTextBox ) m_pTextBox->ClearText ();
	if ( m_pTextBoxName ) m_pTextBoxName->ClearText ();
	//	¹Ú½º Å©±â ÃÖ¼ÒÈ­
}

int CBasicVarTextBoxItemLink::AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddTextNoSplit ( strText, dwColor );

	return -1;
}

int CBasicVarTextBoxItemLink::AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBoxName ) return m_pTextBoxName->AddTextNoSplitBig ( strText, dwColor );
								 m_pTextBoxName->SetFont ( m_pFont10 );
	return -1;
}

int	CBasicVarTextBoxItemLink::SetTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->SetTextNoSplit ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxItemLink::SetText( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) m_pTextBox->SetText( strText, dwColor );
}

void CBasicVarTextBoxItemLink::AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_pTextBox ) m_pTextBox->AddString ( nIndex, strText, dwColor );
}

int CBasicVarTextBoxItemLink::AddText ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddText ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxItemLink::SetUseOverColor ( bool bUseColor )
{
	m_pTextBox->SetUseOverColor ( (bUseColor)?TRUE:FALSE );
}

void CBasicVarTextBoxItemLink::SetOverColor ( const D3DCOLOR& dwColor )
{
	int nIndex = m_pTextBox->GetCount () - 1;
	m_pTextBox->SetOverColor ( nIndex, dwColor );
}

void CBasicVarTextBoxItemLink::RePosControl ( int x, int y )
{
	const float fTextBoxGapX_ = (float)m_pTextBox->GetLongestLine ();
	const float fTextBoxGapXName_ = (float)m_pTextBoxName->GetLongestLine ();

	if ( fTextBoxGapXName_ > fTextBoxGapX_ )
	{
		const UIRECT& rcTextBoxLocalPos = m_pTextBoxName->GetLocalPos ();
		const UIRECT& rcLocalPosDummy = m_pSelfDummy->GetLocalPos ();
		const UIRECT& rcOriginPos = GetLocalPos ();

		AlignSubControl ( rcOriginPos, rcLocalPosDummy );

		if ( m_pTextBoxName )
		{
			int nTotalLine = m_pTextBox->GetTotalLine ();
			const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
			const float fTextBoxGapX = (float)m_pTextBoxName->GetLongestLine () - rcTextBoxLocalPos.sizeX;
			const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;
			
			UIRECT rcLocalNewPos = UIRECT ( 387/*float(x)*/ /*+ m_vMousePointGap.x*/, 529/*float(y)*/ /*+ m_vMousePointGap.y*/,
				rcOriginPos.sizeX + fTextBoxGapX, rcOriginPos.sizeY + fTextBoxGapY );

			AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

			SetGlobalPos ( rcLocalNewPos );		
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
			
			UIRECT rcLocalNewPos = UIRECT ( 387/*float(x)*/ /*+ m_vMousePointGap.x*/, 529/*float(y)*/ /*+ m_vMousePointGap.y*/,
				rcOriginPos.sizeX + fTextBoxGapX, rcOriginPos.sizeY + fTextBoxGapY );

			AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

			SetGlobalPos ( rcLocalNewPos );		
		}
	}
}

void CBasicVarTextBoxItemLink::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	SetUseOverColor ( false );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( !m_bBLOCK_MOUSETRACKING ) RePosControl ( x, y ); 

}

void CBasicVarTextBoxItemLink::SetMousePointGap ( D3DXVECTOR2 vGap )
{
	m_vMousePointGap = vGap;
}

float CBasicVarTextBoxItemLink::GetLongestLine ()
{
	if ( m_pTextBox )
	{
		return (float)m_pTextBox->GetLongestLine ();
	}
	return 0.0f;
}

void CBasicVarTextBoxItemLink::AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
{
	float fLongestLine = GetLongestLine ();
	
	NS_UITEXTCONTROL::MULTILINETEXT& MLTextSet =
		NS_UITEXTCONTROL::GetMLTextWithoutCRLF ( strText, fLongestLine, m_pFont );

	for ( int i = 0; i < (int)MLTextSet.size(); ++i )
	{
		CString strTemp = MLTextSet[i].strLine;
		if ( strTemp.GetLength() )
		{
			AddTextNoSplit ( strTemp, NS_UITEXTCOLOR::DEFAULT );		
		}
	}	
}

int	CBasicVarTextBoxItemLink::GetCount ()
{
	return m_pTextBox->GetCount ();
}

void CBasicVarTextBoxItemLink::SetLineInterval ( const float fLineInterval )
{
	m_pTextBox->SetLineInterval ( fLineInterval );
}

void CBasicVarTextBoxItemLink::SetBlockMouseTracking ( bool bBlock )
{
	m_bBLOCK_MOUSETRACKING = bBlock;
}

void CBasicVarTextBoxItemLink::SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sICONINDEX );
	if ( !pItem ) m_pItemImageRender->SetVisibleSingle ( FALSE );

	if ( m_pItemImageRender )
	{
		m_pItemImageRender->SetItem( sICONINDEX, szTexture );
		m_pItemImageRender->SetVisibleSingle ( TRUE );
	}
}

void CBasicVarTextBoxItemLink::ResetItemRender ()
{
	if ( m_pItemImageRender )
	{
		m_pItemImageRender->ResetItem();
		m_pItemImageRender->SetVisibleSingle ( FALSE );
	}
}

void CBasicVarTextBoxItemLink::SetItemBoxRender ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sICONINDEX );
	if ( !pItem ) m_pItemImageRender->SetVisibleSingle ( FALSE );
		
	if ( m_pItemImage[nIndex] )
	{
		m_pItemImage[nIndex]->SetItem( sICONINDEX, szTexture );
		m_pItemImage[nIndex]->SetVisibleSingle ( TRUE );
	}
}

void CBasicVarTextBoxItemLink::ResetItemBoxRender ()
{
	for ( int i = 0; i < ITEM::SBOX::ITEM_SIZE; i++ )
	{
		if ( m_pItemImage[i] )
		{
			m_pItemImage[i]->ResetItem();
			m_pItemImage[i]->SetVisibleSingle ( FALSE );
		}
	}
}

void CBasicVarTextBoxItemLink::SetResiIcon()
{
	m_pElecIcon->SetVisibleSingle ( TRUE );
	m_pFireIcon->SetVisibleSingle ( TRUE );
	m_pIceIcon->SetVisibleSingle ( TRUE );
	m_pPoiIcon->SetVisibleSingle ( TRUE );
	m_pSpiIcon->SetVisibleSingle ( TRUE );
}

void CBasicVarTextBoxItemLink::HideResiIcon()
{
	m_pElecIcon->SetVisibleSingle ( FALSE );
	m_pFireIcon->SetVisibleSingle ( FALSE );
	m_pIceIcon->SetVisibleSingle ( FALSE );
	m_pPoiIcon->SetVisibleSingle ( FALSE );
	m_pSpiIcon->SetVisibleSingle ( FALSE );
}

void CBasicVarTextBoxItemLink::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ITEM_IMAGE0:
		{
			
		}
		break;
	}
}