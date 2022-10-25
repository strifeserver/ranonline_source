#include "StdAfx.h"
#include "StringUtils.h"

#include "GLPeriod.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "GLFriendClient.h"

#include "../RanClientUILib/Interface/InnerInterface.h"
#include "../RanClientUILib/Interface/UITextControl.h"

#include "Emoticon.h"
#include "dxincommand.h"

#include "GLLandManClient.h"
#include "G-Logic/GLItemLink/GLItemLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//extern BOOL g_bFRAME_LIMIT;


namespace dxincommand
{
#if defined(NDEBUG)
	bool bDISP_FPS = false;
	bool bDISP_CONSOLE = false;
#else
	bool bDISP_FPS = true;
	bool bDISP_CONSOLE = true;
#endif

	CString BOOL2STR ( bool bOn )
	{
		return bOn ? "ON" : "OFF";
	}

	bool command ( const char* szcommand, SITEMLINK* pItemLink )
	{
		if ( szcommand==NULL || szcommand[0]==NULL )	return false;
		if ( szcommand[0]!='/' && szcommand[0]!='&' )	return false;

		STRUTIL::ClearSeparator ();
		STRUTIL::RegisterSeparator ( "\t" );
		STRUTIL::RegisterSeparator ( " " );
		STRUTIL::RegisterSeparator ( "," );

		CString strLINE(szcommand);
	
		CStringArray strArray ;
		STRUTIL::StringSeparate ( strLINE, strArray );
		if ( strArray.GetSize()==0 )					return true;

		CDebugSet::ToListView ( strLINE );

		CString strCOMMAND = strArray.GetAt(0);

		//	Note : 제스쳐.
		int nMOTION = CEmoticon::GetInstance().GetEmoticonOfMotion ( strCOMMAND.GetString() );
		if ( nMOTION!=UINT_MAX )
		{
			bool bACTION = GLGaeaClient::GetInstance().GetCharacter()->ReqGESTURE ( nMOTION, true );
			if ( bACTION )								return true;
		}

		if ( strCOMMAND=="/?" )
		{
			LISTEMOTICON& listEMOTICON = CEmoticon::GetInstance().GetEmoticon();
			LISTEMOTICON_ITER pos = listEMOTICON.begin();
			LISTEMOTICON_ITER end = listEMOTICON.end();

			for ( ; pos!=end; ++pos )
			{
				SEMOTICON &sEMOTICON = (*pos);
				CInnerInterface::GetInstance().PrintConsoleText ( "%s", sEMOTICON.strMotion.c_str() );
			}

			return true;
		}

		//add addstats cmd
		if( strCOMMAND =="/pow")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMPOW, value);
		}
		else if( strCOMMAND =="/dex")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMDEX, value);
		}
		else if( strCOMMAND =="/int")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMSPI, value);
		}
		else if( strCOMMAND =="/vit")
		{
			if(strArray.GetSize() != 2)		return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD value = ( DWORD ) atoi( strPARAM_01.GetString() );
			GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUpCmd( EMSTR, value);
		}

		//	Note : GM 명령.
		DWORD dwUSERLVL = GLGaeaClient::GetInstance().GetCharacter()->m_dwUserLvl;
		if ( dwUSERLVL < USER_SPECIAL )						return true;

		if ( szcommand[0]=='&' )
		{
			int nLEN = (int) strlen(szcommand) - 1;
			if ( nLEN==0 )								return true;

			if ( nLEN > CHAT_MSG_SIZE )	 nLEN = CHAT_MSG_SIZE;

			NET_CHAT NetMsg;	
			NetMsg.nmg.nType	= NET_MSG_CHAT;
			NetMsg.emType		= CHAT_TYPE_GLOBAL;
			if ( pItemLink )
				NetMsg.sITEMLINK = *pItemLink;
			::StringCchCopyN ( NetMsg.szChatMsg, CHAT_MSG_SIZE, szcommand+1, nLEN );

			NETSEND ( &NetMsg );

			return true;
		}

		if ( strCOMMAND=="/dsp" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			if ( strPARAM_01=="off" )
			{
				bDISP_CONSOLE = false;
				bDISP_FPS = false;

				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "console, fps", BOOL2STR(false) );
			}
			if ( strPARAM_01=="console" )
			{
				bDISP_CONSOLE = !bDISP_CONSOLE;

				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", strPARAM_01, BOOL2STR(bDISP_CONSOLE) );
			}
			else if ( strPARAM_01=="fps" )
			{
				bDISP_FPS = !bDISP_FPS;

				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", strPARAM_01, BOOL2STR(bDISP_FPS) );
			}
			else if ( strPARAM_01=="gate" )
			{
				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
				if ( !pLand )							return false;
				
				DxLandGateMan* pGateMan = &pLand->GetLandGateMan();
				if ( !pGateMan )						return false;

				PDXLANDGATE pLandGate = pGateMan->GetListLandGate();
				while ( pLandGate )
				{
					CString strGATE;
					strGATE.Format ( "%d ", pLandGate->GetGateID() );
					
					strGATE += "[";
					if ( pLandGate->GetFlags() & DxLandGate::GATE_OUT )	strGATE += "out";
					if ( (pLandGate->GetFlags()&DxLandGate::GATE_OUT) && (pLandGate->GetFlags()&DxLandGate::GATE_IN) )
						strGATE += "/";
					if ( pLandGate->GetFlags() & DxLandGate::GATE_IN )	strGATE += "in";
					strGATE += "]";

					if ( pLandGate->GetFlags() & DxLandGate::GATE_OUT )
					{
						strGATE += "=>";
						const char *szMAPNAME = GLGaeaClient::GetInstance().GetMapName ( pLandGate->GetToMapID() );
						if ( szMAPNAME )	strGATE += szMAPNAME;
					}

					CInnerInterface::GetInstance().PrintConsoleText ( strGATE.GetString() );

					pLandGate = pLandGate->m_pNext;
				};
			}
			else if ( strPARAM_01=="player" )
			{
				PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
				if ( !pLand )							return false;

				GLCHARCLIENTLIST* pLIST = pLand->GetCharList();
				GLCHARCLIENTNODE *pCharCur = pLIST->m_pHead;
				for ( ; pCharCur; pCharCur = pCharCur->pNext )
				{
					CInnerInterface::GetInstance().PrintConsoleText ( "%s	: charid = %d",
						pCharCur->Data->m_strName.c_str(), pCharCur->Data->GetCharData().dwCharID );
				}
			}
			else if ( strPARAM_01=="allplayer" )
			{
				GLMSG::SNET_GM_VIEWALLPLAYER NetMsg;
				NETSEND ( &NetMsg );
			}
			else if ( strPARAM_01=="event" )
			{
				GLMSG::SNET_GM_VIEWWORKEVENT NetMsg;
				NETSEND ( &NetMsg );
			}
		}

		else if ( strCOMMAND== "/shop_info" )
		{
			if ( strArray.GetSize() != 3 &&  strArray.GetSize() != 1  )			return true;

			SNATIVEID mapID = GLGaeaClient::GetInstance().GetActiveMapID();

			if( mapID.wMainID != GLCONST_CHAR::WMARKET_MID || mapID.wSubID != GLCONST_CHAR::WMARKET_SID )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Can Only use this on Market Map." );
				return true;
			}

			GLMSG::SNET_GM_SHOP_INFO_REQ NetMsg;

			if( strArray.GetSize() == 3 )
			{
				CString strPARAM_01 = strArray.GetAt(1);
				CString strPARAM_02 = strArray.GetAt(2);

				NetMsg.bBasicInfo = FALSE;
				
				NetMsg.sSearchItemID.wMainID = (WORD) atoi ( strPARAM_01.GetString() );
				NetMsg.sSearchItemID.wSubID  = (WORD) atoi ( strPARAM_02.GetString() );
				NETSENDTOFIELD ( &NetMsg );
			}
			else
			{
				NETSENDTOFIELD( &NetMsg );
			}
		}

		else if ( strCOMMAND== "/get_item" ) //add itemcmd
		{
			if ( strArray.GetSize() != 5 &&  strArray.GetSize() != 4  )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Invalid Command! :P" );
				return true;
			}

			if( strArray.GetSize() == 5 )
			{
				CString strPARAM_01 = strArray.GetAt(1); //mid
				CString strPARAM_02 = strArray.GetAt(2); //sid
				CString strPARAM_03 = strArray.GetAt(3); //pass
				CString strPARAM_04 = strArray.GetAt(4); //amount
				
				SNATIVEID	sItemReq;
				sItemReq.wMainID = (WORD) atoi ( strPARAM_01.GetString() );
				sItemReq.wSubID  = (WORD) atoi ( strPARAM_02.GetString() );
				WORD	wPass	 = (WORD) atoi ( strPARAM_03.GetString() );
				WORD	wAmount	 = (WORD) atoi ( strPARAM_04.GetString() );

				GLGaeaClient::GetInstance().GetCharacter()->ReqGMItem(sItemReq,wAmount,wPass);
			}
			else if( strArray.GetSize() == 4 )
			{
				CString strPARAM_01 = strArray.GetAt(1); //mid
				CString strPARAM_02 = strArray.GetAt(2); //sid
				CString strPARAM_03 = strArray.GetAt(3); //pass
				
				SNATIVEID	sItemReq;
				sItemReq.wMainID = (WORD) atoi ( strPARAM_01.GetString() );
				sItemReq.wSubID  = (WORD) atoi ( strPARAM_02.GetString() );
				WORD	wPass	 = (WORD) atoi ( strPARAM_03.GetString() );
				WORD	wAmount	 = (WORD) 1 ;

				GLGaeaClient::GetInstance().GetCharacter()->ReqGMItem(sItemReq,wAmount,wPass);
			}
		}
		else if ( strCOMMAND=="/profile")
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			if ( strPARAM_01=="on" )
			{
				g_bProfile = TRUE;
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "profile", "on" );
			}
			else if ( strPARAM_01=="off" )
			{
				g_bProfile = FALSE;
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "profile", "off" );
			}
		}

		else if ( strCOMMAND=="/chatlog")
		{
			const	int	NORMAL		= 0x0001;	//	일반
			const	int	PRIVATE		= 0x0002;	//	귓속말
			const	int	PARTY		= 0x0004;	//	파티
			const	int	GUILD		= 0x0008;	//	클럽
			const	int	TOALL		= 0x0010;	//	확성기
			const	int ALLIANCE	= 0x0020;	//	동맹
			const	int	SYSTEM		= 0x0100;	//	시스템
			const	int	ALL			= NORMAL | PRIVATE | PARTY | GUILD | TOALL | ALLIANCE | SYSTEM;


			if ( strArray.GetSize() != 3 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			int nChatLogType = 0;

			if ( strPARAM_02 == "normal" )			nChatLogType = NORMAL;
			else if ( strPARAM_02 == "private" )	nChatLogType = PRIVATE;
			else if ( strPARAM_02 == "party" )		nChatLogType = PARTY;
			else if ( strPARAM_02 == "guild" )		nChatLogType = GUILD;
			else if ( strPARAM_02 == "toall" )		nChatLogType = TOALL;
			else if ( strPARAM_02 == "alliance" )	nChatLogType = ALLIANCE;
			else if ( strPARAM_02 == "system" )		nChatLogType = SYSTEM;
			else if ( strPARAM_02 == "all" )		nChatLogType = ALL;
			else	return true;

			if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().ChatLog( true, nChatLogType );
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "Starting Chatlog Type:", strPARAM_02 );
			}
			else if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "Ending ChatLog Type:", strPARAM_02 );
				CInnerInterface::GetInstance().ChatLog( false, nChatLogType );

			}
		}
		else if ( strCOMMAND=="/weather" )
		{
			if ( strArray.GetSize() < 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD dwWeather(NULL);
			bool bActiveWeather(true);
			WORD map_mID = 0, map_sID = 0;
			DWORD dwApplyTime = 0;
            bool  bEveryApply = TRUE;

			if( strArray.GetSize() == 2 )
			{
				bEveryApply = TRUE; 

				if ( strPARAM_01=="clear" )
				{
					if( strArray.GetSize() != 2 ) return true;

					bActiveWeather = FALSE;
					dwWeather	   = NULL;
					CInnerInterface::GetInstance().PrintConsoleText ( "Weather Override REMOVED" );
				}
				else if ( strPARAM_01=="rain" )
				{
					dwWeather = FGW_RAIN;

					CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "Override Global Weather :",strPARAM_01 );
				}
				else if ( strPARAM_01=="snow" )
				{
					dwWeather = FGW_SNOW;

					CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "Override Global Weather :",strPARAM_01 );
				}
				else if ( strPARAM_01=="spore" )
				{
					dwWeather = FGW_SPORE;

					CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "Override Global Weather :",strPARAM_01 );
				}
				else if ( strPARAM_01=="leaves" )
				{
					dwWeather = FGW_LEAVES;

					CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s", "Override Global Weather :",strPARAM_01 );
				}

			}
			// Add OneMap Weather
			else if( strArray.GetSize() == 5 )
			{

				bEveryApply = FALSE; 

				CString strPARAM_02 = strArray.GetAt(2);
				CString strPARAM_03 = strArray.GetAt(3);
				CString strPARAM_04 = strArray.GetAt(4);

				map_mID		= atoi(strPARAM_02.GetString());
				map_sID		= atoi(strPARAM_03.GetString());
				dwApplyTime = atoi(strPARAM_04.GetString());

				if ( strPARAM_01=="rain" )
				{
					dwWeather = FGW_RAIN;

					CInnerInterface::GetInstance().PrintConsoleText ( "Override Weather :%s to MAP MID:%d SID:%d in %d Minutes", 
																	  strPARAM_01,map_mID, map_sID, dwApplyTime );
				}
				else if ( strPARAM_01=="snow" )
				{
					dwWeather = FGW_SNOW;

					CInnerInterface::GetInstance().PrintConsoleText ( "Override Weather :%s to MAP MID:%d SID:%d in %d Minutes", 
																	  strPARAM_01,map_mID, map_sID, dwApplyTime );
				}
				else if ( strPARAM_01=="spore" )
				{
					dwWeather = FGW_SPORE;

					CInnerInterface::GetInstance().PrintConsoleText ( "Override Weather :%s to MAP MID:%d SID:%d in %d Minutes", 
																	  strPARAM_01,map_mID, map_sID, dwApplyTime );
				}
				else if ( strPARAM_01=="leaves" )
				{
					dwWeather = FGW_LEAVES;

					CInnerInterface::GetInstance().PrintConsoleText ( "Override Weather :%s to MAP MID:%d SID:%d in %d Minutes", 
																	  strPARAM_01,map_mID, map_sID, dwApplyTime );
				}
			}
			else if( strArray.GetSize() == 4 )
			{
				CString strPARAM_02 = strArray.GetAt(2);
				CString strPARAM_03 = strArray.GetAt(3);

				map_mID		= atoi(strPARAM_02.GetString());
				map_sID		= atoi(strPARAM_03.GetString());

				if ( strPARAM_01=="clear" )
				{
					dwWeather	   = NULL;
					CInnerInterface::GetInstance().PrintConsoleText ( "Override Weather REMOVED to MAP MID:%d SID:%d", 
																	  map_mID, map_sID );
				}

				bEveryApply = FALSE; 
			}
			else
			{
				return TRUE;
			}


			// Map 설정해서 보내는 메시지
			if( bEveryApply == FALSE )
			{
				GLMSG::SNETSERVER_CTRL_WEATHER2 NetMsg;
				NetMsg.dwWeather   = dwWeather;
				NetMsg.map_mID     = map_mID;
				NetMsg.map_sID     = map_sID;
				NetMsg.dwApplyTime = dwApplyTime;
				NETSEND(&NetMsg);
			// 전체 맵에 적용되는 메시지
			}else
			{
				GLMSG::SNETSERVER_CTRL_WEATHER NetMsg;
				NetMsg.bActive   = bActiveWeather;
				NetMsg.dwWeather = dwWeather;
				NETSEND(&NetMsg);
			}
		}
		else if ( strCOMMAND=="/time" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD dwTIME = (DWORD) atoi ( strPARAM_01.GetString() );
			if ( dwTIME > 24 )	return true;

			GLMSG::SNETSERVER_CTRL_TIME NetMsg;
			NetMsg.dwTime = dwTIME;
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %d", "Override Server TIME", dwTIME );
		}
		else if ( strCOMMAND=="/month" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			DWORD dwMONTH = (DWORD) atoi ( strPARAM_01.GetString() );
			if ( dwMONTH > 12 )	return true;

			GLMSG::SNETSERVER_CTRL_MONTH NetMsg;
			NetMsg.dwMonth = dwMONTH;
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %d", "Override Server MONTH", dwMONTH );
		}
		else if ( strCOMMAND=="/itemhold" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
		
			bool bHold(true);

			if ( strPARAM_01=="on" )
			{
				bHold = true;

				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s	%s", "Set ITEMHOLD to", "on" ,"Item /Gold Drop will follow owner /party rule" );
			}
			else if ( strPARAM_01=="off" )
			{
				bHold = false;
				CInnerInterface::GetInstance().PrintConsoleText ( "%s  : %s	%s", "Set ITEMHOLD to", "off" , "Item /Gold Drop is now free for everyone to loot");
			}

			GLMSG::SNETSERVER_CTRL_GENITEMHOLD NetMsg;
			NetMsg.bHold = bHold;
			NETSEND(&NetMsg);
		}
		else if ( strCOMMAND=="/move2gate" || strCOMMAND=="/m2g" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD dwGATE = (DWORD) atoi ( strPARAM_01.GetString() );

			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
			if ( !pLand )								return false;
			
			DxLandGateMan* pGateMan = &pLand->GetLandGateMan();
			if ( !pGateMan )							return false;

			PDXLANDGATE pGATE = pGateMan->FindLandGate ( dwGATE );
			if ( !pGATE )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "%d gate not exist", dwGATE );
			}

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOVE2GATE_FLD NetMsgFld;
				NetMsgFld.dwGaeaID = 0;
				NetMsgFld.dwGATE = dwGATE;
				NETSEND ( &NetMsgFld );

				CInnerInterface::GetInstance().PrintConsoleText ( "Moving to Gate : %d ", dwGATE );
			}
			else
			{
				GLMSG::SNET_GM_MOVE2GATE NetMsg;
				NetMsg.dwGATE = dwGATE;
				NETSEND ( &NetMsg );

				CInnerInterface::GetInstance().PrintConsoleText ( "Moving to Gate : %d ", dwGATE );
			}
		}
		else if ( strCOMMAND=="/move2pos" || strCOMMAND=="/m2p" )
		{
			if ( strArray.GetSize() != 5 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOVE2MAPPOS_FLD NetMsgFld;
				NetMsgFld.nidMAP.wMainID = ( WORD ) atoi( strPARAM_01.GetString() );
				NetMsgFld.nidMAP.wSubID = ( WORD ) atoi( strPARAM_02.GetString() );
				NetMsgFld.dwPOSX = ( DWORD ) atoi( strPARAM_03.GetString() );
				NetMsgFld.dwPOSY = ( DWORD ) atoi( strPARAM_04.GetString() );
				NetMsgFld.dwGaeaID = 0;

				NETSEND( &NetMsgFld );

				CInnerInterface::GetInstance().PrintConsoleText ( "Moving to Position Map MID:%s  Map SID:%s Coordinates X:%s Coordinates Y:%s ",strPARAM_01,strPARAM_02,strPARAM_03,strPARAM_04);
			}
			else
			{
				GLMSG::SNET_GM_MOVE2MAPPOS NetMsg;
				NetMsg.nidMAP.wMainID = ( WORD ) atoi( strPARAM_01.GetString() );
				NetMsg.nidMAP.wSubID = ( WORD ) atoi( strPARAM_02.GetString() );
				NetMsg.dwPOSX = ( DWORD ) atoi( strPARAM_03.GetString() );
				NetMsg.dwPOSY = ( DWORD ) atoi( strPARAM_04.GetString() );

				NETSEND( &NetMsg );

				CInnerInterface::GetInstance().PrintConsoleText ( "Moving to Position Map MID:%s  Map SID:%s Coordinates X:%s Coordinates Y:%s ",strPARAM_01,strPARAM_02,strPARAM_03,strPARAM_04);

			}
		}
		else if ( strCOMMAND=="/move2char" || strCOMMAND=="/m2c" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNETPC_GM_MOVE2CHAR NetMsg;
			StringCchCopy ( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "Moving To Character : %s", NetMsg.szNAME );
		}

		else if ( strCOMMAND=="/msgview" || strCOMMAND=="/spy")
		{
			if ( strArray.GetSize() < 2 ) return true;
			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNETPC_GM_GETWHISPERMSG NetMsg;
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "View Character Messages : off" );
			}
			else
			{			
				StringCchCopy( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
				CInnerInterface::GetInstance().PrintConsoleText ( "View Character Messages Start Character Name : %s ", NetMsg.szNAME );
			}
			NETSEND(&NetMsg);		

		}

		else if ( strCOMMAND=="/genchar" || strCOMMAND=="/callchar" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNETPC_GM_GENCHAR NetMsg;
			StringCchCopy ( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NETSEND(&NetMsg);

			CInnerInterface::GetInstance().PrintConsoleText ( "Call Character : %s", NetMsg.szNAME );
		}

		else if ( strCOMMAND=="/visible" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			if ( strPARAM_01=="none" )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqVisibleNone();

				CInnerInterface::GetInstance().PrintConsoleText ( "Visible Nothing" );
			}
			else if ( strPARAM_01=="off" )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqVisibleOff();

				CInnerInterface::GetInstance().PrintConsoleText ( "Character now Not Visible InGame" );
			}
			else if ( strPARAM_01=="on" )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqVisibleOn();

				CInnerInterface::GetInstance().PrintConsoleText ( "Character now  Visible InGame" );
			}
		}
		else if ( strCOMMAND=="/kick_cn" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			GLMSG::SNET_GM_KICK_USER NetMsg;
			StringCchCopy ( NetMsg.szNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NetMsg.bName = TRUE;

			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "Disconnecting Character : %s", strPARAM_01 );
		}
		else if ( strCOMMAND=="/kick_ci" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			
			GLMSG::SNET_GM_KICK_USER NetMsg;
			NetMsg.dwID = (DWORD) atoi ( strPARAM_01.GetString() );
			NetMsg.bName = FALSE;

			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "Disconnecting Character ID : %d", NetMsg.dwID );
		}
		else if ( strCOMMAND=="/event_exp_begin" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			float fSCALE = (float) atof ( strPARAM_01.GetString() );

			if ( fSCALE > GLCONST_CHAR::fMaxEventExp )	fSCALE = GLCONST_CHAR::fMaxEventExp;

			CInnerInterface::GetInstance().PrintConsoleText ( "Experience Event Start, Experience scale : %f",
				fSCALE);

			GLMSG::SNET_GM_EVENT_EXP NetMsg;
			NetMsg.fSCALE = fSCALE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/event_exp_end" )
		{
			CInnerInterface::GetInstance().PrintConsoleText ( "Experience Event End" );

			GLMSG::SNET_GM_EVENT_EXP_END NetMsg;
			NETSEND ( &NetMsg );
		}
		else if( strCOMMAND=="/limit_event_begin" )
		{
			//if ( strArray.GetSize() != 7 && strArray.GetSize() != 8  ) return true;
			//add btg attack
			if ( strArray.GetSize() != 7 && strArray.GetSize() != 8 && strArray.GetSize() != 9  ) return true;

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);
			CString strPARAM_05 = strArray.GetAt(5);
			CString strPARAM_06 = strArray.GetAt(6);
			
			int start_Lv	  = (int)atoi(strPARAM_01.GetString());
			int end_Lv		  = (int)atoi(strPARAM_02.GetString());
			int play_Time	  = (int)atoi(strPARAM_03.GetString());
			int buster_Time   = (int)atoi(strPARAM_04.GetString());
			float expGain_Rate  = (float)atof(strPARAM_05.GetString());
			float itemGain_Rate = (float)atof(strPARAM_06.GetString());

			

			//add btg attack
			float attackGain_Rate = 0;
			if( strArray.GetSize() == 8 ) 
			{
				CString strPARAM_07;
				strPARAM_07 = strArray.GetAt(7);
				attackGain_Rate = (float)atof(strPARAM_07.GetString());
			}

			DWORD dwEventMinute = 0;			
			if( strArray.GetSize() == 9 ) 
			{
				CString strPARAM_08;
				strPARAM_08 = strArray.GetAt(8);
				dwEventMinute   = (DWORD)atoi(strPARAM_08.GetString());
			}

			if( expGain_Rate > GLCONST_CHAR::fBTGMaxEventExp ) expGain_Rate = GLCONST_CHAR::fBTGMaxEventExp;
			if( itemGain_Rate > GLCONST_CHAR::fBTGMaxEventItem ) itemGain_Rate = GLCONST_CHAR::fBTGMaxEventItem;

			//add btg attack
			if( attackGain_Rate > GLCONST_CHAR::fBTGMaxEventAttack ) attackGain_Rate = GLCONST_CHAR::fBTGMaxEventAttack;

			CInnerInterface::GetInstance().PrintConsoleText ( 
				"Bonus Time Event Started, From Level: %d ~ %d, Wait Time: %d, Bonus Time: %d ExpGainRate: %.3f ItemGainRate: %.3f EventMinute %d AttackGainRate: %.3f",
				start_Lv, end_Lv, play_Time, buster_Time, expGain_Rate, itemGain_Rate, dwEventMinute,attackGain_Rate );

			GLMSG::SNET_GM_LIMIT_EVENT_BEGIN NetMsg;
			NetMsg.start_Lv		 = start_Lv;
			NetMsg.end_Lv		 = end_Lv;
			NetMsg.play_Time	 = play_Time;
			NetMsg.buster_Time   = buster_Time;
			NetMsg.expGain_Rate  = expGain_Rate;
			NetMsg.itemGain_Rate = itemGain_Rate;
			NetMsg.dwEventMinute = dwEventMinute;
			//add btg attack
			NetMsg.attackGain_Rate = attackGain_Rate;
			NETSEND ( &NetMsg );

		}
		else if( strCOMMAND=="/limit_event_end" )
		{
			CInnerInterface::GetInstance().PrintConsoleText ( "Bonus Time Event Ended" );
			GLMSG::SNET_GM_LIMIT_EVENT_END NetMsg;
			NETSEND ( &NetMsg );

		}
		else if ( strCOMMAND=="/event_item_begin" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			float fSCALE = (float) atof ( strPARAM_01.GetString() );

			if( fSCALE > GLCONST_CHAR::fMaxEventItem ) fSCALE = GLCONST_CHAR::fMaxEventItem;

			CInnerInterface::GetInstance().PrintConsoleText ( "Item Drop Event Start scale:%f", fSCALE );

			GLMSG::SNET_GM_EVEN_ITEM_GEN NetMsg;
			NetMsg.fRATE = fSCALE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/event_item_end" )
		{
			CInnerInterface::GetInstance().PrintConsoleText ( "Item Drop Event End" );

			GLMSG::SNET_GM_EVEN_ITEM_GEN_END NetMsg;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/event_money_begin" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			float fSCALE = (float) atof ( strPARAM_01.GetString() );

			if( fSCALE > GLCONST_CHAR::fMaxEventMoney ) fSCALE = GLCONST_CHAR::fMaxEventMoney;

			CInnerInterface::GetInstance().PrintConsoleText ( "Money Drop Event Started scale:%f", fSCALE );

			GLMSG::SNET_GM_EVEN_MONEY_GEN NetMsg;
			NetMsg.fRATE = fSCALE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/event_money_end" )
		{
			CInnerInterface::GetInstance().PrintConsoleText ( "Money Drop Event End" );

			GLMSG::SNET_GM_EVEN_MONEY_GEN_END NetMsg;
			NETSEND ( &NetMsg );
		}

		else if ( strCOMMAND=="/pk_mode" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			BOOL bMODE(FALSE);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "PK is now OFF" );
				bMODE = FALSE;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "PK is now ON" );
				bMODE = TRUE;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "pk_mode : invalid param" );
				return true;
			}

			GLMSG::SNET_SERVER_PLAYERKILLING_MODE NetMsg;
			NetMsg.bMODE = bMODE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/crow_pos" )
		{
			if ( strArray.GetSize() != 3 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
		
			WORD mid = (WORD) atoi (strPARAM_01.GetString());
			WORD sid = (WORD) atoi (strPARAM_02.GetString());

			CInnerInterface::GetInstance().PrintConsoleText ( "Search Crow : [MID:%d/SID:%d]", mid, sid );

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_WHERE_NPC_FLD NetMsgFld;
				NetMsgFld.nidNPC   = SNATIVEID(mid,sid);
				NetMsgFld.dwGaeaID = 0;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_WHERE_NPC NetMsg;
				NetMsg.nidNPC = SNATIVEID(mid,sid);
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/crow_list" )
		{
			if ( strArray.GetSize() > 2 )			return true;

			BYTE dwFindMob = 0;
			if( strArray.GetSize() != 1 )
			{
				CString strPARAM_01 = strArray.GetAt(1);				
				if ( strPARAM_01=="mob" )
				{
					dwFindMob = 1;
					CInnerInterface::GetInstance().PrintConsoleText ( "Retrieve Mob List" );
				}
				else if ( strPARAM_01=="npc" )
				{
					dwFindMob = 2;
					CInnerInterface::GetInstance().PrintConsoleText ( "Retrieve Npc List" );
				}
			}

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_PRINT_CROWLIST_FLD NetMsgFld;
				NetMsgFld.dwGaeaID = 0;
				NetMsgFld.dwFindMob = dwFindMob;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_PRINT_CROWLIST NetMsg;
				NetMsg.dwFindMob = dwFindMob;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/bighead" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			bool bMODE(false);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighead : off" );
				bMODE = false;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighead : on" );
				bMODE = true;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighead : invalid param" );
				return true;
			}

			GLMSG::SNET_GM_BIGHEAD NetMsg;
			NetMsg.bBIGHEAD = bMODE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/bighand" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			bool bMODE(false);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighand : off" );
				bMODE = false;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighand : on" );
				bMODE = true;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "bighand : invalid param" );
				return true;
			}

			GLMSG::SNET_GM_BIGHAND NetMsg;
			NetMsg.bBIGHAND = bMODE;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/mob_gen" )
		{
			if ( strArray.GetSize() != 5 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);
			
			WORD wMainID = (WORD) atoi (strPARAM_01.GetString() );
			WORD wSubID = (WORD) atoi (strPARAM_02.GetString() );
			WORD wPosX = (WORD) atoi (strPARAM_03.GetString() );
			WORD wPosY = (WORD) atoi (strPARAM_04.GetString() );

			CInnerInterface::GetInstance().PrintConsoleText ( "Summon Single MOB   MOB MID:%s MOB SID:%s Coordinates X:%s Coordinates Y:%s",strPARAM_01,strPARAM_02,strPARAM_03,strPARAM_04 );

			PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( wMainID, wSubID );
			if ( !pCrowData )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen : invalid mob_id" );
				return true;
			}

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOB_GEN_FLD NetMsgFld;
				NetMsgFld.dwGaeaID = 0;
				NetMsgFld.wMainID = wMainID;
				NetMsgFld.wSubID = wSubID;
				NetMsgFld.wPosX = wPosX;
				NetMsgFld.wPosY = wPosY;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOB_GEN NetMsg;
				NetMsg.wMainID = wMainID;
				NetMsg.wSubID = wSubID;
				NetMsg.wPosX = wPosX;
				NetMsg.wPosY = wPosY;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/mob_gen_ex" )
		{
			if ( strArray.GetSize() != 10 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);
			CString strPARAM_05 = strArray.GetAt(5);
			CString strPARAM_06 = strArray.GetAt(6);
			CString strPARAM_07 = strArray.GetAt(7);
			CString strPARAM_08 = strArray.GetAt(8);
			CString strPARAM_09 = strArray.GetAt(9);
			
			WORD wMobMID = (WORD) atoi (strPARAM_01.GetString() );
			WORD wMobSID = (WORD) atoi (strPARAM_02.GetString() );
			WORD wMapMID = (WORD) atoi (strPARAM_03.GetString() );
			WORD wMapSID = (WORD) atoi (strPARAM_04.GetString() );
			WORD wPosX	 = (WORD) atoi (strPARAM_05.GetString() );
			WORD wPosY	 = (WORD) atoi (strPARAM_06.GetString() );
			WORD wRange  = (WORD) atoi (strPARAM_07.GetString() );
			WORD wNum	 = (WORD) atoi (strPARAM_08.GetString() );
			WORD wChannel = (WORD) atoi (strPARAM_09.GetString() );

			CInnerInterface::GetInstance().PrintConsoleText ( "Summon Multiple MOB   [MOB MID:%s MOB SID:%s]   [MAP MID:%s MAP SID:%s]   [Coordinates X:%s Coordinates Y:%s]   Range:%s  Number of MOBS :%s Channel:%s",strPARAM_01,strPARAM_02,strPARAM_03,strPARAM_04,strPARAM_05,strPARAM_06,strPARAM_07,strPARAM_08,strPARAM_09 );


			PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( wMobMID, wMobSID );
			if ( !pCrowData )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid mob_id" );
				return false;
			}
			
			SNATIVEID sMapID(wMapMID, wMapSID);
			GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
			GLMapList::FIELDMAP_ITER iter = MapsList.find ( sMapID.dwID );
			if ( iter==MapsList.end() )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid map_id" );
				return false;
			}

			if ( wRange*100 < wNum )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : out of mob amount in range" );
				return false;
			}

			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOB_GEN_EX_FLD NetMsgFld;
				NetMsgFld.wMobMID  = wMobMID;
				NetMsgFld.wMobSID  = wMobSID;
				NetMsgFld.wMapMID  = wMapMID;
				NetMsgFld.wMapSID  = wMapSID;
				NetMsgFld.wPosX	   = wPosX;
				NetMsgFld.wPosY    = wPosY;
				NetMsgFld.wRange   = wRange;
				NetMsgFld.wNum     = wNum;
				NetMsgFld.dwGaeaID = 0;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOB_GEN_EX NetMsg;
				NetMsg.wMobMID = wMobMID;
				NetMsg.wMobSID = wMobSID;
				NetMsg.wMapMID = wMapMID;
				NetMsg.wMapSID = wMapSID;
				NetMsg.wPosX   = wPosX;
				NetMsg.wPosY   = wPosY;
				NetMsg.wRange  = wRange;
				NetMsg.wNum	   = wNum;
				NetMsg.bThisChannel = wChannel;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/mob_del" )
		{
			if ( strArray.GetSize() > 3 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_del : invalid param" );
				return true;
			}

			WORD wMobMID = USHRT_MAX;
			WORD wMobSID = USHRT_MAX;
			bool bMaterial = false;

			if ( strArray.GetSize() == 2 )
			{
				 CString strPARAM_01 = strArray.GetAt(1);
				 bMaterial = (bool) atoi( strPARAM_01.GetString() );

				 
				 CInnerInterface::GetInstance().PrintConsoleText ( "Deleting MOB"  );

			}

			if ( strArray.GetSize() == 3 )
			{
				CString strPARAM_01 = strArray.GetAt(1);
				CString strPARAM_02 = strArray.GetAt(2);
				wMobMID = (WORD) atoi (strPARAM_01.GetString() );
				wMobSID = (WORD) atoi (strPARAM_02.GetString() );

			CInnerInterface::GetInstance().PrintConsoleText ( "Deleting MOB   [MOB MID:%s MOB SID:%s] ",strPARAM_01,strPARAM_02 );

			}


			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOB_DEL_FLD NetMsgFld;
				NetMsgFld.dwGaeaID = 0;	
				NetMsgFld.sMobID.wMainID = wMobMID;
				NetMsgFld.sMobID.wSubID = wMobSID;
				NetMsgFld.bMaterial = bMaterial;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOB_DEL NetMsg;
				NetMsg.sMobID.wMainID = wMobMID;
				NetMsg.sMobID.wSubID = wMobSID;
				NetMsg.bMaterial = bMaterial;
				NETSEND ( &NetMsg );
			}
		}
		else if ( strCOMMAND=="/mob_del_ex" )
		{
			if ( strArray.GetSize() != 6 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_del_ex : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);
			CString strPARAM_03 = strArray.GetAt(3);
			CString strPARAM_04 = strArray.GetAt(4);
			CString strPARAM_05 = strArray.GetAt(5);
			
			WORD wMobMID = (WORD) atoi (strPARAM_01.GetString() );
			WORD wMobSID = (WORD) atoi (strPARAM_02.GetString() );
			WORD wMapMID = (WORD) atoi (strPARAM_03.GetString() );
			WORD wMapSID = (WORD) atoi (strPARAM_04.GetString() );
			WORD wChannel = (WORD) atoi (strPARAM_05.GetString() );

			
			CInnerInterface::GetInstance().PrintConsoleText ( "Deleting Multiple MOB   [MOB MID:%s MOB SID:%s] [MAP MID:%s MAP SID:%s] Channel:%s ",strPARAM_01,strPARAM_02,strPARAM_03,strPARAM_04,strPARAM_05 );


			PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( wMobMID, wMobSID );
			if ( !pCrowData )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid mob_id" );
				return false;
			}
			
			SNATIVEID sMapID(wMapMID, wMapSID);
			GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
			GLMapList::FIELDMAP_ITER iter = MapsList.find ( sMapID.dwID );
			if ( iter==MapsList.end() )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "mob_gen_ex : invalid map_id" );
				return false;
			}
			if ( DxGlobalStage::GetInstance().IsEmulator() )
			{
				GLMSG::SNET_GM_MOB_DEL_EX_FLD NetMsgFld;
				NetMsgFld.wMobMID = wMobMID;
				NetMsgFld.wMobSID = wMobSID;
				NetMsgFld.wMapMID = wMapMID;
				NetMsgFld.wMapSID = wMapSID;
				NETSEND ( &NetMsgFld );
			}
			else
			{
				GLMSG::SNET_GM_MOB_DEL_EX NetMsg;
				NetMsg.wMobMID = wMobMID;
				NetMsg.wMobSID = wMobSID;
				NetMsg.wMapMID = wMapMID;
				NetMsg.wMapSID = wMapSID;
				NETSEND ( &NetMsg );
			}			
		}
		else if ( strCOMMAND=="/lev_new" )
		{
			if ( strArray.GetSize() != 2 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "lev_new : invalid param" );
				return true;
			}
			CString strPARAM_01 = strArray.GetAt(1);

			PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
			if ( !pLAND )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "lev_new : unknown error" );
				return true;
			}

			SNATIVEID nidMAP = pLAND->GetMapID();

			GLMSG::SNET_GM_MOB_LEV NetMsg;
			NetMsg.wMAP_MID = nidMAP.wMainID;
			NetMsg.wMAP_SID = nidMAP.wSubID;
			StringCchCopy ( NetMsg.szLevelFile, GLMSG::SNET_GM_MOB_LEV::FILE_NAME, strPARAM_01.GetString() );
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "Loading LEVEL File   %s to this map MID:%d SID:%d",strPARAM_01,nidMAP.wMainID,nidMAP.wSubID );

		}
		else if ( strCOMMAND=="/lev_del" )
		{
			PLANDMANCLIENT pLAND = GLGaeaClient::GetInstance().GetActiveMap();
			if ( !pLAND )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "lev_del : unknown error" );
				return true;
			}

			SNATIVEID nidMAP = pLAND->GetMapID();

			GLMSG::SNET_GM_MOB_LEV_CLEAR NetMsg;
			NetMsg.wMAP_MID = nidMAP.wMainID;
			NetMsg.wMAP_SID = nidMAP.wSubID;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "Deleting LEVEL FILE to this map MID:%d SID:%d",nidMAP.wMainID,nidMAP.wSubID );
		}
		else if ( strCOMMAND=="/chatblock_ua" )
		{
			if ( strArray.GetSize() != 3 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "chatblock_ua : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			DWORD dwMinute = (DWORD) atoi (strPARAM_02.GetString());

			GLMSG::SNET_GM_CHAT_BLOCK_UACCOUNT	NetMsg;
			StringCchCopy ( NetMsg.szUACCOUNT, USR_ID_LENGTH+1, strPARAM_01.GetString() );
			NetMsg.dwBLOCK_MINUTE = dwMinute;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "CHATBLOCK : uaccount %s, minute %d", NetMsg.szUACCOUNT, NetMsg.dwBLOCK_MINUTE );
		}
		else if ( strCOMMAND=="/chatblock_cn" )
		{
			if ( strArray.GetSize() != 3 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "chatblock_cn : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			DWORD dwMinute = (DWORD) atoi (strPARAM_02.GetString());

			GLMSG::SNET_GM_CHAT_BLOCK_CHARNAME	NetMsg;
			StringCchCopy ( NetMsg.szCHARNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NetMsg.dwBLOCK_MINUTE = dwMinute;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "CHATBLOCK : charname %s, minute %d", NetMsg.szCHARNAME, NetMsg.dwBLOCK_MINUTE );
		}
		else if ( strCOMMAND=="/chatblock_ci" )
		{
			if ( strArray.GetSize() != 3 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "chatblock_ci : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			DWORD dwCHARID = (DWORD) atoi (strPARAM_01.GetString());
			DWORD dwMinute = (DWORD) atoi (strPARAM_02.GetString());

			GLMSG::SNET_GM_CHAT_BLOCK_CHARID	NetMsg;
			NetMsg.dwCHARID = dwCHARID;
			NetMsg.dwBLOCK_MINUTE = dwMinute;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "CHATBLOCK : charid %d, minute %d", NetMsg.dwCHARID, NetMsg.dwBLOCK_MINUTE );
		}
		else if ( strCOMMAND=="/charinfo_cn" )
		{
			if ( strArray.GetSize() != 2 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "charinfo_cn : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);

			GLMSG::SNET_GM_CHAR_INFO_4NAME NetMsg;
			StringCchCopy ( NetMsg.szCHARNAME, CHAR_SZNAME, strPARAM_01.GetString() );
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "Reading info Character :%s",strPARAM_01 );
		}
		else if ( strCOMMAND=="/charinfo_ci" )
		{
			if ( strArray.GetSize() != 2 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "charinfo_ci : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);
			
			DWORD dwCHARID = (DWORD) atoi (strPARAM_01.GetString());

			GLMSG::SNET_GM_CHAR_INFO_4CHARID NetMsg;
			NetMsg.dwCHARID = dwCHARID;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "Reading info Character :%d",strPARAM_01 );
		}
		else if ( strCOMMAND=="/freepk_on" )
		{
			if ( strArray.GetSize() != 2 )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "freepk : invalid param" );
				return true;
			}

			CString strPARAM_01 = strArray.GetAt(1);

			DWORD dwTIME = (DWORD) atoi (strPARAM_01.GetString());

			CInnerInterface::GetInstance().PrintConsoleText ( "Free PK to Everyone %s Seconds",strPARAM_01 );

			GLMSG::SNET_GM_FREEPK NetMsg;
			NetMsg.dwPKTIME = dwTIME;
			NETSEND ( &NetMsg );
		}
		else if ( strCOMMAND=="/freepk_off" )
		{
			DWORD dwTIME(0);

			GLMSG::SNET_GM_FREEPK NetMsg;
			NetMsg.dwPKTIME = dwTIME;
			NETSEND ( &NetMsg );

			CInnerInterface::GetInstance().PrintConsoleText ( "Free PK Off" );
		}
		else if ( strCOMMAND=="/warning_msg" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			bool bOn(false);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Warning Message : off" );
				bOn = false;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Warning Message : on" );
				bOn = true;
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "warning_msg : invalid param" );
				return true;
			}

			GLMSG::SNET_GM_WARNING_MSG NetMsg;
			NetMsg.bOn = bOn;
			NETSEND ( &NetMsg );
		}
		//	Memo :	(이동속도설정 1~50까지만 설정가능)
		//			(공격속도설정 1~50까지만 설정가능)
		//			(공격력설정 1~50까지만 설정가능)



		else if( strCOMMAND=="/event_speed_begin" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			
			CString strPARAM_01 = strArray.GetAt(1);

			EMGM_EVENT_TYPE emType(EMGM_EVENT_NONE);
			WORD wValue = (WORD)atoi( strPARAM_01.GetString() );

			emType = EMGM_EVENT_SPEED;
			
			GLMSG::SNET_GM_EVENT_EX NetMsg;
			NetMsg.emType = emType;
			NetMsg.wValue = wValue;
			NETSEND ( &NetMsg );
		}

		else if( strCOMMAND=="/event_aspeed_begin" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			
			CString strPARAM_01 = strArray.GetAt(1);

			EMGM_EVENT_TYPE emType(EMGM_EVENT_NONE);
			WORD wValue = (WORD)atoi( strPARAM_01.GetString() );

			emType = EMGM_EVENT_ASPEED;
			
			GLMSG::SNET_GM_EVENT_EX NetMsg;
			NetMsg.emType = emType;
			NetMsg.wValue = wValue;
			NETSEND ( &NetMsg );
		}
		else if( strCOMMAND=="/event_attack_begin" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			
			CString strPARAM_01 = strArray.GetAt(1);

			EMGM_EVENT_TYPE emType(EMGM_EVENT_NONE);
			WORD wValue = (WORD)atoi( strPARAM_01.GetString() );

			emType = EMGM_EVENT_ATTACK;
			
			GLMSG::SNET_GM_EVENT_EX NetMsg;
			NetMsg.emType = emType;
			NetMsg.wValue = wValue;
			NETSEND ( &NetMsg );
		}

		else if( strCOMMAND=="/event_speed_end" )
		{
			if ( strArray.GetSize() != 1 )			return true;

			EMGM_EVENT_TYPE emType(EMGM_EVENT_NONE);

			emType = EMGM_EVENT_SPEED;

			GLMSG::SNET_GM_EVENT_EX_END NetMsg;
			NetMsg.emType = emType;
			NETSEND ( &NetMsg );
		}

		else if( strCOMMAND=="/event_aspeed_end" )
		{
			if ( strArray.GetSize() != 1 )			return true;

			EMGM_EVENT_TYPE emType(EMGM_EVENT_NONE);

			emType = EMGM_EVENT_ASPEED;

			GLMSG::SNET_GM_EVENT_EX_END NetMsg;
			NetMsg.emType = emType;
			NETSEND ( &NetMsg );
		}

		else if( strCOMMAND=="/event_attack_end" )
		{
			if ( strArray.GetSize() != 1 )			return true;

			EMGM_EVENT_TYPE emType(EMGM_EVENT_NONE);

			emType = EMGM_EVENT_ATTACK;

			GLMSG::SNET_GM_EVENT_EX_END NetMsg;
			NetMsg.emType = emType;
			NETSEND ( &NetMsg );
		}
		
		else if( strCOMMAND == "/login" )
		{
			if ( strArray.GetSize() != 2 )			return true;
			CString strPARAM_01 = strArray.GetAt(1);

			bool bOn(false);
			if ( strPARAM_01=="off" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Login: off" );
				bOn = false;
			}
			else if ( strPARAM_01=="on" )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Login : on" );
				bOn = true;
			}

			GLMSG::SNET_SERVER_LOGINMAINTENANCE_AGT NetMsg;
			NetMsg.bOn = bOn;
			NETSEND ( &NetMsg );
		}

//#ifdef _RELEASED // 소환수 내부에서만 되게 막음
		else if ( strCOMMAND=="/summon" )
		{
		
			if ( strArray.GetSize() != 3 ) return true;

			CString strPARAM_01 = strArray.GetAt(1);
			CString strPARAM_02 = strArray.GetAt(2);

			WORD wMainID = (WORD) atoi (strPARAM_01.GetString() );
			WORD wSubID = (WORD) atoi (strPARAM_02.GetString() );

			GLGaeaClient::GetInstance().ReqUseSummon( SNATIVEID( wMainID, wSubID ) );
		}
//#endif
		else
		{
			CString strPARAM_00 = strArray.GetAt(0);
			CInnerInterface::GetInstance().PrintConsoleText ( "invalid command : %s", strPARAM_00.GetString() );
		}

		return true;
	}
};

