#pragma	once

#include "../EngineUIlib/GUInterface/UIWindow.h"
#include "../Interface/UIWindowEx.h"
class	CBasicTextBox;
class	CBasicTextButton;

//	ISREVIVE ()

class CRebirthDialogue : public CUIWindowEx
{
	static const float fDEFAULT_TIME_LEFT;
	static const float fDEFAULT_SELECT_TIME_LEFT;

private:
	enum
	{
		REBIRTH_CHECK_POINT = ET_CONTROL_NEXT,
		REBIRTH_CURRENT_POSITION,
		REBIRTH_RECOVERY_EXP,
		REBIRTH_DIALOGUE_BUY,
		LOGO_SFX_BUTTON_F2,
		CHECK_BUFF,
		CHECK_BUFF1
	};	

public:
	CRebirthDialogue ();
	virtual	~CRebirthDialogue ();

public:
	void	CreateSubControl ();
	CBasicTextBox*		CreateStaticControl ( char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	CBasicButton*	CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID );
	CBasicTextButton* CreateTextButton ( char* szButton, UIGUID ControlID , char* szText );
	CBasicTextButton*	CreateTextButton24 ( char* szButton, UIGUID ControlID , char* szText );	
public:
	void	SET_ITEMREBIRTH ( const bool& bItemRebirth, bool bAutoRebirth, bool bCantUseRebirth );

private:
	void	ShowButton ();
	void	HideButton ();	
	CBasicTextButton*	CreateTextButton23 ( char* szButton, UIGUID ControlID , char* szText );	
	CBasicTextButton*	m_pADD;
	void	DoACTION ()							{ m_bDO_ACTION = true; }
	CUIControl*		m_bg_white;
public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetCallerID( UIGUID ControlID )		{ m_CallerID = ControlID; }
	UIGUID	GetCallerID()						{ return m_CallerID; }
	bool	IsRecoveryAction()					{ return ( m_nACTION == REBIRTH_RECOVERY_EXP ); }

private:
	CUIControl*	CreateDummyControl ( char* szKeyword );

private:
	CBasicButton*	m_pBuff;
	//CBasicButton*	m_pImage;
	CBasicButton*	m_pImage2;
	CBasicTextBox*		m_pTextBox;
	CBasicTextButton*	m_pCheckPointButton;
	CBasicTextButton*	m_pCurrentPositionButton;
	CBasicTextButton*	m_pRecoveryExpButton;

private:
	CUIControl*		m_pDummy_1BUTTON;
	CUIControl*		m_pDummy_2BUTTON_L;
	CUIControl*		m_pDummy_2BUTTON_R;

private:
	float	m_fTIME_LEFT;
	float	m_fSelTIME_LEFT;
	bool	m_bDO_ACTION;
	int		m_nACTION;

	bool	m_bDONE_ACTION;
	bool	m_bAutoRebirth;

	UIGUID	m_CallerID;
};