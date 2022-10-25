
#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"

#include "GLDefine.h"
#include "GLogicData.h"
#include "GLCharacter.h"
class	CBasicTextBox;
class	CBasicLineBoxEx;
class	CBasicButton;
class	CD3DFontPar;
class	CItemImage;
class	CSkillImage;

const DWORD UIMSG_UIVARTEXT_BUTTON_CLOSE_CLICK = UIMSG_USER1;
const DWORD UIMSG_UIVARTEXT_BUTTON_CLOSE_MOUSEIN = UIMSG_USER2;

class	CBasicVarTextBoxEx : public CUIGroup
{
private:
static	const	float	fMOUSEPOINT_GAP;

protected:
	enum
	{
		BUTTON_CLOSE = NO_ID + 1,
	};

public:
	CBasicVarTextBoxEx ();
	virtual	~CBasicVarTextBoxEx ();

public:
	void	CreateSubControl ();

public:
	void	ClearText ();
	int		AddTextNoSplit ( CString strText, D3DCOLOR dwColor );
	int		AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor );
	int		SetTextNoSplit ( CString strText, D3DCOLOR dwColor );
	void	SetText( CString strText, D3DCOLOR	dwColor );
	void	AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor );
	int		AddText ( CString strText, D3DCOLOR dwColor );
	void	AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor );
	CItemImage*	CreateItemImage ( const char* szControl, UIGUID controlID );

public:
	void	SetUseOverColor ( bool bUseColor );
	void	SetOverColor ( const D3DCOLOR& dwColor );
	void	SetLineInterval ( const float fLineInterval );

public:
	int		GetCount ();

	void	RePosControl ( int x, int y );

public:
	void	SetBlockMouseTracking ( bool bBlock );

public:
	void	SetMousePointGap ( D3DXVECTOR2 vGap );

public:
	float	GetLongestLine ();

	void	SetTextAlign ( int nALIGN );

	void	ShowCloseButton( bool bSHOW );

	void	SetItem ( SNATIVEID sICONINDEX, const char* szTexture );	
	void	ResetItem ();
	void		SetSkill ( SNATIVEID sNativeID );
	SNATIVEID	GetSkill ()							{ return m_sNativeID; }
	void	ResetItemBoxRender ();
	void	SetItemBoxRender ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void	SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture );

	void	SetResiIcon();
	void	HideResiIcon();
	void	ResetItemRender ();
public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	bool	m_bBLOCK_MOUSETRACKING;
	SNATIVEID	m_sICONINDEX;

private:
	CItemImage*			m_pItemImageRender;
	CBasicTextBox*		m_pTextBox;
	CBasicTextBox*		m_pTextBoxBig;
	CBasicLineBoxEx*	m_pLineBox;
	CBasicButton*		m_pCloseButton;
	D3DXVECTOR2			m_vMousePointGap;
	CSkillImage*	m_pSkillImage;
	SNATIVEID		m_sNativeID;

private:
	CUIControl*			m_pSelfDummy;

private:
	CD3DFontPar*	m_pFont;
	CD3DFontPar*	m_pFont12;
};