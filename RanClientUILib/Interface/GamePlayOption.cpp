#include "StdAfx.h"
#include "GamePlayOption.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "../EngineLib/DxCommon/DxFontMan.h"

#include "DxParamSet.h"
#include "RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicGamePlayOption::CBasicGamePlayOption () :
	m_pFont ( NULL ),
	m_pConftButton ( NULL ),
	m_pTradeButton ( NULL ),
	m_pPartyButton ( NULL ),
	m_pSimpleHPButton ( NULL ),
	m_pFriendButton ( NULL ),
	m_pMovableOnChatButton ( NULL ),
	m_pShowTipButton ( NULL ),

	m_pForcedAttackButton ( NULL ),
	m_pNameDisplayButton ( NULL ),
	m_pSetDisplayButton ( NULL ),
	m_pRevivalButton ( NULL ),
	m_pHelpPopUpButton ( NULL ),
	m_pIdleAnimButton ( NULL ),
	m_pRankNameButton ( NULL ),
	m_pRankMarkButton ( NULL ),
	m_pAllowBetButton ( NULL ) //add duel bet
{
}

CBasicGamePlayOption::~CBasicGamePlayOption ()
{
}

void CBasicGamePlayOption::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	CBasicTextBox* pTextBox = NULL;

	{	//	�⺻ �ؽ�Ʈ		
		DWORD dwFontColor = NS_UITEXTCOLOR::DEFAULT;
		int nAlign = TEXT_ALIGN_LEFT;

		//	�ڵ� ��� �ź�
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_CONFT_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 0 ) );

		//	�ڵ� �ŷ� �ź�
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_TRADE_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 1 ) );

		//	�ڵ� ��Ƽ �ź�
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_PARTY_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 2 ) );

		//	�̴� HP ǥ��
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_SIMPLEHP_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 3 ) );

		//	�ڵ� ģ�� �ź�
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_FRIEND_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 4 ) );

		//	����â �⺻ ��ġ��
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_DEFAULTPOS_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 5 ) );

		//  ä��â �ձ�
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 6 ) );

		//  �� ����
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_SHOW_TIP_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 7 ) );

		//  ���� ����
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_FORCED_ATTACK_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 8 ) );

		//  �̸� �׻� ǥ��
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_NAME_DISPLAY_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 9 ) );

		//  ��Ȱ ��ų ����
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_REVIVAL_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 10 ) );

		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_SIMPLIFY_CHARACTER_STATIC", m_pFont, dwFontColor, nAlign ); //add pkrank
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 11 ) );

		//jdev help
		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_HELP_POPUP_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 15 ) );

		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_IDLE_ANIM_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 16 ) );

		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_RANK_NAME_STATIC", m_pFont, dwFontColor, nAlign ); //add pkrank
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 17 ) );

		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_RANK_MARK_STATIC", m_pFont, dwFontColor, nAlign ); //add pkrank
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 18 ) );

		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_ALLOW_BET_STATIC", m_pFont, dwFontColor, nAlign ); //add duel bet
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 19 ) );

		pTextBox = CreateStaticControl ( "HWOPTION_GAMEPLAY_SET_REMOVE_STATIC", m_pFont, dwFontColor, nAlign ); //add duel bet
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 20 ) );
	}

	{	//	���
		//	��ư
		m_pConftButton = CreateFlipButton( "HWOPTION_GAMEPLAY_CONFT_BUTTON", "HWOPTION_GAMEPLAY_CONFT_BUTTON_F", HWOPTION_GAMEPLAY_CONFT_BUTTON );
		m_pTradeButton = CreateFlipButton( "HWOPTION_GAMEPLAY_TRADE_BUTTON", "HWOPTION_GAMEPLAY_TRADE_BUTTON_F", HWOPTION_GAMEPLAY_TRADE_BUTTON );
		m_pPartyButton = CreateFlipButton( "HWOPTION_GAMEPLAY_PARTY_BUTTON", "HWOPTION_GAMEPLAY_PARTY_BUTTON_F", HWOPTION_GAMEPLAY_PARTY_BUTTON );
		m_pSimpleHPButton = CreateFlipButton( "HWOPTION_GAMEPLAY_SIMPLEHP_BUTTON", "HWOPTION_GAMEPLAY_SIMPLEHP_BUTTON_F", HWOPTION_GAMEPLAY_SIMPLEHP_BUTTON );
		m_pFriendButton = CreateFlipButton( "HWOPTION_GAMEPLAY_FRIEND_BUTTON", "HWOPTION_GAMEPLAY_FRIEND_BUTTON_F", HWOPTION_GAMEPLAY_FRIEND_BUTTON );
		m_pMovableOnChatButton = CreateFlipButton( "HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_BUTTON", "HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_BUTTON_F", HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_BUTTON );
		m_pShowTipButton = CreateFlipButton( "HWOPTION_GAMEPLAY_SHOW_TIP_BUTTON", "HWOPTION_GAMEPLAY_SHOW_TIP_BUTTON_F", HWOPTION_GAMEPLAY_SHOW_TIP_BUTTON );
		m_pForcedAttackButton = CreateFlipButton( "HWOPTION_GAMEPLAY_FORCED_ATTACK_BUTTON", "HWOPTION_GAMEPLAY_FORCED_ATTACK_BUTTON_F", HWOPTION_GAMEPLAY_FORCED_ATTACK_BUTTON );
		m_pNameDisplayButton = CreateFlipButton( "HWOPTION_GAMEPLAY_NAME_DISPLAY_BUTTON", "HWOPTION_GAMEPLAY_NAME_DISPLAY_BUTTON_F", HWOPTION_GAMEPLAY_NAME_DISPLAY_BUTTON );
		m_pSetDisplayButton = CreateFlipButton( "HWOPTION_GAMEPLAY_SET_DISPLAY_BUTTON", "HWOPTION_GAMEPLAY_SET_DISPLAY_BUTTON_F", HWOPTION_GAMEPLAY_SET_DISPLAY_BUTTON );
		m_pRevivalButton = CreateFlipButton( "HWOPTION_GAMEPLAY_REVIVAL_BUTTON", "HWOPTION_GAMEPLAY_REVIVAL_BUTTON_F", HWOPTION_GAMEPLAY_REVIVAL_BUTTON );
		m_bAutoTargetButton = CreateFlipButton( "HWOPTION_GAMEPLAY_AUTOTARGET_BUTTON", "HWOPTION_GAMEPLAY_AUTOTARGET_BUTTON_F", HWOPTION_GAMEPLAY_AUTOTARGET_BUTTON );

		//jdev help
		m_pHelpPopUpButton = CreateFlipButton( "HWOPTION_GAMEPLAY_HELP_POPUP_BUTTON", "HWOPTION_GAMEPLAY_HELP_POPUP_BUTTON_F", HWOPTION_GAMEPLAY_HELP_POPUP_BUTTON );

		m_pIdleAnimButton = CreateFlipButton( "HWOPTION_GAMEPLAY_IDLE_ANIM_BUTTON", "HWOPTION_GAMEPLAY_IDLE_ANIM_BUTTON_F", HWOPTION_GAMEPLAY_IDLE_ANIM_BUTTON );

		m_pRankNameButton = CreateFlipButton( "HWOPTION_GAMEPLAY_RANK_NAME_BUTTON", "HWOPTION_GAMEPLAY_RANK_NAME_BUTTON_F", HWOPTION_GAMEPLAY_RANK_NAME_BUTTON ); //add pkrank
		m_pRankMarkButton = CreateFlipButton( "HWOPTION_GAMEPLAY_RANK_MARK_BUTTON", "HWOPTION_GAMEPLAY_RANK_MARK_BUTTON_F", HWOPTION_GAMEPLAY_RANK_MARK_BUTTON ); //add pkrank

		m_pAllowBetButton = CreateFlipButton( "HWOPTION_GAMEPLAY_ALLOW_BET_BUTTON", "HWOPTION_GAMEPLAY_ALLOW_BET_BUTTON_F", HWOPTION_GAMEPLAY_ALLOW_BET_BUTTON ); //add duel bet



		{
			CBasicButton* pButton = new CBasicButton;
			pButton->CreateSub ( this, "HWOPTION_GAMEPLAY_DEFAULTPOS_BUTTON", UI_FLAG_DEFAULT, HWOPTION_GAMEPLAY_DEFAULTPOS_BUTTON );
			pButton->CreateFlip ( "HWOPTION_GAMEPLAY_DEFAULTPOS_BUTTON_F", CBasicButton::CLICK_FLIP );				
			RegisterControl ( pButton );
		}
	}
}

