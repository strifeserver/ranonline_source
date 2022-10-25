#include "StdAfx.h"

#include "./BasicVarTextBox.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "./BasicLineBoxEx.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "../EngineUIlib/GUInterface/UIDebugSet.h"
#include "ItemImage.h" 
#include "GLItemMan.h" 
#include "GLItem.h" 
#include "GLGaeaClient.h"  
#include "d3dfont.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	float	CBasicVarTextBox::fMOUSEPOINT_GAP = 20.0f;

CBasicVarTextBox::CBasicVarTextBox () :
	m_pTextBox ( NULL ),
	m_pTextBoxName ( NULL ),
	m_pLineBox ( NULL ),
	m_pFont ( NULL ),
	m_pFont10 ( NULL ),
	m_pSelfDummy ( NULL ),
	m_pItemImageRender ( NULL ),
	m_vMousePointGap (fMOUSEPOINT_GAP,fMOUSEPOINT_GAP),
	m_bBLOCK_MOUSETRACKING ( false ),
	m_pElecIcon(NULL),
	m_pFireIcon(NULL),
	m_pIceIcon(NULL),
	m_pPoiIcon(NULL),
	m_pSpiIcon(NULL),
	m_pPremiumIcon(NULL),
	m_pSelfDummyGray(NULL)

{
}

CBasicVarTextBox::~CBasicVarTextBox ()
{
}

CItemImage*	CBasicVarTextBox::CreateItemImage ( const char* szControl, UIGUID controlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_XSIZE|UI_FLAG_YSIZE, controlID );
	pItemImage->CreateSubControl ();
	pItemImage->SetUseRender ( TRUE );
	pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( pItemImage );

    return pItemImage; 
}

void CBasicVarTextBox::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT,	10, _DEFAULT_FONT_FLAG );
	m_pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT , 13, _DEFAULT_FONT_FLAG | D3DFONT_BOLD );

	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_EX_VARTEXT_GRAY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxDialogueDarkGray ("BASIC_VAR_LINE_BOX" );
	pLineBox->SetNoUpdate ( true );
	//pLineBox->SetDiffuse (  D3DCOLOR_ARGB(0xFF,0xA9,0xA9,0xA9) );
	RegisterControl ( pLineBox );
	m_pLineBox = pLineBox;

	CUIControl* pSelfDummy = new CUIControl;
	pSelfDummy->CreateSub ( this, "BASIC_LINE_BOX_EX_VARTEXT_GRAY" );
	pSelfDummy->SetVisibleSingle ( FALSE );
	RegisterControl ( pSelfDummy );
	m_pSelfDummyGray = pSelfDummy;
	
	CUIControl* pSelfDummy2 = new CUIControl;
	pSelfDummy2->CreateSub ( this, "BASIC_VAR_LINE_BOX" );
	pSelfDummy2->SetVisibleSingle ( FALSE );
	RegisterControl ( pSelfDummy2 );
	m_pSelfDummy = pSelfDummy2;

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

		for ( int i = 0; i < ITEM::SBOX::ITEM_SIZE; i++ )
		{
			CString strBox;
			strBox.Format ( "ITEM_BOX_IMAGE%d", i );
			m_pItemImage[i] = CreateItemImage ( strBox.GetString (), ITEM_IMAGE0 +i );
			m_pItemImage[i]->SetUseRender ( TRUE );
			m_pItemImage[i]->SetVisibleSingle ( FALSE );
			m_pItemImage[i]->CreateSubControl ();
			RegisterControl ( m_pItemImage[i] );
			
		}

	}

	{
		CItemImage* pItemImage = new CItemImage;
		pItemImage->CreateSub ( this, "ITEM_IMAGE_RENDER" );
		pItemImage->CreateSubControl ();
		pItemImage->SetUseRender ( TRUE );
		pItemImage->SetVisibleSingle ( FALSE );
		RegisterControl ( pItemImage );
		m_pItemImageRender = pItemImage;
	}

	
 // RESIST ICON	
	CUIControl* pElecIcon = new CUIControl;
	pElecIcon->CreateSub ( this, "ELEC_ITEM_IMAGE" );
	pElecIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( pElecIcon );
	m_pElecIcon = pElecIcon;

	CUIControl* pFireIcon = new CUIControl;
	pFireIcon->CreateSub ( this, "FIRE_ITEM_IMAGE" );
	pFireIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( pFireIcon );
	m_pFireIcon = pFireIcon;

	CUIControl* pIceIcon = new CUIControl;
	pIceIcon->CreateSub ( this, "ICE_ITEM_IMAGE" );
	pIceIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( pIceIcon );
	m_pIceIcon = pIceIcon;

	CUIControl* pPoiIcon = new CUIControl;
	pPoiIcon->CreateSub ( this, "POI_ITEM_IMAGE" );
	pPoiIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( pPoiIcon );
	m_pPoiIcon = pPoiIcon;

	CUIControl* pSpiIcon = new CUIControl;
	pSpiIcon->CreateSub ( this, "SPI_ITEM_IMAGE" );
	pSpiIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( pSpiIcon );
	m_pSpiIcon = pSpiIcon;


	/*CUIControl* pPremiumIcon = new CUIControl;
	pPremiumIcon->CreateSub ( this, "PREMIUM_ICON" );
	pPremiumIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( pPremiumIcon );
	m_pPremiumIcon = pPremiumIcon;*/
	

	
	/**	
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
		**/
	}
		
