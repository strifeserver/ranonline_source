#include "StdAfx.h"

#include "BasicLineBox.h"
#include "d3dfont.h"
#include "GameTextControl.h"
#include "ItemImage.h"
#include "InnerInterface.h"
#include "ItemMove.h"
#include "BasicComboBox.h"
#include "BasicComboBoxRollOver.h"
#include "UITextControl.h"
#include "GLGaeaClient.h"
#include "BasicTextButton.h"
#include "PetQuickSkillSlot.h"
#include "GLItemMan.h"
#include "DxViewPort.h"


#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../EngineUIlib/GUInterface/BasicProgressBar.h"

#include ".\itempreview.h"

const D3DCOLOR CItemPreview::dwDEFAULT_TRANSPARENCY = D3DCOLOR_ARGB ( 160, 255, 255, 255 );;
const D3DCOLOR CItemPreview::dwFULL_TRANSPARENCY = D3DCOLOR_ARGB ( 255, 255, 255, 255 );;

CItemPreview::CItemPreview(void) :
	m_pd3dDevice(NULL)
	//m_pImageHead(NULL),
	//m_pImageBody(NULL),
	//m_pImagePant(NULL),
	//m_pImageHand(NULL),
	//m_pImageShoe(NULL),
	//m_pImageWeap(NULL),
	//m_pMove_R(NULL),
	//m_pMove_L(NULL),
	//m_pRender(NULL),
	//m_pTextHead(NULL),
	//m_pTextBody(NULL),
	//m_pTextPant(NULL),
	//m_pTextHand(NULL),
	//m_pTextShoe(NULL),
	//m_pTextWeap(NULL),
	//m_sPosionID(NATIVEID_NULL())
{
}

CItemPreview::~CItemPreview(void)
{
}


void CItemPreview::CreateSubControl()
{
	//CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	/*m_pRender = new CUIControl;
	m_pRender->CreateSub( this, "ITEMPREVIEW_WINDOW_BKLINE" );
	m_pRender->SetUseRender( TRUE );
	m_pRender->SetDiffuseAlpha( 0 );
	m_pRender->SetVisibleSingle( FALSE );
	RegisterControl( m_pRender );

	m_pLinebox = new CBasicLineBox;
	m_pLinebox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLinebox->CreateBaseBoxOuter ( "ITEMPREVIEW_WINDOW_MESHRENDER" );
	RegisterControl ( m_pLinebox );
	
	m_pImageHead = CreateItemImage( "ITEMPREVIEW_SLOT_HEAD", IMAGE_HEAD );
	m_pImageBody = CreateItemImage( "ITEMPREVIEW_SLOT_UPPER", IMAGE_BODY );
	m_pImagePant = CreateItemImage( "ITEMPREVIEW_SLOT_LOWER", IMAGE_PANT );
	m_pImageHand = CreateItemImage( "ITEMPREVIEW_SLOT_HAND", IMAGE_HAND );
	m_pImageShoe = CreateItemImage( "ITEMPREVIEW_SLOT_FOOT", IMAGE_FOOT );
	m_pImageWeap = CreateItemImage( "ITEMPREVIEW_SLOT_WEAPON", IMAGE_WEAP );

	m_pTextHead = CreateStaticControl ( "ITEMPREVIEW_TEXTBOX_HEAD", pFont, TEXT_ALIGN_LEFT );
	m_pTextHead->AddText ( ID2GAMEWORD("ITEM_PREVIEW_TEXT", 0), NS_UITEXTCOLOR::PALEGOLDENROD );
	m_pTextBody = CreateStaticControl ( "ITEMPREVIEW_TEXTBOX_UPPER", pFont, TEXT_ALIGN_LEFT );
	m_pTextBody->AddText ( ID2GAMEWORD("ITEM_PREVIEW_TEXT", 1), NS_UITEXTCOLOR::PALEGOLDENROD );
	m_pTextPant = CreateStaticControl ( "ITEMPREVIEW_TEXTBOX_LOWER", pFont, TEXT_ALIGN_LEFT );
	m_pTextPant->AddText ( ID2GAMEWORD("ITEM_PREVIEW_TEXT", 2), NS_UITEXTCOLOR::PALEGOLDENROD );
	m_pTextHand = CreateStaticControl ( "ITEMPREVIEW_TEXTBOX_HAND", pFont, TEXT_ALIGN_LEFT );
	m_pTextHand->AddText ( ID2GAMEWORD("ITEM_PREVIEW_TEXT", 3), NS_UITEXTCOLOR::PALEGOLDENROD );
	m_pTextShoe = CreateStaticControl ( "ITEMPREVIEW_TEXTBOX_FOOT", pFont, TEXT_ALIGN_LEFT );
	m_pTextShoe->AddText ( ID2GAMEWORD("ITEM_PREVIEW_TEXT", 4), NS_UITEXTCOLOR::PALEGOLDENROD );
	m_pTextWeap = CreateStaticControl ( "ITEMPREVIEW_TEXTBOX_WEAPON", pFont, TEXT_ALIGN_LEFT );
	m_pTextWeap->AddText ( ID2GAMEWORD("ITEM_PREVIEW_TEXT", 5), NS_UITEXTCOLOR::PALEGOLDENROD );

	m_pMove_L = CreateFlipButton 
		( "ITEMPREVIEW_WINDOW_ARROW_TURN_LEFT_DEFAULT",
		"ITEMPREVIEW_WINDOW_ARROW_TURN_LEFT_CLICK", MOVE_L, CBasicButton::MOUSEIN_FLIP );
	m_pMove_R = CreateFlipButton 
		( "ITEMPREVIEW_WINDOW_ARROW_TURN_RIGHT_DEFAULT",
		"ITEMPREVIEW_WINDOW_ARROW_TURN_RIGHT_CLICK", MOVE_R, CBasicButton::MOUSEIN_FLIP );*/
}

