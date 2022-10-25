#include "StdAfx.h"
#include "CharacterWindow.h"

#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../EngineUILib/GUInterface/BasicProgressBar.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "GLogicData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	레벨
void CCharacterWindow::SetLevel ( const GLCHARLOGIC& sCharData )
{
	const WORD& wLevel = sCharData.m_wLevel;

    CString strCombine;
	strCombine.Format ( "%d", wLevel );
	if ( m_pLevelText )
	{
		m_pLevelText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::GOLD );
	}
}

void CCharacterWindow::SetName ( const GLCHARLOGIC& sCharData )
{
	CString strCombine1;
	strCombine1.Format ( "[Char ID:%d][Char Name:%s]", sCharData.m_dwCharID,sCharData.m_szName );

	if ( m_pNameText )
	{
		m_pNameText->SetOneLineText ( strCombine1, NS_UITEXTCOLOR::ORANGE );
	}
}

//	경험치
void CCharacterWindow::SetEXP( const GLCHARLOGIC& sCharData )
{
	const LONGLONG& lNOW = sCharData.m_sExperience.lnNow;
	const LONGLONG& lMAX = sCharData.m_sExperience.lnMax;

	int nIndex = 0;
	
	if ( m_pEXPText ) m_pEXPText->ClearText ();

	CString strCombine;
	strCombine.Format ( "%I64d", lNOW );
	if ( m_pEXPText ) nIndex = m_pEXPText->AddTextNoSplit ( strCombine, NS_UITEXTCOLOR::WHITE );

	strCombine.Format ( " / " );
	if ( m_pEXPText ) m_pEXPText->AddString ( nIndex, strCombine, NS_UITEXTCOLOR::LIGHTGREY );

	strCombine.Format ( "%I64d", lMAX );
	if ( m_pEXPText ) m_pEXPText->AddString ( nIndex, strCombine, NS_UITEXTCOLOR::WHITE );

	const float fPercent = float(lNOW) / float(lMAX);
	if ( m_pEXP ) m_pEXP->SetPercent ( fPercent );
}

//	Stat	
void CCharacterWindow::SetPow ( const GLCHARLOGIC& sCharData )
{
	m_pPowText->ClearText ();

	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwPow );
	int nIndex = m_pPowText->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );

	SCHARSTATS sADD = sCharData.GETSTATS_ADD();
	SCHARSTATS sITEM = sCharData.GETSTATS_ITEM();

	if ( !sADD.dwPow ) return ;
	
	strCombine.Format ( "(+%d)", sADD.dwPow );

	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;
	if ( sITEM.dwPow ) dwColor = NS_UITEXTCOLOR::PLUSOPTION;

	m_pPowText->AddString ( nIndex, strCombine, dwColor );	
}

void CCharacterWindow::SetDex ( const GLCHARLOGIC& sCharData )
{
	m_pDexText->ClearText ();

	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwDex );
	int nIndex = m_pDexText->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );

	SCHARSTATS sADD = sCharData.GETSTATS_ADD();
	SCHARSTATS sITEM = sCharData.GETSTATS_ITEM();

	if ( !sADD.dwDex ) return ;

	strCombine.Format ( "(+%d)", sADD.dwDex );

	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;
	if ( sITEM.dwDex ) dwColor = NS_UITEXTCOLOR::PLUSOPTION;

	m_pDexText->AddString ( nIndex, strCombine, dwColor );
}

void CCharacterWindow::SetSpi ( const GLCHARLOGIC& sCharData )
{
	m_pSpiText->ClearText ();

	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwSpi );
	int nIndex = m_pSpiText->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );

	SCHARSTATS sADD = sCharData.GETSTATS_ADD();
	SCHARSTATS sITEM = sCharData.GETSTATS_ITEM();

	if ( !sADD.dwSpi ) return ;

	strCombine.Format ( "(+%d)", sADD.dwSpi );

	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;
	if ( sITEM.dwSpi ) dwColor = NS_UITEXTCOLOR::PLUSOPTION;

	m_pSpiText->AddString ( nIndex, strCombine, dwColor );
}

