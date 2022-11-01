#ifndef GLCHARDATA_H_
#define GLCHARDATA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <hash_map>

#include "./GLInventory.h"
#include "./GLItem.h"
#include "./GLQuestPlay.h"
#include "./GLItemMan.h"
#include "./GLSkill.h"
#include "./GLVEHICLE.h"

#include "../../EngineLib/Common/ByteStream.h"

enum EMCHARDATA
{
	//EMSKILLQUICK_VERSION	= 0x0101,
	//EMSKILLQUICK_SIZE		= 30,
	EMSKILLQUICK_SIZE_101	= 30,
	EMSKILLQUICK_VERSION	= 0x0102, //add f4
	EMSKILLQUICK_SIZE		= 40,//add f4
	EMACTIONQUICK_SIZE		= 6,

	SKILLREALFACT_SIZE		= 14,
	SKILLFACT_SIZE			= 14,

	EMMAX_CLUB_NUM			= 100,

	EMSTORAGE_CHANNEL				= 5,
	
	EMSTORAGE_CHANNEL_DEF			= 0,
	EMSTORAGE_CHANNEL_DEF_SIZE		= 1,

	EMSTORAGE_CHANNEL_SPAN			= 1,
	EMSTORAGE_CHANNEL_SPAN_SIZE		= 3,

	EMSTORAGE_CHANNEL_PREMIUM		= 4,
	EMSTORAGE_CHANNEL_PREMIUM_SIZE	= 1,
};

enum EMGM_EVENT_TYPE
{
	EMGM_EVENT_NONE		= 0x00,	//	����.
	EMGM_EVENT_SPEED	= 0x01,	//	�̵��ӵ�.
	EMGM_EVENT_ASPEED	= 0x02,	//	���ݼӵ�.
	EMGM_EVENT_ATTACK	= 0x04,	//	���ݷ�.
};

struct SCHARSKILL
{
	static DWORD VERSION;
	static DWORD SIZE;

	SNATIVEID	sNativeID;
	WORD		wLevel;

	SCHARSKILL () :
		sNativeID(SNATIVEID::ID_NULL,SNATIVEID::ID_NULL),
		wLevel(0)
	{
	}

	SCHARSKILL ( const SNATIVEID &_sNID, const WORD _wLvl ) :
		sNativeID(_sNID),
		wLevel(_wLvl)
	{
	}
};

struct DAMAGE_SPEC
{
	float		m_fPsyDamageReduce;				// ���� ������ ������
	float		m_fMagicDamageReduce;			// ���� ������ ������
	float		m_fPsyDamageReflection;			// ���� ������ �ݻ���
	float		m_fPsyDamageReflectionRate;		// ���� ������ �ݻ�Ȯ��
	float		m_fMagicDamageReflection;		// ���� ������ �ݻ���
	float		m_fMagicDamageReflectionRate;	// ���� ������ �ݻ�Ȯ��

	DAMAGE_SPEC()	:
		m_fPsyDamageReduce(0.0f),
		m_fMagicDamageReduce(0.0f),
		m_fPsyDamageReflection(0.0f),
		m_fPsyDamageReflectionRate(0.0f),
		m_fMagicDamageReflection(0.0f),
		m_fMagicDamageReflectionRate(0.0f)
	{
	}


	void RESET()
	{
		*this = DAMAGE_SPEC();
	}
};

struct DEFENSE_SKILL
{

	SNATIVEID	m_dwSkillID;					//	�ߵ��� ��ų MID/SID
	WORD		m_wLevel;						//	�ߵ��� ��ų ����
	float		m_fRate;						//	�ߵ��� ��ų MID/SID Ȯ��
	bool		m_bActive;						//	�ߵ��� ��ų ��������

	DEFENSE_SKILL() 
		: m_dwSkillID ( NATIVEID_NULL() )
		, m_wLevel ( 0 )
		, m_fRate ( 0.0f )
		, m_bActive(false)
	{
	}

	void RESET()
	{
		m_dwSkillID = NATIVEID_NULL();
		m_wLevel = 0;
		m_fRate = 0.0f;
	}
};

enum EMACTION_SLOT
{
	EMACT_SLOT_NONE	= 0,
	EMACT_SLOT_DRUG	= 1,
};

struct SACTION_SLOT
{
	enum { VERSION = 0x0100, };

	WORD		wACT;
	SNATIVEID	sNID;

	SACTION_SLOT () :
		wACT(EMACT_SLOT_NONE),
		sNID(false)
	{
	}

	bool VALID () const
	{
		return wACT!=EMACT_SLOT_NONE && sNID!=SNATIVEID(false);
	}

	void RESET ()
	{
		wACT = EMACT_SLOT_NONE;
		sNID = SNATIVEID(false);
	}
};

//	Note : �ɸ��� 1�� �׸�.
//
struct SCHARDATA
{
protected:
	DWORD				m_dwUserID;
public:
	void SetUserID( DWORD dwUserID) { m_dwUserID = dwUserID; }
	DWORD GetUserID() { return m_dwUserID; }

public:
	DWORD				m_dwUserLvl;				//	����� ���� Level.
	__time64_t			m_tPREMIUM;					//	�����̾� ����.
	bool				m_bPREMIUM;					//	���� �����̾� ����.
	__time64_t			m_tCHATBLOCK;				//	�Ӹ� ����.

	__time64_t			m_tSTORAGE[EMSTORAGE_CHANNEL_SPAN_SIZE];	//	â�� ����.
	bool				m_bSTORAGE[EMSTORAGE_CHANNEL_SPAN_SIZE];	//	â�� 2 ���.

