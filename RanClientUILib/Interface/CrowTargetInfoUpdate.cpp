#include "StdAfx.h"
#include "CrowTargetInfo.h"
#include "ClubWindow.h"

#include "InnerInterface.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../EngineUILib/GUInterface/BasicProgressBar.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "GLogicData.h"
#include "ItemSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CCrowTargetInfo::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

//	m_pAcademy_Department->SetOneLineText ( "CNDev", NS_UITEXTCOLOR::SILVER );

}