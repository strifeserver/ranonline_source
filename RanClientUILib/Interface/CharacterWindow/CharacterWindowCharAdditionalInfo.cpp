#include "StdAfx.h"
#include "./CharacterWindowCharAdditionalInfo.h"

#include "../EngineUiLib/GUInterface/BasicTextBox.h"
#include "../EngineUiLib/GUInterface/BasicProgressBar.h"
#include "../EngineUiLib/GUInterface/BasicButtonText.h"
#include "../EngineUiLib/GUInterface/BasicButton.h"
#include "../EngineLib/DxCommon/d3dfont.h"

#include "../BasicTextButton.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../RanClientLib/G-Logic/GLGaeaClient.h"

#include "../InnerInterface.h"
#include "../ModalCallerID.h"
#include "../ModalWindow.h"

#include "../BasicLineBox.h"
#include "GLItemMan.h"
#include "GLItem.h"

#include "../BasicLineBox.h"
#include "../BasicLineBoxEx.h"
#include "CharacterWindowCharStatMark.h"

CCharacterWindowCharAdditionalInfo::CCharacterWindowCharAdditionalInfo () :
	m_pBackGround( NULL ),
	m_pCriticalRateText ( NULL ),
	m_pStrikeHitText ( NULL ),
	m_pMovingSpeedText ( NULL ),
	m_pAttackSpeedText ( NULL ),
	m_pEXPText ( NULL ),
	m_pHPRecoveryText ( NULL ),
	m_pMPRecoveryText ( NULL ),
	m_pSPRecoveryText ( NULL ),
	m_pCPRecoveryText ( NULL ),
	m_pPotionHPText ( NULL ),
	m_pPotionMPText ( NULL ),
	m_pPotionSPText ( NULL ),
	m_pHelp ( NULL ),
	m_pCriticalRateMarkStatic ( NULL ),
	m_pStrikeHitMarkStatic ( NULL ),
	m_pMovingSpeedMarkStatic ( NULL ),
	m_pAttackSpeedMarkStatic ( NULL ),
	m_pEXPMarkStatic ( NULL ),
	m_pHPRecoveryMarkStatic ( NULL ),
	m_pMPRecoveryMarkStatic ( NULL ),
	m_pSPRecoveryMarkStatic ( NULL ),
	m_pCPRecoveryMarkStatic ( NULL ),
	m_pPotionHPMarkStatic ( NULL ),
	m_pPotionMPMarkStatic ( NULL ),
	m_pPotionSPMarkStatic ( NULL ),
	m_pCriticalRateMarkValue ( NULL ),
	m_pStrikeHitMarkValue ( NULL ),
	m_pMovingSpeedMarkValue ( NULL ),
	m_pAttackSpeedMarkValue ( NULL ),
	m_pEXPMarkValue ( NULL ),
	m_pHPRecoveryMarkValue ( NULL ),
	m_pMPRecoveryMarkValue ( NULL ),
	m_pSPRecoveryMarkValue ( NULL ),
	m_pCPRecoveryMarkValue ( NULL ),
	m_pPotionHPMarkValue ( NULL ),
	m_pPotionMPMarkValue ( NULL ),
	m_pPotionSPMarkValue ( NULL ),
	//added by CNDev
	m_pFireMarkValue ( NULL ),
	m_pIceMarkValue ( NULL ),
	m_pElectricMarkValue ( NULL ),
	m_pPoisonMarkValue ( NULL ),
	m_pSpiritMarkValue ( NULL )
{
	SecureZeroMemory( m_pMouseOver, sizeof(m_pMouseOver) );
}

CCharacterWindowCharAdditionalInfo::~CCharacterWindowCharAdditionalInfo ()
{
}

CBasicTextBox* CCharacterWindowCharAdditionalInfo::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

CUIControl*	CCharacterWindowCharAdditionalInfo::CreateControl ( const char* szControl, const UIGUID& cID )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	RegisterControl ( pControl );
	return pControl;
}

CBasicButton* CCharacterWindowCharAdditionalInfo::CreateMouseIn( const char* szButton, const char* szMouseIn ,UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szMouseIn, CBasicButton::MOUSEIN_FLIP );
	RegisterControl ( pButton );
	return pButton;
}

CCharacterWindowCharStatMark*	CCharacterWindowCharAdditionalInfo::CreateStatMark ( const char* szControl, const UIGUID& cID )
{
	CCharacterWindowCharStatMark* pControl = new CCharacterWindowCharStatMark;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateSubControl();
	RegisterControl ( pControl );
	return pControl;
}

