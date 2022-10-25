#include "StdAfx.h"

#include <algorithm>

#include "GLDefine.h"
#include "gltexfile.h"
#include "IniLoader.h"

#include "GLTowerWars.h"

#include "GLLandMan.h"
#include "GLGaeaServer.h"
#include "GLAgentServer.h"

GLTowerWarsAgentMan& GLTowerWarsAgentMan::GetInstance()
{
	static GLTowerWarsAgentMan cInstance;
	return cInstance;
}

bool GLTowerWarsAgentMan::SetMapState ()
{
	
	for ( SW_VEC::size_type i=0; i<m_vecSchoolWAR.size(); ++i )
	{
		GLTowerWars &sSW= m_vecSchoolWAR[i];

		SNATIVEID nidMAP ( sSW.m_dwClubMap );
		GLAGLandMan *pLandMan = GLAgentServer::GetInstance().GetByMapID ( nidMAP );
		if ( pLandMan )
		{
			pLandMan->m_dwTowerWarsMapID = sSW.m_dwID;
			pLandMan->m_bTowerWarsMap = true;
		}
	}

	return true;
}

bool GLTowerWarsAgentMan::ChangeSWSchoolID ( DWORD dwID, DWORD dwSwID )
{
	GLTowerWars *pGuid = Find ( dwID );
	if ( !pGuid )				return false;

	pGuid->ChangeSWSchoolID ( dwSwID );

	return true;
}

bool GLTowerWarsAgentMan::EndBattle ( DWORD dwID, WORD wSchool, bool bEnd )
{
	GLTowerWars *pGuid = Find ( dwID );
	if ( !pGuid )				return false;

	pGuid->EndBattle( wSchool, bEnd );
	pGuid->ChangeSWSchoolID ( wSchool );

	return true;
}

bool GLTowerWarsAgentMan::FrameMove ( float fElapsedAppTime )
{
	if ( GLCONST_CHAR::bPKLESS ) return false;

	for ( SW_VEC::size_type i=0; i<m_vecSchoolWAR.size(); ++i )
	{
		GLTowerWars &sSchoolWAR = m_vecSchoolWAR[i];

		CTime cCurTime = CTime::GetCurrentTime();
		int nDayOfWeek = cCurTime.GetDayOfWeek ();
		int nDay = cCurTime.GetDay ();
		int nHour = cCurTime.GetHour();
		int nMinute = cCurTime.GetMinute ();

		DWORD dwORDER = sSchoolWAR.IsBattleWeekDay(nDayOfWeek,nHour);
		if ( dwORDER!=UINT_MAX )
		{
			if ( sSchoolWAR.IsBattleHourMin ( dwORDER, nHour, nMinute ) )
			{
				sSchoolWAR.DoBattleStart ( dwORDER, nHour );
				m_fRemainTimer = (float)m_vecSchoolWAR[i].m_dwBattleTime;
				m_fTimer = 0.0f;
			}
			else
			{
				sSchoolWAR.UpdateNotifyBattle ( dwORDER, nHour, nMinute );
			}
		}

		if ( sSchoolWAR.IsBattle() )
		{
			sSchoolWAR.m_fBattleTimer += fElapsedAppTime;
			
			if ( sSchoolWAR.bBattleEnd || sSchoolWAR.m_fBattleTimer > float(sSchoolWAR.m_dwBattleTime) )	
			{ 
				sSchoolWAR.DoBattleEnd( true ); 
				m_fRemainTimer = 30.0f; 
			}
		}
	}

	if ( !m_vecSchoolWAR.empty() )
	{
		for( SW_VEC::size_type i=0;i<m_vecSchoolWAR.size();i++)
		{
			GLTowerWars &sSchoolWAR = m_vecSchoolWAR[i];

			if ( m_vecSchoolWAR[i].IsBattle() )
			{
				GLMSG::SNET_TOWERWARS_REMAIN_BRD NetMsgBrd;

				if ( m_fRemainTimer == m_vecSchoolWAR[i].m_dwBattleTime )
				{
					NetMsgBrd.dwTime = (DWORD)m_fRemainTimer;
					GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );
				}

				m_fTimer += fElapsedAppTime;
				m_fRemainTimer -= fElapsedAppTime;

				if ( m_fTimer > 600.0f )
				{
					NetMsgBrd.dwTime = (DWORD)m_fRemainTimer;
					GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );
					m_fTimer = 0.0f;
				}
			}

			if( m_vecSchoolWAR[i].IsForceExit() )
			{
				GLMSG::SNET_TOWERWARS_REMAIN_BRD NetMsgBrd;

				m_fRemainTimer -= fElapsedAppTime;
				NetMsgBrd.dwTime = (DWORD)m_fRemainTimer;
				GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );

				std::string strClubName;
				switch( m_vecSchoolWAR[i].m_dwWinClub )
				{
					case 0:	strClubName = "SG";		break;
					case 1: strClubName = "MP";		break;
					case 2:	strClubName = "PHX";	break;
					default:strClubName = "none";	break;
				}
				
				CString strTEXT;
				strTEXT.Format ( strClubName.c_str() );

				if( m_fRemainTimer <= 0.0f )
				{
					m_vecSchoolWAR[i].SetForceExit( GLTowerWars::FORCE_EXIT_NO );

					GLMSG::SNET_TOWERWARS_END_FLD NetMsgFld;
					NetMsgFld.dwID = m_vecSchoolWAR[i].m_dwID;
					NetMsgFld.bForceExit = true;
					GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );
				}
			}
		}
	}

	return true;
}