CBasicButton* CBasicGamePlayOption::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );
	return pButton;
}

CBasicTextBox* CBasicGamePlayOption::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );
	return pStaticText;
}

void CBasicGamePlayOption::LoadConft ()
{
	m_bConft = DXPARAMSET::GetInstance().m_bDIS_CONFT;
	m_pConftButton->SetFlip ( m_bConft );
}

void CBasicGamePlayOption::LoadTrade ()
{
	m_bTrade = DXPARAMSET::GetInstance().m_bDIS_TRADE;
	m_pTradeButton->SetFlip ( m_bTrade );
}

void CBasicGamePlayOption::LoadParty ()
{
	m_bParty = DXPARAMSET::GetInstance().m_bDIS_PARTY;
	m_pPartyButton->SetFlip ( m_bParty );
}

void CBasicGamePlayOption::LoadSimpleHP ()
{
	m_bSimpleHP = DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP;
	m_pSimpleHPButton->SetFlip ( m_bSimpleHP );
}

void CBasicGamePlayOption::LoadFriend ()
{
	m_bFriend = RANPARAM::bDIS_FRIEND;
	m_pFriendButton->SetFlip ( m_bFriend );
}

void CBasicGamePlayOption::LoadMovableOnChat ()
{
	m_bMovableOnChat = RANPARAM::bMOVABLE_ON_CHAT;
	m_pMovableOnChatButton->SetFlip ( m_bMovableOnChat );
}

