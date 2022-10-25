#include "stdafx.h"
#include "./GLogicEx.h"
#include "./GLItemMan.h"

#include "CItemSetLists.h" //add giftset
#include "CItemSetListOption.h" //add giftset

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//add giftset
void GLCHARLOGIC::UPDATE_ITEMSETLIST () //jdev
{
	{
		DWORD	dwTotal = 0;
		DWORD	dwFound = 0;
		DWORD	dwCount = 0;

		CItemSetLists::ITEMSETLISTOPTION_LIST_ITER iter = CItemSetLists::GetInstance().m_listItemSet.begin();
		CItemSetLists::ITEMSETLISTOPTION_LIST_ITER iter_end = CItemSetLists::GetInstance().m_listItemSet.end();
		for( ; iter != iter_end; ++iter )
		{
			CItemSetListsOption * pItemSetListsOption = *iter;
			for( DWORD i = 0 ; i < CItemSetListsOption::SLOT_MAX ; i ++ )
			{
				if ( pItemSetListsOption->m_sItemSetLists[ i ].fMainID > -1 && pItemSetListsOption->m_sItemSetLists[ i ].fSubID > -1 )
				{
					dwTotal += 1;
					SNATIVEID sID = SNATIVEID( (WORD) pItemSetListsOption->m_sItemSetLists[ i ].fMainID , (WORD) pItemSetListsOption->m_sItemSetLists[ i ].fSubID );
					SITEMCUSTOM sSLotItem = SITEMCUSTOM( NATIVEID_NULL() );
					for( DWORD i = 0 ; i < SLOT_TSIZE ; i ++ )
					{
						sSLotItem = GET_SLOT_ITEM ( (EMSLOT) i );
						if ( sID == sSLotItem.sNativeID ) 
						{
							dwFound += 1;
						}	
					}
				}
			}

			if ( dwTotal != 0 && dwTotal == dwFound )
			{
				dwCount = pItemSetListsOption->m_dwOptionCount;
				for( DWORD i = 0 ; i < dwCount ; i ++ )
				{
					DWORD dwType = pItemSetListsOption->m_sPartOption[ i ].dwType;
					DWORD dwValue = pItemSetListsOption->m_sPartOption[ i ].dwValue;
					switch ( (EMITEM_ADDON) dwType )
					{
					case EMADD_NONE:		break;
					case EMADD_HITRATE:		m_sSUMITEM.nHitRate		+= dwValue;		break;
					case EMADD_AVOIDRATE:	m_sSUMITEM.nAvoidRate	+= dwValue;		break;
					case EMADD_DAMAGE:
						m_sSUMITEM.gdDamage.dwLow+= dwValue;
						m_sSUMITEM.gdDamage.dwMax+= dwValue;
						break;

					case EMADD_DEFENSE:		m_sSUMITEM.nDefense		+= dwValue;		break;
					case EMADD_HP:			m_sSUMITEM.nHP			+= dwValue;		break;
					case EMADD_MP:			m_sSUMITEM.nMP			+= dwValue;		break;
					case EMADD_SP:			m_sSUMITEM.nSP			+= dwValue;		break;
					case EMADD_CP:			m_sSUMITEM.nCP			+= dwValue;		break;
					case EMADD_STATS_POW:	m_sSUMITEM.sStats.dwPow	+= dwValue;		break;
					case EMADD_STATS_STR:	m_sSUMITEM.sStats.dwStr	+= dwValue;		break;
					case EMADD_STATS_SPI:	m_sSUMITEM.sStats.dwSpi	+= dwValue;		break;
					case EMADD_STATS_DEX:	m_sSUMITEM.sStats.dwDex	+= dwValue;		break;
					case EMADD_STATS_INT:	m_sSUMITEM.sStats.dwInt	+= dwValue;		break;
					case EMADD_STATS_STA:	m_sSUMITEM.sStats.dwSta	+= dwValue;		break;
					case EMADD_PA:			m_sSUMITEM.nPA			+= dwValue;		break;
					case EMADD_SA:			m_sSUMITEM.nSA			+= dwValue;		break;
					};	
				}
			}
		}
	}



	{
		//for costume
		DWORD	dwTotal2 = 0;
		DWORD	dwFound2 = 0;
		DWORD	dwCount2 = 0;

		CItemSetLists::ITEMSETLISTOPTION_LIST_ITER iter = CItemSetLists::GetInstance().m_listItemSet.begin();
		CItemSetLists::ITEMSETLISTOPTION_LIST_ITER iter_end = CItemSetLists::GetInstance().m_listItemSet.end();
		for( ; iter != iter_end; ++iter )
		{
			CItemSetListsOption * pItemSetListsOption = *iter;
			for( DWORD i = 0 ; i < CItemSetListsOption::SLOT_MAX ; i ++ )
			{
				if ( pItemSetListsOption->m_sItemSetLists[ i ].fMainID > -1 && pItemSetListsOption->m_sItemSetLists[ i ].fSubID > -1 )
				{
					dwTotal2 += 1;
					SNATIVEID sID = SNATIVEID( (WORD) pItemSetListsOption->m_sItemSetLists[ i ].fMainID , (WORD) pItemSetListsOption->m_sItemSetLists[ i ].fSubID );
					SITEMCUSTOM sSLotItem = SITEMCUSTOM( NATIVEID_NULL() );
					for( DWORD i = 0 ; i < SLOT_TSIZE ; i ++ )
					{
						sSLotItem = GET_SLOT_ITEM ( (EMSLOT) i );
						if ( sID == sSLotItem.nidDISGUISE ) 
						{
							dwFound2 += 1;
						}	
					}
				}
			}

			if ( dwTotal2 == dwFound2 )
			{
				dwCount2 = pItemSetListsOption->m_dwOptionCount;
				for( DWORD i = 0 ; i < dwCount2 ; i ++ )
				{
					DWORD dwType = pItemSetListsOption->m_sPartOption[ i ].dwType;
					DWORD dwValue = pItemSetListsOption->m_sPartOption[ i ].dwValue;
					switch ( (EMITEM_ADDON) dwType )
					{
					case EMADD_NONE:		break;
					case EMADD_HITRATE:		m_sSUMITEM.nHitRate		+= dwValue;		break;
					case EMADD_AVOIDRATE:	m_sSUMITEM.nAvoidRate	+= dwValue;		break;
					case EMADD_DAMAGE:
						m_sSUMITEM.gdDamage.dwLow+= dwValue;
						m_sSUMITEM.gdDamage.dwMax+= dwValue;
						break;

					case EMADD_DEFENSE:		m_sSUMITEM.nDefense		+= dwValue;		break;
					case EMADD_HP:			m_sSUMITEM.nHP			+= dwValue;		break;
					case EMADD_MP:			m_sSUMITEM.nMP			+= dwValue;		break;
					case EMADD_SP:			m_sSUMITEM.nSP			+= dwValue;		break;
					case EMADD_CP:			m_sSUMITEM.nCP			+= dwValue;		break;
					case EMADD_STATS_POW:	m_sSUMITEM.sStats.dwPow	+= dwValue;		break;
					case EMADD_STATS_STR:	m_sSUMITEM.sStats.dwStr	+= dwValue;		break;
					case EMADD_STATS_SPI:	m_sSUMITEM.sStats.dwSpi	+= dwValue;		break;
					case EMADD_STATS_DEX:	m_sSUMITEM.sStats.dwDex	+= dwValue;		break;
					case EMADD_STATS_INT:	m_sSUMITEM.sStats.dwInt	+= dwValue;		break;
					case EMADD_STATS_STA:	m_sSUMITEM.sStats.dwSta	+= dwValue;		break;
					case EMADD_PA:			m_sSUMITEM.nPA			+= dwValue;		break;
					case EMADD_SA:			m_sSUMITEM.nSA			+= dwValue;		break;
					};	
				}
			}
		}
	}
}
