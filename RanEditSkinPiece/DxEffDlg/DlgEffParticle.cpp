// DlgEffParticle.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffParticle.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffparticle.h"

#include "../SelColorDlg.h"
// CDlgEffParticle dialog

IMPLEMENT_DYNAMIC(CDlgEffParticle, CPropertyPage)
CDlgEffParticle::CDlgEffParticle( LOGFONT logfont )
	: CPropertyPage(CDlgEffParticle::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffParticle::~CDlgEffParticle()
{
}

void CDlgEffParticle::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_POS, m_list_POS );
}


BEGIN_MESSAGE_MAP(CDlgEffParticle, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE, OnBnClickedButtonTexture)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_POS_ADD, OnBnClickedButtonPosAdd)
	ON_BN_CLICKED(IDC_BUTTON_POS_DEL, OnBnClickedButtonPosDel)
	ON_BN_CLICKED(IDC_BUTTON_POS_UP, OnBnClickedButtonPosUp)
	ON_BN_CLICKED(IDC_BUTTON_POS_DOWN, OnBnClickedButtonPosDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_POS, OnNMDblclkListPos)
END_MESSAGE_MAP()


// CDlgEffParticle message handlers
BOOL CDlgEffParticle::OnInitDialog()
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

		const int nColumnCount = 1;	
		char* szColumnName1[nColumnCount] = { "Position"};
		int nColumnWidthPercent[nColumnCount] = { 100 };

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

void CDlgEffParticle::SetData( DxSkinPiece* pData, DxEffCharParticle* pEff, BOOL bNEW )
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

void CDlgEffParticle::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_PARTICLE* pProperty = (EFFCHAR_PROPERTY_PARTICLE*)m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Check( this, IDC_CHECK_RANDOMRANGE, pProperty->m_dwFlag&USE_RANDOMRANGE);
			SetWin_Check( this, IDC_CHECK_ROTATE, pProperty->m_dwFlag&USE_ROTATE);
			SetWin_Check( this, IDC_CHECK_SEQ, pProperty->m_dwFlag&USE_SEQUENCE);
			SetWin_Check( this, IDC_CHECK_REMAIN, pProperty->m_dwFlag&USE_REMAIN);
			SetWin_Num_int( this, IDC_EDIT_PEC, pProperty->m_dwParticlePec );
			SetWin_Num_float( this, IDC_EDIT_RANDOMRANGE, pProperty->m_fRandomRange );
			SetWin_Num_float( this, IDC_EDIT_LIFEMIN, pProperty->m_fLifeMin );
			SetWin_Num_float( this, IDC_EDIT_LIFEMAX, pProperty->m_fLifeMax );

			SetWin_Num_float( this, IDC_EDIT_SCALE_X, pProperty->m_vSize.x );
			SetWin_Num_float( this, IDC_EDIT_SCALE_Y, pProperty->m_vSize.y );
			SetWin_Num_float( this, IDC_EDIT_SCALE_Z, pProperty->m_vSize.z );

			SetWin_Num_float( this, IDC_EDIT_ROTATE_MIN, pProperty->m_vRotate.x );
			SetWin_Num_float( this, IDC_EDIT_ROTATE_MAX, pProperty->m_vRotate.y );

			SetWin_Num_int( this, IDC_EDIT_SEQ_COL, pProperty->m_dwSequenceCol );
			SetWin_Num_int( this, IDC_EDIT_SEQ_ROW, pProperty->m_dwSequenceRow );

			SetWin_Num_float( this, IDC_EDIT_VEL_MIN, pProperty->m_vVelocity.x );
			SetWin_Num_float( this, IDC_EDIT_VEL_MAX, pProperty->m_vVelocity.y );

			D3DCOLOR color = pProperty->m_dwColor;
			BYTE a1,r1,g1,b1;
//			Color_DWORD_to_ARGB( color, a1, r1, g1, b1 );
			
			SetWin_Num_int( this, IDC_EDIT_COLOR_A, a1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_R, r1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_G, g1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_B, b1 );

			SetWin_Text( this, IDC_EDIT_TEXTURE, pProperty->m_szTexture );

			UpdatePositionList();
		}
	}
}

