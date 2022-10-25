#include "StdAfx.h"
#include "./InventoryPageWearEx.h"
#include "ItemImage.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "InnerInterface.h"
#include "ItemMove.h"
#include "BasicTextButton.h"
#include "UITextControl.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "./BasicVarTextBox.h"
#include "GameTextControl.h"

#include "d3dfont.h"

#include "UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CInventoryPageWearEx::nOUTOFRANGE = -1;

CInventoryPageWearEx::CInventoryPageWearEx () 
	: m_pMouseOver ( NULL )
	, m_pAButton(NULL)
	, m_pBButton(NULL)
	, m_pARHandSlot(NULL)
	, m_pALHandSlot(NULL)
	, m_pBRHandSlot(NULL)
	, m_pBLHandSlot(NULL)
{
	memset ( m_Button, 0, sizeof ( CUIControl* ) * 2 );	
	memset ( m_ButtonSet, 0, sizeof ( CUIControl* ) * 2 );
//	SecureZeroMemory ( m_pNumberBoxArray, sizeof( m_pNumberBoxArray ) );
}

CInventoryPageWearEx::~CInventoryPageWearEx ()
{
}

void CInventoryPageWearEx::CreateSubControl ( int nClassType )
{

	{	//	æ∆¿Ã≈€ ¿ÃπÃ¡ˆ ( 0 - 11 )
		CString strInvenWearItem[ITEM_IMAGE_SIZE] = 
		{
			"CHARACTER_WINDOW_WEAR_SLOT_HEAD",
			"CHARACTER_WINDOW_WEAR_SLOT_NECK",
			"CHARACTER_WINDOW_WEAR_SLOT_UPPER",
			"CHARACTER_WINDOW_WEAR_SLOT_EX_FIRST_LEFT_WEAPON",

			"CHARACTER_WINDOW_WEAR_SLOT_WRIST",
			"CHARACTER_WINDOW_WEAR_SLOT_HAND",
			"CHARACTER_WINDOW_WEAR_SLOT_LOWER",
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING",
			"CHARACTER_WINDOW_WEAR_SLOT_EX_FIRST_RIGHT_WEAPON",

			"CHARACTER_WINDOW_WEAR_SLOT_FOOT",
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING",

			"CHARACTER_WINDOW_WEAR_SLOT_EX_SECOND_RIGHT_WEAPON",
			"CHARACTER_WINDOW_WEAR_SLOT_EX_SECOND_LEFT_WEAPON",
			"CHARACTER_WINDOW_WEAR_SLOT_VEHICLE",		//√∂
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY",//ª’?
			"CHARACTER_WINDOW_WEAR_SLOT_WAIST",		//ÕÕ√Ë?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY",
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER",
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER",
		};

		CString strInvenWearOver[ITEM_IMAGE_SIZE] = 
		{
			"CHARACTER_WINDOW_WEAR_SLOT_HEAD_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_NECK_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_UPPER_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_EX_FIRST_LEFT_WEAPON_OVER",

			"CHARACTER_WINDOW_WEAR_SLOT_WRIST_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_HAND_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_LOWER_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_EX_FIRST_RIGHT_WEAPON_OVER",

			"CHARACTER_WINDOW_WEAR_SLOT_FOOT_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING_OVER",

			"CHARACTER_WINDOW_WEAR_SLOT_EX_SECOND_RIGHT_WEAPON_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_EX_SECOND_LEFT_WEAPON_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_VEHICLE_OVER",		//√∂
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY_OVER",//ª’?
			"CHARACTER_WINDOW_WEAR_SLOT_WAIST_OVER",		//ÕÕ√Ë?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_ACCESSORY_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_FINGER_OVER",
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_FINGER_OVER",
		};


				CString InventoryText[ITEM_IMAGE_SIZE] = 
		{
			"CHARACTER_WINDOW_WEAR_SLOT_HEAD_NUMBER",			//À—?
			"CHARACTER_WINDOW_WEAR_SLOT_NECK_NUMBER",			// √ÈÕ?
			"CHARACTER_WINDOW_WEAR_SLOT_UPPER_NUMBER",		//‡ ◊È?
			"CHARACTER_WINDOW_WEAR_SLOT_EX_FIRST_LEFT_WEAPON_NUMBER",	//¬—?°√– ÿπ
			"CHARACTER_WINDOW_WEAR_SLOT_WRIST_NUMBER",		//µË“ßÀŸ
			"CHARACTER_WINDOW_WEAR_SLOT_HAND_NUMBER",			//∂ÿß¡◊Õ
			"CHARACTER_WINDOW_WEAR_SLOT_LOWER_NUMBER",		//°“ß‡°ß
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_EARRING_NUMBER",	//·À«π1
			"CHARACTER_WINDOW_WEAR_SLOT_EX_FIRST_RIGHT_WEAPON_NUMBER",	//Õ“«ÿ?
			"CHARACTER_WINDOW_WEAR_SLOT_FOOT_NUMBER",			//√Õß‡∑È?
			"CHARACTER_WINDOW_WEAR_SLOT_RIGHT_EARRING_NUMBER",//·À«π2
			"CHARACTER_WINDOW_WEAR_SLOT_EX_SECOND_RIGHT_WEAPON_NUMBER",
			"CHARACTER_WINDOW_WEAR_SLOT_EX_SECOND_LEFT_WEAPON_NUMBER",
			"CHARACTER_WINDOW_WEAR_SLOT_VEHICLE_NUMBER",		//√∂
			"CHARACTER_WINDOW_WEAR_SLOT_LEFT_ACCESSORY_NUMBER",//ª’?
			"CHARACTER_WINDOW_WEAR_SLOT_WAIST_NUMBER",		//ÕÕ√Ë?
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

	m_pMouseOver = CreateControl ( "ITEM_MOUSE_OVER" );
//	m_pSlotDisplay = CreateControl ( "TRADE_INVENTORY_OVER_0" );
//	m_pSlotBlockDisplay = CreateControl ( "TRADE_OVER_0" );

	CString strButton[2] =
	{
		"SWITCH_BUTTONA",
		"SWITCH_BUTTONB"
	};

	CString strButtonSet[2] =
	{
		"SWITCH_BUTTONA_F",
		"SWITCH_BUTTONB_F"
	};

	for ( int i = 0; i < 2 ; i++ )
	{
		m_Button[i] = new CUIControl;
		m_Button[i]->CreateSub ( this, strButton[i].GetString (), UI_FLAG_DEFAULT, SWITCH_BUTTONA + i );
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_Button[i]->SetTransparentOption( TRUE );
		RegisterControl ( m_Button[i] );

		m_ButtonSet[i] = new CUIControl;
		m_ButtonSet[i]->CreateSub ( this, strButtonSet[i].GetString (), UI_FLAG_DEFAULT );	
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
		m_ButtonSet[i]->SetTransparentOption( TRUE );
		RegisterControl ( m_ButtonSet[i] );
	}

	/*m_pAButton = new CBasicTextButton;
	m_pAButton->CreateSub ( this, "BASIC_TEXT_BUTTON161", UI_FLAG_XSIZE, SWITCH_BUTTONA );
	m_pAButton->CreateBaseButton(	"SWITCH_BUTTONA_F",
									CBasicTextButton::SIZE16s, 
									CBasicButton::RADIO_FLIP, 
									(char*)ID2GAMEWORD("INVEN_TAB_BUTTON",0) );
	m_pAButton->SetFlip ( TRUE );
	RegisterControl ( m_pAButton );

	m_pBButton = new CBasicTextButton;
	m_pBButton->CreateSub ( this, "BASIC_TEXT_BUTTON161", UI_FLAG_XSIZE, SWITCH_BUTTONB );
	m_pBButton->CreateBaseButton(	"SWITCH_BUTTONB_F",
									CBasicTextButton::SIZE16s, 
									CBasicButton::RADIO_FLIP,
									(char*)ID2GAMEWORD("INVEN_TAB_BUTTON",1) );
	m_pBButton->SetFlip ( TRUE );
	RegisterControl ( m_pBButton );*/

	m_pARHandSlot = CreateControl("CHARACTER_WINDOW_WEAR_SLOT_EX_FIRST_RIGHT_WEAPON_OVER");
	m_pALHandSlot = CreateControl("CHARACTER_WINDOW_WEAR_SLOT_EX_FIRST_LEFT_WEAPON_OVER");
	m_pBRHandSlot = CreateControl("CHARACTER_WINDOW_WEAR_SLOT_EX_SECOND_RIGHT_WEAPON_OVER");
	m_pBLHandSlot = CreateControl("CHARACTER_WINDOW_WEAR_SLOT_EX_SECOND_LEFT_WEAPON_OVER");






//	CreateNumberBox ();
}

CUIControl*	CInventoryPageWearEx::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );

	return pControl;
}

