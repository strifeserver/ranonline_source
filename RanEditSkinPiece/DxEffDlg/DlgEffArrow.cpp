// DlgEffArrow.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffArrow.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffarrow.h"

#include "../SelColorDlg.h"

// CDlgEffArrow dialog

IMPLEMENT_DYNAMIC(CDlgEffArrow, CPropertyPage)
CDlgEffArrow::CDlgEffArrow( LOGFONT logfont )
	: CPropertyPage(CDlgEffArrow::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffArrow::~CDlgEffArrow()
{
}

void CDlgEffArrow::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffArrow, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_EFF1, OnBnClickedButtonEff1)
	ON_BN_CLICKED(IDC_BUTTON_EFF2, OnBnClickedButtonEff2)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_MESH, OnBnClickedButtonMesh)
END_MESSAGE_MAP()


// CDlgEffArrow message handlers
BOOL CDlgEffArrow::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	std::string strBlend[5] = 
	{
		"Level 1",
		"Level 2",
		"Level 3",
		"Level 4",
		"Level 5",
	};

	SetWin_Combo_Init( this, IDC_COMBO_BLEND, strBlend, 5 );

	return TRUE;
}

void CDlgEffArrow::SetData( DxSkinPiece* pData, DxEffCharArrow* pEff, BOOL bNEW )
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

void CDlgEffArrow::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_ARROW* pEff = (EFFCHAR_PROPERTY_ARROW*) m_pEff->GetProperty();
		if( pEff )
		{
			SetWin_Text( this, IDC_EDIT_EFF1, pEff->m_szFileName_R );
			SetWin_Text( this, IDC_EDIT_EFF2, pEff->m_szFileName_C );

			SetWin_Check( this, IDC_CHECK_USE_EFF, pEff->m_dwFlag&USEEFFECT );
			SetWin_Check( this, IDC_CHECK_USE_RANDOM, pEff->m_dwFlag&USERANDROTATE );
			SetWin_Check( this, IDC_CHECK_USE_SEC, pEff->m_dwFlag&USESEC );
			SetWin_Check( this, IDC_CHECK_USE_ARROW, pEff->m_dwFlag&USEARROW );
			SetWin_Check( this, IDC_CHECK_USE_SIZE, pEff->m_dwFlag&USESIZE );

			SetWin_Num_int( this, IDC_EDIT_SEC, pEff->m_nSec );
			SetWin_Num_float( this, IDC_EDIT_SIZE, pEff->m_fSize );
			SetWin_Num_float( this, IDC_EDIT_FULLTIME, pEff->m_fFullTime );
			SetWin_Num_float( this, IDC_EDIT_ALPHA, pEff->m_fAlphaTime );
			SetWin_Num_float( this, IDC_EDIT_SET, pEff->m_fSetRate );
			SetWin_Num_float( this, IDC_EDIT_DEPTH, pEff->m_fDepth );

			D3DCOLOR color1 = pEff->m_vColor;;
			BYTE a1,r1,g1,b1;
//			Color_DWORD_to_ARGB( color1, a1, r1, g1, b1 );	
			SetWin_Num_int( this, IDC_EDIT_COLOR_A, a1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_R, r1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_G, g1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_B, b1 );

			SetWin_Combo_Sel( this, IDC_COMBO_BLEND, pEff->m_nBlend-1 );

			SetWin_Text( this, IDC_EDIT_MESH, pEff->m_szMeshFile );
		}
	}
}

BOOL CDlgEffArrow::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_ARROW Property;

		std::string str_R = GetWin_Text( this, IDC_EDIT_EFF1 );
		std::string str_C = GetWin_Text( this, IDC_EDIT_EFF2 );

		StringCchCopy( Property.m_szFileName_R , MAX_PATH, str_R.c_str() );
		StringCchCopy( Property.m_szFileName_C , MAX_PATH, str_C.c_str() );

		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_EFF ), Property.m_dwFlag, USEEFFECT );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_RANDOM ), Property.m_dwFlag, USERANDROTATE );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_SEC ), Property.m_dwFlag, USESEC );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_ARROW ), Property.m_dwFlag, USEARROW );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_SIZE ), Property.m_dwFlag, USESIZE );

		Property.m_nSec = GetWin_Num_int( this, IDC_EDIT_SEC );
		Property.m_fSize = GetWin_Num_float( this, IDC_EDIT_SIZE );
		Property.m_fFullTime = GetWin_Num_float( this, IDC_EDIT_FULLTIME );
		Property.m_fAlphaTime = GetWin_Num_float( this, IDC_EDIT_ALPHA );
		Property.m_fSetRate = GetWin_Num_float( this, IDC_EDIT_SET );
		Property.m_fDepth = GetWin_Num_float( this, IDC_EDIT_DEPTH );

		BYTE a1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_A );
		BYTE r1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_R );
		BYTE g1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_G );
		BYTE b1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_B );
//		Property.m_vColor = Color_ARGB_to_DWORD( a1, r1, g1, b1 );

		Property.m_nBlend = GetWin_Combo_Sel( this, IDC_COMBO_BLEND ) +1;

		std::string strMESH = GetWin_Text( this, IDC_EDIT_MESH );
		StringCchCopy( Property.m_szMeshFile, MAX_PATH, strMESH.c_str() );

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

void CDlgEffArrow::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffArrow::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffArrow::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffArrow::OnBnClickedButtonEff1()
{
	CString szFilter = "Effect File (*.egp)|*.egp|";
	CFileDialog dlg(TRUE,".egp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = DxEffSinglePropGMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_EFF1, dlg.GetFileName().GetString() );
}

void CDlgEffArrow::OnBnClickedButtonEff2()
{
	CString szFilter = "Effect File (*.egp)|*.egp|";
	CFileDialog dlg(TRUE,".egp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = DxEffSinglePropGMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_EFF2, dlg.GetFileName().GetString() );
}

void CDlgEffArrow::OnBnClickedButtonColor()
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

void CDlgEffArrow::OnBnClickedButtonMesh()
{
	CString szFilter = "X Mesh File (*.x)|*.x|";
	CFileDialog dlg(TRUE,".x",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter,this);
	dlg.m_ofn.lpstrInitialDir = DxSimpleMeshMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_MESH, dlg.GetFileName().GetString() );
}
