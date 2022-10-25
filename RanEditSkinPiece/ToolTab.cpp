#include "stdafx.h"
#include <vector>
#include "resource.h"
#include "ToolTab.h"
#include "RanEditSkinPiece.h"
#include "RanEditSkinPieceView.h"

#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTab

IMPLEMENT_DYNAMIC(CToolTab, CPropertySheet)

CToolTab::CToolTab(CRect& rect, LOGFONT logfont, CWnd* pParentWnd)
	 : CPropertySheet("", pParentWnd),
	 m_bNoTabs(FALSE),
	 m_pWndParent(NULL),
	 m_iDistanceFromTop(0),
	 m_pFont ( NULL ),

	m_DlgMain( logfont ),
	m_DlgEffSingle( logfont ),
	m_DlgEffBlur( logfont ),
	m_DlgEffCloneBlur( logfont ),
	m_DlgEffDust( logfont ),
	m_DlgEffShock( logfont ),
	m_DlgEffAttribute( logfont ),
	m_DlgEffArrow( logfont ),
	m_DlgEffSpecular( logfont ),
	m_DlgEffLevel( logfont ),
	m_DlgEffEmit( logfont ),
	m_DlgEffAlpha( logfont ),
	m_DlgEffNeon( logfont ),
	m_DlgEffMark( logfont ),
	m_DlgEffNoAlpha( logfont ),
	m_DlgEffReflection2( logfont ),
	m_DlgEffAmbient( logfont ),
	m_DlgEffDot3( logfont ),
	m_DlgEffMultiTex( logfont ),
	m_DlgEffGhosting( logfont ),
	m_DlgEffSpecular2( logfont ),
	m_DlgEffToon( logfont ),
	m_DlgEffTexDiff( logfont ),
	m_DlgEffParticle( logfont ),
	m_DlgEffBonePos( logfont ),
	m_DlgEffBoneList( logfont ),
	m_DlgEffUserColor( logfont ),
	m_DlgEffNormalMap( logfont ),
	m_DlgEffLine2Bone( logfont ),
	m_DlgEffAround( logfont )
{
	m_pWndParent = pParentWnd;
	m_Rect = rect;

	m_bNoTabs = TRUE;
	m_bStacked = FALSE;

	m_DlgMain.SetToolTab ( this );
	AddPage ( &m_DlgMain );

	m_DlgEffSingle.SetToolTab ( this );
	AddPage ( &m_DlgEffSingle );

	m_DlgEffBlur.SetToolTab ( this );
	AddPage ( &m_DlgEffBlur );

	m_DlgEffCloneBlur.SetToolTab ( this );
	AddPage ( &m_DlgEffCloneBlur );

	m_DlgEffDust.SetToolTab ( this );
	AddPage ( &m_DlgEffDust );

	m_DlgEffShock.SetToolTab ( this );
	AddPage ( &m_DlgEffShock );

	m_DlgEffAttribute.SetToolTab ( this );
	AddPage ( &m_DlgEffAttribute );

	m_DlgEffArrow.SetToolTab ( this );
	AddPage ( &m_DlgEffArrow );

	m_DlgEffSpecular.SetToolTab ( this );
	AddPage ( &m_DlgEffSpecular );

	m_DlgEffLevel.SetToolTab ( this );
	AddPage ( &m_DlgEffLevel );

	m_DlgEffEmit.SetToolTab ( this );
	AddPage ( &m_DlgEffEmit );

	m_DlgEffAlpha.SetToolTab ( this );
	AddPage ( &m_DlgEffAlpha );

	m_DlgEffNeon.SetToolTab ( this );
	AddPage ( &m_DlgEffNeon );

	m_DlgEffMark.SetToolTab ( this );
	AddPage ( &m_DlgEffMark );

	m_DlgEffNoAlpha.SetToolTab ( this );
	AddPage ( &m_DlgEffNoAlpha );

	m_DlgEffReflection2.SetToolTab ( this );
	AddPage ( &m_DlgEffReflection2 );

	m_DlgEffAmbient.SetToolTab ( this );
	AddPage ( &m_DlgEffAmbient );

	m_DlgEffDot3.SetToolTab ( this );
	AddPage ( &m_DlgEffDot3 );

	m_DlgEffMultiTex.SetToolTab ( this );
	AddPage ( &m_DlgEffMultiTex );

	m_DlgEffGhosting.SetToolTab ( this );
	AddPage ( &m_DlgEffGhosting );

	m_DlgEffSpecular2.SetToolTab ( this );
	AddPage ( &m_DlgEffSpecular2 );

	m_DlgEffToon.SetToolTab ( this );
	AddPage ( &m_DlgEffToon );

	m_DlgEffTexDiff.SetToolTab ( this );
	AddPage ( &m_DlgEffTexDiff );

	m_DlgEffParticle.SetToolTab ( this );
	AddPage ( &m_DlgEffParticle );

	m_DlgEffBonePos.SetToolTab ( this );
	AddPage ( &m_DlgEffBonePos );

	m_DlgEffBoneList.SetToolTab ( this );
	AddPage ( &m_DlgEffBoneList );

	m_DlgEffUserColor.SetToolTab ( this );
	AddPage ( &m_DlgEffUserColor );

	m_DlgEffNormalMap.SetToolTab ( this );
	AddPage ( &m_DlgEffNormalMap );

	m_DlgEffLine2Bone.SetToolTab ( this );
	AddPage ( &m_DlgEffLine2Bone );

	m_DlgEffAround.SetToolTab ( this );
	AddPage ( &m_DlgEffAround );

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CToolTab::~CToolTab()
{
	SAFE_DELETE( m_pFont );
}

static int module_piButtons[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };

BOOL CToolTab::OnInitDialog() 
{
	HWND hTabWnd;
	CRect rectTabCtrl;
	ChangeDialogFont( this, m_pFont, CDF_NONE );

	if( m_Rect.right )
	{
		hTabWnd = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL);
		ASSERT(hTabWnd != NULL);
		::GetWindowRect(hTabWnd, &rectTabCtrl); 
		ScreenToClient(rectTabCtrl);

		if( !m_bNoTabs && rectTabCtrl.right > (m_Rect.Width() - 3) )
		{
			rectTabCtrl.right = m_Rect.Width() - 3;
		}
		rectTabCtrl.bottom = 32; 
		::MoveWindow( hTabWnd, 0, m_iDistanceFromTop, rectTabCtrl.Width(), rectTabCtrl.bottom, TRUE );
		BOOL bResult = (BOOL)Default(); 

		if( m_bNoTabs )
		{
			::ShowWindow(hTabWnd, SW_HIDE);
			::EnableWindow(hTabWnd, FALSE);
		}

		if (!m_bStacked)
		{
			hTabWnd = (HWND)SendMessage(PSM_GETTABCONTROL);
			if (hTabWnd != NULL)
			{
				CWnd::ModifyStyle(hTabWnd, TCS_MULTILINE, TCS_SINGLELINE, 0);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			HWND hWnd = ::GetDlgItem(m_hWnd, module_piButtons[i]);
			if (hWnd != NULL)
			{
				::ShowWindow(hWnd, SW_HIDE);
				::EnableWindow(hWnd, FALSE);
			}
		}

		MoveWindow( m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom /*- cyDiff*/ );

		return bResult;
	}

	return CPropertySheet::OnInitDialog();
}

BEGIN_MESSAGE_MAP(CToolTab, CPropertySheet)
	//{{AFX_MSG_MAP(CToolTab)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CToolTab::ActiveDlgPage( int nPage )
{
	DxSkinPiece*	pPiece = CRanEditSkinPieceView::GetView()->m_pPiece;
	if ( !pPiece )	return;

	switch ( nPage )
	{
	case DLG_MAIN:
		{
			SetActivePage ( DLG_MAIN );
			m_DlgMain.SetData( pPiece );
		}break;
	};

}

void CToolTab::EffCharEdit( DxSkinPiece* pData, DxEffChar* pEff, BOOL bNEW /*= FALSE*/ )
{
	if ( pData && pEff )
	{
		DWORD dwTYPEID = pEff->GetTypeID();

		switch ( dwTYPEID )
		{
		case EMEFFCHAR_SINGLE:
			{
				SetActivePage ( DLG_EFF_SINGLE );
				m_DlgEffSingle.SetData( pData, (DxEffCharSingle*)pEff, bNEW );
			}break;
		case EMEFFCHAR_BLUR:
			{
				SetActivePage ( DLG_EFF_BLUR );
				m_DlgEffBlur.SetData( pData, (DxEffCharBlur*)pEff, bNEW );
			}break;
		case EMEFFCHAR_CLONEBLUR:
			{
				SetActivePage ( DLG_EFF_CLONEBLUR );
				m_DlgEffCloneBlur.SetData( pData, (DxEffCharCloneBlur*)pEff, bNEW );
			}break;
		case EMEFFCHAR_DUST:
			{
				SetActivePage ( DLG_EFF_DUST );
				m_DlgEffDust.SetData( pData, (DxEffCharDust*)pEff, bNEW );
			}break;
		case EMEFFCHAR_SHOCK:
			{
				SetActivePage ( DLG_EFF_SHOCK );
				m_DlgEffShock.SetData( pData, (DxEffCharShock*)pEff, bNEW );
			}break;
		case EMEFFCHAR_ATTRIBUTE:
			{
				SetActivePage ( DLG_EFF_ATTRIBUTE );
				m_DlgEffAttribute.SetData( pData, (DxEffCharAttribute*)pEff, bNEW );
			}break;
		case EMEFFCHAR_ARROW:
			{
				SetActivePage ( DLG_EFF_ARROW );
				m_DlgEffArrow.SetData( pData, (DxEffCharArrow*)pEff, bNEW );
			}break;
		case EMEFFCHAR_SPECULAR:
			{
				SetActivePage ( DLG_EFF_SPECULAR );
				m_DlgEffSpecular.SetData( pData, (DxEffCharSpecular*)pEff, bNEW );
			}break;
		case EMEFFCHAR_LEVEL:
			{
				SetActivePage ( DLG_EFF_LEVEL );
				m_DlgEffLevel.SetData( pData, (DxEffCharLevel*)pEff, bNEW );
			}break;
		case EMEFFCHAR_EMIT:
			{
				SetActivePage ( DLG_EFF_EMIT );
				m_DlgEffEmit.SetData( pData, (DxEffCharEmit*)pEff, bNEW );
			}break;
		case EMEFFCHAR_ALPHA:
			{
				SetActivePage ( DLG_EFF_ALPHA );
				m_DlgEffAlpha.SetData( pData, (DxEffCharAlpha*)pEff, bNEW );
			}break;
		case EMEFFCHAR_NEON:
			{
				SetActivePage ( DLG_EFF_NEON );
				m_DlgEffNeon.SetData( pData, (DxEffCharNeon*)pEff, bNEW );
			}break;
		case EMEFFCHAR_MARK:
			{
				SetActivePage ( DLG_EFF_MARK );
				m_DlgEffMark.SetData( pData, (DxEffCharMark*)pEff, bNEW );
			}break;
		case EMEFFCHAR_NOALPHA:
			{
				SetActivePage ( DLG_EFF_NOALPHA );
				m_DlgEffNoAlpha.SetData( pData, (DxEffCharNoAlpha*)pEff, bNEW );
			}break;
		case EMEFFCHAR_REFLECTION2:
			{
				SetActivePage ( DLG_EFF_REFLECTION2 );
				m_DlgEffReflection2.SetData( pData, (DxEffCharReflection2*)pEff, bNEW );
			}break;
		case EMEFFCHAR_AMBIENT:
			{
				SetActivePage ( DLG_EFF_AMBIENT );
				m_DlgEffAmbient.SetData( pData, (DxEffCharAmbient*)pEff, bNEW );
			}break;
		case EMEFFCHAR_DOT3:
			{
				SetActivePage ( DLG_EFF_DOT3 );
				m_DlgEffDot3.SetData( pData, (DxEffCharDot3*)pEff, bNEW );
			}break;
		case EMEFFCHAR_MULTITEX:
			{
				SetActivePage ( DLG_EFF_MULTITEX );
				m_DlgEffMultiTex.SetData( pData, (DxEffCharMultiTex*)pEff, bNEW );
			}break;
		case EMEFFCHAR_GHOSTING:
			{
				SetActivePage ( DLG_EFF_GHOSTING );
				m_DlgEffGhosting.SetData( pData, (DxEffCharGhosting*)pEff, bNEW );
			}break;
		case EMEFFCHAR_SPECULAR2:
			{
				SetActivePage ( DLG_EFF_SPECULAR2 );
				m_DlgEffSpecular2.SetData( pData, (DxEffCharSpecular2*)pEff, bNEW );
			}break;
		case EMEFFCHAR_TOON:
			{
				SetActivePage ( DLG_EFF_TOON );
				m_DlgEffToon.SetData( pData, (DxEffCharToon*)pEff, bNEW );
			}break;
		case EMEFFCHAR_TEXDIFF:
			{
				SetActivePage ( DLG_EFF_TEXDIFF );
				m_DlgEffTexDiff.SetData( pData, (DxEffCharTexDiff*)pEff, bNEW );
			}break;
		case EMEFFCHAR_PARTICLE:
			{
				SetActivePage ( DLG_EFF_PARTICLE );
				m_DlgEffParticle.SetData( pData, (DxEffCharParticle*)pEff, bNEW );
			}break;
		case EMEFFCHAR_BONEPOSEFF:
			{
				SetActivePage ( DLG_EFF_BONEPOSEFF );
				m_DlgEffBonePos.SetData( pData, (DxEffCharBonePosEff*)pEff, bNEW );
			}break;
		case EMEFFCHAR_BONELISTEFF:
			{
				SetActivePage ( DLG_EFF_BONELISTEFF );
				m_DlgEffBoneList.SetData( pData, (DxEffCharBoneListEff*)pEff, bNEW );
			}break;
		case EMEFFCHAR_USERCOLOR:
			{
				SetActivePage ( DLG_EFF_USERCOLOR );
				m_DlgEffUserColor.SetData( pData, (DxEffCharUserColor*)pEff, bNEW );
			}break;
		case EMEFFCHAR_NORMALMAP:
			{
				SetActivePage ( DLG_EFF_NORMALMAP );
				m_DlgEffNormalMap.SetData( pData, (DxEffCharNormal*)pEff, bNEW );
			}break;
		case EMEFFCHAR_LINE2BONE:
			{
				SetActivePage ( DLG_EFF_LINE2BONE );
				m_DlgEffLine2Bone.SetData( pData, (DxEffCharLine2BoneEff*)pEff, bNEW );
			}break;
		case EMEFFCHAR_AROUNDEFFECT:
			{
				SetActivePage ( DLG_EFF_AROUNDEFFECT );
				m_DlgEffAround.SetData( pData, (DxCharAroundEff*)pEff, bNEW );
			}break;
		};
	}
}

HBRUSH CToolTab::CtlColor(CDC* pDc, UINT uCtlColor) 
{
	CWnd*    pwndParent;   // Parent window to property-sheet
	CDC*     pdcParent;    // Parent dc
	COLORREF color;        // color on parent dc           

	pwndParent = GetParent();
	ASSERT( pwndParent->IsKindOf( RUNTIME_CLASS( CWnd ) ) ); 
	pdcParent = pwndParent->GetDC();
	color = pdcParent->GetBkColor();    // Get parent color
	pwndParent->ReleaseDC( pdcParent );
	CBrush brush( color );              // Make a brush

	return (HBRUSH)brush;
}

BOOL CToolTab::PreTranslateMessage(MSG* pMsg)
{
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		{
			if ( pMsg->wParam == VK_ESCAPE )
			{
			}
		}
		break;
	}
	return CPropertySheet::PreTranslateMessage(pMsg);
}