void CCharacterWindowCharAdditionalInfo::CreateSubControl ()
{

/*	m_pBackGround = new CUIControl;
	m_pBackGround->CreateSub ( this, "CHARACTER_WINDOW_ADDITIONAL_BACKGROUND", UI_FLAG_DEFAULT );	
	m_pBackGround->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pBackGround );

	char* szLineBox[2] = 
	{
		"CHARACTER_WINDOW_ADDITIONAL_STAT_POINT1_REGION",
		"CHARACTER_WINDOW_ADDITIONAL_STAT_POINT2_REGION",
		//"CHARACTER_WINDOW_STAT_POINT9_REGION",
	};

	for ( int i=0; i<2; ++i )
	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxQuestList( szLineBox[i] );
		RegisterControl ( pBasicLineBox );
	}*/
	
	{	// Đen Xám Đen Background
		CBasicLineBoxEx* pBasicLineBoxEx0 = new CBasicLineBoxEx;
		pBasicLineBoxEx0->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_DENXAM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx0->CreateBaseBoxDenXamBox ( "CHARACTER_WINDOW_ADDITIONAL_BACKGROUND" );
		RegisterControl ( pBasicLineBoxEx0 );
	}
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx1 = new CBasicLineBoxEx;
		pBasicLineBoxEx1->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_LOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx1->CreateBaseBoxLomBox ( "CHARACTER_WINDOW_ADDITIONAL_STAT_POINT1_REGION" );
		RegisterControl ( pBasicLineBoxEx1 );
	}
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx2 = new CBasicLineBoxEx;
		pBasicLineBoxEx2->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_LOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx2->CreateBaseBoxLomBox ( "CHARACTER_WINDOW_ADDITIONAL_STAT_POINT2_REGION" );
		RegisterControl ( pBasicLineBoxEx2 );
	}
	
	CD3DFontPar* pFontShadow = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );

	const int nAlignLeft = TEXT_ALIGN_LEFT;
	const int nAlignCenter = TEXT_ALIGN_CENTER_X;
	const int nAlignCenterBoth = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;
	const int nAlignRight = TEXT_ALIGN_RIGHT;
	const DWORD& dwSilver = NS_UITEXTCOLOR::SILVER;
	const DWORD& dwWhite = NS_UITEXTCOLOR::WHITE;
	const DWORD& dwSubTitle = NS_UITEXTCOLOR::PALEGOLDENROD;

	//mouse in
	m_pMouseOver[5] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_CRITICAL_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_CRITICAL );
	m_pMouseOver[6] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_STRIKE_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_STRIKE );
	m_pMouseOver[7] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_MOVINGSPEED_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_MOVINGSPEED );
	m_pMouseOver[8] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_ATTACKSPEED_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_ATTACKSPEED );
	m_pMouseOver[9] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_EXP_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_EXP );
	m_pMouseOver[10] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_HPRECOVERY_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_HPRECOVERY );
	m_pMouseOver[11] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_MPRECOVERY_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_MPRECOVERY );
	m_pMouseOver[12] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_SPRECOVERY_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_SPRECOVERY );
	m_pMouseOver[13] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_CPRECOVERY_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_CPRECOVERY );
	m_pMouseOver[14] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_POTIONHP_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_POTIONHP );
	m_pMouseOver[15] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_POTIONMP_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_2_POTIONMP );
	m_pMouseOver[16] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_POTIONSP_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_3_POTIONSP );
	//added by CNDev
	m_pMouseOver[17] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_FIRE_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_3_FIRE );
	m_pMouseOver[18] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_ICE_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_3_ICE );
	m_pMouseOver[19] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_ELECTRIC_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_3_ELECTRIC );
	m_pMouseOver[20] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_POISON_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_3_POISON );
	m_pMouseOver[21] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_SPIRIT_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_3_SPIRIT );
	
	CBasicTextBox* pTextBox = NULL;
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_CRITICAL_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 0), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_STRIKE_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 1), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MOVINGSPEED_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 2), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACKSPEED_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 3), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_EXP_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 4), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HPRECOVERY_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 5), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MPRECOVERY_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 6), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SPRECOVERY_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 7), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_CPRECOVERY_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 8), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONHP_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 9), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONMP_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 10), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONSP_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 11), dwWhite );
	//added by CNDev
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_FIRE_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 19), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ICE_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 20), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ELECTRIC_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 21), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POISON_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 22), dwWhite );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SPIRIT_TEXT", pFontShadow, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC_ADDITIONAL", 23), dwWhite );

	m_pCriticalRateText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_CRITICAL_VALUE", pFont8, nAlignCenter );
	m_pStrikeHitText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_STRIKE_VALUE",pFont8, nAlignCenter );
	m_pMovingSpeedText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MOVINGSPEED_VALUE", pFont8, nAlignCenter );
	m_pAttackSpeedText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACKSPEED_VALUE", pFont8, nAlignCenter );
	m_pEXPText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_EXP_VALUE", pFont8, nAlignCenter );
	m_pHPRecoveryText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HPRECOVERY_VALUE", pFont8, nAlignCenter );
	m_pMPRecoveryText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MPRECOVERY_VALUE", pFont8, nAlignCenter );
	m_pSPRecoveryText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SPRECOVERY_VALUE", pFont8, nAlignCenter );
	m_pCPRecoveryText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_CPRECOVERY_VALUE", pFont8, nAlignCenter );
	m_pPotionHPText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONHP_VALUE", pFont8, nAlignCenter );
	m_pPotionMPText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONMP_VALUE", pFont8, nAlignCenter );
	m_pPotionSPText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONSP_VALUE", pFont8, nAlignCenter );
	//added by CNDev
	m_pFireText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_FIRE_VALUE", pFont8, nAlignCenter );
	m_pIceText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ICE_VALUE", pFont8, nAlignCenter );
	m_pElectricText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ELECTRIC_VALUE", pFont8, nAlignCenter );
	m_pPoisonText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POISON_VALUE", pFont8, nAlignCenter );
	m_pSpiritText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SPIRIT_VALUE", pFont8, nAlignCenter );

	m_pCriticalRateMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_CRITICAL_MARK", pFont8, nAlignLeft );
	m_pStrikeHitMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_STRIKE_MARK", pFont8, nAlignLeft );
	m_pMovingSpeedMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MOVINGSPEED_MARK", pFont8, nAlignLeft );
	m_pAttackSpeedMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACKSPEED_MARK", pFont8, nAlignLeft );
	m_pEXPMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_EXP_MARK", pFont8, nAlignLeft );
	m_pHPRecoveryMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HPRECOVERY_MARK", pFont8, nAlignLeft );
	m_pMPRecoveryMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MPRECOVERY_MARK", pFont8, nAlignLeft );
	m_pSPRecoveryMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SPRECOVERY_MARK", pFont8, nAlignLeft );
	m_pCPRecoveryMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_CPRECOVERY_MARK", pFont8, nAlignLeft );
	m_pPotionHPMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONHP_MARK", pFont8, nAlignLeft );
	m_pPotionMPMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONMP_MARK", pFont8, nAlignLeft );
	m_pPotionSPMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONSP_MARK", pFont8, nAlignLeft );
	//added by CNDev
	m_pFireMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_FIRE_MARK", pFont8, nAlignLeft );
	m_pIceMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ICE_MARK", pFont8, nAlignLeft );
	m_pElectricMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ELECTRIC_MARK", pFont8, nAlignLeft );
	m_pPoisonMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POISON_MARK", pFont8, nAlignLeft );
	m_pSpiritMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SPIRIT_MARK", pFont8, nAlignLeft );

	
	m_pCriticalRateMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_CRITICAL_UP_VALUE", pFont8, nAlignRight );
	m_pStrikeHitMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_STRIKE_UP_VALUE", pFont8, nAlignRight );
	m_pMovingSpeedMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MOVINGSPEED_UP_VALUE", pFont8, nAlignRight );
	m_pAttackSpeedMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACKSPEED_UP_VALUE", pFont8, nAlignRight );
	m_pEXPMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_EXP_UP_VALUE", pFont8, nAlignRight );
	m_pHPRecoveryMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HPRECOVERY_UP_VALUE", pFont8, nAlignRight );
	m_pMPRecoveryMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MPRECOVERY_UP_VALUE", pFont8, nAlignRight );
	m_pSPRecoveryMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SPRECOVERY_UP_VALUE", pFont8, nAlignRight );
	m_pCPRecoveryMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_CPRECOVERY_UP_VALUE", pFont8, nAlignRight );
	m_pPotionHPMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONHP_UP_VALUE", pFont8, nAlignRight );
	m_pPotionMPMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONMP_UP_VALUE", pFont8, nAlignRight );
	m_pPotionSPMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POTIONSP_UP_VALUE", pFont8, nAlignRight );
	//added by CNDev
	m_pFireMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_FIRE_UP_VALUE", pFont8, nAlignRight );
	m_pIceMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ICE_UP_VALUE", pFont8, nAlignRight );
	m_pElectricMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ELECTRIC_UP_VALUE", pFont8, nAlignRight );
	m_pPoisonMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_POISON_UP_VALUE", pFont8, nAlignRight );
	m_pSpiritMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SPIRIT_UP_VALUE", pFont8, nAlignRight );

	m_pCriticalRateMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_CRITICAL_MARK_IMAGE" );
	m_pStrikeHitMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_STRIKE_MARK_IMAGE" );
	m_pMovingSpeedMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_MOVINGSPEED_MARK_IMAGE" );
	m_pAttackSpeedMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_ATTACKSPEED_MARK_IMAGE" );
	m_pEXPMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_SHOOT_EXP_IMAGE" );
	m_pHPRecoveryMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_HPRECOVERY_MARK_IMAGE" );
	m_pMPRecoveryMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_MPRECOVERY_MARK_IMAGE" );
	m_pSPRecoveryMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_SPRECOVERY_MARK_IMAGE" );
	m_pCPRecoveryMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_CPRECOVERY_MARK_IMAGE" );
	m_pPotionHPMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_POTIONHP_MARK_IMAGE" );
	m_pPotionMPMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_POTIONMP_MARK_IMAGE" );
	m_pPotionSPMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_POTIONSP_MARK_IMAGE" );
	//added by CNDev
	m_pFireMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_FIRE_MARK_IMAGE" );
	m_pIceMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_ICE_MARK_IMAGE" );
	m_pElectricMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_ELECTRIC_MARK_IMAGE" );
	m_pPoisonMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_POISON_MARK_IMAGE" );
	m_pSpiritMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_SPIRIT_MARK_IMAGE" );

	m_pHelp = new CBasicButton;
	m_pHelp->CreateSub ( this, "CHARACTER_WINDOW_ADDITIONAL_STAT_QUESTION_BUTTON" , UI_FLAG_DEFAULT, CHARACTER_STAT_HELP );
	m_pHelp->CreateFlip ( "CHARACTER_WINDOW_ADDITIONAL_STAT_QUESTION_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pHelp->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pHelp );
}

