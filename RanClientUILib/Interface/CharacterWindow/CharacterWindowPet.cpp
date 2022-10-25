#include "StdAfx.h"
#include "./CharacterWindowPet.h"
#include "CharacterWindowPetRender.h"

#include "../EngineUiLib/GUInterface/BasicTextBox.h"
#include "../EngineUiLib/GUInterface/BasicButton.h"
#include "../EngineUiLib/GUInterface/BasicButtonRadio.h"
#include "../EngineUiLib/GUInterface/BasicButtonText.h"
#include "../EngineLib/DxCommon/d3dfont.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../RanClientLib/G-Logic/GLGaeaClient.h"


#include "../InnerInterface.h"

#include "../BasicLineBox.h"
#include "../ItemImage.h"
#include "../BasicComboBox.h"
#include "../BasicComboBoxRollOver.h"
#include "../ItemMove.h"
#include "CharacterWindowPetQuickSkillSlot.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterWindowPet::CCharacterWindowPet () :
	m_pRender( NULL ),
	m_pPetName( NULL ),
	m_pPetType( NULL ),
	m_pPetFood( NULL ),
	m_pPetDualSkill( NULL ),
	m_pMouseOver( NULL ),
	m_pcbSkill0( NULL ),
	m_pcbSkill1( NULL ),
	m_pcbSkillRollOver0( NULL ),
	m_pcbSkillRollOver1( NULL ),
	m_pPetSkillSlot0( NULL ),
	m_pPetSkillSlot1( NULL ),
	m_pPetIcon( NULL ),
	m_pSkillButton( NULL ),
	m_pButtonLeft( NULL ),
	m_pButtonRight( NULL ),
	m_bVALIDPET( FALSE ),
	m_bDualSkillBack( FALSE ),
	m_nFoodLast( 0 ),
	m_RollOverID(NO_ID)
{
	SecureZeroMemory( m_pSlotDisplay, sizeof(m_pSlotDisplay) );
	SecureZeroMemory( m_pSlotImage, sizeof(m_pSlotImage) );
}

CCharacterWindowPet::~CCharacterWindowPet ()
{
}

CUIControl*	CCharacterWindowPet::CreateControl ( const char* szControl, const UIGUID& cID )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextBox*	CCharacterWindowPet::CreateTextBox ( char* szKeyword, CD3DFontPar* pFont )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szKeyword );
	pTextBox->SetFont ( pFont );
	RegisterControl ( pTextBox );

	return pTextBox;
}

CItemImage*	CCharacterWindowPet::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );

	return pItemImage;
}

