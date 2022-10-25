#include "StdAfx.h"
#include "LargeMapWindow.h"
#include "LargeMapWindowMobItem.h"
#include "LargeMapWindowCheckBox.h"

#include "../RanClientLib/G-Logic/GLContrlServerMsg.h"
#include "../RanClientLib/G-Logic/GLCrowData.h"
#include "../RanClientLib/G-Logic/GLItemMan.h"

#include "../RanClientUILib/Interface/InnerInterface.h"
#include "../RanClientUILib/Interface/BasicLineBox.h"
#include "../RanClientUILib/Interface/BasicLineBoxEx.h"
#include "../RanClientUILib/Interface/BasicTextBoxEx.h"
#include "../RanClientUILib/Interface/BasicScrollBarEx.h"
#include "../RanClientUILib/Interface/UITextControl.h"
#include "../RanClientUILib/Interface/GameTextControl.h"

#include "../EngineLib/DxCommon/DxFontMan.h"

#include "../EngineUILib/GUInterface/BasicScrollThumbFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CLargeMapWindowMobItem::nSTARTLINE = 0;
const int CLargeMapWindowMobItem::nOUTOFRANGE = -1;

CLargeMapWindowMobItem::CLargeMapWindowMobItem ():
	m_pListText( NULL ),
	m_pListScrollBar( NULL )
{
}

CLargeMapWindowMobItem::~CLargeMapWindowMobItem ()
{
}

void CLargeMapWindowMobItem::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
	pTextBox->CreateSub ( this, "LARGEMAP_ITEM_LIST_TEXTBOX", UI_FLAG_DEFAULT, ITEM_LIST_TEXTBOX );
	pTextBox->SetFont ( pFont9 );		
	pTextBox->SetLineInterval ( 3.0f );
	pTextBox->SetSensitive ( true );
	pTextBox->SetLimitLine ( 50000 );
	RegisterControl ( pTextBox );
	m_pListText = pTextBox;

	int nTotalLine = pTextBox->GetVisibleLine ();

	CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
	pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, ITEM_LIST_SCROLLBAR );
	pScrollBar->CreateBaseScrollBar ( "LARGEMAP_ITEM_LIST_SCROLLBAR" );
	pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( pScrollBar );
	m_pListScrollBar = pScrollBar;
}

void CLargeMapWindowMobItem::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListText->SetUseOverColor ( FALSE );
	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
		m_pListText->SetUseTextColor ( m_nSelectIndex, FALSE );
	
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
	{
		m_pListText->SetUseTextColor ( m_nSelectIndex, TRUE );
		m_pListText->SetTextColor ( m_nSelectIndex, NS_UITEXTCOLOR::ORANGE );
	}

	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

		const int nTotalLine = m_pListText->GetTotalLine ();
		const int nLinePerOneView = m_pListText->GetVisibleLine ();
		CDebugSet::ToView ( 1, 10, "??? ?? %d", nLinePerOneView );
		pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pListText->SetCurLine ( nPos );			
		}
	}

	if ( CHECK_MOUSE_IN ( ITEM_LIST_TEXTBOX ) )
	{
		const int nIndex = m_pListText->GetSelectPos ();
		if ( nIndex < 0 || m_pListText->GetCount () <= nIndex ) return ;

		m_pListText->SetUseOverColor ( TRUE );
		m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::DARKORANGE );

		m_nSelectIndex = nIndex;

		SNATIVEID sItemID = m_pListText->GetTextData ( m_nSelectIndex );
		if( sItemID != NATIVEID_NULL() )
		{
			SITEMCUSTOM sNewItemCustom(sItemID);
			if ( sNewItemCustom.sNativeID != NATIVEID_NULL () )
			{
				CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sNewItemCustom, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
			}
		}
	}
}

void CLargeMapWindowMobItem::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( cID, dwMsg );

	switch(cID)
	{
	case ITEM_LIST_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				const int nIndex = m_pListText->GetSelectPos ();
				if ( nIndex < 0 || m_pListText->GetCount () <= nIndex ) return ;

				m_pListText->SetUseOverColor ( TRUE );
				m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::DARKORANGE );

				m_nSelectIndex = nIndex;

				SNATIVEID sItemID = m_pListText->GetTextData ( m_nSelectIndex );
				if( sItemID != NATIVEID_NULL() )
				{
					SITEMCUSTOM sNewItemCustom(sItemID);
					if ( sNewItemCustom.sNativeID != NATIVEID_NULL () )
					{
						CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sNewItemCustom, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
					}
				}
			}
		}
		break;
	}
}

void CLargeMapWindowMobItem::LoadMob (NET_MSG_GENERIC* nmg)
{
	GLMSG::SNETPC_REQ_CROWLIST_FB* pNetMsg = (GLMSG::SNETPC_REQ_CROWLIST_FB*)nmg;
    PCROWDATA pCrow = GLCrowDataMan::GetInstance().GetCrowData( pNetMsg->sCrowID.wMainID, pNetMsg->sCrowID.wSubID );

	map<DWORD, int> vecItemDataList;
	SGENITEM m_sGenItem;
	DWORD dwID;

	if ( pCrow )
	{
		if ( !pCrow->m_sGenerate.m_strGenItem.empty() )
		{
			m_sGenItem.LOADFILE ( pCrow->m_sGenerate.m_strGenItem.c_str() );

			for( int _b = 0; _b < SGENITEM::EMGSPEC; _b++ )
			{
				if ( m_sGenItem.m_sItemSpec[_b].dwSPECID < 1 ) 
					continue;

				for(int i = 0; i < GLItemMan::MAX_MID; i++ )
				for(int j = 0; j < GLItemMan::MAX_SID; j++ )
				{
					SITEM* pITEM = GLItemMan::GetInstance().GetItem( i,j );
					if ( !pITEM ) 
						continue;

					if ( pITEM->sGenerateOp.dwSpecID != m_sGenItem.m_sItemSpec[_b].dwSPECID ) 
						continue;

					dwID = pITEM->sBasicOp.sNativeID.dwID;
					if( vecItemDataList.count( dwID ) == 0 ) 
						vecItemDataList.insert( std::make_pair( dwID, 1) );
				}
			}
		}
	}

	map<DWORD, int>::iterator iter = vecItemDataList.begin();
	map<DWORD, int>::iterator iter_end = vecItemDataList.end();

	for ( ; iter != iter_end; ++iter )
	{
		SNATIVEID sID = iter->first;
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sID );
		CString strName = pITEM->GetName();
		int nIndex = m_pListText->AddText ( strName, NS_UITEXTCOLOR::WHITE );	
		m_pListText->SetTextData ( nIndex, pITEM->sBasicOp.sNativeID.dwID );
	}

	m_pListText->SetCurLine ( 0 );

	const int nTotal = m_pListText->GetCount ();
	const int nViewPerPage = m_pListText->GetVisibleLine ();	
	m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
	m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
}

void CLargeMapWindowMobItem::ClearMobList()
{
	m_pListText->ClearText ();
}