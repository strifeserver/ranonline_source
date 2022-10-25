#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"

class	CPlayerKillStreakType;
class	CPlayerKillStreakDisplay : public CUIGroup
{
public:
	enum
	{
		FIRST_BLOOD = NO_ID + 1,
		DOUBLE_KILL,
		TRIPLE_KILL,
		ULTRA_KILL,
		RAMPAGE,
		KILLING_SPREE,
		DOMINATING,
		MEGA_KILL,
		UNSTOPPABLE,
		WICKED_SICK,
		MONSTER_KILL,
		GODLIKE,
		HOLY_SHIT,
		OWNAGE,
		MAX_TYPE = 14
	};

public:
	CPlayerKillStreakDisplay ();
	virtual	~CPlayerKillStreakDisplay ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	bool	START ( UIGUID cID );
	bool	RESET ( UIGUID cID );
	bool	STOP ( UIGUID cID );

	bool	KEEP_START ();
	void	KEEP_STOP ();

private:
	CPlayerKillStreakType*		m_pKillStreak_TYPE[MAX_TYPE];
	CUIControl*			m_pKillStreak_KEEP[MAX_TYPE];

	CUIControl*		m_pPositionControl;	
};