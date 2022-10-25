// DlgEffLine2Bone.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffLine2Bone.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffline2bone.h"

#include "../SelColorDlg.h"

// CDlgEffLine2Bone dialog

IMPLEMENT_DYNAMIC(CDlgEffLine2Bone, CPropertyPage)
CDlgEffLine2Bone::CDlgEffLine2Bone( LOGFONT logfont )
	: CPropertyPage(CDlgEffLine2Bone::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffLine2Bone::~CDlgEffLine2Bone()
{
}

void CDlgEffLine2Bone::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_POS, m_list_POS );
}


BEGIN_MESSAGE_MAP(CDlgEffLine2Bone, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_1, OnBnClickedButtonColor1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR_2, OnBnClickedButtonColor2)
	ON_BN_CLICKED(IDC_BUTTON_TEX_1, OnBnClickedButtonTex1)
	ON_BN_CLICKED(IDC_BUTTON_TEX_2, OnBnClickedButtonTex2)
	ON_BN_CLICKED(IDC_BUTTON_BONE_1, OnBnClickedButtonBone1)
	ON_BN_CLICKED(IDC_BUTTON_BONE_2, OnBnClickedButtonBone2)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_POS, OnNMDblclkListPos)
END_MESSAGE_MAP()


// CDlgEffLine2Bone message handlers
BOOL CDlgEffLine2Bone::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_POS.SetExtendedStyle ( m_list_POS.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_POS.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 2;	
		char* szColumnName1[nColumnCount] = { "Bone1", "Bone2" };
		int nColumnWidthPercent[nColumnCount] = { 50, 50 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_POS.InsertColumn (i, &lvColumn );
		}
	}

	std::string str[4] = 
	{
		"None","Start->End","Blur","Each"
	};

	SetWin_Combo_Init( this, IDC_COMBO_OTHER, str, 4 );

	return TRUE;
}

void CDlgEffLine2Bone::SetData( DxSkinPiece* pData, DxEffCharLine2BoneEff* pEff, BOOL bNEW )
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
			CRanEditSkinPieceView::GetView()->bRenderSkeleton = TRUE;
			CRanEditSkinPieceView::GetView()->bMeshDetect = FALSE;
			ShowData();
		}
	}
}

void CDlgEffLine2Bone::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_LINE2BONEEFF* pProperty = (EFFCHAR_PROPERTY_LINE2BONEEFF*) m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Num_int( this, IDC_EDIT_PEC, pProperty->m_dwParticlePec );
			SetWin_Num_float( this, IDC_EDIT_SHAKE, pProperty->m_fShake );
			SetWin_Num_float( this, IDC_EDIT_LENGTH, pProperty->m_fCreateLength );

			SetWin_Num_float( this, IDC_EDIT_LIFE_X, pProperty->m_vLife.x );
			SetWin_Num_float( this, IDC_EDIT_LIFE_Y, pProperty->m_vLife.y );

			SetWin_Num_float( this, IDC_EDIT_SIZE_X, pProperty->m_vSize.x );
			SetWin_Num_float( this, IDC_EDIT_SIZE_Y, pProperty->m_vSize.y );
			SetWin_Num_float( this, IDC_EDIT_SIZE_Z, pProperty->m_vSize.z );

			SetWin_Num_float( this, IDC_EDIT_X_SCALE_X, pProperty->m_vScaleX.x );
			SetWin_Num_float( this, IDC_EDIT_X_SCALE_Y, pProperty->m_vScaleX.y );

			SetWin_Num_float( this, IDC_EDIT_Y_SCALE_X, pProperty->m_vScaleY.x );
			SetWin_Num_float( this, IDC_EDIT_Y_SCALE_Y, pProperty->m_vScaleY.y );
			SetWin_Num_float( this, IDC_EDIT_Y_SCALE_Z, pProperty->m_vScaleY.z );

			SetWin_Num_int( this, IDC_EDIT_SEC_COL, pProperty->m_dwSequenceCol );
			SetWin_Num_int( this, IDC_EDIT_SEC_ROW, pProperty->m_dwSequenceRow );

			D3DCOLOR color1 = pProperty->m_dwColor;
			D3DCOLOR color2 = pProperty->m_dwColor2;
			BYTE a1,r1,g1,b1;
			BYTE a2,r2,g2,b2;