void CBasicVarTextBox::SetTextAlign ( int nALIGN )
{
	m_pTextBox->SetTextAlign ( nALIGN );
}

void CBasicVarTextBox::ClearText ()
{
	if ( m_pTextBox ) m_pTextBox->ClearText ();
	if ( m_pTextBoxName ) m_pTextBoxName->ClearText ();
	//	박스 크기 최소화
}

int CBasicVarTextBox::AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddTextNoSplit ( strText, dwColor );

	return -1;
}

int CBasicVarTextBox::AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBoxName ) return m_pTextBoxName->AddTextNoSplit ( strText, dwColor );
	m_pTextBoxName->SetFont ( m_pFont10 );

	return -1;
}

int	CBasicVarTextBox::SetTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->SetTextNoSplit ( strText, dwColor );

	return -1;
}

void CBasicVarTextBox::SetText( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) m_pTextBox->SetText( strText, dwColor );
}

void CBasicVarTextBox::AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_pTextBox ) m_pTextBox->AddString ( nIndex, strText, dwColor );
}

int CBasicVarTextBox::AddText ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddText ( strText, dwColor );

	return -1;
}

void CBasicVarTextBox::SetUseOverColor ( bool bUseColor )
{
	m_pTextBox->SetUseOverColor ( (bUseColor)?TRUE:FALSE );
}

void CBasicVarTextBox::SetOverColor ( const D3DCOLOR& dwColor )
{
	int nIndex = m_pTextBox->GetCount () - 1;
	m_pTextBox->SetOverColor ( nIndex, dwColor );
}

void CBasicVarTextBox::RePosControl ( int x, int y )
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
			
			//UIRECT rcLocalNewPos = UIRECT ( 387/*float(x)*/ /*+ m_vMousePointGap.x*/, 529/*float(y)*/ /*+ m_vMousePointGap.y*/,
			//	rcOriginPos.sizeX + fTextBoxGapX, rcOriginPos.sizeY + fTextBoxGapY );

			UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
			(rcOriginPos.sizeX + 5.0f) + (fTextBoxGapX + 5.0f), rcOriginPos.sizeY + fTextBoxGapY );

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
			
			//UIRECT rcLocalNewPos = UIRECT ( 387/*float(x)*/ /*+ m_vMousePointGap.x*/, 529/*float(y)*/ /*+ m_vMousePointGap.y*/,
			//	rcOriginPos.sizeX + fTextBoxGapX, rcOriginPos.sizeY + fTextBoxGapY );

			UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
			(rcOriginPos.sizeX + 0.0f) + (fTextBoxGapX + 0.0f), rcOriginPos.sizeY + fTextBoxGapY );

			AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

			SetGlobalPos ( rcLocalNewPos );		
		}
	}
}

void CBasicVarTextBox::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	SetUseOverColor ( false );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_bCHECK_MOUSE_STATE = false;

	m_PosX = x;
	m_PosY = y;

	if ( IsExclusiveSelfControl() )
	{
		SetLocalPos ( D3DXVECTOR2 ( x - m_vGap.x, y - m_vGap.y ) );
	}

	if ( !m_bBLOCK_MOUSETRACKING ) RePosControl ( x, y ); 

	m_pLineBox->SetDiffuse (  D3DCOLOR_ARGB(195,0xF5,0xF2,0xF3) );
}

