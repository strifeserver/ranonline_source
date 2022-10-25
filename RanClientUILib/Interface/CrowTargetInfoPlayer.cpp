#include "StdAfx.h"
#include "CrowTargetInfoPlayer.h"
#include "InnerInterface.h"
#include "BasicLineBox.h"
#include "../EngineLib/DxCommon/d3dfont.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "BasicLineBox.h"
#include "BasicLineBoxEx.h"
#include "../Interface/GameTextControl.h"
#include "../Interface/UITextControl.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"

#include "../RanClientLib/G-Logic/GLGaeaClient.h"
#include "../RanClientLib/G-Logic/GLCrowData.h"
#include "../RanClientLib/G-Logic/GLCrow.h"
#include "../RanClientLib/G-Logic/GLCrowClient.h"
#include "../RanClientLib/G-Logic/GlogicEx.h"

#include "GLCharData.h"
#include "GLCharacter.h"
#include "GLChar.h"
#include "GLCharClient.h"
#include "GLogicData.h"
#include "GLCharDefine.h"
#include "UIInfoLoader.h"
#include "GLCrowRenList.h"
#include "GLStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCrowTargetInfoPlayer::CCrowTargetInfoPlayer () :
	m_pNameBoxDummy ( NULL )
{
}

CCrowTargetInfoPlayer::~CCrowTargetInfoPlayer ()
{
}

void CCrowTargetInfoPlayer::CreateSubControl ()
{
	m_pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	m_pTargetBox = new CBasicLineBoxEx;
	m_pTargetBox->CreateSub ( this, "BASIC_LINE_BOX_EX_VARTEXT_GRAY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTargetBox->CreateBaseBoxDialogueDarkGray ( "RNCROW_TARGET_INFOBOX_PLAYER" );
	m_pTargetBox->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pTargetBox );

	CBasicTextBox* m_pSchool = new CBasicTextBox;
	m_pSchool->CreateSub ( this, "SCHOOL_NAMEBOX_PLAYER" );
    m_pSchool->SetFont ( m_pFont8 );
	m_pSchool->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pSchool->SetControlNameEx ( "School Text" );	
	m_pSchool->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pSchool );
	m_pSchoolBox = m_pSchool;
	
	CBasicTextBox* m_pClass = new CBasicTextBox;
	m_pClass->CreateSub ( this, "CLASS_NAMEBOX_PLAYER" );
    m_pClass->SetFont ( m_pFont8 );
	m_pClass->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pClass->SetControlNameEx ( "Class Text" );	
	m_pClass->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pClass );
	m_pClassBox = m_pClass;

	CBasicTextBox* m_pReborn = new CBasicTextBox;
	m_pReborn->CreateSub ( this, "REBORN_NAMEBOX_PLAYER" );
    m_pReborn->SetFont ( m_pFont8 );
	m_pReborn->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pReborn->SetControlNameEx ( "Reborn Text" );	
	m_pReborn->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pReborn );
	m_pRebornBox  = m_pReborn ;

	CBasicTextBox* m_pGuild = new CBasicTextBox;
	m_pGuild->CreateSub ( this, "GUILD_NAMEBOX_PLAYER" );
    m_pGuild->SetFont ( m_pFont8 );
	m_pGuild->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pGuild->SetControlNameEx ( "Guild Text" );	
	m_pGuild->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pGuild );
	m_pGuildBox  = m_pGuild ;

	/*CBasicTextBox* m_pParty = new CBasicTextBox;
	m_pParty->CreateSub ( this, "Party_NAMEBOX_PLAYER" );
    m_pParty->SetFont ( m_pFont8 );
	m_pParty->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pParty->SetControlNameEx ( "Party Text" );	
	m_pParty->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pParty );
	m_pPartyBox  = m_pParty ;*/
	
	CBasicTextBox* m_pText = new CBasicTextBox;
	m_pText->CreateSub ( this, "TEXT_NAMEBOX_PLAYER" );
    m_pText->SetFont ( m_pFont8 );
	m_pText->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pText->SetControlNameEx ( "Text" );	
	m_pText->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pText );
	m_pTextBox = m_pText;

	CBasicTextBox* pNameBox = new CBasicTextBox;
	pNameBox->CreateSub ( this, "CROW_NAMEBOX_PLAYER" );
    pNameBox->SetFont ( m_pFont8 );
	pNameBox->SetTextAlign ( TEXT_ALIGN_LEFT );
	pNameBox->SetControlNameEx ( "Name Text" );	
	pNameBox->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( pNameBox );
	m_pNameBox = pNameBox;
	
	m_pNameBoxDummy = CreateControl ( "CROW_NAMEBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
}

