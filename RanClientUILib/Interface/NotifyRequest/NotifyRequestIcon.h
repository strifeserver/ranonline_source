#pragma once

#include "../EngineUILib/GUInterface/UIGroup.h"

class CNotifyRequestIcon : public CUIGroup
{
private:
static	const	float	fBLINK_TIME_LOOP;

protected:
	enum
	{
		NOTIFY_REQUEST_ICON_DEFAULT = NO_ID + 1,
	};
public:
	CNotifyRequestIcon(void);
	~CNotifyRequestIcon(void);


public:
	void			CreateSubControl ();
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void	SET_ALARM ();

private:
	bool m_bAlarm;
	CUIControl*	m_pAlarmBlink;
	float m_fBLINK_TIME;

};