	WORD				m_wINVENLINE;				//	�߰��� �κ��丮 �ټ�.
	DWORD				m_dwServerID;			
	DWORD				m_dwCharID;

	char				m_szName[CHAR_SZNAME];		//	�̸�. (����)

	EMTRIBE				m_emTribe;					//	����. (����)
	EMCHARCLASS			m_emClass;					//	����. (����)
	WORD				m_wSchool;					//	�п�.
	WORD				m_wSex;						//	����.
	WORD				m_wHair;					//	�Ӹ���Ÿ��.
	WORD				m_wHairColor;				//	�Ӹ�����
	WORD				m_wFace;					//	�󱼸��.

	int					m_nBright;					//	�Ӽ�.
	int					m_nLiving;					//	��Ȱ.
	
	WORD				m_wLevel;					//	����.
	
	DWORD				m_dwReborn;		// add reborn
	DWORD				m_dwPkWin;		//add pk
	DWORD				m_dwPkLoss;		//add pk

	LONGLONG			m_lnMoney;					//	���� �ݾ�.
	LONGLONG			m_lnVoteP;		//add money 2
	LONGLONG			m_lnPremP;		//add money 3
	LONGLONG			m_lnContribP;	//add contributionpoint by CNDev

	bool				m_bMoneyUpdate;				// ���� ������Ʈ �Ǿ����� �ƴ���
	bool				m_bStorageMoneyUpdate;		// â�� ���� ������Ʈ �Ǿ����� �ƴ���
	WORD				m_wTempLevel;				// �񱳿� ���� �ӽ� ����
	LONGLONG			m_lnTempMoney;				// �񱳿� ���� �ݾ� �ӽ� ����
	LONGLONG			m_lnTempStorageMoney;		// �񱳿� â��  �ݾ� �ӽ� ����
	LONGLONG			m_lVNGainSysMoney;			//  ��Ʈ�� Ž�� ���� ���� �ݾ�

	DWORD				m_dwGuild;					//	��� ��ȣ.
	char				m_szNick[CHAR_SZNAME];		//	����.
	__time64_t			m_tSECEDE;					//	Ż��ð�.

	SCHARSTATS			m_sStats;					//	Stats.
	DWORD				m_wStatsPoint;				//	���� stats ����Ʈ.

	DWORD				m_wAP;						//	�⺻ ���ݷ�.
	DWORD				m_wDP;						//	�⺻ ����.

	DWORD				m_wPA;						//	����ġ.
	DWORD				m_wSA;						//	���ġ.
	DWORD				m_wMA;						//	����ġ.

	GLLLDATA			m_sExperience;				//	����ġ. ( ����/�������뵵�ް� )
	LONGLONG			m_lnReExp;					//  ȸ���Ҽ� �ִ� ����ġ

	DWORD				m_dwSkillPoint;				//	Skill Point
	LONGLONG			m_lVNGainSysExp;			// ��Ʈ�� Ž�� ���� ���� ����ġ

	GLPADATA			m_sHP;						//	������. ( ����/�ִ뷮 )
	GLPADATA			m_sMP;						//	���ŷ�. ( ����/�ִ뷮 )
	GLPADATA			m_sSP;						//	�ٷ·�.	( ����/�ִ뷮 )
	GLPADATA			m_sCP;	//add cp

	WORD				m_wPK;						//	�� PK Ƚ��.

	bool				m_bEventBuster;				//	�̺�Ʈ ������ �ƴ���

	__time64_t			m_tLoginTime;				//  �α� �ð��̳� �̺�Ʈ ���۽ð�

	int					m_EventStartLv;				//  �̺�Ʈ �ּ� ����
	int					m_EventEndLv;				//  �̺�Ʈ �ִ� ����

	int					m_RemainEventTime;			//  �̺�Ʈ ������� ���� �ð�
	int					m_RemainBusterTime;			//  �ν��� �����ð�

	bool				m_bEventApply;				//  �̺�Ʈ ������ ����
	bool				m_bEventStart;				//  �̺�Ʈ �����ߴ��� �ƴ���, �̺�Ʈ �������

	int					m_EventStartTime;			//  �̺�Ʈ ���� ���� �ð�
	int					m_EventBusterTime;			//  �̺�Ʈ �ν��� Ÿ��

	LONGLONG			m_VietnamGameTime;			//	����� ���� �ð�
	BYTE				m_dwVietnamGainType;		//  ��Ʈ�� Ž�� ���� �ý��� 
	DWORD				m_dwVietnamInvenCount;		// �ű�� �ִ� �������� ����

    TCHAR				m_szPhoneNumber[SMS_RECEIVER]; // ĳ���� �� ��ȣ

	bool				m_bRankName;					//add pkrank
	bool				m_bRankMark;					//add pkrank
	bool				m_bAllowBet;					//add duel bet

	//add directval
	int				m_nHP_Potion_Rate;
	int				m_nMP_Potion_Rate;
	int				m_nSP_Potion_Rate;

	//add pk streak
	DWORD				m_dwPkCounter;
	float				m_fPkTimer;

	int				m_dwTDCounterSGNUC;
	int				m_dwTDCounterMPNUC;
	int				m_dwTDCounterPHXNUC;
	
	int				m_dwTDCounterSGFAC;
	int				m_dwTDCounterMPFAC;
	int				m_dwTDCounterPHXFAC;
	