void CToolTab::ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag)
{
	CRect windowRect;
	TEXTMETRIC tmOld, tmNew;
	CDC * pDC = pWnd->GetDC();
	CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pSavedFont);
	pWnd->ReleaseDC(pDC);

	CRect clientRect, newClientRect, newWindowRect;

	pWnd->GetWindowRect(windowRect);
	pWnd->GetClientRect(clientRect);

	double fHeigth = (double)(m_Rect.bottom-m_Rect.top) / clientRect.bottom;
	double fWidth = (double) (m_Rect.right-m_Rect.left) / clientRect.right;

	if (nFlag != CDF_NONE)
	{
		long xDiff = windowRect.Width() - clientRect.Width();
		long yDiff = windowRect.Height() - clientRect.Height();
	
		newClientRect.left = newClientRect.top = 0;
		newClientRect.right = clientRect.right * (LONG)fWidth;
		newClientRect.bottom = clientRect.bottom * (LONG)fHeigth; 

		if (nFlag == CDF_TOPLEFT)
		{
			newWindowRect.left = windowRect.left;
			newWindowRect.top = windowRect.top;
			newWindowRect.right = windowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = windowRect.top + newClientRect.bottom + yDiff;
		}
		else if (nFlag == CDF_CENTER) 
		{
			newWindowRect.left = windowRect.left - (newClientRect.right - clientRect.right)/2;
			newWindowRect.top = windowRect.top -(newClientRect.bottom - clientRect.bottom)/2;
			newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
		}
		pWnd->MoveWindow(newWindowRect);
	}

	pWnd->SetFont(pFont);
	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);

	while (pChildWnd)
	{
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(windowRect);
		CString strClass;
		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
		strClass.MakeUpper();
		if(strClass==_T("COMBOBOX"))
		{
			CRect rect;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
			windowRect.right = rect.right;
			windowRect.bottom = rect.bottom;
		}
		pWnd->ScreenToClient(windowRect);
		windowRect.left = windowRect.left * (LONG)fWidth; 
		windowRect.right = windowRect.right * (LONG)fWidth; 
		windowRect.top = windowRect.top * (LONG)fHeigth; 
		windowRect.bottom = windowRect.bottom * (LONG)fHeigth; 
		pChildWnd->MoveWindow(windowRect);
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}
void CToolTab::ClearData()
{
}

