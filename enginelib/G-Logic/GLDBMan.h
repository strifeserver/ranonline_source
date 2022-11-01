#ifndef GLDBMAN_H_
#define GLDBMAN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gassert.h"
#include <vector>

#include "../../NetClientLib/s_NetGlobal.h" // struct VIEWLOGITEMEXCHANGEMAX; struct SHOPPURCHASE;
// #include "../../NetServerLib/s_CDbAction.h" // class CDbAction;

#include "../../EngineLib/Common/ByteStream.h" // class CByteStream;

#include "../../RanClientLib/G-Logic/GLCharData.h" // struct SCHARDATA2;
#include "../../RanClientLib/G-Logic/GLClubMan.h" // struct GLCLUBMEMBER; struct GLCLUBINFO;
#include "../../RanClientLib/G-Logic/GLGuidance.h" // struct GLGUID_DB;
//add sw buff
#include "../../RanClientLib/G-Logic/GLSchoolWars.h" // struct GLSWID;
#include "../../RanClientLib/G-Logic/GLPet.h" //struct GLPET;


// struct VIEWLOGITEMEXCHANGEMAX;
// struct SHOPPURCHASE;
// struct SCHARDATA2;
// struct GLCLUBMEMBER;
// struct GLCLUBINFO;
// struct GLGUID_DB;
// struct GLPET;

// class CByteStream;
class CDbAction;

struct SLOGITEMEXCHANGE
{
	int			m_nNID_M;			// ������ �ֹ�ȣ
	int			m_nNID_S;			// ������ �ι�ȣ
	int			m_nSGNum;			// �����׷�
	int			m_nSvrNum;			// ������ȣ
	int			m_nFldNum;			// �ʵ��ȣ
	int			m_nMakeType;		// Ÿ��
	LONGLONG	m_lnMakeNum;		// ������ȣ
	int			m_nFromType;		// from�� ���� ( ����, �ɸ���, Ŭ�� )
	int			m_nItemFrom;		// ������ �����
	int			m_nToType;			// to�� ���� ( ����, �ɸ���, Ŭ�� )
	int			m_nItemTo;			// ������ ������
	int			m_nExchangeFlag;	// ��ȯ Ÿ��
	int			m_nNum;

	//	�ڽ��� ��ȯ ����.
	int			m_nCOSTUME_MID;
	int			m_nCOSTUME_SID;

	//	���� ����.
	short		m_nDamage;
	short		m_nDefense;
	short		m_nFire;
	short		m_nIce;
	short		m_nPoison;
	short		m_nElectric;
	short		m_nSpirit;

	SLOGITEMEXCHANGE(
		int _nNID_M,
		int _nNID_S,
		int _nSGNum,
		int _nSvrNum,
		int _nFldNum,
		int _nMakeType,
		LONGLONG _lnMakeNum,
		int _nFromType,
		int _nItemFrom,
		int _nToType,
		int _nItemTo,
		int _nExchangeFlag,
		int _nNum,

		int _nCOSTUME_MID,
		int _nCOSTUME_SID,

		short _nDamage,
		short _nDefense,
		short _nFire,
		short _nIce,
		short _nPoison,
		short _nElectric,
		short _nSpirit	) 
		: m_nNID_M(_nNID_M)
		, m_nNID_S(_nNID_S)
		, m_nSGNum(_nSGNum)
		, m_nSvrNum(_nSvrNum)
		, m_nFldNum(_nFldNum)
		, m_nMakeType(_nMakeType)
		, m_lnMakeNum(_lnMakeNum)
		, m_nFromType(_nFromType)
		, m_nItemFrom(_nItemFrom)
		, m_nToType(_nToType)
		, m_nItemTo(_nItemTo)
		, m_nExchangeFlag(_nExchangeFlag)
		, m_nNum(_nNum)
		, m_nCOSTUME_MID(_nCOSTUME_MID)
		, m_nCOSTUME_SID(_nCOSTUME_SID)
		, m_nDamage(_nDamage)
		, m_nDefense(_nDefense)
		, m_nFire(_nFire)
		, m_nIce(_nIce)
		, m_nPoison(_nPoison)
		, m_nElectric(_nElectric)
		, m_nSpirit(_nSpirit)
	{
	}

	SLOGITEMEXCHANGE () 
		: m_nNID_M(0)
		, m_nNID_S(0)
		, m_nSGNum(0)
		, m_nSvrNum(0)
		, m_nFldNum(0)
		, m_nMakeType(0)
		, m_lnMakeNum(0)
		, m_nFromType(0)
		, m_nItemFrom(0)
		, m_nToType(0)
		, m_nItemTo(0)
		, m_nExchangeFlag(0)
		, m_nNum(0)
		, m_nCOSTUME_MID(0)
		, m_nCOSTUME_SID(0)
		, m_nDamage(0)
		, m_nDefense(0)
		, m_nFire(0)
		, m_nIce(0)
		, m_nPoison(0)
		, m_nElectric(0)
		, m_nSpirit(0)
	{
	}
};

struct SLOGMONEYEXCHANGE
{
	int			m_nSGNum;
	int			m_nSvrNum;
	int			m_nFldNum;
	int			m_nFromType;
	int			m_nItemFrom;
	int			m_nToType;
	int			m_nItemTo;
	LONGLONG	m_lnPrice;
	int			m_nExchangeFlag;

	SLOGMONEYEXCHANGE () 
		: m_nSGNum(0)
		, m_nSvrNum(0)
		, m_nFldNum(0)
		, m_nFromType(0)
		, m_nItemFrom(0)
		, m_nToType(0)
		, m_nItemTo(0)
		, m_lnPrice(0)
		, m_nExchangeFlag(0)
	{
	}

