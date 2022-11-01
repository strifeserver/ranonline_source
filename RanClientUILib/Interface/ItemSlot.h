//	������ ����
//
//	���� �ۼ��� : ���⿱
//	���� ������ : 
//	�α�
//		[2003.12.6]
//			@ �ۼ�
//

#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "GLDefine.h"
#include "GLItemMan.h"

////////////////////////////////////////////////////////////////////
//	����� �޽��� ����
const DWORD UIMSG_MOUSEIN_ITEMSLOT = UIMSG_USER1;
////////////////////////////////////////////////////////////////////

class	CItemImage;
class	CBasicTextBox;
class	CBasicTextButton;
class	CBasicLineBoxEx;
class	CBasicVarTextBox;

class	CItemSlot : public CUIGroup
{
protected:
static	const	int	nOUTOFRANGE;

public:
	enum
	{
		nLIMIT_COLUMN = 10
		//nLIMIT_COLUMN = EM_INVENSIZE_X
	};

protected:
	int		m_nIndex;
	int		m_nMaxColumn;

	bool	m_bBLOCK;

public:
	CUIControl*		m_pMouseOver;

protected:
	CBasicTextBox*		m_pNumberBoxArray[nLIMIT_COLUMN];
	CBasicTextBox*	 m_pItemNameStatic[nLIMIT_COLUMN];
	CBasicTextBox*	 m_pItemPriceStatic[nLIMIT_COLUMN];
	CBasicTextBox*	 m_pItemStockStatic[nLIMIT_COLUMN];
	
	CBasicTextBox*	 m_pItemName[nLIMIT_COLUMN];
	CBasicTextBox*	 m_pItemPrice[nLIMIT_COLUMN];
	CBasicTextBox*	 m_pItemStock[nLIMIT_COLUMN];
	CBasicTextBox*	 m_pItemCur[nLIMIT_COLUMN];
	CBasicVarTextBox*	m_pInfo;


protected:
	CItemImage*		m_pItemImageArray[nLIMIT_COLUMN];	
	SINVENITEM		m_InvenItem[nLIMIT_COLUMN];
	CUIControl*		m_pBlock[6];
	CUIControl*		m_pWrap[6];
	CBasicLineBoxEx*		m_pWhiteBack[nLIMIT_COLUMN];
	CBasicTextButton* m_bBButton[nLIMIT_COLUMN];
	//	bool	m_bTOP_CONTROL;

private:
	enum
	{
		ITEM_IMAGE0 = NO_ID + 1,
		ITEM_IMAGE1,
		ITEM_IMAGE2,
		ITEM_IMAGE3,
		ITEM_IMAGE4,
		ITEM_IMAGE5,
		ITEM_IMAGE6,
		ITEM_IMAGE7,
		ITEM_IMAGE8,
		ITEM_IMAGE9,
		ITEMSHOP_BUY_BUTTON0,
		ITEMSHOP_BUY_BUTTON1,
		ITEMSHOP_BUY_BUTTON2,
		ITEMSHOP_BUY_BUTTON3,
		ITEMSHOP_BUY_BUTTON4,
		ITEMSHOP_BUY_BUTTON5,
		ITEMSHOP_BUY_BUTTON6,
		ITEMSHOP_BUY_BUTTON7,
		ITEMSHOP_BUY_BUTTON8,
		ITEMSHOP_BUY_BUTTON9,
	};

public:
	CItemSlot ();
	virtual	~CItemSlot ();

public:
	void	CreateSubControl ( int nMaxColumn, BOOL bNumberUse = FALSE , BOOL bItemShop = FALSE );

	BOOL	m_bItemShop;
	BOOL	bCheckItemShop() { return m_bItemShop; }

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CItemImage*		CreateItemImage ( const char* szControl, UIGUID ControlID );
	void	CreateMouseOver ( char* szControl );	
	void	CreateNumberBox ();
	CUIControl*	CreateFlipImage ( const char* szFlip );
	void	CreateItemShopText ();
	CBasicTextButton*	CreateTextButton ( const char* szButton, UIGUID ControlID, char* szText );

private:
	CBasicTextBox*	CreateNumberBox ( const char* szControl );
	CUIControl*		m_pFlipImage[nLIMIT_COLUMN];
	CBasicTextBox*	  CreateStaticControl ( const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );

public:
	void	SetItemImage ( int nIndex, SINVENITEM& ref_InvenItem );
	SINVENITEM&	GetItemImage ( int nIndex )		{ return m_InvenItem[nIndex]; }
	void	ResetItemImage ( int nIndex );
	void	SetItemUnUse ( int nIndex );
	void	SetItemUse ( int nIndex );

public:
	void	SetItemIndex ( int nIndex )			{ m_nIndex = nIndex; }
	int		GetItemIndex ( )					{ return m_nIndex; }
	void	SetFlipItem ( int nIndex, BOOL bFlip );
	BOOL	IsFlipItem ( int nIndex );

public:
	void	SetNumber ( int nIndex, int nNumber, int nMaxNumber );
	void	SetNumber2 ( int nIndex, int nNumber );
	void	SetNumber3 ( int nIndex, int nNumber );
	void	ResetNumber ( int nIndex );

	void	SetBLOCK ();
	void	ResetBLOCK ();
	bool	IsBLOCK ()							{ return m_bBLOCK; }
};