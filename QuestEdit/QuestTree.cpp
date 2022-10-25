// QuestTree.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestEdit.h"
#include "QuestTree.h"
#include "SheetWithTab.h"
#include "InputIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CQuestTree, CPropertyPage)
CQuestTree::CQuestTree( LOGFONT logfont )
	: CPropertyPage(CQuestTree::IDD),
	m_pFont ( NULL )
{
	m_bDlgInit = FALSE;
	m_pDummyQuestNode = new QUESTNODE;
	m_pDummyQuestNode->pQUEST = new GLQUEST;
	m_bDummyHasQuest = FALSE;

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CQuestTree::~CQuestTree()
{
	SAFE_DELETE ( m_pDummyQuestNode->pQUEST );
	SAFE_DELETE ( m_pDummyQuestNode );	
	SAFE_DELETE ( m_pFont );
}

void CQuestTree::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ITEM, m_ctrlQuestTree);
}


BEGIN_MESSAGE_MAP(CQuestTree, CPropertyPage)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_ITEM, OnNMRclickTreeItem)
END_MESSAGE_MAP()


BOOL CQuestTree::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_pSheetTab->ChangeDialogFont( this, m_pFont, CDF_TOPLEFT );

	m_bDlgInit = TRUE;
	UpdateTree ();

	return TRUE;  
}

void CQuestTree::OnNMRclickTreeItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	POINT OriginPoint;
	GetCursorPos ( &OriginPoint );

	ForceSelectOnRightClick ( OriginPoint );
	RunSelectMenu ( GetSelectMenu ( OriginPoint ) );
}

void CQuestTree::RunSelectMenu ( int nSelect )
{
	HTREEITEM hItem = m_ctrlQuestTree.GetSelectedItem ();

	PQUESTNODE pQuestNode = NULL;
	if ( hItem )
	{
		pQuestNode = (PQUESTNODE)m_ctrlQuestTree.GetItemData ( hItem );
	}

	switch ( nSelect )
	{
	case ID_MENUITEM_QUEST_MODIFY:
		{
			if ( GetSelectItem () )
			{				
				m_pSheetTab->ActiveQuestPage ( QUESTTREE_PAGE, pQuestNode );
			}			
		}
		break;
	case ID_MENUITEM_QUESTSTEP_MODIFY:
		{
			if ( GetSelectItem () )
			{
				m_pSheetTab->ActiveQuestStepPage ( QUESTTREE_PAGE, pQuestNode );
			}			
		}
		break;
	case ID_MENUITEM_ADD:
		{
			AddQuest ();
		}
		break;
	case ID_MENUITEM_DEL:
		{
			if ( GetSelectItem () )
			{
				if ( MessageBox ( "Do you want to delete?", "Question", MB_YESNO ) == IDYES )
				{
					DelQuest();
				}
			}
		}
		break;
	case ID_MENUITEM_COPY:
		{
			CopyQuest ();
		}
		break;
	case ID_MENUITEM_PASTE_NEW:
		{
			PasteQuestNew ();
		}
		break;
	case ID_MENUITEM_PASTE_VALUE:
		{
			if ( MessageBox ( "Do you want to overwrite?", "Question", MB_YESNO ) == IDYES )
			{
				PasteQuestValue ();
			}	
		}
		break;
	};
}

int	 CQuestTree::GetSelectMenu ( POINT pt )
{
	int pMenuID = 0;
	CMenu Menu;

	int SelectionMade = -1;

	VERIFY ( Menu.LoadMenu ( IDR_MENU_QUEST_POPUP ) );

	CMenu* pPopup = Menu.GetSubMenu ( pMenuID );
	ASSERT ( pPopup != NULL );	

	SetReadMode( pPopup );

	SelectionMade = pPopup->TrackPopupMenu (
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
		pt.x, pt.y, this );

	pPopup->DestroyMenu ();

	return SelectionMade;
}

BOOL	CQuestTree::UpdateTree ()
{	
	if ( !m_bDlgInit )
	{
		return FALSE;
	}

	CleanAllQuest ();

	GLQuestMan::MAPQUEST& m_mapQuestMap = GLQuestMan::GetInstance().GetMap();

	GLQuestMan::MAPQUEST_ITER iter_pos;
	DWORD dwQuestID = 0;
	PQUESTNODE pQuestNode = NULL;
	CString strItem;

	for ( iter_pos = m_mapQuestMap.begin(); iter_pos != m_mapQuestMap.end(); iter_pos++ )
	{
		HTREEITEM hMainItem= NULL;
		
		dwQuestID = (DWORD) iter_pos->first;	
		pQuestNode = (PQUESTNODE) &iter_pos->second;

        if ( pQuestNode ) 
		{	
			strItem = GetFormatName ( pQuestNode );
			hMainItem = m_ctrlQuestTree.InsertItem ( strItem, m_TreeRoot );
			m_ctrlQuestTree.SetItemData ( hMainItem, (DWORD_PTR)pQuestNode );
		}
	}

	return TRUE;

}

