#pragma	once

#include "../UIWindowEx.h"
class	CBasicTextButton;
//class	CSchoolWarPage;
class	CBasicLineBoxEx;
class	CBasicTextBox;
class	CPvpWindowDisplay : public CUIWindowEx
{
protected:
	enum
	{
		SCHOOLWAR_BACKGROUND = ET_CONTROL_NEXT,
		SCHOOLWAR_PAGE,
		SCHOOLWAR_BUTTON_PARTICIPATE,
		SCHOOLWAR_BUTTON_PAGE,
		SCHOOLWAR_BUTTON_CLOSE,
	};

public:
	CPvpWindowDisplay ();
	virtual	~CPvpWindowDisplay ();
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	CBasicTextButton*	CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText );
	CBasicTextButton*	CreateTextButton ( char* szButton, UIGUID ControlID , char* szText );

	void CreateSubControl ();

private:
	int								m_nActivePage;
	CUIControl*						m_pBackGround;
	CBasicTextButton*				m_pButtonSchoolWarPage;
	//CBasicTextButton*				m_pButtonRank[3];
	//CSchoolWarPage*					m_pPageSchoolWar;

	CBasicLineBoxEx*				m_pLineBoxTop;
	CBasicLineBoxEx*				m_pLineBoxUpper;
	CBasicLineBoxEx*				m_pLineBoxBottom;
	CBasicLineBoxEx*				m_pLineBoxTopWhite;
	CBasicLineBoxEx*				m_pLineBoxBottomWhite;
	CBasicTextBox*					m_pTimeTextNext;
	CBasicTextBox*					m_pTextProgress;
	CBasicTextBox*					m_pTextRegister;
	CBasicTextButton*				m_pButtonClose;
	CBasicTextButton*				m_pButtonParticipate;

	CBasicTextBox*					m_pTimeTextDuration;
	CBasicTextBox*					m_pTimeTextEnd;
	CBasicTextBox*					m_pWinnerText;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	SetWarTime(float fElapsedTime);
	void	RefreshRanking();
	void	OpenPage( int nPage );
	void	SetWinImg ( int nWin );
	CUIControl*	m_pSchoolWarImage;
	CUIControl* m_pSchoolImg[3];
	CUIControl* m_pSchoolWinImg[3];
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign );
	CBasicTextBox*	m_pText[16];
	CBasicTextBox*	m_pWinLose[2];
};