void CBasicVarTextBox::SetMousePointGap ( D3DXVECTOR2 vGap )
{
	m_vMousePointGap = vGap;
}

float CBasicVarTextBox::GetLongestLine ()
{
	if ( m_pTextBox )
	{
		return (float)m_pTextBox->GetLongestLine ();
	}
	return 0.0f;
}

void CBasicVarTextBox::AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
{
	float fLongestLine = GetLongestLine ();
	float fLongLine = float(m_pTextBoxName->GetLongestLine());

	if ( fLongLine > fLongestLine )
	{
		NS_UITEXTCONTROL::MULTILINETEXT& MLTextSet = NS_UITEXTCONTROL::GetMLTextWithoutCRLF ( strText, fLongLine, m_pFont );

		for ( int i = 0; i < (int)MLTextSet.size(); ++i )
		{
			CString strTemp = MLTextSet[i].strLine;
			if ( strTemp.GetLength() )
			{
				AddTextNoSplit ( strTemp, dwColor );		
			}
		}	
	}
	else
	{
		NS_UITEXTCONTROL::MULTILINETEXT& MLTextSet = NS_UITEXTCONTROL::GetMLTextWithoutCRLF ( strText, fLongestLine, m_pFont );

		for ( int i = 0; i < (int)MLTextSet.size(); ++i )
		{
			CString strTemp = MLTextSet[i].strLine;
			if ( strTemp.GetLength() )
			{
				AddTextNoSplit ( strTemp, dwColor );	
			}
		}
	}
}

int	CBasicVarTextBox::GetCount ()
{
	return m_pTextBox->GetCount ();
}

void CBasicVarTextBox::SetLineInterval ( const float fLineInterval )
{
	m_pTextBox->SetLineInterval ( fLineInterval );
}

void CBasicVarTextBox::SetBlockMouseTracking ( bool bBlock )
{
	m_bBLOCK_MOUSETRACKING = bBlock;
}

void CBasicVarTextBox::SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture ) 
{ 
    SITEM* pItem = GLItemMan::GetInstance().GetItem ( sICONINDEX ); 
    if ( !pItem ) m_pItemImageRender->SetVisibleSingle ( FALSE ); 

    if ( m_pItemImageRender ) 
    { 
        m_pItemImageRender->SetItem( sICONINDEX, szTexture ); 
        m_pItemImageRender->SetVisibleSingle ( TRUE ); 
    } 
} 

void CBasicVarTextBox::ResetItemRender () 
{ 
    if ( m_pItemImageRender ) 
    { 
        m_pItemImageRender->ResetItem(); 
        m_pItemImageRender->SetVisibleSingle ( FALSE ); 
    } 
}  
void CBasicVarTextBox::SetResiIcon()
{
	m_pElecIcon->SetVisibleSingle ( FALSE );
	m_pFireIcon->SetVisibleSingle ( FALSE );
	m_pIceIcon->SetVisibleSingle ( FALSE );
	m_pPoiIcon->SetVisibleSingle ( FALSE );
	m_pSpiIcon->SetVisibleSingle ( FALSE );
}

void CBasicVarTextBox::ResetResiIcon()
{
	m_pElecIcon->SetVisibleSingle ( FALSE );
	m_pFireIcon->SetVisibleSingle ( FALSE );
	m_pIceIcon->SetVisibleSingle ( FALSE );
	m_pPoiIcon->SetVisibleSingle ( FALSE );
	m_pSpiIcon->SetVisibleSingle ( FALSE );
}
void CBasicVarTextBox::SetPremium()
{
	
	
}
void CBasicVarTextBox::ResetPremium()
{

}
void CBasicVarTextBox::SetItemBoxRender ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex )
{
	if ( m_pItemImage[nIndex] )
	{
		m_pItemImage[nIndex]->SetItem( sICONINDEX, szTexture );
		m_pItemImage[nIndex]->SetVisibleSingle ( TRUE );
	}
}

void CBasicVarTextBox::ResetItemBoxRender ()
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

void CBasicVarTextBox::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ITEM_IMAGE0:
		{
			
		}
		break;
	}
}