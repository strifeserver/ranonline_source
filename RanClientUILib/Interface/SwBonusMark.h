#pragma	once

#include "../EngineUILib/GUInterface/UIGroup.h"


class CSwBonusMark : public CUIGroup
{
public:
	enum
	{
		SWIconOnMouse = NO_ID + 1,
		SWIconOnMouse1,
		SWIconOnMouse2,
	};

public:
	CSwBonusMark();
	virtual ~CSwBonusMark();

	void	CreateSubControl ();
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	void	SetSwBonus( bool isWinner );

private:
	CBasicButton* m_pImage;
};