	SLOGMONEYEXCHANGE (
		int _nSGNum,
		int _nSvrNum,
		int _nFldNum,

		int _nFromType,
		int _nItemFrom,
		
		int _nToType,
		int _nItemTo,

		LONGLONG _lnPrice,
		int _nExchangeFlag
		) 
		: m_nSGNum(_nSGNum)
		, m_nSvrNum(_nSvrNum)
		, m_nFldNum(_nFldNum)
		, m_nFromType(_nFromType)
		, m_nItemFrom(_nItemFrom)
		, m_nToType(_nToType)
		, m_nItemTo(_nItemTo)
		, m_lnPrice(_lnPrice)
		, m_nExchangeFlag(_nExchangeFlag)
	{
	}
};

struct SLOGITEMCONVERSION
{
	int			m_nNIDMain;
	int			m_nNIDSub;
	int			m_nSGNum;
	int			m_nSvrNum;
	int			m_nFldNum;
	int			m_nMakeType;
	LONGLONG	m_lnMakeNum;

	int			m_nFromType;
	int			m_nItemFrom;

	int			m_nCOSTUME_MID;
	int			m_nCOSTUME_SID;

	short		m_nDamage;
	short		m_nDefense;
	short		m_nFire;
	short		m_nIce;
	short		m_nPoison;
	short		m_nElectric;
	short		m_nSpirit;

	DWORD		m_dwOwnerID;//add ownership
	WORD		m_wColor;//add itemcolor

	SLOGITEMCONVERSION () 
		: m_nNIDMain(0)
		, m_nNIDSub(0)
		, m_nSGNum(0)
		, m_nSvrNum(0)
		, m_nFldNum(0)
		, m_nMakeType(0)
		, m_lnMakeNum(0)
		, m_nFromType(0)
		, m_nItemFrom(0)
		, m_nCOSTUME_MID(0)
		, m_nCOSTUME_SID(0)
		, m_nDamage(0)
		, m_nDefense(0)
		, m_nFire(0)
		, m_nIce(0)
		, m_nPoison(0)
		, m_nElectric(0)
		, m_nSpirit(0)
		, m_dwOwnerID(0)//add ownership
		, m_wColor(0)//add itemcolor
	{
	}

	SLOGITEMCONVERSION (
		int _nNIDMain,
		int _nNIDSub,
		int _nSGNum,
		int _nSvrNum,
		int _nFldNum,
		int _nMakeType,
		LONGLONG _lnMakeNum,

		int _nFromType,
		int _nItemFrom,

		int _nCOSTUME_MID,
		int _nCOSTUME_SID,

		short _nDamage,
		short _nDefense,
		short _nFire,
		short _nIce,
		short _nPoison,
		short _nElectric,
		short _nSpirit,
		DWORD _dwOwnerID,//add ownership
		WORD _wColor )//add itemcolor
		: m_nNIDMain(_nNIDMain)
		, m_nNIDSub(_nNIDSub)
		, m_nSGNum(_nSGNum)
		, m_nSvrNum(_nSvrNum)
		, m_nFldNum(_nFldNum)
		, m_nMakeType(_nMakeType)
		, m_lnMakeNum(_lnMakeNum)
		, m_nFromType(_nFromType)
		, m_nItemFrom(_nItemFrom)
		, m_nCOSTUME_MID(_nCOSTUME_MID)
		, m_nCOSTUME_SID(_nCOSTUME_SID)
		, m_nDamage(_nDamage)
		, m_nDefense(_nDefense)
		, m_nFire(_nFire)
		, m_nIce(_nIce)
		, m_nPoison(_nPoison)
		, m_nElectric(_nElectric)
		, m_nSpirit(_nSpirit)
		, m_dwOwnerID(_dwOwnerID)//add ownership
		, m_wColor( _wColor ) //add itemcolor
	{
	}
};

struct SLOGACTION
{
	int			m_nCHARID;
	int			m_nTYPE;

	int			m_nTARTYPE;
	int			m_nTARID;

	__int64		m_nEXP;
	int			m_nBRIGHT_POINT;
	int			m_nLIFE_POINT;
	int			m_nMONEY;

	SLOGACTION () 
		: m_nCHARID(0)
		, m_nTYPE(0)
		, m_nTARTYPE(0)
		, m_nTARID(0)
		, m_nEXP(0)
		, m_nBRIGHT_POINT(0)
		, m_nLIFE_POINT(0)
		, m_nMONEY(0)
	{
	}
};

/**
* �������� �����ɼ� �α׸� ����� ���� ����ü.
*/
struct SLOGRANDOMOPTION
{
	// ������ ������
	int nNIDMain; // �����۰�����ȣ MID
	int nNIDSub;  // �����۰�����ȣ SID
	int nSGNum;   // �����׷�
	int nSvrNum;  
	int nFldNum;  // �ʵ��ȣ
	int nMakeType; // �߻�Ÿ��
	LONGLONG lnMakeNum; // �����۰�����ȣ

	//	���� �ɼ� ����
	BYTE cOptTYPE1; // ���� �ɼ� ���� #1
	BYTE cOptTYPE2; // ���� �ɼ� ���� #2
	BYTE cOptTYPE3; // ���� �ɼ� ���� #3
	BYTE cOptTYPE4; // ���� �ɼ� ���� #4

	// ���� �ɼ� ��
	short nOptVALUE1; // % #1
	short nOptVALUE2; // % #2
	short nOptVALUE3; // % #3
	short nOptVALUE4; // % #4

	SLOGRANDOMOPTION()
		: nNIDMain(0)
		, nNIDSub(0)
		, nSGNum(0)
		, nSvrNum(0)
		, nFldNum(0)
		, nMakeType(0)
		, lnMakeNum(0)
		, cOptTYPE1(0) 
		, cOptTYPE2(0) 
		, cOptTYPE3(0) 
		, cOptTYPE4(0) 
		, nOptVALUE1(0)
		, nOptVALUE2(0)
		, nOptVALUE3(0)
		, nOptVALUE4(0)
	{
	}
};

