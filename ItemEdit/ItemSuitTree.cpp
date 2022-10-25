// AllItemView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include <vector>
#include "ItemEdit.h"
#include "SheetWithTab.h"
#include "ItemSuitTree.h"
#include ".\itemsuittree.h"
#include "GLStringTable.h"

// CItemSuitTree 대화 상자입니다.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CItemSuitTree, CPropertyPage)
CItemSuitTree::CItemSuitTree( LOGFONT logfont )
	: CPropertyPage(CItemSuitTree::IDD),
	m_pFont( NULL )
{
	m_bDlgInit = FALSE;
	m_pDummyItemNode = new CItemNode;	
	m_bDummyHasItem = FALSE;

	
	
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CItemSuitTree::~CItemSuitTree()
{
	SAFE_DELETE ( m_pDummyItemNode );
	SAFE_DELETE ( m_pFont );
}

void CItemSuitTree::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ITEM, m_ctrlItemTree);
}


BEGIN_MESSAGE_MAP(CItemSuitTree, CPropertyPage)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_ITEM, OnNMRclickTreeItem)
	ON_NOTIFY(NM_CLICK, IDC_TREE_ITEM, OnNMClickTreeItem)
END_MESSAGE_MAP()


// CItemSuitTree 메시지 처리기입니다.

BOOL CItemSuitTree::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_pSheetTab->ChangeDialogFont( this, m_pFont, CDF_TOPLEFT );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_bDlgInit = TRUE;
	UpdateTree ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CItemSuitTree::OnNMRclickTreeItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;	

	POINT OriginPoint;
	GetCursorPos ( &OriginPoint );

	//	우클릭 강제 설정
	ForceSelectOnRightClick ( OriginPoint );

	//	메뉴 선택 & 선택 메뉴 실행
	RunSelectMenu ( GetSelectMenu ( OriginPoint ) );
}

void CItemSuitTree::RunSelectMenu ( int nSelect )
{
	HTREEITEM hItem = m_ctrlItemTree.GetSelectedItem ();

	PITEMNODE pItemNode = NULL;
	if ( hItem )
	{
		pItemNode = (PITEMNODE)m_ctrlItemTree.GetItemData ( hItem );
	}

	switch ( nSelect )
	{
	case ID_MENUITEM_PARENTMODIFY:
		{
			if ( GetSelectItem () )
			{				
				m_pSheetTab->ActiveItemParentPage ( ITEMSUITTREEPAGE, &pItemNode->m_sItem );
			}			
		}
		break;
	case ID_MENUITEM_SUITMODIFY:
		{
			if ( GetSelectItem () )
			{
				m_pSheetTab->ActiveItemSuitPage ( ITEMSUITTREEPAGE, &pItemNode->m_sItem );
			}			
		}
		break;
	case ID_MENUITEM_ADD:
		{
			AddItem ();
		}
		break;
	case ID_MENUITEM_DEL:
		{
			if ( GetSelectItem () )
			{

				CString strTemp;
				strTemp.Format ( "Do you want to delete? [%03d/%03d] %s"
					,GetSelectItem ()->m_sItem.sBasicOp.sNativeID.wMainID
					,GetSelectItem ()->m_sItem.sBasicOp.sNativeID.wSubID
					,GetSelectItem ()->m_sItem.GetName() );

				if ( MessageBox ( strTemp, "Question", MB_YESNO ) == IDYES )
				{
					DelItem();
				}
			}
		}
		break;
	case ID_MENUITEM_COPY:
		{
			CopyItem ();
		}
		break;
	case ID_MENUITEM_PASTE_NEW:
		{
			PasteItemNew ();
		}
		break;
	case ID_MENUITEM_PASTE_VALUE:
		{
			if ( GetSelectItem () )
			{

				CString strTemp;
				strTemp.Format ( "Do you want to replace? [%03d/%03d] %s"
					,GetSelectItem ()->m_sItem.sBasicOp.sNativeID.wMainID
					,GetSelectItem ()->m_sItem.sBasicOp.sNativeID.wSubID
					,GetSelectItem ()->m_sItem.GetName() );

				if ( MessageBox ( strTemp, "Question", MB_YESNO ) == IDYES )
				{
					PasteItemValue ();
				}
			}
		}
		break;
	};
}

