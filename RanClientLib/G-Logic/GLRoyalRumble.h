#pragma once

#include "GLRoyalRumbleDefine.h"

#define SCHOOL_RANK_NUM		10
#define SCHOOL_NAME		15

class GLRoyalRumble
{
public:
	enum 
	{		
		MAX_TIME = 56,
	};

	enum EM_BATTLE_STATE 
	{
		BATTLE_NOSTART		= 0,
		BATTLE_READY		= 1,
		BATTLE_START		= 2,	
		BATTLE_END_AWARD	= 3,	
		BATTLE_END			= 4,	
	};

public:
	DWORD				m_dwID;		
	std::string			m_strName;		

	DWORD				m_dwClubMap;		
	DWORD				m_dwCLubMapGate;	
	DWORD				m_dwClubHallMap;	
	DWORD				m_dwClubHallGate;		

	DWORD				m_dwWinClub;	

	DWORD				m_dwBattleTime;
	SRR_TIME			m_sRrTIME[MAX_TIME];			

	SRRP_AWARD_ITEM		m_sRrPAwardItem;
	RRP_AWARD_CHAR		m_vecRrPAwardChar;
	RRP_RANK_INFO_MAP	m_mapRrPScore;

	bool				m_bNotifyOneHour;
	bool				m_bNotifyHalfHour;
	bool				m_bNotify10MinHour;

	EM_BATTLE_STATE		m_emBattleState;
	DWORD				m_dwBattleOrder;	
	DWORD				m_dwLastBattleHour;
	float				m_fBattleTimer;

//	DWORD				m_dwCERTIFY_CHARID;
//	DWORD				m_dwCERTIFY_NPCID;
//	float				m_fCERTIFY_TIMER;
//	D3DXVECTOR3			m_vCERTIFY_POS;

	float				m_fCHECK_TIMER;			//	��Ʋ ������ Ÿ�̸ӵ� ( ��ǰ�̶� ���� �ñ涧 ��� ) 
	float				m_fCHECK_TIME;			//

//	bool				m_bBattleEndCheck;		// ������ �������� ����� ������ ��������
//	float				m_fCHECK_TIMER;
//	float				m_fCHECK_TIME;


	float				m_fRankingUpdate;		//	��ŷ Ÿ�̸�

public:
	GLRoyalRumble ();

	GLRoyalRumble ( const GLRoyalRumble &value )
	{
		operator= ( value );
	}

	GLRoyalRumble& operator= ( const GLRoyalRumble& value );

	bool operator < ( const GLRoyalRumble& sRoyalRUMBLE )
	{
		return m_dwID < sRoyalRUMBLE.m_dwID;
	}

public:
	bool Load ( std::string strFile );

public:
	void AddRRPScore( DWORD dwKillChar, DWORD dwKillGaeaID, DWORD dwDeathChar, DWORD dwDeathGaeaID );
	SRRP_RANK_INFO* GetRRPRankInof( DWORD dwCharID );

public:
//	bool IsCertify()		{ return m_dwCERTIFY_CHARID!=0; }

public:
//	bool IsNewCommission ();
//	bool IsGuidClub ( DWORD dwClubID );

public:
	bool IsBattle () { return ( m_emBattleState == BATTLE_START ); }
	bool IsBattleReady() { return ( m_emBattleState == BATTLE_READY ); }
	bool IsBattleEndAward() { return ( m_emBattleState == BATTLE_END_AWARD); } //jdev test
	bool IsBattleEnd() { return ( m_emBattleState == BATTLE_END ); }
	bool IsEnterMap(DWORD dwCharID);
	DWORD IsBattleWeekDay ( int nDayOfWeek, int nHour );

	void SetBattle( EM_BATTLE_STATE emBattleState )	{ m_emBattleState = emBattleState; }
	
