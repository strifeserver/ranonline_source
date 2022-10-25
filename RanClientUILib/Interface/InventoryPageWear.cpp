#include "StdAfx.h"
#include "InventoryPageWear.h"
#include "ItemImage.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "InnerInterface.h"
#include "ItemMove.h"

#include "UITextControl.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "./BasicVarTextBox.h"
#include "d3dfont.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CInventoryPageWear::nOUTOFRANGE = -1;

CInventoryPageWear::CInventoryPageWear () 
	: m_pMouseOver ( NULL )
{
//SecureZeroMemory ( m_pNumberBoxArray, sizeof( m_pNumberBoxArray ) );
}

CInventoryPageWear::~CInventoryPageWear ()
{
}

void CInventoryPageWear::CreateSubControl ( int nClassType )
{
	{	//	?????? ?????? ( 0 - 11 )
		CString	strInvenWearBack[GLCI_NUM_2012] = 
		{
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_BRAWLER_M",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_ARMS_M",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_ARCHER_W",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_SPIRIT_W",

		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_EXTREME_M",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_EXTREME_W",

		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_BRAWLER_W",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_ARMS_W",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_ARCHER_M",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_SPIRIT_M",

		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_SCIENCE_M",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_SCIENCE_W",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_ASSASSIN_M",
		"CHARACTER_WINDOW_CHARACTER_MESHRENDER_ASSASSIN_W",
		};

		CreateControl ( strInvenWearBack[nClassType].GetString () );

		CString strInvenWearItem[ITEM_IMAGE_SIZE] = 
		{
			"CHARACTER_WINDOW_WEAR_SLOT_HEAD",			//หั?
			"CHARACTER_WINDOW_WEAR_SLOT_NECK",			//สร้อ?
			"CHARACTER_WINDOW_WEAR_SLOT_UPPER",			//เสื้?
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON",	//ยั?กระสุน
			"CHARACTER_WINDOW_WEAR_SLOT_WRIST",			//ต่างหู
			"CHARACTER_WINDOW_WEAR_SLOT_HAND",			//ถุงมือ
			"CHARACTER_WINDOW_WEAR_SLOT_LOWER",			//กางเกง
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING",	//แหวน1
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON",	//อาวุ?
			"CHARACTER_WINDOW_WEAR_SLOT_FOOT",			//รองเท้?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING",//แหวน2
			"CHARACTER_WINDOW_WEAR_SLOT_VEHICLE",		//รถ
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY",//ปี?
			"CHARACTER_WINDOW_WEAR_SLOT_WAIST",			//ออร่?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY",
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER",
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER",
		};

		CString strInvenWearOver[ITEM_IMAGE_SIZE] = 
		{
			"CHARACTER_WINDOW_WEAR_SLOT_HEAD_OVER",			//หั?
			"CHARACTER_WINDOW_WEAR_SLOT_NECK_OVER",			//สร้อ?
			"CHARACTER_WINDOW_WEAR_SLOT_UPPER_OVER",		//เสื้?
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON_OVER",	//ยั?กระสุน
			"CHARACTER_WINDOW_WEAR_SLOT_WRIST_OVER",		//ต่างหู
			"CHARACTER_WINDOW_WEAR_SLOT_HAND_OVER",			//ถุงมือ
			"CHARACTER_WINDOW_WEAR_SLOT_LOWER_OVER",		//กางเกง
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING_OVER",	//แหวน1
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON_OVER",	//อาวุ?
			"CHARACTER_WINDOW_WEAR_SLOT_FOOT_OVER",			//รองเท้?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING_OVER",//แหวน2
			"CHARACTER_WINDOW_WEAR_SLOT_VEHICLE_OVER",		//รถ
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY_OVER",//ปี?
			"CHARACTER_WINDOW_WEAR_SLOT_WAIST_OVER",		//ออร่?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER_OVER",
		};

			CString InventoryText[ITEM_IMAGE_SIZE] = 
		{
			"CHARACTER_WINDOW_WEAR_SLOT_HEAD_NUMBER",			//หั?
			"CHARACTER_WINDOW_WEAR_SLOT_NECK_NUMBER",			//สร้อ?
			"CHARACTER_WINDOW_WEAR_SLOT_UPPER_NUMBER",		//เสื้?
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_WEAPON_NUMBER",	//ยั?กระสุน
			"CHARACTER_WINDOW_WEAR_SLOT_WRIST_NUMBER",		//ต่างหู
			"CHARACTER_WINDOW_WEAR_SLOT_HAND_NUMBER",			//ถุงมือ
			"CHARACTER_WINDOW_WEAR_SLOT_LOWER_NUMBER",		//กางเกง
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING_NUMBER",	//แหวน1
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_WEAPON_NUMBER",	//อาวุ?
			"CHARACTER_WINDOW_WEAR_SLOT_FOOT_NUMBER",			//รองเท้?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING_NUMBER",//แหวน2
			"CHARACTER_WINDOW_WEAR_SLOT_VEHICLE_NUMBER",		//รถ
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY_NUMBER",//ปี?
			"CHARACTER_WINDOW_WEAR_SLOT_WAIST_NUMBER",		//ออร่?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY_NUMBER",
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER_NUMBER",
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER_NUMBER",

		};

		for ( int i = 0; i < ITEM_IMAGE_SIZE; i++ )
		{
			m_pItemImage[i] = CreateItemImage ( strInvenWearItem[i].GetString (), ITEM_IMAGE0 + i );
			m_pSlotDisplay[i] = CreateControl ( strInvenWearOver[i].GetString () );
			m_pNumberBoxArray[i] = CreateNumberBox ( InventoryText[i].GetString () );
			m_pNumberBoxArray[i]->SetOneLineText ( "",0 );

			m_pSlotDisplay[i]->SetVisibleSingle ( FALSE );
		}
	}

//	CreateNumberBox ();

	m_pMouseOver = CreateControl ( "ITEM_MOUSE_OVER" );
//	m_pSlotDisplay = CreateControl ( "TRADE_INVENTORY_OVER_0" );
//	m_pSlotBlockDisplay = CreateControl ( "TRADE_OVER_0" );
}

