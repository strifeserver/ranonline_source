#include "StdAfx.h"
#include "CrowTargetInfo.h"
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

CCrowTargetInfo::CCrowTargetInfo () :
	m_pNameBoxDummy ( NULL )
{
}

CCrowTargetInfo::~CCrowTargetInfo ()
{
}

void CCrowTargetInfo::CreateSubControl ()
{
	m_pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	m_pTargetBox = new CBasicLineBoxEx;
	m_pTargetBox->CreateSub ( this, "BASIC_LINE_BOX_EX_VARTEXT_GRAY", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTargetBox->CreateBaseBoxDialogueDarkGray ( "RNCROW_TARGET_INFOBOX" );
	m_pTargetBox->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pTargetBox );

	CBasicTextBox* m_pHP = new CBasicTextBox;
	m_pHP->CreateSub ( this, "HP_NAMEBOX" );
    m_pHP->SetFont ( m_pFont8 );
	m_pHP->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pHP->SetControlNameEx ( "HP Text" );	
	m_pHP->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pHP );
	m_pHPBox = m_pHP;

	CBasicTextBox* pNameBox = new CBasicTextBox;
	pNameBox->CreateSub ( this, "CROW_NAMEBOX" );
    pNameBox->SetFont ( m_pFont8 );
	pNameBox->SetTextAlign ( TEXT_ALIGN_LEFT );
	pNameBox->SetControlNameEx ( "HP Text" );	
	pNameBox->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( pNameBox );
	m_pNameBox = pNameBox;
	
	m_pNameBoxDummy = CreateControl ( "CROW_NAMEBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
}

CUIControl*	CCrowTargetInfo::CreateControl ( const char* szControl, WORD wAlignFlag )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, wAlignFlag );
	RegisterControl ( pControl );
	return pControl;
}

void CCrowTargetInfo::SetTargetInfo ( int fNow, int fMax, int fPercent,DWORD Level,  CString strName )
{	
		CString strCombine;
		CString combine;
		strCombine.Format ( "%d/%d (%d", fNow,fMax,fPercent );
		strCombine += "%)";
		m_pHPBox->SetOneLineText ( strCombine, NS_UITEXTCOLOR::GOLD );
		m_pNameBox->ClearText ();

		SIZE Size;
		m_pFont8->GetTextExtent ( strName.GetString (), Size );
		
		CString strTemp;
		strTemp.Format ( "Lv.%d %s",Level , strName );
		m_pNameBox->SetOneLineText ( strTemp, NS_UITEXTCOLOR::WHITE );	
		m_strName = strName;
}
