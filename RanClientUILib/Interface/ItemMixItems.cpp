#include "StdAfx.h"
#include "ItemMixItems.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "ItemImage.h"
#include "BasicTextBoxEx.h"
#include "BasicScrollBarEx.h"
#include "../EngineUILib/GUInterface/BasicScrollThumbFrame.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "BasicLineBox.h"
#include "GameTextControl.h"
#include "BasicTextButton.h"
#include "BasicLineBox.h"
#include "InnerInterface.h"
#include "UITextControl.h"
#include "BasicChat.h"
#include "../EngineUILib/GUInterface/UIKeyCheck.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "GLItemDef.h"
#include "GLItem.h"
#include "../RanClientLib/G-Logic/GLItemMixMan.h"
#include "GameTextControl.h"
#include "D3DFont.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "../EngineUILib/GUInterface/BasicButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CItemMixItems::nLIMIT_CHAR = 16;

CItemMixItems::CItemMixItems ()
	: m_nSelectIndex ( -1 )
	, m_bINIT( FALSE )
	, m_pListText(NULL)
	, m_pListScrollBar(NULL)
	, m_pMoneyTextBox( NULL )
	, m_pRateTextBox ( NULL )
{
	memset( m_pMeterialsItemImage, NULL, sizeof( CItemImage* ) * ITEMMIX_ITEMNUM );
	memset( m_pMeterialsItemNum, NULL, sizeof( CBasicTextBox* ) * ITEMMIX_ITEMNUM );
	memset( m_pMeterialsItemNumBackImage, NULL, sizeof( CUIControl* ) * ITEMMIX_ITEMNUM );
}

CItemMixItems::~CItemMixItems ()
{
}