void CCharacterWindowPet::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CHARACTER_WINDOW_PET_SETTING" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CHARACTER_WINDOW_PET_INVEN_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CHARACTER_WINDOW_PET_SKILL_LINE_A" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CHARACTER_WINDOW_PET_SKILL_LINE_B" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CHARACTER_WINDOW_PET_LINE_INFO" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "CHARACTER_WINDOW_PET_LINE_1" );
	RegisterControl ( pBasicLineBox );

	CreateControl( "CHARACTER_WINDOW_PET_MESHRENDER_LINE_REGION" );

	CBasicTextBox* pTextBox(NULL);

	pTextBox = CreateTextBox ( "CHARACTER_WINDOW_PET_NAME_STATIC", pFont8 );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC", 0) );
	pTextBox = CreateTextBox ( "CHARACTER_WINDOW_PET_TYPE_STATIC", pFont8 );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC", 1) );
	pTextBox = CreateTextBox ( "CHARACTER_WINDOW_PET_FOOD_STATIC", pFont8 );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC", 3), NS_UITEXTCOLOR::YELLOW );

	pTextBox = CreateTextBox ( "CHARACTER_WINDOW_PET_SKILL_STATIC_A", pFont8 );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC", 4) );
	pTextBox = CreateTextBox ( "CHARACTER_WINDOW_PET_SKILL_STATIC_B", pFont8 );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC", 5) );

	pTextBox = CreateTextBox ( "CHARACTER_WINDOW_PET_ITEM_STATIC_A", pFont8 );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC", 9) );
	pTextBox = CreateTextBox ( "CHARACTER_WINDOW_PET_ITEM_STATIC_B", pFont8 );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC", 10) );

	m_pPetName = CreateTextBox ( "CHARACTER_WINDOW_PET_NAME_TEXTBOX", pFont8 );
	m_pPetType = CreateTextBox ( "CHARACTER_WINDOW_PET_TYPE_TEXTBOX", pFont8 );
	m_pPetFood = CreateTextBox ( "CHARACTER_WINDOW_PET_FOOD_TEXTBOX", pFont8 );
	m_pPetDualSkill = CreateTextBox ( "CHARACTER_WINDOW_PET_DUALSKILL_TEXT", pFont8 );


	CString strPetSlot[SLOT_IMAGE_SIZE] = 
	{
		"CHARACTER_WINDOW_PET_SLOT0",
		"CHARACTER_WINDOW_PET_SLOT1",
	};

	CString strPetSlotOver[SLOT_IMAGE_SIZE] = 
	{
		"CHARACTER_WINDOW_PET_SLOT_OVER_IMAGE0",
		"CHARACTER_WINDOW_PET_SLOT_OVER_IMAGE1",
	};

	for ( int i = 0; i < SLOT_IMAGE_SIZE; i++ )
	{
		m_pSlotImage[i] = CreateItemImage( strPetSlot[i].GetString(), CHARACTER_PET_SLOT_IMAGE_TYPE1 + i );
		m_pSlotDisplay[i] = CreateControl( strPetSlotOver[i].GetString() );
		m_pSlotDisplay[i]->SetVisibleSingle( FALSE );
	}

	m_pMouseOver = CreateControl( "ITEM_MOUSE_OVER" );
	m_pMouseOver->SetVisibleSingle( FALSE );

	CBasicComboBox * pComboBox0 = new CBasicComboBox;
	pComboBox0->CreateSub( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_PET_SKILL_COMBO_OPEN0 );
	pComboBox0->CreateBaseComboBox( "CHARACTER_WINDOW_PET_SKILL_COMBO_OPEN0" );			
	RegisterControl( pComboBox0 );
	m_pcbSkill0 = pComboBox0;

	CBasicComboBox * pComboBox1 = new CBasicComboBox;
	pComboBox1->CreateSub( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_PET_SKILL_COMBO_OPEN1 );
	pComboBox1->CreateBaseComboBox( "CHARACTER_WINDOW_PET_SKILL_COMBO_OPEN1" );			
	RegisterControl( pComboBox1 );
	m_pcbSkill1 = pComboBox1;

	CBasicComboBoxRollOver * pComboBoxRollOver0 = new CBasicComboBoxRollOver;
	pComboBoxRollOver0->CreateSub( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_PET_SKILL_COMBO_ROLLOVER0 );
	pComboBoxRollOver0->CreateBaseComboBoxRollOver( "CHARACTER_WINDOW_PET_SKILL_COMBO_ROLLOVER0" );
	pComboBoxRollOver0->SetVisibleSingle( FALSE );
	RegisterControl( pComboBoxRollOver0 );
	m_pcbSkillRollOver0 = pComboBoxRollOver0;

	CBasicComboBoxRollOver * pComboBoxRollOver1 = new CBasicComboBoxRollOver;
	pComboBoxRollOver1->CreateSub( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_PET_SKILL_COMBO_ROLLOVER1 );
	pComboBoxRollOver1->CreateBaseComboBoxRollOver( "CHARACTER_WINDOW_PET_SKILL_COMBO_ROLLOVER1" );
	pComboBoxRollOver1->SetVisibleSingle( FALSE );
	RegisterControl( pComboBoxRollOver1 );
	m_pcbSkillRollOver1 = pComboBoxRollOver1;

	m_pPetSkillSlot0 = new CCharacterWindowPetQuickSkillSlot;
	m_pPetSkillSlot0->CreateSub( this, "CHARACTER_WINDOW_PET_SKILL_SLOT0", UI_FLAG_DEFAULT, CHARACTER_PET_SKILL_SLOT0 );
	m_pPetSkillSlot0->CreateSubControl();
	//m_pPetSkillSlot0->SetSkillSlot(0);
	RegisterControl ( m_pPetSkillSlot0 );

	m_pPetSkillSlot1 = new CCharacterWindowPetQuickSkillSlot;
	m_pPetSkillSlot1->CreateSub( this, "CHARACTER_WINDOW_PET_SKILL_SLOT1", UI_FLAG_DEFAULT, CHARACTER_PET_SKILL_SLOT1 );
	m_pPetSkillSlot1->CreateSubControl();
	//m_pPetSkillSlot1->SetSkillSlot(1);
	RegisterControl ( m_pPetSkillSlot1 );

	m_pPetIcon = CreateItemImage( "CHARACTER_WINDOW_PET_ICON", CHARACTER_PET_ICON );

	m_pSkillButton = new CBasicButtonText;
	m_pSkillButton->CreateRanButtonBlue ( this, "CHARACTER_WINDOW_PET_SKILL_BUTTON", CHARACTER_PET_SKILL_BUTTON );
	m_pSkillButton->CreateTextBox ( "CHARACTER_WINDOW_PET_SKILL_BUTTON_TEXTBOX", pFont8, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pSkillButton->SetOneLineText( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC", 11) );
	RegisterControl ( m_pSkillButton );

	m_pRender = new CCharacterWindowPetRender;
	m_pRender->CreateSub ( this, "CHARACTER_WINDOW_PET_MESHRENDER_REGION" );
	m_pRender->CreateSubControl ();
	RegisterControl ( m_pRender );

	m_pButtonLeft = new CBasicButton;
	m_pButtonLeft->CreateSub ( this, "CHARACTER_WINDOW_PET_ARROW_TURN_LEFT_DEFAULT", UI_FLAG_DEFAULT, CHARACTER_PET_BUTTON_LEFT );
	m_pButtonLeft->CreateFlip ( "CHARACTER_WINDOW_PET_ARROW_TURN_LEFT_CLICK", CBasicButton::CLICK_FLIP );
	m_pButtonLeft->CreateMouseOver ( "CHARACTER_WINDOW_PET_ARROW_TURN_LEFT_OVER" );
	m_pButtonLeft->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonLeft );

	m_pButtonRight = new CBasicButton;
	m_pButtonRight->CreateSub ( this, "CHARACTER_WINDOW_PET_ARROW_TURN_RIGHT_DEFAULT", UI_FLAG_DEFAULT, CHARACTER_PET_BUTTON_RIGHT );
	m_pButtonRight->CreateFlip ( "CHARACTER_WINDOW_PET_ARROW_TURN_RIGHT_CLICK", CBasicButton::CLICK_FLIP );
	m_pButtonRight->CreateMouseOver ( "CHARACTER_WINDOW_PET_ARROW_TURN_RIGHT_OVER" );
	m_pButtonRight->SetVisibleSingle( TRUE );
	RegisterControl ( m_pButtonRight );
}


