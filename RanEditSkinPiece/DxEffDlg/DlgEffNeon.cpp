// DlgEffNeon.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffNeon.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffneon.h"

#include "../SelColorDlg.h"
// CDlgEffNeon dialog

IMPLEMENT_DYNAMIC(CDlgEffNeon, CPropertyPage)
CDlgEffNeon::CDlgEffNeon( LOGFONT logfont )
	: CPropertyPage(CDlgEffNeon::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
	, m_nMatEdit( -1 )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffNeon::~CDlgEffNeon()
{
}

void CDlgEffNeon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_SPECULAR, m_list_Material );
}


BEGIN_MESSAGE_MAP(CDlgEffNeon, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_CHECK_GLOW_DEF, OnBnClickedCheckGlowDef)
	ON_BN_CLICKED(IDC_CHECK_GLOW_BURN, OnBnClickedCheckGlowBurn)
	ON_BN_CLICKED(IDC_CHECK_TEXTURE_ALL, OnBnClickedCheckTextureAll)
	ON_BN_CLICKED(IDC_CHECK_TEXTURE_SELECTION, OnBnClickedCheckTextureSelection)
	ON_BN_CLICKED(IDC_CHECK_COLOR_SOURCE, OnBnClickedCheckColorSource)
	ON_BN_CLICKED(IDC_CHECK_COLOR_SOURCE2, OnBnClickedCheckColorSource2)
	ON_BN_CLICKED(IDC_BUTTON_TEXT, OnBnClickedButtonText)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_SPECULAR_OK, OnBnClickedButtonSpecularOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SPECULAR, OnNMDblclkListSpecular)
END_MESSAGE_MAP()


// CDlgEffNeon message handlers
BOOL CDlgEffNeon::OnInitDialog()
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


	return TRUE;
}

void CDlgEffNeon::SetData( DxSkinPiece* pData, DxEffCharNeon* pEff, BOOL bNEW )
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

void CDlgEffNeon::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_NEON* pProperty = (EFFCHAR_PROPERTY_NEON*) m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Check( this, IDC_CHECK_GLOW_DEF, pProperty->m_dwFlag&USE_GENERAL );
			SetWin_Check( this, IDC_CHECK_GLOW_BURN, pProperty->m_dwFlag&USE_BURN );
			SetWin_Check( this, IDC_CHECK_TEXTURE_ALL, pProperty->m_dwFlag&USE_ALL_TEX );
			SetWin_Check( this, IDC_CHECK_TEXTURE_SELECTION, pProperty->m_dwFlag&USE_SELECT_TEX );
			SetWin_Check( this, IDC_CHECK_COLOR_SOURCE, pProperty->m_dwFlag&USE_GLOW_SRCTEX );
			SetWin_Check( this, IDC_CHECK_COLOR_SOURCE2, pProperty->m_dwFlag&USE_GLOW_USER );

			D3DCOLOR color1 = pProperty->m_cColor;
			BYTE a1,r1,g1,b1;
//			Color_DWORD_to_ARGB( color1, a1, r1, g1, b1 );
			
			SetWin_Num_int( this, IDC_EDIT_COLOR_A, a1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_R, r1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_G, g1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_B, b1 );
			SetWin_Text( this, IDC_EDIT_TEX, pProperty->m_szTexture );

			ShowMaterialList();
		}
	}
}

void CDlgEffNeon::ShowMaterialList()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_NEON* pProperty = (EFFCHAR_PROPERTY_NEON*) m_pEff->GetProperty();
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

BOOL CDlgEffNeon::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_NEON Property;

		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_GLOW_DEF ), Property.m_dwFlag, USE_GENERAL );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_GLOW_BURN ), Property.m_dwFlag, USE_BURN );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_TEXTURE_ALL ), Property.m_dwFlag, USE_ALL_TEX );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_TEXTURE_SELECTION ), Property.m_dwFlag, USE_SELECT_TEX );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_COLOR_SOURCE ), Property.m_dwFlag, USE_GLOW_SRCTEX );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_COLOR_SOURCE2 ), Property.m_dwFlag, USE_GLOW_USER );

		BYTE a1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_A );
		BYTE r1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_R );
		BYTE g1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_G );
		BYTE b1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_B );

//		Property.m_cColor = Color_ARGB_to_DWORD( a1, r1, g1, b1 );
		Property.m_dwMaterials = m_pEff->GetMaterialNum();

		std::string str = GetWin_Text( this, IDC_EDIT_TEX );
		StringCchCopy( Property.m_szTexture , MAX_PATH, str.c_str() );

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

void CDlgEffNeon::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffNeon::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffNeon::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffNeon::OnBnClickedCheckGlowDef()
{
	SetWin_Check( this, IDC_CHECK_GLOW_DEF, TRUE );
	SetWin_Check( this, IDC_CHECK_GLOW_BURN, FALSE );
}

void CDlgEffNeon::OnBnClickedCheckGlowBurn()
{
	SetWin_Check( this, IDC_CHECK_GLOW_DEF, FALSE );
	SetWin_Check( this, IDC_CHECK_GLOW_BURN, TRUE );
}

void CDlgEffNeon::OnBnClickedCheckTextureAll()
{
	SetWin_Check( this, IDC_CHECK_TEXTURE_ALL, TRUE );
	SetWin_Check( this, IDC_CHECK_TEXTURE_SELECTION, FALSE );
}

void CDlgEffNeon::OnBnClickedCheckTextureSelection()
{
	SetWin_Check( this, IDC_CHECK_TEXTURE_ALL, FALSE );
	SetWin_Check( this, IDC_CHECK_TEXTURE_SELECTION, TRUE );
}

void CDlgEffNeon::OnBnClickedCheckColorSource()
{
	SetWin_Check( this, IDC_CHECK_COLOR_SOURCE, TRUE );
	SetWin_Check( this, IDC_CHECK_COLOR_SOURCE2, FALSE );
}

void CDlgEffNeon::OnBnClickedCheckColorSource2()
{
	SetWin_Check( this, IDC_CHECK_COLOR_SOURCE, FALSE );
	SetWin_Check( this, IDC_CHECK_COLOR_SOURCE2, TRUE );
}

void CDlgEffNeon::OnBnClickedButtonText()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEX, dlg.GetFileName().GetString() );
}

void CDlgEffNeon::OnBnClickedButtonColor()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDIT_COLOR_R );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDIT_COLOR_G );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDIT_COLOR_B );

	COLORREF Color = RGB(nCUR_R, nCUR_G, nCUR_B );
	CSelColorDlg dlg(GetRValue(Color), GetGValue(Color),GetBValue(Color));

	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		BYTE nR = GetRValue( crNewColor );
		BYTE nG = GetGValue( crNewColor );
		BYTE nB = GetBValue( crNewColor );

		SetWin_Num_int( this, IDC_EDIT_COLOR_R, nR );
		SetWin_Num_int( this, IDC_EDIT_COLOR_G, nG );
		SetWin_Num_int( this, IDC_EDIT_COLOR_B, nB );
	}
}

void CDlgEffNeon::OnBnClickedButtonSpecularOk()
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

void CDlgEffNeon::OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult)
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