HRESULT CItemMixItems::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	m_bINIT = TRUE;

	LoadItemMixList();

	return S_OK;
}
CBasicTextBox* CItemMixItems::CreateTextBox ( const char* szTextBox )
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szTextBox );
    pTextBox->SetFont ( pFont8 );	
	RegisterControl ( pTextBox );
	return pTextBox;
}
void CItemMixItems::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, D3DFONT_SHADOW );
	
	{
		CUIControl* pControl = CreateControl ( "ITEMMIX_MONEY_BACK" );
		pControl->SetAlignFlag ( UI_FLAG_DEFAULT );

		CBasicButton* pButton = NULL;
		pButton = CreateFlipButton ( "ITEMMIX_MONEY_BUTTON", "ITEMMIX_MONEY_BUTTON_F", ITEMMIX_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
		pButton->CreateMouseOver ( "ITEMMIX_MONEY_BUTTON_F" );
		pButton->SetAlignFlag ( UI_FLAG_DEFAULT );
		pButton->SetUseDynamic ( TRUE );

		m_pMoneyTextBox = CreateStaticControl ( "ITEMMIX_MONEY_TEXT", pFont9, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
		m_pMoneyTextBox->SetAlignFlag ( UI_FLAG_DEFAULT );

		m_pRateTextBox = CreateStaticControl ( "ITEMMIX_RATE_TEXT", pFont9, NS_UITEXTCOLOR::DEFAULT, TEXT_ALIGN_LEFT );
		m_pRateTextBox->SetAlignFlag ( UI_FLAG_DEFAULT );

		CString strImage[ITEMMIX_CATEGORYNUM] =
		{
			"IMIX_BOT_ALL_ACTIVE",
			"IMIX_BOT_HEAD_ACTIVE",
			"IMIX_BOT_UPPER_ACTIVE",
			"IMIX_BOT_UNDER_ACTIVE",
			"IMIX_BOT_SHOES_ACTIVE",
			"IMIX_BOT_GLOVES_ACTIVE",
			"IMIX_BOT_WEAPON_ACTIVE",
			"IMIX_BOT_NECKLACE_ACTIVE",
			"IMIX_BOT_BRACELET_ACTIVE",
			"IMIX_BOT_RING_ACTIVE",
			"IMIX_BOT_ETC_ACTIVE",
			"IMIX_BOT_RANDB_ACTIVE",
			"IMIX_BOT_COS_ACTIVE",
		};

		CString strImage_f[ITEMMIX_CATEGORYNUM] =
		{
			"IMIX_BOT_ALL_OVER",
			"IMIX_BOT_HEAD_OVER",
			"IMIX_BOT_UPPER_OVER",
			"IMIX_BOT_UNDER_OVER",
			"IMIX_BOT_SHOES_OVER",
			"IMIX_BOT_GLOVES_OVER",
			"IMIX_BOT_WEAPON_OVER",
			"IMIX_BOT_NECKLACE_OVER",
			"IMIX_BOT_BRACELET_OVER",
			"IMIX_BOT_RING_OVER",
			"IMIX_BOT_ETC_OVER",
			"IMIX_BOT_RANDB_OVER",
			"IMIX_BOT_COS_OVER",
		};
		
		
		for ( int i = 0; i < ITEMMIX_CATEGORYNUM; ++i )
		{
			m_pCategoryButton[i] = new CBasicButton;
			m_pCategoryButton[i]->CreateSub ( this,strImage[i].GetString(), UI_FLAG_DEFAULT, ITEMMIX_CATEGORY_BUTTON0 + i);
			m_pCategoryButton[i]->CreateFlip ( strImage_f[i].GetString(), CBasicButton::RADIO_FLIP );
			m_pCategoryButton[i]->SetUseGlobalAction ( TRUE );
			m_pCategoryButton[i]->SetFlip ( FALSE );
			m_pCategoryButton[i]->SetVisibleSingle ( TRUE );
			RegisterControl ( m_pCategoryButton[i] );
		}

	}
	//	텍스트 박스
	{
		CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
		pTextBox->CreateSub ( this, "ITEMMIX_LIST_TEXTBOX", UI_FLAG_DEFAULT, ITEMMIX_LIST_TEXTBOX );
		pTextBox->SetFont ( pFont9 );		
		pTextBox->SetLineInterval ( 3.0f );
		pTextBox->SetSensitive ( true );
		pTextBox->SetLimitLine ( 10000 );
		RegisterControl ( pTextBox );
		m_pListText = pTextBox;

		int nTotalLine = pTextBox->GetVisibleLine ();

		//	스크롤바
		CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
		pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, ITEMMIX_LIST_SCROLLBAR );
		pScrollBar->CreateBaseScrollBar ( "ITEMMIX_LIST_SCROLLBAR" );
		pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
		RegisterControl ( pScrollBar );
		m_pListScrollBar = pScrollBar;
	}
			CString str;
			str.Format("ITEM_MIX_ITEMS_SLOTR");

			m_pResultItemImage = new CItemImage;
			m_pResultItemImage->CreateSub( this, str, UI_FLAG_DEFAULT, ITEM_MIX_ITEMS_SLOTR );
			m_pResultItemImage->CreateSubControl();
			RegisterControl( m_pResultItemImage );

			m_pMeterialsItemNumBackImage[0] = new CBasicLineBox;
			m_pMeterialsItemNumBackImage[0]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pMeterialsItemNumBackImage[0]->CreateBaseBoxSkill ( "ITEM_MIX_NUM_SLOT0_BACK");
			RegisterControl ( m_pMeterialsItemNumBackImage[0] );

			m_pMeterialsItemNumBackImage[1] = new CBasicLineBox;
			m_pMeterialsItemNumBackImage[1]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pMeterialsItemNumBackImage[1]->CreateBaseBoxSkill ( "ITEM_MIX_NUM_SLOT1_BACK");
			RegisterControl ( m_pMeterialsItemNumBackImage[1] );

			m_pMeterialsItemNumBackImage[2] = new CBasicLineBox;
			m_pMeterialsItemNumBackImage[2]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pMeterialsItemNumBackImage[2]->CreateBaseBoxSkill ( "ITEM_MIX_NUM_SLOT2_BACK");
			RegisterControl ( m_pMeterialsItemNumBackImage[2] );

			m_pMeterialsItemNumBackImage[3] = new CBasicLineBox;
			m_pMeterialsItemNumBackImage[3]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pMeterialsItemNumBackImage[3]->CreateBaseBoxSkill ( "ITEM_MIX_NUM_SLOT3_BACK");
			RegisterControl ( m_pMeterialsItemNumBackImage[3] );
			
			m_pMeterialsItemNumBackImage[4] = new CBasicLineBox;
			m_pMeterialsItemNumBackImage[4]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pMeterialsItemNumBackImage[4]->CreateBaseBoxSkill ( "ITEM_MIX_NUM_SLOT4_BACK");
			RegisterControl ( m_pMeterialsItemNumBackImage[4] );

		for( int i = 0; i < ITEMMIX_ITEMNUM; ++i )
		{

			CString strTemp;
			strTemp.Format("ITEM_MIX_ITEMS_SLOT%d", i );

			m_pMeterialsItemImage[i] = new CItemImage;
			m_pMeterialsItemImage[i]->CreateSub( this, strTemp, UI_FLAG_DEFAULT, ITEM_MIX_ITEMS_SLOT0+i );
			m_pMeterialsItemImage[i]->CreateSubControl();
			RegisterControl( m_pMeterialsItemImage[i] );

			strTemp.Format("ITEM_MIX_ITEMS_SLOT%d_AMOUNT", i );
			m_pMeterialsItemNum[i] = CreateTextBox ( strTemp );	
			m_pMeterialsItemNum[i]->SetTextAlign ( TEXT_ALIGN_RIGHT );		
		

		}

			

			
}


