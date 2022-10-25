// CDlgEffSingle.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffSingle.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include ".\dlgeffsingle.h"
#include "../ToolTab.h"

// CDlgEffSingle dialog

IMPLEMENT_DYNAMIC(CDlgEffSingle, CPropertyPage)
CDlgEffSingle::CDlgEffSingle( LOGFONT logfont )
	: CPropertyPage(CDlgEffSingle::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	D3DXMatrixIdentity( &matEff );
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffSingle::~CDlgEffSingle()
{
}

void CDlgEffSingle::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_DOT1, m_list_Dot1 );
	DDX_Control( pDX, IDC_LIST_DOT2, m_list_Dot2 );
}


BEGIN_MESSAGE_MAP(CDlgEffSingle, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_1DOT, OnBnClickedCheck1dot)
	ON_BN_CLICKED(IDC_CHECK_2DOTS, OnBnClickedCheck2dots)
	ON_BN_CLICKED(IDC_CHECK_MATRIX, OnBnClickedCheckMatrix)
	ON_BN_CLICKED(IDC_BUTTON_FILENAME, OnBnClickedButtonFilename)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOT1, OnNMDblclkListDot1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOT2, OnNMDblclkListDot2)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
END_MESSAGE_MAP()


// CDlgEffSingle message handlers
BOOL CDlgEffSingle::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	std::string strAni[3] = 
	{
		"None",
		"Attack",
		"Other"
	};


	std::string strWeather[3] = 
	{
		"None",
		"Not Use Rain",
		"Use Rain"
	};

	SetWin_Combo_Init( this, IDC_COMBO_ANISET, strAni, 3 );
	SetWin_Combo_Init( this, IDC_COMBO_WEATHERSET, strWeather, 3 );

	std::string strDot[9] = 
	{
		"IMAGE01",
		"IMAGE02",
		"IMAGE11",
		"IMAGE12",
		"IMAGE21",
		"IMAGE22",
		"RAIN01",
		"RAIN02",
		"RAIN03",
	};
	
	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Dot1.SetExtendedStyle ( m_list_Dot1.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Dot1.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 1;	
		char* szColumnName1[nColumnCount] = { "Trace"};
		int nColumnWidthPercent[nColumnCount] = { 100 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Dot1.InsertColumn (i, &lvColumn );
		}
	}

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Dot2.SetExtendedStyle ( m_list_Dot2.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Dot2.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 1;	
		char* szColumnName1[nColumnCount] = { "Trace"};
		int nColumnWidthPercent[nColumnCount] = { 100 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Dot2.InsertColumn (i, &lvColumn );
		}
	}

	for ( int i =0; i < 9; ++i )
	{
		m_list_Dot1.InsertItem( i, strDot[i].c_str() );
		m_list_Dot2.InsertItem( i, strDot[i].c_str() );
	}

	return TRUE;
}

void CDlgEffSingle::SetData( DxSkinPiece* pData, DxEffCharSingle* pEff, BOOL bNEW )
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

void CDlgEffSingle::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_SINGLE* Property = (EFFCHAR_PROPERTY_SINGLE*)m_pEff->GetProperty();
		if ( Property )
		{
			SetWin_Text( this, IDC_EDIT_FILENAME, Property->m_szFileName );
			SetWin_Text( this, IDC_EDIT_TRACE1, Property->m_szTrace );
			SetWin_Text( this, IDC_EDIT_TRACE2, Property->m_szTraceBack );
			SetWin_Num_float( this, IDC_EDIT_MOVEPOS_1, Property->m_fDepthUp );
			SetWin_Num_float( this, IDC_EDIT_MOVEPOS_2, Property->m_fDepthDown );
			SetWin_Num_float( this, IDC_EDIT_SCALE, Property->m_fScale );

			SetWin_Check( this, IDC_CHECK_1DOT, Property->m_dwFlag&USE_1POINT );
			SetWin_Check( this, IDC_CHECK_2DOTS, Property->m_dwFlag&USE_2POINT );
			SetWin_Check( this, IDC_CHECK_MATRIX, Property->m_dwFlag&USE_MATRIX );

			SetWin_Check( this, IDC_CHECK_AUTOMOVE, Property->m_dwFlag&USE_AUTOMOVE );
			SetWin_Check( this, IDC_CHECK_REPEATMOVE, Property->m_dwFlag&USE_REPEATMOVE );
			SetWin_Num_float( this, IDC_EDIT_REPEATSPEED, Property->m_fRepeatSpeed );

			SetWin_Check( this, IDC_CHECK_REPEAT, Property->m_dwFlag&USE_REPEAT );
			SetWin_Num_float( this, IDC_EDIT_COOLDOWN, Property->m_fCoolTime );

			SetWin_Combo_Sel( this, IDC_COMBO_ANISET, Property->m_nAniMType );
			SetWin_Combo_Sel( this, IDC_COMBO_WEATHERSET, Property->m_nWeather );
		}
	}
}