void CCharacterWindowCharAdditionalInfo::SetData( const GLCHARLOGIC& sCharData )
{
	m_pCriticalRateText->ClearText();
	m_pStrikeHitText->ClearText();
	m_pMovingSpeedText->ClearText();
	m_pAttackSpeedText->ClearText();
	m_pEXPText->ClearText();
	m_pHPRecoveryText->ClearText();
	m_pMPRecoveryText->ClearText();
	m_pSPRecoveryText->ClearText();
	m_pCPRecoveryText->ClearText();
	m_pPotionHPText->ClearText();
	m_pPotionMPText->ClearText();
	m_pPotionSPText->ClearText();
	//added by CNDev
	m_pFireText->ClearText();
	m_pIceText->ClearText();
	m_pElectricText->ClearText();
	m_pPoisonText->ClearText();
	m_pSpiritText->ClearText();

	SCHARSTATS sADD = sCharData.GETSTATS_ADD();
	SCHARSTATS sITEM = sCharData.GETSTATS_ITEM();

	CString strCombine;
	strCombine.Format ( "%.2f", sCharData.m_fCrit_Rate );
	if ( m_pCriticalRateText ) m_pCriticalRateText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%.2f", sCharData.m_fBlow_Rate );
	if ( m_pStrikeHitText ) m_pStrikeHitText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%.2f", sCharData.m_fSKILL_MOVE );
	if ( m_pMovingSpeedText ) m_pMovingSpeedText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%.2f", sCharData.m_fATTVELO );
	if ( m_pAttackSpeedText ) m_pAttackSpeedText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%.2f", sCharData.m_fEXP_RATE );
	if ( m_pEXPText ) m_pEXPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%2.2f", sCharData.m_fINCR_HP * 100.0f );
	if ( m_pHPRecoveryText ) m_pHPRecoveryText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%2.2f", sCharData.m_fINCR_MP * 100.0f );
	if ( m_pMPRecoveryText ) m_pMPRecoveryText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%2.2f", sCharData.m_fINCR_SP * 100.0f );
	if ( m_pSPRecoveryText ) m_pSPRecoveryText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%2.2f", sCharData.m_fCP_GAIN );
	if ( m_pCPRecoveryText ) m_pCPRecoveryText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%d%", sCharData.m_sSUMITEM.nHP_Potion_Rate );
	if ( m_pPotionHPText ) m_pPotionHPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%d%", sCharData.m_sSUMITEM.nMP_Potion_Rate );
	if ( m_pPotionMPText ) m_pPotionMPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );

	strCombine.Format ( "%d%", sCharData.m_sSUMITEM.nSP_Potion_Rate );
	if ( m_pPotionSPText ) m_pPotionSPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	
	//added by CNDev
	strCombine.Format ( "%d%", sCharData.m_sSUMRESIST_SKILL.nFire );
	if ( m_pFireText ) m_pFireText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	
	strCombine.Format ( "%d%", sCharData.m_sSUMRESIST_SKILL.nIce );
	if ( m_pIceText ) m_pIceText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	
	strCombine.Format ( "%d%", sCharData.m_sSUMRESIST_SKILL.nElectric );
	if ( m_pElectricText ) m_pElectricText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	
	strCombine.Format ( "%d%", sCharData.m_sSUMRESIST_SKILL.nPoison );
	if ( m_pPoisonText ) m_pPoisonText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	
	strCombine.Format ( "%d%", sCharData.m_sSUMRESIST_SKILL.nSpirit );
	if ( m_pSpiritText ) m_pSpiritText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
}

