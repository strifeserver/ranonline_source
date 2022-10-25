// DlgEffMark.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffMark.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffmark.h"

#include "../SelColorDlg.h"
// CDlgEffMark dialog

IMPLEMENT_DYNAMIC(CDlgEffMark, CPropertyPage)
CDlgEffMark::CDlgEffMark( LOGFONT logfont )
	: CPropertyPage(CDlgEffMark::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffMark::~CDlgEffMark()
{
}

void CDlgEffMark::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffMark, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_TEX1, OnBnClickedButtonTex1)
	ON_BN_CLICKED(IDC_BUTTON_TEX2, OnBnClickedButtonTex2)
	ON_BN_CLICKED(IDC_BUTTON_COLOR1, OnBnClickedButtonColor1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR2, OnBnClickedButtonColor2)
	ON_BN_CLICKED(IDC_CHECK_USE1, OnBnClickedCheckUse1)
	ON_BN_CLICKED(IDC_CHECK_USE2, OnBnClickedCheckUse2)
END_MESSAGE_MAP()


// CDlgEffMark message handlers
BOOL CDlgEffMark::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	return TRUE;
}

void CDlgEffMark::SetData( DxSkinPiece* pData, DxEffCharMark* pEff, BOOL bNEW )
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

void CDlgEffMark::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_MARK* pProperty = (EFFCHAR_PROPERTY_MARK*)m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Check( this, IDC_CHECK_USE1, ( pProperty->m_nTexNum == 1 ) );
			SetWin_Check( this, IDC_CHECK_USE2, ( pProperty->m_nTexNum == 2 ) );

			SetWin_Text( this, IDC_EDIT_TEX1, pProperty->m_szTex1 );
			SetWin_Text( this, IDC_EDIT_TEX2, pProperty->m_szTex2 );

			SetWin_Num_float( this, IDC_EDIT_COLORS_A_1, pProperty->m_cDiffuse1.a );
			SetWin_Num_float( this, IDC_EDIT_COLORS_R_1, pProperty->m_cDiffuse1.r );
			SetWin_Num_float( this, IDC_EDIT_COLORS_G_1, pProperty->m_cDiffuse1.g );
			SetWin_Num_float( this, IDC_EDIT_COLORS_B_1, pProperty->m_cDiffuse1.b );
			SetWin_Num_float( this, IDC_EDIT_COLORS_A_2, pProperty->m_cDiffuse2.a );
			SetWin_Num_float( this, IDC_EDIT_COLORS_R_2, pProperty->m_cDiffuse2.r );
			SetWin_Num_float( this, IDC_EDIT_COLORS_G_2, pProperty->m_cDiffuse2.g );
			SetWin_Num_float( this, IDC_EDIT_COLORS_B_2, pProperty->m_cDiffuse2.b );
			SetWin_Num_float( this, IDC_EDIT_VTEX_X_1, pProperty->m_vTex01.x );
			SetWin_Num_float( this, IDC_EDIT_VTEX_Y_1, pProperty->m_vTex01.y );
			SetWin_Num_float( this, IDC_EDIT_VTEX_X_2, pProperty->m_vTex02.x );
			SetWin_Num_float( this, IDC_EDIT_VTEX_Y_2, pProperty->m_vTex02.y );
			SetWin_Num_float( this, IDC_EDIT_VTEXUV_X_1, pProperty->m_vTexUV01.x );
			SetWin_Num_float( this, IDC_EDIT_VTEXUV_Y_1, pProperty->m_vTexUV01.y );
			SetWin_Num_float( this, IDC_EDIT_VTEXUV_X_2, pProperty->m_vTexUV02.x );
			SetWin_Num_float( this, IDC_EDIT_VTEXUV_Y_2, pProperty->m_vTexUV02.y );
		}
	}
}