CItemImage*	CInventoryPageWearEx::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );

	return pItemImage;
}

EMSLOT CInventoryPageWearEx::IMAGE2EMSLOT ( int nIndex )
{
	switch ( nIndex )
	{	
	case ITEM_IMAGE0:	return SLOT_HEADGEAR;	//	∏”∏Æ
	case ITEM_IMAGE1:	return SLOT_NECK;		//	∏Ò∞…¿Ã
	case ITEM_IMAGE2:	return SLOT_UPPER;		//	ªÛ¿«
	case ITEM_IMAGE3:	return SLOT_LHAND;		//	øﬁº’µµ±∏
	case ITEM_IMAGE4:	return SLOT_WRIST;		//	º’∏Ò
	case ITEM_IMAGE5:	return SLOT_HAND;		//	¿Â∞©
	case ITEM_IMAGE6:	return SLOT_LOWER;		//	«œ¿«
	case ITEM_IMAGE7:	return SLOT_LFINGER;	//	øﬁº’ º’∞°∂Ù
	case ITEM_IMAGE8:	return SLOT_RHAND;		//	ø¿∏•º’µµ±∏
	case ITEM_IMAGE9:	return SLOT_FOOT;		//	Ω≈πﬂ
	case ITEM_IMAGE10:	return SLOT_RFINGER;	//	ø¿∏•º’ º’∞°∂Ù	
	case ITEM_IMAGE11:	return SLOT_RHAND_S;	//	ø¿∏•º’µµ±∏, ±ÿ∞≠∫Œ
	case ITEM_IMAGE12:	return SLOT_LHAND_S;	//	øﬁº’µµ±∏, ±ÿ∞≠∫Œ
	case ITEM_IMAGE13:	return SLOT_VEHICLE;	//  ≈ª∞Õ
	case ITEM_IMAGE14:	return SLOT_WING;
	case ITEM_IMAGE15:	return SLOT_AURA;
	case ITEM_IMAGE16:	return SLOT_BELT;
	case ITEM_IMAGE17:	return SLOT_EARRING;
	case ITEM_IMAGE18:	return SLOT_ACCESSORY;
	case ITEM_IMAGE19:	return SLOT_CON;
	}

	GASSERT ( 0 && "????? ??????????." );


	return SLOT_TSIZE;
}