/**
* �� �׼� �α׸� ����� ���� ����ü.
*/
struct SLOGPETACTION 
{
	int nPetNum;		// �� �ѹ�
	int nItemMID;		// ������ MID
	int nItemSID;		// ������ SID
	int nActionType;	// �� �׼� Ÿ��
	int nPetFull;		// �� ������

	SLOGPETACTION()
		: nPetNum(0)
		, nItemMID(0)
		, nItemSID(0)
		, nActionType(0)
		, nPetFull(0)
	{
	}
};

struct SLOGVEHICLEACTION 
{
	int nVehicleNum;		// �� �ѹ�
	int nItemMID;		// ������ MID
	int nItemSID;		// ������ SID
	int nActionType;	// �� �׼� Ÿ��
	int nVehicleFull;		// �� ������

	SLOGVEHICLEACTION()
		: nVehicleNum(0)
		, nItemMID(0)
		, nItemSID(0)
		, nActionType(0)
		, nVehicleFull(0)
	{
	}
};

class GLDBMan
{
public:
	virtual int SaveCharacter( LPVOID _pbuffer ) = 0;
	virtual int	ReadUserInven( SCHARDATA2* pChaData2 ) = 0;

	virtual int DelChaFriend( int nChaP, int nChaS )				{ return 0; };
	virtual int AddChaFriend( int nChaP, int nChaS )				{ return 0; };
	virtual int SetChaFriend( int nChaP, int nChaS, int nFlag )	{ return 0; };

	virtual int LogPartyMatch( int nSGNum, int nSvrNum, WORD wWin, WORD wLost )			{ return 0; }

    virtual LONGLONG GetItemMaxNum(
						int nSGNum,
						int nSvrNum,
						int nFldNum, 
                        std::vector<VIEWLOGITEMEXCHANGEMAX> &v )             { return 0; }

	virtual int LogItemExchange( const SLOGITEMEXCHANGE &sEXCHANGE )					{ return 0; }
	virtual int LogMoneyExchange( const SLOGMONEYEXCHANGE &_sLOG )						{ return 0; }
    virtual int LogItemConversion( const SLOGITEMCONVERSION &_sLOG )					{ return 0; }
    virtual int LogAction( const SLOGACTION &_sLOG )									{ return 0; }
    
	/**
	* Shop ���� ������ �������� �����´�.
	* \param strUID ����ID
	* \param &v ��ǰ�� ���� ����
	* \return 
	*/
    virtual int GetPurchaseItem( CString strUID, std::vector<SHOPPURCHASE> &v ) { return 0; } 
	virtual int GetItemShop( std::vector<ITEMSHOP> &v ) { return 0; } 
	/**
	* ������ ������ �� �ִ� ��ǰ���� Ȯ���Ѵ�.
	* \param strPurKey �����ڵ�
	* \param nFlag (0 : ����, 1 : ���ſϷ�, 2 : ������ҽ�û, 3 : �������ó��)
	* \return 1:�������� ���� �̿��ǰ�:�������� ����
	*/
    virtual int SetPurchaseItem( CString strPurKey, int nFlag=1 ) { return 0; }
	virtual int SetItemShop( CString strItemNum, DWORD dwUserID ) { return 0; }
	/**
	* ĳ���� ����, ĳ���ͻ��� ���� ����
	*/
	virtual int UserChaNumIncrease( int nUserNum ) { return 0; }

	/**
	* �ش� ��Ŀ ����ð��� ���Ѵ�.
	* \n 1 �� 5 �� ��Ŀ�� ���� �� ����.
	* \n 2, 3, 4 �� ��Ŀ�� ����ð��� ���� �� �ִ�.
	* \param nUserNum ����ڹ�ȣ
	* \param nStorageNum ���丮����ȣ
	* \param tTime ����ð�
	* \return -1:DB_ERROR  0:DB_OK
	*/
    virtual int SetChaStorageDate(
					int nUserNum,
					int nStorageNum,
					__time64_t tTime ) { return 0; }
    
	/**
	* �ش� ĳ������ �κ��丮 �߰� �ټ��� �����Ѵ�.
	* \n ���� 0 ���� ���� �߰��� 1 �� �Է�
    * \n ���� ���� ���¿��� �� ���� �߰��� 2 �� �Է� (���� �ټ��� �Է��Ѵ�)
    * \n ���� ���� ���¿��� ���� �߰��� 3 �� �Է�
    * \n �ִ� 3������ �Է°���.
	* \param nChaNum ĳ���͹�ȣ
	* \param wInvenLine �߰��ټ�
	* \return 
	*/
    virtual int SetChaInvenNum(
					int nChaNum,
					WORD wInvenLine ) { return 0; }
    
	/**
	* �ش� ������� �����̾� �Ⱓ�� �����Ѵ�.
	*/
    virtual int SetPremiumTime(
					int nUserNum,
					__time64_t tPremiumTime ) { return 0; }
    
	/**    
	* ������ȯ ī�� ����� ���� �������� �����Ѵ�.
	*/
    virtual int SetLastCallPos(
					int nChaNum,
					DWORD dwMapID,
					D3DXVECTOR3 vPos ) { return 0; }
    
	/**
	* ���ο� ĳ���͸� �����Ѵ�.
	*/
    virtual int	CreateNewCharacter(SCHARDATA2* pCharData2) { return 0; }

    /**
    * ���ο� Ŭ���� �����Ѵ�.
    * �Է�
    *     strClubName : Ŭ���̸�
    *     dwChaNum : Ŭ���� �����Ϸ��� ĳ���� ��ȣ (�� ĳ���Ͱ� Ŭ���� �����Ͱ� �ȴ�)
    * ���
    *     -1 : �̹� ��帶���ͷ� ��ϵǾ� ���� �����Ұ�.
    *     -2 : �������� �����߻� (�ߺ��ȱ���̸�)
    *      0 ���� Ŭ�� : ������ ����ȣ��
	*/
    virtual int CreateClub(CString strClubName, DWORD dwChaNum) { return 0; }
    