void CCharacterWindowPet::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case CHARACTER_PET_SLOT_IMAGE_TYPE1:
	case CHARACTER_PET_SLOT_IMAGE_TYPE2:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{	
				int nIndex = ControlID - CHARACTER_PET_SLOT_IMAGE_TYPE1;

				if ( !CInnerInterface::GetInstance().IsFirstItemSlot () ) return ;

				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
				if ( !pItemMove )
				{
					GASSERT ( 0 && "CItemMove" );
					return ;
				}

				if ( pItemMove->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPos = m_pSlotImage[nIndex]->GetGlobalPos ();
					pItemMove->SetGlobalPos ( rcSlotPos );						

					AddMessageEx ( UIMSG_MOUSEIN_WEARSLOT | UIMSG_TOTOPPARENT );
				}				

				if ( pItemMove->GetItem () == NATIVEID_NULL () && m_pSlotImage[nIndex]->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcImagePos = m_pSlotImage[nIndex]->GetGlobalPos ();
					m_pMouseOver->SetGlobalPos ( rcImagePos );
					m_pMouseOver->SetVisibleSingle ( TRUE );
				}

				if ( dwMsg & UIMSG_LB_UP )
				{
					GLGaeaClient::GetInstance().GetPetClient()->ReqChangeAccessory( IMAGE2EMSLOT(ControlID) );
				}

				if ( dwMsg & UIMSG_RB_UP )
				{
					GLGaeaClient::GetInstance().GetPetClient()->ReqRemoveSlotItem( IMAGE2EMSLOT(ControlID) );
				}

				if( m_ItemCustomArray[nIndex].sNativeID != NATIVEID_NULL () )
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( m_ItemCustomArray[nIndex], FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
				}
			}
		}break;

	case CHARACTER_PET_SKILL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				if ( !CInnerInterface::GetInstance().IsVisibleGroup( PET_SKILL_WINDOW ) )
				{
					if( m_bVALIDPET )
						CInnerInterface::GetInstance().ShowGroupFocus( PET_SKILL_WINDOW );
				}
				else
				{
					CInnerInterface::GetInstance().HideGroup( PET_SKILL_WINDOW );
				}
			}
		}
		break;

	case CHARACTER_PET_SKILL_COMBO_OPEN0:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pcbSkillRollOver0->SetVisibleSingle( TRUE );
					m_pcbSkillRollOver0->SetScrollPercent ( 0.0f );

					m_RollOverID = CHARACTER_PET_SKILL_COMBO_ROLLOVER0;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_PET_SKILL_COMBO_OPEN1:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pcbSkillRollOver1->SetVisibleSingle( TRUE );
					m_pcbSkillRollOver1->SetScrollPercent ( 0.0f );

					m_RollOverID = CHARACTER_PET_SKILL_COMBO_ROLLOVER1;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_PET_SKILL_COMBO_ROLLOVER0:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );

				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{				
				int nIndex = m_pcbSkillRollOver0->GetSelectIndex ();
				if ( nIndex < 0 ) return ;

				SNATIVEID sID( m_pcbSkillRollOver0->GetTextData( nIndex ) );

				if( nIndex > 0 )
				{
					if( !GLGaeaClient::GetInstance().GetPetClient()->CheckSkill( sID ) )
					{
						return;
					}

					//if ( GLGaeaClient::GetInstance().GetPetClient()->m_sActiveSkillID[1] == sID )
					//{
					//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_DUALSKILL_INVALID_SKILL_SELECTED") );
					//	return;
					//}
				}

				CString & strText = m_pcbSkillRollOver0->GetSelectText( nIndex );
				m_pcbSkill0->SetText( strText );

				if( nIndex == 0 ) sID = NATIVEID_NULL();

				GLGaeaClient::GetInstance().GetPetClient()->ReqChangeActiveSkill( sID/*, 0*/ );
			}
		}break;

	case CHARACTER_PET_SKILL_COMBO_ROLLOVER1:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );

				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{				
				int nIndex = m_pcbSkillRollOver1->GetSelectIndex ();
				if ( nIndex < 0 ) return ;

				SNATIVEID sID( m_pcbSkillRollOver1->GetTextData( nIndex ) );

				//if( nIndex > 0 )
				//{
					//if( !GLGaeaClient::GetInstance().GetPetClient()->CheckSkill( sID ) )
					//{
					//	return;
					//}

					//if ( GLGaeaClient::GetInstance().GetPetClient()->m_sActiveSkillID[0] == sID )
					//{
					//	CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_REQ_DUALSKILL_INVALID_SKILL_SELECTED") );
					//	return;
					//}
				//}

				//CString & strText = m_pcbSkillRollOver1->GetSelectText( nIndex );
				//m_pcbSkill1->SetText( strText );

				//if( nIndex == 0 ) sID = NATIVEID_NULL();

				//GLGaeaClient::GetInstance().GetPetClient()->ReqChangeActiveSkill( sID, 1 );
			}
		}break;

	case CHARACTER_PET_BUTTON_RIGHT:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				if ( m_pRender )	m_pRender->m_vRot.x +=0.1f;
			}
		}break;
	case CHARACTER_PET_BUTTON_LEFT:
		{
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				if ( m_pRender )	m_pRender->m_vRot.x -=0.1f;
			}
		}break;
	}
}

