// DlgEffToon.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffToon.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgefftoon.h"

// CDlgEffToon dialog

IMPLEMENT_DYNAMIC(CDlgEffToon, CPropertyPage)
CDlgEffToon::CDlgEffToon( LOGFONT logfont )
	: CPropertyPage(CDlgEffToon::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffToon::~CDlgEffToon()
{
}

void CDlgEffToon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffToon, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE, OnBnClickedButtonTexture)
	ON_BN_CLICKED(IDC_BUTTON_OBLI, OnBnClickedButtonObli)
END_MESSAGE_MAP()


// CDlgEffToon message handlers
BOOL CDlgEffToon::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	return TRUE;
}

void CDlgEffToon::SetData( DxSkinPiece* pData, DxEffCharToon* pEff, BOOL bNEW )
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

void CDlgEffToon::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_TOON* pProperty = (EFFCHAR_PROPERTY_TOON*)m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Check( this, IDC_CHECK_OBLI, pProperty->m_dwFlag&USE_OBLIQUE);
			SetWin_Num_float( this, IDC_EDITS_SIZE, pProperty->m_fSize );
			SetWin_Num_float( this, IDC_EDITS_SIZEADD, pProperty->m_fSizeADD );
			SetWin_Text( this, IDC_EDIT_TEXTURE, pProperty->m_szToonTex );
			SetWin_Text( this, IDC_EDIT_TEXTURE_OBLI, pProperty->m_szObliqueTex );
		}
	}
}

BOOL CDlgEffToon::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_TOON Property;
		Property.m_fSize = GetWin_Num_float( this, IDC_EDITS_SIZE );
		Property.m_fSizeADD = GetWin_Num_float( this, IDC_EDITS_SIZEADD );
		std::string strTex = GetWin_Text( this, IDC_EDIT_TEXTURE );
		std::string strObli = GetWin_Text( this, IDC_EDIT_TEXTURE_OBLI );
		StringCchCopy( Property.m_szToonTex, 32, strTex.c_str() );
		StringCchCopy( Property.m_szObliqueTex, 32, strObli.c_str() );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_OBLI ), Property.m_dwFlag, USE_OBLIQUE );

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

		m_pPiece->CreateCartoonMesh( CRanEditSkinPieceView::GetView()->Get3DDevice(), Property.m_fSize, Property.m_fSizeADD );
		m_pPiece->CreateNormalMapMESH( CRanEditSkinPieceView::GetView()->Get3DDevice() );

		return TRUE;
	}

	return FALSE;
}

void CDlgEffToon::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffToon::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffToon::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffToon::OnBnClickedButtonTexture()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEXTURE, dlg.GetFileName().GetString() );
}

void CDlgEffToon::OnBnClickedButtonObli()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEXTURE_OBLI, dlg.GetFileName().GetString() );
}