int	 CItemSuitTree::GetSelectMenu ( POINT pt )
{
	//	<--	실제 작동 메뉴 따기
	int pMenuID = 0;
	CMenu Menu;

	int SelectionMade = -1;
	//switch ( m_SelectMenu )
	//{
	//case 0:
	//	{
		VERIFY ( Menu.LoadMenu ( IDR_MENU_ITEMSUIT_POPUP ) );

		CMenu* pPopup = Menu.GetSubMenu ( pMenuID );
		ASSERT ( pPopup != NULL );	

		SetReadMode( pPopup );

		SelectionMade = pPopup->TrackPopupMenu (
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
			pt.x, pt.y, this );

		pPopup->DestroyMenu ();
	//	}
	//	break;
	//}
	//	-->	

	return SelectionMade;
}

BOOL	CItemSuitTree::UpdateTree ()
{	
	if ( !m_bDlgInit )
	{
		return FALSE;
	}

	CleanAllItem ();

	for ( int MID = 0; MID < GLItemMan::MAX_MID; MID++ )
	{
		HTREEITEM hMainItem= NULL;
		HTREEITEM hSubItem = NULL;

		for ( int SID = 0; SID < GLItemMan::MAX_SID; SID++ )
		{        		
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( MID, SID );

			if ( pItem )
			{
				CString strItem = GetFormatName ( pItem );
				if ( !hMainItem )
				{
					hMainItem = m_ctrlItemTree.InsertItem ( strItem, m_TreeRoot );
					m_ctrlItemTree.SetItemData ( hMainItem, (DWORD_PTR)pItem );

					m_TreeItemList[MID][SID] = hMainItem;
				}
				else
				{
					hSubItem = m_ctrlItemTree.InsertItem ( strItem, hMainItem );
					m_ctrlItemTree.SetItemData ( hSubItem, (DWORD_PTR)pItem );

					m_TreeItemList[MID][SID] = hSubItem;
				}				
			}
		}
	}

	return TRUE;
}

void CItemSuitTree::CleanAllItem ()
{
	if ( !m_bDlgInit )
	{
		return ;
	}

	m_ctrlItemTree.DeleteAllItems ();
	m_TreeRoot = m_ctrlItemTree.InsertItem ( "Root Node" );
}

BOOL	CItemSuitTree::CopyItem ()
{
	HTREEITEM hMainItem = m_ctrlItemTree.GetSelectedItem ();
	if ( hMainItem && hMainItem != m_TreeRoot )
	{
		*m_pDummyItemNode = *((PITEMNODE)m_ctrlItemTree.GetItemData ( hMainItem ));
		m_bDummyHasItem = TRUE;
		strTempName = GetSelectItem()->m_sItem.GetName();
		strTempDesc = GetSelectItem()->m_sItem.GetComment();
		return TRUE;
	}

	return FALSE;
}