	int				m_dwTDCounterSGTHE;
	int				m_dwTDCounterMPTHE;
	int				m_dwTDCounterPHXTHE;

	
	int				m_dwTDTotalSG;
	int				m_dwTDTotalMP;
	int				m_dwTDTotalPHX;
	DWORD               m_dwTickCount; //add autopots detection
	float               m_fTickDelay; //add autopots detection
	SCHARDATA () 
		: m_dwUserID(0)
		, m_dwUserLvl(0)
		, m_tPREMIUM(0)
		, m_bPREMIUM(false)
		, m_tCHATBLOCK(0)
		, m_wINVENLINE(0)
		, m_dwServerID(0)
		, m_dwCharID(0)
		, m_emTribe(TRIBE_HUMAN)
		, m_emClass(GLCC_FIGHTER_M)
		, m_wSchool(0)
		, m_wSex(0)
		, m_wHair(0)
		, m_wHairColor(0)
		, m_wFace(0)
		, m_nBright(1)
		, m_nLiving(0)
		, m_wLevel(0)
		, m_lnMoney(0)
		, m_lnVoteP(0) //add money 2
		, m_lnPremP(0) //add money 3
		, m_lnContribP(0) //add contributionpoint by CNDev
		, m_lVNGainSysMoney(0)
		, m_bMoneyUpdate(FALSE)
		, m_bStorageMoneyUpdate(FALSE)
		, m_wTempLevel(0)
		, m_lnTempMoney(0)
		, m_lnTempStorageMoney(0)
		, m_dwGuild(CLUB_NULL)
		, m_tSECEDE(0)
		, m_wStatsPoint(0)
		, m_wAP(0)
		, m_wDP(0)
		, m_wPA(0)
		, m_wSA(0)
		, m_wMA(0)
		, m_dwSkillPoint(0)
		, m_wPK(0)
		, m_EventStartTime(0)
		, m_EventBusterTime(0)
		, m_bEventBuster(FALSE)
		, m_bEventStart(FALSE)
		, m_tLoginTime(0)
		, m_EventStartLv(0)
		, m_EventEndLv(0)
		, m_bEventApply(FALSE)
		, m_RemainEventTime(0)
		, m_RemainBusterTime(0)
		, m_VietnamGameTime(0)
		, m_dwVietnamGainType(0)
		, m_lVNGainSysExp(0)
		, m_dwVietnamInvenCount(0)
		, m_lnReExp ( 0 )
		, m_dwReborn ( 0 )		//add reborn
		, m_dwPkWin( 0 )		//add pk
		, m_dwPkLoss ( 0 )		//add pk
		, m_bRankName(FALSE)	//add pkrank
		, m_bRankMark(FALSE)	//add pkrank
		, m_bAllowBet(FALSE)	//add duel bet
		,m_dwTDCounterSGNUC(0)
		,m_dwTDCounterMPNUC(0)
		,m_dwTDCounterPHXNUC(0)
	
		,m_dwTDCounterSGFAC(0)
		,m_dwTDCounterMPFAC(0)
		,m_dwTDCounterPHXFAC(0)
	
		,m_dwTDCounterSGTHE(0)
		,m_dwTDCounterMPTHE(0)
		,m_dwTDCounterPHXTHE(0)
		,m_dwTDTotalSG(0)
		,m_dwTDTotalMP(0)
		,m_dwTDTotalPHX(0)
		, m_nHP_Potion_Rate(0) //add directval
		, m_nMP_Potion_Rate(0)
		, m_nSP_Potion_Rate(0)
		//add pk streak
		, m_dwPkCounter(0)		
		, m_fPkTimer(0.0f)
		, m_fTickDelay(0.0f) //add autopots detection
		, m_dwTickCount(0) //add autopots detection
	{
		

		memset(m_szName, 0, sizeof(char) * CHAR_SZNAME);
		memset(m_szNick, 0, sizeof(char) * CHAR_SZNAME);
		memset(m_szPhoneNumber, 0, sizeof(TCHAR) * SMS_RECEIVER);

		for ( int i=0; i<EMSTORAGE_CHANNEL_SPAN_SIZE; ++i )	m_tSTORAGE[i] = 0;
		for ( int i=0; i<EMSTORAGE_CHANNEL_SPAN_SIZE; ++i )	m_bSTORAGE[i] = false;
	}

	SCHARDATA &GETCHARDATA ()			{ return *this; }
	EMCHARINDEX GETCHARINDEX () const	{ return CharClassToIndex ( m_emClass ); }

	bool IsKEEP_STORAGE ( DWORD dwCHANNEL );	//	�ش� ��ȣ�� ��Ŀ�� ������ �ñ�� �ִ��� �˻�.
	CTime GetStorageTime (  DWORD dwCHANNEL );
	WORD GetOnINVENLINE ();
	void CalcPREMIUM ();
};
typedef SCHARDATA* PCHARDATA;

typedef std::vector<ITEMSHOP>			VECITEMSHOP;

typedef std::map<std::string,ITEMSHOP>		ITEMMAPSHOP;
typedef ITEMMAPSHOP::iterator					ITEMMAPSHOP_ITER;

typedef std::map<DWORD,std::string>			ITEMMAPSHOP_KEY;
typedef ITEMMAPSHOP_KEY::iterator				ITEMMAPSHOP_KEY_ITER;

typedef std::vector<SHOPPURCHASE>			VECSHOP;

typedef std::map<std::string,SHOPPURCHASE>	MAPSHOP;
typedef MAPSHOP::iterator					MAPSHOP_ITER;

typedef std::map<DWORD,std::string>			MAPSHOP_KEY;
typedef MAPSHOP_KEY::iterator				MAPSHOP_KEY_ITER;


