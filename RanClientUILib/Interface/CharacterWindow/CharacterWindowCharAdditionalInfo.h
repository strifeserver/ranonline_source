#pragma	once

#include "../Interface/UIWindowEx.h"
#include "GLCharDefine.h"
#include "../RanClientLib/G-Logic/GLogicEx.h"

class	CD3DFontPar;
class	CBasicProgressBar;//qweq
class	CBasicTextBox;
class	CBasicButtonText;
class	CBasicButton;//qweq
class	CCharacterWindowCharStatMark;
struct	GLCHARLOGIC;

class CCharacterWindowCharAdditionalInfo : public CUIWindowEx
{
protected:
	enum
	{
		CHARACTER_STAT_HELP = ET_CONTROL_NEXT + 1,

		MOUSE_OVER_STAT_2_CRITICAL,
		MOUSE_OVER_STAT_2_STRIKE,
		MOUSE_OVER_STAT_2_MOVINGSPEED,
		MOUSE_OVER_STAT_2_ATTACKSPEED,
		MOUSE_OVER_STAT_2_EXP,
		MOUSE_OVER_STAT_2_HPRECOVERY,
		MOUSE_OVER_STAT_2_MPRECOVERY,
		MOUSE_OVER_STAT_2_SPRECOVERY,
		MOUSE_OVER_STAT_2_CPRECOVERY,
		MOUSE_OVER_STAT_2_POTIONHP,
		MOUSE_OVER_STAT_2_POTIONMP,
		MOUSE_OVER_STAT_3_POTIONSP,
		MOUSE_OVER_STAT_3_FIRE,
		MOUSE_OVER_STAT_3_ICE,
		MOUSE_OVER_STAT_3_ELECTRIC,
		MOUSE_OVER_STAT_3_POISON,
		MOUSE_OVER_STAT_3_SPIRIT,

		MOUSE_OVER_BUTTONS = 17,
	};

public:
    CCharacterWindowCharAdditionalInfo ();
	virtual	~CCharacterWindowCharAdditionalInfo ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
//	virtual void SetGlobalPos(const D3DXVECTOR2& vPos);

public:
	void	CreateSubControl ();

private:
	CBasicTextBox*	CreateStaticControl ( char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	CUIControl*		CreateControl ( const char* szControl, const UIGUID& cID = NO_ID );
	CBasicButtonText*	CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText );
	CBasicButtonText*	CreateButtonRed ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText );
	CBasicButton*	CreateMouseIn( const char* szButton, const char* szMouseIn ,UIGUID ControlID );
	CCharacterWindowCharStatMark*		CreateStatMark( const char* szControl, const UIGUID& cID = NO_ID );

private:
	void	SetPointControl ();

private:
	void	SetData ( const GLCHARLOGIC& sCharData );
	void	SetUpData ( const GLCHARLOGIC& sCharData );
	void	ShowTip( int nSelect );

protected:
	GLCHARLOGIC		m_sCharLogic;

private:
	CUIControl*				m_pBackGround;

	CBasicTextBox*	m_pCriticalRateText;
	CBasicTextBox*	m_pStrikeHitText;
	CBasicTextBox*	m_pMovingSpeedText;
	CBasicTextBox*	m_pAttackSpeedText;
	CBasicTextBox*	m_pEXPText;
	CBasicTextBox*	m_pHPRecoveryText;
	CBasicTextBox*	m_pMPRecoveryText;
	CBasicTextBox*	m_pSPRecoveryText;
	CBasicTextBox*	m_pCPRecoveryText;
	CBasicTextBox*	m_pPotionHPText;
	CBasicTextBox*	m_pPotionMPText;
	CBasicTextBox*	m_pPotionSPText;
	CBasicTextBox*	m_pFireText;
	CBasicTextBox*	m_pIceText;
	CBasicTextBox*	m_pElectricText;
	CBasicTextBox*	m_pPoisonText;
	CBasicTextBox*	m_pSpiritText;

	CBasicButton*	m_pHelp;

	CBasicButton*	m_pMouseOver[MOUSE_OVER_BUTTONS];

	CBasicTextBox*	m_pCriticalRateMarkStatic;
	CBasicTextBox*	m_pStrikeHitMarkStatic;
	CBasicTextBox*	m_pMovingSpeedMarkStatic;
	CBasicTextBox*	m_pAttackSpeedMarkStatic;
	CBasicTextBox*	m_pEXPMarkStatic;
	CBasicTextBox*	m_pHPRecoveryMarkStatic;
	CBasicTextBox*	m_pMPRecoveryMarkStatic;
	CBasicTextBox*	m_pSPRecoveryMarkStatic;
	CBasicTextBox*	m_pCPRecoveryMarkStatic;
	CBasicTextBox*	m_pPotionHPMarkStatic;
	CBasicTextBox*	m_pPotionMPMarkStatic;
	CBasicTextBox*	m_pPotionSPMarkStatic;
	CBasicTextBox*	m_pFireMarkStatic;
	CBasicTextBox*	m_pIceMarkStatic;
	CBasicTextBox*	m_pElectricMarkStatic;
	CBasicTextBox*	m_pPoisonMarkStatic;
	CBasicTextBox*	m_pSpiritMarkStatic;

	CBasicTextBox*	m_pCriticalRateMarkValue;
	CBasicTextBox*	m_pStrikeHitMarkValue;
	CBasicTextBox*	m_pMovingSpeedMarkValue;
	CBasicTextBox*	m_pAttackSpeedMarkValue;
	CBasicTextBox*	m_pEXPMarkValue;
	CBasicTextBox*	m_pHPRecoveryMarkValue;
	CBasicTextBox*	m_pMPRecoveryMarkValue;
	CBasicTextBox*	m_pSPRecoveryMarkValue;
	CBasicTextBox*	m_pCPRecoveryMarkValue;
	CBasicTextBox*	m_pPotionHPMarkValue;
	CBasicTextBox*	m_pPotionMPMarkValue;
	CBasicTextBox*	m_pPotionSPMarkValue;
	CBasicTextBox*	m_pFireMarkValue;
	CBasicTextBox*	m_pIceMarkValue;
	CBasicTextBox*	m_pElectricMarkValue;
	CBasicTextBox*	m_pPoisonMarkValue;
	CBasicTextBox*	m_pSpiritMarkValue;

	CCharacterWindowCharStatMark*	m_pCriticalRateMark;
	CCharacterWindowCharStatMark*	m_pStrikeHitMark;
	CCharacterWindowCharStatMark*	m_pMovingSpeedMark;
	CCharacterWindowCharStatMark*	m_pAttackSpeedMark;
	CCharacterWindowCharStatMark*	m_pEXPMark;
	CCharacterWindowCharStatMark*	m_pHPRecoveryMark;
	CCharacterWindowCharStatMark*	m_pMPRecoveryMark;
	CCharacterWindowCharStatMark*	m_pSPRecoveryMark;
	CCharacterWindowCharStatMark*	m_pCPRecoveryMark;
	CCharacterWindowCharStatMark*	m_pPotionHPMark;
	CCharacterWindowCharStatMark*	m_pPotionMPMark;
	CCharacterWindowCharStatMark*	m_pPotionSPMark;
	CCharacterWindowCharStatMark*	m_pFireMark;
	CCharacterWindowCharStatMark*	m_pIceMark;
	CCharacterWindowCharStatMark*	m_pElectricMark;
	CCharacterWindowCharStatMark*	m_pPoisonMark;
	CCharacterWindowCharStatMark*	m_pSpiritMark;

public:
	void	InitData();
	DWORD	GetTextColor( float nVal );
};