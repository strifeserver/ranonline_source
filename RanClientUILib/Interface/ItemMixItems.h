#pragma	once

#include "UIWindowEx.h"
#include "GLItem.h"
#include "../RanClientLib/G-Logic/GLItemMixMan.h"
#include "GLCharData.h"
#include "GLCharacter.h"
class	CItemImage;
class	CBasicButton;
class	CBasicTextBoxEx;
class	CBasicScrollBarEx;
class	CBasicTextButton;
class	CBasicLineBox;
class	CUIEditBox;
class	GLCharacter;

class	CItemMixItems : public CUIWindowEx , public GLCHARLOGIC
{
private:
static	const int nLIMIT_CHAR;

private:
	enum
	{
		ITEMMIX_LIST_TEXTBOX = NO_ID + 1,
		ITEM_MIX_ITEMS_SLOT0,
		ITEM_MIX_ITEMS_SLOT1,
		ITEM_MIX_ITEMS_SLOT2,
		ITEM_MIX_ITEMS_SLOT3,
		ITEM_MIX_ITEMS_SLOT4,
		ITEM_MIX_ITEMS_SLOTR,
		ITEMMIX_LIST_SCROLLBAR,
		ITEMMIX_CATEGORY_BUTTON0,
		ITEMMIX_CATEGORY_BUTTON1,
		ITEMMIX_CATEGORY_BUTTON2,
		ITEMMIX_CATEGORY_BUTTON3,
		ITEMMIX_CATEGORY_BUTTON4,
		ITEMMIX_CATEGORY_BUTTON5,
		ITEMMIX_CATEGORY_BUTTON6,
		ITEMMIX_CATEGORY_BUTTON7,
		ITEMMIX_CATEGORY_BUTTON8,
		ITEMMIX_CATEGORY_BUTTON9,
		ITEMMIX_CATEGORY_BUTTON10,
		ITEMMIX_CATEGORY_BUTTON11,
		ITEMMIX_CATEGORY_BUTTON12,
		ITEMMIX_MONEY_BUTTON,
	};


public:
	CItemMixItems ();
	virtual ~CItemMixItems ();

public:
	void	CreateSubControl ();
	CBasicTextButton*	CreateTextButton14 ( char* szButton, UIGUID ControlID , char* szText );	

public:
	
	DWORD	GetItemID() { return m_nItemID; }
	BOOL IsSelectedIndex();
	void	LoadItemMixList ();

public:
	void	PARTY_FRIEND ( const CString& strName );	
	const	CString& GET_ITEMMIX_ITEM_NAME () const						{ return m_strSelectName; }
	CString	GetNameSelected(){ return m_strSelectName.GetString(); }
	int		m_nIndex;
	void	SetItemIndex ( int nIndex )				{ m_nIndex = nIndex; }

private:
	BOOL	GetItemMix ( const DWORD nItemID );

	SMIXDATA sMixData;
public:
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID cID, DWORD dwMsg );
	void	FlipBot ( int nCategory );
private:
	void	LoadItemMixItems ( const ITEM_MIX& sItemMix );

private:
	int		m_nSelectIndex;
	CString	m_strSelectName;

private:
	CBasicTextBoxEx*	m_pListText;
	CBasicScrollBarEx*	m_pListScrollBar;
	//CUIControl*			m_pBackImage;
	DWORD				m_nItemID;
	DWORD				m_nCategory;
	ITEM_MIX			pItemMix;
	//CBasicTextButton*	m_pNote;

	BOOL				m_bINIT;

	enum
	{
		ITEM = 0
	};
	
protected:
	CBasicTextBox*	CreateTextBox ( const char* szTextBox );
	CBasicButton*		m_pCategoryButton[ITEMMIX_CATEGORYNUM];
	CBasicTextBox*		m_pMoneyTextBox;
	CBasicTextBox*		m_pRateTextBox;
	CItemImage*			m_pMeterialsItemImage[ITEMMIX_ITEMNUM];
	CBasicTextBox*		m_pMeterialsItemNum[ITEMMIX_ITEMNUM];
	CItemImage*			m_pResultItemImage;
	CBasicLineBox*		m_pMeterialsItemNumBackImage[ITEMMIX_ITEMNUM];

public:
	//SFRIEND & GetFriendInfo() { return m_sFriend; }
	
	void	SetMoney ( LONGLONG Money );
	void	SetRate ( DWORD Rate );
};