void CCharacterWindowCharAdditionalInfo::SetUpData( const GLCHARLOGIC& sCharData )
{
	//1st version of stats comparison
	//compare stats from character data
	m_pCriticalRateMarkStatic->ClearText();
	m_pStrikeHitMarkStatic->ClearText();
	m_pMovingSpeedMarkStatic->ClearText();
	m_pAttackSpeedMarkStatic->ClearText();
	m_pEXPMarkStatic->ClearText();
	m_pHPRecoveryMarkStatic->ClearText();
	m_pMPRecoveryMarkStatic->ClearText();
	m_pSPRecoveryMarkStatic->ClearText();
	m_pCPRecoveryMarkStatic->ClearText();
	m_pPotionHPMarkStatic->ClearText();
	m_pPotionMPMarkStatic->ClearText();
	m_pPotionSPMarkStatic->ClearText();
	//added by CNDev
	m_pFireMarkStatic->ClearText();
	m_pIceMarkStatic->ClearText();
	m_pElectricMarkStatic->ClearText();
	m_pPoisonMarkStatic->ClearText();
	m_pSpiritMarkStatic->ClearText();

	m_pCriticalRateMarkValue->ClearText();
	m_pStrikeHitMarkValue->ClearText();
	m_pMovingSpeedMarkValue->ClearText();
	m_pAttackSpeedMarkValue->ClearText();
	m_pEXPMarkValue->ClearText();
	m_pHPRecoveryMarkValue->ClearText();
	m_pMPRecoveryMarkValue->ClearText();
	m_pSPRecoveryMarkValue->ClearText();
	m_pCPRecoveryMarkValue->ClearText();
	m_pPotionHPMarkValue->ClearText();
	m_pPotionMPMarkValue->ClearText();
	m_pPotionSPMarkValue->ClearText();
	//added by CNDev
	m_pFireMarkValue->ClearText();
	m_pIceMarkValue->ClearText();
	m_pElectricMarkValue->ClearText();
	m_pPoisonMarkValue->ClearText();
	m_pSpiritMarkValue->ClearText();

	m_pCriticalRateMark->SetVisibleSingle ( FALSE );
	m_pStrikeHitMark->SetVisibleSingle ( FALSE );
	m_pMovingSpeedMark->SetVisibleSingle ( FALSE );
	m_pAttackSpeedMark->SetVisibleSingle ( FALSE );
	m_pEXPMark->SetVisibleSingle ( FALSE );
	m_pHPRecoveryMark->SetVisibleSingle ( FALSE );
	m_pMPRecoveryMark->SetVisibleSingle ( FALSE );
	m_pSPRecoveryMark->SetVisibleSingle ( FALSE );
	m_pCPRecoveryMark->SetVisibleSingle ( FALSE );
	m_pPotionHPMark->SetVisibleSingle ( FALSE );
	m_pPotionMPMark->SetVisibleSingle ( FALSE );
	m_pPotionSPMark->SetVisibleSingle ( FALSE );
	//added by CNDev
	m_pFireMark->SetVisibleSingle ( FALSE );
	m_pIceMark->SetVisibleSingle ( FALSE );
	m_pElectricMark->SetVisibleSingle ( FALSE );
	m_pPoisonMark->SetVisibleSingle ( FALSE );
	m_pSpiritMark->SetVisibleSingle ( FALSE );

	float nCriticalRate = m_sCharLogic.m_fCrit_Rate - sCharData.m_fCrit_Rate;
	float nStrikeHit = m_sCharLogic.m_fBlow_Rate - sCharData.m_fBlow_Rate;
	float nMovingSpeed = m_sCharLogic.m_fSKILL_MOVE - sCharData.m_fSKILL_MOVE;
	float nAttackSpeed = m_sCharLogic.m_fATTVELO - sCharData.m_fATTVELO;
	float nEXP = m_sCharLogic.m_fEXP_RATE - sCharData.m_fEXP_RATE;
	float nHPRecovery = m_sCharLogic.m_fINCR_HP - sCharData.m_fINCR_HP;
	float nMPRecovery = m_sCharLogic.m_fINCR_MP - sCharData.m_fINCR_MP;
	float nSPRecovery = m_sCharLogic.m_fINCR_SP - sCharData.m_fINCR_SP;
	float nCPRecovery = m_sCharLogic.m_fCP_GAIN - sCharData.m_fCP_GAIN;
	int nPotionHP = m_sCharLogic.m_sSUMITEM.nHP_Potion_Rate - sCharData.m_sSUMITEM.nHP_Potion_Rate;
	int nPotionMP = m_sCharLogic.m_sSUMITEM.nMP_Potion_Rate - sCharData.m_sSUMITEM.nMP_Potion_Rate;
	int nPotionSP = m_sCharLogic.m_sSUMITEM.nSP_Potion_Rate - sCharData.m_sSUMITEM.nSP_Potion_Rate;
	//added by CNDev
	int nFireResist = m_sCharLogic.m_sSUMRESIST_SKILL.nFire - sCharData.m_sSUMRESIST_SKILL.nFire;
	int nIceResist = m_sCharLogic.m_sSUMRESIST_SKILL.nIce - sCharData.m_sSUMRESIST_SKILL.nIce;
	int nElectricResist = m_sCharLogic.m_sSUMRESIST_SKILL.nElectric - sCharData.m_sSUMRESIST_SKILL.nElectric;
	int nPoisonResist = m_sCharLogic.m_sSUMRESIST_SKILL.nPoison - sCharData.m_sSUMRESIST_SKILL.nPoison;
	int nSpiritResist = m_sCharLogic.m_sSUMRESIST_SKILL.nSpirit - sCharData.m_sSUMRESIST_SKILL.nSpirit;

	CString strCombine;
	strCombine.Format ( "%s", "-" );
	//Art Gwapo

	if ( nCriticalRate == 0.00f ) 
	{
		m_pCriticalRateMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pCriticalRateMark->SetVisibleSingle ( TRUE );
//		m_pCriticalRateMark->CheckValueFloat( nCriticalRate );
	}

	if ( nStrikeHit == 0.00f )	
	{
		m_pStrikeHitMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pStrikeHitMark->SetVisibleSingle ( TRUE );
//		m_pStrikeHitMark->CheckValueFloat( nStrikeHit );
	}

	if ( nMovingSpeed == 0.00f )		
	{
		m_pMovingSpeedMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pMovingSpeedMark->SetVisibleSingle ( TRUE );
//		m_pMovingSpeedMark->CheckValueFloat( nMovingSpeed );
	}

	if ( nAttackSpeed == 0.00f )		
	{
		m_pAttackSpeedMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pAttackSpeedMark->SetVisibleSingle ( TRUE );
//		m_pAttackSpeedMark->CheckValueFloat( nAttackSpeed );
	}

	if ( nEXP == 0.00f )	
	{
		m_pEXPMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pEXPMark->SetVisibleSingle ( TRUE );
//		m_pEXPMark->CheckValueFloat( nEXP );
	}

	if ( nHPRecovery == 0.00f )		
	{
		m_pHPRecoveryMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pHPRecoveryMark->SetVisibleSingle ( TRUE );
//		m_pHPRecoveryMark->CheckValueFloat( nHPRecovery );
	}

	if ( nMPRecovery == 0.00f )		
	{
		m_pMPRecoveryMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pMPRecoveryMark->SetVisibleSingle ( TRUE );
//		m_pMPRecoveryMark->CheckValueFloat( nMPRecovery );
	}

	if ( nSPRecovery == 0.00f )		
	{
		m_pSPRecoveryMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pSPRecoveryMark->SetVisibleSingle ( TRUE );
//		m_pSPRecoveryMark->CheckValueFloat( nSPRecovery );
	}

	if ( nCPRecovery == 0.00f )		
	{
		m_pCPRecoveryMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pCPRecoveryMark->SetVisibleSingle ( TRUE );
//		m_pCPRecoveryMark->CheckValueFloat( nCPRecovery );
	}

	if ( nPotionHP == 0 )		
	{
		m_pPotionHPMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pPotionHPMark->SetVisibleSingle ( TRUE );
		m_pPotionHPMark->CheckValue( nPotionHP );
	}

	if ( nPotionMP == 0 )	
	{
		m_pPotionMPMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pPotionMPMark->SetVisibleSingle ( TRUE );
		m_pPotionMPMark->CheckValue( nPotionMP );
	}

	if ( nPotionSP == 0 )	
	{
		m_pPotionSPMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pPotionSPMark->SetVisibleSingle ( TRUE );
		m_pPotionSPMark->CheckValue( nPotionSP );
	}
	//added by CNDev
	if ( nFireResist == 0 )
	{
		m_pFireMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pFireMark->SetVisibleSingle ( TRUE );
		m_pFireMark->CheckValue ( nFireResist );
	}

	if ( nIceResist == 0 )
	{
		m_pIceMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pIceMark->SetVisibleSingle ( TRUE );
		m_pIceMark->CheckValue ( nFireResist );
	}
	
	if ( nElectricResist == 0 )
	{
		m_pElectricMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pElectricMark->SetVisibleSingle ( TRUE );
		m_pElectricMark->CheckValue ( nFireResist );
	}
	
	if ( nPoisonResist == 0 )
	{
		m_pPoisonMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pPoisonMark->SetVisibleSingle ( TRUE );
		m_pPoisonMark->CheckValue ( nFireResist );
	}
	
	if ( nFireResist == 0 )
	{
		m_pSpiritMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}else{
		m_pSpiritMark->SetVisibleSingle ( TRUE );
		m_pSpiritMark->CheckValue ( nFireResist );
	}

	strCombine.Format ( "%.2f%%", nCriticalRate );
	if ( m_pCriticalRateMarkValue ) m_pCriticalRateMarkValue->SetOneLineText ( strCombine, GetTextColor( nCriticalRate ) );
	strCombine.Format ( "%.2f%%", nStrikeHit );
	if ( m_pStrikeHitMarkValue ) m_pStrikeHitMarkValue->SetOneLineText ( strCombine, GetTextColor( nStrikeHit ) );
	strCombine.Format ( "%.2f%%", nMovingSpeed );
	if ( m_pMovingSpeedMarkValue ) m_pMovingSpeedMarkValue->SetOneLineText ( strCombine, GetTextColor( nMovingSpeed ) );
	strCombine.Format ( "%.2f%%", nAttackSpeed );
	if ( m_pAttackSpeedMarkValue ) m_pAttackSpeedMarkValue->SetOneLineText ( strCombine, GetTextColor( nAttackSpeed ) );
	strCombine.Format ( "%.2f%%", nEXP );
	if ( m_pEXPMarkValue ) m_pEXPMarkValue->SetOneLineText ( strCombine, GetTextColor( nEXP ) );
	strCombine.Format ( "%2.2f%%", nHPRecovery * 100.0f );
	if ( m_pHPRecoveryMarkValue ) m_pHPRecoveryMarkValue->SetOneLineText ( strCombine, GetTextColor( nHPRecovery ) );
	strCombine.Format ( "%2.2f%%", nMPRecovery * 100.0f );
	if ( m_pMPRecoveryMarkValue ) m_pMPRecoveryMarkValue->SetOneLineText ( strCombine, GetTextColor( nMPRecovery ) );
	strCombine.Format ( "%2.2f%%", nSPRecovery  * 100.0f );
	if ( m_pSPRecoveryMarkValue ) m_pSPRecoveryMarkValue->SetOneLineText ( strCombine, GetTextColor( nSPRecovery ) );
	strCombine.Format ( "%2.2f%%", nCPRecovery );
	if ( m_pCPRecoveryMarkValue ) m_pCPRecoveryMarkValue->SetOneLineText ( strCombine, GetTextColor( nCPRecovery ) );
	strCombine.Format ( "%d", nPotionHP );
	if ( m_pPotionHPMarkValue ) m_pPotionHPMarkValue->SetOneLineText ( strCombine, GetTextColor( nPotionHP ) );
	strCombine.Format ( "%d", nPotionMP );
	if ( m_pPotionMPMarkValue ) m_pPotionMPMarkValue->SetOneLineText ( strCombine, GetTextColor( nPotionMP ) );
	strCombine.Format ( "%d", nPotionSP );
	if ( m_pPotionSPMarkValue ) m_pPotionSPMarkValue->SetOneLineText ( strCombine, GetTextColor( nPotionSP ) );
	//added by CNDev
	strCombine.Format ( "%d", nFireResist );
	if ( m_pFireMarkValue ) m_pFireMarkValue->SetOneLineText ( strCombine, GetTextColor( nFireResist ) );
	strCombine.Format ( "%d", nIceResist );
	if ( m_pIceMarkValue ) m_pIceMarkValue->SetOneLineText ( strCombine, GetTextColor( nIceResist ) );
	strCombine.Format ( "%d", nElectricResist );
	if ( m_pElectricMarkValue ) m_pElectricMarkValue->SetOneLineText ( strCombine, GetTextColor( nElectricResist ) );
	strCombine.Format ( "%d", nPoisonResist );
	if ( m_pPoisonMarkValue ) m_pPoisonMarkValue->SetOneLineText ( strCombine, GetTextColor( nPoisonResist ) );
	strCombine.Format ( "%d", nSpiritResist );
	if ( m_pSpiritMarkValue ) m_pSpiritMarkValue->SetOneLineText ( strCombine, GetTextColor( nSpiritResist ) );
}

