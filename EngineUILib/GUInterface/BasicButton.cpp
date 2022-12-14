#include "StdAfx.h"
#include "BasicButton.h"
#include "UIKeyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float CBasicButton::fBUTTON_CLICK = 1.0f;

CBasicButton::CBasicButton():
	m_wFlipMode(NOT_READY),
	m_bDown(FALSE),
	m_bMouseIn(FALSE),
	m_bFlip ( FALSE ),
	m_bOnBack ( FALSE ),
	m_bUseDynamic ( FALSE ),
	m_bUseGlobalAction ( FALSE ),
	m_KeyCode ( NULL ),
	m_KeyCodeEx ( NULL ),
	m_pFlipControl ( NULL ),
	m_pMouseOver ( NULL )
{
}

CBasicButton::~CBasicButton()
{
}

void CBasicButton::CreateRanButtonBlue( CUIControl* pParent, const char* szControlKeyword, UIGUID WndID )
{	
	CreateSub( pParent, "RAN_BUTTON", UI_FLAG_XSIZE | UI_FLAG_YSIZE, WndID );

	CUIControl* pDefault = new CUIControl;
	pDefault->CreateSub ( this, "RAN_BLUE_BUTTON_DEFAULT" , UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pDefault->SetVisibleSingle ( TRUE );
	RegisterControl ( pDefault );

	CUIControl* pMouseOver = new CUIControl;
	pMouseOver->CreateSub ( this, "RAN_BLUE_BUTTON_MOUSEIN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pMouseOver->SetVisibleSingle ( FALSE );
	RegisterControl ( pMouseOver );
	m_pMouseOver = pMouseOver;

	CUIControl* pFlipControl = new CUIControl;
	pFlipControl->CreateSub ( this, "RAN_BLUE_BUTTON_CLICK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pFlipControl->SetVisibleSingle ( FALSE );
	RegisterControl ( pFlipControl );
	m_pFlipControl = pFlipControl;

	CUIControl* pLeft = new CUIControl;
	pLeft->CreateSub ( this, "RAN_BUTTON_DEFAULT_LINE_LEFT" , UI_FLAG_YSIZE );
	pLeft->SetVisibleSingle ( TRUE );
	RegisterControl ( pLeft );

	CUIControl* pRight = new CUIControl;
	pRight->CreateSub ( this, "RAN_BUTTON_DEFAULT_LINE_RIGHT" , UI_FLAG_RIGHT | UI_FLAG_YSIZE );
	pRight->SetVisibleSingle ( TRUE );
	RegisterControl ( pRight );

	CUIControl* pTop = new CUIControl;
	pTop->CreateSub ( this, "RAN_BUTTON_DEFAULT_LINE_TOP" , UI_FLAG_XSIZE );
	pTop->SetVisibleSingle ( TRUE );
	RegisterControl ( pTop );

	CUIControl* pButtom = new CUIControl;
	pButtom->CreateSub ( this, "RAN_BUTTON_DEFAULT_LINE_BOTTOM" , UI_FLAG_BOTTOM | UI_FLAG_XSIZE );
	pButtom->SetVisibleSingle ( TRUE );
	RegisterControl ( pButtom );

	m_wFlipMode = CLICK_FLIP;

	CUIControl TempControl;
	TempControl.Create ( 1, szControlKeyword );
	const UIRECT& rcParentOldPos = GetLocalPos ();
	const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
	AlignSubControl ( rcParentOldPos, rcParentNewPos );
	SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );
}

void CBasicButton::CreateRanButtonRed( CUIControl* pParent, const char* szControlKeyword, UIGUID WndID )
{	
	CreateSub( pParent, "RAN_BUTTON", UI_FLAG_XSIZE | UI_FLAG_YSIZE, WndID );

	CUIControl* pDefault = new CUIControl;
	pDefault->CreateSub ( this, "RAN_RED_BUTTON_DEFAULT" , UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pDefault->SetVisibleSingle ( TRUE );
	RegisterControl ( pDefault );

	CUIControl* pMouseOver = new CUIControl;
	pMouseOver->CreateSub ( this, "RAN_RED_BUTTON_MOUSEIN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pMouseOver->SetVisibleSingle ( FALSE );
	RegisterControl ( pMouseOver );
	m_pMouseOver = pMouseOver;

	CUIControl* pFlipControl = new CUIControl;
	pFlipControl->CreateSub ( this, "RAN_RED_BUTTON_CLICK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pFlipControl->SetVisibleSingle ( FALSE );
	RegisterControl ( pFlipControl );
	m_pFlipControl = pFlipControl;

	CUIControl* pLeft = new CUIControl;
	pLeft->CreateSub ( this, "RAN_BUTTON_DEFAULT_LINE_LEFT" , UI_FLAG_YSIZE );
	pLeft->SetVisibleSingle ( TRUE );
	RegisterControl ( pLeft );

	CUIControl* pRight = new CUIControl;
	pRight->CreateSub ( this, "RAN_BUTTON_DEFAULT_LINE_RIGHT" , UI_FLAG_RIGHT | UI_FLAG_YSIZE );
	pRight->SetVisibleSingle ( TRUE );
	RegisterControl ( pRight );

	CUIControl* pTop = new CUIControl;
	pTop->CreateSub ( this, "RAN_BUTTON_DEFAULT_LINE_TOP" , UI_FLAG_XSIZE );
	pTop->SetVisibleSingle ( TRUE );
	RegisterControl ( pTop );

	CUIControl* pButtom = new CUIControl;
	pButtom->CreateSub ( this, "RAN_BUTTON_DEFAULT_LINE_BOTTOM" , UI_FLAG_BOTTOM | UI_FLAG_XSIZE );
	pButtom->SetVisibleSingle ( TRUE );
	RegisterControl ( pButtom );

	m_wFlipMode = CLICK_FLIP;

	CUIControl TempControl;
	TempControl.Create ( 1, szControlKeyword );
	const UIRECT& rcParentOldPos = GetLocalPos ();
	const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
	AlignSubControl ( rcParentOldPos, rcParentNewPos );
	SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );
}

void CBasicButton::CreateMouseOver ( char* KeyValue )
{	
	if ( m_wFlipMode == NOT_READY )
	{
		GASSERT ( 0 && "CreateFlip() ???? ?????? ???????? ??????." );
		return ;
	}
	if ( m_wFlipMode == MOUSEIN_FLIP )
	{
		GASSERT ( 0 && "???????? ?????????? ???? ?????? ???????????? ???? ?????? ?????? ????????." );
		return ;
	}

	CUIControl* pMouseOver = new CUIControl;
	pMouseOver->CreateSub ( this, KeyValue );
	pMouseOver->SetVisibleSingle ( FALSE );
	RegisterControl ( pMouseOver );
	m_pMouseOver = pMouseOver;
}

void CBasicButton::CreateFlip( const char *keyvalue, WORD FlipType )
{
	CUIControl* pFlipControl = new CUIControl;
	pFlipControl->CreateSub ( this, keyvalue );
	pFlipControl->SetVisibleSingle ( FALSE );
	RegisterControl ( pFlipControl );
	m_pFlipControl = pFlipControl;

	m_wFlipMode = FlipType;
}

void CBasicButton::Reset()
{
	CUIControl::Reset();
	m_bDown = FALSE;
	m_bMouseIn = FALSE;
}

void CBasicButton::SetFlip()
{
	switch ( m_wFlipMode )
	{
	case CLICK_FLIP:
	case KEEP_FLIP:
	case RADIO_FLIP:
		m_bDown=TRUE;
		break;
	case MOUSEIN_FLIP:
		m_bMouseIn=TRUE;
		break;
	}
}

void CBasicButton::SetFlip ( BOOL bFlip )
{
	if ( m_pFlipControl ) m_pFlipControl->SetVisibleSingle ( bFlip );
	if ( m_pMouseOver && bFlip )
	{
		m_pMouseOver->SetVisibleSingle ( FALSE );
	}

	if ( IsUseDynamic() )
	{
		CUIControl::SetDiffuse ( D3DCOLOR_ARGB ( 255, 255, 255, 255 ) );
		if ( m_bFlip )
		{
			CUIControl::SetDiffuse ( D3DCOLOR_ARGB ( 0, 255, 255, 255 ) );
			SetFlipPosition ();
		}
	}

	m_bFlip = bFlip;
}

void CBasicButton::TranslateMouseMessage ( DWORD dwMsg )
{	
	BOOL bMouseOver = FALSE;
	if ( CHECK_MOUSE_IN ( dwMsg ) )	bMouseOver = TRUE;
	if ( m_pMouseOver )	m_pMouseOver->SetVisibleSingle ( bMouseOver );

	switch ( m_wFlipMode )
	{
	case CLICK_FLIP:
		{			
			if ( CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				SetFlip ( TRUE );
			}			
		}
		break;

	case MOUSEIN_FLIP:
		{
			//	?????? ??
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SetFlip ( TRUE );
			}

		}
		break;

	case KEEP_FLIP:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_bOnBack = m_bOn;
					m_bOn = !m_bOn;

//					SoundPlay ();
				}
				else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					//	?????? ???????? ?????? ??????
					//	?????? ?????? ?????? ????
					//	???? ?????? ???????????? ??????.
					m_bOnBack = !m_bOnBack;
					m_bOn = m_bOnBack;						
				}
			}
			else
			{
				m_bOn = m_bOnBack;
			}

			SetFlip ( m_bFlip );
		}
		break;

	case RADIO_FLIP:
		{	
			CUIControl::SetDiffuse ( D3DCOLOR_ARGB(255,255,255,255) );			
			if ( m_bFlip )
			{
                CUIControl::SetDiffuse ( D3DCOLOR_ARGB(0,0,0,0) );				
			}			
		}
		break;
	}
}

