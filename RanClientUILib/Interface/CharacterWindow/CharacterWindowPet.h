#pragma	once
//r
#include "../Interface/UIWindowEx.h"
#include "GLCharDefine.h"
#include "../RanClientLib/G-Logic/GLItem.h"
#include "../RanClientLib/G-Logic/GLItemMan.h"

const DWORD UIMSG_MOUSEIN_WEARSLOT = UIMSG_USER1;

class	CD3DFontPar;
class	CBasicTextBox;
class	CBasicButton;
class	CBasicButtonRadio;
class	CCharacterWindowPetRender;
class	CItemImage;
class	CBasicComboBox;
class	CBasicComboBoxRollOver;
class	CCharacterWindowPetQuickSkillSlot;
class	CBasicButtonText;


class CCharacterWindowPet : public CUIGroup
{
protected:
	enum
	{
		CHARACTER_PET_SLOT_IMAGE_TYPE1 = NO_ID + 1,
		CHARACTER_PET_SLOT_IMAGE_TYPE2,
		CHARACTER_PET_SKILL_COMBO_OPEN0,
		CHARACTER_PET_SKILL_COMBO_OPEN1,
		CHARACTER_PET_SKILL_COMBO_ROLLOVER0,
		CHARACTER_PET_SKILL_COMBO_ROLLOVER1,
		CHARACTER_PET_SKILL_SLOT0,
		CHARACTER_PET_SKILL_SLOT1,
		CHARACTER_PET_ICON,
		CHARACTER_PET_SKILL_BUTTON,
		CHARACTER_PET_BUTTON_LEFT,
		CHARACTER_PET_BUTTON_RIGHT,

		SLOT_IMAGE_SIZE = 2,
	};

public:
    CCharacterWindowPet ();
	virtual	~CCharacterWindowPet ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	CUIControl*				CreateControl ( const char* szControl, const UIGUID& cID = NO_ID );
	CBasicTextBox *			CreateTextBox( char* szKeyword, CD3DFontPar* pFont );
	CItemImage*				CreateItemImage( const char* szControl, UIGUID ControlID );

public:
	void			CreateSubControl ();


public:
	CCharacterWindowPetRender*	m_pRender;
	CBasicTextBox*				m_pPetName;
	CBasicTextBox*				m_pPetType;
	CBasicTextBox*				m_pPetFood;
	CBasicTextBox*				m_pPetDualSkill;

	CUIControl*					m_pMouseOver;
	CUIControl*					m_pSlotDisplay[SLOT_IMAGE_SIZE];
	CItemImage*					m_pSlotImage[SLOT_IMAGE_SIZE];
	SITEMCUSTOM					m_ItemCustomArray[SLOT_IMAGE_SIZE];

	CBasicComboBox*				m_pcbSkill0;
	CBasicComboBox*				m_pcbSkill1;
	CBasicComboBoxRollOver*		m_pcbSkillRollOver0;
	CBasicComboBoxRollOver*		m_pcbSkillRollOver1;

	CCharacterWindowPetQuickSkillSlot*	m_pPetSkillSlot0;
	CCharacterWindowPetQuickSkillSlot*	m_pPetSkillSlot1;

	CItemImage*					m_pPetIcon;
	CBasicButtonText*			m_pSkillButton;

	CBasicButton*				m_pButtonLeft;
	CBasicButton*				m_pButtonRight;

public:
	BOOL			m_bVALIDPET;
	BOOL			m_bDualSkillBack;
	int				m_nFoodLast;
	UIGUID			m_RollOverID;
	BOOL			m_bFirstLBUP;
	std::string		m_strNameLast;

public:
	void	ClearRender();
	void	ShowPet();
	void	ClearPet();
	void	SetPetFood( int nFULL );
	void	SetSkillCombo();
	void	LoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom );
	void	UnLoadItem ( int nIndex );
	EMSUIT	IMAGE2EMSLOT ( UIGUID ControlID );
	void	SetName( const std::string & strName );
	void	SetType( PETTYPE emTYPE );
};