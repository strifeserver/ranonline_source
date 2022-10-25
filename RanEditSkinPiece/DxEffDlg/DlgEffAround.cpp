// DlgEffAround.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffAround.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffaround.h"

#include "../SelColorDlg.h"
// CDlgEffAround dialog

IMPLEMENT_DYNAMIC(CDlgEffAround, CPropertyPage)
CDlgEffAround::CDlgEffAround( LOGFONT logfont )
	: CPropertyPage(CDlgEffAround::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffAround::~CDlgEffAround()
{
}

void CDlgEffAround::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_POS, m_list_POS );
}


BEGIN_MESSAGE_MAP(CDlgEffAround, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_TEX, OnBnClickedButtonTex)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_POS, OnNMDblclkListPos)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnBnClickedButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CDlgEffAround message handlers
BOOL CDlgEffAround::OnInitDialog()
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
		char* szColumnName1[nColumnCount] = { "Bone", "Position"};
		int nColumnWidthPercent[nColumnCount] = { 60,40 };

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

	return TRUE;
}

void CDlgEffAround::SetData( DxSkinPiece* pData, DxCharAroundEff* pEff, BOOL bNEW )
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
			CRanEditSkinPieceView::GetView()->bMeshDetect = TRUE;
			ShowData();
		}
	}
}

void CDlgEffAround::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_CHAR_AROUND* pProperty = (EFFCHAR_PROPERTY_CHAR_AROUND*)m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Num_float( this, IDC_EDIT_LIFE_X, pProperty->m_fMinLifeTime );
			SetWin_Num_float( this, IDC_EDIT_LIFE_Y, pProperty->m_fMaxLifeTime );

			SetWin_Num_float( this, IDC_EDIT_REPLAY_X, pProperty->m_fMinRivalTime );
			SetWin_Num_float( this, IDC_EDIT_REPLAY_Y, pProperty->m_fMaxRivalTime );

			SetWin_Num_float( this, IDC_EDIT_XSIZE_X, pProperty->m_vMinSize.x );
			SetWin_Num_float( this, IDC_EDIT_XSIZE_Y, pProperty->m_vMinSize.y );

			SetWin_Num_float( this, IDC_EDIT_YSIZE_X, pProperty->m_vMaxSize.x );
			SetWin_Num_float( this, IDC_EDIT_YSIZE_Y, pProperty->m_vMaxSize.y );

			SetWin_Num_float( this, IDC_EDIT_SPEED_MIN, pProperty->m_fMinTextureSpeed );
			SetWin_Num_float( this, IDC_EDIT_SPEED_MAX, pProperty->m_fMaxTextureSpeed );

			SetWin_Num_int( this, IDC_EDIT_SEQ_COL, pProperty->m_dwSequenceCol );
			SetWin_Num_int( this, IDC_EDIT_SEQ_ROW, pProperty->m_dwSequenceRow );

			D3DCOLOR color = pProperty->m_dwColor;
			BYTE a1,r1,g1,b1;
//			Color_DWORD_to_ARGB( color, a1, r1, g1, b1 );

			SetWin_Num_int( this, IDC_EDIT_COLORS_A, a1 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_R, r1 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_G, g1 );
			SetWin_Num_int( this, IDC_EDIT_COLORS_B, b1 );

			SetWin_Check( this, IDC_CHECK_SEQ, pProperty->m_dwFlag&USE_SEQUENCE );
			SetWin_Text( this, IDC_EDIT_TEXTURE, pProperty->m_szTexture );

			UpdateList();
		}
	}
}

void CDlgEffAround::UpdateList()
{
	if ( m_pPiece && m_pEff )
	{
		m_list_POS.DeleteAllItems();
		char szTempChar[512];

		vector<SAROUNDEFF_DATA> VEC;
		VEC.clear();
		VEC = m_pEff->GetAroundEffect();

		size_t nSize = VEC.size();

		for( size_t i=0; i<nSize; ++i )
		{
			SAROUNDEFF_DATA sData = VEC[i];

			m_list_POS.InsertItem( (int)i, sData.strBoneName.c_str() );
			sprintf( szTempChar, "X:%2.2f  Y:%2.2f  Z:%2.2f", sData.vPos.x, sData.vPos.y, sData.vPos.z );
			m_list_POS.SetItemText( (int)i, 1 , szTempChar );
		}

		SetWin_Text( this, IDC_EDIT_BONE, "" );
		SetWin_Num_float( this, IDC_EDIT_GET_X, 0.0f );
		SetWin_Num_float( this, IDC_EDIT_GET_Y, 0.0f );
		SetWin_Num_float( this, IDC_EDIT_GET_Z, 0.0f );
		SetWin_Num_float( this, IDC_EDIT_DIST, 1.0f );
	}
}