CUIControl*	CInventoryPageWear::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );

	return pControl;
}

CItemImage*	CInventoryPageWear::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );

	return pItemImage;
}

EMSLOT CInventoryPageWear::IMAGE2EMSLOT ( int nIndex )
{
	switch ( nIndex )
	{	
	case ITEM_IMAGE0:	return SLOT_HEADGEAR;	//	????
	case ITEM_IMAGE1:	return SLOT_NECK;		//	??????
	case ITEM_IMAGE2:	return SLOT_UPPER;		//	????
	case ITEM_IMAGE3:	return SLOT_LHAND;		//	????????
	case ITEM_IMAGE4:	return SLOT_WRIST;		//	????
	case ITEM_IMAGE5:	return SLOT_HAND;		//	????
	case ITEM_IMAGE6:	return SLOT_LOWER;		//	????
	case ITEM_IMAGE7:	return SLOT_LFINGER;	//	???? ?????
	case ITEM_IMAGE8:	return SLOT_RHAND;		//	??????????
	case ITEM_IMAGE9:	return SLOT_FOOT;		//	????
	case ITEM_IMAGE10:	return SLOT_RFINGER;	//	?????? ?????	
	case ITEM_IMAGE11:	return SLOT_VEHICLE;	
	case ITEM_IMAGE12:	return SLOT_WING;	
	case ITEM_IMAGE13:	return SLOT_AURA;	
	case ITEM_IMAGE14:	return SLOT_BELT;
	case ITEM_IMAGE15:	return SLOT_EARRING;
	case ITEM_IMAGE16:	return SLOT_ACCESSORY;
	case ITEM_IMAGE17:	return SLOT_CON;
	}

	GASSERT ( 0 && "????? ??????????." );
	return SLOT_TSIZE;
}

void CInventoryPageWear::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex ( nOUTOFRANGE );
	if ( m_pMouseOver ) m_pMouseOver->SetVisibleSingle ( FALSE );
//	if ( m_pSlotDisplay ) m_pSlotDisplay->SetVisibleSingle ( FALSE );
//	if ( m_pSlotBlockDisplay ) m_pSlotBlockDisplay->SetVisibleSingle ( FALSE );

	//	????, ???? ?????? ??????
	CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
	SNATIVEID sHOLD_ITEM_ID(false);