BOOL CDlgEffSingle::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_SINGLE Property;

		std::string strFile = GetWin_Text( this, IDC_EDIT_FILENAME );
		std::string strDot1 = GetWin_Text( this, IDC_EDIT_TRACE1 );
		std::string strDot2 = GetWin_Text( this, IDC_EDIT_TRACE2 );

		StringCchCopy( Property.m_szFileName , MAX_PATH, strFile.c_str() );
		StringCchCopy( Property.m_szTrace, STRACE_NSIZE, strDot1.c_str() );
		StringCchCopy( Property.m_szTraceBack, STRACE_NSIZE, strDot2.c_str() );
		
		Property.m_fDepthUp = GetWin_Num_float( this, IDC_EDIT_MOVEPOS_1 );
		Property.m_fDepthDown = GetWin_Num_float( this, IDC_EDIT_MOVEPOS_2 );
		Property.m_fScale = GetWin_Num_float( this, IDC_EDIT_SCALE );
		Property.m_fRepeatSpeed = GetWin_Num_float( this, IDC_EDIT_REPEATSPEED );
		Property.m_fCoolTime = GetWin_Num_float( this, IDC_EDIT_COOLDOWN );
			
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_1DOT ), Property.m_dwFlag,USE_1POINT );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_2DOTS ), Property.m_dwFlag,USE_2POINT );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_MATRIX ), Property.m_dwFlag,USE_MATRIX );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_AUTOMOVE ), Property.m_dwFlag,USE_AUTOMOVE );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_REPEATMOVE ), Property.m_dwFlag,USE_REPEATMOVE );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_REPEAT ), Property.m_dwFlag,USE_REPEAT );

		Property.m_nAniMType = GetWin_Combo_Sel( this, IDC_COMBO_ANISET );
		Property.m_nWeather = GetWin_Combo_Sel( this, IDC_COMBO_WEATHERSET );

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

void CDlgEffSingle::OnBnClickedCheck1dot()
{
	SetWin_Check( this, IDC_CHECK_1DOT, TRUE );
	SetWin_Check( this, IDC_CHECK_2DOTS, FALSE );
	SetWin_Check( this, IDC_CHECK_MATRIX, FALSE );
}

void CDlgEffSingle::OnBnClickedCheck2dots()
{
	SetWin_Check( this, IDC_CHECK_1DOT, FALSE );
	SetWin_Check( this, IDC_CHECK_2DOTS, TRUE );
	SetWin_Check( this, IDC_CHECK_MATRIX, FALSE );
}

void CDlgEffSingle::OnBnClickedCheckMatrix()
{
	SetWin_Check( this, IDC_CHECK_1DOT, FALSE );
	SetWin_Check( this, IDC_CHECK_2DOTS, FALSE );
	SetWin_Check( this, IDC_CHECK_MATRIX, TRUE );
}

void CDlgEffSingle::OnBnClickedButtonFilename()
{
	CString szFilter = "Effect File (*.egp)|*.egp|";
	CFileDialog dlg(TRUE,".egp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = DxEffSinglePropGMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_FILENAME, dlg.GetFileName().GetString() );
}

void CDlgEffSingle::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffSingle::OnNMDblclkListDot1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_Dot1.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		std::string str = m_list_Dot1.GetItemText( nSelect, 0 );
		SetWin_Text( this, IDC_EDIT_TRACE1, str.c_str());
	}
}

void CDlgEffSingle::OnNMDblclkListDot2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_Dot2.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		std::string str = m_list_Dot2.GetItemText( nSelect, 0 );
		SetWin_Text( this, IDC_EDIT_TRACE2, str.c_str());
	}
}

void CDlgEffSingle::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffSingle::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