    /**
    * Ŭ���� �����Ѵ�.
    * dwClub : Ŭ����ȣ
    * dwChaNum : Ŭ���� �����Ϸ��� ĳ���͹�ȣ(ĳ���� ��ȣ�� Ŭ�� �����Ͱ� �ƴϸ� ������ ���� �ʴ´�)
	*/
    virtual int DeleteClub(DWORD dwClub, DWORD dwChaNum)                                  { return 0; }
    
    /**
    * Ŭ����ũ�� �����Ѵ�.
    * dwClub : Ŭ����ȣ
    * dwRank : ��ŷ
	*/
    virtual int SetClubRank(DWORD dwClub, DWORD dwRank)                                   { return 0; }

    virtual int ReadClubMarkImage(
					DWORD dwClub, 
					CByteStream &ByteStream ) { return 0; }

    virtual int WriteClubMarkImage(
					DWORD dwClub,
					DWORD dwMarkVer,
					BYTE* pData,
					int nSize ) { return 0; }
    
	/**
	* Ŭ�� ��ü�ð��� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDiss ��ü�ð�
	* \return 
	*/
    virtual int SetClubDissolutionTime(DWORD dwClub, __time64_t tDiss)                    { return 0; }

	/**
	* Ŭ���� ����Ż�� Ȥ�� �����ð��� ���Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tSec �ð�
	* \return 
	*/
	virtual int SetClubAllianceSec(DWORD dwClub, __time64_t tSec) { return 0; }

	/**
	* Ŭ���� �����ػ� �ð��� ���Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDis �ػ�ð�
	* \return 
	*/
	virtual int SetClubAllianceDis(DWORD dwClub, __time64_t tDis) { return 0; }

	/**
	* Ŭ���� ������ ���� �ð��� ���Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDis ������ ���ӽð�
	* \return 
	*/
	virtual int SetClubAuthorityTime(DWORD dwClub, __time64_t tAuthority) { return 0; }
	

    /**
    * ������ ��� Ŭ�������� �����´�    
    * \param vClub : Ŭ������
	*/
    virtual int GetClubInfo(std::vector<GLCLUBINFO> &vClub)                               { return 0; }
    
    /**
    * �ش� Ŭ���� ����� �����´�
    * \param dwGuild : Ŭ����ȣ
    * \param vMember : Ŭ�����
	*/
    virtual int GetClubMember(DWORD dwClub, std::vector<GLCLUBMEMBER> &vMember)           { return 0; }
    
    /**
    * ���ο� ĳ���͸� Ŭ���� ���Խ�Ų��
    * \param dwClub : Ŭ����ȣ
    * \param dwChaNum : ���Խ�Ű���� ĳ���͹�ȣ
	*/
    virtual int AddClubMember(DWORD dwClub, DWORD dwChaNum)                               { return 0; }
    
    /**
    * ���ԵǾ� �ִ� ĳ���͸� Ŭ������ Ż���Ų��
    * \param dwChaNum : ĳ���͹�ȣ
	*/
    virtual int DeleteClubMember(DWORD dwChaNum)                                          { return 0; }

	/**
	* Ŭ�� ��Ʋ�� ��û�Ѵ�.
	* \param dwClubP ��Ŭ����ȣ( Ŭ�� ��Ʋ�� ��û�� ���� �� )
	* \param dwClubS ���� Ŭ����ȣ( Ŭ�� ��Ʋ�� ��û�� �� Ŭ�� )
	* \return
	*/
	virtual int SetClubBattle( DWORD dwClubP, DWORD dwClub, DWORD dwEndTime, int nAlliance )	{ return 0; }

	/**
	* Ŭ�� ��Ʋ�� �����Ѵ�.
	* \param dwClubP ��Ŭ�� ��ȣ( Ŭ�� ��Ʋ�� ��û�� ���� �� )
	* \param dwClubS ���� Ŭ�� ��ȣ( Ŭ�� ��Ʋ�� ��û�� �� Ŭ�� )
	* \param nGuFlag ���п���
	* \param nGuKillNum Ŭ�� ��Ʋ ų��
	* \return
	*/
	virtual int EndClubBattle( DWORD dwClubP, DWORD dwClubS, int nGuFlag, int nGuKillNum, int nGuDeathNum, bool bAlliance )	{ return 0; }

	/**
	* Ŭ�� ��Ʋ������ �����Ѵ�,( Ŭ�� ��Ʋ�� ����Ǳ��� ����������� �ʿ��Ѱ�� ��Ʋ ������ ������ �ʿ��� )
	* \param dwClubP ��Ŭ�� ��ȣ( Ŭ�� ��Ʋ�� ��û�� ���� ��)
	* \param dwClubS ���� Ŭ�� ��ȣ( Ŭ�� ��Ʋ�� ��û�� �� Ŭ�� )
	* \param nGuKillNum Ŭ�� ��Ʋ ų��
	* \return
	*/
	virtual int SaveClubBattle( DWORD dwClubP, DWORD dwClubS, int nGuKillNum, int nGuDeathNum )	{ return 0; }

	
	/**
	* ���͹�Ʋ ������ �ʱ�ȭ ��Ű���.
	* \param dwClub Ŭ�� ��ȣ
	* \return
	*/
	virtual int ReSetAllianceBattle( DWORD dwClub ) { return 0; }


	/**
	* �ش� Ŭ���� ��Ʋ���� Ŭ�� ������ �����´�.
	* \param dwClub Ŭ����ȣ
	* \param &vMember Ŭ����Ʋ ����
	* \return 
	*/
    virtual int GetClubBattleInfo( DWORD dwClub, std::vector<GLCLUBBATTLE> &vBattleInfo )	 { return 0; }

    /**
    * �ش� ĳ������ ����ġ�� �����Ѵ�.
	* ĳ���͹�ȣ�� 0 ���� Ŀ���Ѵ�.
    * ����ġ�� 0 ���� Ŀ���Ѵ�.
    * \param  nChaNum : ĳ���͹�ȣ
    * \param   llExp : ����ġ
	*/    
    virtual int SetChaExp(int nChaNum, LONGLONG llExp)                                      { return 0; }