void CItemMixItems::LoadItemMixList ()
{
	if( !m_bINIT ) return; // InitDeviceObject() 호출 후에 실행 되어야 한다.

	//	친구 목록을 갱신한다.
	m_pListText->ClearText ();
	FlipBot ( 0 );
	GLItemMixMan::ITEMMIX_STRMAP& ItemMixStrMap = GLItemMixMan::GetInstance().GetItemMix ();
	
	{
		GLItemMixMan::ITEMMIX_STRMAP_ITER iter = ItemMixStrMap.begin ();
		GLItemMixMan::ITEMMIX_STRMAP_ITER iter_end = ItemMixStrMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			ITEM_MIX& sItemMix = (*iter).second;
			if ( sItemMix.sResultItem.sNID != NATIVEID_NULL() ) 
			LoadItemMixItems ( sItemMix );
		}
	}

	{
		GLItemMixMan::ITEMMIX_STRMAP_ITER iter = ItemMixStrMap.begin ();
		GLItemMixMan::ITEMMIX_STRMAP_ITER iter_end = ItemMixStrMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			ITEM_MIX& sItemMix = (*iter).second;
			if ( sItemMix.sResultItem.sNID != NATIVEID_NULL() ) 
			LoadItemMixItems ( sItemMix );
		}
	}

	m_pListText->SetCurLine ( 0 );

	const int nTotal = m_pListText->GetCount ();
	const int nViewPerPage = m_pListText->GetVisibleLine ();	
	m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
	m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
}

void CItemMixItems::LoadItemMixItems ( const ITEM_MIX& sItemMix )
{
	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemMix.sResultItem.sNID );
	const CString& strName = pItemData->GetName();

	static CString strKeyword[] =
	{
		"ITEM_MIX_ICON",
		"ITEM_MIX_ICON",
		"ITEM_MIX_ICON_LOCK"
	};

	D3DCOLOR dwTextColor;
	int nIcon;

	dwTextColor = NS_UITEXTCOLOR::WHITE;
	nIcon = ITEM;
	
	CString strTemp;
	CString strTemp2;
	
	strTemp.Format( "%s", strName );

	int nIndex = m_pListText->AddText ( strTemp, dwTextColor );	
	m_pListText->SetTextImage ( nIndex, strKeyword[nIcon] );	
	m_pListText->SetTextData ( nIndex, sItemMix.dwKey );
}