void CCharacterWindowPet::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	BOOL bVALIDPET = GLGaeaClient::GetInstance().GetPetClient()->IsVALID();
	if ( bVALIDPET != m_bVALIDPET )
	{
		if ( bVALIDPET )
		{
			ShowPet();
		}
		else
		{
			ClearPet();
		}

		m_bVALIDPET = bVALIDPET;
	}

	if ( m_bVALIDPET )
	{
		int nFull = GLGaeaClient::GetInstance().GetPetClient()->m_nFull;
		if ( nFull != m_nFoodLast )
		{
			SetPetFood( nFull );
		}

		BOOL bDualPet = false; //GLGaeaClient::GetInstance().GetPetClient()->m_bDualSkill;
		if ( bDualPet != m_bDualSkillBack )
		{
			SetSkillCombo();
		}

		std::string strPetNAME = GLGaeaClient::GetInstance().GetPetClient()->m_szName;
		if ( m_strNameLast != strPetNAME )
		{
			SetName( strPetNAME );
		}
	}

	if( m_RollOverID == NO_ID )
	{
		if( m_pMouseOver ) m_pMouseOver->SetVisibleSingle ( FALSE );

		if ( m_pPetSkillSlot0 )	
		{
			if ( !m_pPetSkillSlot0->IsVisible() )
				m_pPetSkillSlot0->SetVisibleSingle( TRUE );
		}

		if ( m_pPetSkillSlot1 )
		{
			if ( !m_pPetSkillSlot1->IsVisible() )
				m_pPetSkillSlot1->SetVisibleSingle( TRUE );
		}

		CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
		SNATIVEID sHOLD_ITEM_ID(false);
		if ( pItemMove ) sHOLD_ITEM_ID = pItemMove->GetItem();

		for( int i=0; i<SLOT_IMAGE_SIZE; ++i )
		{
			SITEMCUSTOM & sItemCustomOld = m_ItemCustomArray[i];
			SITEMCUSTOM & sItemCustom = GLGaeaClient::GetInstance().GetPetClient()->m_PutOnItems[i];

			if ( sItemCustom != sItemCustomOld )
			{		
				if ( sItemCustom.sNativeID != NATIVEID_NULL() )
				{
					LoadItem ( i, sItemCustom );
				}
				else
				{
					UnLoadItem ( i );
				}
			}

			m_pSlotDisplay[i]->SetVisibleSingle ( FALSE );

			if( sHOLD_ITEM_ID != NATIVEID_NULL() )
			{
				if( GLGaeaClient::GetInstance().GetPetClient()->CheckSlotItem( sHOLD_ITEM_ID, IMAGE2EMSLOT( i + CHARACTER_PET_SLOT_IMAGE_TYPE1 ) ) )
				{
					const UIRECT& rcImagePos = m_pSlotImage[i]->GetGlobalPos ();
					D3DXVECTOR2 vPos( rcImagePos.left, rcImagePos.top );

					m_pSlotDisplay[i]->SetVisibleSingle( TRUE );
				}
			}
		}

		CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	}
	else
	{
		ResetMessageEx ();

		CUIControl::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		CUIControl* pControl = m_ControlContainer.FindControl ( m_RollOverID );
		if ( !pControl )
		{
			GASSERT ( 0 && "Oh Fuck!." );
			return ;
		}

		pControl->Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		DWORD dwControlMsg = pControl->GetMessageEx ();
		NS_UIDEBUGSET::BlockBegin ();
		if ( dwControlMsg ) TranslateUIMessage ( m_RollOverID, dwControlMsg );
		NS_UIDEBUGSET::BlockEnd ();

		if ( !(dwControlMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL) )
		{
			DWORD dwMsg = GetMessageEx ();
			if ( dwMsg & UIMSG_LB_UP )
			{
				if ( !m_bFirstLBUP )
				{
					m_RollOverID = NO_ID;
					pControl->SetVisibleSingle ( FALSE );					
				}
				m_bFirstLBUP = FALSE;
			}
		}

		if ( m_RollOverID == CHARACTER_PET_SKILL_COMBO_ROLLOVER0 )
		{
			if ( m_pPetSkillSlot1 )	
				m_pPetSkillSlot1->SetVisibleSingle( FALSE );
		}
	}
}

