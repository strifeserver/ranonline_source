#include "stdafx.h"
#include <vector>
#include "resource.h"
#include "RanManagerCharTab.h"
#include "RanManagerComments.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CRanManagerCharTab, CPropertySheet)

CRanManagerCharTab::CRanManagerCharTab(CRect& rect, LOGFONT logfont, CWnd* pParentWnd)
	 : CPropertySheet("jdev", pParentWnd),
	 m_bNoTabs(FALSE),
	 m_pWndParent(NULL),
	 m_iDistanceFromTop(0),

	 m_CharDefault( logfont ),
	 m_CharEdit( logfont ),
	 m_CharPutOn( logfont ),
	 m_CharInven( logfont ),
	 m_CharSkill( logfont ),
	 m_CharQuest( logfont ),
	 m_CharVehicle( logfont ),
	 m_CharPet( logfont ),
	 m_CharSkillSlot( logfont ),
	 m_CharQuickSlot( logfont ),
	 m_CharCoolDown( logfont ),

	 m_pFont ( NULL )
{
	m_pWndParent = pParentWnd;
	m_Rect = rect;

	m_bNoTabs = TRUE;
	m_bStacked = FALSE;

	m_CharDefault.SetToolTab ( this );
	AddPage ( &m_CharDefault );
	m_CharEdit.SetToolTab ( this );
	AddPage ( &m_CharEdit );
	m_CharPutOn.SetToolTab ( this );
	AddPage ( &m_CharPutOn );
	m_CharInven.SetToolTab ( this );
	AddPage ( &m_CharInven );
	m_CharSkill.SetToolTab ( this );
	AddPage ( &m_CharSkill );
	m_CharQuest.SetToolTab ( this );
	AddPage ( &m_CharQuest );
	m_CharVehicle.SetToolTab ( this );
	AddPage ( &m_CharVehicle );
	m_CharPet.SetToolTab ( this );
	AddPage ( &m_CharPet );
	m_CharSkillSlot.SetToolTab ( this );
	AddPage ( &m_CharSkillSlot );
	m_CharQuickSlot.SetToolTab ( this );
	AddPage ( &m_CharQuickSlot );
	m_CharCoolDown.SetToolTab ( this );
	AddPage ( &m_CharCoolDown );



	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CRanManagerCharTab::~CRanManagerCharTab()
{
	SAFE_DELETE( m_pFont );
}

static int module_piButtons[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };

BOOL CRanManagerCharTab::OnInitDialog() 
{
	HWND hTabWnd;
	CRect rectTabCtrl;
	ChangeDialogFont( this, m_pFont, BCDF_NONE );

	if( m_Rect.right )	{
		hTabWnd = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL);
		ASSERT(hTabWnd != NULL);
		::GetWindowRect(hTabWnd, &rectTabCtrl); 
		ScreenToClient(rectTabCtrl);

		if( !m_bNoTabs && rectTabCtrl.right > (m_Rect.Width() - 3) )	{
			rectTabCtrl.right = m_Rect.Width() - 3;	}
		rectTabCtrl.bottom = 32; 
		::MoveWindow( hTabWnd, 0, m_iDistanceFromTop, rectTabCtrl.Width(), rectTabCtrl.bottom, TRUE );
		BOOL bResult = (BOOL)Default(); 

		if( m_bNoTabs )	{
			::ShowWindow(hTabWnd, SW_HIDE);
			::EnableWindow(hTabWnd, FALSE);	}

		if (!m_bStacked)	{
			hTabWnd = (HWND)SendMessage(PSM_GETTABCONTROL);
			if (hTabWnd != NULL)	{
				CWnd::ModifyStyle(hTabWnd, TCS_MULTILINE, TCS_SINGLELINE, 0);	}	}

		for (int i = 0; i < 4; i++)	{
			HWND hWnd = ::GetDlgItem(m_hWnd, module_piButtons[i]);
			if (hWnd != NULL)	{
				::ShowWindow(hWnd, SW_HIDE);
				::EnableWindow(hWnd, FALSE);	}	}

		MoveWindow( m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom /*- cyDiff*/ );
		return bResult;
	}	return CPropertySheet::OnInitDialog();
}

