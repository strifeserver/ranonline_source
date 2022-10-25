#pragma	once

#include "../EngineUILib/GUInterface/UIGroup.h"
#include "GLInventory.h"

class	CItemSlot;
class	CBasicScrollBarEx;
class	CBasicButton;
class	CBasicTextBox;
class	CD3DFontPar;
struct	SINVENITEM;

class	CInventoryPage : public CUIGroup
{
private:
static	const	int	nSTARTLINE;
static	const	int	nOUTOFRANGE;

private:
	enum
	{
		nONE_VIEW_SLOT_DEFAULT = 6,
		nMAX_ONE_VIEW_SLOT = 10,
		nMAX_ONE_VIEW_SLOT2 = 20,
		nMAX_ONE_VIEW_SLOT3 = 30,
		nMAX_ONE_VIEW_SLOT4 = 40,
		nMAX_ONE_VIEW_SLOT5 = 50,
	};

protected:
	enum
	{
		EM_INVENSIZE_Y_FORCE = EM_INVENSIZE_Y,
		ITEM_SLOT00 = NO_ID + 1,
		ITEM_SLOT01,ITEM_SLOT02,ITEM_SLOT03,ITEM_SLOT04,ITEM_SLOT05,ITEM_SLOT06,ITEM_SLOT07,ITEM_SLOT08,ITEM_SLOT09,
		ITEM_SLOT10,ITEM_SLOT11,ITEM_SLOT12,ITEM_SLOT13,ITEM_SLOT14,ITEM_SLOT15,ITEM_SLOT16,ITEM_SLOT17,ITEM_SLOT18,ITEM_SLOT19,
		ITEM_SLOT20,ITEM_SLOT21,ITEM_SLOT22,ITEM_SLOT23,ITEM_SLOT24,ITEM_SLOT25,ITEM_SLOT26,ITEM_SLOT27,ITEM_SLOT28,ITEM_SLOT29,
		ITEM_SLOT30,ITEM_SLOT31,ITEM_SLOT32,ITEM_SLOT33,ITEM_SLOT34,ITEM_SLOT35,ITEM_SLOT36,ITEM_SLOT37,ITEM_SLOT38,ITEM_SLOT39,
		ITEM_SLOT40,ITEM_SLOT41,ITEM_SLOT42,ITEM_SLOT43,ITEM_SLOT44,ITEM_SLOT45,ITEM_SLOT46,ITEM_SLOT47,ITEM_SLOT48,ITEM_SLOT49,
		ITEM_SLOT_END = ITEM_SLOT00 + EM_INVENSIZE_Y_FORCE,
		INVENTORY_BUTTON0,
		INVENTORY_BUTTON1,
		INVENTORY_BUTTON2,
		INVENTORY_BUTTON3,
		INVENTORY_BUTTON4,
	};

public:
	CInventoryPage ();
	virtual	~CInventoryPage ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

	void	UpdateBLOCK ();

	void	INVENTORY1();
	void	INVENTORY2();
	void	INVENTORY3();
	void	INVENTORY4();
	void	INVENTORY5();

protected:
	void	ResetAllItemSlotRender ( int index,int nTotal );
	void	SetItemSlotRender ( int nStartIndex, int nTotal );

public:
	void	SetItemIndex ( int nPosX, int nPosY );
	void	GetItemIndex ( int* pnPosX, int* pnPosY );

public:
	void	LoadItemPage ( GLInventory &ItemData );
	void	LoadItem ( SINVENITEM& ref_InvenItem );

	SINVENITEM&	GetItem ( int nPosX, int nPosY );

public:
	void	UnLoadItemPage ();
	void	UnLoadItem ( int nPosX, int nPosY );

protected:
	CItemSlot*	CreateItemSlot ( CString strKeyword, UIGUID ControlID );
	CUIControl*	CreateControl ( const char* szControl );

	CUIControl*	m_Button[5];
	CUIControl*	m_ButtonSet[5];
	CBasicTextBox*	m_ButtonText[5];

private:
	CItemSlot*	m_pItemSlotArray[EM_INVENSIZE_Y_FORCE];
	CUIControl*	m_pItemSlotArrayDummy[nMAX_ONE_VIEW_SLOT5];
	CBasicButton*	m_pPageButton[5];

	int		m_nPosX;
	int		m_nPosY;
	int		m_nCurPos;
	int		m_nONE_VIEW_SLOT;
public:
	void	SetOneViewSlot ( const int& nMAX_ONE_VIEW );
	const int& GetOneViewSlot () const			{ return m_nONE_VIEW_SLOT; }

};