BOOL CItemMixItems::GetItemMix ( const DWORD nItemID )
{
	GLItemMixMan::ITEMMIX_STRMAP& ItemMixKeyMap = GLItemMixMan::GetInstance().GetItemMix ();
	
	GLItemMixMan::ITEMMIX_STRMAP_ITER iter = ItemMixKeyMap.begin();
	GLItemMixMan::ITEMMIX_STRMAP_ITER iter_end = ItemMixKeyMap.end ();	
	for ( ; iter != iter_end; ++iter )
	{
		ITEM_MIX& sItemMix = (*iter).second;
			
		if ( sItemMix.dwKey == nItemID ) 
		{
			m_nItemID = nItemID;
			
					
			return TRUE;
		}
		
	}

	return FALSE;
}

void CItemMixItems::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case ITEMMIX_CATEGORY_BUTTON0:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = cID - ITEMMIX_CATEGORY_BUTTON0;
				if ( nIndex < 0 || nIndex >=  ITEMMIX_CATEGORY_BUTTON0 )	return;
				
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( ID2GAMEWORD( "PRODUCT_TYPE_STRING" , 0 ), NS_UITEXTCOLOR::WHITE  );				
				if ( UIMSG_LB_UP & dwMsg )
				{
					m_pListText->ClearText ();
					CString strNum;
					FlipBot ( nIndex );
					GLItemMixMan::ITEMMIX_STRMAP& ItemMixKeyMap = GLItemMixMan::GetInstance().GetItemMix();
					GLItemMixMan::ITEMMIX_STRMAP_ITER iter = ItemMixKeyMap.begin();
					GLItemMixMan::ITEMMIX_STRMAP_ITER iter_end = ItemMixKeyMap.end ();
					
					for ( ; iter != iter_end; ++iter )
					{
						ITEM_MIX& sItemMix = (*iter).second;
						
							SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemMix.sResultItem.sNID );
							if ( pItemData )
							{
								LoadItemMixItems ( sItemMix );
							}
					}
					m_pListText->SetCurLine ( 0 );
					const int nTotal = m_pListText->GetCount ();
					const int nViewPerPage = m_pListText->GetVisibleLine ();	
					m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
					m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
				}
			}
		}
	case ITEMMIX_CATEGORY_BUTTON1:
	case ITEMMIX_CATEGORY_BUTTON2:
	case ITEMMIX_CATEGORY_BUTTON3:
	case ITEMMIX_CATEGORY_BUTTON4:
	case ITEMMIX_CATEGORY_BUTTON5:
	case ITEMMIX_CATEGORY_BUTTON6:
	case ITEMMIX_CATEGORY_BUTTON7:
	case ITEMMIX_CATEGORY_BUTTON8:
	case ITEMMIX_CATEGORY_BUTTON9:
	case ITEMMIX_CATEGORY_BUTTON10:
	case ITEMMIX_CATEGORY_BUTTON11:
	case ITEMMIX_CATEGORY_BUTTON12:
		{
			
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = cID - ITEMMIX_CATEGORY_BUTTON1 + 1;
				if ( nIndex < 1 || nIndex >=  ITEMMIX_CATEGORY_BUTTON12 )	return;
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( ID2GAMEWORD( "PRODUCT_TYPE_STRING" , nIndex ), NS_UITEXTCOLOR::WHITE  );				
				
				if ( UIMSG_LB_UP & dwMsg )
				{
					FlipBot ( nIndex );
					m_pListText->ClearText ();
					m_nSelectIndex = nIndex;
					CString strNum;
					GLItemMixMan::ITEMMIX_STRMAP& ItemMixKeyMap = GLItemMixMan::GetInstance().GetItemMix();
					GLItemMixMan::ITEMMIX_STRMAP_ITER iter = ItemMixKeyMap.begin();
					GLItemMixMan::ITEMMIX_STRMAP_ITER iter_end = ItemMixKeyMap.end ();
					
					for ( ; iter != iter_end; ++iter )
					{
						ITEM_MIX& sItemMix = (*iter).second;
						if ( sItemMix.dwCategory == nIndex ) 
						{
							m_nCategory = nIndex;
							SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemMix.sResultItem.sNID );
							if ( pItemData )
							{
								LoadItemMixItems ( sItemMix );
							}
						}
					}
					m_pListText->SetCurLine ( 0 );
					const int nTotal = m_pListText->GetCount ();
					const int nViewPerPage = m_pListText->GetVisibleLine ();	
					m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
					m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
				}
			}
		}
		break;
	case ITEM_MIX_ITEMS_SLOTR:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
			
				SITEMCUSTOM sItem1 = sMixData.sResult;
				if( sItem1.sNativeID != NATIVEID_NULL() ) 
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItem1 , FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
				}
			}
		}
		break;
	case ITEM_MIX_ITEMS_SLOT0:
	case ITEM_MIX_ITEMS_SLOT1:
	case ITEM_MIX_ITEMS_SLOT2:
	case ITEM_MIX_ITEMS_SLOT3:
	case ITEM_MIX_ITEMS_SLOT4:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = cID - ITEM_MIX_ITEMS_SLOT0;
				
				if ( nIndex < 0 || nIndex >= SMIXDATA::SMIX_SLOTS )	return;

					SITEMCUSTOM sItem1 = sMixData.sItems[nIndex];
					if( sItem1.sNativeID != NATIVEID_NULL() ) 
					{
						CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItem1 , FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
					}
			}
		}
		break;
	case ITEMMIX_LIST_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				const int nIndex = m_pListText->GetSelectPos ();
				if ( nIndex < 0 || m_pListText->GetCount () <= nIndex ) return ;

				m_pListText->SetUseOverColor ( TRUE );
				m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::DARKGRAY );

				if( UIMSG_LB_UP & dwMsg )
				{
					m_nSelectIndex = nIndex;
					DWORD nItemID = m_pListText->GetTextData ( m_nSelectIndex );
					if( !GetItemMix( nItemID ) ) m_strSelectName.Empty();
					
				}
				
				if ( UIMSG_LB_UP & dwMsg )
				{
					m_nSelectIndex = nIndex;
					CString strNum;
					DWORD nItemID = m_pListText->GetTextData ( m_nSelectIndex );
					GLItemMixMan::ITEMMIX_STRMAP& ItemMixKeyMap = GLItemMixMan::GetInstance().GetItemMix();
					GLItemMixMan::ITEMMIX_STRMAP_ITER iter = ItemMixKeyMap.begin();
					GLItemMixMan::ITEMMIX_STRMAP_ITER iter_end = ItemMixKeyMap.end ();
					for ( ; iter != iter_end; ++iter )
					{
						ITEM_MIX& sItemMix = (*iter).second;
						if ( sItemMix.dwKey == nItemID ) 
						{
							m_nItemID = nItemID;
							SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemMix.sResultItem.sNID );
							if ( pItemData )
							{
								m_pResultItemImage->SetItem( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );
								sMixData.sResult = sItemMix.sResultItem.sNID;
								SetMoney ( sItemMix.dwPrice );
								SetRate ( sItemMix.dwRate );
							}
							else
							{
								m_pResultItemImage->ResetItem();
								sMixData.sResult = NATIVEID_NULL();
							}
							for ( int i = 0; i < ITEMMIX_ITEMNUM; ++i )
							{
								SITEM* pItem	 = GLItemMan::GetInstance().GetItem( sItemMix.sMeterialItem[i].sNID );
								if( pItem )
								{
									strNum.Format( "%d", sItemMix.sMeterialItem[i].nNum );
									m_pMeterialsItemImage[i]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
									m_pMeterialsItemNum[i]->SetText( strNum, NS_UITEXTCOLOR::WHITE );
									sMixData.sItems[i] = sItemMix.sMeterialItem[i].sNID;
									
								}
								else
								{
									m_pMeterialsItemImage[i]->ResetItem();
									m_pMeterialsItemNum[i]->ClearText();
									sMixData.sItems[i] = NATIVEID_NULL();
								}
							}
						}
					}
				}	
			}
		}
		break;



	}
}