//			Color_DWORD_to_ARGB( color1, a1, r1, g1, b1 );
//			Color_DWORD_to_ARGB( color2, a2, r2, g2, b2 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_A_1, a1 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_R_1, r1 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_G_1, g1 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_B_1, b1 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_A_2, a2 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_R_2, r2 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_G_2, g2 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_B_2, b2 );

			SetWin_Check( this, IDC_CHECK_CAM_MOVE, pProperty->m_bWithCamMove );
			SetWin_Combo_Sel( this, IDC_COMBO_OTHER, pProperty->m_dwOtherColor );

			SetWin_Check( this, IDC_CHECK_SEQ, pProperty->m_dwFlag&USE_SEQUENCE );

			SetWin_Text( this, IDC_EDIT_TEX_1, pProperty->m_szTexture );
			SetWin_Text( this, IDC_EDIT_TEX_2, pProperty->m_szTexture2 );

			UpdateBoneList();
		}
	}
}

void CDlgEffLine2Bone::UpdateBoneList()
{
	if ( m_pPiece && m_pEff )
	{
		DWORD dwNUM = m_pEff->GetLineSize();
		m_list_POS.DeleteAllItems();

		for( DWORD dw=0; dw< dwNUM; ++ dw )
		{
			std::string str1;
			std::string str2;
			
			m_pEff->GetBoneName( dw, str1, str2 );
			
			m_list_POS.InsertItem( dw, str1.c_str() );
			m_list_POS.SetItemText( dw, 1 , str2.c_str() );
		}

		SetWin_Text( this, IDC_EDIT_BONE_1, "" );
		SetWin_Text( this, IDC_EDIT_BONE_2, "" );
	}
}

BOOL CDlgEffLine2Bone::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_LINE2BONEEFF Property;

		Property.m_dwParticlePec = GetWin_Num_int( this, IDC_EDIT_PEC );
		Property.m_fShake = GetWin_Num_float( this, IDC_EDIT_SHAKE );
		Property.m_fCreateLength = GetWin_Num_float( this, IDC_EDIT_LENGTH );

		Property.m_vLife.x = GetWin_Num_float( this, IDC_EDIT_LIFE_X );
		Property.m_vLife.y = GetWin_Num_float( this, IDC_EDIT_LIFE_Y );

		Property.m_vSize.x = GetWin_Num_float( this, IDC_EDIT_SIZE_X );
		Property.m_vSize.y = GetWin_Num_float( this, IDC_EDIT_SIZE_Y );;
		Property.m_vSize.z = GetWin_Num_float( this, IDC_EDIT_SIZE_Z );

		Property.m_vScaleX.x = GetWin_Num_float( this, IDC_EDIT_X_SCALE_X );
		Property.m_vScaleX.y = GetWin_Num_float( this, IDC_EDIT_X_SCALE_Y );

		Property.m_vScaleY.x = GetWin_Num_float( this, IDC_EDIT_Y_SCALE_X );
		Property.m_vScaleY.y = GetWin_Num_float( this, IDC_EDIT_Y_SCALE_Y );
		Property.m_vScaleY.z = GetWin_Num_float( this, IDC_EDIT_Y_SCALE_Z );

		Property.m_dwSequenceCol = GetWin_Num_int( this, IDC_EDIT_SEC_COL );
		Property.m_dwSequenceRow = GetWin_Num_int( this, IDC_EDIT_SEC_ROW );

		BYTE a1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_A_1 );
		BYTE r1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_R_1 );
		BYTE g1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_G_1 );
		BYTE b1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_B_1 );

		BYTE a2 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_A_2 );
		BYTE r2 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_R_2 );
		BYTE g2 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_G_2 );
		BYTE b2 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_B_2 );

