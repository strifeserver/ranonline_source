#pragma once

#include "../EngineUIlib/GUInterface/UIGroup.h"

class CGambleResultOddBox : public CUIGroup
{

public:
	CGambleResultOddBox(void);
	virtual ~CGambleResultOddBox(void);

public:
	void	CreateSubControl ();
	
public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
public:
	void SetWinLose(BOOL bWin);

private:
	CUIControl * m_pControl;
	float m_TotElapsedTime;
	BOOL  m_bWinLose;
};