void CCharacterWindow::SetInt ( const GLCHARLOGIC& sCharData )
{
	m_pIntText->ClearText ();

	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwInt );
	int nIndex = m_pIntText->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );

	SCHARSTATS sADD = sCharData.GETSTATS_ADD();
	SCHARSTATS sITEM = sCharData.GETSTATS_ITEM();

	if ( !sADD.dwInt ) return ;

	strCombine.Format ( "(+%d)", sADD.dwInt );

	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;
	if ( sITEM.dwInt ) dwColor = NS_UITEXTCOLOR::PLUSOPTION;

	m_pIntText->AddString ( nIndex, strCombine, dwColor );
}

void CCharacterWindow::SetStr ( const GLCHARLOGIC& sCharData )
{
	m_pStrText->ClearText ();

	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwStr );
	int nIndex = m_pStrText->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );

	SCHARSTATS sADD = sCharData.GETSTATS_ADD();
	SCHARSTATS sITEM = sCharData.GETSTATS_ITEM();

	if ( !sADD.dwStr ) return ;

	strCombine.Format ( "(+%d)", sADD.dwStr );

	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;
	if ( sITEM.dwStr ) dwColor = NS_UITEXTCOLOR::PLUSOPTION;

	m_pStrText->AddString ( nIndex, strCombine, dwColor );
}

void CCharacterWindow::SetSta ( const GLCHARLOGIC& sCharData )
{
	m_pStaText->ClearText ();

	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwSta );
	int nIndex = m_pStaText->AddText ( strCombine, NS_UITEXTCOLOR::WHITE );

	SCHARSTATS sADD = sCharData.GETSTATS_ADD();
	SCHARSTATS sITEM = sCharData.GETSTATS_ITEM();

	if ( !sADD.dwSta ) return ;

	strCombine.Format ( "(+%d)", sADD.dwSta );

	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;
	if ( sITEM.dwSta ) dwColor = NS_UITEXTCOLOR::PLUSOPTION;

	m_pStaText->AddString ( nIndex, strCombine, dwColor );
}

void CCharacterWindow::SetPointControl ( BOOL bPointUsable )
{
	//	Point
	if ( m_pPointDisable ) m_pPointDisable->SetVisibleSingle ( !bPointUsable );
	if ( m_pPointEnable ) m_pPointEnable->SetVisibleSingle ( bPointUsable );

	//	Stats Button
	if ( m_pPowButton ) m_pPowButton->SetVisibleSingle ( bPointUsable );
	if ( m_pDexButton ) m_pDexButton->SetVisibleSingle ( bPointUsable );
	if ( m_pSpiButton ) m_pSpiButton->SetVisibleSingle ( bPointUsable );

	if ( m_pStrButton ) m_pStrButton->SetVisibleSingle ( bPointUsable );
	if ( m_pStaButton ) m_pStaButton->SetVisibleSingle ( bPointUsable );

	if ( m_pPowDiaableButton ) m_pPowDiaableButton->SetVisibleSingle ( !bPointUsable );
	if ( m_pDexDiaableButton ) m_pDexDiaableButton->SetVisibleSingle ( !bPointUsable );
	if ( m_pSpiDiaableButton ) m_pSpiDiaableButton->SetVisibleSingle ( !bPointUsable );

	if ( m_pStrDiaableButton ) m_pStrDiaableButton->SetVisibleSingle ( !bPointUsable );
	if ( m_pStaDiaableButton ) m_pStaDiaableButton->SetVisibleSingle ( !bPointUsable );
}

void CCharacterWindow::SetPoint ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_wStatsPoint );
	if ( m_pPointText ) m_pPointText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::GOLD );
}