BEGIN_MESSAGE_MAP(CRanManagerCharTab, CPropertySheet)
	//{{AFX_MSG_MAP(CRanManagerCharTab)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CRanManagerCharTab::ActivePage(int nPage ,int nChar )
{
	switch (nPage)
	{
	case CHARPAGE_MAIN:	
		{
			SetActivePage ( CHARPAGE_MAIN );
		}break;
	case CHARPAGE_EDIT:	
		{
			SetActivePage ( CHARPAGE_EDIT );
			m_CharEdit.SetChar(nChar);
		}break;
	case CHARPAGE_PUTON:	
		{
			SetActivePage ( CHARPAGE_PUTON );
			m_CharPutOn.SetChar(nChar);
		}break;
	case CHARPAGE_INVEN:	
		{
			SetActivePage ( CHARPAGE_INVEN );
			m_CharInven.SetChar(nChar);
		}break;
	case CHARPAGE_SKILL:	
		{
			SetActivePage ( CHARPAGE_SKILL );
			m_CharSkill.SetChar(nChar);
		}break;
	case CHARPAGE_QUEST:	
		{
			SetActivePage ( CHARPAGE_QUEST );
			m_CharQuest.SetChar(nChar);
		}break;
	case CHARPAGE_VEHICLE:	
		{
			SetActivePage ( CHARPAGE_VEHICLE );
			m_CharVehicle.SetChar(nChar);
		}break;
	case CHARPAGE_PET:	
		{
			SetActivePage ( CHARPAGE_PET );
			m_CharPet.SetChar(nChar);
		}break;
	case CHARPAGE_SKILLSLOT:	
		{
			SetActivePage ( CHARPAGE_SKILLSLOT );
			m_CharSkillSlot.SetChar(nChar);
		}break;
	case CHARPAGE_QUICKSLOT:	
		{
			SetActivePage ( CHARPAGE_QUICKSLOT );
			m_CharQuickSlot.SetChar(nChar);
		}break;
	case CHARPAGE_COOLDOWN:	
		{
			SetActivePage ( CHARPAGE_COOLDOWN );
			m_CharCoolDown.SetChar(nChar);
		}break;

	};
}

HBRUSH CRanManagerCharTab::CtlColor(CDC* pDc, UINT uCtlColor) 
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

BOOL CRanManagerCharTab::PreTranslateMessage(MSG* pMsg)
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

void CRanManagerCharTab::ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag)
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

	if (nFlag != BCDF_NONE)	{
		long xDiff = windowRect.Width() - clientRect.Width();
		long yDiff = windowRect.Height() - clientRect.Height();
	
		newClientRect.left = newClientRect.top = 0;
		newClientRect.right = clientRect.right * (LONG)fWidth;
		newClientRect.bottom = clientRect.bottom * (LONG)fHeigth; 

		if (nFlag == BCDF_TOPLEFT)	{
			newWindowRect.left = windowRect.left;
			newWindowRect.top = windowRect.top;
			newWindowRect.right = windowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = windowRect.top + newClientRect.bottom + yDiff;	}
		else if (nFlag == BCDF_CENTER) {
			newWindowRect.left = windowRect.left - (newClientRect.right - clientRect.right)/2;
			newWindowRect.top = windowRect.top -(newClientRect.bottom - clientRect.bottom)/2;
			newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;	}
		pWnd->MoveWindow(newWindowRect);	}

	pWnd->SetFont(pFont);
	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);

	while (pChildWnd)	{
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(windowRect);
		CString strClass;
		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
		strClass.MakeUpper();
		if(strClass==_T("COMBOBOX"))	{
			CRect rect;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
			windowRect.right = rect.right;
			windowRect.bottom = rect.bottom;	}
		pWnd->ScreenToClient(windowRect);
		windowRect.left = windowRect.left * (LONG)fWidth; 
		windowRect.right = windowRect.right * (LONG)fWidth; 
		windowRect.top = windowRect.top * (LONG)fHeigth; 
		windowRect.bottom = windowRect.bottom * (LONG)fHeigth; 
		pChildWnd->MoveWindow(windowRect);
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);	}
}
void CRanManagerCharTab::ClearData()
{
}