BOOL	CQuestTree::ConvertToAll()
{	
	if ( !m_bDlgInit )
	{
		return FALSE;
	}

	CleanAllQuest ();

	GLQuestMan::MAPQUEST& m_mapQuestMap = GLQuestMan::GetInstance().GetMap();

	GLQuestMan::MAPQUEST_ITER iter_pos;
	DWORD dwQuestID = 0;
	PQUESTNODE pQuestNode = NULL;
	CString strItem;

	DWORD dwNum;
	dwNum =m_mapQuestMap.size();
	strItem.Format ("Will Convert all %d Quest Files",dwNum);

	if ( MessageBox (strItem,"Question",MB_YESNO)  == IDYES )
	{
		for ( iter_pos = m_mapQuestMap.begin(); iter_pos != m_mapQuestMap.end(); iter_pos++ )
		{
			dwQuestID = (DWORD) iter_pos->first;	
			pQuestNode = (PQUESTNODE) &iter_pos->second;

			if ( pQuestNode ) 
			{
				pQuestNode->pQUEST->m_sSTARTOPT.dwCLASS = GLCC_ALL_2012;
				iter_pos->second.pQUEST->SAVE( iter_pos->second.strFILE.c_str() );
			}
		}
		UpdateTree();
	}
	return TRUE;
}

void CQuestTree::CleanAllQuest ()
{
	if ( !m_bDlgInit )
	{
		return ;
	}

	m_ctrlQuestTree.DeleteAllItems ();
	m_TreeRoot = m_ctrlQuestTree.InsertItem ( "Root Node" );
}

BOOL	CQuestTree::CopyQuest ()
{
	HTREEITEM hMainItem = m_ctrlQuestTree.GetSelectedItem ();
	if ( hMainItem && hMainItem != m_TreeRoot )
	{
		PQUESTNODE pQuestNode = (PQUESTNODE)m_ctrlQuestTree.GetItemData ( hMainItem );
		*(m_pDummyQuestNode->pQUEST) = *(pQuestNode->pQUEST);
		m_pDummyQuestNode->strFILE = pQuestNode->strFILE;
		m_bDummyHasQuest = TRUE;
		return TRUE;
	}
	return FALSE;
}

void CQuestTree::SetSelectItem ( PQUESTNODE pQuestNode )
{
	m_pQuestNode = pQuestNode;
}

PQUESTNODE CQuestTree::GetSelectItem ()
{
	return m_pQuestNode;
}

BOOL CQuestTree::PreTranslateMessage(MSG* pMsg)
{
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		{
			if ( pMsg->wParam == VK_RETURN )
			{
				HTREEITEM hItem = m_ctrlQuestTree.GetSelectedItem ();
				if ( !hItem ) return TRUE;

				PQUESTNODE pQuestNode = (PQUESTNODE)m_ctrlQuestTree.GetItemData ( hItem );
				if ( !pQuestNode )
				{
					return TRUE;
				}

				m_pSheetTab->ActiveQuestPage ( QUEST_PAGE, pQuestNode );
			}

			if ( ::GetKeyState ( VK_CONTROL ) < 0 )
			{
				if ( pMsg->wParam == 'C' )
				{
					RunSelectMenu ( ID_MENUITEM_COPY );
				}
			}

			if ( ::GetKeyState ( VK_CONTROL ) < 0 )
			{
				if ( pMsg->wParam == 'V' )
				{
					HTREEITEM hItem = m_ctrlQuestTree.GetSelectedItem ();
					PQUESTNODE pQuestNode = (PQUESTNODE)m_ctrlQuestTree.GetItemData ( hItem );
					SetSelectItem ( pQuestNode );

					RunSelectMenu ( ID_MENUITEM_PASTE_NEW );
				}
			}

			if ( ::GetKeyState ( VK_CONTROL ) < 0 )
			{
				if ( pMsg->wParam == 'B' )
				{
					HTREEITEM hItem = m_ctrlQuestTree.GetSelectedItem ();
					PQUESTNODE pQuestNode = (PQUESTNODE)m_ctrlQuestTree.GetItemData ( hItem );
					SetSelectItem ( pQuestNode );

					RunSelectMenu ( ID_MENUITEM_PASTE_VALUE );
				}
			}
		}
		break;
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CQuestTree::SetReadMode( CMenu* pMenu )
{
}