    /**
    * �ش� ĳ������ ����ġ�� �����´�.    
	* ĳ���͹�ȣ�� 0 ���� Ŀ���Ѵ�.
    * ����ġ�� 0 ���� ������ �����̴�.
    * \param nChaNum ĳ���͹�ȣ
    *  LONGLONG : ����ġ
	*/    
    virtual LONGLONG GetChaExp(int nChaNum)                                                 { return 0; }
    
    /**
    * ���ӳ����� �����̳� �Ű��� �Ű��� ������ DB�� ����Ѵ�.    
    * \param nSGNum �����׷�
    * \param ChaNum �Ű��� ĳ���͹�ȣ
    * \param strMsg �Ű����� (�ִ�ũ��� CHAT_MSG_SIZE)
	*/
    virtual int LogAppeal(int nSGNum, int nChaNum, CString strMsg) { return 0; }
    
    /**
    * ���� ��������� ���� Ŭ�������͸� �����´�.
	*/
    virtual int GetClubRegion(std::vector<GLGUID_DB> &vGUID_DB) { return 0; }
    
	//add sw buff
	virtual int GetSwRegion(std::vector<GLSWID> &vSWID_DB) { return 0; }
    /**
    * ������ �������� ������ Ŭ�� ������ ���Ѵ�.
    * \param dwRegionID : ���� ID
    * \param dwClub : Ŭ�� ID
    * \param fTax : ����
	*/
    virtual int SetClubRegion(DWORD dwRegionID, DWORD dwClub=0, float fTax=0) { return 0; }
    
	//add sw buff
	virtual int SetSwRegion(DWORD dwSwID, DWORD dwSwNum=3) { return 0; }
    /**
    * �ش� ������ ���� ���¸� �����Ѵ�.
    * \param dwRegionID ���� ID
    * \param dwClub Ŭ�� ID (�⺻ 0 �̴�)
	*/
    virtual int DelClubRegion(DWORD dwRegionID, DWORD dwClub=0) { return 0; }
	
	/**
	* �ش�Ŭ���� �����ݾ��� �����´�. (Storage)
	* \param dwClub Ŭ����ȣ
	* \param ���ϰ��� >= 0, 0 ���� ������ ERROR
	*/
    virtual LONGLONG GetClubMoney(DWORD dwClub) { return 0; }

	/**
	* �ش�Ŭ���� �����ݾ��� �����Ѵ�. (Storage)
	* \param dwClub : Ŭ����ȣ
	* \param llMoney : �����ݾ� (�ݾ��� >= 0)
	*/
	virtual int	SetClubMoney(DWORD dwClub, LONGLONG llMoney) { return 0; }
	
	/**
	* �ش�Ŭ���� ���Աݾ��� �����Ѵ�. (Income money)
	* \param dwClub : Ŭ����ȣ
	* \param llMoney : ���Աݾ� (�ݾ��� >= 0)
	*/
	virtual int	SetClubIncomeMoney(DWORD dwClub, LONGLONG llMoney) { return 0; }
    
    /**
	* �ش�Ŭ���� ���Աݾ��� �����´�. (Income money)
	* \param dwClub : Ŭ����ȣ
	* \return ���ϰ��� >= 0, 0 ���� ������ ERROR
	*/
	virtual LONGLONG GetClubIncomeMoney(DWORD dwClub) { return 0; }
    
    /**
	* �ش�Ŭ���� Ŭ��â���� �о�´�.
	* \param dwClub : Ŭ����ȣ
	* \param ByteStream : ����
	*/
	virtual int ReadClubStorage(DWORD dwClub, CByteStream &ByteStream) { return 0; }
    
    /**
	* �ش�Ŭ���� Ŭ��â���� �����Ѵ�.
	* \param dwClub : Ŭ����ȣ
	* \param pData : â����ġ
	* \param nSize : ���� ��üũ��
	*/	
	virtual int	WriteClubStorage(DWORD dwClub, BYTE* pData, int nSize) { return 0; }
	
	/**
	* �ҹ����α׷� ��볻���� ����Ѵ�.
	* \param nSGNum �����׷�
	* \param nSvrNum ������ȣ
	* \param nUserNum ������ȣ
	* \param nChaNum ĳ���͹�ȣ
	* \param nHackProgramNum ��ŷ���α׷���ȣ
	* \param szComment �޸�Ʈ
	* \return 
	*/
	virtual int LogHackProgram(
		int nSGNum, 
		int nSvrNum, 
		int nUserNum, 
		int nChaNum, 
		int nHackProgramNum,
		const TCHAR* szComment) { return 0 ; }
	
	/**
	* �� Ŭ������ �����Ѵ�
	* \param dwClub : Ŭ����ȣ
	* \param dwChaNum : ĳ���͹�ȣ
	* \param dwSubMasterFlags : �÷��� (0 ���� ������ �Ϲݱ����� �ȴ�)
	*/
	virtual int	SetClubMasterFlags(
		DWORD dwClub,
		DWORD dwChaNum,
		DWORD dwSubMasterFlags) { return 0 ; }

	/**
	* ���� Ŭ���� �Ἲ�Ѵ�.
	* \ db �� Ŭ���� ���Ἲ�� üũ���� �ʴ´�.
	* \ A �� B �� ������ ���¿��� 
	* \ B �� C �� ������ ����
	* \ C �� A �� ������ ���� ������ ü�� ������ �߻��Ѵ�.	
	* \param dwClubP : �� Ŭ����ȣ (���� Ŭ���� ��)
	* \param dwClubS : ���� Ŭ����ȣ (�� Ŭ�� ������ ���� ���� Ŭ��)
	*/
	virtual int	SetClubAlliance(DWORD dwClubP, DWORD dwClubS) { return 0 ; }

