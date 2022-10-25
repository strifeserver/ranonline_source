// DlgEffBonePos.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffBonePos.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffbonepos.h"

#include "../SelColorDlg.h"
// CDlgEffBonePos dialog

IMPLEMENT_DYNAMIC(CDlgEffBonePos, CPropertyPage)
CDlgEffBonePos::CDlgEffBonePos( LOGFONT logfont )
	: CPropertyPage(CDlgEffBonePos::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffBonePos::~CDlgEffBonePos()
{
}

void CDlgEffBonePos::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffBonePos, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_BONE_SELECT, OnBnClickedButtonBoneSelect)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE, OnBnClickedButtonTexture)
	ON_BN_CLICKED(IDC_BUTTON_GET_FROM_SCREEN, OnBnClickedButtonGetFromScreen)
END_MESSAGE_MAP()


// CDlgEffBonePos message handlers
BOOL CDlgEffBonePos::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	return TRUE;
}

void CDlgEffBonePos::SetData( DxSkinPiece* pData, DxEffCharBonePosEff* pEff, BOOL bNEW )
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

void CDlgEffBonePos::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_BONEPOSEFF* pProperty = (EFFCHAR_PROPERTY_BONEPOSEFF*)m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Check( this, IDC_CHECK_ROTATE, pProperty->m_dwFlag&USE_ROTATE );
			SetWin_Check( this, IDC_CHECK_SEQ, pProperty->m_dwFlag&USE_SEQUENCE );
			SetWin_Check( this, IDC_CHECK_OFFSET, pProperty->m_dwFlag&USE_BONEOFFSETPOS );
			SetWin_Check( this, IDC_CHECK_USE_REMAIN, pProperty->m_dwFlag&USE_REMAIN );

			SetWin_Num_int( this, IDC_EDIT_PEC, pProperty->m_dwParticlePec );
			SetWin_Num_float( this, IDC_EDIT_CREATELENGTH, pProperty->m_fCreateLength );
			SetWin_Num_float( this, IDC_EDIT_LIFEMIN, pProperty->m_vLife.x );
			SetWin_Num_float( this, IDC_EDIT_LIFEMAX, pProperty->m_vLife.y );

			SetWin_Num_float( this, IDC_EDIT_SCALE_X, pProperty->m_vSize.x );
			SetWin_Num_float( this, IDC_EDIT_SCALE_Y, pProperty->m_vSize.y );
			SetWin_Num_float( this, IDC_EDIT_SCALE_Z, pProperty->m_vSize.z );

			SetWin_Num_float( this, IDC_EDIT_ROTATE_MIN, pProperty->m_vRotate.x );
			SetWin_Num_float( this, IDC_EDIT_ROTATE_MAX, pProperty->m_vRotate.y );

			SetWin_Num_int( this, IDC_EDIT_SEQ_COL, pProperty->m_dwSequenceCol );
			SetWin_Num_int( this, IDC_EDIT_SEQ_ROW, pProperty->m_dwSequenceRow );

			SetWin_Num_float( this, IDC_EDIT_VEL_X, pProperty->m_vVelocity.x );
			SetWin_Num_float( this, IDC_EDIT_VEL_Y, pProperty->m_vVelocity.y );
			SetWin_Num_float( this, IDC_EDIT_VEL_Z, pProperty->m_vVelocity.z );

			SetWin_Num_float( this, IDC_EDIT_LOCAL_X, pProperty->m_vLocalPos.x );
			SetWin_Num_float( this, IDC_EDIT_LOCAL_Y, pProperty->m_vLocalPos.y );
			SetWin_Num_float( this, IDC_EDIT_LOCAL_Z, pProperty->m_vLocalPos.z );

			D3DCOLOR color = pProperty->m_dwColor;
			BYTE a1,r1,g1,b1;
//			Color_DWORD_to_ARGB( color, a1, r1, g1, b1 );

			SetWin_Num_int( this, IDC_EDIT_COLOR_A, a1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_R, r1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_G, g1 );
			SetWin_Num_int( this, IDC_EDIT_COLOR_B, b1 );

			SetWin_Text( this, IDC_EDIT_BONE, pProperty->m_szBoneName );
			SetWin_Text( this, IDC_EDIT_TEXTURE, pProperty->m_szTexture );

			CRanEditSkinPieceView::GetView()->SetEditMatrix( m_pEff->GetInfluenceMatrix(), !( pProperty->m_dwFlag & USE_BONEOFFSETPOS ) );
		}
	}
}