//		Property.m_dwColor = Color_ARGB_to_DWORD( a1, r1, g1, b1 );
//		Property.m_dwColor2 = Color_ARGB_to_DWORD( a2, r2, g2, b2 );

		Property.m_bWithCamMove = GetWin_Check( this, IDC_CHECK_CAM_MOVE );
		Property.m_dwOtherColor = GetWin_Combo_Sel( this, IDC_COMBO_OTHER );
		
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SEQ ), Property.m_dwFlag, USE_SEQUENCE );

		std::string str1 = GetWin_Text( this, IDC_EDIT_TEX_1 );
		std::string str2 = GetWin_Text( this, IDC_EDIT_TEX_2 );

		StringCchCopy( Property.m_szTexture ,BONELISTEFF_TEXNAME_SIZE, str1.c_str() );
		StringCchCopy( Property.m_szTexture2 , BONELISTEFF_TEXNAME_SIZE, str2.c_str() );

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

void CDlgEffLine2Bone::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffLine2Bone::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffLine2Bone::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffLine2Bone::OnBnClickedButtonColor1()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDIT_COLORS_R_1 );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDIT_COLORS_G_1 );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDIT_COLORS_B_1 );

	COLORREF Color = RGB(nCUR_R, nCUR_G, nCUR_B );
	CSelColorDlg dlg(GetRValue(Color), GetGValue(Color),GetBValue(Color));

	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		BYTE nR = GetRValue( crNewColor );
		BYTE nG = GetGValue( crNewColor );
		BYTE nB = GetBValue( crNewColor );

		SetWin_Num_int( this, IDC_EDIT_COLORS_R_1, nR );
		SetWin_Num_int( this, IDC_EDIT_COLORS_G_1, nG );
		SetWin_Num_int( this, IDC_EDIT_COLORS_B_1, nB );
	}
}

void CDlgEffLine2Bone::OnBnClickedButtonColor2()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDIT_COLORS_R_2 );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDIT_COLORS_G_2 );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDIT_COLORS_B_2 );

	COLORREF Color = RGB(nCUR_R, nCUR_G, nCUR_B );
	CSelColorDlg dlg(GetRValue(Color), GetGValue(Color),GetBValue(Color));

	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		BYTE nR = GetRValue( crNewColor );
		BYTE nG = GetGValue( crNewColor );
		BYTE nB = GetBValue( crNewColor );

		SetWin_Num_int( this, IDC_EDIT_COLORS_R_2, nR );
		SetWin_Num_int( this, IDC_EDIT_COLORS_G_2, nG );
		SetWin_Num_int( this, IDC_EDIT_COLORS_B_2, nB );
	}
}

void CDlgEffLine2Bone::OnBnClickedButtonTex1()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEX_1, dlg.GetFileName().GetString() );
}

void CDlgEffLine2Bone::OnBnClickedButtonTex2()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEX_2, dlg.GetFileName().GetString() );
}

void CDlgEffLine2Bone::OnBnClickedButtonBone1()
{
	if ( m_pPiece && m_pEff )
	{
		std::string str = CRanEditSkinPieceView::GetView()->strBoneName;
		if ( str.size() != 0 )
		{
			SetWin_Text( this, IDC_EDIT_BONE_1, str.c_str() );
		}
	}
}

void CDlgEffLine2Bone::OnBnClickedButtonBone2()
{
	if ( m_pPiece && m_pEff )
	{
		std::string str = CRanEditSkinPieceView::GetView()->strBoneName;
		if ( str.size() != 0 )
		{
			SetWin_Text( this, IDC_EDIT_BONE_2, str.c_str() );
		}
	}
}

void CDlgEffLine2Bone::OnBnClickedButtonInsert()
{
	if ( m_pPiece && m_pEff )
	{
		std::string str1 = GetWin_Text( this, IDC_EDIT_BONE_1 );
		std::string str2 = GetWin_Text( this, IDC_EDIT_BONE_2 );

		m_pEff->InsertLine( str1.c_str(), str2.c_str() );
		UpdateBoneList();
	}
}

void CDlgEffLine2Bone::OnBnClickedButtonDelete()
{
	int nSelect = m_list_POS.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		m_pEff->DeleteLine( nSelect );
		UpdateBoneList();
	}
}

void CDlgEffLine2Bone::OnNMDblclkListPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_POS.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		m_pEff->SelectLine( nSelect );
	}
}
