#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "GLDefine.h"

//	NOTE
//		??? ?? ???
const DWORD UIMSG_MOUSEIN_BUTTON = UIMSG_USER1;
const DWORD UIMSG_LEVEL_UP = UIMSG_USER2;
const DWORD UIMSG_MOUSEIN_SKILLIMAGE = UIMSG_USER3;
const DWORD UIMSG_MOUSEIN_BUTTON2 = UIMSG_USER4;

class	CSkillImage;
class	CBasicButton;
class	CBasicTextBox;
class	CBasicLineBox;
class	CBasicLineBoxEx;
class	CBasicButtonText;

class	CSkillSlot : public CUIGroup
{
protected:
	enum
	{
		SKILL_SLOT_LEVEL_UP = NO_ID + 1,
		SKILL_SLOT_IMAGE,
		SKILL_IMAGE,
		SKILL_SLOT_LEVEL_DOWN
	};

public:
	CSkillSlot ();
	virtual	~CSkillSlot ();

public:
	void	CreateSubControl ( SNATIVEID sNativeID );

public:
	void		SetNativeSkillID ( SNATIVEID sNativeID )			{ m_sNativeID = sNativeID; }
	SNATIVEID	GetNativeSkillID ()									{ return m_sNativeID; }
	BOOL		m_bSKILLSLOT_ACTIVE;
	BOOL		m_bSKILLSLOT_MOUSE_IN;
	void		SKILLSLOT_ACTIVE();


public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

protected:
	CUIControl*		CreateControl ( char* szControl );
	CSkillImage*	CreateSkillImage ( UIGUID ControlID );
	CBasicButton*	CreateFlipButton ( char* szButton, char* szButtonFlip, char* szMouseOver, UIGUID ControlID );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign );

private:
	CSkillImage*	m_pSkillImage;
	//CBasicButton*	m_pSkillUpButton;
	CBasicTextBox*	m_pTextBoxLine1;
	CBasicTextBox*	m_pTextBoxLine2;
	CUIControl*		m_pNotLearnImage;
	CUIControl*		m_pSkillSlotAvtive;
	CBasicLineBoxEx*	m_pLineBoxGreen;
	CBasicLineBoxEx*	m_pLineBoxBlue;
	CBasicLineBoxEx*	m_pLineBoxYellow;

private:
	WORD		m_wLevel;

	CString		m_strLine1;
	CString		m_strLine2;

private:
	SNATIVEID		m_sNativeID;

	BOOL	m_bLearnSkill;
	BOOL	m_bLEVELUP_ENABLE;
	BOOL	m_bCanLearn;

public:
	CBasicButtonText*	CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText );
	CBasicButtonText*	m_pButton_Plus;
	CUIControl*			m_pButton_Black;

	CBasicButtonText*	m_pButton_Minus;
	CUIControl*			m_pButton_Black2;

	int nLevel;
};