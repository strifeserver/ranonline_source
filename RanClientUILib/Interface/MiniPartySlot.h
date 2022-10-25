#pragma	once

#include "../EngineUILib/GUInterface/UIGroup.h"
#include "GLCharDefine.h"
#include "GLParty.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../EngineUILib/GUInterface/BasicProgressBar.h"

////////////////////////////////////////////////////////////////////
//	??? ??? ??
const DWORD UIMSG_MOUSEIN_RB_EVENT = UIMSG_USER1;
////////////////////////////////////////////////////////////////////

class	CBasicTextBox;
class	CBasicProgressBar;
class	CBasicLineBox;

class	CMiniPartySlot : public CUIGroup
{
protected:
	enum
	{
		HP_BAR = NO_ID + 1,
	};

	CUIControl*			m_pNumber[MAXPARTY];
	//CUIControl*		m_pClass[GLCI_NUM];

public:
	CMiniPartySlot ();
	virtual	~CMiniPartySlot ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetPlayerName ( CString strName )	{ m_pPlayerName->SetOneLineText( strName ); }
	//void	SetClass ( int nClassType );
	void	SetNumber ( int nSlotNumber );
	CUIControl*			m_pClass[GLCI_NUM_2015];
	void	SetClass ( int nClassType );
	CUIControl*			m_pMaster;
	CBasicTextBox*		m_pNumberSlot;
	CBasicLineBox*		pLineBoxCombo;
	CBasicLineBox*		pLineBox1;
	CBasicTextBox*		m_pPlayerName;

	void	SetTextColorChange();
	void	SetTextColorRevert();
	BOOL	m_bMaster;
	CUIControl*			m_pSchool[3];

	void	SetSchool ( int nSchoolType );

protected:
	CUIControl*		CreateControl ( const char* szControl );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR D3DCOLOR, int nAlign );
};