void CBasicGamePlayOption::LoadShowTip ()
{
	m_bShowTip = RANPARAM::bSHOW_TIP;
	m_pShowTipButton->SetFlip ( m_bShowTip );
}

void CBasicGamePlayOption::LoadForcedAttack ()
{
	m_bForcedAttack = RANPARAM::bFORCED_ATTACK;
	m_pForcedAttackButton->SetFlip ( m_bForcedAttack );
}

void CBasicGamePlayOption::LoadNameDisplay ()
{
	m_bNameDisplay = RANPARAM::bNAME_DISPLAY;
	m_pNameDisplayButton->SetFlip ( m_bNameDisplay );
}

void CBasicGamePlayOption::LoadSetDisplay ()
{
	m_bSetDisplay = RANPARAM::bSET_DISPLAY;
	m_pSetDisplayButton->SetFlip ( m_bSetDisplay );
}

void CBasicGamePlayOption::LoadRevival()
{
	m_bNon_Rebirth = RANPARAM::bNON_Rebirth;
	m_pRevivalButton->SetFlip ( m_bNon_Rebirth );
}
//jdev help
void CBasicGamePlayOption::LoadHelpPopUp()
{
	m_bHelpPopUp = RANPARAM::bHelpPopUp;
	m_pHelpPopUpButton->SetFlip ( m_bHelpPopUp );
}

void CBasicGamePlayOption::LoadIdleAnim()
{
	m_bIdleAnim = RANPARAM::bIdleAnim;
	m_pIdleAnimButton->SetFlip ( m_bIdleAnim );
}
void CBasicGamePlayOption::LoadAutoTarget() // autopilot
{
	m_bAutoTarget = RANPARAM::bAutoTarget;
	m_bAutoTargetButton->SetFlip ( m_bAutoTarget );
}
void CBasicGamePlayOption::LoadRankName() //add pkrank
{
	m_bRankName = RANPARAM::bShowRankName;
	m_pRankNameButton->SetFlip ( m_bRankName );
}


void CBasicGamePlayOption::LoadRankMark() //add pkrank
{
	m_bRankMark = RANPARAM::bShowRankMark;
	m_pRankMarkButton->SetFlip ( m_bRankMark );
}

void CBasicGamePlayOption::LoadAllowBet() //add duel bet
{
	m_bAllowBet = RANPARAM::bAllowBet;
	m_pAllowBetButton->SetFlip ( m_bAllowBet );
}


void CBasicGamePlayOption::LoadCurrentOption()
{
	LoadConft();
	LoadTrade();
	LoadParty();
	LoadSimpleHP();
	LoadFriend();
	LoadMovableOnChat();
	LoadShowTip();
	LoadForcedAttack();
	LoadNameDisplay();
	LoadSetDisplay();
	LoadRevival();
	//jdev help
	LoadHelpPopUp();
	LoadIdleAnim();
	LoadRankName(); //add pkrank
	LoadRankMark(); //add pkrank
	LoadAllowBet(); //add duel bet
}

void CBasicGamePlayOption::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadCurrentOption();
	}
}