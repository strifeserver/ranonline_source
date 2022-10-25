// DlgEffMultiTex.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffMultiTex.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffmultitex.h"

#include "../SelColorDlg.h"
// CDlgEffMultiTex dialog

IMPLEMENT_DYNAMIC(CDlgEffMultiTex, CPropertyPage)
CDlgEffMultiTex::CDlgEffMultiTex( LOGFONT logfont )
	: CPropertyPage(CDlgEffMultiTex::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
	, m_nMatEdit( -1 )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffMultiTex::~CDlgEffMultiTex()
{
}

void CDlgEffMultiTex::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_SPECULAR, m_list_Material );
}


BEGIN_MESSAGE_MAP(CDlgEffMultiTex, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_SPECULAR_OK, OnBnClickedButtonSpecularOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SPECULAR, OnNMDblclkListSpecular)
	ON_BN_CLICKED(IDC_CHECK_ALL_TEX, OnBnClickedCheckAllTex)
	ON_BN_CLICKED(IDC_CHECK_LIST_TEX, OnBnClickedCheckListTex)
	ON_BN_CLICKED(IDC_CHECK_USE_TEX1, OnBnClickedCheckUseTex1)
	ON_BN_CLICKED(IDC_CHECK_USE_TEX2, OnBnClickedCheckUseTex2)
	ON_BN_CLICKED(IDC_BUTTON_TEX_1, OnBnClickedButtonTex1)
	ON_BN_CLICKED(IDC_BUTTON_TEX_2, OnBnClickedButtonTex2)
	ON_BN_CLICKED(IDC_BUTTON_TEXMAP, OnBnClickedButtonTexmap)
	ON_BN_CLICKED(IDC_BUTTON_COLOR1, OnBnClickedButtonColor1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR2, OnBnClickedButtonColor2)
END_MESSAGE_MAP()


// CDlgEffMultiTex message handlers
BOOL CDlgEffMultiTex::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Material.SetExtendedStyle ( m_list_Material.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Material.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 2;	
		char* szColumnName1[nColumnCount] = { "Material", "Used"};
		int nColumnWidthPercent[nColumnCount] = { 80,20 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Material.InsertColumn (i, &lvColumn );
		}
	}

	std::string strBlend[3] = 
	{
		"Level 0",
		"Level 1",
		"Level 2",
	};

	SetWin_Combo_Init( this, IDC_COMBO_BLEND, strBlend, 3 );

	return TRUE;
}

void CDlgEffMultiTex::SetData( DxSkinPiece* pData, DxEffCharMultiTex* pEff, BOOL bNEW )
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
			if ( bNEWEFF )
			{
				//if effect is new then create new materials also
				//derive from mesh materials
				DWORD dwMAT = m_pPiece->m_pmcMesh->GetNumMaterials();
				DXMATERIAL_CHAR_EFF* pMaterials = new DXMATERIAL_CHAR_EFF[dwMAT];

				for ( DWORD i=0; i<dwMAT; i++ )
				{
					StringCchCopy( pMaterials[i].szTexture,	MAX_PATH, m_pPiece->m_pmcMesh->pMaterials[i].pTextureFilename );
				}

				m_pEff->SetMaterials( CRanEditSkinPieceView::GetView()->Get3DDevice(), dwMAT, pMaterials );
			}

			ShowData();
		}
	}
}

void CDlgEffMultiTex::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_MULTITEX* pProperty = (EFFCHAR_PROPERTY_MULTITEX*) m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Check( this, IDC_CHECK_USE_TEX1, ( pProperty->m_nTexNum == 1 ) );
			SetWin_Check( this, IDC_CHECK_USE_TEX2, ( pProperty->m_nTexNum == 2 ) );

			SetWin_Text( this, IDC_EDIT_TEX_1, pProperty->m_szTex1 );
			SetWin_Text( this, IDC_EDIT_TEX_2, pProperty->m_szTex2 );
			SetWin_Text( this, IDC_EDIT_TEXMAP, pProperty->m_szTexture );

			SetWin_Check( this, IDC_CHECK_ALL_TEX, pProperty->m_dwFlag&USE_ALL_TEX );
			SetWin_Check( this, IDC_CHECK_LIST_TEX, pProperty->m_dwFlag&USE_SELECT_TEX );
			SetWin_Check( this, IDC_CHECK_USE_TEXMAP, pProperty->m_dwFlag&USETEXMAP );

			SetWin_Combo_Sel( this, IDC_COMBO_BLEND, pProperty->m_nBlend );

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

			ShowMaterialList();
		}
	}
}