void CBasicButton::SetFlipPosition ()
{
	if ( m_pFlipControl )
	{
		if ( !IsFlip () )
		{
			const UIRECT& rcBackPos = GetGlobalPos ();
			m_pFlipControl->SetGlobalPos ( D3DXVECTOR2 ( rcBackPos.left + fBUTTON_CLICK, rcBackPos.top + fBUTTON_CLICK ) );
		}
	}
}

void CBasicButton::ResetFlipPosition ()
{
	if ( m_pFlipControl )
	{
		if ( IsFlip () )
		{
			const UIRECT& rcBackPos = GetGlobalPos ();
			m_pFlipControl->SetGlobalPos ( D3DXVECTOR2 ( rcBackPos.left, rcBackPos.top ) );
		}
	}
}


void CBasicButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( IsUseDynamic() )
	{
		ResetFlipPosition ();
	}

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pMouseOver )	m_pMouseOver->SetVisibleSingle ( FALSE );
	if ( m_wFlipMode != KEEP_FLIP && m_wFlipMode != RADIO_FLIP ) SetFlip ( FALSE );

	if ( bFirstControl )
	{		
		CUIControl* pParent = GetTopParent ();
		BOOL bFocusControl = ( pParent && pParent->IsFocusControl() ) ? TRUE : FALSE;
		if ( bFocusControl || m_bUseGlobalAction )
		{
			TranslateMouseMessage ( GetMessageEx () );
		}

		if ( bFocusControl )
		{
			if ( m_KeyCode || m_KeyCodeEx )
			{
				if ( UIKeyCheck::GetInstance()->Check ( m_KeyCode, DXKEY_DOWN ) )
				{
					AddMessageEx ( UIMSG_KEY_FOCUSED );
				}

				if ( UIKeyCheck::GetInstance()->Check ( m_KeyCodeEx, DXKEY_DOWN ) )
				{
					AddMessageEx ( UIMSG_KEY_FOCUSED );
				}
			}
		}
	}
}

BOOL CBasicButton::IsVIsibleMouseOver ()
{
	if ( !m_pMouseOver )
	{
		GASSERT ( 0 && "?????? ???? ???????? ?????????? ??????????." );
		return FALSE;
	}

	return m_pMouseOver->IsVisible ();
}

void CBasicButton::SetVisibleMouseOver ( BOOL bVisible )
{
	if ( !m_pMouseOver )
	{
		GASSERT ( 0 && "?????? ???? ???????? ?????????? ??????????." );
		return ;
	}

	m_pMouseOver->SetVisibleSingle ( bVisible );
}

void CBasicButton::DoFlipUpdate ()
{
	CUIControl::SetDiffuse ( D3DCOLOR_ARGB(255,255,255,255) );			
	if ( m_bFlip )
	{
        CUIControl::SetDiffuse ( D3DCOLOR_ARGB(0,0,0,0) );				
	}	
}