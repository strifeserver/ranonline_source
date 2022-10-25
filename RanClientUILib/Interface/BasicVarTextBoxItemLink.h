//	기본 정보 표시
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.11.27]
//			@ 작성
//

#pragma	once

#include "UIWindowEx.h"

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "GLDefine.h"
#include "GLogicData.h"
#include "GLCharacter.h"
class	CBasicTextBox;
class	CBasicLineBoxEx;
class	CD3DFontPar;
class	CD3DFontXPar;
class	CItemImage;
class	CSkillImage;


class	CBasicVarTextBoxItemLink : public CUIWindowEx
{
	enum{
		ITEM_IMAGE0,
	};
private:
static	const	float	fMOUSEPOINT_GAP;

public:
	CBasicVarTextBoxItemLink ();
	virtual	~CBasicVarTextBoxItemLink ();

public:
	void	CreateSubControl ();

public:
	void	ClearText ();
	int		AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor );
	int		AddTextNoSplit ( CString strText, D3DCOLOR dwColor );
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
	GLCharacter					m_Character;
	SCHARDATA2& GetCharacterData ()						{ return m_Character.GetCharData(); }
	int		GetCount ();
	void	RePosControl ( int x, int y );
		CUIControl*		m_pElecIcon;
	CUIControl*		m_pFireIcon;
	CUIControl*		m_pIceIcon;
	CUIControl*		m_pPoiIcon;
	CUIControl*		m_pSpiIcon;

public:
	void	SetBlockMouseTracking ( bool bBlock );

public:
	void	SetMousePointGap ( D3DXVECTOR2 vGap );

public:
	float	GetLongestLine ();
	void	SetTextAlign ( int nALIGN );

	void	SetItem ( SNATIVEID sICONINDEX, const char* szTexture );	
	void	ResetItem ();
	void		SetSkill ( SNATIVEID sNativeID );
	SNATIVEID	GetSkill ()							{ return m_sNativeID; }
	void	ResetItemBoxRender ();
	void	SetItemBoxRender ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void	SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture );
	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	void	SetResiIcon();
	void	HideResiIcon();
	void	ResetItemRender ();
public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	bool	m_bBLOCK_MOUSETRACKING;
	SNATIVEID	m_sICONINDEX;

public:
	CItemImage*			m_pItemImageRender;
	CItemImage*			m_pItemImage[10];
	CBasicTextBox*		m_pTextBox;
//	CBasicTextBox*		m_pTextBoxLeft;
	CBasicTextBox*		m_pTextBoxName;
	CBasicLineBoxEx*	m_pLineBox;
	D3DXVECTOR2			m_vMousePointGap;
	CSkillImage*	m_pSkillImage;
	SNATIVEID		m_sNativeID;


private:
	CUIControl*			m_pSelfDummy;

private:
	CD3DFontPar*	m_pFont;
	CD3DFontPar*	m_pFont10;
protected:

};