HRESULT CItemPreview::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT( pd3dDevice );
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIWindowEx::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

HRESULT CItemPreview::RestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	HRESULT hr = S_OK;
	hr = CUIWindowEx::RestoreDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	const LONG lResolution = CInnerInterface::GetInstance().GetResolution();
	WORD X_RES = HIWORD( lResolution );
	WORD Y_RES = LOWORD( lResolution );

	const UIRECT& rcWindow = GetGlobalPos();

	D3DXVECTOR2 vPos;
	vPos.x = ( (X_RES) / 2.0f );
	vPos.y = ( Y_RES - rcWindow.sizeY ) / 2.0f;
	SetGlobalPos( vPos );

	return S_OK;
}

HRESULT	CItemPreview::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if( !IsVisible() ) return S_OK;

	HRESULT hr = S_OK;

	hr = CUIWindowEx::Render ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;


	/*DWORD dwMinFilter(0), dwMagFilter(0), dwMipFilter(0);
	pd3dDevice->GetSamplerState( 0, D3DSAMP_MINFILTER, &dwMinFilter );
	pd3dDevice->GetSamplerState( 0, D3DSAMP_MAGFILTER, &dwMagFilter );
	pd3dDevice->GetSamplerState( 0, D3DSAMP_MIPFILTER, &dwMipFilter );

	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, dwMinFilter );
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, dwMagFilter );
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, dwMipFilter );*/

	return hr;
}

void CItemPreview::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{		
	}else
	{
		//CInnerInterface::GetInstance().HideGroup( ITEM_PREVIEW );
	}
}

CItemImage*	CItemPreview::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );

	return pItemImage;
}

CBasicTextButton* CItemPreview::CreateTextButton ( char* szButton, UIGUID ControlID, char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE14;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText );
	RegisterControl ( pTextButton );
	return pTextButton;
}

CBasicTextBox* CItemPreview::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

void CItemPreview::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	//SNATIVEID & sPosionOldID = m_sPosionID;
	//SNATIVEID & sPosionID = GLGaeaClient::GetInstance().GetSummonClient()->GetPosionItem();

	/*if ( sPosionID != NATIVEID_NULL() )
	{
		LoadItem ( sPosionID );
	}
	else
	{
		UnLoadItem ();
	}*/

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CItemPreview::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	};
}

/*void CItemPreview::LoadItem( SNATIVEID& ref_sItemID )
{
	m_sPosionID = ref_sItemID;

	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( m_sPosionID );
	//m_pPosionImage->SetItem ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile(), pItemData->sBasicOp.sNativeID );

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter ();
	DWORD dwAmount = pCharacter->m_cInventory.CountTurnItem( m_sPosionID );

	CString strNumber;
	strNumber.Format ( "%d", dwAmount );
}*/

/*void CItemPreview::UnLoadItem ()
{
	m_sPosionID	= NATIVEID_NULL ();
}*/