struct SCHARDATA2 : public SCHARDATA
{
	typedef stdext::hash_map<DWORD,SCHARSKILL>		SKILL_MAP;
	typedef SKILL_MAP::iterator						SKILL_MAP_ITER;
	typedef SKILL_MAP::const_iterator				SKILL_MAP_CITER;

	char					m_szUID[USR_ID_LENGTH+1];

	SKILL_MAP				m_ExpSkills;						//	���� ��ų �Ӽ�.
	SITEMCUSTOM				m_PutOnItems[SLOT_TSIZE];			//	���� Item.

	WORD					m_wSKILLQUICK_ACT;					//	��ų �������� ��Ƽ��� ��ų.
	SNATIVEID				m_sSKILLQUICK[EMSKILLQUICK_SIZE];	//	��ų ������.
	SACTION_SLOT			m_sACTIONQUICK[EMACTIONQUICK_SIZE];	//	�׼� ������.

	GLInventory				m_cInventory;						//	�κ��丮.

	BOOL					m_bServerStorage;					//	â�� ��ȿ��. ( ������. ) ( ���� �����Ǵ� ĳ���ʹ� ������ ��ȿ�ϰ� ������ üũ��. - GLCHARLOGIC::INIT_DATA() )
	LONGLONG				m_lnStorageMoney;					//	â�� ���� �ݾ�.
	BOOL					m_bStorage[EMSTORAGE_CHANNEL];		//	â�� ��ȿ��. ( Ŭ���̾�Ʈ��. )
	GLInventory				m_cStorage[EMSTORAGE_CHANNEL];		//	â��.

	GLQuestPlay				m_cQuestPlay;						//	����Ʈ.

	//	���� ����.
	MAPSHOP					m_mapCharged;						//	������ ������ ���.
	
	//	Ŭ���̾�Ʈ ����.
	GLInventory				m_cInvenCharged;					//	������ ������ �κ�.
	MAPSHOP_KEY				m_mapChargedKey;					//	������ ������ �κ��� �ش� ��ġ purkey ã��.

	ITEMMAPSHOP				m_mapItemShop;						//	������ ������ ���.
	
	GLInventory				m_cInvenItemShopVote[13];					//	������ ������ �κ�.
	ITEMMAPSHOP_KEY			m_mapItemShopVoteKey[13];					//	������ ������ �κ��� �ش� ��ġ purkey ã��.
	
	GLInventory				m_cInvenItemShopPremium[13];					//	������ ������ �κ�.
	ITEMMAPSHOP_KEY			m_mapItemShopPremiumKey[13];					//	������ ������ �κ��� �ش� ��ġ purkey ã��.

	GLInventory				m_cInvenItemShop[13];					//	������ ������ �κ�.
	ITEMMAPSHOP_KEY			m_mapItemShopKey[13];					//	������ ������ �κ��� �ش� ��ġ purkey ã��.

	//	Note : �ʱ� ���۵� ��, �� ��ġ��.
	//
	SNATIVEID			m_sStartMapID;				//	�ʱ� ���� ��.
	DWORD				m_dwStartGate;				//	�ʱ� ���� ����Ʈ.
	D3DXVECTOR3			m_vStartPos;				//	�ʱ� ���� ��ġ.

	SNATIVEID			m_sSaveMapID;				//	���� ��.
	D3DXVECTOR3			m_vSavePos;					//	���� ��ġ.

	SNATIVEID			m_sLastCallMapID;			//	������ȯ ��.
	D3DXVECTOR3			m_vLastCallPos;				//	������ȯ ��ġ.

	INT					m_dwThaiCCafeClass;			// �±� ���̹� ī�� 
	INT					m_nMyCCafeClass;			// �����̽þ� PC�� �̺�Ʈ 
	SChinaTime			m_sChinaTime;				// �߱� �ð��� ����
	SEventTime			m_sEventTime;				// �̺�Ʈ �ð�

	SVietnamGainSystem  m_sVietnamSystem;			// ��Ʈ�� Ž�й��� �ý���
	GLInventory			m_cVietnamInventory;		// ��Ʈ�� ������ Ž�� �κ��丮
	bool				m_bVietnamLevelUp;			// ��Ʈ�� ����ġ ȹ�� ������ ���� �����ܰ��� ������ ������ų �� �ִ�.

	// ������ ���� ����	// ITEMREBUILD_MARK
	BOOL				m_bRebuildOpen;					// ����â ���� ����
	SINVEN_POS			m_sRebuildCardPos;
	SINVEN_POS			m_sRebuildItem;					// ??? ??? ???? ???
	SINVEN_POS			m_sPreInventoryItem;			// ??? ?????? ???? ?? ???
	LONGLONG			m_i64RebuildCost;				// ??? ??? ??
	LONGLONG			m_i64RebuildInput;				// ??? ??? ??

	//sealed card NaJDeV
	SINVEN_POS			m_sRebuildSeal; 
	WORD				m_wSealType;

	bool				m_bTracingUser;			// ?? ???? ???? ???

	SNATIVEID			m_sSummonPosionID;		// ��ȯ�� ���� ������ ���̵�

	typedef std::map<DWORD,ITEM_COOLTIME>	COOLTIME_MAP;
	typedef COOLTIME_MAP::iterator			COOLTIME_MAP_ITER;
	typedef COOLTIME_MAP::const_iterator	COOLTIME_MAP_CITER;

	COOLTIME_MAP		m_mapCoolTimeType;		//	������ Ÿ�Ժ� ��Ÿ��
    COOLTIME_MAP		m_mapCoolTimeID;		//	������ MID/SID�� ��Ÿ��
	