void CCharacterWindowCharAdditionalInfo::SetPointControl ()
{
}

void CCharacterWindowCharAdditionalInfo::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case CHARACTER_STAT_HELP:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO_SPLIT(  ID2GAMEINTEXT( "CHARACTER_WINDOW_ADDITIONAL_INFO_QUESTION_BUTTON" ) , NS_UITEXTCOLOR::DARKLBUE );
			}
		}break;

	case MOUSE_OVER_STAT_2_CRITICAL:
	case MOUSE_OVER_STAT_2_STRIKE:
	case MOUSE_OVER_STAT_2_MOVINGSPEED:
	case MOUSE_OVER_STAT_2_ATTACKSPEED:
	case MOUSE_OVER_STAT_2_EXP:
	case MOUSE_OVER_STAT_2_HPRECOVERY:
	case MOUSE_OVER_STAT_2_MPRECOVERY:
	case MOUSE_OVER_STAT_2_SPRECOVERY:
	case MOUSE_OVER_STAT_2_CPRECOVERY:
	case MOUSE_OVER_STAT_2_POTIONHP:
	case MOUSE_OVER_STAT_2_POTIONMP:
	case MOUSE_OVER_STAT_3_POTIONSP:
	//added by CNDev
	case MOUSE_OVER_STAT_3_FIRE:
	case MOUSE_OVER_STAT_3_ICE:
	case MOUSE_OVER_STAT_3_ELECTRIC:
	case MOUSE_OVER_STAT_3_POISON:
	case MOUSE_OVER_STAT_3_SPIRIT:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - MOUSE_OVER_STAT_2_CRITICAL;
				ShowTip( nSelect );
			}
		}break;

	case ET_CONTROL_BUTTON:
			{
				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					CInnerInterface::GetInstance().HideGroup ( GetWndID () );
					CInnerInterface::GetInstance().HideGroup ( CHARACTER_WINDOW );
					CInnerInterface::GetInstance().HideGroup ( CHARACTER_ADDITIONAL_WINDOW );

				}
				break;
			}
	case ET_CONTROL_TITLE:
		case ET_CONTROL_TITLE_F:
			{
				if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
				{
					CInnerInterface::GetInstance().SetDefaultPosInterface( CHARACTER_WINDOW );
				}
				break;
			}	
	}
}