//	Status
void CCharacterWindow::SetDamage ( const GLCHARLOGIC& sCharData )
{	
	const int& nLow = sCharData.GETFORCE_LOW ();
	const int& nHigh = sCharData.GETFORCE_HIGH ();

	CString strCombine;
	strCombine.Format ( "%d~%d", nLow, nHigh );
	if ( m_pDamageText ) m_pDamageText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetDefense ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.GETDEFENSE () );
	if ( m_pDefenseText ) m_pDefenseText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetReqPa ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_wSUM_PA );
	if ( m_pReqPaText ) m_pReqPaText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetReqSa ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_wSUM_SA );
	if ( m_pReqSaText ) m_pReqSaText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetReqMa ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_wSUM_MA );
	if ( m_pReqMaText ) m_pReqMaText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetHitRate ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.GETHIT () );
	if ( m_pHitRateText ) m_pHitRateText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetHP ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d/%d", sCharData.GETHP (), sCharData.GETMAXHP () );
	if ( m_pHPText ) m_pHPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetMP ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d/%d", sCharData.m_sMP.dwNow, sCharData.m_sMP.dwHigh );
	if ( m_pMPText ) m_pMPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetSP ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d/%d", sCharData.m_sSP.dwNow, sCharData.m_sSP.dwHigh );
	if ( m_pSPText ) m_pSPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetCP ( const GLCHARLOGIC& sCharData ) //add cp
{
	CString strCombine;
	strCombine.Format ( "%d/%d", sCharData.m_sCP.dwNow, sCharData.m_sCP.dwHigh );
	if ( m_pCPText ) m_pCPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetAvoidRate ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.GETAVOID () );
	if ( m_pAvoidRateText ) m_pAvoidRateText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

//	저항력
void CCharacterWindow::SetFireResi ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nFire );
	if ( m_pFireText ) m_pFireText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetColdResi ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nIce );
	if ( m_pColdText ) m_pColdText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetElectricResi ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nElectric );
	if ( m_pElectricText ) m_pElectricText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetPoisonResi ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nPoison );
	if ( m_pPoisonText ) m_pPoisonText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

void CCharacterWindow::SetSpiritResi ( const GLCHARLOGIC& sCharData )
{
	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nSpirit );
	if ( m_pSpiritText ) m_pSpiritText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
}

//	사회성향
//void CCharacterWindow::SetPK ( const GLCHARLOGIC& sCharData )
//{
//	CString strCombine;
//	strCombine.Format ( "%d", sCharData.m_wPK );
//	if ( m_pPKText ) m_pPKText->SetOneLineText ( strCombine );
//}

void CCharacterWindow::SetBright ( const GLCHARLOGIC& sCharData )
{
	const int nMIN = -100;
	const int nMAX = 100;
	const int nRANGE = nMAX - nMIN;

	int nPercent = sCharData.GETBRIGHTPER ();
	nPercent = -(nPercent);	//	이미지가 반대로 뒤집혀있어서.
	if ( nPercent < nMIN ) nPercent = nMIN;
	if ( nPercent > nMAX ) nPercent = nMAX;

	nPercent += nMAX;

	float fPercent = static_cast<float>(nPercent) / static_cast<float>(nRANGE);

	const UIRECT& rcFramePos = m_pBrightFrame->GetGlobalPos ();
	const UIRECT& rcThumbPos = m_pBrightSlider->GetGlobalPos ();

	float fMOVE = (rcFramePos.sizeX - rcThumbPos.sizeX) * fPercent;
	float fLeft = rcFramePos.left + fMOVE;

	m_pBrightSlider->SetGlobalPos ( D3DXVECTOR2 ( fLeft, rcThumbPos.top ) );
}

void CCharacterWindow::SetPKState ( const GLCHARLOGIC& sCharData )
{
	if ( m_pPKStateText )
	{		
		m_pPKStateText->SetOneLineText ( GLGaeaClient::GetInstance().GetCharacter()->GET_PK_NAME ().c_str(), NS_UITEXTCOLOR::WHITE );
	}
}

void CCharacterWindow::SetGrade ( const GLCHARLOGIC& sCharData )
{
	if ( m_pGradeText )
	{
//		m_pGradeText->SetOneLineText ( ID2GAMEWORD("ACADEMY_NAME",sCharData.m_wSchool) );
	}
}

void CCharacterWindow::SetActivityPoint ( const GLCHARLOGIC& sCharData )
{
	if ( m_pActivityPoint )
	{
		CString strCombine;
		strCombine.Format ( "%d", sCharData.m_nLiving );
		m_pActivityPoint->SetOneLineText ( strCombine, NS_UITEXTCOLOR::WHITE );
	}
}

