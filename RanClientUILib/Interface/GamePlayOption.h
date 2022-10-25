#pragma	once

#include "../EngineUILib/GUInterface/UIGroup.h"

class	CBasicTextBox;
class	CBasicButton;
class	CD3DFontPar;

class	CBasicGamePlayOption : public CUIGroup
{
protected:
	enum
	{
		HWOPTION_GAMEPLAY_CONFT_BUTTON = NO_ID + 1,		//	'자동' 대련 거부
		HWOPTION_GAMEPLAY_TRADE_BUTTON,					//	'자동' 거래 거부
		HWOPTION_GAMEPLAY_PARTY_BUTTON,					//	'자동' 파티 거부
		HWOPTION_GAMEPLAY_SIMPLEHP_BUTTON,				//
		HWOPTION_GAMEPLAY_FRIEND_BUTTON,				//	자동 친구 요청 거부
		HWOPTION_GAMEPLAY_DEFAULTPOS_BUTTON,			//	디폴트 위치이동
		HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_BUTTON,		//	채팅창 위에서도 움직이기
		HWOPTION_GAMEPLAY_SHOW_TIP_BUTTON,				//	팁 보여주기
		HWOPTION_GAMEPLAY_FORCED_ATTACK_BUTTON,			// 강제 공격
		HWOPTION_GAMEPLAY_NAME_DISPLAY_BUTTON,			// 이름 항상 표시
		HWOPTION_GAMEPLAY_SET_DISPLAY_BUTTON,			// 이름 항상 표시
		HWOPTION_GAMEPLAY_REVIVAL_BUTTON,			// 부활 스킬 금지 유무
		//jdev help
		HWOPTION_GAMEPLAY_HELP_POPUP_BUTTON,
		HWOPTION_GAMEPLAY_IDLE_ANIM_BUTTON,

		HWOPTION_GAMEPLAY_RANK_NAME_BUTTON, //add pkrank
		HWOPTION_GAMEPLAY_RANK_MARK_BUTTON, //add pkrank
		HWOPTION_GAMEPLAY_ALLOW_BET_BUTTON, //add duel bet
	};

public:
	CBasicGamePlayOption ();
	virtual	~CBasicGamePlayOption ();

public:
	void	CreateSubControl ();

private:
	CBasicButton*	CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR D3DCOLOR, int nAlign );

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	LoadCurrentOption();

private:
	void	LoadConft ();
	void	LoadTrade ();
	void	LoadParty ();
	void	LoadSimpleHP ();
	void	LoadFriend ();
	void	LoadMovableOnChat ();
	void	LoadShowTip ();
	void	LoadForcedAttack ();
	void	LoadNameDisplay ();
	void	LoadSetDisplay();//Added by Eugene Saya  5/10/16
	void	LoadRevival ();
	//jdev help
	void	LoadHelpPopUp ();
	void	LoadIdleAnim();

	void	LoadRankName(); //add pkrank
	void	LoadRankMark(); //add pkrank

	void	LoadAllowBet(); //add duel bet

private:
	CD3DFontPar*	m_pFont;	

private:
	CBasicButton*		m_pConftButton;
	CBasicButton*		m_pTradeButton;
	CBasicButton*		m_pPartyButton;
	CBasicButton*		m_pSimpleHPButton;
	CBasicButton*		m_pFriendButton;
	CBasicButton*		m_pMovableOnChatButton;
	CBasicButton*		m_pShowTipButton;
	CBasicButton*		m_pForcedAttackButton;
	CBasicButton*		m_pNameDisplayButton;
	CBasicButton*		m_pSetDisplayButton;//Added by Eugene Saya  5/10/16
	CBasicButton*		m_pRevivalButton;
	//jdev help
	CBasicButton*		m_pHelpPopUpButton;
	CBasicButton*		m_pIdleAnimButton;

	CBasicButton*		m_pRankNameButton;//add pkrank
	CBasicButton*		m_pRankMarkButton;//add pkrank
	CBasicButton*		m_pAllowBetButton;//add duel bet

public:
	BOOL m_bConft;
	BOOL m_bTrade;
	BOOL m_bParty;
	BOOL m_bSimpleHP;
	BOOL m_bFriend;
	BOOL m_bMovableOnChat;
	BOOL m_bShowTip;
	BOOL m_bForcedAttack;
	BOOL m_bNameDisplay;
	BOOL m_bSetDisplay;//Added by Eugene Saya  5/10/16
	BOOL m_bNon_Rebirth;
	//jdev help
	BOOL m_bHelpPopUp;
	BOOL m_bIdleAnim;

	BOOL m_bRankName;//add pkrank
	BOOL m_bRankMark;//add pkrank

	BOOL m_bAllowBet;//add duel bet
};