void CCharacterWindowCharAdditionalInfo::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();	

	InitData();
	SetData ( sCharData );
	SetUpData ( sCharData );

	SetPointControl ();

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCharacterWindowCharAdditionalInfo::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
}

void CCharacterWindowCharAdditionalInfo::ShowTip( int nSelect )
{
	if ( nSelect < 0 || nSelect >= MOUSE_OVER_BUTTONS )
		return;

	std::string strTip[MOUSE_OVER_BUTTONS] = 
	{
		//corrected by CNDev
		"CHARACTER_WINDOW_ADDITION_CRITICAL_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_STRIKE_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_MOVE_SPEED_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_ATTACKSPEED_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_EXP_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_HPRECOVERY_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_MPRECOVERY_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_SPRECOVERY_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_CPRECOVERY_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_POTIONHP_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_POTIONMP_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_POTIONSP_TOOLTIP",
		//added by CNDev
		"CHARACTER_WINDOW_ADDITION_FIRE_RESIST_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_COLD_RESIST_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_ELECTRIC_RESIST_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_POISON_RESIST_TOOLTIP",
		"CHARACTER_WINDOW_ADDITION_SPIRIT_RESIST_TOOLTIP",
	};

	CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(  ID2GAMEINTEXT( strTip[ nSelect ].c_str() ) , NS_UITEXTCOLOR::GOLD );
}