	SCHARDATA2();
	void Assign ( SCHARDATA2 &CharData );

	EMCHARINDEX GETCHARINDEX () const { return CharClassToIndex ( m_emClass ); }

	BOOL SETEXPSKILLS_BYBUF ( CByteStream &ByteStream );
	BOOL GETEXPSKILLS_BYBUF ( CByteStream &ByteStream ) const;

	BOOL GETPUTONITEMS_BYBUF ( CByteStream &ByteStream ) const;
	
	BOOL SETSKILL_QUICKSLOT ( CByteStream &ByteStream );
	BOOL GETSKILL_QUICKSLOT ( CByteStream &ByteStream ) const;

	BOOL SETACTION_QUICKSLOT ( CByteStream &ByteStream );
	BOOL GETACTION_QUICKSLOT ( CByteStream &ByteStream ) const;


	BOOL SETINVENTORY_BYBUF ( CByteStream &ByteStream );
	BOOL GETINVENTORYE_BYBUF ( CByteStream &ByteStream ) const;

	BOOL SETSTORAGE_BYBUF ( CByteStream &ByteStream );
	BOOL GETSTORAGE_BYBUF ( CByteStream &ByteStream ) const;

	BOOL SETQUESTPLAY ( CByteStream &ByteStream );
	BOOL GETQUESTPLAY ( CByteStream &ByteStream ) const;

	BOOL SETSHOPPURCHASE ( VECSHOP &vecSHOP );
	BOOL SETITEMSHOP ( VECITEMSHOP &vecSHOP );

	// ��Ʈ�� Ž�й��� �ý��� �߰��� ���� ĳ���� �߰� �κ��丮 Ȯ��
	BOOL SETVTADDINVENTORY_BYBUF ( CByteStream &ByteStream );
	BOOL GETVTADDINVENTORYE_BYBUF ( CByteStream &ByteStream ) const;

	BOOL SETITEMCOOLTIME_BYBUF( CByteStream &ByteStream ); 
	BOOL GETITEMCOOLTIME_BYBUF( CByteStream &ByteStream ) const; 

public: //sealed card NaJDeV
	const SITEMCUSTOM& GET_REBUILD_SEAL(); //sealed card
	const WORD GET_SEAL_TYE() { return m_wSealType; }

public:
	BOOL LOADFILE ( const char* szFileName );

public:
	//	Ŭ���̾�Ʈ ����.
	BOOL ADDSHOPPURCHASE ( const char* szPurKey, SNATIVEID nidITEM );
	BOOL DELSHOPPURCHASE ( const DWORD dwID );

public:	// ITEMREBUILD_MARK
	const SITEMCUSTOM& GET_REBUILD_ITEM();
	const SITEMCUSTOM& GET_PREHOLD_ITEM();	// ??? ???? ???

public:	// ITEMREBUILD_MARK
	VOID InitRebuildData();
	VOID OpenRebuild()							{ m_bRebuildOpen = TRUE; }
	VOID CloseRebuild()							{ m_bRebuildOpen = FALSE; }
	const BOOL ValidRebuildOpen()				{ return m_bRebuildOpen; }
	const LONGLONG GetRebuildCost()				{ return m_i64RebuildCost; }
	const LONGLONG GetRebuildInput()			{ return m_i64RebuildInput; }

private:
	SCHARDATA2(const SCHARDATA2 &Inven )		{ GASSERT(0&&"������ ���� ����!"); }
	SCHARDATA2& operator= ( SCHARDATA2 &Inven )	{ GASSERT(0&&"������ ���� ����!"); return *this; }
};
typedef SCHARDATA2* PCHARDATA2;


struct SSTATEBLOW
{
	EMSTATE_BLOW	emBLOW;			//	���� �̻� ����.
	float			fAGE;			//	���� �ð�.
	float			fSTATE_VAR1;	//	���� �� 1.
	float			fSTATE_VAR2;	//	���� �� 2.

	SSTATEBLOW () :
		emBLOW(EMBLOW_NONE),
		fAGE(0),
		fSTATE_VAR1(0),
		fSTATE_VAR2(0)
	{

	}
};

struct SSKILLFACT
{
	SNATIVEID		sNATIVEID;		//	��ų ID.
	WORD			wLEVEL;			//	��ų ����.
	float			fAGE;			//	������ ���� �ð�.

	SKILL::EMTYPES	emTYPE;			//	�⺻ ����.
	float			fMVAR;			//	�⺻ ��ġ.

	EMIMPACT_ADDON	emADDON[SKILL::MAX_LEVEL];
	float			fADDON_VAR[SKILL::MAX_LEVEL];		

	EMSPEC_ADDON	emSPEC[SKILL::MAX_LEVEL];
	float			fSPECVAR1[SKILL::MAX_LEVEL];
	float			fSPECVAR2[SKILL::MAX_LEVEL];
	DWORD			dwSPECFLAG[SKILL::MAX_LEVEL];
	SNATIVEID		dwNativeID[SKILL::MAX_LEVEL];

	DWORD			dwSpecialSkill;		  // Ư����ų
	bool			bRanderSpecialEffect; // Ư�� ��ų ���� ����Ʈ�� �ߵ��Ǿ����� ����

	SSKILLFACT () :
		sNATIVEID(NATIVEID_NULL()),
		wLEVEL(0),
		fAGE(0),

		emTYPE(SKILL::EMFOR_VARHP),
		fMVAR(0),