//	bool bFOUND_TOWEAR = false;
	if ( pItemMove ) sHOLD_ITEM_ID = pItemMove->GetItem();

	for ( int i = 0; i < SLOT_NSIZE_2; i++ )
	{
		const SITEMCUSTOM& sItemCustomOld = GetItem ( i );

		EMSLOT emSlot = IMAGE2EMSLOT ( i + ITEM_IMAGE0 );
		SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_SLOT_ITEM ( emSlot );


		//	NOTE
		//		???? ???????? ????????,
		//		???????? ????? ????????,
		//		????/?????? ?????? ????????.
		if ( sItemCustom != sItemCustomOld )
		{		
			if ( sItemCustom.sNativeID != NATIVEID_NULL () )
			{
				LoadItem ( i, sItemCustom );
			}
			else
			{
				UnLoadItem ( i );
			}
		}

		m_pSlotDisplay[i]->SetVisibleSingle ( FALSE );

		//	NOTE
		//		?????? ??? ????
		if ( sHOLD_ITEM_ID != NATIVEID_NULL () /*&& !bFOUND_TOWEAR*/ )
		{
			if ( GLGaeaClient::GetInstance().GetCharacter()->CHECKSLOT_ITEM ( sHOLD_ITEM_ID, emSlot ) )
			{
				const UIRECT& rcImagePos = m_pItemImage[i]->GetGlobalPos ();
				D3DXVECTOR2 vPos ( rcImagePos.left, rcImagePos.top );

				if ( GLGaeaClient::GetInstance().GetCharacter()->ACCEPT_ITEM ( sHOLD_ITEM_ID ) )
				{
					m_pSlotDisplay[i]->SetVisibleSingle ( TRUE );
				}

//				bFOUND_TOWEAR = true;
			}
		}
	}
	
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CInventoryPageWear::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ITEM_IMAGE0:
	case ITEM_IMAGE1:
	case ITEM_IMAGE2:
	case ITEM_IMAGE3:
	case ITEM_IMAGE4:
	case ITEM_IMAGE5:
	case ITEM_IMAGE6:
	case ITEM_IMAGE7:
	case ITEM_IMAGE8:
	case ITEM_IMAGE9:
	case ITEM_IMAGE10:
	case ITEM_IMAGE11:
	case ITEM_IMAGE12: 	//add wing
	case ITEM_IMAGE13: //add aura
	case ITEM_IMAGE14:
	case ITEM_IMAGE15:
	case ITEM_IMAGE16:
	case ITEM_IMAGE17:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = ControlID - ITEM_IMAGE0;
				SetItemIndex ( nIndex );

				if ( !CInnerInterface::GetInstance().IsFirstItemSlot () ) return ;

				//	????, ???? ?????? ??????
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
				if ( !pItemMove )
				{
					GASSERT ( 0 && "CItemMove?? ????????." );
					return ;
				}

				if ( pItemMove->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPos = m_pItemImage[nIndex]->GetGlobalPos ();
					pItemMove->SetGlobalPos ( rcSlotPos );						

					AddMessageEx ( UIMSG_MOUSEIN_WEARSLOT | UIMSG_TOTOPPARENT );
				}				

				//	????? ???? ??????
				if ( pItemMove->GetItem () == NATIVEID_NULL () && m_pItemImage[m_nIndex]->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcImagePos = m_pItemImage[nIndex]->GetGlobalPos ();
					m_pMouseOver->SetGlobalPos ( rcImagePos );
					m_pMouseOver->SetVisibleSingle ( TRUE );
				}

				

				if ( dwMsg & UIMSG_RB_UP )
				{
				//int nPosX, nPosY;
				//m_pPage->GetItemIndex ( &nPosX, &nPosY );
				//if ( nPosX < 0 || nPosY < 0 ) return;
//				WORD wPosX, wPosY;
				EMSLOT J;
				SITEM* pItemData = GLItemMan::GetInstance().GetItem ( m_ItemCustomArray[nIndex].sNativeID );
					if ( !pItemData  ) return;
		
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
					if ( !pCharacter ) return;	

				EMITEM_TYPE emItemType = pItemData->sBasicOp.emItemType;

				//pInvenItem = pCharacter->m_cInventory.FindPosItem ( pInvenItem->wPosX, pInvenItem->wPosY );
					//if ( !pInvenItem ) return;
				if (pItemData)
						{
							switch ( pItemData->sSuitOp.emSuit )
							{
							case SUIT_HEADGEAR:		J = SLOT_HEADGEAR;break;
							case SUIT_UPPER:		J = SLOT_UPPER;break;
							case SUIT_LOWER:		J = SLOT_LOWER;break;
							case SUIT_HAND:			J = SLOT_HAND;break;
							case SUIT_FOOT:			J = SLOT_FOOT;break;
							case SUIT_NECK:			J = SLOT_NECK;break;
							case SUIT_WRIST:		J = SLOT_WRIST;break;
							case SUIT_VEHICLE:		J = SLOT_VEHICLE;break;
							case SUIT_WING:			J = SLOT_WING;break;
							case SUIT_BELT:			J = SLOT_BELT;break;
							case SUIT_ACCESSORY:	J = SLOT_ACCESSORY;break;
							case SUIT_AURA:			J =	SLOT_AURA;break;
							case SUIT_EARRING:		J =	SLOT_EARRING;break;
							case SUIT_CON:			J =	SLOT_CON;break;

							case SUIT_HANDHELD:	
								if ( emItemType == ITEM_ARROW || emItemType == ITEM_CHARM || emItemType == ITEM_BULLET )
								{
									if(GLGaeaClient::GetInstance().GetCharacter()->IsUseArmSub())
									{
										J = SLOT_LHAND_S;
									}
									else{
										J = SLOT_LHAND;
									}
								}
								else
								{
									if(GLGaeaClient::GetInstance().GetCharacter()->IsUseArmSub())
									{
										J = SLOT_RHAND_S;
									}
									else{
										J = SLOT_RHAND;
									}
								}
								break;
								
							case SUIT_FINGER:	
								{
									
									if ( m_bRingSwap )
									{
										m_bRingSwap = FALSE;
										J = SLOT_RFINGER;
									}
									else
									{
										J = SLOT_LFINGER;
										m_bRingSwap = TRUE;
									}
								}
								break;
						
							};
							GLGaeaClient::GetInstance().GetCharacter()->ReqUnEquipItem ( J );
						}
				
				return ;
				}
			}
		}
		break;
}
}
	

