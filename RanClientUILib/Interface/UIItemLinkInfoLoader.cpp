#include "StdAfx.h"
#include "UIInfoLoader.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "GLItemDef.h"
#include "GLItem.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "InnerInterface.h"
#include "ItemImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NS_ITEMINFO_ITEMLINK
{
    SITEMCUSTOM	m_sItemCustomBACK;
	BOOL		m_bShopOpenBACK;
    BOOL		m_bInMarketBACK;
	BOOL		m_bInPrivateMarketBACK;
	BOOL		m_bIsWEAR_ITEMBACK;
	WORD		m_wPosXBACK;
	WORD		m_wPosYBACK;

	void AddItemRender ( SNATIVEID sICONINDEX, const char* szTexture ) 
	{ 
		CInnerInterface::GetInstance().ADDITEM_RENDER ( sICONINDEX, szTexture ); 
	} 
	
	void ResetItemRender () 
	{ 
		CInnerInterface::GetInstance().RESETITEM_RENDER ();	
	}

	void SetNameItem ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDNAME_RENDER_ITEMLINK ( strText, dwColor );
	}

	void ResetNameRender ()
	{
		//CInnerInterface::GetInstance().RESETNAME_RENDER_ITEMLINK ();
	}

/*	void AddItemRender ( SNATIVEID sICONINDEX, const char* szTexture )
	{
		CInnerInterface::GetInstance().ADDITEM_RENDER_ITEMLINK ( sICONINDEX, szTexture );
	}

	void ResetItemRender ()
	{
		CInnerInterface::GetInstance().RESETITEM_RENDER_ITEMLINK ();	
	}*/

	void AddItemBoxRender ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex )
	{
		CInnerInterface::GetInstance().ADDITEMBOX_RENDER_ITEMLINK ( sICONINDEX, szTexture, nIndex );
	}

	void ResetItemBoxRender ()
	{
		CInnerInterface::GetInstance().RESETITEMBOX_RENDER_ITEMLINK ();	
	}
 
	void SetResiIcons ()
	{
		CInnerInterface::GetInstance().SETRESIICON_ITEMLINK ();	
	}
	
	void ResetResiIcons ()
	{
		CInnerInterface::GetInstance().RESETRESIICON_ITEMLINK ();	
	}

	void	RESET ()
	{
		m_sItemCustomBACK.sNativeID = NATIVEID_NULL ();
		m_bShopOpenBACK = FALSE;
		m_bInMarketBACK = FALSE;
		m_bInPrivateMarketBACK = FALSE;
		m_bIsWEAR_ITEMBACK = FALSE;
		ResetItemRender ();
		ResetItemBoxRender();
		ResetNameRender ();
		ResetResiIcons ();
	}

	void AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_NOSPLIT_ITEMLINK ( strText, dwColor );
	}

	void AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_LONGESTLINE_SPLIT_ITEMLINK ( strText, dwColor );
	}

	void RemoveAllInfo ()
	{
		CInnerInterface::GetInstance().CLEAR_TEXT_ITEMLINK ();
	}

	char*	GetNumberWithSign ( int nValue )
	{
		static	char szNumber[128];

		if ( 0 < nValue )
		{
			StringCchPrintf ( szNumber, 128, "+%d", nValue );
		}
		else if ( nValue < 0 )
		{
			StringCchPrintf ( szNumber, 128, "%d", nValue );
		}

		return szNumber;
	}

	void AddInfoItemAddon ( int nBasic, int nAddon, CString strFormat )
	{
		CString Text;
		if ( nAddon )
		{
			Text.Format ( "%s:%d%s", strFormat, nBasic, GetNumberWithSign ( nAddon ) );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasic )
		{
			Text.Format ( "%s:%d", strFormat, nBasic );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void APPEND_ITEM_GRADE ( CString& strOrigin, BYTE uGRADE )
	{
		if ( !uGRADE ) return ;

		CString strGRADE;
		strGRADE.Format ( "[+%d]", uGRADE );
		strOrigin += strGRADE;
	}

	void AddInfoItemAddon ( int nBasic, int nAddon, BYTE uGRADE, CString strFormat )
	{
		CString Text;
		if ( nAddon )
		{
			Text.Format ( "%s:%d%s", strFormat, nBasic, GetNumberWithSign( nAddon ) );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasic )
		{
			Text.Format ( "%s:%d", strFormat, nBasic );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void AddInfoItemAddonRange ( int nBasicMin, int nBasicMax, int nAddon, BYTE uGRADE, CString strFormat )
	{
		CString Text;

		if ( nAddon )
		{
			Text.Format ( "%s:%d%s~%d%s", strFormat, nBasicMin, GetNumberWithSign( nAddon ), nBasicMax, GetNumberWithSign( nAddon ) );
            APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasicMin || nBasicMax )
		{
			Text.Format ( "%s:%d~%d", strFormat, nBasicMin, nBasicMax );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void AddItemTurnInfo ( const SITEMCUSTOM &sItemCustom, const BOOL bInMarket, const BOOL bInPrivateMarket )
	{
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );

		CString strText;

		//switch ( pItemData->sBasicOp.emItemType )
		//{
		//case ITEM_CHARM:
		//case ITEM_ARROW:
		//case ITEM_CURE:
		//case ITEM_GRINDING:
			if ( pItemData->sDrugOp.wPileNum > 1 )
			{
				WORD wPileNum = pItemData->sDrugOp.wPileNum;
				WORD wTurnNum = sItemCustom.wTurnNum;				
				if ( bInPrivateMarket )	//	°³ÀÎ »óÁ¡
				{
					strText.Format("%s:%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum);
					AddTextNoSplit(strText,NS_UITEXTCOLOR::CHARTREUSE);

					return ;
				}

				if ( bInMarket )	wTurnNum = pItemData->GETAPPLYNUM();
				strText.Format("%s:%d/%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum, wPileNum);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
			}
			//break;
		//}
	}

	void AddInfoPetSkillItem( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.sNativeID;
		GLPetClient* pPetClient = GLGaeaClient::GetInstance().GetPetClient();
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		//	Note : ½ºÅ³ Á¤º¸ °¡Á®¿È.
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			if ( pPetClient->ISLEARNED_SKILL( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}
		}
	}

	void AddInfoSkillItem ( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.sNativeID;
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();	
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		//	Note : ½ºÅ³ Á¤º¸ °¡Á®¿È.
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			//	±âº» Á¤º¸
			{
				//	2. µî±Þ
				strText.Format("%s:%d",ID2GAMEWORD("SKILL_BASIC_INFO", 1), pSkill->m_sBASIC.dwGRADE);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);

				//	3. ¼Ó¼º
				strText.Format("%s:%s",ID2GAMEWORD("SKILL_BASIC_INFO", 2), COMMENT::BRIGHT[pSkill->m_sLEARN.emBRIGHT].c_str());
				AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);

				//	4. Á÷¾÷
				strText.Format ("%s:", ID2GAMEWORD("SKILL_BASIC_INFO", 3) );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_FIGHTER_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[0].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_FIGHTER_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[6].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARMS_M )			strText.AppendFormat ( "%s", COMMENT::CHARCLASS[1].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARMS_W )			strText.AppendFormat ( "%s", COMMENT::CHARCLASS[7].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[8].c_str() );				
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[2].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SPIRIT_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[9].c_str() );				
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SPIRIT_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[3].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[4].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[5].c_str() );
				//add class
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SCIENCE_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[10].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SCIENCE_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[11].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASIN_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[12].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASIN_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[13].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_TESTING_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[14].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_TESTING_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[15].c_str() );

				//AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqCharClass & pCharacter->m_emClass ) );
			}

			{
				//	¸ñÇ¥ °¡´É°Å¸®, ¿Þ¼Õ, ¿À¸¥¼Õ µµ±¸
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

				CString strUSEITEM;
				if ( sBASIC.emUSE_LITEM )
				{
					if ( sBASIC.emUSE_LITEM != ITEMATT_NOCARE )
					{
						strText.Format( "%s:%s", 
										ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 0), 
										ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_LITEM - 1 ) );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
					}
				}

				if ( sBASIC.emUSE_RITEM )
				{
					if ( sBASIC.emUSE_RITEM != ITEMATT_NOCARE )
					{
						strText.Format ( "%s:%s", 
										ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 1), 
										ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_RITEM - 1 ) );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
					}
				}
			}

			//	ÀÌ¹Ì ¹è¿î ½ºÅ³
			if ( pCharacter->GETLEARNED_SKILL ( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}

			//	¿ä±¸Ä¡ Á¤º¸
			{
				//AddTextNoSplit( ID2GAMEWORD("SKILL_CATEGORY", 7), NS_UITEXTCOLOR::LIGHTSKYBLUE);
				AddTextNoSplit ( "\n", NS_UITEXTCOLOR::BLACK);
				const WORD wLevel = 0;
				SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
				SKILL::SLEARN_LVL& sLVL = sLEARN.sLVL_STEP[wLevel];
			
				//	1. ¿ä±¸º¸À¯½ºÅ³
				SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;			
				if ( NeedSkillID != NATIVEID_NULL() )
				{
					BOOL bVALID = FALSE;
					BOOL bNeedSkillLevel = FALSE;

					CString strNeedSkillName;
					CString strNeedSkillLevel;

					PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData ( NeedSkillID.wMainID, NeedSkillID.wSubID );
					strNeedSkillName.Format("%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_INFO", 0), pNeedSkill->GetName() );
					bVALID = pCharacter->ISLEARNED_SKILL ( NeedSkillID );			

					//	2. ¿ä±¸º¸À¯½ºÅ³·¹º§
					if ( 0 < sLVL.dwSKILL_LVL )
					{
						strNeedSkillLevel.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 1), (sLVL.dwSKILL_LVL + 1) );
						bNeedSkillLevel = TRUE;

						SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;				
						SCHARDATA2::SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
						if ( iter != map.end() )
						{
							SCHARSKILL& rCharSkill = (*iter).second;

							//	»ö Á¶Àý
							bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
						}
					}

					AddTextNoSplit(strNeedSkillName,NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ));

					if ( bNeedSkillLevel )
						AddTextNoSplit(strNeedSkillLevel, NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ) );
				}

				//	3. ¿ä±¸°æÇèÄ¡
				if ( 0 < sLVL.dwSKP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 2), sLVL.dwSKP);				
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_dwSkillPoint >= sLVL.dwSKP ) );
				}

				//	4. ¿ä±¸·¹º§
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

				//	5. Á¶°Ç - ¾Ï±¤
				BOOL bValue = TRUE;
				strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
				if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
				{
					if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
					{
						bValue = FALSE;
					}
				}
				AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );


				//	Stats
				//	1. ¿ä±¸Èû
				if ( 0 < sLVL.sSTATS.dwPow )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 0), sLVL.sSTATS.dwPow);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwPow >= sLVL.sSTATS.dwPow ) );
				}

				//	2. ¿ä±¸Ã¼·Â
				if ( 0 < sLVL.sSTATS.dwStr )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 1), sLVL.sSTATS.dwStr);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwStr >= sLVL.sSTATS.dwStr ) );
				}

				//	3. ¿ä±¸Á¤½Å
				if ( 0 < sLVL.sSTATS.dwSpi )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 2), sLVL.sSTATS.dwSpi);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSpi >= sLVL.sSTATS.dwSpi ));
				}

				//	4. ¿ä±¸¹ÎÃ¸
				if ( 0 < sLVL.sSTATS.dwDex )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 3), sLVL.sSTATS.dwDex);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwDex >= sLVL.sSTATS.dwDex ) );
				}

				//	5. ¿ä±¸Áö·Â
				if ( 0 < sLVL.sSTATS.dwInt )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 4), sLVL.sSTATS.dwInt);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwInt >= sLVL.sSTATS.dwInt ) );
				}

				//	6. ¿ä±¸±Ù·Â
				if ( 0 < sLVL.sSTATS.dwSta )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 5), sLVL.sSTATS.dwSta);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSta >= sLVL.sSTATS.dwSta ) );
				}
			}
		}
	}

	void AddTextAddValue( const SITEMCUSTOM & sItemCustom, const ITEM::SSUIT & sSUIT )
	{
		CString strText;
		BOOL bLEAST_HAVE_ONE(TRUE);
		BOOL bADD_HP(FALSE), bADD_MP(FALSE), bADD_SP(FALSE), bADD_MA(FALSE);
		INT arrVALUE[EMADD_SIZE];
		SecureZeroMemory( arrVALUE, sizeof(arrVALUE) );

		for ( int i = 0; i < ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			EMITEM_ADDON emTYPE = sSUIT.sADDON[i].emTYPE;

			if ( emTYPE != EMADD_NONE )
			{
				//	NOTE
				//		°¡»êÈ¿°ú°¡ Á¸ÀçÇÒ°æ¿ì¿¡¸¸ Å¸ÀÌÆ²À»
				//		Ãâ·ÂÇÏ±â À§ÇØ, Àû¾îµµ ÇÏ³ª°¡ Á¸ÀçÇÒ¶§
				//		»Ñ¸°´Ù´Â °ÍÀ» Ã¼Å©ÇÑ´Ù.
				if ( bLEAST_HAVE_ONE )
				{
					//AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 10 ),NS_UITEXTCOLOR::LIGHTSKYBLUE);
					AddTextNoSplit( " ",NS_UITEXTCOLOR::LIGHTSKYBLUE);
					bLEAST_HAVE_ONE = FALSE;
				}

				int nVALUE = sSUIT.sADDON[i].nVALUE;

				switch ( emTYPE )
				{
				case EMADD_HP:
					if( !bADD_HP )
					{
						arrVALUE[EMADD_HP] = sItemCustom.GETADDHP();
						bADD_HP = TRUE;
					}
					break;
				case EMADD_MP:
					if( !bADD_MP )
					{
						arrVALUE[EMADD_MP] = sItemCustom.GETADDMP();
						bADD_MP = TRUE;
					}
					break;
				case EMADD_SP:
					if( !bADD_SP )
					{
						arrVALUE[EMADD_SP] = sItemCustom.GETADDSP();
						bADD_SP = TRUE;
					}
					break;
				case EMADD_MA:
					if( !bADD_MA )
					{
						arrVALUE[EMADD_MA] = sItemCustom.GETADDMA();
						bADD_MA = TRUE;
					}
					break;
				case EMADD_HITRATE:		arrVALUE[EMADD_HITRATE] += nVALUE;		break;
				case EMADD_AVOIDRATE:	arrVALUE[EMADD_AVOIDRATE] += nVALUE;	break;
				case EMADD_DAMAGE:		arrVALUE[EMADD_DAMAGE] += nVALUE;		break;
				case EMADD_DEFENSE:		arrVALUE[EMADD_DEFENSE] += nVALUE;		break;
				case EMADD_STATS_POW:	arrVALUE[EMADD_STATS_POW] += nVALUE;	break;
				case EMADD_STATS_STR:	arrVALUE[EMADD_STATS_STR] += nVALUE;	break;
				case EMADD_STATS_SPI:	arrVALUE[EMADD_STATS_SPI] += nVALUE;	break;
				case EMADD_STATS_DEX:	arrVALUE[EMADD_STATS_DEX] += nVALUE;	break;
				case EMADD_STATS_INT:	arrVALUE[EMADD_STATS_INT] += nVALUE;	break;
				case EMADD_STATS_STA:	arrVALUE[EMADD_STATS_STA] += nVALUE;	break;
				case EMADD_PA:			arrVALUE[EMADD_PA] += nVALUE;			break;
				case EMADD_SA:			arrVALUE[EMADD_SA] += nVALUE;			break;
				case EMADD_CP:			arrVALUE[EMADD_CP] += nVALUE;			break; //add cp
				};
			}
		}

		for( int i=1; i<EMADD_SIZE; ++i)
		{
			if( arrVALUE[i] != 0 )
			{
				strText.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFO", i ), GetNumberWithSign( arrVALUE[i] ) );
				AddTextNoSplit( strText, NS_UITEXTCOLOR::LIME );
			}
		}

		if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
		{
			SITEM* pJDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
			if ( pJDisguiseData )
			{
				if ( pJDisguiseData->sBasicOp.IsCostumeCombine() )
				{
					//add jitem
					INT arrVALUECOS[EMADD_SIZE];
					SecureZeroMemory( arrVALUECOS, sizeof(arrVALUECOS) );
	
					for ( int j = 0; j < ITEM::SSUIT::ADDON_SIZE; ++j )
					{
						EMITEM_ADDON emTYPEJ = pJDisguiseData->sSuitOp.sADDON[j].emTYPE;

						if ( emTYPEJ != EMADD_NONE )
						{
							if ( bLEAST_HAVE_ONE )
							{
								AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 13 ),NS_UITEXTCOLOR::PALEGREEN);
								bLEAST_HAVE_ONE = FALSE;
							}

							int nVALUECOS = pJDisguiseData->sSuitOp.sADDON[j].nVALUE;
							switch ( emTYPEJ )
							{
								case EMADD_HP:			arrVALUECOS[EMADD_HP] += nVALUECOS;			break;
								case EMADD_MP:			arrVALUECOS[EMADD_MP] += nVALUECOS;			break;
								case EMADD_SP:			arrVALUECOS[EMADD_SP] += nVALUECOS;			break;
								case EMADD_MA:			arrVALUECOS[EMADD_MA] += nVALUECOS;			break;
								case EMADD_HITRATE:		arrVALUECOS[EMADD_HITRATE] += nVALUECOS;		break;
								case EMADD_AVOIDRATE:	arrVALUECOS[EMADD_AVOIDRATE] += nVALUECOS;		break;
								case EMADD_DAMAGE:		arrVALUECOS[EMADD_DAMAGE] += nVALUECOS;		break;
								case EMADD_DEFENSE:		arrVALUECOS[EMADD_DEFENSE] += nVALUECOS;		break;
								case EMADD_STATS_POW:	arrVALUECOS[EMADD_STATS_POW] += nVALUECOS;		break;
								case EMADD_STATS_STR:	arrVALUECOS[EMADD_STATS_STR] += nVALUECOS;		break;
								case EMADD_STATS_SPI:	arrVALUECOS[EMADD_STATS_SPI] += nVALUECOS;		break;
								case EMADD_STATS_DEX:	arrVALUECOS[EMADD_STATS_DEX] += nVALUECOS;		break;
								case EMADD_STATS_INT:	arrVALUECOS[EMADD_STATS_INT] += nVALUECOS;		break;
								case EMADD_STATS_STA:	arrVALUECOS[EMADD_STATS_STA] += nVALUECOS;		break;
								case EMADD_PA:			arrVALUECOS[EMADD_PA] += nVALUECOS;			break;
								case EMADD_SA:			arrVALUECOS[EMADD_SA] += nVALUECOS;			break;
								case EMADD_CP:			arrVALUECOS[EMADD_CP] += nVALUECOS;			break; //add cp
							};
						}
					}

					CString strTextCos;
					for( int i=1; i<EMADD_SIZE; ++i)
					{
						if( arrVALUECOS[i] != 0 )
						{
							strTextCos.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFOCOS", i ), GetNumberWithSign( arrVALUECOS[i] ) );
							AddTextNoSplit( strTextCos, NS_UITEXTCOLOR::PALEGREEN );
						}		
					}
				}
			}	
		}
	}

	void LOAD_SIMPLE ( const SITEMCUSTOM &sItemCustom )
	{
		if ( m_sItemCustomBACK == sItemCustom ) return ;

		m_sItemCustomBACK = sItemCustom;

		RemoveAllInfo();

		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		if ( !pItemData ) return ;
		
		DWORD dwLevel = pItemData->sBasicOp.emLevel;
		//	ÀÌ¸§
		AddTextNoSplit ( pItemData->GetName(), COMMENT::ITEMCOLOR[dwLevel] );
	}

	void LOAD ( const SITEMCUSTOM &sItemCustom, const BOOL bShopOpen, const BOOL bInMarket, const BOOL bInPrivateMarket, const BOOL bIsWEAR_ITEM, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID )
	{	

		if ( m_sItemCustomBACK == sItemCustom && m_bShopOpenBACK == bShopOpen
			&& m_bInMarketBACK == bInMarket && m_bInPrivateMarketBACK == bInPrivateMarket
			&& m_bIsWEAR_ITEMBACK == bIsWEAR_ITEM && m_wPosXBACK == wPosX && m_wPosYBACK == wPosY)	return ;

		m_sItemCustomBACK = sItemCustom;
		m_bShopOpenBACK = bShopOpen;
		m_bInMarketBACK = bInMarket;
		m_bInPrivateMarketBACK = bInPrivateMarket;
		m_bIsWEAR_ITEMBACK = bIsWEAR_ITEM;
		m_wPosXBACK = wPosX;
		m_wPosYBACK = wPosY;

		RemoveAllInfo();

		CString strText, strText2;
		BOOL bValue = FALSE;
		int nExtraValue = 0;

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		SNATIVEID		m_sNativeID;


		{
			{
				BOOL bInsert = FALSE;
				EMITEMLEVEL emLevel = pItemData->sBasicOp.emLevel;
				strText.Format ( "%s",pItemData->GetName() );
				BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_NUNE);

				switch ( emLevel )
				{
				case LEVEL_NORMAL:
				case LEVEL_RARE:
					{
						CString strCombine;
						if ( uGRADE == 0 )
						{ strCombine.Format ( "%s", strText ); }

						else

						{ strCombine.Format ( "+%d %s", uGRADE, strText ); }

						DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

						DWORD strColorName[12] = 
						{			
							NS_UITEXTCOLOR::WHITE,//0
							NS_UITEXTCOLOR::WHITE,//1
							NS_UITEXTCOLOR::WHITE,//2
							NS_UITEXTCOLOR::WHITE,//3
							NS_UITEXTCOLOR::WHITESMOKE,//4
							NS_UITEXTCOLOR::SILVER,//5
							NS_UITEXTCOLOR::SILVER,//6
							NS_UITEXTCOLOR::ORANGE,//7 
							NS_UITEXTCOLOR::ORANGE,//8
							NS_UITEXTCOLOR::GOLD,//9
							NS_UITEXTCOLOR::YELLOW,//10
							NS_UITEXTCOLOR::DODGERBLUE,//11
						};

						int ColorGrade;
						if( uGRADE > 17 )
						{ 
							ColorGrade = 0;
						}
						else
						{
							ColorGrade = uGRADE;
						}
						SetNameItem( strCombine, strColorName[ColorGrade] );
						//Allow Spaces
						AddTextNoSplit ( " ", NS_UITEXTCOLOR::BLACK );
						AddTextNoSplit ( "\n", NS_UITEXTCOLOR::BLACK );
					}
					break;
					/*{
						CString strAdd;
						if ( uGRADE == 0 ) {
						strAdd.Format ( "        %s",strText); 
						} else
						{
							strAdd.Format ( "        +%d%s", uGRADE, strText); 
						}
						SetNameItem( strAdd, NS_UITEXTCOLOR::YELLOW );
						//Allow Spaces
						AddTextNoSplit ( " ", NS_UITEXTCOLOR::BLACK );
						AddTextNoSplit ( "\n", NS_UITEXTCOLOR::BLACK );
					}
					break;*/
				case LEVEL_UNIQUE:
					{
						CString strAdd;
						if ( uGRADE == 0 ) {
						strAdd.Format ( "        %s",strText); 
						} else
						{
							strAdd.Format ( "        +%d%s", uGRADE, strText); 
						}
						SetNameItem( strAdd, NS_UITEXTCOLOR::VIOLET );
						//Allow Spaces
						AddTextNoSplit ( " ", NS_UITEXTCOLOR::BLACK );
						AddTextNoSplit ( "\n", NS_UITEXTCOLOR::BLACK );
					}
					break;
				case LEVEL_VOTE_ITEM:
					{
						CString strAdd;
						if ( uGRADE == 0 ) {
						strAdd.Format ( "        %s",strText); 
						} else
						{
							strAdd.Format ( "        +%d%s", uGRADE, strText); 
						}   
						SetNameItem( strAdd, NS_UITEXTCOLOR::AQUABLUE );
						//Allow Spaces
						AddTextNoSplit ( " ", NS_UITEXTCOLOR::BLACK );
						AddTextNoSplit ( "\n", NS_UITEXTCOLOR::BLACK );
					}
					break;
				case LEVEL_PREMIUM_ITEM:
					{
						CString strAdd;
						if ( uGRADE == 0 ) {
						strAdd.Format ( "        %s",strText); 
						} else
						{
							strAdd.Format ( "        +%d%s", uGRADE, strText); 
						}   
						SetNameItem( strAdd, NS_UITEXTCOLOR::GREENYELLOW );
						//Allow Spaces
						AddTextNoSplit ( " ", NS_UITEXTCOLOR::BLACK );
						AddTextNoSplit ( "\n", NS_UITEXTCOLOR::BLACK );
					}
					break;
				}

				CString strSchool;
				if ( pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL )
					{
						CString strSchool;
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)
						{
							strSchool += "Sacred Gate";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)
						{
							strSchool += "Mystic Peak";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)
						{
							strSchool += "Phoenix";
						}						
					}
					else
					{
						strSchool = ID2GAMEWORD ( "ITEM_REQ_SCHOOL", 3 );
					}

			if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
			{
				SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
				DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
				DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
				CString szClass;

				if( dwReqCharClass )
					{
					//if ( (dwReqCharClass==GLCC_ALL_NEWSEX) && (dwReqCharClass_Disguise==GLCC_ALL_NEWSEX) )
					//add class
					if ( (dwReqCharClass==GLCC_ALL_2012) && (dwReqCharClass_Disguise==GLCC_ALL_2012) )
					{
						szClass = "/";
						szClass += ID2GAMEWORD("ITEM_ALL_CLASS");
					}
					else
					{
						if ( (dwReqCharClass&GLCC_FIGHTER_M) && (dwReqCharClass_Disguise&GLCC_FIGHTER_M) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
						}
						if ( (dwReqCharClass&GLCC_FIGHTER_W) && (dwReqCharClass_Disguise&GLCC_FIGHTER_W) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
						}
						if ( (dwReqCharClass&GLCC_ARMS_M)  && (dwReqCharClass_Disguise&GLCC_ARMS_M) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
						}
						if ( (dwReqCharClass&GLCC_ARMS_W)  && (dwReqCharClass_Disguise&GLCC_ARMS_W) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
						}
						if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
						}
						if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
						}
						if ( (dwReqCharClass&GLCC_SPIRIT_M)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_M) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
						}
						if ( (dwReqCharClass&GLCC_SPIRIT_W)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_W) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
						}
						if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
						}
						if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
						}
						//add class
						if ( (dwReqCharClass&GLCC_SCIENCE_M)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_M) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
						}
						if ( (dwReqCharClass&GLCC_SCIENCE_W)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_W) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
						}
						if ( (dwReqCharClass&GLCC_ASSASIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_M) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
						}
						if ( (dwReqCharClass&GLCC_ASSASIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_W) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
						}
						if ( (dwReqCharClass&GLCC_TESTING_M)  && (dwReqCharClass_Disguise&GLCC_TESTING_M) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
						}
						if ( (dwReqCharClass&GLCC_TESTING_W)  && (dwReqCharClass_Disguise&GLCC_TESTING_W) )
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
						}
					}

						strText.Format( "%s", szClass.GetString() );
						//AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor( 
						//(dwReqCharClass&pCharacter->m_emClass) && (dwReqCharClass_Disguise&pCharacter->m_emClass) ) );
				}
			}
			else
			{
				DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
					CString szClass;
						
					if( dwReqCharClass )
					{
					//if ( dwReqCharClass==GLCC_ALL_NEWSEX)
					//add class
					if ( dwReqCharClass==GLCC_ALL_2012)
					{
						szClass = "/";
						szClass += ID2GAMEWORD("ITEM_ALL_CLASS");
					}
					else
					{
						if ( dwReqCharClass & GLCC_FIGHTER_M)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
						}
						if ( dwReqCharClass & GLCC_FIGHTER_W)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
						}
						if ( dwReqCharClass & GLCC_ARMS_M)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
						}
						if ( dwReqCharClass & GLCC_ARMS_W)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
						}
						if ( dwReqCharClass & GLCC_ARCHER_M)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
						}
						if ( dwReqCharClass & GLCC_ARCHER_W)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
						}
						if ( dwReqCharClass & GLCC_SPIRIT_M)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
						}
						if ( dwReqCharClass & GLCC_SPIRIT_W)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
						}
						if ( dwReqCharClass & GLCC_EXTREME_M)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
						}
						if ( dwReqCharClass & GLCC_EXTREME_W)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
						}
						//add class
						if ( dwReqCharClass & GLCC_SCIENCE_M)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
						}
						if ( dwReqCharClass & GLCC_SCIENCE_W)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
						}
						if ( dwReqCharClass & GLCC_ASSASIN_M)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
						}
						if ( dwReqCharClass & GLCC_ASSASIN_W)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
						}
						if ( dwReqCharClass & GLCC_TESTING_M)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
						}
						if ( dwReqCharClass & GLCC_TESTING_W)
						{
							szClass+="/";
							szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
						}
						//
					}

					strText.Format( "%s", szClass.GetString() );
					//AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
				}
			}

			CString strItemReq = "\t" + strSchool + strText;
			AddTextNoSplit ( strItemReq, NS_UITEXTCOLOR::WHITE );
			
			if ( pItemData->sBasicOp.wReqLevelDW || pItemData->sBasicOp.wReqLevelUP )
			{
				bool bReqLevel = true;
				strText.Format( "\t%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
									
				if ( pItemData->sBasicOp.wReqLevelUP )
				{
					strText2.Format ( " ~ %d",pItemData->sBasicOp.wReqLevelUP );
					strText += strText2; 
					bReqLevel = (pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel);
				}

				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel && bReqLevel ) );
			}
			else
			{
				AddTextNoSplit( " ", NS_UITEXTCOLOR::BLACK );
			}

			if ( pItemData->sBasicOp.emItemType == ITEM_BOX )
			{
				AddTextNoSplit ( " ", NS_UITEXTCOLOR::BLACK );
				AddTextNoSplit ( " ", NS_UITEXTCOLOR::BLACK );
				AddTextNoSplit ( " ", NS_UITEXTCOLOR::BLACK );
			}

			const int nELEC   = sItemCustom.GETRESIST_ELEC();
			const int nFIRE   = sItemCustom.GETRESIST_FIRE();
			const int nICE    = sItemCustom.GETRESIST_ICE();
			const int nPOISON = sItemCustom.GETRESIST_POISON();
			const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();
					
			if ( (nELEC) && (nFIRE) && (nICE) && (nPOISON) && (nSPIRIT) )
			{
				strText.Format("Resistance:\t%d\t%d\t%d\t%d\t%d", nELEC,  nFIRE,  nICE,  nPOISON,  nSPIRIT );
				BYTE uGRADE = ( (sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC)) 	 && 
								(sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE)) 	 && 
								(sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE)) 	 && 
								(sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON)) && 
								(sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT)));
				APPEND_ITEM_GRADE ( strText, uGRADE );
				AddTextNoSplit(strText,NS_UITEXTCOLOR::LIME);
			}
			//	½Òµ{»RÁÐ
			if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
			{
				SITEM* pDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );

				strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 3 ), pDisguiseData->GetName() );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::GREENYELLOW );
			}

			//	ÄÚ½ºÃõ ±â°£ Ç¥½Ã
			if ( sItemCustom.tDISGUISE!=0 )
			{				
				CTime cTime(sItemCustom.tDISGUISE);
				if ( cTime.GetYear()!=1970 )
				{
					CString strExpireDate;
					strExpireDate = CInnerInterface::GetInstance().MakeString( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
																				(cTime.GetYear ()%2000), 
																				cTime.GetMonth (), 
																				cTime.GetDay (), 
																				cTime.GetHour (), 
																				cTime.GetMinute () );

					strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 7 ), strExpireDate );			
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}
			}

			DWORD dwCOMMISSION_MONEY = 0;

			//	2.°¡°Ý
			if ( pItemData->sBasicOp.dwBuyPrice || bInPrivateMarket )
			{
				if ( bInPrivateMarket )
				{
					bool bOPENER;
					DWORD dwPrivateMarketID;
					CInnerInterface::GetInstance().GetPrivateMarketInfo ( bOPENER, dwPrivateMarketID );
					if ( bOPENER )	//	ÆÄ´Â »ç¶÷
					{
						GLPrivateMarket &sPMarket = GLGaeaClient::GetInstance().GetCharacter()->m_sPMarket;

						const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
						if ( pSALE )
						{
							CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
							strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );

							DWORD dwColorTemp(0);
							if ( pSALE->llPRICE < 100000 )
								dwColorTemp = NS_UITEXTCOLOR::WHITE;
							else if ( pSALE->llPRICE < 1000000 )
								dwColorTemp = NS_UITEXTCOLOR::ORANGE;
							else if ( pSALE->llPRICE < 10000000 )
								dwColorTemp = NS_UITEXTCOLOR::YELLOW;
							else if ( pSALE->llPRICE < 100000000 )
								dwColorTemp = D3DCOLOR_ARGB(0xFF,0x3F,0xB6,0x10);
							else if ( pSALE->llPRICE < 1000000000 )
								dwColorTemp = D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
							else if ( pSALE->llPRICE < 10000000000 )
								dwColorTemp = D3DCOLOR_ARGB(0xFF,0x7F,0xAA,0xFF);
							else
								dwColorTemp = D3DCOLOR_ARGB(0xFF,0xE1,0x3E,0x78);

							AddTextNoSplit ( strText, NS_UITEXTCOLOR::GOLD );
						}						
					}
					else		//	»ç´Â »ç¶÷
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar ( dwPrivateMarketID );
						if ( !pCLIENT ) return ;

						GLPrivateMarket &sPMarket = pCLIENT->m_sPMarket;

						const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
						if ( pSALE )
						{
							CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
							strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );	

							DWORD dwColorTemp(0);
							if ( pSALE->llPRICE < 100000 )
								dwColorTemp = NS_UITEXTCOLOR::WHITE;
							else if ( pSALE->llPRICE < 1000000 )
								dwColorTemp = NS_UITEXTCOLOR::ORANGE;
							else if ( pSALE->llPRICE < 10000000 )
								dwColorTemp = NS_UITEXTCOLOR::YELLOW;
							else if ( pSALE->llPRICE < 100000000 )
								dwColorTemp = D3DCOLOR_ARGB(0xFF,0x3F,0xB6,0x10);
							else if ( pSALE->llPRICE < 1000000000 )
								dwColorTemp = D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
							else if ( pSALE->llPRICE < 10000000000 )
								dwColorTemp = D3DCOLOR_ARGB(0xFF,0x7F,0xAA,0xFF);
							else
								dwColorTemp = D3DCOLOR_ARGB(0xFF,0xE1,0x3E,0x78);

							AddTextNoSplit ( strText, NS_UITEXTCOLOR::GOLD );
						}
					}
				}				
				else if ( bShopOpen && bInMarket )	// npc open , check item inside inventory ( item price inside npc)
				{
					LONGLONG dwNpcSellPrice = 0;

					volatile LONGLONG dwPrice = 0;
					volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();
					volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;

					//if( sNpcNativeID.wMainID != 0 && sNpcNativeID.wSubID != 0 ) //npc itemprice fix
					{
						PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( sNpcNativeID );
						if( pCrowData != NULL )
						{
							LONGLONG dwNpcPrice = pCrowData->GetNpcSellPrice( pItemData->sBasicOp.sNativeID.dwID );
							//add money 2
							//no tax for vote item
							if ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
							{
								dwPrice = pItemData->sBasicOp.dwBuyPrice;
							}
							//add money 3
							//no tax for premium item
							else if ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
							{
								dwPrice = pItemData->sBasicOp.dwBuyPrice;
							}
							else
							{			
								if( dwNpcPrice == 0 )
								{								
									dwNpcSellPrice = pItemData->sBasicOp.dwBuyPrice;
									dwPrice = DWORD ( (float)dwNpcSellPrice * fSHOP_RATE_C );
								}
								else
								{
									dwNpcSellPrice = dwNpcPrice;
									dwPrice = dwNpcSellPrice;								
								}
							}
						}
					}				

					
					CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwPrice, 3, "," );

					/*strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );
					*/

					//add money 2 checking price red 
		
					//	Ä¿¹Ì¼Ç ¾×¼ö
					//add money 2
					//no tax for vote item
					if ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
					{
						dwCOMMISSION_MONEY  = 0;
					}
					//add money 3
					//no tax for premium item
					else if ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
					{
						dwCOMMISSION_MONEY  = 0;
					}
					else
					{
						dwCOMMISSION_MONEY = dwPrice - dwNpcSellPrice;
					}
					
					D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;

					if ( dwPrice < 100000 )
						dwColor = NS_UITEXTCOLOR::WHITE;
					else if ( dwPrice < 1000000 )
						dwColor = NS_UITEXTCOLOR::ORANGE;
					else if ( dwPrice < 10000000 )
						dwColor = NS_UITEXTCOLOR::YELLOW;
					else if ( dwPrice < 100000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x3F,0xB6,0x10);
					else if ( dwPrice < 1000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
					else if ( dwPrice < 10000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x7F,0xAA,0xFF);
					else
						dwColor = D3DCOLOR_ARGB(0xFF,0xE1,0x3E,0x78);
					/*if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
					{
						dwColor = NS_UITEXTCOLOR::PALEGREEN;
					}*/
					//add money 2

					// done price color respond with item gold price or item  premium price
					//add money 2
					if ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) // check price color for premium items
					{
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 11), strMoney );

						if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnVoteP )
						{
							dwColor = NS_UITEXTCOLOR::LIGHTSKYBLUE;
						}
					}
					//add money 3
					else if ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) // check price color for premium items
					{
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 13), strMoney );

						if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnPremP )
						{
							dwColor = NS_UITEXTCOLOR::LIGHTSKYBLUE;
						}
					}

					else
					{
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );

						if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
						{
							dwColor = NS_UITEXTCOLOR::PALEGREEN;
						}
					}

					//DWORD dwColor(0);
					if ( dwPrice < 100000 )
						dwColor = NS_UITEXTCOLOR::WHITE;
					else if ( dwPrice < 1000000 )
						dwColor = NS_UITEXTCOLOR::ORANGE;
					else if ( dwPrice < 10000000 )
						dwColor = NS_UITEXTCOLOR::YELLOW;
					else if ( dwPrice < 100000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x3F,0xB6,0x10);
					else if ( dwPrice < 1000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
					else if ( dwPrice < 10000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x7F,0xAA,0xFF);
					else
						dwColor = D3DCOLOR_ARGB(0xFF,0xE1,0x3E,0x78);
				
					AddTextNoSplit ( strText, dwColor );
				}
				else if ( bShopOpen ) // npc open (check item price of items inside inventory not inside npc)	// 
				{
					if ( pItemData->sBasicOp.IsPointRecover() )
					{//add and modified for //add point sell
						volatile DWORD dwPrice = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
						dwCOMMISSION_MONEY = 0;
						CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwPrice, 3, "," );

						

						if ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
						{	
							strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 14), strMoney );
                            DWORD dwColor(0);
                            if ( dwPrice < 100000 )
						dwColor = NS_UITEXTCOLOR::WHITE;
					else if ( dwPrice < 1000000 )
						dwColor = NS_UITEXTCOLOR::ORANGE;
					else if ( dwPrice < 10000000 )
						dwColor = NS_UITEXTCOLOR::YELLOW;
					else if ( dwPrice < 100000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x3F,0xB6,0x10);
					else if ( dwPrice < 1000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
					else if ( dwPrice < 10000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x7F,0xAA,0xFF);
					else
						dwColor = D3DCOLOR_ARGB(0xFF,0xE1,0x3E,0x78);

							AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
						}
						else if ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
						{	
							strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 15), strMoney );
                             DWORD dwColor(0);
							if ( dwPrice < 100000 )
						dwColor = NS_UITEXTCOLOR::WHITE;
					else if ( dwPrice < 1000000 )
						dwColor = NS_UITEXTCOLOR::ORANGE;
					else if ( dwPrice < 10000000 )
						dwColor = NS_UITEXTCOLOR::YELLOW;
					else if ( dwPrice < 100000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x3F,0xB6,0x10);
					else if ( dwPrice < 1000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
					else if ( dwPrice < 10000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x7F,0xAA,0xFF);
					else
						dwColor = D3DCOLOR_ARGB(0xFF,0xE1,0x3E,0x78);
							
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
						}
						else
						{
							AddTextNoSplit ( "Wrong item setting, enable only point recover if item is set to premium or vote", NS_UITEXTCOLOR::RED );
						}
					}
					else
					{//default
						volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetSaleRate();
						volatile float fSALE_DISCOUNT = fSHOP_RATE * 0.01f;

						volatile DWORD dwPrice = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
						volatile DWORD dwSALE_PRICE = DWORD ( dwPrice * fSALE_DISCOUNT );					

						//	Ä¿¹Ì¼Ç ¾×¼ö
						volatile DWORD dwDISPRICE = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
						dwCOMMISSION_MONEY = dwDISPRICE - dwSALE_PRICE;

						CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwSALE_PRICE, 3, "," );
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 12), strMoney );
                        
						DWORD dwColor(0);
							if ( dwSALE_PRICE < 100000 )
						dwColor = NS_UITEXTCOLOR::WHITE;
					else if ( dwSALE_PRICE < 1000000 )
						dwColor = NS_UITEXTCOLOR::ORANGE;
					else if ( dwSALE_PRICE < 10000000 )
						dwColor = NS_UITEXTCOLOR::YELLOW;
					else if ( dwSALE_PRICE < 100000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x3F,0xB6,0x10);
					else if ( dwSALE_PRICE < 1000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
					else if ( dwSALE_PRICE < 10000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x7F,0xAA,0xFF);
					else
						dwColor = D3DCOLOR_ARGB(0xFF,0xE1,0x3E,0x78);
						

						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
					}

					//CString strErr;
					//strErr.Format ( "fSHOP_RATE:%f / fSALE_DISCOUNT:%f / dwPrice:%d /dwSALE_PRICE:%d ",
					//fSHOP_RATE,fSALE_DISCOUNT,dwPrice,dwSALE_PRICE );
					//MessageBox ( NULL, strErr.GetString(), "ERROR", MB_OK );

				}
			}

			//	¼ö¼ö·á Ç¥½Ã
			if ( dwCOMMISSION_MONEY )
			{
				CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwCOMMISSION_MONEY, 3, "," );
				strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 6), strMoney );
                DWORD dwColor(0);
				if ( dwCOMMISSION_MONEY < 100000 )
						dwColor = NS_UITEXTCOLOR::WHITE;
					else if ( dwCOMMISSION_MONEY < 1000000 )
						dwColor = NS_UITEXTCOLOR::ORANGE;
					else if ( dwCOMMISSION_MONEY < 10000000 )
						dwColor = NS_UITEXTCOLOR::YELLOW;
					else if ( dwCOMMISSION_MONEY < 100000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x3F,0xB6,0x10);
					else if ( dwCOMMISSION_MONEY < 1000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
					else if ( dwCOMMISSION_MONEY < 10000000000 )
						dwColor = D3DCOLOR_ARGB(0xFF,0x7F,0xAA,0xFF);
					else
						dwColor = D3DCOLOR_ARGB(0xFF,0xE1,0x3E,0x78);

				AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
			}

			//	¼ö·®Ç¥½Ã
			AddItemTurnInfo ( sItemCustom, bInMarket, bInPrivateMarket );

			//	±â°£Ç¥½Ã
			if ( pItemData->IsTIMELMT () )
			{
				CTime cTime(sItemCustom.tBORNTIME);
				if ( cTime.GetYear()!=1970 )
				{
					CTimeSpan sLMT(pItemData->sDrugOp.tTIME_LMT);
					cTime += sLMT;

					CString strExpireDate;
					strExpireDate = CInnerInterface::GetInstance().MakeString ( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
						(cTime.GetYear ()%2000), cTime.GetMonth (), cTime.GetDay (), cTime.GetHour (), cTime.GetMinute () );

					strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 4 ), strExpireDate );			
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}
			}

			

			//add reborn req
			if ( pItemData->sBasicOp.dwRebornReq )
			{
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 3), pItemData->sBasicOp.dwRebornReq );
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwRebornReq <= pCharacter->m_dwReborn  ) );
			}


			//	Àç»ç¿ë ´ë±â½Ã°£
			if ( pItemData->sBasicOp.IsCoolTime() )
			{
				CString strTime = "";
				CTimeSpan cCoolTime( pItemData->sBasicOp.dwCoolTime );
				
				if ( cCoolTime.GetHours() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetHours(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 0 ) );
				if ( cCoolTime.GetMinutes() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetMinutes(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 1 ) );
				if ( cCoolTime.GetSeconds() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetSeconds(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 2 ) );

				strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 8 ), strTime );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );	
			}

			//	³²Àº½Ã°£


			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			if ( pCharacter && pCharacter->IsCoolTime( pItemData->sBasicOp.sNativeID ) )
			{
				CString strTime = "";
				__time64_t tCurTime =  GLGaeaClient::GetInstance().GetCurrentTime().GetTime();
				__time64_t tCoolTime = pCharacter->GetMaxCoolTime( pItemData->sBasicOp.sNativeID );				

				CTimeSpan cReTime( tCoolTime - tCurTime );

                if ( cReTime.GetHours() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetHours(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 0 ) );
				if ( cReTime.GetMinutes() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetMinutes(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 1 ) );
				if ( cReTime.GetSeconds() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetSeconds(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 2 ) );

				strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 9 ), strTime );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//	AdvInfo
			SCHARSTATS& rItemStats = pItemData->sBasicOp.sReqStats;
			SCHARSTATS& rCharStats = pCharacter->m_sSUMSTATS;
			EMITEM_TYPE emItemType = pItemData->sBasicOp.emItemType;

			BOOL bSetItemList(FALSE);//add ®M¸Ë¨t²Î

			switch ( emItemType )
			{
			case ITEM_SUIT:
			case ITEM_REVIVE:
			case ITEM_ANTI_DISAPPEAR:
            //case ITEM_VEHICLE:
				{

					bSetItemList = TRUE;//add ®M¸Ë¨t²Î
					//AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					BYTE uGRADE = 0;

					//	°ø°Ý ¼Ó¼º
					if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
					{
						strText.Format("%s (%s)", ID2GAMEWORD("ITEM_ADVANCED_INFO", 6), COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	Âø¿ëÀ§Ä¡
					if(pItemData->sSuitOp.emSuit!=SUIT_HANDHELD)
					{
						strText.Format("%s ", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
						AddTextNoSplit (strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	°ø°Ý °Å¸®
					if ( sItemCustom.GETATTRANGE() )
					{
						strText.Format("%s: %dm",ID2GAMEWORD("ITEM_ADVANCED_INFO", 2), sItemCustom.GETATTRANGE() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	°ø°Ý·Â
					GLPADATA &sDamage = sItemCustom.getdamage();
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
					AddInfoItemAddonRange ( sDamage.dwLow, sDamage.dwHigh, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );

					//	±â·ÂÄ¡
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					if ( nExtraValue )
					{
						uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
						strText.Format ( "%s:+%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 8), nExtraValue );
						APPEND_ITEM_GRADE ( strText, uGRADE );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PRIVATE );
					}

					//	¹æ¾î·Â
					short nDefense = sItemCustom.getdefense();
					nExtraValue = sItemCustom.GETGRADE_DEFENSE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);
					AddInfoItemAddon ( nDefense, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

					//	¸íÁß·ü
					if ( sItemCustom.GETHITRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETHITRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );		
					}

					//	È¸ÇÇÀ²
					if ( sItemCustom.GETAVOIDRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETAVOIDRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );
					}
					
					//	SP ¼Ò¸ð
					const WORD wReqSP = sItemCustom.GETREQ_SP();
					if ( 0 < wReqSP )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7), wReqSP );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::ORANGE );
					}

					//	Á¶°Ç - ¾Ï±¤
					/*
					bValue = TRUE;
					strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
					if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
					{
						if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
						{
							bValue = FALSE;
						}
					}
					AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );					
					*/

					//	Âø¿ëÁ¶°Ç->°ÝÅõÄ¡
					if ( pItemData->sBasicOp.wReqPA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqPA <= pCharacter->m_wSUM_PA ) );
					}
					//	Âø¿ëÁ¶°Ç->»ç°ÝÄ¡
					if ( pItemData->sBasicOp.wReqSA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
						AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqSA <= pCharacter->m_wSUM_SA ));
					}

					//	Stats
					//	Âø¿ëÁ¶°Ç->Èû
					if ( rItemStats.dwPow )
					{
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.dwPow );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwPow <= rCharStats.dwPow ) );
					}
					//	Âø¿ëÁ¶°Ç->Ã¼·Â
					if ( rItemStats.dwStr )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.dwStr );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwStr <= rCharStats.dwStr ) );
					}
					//	Âø¿ëÁ¶°Ç->Á¤½Å·Â
					if ( rItemStats.dwSpi)
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.dwSpi );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSpi <= rCharStats.dwSpi ) );
					}
					//	Âø¿ëÁ¶°Ç->¹ÎÃ¸¼º
					if ( rItemStats.dwDex )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.dwDex );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwDex <= rCharStats.dwDex ) );
					}
					//	Âø¿ëÁ¶°Ç->Áö·Â
					if ( rItemStats.dwInt  )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.dwInt );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwInt <= rCharStats.dwInt ) );
					}
					//	Âø¿ëÁ¶°Ç->±Ù·Â
					if ( rItemStats.dwSta )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.dwSta );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSta <= rCharStats.dwSta ) );
					}

					//-----------------------------------------------------------------------------------------------
					//	ÀúÇ×°ª
					/*const int nELEC   = sItemCustom.GETRESIST_ELEC();
					const int nFIRE   = sItemCustom.GETRESIST_FIRE();
					const int nICE    = sItemCustom.GETRESIST_ICE();
					const int nPOISON = sItemCustom.GETRESIST_POISON();
					const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();

					if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 3 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						if ( nELEC )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 0 ), nELEC );	
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nFIRE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 1 ), nFIRE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nICE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 2 ), nICE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nPOISON )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 3 ), nPOISON );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nSPIRIT )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 4 ), nSPIRIT );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
					}*/
		
					//	»óÅÂÀÌ»ó
					EMSTATE_BLOW emBLOW = pItemData->sSuitOp.sBLOW.emTYPE;
					if ( emBLOW !=EMBLOW_NONE )
					{
						AddTextNoSplit ( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );

						if ( emBLOW == EMBLOW_NUMB )
						{
							float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];
							float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];
							strText.Format("%.0f", pItemData->sSuitOp.sBLOW.fRATE );
							strText += ("% chance of Paralysis effect, decreased" );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							strText.Format("movement and attack speed by %.0f", fVAR1 );
							strText += "%, skill";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							strText.Format("cool down time is increased by %.0f for %.0f", fVAR2, pItemData->sSuitOp.sBLOW.fLIFE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							AddTextNoSplit("seconds",NS_UITEXTCOLOR::YELLOW);
						}
						else if ( emBLOW == EMBLOW_STUN )
						{
							strText.Format("%.0f", pItemData->sSuitOp.sBLOW.fRATE );
							strText += ( "% chance of %s effect, unable to act for", COMMENT::BLOW[emBLOW].c_str() );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							strText.Format("%.1f seconds", pItemData->sSuitOp.sBLOW.fLIFE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
						}
						else if ( emBLOW == EMBLOW_STONE )
						{
							float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];
							strText.Format("%.0f", pItemData->sSuitOp.sBLOW.fRATE );
							strText += "% chance Fossil effect, decreased";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							strText.Format("movement and attack speed by %.0f", fVAR1 );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							AddTextNoSplit("seconds",NS_UITEXTCOLOR::YELLOW);
						}
						else if ( emBLOW == EMBLOW_MAD )
						{
							float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];
							strText.Format("With the probability of %.0f", pItemData->sSuitOp.sBLOW.fRATE );
							strText += "% to give Fossil";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							strText.Format(" effect to the opponent and attack with the " );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							strText.Format(" effect of attack speed and moving speed" );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
							strText.Format(" continuous %.1f seconds to decreased %.0f", pItemData->sSuitOp.sBLOW.fLIFE, fVAR2  );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::YELLOW);
						}
						else {
							strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 0 ), COMMENT::BLOW[emBLOW].c_str() );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

							strText.Format("%s:%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 1 ),pItemData->sSuitOp.sBLOW.fLIFE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

							strText.Format("%s:%.2f%%", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 2 ),pItemData->sSuitOp.sBLOW.fRATE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

							{
								float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];

								if( fVAR1 != 0.0f )
								{
									if ( COMMENT::IsBLOW1_PER(emBLOW) )
										strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );
									else
										strText.Format("%s:%.2f", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );

									AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
								}
							}

							{
								float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];

								if( fVAR2 != 0.0f )
								{
									if ( COMMENT::IsBLOW2_PER(emBLOW) )
										strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );
									else
										strText.Format("%s:%.2f", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );

									AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
								}
							}
						}
					}


					//	NOTE
					//		°¡»ê È¿°ú
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );

					//	Æ¯¼ö±â´É
					//
					{
						BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
						BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);


						EMITEM_VAR emITEM_VAR = pItemData->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
						/*
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC )
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
						*/
						
						if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
						{
							SITEM* pJDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
							if ( pJDisguiseData )
							{
								if ( pJDisguiseData->sBasicOp.IsCostumeCombine() )
								{
									EMITEM_VAR emITEMCOS_VAR = pJDisguiseData->sSuitOp.sVARIATE.emTYPE;
									EMITEM_VAR emITEMCOS_VOL = pJDisguiseData->sSuitOp.sVOLUME.emTYPE;
								//add jitem

									float fVAR_SCALE(0);
									if ( emITEMCOS_VAR != EMVAR_NONE )
									{			
										
										fVAR_SCALE = pJDisguiseData->sSuitOp.sVARIATE.fVariate;
										CString strText2;
										fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEMCOS_VAR];
										strText2.Format("Costume ADD:%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEMCOS_VAR ), fVAR_SCALE );
										if ( COMMENT::IsITEMVAR_SCALE(emITEMCOS_VAR) )		strText2 += "%";
										AddTextNoSplit(strText2,NS_UITEXTCOLOR::DODGERBLUE);
									}

									if ( emITEMCOS_VOL != EMVAR_NONE)
									{
										CString strText3;
										float fVOLUME = pJDisguiseData->sSuitOp.sVOLUME.fVolume;
										strText3.Format("Costume ADD:%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEMCOS_VOL ), fVOLUME );
										AddTextNoSplit(strText3,NS_UITEXTCOLOR::DODGERBLUE);
									}
								}
							}
						}



						//	Æ¯¼ö±â´É ( º¯È­À² )
						//
						float fVAR_SCALE(0);
						if ( emITEM_VAR != EMVAR_NONE )
						{
							switch ( emITEM_VAR )
							{
							case EMVAR_HP:
								bVAR_HP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCHP();
								break;
							case EMVAR_MP:
								bVAR_MP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCMP();
								break;
							case EMVAR_SP:
								bVAR_SP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCSP();
								break;
							case EMVAR_AP:
								bVAR_AP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCAP();
								break;
							default:
								fVAR_SCALE = pItemData->sSuitOp.sVARIATE.fVariate;
								break;
							};

							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_HP && sItemCustom.GETINCHP() )
						{
							fVAR_SCALE = sItemCustom.GETINCHP();
							emITEM_VAR = EMVAR_HP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_MP && sItemCustom.GETINCMP() )
						{
							fVAR_SCALE = sItemCustom.GETINCMP();
							emITEM_VAR = EMVAR_MP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_SP && sItemCustom.GETINCSP() )
						{
							fVAR_SCALE = sItemCustom.GETINCSP();
							emITEM_VAR = EMVAR_SP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_AP && sItemCustom.GETINCAP() )
						{
							fVAR_SCALE = sItemCustom.GETINCAP();
							emITEM_VAR = EMVAR_AP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						// °æÇèÄ¡ ¹èÀ²
						float fExpMultiple = pItemData->GetExpMultiple();
						if( fExpMultiple != 1.0f )
						{
							strText.Format("%s %2.2f", ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ), fExpMultiple);
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						//	Æ¯¼ö±â´É ( º¯È­·® )
						//
						if ( emITEM_VOL != EMVAR_NONE)
						{
							if ( emITEM_VOL == EMVAR_MOVE_SPEED )
							{
								float fVOLUME = sItemCustom.GETMOVESPEED();
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							else
							{
								float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							
						}
					}

					// Note : ·£´ý ¼öÄ¡
					if( sItemCustom.IsSetRandOpt() )
					{
						CString strDescText;

						//AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 11 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						AddTextNoSplit( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );
						INT nRandOptType = sItemCustom.GETOptTYPE1();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::LIGHTSKYBLUE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE2();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE3();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE4();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}
					}
				}
				break;

			case ITEM_CHARM:
				{
					/*AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					//	°ø°Ý·Â
					GLPADATA &sDAMAGE = sItemCustom.GETDAMAGE();
					if ( sDAMAGE.dwLow || sDAMAGE.dwHigh  )
					{
						strText.Format("Talisman %s:%s~%s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), 
													GetNumberWithSign ( sDAMAGE.dwLow ), 
													GetNumberWithSign ( sDAMAGE.dwHigh ) );

						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}
 

					//	NOTE
					//		°¡»ê È¿°ú
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );	*/

					bSetItemList = TRUE;//add ®M¸Ë¨t²Î
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					BYTE uGRADE = 0;

					//	°ø°Ý·Â
					GLPADATA &sDamage = sItemCustom.getdamage();
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
					AddInfoItemAddonRange ( sDamage.dwLow, sDamage.dwHigh, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );

					//	±â·ÂÄ¡
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					if ( nExtraValue )
					{
						uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
						strText.Format ( "%s:+%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 8), nExtraValue );
						APPEND_ITEM_GRADE ( strText, uGRADE );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PRIVATE );
					}

					//	¹æ¾î·Â
					short nDefense = sItemCustom.getdefense();
					nExtraValue = sItemCustom.GETGRADE_DEFENSE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);
					AddInfoItemAddon ( nDefense, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

					//	°ø°Ý °Å¸®
					if ( sItemCustom.GETATTRANGE() )
					{
						strText.Format("%s:%d",ID2GAMEWORD("ITEM_ADVANCED_INFO", 2), sItemCustom.GETATTRANGE() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	¸íÁß·ü
					if ( sItemCustom.GETHITRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETHITRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );		
					}

					//	È¸ÇÇÀ²
					if ( sItemCustom.GETAVOIDRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETAVOIDRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );
					}

					//	Âø¿ëÀ§Ä¡
					strText.Format("%s:%s ", ID2GAMEWORD("ITEM_ADVANCED_INFO", 5), COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
					AddTextNoSplit (strText, NS_UITEXTCOLOR::DEFAULT );

					//	°ø°Ý ¼Ó¼º
					if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
					{
						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO", 6), COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	SP ¼Ò¸ð
					const WORD wReqSP = sItemCustom.GETREQ_SP();
					if ( 0 < wReqSP )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7), wReqSP );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::ORANGE );
					}
				
					//	Âø¿ëÁ¶°Ç-------------------------------------------------------------------------------------
					//	Âø¿ëÁ¶°Ç-Âø¿ë°¡´É Å¬·¡½º
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 2 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if ( pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL )
					{
						CString strSchool;
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_00)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_01)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_02)].c_str();
							strSchool+=" ";
						}						

						AddTextNoSplit ( strSchool, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool) ) );
					}

					if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
					{
						SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
						DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;

						if( dwReqCharClass )
						{
							//if ( (dwReqCharClass==GLCC_ALL_NEWSEX) && (dwReqCharClass_Disguise==GLCC_ALL_NEWSEX) )
							//add class
							if ( (dwReqCharClass==GLCC_ALL_2012) && (dwReqCharClass_Disguise==GLCC_ALL_2012) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( (dwReqCharClass&GLCC_FIGHTER_M) && (dwReqCharClass_Disguise&GLCC_FIGHTER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_FIGHTER_W) && (dwReqCharClass_Disguise&GLCC_FIGHTER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
									szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARMS_M)  && (dwReqCharClass_Disguise&GLCC_ARMS_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARMS_W)  && (dwReqCharClass_Disguise&GLCC_ARMS_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SPIRIT_M)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SPIRIT_W)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
									szClass+=" ";
								}
								//add class
								if ( (dwReqCharClass&GLCC_SCIENCE_M)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SCIENCE_W)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TESTING_M)  && (dwReqCharClass_Disguise&GLCC_TESTING_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TESTING_W)  && (dwReqCharClass_Disguise&GLCC_TESTING_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
								}
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor( 
								(dwReqCharClass&pCharacter->m_emClass) && (dwReqCharClass_Disguise&pCharacter->m_emClass) ) );
						}
					}
					else
					{
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;
						
						if( dwReqCharClass )
						{
							//if ( dwReqCharClass==GLCC_ALL_NEWSEX)
							//add class
							if ( dwReqCharClass==GLCC_ALL_2012)
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( dwReqCharClass & GLCC_FIGHTER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_FIGHTER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARMS_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARMS_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARCHER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARCHER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SPIRIT_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SPIRIT_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_EXTREME_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_EXTREME_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
									szClass+=" ";
								}
								//add class
								if ( dwReqCharClass & GLCC_SCIENCE_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SCIENCE_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASIN_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASIN_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TESTING_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TESTING_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
								}
								//
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
						}
					}

					//	Á¶°Ç - ¾Ï±¤
					bValue = TRUE;
					strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
					if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
					{
						if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
						{
							bValue = FALSE;
						}
					}
					AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );					

					//	Âø¿ëÁ¶°Ç->°ÝÅõÄ¡
					if ( pItemData->sBasicOp.wReqPA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqPA <= pCharacter->m_wSUM_PA ) );
					}
					//	Âø¿ëÁ¶°Ç->»ç°ÝÄ¡
					if ( pItemData->sBasicOp.wReqSA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
						AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqSA <= pCharacter->m_wSUM_SA ));
					}

					//	Stats
					//	Âø¿ëÁ¶°Ç->Èû
					if ( rItemStats.dwPow )
					{
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.dwPow );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwPow <= rCharStats.dwPow ) );
					}
					//	Âø¿ëÁ¶°Ç->Ã¼·Â
					if ( rItemStats.dwStr )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.dwStr );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwStr <= rCharStats.dwStr ) );
					}
					//	Âø¿ëÁ¶°Ç->Á¤½Å·Â
					if ( rItemStats.dwSpi)
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.dwSpi );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSpi <= rCharStats.dwSpi ) );
					}
					//	Âø¿ëÁ¶°Ç->¹ÎÃ¸¼º
					if ( rItemStats.dwDex )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.dwDex );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwDex <= rCharStats.dwDex ) );
					}
					//	Âø¿ëÁ¶°Ç->Áö·Â
					if ( rItemStats.dwInt  )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.dwInt );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwInt <= rCharStats.dwInt ) );
					}
					//	Âø¿ëÁ¶°Ç->±Ù·Â
					if ( rItemStats.dwSta )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.dwSta );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSta <= rCharStats.dwSta ) );
					}

					//	»óÅÂÀÌ»ó
					EMSTATE_BLOW emBLOW = pItemData->sSuitOp.sBLOW.emTYPE;
					if ( emBLOW !=EMBLOW_NONE )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 4 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 0 ), COMMENT::BLOW[emBLOW].c_str() );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 1 ),pItemData->sSuitOp.sBLOW.fLIFE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f%%", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 2 ),pItemData->sSuitOp.sBLOW.fRATE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						{
							float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];

							if( fVAR1 != 0.0f )
							{
								if ( COMMENT::IsBLOW1_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}

						{
							float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];

							if( fVAR2 != 0.0f )
							{
								if ( COMMENT::IsBLOW2_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}
					}


					//	NOTE
					//		°¡»ê È¿°ú
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );

					//	Æ¯¼ö±â´É
					//
					{
						BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
						BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);

						EMITEM_VAR emITEM_VAR = pItemData->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC )
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
						
						if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
						{
							SITEM* pJDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
							if ( pJDisguiseData )
							{
								if ( pJDisguiseData->sBasicOp.IsCostumeCombine() )
								{
									EMITEM_VAR emITEMCOS_VAR = pJDisguiseData->sSuitOp.sVARIATE.emTYPE;
									EMITEM_VAR emITEMCOS_VOL = pJDisguiseData->sSuitOp.sVOLUME.emTYPE;
								//add jitem

									float fVAR_SCALE(0);
									if ( emITEMCOS_VAR != EMVAR_NONE )
									{			
										
										fVAR_SCALE = pJDisguiseData->sSuitOp.sVARIATE.fVariate;
										CString strText2;
										fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEMCOS_VAR];
										strText2.Format("Costume ADD:%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEMCOS_VAR ), fVAR_SCALE );
										if ( COMMENT::IsITEMVAR_SCALE(emITEMCOS_VAR) )		strText2 += "%";
										AddTextNoSplit(strText2,NS_UITEXTCOLOR::DODGERBLUE);
									}

									if ( emITEMCOS_VOL != EMVAR_NONE)
									{
										CString strText3;
										float fVOLUME = pJDisguiseData->sSuitOp.sVOLUME.fVolume;
										strText3.Format("Costume ADD:%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEMCOS_VOL ), fVOLUME );
										AddTextNoSplit(strText3,NS_UITEXTCOLOR::DODGERBLUE);
									}
								}
							}
						}



						//	Æ¯¼ö±â´É ( º¯È­À² )
						//
						float fVAR_SCALE(0);
						if ( emITEM_VAR != EMVAR_NONE )
						{
							switch ( emITEM_VAR )
							{
							case EMVAR_HP:
								bVAR_HP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCHP();
								break;
							case EMVAR_MP:
								bVAR_MP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCMP();
								break;
							case EMVAR_SP:
								bVAR_SP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCSP();
								break;
							case EMVAR_AP:
								bVAR_AP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCAP();
								break;
							default:
								fVAR_SCALE = pItemData->sSuitOp.sVARIATE.fVariate;
								break;
							};

							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_HP && sItemCustom.GETINCHP() )
						{
							fVAR_SCALE = sItemCustom.GETINCHP();
							emITEM_VAR = EMVAR_HP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_MP && sItemCustom.GETINCMP() )
						{
							fVAR_SCALE = sItemCustom.GETINCMP();
							emITEM_VAR = EMVAR_MP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_SP && sItemCustom.GETINCSP() )
						{
							fVAR_SCALE = sItemCustom.GETINCSP();
							emITEM_VAR = EMVAR_SP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_AP && sItemCustom.GETINCAP() )
						{
							fVAR_SCALE = sItemCustom.GETINCAP();
							emITEM_VAR = EMVAR_AP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						// °æÇèÄ¡ ¹èÀ²
						float fExpMultiple = pItemData->GetExpMultiple();
						if( fExpMultiple != 1.0f )
						{
							strText.Format("%s %2.2f%s", 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ), 
												fExpMultiple, 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 1 ) );

							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						//	Æ¯¼ö±â´É ( º¯È­·® )
						//
						if ( emITEM_VOL != EMVAR_NONE)
						{
							if ( emITEM_VOL == EMVAR_MOVE_SPEED )
							{
								float fVOLUME = sItemCustom.GETMOVESPEED();
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							else
							{
								float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							
						}
					}

					// Note : ·£´ý ¼öÄ¡
					if( sItemCustom.IsSetRandOpt() )
					{
						CString strDescText;

						//AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 11 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						
						AddTextNoSplit( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );
						INT nRandOptType = sItemCustom.GETOptTYPE1();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::LIGHTSKYBLUE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE2();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE3();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE4();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}
					}
				}
				break;

			case ITEM_ARROW:
				{
					/*AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					//	°ø°Ý·Â
					GLPADATA &sDAMAGE = sItemCustom.GETDAMAGE();
					if ( sDAMAGE.dwLow || sDAMAGE.dwHigh  )
					{
						strText.Format("Arrow %s:%s~%s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), 
													GetNumberWithSign ( sDAMAGE.dwLow ), 
													GetNumberWithSign ( sDAMAGE.dwHigh ) );

						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}
 

					//	NOTE
					//		°¡»ê È¿°ú
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );	*/

					bSetItemList = TRUE;//add ®M¸Ë¨t²Î
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					BYTE uGRADE = 0;

					//	°ø°Ý·Â
					GLPADATA &sDamage = sItemCustom.getdamage();
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
					AddInfoItemAddonRange ( sDamage.dwLow, sDamage.dwHigh, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );

					//	±â·ÂÄ¡
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					if ( nExtraValue )
					{
						uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
						strText.Format ( "%s:+%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 8), nExtraValue );
						APPEND_ITEM_GRADE ( strText, uGRADE );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PRIVATE );
					}

					//	¹æ¾î·Â
					short nDefense = sItemCustom.getdefense();
					nExtraValue = sItemCustom.GETGRADE_DEFENSE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);
					AddInfoItemAddon ( nDefense, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

					//	°ø°Ý °Å¸®
					if ( sItemCustom.GETATTRANGE() )
					{
						strText.Format("%s:%d",ID2GAMEWORD("ITEM_ADVANCED_INFO", 2), sItemCustom.GETATTRANGE() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	¸íÁß·ü
					if ( sItemCustom.GETHITRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETHITRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );		
					}

					//	È¸ÇÇÀ²
					if ( sItemCustom.GETAVOIDRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETAVOIDRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );
					}

					//	Âø¿ëÀ§Ä¡
					strText.Format("%s:%s ", ID2GAMEWORD("ITEM_ADVANCED_INFO", 5), COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
					AddTextNoSplit (strText, NS_UITEXTCOLOR::DEFAULT );

					//	°ø°Ý ¼Ó¼º
					if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
					{
						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO", 6), COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	SP ¼Ò¸ð
					const WORD wReqSP = sItemCustom.GETREQ_SP();
					if ( 0 < wReqSP )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7), wReqSP );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::ORANGE );
					}
				
					//	Âø¿ëÁ¶°Ç-------------------------------------------------------------------------------------
					//	Âø¿ëÁ¶°Ç-Âø¿ë°¡´É Å¬·¡½º
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 2 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if ( pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL )
					{
						CString strSchool;
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_00)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_01)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_02)].c_str();
							strSchool+=" ";
						}						

						AddTextNoSplit ( strSchool, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool) ) );
					}

					if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
					{
						SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
						DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;

						if( dwReqCharClass )
						{
							//if ( (dwReqCharClass==GLCC_ALL_NEWSEX) && (dwReqCharClass_Disguise==GLCC_ALL_NEWSEX) )
							//add class
							if ( (dwReqCharClass==GLCC_ALL_2012) && (dwReqCharClass_Disguise==GLCC_ALL_2012) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( (dwReqCharClass&GLCC_FIGHTER_M) && (dwReqCharClass_Disguise&GLCC_FIGHTER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_FIGHTER_W) && (dwReqCharClass_Disguise&GLCC_FIGHTER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
									szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARMS_M)  && (dwReqCharClass_Disguise&GLCC_ARMS_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARMS_W)  && (dwReqCharClass_Disguise&GLCC_ARMS_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SPIRIT_M)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SPIRIT_W)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
									szClass+=" ";
								}
								//add class
								if ( (dwReqCharClass&GLCC_SCIENCE_M)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SCIENCE_W)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TESTING_M)  && (dwReqCharClass_Disguise&GLCC_TESTING_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TESTING_W)  && (dwReqCharClass_Disguise&GLCC_TESTING_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
								}
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor( 
								(dwReqCharClass&pCharacter->m_emClass) && (dwReqCharClass_Disguise&pCharacter->m_emClass) ) );
						}
					}
					else
					{
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;
						
						if( dwReqCharClass )
						{
							//if ( dwReqCharClass==GLCC_ALL_NEWSEX)
							//add class
							if ( dwReqCharClass==GLCC_ALL_2012)
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( dwReqCharClass & GLCC_FIGHTER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_FIGHTER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARMS_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARMS_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARCHER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARCHER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SPIRIT_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SPIRIT_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_EXTREME_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_EXTREME_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
									szClass+=" ";
								}
								//add class
								if ( dwReqCharClass & GLCC_SCIENCE_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SCIENCE_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASIN_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASIN_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TESTING_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TESTING_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
								}
								//
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
						}
					}

					//	Á¶°Ç - ¾Ï±¤
					bValue = TRUE;
					strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
					if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
					{
						if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
						{
							bValue = FALSE;
						}
					}
					AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );					

					//	Âø¿ëÁ¶°Ç->°ÝÅõÄ¡
					if ( pItemData->sBasicOp.wReqPA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqPA <= pCharacter->m_wSUM_PA ) );
					}
					//	Âø¿ëÁ¶°Ç->»ç°ÝÄ¡
					if ( pItemData->sBasicOp.wReqSA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
						AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqSA <= pCharacter->m_wSUM_SA ));
					}

					//	Stats
					//	Âø¿ëÁ¶°Ç->Èû
					if ( rItemStats.dwPow )
					{
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.dwPow );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwPow <= rCharStats.dwPow ) );
					}
					//	Âø¿ëÁ¶°Ç->Ã¼·Â
					if ( rItemStats.dwStr )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.dwStr );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwStr <= rCharStats.dwStr ) );
					}
					//	Âø¿ëÁ¶°Ç->Á¤½Å·Â
					if ( rItemStats.dwSpi)
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.dwSpi );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSpi <= rCharStats.dwSpi ) );
					}
					//	Âø¿ëÁ¶°Ç->¹ÎÃ¸¼º
					if ( rItemStats.dwDex )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.dwDex );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwDex <= rCharStats.dwDex ) );
					}
					//	Âø¿ëÁ¶°Ç->Áö·Â
					if ( rItemStats.dwInt  )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.dwInt );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwInt <= rCharStats.dwInt ) );
					}
					//	Âø¿ëÁ¶°Ç->±Ù·Â
					if ( rItemStats.dwSta )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.dwSta );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSta <= rCharStats.dwSta ) );
					}

					//-----------------------------------------------------------------------------------------------
					//	ÀúÇ×°ª
					const int nELEC   = sItemCustom.GETRESIST_ELEC();
					const int nFIRE   = sItemCustom.GETRESIST_FIRE();
					const int nICE    = sItemCustom.GETRESIST_ICE();
					const int nPOISON = sItemCustom.GETRESIST_POISON();
					const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();

					if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 3 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						if ( nELEC )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 0 ), nELEC );	
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nFIRE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 1 ), nFIRE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nICE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 2 ), nICE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nPOISON )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 3 ), nPOISON );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nSPIRIT )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 4 ), nSPIRIT );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
					}


					//	»óÅÂÀÌ»ó
					EMSTATE_BLOW emBLOW = pItemData->sSuitOp.sBLOW.emTYPE;
					if ( emBLOW !=EMBLOW_NONE )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 4 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 0 ), COMMENT::BLOW[emBLOW].c_str() );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 1 ),pItemData->sSuitOp.sBLOW.fLIFE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f%%", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 2 ),pItemData->sSuitOp.sBLOW.fRATE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						{
							float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];

							if( fVAR1 != 0.0f )
							{
								if ( COMMENT::IsBLOW1_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}

						{
							float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];

							if( fVAR2 != 0.0f )
							{
								if ( COMMENT::IsBLOW2_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}
					}


					//	NOTE
					//		°¡»ê È¿°ú
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );

					//	Æ¯¼ö±â´É
					//
					{
						BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
						BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);

						EMITEM_VAR emITEM_VAR = pItemData->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC )
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
						
						if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
						{
							SITEM* pJDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
							if ( pJDisguiseData )
							{
								if ( pJDisguiseData->sBasicOp.IsCostumeCombine() )
								{
									EMITEM_VAR emITEMCOS_VAR = pJDisguiseData->sSuitOp.sVARIATE.emTYPE;
									EMITEM_VAR emITEMCOS_VOL = pJDisguiseData->sSuitOp.sVOLUME.emTYPE;
								//add jitem

									float fVAR_SCALE(0);
									if ( emITEMCOS_VAR != EMVAR_NONE )
									{			
										
										fVAR_SCALE = pJDisguiseData->sSuitOp.sVARIATE.fVariate;
										CString strText2;
										fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEMCOS_VAR];
										strText2.Format("Costume ADD:%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEMCOS_VAR ), fVAR_SCALE );
										if ( COMMENT::IsITEMVAR_SCALE(emITEMCOS_VAR) )		strText2 += "%";
										AddTextNoSplit(strText2,NS_UITEXTCOLOR::DODGERBLUE);
									}

									if ( emITEMCOS_VOL != EMVAR_NONE)
									{
										CString strText3;
										float fVOLUME = pJDisguiseData->sSuitOp.sVOLUME.fVolume;
										strText3.Format("Costume ADD:%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEMCOS_VOL ), fVOLUME );
										AddTextNoSplit(strText3,NS_UITEXTCOLOR::DODGERBLUE);
									}
								}
							}
						}



						//	Æ¯¼ö±â´É ( º¯È­À² )
						//
						float fVAR_SCALE(0);
						if ( emITEM_VAR != EMVAR_NONE )
						{
							switch ( emITEM_VAR )
							{
							case EMVAR_HP:
								bVAR_HP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCHP();
								break;
							case EMVAR_MP:
								bVAR_MP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCMP();
								break;
							case EMVAR_SP:
								bVAR_SP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCSP();
								break;
							case EMVAR_AP:
								bVAR_AP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCAP();
								break;
							default:
								fVAR_SCALE = pItemData->sSuitOp.sVARIATE.fVariate;
								break;
							};

							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_HP && sItemCustom.GETINCHP() )
						{
							fVAR_SCALE = sItemCustom.GETINCHP();
							emITEM_VAR = EMVAR_HP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_MP && sItemCustom.GETINCMP() )
						{
							fVAR_SCALE = sItemCustom.GETINCMP();
							emITEM_VAR = EMVAR_MP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_SP && sItemCustom.GETINCSP() )
						{
							fVAR_SCALE = sItemCustom.GETINCSP();
							emITEM_VAR = EMVAR_SP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_AP && sItemCustom.GETINCAP() )
						{
							fVAR_SCALE = sItemCustom.GETINCAP();
							emITEM_VAR = EMVAR_AP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						// °æÇèÄ¡ ¹èÀ²
						float fExpMultiple = pItemData->GetExpMultiple();
						if( fExpMultiple != 1.0f )
						{
							strText.Format("%s %2.2f%s", 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ), 
												fExpMultiple, 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 1 ) );

							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						//	Æ¯¼ö±â´É ( º¯È­·® )
						//
						if ( emITEM_VOL != EMVAR_NONE)
						{
							if ( emITEM_VOL == EMVAR_MOVE_SPEED )
							{
								float fVOLUME = sItemCustom.GETMOVESPEED();
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							else
							{
								float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							
						}
					}

					// Note : ·£´ý ¼öÄ¡
					if( sItemCustom.IsSetRandOpt() )
					{
						CString strDescText;

						//AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 11 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						AddTextNoSplit( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );

						INT nRandOptType = sItemCustom.GETOptTYPE1();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::LIGHTSKYBLUE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE2();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE3();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE4();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}
					}
				}
				break;
				//add bullet
			case ITEM_BULLET:
				{
					/*AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					//	°ø°Ý·Â
					GLPADATA &sDAMAGE = sItemCustom.GETDAMAGE();
					if ( sDAMAGE.dwLow || sDAMAGE.dwHigh  )
					{
						strText.Format("Bullet %s:%s~%s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), 
													GetNumberWithSign ( sDAMAGE.dwLow ), 
													GetNumberWithSign ( sDAMAGE.dwHigh ) );

						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}*/
 

					//	NOTE
					//		°¡»ê È¿°ú
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );		

					bSetItemList = TRUE;//add ®M¸Ë¨t²Î
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					BYTE uGRADE = 0;

					//	°ø°Ý·Â
					GLPADATA &sDamage = sItemCustom.getdamage();
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
					AddInfoItemAddonRange ( sDamage.dwLow, sDamage.dwHigh, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );

					//	±â·ÂÄ¡
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					if ( nExtraValue )
					{
						uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
						strText.Format ( "%s:+%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 8), nExtraValue );
						APPEND_ITEM_GRADE ( strText, uGRADE );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PRIVATE );
					}

					//	¹æ¾î·Â
					short nDefense = sItemCustom.getdefense();
					nExtraValue = sItemCustom.GETGRADE_DEFENSE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);
					AddInfoItemAddon ( nDefense, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

					//	°ø°Ý °Å¸®
					if ( sItemCustom.GETATTRANGE() )
					{
						strText.Format("%s:%d",ID2GAMEWORD("ITEM_ADVANCED_INFO", 2), sItemCustom.GETATTRANGE() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	¸íÁß·ü
					if ( sItemCustom.GETHITRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETHITRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );		
					}

					//	È¸ÇÇÀ²
					if ( sItemCustom.GETAVOIDRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETAVOIDRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );
					}

					//	Âø¿ëÀ§Ä¡
					strText.Format("%s:%s ", ID2GAMEWORD("ITEM_ADVANCED_INFO", 5), COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
					AddTextNoSplit (strText, NS_UITEXTCOLOR::DEFAULT );

					//	°ø°Ý ¼Ó¼º
					if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
					{
						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO", 6), COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	SP ¼Ò¸ð
					const WORD wReqSP = sItemCustom.GETREQ_SP();
					if ( 0 < wReqSP )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7), wReqSP );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::ORANGE );
					}
				
					//	Âø¿ëÁ¶°Ç-------------------------------------------------------------------------------------
					//	Âø¿ëÁ¶°Ç-Âø¿ë°¡´É Å¬·¡½º
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 2 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if ( pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL )
					{
						CString strSchool;
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_00)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_01)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_02)].c_str();
							strSchool+=" ";
						}						

						AddTextNoSplit ( strSchool, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool) ) );
					}

					if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
					{
						SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
						DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;

						if( dwReqCharClass )
						{
							//if ( (dwReqCharClass==GLCC_ALL_NEWSEX) && (dwReqCharClass_Disguise==GLCC_ALL_NEWSEX) )
							//add class
							if ( (dwReqCharClass==GLCC_ALL_2012) && (dwReqCharClass_Disguise==GLCC_ALL_2012) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( (dwReqCharClass&GLCC_FIGHTER_M) && (dwReqCharClass_Disguise&GLCC_FIGHTER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_FIGHTER_W) && (dwReqCharClass_Disguise&GLCC_FIGHTER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
									szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARMS_M)  && (dwReqCharClass_Disguise&GLCC_ARMS_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARMS_W)  && (dwReqCharClass_Disguise&GLCC_ARMS_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SPIRIT_M)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SPIRIT_W)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
									szClass+=" ";
								}
								//add class
								if ( (dwReqCharClass&GLCC_SCIENCE_M)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SCIENCE_W)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TESTING_M)  && (dwReqCharClass_Disguise&GLCC_TESTING_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TESTING_W)  && (dwReqCharClass_Disguise&GLCC_TESTING_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
								}
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor( 
								(dwReqCharClass&pCharacter->m_emClass) && (dwReqCharClass_Disguise&pCharacter->m_emClass) ) );
						}
					}
					else
					{
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;
						
						if( dwReqCharClass )
						{
							//if ( dwReqCharClass==GLCC_ALL_NEWSEX)
							//add class
							if ( dwReqCharClass==GLCC_ALL_2012)
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( dwReqCharClass & GLCC_FIGHTER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_FIGHTER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARMS_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARMS_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARCHER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARCHER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SPIRIT_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SPIRIT_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_EXTREME_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_EXTREME_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
									szClass+=" ";
								}
								//add class
								if ( dwReqCharClass & GLCC_SCIENCE_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SCIENCE_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASIN_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASIN_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TESTING_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TESTING_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
								}
								//
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
						}
					}

					//	Á¶°Ç - ¾Ï±¤
					bValue = TRUE;
					strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
					if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
					{
						if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
						{
							bValue = FALSE;
						}
					}
					AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );					

					//	Âø¿ëÁ¶°Ç->°ÝÅõÄ¡
					if ( pItemData->sBasicOp.wReqPA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqPA <= pCharacter->m_wSUM_PA ) );
					}
					//	Âø¿ëÁ¶°Ç->»ç°ÝÄ¡
					if ( pItemData->sBasicOp.wReqSA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
						AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqSA <= pCharacter->m_wSUM_SA ));
					}

					//	Stats
					//	Âø¿ëÁ¶°Ç->Èû
					if ( rItemStats.dwPow )
					{
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.dwPow );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwPow <= rCharStats.dwPow ) );
					}
					//	Âø¿ëÁ¶°Ç->Ã¼·Â
					if ( rItemStats.dwStr )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.dwStr );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwStr <= rCharStats.dwStr ) );
					}
					//	Âø¿ëÁ¶°Ç->Á¤½Å·Â
					if ( rItemStats.dwSpi)
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.dwSpi );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSpi <= rCharStats.dwSpi ) );
					}
					//	Âø¿ëÁ¶°Ç->¹ÎÃ¸¼º
					if ( rItemStats.dwDex )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.dwDex );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwDex <= rCharStats.dwDex ) );
					}
					//	Âø¿ëÁ¶°Ç->Áö·Â
					if ( rItemStats.dwInt  )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.dwInt );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwInt <= rCharStats.dwInt ) );
					}
					//	Âø¿ëÁ¶°Ç->±Ù·Â
					if ( rItemStats.dwSta )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.dwSta );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSta <= rCharStats.dwSta ) );
					}

					//-----------------------------------------------------------------------------------------------
					//	ÀúÇ×°ª
					const int nELEC   = sItemCustom.GETRESIST_ELEC();
					const int nFIRE   = sItemCustom.GETRESIST_FIRE();
					const int nICE    = sItemCustom.GETRESIST_ICE();
					const int nPOISON = sItemCustom.GETRESIST_POISON();
					const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();

					if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 3 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						if ( nELEC )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 0 ), nELEC );	
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nFIRE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 1 ), nFIRE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nICE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 2 ), nICE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nPOISON )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 3 ), nPOISON );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nSPIRIT )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 4 ), nSPIRIT );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
					}


					//	»óÅÂÀÌ»ó
					EMSTATE_BLOW emBLOW = pItemData->sSuitOp.sBLOW.emTYPE;
					if ( emBLOW !=EMBLOW_NONE )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 4 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 0 ), COMMENT::BLOW[emBLOW].c_str() );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 1 ),pItemData->sSuitOp.sBLOW.fLIFE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f%%", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 2 ),pItemData->sSuitOp.sBLOW.fRATE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						{
							float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];

							if( fVAR1 != 0.0f )
							{
								if ( COMMENT::IsBLOW1_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}

						{
							float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];

							if( fVAR2 != 0.0f )
							{
								if ( COMMENT::IsBLOW2_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}
					}


					//	NOTE
					//		°¡»ê È¿°ú
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );

					//	Æ¯¼ö±â´É
					//
					{
						BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
						BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);

						EMITEM_VAR emITEM_VAR = pItemData->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC )
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
						
						if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
						{
							SITEM* pJDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
							if ( pJDisguiseData )
							{
								if ( pJDisguiseData->sBasicOp.IsCostumeCombine() )
								{
									EMITEM_VAR emITEMCOS_VAR = pJDisguiseData->sSuitOp.sVARIATE.emTYPE;
									EMITEM_VAR emITEMCOS_VOL = pJDisguiseData->sSuitOp.sVOLUME.emTYPE;
								//add jitem

									float fVAR_SCALE(0);
									if ( emITEMCOS_VAR != EMVAR_NONE )
									{			
										
										fVAR_SCALE = pJDisguiseData->sSuitOp.sVARIATE.fVariate;
										CString strText2;
										fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEMCOS_VAR];
										strText2.Format("Costume ADD:%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEMCOS_VAR ), fVAR_SCALE );
										if ( COMMENT::IsITEMVAR_SCALE(emITEMCOS_VAR) )		strText2 += "%";
										AddTextNoSplit(strText2,NS_UITEXTCOLOR::DODGERBLUE);
									}

									if ( emITEMCOS_VOL != EMVAR_NONE)
									{
										CString strText3;
										float fVOLUME = pJDisguiseData->sSuitOp.sVOLUME.fVolume;
										strText3.Format("Costume ADD:%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEMCOS_VOL ), fVOLUME );
										AddTextNoSplit(strText3,NS_UITEXTCOLOR::DODGERBLUE);
									}
								}
							}
						}



						//	Æ¯¼ö±â´É ( º¯È­À² )
						//
						float fVAR_SCALE(0);
						if ( emITEM_VAR != EMVAR_NONE )
						{
							switch ( emITEM_VAR )
							{
							case EMVAR_HP:
								bVAR_HP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCHP();
								break;
							case EMVAR_MP:
								bVAR_MP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCMP();
								break;
							case EMVAR_SP:
								bVAR_SP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCSP();
								break;
							case EMVAR_AP:
								bVAR_AP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCAP();
								break;
							default:
								fVAR_SCALE = pItemData->sSuitOp.sVARIATE.fVariate;
								break;
							};

							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_HP && sItemCustom.GETINCHP() )
						{
							fVAR_SCALE = sItemCustom.GETINCHP();
							emITEM_VAR = EMVAR_HP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_MP && sItemCustom.GETINCMP() )
						{
							fVAR_SCALE = sItemCustom.GETINCMP();
							emITEM_VAR = EMVAR_MP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_SP && sItemCustom.GETINCSP() )
						{
							fVAR_SCALE = sItemCustom.GETINCSP();
							emITEM_VAR = EMVAR_SP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_AP && sItemCustom.GETINCAP() )
						{
							fVAR_SCALE = sItemCustom.GETINCAP();
							emITEM_VAR = EMVAR_AP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						// °æÇèÄ¡ ¹èÀ²
						float fExpMultiple = pItemData->GetExpMultiple();
						if( fExpMultiple != 1.0f )
						{
							strText.Format("%s %2.2f%s", 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ), 
												fExpMultiple, 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 1 ) );

							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						//	Æ¯¼ö±â´É ( º¯È­·® )
						//
						if ( emITEM_VOL != EMVAR_NONE)
						{
							if ( emITEM_VOL == EMVAR_MOVE_SPEED )
							{
								float fVOLUME = sItemCustom.GETMOVESPEED();
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							else
							{
								float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							
						}
					}

					// Note : ·£´ý ¼öÄ¡
					if( sItemCustom.IsSetRandOpt() )
					{
						CString strDescText;

						//AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 11 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						AddTextNoSplit( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );

						INT nRandOptType = sItemCustom.GETOptTYPE1();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::LIGHTSKYBLUE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE2();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE3();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE4();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}
					}
				}
				break;

			case ITEM_CURE:
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 7 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if( pItemData->sDrugOp.wCureVolume == 0 )
					{
						strText.Format("%s",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str());
					}
					else
					{
						strText.Format("%s:%d",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str(), pItemData->sDrugOp.wCureVolume);
					}

					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT);
				}
				break;

			case ITEM_SKILL:
				{
					AddInfoSkillItem ( sItemCustom );
				}
				break;

			case ITEM_PET_SKILL:
				{
					AddInfoPetSkillItem( sItemCustom );
				}
				break;

			case ITEM_GRINDING:
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY_EX", 0 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
				}
				break;

				// °æÇèÄ¡ È¹µæ Ä«µå Á¤º¸
			case ITEM_VIETNAM_EXPGET:
				{
					if ( pItemData->sDrugOp.bRatio )
					{
						strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 1) , pItemData->sDrugOp.wCureVolume );						
					}
					else
					{
						strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 0), pItemData->sDrugOp.wCureVolume );
					}

					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}
				break;
				// ¾ÆÀÌÅÛ È¹µæ Ä«µå Á¤º¸
			case ITEM_VIETNAM_ITEMGET:
				{
					strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_ITEM", 0) , pItemData->sDrugOp.wCureVolume );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}
				break;

			case ITEM_VEHICLE:
				{
					//bSetItemList = TRUE;//add ®M¸Ë¨t²Î
					strText.Format(ID2GAMEWORD ("ITEM_VEHICLE_TYPE_INFO", 0) , COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str() );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);

					strText.Format( "%s %s", ID2GAMEWORD ("ITEM_VEHICLE_NAME",0), COMMENT::VEHICLE_TYPE[pItemData->sVehicle.emVehicleType].c_str() );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);			

					bSetItemList = TRUE;//add ®M¸Ë¨t²Î
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					BYTE uGRADE = 0;

					//	°ø°Ý·Â
					GLPADATA &sDamage = sItemCustom.getdamage();
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
					AddInfoItemAddonRange ( sDamage.dwLow, sDamage.dwHigh, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );

					//	±â·ÂÄ¡
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					if ( nExtraValue )
					{
						uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
						strText.Format ( "%s:+%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 8), nExtraValue );
						APPEND_ITEM_GRADE ( strText, uGRADE );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PRIVATE );
					}

					//	¹æ¾î·Â
					short nDefense = sItemCustom.getdefense();
					nExtraValue = sItemCustom.GETGRADE_DEFENSE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);
					AddInfoItemAddon ( nDefense, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

					//	°ø°Ý °Å¸®
					if ( sItemCustom.GETATTRANGE() )
					{
						strText.Format("%s:%d",ID2GAMEWORD("ITEM_ADVANCED_INFO", 2), sItemCustom.GETATTRANGE() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	¸íÁß·ü
					if ( sItemCustom.GETHITRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETHITRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );		
					}

					//	È¸ÇÇÀ²
					if ( sItemCustom.GETAVOIDRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETAVOIDRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );
					}

					//	Âø¿ëÀ§Ä¡
					strText.Format("%s:%s ", ID2GAMEWORD("ITEM_ADVANCED_INFO", 5), COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
					AddTextNoSplit (strText, NS_UITEXTCOLOR::DEFAULT );

					//	°ø°Ý ¼Ó¼º
					if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
					{
						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO", 6), COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	SP ¼Ò¸ð
					const WORD wReqSP = sItemCustom.GETREQ_SP();
					if ( 0 < wReqSP )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7), wReqSP );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::ORANGE );
					}

					bool bInfo(true);

					
					VEHICLEITEMINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.find ( sItemCustom.dwVehicleID );
					if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.end() )
					{
						iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.find ( sItemCustom.dwVehicleID );
						if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.end() )	bInfo = false;
					}


					if ( bInfo ) 
					{
						SVEHICLEITEMINFO sVehicleItemInfo = (*iter).second;
						for ( int i = 0; i < VEHICLE_TYPE_SIZE; ++i ) 
						{
							SITEM* pItem = GLItemMan::GetInstance().GetItem ( sVehicleItemInfo.m_PutOnItems[i].sNativeID );
							if ( pItem )
							{
								strText.Format("%s %s", ID2GAMEWORD ("ITEM_VEHICLE_SLOT",i), pItem->GetName () );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
							}
						}

						strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_VEHICLE_BATTERY",0), sVehicleItemInfo.m_nFull/10.0f );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}

					/*EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
					//	ÀÌµ¿¼Óµµ º¯È­·®
					//
					if ( emITEM_VOL != EMVAR_NONE)
					{
						if ( emITEM_VOL == EMVAR_MOVE_SPEED )
						{
							float fVOLUME = sItemCustom.GETMOVESPEED();
							strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						else
						{
							float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
							strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
					}*/
				
					//	Âø¿ëÁ¶°Ç-------------------------------------------------------------------------------------
					//	Âø¿ëÁ¶°Ç-Âø¿ë°¡´É Å¬·¡½º
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 2 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if ( pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL )
					{
						CString strSchool;
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_00)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_01)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_02)].c_str();
							strSchool+=" ";
						}						

						AddTextNoSplit ( strSchool, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool) ) );
					}

					if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
					{
						SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
						DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;

						if( dwReqCharClass )
						{
							//if ( (dwReqCharClass==GLCC_ALL_NEWSEX) && (dwReqCharClass_Disguise==GLCC_ALL_NEWSEX) )
							//add class
							if ( (dwReqCharClass==GLCC_ALL_2012) && (dwReqCharClass_Disguise==GLCC_ALL_2012) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( (dwReqCharClass&GLCC_FIGHTER_M) && (dwReqCharClass_Disguise&GLCC_FIGHTER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_FIGHTER_W) && (dwReqCharClass_Disguise&GLCC_FIGHTER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
									szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARMS_M)  && (dwReqCharClass_Disguise&GLCC_ARMS_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARMS_W)  && (dwReqCharClass_Disguise&GLCC_ARMS_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SPIRIT_M)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SPIRIT_W)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
									szClass+=" ";
								}
								//add class
								if ( (dwReqCharClass&GLCC_SCIENCE_M)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_SCIENCE_W)  && (dwReqCharClass_Disguise&GLCC_SCIENCE_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASIN_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TESTING_M)  && (dwReqCharClass_Disguise&GLCC_TESTING_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TESTING_W)  && (dwReqCharClass_Disguise&GLCC_TESTING_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
								}
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor( 
								(dwReqCharClass&pCharacter->m_emClass) && (dwReqCharClass_Disguise&pCharacter->m_emClass) ) );
						}
					}
					else
					{
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;
						
						if( dwReqCharClass )
						{
							//if ( dwReqCharClass==GLCC_ALL_NEWSEX)
							//add class
							if ( dwReqCharClass==GLCC_ALL_2012)
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( dwReqCharClass & GLCC_FIGHTER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_FIGHTER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARMS_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARMS_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARCHER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ARCHER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SPIRIT_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SPIRIT_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_EXTREME_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_EXTREME_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
									szClass+=" ";
								}
								//add class
								if ( dwReqCharClass & GLCC_SCIENCE_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_SCIENCE_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASIN_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASIN_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TESTING_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TESTING_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str();
								}
								//
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
						}
					}

					//	Á¶°Ç - ¾Ï±¤
					bValue = TRUE;
					strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
					if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
					{
						if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
						{
							bValue = FALSE;
						}
					}
					AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );					

					//	Âø¿ëÁ¶°Ç->°ÝÅõÄ¡
					if ( pItemData->sBasicOp.wReqPA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqPA <= pCharacter->m_wSUM_PA ) );
					}
					//	Âø¿ëÁ¶°Ç->»ç°ÝÄ¡
					if ( pItemData->sBasicOp.wReqSA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
						AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqSA <= pCharacter->m_wSUM_SA ));
					}

					//	Stats
					//	Âø¿ëÁ¶°Ç->Èû
					if ( rItemStats.dwPow )
					{
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.dwPow );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwPow <= rCharStats.dwPow ) );
					}
					//	Âø¿ëÁ¶°Ç->Ã¼·Â
					if ( rItemStats.dwStr )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.dwStr );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwStr <= rCharStats.dwStr ) );
					}
					//	Âø¿ëÁ¶°Ç->Á¤½Å·Â
					if ( rItemStats.dwSpi)
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.dwSpi );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSpi <= rCharStats.dwSpi ) );
					}
					//	Âø¿ëÁ¶°Ç->¹ÎÃ¸¼º
					if ( rItemStats.dwDex )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.dwDex );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwDex <= rCharStats.dwDex ) );
					}
					//	Âø¿ëÁ¶°Ç->Áö·Â
					if ( rItemStats.dwInt  )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.dwInt );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwInt <= rCharStats.dwInt ) );
					}
					//	Âø¿ëÁ¶°Ç->±Ù·Â
					if ( rItemStats.dwSta )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.dwSta );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSta <= rCharStats.dwSta ) );
					}

					//-----------------------------------------------------------------------------------------------
					//	ÀúÇ×°ª
					const int nELEC   = sItemCustom.GETRESIST_ELEC();
					const int nFIRE   = sItemCustom.GETRESIST_FIRE();
					const int nICE    = sItemCustom.GETRESIST_ICE();
					const int nPOISON = sItemCustom.GETRESIST_POISON();
					const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();

					if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 3 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						if ( nELEC )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 0 ), nELEC );	
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nFIRE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 1 ), nFIRE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nICE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 2 ), nICE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nPOISON )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 3 ), nPOISON );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nSPIRIT )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 4 ), nSPIRIT );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
					}


					//	»óÅÂÀÌ»ó
					EMSTATE_BLOW emBLOW = pItemData->sSuitOp.sBLOW.emTYPE;
					if ( emBLOW !=EMBLOW_NONE )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 4 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 0 ), COMMENT::BLOW[emBLOW].c_str() );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 1 ),pItemData->sSuitOp.sBLOW.fLIFE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f%%", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 2 ),pItemData->sSuitOp.sBLOW.fRATE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						{
							float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];

							if( fVAR1 != 0.0f )
							{
								if ( COMMENT::IsBLOW1_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}

						{
							float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];

							if( fVAR2 != 0.0f )
							{
								if ( COMMENT::IsBLOW2_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}
					}


					//	NOTE
					//		°¡»ê È¿°ú
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );

					//	Æ¯¼ö±â´É
					//
					{
						BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
						BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);

						EMITEM_VAR emITEM_VAR = pItemData->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC )
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
						
						if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
						{
							SITEM* pJDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
							if ( pJDisguiseData )
							{
								if ( pJDisguiseData->sBasicOp.IsCostumeCombine() )
								{
									EMITEM_VAR emITEMCOS_VAR = pJDisguiseData->sSuitOp.sVARIATE.emTYPE;
									EMITEM_VAR emITEMCOS_VOL = pJDisguiseData->sSuitOp.sVOLUME.emTYPE;
								//add jitem

									float fVAR_SCALE(0);
									if ( emITEMCOS_VAR != EMVAR_NONE )
									{			
										
										fVAR_SCALE = pJDisguiseData->sSuitOp.sVARIATE.fVariate;
										CString strText2;
										fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEMCOS_VAR];
										strText2.Format("Costume ADD:%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEMCOS_VAR ), fVAR_SCALE );
										if ( COMMENT::IsITEMVAR_SCALE(emITEMCOS_VAR) )		strText2 += "%";
										AddTextNoSplit(strText2,NS_UITEXTCOLOR::DODGERBLUE);
									}

									if ( emITEMCOS_VOL != EMVAR_NONE)
									{
										CString strText3;
										float fVOLUME = pJDisguiseData->sSuitOp.sVOLUME.fVolume;
										strText3.Format("Costume ADD:%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEMCOS_VOL ), fVOLUME );
										AddTextNoSplit(strText3,NS_UITEXTCOLOR::DODGERBLUE);
									}
								}
							}
						}



						//	Æ¯¼ö±â´É ( º¯È­À² )
						//
						float fVAR_SCALE(0);
						if ( emITEM_VAR != EMVAR_NONE )
						{
							switch ( emITEM_VAR )
							{
							case EMVAR_HP:
								bVAR_HP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCHP();
								break;
							case EMVAR_MP:
								bVAR_MP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCMP();
								break;
							case EMVAR_SP:
								bVAR_SP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCSP();
								break;
							case EMVAR_AP:
								bVAR_AP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCAP();
								break;
							default:
								fVAR_SCALE = pItemData->sSuitOp.sVARIATE.fVariate;
								break;
							};

							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_HP && sItemCustom.GETINCHP() )
						{
							fVAR_SCALE = sItemCustom.GETINCHP();
							emITEM_VAR = EMVAR_HP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_MP && sItemCustom.GETINCMP() )
						{
							fVAR_SCALE = sItemCustom.GETINCMP();
							emITEM_VAR = EMVAR_MP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_SP && sItemCustom.GETINCSP() )
						{
							fVAR_SCALE = sItemCustom.GETINCSP();
							emITEM_VAR = EMVAR_SP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						if ( !bVAR_AP && sItemCustom.GETINCAP() )
						{
							fVAR_SCALE = sItemCustom.GETINCAP();
							emITEM_VAR = EMVAR_AP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						// °æÇèÄ¡ ¹èÀ²
						float fExpMultiple = pItemData->GetExpMultiple();
						if( fExpMultiple != 1.0f )
						{
							strText.Format("%s %2.2f%s", 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ), 
												fExpMultiple, 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 1 ) );

							AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
						}

						//	Æ¯¼ö±â´É ( º¯È­·® )
						//
						if ( emITEM_VOL != EMVAR_NONE)
						{
							if ( emITEM_VOL == EMVAR_MOVE_SPEED )
							{
								float fVOLUME = sItemCustom.GETMOVESPEED();
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							else
							{
								float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DODGERBLUE);
							}
							
						}
					}

					// Note : ·£´ý ¼öÄ¡
					if( sItemCustom.IsSetRandOpt() )
					{
						CString strDescText;

						//AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 11 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						AddTextNoSplit( " ", NS_UITEXTCOLOR::LIGHTSKYBLUE );

						INT nRandOptType = sItemCustom.GETOptTYPE1();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::LIGHTSKYBLUE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE2();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE3();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE4();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
							}
						}
					}
				}
				//break;
				break;

			case ITEM_PET_CARD:
				{
					if ( sItemCustom.dwPetID == 0 ) break;
					PETCARDINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfo.find ( sItemCustom.dwPetID );
					if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfo.end() )
					{
						iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfoTemp.find ( sItemCustom.dwPetID );
						if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfoTemp.end() ) break;
					}

					SPETCARDINFO sPetCardInfo = (*iter).second;

					strText.Format(ID2GAMEWORD ("ITEM_PET_INFO", 0 ));
					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);

					strText.Format( "%s %s", ID2GAMEWORD ("ITEM_PET_NAME",0), sPetCardInfo.m_szName );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);

					strText.Format( "%s %s", ID2GAMEWORD ("ITEM_PET_TYPE",0), COMMENT::PET_TYPE[sPetCardInfo.m_emTYPE].c_str() );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);

					strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_PET_FULL",0), sPetCardInfo.m_nFull/10.0f );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sPetCardInfo.m_PutOnItems[PET_ACCETYPE_A].sNativeID );
					if ( pItem )
					{
						strText.Format("%s %s", ID2GAMEWORD ("ITEM_PET_SLOTA",0), pItem->GetName () );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}

					if ( sItemCustom.tDISGUISE != 0 )
					{
						CTime currentTime = GLGaeaClient::GetInstance().GetCurrentTime();
						CTime startTime   = sItemCustom.tBORNTIME;
						CTimeSpan timeSpan = currentTime - startTime;
						if( timeSpan.GetTotalSeconds() < sItemCustom.tDISGUISE )
						{
							strText.Format("[%s]", ID2GAMEWORD ("ITEM_PET_USE_SKINPACK",0) );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GOLD);
							strText.Format("%s : %dsec", ID2GAMEWORD ("CLUB_BATTLE_TIME",0), sItemCustom.tDISGUISE - timeSpan.GetTotalSeconds() );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GOLD);
						}
					}

					AddTextNoSplit( ID2GAMEWORD ("ITEM_PET_SKILLS", 0 ), NS_UITEXTCOLOR::DEFAULT );

					PETSKILL_MAP_CITER pos = sPetCardInfo.m_ExpSkills.begin();
					PETSKILL_MAP_CITER pos_end = sPetCardInfo.m_ExpSkills.end();
					for ( ;pos != pos_end; ++pos )
					{
						const PETSKILL& sPetSkill = (*pos).second;
						PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( sPetSkill.sNativeID );
						if ( pSKILL )
						{
							strText.Format("%s", pSKILL->GetName() );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
						}
					}
				}
				break;

				/////////
				case ITEM_SUMMON_CARD:
				{

					if ( pItemData->sBasicOp.sSumID!=SNATIVEID(false) )
					{
						SCROWDATA* pCrow =GLCrowDataMan::GetInstance().GetCrowData ( pItemData->sBasicOp.sSumID );

						if (pCrow)
						{
							{
								strText.Format("%s %s ", ID2GAMEWORD ("ITEM_SUMMON_INFO",0), pCrow->GetName () );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
							}

							{
								strText.Format("%s %s ", ID2GAMEWORD ("ITEM_SUMMON_INFO",1),COMMENT::SUMMONTYPE[pItemData->sBasicOp.m_emSumType].c_str() );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
							}

							{
								strText.Format("%s %d", ID2GAMEWORD ("ITEM_SUMMON_INFO",2), pCrow->m_sBasic.m_dwHP );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
							}

							{
									strText.Format("%s %d ~ %d"
									, ID2GAMEWORD ("ITEM_SUMMON_INFO",3)
									, pCrow->m_sCrowAttack[0].sDamage.dwLow
									, pCrow->m_sCrowAttack[0].sDamage.dwHigh
									);
								AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
							}
						}
					}

				}
				break;
				///////////
			}
			
			if( emItemType != ITEM_PET_SKILL )
			{
				//AddTextNoSplit( ID2GAMEWORD( "ITEM_CATEGORY", 8 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

				// Note : ½Ã±Í/Á÷±Í Ä«µå¿¡ ¸ÊÀÌ¸§ ÁÂÇ¥ Ãâ·Â
				if( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_REGEN )
				{
					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();	

					CString strName( GLGaeaClient::GetInstance().GetMapName( pCharacter->m_sStartMapID ) );
					if( strName == _T("(null)") ) strName.Empty();

					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_CALL_MAP", 0 ), strName );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

					SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pCharacter->m_sStartMapID );
					if( pMapNode )
					{
						GLLevelFile cLevelFile;
						BOOL bOk = cLevelFile.LoadFile( pMapNode->strFile.c_str(), TRUE, NULL );
						if( bOk )
						{	
							D3DXVECTOR3 vStartPos;
							PDXLANDGATE pGate = cLevelFile.GetLandGateMan().FindLandGate( pCharacter->m_dwStartGate );
							if( pGate )
							{
								vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
							}
							else
							{
								pGate = cLevelFile.GetLandGateMan().FindLandGate( DWORD(0) );
								if( pGate ) vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
							}

							int nPosX(0), nPosY(0);
							cLevelFile.GetMapAxisInfo().Convert2MapPos( vStartPos.x, vStartPos.z, nPosX, nPosY );

							strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1 ), nPosX, nPosY );
							AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
						}
					}
				}
				else if( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_LASTCALL )
				{
					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();

					if( pCharacter->m_sLastCallMapID.IsValidNativeID() )
					{
						CString strName( GLGaeaClient::GetInstance().GetMapName( pCharacter->m_sLastCallMapID ) );
						if( strName == _T("(null)") ) strName.Empty();

						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_CALL_MAP", 0 ), strName );
						AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

						SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pCharacter->m_sLastCallMapID );
						if( pMapNode )
						{
							GLLevelFile cLevelFile;
							BOOL bOk = cLevelFile.LoadFile( pMapNode->strFile.c_str(), TRUE, NULL );
							if( bOk )
							{	
								int nPosX(0), nPosY(0);
								cLevelFile.GetMapAxisInfo().Convert2MapPos( pCharacter->m_vLastCallPos.x, pCharacter->m_vLastCallPos.z, nPosX, nPosY );

								strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1 ), nPosX, nPosY );
								AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
							}
						}
					}
					else
					{
						AddTextNoSplit( ID2GAMEWORD("ITEM_CALL_MAP", 2 ), NS_UITEXTCOLOR::RED);
					}
				}
				else if ( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_TELEPORT )
				{
					CString strName( GLGaeaClient::GetInstance().GetMapName( pItemData->sBasicOp.sSubID ) );
					if( strName == _T("(null)") ) strName.Empty();

					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_TELEPORT_MAP", 0 ), strName );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

					strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_TELEPORT_MAP", 1 ), pItemData->sBasicOp.wPosX, pItemData->sBasicOp.wPosY );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
				}
			}
			{ // ¼³¸í
				const TCHAR * pszComment(NULL);

				if ( (emItemType != ITEM_SKILL) || (emItemType != ITEM_PET_SKILL) )
				{
					pszComment = pItemData->GetComment();
				}
				else
				{
					SNATIVEID sNativeID = sItemCustom.sNativeID;
					SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
					SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

					//	Note : ½ºÅ³ Á¤º¸ °¡Á®¿È.
					PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
					if ( pSkill )
					{
						pszComment = pSkill->GetDesc();
					}
				}

				if( pszComment )
				{
					int StrLength = static_cast<int>( strlen( pszComment ) );
					if( StrLength )
					{
						AddTextNoSplit ( ID2GAMEWORD( "ITEM_CATEGORY", 9 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						AddTextLongestLineSplit( pszComment, NS_UITEXTCOLOR::DEFAULT );
					}
				}
			}

			//add money 2
			{
				DWORD dwFlags = pItemData->sBasicOp.dwFlags;
				DWORD dwFlags2 = pItemData->sBasicOp.dwFlags;
				DWORD dwFlags3 = pItemData->sBasicOp.dwFlags;
				DWORD dwFlags4 = pItemData->sBasicOp.dwFlags;
				//DWORD dwFlagsetc = pItemData->sBasicOp.dwFlags;
				CString szFlag1, szFlag2, szFlag3, szFlag4/*, szFlagetc*/;
				
				if( dwFlags )
					{				
						if ( dwFlags & TRADE_SALE )
						{
							szFlag1+=( "Shop Trade Possible" );
							szFlag1+=" ";
							strText.Format( "%s", szFlag1.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );
						}
						else
						{
							szFlag1+=( "Shop Trade Impossible" );
							szFlag1+=" ";
							strText.Format( "%s", szFlag1.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKGRAY );
						}
					}
				
				if ( dwFlags2 )
				{
					if ( ( ( dwFlags & TRADE_THROW ) || ( dwFlags & TRADE_GARBAGE ) ) ||
					   ( ( dwFlags & TRADE_THROW ) && ( dwFlags & TRADE_GARBAGE ) ) )
					{
						szFlag2+=( "Discard Item Possible" );
						szFlag2+=" ";
						strText.Format( "%s", szFlag2.GetString() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );
					}
					else{
						szFlag2+=( "Discard Item Impossible" );
						szFlag2+=" ";
						strText.Format( "%s", szFlag2.GetString() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKGRAY );
					}
				}
				
				if( dwFlags3 )
				{	
					if ( ( ( dwFlags & TRADE_EXCHANGE ) || ( dwFlags & TRADE_LOCKER ) ) ||
					   ( ( dwFlags & TRADE_EXCHANGE ) && ( dwFlags & TRADE_LOCKER ) ) )
					{
						szFlag3+=( "User Trade Possible" );
						szFlag3+=" ";
						strText.Format( "%s", szFlag3.GetString() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );
					}
					else{
						szFlag3+=( "User Trade Impossible" );
						szFlag3+=" ";
						strText.Format( "%s", szFlag3.GetString() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKGRAY );
					}
				}	
				
				if ( dwFlags4 )
				{
					if ( ( ( dwFlags & ITEM_POINT_RECOVER ) || ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) || ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) ) ||
					   ( ( dwFlags & ITEM_POINT_RECOVER ) && ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) && ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) ) )
					{
						szFlag4+=( "VIP Item" );
						szFlag4+=" ";
						strText.Format( "%s", szFlag4.GetString() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );
					}
					else{
						szFlag4+=( "Common Item" );
						szFlag4+=" ";
						strText.Format( "%s", szFlag4.GetString() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKGRAY );
					}
				}
			}

			{
				BOOL bAccept(FALSE);

				//AddTextNoSplit ( ID2GAMEWORD( "ITEM_CATEGORY", 8 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

				if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
				{
					bAccept = pCharacter->ACCEPT_ITEM( sItemCustom.sNativeID, sItemCustom.nidDISGUISE );
				}
				else
				{
					bAccept = pCharacter->ACCEPT_ITEM( sItemCustom.sNativeID );
				}


				if ( bAccept )
				{
					strText.Format ( "[%s]", ID2GAMEWORD("ITEM_USABLE", 0 ) );
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::ENABLE );

					DWORD dwNeedSP = pCharacter->CALC_ACCEPTP ( sItemCustom.sNativeID );
					if ( 0 < dwNeedSP )
					{
						strText.Format ( "%s:%d", ID2GAMEWORD("ITEM_NEEDSP" ), dwNeedSP );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::PALEGREEN );
					}
				}
				else
				{
					strText.Format ( "[%s]", ID2GAMEWORD("ITEM_USABLE", 1 ) );
					//AddTextNoSplit(strText, NS_UITEXTCOLOR::RED );
				}
			}
			//add ownership
			{

				DWORD dwOwner = sItemCustom.dwOwnerID;
				DWORD dwOwnerID =GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;

				if ( pItemData->sBasicOp.IsOwner() )
				{
					if (pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
					{
						strText.Format( "Vote Item Owner ID: %d", dwOwner );
						if ( !( dwOwnerID == dwOwner))
						{
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::RED );
						}
						else
						{
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
					}
					//
					if (pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
					{
						strText.Format( "Premium Item Owner ID: %d", dwOwner );
						if ( !( dwOwnerID == dwOwner))
						{
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::RED );
						}
						else
						{
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
					}
				}
			}
		}
		{
			const int nELEC   = sItemCustom.GETRESIST_ELEC();
			const int nFIRE   = sItemCustom.GETRESIST_FIRE();
			const int nICE    = sItemCustom.GETRESIST_ICE();
			const int nPOISON = sItemCustom.GETRESIST_POISON();
			const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();
					
			if ( (nELEC) && (nFIRE) && (nICE) && (nPOISON) && (nSPIRIT) )
			{
				SetResiIcons();
			}
			else
			{
				ResetResiIcons();
			}
		}
		if ( sNpcNativeID != sItemCustom.sNativeID ) 
			{
				sNpcNativeID = sItemCustom.sNativeID;
		
				if ( sNpcNativeID.IsValidNativeID() )
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNpcNativeID );
					if ( !pItem )
					{
						GASSERT ( 0 && "µî·ÏµÇÁö ¾ÊÀº IDÀÔ´Ï´Ù." );
						return ;
						ResetItemRender ();
						ResetItemBoxRender();
						ResetNameRender ();
					}	
					
					
					ResetItemRender ();	
					ResetNameRender ();
					ResetItemBoxRender();
					AddItemRender ( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
					{
						if ( pItem->sBasicOp.emItemType != ITEM_BOX  )
						{
							return;
							ResetItemRender ();	
							ResetNameRender ();
							ResetItemBoxRender();
						}
						
						ResetItemRender ();	
						ResetItemBoxRender();
						AddItemRender ( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
						AddTextNoSplit ( "Shift-Click + Right-Click to Show Item Preview in your character. Items aren't Random.", NS_UITEXTCOLOR::WHITE );

						for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
						{
							SITEMCUSTOM sCUSTOM;
							sCUSTOM.sNativeID = pItem->sBox.sITEMS[i].nidITEM;

							if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

							SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
							if ( !pItemData ) 
							{
								return;
							}

							AddItemBoxRender ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile(), i );
						}
					}
				}
			}
		}
	}
};