	// ����Ŭ�� ���۽ð��ΰ�?
	bool IsBattleHour ( DWORD dwORDER, int nHour );

public:
	// ���������� ����Ŭ���������� �Ͼ ���� �����Ѵ�.
	void UpdateBattleDay ( int nDay );
	// ����Ŭ�� ������ ���� �ð��� Ŭ���̾�Ʈ���� �˸���.
	void UpdateNotifyBattle ( DWORD dwORDER, int nHour, int nMinute );
//	void UpdateCommission ( float fElaps );

	//	���� Ŭ���� ����ǰ� ���� ������ ƨ����.
	void CheckExtraGuild ( float fElaps );
	bool IsCheckExtraGuild() { return true; }

	void CheckAwardClub( float fElaps );	//	���� ����
	void CheckExtraClub( float fElaps );	//	���� �������� 

public:
	// ����Ŭ�� �������� �����Ѵ�.
	void DoBattleStart ( DWORD dwORDER, int nDay );
	void DoBattleEnd();

//	void DoCertifyEnd();

public:
//	bool ChangeGuidClub ( DWORD dwClubID );
//	bool ChangeCommission ( float fRate );
};

class GLRoyalRumbleMan
{
public:
	typedef std::vector<GLRoyalRumble>			RR_VEC;
	typedef RR_VEC::iterator						RR_VEC_ITER;
	typedef std::pair<RR_VEC_ITER,RR_VEC_ITER>	RR_VEC_PAIR;

protected:
	RR_VEC											m_vecRoyalRUMBLE;

public:
	bool Load ( std::vector<std::string> &vecFiles );
//	bool SetState ( VECGUID_DB &vecGUID );

public:
	GLRoyalRumble* Find ( DWORD dwID );
	bool IsBattle ();
//	bool IsCheckExtraGuild ();

public:
	GLRoyalRumbleMan ()
	{
	}

public:
	//static GLGuidanceMan& GetInstance();
};

class GLRoyalRumbleAgentMan : public GLRoyalRumbleMan
{
protected:
	float m_fRemainTimer, m_fTimer;

public:
//	bool ChangeGuidClub ( DWORD dwID, DWORD dwClubID );
//	bool ChangeCommission ( DWORD dwID, float fRate );

public:
//	GLClubDeathMatch* FindByClubID ( DWORD dwClubID );

public:
	bool SetMapState ();

public:
//	DWORD GetGuidID ( DWORD dwClubID );
	DWORD GetRemainTime () { return (DWORD)m_fRemainTimer; }

public:
	bool FrameMove ( float fElapsedAppTime );

public:
	GLRoyalRumbleAgentMan ()
		: m_fRemainTimer(0)
		, m_fTimer(0)
	{
	}

public:
	static GLRoyalRumbleAgentMan& GetInstance();
};

class GLRoyalRumbleFieldMan : public GLRoyalRumbleMan
{
public:
	bool SetMapState ();

public:
	bool ReadyBattle( DWORD dwID );
	bool BeginBattle ( DWORD dwID );
	bool EndBattle ( DWORD dwID );

public:
//	bool ChangeGuidClub ( DWORD dwID, DWORD dwClubID );
//	bool ChangeCommission ( DWORD dwID, float fRate );

//	bool DoCertify ( DWORD dwID, DWORD dwCHARID, DWORD dwNPCID, const D3DXVECTOR3 &vPOS );

public:
	DWORD GetWinClubID ( DWORD dwCharID );
//	EMCHECKCERTIFY CheckCertify ( DWORD dwID, DWORD dwCHARID );
	const std::string GetName ( DWORD dwID );

//	void CheckExtraGuild ( float fElaps );

	void UpdateRRPRanking( DWORD dwID );
	void RRAwardItem( DWORD dwID );

public:
	bool FrameMove ( float fElaps );

protected:
	GLRoyalRumbleFieldMan ()
	{
	}

public:
	static GLRoyalRumbleFieldMan& GetInstance();
};