void CCharacterWindowCharAdditionalInfo::InitData()
{
	GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
	if ( pChar )
	{
		m_sCharLogic.INIT_NEW_CHAR( 
			CharClassToIndex( pChar->m_emClass ),
			pChar->GetUserID(),
			pChar->m_dwServerID,
			pChar->m_szName,
			pChar->m_wSchool,
			pChar->m_wHair,
			pChar->m_wFace,
			pChar->m_wHairColor,
			pChar->m_wSex );

		m_sCharLogic.Assign( pChar->GetCharData() );

		m_sCharLogic.SetUseArmSub( pChar->IsUseArmSub() );

		m_sCharLogic.m_bVehicle = pChar->m_bVehicle;

		SITEMCUSTOM sItemCheck = CInnerInterface::GetInstance().GetItemInfoTemp();
		
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCheck.sNativeID );
		if ( pItem && pItem->sBasicOp.emItemType == ITEM_SUIT )
		{
			EMSLOT emSlot;
			switch ( pItem->sSuitOp.emSuit )
			{
			case SUIT_HEADGEAR:		
				emSlot = SLOT_HEADGEAR;	
				break;
			case SUIT_UPPER:		
				emSlot = SLOT_UPPER;	
				break;
			case SUIT_LOWER:		
				emSlot = SLOT_LOWER;	
				break;
			case SUIT_HAND:			
				emSlot = SLOT_HAND;	
				break;
			case SUIT_FOOT:			
				emSlot = SLOT_FOOT;	
				break;
			case SUIT_HANDHELD:		
				{
					if( pChar->IsUseArmSub() )	emSlot = SLOT_RHAND_S;
					else	emSlot = SLOT_RHAND;
				}break;
			case SUIT_NECK:				
				emSlot = SLOT_NECK;	
				break;
			case SUIT_WRIST:			
				emSlot = SLOT_WRIST;	
				break;
			case SUIT_FINGER:		
				{
					if ( pChar->GetCharLogic().VALID_SLOT_ITEM( SLOT_RFINGER ) )	emSlot = SLOT_LFINGER;
					else	emSlot = SLOT_RFINGER;
				}break;
			case SUIT_VEHICLE:			
				emSlot = SLOT_VEHICLE;	
				break;

			case SUIT_EARRING:		
                emSlot = SLOT_LEARRING;	
				break;
			case SUIT_ORNAMENT:		
                emSlot = SLOT_REARRING;	
				break;
			case SUIT_ACCESSORIES:		
				{
					if ( pChar->GetCharLogic().VALID_SLOT_ITEM( SLOT_RACCESSORY ) )	emSlot = SLOT_LACCESSORY;
					else	emSlot = SLOT_RACCESSORY;
				}break;

			case SUIT_WAISTBAND:			
				emSlot = SLOT_WAIST;	
				break;

			default:				
				emSlot = SLOT_NSIZE_S_2;	
				break;
			};

			if ( pItem->sBasicOp.emItemType == ITEM_ARROW ||
				pItem->sBasicOp.emItemType == ITEM_CHARM ||
				pItem->sBasicOp.emItemType == ITEM_BULLET )	
			{
				if( pChar->IsUseArmSub() )	emSlot = SLOT_LHAND_S;
				else	emSlot = SLOT_LHAND;
			}

			if ( emSlot != SLOT_NSIZE_S_2 )
			{
				m_sCharLogic.m_PutOnItems[emSlot] = sItemCheck;
			}
		}

		for ( int i=0; i<SKILLFACT_SIZE; ++i )
		{
			m_sCharLogic.m_sSKILLFACT[i] = pChar->m_sSKILLFACT[i];
		}

		m_sCharLogic.m_sPETSKILLFACT = pChar->m_sPETSKILLFACT;

		m_sCharLogic.m_sQITEMFACT = pChar->m_sQITEMFACT;
		m_sCharLogic.m_sEVENTFACT = pChar->m_sEVENTFACT;

		m_sCharLogic.INIT_DATA( TRUE, FALSE );
		m_sCharLogic.UPDATE_DATA( 0.0f , 0.0f );
	}
}

DWORD CCharacterWindowCharAdditionalInfo::GetTextColor( float nVal )
{
	if ( nVal == 0 )	return NS_UITEXTCOLOR::WHITE;
	if ( nVal > 0 )		return NS_UITEXTCOLOR::BRIGHTGREEN;
	if ( nVal < 0 )		return NS_UITEXTCOLOR::RED;

	return NS_UITEXTCOLOR::WHITE;
};