	/**
	* ���� Ŭ���� ��ü�Ѵ�.
	* \param dwClubP : �� Ŭ����ȣ (���� Ŭ���� ��)
	* \param dwClubS : ���� Ŭ����ȣ (�� Ŭ�� ������ ���� ���� Ŭ��)
	*/
	virtual int	DelClubAlliance(DWORD dwClubP, DWORD dwClubS) { return 0; }
	
	/**
	* ���� Ŭ�� ����Ʈ�� �����´�.
	* <��Ŭ��,����Ŭ��>
	*/
	virtual int	GetClubAlliance ( std::vector< std::pair<DWORD,DWORD> > &vecAlliance )	{ return 0; }

	/**
	* �ش� ������� ä�� ���Ͻð��� �����´�
	*/
	virtual __time64_t  GetChatBlockTime (int nUserNum) { return 0; }

	/**
	* �ش� ������� ä�� ���Ͻð��� �����Ѵ�
	*/
	virtual int SetChatBlockTime (int nUserNum, __time64_t tBlockTime) { return 0; }
	virtual int UserBlock (int nUserNum, int nDay ) { return 0; }
	
	/**
	* Ŭ���� ���������� �����Ѵ�.
	*/
	virtual int SetClubNotice (DWORD dwClub, const char* szClubNotice) { return 0; }
	
	/**
	* Ŭ���� ����Ŭ�� ���� �븮�ڸ� �����Ѵ�.
	*/
	virtual int SetClubDeputy (DWORD dwClub, DWORD dwDeputy) { return 0; }

	/**
	* Ŭ���� ������ ������ �����Ѵ�.
	*/
	virtual int SetClubAuthority (DWORD dwClub, DWORD dwMasterID) { return 0; }

	

	/**
	* ĳ������ �̸��� �����Ѵ�.
	* \param dwChaNum ĳ���͹�ȣ
	* \param szCharName ������ ĳ�����̸�
	* \return DB_OK, DB_ERROR
	*/
	virtual int RenameCharacter (DWORD dwChaNum, char* szCharName) { return 0; }

	/**
	* ĳ������ �Ӹ����� �����Ѵ�.
	* \return DB_OK, DB_ERROR
	*/
	virtual int SetChaHairColor(DWORD dwChaNum, int nHairColor) { return 0; }
	
	/**
	* ĳ������ ��Ÿ���� �����Ѵ�.
	* \return DB_OK, DB_ERROR	
	*/
	virtual int SetChaHairStyle(DWORD dwChaNum, int nHairStyle) { return 0; }

	/**
	* ĳ������ �󱼽�Ÿ���� �����Ѵ�.
	* \return DB_OK, DB_ERROR	
	*/
	virtual int SetChaFaceStyle(DWORD dwChaNum, int nFaceStyle) { return 0; }


	/**
	* ĳ������ ������ �����Ѵ�.
	* \return DB_OK, DB_ERROR	
	*/
	virtual int SetChaGenderChange( DWORD dwChaNum, int nClass, int nSex, int nFace, int nHair, int nHairColor) { return 0; }

	/**
	* ĳ������ ��ġ������ �����Ѵ�.
	* \param dwChaNum ĳ���� ��ȣ
	* \param dwStartmapID �����ϴ� �� ��ȣ
	* \param dwStartGate �����ϴ� ����Ʈ ��ȣ
	* \param fStartPosX �����ϴ� X ��ǥ
	* \param fStartPosY �����ϴ� Y ��ǥ
	* \param fStartPosZ �����ϴ� Z ��ǥ
	* \param dwSaveMapID ���������� �÷����� �� ��ȣ
	* \param fSavePosX ���������� �÷����� X ��ǥ
	* \param fSavePosY ���������� �÷����� Y ��ǥ
	* \param fSavePosZ ���������� �÷����� Z ��ǥ
	* \param dwLastCallMapID ���� ��ȯī�忡 ����� �� ��ȣ
	* \param fLastCallPosX ���� ��ȯī�忡 ����� X ��ǥ
	* \param fLastCallPosY ���� ��ȯī�忡 ����� Y ��ǥ
	* \param fLastCallPosZ ���� ��ȯī�忡 ����� Z ��ǥ
	* \return DB_OK, DB_ERROR
	*/
	virtual int	SaveChaPos(DWORD dwChaNum,
				 DWORD dwStartMapID,
				 DWORD dwStartGate,
				 FLOAT fStartPosX,
				 FLOAT fStartPosY,
				 FLOAT fStartPosZ,
				 DWORD dwSaveMapID,
				 FLOAT fSavePosX,
				 FLOAT fSavePosY,
				 FLOAT fSavePosZ,
				 DWORD dwLastCallMapID,
				 FLOAT fLastCallPosX,
				 FLOAT fLastCallPosY,
				 FLOAT fLastCallPosZ) { return 0; }

	/**
	* ���ο� ���� �����Ѵ�.
	* \param nChaNum  �� ������ ĳ���� ��ȣ
	* \param szPetName ���̸�
	* \param nPetType ���� Ÿ��
	* \param nPetMID �� MID
	* \param nPetSid �� SID
	* \param nPetStyle ���� ��Ÿ��
	* \param nPetColor ���� �÷�
	* \param m_nPetCardMID �� ī���� MID
	* \param m_nPetCardSID �� ī���� SID
	* \return DB_ERROR �Ǵ� ������������ Pet �� ������ȣ
	*/
	virtual int CreatePet(
		int nChaNum,
		const TCHAR* szPetName,
		int nPetType,
		int nPetMID,
		int nPetSID,
		int nPetStyle,
		int nPetColor,
		int nPetCardMID,
	    int nPetCardSID
		) { return 0; }

	/**
	* ���� �̸��� �����Ѵ�.
	* \param nChaNum ĳ���� ��ȣ
	* \param nPetNum ���� ������ȣ
	* \param szPetName ���̸�
	* \return DB_ERROR, DB_OK
	*/
	virtual int RenamePet( int nChaNum, int nPetNum, const TCHAR* szPetName) { return 0; }