BOOL	CItemSuitTree::PasteItemNew ()
{
	if ( !m_bDummyHasItem )
	{
		return FALSE;
	}

	HTREEITEM hMainItem = m_ctrlItemTree.GetSelectedItem ();
	if ( hMainItem && GetSelectItem () )	//	서브로 추가시
	{
		WORD MID = GetSelectItem()->m_sItem.sBasicOp.sNativeID.wMainID;
		WORD SID = GLItemMan::GetInstance().FindFreeSID ( MID );
		if ( SID == ITEMID_NOTFOUND )
		{
			return FALSE;
		}

		CItemNode sItemNode;
		sItemNode = *m_pDummyItemNode;
		sItemNode.m_sItem.sBasicOp.sNativeID.wMainID= MID;
		sItemNode.m_sItem.sBasicOp.sNativeID.wSubID = SID;

		CString strTemp;
		strTemp.Format( "IN_%03d_%03d" , MID , SID);
		sItemNode.m_sItem.sBasicOp.strName = strTemp.GetString();
		strTemp.Format( "ID_%03d_%03d" , MID , SID);
		sItemNode.m_sItem.sBasicOp.strComment = strTemp.GetString();

		GLItemMan::GetInstance().InsertItem ( MID, SID, &sItemNode );

		strTempNameKey = sItemNode.m_sItem.sBasicOp.strName;
		strTempDescKey = sItemNode.m_sItem.sBasicOp.strComment;
		GLStringTable::GetInstance().DeleteString ( strTempNameKey, strTempDescKey, GLStringTable::ITEM );
		GLStringTable::GetInstance().InsertString ( strTempNameKey, strTempName, GLStringTable::ITEM );
		GLStringTable::GetInstance().InsertString ( strTempDescKey, strTempDesc, GLStringTable::ITEM );

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( MID, SID );

		HTREEITEM hParentItem = m_ctrlItemTree.GetParentItem ( hMainItem );
		if ( hParentItem != m_TreeRoot )
		{
			UpdateItem (pITEM, hParentItem );
		}
		else
		{
			UpdateItem ( pITEM, hMainItem );
		}

		return TRUE;
	}
	else	//	메인으로 추가시
	{
		WORD MID = GLItemMan::GetInstance().FindFreeMID ();
		if ( MID == ITEMID_NOTFOUND )
		{
			return FALSE;
		}
		WORD SID = GLItemMan::GetInstance().FindFreeSID ( MID );
		if ( SID == ITEMID_NOTFOUND )
		{
			return FALSE;
		}
		
		CItemNode sItemNode;
		sItemNode = *m_pDummyItemNode;
		sItemNode.m_sItem.sBasicOp.sNativeID.wMainID = MID;
		sItemNode.m_sItem.sBasicOp.sNativeID.wSubID = SID;

		CString strTemp;
		strTemp.Format( "IN_%03d_%03d" , MID , SID);
		sItemNode.m_sItem.sBasicOp.strName = strTemp.GetString();
		strTemp.Format( "ID_%03d_%03d" , MID , SID);
		sItemNode.m_sItem.sBasicOp.strComment = strTemp.GetString();

		GLItemMan::GetInstance().InsertItem ( MID, SID, &sItemNode );

		strTempNameKey = sItemNode.m_sItem.sBasicOp.strName;
		strTempDescKey = sItemNode.m_sItem.sBasicOp.strComment;
		GLStringTable::GetInstance().DeleteString ( strTempNameKey, strTempDescKey, GLStringTable::ITEM );
		GLStringTable::GetInstance().InsertString ( strTempNameKey, strTempName, GLStringTable::ITEM );
		GLStringTable::GetInstance().InsertString ( strTempDescKey, strTempDesc, GLStringTable::ITEM );


		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( MID, SID );

		UpdateItem ( pITEM, NULL );
		
		return TRUE;
	}
}

BOOL	CItemSuitTree::PasteItemValue ()
{
	if ( !m_bDummyHasItem )
	{
		return FALSE;
	}

	HTREEITEM hMainItem = m_ctrlItemTree.GetSelectedItem ();
	if ( hMainItem && GetSelectItem () )
	{		
		WORD MID = GetSelectItem()->m_sItem.sBasicOp.sNativeID.wMainID;
		WORD SID = GetSelectItem()->m_sItem.sBasicOp.sNativeID.wSubID;

		*(GetSelectItem()) = *m_pDummyItemNode;

		GetSelectItem()->m_sItem.sBasicOp.sNativeID.wMainID= MID;
		GetSelectItem()->m_sItem.sBasicOp.sNativeID.wSubID = SID;

		CString strTemp;
		strTemp.Format( "IN_%03d_%03d" , MID , SID);
		GetSelectItem()->m_sItem.sBasicOp.strName = strTemp.GetString();
		strTemp.Format( "ID_%03d_%03d" , MID , SID);
		GetSelectItem()->m_sItem.sBasicOp.strComment = strTemp.GetString();


		

		strTempNameKey = GetSelectItem()->m_sItem.sBasicOp.strName;
		strTempDescKey = GetSelectItem()->m_sItem.sBasicOp.strComment;
		GLStringTable::GetInstance().DeleteString ( strTempNameKey, strTempDescKey, GLStringTable::ITEM );
		GLStringTable::GetInstance().InsertString ( strTempNameKey, strTempName, GLStringTable::ITEM );
		GLStringTable::GetInstance().InsertString ( strTempDescKey, strTempDesc, GLStringTable::ITEM );

		m_ctrlItemTree.SetItemText ( hMainItem, GetFormatName ( &GetSelectItem()->m_sItem ) );

		
		return TRUE;
	}

	return FALSE;
}

