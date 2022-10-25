// DlgEffAlpha.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffAlpha.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffalpha.h"


// CDlgEffAlpha dialog

IMPLEMENT_DYNAMIC(CDlgEffAlpha, CPropertyPage)
CDlgEffAlpha::CDlgEffAlpha( LOGFONT logfont )
	: CPropertyPage(CDlgEffAlpha::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffAlpha::~CDlgEffAlpha()
{
}

void CDlgEffAlpha::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffAlpha, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
END_MESSAGE_MAP()


// CDlgEffAlpha message handlers
BOOL CDlgEffAlpha::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	return TRUE;
}

void CDlgEffAlpha::SetData( DxSkinPiece* pData, DxEffCharAlpha* pEff, BOOL bNEW )
{
	if ( pData ) 
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pPiece = pData;
		m_pEff = pEff;
		bNEWEFF = bNEW;

		if ( m_pPiece && m_pEff )
		{
			ShowData();
		}
	}
}

void CDlgEffAlpha::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_ALPHA* pProperty = (EFFCHAR_PROPERTY_ALPHA*)m_pEff->GetProperty();
		if( pProperty )
		{
			SetWin_Num_float( this, IDC_EDIT_ALPHA, pProperty->m_fAlpha );
			SetWin_Num_float( this, IDC_EDIT_PLAYTIME, pProperty->m_fPlayTime );
			SetWin_Num_float( this, IDC_EDIT_WAITTIME, pProperty->m_fWaitTime );
			SetWin_Check( this, IDC_CHECK_USE_TRANSLUCENT, pProperty->m_dwFlags&USETRANSLUCENT );
		}
	}
}

BOOL CDlgEffAlpha::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_ALPHA Property;

		Property.m_fAlpha = GetWin_Num_float( this, IDC_EDIT_ALPHA );
		Property.m_fPlayTime = GetWin_Num_float( this, IDC_EDIT_PLAYTIME );
		Property.m_fWaitTime = GetWin_Num_float( this, IDC_EDIT_WAITTIME );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_TRANSLUCENT ), Property.m_dwFlags, USETRANSLUCENT );

		m_pEff->SetProperty( &Property );
		HRESULT hr = m_pEff->Create( CRanEditSkinPieceView::GetView()->Get3DDevice() );
		if ( FAILED(hr) )
		{
			return FALSE;
		}

		if ( bNEWEFF )
		{
			m_pPiece->AddEffList( m_pEff );
			bNEWEFF = FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

void CDlgEffAlpha::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffAlpha::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffAlpha::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}
