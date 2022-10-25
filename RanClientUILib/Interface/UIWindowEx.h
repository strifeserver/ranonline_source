#pragma	once

#include "../EngineUIlib/GUInterface/UIWindow.h"

class	CBasicButton;
class	CBasicTextBox;

class CUIWindowEx : public CUIWindow
{
public:
	CUIWindowEx ();
	virtual	~CUIWindowEx ();

public:
	virtual void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );

public:
	void	CreateBaseWindowBlack ( char* szWindowKeyword, char* szWindowName );
	void	CreateBaseWindowBlackEmpty ( char* szWindowKeyword );
	void	CreateBaseWindowLightGray ( char* szWindowKeyword, char* szWindowName );
	void	CreateBaseWindowLightGrayUntitled ( char* szWindowKeyword );
	void	CreateBaseWindowLightGrayUntitledNoClose ( char* szWindowKeyword );
	void	CreateBaseNewWindow ( char* szWindowKeyword, char* szWindowName );
	void	CreateBaseNewWindowNoTitle ( char* szWindowKeyword );
	void	CreateBaseWindowNoTitle ( char* szWindowKeyword );
	void	CreateBaseWindowMiniParty ( char* szWindowKeyword, char* szWindowName );
	void	CreateBaseWindowNoTittleNoClose ( char* szWindowKeyword );
	void	CreateBaseWindowLightGray2 ( char* szWindowKeyword );

protected:
	CUIControl*		CreateControl ( const char* szControl, const UIGUID& cID = NO_ID );
	CBasicButton*	CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID, WORD wFlipType );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign );

protected:
	void	ResizeControl ( char* szWindowKeyword );
};
