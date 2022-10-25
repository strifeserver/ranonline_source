//	기본 정보 표시
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.11.27]
//			@ 작성
//

#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "GLItem.h"

class	CBasicTextBox;
class	CBasicLineBoxEx;
class	CD3DFontPar;
class	CItemImage; 
struct	SNATIVEID;
class	GLItem;
class	CItemImage;

class	CBasicVarTextBox : public CUIGroup
{
	enum
	{
		ITEM_IMAGE0 = NO_ID,
		ITEM_IMAGE1,
		ITEM_IMAGE2,
		ITEM_IMAGE3,
		ITEM_IMAGE4,
		ITEM_IMAGE5,
		ITEM_IMAGE6,
		ITEM_IMAGE7,
		ITEM_IMAGE8,
		ITEM_IMAGE9,
	};
private:
static	const	float	fMOUSEPOINT_GAP;

public:
	CBasicVarTextBox ();
	virtual	~CBasicVarTextBox ();

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

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	bool	m_bBLOCK_MOUSETRACKING;

private:
	CBasicTextBox*		m_pTextBox;
	CBasicLineBoxEx*	m_pLineBox;
	D3DXVECTOR2			m_vMousePointGap;
	
private:
	CUIControl*			m_pSelfDummy;
	CUIControl*			m_pSelfDummyGray;
	
public: 
	CItemImage*    CreateItemImage ( const char* szControl, UIGUID controlID );
	CItemImage*			m_pItemImageRender;
	CItemImage*			m_pItemImage[ITEM::SBOX::ITEM_SIZE];
	CUIControl*			m_pElecIcon;
	CUIControl*			m_pFireIcon;
	CUIControl*			m_pIceIcon;
	CUIControl*			m_pPoiIcon;
	CUIControl*			m_pSpiIcon;
	CUIControl*			m_pPremiumIcon;
	CBasicTextBox*		m_pTextBoxName;

	void    SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture ); 
	void    ResetItemRender ();  
	void	SetItemBoxRender ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void	ResetItemBoxRender ();
	void	SetResiIcon();
	void	ResetResiIcon();
	void	SetPremium();
	void	ResetPremium();
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

	bool		m_bCHECK_MOUSE_STATE;
	int			m_PosX;
	int			m_PosY;
	D3DXVECTOR2	m_vGap;

	void	CheckMouseState ();
private:
	CD3DFontPar*	m_pFont;
	CD3DFontPar*	m_pFont10;
	
};