#pragma once

#include "../EngineUILib/GUInterface/UIGroup.h"
class	CBasicButton;

class CItemCompound : public CUIGroup
{
public:
	CItemCompound(void);
	~CItemCompound(void);

public:
	enum
	{
		COMPOUND_BUTTON	= NO_ID + 1,
	};

public:
	void	CreateSubControl ();


public:
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CBasicButton*		m_pCompoundButton;
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	
};