void CCharacterWindowPet::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	ClearPet();

	m_bVALIDPET = FALSE;
}

void CCharacterWindowPet::ClearRender()
{
	if( m_pRender )
	{
		m_pRender->ClearRender();
	}
}

void CCharacterWindowPet::ShowPet()
{
	std::string strPetName = GLGaeaClient::GetInstance().GetPetClient()->m_szName;
	SetName( strPetName.c_str() );

	PETTYPE emTYPE = GLGaeaClient::GetInstance().GetPetClient()->m_emTYPE;
	SetType( emTYPE );
	
	if ( m_pPetIcon )
	{
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( GLGaeaClient::GetInstance().GetCharacter()->m_sPetCardNativeID );
		if ( pItemData )
		{
			m_pPetIcon->SetItem ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );
		}
	}

	int nFull = GLGaeaClient::GetInstance().GetPetClient()->m_nFull;
	SetPetFood( nFull );
	SetSkillCombo();
}

void CCharacterWindowPet::ClearPet()
{
	if ( m_pPetName )	m_pPetName->ClearText();
	if ( m_pPetType )	m_pPetType->ClearText();
	if ( m_pPetFood )	m_pPetFood->ClearText();
	if ( m_pPetDualSkill )	m_pPetDualSkill->ClearText();

	for ( int i = 0; i < SLOT_IMAGE_SIZE; ++ i )
	{
		if ( m_pSlotImage[i] )
			m_pSlotImage[i]->ResetItem();

		m_ItemCustomArray[i] = SITEMCUSTOM();
	}

	if ( m_pcbSkillRollOver0 )	m_pcbSkillRollOver0->ClearText();
	if ( m_pcbSkillRollOver1 )	m_pcbSkillRollOver1->ClearText();
	if ( m_pPetIcon )	m_pPetIcon->ResetItem();

	m_pcbSkill0->SetText( "" );
	m_pcbSkill1->SetText( "" );

	ClearRender();
}

