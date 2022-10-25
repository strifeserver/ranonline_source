#pragma once

#include "UIWindowEx.h"
#include "GLItem.h"
#include "DxSkinChar.h"
#include "DxCharSham.h"
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

class CItemPreview : public CUIWindowEx
{
	static const D3DCOLOR dwDEFAULT_TRANSPARENCY;
	static const D3DCOLOR dwFULL_TRANSPARENCY;

	enum
	{
		//IMAGE_HEAD = ET_CONTROL_NEXT,
		//IMAGE_BODY,
		//IMAGE_PANT,
		//IMAGE_HAND,
		//IMAGE_FOOT,
		//IMAGE_WEAP,
		//MOVE_R,
		//MOVE_R_F,
		//MOVE_L,
		//MOVE_L_F,

	};
public:
	CItemPreview(void);
	~CItemPreview(void);

protected:


	
private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;

public:
	HRESULT InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice);

public:


public:
	void				CreateSubControl ();
	CBasicTextButton*	CreateTextButton ( char* szButton, UIGUID ControlID, char* szText );
	CBasicTextBox*		CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	CItemImage*			CreateItemImage ( const char* szControl, UIGUID ControlID );

	//void				LoadItem ( SNATIVEID& ref_sItemID );
	//void				UnLoadItem ();

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:

	//CBasicLineBox*	m_pLinebox;
	//CUIControl*		m_pRender;

	//CBasicTextBox*	m_pTextHead;
	//CBasicTextBox*	m_pTextBody;
	//CBasicTextBox*	m_pTextPant;
	//CBasicTextBox*	m_pTextHand;
	//CBasicTextBox*	m_pTextShoe;
	//CBasicTextBox*	m_pTextWeap;

	//CItemImage*		m_pImageHead;
	//CItemImage*		m_pImageBody;
	//CItemImage*		m_pImagePant;
	//CItemImage*		m_pImageHand;
	//CItemImage*		m_pImageShoe;
	//CItemImage*		m_pImageWeap;

	//CBasicButton*		m_pMove_R;
	//CBasicButton*		m_pMove_L;

	//SNATIVEID			m_sPosionID;
};