BOOL CDlgEffMark::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_MARK Property;

		BOOL bUse1 = GetWin_Check( this, IDC_CHECK_USE1 );
		BOOL bUse2 = GetWin_Check( this, IDC_CHECK_USE2 );

		Property.m_nTexNum = (bUse1)? 1: (bUse2)? 2:0;

		std::string str1 = GetWin_Text( this, IDC_EDIT_TEX1 );
		std::string str2 = GetWin_Text( this, IDC_EDIT_TEX2 );

		StringCchCopy( Property.m_szTex1, MAX_PATH, str1.c_str() );
		StringCchCopy( Property.m_szTex2, MAX_PATH, str2.c_str() );

		Property.m_cDiffuse1.a = GetWin_Num_float( this, IDC_EDIT_COLORS_A_1 );
		Property.m_cDiffuse1.r = GetWin_Num_float( this, IDC_EDIT_COLORS_R_1 );
		Property.m_cDiffuse1.g = GetWin_Num_float( this, IDC_EDIT_COLORS_G_1 );
		Property.m_cDiffuse1.b = GetWin_Num_float( this, IDC_EDIT_COLORS_B_1 );
		Property.m_cDiffuse2.a = GetWin_Num_float( this, IDC_EDIT_COLORS_A_2 );
		Property.m_cDiffuse2.r = GetWin_Num_float( this, IDC_EDIT_COLORS_R_2 );
		Property.m_cDiffuse2.g = GetWin_Num_float( this, IDC_EDIT_COLORS_G_2 );
		Property.m_cDiffuse2.b = GetWin_Num_float( this, IDC_EDIT_COLORS_B_2 );
		Property.m_vTex01.x = GetWin_Num_float( this, IDC_EDIT_VTEX_X_1 );
		Property.m_vTex01.y = GetWin_Num_float( this, IDC_EDIT_VTEX_Y_1 );
		Property.m_vTex02.x = GetWin_Num_float( this, IDC_EDIT_VTEX_X_2 );
		Property.m_vTex02.y = GetWin_Num_float( this, IDC_EDIT_VTEX_Y_2 );
		Property.m_vTexUV01.x = GetWin_Num_float( this, IDC_EDIT_VTEXUV_X_1 );
		Property.m_vTexUV01.y = GetWin_Num_float( this, IDC_EDIT_VTEXUV_Y_1 );
		Property.m_vTexUV02.x = GetWin_Num_float( this, IDC_EDIT_VTEXUV_X_2 );
		Property.m_vTexUV02.y = GetWin_Num_float( this, IDC_EDIT_VTEXUV_Y_2 );

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

void CDlgEffMark::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffMark::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffMark::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffMark::OnBnClickedButtonTex1()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEX1, dlg.GetFileName().GetString() );
}

void CDlgEffMark::OnBnClickedButtonTex2()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEX2, dlg.GetFileName().GetString() );
}

void CDlgEffMark::OnBnClickedButtonColor1()
{
	float fr = GetWin_Num_float( this, IDC_EDIT_COLORS_R_1 );
	float fg = GetWin_Num_float( this, IDC_EDIT_COLORS_G_1 );
	float fb = GetWin_Num_float( this, IDC_EDIT_COLORS_B_1 );

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

		SetWin_Num_float( this, IDC_EDIT_COLORS_R_1, fR / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_COLORS_G_1, fG / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_COLORS_B_1, fB / 255.0f );
	}
}

void CDlgEffMark::OnBnClickedButtonColor2()
{
	float fr = GetWin_Num_float( this, IDC_EDIT_COLORS_R_2 );
	float fg = GetWin_Num_float( this, IDC_EDIT_COLORS_G_2 );
	float fb = GetWin_Num_float( this, IDC_EDIT_COLORS_B_2 );

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

		SetWin_Num_float( this, IDC_EDIT_COLORS_R_2, fR / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_COLORS_G_2, fG / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_COLORS_B_2, fB / 255.0f );
	}
}

void CDlgEffMark::OnBnClickedCheckUse1()
{
	SetWin_Check( this, IDC_CHECK_USE1, TRUE );
	SetWin_Check( this, IDC_CHECK_USE2, FALSE );

}

void CDlgEffMark::OnBnClickedCheckUse2()
{
	SetWin_Check( this, IDC_CHECK_USE1, FALSE );
	SetWin_Check( this, IDC_CHECK_USE2, TRUE );
}
