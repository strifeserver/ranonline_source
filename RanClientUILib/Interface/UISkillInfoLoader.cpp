#include "StdAfx.h"
#include "UIInfoLoader.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NS_SKILLINFO
{
	SNATIVEID	m_sNativeIDBack;
	BOOL		m_bNextLevel_BEFORE_FRAME = FALSE;
	
	void AddItemRender ( SNATIVEID sICONINDEX, const char* szTexture ) 
	{ 
		CInnerInterface::GetInstance().ADDITEM_RENDER ( sICONINDEX, szTexture ); 
	} 

	void ResetItemRender () 
	{ 
		CInnerInterface::GetInstance().RESETITEM_RENDER ();     
	} 

	void RESET ()
	{
		m_sNativeIDBack = NATIVEID_NULL();
		m_bNextLevel_BEFORE_FRAME = FALSE;
		ResetItemRender ();
//		ResetResiIcon();
	}

	void AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_NOSPLIT ( strText, dwColor );
	}

	void AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_NOSPLIT_BIG ( strText, dwColor );
	}
	     
	void AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_LONGESTLINE_SPLIT ( strText, dwColor );
	}

	void AddTextMultiColorSplit ( CString strText , D3DCOLOR dwColor, CString strText2 , D3DCOLOR dwColor2 )
	{
		CInnerInterface::GetInstance().ADDTEXT_MULTICOLORSPLIT ( strText, dwColor ,  strText2 , dwColor2 );
	}
	void AddTextMultiColorSplit4 ( CString strText , D3DCOLOR dwColor , CString strText2 , D3DCOLOR dwColor2 ,CString strText3 , D3DCOLOR dwColor3 ,CString strText4 , D3DCOLOR dwColor4 )
	{
		CInnerInterface::GetInstance().ADDTEXT_MULTICOLORSPLIT4 ( strText, dwColor , strText2 , dwColor2 , strText3, dwColor3 ,strText4 , dwColor4 );
	}
	void AddTextMultiColorSplit2 ( CString strText , D3DCOLOR dwColor , CString strText2 , D3DCOLOR dwColor2 ,CString strText3 , D3DCOLOR dwColor3 ,CString strText4 , D3DCOLOR dwColor4 )
	{
		CInnerInterface::GetInstance().ADDTEXT_MULTICOLORSPLIT2 ( strText, dwColor , strText2 , dwColor2 , strText3, dwColor3 ,strText4 , dwColor4 );
	}
	void AddTextMultiColorSplit3 ( CString strText , D3DCOLOR dwColor , CString strText2 , D3DCOLOR dwColor2 ,CString strText3 , D3DCOLOR dwColor3 ,CString strText4 , D3DCOLOR dwColor4 , CString strText5 , D3DCOLOR dwColor5 )
	{
		CInnerInterface::GetInstance().ADDTEXT_MULTICOLORSPLIT3 ( strText, dwColor , strText2 , dwColor2 , strText3, dwColor3 ,strText4 , dwColor4 , strText5 , dwColor5 );
	}

	void RemoveAllInfo ()
	{
		CInnerInterface::GetInstance().CLEAR_TEXT ();
	}

	void LOAD ( GLSKILL* const pSkill, SCHARSKILL* const pCharSkill, BOOL bNextLevel )
	{
		CString	strText;

		//	스킬 정보 입력

		WORD wDispLevel = 0;			
		if ( pCharSkill )	//	배운 스킬
		{
			wDispLevel = pCharSkill->wLevel + 1;				
		}
		
		if ( !pSkill ) 
        {     
            return ; 
            ResetItemRender (); 
//			ResetResiIcon ();
        } 

        AddItemRender ( pSkill->m_sEXT_DATA.sICONINDEX, pSkill->m_sEXT_DATA.strICONFILE.c_str() ); 
		
		BOOL bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL ( pSkill->m_sBASIC.sNATIVEID );
		if ( bNextLevel )
		{
			if ( bMASTER )
			{
				//	NOTE
				//		만약, 마스터라면
				//		다음 레벨을 뿌리는것이 아니라,
				//		현재 레벨의 값들을 뿌리도록 한다.
				bNextLevel = FALSE;
			}
			else
			{
				if ( wDispLevel )
				{				
					strText.Format ("  %s:%d", ID2GAMEWORD("SKILL_NEXTLEVEL"), wDispLevel + 1 );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::RED);
				}
			}
		}

		//strText = ID2GAMEWORD("SKILL_CATEGORY", 0);
		//AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);

		//	기본 정보
		{
			//	1. Skill Name			
			strText.Format("        %s", pSkill->GetName());
			if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
			{
				CString strDescText;
				strDescText.Format( "(%s)",COMMENT::SKILL_ROLE[SKILL::EMROLE_PASSIVE].c_str() );
				strText += strDescText;
			}
			AddTextNoSplitBig(strText,NS_UITEXTCOLOR::DEFAULT);
			AddTextNoSplit("",NS_UITEXTCOLOR::DEFAULT);

			//	2. Skill Level
			strText.Format("          Level %d ", pSkill->m_sBASIC.dwGRADE);
			AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);		
			
			//	3.(Neutral)
			
			//strText.Format("%s:%s",ID2GAMEWORD("SKILL_BASIC_INFO", 2), COMMENT::BRIGHT[pSkill->m_sLEARN.emBRIGHT].c_str());
			//AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
			//strText.Format(" ");
			//AddTextNoSplit(" ", NS_UITEXTCOLOR::DEFAULT);

			//	4. Normal / Extreme / Missile / Melee / Magic
			


			CString szSKillFlag;
			CString	strText;
			
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_FIGHTER_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_FIGHTER_W ))	strText.AppendFormat ( "Brawler/");
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_ARMS_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_ARMS_W ))	strText.Format ( "Swordsman/");
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_W ))	strText.Format ( "Archer/");
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_SPIRIT_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_SPIRIT_W ))	strText.Format ( "Shaman/");
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_W ))	strText.Format ( "Extreme/");
			//add class
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_SCIENCE_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_SCIENCE_W ))	strText.Format ( "Gunner/");
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASIN_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASIN_W ))	strText.Format ( "Assassin/");
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_TESTING_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_TESTING_W ))	strText.Format ( "Magician/");
			if (( pSkill->m_sLEARN.dwCLASS & GLCC_SHAPER_M )&&
			 ( pSkill->m_sLEARN.dwCLASS & GLCC_SHAPER_W ))	strText.Format ( "Shaper/");
			
			if ( pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_SHORT )
			{
				szSKillFlag+=("Melee");
				strText.AppendFormat( "%s", szSKillFlag.GetString() );
			}
			if ( pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_PHY_LONG )
			{
				szSKillFlag+=("Missile");
				strText.AppendFormat( "%s", szSKillFlag.GetString() );
			}
			if ( pSkill->m_sBASIC.emAPPLY == SKILL::EMAPPLY_MAGIC )
			{
				szSKillFlag+=("Magic");
				strText.AppendFormat( "%s", szSKillFlag.GetString() );
			}

			
			AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
			
			
			
		}
		{ //Applicable Weapon
			
			const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;		

			if ( (sBASIC.emUSE_LITEM && (sBASIC.emUSE_LITEM != ITEMATT_NOCARE)) || 
				(sBASIC.emUSE_RITEM && (sBASIC.emUSE_RITEM != ITEMATT_NOCARE)) )
			{
				//AddTextNoSplit("Applicable Weapon :",NS_UITEXTCOLOR::LIGHTSKYBLUE);
			}

			if ( sBASIC.emUSE_LITEM )
			{
				if ( sBASIC.emUSE_LITEM != ITEMATT_NOCARE )
				{
					strText.Format ( "Applicable Weapon :%s",ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_LITEM - 1 ) );

					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}
			}

			if ( sBASIC.emUSE_RITEM )
			{
				if ( sBASIC.emUSE_RITEM != ITEMATT_NOCARE )
				{
					strText.Format ( "Applicable Weapon :%s",ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_RITEM - 1 ) );

					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}
			}
		}
		{
			//Consumption Req.
			
			const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;
		
			if ( pCharSkill )	//	배운 스킬일 경우에...
			{
				WORD wLevel = pCharSkill->wLevel;
				if ( bNextLevel ) wLevel = pCharSkill->wLevel + 1;
				
				const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
				
				if ( pSkill->m_sBASIC.emROLE != SKILL::EMROLE_PASSIVE )
				{
				//AddTextNoSplit("Consumption Requirements :",NS_UITEXTCOLOR::DEFAULT);
				strText.Format("Consumption Requirements: ");
				
				//	Arrow Consumed
				if ( 0 < sDATA_LVL.wUSE_ARROWNUM )
				{
					
					strText.AppendFormat("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 0), sDATA_LVL.wUSE_ARROWNUM);
					
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				//	Talisman Consumed
				if ( 0 < sDATA_LVL.wUSE_CHARMNUM )
				{
					strText.AppendFormat("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 1), sDATA_LVL.wUSE_CHARMNUM);
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				//	EXP Consumed
				//if ( 0 < sDATA_LVL.wUSE_EXP )
				//{
				//	strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 2), sDATA_LVL.wUSE_EXP);
				//	AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				//}

				//	HP Consumed
				if ( 0 < sDATA_LVL.wUSE_HP )
				{
					strText.AppendFormat("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 3), sDATA_LVL.wUSE_HP);
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				/*//	MP Consumed
				if ( 0 < sDATA_LVL.wUSE_MP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 4), sDATA_LVL.wUSE_MP);
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				//	SP Consumed
				if ( 0 < sDATA_LVL.wUSE_SP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 5), sDATA_LVL.wUSE_SP);
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}*/
				
				// CP Consumed
				if ( 0 < sDATA_LVL.wUSE_CP )
				{
					strText.AppendFormat("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 9), sDATA_LVL.wUSE_CP);
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}
				
				// Bullet Consumed
				if ( 0 < sDATA_LVL.wUSE_BULLET )
				{
					strText.AppendFormat("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 10), sDATA_LVL.wUSE_BULLET);
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}
				
				// Extra Consumed
				if ( 0 < sDATA_LVL.wUSE_EXTRA )
				{
					strText.AppendFormat("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 11), sDATA_LVL.wUSE_EXTRA);
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}
				
				if(( 0 < sDATA_LVL.wUSE_MP ) && ( 0 < sDATA_LVL.wUSE_SP ))
				{
					strText.AppendFormat ("%s %d %s %d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 4), sDATA_LVL.wUSE_MP
													   ,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 5), sDATA_LVL.wUSE_SP );
				
				}
				
					/*if ( ( 0 < sDATA_LVL.wUSE_ARROWNUM ) && ( 0 < sDATA_LVL.wUSE_CHARMNUM ) && ( 0 < sDATA_LVL.wUSE_EXP ) && 
					 ( 0 < sDATA_LVL.wUSE_HP ) && ( 0 < sDATA_LVL.wUSE_MP ) && ( 0 < sDATA_LVL.wUSE_SP ) && 
					 ( 0 < sDATA_LVL.wUSE_CP ) && ( 0 < sDATA_LVL.wUSE_BULLET ) && ( 0 < sDATA_LVL.wUSE_EXTRA ) )
					 
						{
					 
							strText.AppendFormat ( "(%s %d)(%s %d)(%s %d)(%s %d)(%s %d)(%s %d)(%s %d)(%s %d)(%s %d)"
								,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 0), sDATA_LVL.wUSE_ARROWNUM
									,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 1), sDATA_LVL.wUSE_CHARMNUM
										,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 2), sDATA_LVL.wUSE_EXP
											,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 3), sDATA_LVL.wUSE_HP
												,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 4), sDATA_LVL.wUSE_MP
													,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 5), sDATA_LVL.wUSE_SP
														,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 9), sDATA_LVL.wUSE_CP
															,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 10), sDATA_LVL.wUSE_BULLET
																,ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 11), sDATA_LVL.wUSE_EXTRA);
								
								
								
																		
					 
						}*/
						
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
				
				
				}
			
		
			}
		}
		
		//Delay
		{
			const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;
		
			if ( pCharSkill )	//	배운 스킬일 경우에...
			{
				WORD wLevel = pCharSkill->wLevel;
				if ( bNextLevel ) wLevel = pCharSkill->wLevel + 1;

				const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
			
				if ( 0 < sDATA_LVL.fDELAYTIME )
				{
					strText.Format("Delay: %2.1fSecond(s)", sDATA_LVL.fDELAYTIME);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}
			}
		}
		//AddTextNoSplit(" ",NS_UITEXTCOLOR::DEFAULT); //SPACER
		{
				BOOL bNOT_LEARN = wDispLevel==0;
				BOOL bMaster = FALSE;
		//	요구치 정보
		if ( (!bMaster && bNextLevel) || bNOT_LEARN )
		{
			AddTextNoSplit("Skill Requirements:",NS_UITEXTCOLOR::DEFAULT);

			WORD wNextLevel = wDispLevel;
			GLCharacter* const pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
			SKILL::SLEARN_LVL& sLVL = sLEARN.sLVL_STEP[wNextLevel];			

		
			//	1. 요구보유스킬
			SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;			
			if ( NeedSkillID != NATIVEID_NULL() )
			{
				BOOL bVALID = FALSE;
				BOOL bNeedSkillLevel = FALSE;

				CString strNeedSkillName;
				CString strNeedSkillLevel;

				PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData ( NeedSkillID.wMainID, NeedSkillID.wSubID );
				if(( pNeedSkill ) && ( 0 < sLVL.dwSKILL_LVL ))
				{
				strNeedSkillName.Format("%s %dLv", pNeedSkill->GetName(), (sLVL.dwSKILL_LVL + 1) );
				bVALID = pCharacter->ISLEARNED_SKILL ( NeedSkillID );
				bNeedSkillLevel = TRUE;
				SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;				
				SCHARDATA2::SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
				if ( iter != map.end() )
					{
						SCHARSKILL& rCharSkill = (*iter).second;

						//	색 조절
						bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
					}

				}

				//	2. 요구보유스킬레벨
				/*if ( 0 < sLVL.dwSKILL_LVL )
				{
					strNeedSkillLevel.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 1), (sLVL.dwSKILL_LVL + 1) );
					bNeedSkillLevel = TRUE;

					SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;				
					SCHARDATA2::SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
					if ( iter != map.end() )
					{
						SCHARSKILL& rCharSkill = (*iter).second;

						//	색 조절
						bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
					}
				}*/

				AddTextNoSplit(strNeedSkillName,NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ));

				//if ( bNeedSkillLevel )
				//	AddTextNoSplit(strNeedSkillLevel, NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ) );
			}

			//	3. 요구경험치
			/*if ( 0 < sLVL.dwSKP )
			{
				strText.Format("%s %d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 2), sLVL.dwSKP);				
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_dwSkillPoint >= sLVL.dwSKP ) );
			}

			//	4. 요구레벨
			if ( 0 < sLVL.dwLEVEL )
			{
				strText.Format("%s %d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 3), sLVL.dwLEVEL);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETLEVEL () >= int(sLVL.dwLEVEL) ) );
			}*/

			if (( 0 < sLVL.dwSKP )&&( 0 < sLVL.dwLEVEL ))
			{
				strText.Format("Character Level: %d Skill Points: %d ",sLVL.dwLEVEL,sLVL.dwSKP);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor (( pCharacter->m_dwSkillPoint >= sLVL.dwSKP )&&
																			( pCharacter->GETLEVEL () >= int(sLVL.dwLEVEL) )) );
			}
			
			if ( 0 < sLVL.dwREBORN )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 4), sLVL.dwREBORN);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETREBORN () >= int(sLVL.dwREBORN) ) );
			}

			if ( 0 < sLVL.dwMoneyReq )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 5), sLVL.dwMoneyReq );
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnMoney >= int(sLVL.dwMoneyReq ) ) );
			}

			if ( 0 < sLVL.dwPremPoint )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 6), sLVL.dwPremPoint );
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnPremP >= int(sLVL.dwPremPoint ) ) );
			}

			if ( 0 < sLVL.dwVotePoint )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 7), sLVL.dwVotePoint );
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnVoteP >= int(sLVL.dwVotePoint ) ) );
			}

			//	조건 - 암광
			//BOOL bValue = TRUE;
			//strText.Format ( "%s", COMMENT::BRIGHT[sLEARN.emBRIGHT].c_str() );
			//if ( sLEARN.emBRIGHT != BRIGHT_BOTH )
			//{
			//	if ( pCharacter->GETBRIGHT() != sLEARN.emBRIGHT )
			//	{
			//		bValue = FALSE;
			//	}
			//}
			//AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );


			//	Stats
			//	1. 요구힘
			if ( 0 < sLVL.sSTATS.dwPow )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 0), sLVL.sSTATS.dwPow);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwPow >= sLVL.sSTATS.dwPow ) );
			}

			//	2. 요구체력
			if ( 0 < sLVL.sSTATS.dwStr )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 1), sLVL.sSTATS.dwStr);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwStr >= sLVL.sSTATS.dwStr ) );
			}

			//	3. 요구정신
			if ( 0 < sLVL.sSTATS.dwSpi )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 2), sLVL.sSTATS.dwSpi);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSpi >= sLVL.sSTATS.dwSpi ));
			}

			//	4. 요구민첩
			if ( 0 < sLVL.sSTATS.dwDex )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 3), sLVL.sSTATS.dwDex);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwDex >= sLVL.sSTATS.dwDex ) );
			}

			//	5. 요구지력
			if ( 0 < sLVL.sSTATS.dwInt )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 4), sLVL.sSTATS.dwInt);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwInt >= sLVL.sSTATS.dwInt ) );
			}

			//	6. 요구근력
			if ( 0 < sLVL.sSTATS.dwSta )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 5), sLVL.sSTATS.dwSta);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSta >= sLVL.sSTATS.dwSta ) );
			}
		}
		
		
		}
		

		//	마스터
		BOOL bMaster = FALSE;
		if ( bMASTER )
		{
			AddTextNoSplit(ID2GAMEWORD("SKILL_ETC_MASTER"), NS_UITEXTCOLOR::ENABLE);
			bMaster = TRUE;
		}
		//else
		//{
			/*if ( wDispLevel )
			{			
				if( !bNextLevel ) 
					strText.Format ("%s:%d", ID2GAMEWORD("SKILL_BASIC_INFO", 4), wDispLevel );

				AddTextNoSplit(strText, NS_UITEXTCOLOR::ENABLE);
			}*/
		//}


		/*//	목표 가능거리, 왼손, 오른손 도구
		{
			const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;		

			if ( sBASIC.wTARRANGE )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_TARGET_RANGE", 0), sBASIC.wTARRANGE);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			}

			if ( (sBASIC.emUSE_LITEM && (sBASIC.emUSE_LITEM != ITEMATT_NOCARE)) || 
				(sBASIC.emUSE_RITEM && (sBASIC.emUSE_RITEM != ITEMATT_NOCARE)) )
			{
				AddTextNoSplit(ID2GAMEWORD("SKILL_CATEGORY", 1),NS_UITEXTCOLOR::LIGHTSKYBLUE);
			}

			if ( sBASIC.emUSE_LITEM )
			{
				if ( sBASIC.emUSE_LITEM != ITEMATT_NOCARE )
				{
					strText.Format ( "%s:%s",	ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 0), 
												ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_LITEM - 1 ) );

					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}
			}

			if ( sBASIC.emUSE_RITEM )
			{
				if ( sBASIC.emUSE_RITEM != ITEMATT_NOCARE )
				{
					strText.Format ( "%s:%s",	ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 1), 
												ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_RITEM - 1 ) );

					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}
			}
		}*/
		//AddTextNoSplit(" ",NS_UITEXTCOLOR::LIGHTSKYBLUE);
	//	const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;	
		const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;
		
		//	기본 속성
		if ( pCharSkill )	//	배운 스킬일 경우에...
		{		
			WORD wLevel = pCharSkill->wLevel;
			if ( bNextLevel ) wLevel = pCharSkill->wLevel + 1;
			CString strTros;

			const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];

			AddTextNoSplit("",NS_UITEXTCOLOR::WHITE);
			AddTextNoSplit("[The Range of Skill]",NS_UITEXTCOLOR::GOLD);

			strTros.Format ("The %d target(s) within %dm and %d angle off attack range.", sDATA_LVL.wTARNUM, sDATA_LVL.wAPPLYRANGE, sDATA_LVL.wAPPLYANGLE);
			AddTextNoSplit(strTros,NS_UITEXTCOLOR::WHITE);	

			//if ( ( 0 < sDATA_LVL.wTARNUM ) && ( 0 < sDATA_LVL.wAPPLYRANGE ) && ( 0 < sDATA_LVL.wAPPLYANGLE ) )
		//	{
		//	AddTextNoSplit("[The Range of Skill]",NS_UITEXTCOLOR::WHITE);

		//	strTros.Format ("The %d targets within %dm and %d angle off attack range.", sDATA_LVL.wTARNUM, sDATA_LVL.wAPPLYRANGE, sDATA_LVL.wAPPLYANGLE);
		//	AddTextNoSplit(strTros,NS_UITEXTCOLOR::WHITE);
		//	}

			//	딜레이 타임
			/*if ( 0 < sDATA_LVL.fDELAYTIME )
			{
				strText.Format("%s:%2.1f",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 0), sDATA_LVL.fDELAYTIME);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			}*/

			//	지속 시간
			//if ( 0 < sDATA_LVL.fLIFE )
			//{
			//	strText.Format("%s:%2.1f",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 1), sDATA_LVL.fLIFE);
			//	AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			//}
			
			//	Target #
			/*if ( 0 < sDATA_LVL.wTARNUM )
			{
				strTarget+=("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 6), sDATA_LVL.wTARNUM);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			}
			
			//	Range
			if ( 0 < sDATA_LVL.wAPPLYRANGE )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 2), sDATA_LVL.wAPPLYRANGE);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			}*/

			//	적용 갯수
			//if ( 0 < sDATA_LVL.wAPPLYNUM )
			//{
			//	strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 3), sDATA_LVL.wAPPLYNUM);
			//	AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			//}

			//	Angle
			//if ( 0 < sDATA_LVL.wAPPLYANGLE )
			//{
			//	strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 4), sDATA_LVL.wAPPLYANGLE);
			//	AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			//}

			//	관통 정도
			//if ( 0 < sDATA_LVL.wPIERCENUM )
			//{
			//	strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 5), sDATA_LVL.wPIERCENUM);
			//	AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			//}
			
			/*if ( 0 != sDATA_LVL.fBASIC_VAR )
			{
				float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];

				if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
					strText.Format("%s:%2.1f%%",COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				else
					strText.Format("%s:%2.1f",COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );

				AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			}*/
			

			///////////////////////////////////////////////////////////////////////
			//	사용시 소진량
			/*if ( pSkill->m_sBASIC.emROLE != SKILL::EMROLE_PASSIVE )
			{
				AddTextNoSplit(ID2GAMEWORD("SKILL_CATEGORY", 3),NS_UITEXTCOLOR::LIGHTSKYBLUE);

				//	화살 개수
				if ( 0 < sDATA_LVL.wUSE_ARROWNUM )
				{
					
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 0), sDATA_LVL.wUSE_ARROWNUM);
					
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				//	부적 개수
				if ( 0 < sDATA_LVL.wUSE_CHARMNUM )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 1), sDATA_LVL.wUSE_CHARMNUM);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				//	EXP 소진량
				//if ( 0 < sDATA_LVL.wUSE_EXP )
				//{
				//	strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 2), sDATA_LVL.wUSE_EXP);
				//	AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				//}

				//	HP 소진량
				if ( 0 < sDATA_LVL.wUSE_HP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 3), sDATA_LVL.wUSE_HP);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				//	MP 소진량
				if ( 0 < sDATA_LVL.wUSE_MP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 4), sDATA_LVL.wUSE_MP);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				//	SP 소진량
				if ( 0 < sDATA_LVL.wUSE_SP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 5), sDATA_LVL.wUSE_SP);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				if ( 0 < sDATA_LVL.wUSE_CP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 9), sDATA_LVL.wUSE_CP);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				if ( 0 < sDATA_LVL.wUSE_BULLET )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 10), sDATA_LVL.wUSE_BULLET);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				if ( 0 < sDATA_LVL.wUSE_EXTRA )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 11), sDATA_LVL.wUSE_EXTRA);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				} 
			}*/
		}
		
		AddTextNoSplit(" ",NS_UITEXTCOLOR::LIGHTSKYBLUE); //Spacer
		//	Skill Effect
		if ( pCharSkill )	
		{	
			if ( sAPPLY.emSTATE_BLOW != EMBLOW_NONE )
			{
				AddTextNoSplit("[Attributes]",NS_UITEXTCOLOR::GOLD);

				AddTextNoSplit(COMMENT::BLOW [ sAPPLY.emSTATE_BLOW ].c_str(), NS_UITEXTCOLOR::GREEN );

				WORD wLevel = pCharSkill->wLevel;
				if ( bNextLevel ) wLevel = pCharSkill->wLevel + 1;

				const SKILL::SSTATE_BLOW& sSTATE_BLOW = sAPPLY.sSTATE_BLOW[wLevel];
				if ( 0 < sSTATE_BLOW.fRATE )
				{			
					strText.Format ( "%s:%2.1f", ID2GAMEWORD( "SKILL_STATE_BLOW", 0), sSTATE_BLOW.fRATE );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}

				if ( 0 < sSTATE_BLOW.fVAR1 )
				{
					strText.Format ( "%s:%2.1f", COMMENT::BLOW_VAR1[sAPPLY.emSTATE_BLOW].c_str(), sSTATE_BLOW.fVAR1 );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}

				if ( 0 < sSTATE_BLOW.fVAR2 )
				{
					strText.Format ( "%s:%2.1f", COMMENT::BLOW_VAR2[sAPPLY.emSTATE_BLOW].c_str(), sSTATE_BLOW.fVAR2 );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}
			}
		}

		//	부가효과
		if ( pCharSkill )	//	배운 스킬일 경우에...
		{
			if ( sAPPLY.emADDON != EMIMPACTA_NONE )
			{
				WORD wLevel = pCharSkill->wLevel;
				if ( bNextLevel ) wLevel = pCharSkill->wLevel + 1;

				const float fADDON_VAR = sAPPLY.fADDON_VAR[wLevel];
				if ( 0 < fADDON_VAR )
				{
					AddTextNoSplit("[Appended Effect]",NS_UITEXTCOLOR::GOLD);

					float fADDON_VAR_SCALE = fADDON_VAR * COMMENT::IMPACT_ADDON_SCALE[sAPPLY.emADDON];
					
					if ( COMMENT::IsIMPACT_ADDON_PER(sAPPLY.emADDON) )
					{
						strText.Format ( "%s:%2.2f%%", COMMENT::IMPACT_ADDON[sAPPLY.emADDON].c_str(), fADDON_VAR_SCALE );
					}
					else
					{
						strText.Format ( "%s:%2.2f", COMMENT::IMPACT_ADDON[sAPPLY.emADDON].c_str(), fADDON_VAR_SCALE );
					}

					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}
			}
		}
		AddTextNoSplit(" ",NS_UITEXTCOLOR::DEFAULT); //SPACER
		//	특수효과
		if ( pCharSkill )	//	배운 스킬일 경우에...
		{
			WORD wLevel = pCharSkill->wLevel;
			if ( bNextLevel ) wLevel = pCharSkill->wLevel + 1;

			const SKILL::SSPEC& sSPEC = sAPPLY.sSPEC[wLevel];
			const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
			CString strText;

			AddTextNoSplit("[Skill Effect]",NS_UITEXTCOLOR::GOLD);
				
				float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];

					strText.AppendFormat("Let target's HP decrease %2.1f", fBASIC_VAR_SCALE );
				
				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );

			if ( sAPPLY.emSPEC != EMSPECA_NULL )
			{

				strText.Format ( "%s:%s", ID2GAMEWORD("SKILL_SPEC_TYPE", 0), COMMENT::SPEC_ADDON[sAPPLY.emSPEC].c_str() );
				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				
				strText.Empty();
				CString strTemp;

				if ( sAPPLY.emSPEC == EMSPECA_NONBLOW || sAPPLY.emSPEC == EMSPECA_RECBLOW )
				{

					if ( sSPEC.dwFLAG & DIS_NUMB )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_NUMB].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_STUN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_STUN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_STONE )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_STONE].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_BURN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_BURN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_FROZEN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_FROZEN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_MAD )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_MAD].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_POISON )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_POISON].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_CURSE )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_CURSE].c_str() );
						strText += strTemp;
					}
				}
				// 발동 스킬
				else if ( sAPPLY.emSPEC == EMSPECA_DEFENSE_SKILL_ACTIVE )
				{
					GLSKILL* pSkillTemp = GLSkillMan::GetInstance().GetData( sSPEC.dwNativeID );
					if ( pSkillTemp )
					{
						strTemp.Format ( "%s Lv%d", pSkillTemp->GetName(),sSPEC.dwFLAG );
						strText += strTemp;
					}
				}

				if ( strText.GetLength () )
				{
					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}

				// 특수효과가 없음 일때는 화면에 보여주지 않도록 임시 수정~
				// 특수효과가 없음 일때는 툴에서 직접 값에 0을 넣도록 바꾸는게 좋다 ( 준혁 )
				if ( 0 != sSPEC.fVAR1 )
				{
					if( COMMENT::SPEC_ADDON_VAR1[EMSPECA_NULL] != COMMENT::SPEC_ADDON_VAR1[sAPPLY.emSPEC] )
					{
						float fVAR1_SCALE = sSPEC.fVAR1 * COMMENT::SPEC_ADDON_VAR1_SCALE[sAPPLY.emSPEC];

						if ( COMMENT::IsSPEC_ADDON1_PER(sAPPLY.emSPEC) )
							strText.Format ( "%s:%2.2f%%", COMMENT::SPEC_ADDON_VAR1[sAPPLY.emSPEC].c_str(), fVAR1_SCALE );
						else
							strText.Format ( "%s:%2.2f", COMMENT::SPEC_ADDON_VAR1[sAPPLY.emSPEC].c_str(), fVAR1_SCALE );
						
						AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
					}
				}

				if ( 0 != sSPEC.fVAR2 )
				{
					if( COMMENT::SPEC_ADDON_VAR2[EMSPECA_NULL] != COMMENT::SPEC_ADDON_VAR2[sAPPLY.emSPEC] )
					{
						float fVAR2_SCALE = sSPEC.fVAR2 * COMMENT::SPEC_ADDON_VAR2_SCALE[sAPPLY.emSPEC];

						if ( COMMENT::IsSPEC_ADDON2_PER(sAPPLY.emSPEC) )
							strText.Format ( "%s:%2.1f%%", COMMENT::SPEC_ADDON_VAR2[sAPPLY.emSPEC].c_str(), fVAR2_SCALE );
						else
							strText.Format ( "%s:%2.1f", COMMENT::SPEC_ADDON_VAR2[sAPPLY.emSPEC].c_str(), fVAR2_SCALE );

						AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
					}
				}				
			}
		}

		BOOL bNOT_LEARN = wDispLevel==0;

		//	요구치 정보
		/*if ( (!bMaster && bNextLevel) || bNOT_LEARN )
		{
			AddTextNoSplit("Skill Prerequisites :",NS_UITEXTCOLOR::DEFAULT);

			WORD wNextLevel = wDispLevel;
			GLCharacter* const pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
			SKILL::SLEARN_LVL& sLVL = sLEARN.sLVL_STEP[wNextLevel];			

		
			//	1. 요구보유스킬
			SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;			
			if ( NeedSkillID != NATIVEID_NULL() )
			{
				BOOL bVALID = FALSE;
				BOOL bNeedSkillLevel = FALSE;

				CString strNeedSkillName;
				CString strNeedSkillLevel;

				PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData ( NeedSkillID.wMainID, NeedSkillID.wSubID );
				if( pNeedSkill ) strNeedSkillName.Format("%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_INFO", 0), pNeedSkill->GetName() );
				bVALID = pCharacter->ISLEARNED_SKILL ( NeedSkillID );			

				//	2. 요구보유스킬레벨
				if ( 0 < sLVL.dwSKILL_LVL )
				{
					strNeedSkillLevel.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 1), (sLVL.dwSKILL_LVL + 1) );
					bNeedSkillLevel = TRUE;

					SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;				
					SCHARDATA2::SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
					if ( iter != map.end() )
					{
						SCHARSKILL& rCharSkill = (*iter).second;

						//	색 조절
						bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
					}
				}

				AddTextNoSplit(strNeedSkillName,NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ));

				if ( bNeedSkillLevel )
					AddTextNoSplit(strNeedSkillLevel, NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ) );
			}

			//	3. 요구경험치
			if ( 0 < sLVL.dwSKP )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 2), sLVL.dwSKP);				
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_dwSkillPoint >= sLVL.dwSKP ) );
			}

			//	4. 요구레벨
			if ( 0 < sLVL.dwLEVEL )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 3), sLVL.dwLEVEL);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETLEVEL () >= int(sLVL.dwLEVEL) ) );
			}

			if ( 0 < sLVL.dwREBORN )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 4), sLVL.dwREBORN);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETREBORN () >= int(sLVL.dwREBORN) ) );
			}

			if ( 0 < sLVL.dwMoneyReq )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 5), sLVL.dwMoneyReq );
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnMoney >= int(sLVL.dwMoneyReq ) ) );
			}

			if ( 0 < sLVL.dwPremPoint )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 6), sLVL.dwPremPoint );
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnPremP >= int(sLVL.dwPremPoint ) ) );
			}

			if ( 0 < sLVL.dwVotePoint )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 7), sLVL.dwVotePoint );
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnVoteP >= int(sLVL.dwVotePoint ) ) );
			}

			//	조건 - 암광
			//BOOL bValue = TRUE;
			//strText.Format ( "%s", COMMENT::BRIGHT[sLEARN.emBRIGHT].c_str() );
			//if ( sLEARN.emBRIGHT != BRIGHT_BOTH )
			//{
			//	if ( pCharacter->GETBRIGHT() != sLEARN.emBRIGHT )
			//	{
			//		bValue = FALSE;
			//	}
			//}
			//AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );


			//	Stats
			//	1. 요구힘
			if ( 0 < sLVL.sSTATS.dwPow )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 0), sLVL.sSTATS.dwPow);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwPow >= sLVL.sSTATS.dwPow ) );
			}

			//	2. 요구체력
			if ( 0 < sLVL.sSTATS.dwStr )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 1), sLVL.sSTATS.dwStr);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwStr >= sLVL.sSTATS.dwStr ) );
			}

			//	3. 요구정신
			if ( 0 < sLVL.sSTATS.dwSpi )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 2), sLVL.sSTATS.dwSpi);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSpi >= sLVL.sSTATS.dwSpi ));
			}

			//	4. 요구민첩
			if ( 0 < sLVL.sSTATS.dwDex )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 3), sLVL.sSTATS.dwDex);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwDex >= sLVL.sSTATS.dwDex ) );
			}

			//	5. 요구지력
			if ( 0 < sLVL.sSTATS.dwInt )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 4), sLVL.sSTATS.dwInt);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwInt >= sLVL.sSTATS.dwInt ) );
			}

			//	6. 요구근력
			if ( 0 < sLVL.sSTATS.dwSta )
			{
				strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 5), sLVL.sSTATS.dwSta);
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSta >= sLVL.sSTATS.dwSta ) );
			}
		}*/

		if ( bNextLevel ) return ;

		//	간단한 설명
		if( !pSkill->GetDesc() )	return;

		int StrLength = static_cast<int>( strlen( pSkill->GetDesc() ) );
		if ( StrLength )
		{
			AddTextNoSplit (/* ID2GAMEWORD ( "SKILL_CATEGORY", 8 )*/" ", NS_UITEXTCOLOR::LIGHTSKYBLUE );
			AddTextNoSplit("[Skill Description]",NS_UITEXTCOLOR::GOLD);
			AddTextLongestLineSplit ( pSkill->GetDesc(), NS_UITEXTCOLOR::WHITE );
		}
	}


	void LOAD ( const SNATIVEID& sNativeID, const BOOL bNextLevel )
	{
		if ( sNativeID != NATIVEID_NULL() )
		{
			BOOL bUPDATE = FALSE;
			if (( bNextLevel != m_bNextLevel_BEFORE_FRAME ) ||
				( sNativeID != m_sNativeIDBack) )
			{			
				bUPDATE = TRUE;
			}

			if ( bUPDATE )
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sNativeID.wMainID, sNativeID.wSubID );
				if ( pSkill )	//	스킬이 존재할 경우
				{
					RemoveAllInfo ();

					SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( sNativeID );					
					LOAD ( pSkill, pCharSkill, bNextLevel );
				}

				m_sNativeIDBack = sNativeID;
				m_bNextLevel_BEFORE_FRAME = bNextLevel;
			}		
		}
	}
};