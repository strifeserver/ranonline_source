// DlgEffTexDiff.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffTexDiff.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgefftexdiff.h"

#include "../SelColorDlg.h"
// CDlgEffTexDiff dialog

IMPLEMENT_DYNAMIC(CDlgEffTexDiff, CPropertyPage)
CDlgEffTexDiff::CDlgEffTexDiff( LOGFONT logfont )
	: CPropertyPage(CDlgEffTexDiff::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
	, m_nMatEdit( -1 )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffTexDiff::~CDlgEffTexDiff()
{
}

void CDlgEffTexDiff::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_SPECULAR, m_list_Material );
}


BEGIN_MESSAGE_MAP(CDlgEffTexDiff, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_SPECULAR, OnBnClickedButtonSpecular)
	ON_BN_CLICKED(IDC_BUTTON_SPECULAR_OK, OnBnClickedButtonSpecularOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SPECULAR, OnNMDblclkListSpecular)
END_MESSAGE_MAP()


// CDlgEffTexDiff message handlers
BOOL CDlgEffTexDiff::OnInitDialog()
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

		const int nColumnCount = 4;	
		char* szColumnName1[nColumnCount] = { "Material", "Used", "Effect", "Power" };
		int nColumnWidthPercent[nColumnCount] = { 40,10,40,10 };

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

	std::string strInten[3] = 
	{
		"X1",
		"X2",
		"X4"
	};

	SetWin_Combo_Init( this, IDC_COMBO_INTEN , strInten, 3 );

	return TRUE;
}

void CDlgEffTexDiff::SetData( DxSkinPiece* pData, DxEffCharTexDiff* pEff, BOOL bNEW )
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

void CDlgEffTexDiff::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_TEXDIFF* pProperty = (EFFCHAR_PROPERTY_TEXDIFF*) m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Check( this, IDC_CHECK_UPDOWN, pProperty->m_dwFlag&NO_UPDOWN);
			SetWin_Num_float( this, IDC_EDIT_SPEED, pProperty->m_fSpeed );
			SetWin_Combo_Sel( this, IDC_COMBO_INTEN, pProperty->m_dwIntensity );
			ShowMaterialList();
		}
	}
}

void CDlgEffTexDiff::ShowMaterialList()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_TEXDIFF* pProperty = (EFFCHAR_PROPERTY_TEXDIFF*) m_pEff->GetProperty();
		DXMATERIAL_CHAR_EFF*	pMAT = m_pEff->GetMaterials();

		std::string str[2] = {"No","Yes"};

		if ( pProperty&&pMAT )
		{
			m_list_Material.DeleteAllItems();
			char szTempChar[512];

			for ( DWORD dw = 0; dw < pProperty->m_dwMaterials; ++dw )
			{
				m_list_Material.InsertItem( dw, pMAT[dw].szTexture );
				m_list_Material.SetItemText( dw, 1, str[pMAT[dw].bEffUse].c_str() );
				m_list_Material.SetItemText( dw, 2, pMAT[dw].szEffTex );

				sprintf( szTempChar, "%2.2f", pMAT[dw].d3dMaterial.Power );
				m_list_Material.SetItemText( dw, 3, szTempChar );
			}

			SetWin_Enable( this, IDC_CHECK_SPECULAR, FALSE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR, FALSE );
			SetWin_Enable( this, IDC_EDIT_SPECULAR, FALSE );
			SetWin_Enable( this, IDC_EDIT_SPECULARPOWER, FALSE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR_OK, FALSE );
			m_nMatEdit = -1;
		}
	}
}

BOOL CDlgEffTexDiff::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_TEXDIFF Property;
		Property.m_fSpeed = GetWin_Num_float( this, IDC_EDIT_SPEED );
		Property.m_dwIntensity = GetWin_Combo_Sel( this, IDC_COMBO_INTEN );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_UPDOWN ), Property.m_dwFlag, NO_UPDOWN );
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

void CDlgEffTexDiff::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffTexDiff::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffTexDiff::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffTexDiff::OnBnClickedButtonSpecular()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_SPECULAR, dlg.GetFileName().GetString() );
}

void CDlgEffTexDiff::OnBnClickedButtonSpecularOk()
{
	if( m_nMatEdit == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		DXMATERIAL_CHAR_EFF*	pMAT = m_pEff->GetMaterials();
		if ( pMAT )
		{
			pMAT[m_nMatEdit].bEffUse = GetWin_Check( this, IDC_CHECK_SPECULAR );
			pMAT[m_nMatEdit].d3dMaterial.Power = GetWin_Num_float( this, IDC_EDIT_SPECULARPOWER );
			std::string str = GetWin_Text( this, IDC_EDIT_SPECULAR );
			StringCchCopy( pMAT[m_nMatEdit].szEffTex, MAX_PATH, str.c_str() );
			ShowMaterialList();
		}
	}
}

void CDlgEffTexDiff::OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult)
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
			SetWin_Enable( this, IDC_BUTTON_SPECULAR, TRUE );
			SetWin_Enable( this, IDC_EDIT_SPECULAR, TRUE );
			SetWin_Enable( this, IDC_EDIT_SPECULARPOWER, TRUE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR_OK, TRUE );
			m_nMatEdit = nSelect;

			SetWin_Check( this, IDC_CHECK_SPECULAR, pMAT[nSelect].bEffUse );
			SetWin_Text( this, IDC_EDIT_SPECULAR, pMAT[nSelect].szEffTex );
			SetWin_Num_float( this, IDC_EDIT_SPECULARPOWER, pMAT[nSelect].d3dMaterial.Power );
		}
	}
}