		dwSpecialSkill(0),
		bRanderSpecialEffect(FALSE)

	{
		for(int i=0; i<SKILL::MAX_LEVEL;++i)
		{
			emADDON[i] = EMIMPACTA_NONE;
			fADDON_VAR[i] = 0.0f;

			emSPEC[i] = EMSPECA_NULL;
			fSPECVAR1[i] = 0.0f;
			fSPECVAR2[i]  = 0.0f;
			dwSPECFLAG[i] = NULL;
			dwNativeID[i] = NATIVEID_NULL();
		}
	}

	BOOL IsSpecialSkill ( DWORD dwState )		 		{ return dwSpecialSkill == dwState ? TRUE : FALSE ; }
	void SetSpecialSkill ( DWORD dwState )				{ dwSpecialSkill = dwState; }


	void RESET ();
};


struct SLANDEFFECT
{
	D3DXVECTOR2			vMinPos;
	D3DXVECTOR2			vMaxPos;
	EMLANDEFFECT_TYPE	emLandEffectType;
	float				fValue;

	SLANDEFFECT() :
		vMinPos( 0.0f, 0.0f ),
		vMaxPos( 0.0f, 0.0f ),
		emLandEffectType(EMLANDEFFECT_ATK_SPEED),
		fValue( 0.0f )
	{
	}

	void Init()
	{
		vMinPos			 = D3DXVECTOR2( 0.0f, 0.0f );
		vMaxPos			 = D3DXVECTOR2( 0.0f, 0.0f );
		emLandEffectType = EMLANDEFFECT_ATK_SPEED;
		fValue			 = 0.0f;
	}

	bool IsUse()
	{
		if( vMinPos		 	 == D3DXVECTOR2( 0.0f, 0.0f ) &&
			vMaxPos			 == D3DXVECTOR2( 0.0f, 0.0f ) &&
			emLandEffectType == EMLANDEFFECT_ATK_SPEED &&
			fValue			 == 0.0f ) return FALSE;
		return TRUE;
	}

	bool operator == ( const SLANDEFFECT &value )
	{
		if( vMaxPos			 != value.vMaxPos )			 return FALSE;
		if( vMinPos			 != value.vMinPos )			 return FALSE;
		if( emLandEffectType != value.emLandEffectType ) return FALSE;
		if( fValue		     != value.fValue )			 return FALSE;

		return TRUE;
	}

};

typedef std::vector<SLANDEFFECT> VEC_LANDEFF;
typedef VEC_LANDEFF::iterator	 VEC_LANDEFF_ITER;

struct SPASSIVE_SKILL_DATA
{
	short	m_nHP;
	short	m_nMP;
	short	m_nSP;
	short	m_nDAMAGE;
	short	m_nDEFENSE;
	short	m_nHIT;
	short	m_nAVOID;
	short	m_nPIERCE;
	float	m_fTARRANGE;
	float	m_fMOVEVELO;
	float	m_fATTVELO;
	float	m_fSKILLDELAY;
	float	m_fINCR_HP;
	float	m_fINCR_MP;
	float	m_fINCR_SP;
	float	m_fDAMAGE_RATE;
	float	m_fDEFENSE_RATE;
	DAMAGE_SPEC m_sDamageSpec;
	short	m_nPA;
	short	m_nSA;
	short	m_nMA;
	float	m_fHP_RATE;
	float	m_fMP_RATE;
	float	m_fSP_RATE;
	short	m_nCP;	//add cp
	float	m_fCP_GAIN; //add cp
	float	m_fCrit_Dmg; //add skill critdmg
	float	m_fBlow_Dmg; //add skill blowdmg
	float	m_fCrit_Rate; //add skill critrate
	float	m_fBlow_Rate; //add skill blowrate



	SRESIST	m_sSUMRESIST;					//	���װ�.

	SPASSIVE_SKILL_DATA () :
		m_nHP(0),
		m_nMP(0),
		m_nSP(0),
		m_fINCR_HP(0),
		m_fINCR_MP(0),
		m_fINCR_SP(0),
		m_nDAMAGE(0),
		m_nDEFENSE(0),
		m_nHIT(0),
		m_nAVOID(0),
		m_nPIERCE(0),
		m_fTARRANGE(0),
		m_fMOVEVELO(0),
		m_fATTVELO(0),
		m_fSKILLDELAY(0),
		m_fDAMAGE_RATE(0),
		m_fDEFENSE_RATE(0),
		m_nPA(0),
		m_nSA(0),
		m_nMA(0),
		m_fHP_RATE(0),
		m_fMP_RATE(0),
		m_fSP_RATE(0),
		m_nCP(0),
		m_fCP_GAIN(0),
		m_fCrit_Dmg(0), //add skill critdmg
		m_fBlow_Dmg(0), //add skill blowdmg
		m_fCrit_Rate(0), //add skill critrate
		m_fBlow_Rate(0) //add skill blowrate
	{
	}
};

struct SQITEM_FACT
{
	EMITEM_QUESTION	emType;
	float			fTime;
	WORD			wParam1;
	WORD			wParam2;

	SQITEM_FACT () :
		emType(QUESTION_NONE),
		fTime(0),
		wParam1(0),
		wParam2(0)
	{
	}

	bool IsACTIVE ()
	{
		return emType!=QUESTION_NONE;
	}

	void RESET ()
	{
		emType = QUESTION_NONE;
		fTime = 0;
		wParam1 = 0;
		wParam2 = 0;
	};
};