void CCharacterWindow::SetAcademy ( const GLCHARLOGIC& sCharData )
{
	if ( m_pAcademy )
	{
		m_pAcademy->SetOneLineText ( ID2GAMEWORD("ACADEMY_NAME",sCharData.m_wSchool), NS_UITEXTCOLOR::WHITE );

		for ( int i = 0; i < nSCHOOL_MARK; ++i ) m_pSchoolMark[sCharData.m_wSchool]->SetVisibleSingle ( FALSE );
		m_pSchoolMark[sCharData.m_wSchool]->SetVisibleSingle ( TRUE );
	}
}

void CCharacterWindow::SetDepartment ( const GLCHARLOGIC& sCharData )
{	
	if ( m_pDepartment )
	{
		m_pDepartment->SetOneLineText ( COMMENT::CHARCLASS[CharClassToIndex(sCharData.m_emClass)].c_str(), NS_UITEXTCOLOR::WHITE );
	}
}

void CCharacterWindow::SetPremium ()
{
	if ( m_pPremiumText )
	{
		m_pPremiumTextStatic->SetVisibleSingle ( FALSE );

		bool bPREMIUM = GLGaeaClient::GetInstance().GetCharacter()->m_bPREMIUM;
		if ( bPREMIUM )
		{
			__time64_t tPREMIUM = GLGaeaClient::GetInstance().GetCharacter()->m_tPREMIUM;
			CTime cTime ( tPREMIUM );
			CString strExpireDate;
			strExpireDate.Format ( "%02d/%02d/%02d", cTime.GetYear()%2000, cTime.GetMonth(), cTime.GetDay () );
			m_pPremiumText->SetOneLineText ( strExpireDate, NS_UITEXTCOLOR::DARKORANGE );
			m_pPremiumTextStatic->SetVisibleSingle ( TRUE );
		}
	}
}

void CCharacterWindow::SetPhoneNumber( const CString & strPhoneNumber )
{
	if( m_pMobileText )
	{
		m_pMobileText->SetOneLineText( strPhoneNumber, NS_UITEXTCOLOR::SILVER );
	}
}
void CCharacterWindow::SetPkRank( const GLCHARLOGIC& sCharData )
{
	const DWORD& dwPkWin = sCharData.m_dwPkWin;

	if( m_pPKRText )
	{
		if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[19] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[19].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[19]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[18] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[18].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[18]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[17] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[17].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[17]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[16] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[16].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[16]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[15] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[15].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[15]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[14] ) 
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[14].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[14]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[13] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[13].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[13]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[12] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[12].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[12]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[11] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[11].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[11]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[10] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[10].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[10]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[9] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[9].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[9]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[8] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[8].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[8]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[7] ) 
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[7].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[7]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[6] ) 
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[6].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[6]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[5] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[5].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[5]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[4] ) 
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[4].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[4]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[3] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[3].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[3]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[2] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[2].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[2]->SetVisibleSingle ( TRUE );
		}
		else if  ( dwPkWin > GLCONST_CHAR::dwRank_Point[1] )
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[1].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[1]->SetVisibleSingle ( TRUE );
		}
		else
		{
			m_pPKRText->SetOneLineText( GLCONST_CHAR::strRank_Name[0].c_str(), NS_UITEXTCOLOR::GOLD );
			m_pRankMark[0]->SetVisibleSingle ( TRUE );
		}

		
	}
}
void CCharacterWindow::SetRB( const GLCHARLOGIC& sCharData )
{
	const DWORD& dwReborn = sCharData.m_dwReborn;

    CString strCombine;
	strCombine.Format ( "%d", dwReborn );
	if ( m_pRBText )
	{
		m_pRBText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}
}
void CCharacterWindow::SetPKW( const GLCHARLOGIC& sCharData )
{
	const DWORD& dwPkWin = sCharData.m_dwPkWin;

    CString strCombine;
	strCombine.Format ( "%d", dwPkWin );
	if ( m_pPKWText )
	{
		m_pPKWText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}
}
void CCharacterWindow::SetPKL( const GLCHARLOGIC& sCharData )
{
	const DWORD& dwPkLoss = sCharData.m_dwPkLoss;

    CString strCombine;
	strCombine.Format ( "%d", dwPkLoss );
	if ( m_pPKLText )
	{
		m_pPKLText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}
}