	/**
	* ���� �������� �����Ѵ�.
	* \param nChaNum ���� ���ο� ������
	* \param nPetNum �������� ������ �� ��ȣ
	*/
	virtual int ExchangePet(int nChaNum, int nPetNum) { return 0; }

	/**
	* ���� �÷��� �����Ѵ�.
	* \param nChaNum ĳ���� ��ȣ
	* \param nPetNum �÷��� �����Ϸ��� ���� ��ȣ
	* \param nPetColor ���� �÷�
	*/
	virtual int SetPetColor(int nChaNum, int nPetNum, int nPetColor) { return 0; }

	/**
	* ���� ��Ÿ���� �����Ѵ�.
	* \param nPetNum ��Ÿ���� �����Ϸ��� ���� ��ȣ
	* \param nPetStyle ���� ��Ÿ��
	*
	*/
	virtual int SetPetStyle( int nChaNum, int nPetNum, int nPetStyle) { return 0; }

	/**
	* ���� �������� �����Ѵ�. �������� 0-100 ����
	* \param nPetNum �������� ������ ���� ��ȣ
	* \param fPetFull ������
	*/
	virtual int SetPetFull( int nChaNum, int nPetNum, int nPetFull) { return 0; }

	/**
	* ���� �κ��丮 ������Ʈ
	*/
	virtual int SetPetInven(
		int nChaNum,
		int nPetNum,
		CByteStream &ByteStream ) { return 0; }

	/**
	* ���� ��ų ������Ʈ
	*  
	*/	
	virtual int SetPetSkill(
		int nChaNum,
		int nPetNum,
		int nPetInvenType,
		int nPetInvenMID,
		int nPetInvenSID,
		int nPetInvenCMID,
		int nPetInvenCSID,
		int nPetInvenAvailable) { return 0; }

	/**
	* ���� �����Ѵ�.
	* \param nPetNum ������ ���� ��ȣ
	*/
	virtual int	DeletePet(int nChaNum, int nPetNum) { return 0; }
	
	/**
	* ���� ��ų�� �����´�.
	* \param pPet ���� ������	
	*/
	virtual int GetPetSkill(
				 int nChaNum,
				 GLPET* pPet,
				 DWORD dwPetNum ) { return 0; }
	
	/**
	* ���� �κ��丮 ������ �����´�
	*/
	virtual int GetPetInven( 
				 int nChaNum,
				 GLPET* pPet,
				 DWORD dwPetNum ) { return 0; }
	
	/**
	* ���� ������ �����´�.
	*/
	virtual int GetPet(
				 GLPET* pPet,
				 DWORD dwPetNum,
				 DWORD dwClientID) { return 0; }

	/**
	* ���� �������� �����´�
	* \param dwPetNum �� ��ȣ
	* \return ���� ������ or DB_ERROR
	*/
	virtual int GetPetFull( int nChaNum, DWORD dwPetNum ) { return 0; }

	virtual int GetPoints( int nChaNum , DWORD dwUserID , DWORD dwClientID , int *PPoints , int *VPPoints ) { return 0; }

	/**
	* �Ҹ�� ���� ����Ʈ�� �����´�.
	* \param nChaNum �Ҹ�� �� ����Ʈ�� ����Ϸ��� ĳ���� ��ȣ
	* \param vecPetReList �Ҹ�� �� ����Ʈ�� ���� ����
	*/
	virtual int GetRestorePetList( int nChaNum, std::vector<PET_RELIST> &vecPetReList ) { return 0; }

	/**
	* ���� ��Ȱ��Ų��.
	* \param nPetNum ��Ȱ�� �� ��ȣ
	* \param nChaNum ��Ȱ�� �䱸�� ĳ���� ��ȣ
	*/
	virtual int RestorePet( DWORD dwPetNum, int nChaNum ) { return 0; }

	/**
	* ���� ��Ų���� ������Ʈ �Ѵ�.
	* \param nChaNum		�ּ��� ĳ���� ��ȣ
	* \param dwPetNum		�� ��ȣ
	* \param nPetSkinMID	�� ��Ų MID
	* \param nPetSkinSID	�� ��Ų SID
	* \param nPetSkinScale	�� ��Ų Scale
	* \param nPetSkinTime	�� ��Ų ����ð�
	*/
	virtual int UpdatePetSkin( int nChaNum,
							   DWORD dwPetNum,
							   int nPetSkinMID,
							   int nPetSkinSID,
							   int nPetSkinScale,
							   int nPetSkinTime ) { return 0; }

	virtual int SetPetDualSkill(int nChaNum, int nPetNum){return 0;}
	/**
	* SMS ���ڸ޽����� �߼��Ѵ�.
	*
	* \param nSendChaNum SMS �� �߼��ϴ� ĳ���� ��ȣ	
	* \param nReceiveChaNum SMS �� ���� ĳ���� ��ȣ
	* \param szReceivePhone SMS �� ���� ĳ������ ��ȭ��ȣ
	* \param szSmsMsg SMS �޽��� ����
	*/
	virtual int LogSMS(
					int nSendChaNum,					
					int nReceiveChaNum,
					const TCHAR* szReceivePhone,
					const TCHAR* szSmsMsg ) { return 0; }

	/**
	* �������� �����ɼ� �α׸� �����.
	* \param _sLOG �����ɼ� ����ü
	*/
	virtual int LogRandomItem( const SLOGRANDOMOPTION &_sLOG ) { return 0; }

	/**
	* ĳ������ �޴��� ��ȣ�� �����Ѵ�.
	* \param nChaNum ĳ���� ��ȣ
	* \param szPhoneNumber ��ȭ��ȣ NULL ���� ���� 14�� SMS_RECEIVER
	*/
	virtual int SetChaPhoneNumber(
					int nChaNum,
					const TCHAR* szPhoneNumber ) { return 0; }