BOOL CDlgEffAround::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_CHAR_AROUND Property;

		Property.m_fMinLifeTime = GetWin_Num_float( this, IDC_EDIT_LIFE_X );
		Property.m_fMaxLifeTime = GetWin_Num_float( this, IDC_EDIT_LIFE_Y );
		Property.m_fMinRivalTime = GetWin_Num_float( this, IDC_EDIT_REPLAY_X );
		Property.m_fMaxRivalTime = GetWin_Num_float( this, IDC_EDIT_REPLAY_Y );
		Property.m_vMinSize.x = GetWin_Num_float( this, IDC_EDIT_XSIZE_X );
		Property.m_vMinSize.y = GetWin_Num_float( this, IDC_EDIT_XSIZE_Y );
		Property.m_vMaxSize.x = GetWin_Num_float( this, IDC_EDIT_YSIZE_X );
		Property.m_vMaxSize.y = GetWin_Num_float( this, IDC_EDIT_YSIZE_Y );
		Property.m_fMinTextureSpeed = GetWin_Num_float( this, IDC_EDIT_SPEED_MIN );
		Property.m_fMaxTextureSpeed = GetWin_Num_float( this, IDC_EDIT_SPEED_MAX );
		Property.m_dwSequenceCol = GetWin_Num_int( this, IDC_EDIT_SEQ_COL );
		Property.m_dwSequenceRow = GetWin_Num_int( this, IDC_EDIT_SEQ_ROW );

		BYTE a1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_A );
		BYTE r1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_R );
		BYTE g1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_G );
		BYTE b1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLORS_B );
//		Property.m_dwColor = Color_ARGB_to_DWORD( a1, r1, g1, b1 );
		
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SEQ ), Property.m_dwFlag, USE_SEQUENCE );
		
		std::string str = GetWin_Text( this, IDC_EDIT_TEXTURE );
		StringCchCopy( Property.m_szTexture, AROUNDEFFECT_TEXNAME_SIZE, str.c_str() );

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

void CDlgEffAround::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffAround::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffAround::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffAround::OnBnClickedButtonColor()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDIT_COLORS_R );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDIT_COLORS_G );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDIT_COLORS_B );

	COLORREF Color = RGB(nCUR_R, nCUR_G, nCUR_B );
	CSelColorDlg dlg(GetRValue(Color), GetGValue(Color),GetBValue(Color));

	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		BYTE nR = GetRValue( crNewColor );
		BYTE nG = GetGValue( crNewColor );
		BYTE nB = GetBValue( crNewColor );

		SetWin_Num_int( this, IDC_EDIT_COLORS_R, nR );
		SetWin_Num_int( this, IDC_EDIT_COLORS_G, nG );
		SetWin_Num_int( this, IDC_EDIT_COLORS_B, nB );
	}
}

void CDlgEffAround::OnBnClickedButtonTex()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEXTURE, dlg.GetFileName().GetString() );
}

void CDlgEffAround::OnNMDblclkListPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlgEffAround::OnBnClickedButtonGet()
{
	if ( m_pPiece && m_pEff )
	{
		std::string str = CRanEditSkinPieceView::GetView()->strBoneName;
		if ( str.size() == 0 )	
		{
			CDebugSet::MsgBox( "Invalid Selected Bone" );
			return;
		}

		D3DXVECTOR3 vPOS = CRanEditSkinPieceView::GetView()->m_vCOL;
		DWORD dwCOLID = CRanEditSkinPieceView::GetView()->m_dwCOL;

		D3DXVECTOR3 vVALID = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		if ( vPOS == vVALID || dwCOLID == COL_ERR )
		{
			CDebugSet::MsgBox( "Invalid Selected Mesh Collision" );
			return;
		}

		SetWin_Text( this, IDC_EDIT_BONE, str.c_str() );
		SetWin_Num_float( this, IDC_EDIT_GET_X, vPOS.x );
		SetWin_Num_float( this, IDC_EDIT_GET_Y, vPOS.y );
		SetWin_Num_float( this, IDC_EDIT_GET_Z, vPOS.z );
		SetWin_Num_float( this, IDC_EDIT_DIST, 1.0f );
	}
}

void CDlgEffAround::OnBnClickedButtonInsert()
{
	if ( m_pPiece && m_pEff && m_pPiece->m_pSkeleton )
	{
		SAROUNDEFF_DATA sDATA;
		sDATA.strBoneName = GetWin_Text( this, IDC_EDIT_BONE ).GetString();
		sDATA.vPos.x = GetWin_Num_float( this, IDC_EDIT_GET_X );
		sDATA.vPos.y = GetWin_Num_float( this, IDC_EDIT_GET_Y );
		sDATA.vPos.z = GetWin_Num_float( this, IDC_EDIT_GET_Z );
		sDATA.fMeshDist = GetWin_Num_float( this, IDC_EDIT_DIST );
		sDATA.vNormal = ( sDATA.vPos * sDATA.fMeshDist );
		
		m_pEff->InsertAroundEffect( sDATA, TRUE );
		UpdateList();
	}
	else
	{
		CDebugSet::MsgBox( "Initialize effect first" );
	}
}

void CDlgEffAround::OnBnClickedButtonDel()
{
	int nSelect = m_list_POS.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		m_pEff->DeleteAroundEffect( nSelect );
		UpdateList();
	}
}
