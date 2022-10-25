// DlgEffGhosting.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffGhosting.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffghosting.h"

#include "../SelColorDlg.h"
// CDlgEffGhosting dialog

IMPLEMENT_DYNAMIC(CDlgEffGhosting, CPropertyPage)
CDlgEffGhosting::CDlgEffGhosting( LOGFONT logfont )
	: CPropertyPage(CDlgEffGhosting::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffGhosting::~CDlgEffGhosting()
{
}

void CDlgEffGhosting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffGhosting, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
END_MESSAGE_MAP()


// CDlgEffGhosting message handlers
BOOL CDlgEffGhosting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	SetWin_Combo_Init( this, IDC_COMBO_ANI_MAIN, COMMENT::ANI_MAINTYPE, AN_TYPE_SIZE );
	SetWin_Combo_Init( this, IDC_COMBO_ANI_SUB, COMMENT::ANI_SUBTYPE, AN_SUB_00_SIZE );

	return TRUE;
}

void CDlgEffGhosting::SetData( DxSkinPiece* pData, DxEffCharGhosting* pEff, BOOL bNEW )
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

void CDlgEffGhosting::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_GHOSTING* pProperty = (EFFCHAR_PROPERTY_GHOSTING*)m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Combo_Sel( this, IDC_COMBO_ANI_MAIN, pProperty->m_AniMType );
			SetWin_Combo_Sel( this, IDC_COMBO_ANI_SUB, pProperty->m_AniSType );
			SetWin_Num_int( this, IDC_EDIT_GNUM, pProperty->m_dwGhostingNum );
			SetWin_Num_int( this, IDC_EDIT_UNIT, pProperty->m_dwUnit );
			SetWin_Num_int( this, IDC_EDIT_ALPHA, pProperty->m_dwAlphaMax );

			D3DCOLOR color = pProperty->m_dwColor;
			BYTE a1,r1,g1,b1;
//			Color_DWORD_to_ARGB( color, a1, r1, g1, b1 );
			
			SetWin_Num_int( this, IDC_EDIT_COLOR_A, a1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_R, r1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_G, g1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_B, b1 );

		}
	}
}

BOOL CDlgEffGhosting::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_GHOSTING Property;
		Property.m_AniMType = (EMANI_MAINTYPE)GetWin_Combo_Sel( this, IDC_COMBO_ANI_MAIN );
		Property.m_AniSType = (EMANI_SUBTYPE)GetWin_Combo_Sel( this, IDC_COMBO_ANI_SUB );

		Property.m_dwGhostingNum = GetWin_Num_int( this, IDC_EDIT_GNUM );
		Property.m_dwUnit = GetWin_Num_int( this, IDC_EDIT_UNIT );
		Property.m_dwAlphaMax = GetWin_Num_int( this, IDC_EDIT_ALPHA );

		BYTE a1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_A );
		BYTE r1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_R );
		BYTE g1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_G );
		BYTE b1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_B );
//		Property.m_dwColor = Color_ARGB_to_DWORD( a1, r1, g1, b1 );

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

void CDlgEffGhosting::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffGhosting::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffGhosting::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffGhosting::OnBnClickedButtonColor()
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