	/**
	* ���� �׼� �α׸� �����.
	* \param _sLOG ��׼Ƿα� ����ü
	*/
	virtual int LogPetAction( const SLOGPETACTION &_sLOG ) { return 0; }

	/**
	* Terra
	* ������ �н����带 �ְų� üũ�Ѵ�.
	* \param szTID Terra UserID
	* \param szPasswd ����� �н�����
	* \return 
	*/
	virtual int TerraUserPassCheck(
					const TCHAR* szTID,
					const TCHAR* szPasswd,
					int	nCheckFlag ) { return 0; }
	
	/**
	* Daum
	* ������ �н����带 �ְų� üũ�Ѵ�.
	* \param szDaumGID Daum UserGID
	* \param szDaumPass ����� �н�����
	* \return 
	*/
	virtual int DaumUserPassCheck(const TCHAR* szDaumGID, 
								  const TCHAR* szDaumPasswd,
								  int	nCheckFlag ) { return 0; }


	/**
	* Excite
	* ������ �н����带 �ְų� üũ�Ѵ�.
	* \param szUID Excite UserID
	* \param szPasswd ����� �н�����
	* \return 
	*/
	virtual int ExciteUserPassCheck(const TCHAR* szUID,
								  const TCHAR* szUserID,
								  const TCHAR* szPasswd,
								  int	nCheckFlag ) { return 0; }


	/**
	* Thailand
	* ������� PC�� Ŭ������ üũ�Ѵ�.	
	* \param szUsrIP ������� IP Address
	* \return DB ������ �����
	*/
	virtual int	ThaiUserClassCheck( const TCHAR* szUsrIP ) { return 0; };

	/** 
	* Malaysia
	* ������� PC�� Ŭ������ üũ�Ѵ�.	
	* \param szUsrIP ������� IP Address
	* \return DB ������ �����
	*/
	virtual int	MyUserClassCheck( const TCHAR* szUsrIP ) { return 0; };

	
	/**
	* ���ο� Ż���� �����Ѵ�.
	* \param nChaNum  Ż�� ������ ĳ���� ��ȣ
	* \param szVehicleName Ż�� �̸�
	* \param nVehicleType Ż���� Ÿ��
	* \param nVehicleMID Ż�� MID
	* \param nVehicleSID Ż�� SID
	* \param nVehicleCardMID Ż���� ������ MID
	* \param nVehicleCardSID Ż���� ������ SID
	* \return DB_ERROR �Ǵ� ������������ Ż���� ������ȣ
	*/
	virtual int CreateVehicle(
		int nCharNum,
		const TCHAR* szVehicleName,
		int nVehicleType,
		int nVehicleCardMID,
		int nVehicleCardSID
		)	{ return 0; }


	/**
	* Ż���� ���͸��� �����Ѵ�. ���͸��� 0-1000 ����
	* \param nVehicleBttery ���͸��� ������ Ż���� ��ȣ
	* \param nVehicleBttery ���͸�
	*/
	virtual int SetVehicleBattery(
		int nVehicleNum,
		int nCharNum,
		int nVehicleBttery
		) { return 0; }

	virtual int SetVehicleColor( //add bike color
		int nVehicleNum,
		int nCharNum,
		int nColor,
		int nColor1,
		int nColor2,
		int nColor3,
		int nColor4,
		int nColor5
		) { return 0; }


	/**
	* Ż���� ���͸��� �����´�.
	* \param nVehicleBttery ���͸��� ������ Ż���� ��ȣ
	*/
	virtual  int GetVehicleBattery(
		int nVehicleNum,
		int nCharNum ) { return 0; }


	/**
	* Ż���� �����Ѵ�.
	* \param nVehicle ������ Ż���� ��ȣ
	*/
	virtual int DeleteVehicle(
		int nVehicle, 
		int nCharNum) { return 0; }

	/**
	* Ż���� ������ �����´�.
	*/
	virtual int GetVehicle(
		GLVEHICLE* pVehicle,
		int nVehicleNum,
		int nCharNum) { return 0; }

	/**
	* Ż���� �κ��丮 ������ �����´�
	*/
	virtual int GetVehicleInven( 
		GLVEHICLE* pVehicle,
		int nVehicleNum,
		int nCharNum ) { return 0; }

	/**
	*Ż�� �κ��丮 ������Ʈ
	*/
	virtual int SetVehicleInven(
		int nVehicle,
		int nCharNum,
		CByteStream &ByteStream ) { return 0; }

	/**
	* Ż���� �׼� �α׸� �����.
	* \param _sLOG Ż�;׼Ƿα� ����ü
	*/
	virtual int LogVehicleAction( const SLOGVEHICLEACTION &_sLOG ) { return 0; }


	// �⼮�� �̺�Ʈ 
	virtual int LogUserAttend( int nUserNum, std::vector<USER_ATTEND_INFO> &vecUserAttend ) { return 0; }

	virtual int InsertUserAttend( int nUserNum, int nCount, int nAttendReward ) { return 0; }

	virtual int InsertAttendItem( CString strPurKey, CString strUserID, int nItemMID, int nItemSID ) { return 0; }

	//	UserMoney Add
	virtual int UpdateUserMoneyAdd( DWORD dwUserID, LONGLONG lnUserMoney ) { return 0; }
	virtual int UpdateUserLastInfoAdd( DWORD dwUserID, LONGLONG lnUserMoney ) { return 0; }

public:
	virtual void AddJob ( CDbAction *pDbActJob )
	{
		GASSERT ( 0&&"GLDBMan::AddJob()" );
	}

	virtual void AddLogJob ( CDbAction *pDbActJob )
	{
		GASSERT ( 0&&"GLDBMan::AddJob()" );
	}

	virtual void AddUserJob ( CDbAction *pDbActJob )
	{
		GASSERT ( 0&&"GLDBMan::AddJob()" );
	}

	virtual void AddWebJob ( CDbAction *pDbActJob )
	{
		GASSERT ( 0&&"GLDBMan::AddJob()" );
	}
};

#endif // GLDBMAN_H_
