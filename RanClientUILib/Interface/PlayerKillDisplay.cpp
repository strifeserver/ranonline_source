#include "StdAfx.h"
#include "PlayerKillDisplay.h"
#include "PlayerKillType.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"
#include "DxViewPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const float fKeepTime = 5.0f;

CPlayerKillStreakDisplay::CPlayerKillStreakDisplay ()
{
}

CPlayerKillStreakDisplay::~CPlayerKillStreakDisplay ()
{
}

void CPlayerKillStreakDisplay::CreateSubControl ()
{
	CString strKeyword[MAX_TYPE] = 
	{
		"PK_FIRST_BLOOD",
		"PK_DOUBLE_KILL",
		"PK_TRIPLE_KILL",
		"PK_ULTRA_KILL",
		"PK_RAMPAGE",
		"PK_KILLING_SPREE",
		"PK_DOMINATING",
		"PK_MEGA_KILL",
		"PK_UNSTOPPABLE",
		"PK_WICKED_SICK",
		"PK_MONSTER_KILL",
		"PK_GODLIKE",
		"PK_HOLY_SHIT",
		"PK_OWNAGE"
	};

	for ( int i = 0; i < MAX_TYPE; ++i )
	{
		m_pKillStreak_TYPE[i] = new CPlayerKillStreakType;
		m_pKillStreak_TYPE[i]->CreateSub ( this, strKeyword[i].GetString(), UI_FLAG_DEFAULT, FIRST_BLOOD + i );
		m_pKillStreak_TYPE[i]->CreateSubControl ( strKeyword[i] );
		m_pKillStreak_TYPE[i]->SetVisibleSingle ( FALSE );
		m_pKillStreak_TYPE[i]->SetUseRender ( FALSE );
		m_pKillStreak_TYPE[i]->STOP ();
		m_pKillStreak_TYPE[i]->RESET ();		
		RegisterControl ( m_pKillStreak_TYPE[i] );

		m_pKillStreak_KEEP[i] = new CUIControl;
		m_pKillStreak_KEEP[i]->CreateSub ( this, strKeyword[i].GetString() );
		m_pKillStreak_KEEP[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pKillStreak_KEEP[i] );
	}

	m_pPositionControl = new CUIControl;
	m_pPositionControl->CreateSub ( this, "PLAYERKILL_DISPLAY_POSITION" );
	m_pPositionControl->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pPositionControl );
}

bool	CPlayerKillStreakDisplay::START ( UIGUID cID )
{
	if ( cID < FIRST_BLOOD || OWNAGE < cID ) return false;

	int nIndex = cID - FIRST_BLOOD;
	m_pKillStreak_TYPE[nIndex]->SetVisibleSingle ( TRUE );
    m_pKillStreak_TYPE[nIndex]->START ();

	return true;
}

bool	CPlayerKillStreakDisplay::RESET ( UIGUID cID )
{
	if ( cID < FIRST_BLOOD || OWNAGE < cID ) return false;

	int nIndex = cID - FIRST_BLOOD;	
	m_pKillStreak_TYPE[nIndex]->RESET ();

    return true;
}

bool	CPlayerKillStreakDisplay::STOP ( UIGUID cID )
{
	if ( cID < FIRST_BLOOD || OWNAGE < cID ) return false;

	int nIndex = cID - FIRST_BLOOD;	

	m_pKillStreak_TYPE[nIndex]->STOP ();
	m_pKillStreak_TYPE[nIndex]->SetVisibleSingle ( FALSE );

	return true;
}

void CPlayerKillStreakDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	D3DXVECTOR3 vPos = GLGaeaClient::GetInstance().GetCharacter()->GetPosBodyHeight();

	static D3DXVECTOR3 vScreenBack;
	D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );

	// 마우스 움직임에 흔들림을 보정한다.
	if( abs( vScreenBack.x - vScreen.x ) < 1.0f )
	{
		vScreen.x = vScreenBack.x;
	}
	
	bool bPLAYING( false );

	float fTime = 0.0f;

	for ( int i = 0; i < MAX_TYPE; ++i )
	{
		const UIRECT& rcOriginPos = m_pKillStreak_TYPE[i]->GetGlobalPos ();

		D3DXVECTOR2 vPos2;
		vPos2.x = floor(vScreen.x - ( rcOriginPos.sizeX * 0.5f ));
		vPos2.y = m_pPositionControl->GetGlobalPos().top;

		if ( m_pKillStreak_TYPE[i]->ISPLAYING () )
		{
			m_pKillStreak_TYPE[i]->SetGlobalPos ( vPos2 );

			bPLAYING = true;
		}
		else
		{
			STOP ( FIRST_BLOOD + i);
		}
	}

	vScreenBack = vScreen;

	if ( !bPLAYING )	SetVisibleSingle( FALSE );
}