void CItemMixItems::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListText->SetUseOverColor ( FALSE );
	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
		m_pListText->SetUseTextColor ( m_nSelectIndex, FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	DWORD nItemID = m_pListText->GetTextData ( m_nSelectIndex );
	GLItemMixMan::ITEMMIX_STRMAP& ItemMixKeyMap = GLItemMixMan::GetInstance().GetItemMix();
	GLItemMixMan::ITEMMIX_STRMAP_ITER iter = ItemMixKeyMap.begin();
	GLItemMixMan::ITEMMIX_STRMAP_ITER iter_end = ItemMixKeyMap.end ();
	CString strNum;
	for ( ; iter != iter_end; ++iter )
	{
		ITEM_MIX& sItemMix = (*iter).second;
		if ( sItemMix.dwKey == nItemID ) 
		{
			m_nItemID = nItemID;
			SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemMix.sResultItem.sNID );
			if ( pItemData )
			{
				m_pResultItemImage->SetItem( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );
				sMixData.sResult = sItemMix.sResultItem.sNID;
			}
			else
			{
				m_pResultItemImage->ResetItem();
				sMixData.sResult = NATIVEID_NULL();
			}
			for ( int i = 0; i < ITEMMIX_ITEMNUM; ++i )
			{
				SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemMix.sMeterialItem[i].sNID );
				if( pItem )
				{
					strNum.Format( "%d", sItemMix.sMeterialItem[i].nNum );
					m_pMeterialsItemImage[i]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
					m_pMeterialsItemNum[i]->SetText( strNum, NS_UITEXTCOLOR::WHITE );
					sMixData.sItems[i] = sItemMix.sMeterialItem[i].sNID;
					
				}
				else
				{
					m_pMeterialsItemImage[i]->ResetItem();
					m_pMeterialsItemNum[i]->ClearText();
					sMixData.sItems[i] = NATIVEID_NULL();
				}
			}
		}
	}

	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
	{
		m_pListText->SetUseTextColor ( m_nSelectIndex, TRUE );
		m_pListText->SetTextColor ( m_nSelectIndex, NS_UITEXTCOLOR::DARKGRAY );
	}

	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

		const int nTotalLine = m_pListText->GetTotalLine ();
		const int nLinePerOneView = m_pListText->GetVisibleLine ();
		CDebugSet::ToView ( 1, 10, "보이는 라인 %d", nLinePerOneView );
		pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pListText->SetCurLine ( nPos );			
		}
	}
}

void CItemMixItems::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadItemMixList ();

		m_nSelectIndex = -1;

		
	}
}

void CItemMixItems::FlipBot ( int nCategory )
{
	for ( int i = 0; i < ITEMMIX_CATEGORYNUM; ++i )
	{
		m_pCategoryButton[i]->SetFlip( FALSE );
	}
	m_pCategoryButton[nCategory]->SetFlip( TRUE );					
}
void CItemMixItems::SetMoney ( LONGLONG Money )
{
	//	돈자리수
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( Money, 3, "," );
	m_pMoneyTextBox->SetOneLineText ( strTemp );
}
void CItemMixItems::SetRate ( DWORD Rate )
{
	CString strTemp;
	strTemp.Format ("Success Rate : %d", Rate);
	strTemp += _T('%');
	m_pRateTextBox->SetOneLineText ( strTemp );
}
BOOL CItemMixItems::IsSelectedIndex()
{
	return ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount() );
}

/*const DWORD CItemMixItems::GetFriendID()
{
	if ( !IsSelectedIndex() )
		return NATIVEID_NULL().dwID;

	return m_pListText->GetTextData ( m_nSelectIndex );
}*/