BOOL CItemSuitTree::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		{
			if ( pMsg->wParam == VK_RETURN )
			{
				HTREEITEM hItem = m_ctrlItemTree.GetSelectedItem ();
				if ( !hItem ) return TRUE;

				PITEMNODE pItemNode = (PITEMNODE)m_ctrlItemTree.GetItemData ( hItem );
				if ( !pItemNode )
				{
					return TRUE;
				}

				m_pSheetTab->ActiveItemParentPage ( ITEMSUITTREEPAGE, &pItemNode->m_sItem );
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
					HTREEITEM hItem = m_ctrlItemTree.GetSelectedItem ();
					PITEMNODE pItemNode = (PITEMNODE)m_ctrlItemTree.GetItemData ( hItem );
					SetSelectItem ( pItemNode );

					RunSelectMenu ( ID_MENUITEM_PASTE_NEW );
				}
			}

			if ( ::GetKeyState ( VK_CONTROL ) < 0 )
			{
				if ( pMsg->wParam == 'B' )
				{
					HTREEITEM hItem = m_ctrlItemTree.GetSelectedItem ();
					PITEMNODE pItemNode = (PITEMNODE)m_ctrlItemTree.GetItemData ( hItem );
					SetSelectItem ( pItemNode );

					RunSelectMenu ( ID_MENUITEM_PASTE_VALUE );
				}
			}

		}
		break;
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CItemSuitTree::SetSelectItem ( PITEMNODE pItemNode )
{
	m_pItemNode = pItemNode;
}

PITEMNODE CItemSuitTree::GetSelectItem ()
{
	return m_pItemNode;
}
void CItemSuitTree::OnNMClickTreeItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CItemSuitTree::SetReadMode( CMenu* pMenu )
{
}

void CItemSuitTree::JDev1()
{
	CString strTemp;
	strTemp.Format ("%s\n%s\n%s\n%s"
		,"Gunner/Assasin M will be added to (Brawler,Swordsman,Archer,Shamman,Extreme) M"
		,"Gunner/Assasin W will be added to (Brawler,Swordsman,Archer,Shamman,Extreme) W"
		,"both Gunner/Assasin M/W will be added to (Brawler,Swordsman,Archer,Shamman,Extreme) M/W"
		,"Continue?" );
	if ( MessageBox ( strTemp, "Question", MB_YESNO ) == IDYES )
	{
		CleanAllItem ();

		for ( int MID = 0; MID < GLItemMan::MAX_MID; MID++ )
		{
			HTREEITEM hMainItem= NULL;
			HTREEITEM hSubItem = NULL;

			for ( int SID = 0; SID < GLItemMan::MAX_SID; SID++ )
			{        		
				SITEM* pItem = GLItemMan::GetInstance().GetItem ( MID, SID );

				if ( pItem )
				{
					if ( pItem->sBasicOp.dwReqCharClass ==  787 ) pItem->sBasicOp.dwReqCharClass = 22291;
					if ( pItem->sBasicOp.dwReqCharClass ==  236 ) pItem->sBasicOp.dwReqCharClass = 43244;
					if ( pItem->sBasicOp.dwReqCharClass ==  1023 ) pItem->sBasicOp.dwReqCharClass = 65535;

				}
			}
		}
		CopyCPS();
		UpdateTree ();
	}	
}

