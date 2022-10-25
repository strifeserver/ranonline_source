#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "../RanClientLib/G-Logic/GLContrlServerMsg.h"
#include "../RanClientLib/G-Logic/GLItemMan.h"
#include "LargeMapWindowCheckBox.h"

class CBasicScrollBarEx;
class CLargeMapWindowCheckBox;
class CBasicTextBoxEx;
class CLargeMapWindowMobItem : public CUIGroup
{
private:
static	const	int	nSTARTLINE;
static	const	int	nOUTOFRANGE;

protected:	
	enum
	{
		ITEM_LIST_TEXTBOX,
		ITEM_LIST_SCROLLBAR,
	};

public:
	CLargeMapWindowMobItem ();
	virtual	~CLargeMapWindowMobItem ();
	void CreateSubControl();
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

	void LoadMob (NET_MSG_GENERIC* nmg);
	void ClearMobList();

private:
	CBasicTextBoxEx*	m_pListText;
	CBasicScrollBarEx*	m_pListScrollBar;
	int		m_nSelectIndex;
};