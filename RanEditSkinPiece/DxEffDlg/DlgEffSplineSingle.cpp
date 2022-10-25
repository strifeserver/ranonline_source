// DlgEffSplineSingle.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffSplineSingle.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffsplinesingle.h"

// CDlgEffSplineSingle dialog

IMPLEMENT_DYNAMIC(CDlgEffSplineSingle, CPropertyPage)
CDlgEffSplineSingle::CDlgEffSplineSingle( LOGFONT logfont )
	: CPropertyPage(CDlgEffSplineSingle::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffSplineSingle::~CDlgEffSplineSingle()
{
}

void CDlgEffSplineSingle::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffSplineSingle, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
END_MESSAGE_MAP()


// CDlgEffSplineSingle message handlers
BOOL CDlgEffSplineSingle::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	return TRUE;
}

void CDlgEffSplineSingle::SetData( DxSkinPiece* pData, DxEffCharSplineSingleEff* pEff, BOOL bNEW )
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

void CDlgEffSplineSingle::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_SPLINESINGLEEFF* pProperty = (EFFCHAR_PROPERTY_SPLINESINGLEEFF*)m_pEff->GetProperty();
		if ( pProperty )
		{
		}
	}
}

BOOL CDlgEffSplineSingle::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_SPLINESINGLEEFF Property;

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

void CDlgEffSplineSingle::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffSplineSingle::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffSplineSingle::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}