void CItemSuitTree::JDev2()
{

	CString strTemp;
	strTemp.Format ("%s\n%s\n%s"
		,"Gunner M will be added to each(Brawler,Swordsman,Archer,Shamman) M)"
		,"Gunner W will be added to each(Brawler,Swordsman,Archer,Shamman) W)"
		,"Continue?" );
	if ( MessageBox ( strTemp, "Question", MB_YESNO ) == IDYES )
	{
		CleanAllItem ();
		for ( int MID = 0; MID < GLItemMan::MAX_MID; MID++ )
		{
			HTREEITEM hMainItem= NULL;
			HTREEITEM hSubItem = NULL;

			for ( int SID = 0; SID < GLItemMan::MAX_SID; SID++ )
			{        		
				SITEM* pItem = GLItemMan::GetInstance().GetItem ( MID, SID );

				if ( pItem )
				{
					if ( pItem->sBasicOp.dwReqCharClass ==  65 ) pItem->sBasicOp.dwReqCharClass = 3137;
					if ( pItem->sBasicOp.dwReqCharClass ==  130 ) pItem->sBasicOp.dwReqCharClass = 3202;
					if ( pItem->sBasicOp.dwReqCharClass ==  260 ) pItem->sBasicOp.dwReqCharClass = 3332;
					if ( pItem->sBasicOp.dwReqCharClass ==  520 ) pItem->sBasicOp.dwReqCharClass = 3592;
				}

			}
		}
		CopyCPS();
		UpdateTree ();
	}	
}
// fix class number by CN Dev
void CItemSuitTree::JDev3()
{
	CString strTemp;
	strTemp.Format ("%s\n%s\n%s\n%s"
		,"Assasin M will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner) M"
		,"Assasin W will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner) W"
		,"both Assasin M/W will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner) M/W"
		,"Continue?" );
	if ( MessageBox ( strTemp, "Question", MB_YESNO ) == IDYES )
	{
		CleanAllItem ();

		for ( int MID = 0; MID < GLItemMan::MAX_MID; MID++ )
		{
			HTREEITEM hMainItem= NULL;
			HTREEITEM hSubItem = NULL;

			for ( int SID = 0; SID < GLItemMan::MAX_SID; SID++ )
			{        		
				SITEM* pItem = GLItemMan::GetInstance().GetItem ( MID, SID );

				if ( pItem )
				{
					if ( pItem->sBasicOp.dwReqCharClass ==  1811 ) pItem->sBasicOp.dwReqCharClass = 5907;
					if ( pItem->sBasicOp.dwReqCharClass ==  2284 ) pItem->sBasicOp.dwReqCharClass = 10476;
					if ( pItem->sBasicOp.dwReqCharClass ==  4095 ) pItem->sBasicOp.dwReqCharClass = 16383;
				}
			}
		}
		CopyCPS();
		UpdateTree ();
	}	
}
// add convert to all class by CN Dev
void CItemSuitTree::JDev4()
{
	CString strTemp;
	strTemp.Format ("%s\n%s\n%s\n%s"
		,"Magician M will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner,Assassin) M"
		,"Magician W will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner,Assassin) W"
		,"both Magician M/W will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner,Assassin) M/W"
		,"Continue?" );
	if ( MessageBox ( strTemp, "Question", MB_YESNO ) == IDYES )
	{
		CleanAllItem ();

		for ( int MID = 0; MID < GLItemMan::MAX_MID; MID++ )
		{
			HTREEITEM hMainItem= NULL;
			HTREEITEM hSubItem = NULL;

			for ( int SID = 0; SID < GLItemMan::MAX_SID; SID++ )
			{        		
				SITEM* pItem = GLItemMan::GetInstance().GetItem ( MID, SID );

				if ( pItem )
				{
					if ( pItem->sBasicOp.dwReqCharClass ==  5907 ) pItem->sBasicOp.dwReqCharClass = 22291;
					if ( pItem->sBasicOp.dwReqCharClass ==  10476 ) pItem->sBasicOp.dwReqCharClass = 43244;
					if ( pItem->sBasicOp.dwReqCharClass ==  16383 ) pItem->sBasicOp.dwReqCharClass = 65535;
				}
			}
		}
		CopyCPS();
		UpdateTree ();
	}	
}
// SHAPER convertion - Eduj
void CItemSuitTree::JDev5()
{
	CString strTemp;
	strTemp.Format ("%s\n%s\n%s\n%s"
		,"Shaper M will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner,Assassin,Magician) M"
		,"Shaper W will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner,Assassin,Magician) W"
		,"both Shaper M/W will be added to (Brawler,Swordsman,Archer,Shamman,Extreme,Gunner,Assassin,Magician) M/W"
		,"Continue?" );
	if ( MessageBox ( strTemp, "Question", MB_YESNO ) == IDYES )
	{
		CleanAllItem ();

		for ( int MID = 0; MID < GLItemMan::MAX_MID; MID++ )
		{
			HTREEITEM hMainItem= NULL;
			HTREEITEM hSubItem = NULL;

			for ( int SID = 0; SID < GLItemMan::MAX_SID; SID++ )
			{        		
				SITEM* pItem = GLItemMan::GetInstance().GetItem ( MID, SID );

				if ( pItem )
				{
					if ( pItem->sBasicOp.dwReqCharClass ==  22291 ) pItem->sBasicOp.dwReqCharClass = 87827;
					if ( pItem->sBasicOp.dwReqCharClass ==  43244 ) pItem->sBasicOp.dwReqCharClass = 174316;
					if ( pItem->sBasicOp.dwReqCharClass ==  65535 ) pItem->sBasicOp.dwReqCharClass = 262143;
;
				}
			}
		}
		CopyCPS();
		UpdateTree ();
	}	
}
void CItemSuitTree::CopyCPS()
{
	for ( int MID = 0; MID < GLItemMan::MAX_MID; MID++ )
	{
		for ( int SID = 0; SID < GLItemMan::MAX_SID; SID++ )
		{        		
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( MID, SID );
			if ( pItem )
			{
				if ( !pItem->sBasicOp.strWearingFile[0].empty() )
				{
					pItem->sBasicOp.strWearingFile[1] = pItem->sBasicOp.strWearingFile[0].c_str();
					pItem->sBasicOp.strWearingFile[4] = pItem->sBasicOp.strWearingFile[0].c_str();
					pItem->sBasicOp.strWearingFile[8] = pItem->sBasicOp.strWearingFile[0].c_str();
					pItem->sBasicOp.strWearingFile[9] = pItem->sBasicOp.strWearingFile[0].c_str();
					pItem->sBasicOp.strWearingFile[10] = pItem->sBasicOp.strWearingFile[0].c_str();
					pItem->sBasicOp.strWearingFile[12] = pItem->sBasicOp.strWearingFile[0].c_str();
					pItem->sBasicOp.strWearingFile[14] = pItem->sBasicOp.strWearingFile[0].c_str();
					pItem->sBasicOp.strWearingFile[16] = pItem->sBasicOp.strWearingFile[0].c_str();
				}
				else if ( !pItem->sBasicOp.strWearingFile[1].empty() )
				{
					pItem->sBasicOp.strWearingFile[0] = pItem->sBasicOp.strWearingFile[1].c_str();
					pItem->sBasicOp.strWearingFile[4] = pItem->sBasicOp.strWearingFile[1].c_str();
					pItem->sBasicOp.strWearingFile[8] = pItem->sBasicOp.strWearingFile[1].c_str();
					pItem->sBasicOp.strWearingFile[9] = pItem->sBasicOp.strWearingFile[1].c_str();
					pItem->sBasicOp.strWearingFile[10] = pItem->sBasicOp.strWearingFile[1].c_str();
					pItem->sBasicOp.strWearingFile[12] = pItem->sBasicOp.strWearingFile[1].c_str();
					pItem->sBasicOp.strWearingFile[14] = pItem->sBasicOp.strWearingFile[1].c_str();
					pItem->sBasicOp.strWearingFile[16] = pItem->sBasicOp.strWearingFile[1].c_str();
				}
				else if ( !pItem->sBasicOp.strWearingFile[4].empty() )
				{
					pItem->sBasicOp.strWearingFile[0] = pItem->sBasicOp.strWearingFile[4].c_str();
					pItem->sBasicOp.strWearingFile[1] = pItem->sBasicOp.strWearingFile[4].c_str();
					pItem->sBasicOp.strWearingFile[8] = pItem->sBasicOp.strWearingFile[4].c_str();
					pItem->sBasicOp.strWearingFile[9] = pItem->sBasicOp.strWearingFile[4].c_str();
					pItem->sBasicOp.strWearingFile[10] = pItem->sBasicOp.strWearingFile[4].c_str();
					pItem->sBasicOp.strWearingFile[12] = pItem->sBasicOp.strWearingFile[4].c_str();
					pItem->sBasicOp.strWearingFile[14] = pItem->sBasicOp.strWearingFile[4].c_str();
					pItem->sBasicOp.strWearingFile[16] = pItem->sBasicOp.strWearingFile[4].c_str();
				}
				else if ( !pItem->sBasicOp.strWearingFile[8].empty() )
				{
					pItem->sBasicOp.strWearingFile[0] = pItem->sBasicOp.strWearingFile[8].c_str();
					pItem->sBasicOp.strWearingFile[1] = pItem->sBasicOp.strWearingFile[8].c_str();
					pItem->sBasicOp.strWearingFile[4] = pItem->sBasicOp.strWearingFile[8].c_str();
					pItem->sBasicOp.strWearingFile[9] = pItem->sBasicOp.strWearingFile[8].c_str();
					pItem->sBasicOp.strWearingFile[10] = pItem->sBasicOp.strWearingFile[8].c_str();
					pItem->sBasicOp.strWearingFile[12] = pItem->sBasicOp.strWearingFile[8].c_str();
					pItem->sBasicOp.strWearingFile[14] = pItem->sBasicOp.strWearingFile[8].c_str();
					pItem->sBasicOp.strWearingFile[16] = pItem->sBasicOp.strWearingFile[8].c_str();
				}
				else if ( !pItem->sBasicOp.strWearingFile[9].empty() )
				{
					pItem->sBasicOp.strWearingFile[0] = pItem->sBasicOp.strWearingFile[9].c_str();
					pItem->sBasicOp.strWearingFile[1] = pItem->sBasicOp.strWearingFile[9].c_str();
					pItem->sBasicOp.strWearingFile[4] = pItem->sBasicOp.strWearingFile[9].c_str();
					pItem->sBasicOp.strWearingFile[8] = pItem->sBasicOp.strWearingFile[9].c_str();
					pItem->sBasicOp.strWearingFile[10] = pItem->sBasicOp.strWearingFile[9].c_str();
					pItem->sBasicOp.strWearingFile[12] = pItem->sBasicOp.strWearingFile[9].c_str();
					pItem->sBasicOp.strWearingFile[14] = pItem->sBasicOp.strWearingFile[9].c_str();
					pItem->sBasicOp.strWearingFile[16] = pItem->sBasicOp.strWearingFile[9].c_str();
				}

				if ( !pItem->sBasicOp.strWearingFile[2].empty() )
				{
					pItem->sBasicOp.strWearingFile[3] = pItem->sBasicOp.strWearingFile[2].c_str();
					pItem->sBasicOp.strWearingFile[5] = pItem->sBasicOp.strWearingFile[2].c_str();
					pItem->sBasicOp.strWearingFile[6] = pItem->sBasicOp.strWearingFile[2].c_str();
					pItem->sBasicOp.strWearingFile[7] = pItem->sBasicOp.strWearingFile[2].c_str();
					pItem->sBasicOp.strWearingFile[11] = pItem->sBasicOp.strWearingFile[2].c_str();
					pItem->sBasicOp.strWearingFile[13] = pItem->sBasicOp.strWearingFile[2].c_str();
					pItem->sBasicOp.strWearingFile[15] = pItem->sBasicOp.strWearingFile[2].c_str();
					pItem->sBasicOp.strWearingFile[17] = pItem->sBasicOp.strWearingFile[2].c_str();
				}
				else if ( !pItem->sBasicOp.strWearingFile[3].empty() )
				{
					pItem->sBasicOp.strWearingFile[2] = pItem->sBasicOp.strWearingFile[3].c_str();
					pItem->sBasicOp.strWearingFile[5] = pItem->sBasicOp.strWearingFile[3].c_str();
					pItem->sBasicOp.strWearingFile[6] = pItem->sBasicOp.strWearingFile[3].c_str();
					pItem->sBasicOp.strWearingFile[7] = pItem->sBasicOp.strWearingFile[3].c_str();
					pItem->sBasicOp.strWearingFile[11] = pItem->sBasicOp.strWearingFile[3].c_str();
					pItem->sBasicOp.strWearingFile[13] = pItem->sBasicOp.strWearingFile[3].c_str();
					pItem->sBasicOp.strWearingFile[15] = pItem->sBasicOp.strWearingFile[3].c_str();
					pItem->sBasicOp.strWearingFile[17] = pItem->sBasicOp.strWearingFile[3].c_str();
				}
				else if ( !pItem->sBasicOp.strWearingFile[5].empty() )
				{
					pItem->sBasicOp.strWearingFile[2] = pItem->sBasicOp.strWearingFile[5].c_str();
					pItem->sBasicOp.strWearingFile[3] = pItem->sBasicOp.strWearingFile[5].c_str();
					pItem->sBasicOp.strWearingFile[6] = pItem->sBasicOp.strWearingFile[5].c_str();
					pItem->sBasicOp.strWearingFile[7] = pItem->sBasicOp.strWearingFile[5].c_str();
					pItem->sBasicOp.strWearingFile[11] = pItem->sBasicOp.strWearingFile[5].c_str();
					pItem->sBasicOp.strWearingFile[13] = pItem->sBasicOp.strWearingFile[5].c_str();
					pItem->sBasicOp.strWearingFile[15] = pItem->sBasicOp.strWearingFile[5].c_str();
					pItem->sBasicOp.strWearingFile[17] = pItem->sBasicOp.strWearingFile[5].c_str();
				}
				else if ( !pItem->sBasicOp.strWearingFile[6].empty() )
				{
					pItem->sBasicOp.strWearingFile[2] = pItem->sBasicOp.strWearingFile[6].c_str();
					pItem->sBasicOp.strWearingFile[3] = pItem->sBasicOp.strWearingFile[6].c_str();
					pItem->sBasicOp.strWearingFile[5] = pItem->sBasicOp.strWearingFile[6].c_str();
					pItem->sBasicOp.strWearingFile[7] = pItem->sBasicOp.strWearingFile[6].c_str();
					pItem->sBasicOp.strWearingFile[11] = pItem->sBasicOp.strWearingFile[6].c_str();
					pItem->sBasicOp.strWearingFile[13] = pItem->sBasicOp.strWearingFile[6].c_str();
					pItem->sBasicOp.strWearingFile[15] = pItem->sBasicOp.strWearingFile[6].c_str();
					pItem->sBasicOp.strWearingFile[17] = pItem->sBasicOp.strWearingFile[6].c_str();
				}
				else if ( !pItem->sBasicOp.strWearingFile[7].empty() )
				{
					pItem->sBasicOp.strWearingFile[2] = pItem->sBasicOp.strWearingFile[7].c_str();
					pItem->sBasicOp.strWearingFile[3] = pItem->sBasicOp.strWearingFile[7].c_str();
					pItem->sBasicOp.strWearingFile[5] = pItem->sBasicOp.strWearingFile[7].c_str();
					pItem->sBasicOp.strWearingFile[6] = pItem->sBasicOp.strWearingFile[7].c_str();
					pItem->sBasicOp.strWearingFile[11] = pItem->sBasicOp.strWearingFile[7].c_str();
					pItem->sBasicOp.strWearingFile[13] = pItem->sBasicOp.strWearingFile[7].c_str();
					pItem->sBasicOp.strWearingFile[15] = pItem->sBasicOp.strWearingFile[7].c_str();
					pItem->sBasicOp.strWearingFile[17] = pItem->sBasicOp.strWearingFile[7].c_str();
				}
			}
		}
	}
}