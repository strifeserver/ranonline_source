// DlgEffDot3.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffDot3.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffdot3.h"


// CDlgEffDot3 dialog

IMPLEMENT_DYNAMIC(CDlgEffDot3, CPropertyPage)
CDlgEffDot3::CDlgEffDot3( LOGFONT logfont )
	: CPropertyPage(CDlgEffDot3::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffDot3::~CDlgEffDot3()
{
}

void CDlgEffDot3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffDot3, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE, OnBnClickedButtonTexture)
	ON_BN_CLICKED(IDC_BUTTON_SRC_TEX_1, OnBnClickedButtonSrcTex1)
	ON_BN_CLICKED(IDC_BUTTON_SRC_TEX_2, OnBnClickedButtonSrcTex2)
	ON_BN_CLICKED(IDC_BUTTON_SRC_TEX_3, OnBnClickedButtonSrcTex3)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_1, OnBnClickedButtonTexture1)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_2, OnBnClickedButtonTexture2)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE_3, OnBnClickedButtonTexture3)
	ON_BN_CLICKED(IDC_BUTTON_DOT_TEXTURE_1, OnBnClickedButtonDotTexture1)
	ON_BN_CLICKED(IDC_BUTTON_DOT_TEXTURE_2, OnBnClickedButtonDotTexture2)
	ON_BN_CLICKED(IDC_BUTTON_DOT_TEXTURE_3, OnBnClickedButtonDotTexture3)
END_MESSAGE_MAP()


// CDlgEffDot3 message handlers
BOOL CDlgEffDot3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	std::string strDetail[5] = 
	{
		"CRDT_DEFAULT",
		"CRDT_SPECULAR",
		"CRDT_DOT3_SPECULAR",
		"CRDT_DOT3_REFLECT",
		"CRDT_GLOW",
	};

	SetWin_Combo_Init( this, IDC_COMBO_DETAIL_1, strDetail, 5 );
	SetWin_Combo_Init( this, IDC_COMBO_DETAIL_2, strDetail, 5 );
	SetWin_Combo_Init( this, IDC_COMBO_DETAIL_3, strDetail, 5 );

	std::string strCOLOROP[3] = 
	{
		"Modulate",
		"Modulate(2X)",
		"Modulate(4X)",
	};

	SetWin_Combo_Init( this, IDC_COMBO_COLOROP, strCOLOROP, 3 );

	return TRUE;
}

void CDlgEffDot3::SetData( DxSkinPiece* pData, DxEffCharDot3* pEff, BOOL bNEW )
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

void CDlgEffDot3::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_DOT3* pProperty = (EFFCHAR_PROPERTY_DOT3*)m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Check( this, IDC_CHECK_USE_TEX_1, pProperty->m_dwFlag&USETEXTURE1 );
			SetWin_Check( this, IDC_CHECK_USE_TEX_2, pProperty->m_dwFlag&USETEXTURE2 );
			SetWin_Check( this, IDC_CHECK_USE_TEX_3, pProperty->m_dwFlag&USETEXTURE3 );

			SetWin_Combo_Sel( this, IDC_COMBO_COLOROP, pProperty->m_dwColorOP - 4 );

			SetWin_Combo_Sel( this, IDC_COMBO_DETAIL_1, pProperty->m_dwDetail_1 );
			SetWin_Combo_Sel( this, IDC_COMBO_DETAIL_2, pProperty->m_dwDetail_2 );
			SetWin_Combo_Sel( this, IDC_COMBO_DETAIL_3, pProperty->m_dwDetail_3 );

			SetWin_Text( this, IDC_EDIT_TEXTURE, pProperty->m_szTexture );
			SetWin_Text( this, IDC_EDIT_SRC_TEX1, pProperty->m_szSrcTex_1 );
			SetWin_Text( this, IDC_EDIT_SRC_TEX2, pProperty->m_szSrcTex_2 );
			SetWin_Text( this, IDC_EDIT_SRC_TEX3, pProperty->m_szSrcTex_3 );
			SetWin_Text( this, IDC_EDIT_TEXTURE_1, pProperty->m_szTexture_1 );
			SetWin_Text( this, IDC_EDIT_TEXTURE_2, pProperty->m_szTexture_2 );
			SetWin_Text( this, IDC_EDIT_TEXTURE_3, pProperty->m_szTexture_3 );
			SetWin_Text( this, IDC_EDIT_DOTTEXTURE_1, pProperty->m_szDot3Tex_1 );
			SetWin_Text( this, IDC_EDIT_DOTTEXTURE_2, pProperty->m_szDot3Tex_2 );
			SetWin_Text( this, IDC_EDIT_DOTTEXTURE_3, pProperty->m_szDot3Tex_3 );
		}
	}
}