EMSLOT CInventoryPageWear::GetItemSlot ()
{
	return IMAGE2EMSLOT ( GetItemIndex () + ITEM_IMAGE0 );
}

void CInventoryPageWear::LoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom )
{
	m_ItemCustomArray[nIndex] = ref_sItemCustom;

	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( ref_sItemCustom.sNativeID );
	if ( pItemData )
	{
		const WORD wPileNum = pItemData->sDrugOp.wPileNum;
		const WORD wTurnNum = ref_sItemCustom.wTurnNum;
		if ( wPileNum>1 || wTurnNum>1 )
		{
			CString strNumber;
			strNumber.Format ( "%d", wTurnNum );
			DWORD dwColor = NS_UITEXTCOLOR::WHITE;
			if ( wTurnNum == wPileNum ) dwColor = NS_UITEXTCOLOR::ORANGE;
			m_pNumberBoxArray[nIndex]->SetOneLineText ( strNumber.GetString(), dwColor );
			m_pNumberBoxArray[nIndex]->SetVisibleSingle ( TRUE );
		}else{
			CString Upgrade="";
			DWORD dwColor = NS_UITEXTCOLOR::WHITE;
			int PlusItem = ref_sItemCustom.GETGRADE(EMGRINDING_NUNE);
			if ( PlusItem !=0 ) Upgrade.Format("+%d",PlusItem);
			m_pNumberBoxArray[nIndex]->SetOneLineText ( Upgrade, dwColor );
		}
	}
	m_pItemImage[nIndex]->SetItem ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile(), pItemData->sBasicOp.sNativeID );
}

SITEMCUSTOM& CInventoryPageWear::GetItem ( int nIndex )
{
	return m_ItemCustomArray[nIndex];
}

void CInventoryPageWear::UnLoadItem ( int nIndex )
{
	m_ItemCustomArray[nIndex].sNativeID = NATIVEID_NULL ();
	m_pItemImage[nIndex]->ResetItem ();
	m_pNumberBoxArray[nIndex]->ClearText ();

	
}

CBasicTextBox* CInventoryPageWear::CreateNumberBox ( const char* szControl )
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
    pTextBox->SetFont ( pFont8 );
	pTextBox->SetTextAlign ( TEXT_ALIGN_RIGHT );
	RegisterControl ( pTextBox );

	return pTextBox;

}