void CDlgEffParticle::UpdatePositionList()
{
	if ( m_pPiece && m_pEff )
	{
		m_list_POS.DeleteAllItems();

		DWORD dwSIZE = m_pEff->GetPoistionSize();
		for( DWORD dw=0; dw<dwSIZE; ++ dw )
		{
			m_list_POS.InsertItem( dw, m_pEff->GetPositionString( dw ).c_str() );
		}

		m_pEff->EditApplyResetData();
	}
}

BOOL CDlgEffParticle::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_PARTICLE Property;

		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_RANDOMRANGE ), Property.m_dwFlag, USE_RANDOMRANGE );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ROTATE ), Property.m_dwFlag, USE_ROTATE );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SEQ ), Property.m_dwFlag, USE_SEQUENCE );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_REMAIN ), Property.m_dwFlag, USE_REMAIN );

		Property.m_dwParticlePec = GetWin_Num_int( this, IDC_EDIT_PEC );
		Property.m_fRandomRange = GetWin_Num_float( this, IDC_EDIT_RANDOMRANGE );
		Property.m_fLifeMin = GetWin_Num_float( this, IDC_EDIT_LIFEMIN );
		Property.m_fLifeMax = GetWin_Num_float( this, IDC_EDIT_LIFEMAX );
		Property.m_vSize.x = GetWin_Num_float( this, IDC_EDIT_SCALE_X );
		Property.m_vSize.y = GetWin_Num_float( this, IDC_EDIT_SCALE_Y );
		Property.m_vSize.z = GetWin_Num_float( this, IDC_EDIT_SCALE_Z );
		Property.m_vRotate.x = GetWin_Num_float( this, IDC_EDIT_ROTATE_MIN );
		Property.m_vRotate.y = GetWin_Num_float( this, IDC_EDIT_ROTATE_MAX );
		Property.m_dwSequenceCol = GetWin_Num_int( this, IDC_EDIT_SEQ_COL );
		Property.m_dwSequenceRow = GetWin_Num_int( this, IDC_EDIT_SEQ_ROW );
		Property.m_vVelocity.x = GetWin_Num_float( this, IDC_EDIT_VEL_MIN );
		Property.m_vVelocity.y = GetWin_Num_float( this, IDC_EDIT_VEL_MAX );

		BYTE a1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_A );
		BYTE r1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_R );
		BYTE g1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_G );
		BYTE b1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_B );

//		Property.m_dwColor = Color_ARGB_to_DWORD( a1, r1, g1, b1 );

		std::string strTEX = GetWin_Text( this, IDC_EDIT_TEXTURE );
			
		StringCchCopy( Property.m_szTexture , TEX_SIZE_64, strTEX.c_str() );

		m_pEff->SetProperty( &Property );
		m_pEff->EditApplyResetData();

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

void CDlgEffParticle::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffParticle::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffParticle::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffParticle::OnBnClickedButtonTexture()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEXTURE, dlg.GetFileName().GetString() );
}

void CDlgEffParticle::OnBnClickedButtonColor()
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

void CDlgEffParticle::OnBnClickedButtonPosAdd()
{
	if ( m_pPiece && m_pEff )
	{
		D3DXVECTOR3 vPOS = CRanEditSkinPieceView::GetView()->m_vCOL;
		DWORD dwCOLID = CRanEditSkinPieceView::GetView()->m_dwCOL;

		D3DXVECTOR3 vVALID = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		if ( vPOS != vVALID && dwCOLID != COL_ERR )
		{
			m_pEff->InsertPoint( vPOS );
			UpdatePositionList();
		}
	}
}

void CDlgEffParticle::OnBnClickedButtonPosDel()
{
	int nSelect = m_list_POS.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		m_pEff->DeletePoint( nSelect );
		UpdatePositionList();
	}
}

void CDlgEffParticle::OnBnClickedButtonPosUp()
{
	int nSelect = m_list_POS.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		m_pEff->DataUp( nSelect );
		UpdatePositionList();
	}
}

void CDlgEffParticle::OnBnClickedButtonPosDown()
{
	int nSelect = m_list_POS.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		m_pEff->DataDown( nSelect );
		UpdatePositionList();
	}
}

void CDlgEffParticle::OnNMDblclkListPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_POS.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		m_pEff->SetPoistionEditPos( nSelect );
	}
}