BOOL CDlgEffBonePos::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_BONEPOSEFF Property;

		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ROTATE ), Property.m_dwFlag, USE_ROTATE );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SEQ ), Property.m_dwFlag, USE_SEQUENCE );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_OFFSET ), Property.m_dwFlag, USE_BONEOFFSETPOS );
		SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_USE_REMAIN ), Property.m_dwFlag, USE_REMAIN );

		Property.m_dwParticlePec = GetWin_Num_int( this, IDC_EDIT_PEC );
		Property.m_fCreateLength = GetWin_Num_float( this, IDC_EDIT_CREATELENGTH );
		Property.m_vLife.x = GetWin_Num_float( this, IDC_EDIT_LIFEMIN );
		Property.m_vLife.y = GetWin_Num_float( this, IDC_EDIT_LIFEMAX );

		Property.m_vSize.x = GetWin_Num_float( this, IDC_EDIT_SCALE_X );
		Property.m_vSize.y = GetWin_Num_float( this, IDC_EDIT_SCALE_Y );
		Property.m_vSize.z = GetWin_Num_float( this, IDC_EDIT_SCALE_Z );

		Property.m_vRotate.x = GetWin_Num_float( this, IDC_EDIT_ROTATE_MIN );
		Property.m_vRotate.y = GetWin_Num_float( this, IDC_EDIT_ROTATE_MAX );

		Property.m_dwSequenceCol = GetWin_Num_int( this, IDC_EDIT_SEQ_COL );
		Property.m_dwSequenceRow = GetWin_Num_int( this, IDC_EDIT_SEQ_ROW );

		Property.m_vVelocity.x = GetWin_Num_float( this, IDC_EDIT_VEL_X );
		Property.m_vVelocity.y = GetWin_Num_float( this, IDC_EDIT_VEL_Y );
		Property.m_vVelocity.z = GetWin_Num_float( this, IDC_EDIT_VEL_Z );

		Property.m_vLocalPos.x = GetWin_Num_float( this, IDC_EDIT_LOCAL_X );
		Property.m_vLocalPos.y = GetWin_Num_float( this, IDC_EDIT_LOCAL_Y );
		Property.m_vLocalPos.z = GetWin_Num_float( this, IDC_EDIT_LOCAL_Z );

		BYTE a1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_A );
		BYTE r1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_R );
		BYTE g1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_G );
		BYTE b1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_COLOR_B );

//		Property.m_dwColor = Color_ARGB_to_DWORD( a1, r1, g1, b1 );

		std::string strBone = GetWin_Text( this, IDC_EDIT_BONE );
		std::string strTex = GetWin_Text( this, IDC_EDIT_TEXTURE );

		StringCchCopy( Property.m_szBoneName , BONEPOSEFF_TEXNAME_SIZE, strBone.c_str() );
		StringCchCopy( Property.m_szTexture , BONEPOSEFF_TEXNAME_SIZE, strTex.c_str() );

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

		CRanEditSkinPieceView::GetView()->SetEditMatrix( m_pEff->GetInfluenceMatrix(), !( Property.m_dwFlag & USE_BONEOFFSETPOS ) );

		return TRUE;
	}

	return FALSE;
}

void CDlgEffBonePos::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffBonePos::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffBonePos::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffBonePos::OnBnClickedButtonBoneSelect()
{
	if ( m_pPiece && m_pEff )
	{
		std::string str = CRanEditSkinPieceView::GetView()->strBoneName;
		if ( str.size() != 0 )
		{
			SetWin_Text( this, IDC_EDIT_BONE, str.c_str() );

			//reset the position modifier when we change the bone
			SetWin_Num_float( this, IDC_EDIT_LOCAL_X, 0.0f );
			SetWin_Num_float( this, IDC_EDIT_LOCAL_Y, 0.0f );
			SetWin_Num_float( this, IDC_EDIT_LOCAL_Z, 0.0f );
			SetWin_Check( this, IDC_CHECK_OFFSET, FALSE );
		}
		else
		{
			CDebugSet::MsgBox( "Empty Bone" );
		}
	}
}

void CDlgEffBonePos::OnBnClickedButtonColor()
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

void CDlgEffBonePos::OnBnClickedButtonTexture()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEXTURE, dlg.GetFileName().GetString() );
}

void CDlgEffBonePos::OnBnClickedButtonGetFromScreen()
{
	if ( m_pPiece && m_pEff )
	{
		m_pEff->SetChangeMatrix( CRanEditSkinPieceView::GetView()->matEdit );
		ShowData();
	}
}