struct SEventState
{
	float				fItemGainRate;		// �̺�Ʈ �� ��ԵǴ� ������ �����
	float				fExpGainRate;		// �̺�Ʈ �� ��ԵǴ� ����ġ ����
	int					MinEventLevel;		// �̺�Ʈ �ּ� ����
	int					MaxEventLevel;		// �̺�Ʈ �ִ� ����
	bool				bEventStart;		// �̺�Ʈ�� ���۵ƴ��� �ƴ���
	int					EventPlayTime;		// �̺�Ʈ�� ���� �Ǵ� �÷��� �ð�
	int					EventBusterTime;	// �̺�Ʈ�� ���ӵǴ� �ð�
	CTime				EventStartTime;		// �̺�Ʈ�� ���� ������ �ð�
	DWORD				dwEventEndMinute;	// �̺�Ʈ ���� �ð�

	//add btg attack
	float				fAttackGainRate;


	SEventState()
	{
		Init();
	}
	void Init()
	{
		fItemGainRate    = 1.0f;
		fExpGainRate     = 1.0f;
		MinEventLevel    = 0;		// �̺�Ʈ �ּ� ����
		MaxEventLevel    = 0;		// �̺�Ʈ �ִ� ����
		bEventStart      = FALSE;			// �̺�Ʈ�� ���۵ƴ��� �ƴ���
		EventPlayTime    = 0;		// �̺�Ʈ�� ���� �ð�
		EventBusterTime  = 0;		// �̺�Ʈ�� ���ӵǴ� �ð�
		EventStartTime   = 0;
		dwEventEndMinute = 0;

		//add btg attack
		fAttackGainRate    = 1.0f;

	}
};

struct SEVENT_FACT
{
	INT				nType;
	WORD			wSpeed;
	WORD			wASpeed;
	WORD			wAttack;

	SEVENT_FACT () 
		: nType(EMGM_EVENT_NONE)
		, wSpeed(0)
		, wASpeed(0)
		, wAttack(0)
	{
	}

	bool IsACTIVE( EMGM_EVENT_TYPE emType )
	{
		return (nType&emType)!=EMGM_EVENT_NONE;
	}

	void SetEVENT( EMGM_EVENT_TYPE emType, WORD wValue )
	{
		nType |= emType;

		switch( emType )
		{
		case EMGM_EVENT_SPEED:	wSpeed=wValue;	break;
		case EMGM_EVENT_ASPEED:	wASpeed=wValue;	break;
		case EMGM_EVENT_ATTACK:	wAttack=wValue;	break;
		}
	}

	void ResetEVENT( EMGM_EVENT_TYPE emType )
	{
		nType &= ~emType;
	}
};

struct SDROP_STATEBLOW
{
	EMSTATE_BLOW	emBLOW;			//	���� �̻� ����.
	float			fAGE;			//	���� �ð�.
	float			fSTATE_VAR1;	//	���� �� 1.
	float			fSTATE_VAR2;	//	���� �� 2.

	SDROP_STATEBLOW () 
		: emBLOW(EMBLOW_NONE)
		, fAGE(0)
		, fSTATE_VAR1(0)
		, fSTATE_VAR2(0)
	{
	}

	SDROP_STATEBLOW& operator= ( SSTATEBLOW &sblow )
	{
		emBLOW = sblow.emBLOW;
		fAGE = sblow.fAGE;
		fSTATE_VAR1 = sblow.fSTATE_VAR1;
		fSTATE_VAR2 = sblow.fSTATE_VAR2;

		return *this;
	}
};

struct SDROP_SKILLFACT
{
	SNATIVEID		sNATIVEID;		//	��ų ID.
	WORD			wSLOT;			//	���� ��ġ.
	WORD			wLEVEL;			//	��ų LEVEL.
	float			fAGE;			//	������ ���� �ð�.

	SDROP_SKILLFACT () 
		: sNATIVEID(NATIVEID_NULL())
		, wSLOT(0)
		, wLEVEL(0)
		, fAGE(0)
	{
	}

	void Assign ( SSKILLFACT &sfact, WORD _wSLOT )
	{
		wSLOT = _wSLOT;
		sNATIVEID = sfact.sNATIVEID;
		wLEVEL = sfact.wLEVEL;
		fAGE = sfact.fAGE;
	}
};

struct SDROP_CHAR
{
	enum
	{
		CHAR_GEN	= 0x001,
		CLUB_CD		= 0x002
	};

	char			szName[CHAR_SZNAME];		//	�̸�.
	EMTRIBE			emTribe;					//	����.
	EMCHARCLASS		emClass;					//	����.
	WORD			wSchool;					//	�п�.
	WORD			wHair;						//	�Ӹ�ī��.
	WORD			wHairColor;					//  �Ӹ�ī�� �÷�
	WORD			wFace;						//	�󱼸��.
	WORD			wSex;						//  ����
	int				dwReborn;

	int				nBright;					//	�Ӽ�.

	DWORD			dwCharID;					//	�ɸ���ID.
	WORD			wLevel;						//	����.
	DWORD			dwGuild;					//	��� ��ȣ.
	DWORD			dwAlliance;					//	���� ��ȣ
	char			szClubName[CHAR_SZNAME];	//	Ŭ�� �̸�.
	DWORD			dwGuildMarkVer;				//	��� ��ũ ��ȣ.
	DWORD			dwGuildMaster;				//	��� ������.
	char			szNick[CHAR_SZNAME];		//	����.
	
	DWORD			dwParty;					//	��Ƽ ��ȣ.
	DWORD			dwPMasterID;				//	��Ƽ ������ ID.