void CInventoryPageWearEx::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
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

	for ( int i = 0; i < SLOT_NSIZE_S_2; i++ )
	{
		const SITEMCUSTOM& sItemCustomOld = GetItem ( i );

		EMSLOT emSlot = IMAGE2EMSLOT ( i + ITEM_IMAGE0 );
		SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_SLOT_ITEM ( emSlot );





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

void CInventoryPageWearEx::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
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
	case ITEM_IMAGE12:	
	case ITEM_IMAGE13:
		//add wing
	case ITEM_IMAGE14:
		//add aura
	case ITEM_IMAGE15:
	case ITEM_IMAGE16:
	case ITEM_IMAGE17:
	case ITEM_IMAGE18:
	case ITEM_IMAGE19:
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

					AddMessageEx ( UIMSG_MOUSEIN_WEARSLOTEX | UIMSG_TOTOPPARENT );
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

	case SWITCH_BUTTONA:
		{
			if( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				SWITCH1();
				GLGaeaClient::GetInstance().GetCharacter()->ReqSlotChange();
			}
		}
		break;
	case SWITCH_BUTTONB:
		{
			if( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				SWITCH2();
				GLGaeaClient::GetInstance().GetCharacter()->ReqSlotChange();
			}
		}
		break;

	/*case TEXT_A_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqSlotChange();
			}
		}
		break;

	case TEXT_B_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqSlotChange();
			}
		}
		break;*/
	}
}
	