BOOL CDlgEffDot3::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_DOT3 Property;
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_TEX_1 ), Property.m_dwFlag, USETEXTURE1 );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_TEX_2 ), Property.m_dwFlag, USETEXTURE2 );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_TEX_3 ), Property.m_dwFlag, USETEXTURE3 );
		Property.m_dwColorOP = GetWin_Combo_Sel( this, IDC_COMBO_COLOROP ) + 4;
		Property.m_dwDetail_1 = (CRDT_TYPE)GetWin_Combo_Sel( this, IDC_COMBO_DETAIL_1 );
		Property.m_dwDetail_2 = (CRDT_TYPE)GetWin_Combo_Sel( this, IDC_COMBO_DETAIL_2 );
		Property.m_dwDetail_3 = (CRDT_TYPE)GetWin_Combo_Sel( this, IDC_COMBO_DETAIL_3 );

		std::string strTexture = GetWin_Text( this, IDC_EDIT_TEXTURE );
		std::string strSrcTexture1 = GetWin_Text( this, IDC_EDIT_SRC_TEX1 );
		std::string strSrcTexture2 = GetWin_Text( this, IDC_EDIT_SRC_TEX2 );
		std::string strSrcTexture3 = GetWin_Text( this, IDC_EDIT_SRC_TEX3 );
		std::string strTexture_1 = 	GetWin_Text( this, IDC_EDIT_TEXTURE_1 );
		std::string strTexture_2 =	GetWin_Text( this, IDC_EDIT_TEXTURE_2 );
		std::string strTexture_3 =	GetWin_Text( this, IDC_EDIT_TEXTURE_3 );
		std::string strTextureDot_1 = GetWin_Text( this, IDC_EDIT_DOTTEXTURE_1 );
		std::string strTextureDot_2 = GetWin_Text( this, IDC_EDIT_DOTTEXTURE_2 );
		std::string strTextureDot_3 = GetWin_Text( this, IDC_EDIT_DOTTEXTURE_3 );

		StringCchCopy( Property.m_szTexture , MAX_PATH, strTexture.c_str() );
		StringCchCopy( Property.m_szSrcTex_1 , MAX_PATH, strSrcTexture1.c_str() );
		StringCchCopy( Property.m_szSrcTex_2 , MAX_PATH, strSrcTexture2.c_str() );
		StringCchCopy( Property.m_szSrcTex_3 , MAX_PATH, strSrcTexture3.c_str() );
		StringCchCopy( Property.m_szTexture_1 , MAX_PATH, strTexture_1.c_str() );
		StringCchCopy( Property.m_szTexture_2 , MAX_PATH, strTexture_2.c_str() );
		StringCchCopy( Property.m_szTexture_3 , MAX_PATH, strTexture_3.c_str() );
		StringCchCopy( Property.m_szDot3Tex_1 , MAX_PATH, strTextureDot_1.c_str() );
		StringCchCopy( Property.m_szDot3Tex_2 , MAX_PATH, strTextureDot_2.c_str() );
		StringCchCopy( Property.m_szDot3Tex_3 , MAX_PATH, strTextureDot_3.c_str() );

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

void CDlgEffDot3::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffDot3::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffDot3::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffDot3::AddTexture( int nID )
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".x",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, nID, dlg.GetFileName().GetString() );
}

void CDlgEffDot3::OnBnClickedButtonTexture()
{
	AddTexture( IDC_EDIT_TEXTURE );
}

void CDlgEffDot3::OnBnClickedButtonSrcTex1()
{
	AddTexture( IDC_EDIT_SRC_TEX1 );
}

void CDlgEffDot3::OnBnClickedButtonSrcTex2()
{
	AddTexture( IDC_EDIT_SRC_TEX2 );
}

void CDlgEffDot3::OnBnClickedButtonSrcTex3()
{
	AddTexture( IDC_EDIT_SRC_TEX3 );
}

void CDlgEffDot3::OnBnClickedButtonTexture1()
{
	AddTexture( IDC_EDIT_TEXTURE_1 );
}

void CDlgEffDot3::OnBnClickedButtonTexture2()
{
	AddTexture( IDC_EDIT_TEXTURE_2 );
}

void CDlgEffDot3::OnBnClickedButtonTexture3()
{
	AddTexture( IDC_EDIT_TEXTURE_3 );
}

void CDlgEffDot3::OnBnClickedButtonDotTexture1()
{
	AddTexture( IDC_EDIT_DOTTEXTURE_1 );
}

void CDlgEffDot3::OnBnClickedButtonDotTexture2()
{
	AddTexture( IDC_EDIT_DOTTEXTURE_2 );
}

void CDlgEffDot3::OnBnClickedButtonDotTexture3()
{
	AddTexture( IDC_EDIT_DOTTEXTURE_3 );
}
