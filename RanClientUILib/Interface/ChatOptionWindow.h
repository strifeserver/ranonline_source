#pragma once

#include "../Interface/UIWindowEx.h"

class CBasicChatLeftBar;
class CBasicChatRightBody;
class CUIChatShowFlag;
class CBasicButton;

////////////////////////////////////////////////////////////////////
//	사용자 메시지 정의
//const DWORD UIMSG_MOUSEIN_BLOCK_CHARACTER_MOVE = UIMSG_USER1;
////////////////////////////////////////////////////////////////////

class	CChatOptionWindow : public CUIWindowEx
{
private:
	enum
	{
		LEFT_BAR = NO_ID + 1,
		RIGHT_BODY,
		CHAT_SHOW_FLAG,
		CHAT_FLAG_BUTTON,
	};

public:
	CChatOptionWindow ();
	virtual	~CChatOptionWindow ();

public:
	void	CreateSubControl ();

public:
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CBasicChatLeftBar*		m_pBasicChatLeftBar;
	CBasicChatRightBody*	m_pBasicChatRightBody;
private:
	BOOL		m_bFirstGap;
	D3DXVECTOR2	m_vGap;

	int			m_PosX;
	int			m_PosY;

	BOOL		m_bCheckButton;

private:
	CUIChatShowFlag* m_pUIChatShowFlag;
	CBasicButton*	m_pChatShowFlagButton;
};