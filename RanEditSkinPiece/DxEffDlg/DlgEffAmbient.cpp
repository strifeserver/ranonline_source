// DlgEffAmbient.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffAmbient.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffambient.h"

#include "../SelColorDlg.h"
// CDlgEffAmbient dialog

IMPLEMENT_DYNAMIC(CDlgEffAmbient, CPropertyPage)
CDlgEffAmbient::CDlgEffAmbient( LOGFONT logfont )
	: CPropertyPage(CDlgEffAmbient::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffAmbient::~CDlgEffAmbient()
{
}

void CDlgEffAmbient::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffAmbient, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
END_MESSAGE_MAP()


// CDlgEffAmbient message handlers
BOOL CDlgEffAmbient::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	return TRUE;
}

void CDlgEffAmbient::SetData( DxSkinPiece* pData, DxEffCharAmbient* pEff, BOOL bNEW )
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

void CDlgEffAmbient::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_AMBIENT* pProperty = (EFFCHAR_PROPERTY_AMBIENT*)m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Num_float( this, IDC_EDIT_TIME, pProperty->m_fFullTime );
			SetWin_Num_float( this, IDC_EDIT_AMBIENT_A, pProperty->m_cColor.a );
			SetWin_Num_float( this, IDC_EDIT_AMBIENT_R, pProperty->m_cColor.r );
			SetWin_Num_float( this, IDC_EDIT_AMBIENT_G, pProperty->m_cColor.g );
			SetWin_Num_float( this, IDC_EDIT_AMBIENT_B, pProperty->m_cColor.b );

			SetWin_Check( this, IDC_CHECK_ALLTIME, pProperty->m_dwFlage&USEALL );
			SetWin_Check( this, IDC_CHECK_TEXCOLOR, pProperty->m_dwFlage&USETEXCOLOR );
		}
	}
}

BOOL CDlgEffAmbient::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_AMBIENT Property;
		Property.m_fFullTime = GetWin_Num_float( this, IDC_EDIT_TIME );
		Property.m_cColor.a = GetWin_Num_float( this, IDC_EDIT_AMBIENT_A );
		Property.m_cColor.r = GetWin_Num_float( this, IDC_EDIT_AMBIENT_R );
		Property.m_cColor.g = GetWin_Num_float( this, IDC_EDIT_AMBIENT_G );
		Property.m_cColor.b = GetWin_Num_float( this, IDC_EDIT_AMBIENT_B );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ALLTIME ), Property.m_dwFlage, USEALL );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_TEXCOLOR ), Property.m_dwFlage, USETEXCOLOR );

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

void CDlgEffAmbient::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffAmbient::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffAmbient::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffAmbient::OnBnClickedButtonColor()
{
	float fr = GetWin_Num_float( this, IDC_EDIT_AMBIENT_R );
	float fg = GetWin_Num_float( this, IDC_EDIT_AMBIENT_G );
	float fb = GetWin_Num_float( this, IDC_EDIT_AMBIENT_B );

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

		SetWin_Num_float( this, IDC_EDIT_AMBIENT_R, fR / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_AMBIENT_G, fG / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_AMBIENT_B, fB / 255.0f );
	}
}