void CCharacterWindowPet::SetPetFood( int nFULL )
{
	if ( m_pPetFood )
	{
		CString strTemp;
		strTemp.Format( "%.2f%%", nFULL/10.0f );
		m_pPetFood ->SetOneLineText( strTemp, NS_UITEXTCOLOR::YELLOW );
	}

	m_nFoodLast = nFULL;
}

void CCharacterWindowPet::SetSkillCombo()
{
	bool bDualPet = false;//GLGaeaClient::GetInstance().GetPetClient()->m_bDualSkill;

	GASSERT( m_pcbSkillRollOver0 );
	GASSERT( m_pcbSkillRollOver1 );
	m_pcbSkillRollOver0->ClearText();
	m_pcbSkillRollOver1->ClearText();

	int nIndex = m_pcbSkillRollOver0->AddText( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC",8) );
	m_pcbSkillRollOver0->SetTextData( nIndex, SNATIVEID( false ).dwID );

	if ( bDualPet )
	{
		nIndex = m_pcbSkillRollOver1->AddText( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC",8) );
		m_pcbSkillRollOver1->SetTextData( nIndex, SNATIVEID( false ).dwID );
	}

	PETSKILL_MAP & mapPetSkill = GLGaeaClient::GetInstance().GetPetClient()->m_ExpSkills;

	PETSKILL_MAP_ITER pos = mapPetSkill.begin();
	PETSKILL_MAP_ITER end = mapPetSkill.end();
	for ( ; pos!=end; ++pos )
	{
		PETSKILL petSkill( (*pos).second );

		PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( petSkill.sNativeID );
		if( !pSKILL ) continue;

		int nIndex = m_pcbSkillRollOver0->AddText( pSKILL->GetName() );
		m_pcbSkillRollOver0->SetTextData( nIndex, petSkill.sNativeID.dwID );

		if ( bDualPet )
		{
			nIndex = m_pcbSkillRollOver1->AddText( pSKILL->GetName() );
			m_pcbSkillRollOver1->SetTextData( nIndex, petSkill.sNativeID.dwID );
		}
	}

	{
		SNATIVEID & sID = GLGaeaClient::GetInstance().GetPetClient()->m_sActiveSkillID/*[0]*/;
		PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( sID );
		if( !pSKILL )
		{
			m_pcbSkill0->SetText( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC",8) );
		}
		else
		{
			m_pcbSkill0->SetText( pSKILL->GetName() );
		}
	}

	if ( bDualPet )
	{
		/*SNATIVEID & sID = GLGaeaClient::GetInstance().GetPetClient()->m_sActiveSkillID[1];
		PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( sID );
		if( !pSKILL )
		{
			m_pcbSkill1->SetText( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC",8) );
		}
		else
		{
			m_pcbSkill1->SetText( pSKILL->GetName() );
		}*/
	}

	m_bDualSkillBack = bDualPet;

	if ( m_pPetDualSkill )
	{
		if( bDualPet )
		{
			m_pPetDualSkill->SetOneLineText( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC",6) );
		}
		else
		{
			m_pPetDualSkill->SetOneLineText( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC",7) );
		}
	}
}


void CCharacterWindowPet::LoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom )
{
	m_ItemCustomArray[nIndex] = ref_sItemCustom;

	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( ref_sItemCustom.sNativeID );
	m_pSlotImage[nIndex]->SetItem ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );
}

void CCharacterWindowPet::UnLoadItem ( int nIndex )
{
	m_ItemCustomArray[nIndex].sNativeID = NATIVEID_NULL ();
	m_pSlotImage[nIndex]->ResetItem ();
}

EMSUIT CCharacterWindowPet::IMAGE2EMSLOT ( UIGUID ControlID )
{
	switch ( ControlID )
	{	
	case CHARACTER_PET_SLOT_IMAGE_TYPE1:	return SUIT_PET_A;
	case CHARACTER_PET_SLOT_IMAGE_TYPE2:	return SUIT_PET_B;
	}

	GASSERT ( 0 && "CCharacterWindowPet::IMAGE2EMSLOT()" );
	return SUIT_NSIZE;
}

void CCharacterWindowPet::SetName( const std::string & strName )
{
	if ( m_pPetName )
	{
		m_pPetName->SetOneLineText( strName.c_str() );	

		m_strNameLast = strName.c_str();
	}
}

void CCharacterWindowPet::SetType( PETTYPE emTYPE )
{
	if ( m_pPetType )
	{
		if( emTYPE == PETTYPE_NONE )
		{
			m_pPetType->SetOneLineText( ID2GAMEWORD("CHARACTER_WINDOW_PET_TEXT_STATIC",2) );
		}else {
			m_pPetType->SetOneLineText( COMMENT::PET_TYPE[emTYPE].c_str() );
		}
	}
}
