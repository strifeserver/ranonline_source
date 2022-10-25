// DlgEffShock.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffShock.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffshock.h"
#include "../SelColorDlg.h"

// CDlgEffShock dialog

IMPLEMENT_DYNAMIC(CDlgEffShock, CPropertyPage)
CDlgEffShock::CDlgEffShock( LOGFONT logfont )
	: CPropertyPage(CDlgEffShock::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffShock::~CDlgEffShock()
{
}

void CDlgEffShock::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffShock, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
END_MESSAGE_MAP()


// CDlgEffShock message handlers
BOOL CDlgEffShock::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	return TRUE;
}

void CDlgEffShock::SetData( DxSkinPiece* pData, DxEffCharShock* pEff, BOOL bNEW )
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

void CDlgEffShock::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_SHOCK* pProperty = (EFFCHAR_PROPERTY_SHOCK*)m_pEff->GetProperty();
		if( pProperty )
		{
			SetWin_Num_float( this, IDC_EDIT_COLOR_A_1, pProperty->m_cColor.a );
			SetWin_Num_float( this, IDC_EDIT_COLOR_R_1, pProperty->m_cColor.r );
			SetWin_Num_float( this, IDC_EDIT_COLOR_G_1, pProperty->m_cColor.g );
			SetWin_Num_float( this, IDC_EDIT_COLOR_B_1, pProperty->m_cColor.b );
			SetWin_Num_float( this, IDC_EDIT_FULLTIME, pProperty->m_fFullTime );
		}
	}
}

BOOL CDlgEffShock::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_SHOCK Property;
		Property.m_cColor.a = GetWin_Num_float( this, IDC_EDIT_COLOR_A_1 );
		Property.m_cColor.r = GetWin_Num_float( this, IDC_EDIT_COLOR_R_1 );
		Property.m_cColor.g = GetWin_Num_float( this, IDC_EDIT_COLOR_G_1 );
		Property.m_cColor.b = GetWin_Num_float( this, IDC_EDIT_COLOR_B_1 );
		Property.m_fFullTime = GetWin_Num_float( this, IDC_EDIT_FULLTIME );

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

void CDlgEffShock::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffShock::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffShock::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffShock::OnBnClickedButtonColor()
{
	float fr = GetWin_Num_float( this, IDC_EDIT_COLOR_R_1 );
	float fg = GetWin_Num_float( this, IDC_EDIT_COLOR_G_1 );
	float fb = GetWin_Num_float( this, IDC_EDIT_COLOR_B_1 );

	BYTE nCUR_R = BYTE ( fr * 255.0f );
	BYTE nCUR_G = BYTE ( fg * 255.0f );
	BYTE nCUR_B = BYTE ( fb * 255.0f );

	COLORREF Color = RGB(nCUR_R, nCUR_G, nCUR_B );
	CSelColorDlg dlg(GetRValue(Color), GetGValue(Color),GetBValue(Color));

	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		float fR = (float)GetRValue( crNewColor );
		float fG = (float)GetGValue( crNewColor );
		float fB = (float)GetBValue( crNewColor );

		SetWin_Num_float( this, IDC_EDIT_COLOR_R_1, fR / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_COLOR_G_1, fG / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_COLOR_B_1, fB / 255.0f );
	}
}
