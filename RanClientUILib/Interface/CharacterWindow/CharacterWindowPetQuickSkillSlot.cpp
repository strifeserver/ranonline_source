#include "StdAfx.h"
#include "CharacterWindowPetQuickSkillSlot.h"

#include "GLGaeaClient.h"
#include "../PetSkillImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterWindowPetQuickSkillSlot::CCharacterWindowPetQuickSkillSlot ()
	: m_pSkillImage ( NULL )
	, m_pSkillMouseOver ( NULL )
	, m_pd3dDevice ( NULL )
	//, nSkillSlot(0)
{
}

CCharacterWindowPetQuickSkillSlot::~CCharacterWindowPetQuickSkillSlot ()
{
}

void  CCharacterWindowPetQuickSkillSlot::CreateSubControl ()
{
	CreateSkillImage ();
	CreateMouseOver ();
}

void CCharacterWindowPetQuickSkillSlot::CreateSkillImage ()
{
	CPetSkillImage* pSkillImage = new CPetSkillImage;
	pSkillImage->CreateSub ( this, "PET_SKILL_IMAGE" );
	pSkillImage->CreateSubControl ();
	pSkillImage->SetUseRender ( TRUE );
	pSkillImage->SetVisibleSingle ( FALSE );
    RegisterControl ( pSkillImage );
	m_pSkillImage = pSkillImage;
}

void CCharacterWindowPetQuickSkillSlot::CreateMouseOver ()
{
	CUIControl* pSkillMouseOver = new CUIControl;
	pSkillMouseOver->CreateSub ( this, "PET_SKILL_MOUSE_OVER", UI_FLAG_DEFAULT, QUICK_SKILL_MOUSE_OVER );
	pSkillMouseOver->SetVisibleSingle ( TRUE );
	RegisterControl ( pSkillMouseOver );
	m_pSkillMouseOver = pSkillMouseOver;
}

void CCharacterWindowPetQuickSkillSlot::UpdateSlot( SNATIVEID sNativeID )
{	
	if ( !m_pSkillImage )
	{
		GASSERT ( 0 && "스킬 이미지가 만들어지지 않았습니다."  );
		return ;
	}

	if ( sNativeID != NATIVEID_NULL() )
	{
		m_pSkillImage->SetSkill ( sNativeID );
		m_pSkillImage->SetVisibleSingle ( TRUE );
		m_pSkillImage->SetSkillProcess ( sNativeID );
	}
	else
	{
		m_pSkillImage->SetVisibleSingle ( FALSE );
		m_pSkillImage->ResetSkill ();
	}	
}

void CCharacterWindowPetQuickSkillSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetPetClient()->m_sActiveSkillID/*[nSkillSlot]*/;
	UpdateSlot( sNativeID );
}

UIRECT CCharacterWindowPetQuickSkillSlot::GetAbsPosSkillImage ()
{	
	if ( !m_pSkillImage )
	{
		GASSERT ( 0 && "m_pSkillImage가 NULL입니다." );
		return UIRECT ();
	}
	
	return m_pSkillImage->GetGlobalPos();	
}