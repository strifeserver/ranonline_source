#pragma	once

#include "../UIWindowEx.h"

class CBasicTextButton;
class CFriendWindowNew : public CUIWindowEx
{
protected:
	enum
	{
		BUTTON_FRIEND = ET_CONTROL_NEXT,
		BUTTON_BLOCK,
	};

public:
    CFriendWindowNew ();
	virtual	~CFriendWindowNew ();

public:
	void				CreateSubControl ();
	virtual	void		TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	CBasicTextButton*	CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText );

	void	SetPage(int nIndex);
public:
	CUIControl*								m_pBackGround;
	CBasicTextButton*						m_pButtonFriend;
	CBasicTextButton*						m_pButtonBlock;
};