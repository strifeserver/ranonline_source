#pragma	once

#include "UIWindowEx.h"

class CRebuildInventoryPage;

//--------------------------------------------------------------------
// RebuildInventoryWindow : √÷¡ÿ«ı
//--------------------------------------------------------------------
class CRebuildInventoryWindow : public CUIWindowEx	// ITEMREBUILD_MARK
{
protected:
	enum
	{
		REBUILDINVENTORY_MONEY_BUTTON = ET_CONTROL_NEXT,
		REBUILDINVENTORY_PLUS_MONEY_BUTTON,
		REBUILDINVENTORY_MINUS_MONEY_BUTTON,
		REBUILDINVENTORY_PAGE
	};

private:
	CRebuildInventoryPage*	m_pPage;
	CBasicTextBox*			m_pMoneyTextBox;
	INT						m_nONE_VIEW_SLOT;
	bool	m_bOPENER;


public:
	CRebuildInventoryWindow();
	virtual	~CRebuildInventoryWindow();

public:
	VOID CreateSubControl();

public:
	virtual	VOID Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	VOID TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void	SetOPENER ( bool bOPENER );

private:
	void	SetMoney ( LONGLONG Money );
//	bool	IsTurnITEM ( SITEMCUSTOM& sItemCustom );

public:
	void	SetOneViewSlot ( const int& nONE_VIEW_SLOT );
	const int& GetOneViewSlot () const;
};