EMSLOT CInventoryPageWearEx::GetItemSlot ()
{
	return IMAGE2EMSLOT ( GetItemIndex () + ITEM_IMAGE0 );
}

void CInventoryPageWearEx::LoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom )
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

SITEMCUSTOM& CInventoryPageWearEx::GetItem ( int nIndex )
{
	return m_ItemCustomArray[nIndex];
}

void CInventoryPageWearEx::UnLoadItem ( int nIndex )
{
	m_ItemCustomArray[nIndex].sNativeID = NATIVEID_NULL ();
	m_pItemImage[nIndex]->ResetItem ();
	m_pNumberBoxArray[nIndex]->ClearText ();
	//m_pNumberBoxArray[nIndex]->SetVisibleSingle ( FALSE );
}
CBasicTextBox* CInventoryPageWearEx::CreateNumberBox ( const char* szControl )
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
    pTextBox->SetFont ( pFont8 );
	pTextBox->SetTextAlign ( TEXT_ALIGN_RIGHT );
	RegisterControl ( pTextBox );

	return pTextBox;
}

void CInventoryPageWearEx::SWITCH1()
{
	for ( int i = 0; i < 2; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[0]->SetVisibleSingle ( TRUE );
	m_Button[0]->SetVisibleSingle ( FALSE );

	m_pARHandSlot->SetVisibleSingle( FALSE );
	m_pALHandSlot->SetVisibleSingle( FALSE );
	m_pBRHandSlot->SetVisibleSingle( TRUE );
	m_pBLHandSlot->SetVisibleSingle( TRUE );
}
void CInventoryPageWearEx::SWITCH2()
{
	for ( int i = 0; i < 2; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[1]->SetVisibleSingle ( TRUE );
	m_Button[1]->SetVisibleSingle ( FALSE );

	m_pARHandSlot->SetVisibleSingle( TRUE );
	m_pALHandSlot->SetVisibleSingle( TRUE );
	m_pBRHandSlot->SetVisibleSingle( FALSE );
	m_pBLHandSlot->SetVisibleSingle( FALSE );
}

void CInventoryPageWearEx::SetTabButton( BOOL bMain )
{
	if( bMain )
	{
		for ( int i = 0; i < 2; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[0]->SetVisibleSingle ( TRUE );
	m_Button[0]->SetVisibleSingle ( FALSE );



		m_pARHandSlot->SetVisibleSingle( FALSE );
		m_pALHandSlot->SetVisibleSingle( FALSE );
		m_pBRHandSlot->SetVisibleSingle( TRUE );
		m_pBLHandSlot->SetVisibleSingle( TRUE );
	}
	else
	{
		for ( int i = 0; i < 2; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[1]->SetVisibleSingle ( TRUE );
	m_Button[1]->SetVisibleSingle ( FALSE );



		m_pARHandSlot->SetVisibleSingle( TRUE );
		m_pALHandSlot->SetVisibleSingle( TRUE );
		m_pBRHandSlot->SetVisibleSingle( FALSE );
		m_pBLHandSlot->SetVisibleSingle( FALSE );

	}
}