CUIControl*	CCrowTargetInfoPlayer::CreateControl ( const char* szControl, WORD wAlignFlag )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, wAlignFlag );
	RegisterControl ( pControl );
	return pControl;
}

void CCrowTargetInfoPlayer::SetTargetInfo ( int fNow, int fMax, int fPercent3,int fSchool, int strReborn, CString strClass, int strLevel, CString strPartyMember,CString strPartyM, CString strPartyNew,CString strPartyJoined,CString strParty, CString strGuild, CString strName )
{	
		CString strSchoolCombine;
		strSchoolCombine.Format ("%s", ID2GAMEWORD("ACADEMY_NAME",fSchool));
		m_pSchoolBox->SetOneLineText ( strSchoolCombine, NS_UITEXTCOLOR::WHITE );
		
		CString strClassCombine;
		strClassCombine.Format ("%s", strClass );
		m_pClassBox->SetOneLineText ( strClassCombine, NS_UITEXTCOLOR::DARKGRAY );
		CString strRebornCombine;

		strRebornCombine.Format ("HP: (%d" ,fPercent3 );
		strRebornCombine += "%)";
		m_pRebornBox->SetOneLineText ( strRebornCombine, NS_UITEXTCOLOR::RED );

		if(strGuild != ""){
		CString strGuildCombine;
		strGuildCombine.Format ("%s", strGuild);
		m_pGuildBox->SetOneLineText ( strGuildCombine, NS_UITEXTCOLOR::DARKGRAY );
		}else{
		CString strGuildCombine;
		strGuildCombine.Format ("There is no club joined." );
		m_pGuildBox->SetOneLineText ( strGuildCombine, NS_UITEXTCOLOR::DARKGRAY );
		}

		if(strParty != ""){ //New Target Info for Party by CNDev
		CString strPartyCombine;
		strPartyCombine.Format ("%s (%s)", strParty, strPartyNew );
		m_pTextBox->SetOneLineText ( strPartyCombine, NS_UITEXTCOLOR::JERMTHESLAYER );
		}else{
		CString strPartyCombine;
		strPartyCombine.Format ("There is no party joined");
		m_pTextBox->SetOneLineText ( strPartyCombine, NS_UITEXTCOLOR::JERMTHESLAYER );
		}
		if(strPartyNew != ""){ //New Target Info for Party by CNDev
		CString strPartyCombine;
		strPartyCombine.Format ("%s (%s)", strParty, strPartyNew );
		m_pTextBox->SetOneLineText ( strPartyCombine, NS_UITEXTCOLOR::JERMTHESLAYER );
		}else{
		CString strPartyCombine;
		strPartyCombine.Format ("There is no party joined.");
		m_pTextBox->SetOneLineText ( strPartyCombine, NS_UITEXTCOLOR::JERMTHESLAYER );
		}

		m_pRebornBox->SetOneLineText ( "Shift-Click to open the window of shortcut keys", NS_UITEXTCOLOR::GOLD );
		
		m_pNameBox->ClearText ();

		SIZE Size;
		m_pFont8->GetTextExtent ( strName.GetString (), Size );
		
		CString strTemp;
		strTemp.Format ( "Lv.%d %s", strLevel, strName );
		m_pNameBox->SetOneLineText ( strTemp, NS_UITEXTCOLOR::PALEGREEN );	
		m_strName = strName;
}