	GLPADATA		sHP;						//	������. ( ����/�ִ뷮 )
	GLPADATA		sMP;
	GLPADATA		sSP;
	SRESIST			sSUMRESIST_SKILL;

	DWORD			dwGaeaID;					//	���� �޸� �ε�����.
	SNATIVEID		sMapID;						//	���� �� ID.
	DWORD			dwCeID;						//	�� ID.
	D3DXVECTOR3		vPos;						//	��ġ.
	D3DXVECTOR3		vDir;						//	��ġ.

	EMACTIONTYPE	Action;						//	���� �׼�.
	DWORD			dwActState;					//	���� �׼� �÷���.
	D3DXVECTOR3		vTarPos;					//	���� ��ǥ ��ġ.

	DWORD			dwSummonGUID;				//  ��ȯ�� ID

	SDROP_SKILLFACT	sSKILLFACT[SKILLFACT_SIZE];
	SDROP_STATEBLOW	sSTATEBLOWS[EMBLOW_MULTI];
	int				nLandEffect[EMLANDEFFECT_MULTI];	//  ���� �̻� ȿ����

	SQITEM_FACT		sQITEMFACT;
	SEVENT_FACT		sEVENTFACT;
	SEventState		sEventState;
	
	SITEMCLIENT		m_PutOnItems[SLOT_NSIZE_S_2];	//	���� Item.
	BOOL			m_bVehicle;
	CLIENT_VEHICLE	m_sVehicle;

	bool			m_bItemShopOpen;		// ItemShopOpen

	BOOL			m_bUseArmSub;				// �ذ��θ� ���� ���� ���� ��뿩��

	DWORD			dwFLAGS;					//	��Ÿ �Ӽ�.

	SPASSIVE_SKILL_DATA	sPASSIVE_SKILL;			//	passive skill data.
	
	DWORD			m_dwANISUBTYPE;				//	�ִϸ��̼� ����Ÿ��

	DWORD			m_dwPkWin; //add pkrank

	BOOL			m_bRankName;
	BOOL			m_bRankMark;

	DWORD			m_dwReborn;//����k�U

	SCHARSTATS			m_sStats;

	int					m_nSUM_HIT;
	int					m_nSUM_AVOID;
	GLPADATA			m_gdDAMAGE_PHYSIC;
	int					m_nDEFENSE_SKILL;

	DWORD				m_wSUM_AP;						//	�⺻ ���ݷ�.
	DWORD				m_wSUM_DP;						//	�⺻ ����.
	DWORD				m_wSUM_PA;						//	����ġ.
	DWORD				m_wSUM_SA;						//	���ġ.
	DWORD				m_wSUM_MA;						//	����ġ.

	DWORD				m_dwPkLoss;		//add pk

	LONGLONG			m_lnMoney;					//	���� �ݾ�.
	LONGLONG			m_lnVoteP;		//add money 2
	LONGLONG			m_lnPremP;		//add money 3
	LONGLONG			m_lnContribP;	//add contributionpoint by CNDev

	SDROP_CHAR () 
		: emTribe(TRIBE_HUMAN)
		, emClass(GLCC_FIGHTER_M)
		, dwReborn(0)
		, wSchool(0)
		, wHairColor(0)
		, wSex(0)
		, wHair(0)
		, wFace(0)
		, nBright(0)
		, dwCharID(0)
		, wLevel(1)
		, dwGuild(CLUB_NULL)
		, dwGuildMarkVer(0)
		, dwGuildMaster(0)
		, dwAlliance(0)
		, dwParty(PARTY_NULL)
		, dwPMasterID(GAEAID_NULL)
		, dwGaeaID(0)
		, dwCeID(0)
		, vPos(0,0,0)
		, vDir(0,0,-1)
		, Action(GLAT_IDLE)
		, dwActState(NULL)
		, vTarPos(0,0,0)
		, dwFLAGS(NULL)
		, m_bUseArmSub(FALSE)
		, m_bVehicle ( FALSE )
		, m_bItemShopOpen( false )
		, dwSummonGUID(GAEAID_NULL)
		, m_dwANISUBTYPE( 0 )
		, m_dwPkWin( 0 )
		, m_bRankName(FALSE)
		, m_bRankMark(FALSE)
		, m_dwReborn(0)
		, m_nSUM_HIT(0)
		, m_nSUM_AVOID(0)
		, m_wSUM_AP(0)
		, m_wSUM_DP(0)
		, m_wSUM_PA(0)
		, m_wSUM_SA(0)
		, m_wSUM_MA(0)
		, m_dwPkLoss(0)
		, m_lnMoney(0)
		, m_lnVoteP(0) //add money 2
		, m_lnPremP(0) //add money 3
		, m_lnContribP(0) //add contributionpoint by CNDev
	{
		for( int i=0; i < EMLANDEFFECT_MULTI; i++)
		{
			nLandEffect[i] = -1;
		}
		memset(szName, 0, sizeof(char) * CHAR_SZNAME);
		memset(szNick, 0, sizeof(char) * CHAR_SZNAME);
		memset(szClubName, 0, sizeof(char) * CHAR_SZNAME);
	}
public:
	int GETHIT () const				{ return m_nSUM_HIT; }
	int GETAVOID () const			{ return m_nSUM_AVOID; }
	int GETFORCE_LOW () const		{ return m_gdDAMAGE_PHYSIC.dwLow; }
	int GETFORCE_HIGH () const		{ return m_gdDAMAGE_PHYSIC.dwHigh; }
	int GETDEFENSE () const			{ return m_nDEFENSE_SKILL; }
};

#endif // GLCHARDATA_H_