void CDlgEffMultiTex::ShowMaterialList()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_MULTITEX* pProperty = (EFFCHAR_PROPERTY_MULTITEX*) m_pEff->GetProperty();
		DXMATERIAL_CHAR_EFF*	pMAT = m_pEff->GetMaterials();

		std::string str[2] = {"No","Yes"};

		if ( pProperty&&pMAT )
		{
			m_list_Material.DeleteAllItems();
			for ( DWORD dw = 0; dw < pProperty->m_dwMaterials; ++dw )
			{
				m_list_Material.InsertItem( dw, pMAT[dw].szTexture );
				m_list_Material.SetItemText( dw, 1, str[pMAT[dw].bEffUse].c_str() );
			}

			SetWin_Enable( this, IDC_CHECK_SPECULAR, FALSE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR_OK, FALSE );
			m_nMatEdit = -1;
		}
	}
}

BOOL CDlgEffMultiTex::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_MULTITEX Property;

		BOOL bUse1 = GetWin_Check( this, IDC_CHECK_USE_TEX1 );
		BOOL bUse2 = GetWin_Check( this, IDC_CHECK_USE_TEX2 );

		Property.m_nTexNum = (bUse1)? 1: (bUse2)? 2:0;

		std::string strTex1 = GetWin_Text( this, IDC_EDIT_TEX_1 );
		std::string strTex2 = GetWin_Text( this, IDC_EDIT_TEX_2 );
		std::string strTexture = GetWin_Text( this, IDC_EDIT_TEXMAP );

		StringCchCopy( Property.m_szTex1, MAX_PATH, strTex1.c_str() );
		StringCchCopy( Property.m_szTex2, MAX_PATH, strTex2.c_str() );
		StringCchCopy( Property.m_szTexture, MAX_PATH, strTexture.c_str() );

		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ALL_TEX ), Property.m_dwFlag, USE_ALL_TEX);
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_LIST_TEX ), Property.m_dwFlag, USE_SELECT_TEX );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_TEXMAP ), Property.m_dwFlag, USETEXMAP );

		Property.m_nBlend = GetWin_Combo_Sel( this, IDC_COMBO_BLEND );
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

		Property.m_dwMaterials = m_pEff->GetMaterialNum();

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

void CDlgEffMultiTex::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffMultiTex::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffMultiTex::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffMultiTex::OnBnClickedButtonSpecularOk()
{
	if( m_nMatEdit == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		DXMATERIAL_CHAR_EFF*	pMAT = m_pEff->GetMaterials();
		if ( pMAT )
		{
			pMAT[m_nMatEdit].bEffUse = GetWin_Check( this, IDC_CHECK_SPECULAR );
			ShowMaterialList();
		}
	}
}

void CDlgEffMultiTex::OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_Material.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		DXMATERIAL_CHAR_EFF*	pMAT = m_pEff->GetMaterials();
		if ( pMAT )
		{
			SetWin_Enable( this, IDC_CHECK_SPECULAR, TRUE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR_OK, TRUE );
			m_nMatEdit = nSelect;

			SetWin_Check( this, IDC_CHECK_SPECULAR, pMAT[nSelect].bEffUse );
		}
	}
}

void CDlgEffMultiTex::OnBnClickedCheckAllTex()
{
	SetWin_Check( this, IDC_CHECK_ALL_TEX, TRUE );
	SetWin_Check( this, IDC_CHECK_LIST_TEX, FALSE );
}

void CDlgEffMultiTex::OnBnClickedCheckListTex()
{
	SetWin_Check( this, IDC_CHECK_ALL_TEX, FALSE );
	SetWin_Check( this, IDC_CHECK_LIST_TEX, TRUE );
}

void CDlgEffMultiTex::OnBnClickedCheckUseTex1()
{
	SetWin_Check( this, IDC_CHECK_USE_TEX1, TRUE );
	SetWin_Check( this, IDC_CHECK_USE_TEX2, FALSE );
}

void CDlgEffMultiTex::OnBnClickedCheckUseTex2()
{
	SetWin_Check( this, IDC_CHECK_USE_TEX1, FALSE );
	SetWin_Check( this, IDC_CHECK_USE_TEX2, TRUE );
}

void CDlgEffMultiTex::OnBnClickedButtonTex1()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEX_1, dlg.GetFileName().GetString() );
}

void CDlgEffMultiTex::OnBnClickedButtonTex2()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEX_2, dlg.GetFileName().GetString() );
}

void CDlgEffMultiTex::OnBnClickedButtonTexmap()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEXMAP, dlg.GetFileName().GetString() );
}

void CDlgEffMultiTex::OnBnClickedButtonColor1()
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

void CDlgEffMultiTex::OnBnClickedButtonColor2()
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
