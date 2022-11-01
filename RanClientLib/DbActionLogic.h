#ifndef DBACTIONLOGIC_H_
#define DBACTIONLOGIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../NetServerLib/s_CDbAction.h"
#include "../G-Logic/GLPet.h"
#include "../G-Logic/GLVEHICLE.h"

/**
 * \ingroup RanClientLib
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-05-31
 *
 * \author 
 *
 * \par license
 * 
 * \todo 
 *
 * \bug 
 *
 */

class CDbActSaveChar : public CDbAction
{
public:
	CDbActSaveChar ();
	virtual ~CDbActSaveChar();
	
	bool SetInfo ( DWORD dwClient, DWORD dwGaea, SCHARDATA2 *pCHARDATA );

	virtual int Execute ( CServer* pServer );

protected:
	DWORD		m_dwClient;
	DWORD		m_dwGaea;
	SCHARDATA2	m_sCHARDATA;
};

class CDbActToAgentMsg : public CDbAction
{
public:
	CDbActToAgentMsg ();
	virtual ~CDbActToAgentMsg();
	
	bool SetMsg ( DWORD dwClient, NET_MSG_GENERIC *nmg );

	virtual int Execute ( CServer* pServer );

protected:
	DWORD	m_dwClientID;
	BYTE	m_aryMsg[NET_DATA_BUFSIZE];
};

class CDelChaFriend : public CDbAction
{
public:
	CDelChaFriend(int nChaP, int nChaS);
	virtual ~CDelChaFriend() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaP;
	int m_nChaS;
};

class CAddChaFriend : public CDbAction
{
public:
	CAddChaFriend(int nChaP, int nChaS);
	virtual ~CAddChaFriend() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaP;
	int m_nChaS;
};

class CSetChaFriend : public CDbAction
{
public:
	CSetChaFriend(int nChaP, int nChaS, int nFlag);
	virtual ~CSetChaFriend() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaP;
	int m_nChaS;
	int m_nFlag;
};

class CLogPartyMatch : public CDbAction
{
public:
	CLogPartyMatch(int nSGNum, int nSvrNum, WORD wWin, WORD wLost);
	virtual ~CLogPartyMatch() {};
	virtual int Execute (CServer* pServer);

protected:
	int		m_nSGNum;
	int		m_nSvrNum;
	WORD	m_wWin;
	WORD	m_wLost;	
};

class CLogItemExchange : public CDbAction
{
public:
	CLogItemExchange ();
	CLogItemExchange ( const SLOGITEMEXCHANGE &sEXCHANGE );
	virtual ~CLogItemExchange() {};
    virtual int Execute (CServer* pServer);

protected:
	SLOGITEMEXCHANGE m_sEXCHANGE;
};

class CLogMoneyExchange : public CDbAction
{
public:
	CLogMoneyExchange ();
	CLogMoneyExchange ( const SLOGMONEYEXCHANGE &_sLOG );
	virtual ~CLogMoneyExchange() {};
    virtual int Execute (CServer* pServer);	

protected:
	SLOGMONEYEXCHANGE m_sLOG;
};

class CLogItemConversion : public CDbAction
{
public:
	CLogItemConversion(
		const SLOGITEMCONVERSION &_sLOG );
	virtual ~CLogItemConversion() {};
    virtual int Execute( CServer* pServer );

protected:
	SLOGITEMCONVERSION m_sLOG;
};

/**
* �������� �����ɼ� �α׸� �����.
* \param _sLOG �����ɼ� ����ü
*/
class CLogRandomItem : public CDbAction
{
public:
	CLogRandomItem(
		const SLOGRANDOMOPTION& sLOG );
	virtual ~CLogRandomItem() {};
	virtual int Execute( CServer* pServer );

protected:
	SLOGRANDOMOPTION m_sLOG;
};

/**
* ���� �׼� �α׸� �����.
* \param _sLOG ��׼Ƿα� ����ü
*/
class CLogPetAction : public CDbAction
{
public:
	CLogPetAction( const SLOGPETACTION& sLOG );
	virtual ~CLogPetAction() {};
	virtual int Execute( CServer* pServer );

protected:
	SLOGPETACTION m_sLOG;
};

/**
 * Shop ���� ������ �������� �����´�.
 */
class CGetPurchaseItem : public CDbAction
{
public:
    CGetPurchaseItem(CString strUID, DWORD dwCharID); 
    virtual ~CGetPurchaseItem() {};
    virtual int Execute(CServer* pServer);

protected:
	DWORD	m_dwCharID;
    CString m_strUID;
    std::vector<SHOPPURCHASE> m_vItem;
};


class CGetItemShop : public CDbAction
{
public:
    //CGetItemShop(CString strUID, DWORD dwCharID); 
	CGetItemShop( /*SNATIVEID sItemID,
	WORD wItemPrice,
	WORD wItemStock,
	WORD wItemNum,*/
	DWORD dwCharID ); 
    virtual ~CGetItemShop() {};
    virtual int Execute(CServer* pServer);

protected:
	/*SNATIVEID m_sItemID;
	WORD m_wItemPrice;
	WORD m_wItemStock;
	WORD m_wItemNum;*/
	DWORD	m_dwCharID;
    std::vector<ITEMSHOP> m_vItem;
};

/**
 * ������ ������ �� �ִ� ��ǰ���� Ȯ���Ѵ�.
 * �Է°�
 * nFlag (0 : ����, 1 : ���ſϷ�, 2 : ������ҽ�û, 3 : �������ó��)
 * ��°�
 * 1 : �������� ����
 * �̿��ǰ� : �������� ����
 */
class CSetPurchaseItem : public CDbAction
{
public:
    CSetPurchaseItem(CString strPurKey, int nFlag=1);
    virtual ~CSetPurchaseItem() {};
    virtual int Execute(CServer* pServer);

protected:
    CString m_strPurKey;
    int m_nFlag;
};

class CSetItemShop : public CDbAction
{
public:
    CSetItemShop(CString strItemNum, DWORD dwUserID);
    virtual ~CSetItemShop() {};
    virtual int Execute(CServer* pServer);

protected:
    CString m_strItemNum;
    DWORD m_dwUserID;
};

class CItemShop_Get : public CDbAction
{
public:
    CItemShop_Get ( DWORD dwClient, DWORD dwUserID, CString strPurKey );
    virtual ~CItemShop_Get() {};
    virtual int Execute(CServer* pServer);

protected:
	DWORD		m_dwClient;
	DWORD		m_dwUserID;
    CString		m_strPurKey;
};


/**
 * ������ �������� �κ��丮�� �����ɴϴ�.
 */
class CPurchaseItem_Get : public CDbAction
{
public:
    CPurchaseItem_Get ( DWORD dwClient, DWORD dwUserID, CString strPurKey, SNATIVEID nidITEM, DWORD dwInvenPos );
    virtual ~CPurchaseItem_Get() {};
    virtual int Execute(CServer* pServer);

protected:
	DWORD		m_dwClient;
	DWORD		m_dwUserID;
    CString		m_strPurKey;
	SNATIVEID	m_nidITEM;
	DWORD		m_dwInvenPos;
};

/**
 * ������ �������� �κ��� �ֱ⿡ �������� ��� flag ����.
 */
class CPurchaseItem_Restore : public CDbAction
{
public:
    CPurchaseItem_Restore ( CString strPurKey );
    virtual ~CPurchaseItem_Restore() {};
    virtual int Execute(CServer* pServer);

protected:
    CString m_strPurKey;
};

/**
 * ĳ�����߰�ī�� ������
 * ���������� ���������� ĳ���� ���ڸ� 1 ������Ų��.
 */
class CItemChaCreateNumIncrease : public CDbAction
{
public:
    CItemChaCreateNumIncrease(int nUserNum);
    virtual ~CItemChaCreateNumIncrease() {};
    virtual int Execute(CServer* pServer);

protected:
    int m_nUserNum;
};

/**
 * �ش� ��Ŀ ����ð��� ���Ѵ�.
 * 1 �� 5 �� ��Ŀ�� ���� �� ����.
 * 2, 3, 4 �� ��Ŀ�� ����ð��� ���� �� �ִ�.
 */
class CSetChaStorageDate : public CDbAction
{
public:
    CSetChaStorageDate(int nUserNum, int nStorageNum, __time64_t tTime);
    virtual ~CSetChaStorageDate() {} ;
    virtual int Execute(CServer* pServer);

protected:
    int m_nUserNum;
    int m_nStorageNum;
    __time64_t m_tTime;
};

/**
 * �ش� ĳ������ �κ��丮 �߰� �ټ��� �����Ѵ�.
 * ���� 
 * ���� 0 ���� ���� �߰��� 1 �� �Է�
 * ���� ���� ���¿��� �� ���� �߰��� 2 �� �Է� (���� �ټ��� �Է��Ѵ�)
 * ���� ���� ���¿��� ���� �߰��� 3 �� �Է�
 * �ִ� 3������ �Է°���.
 */
class CSetChaInvenNum : public CDbAction
{
public :
    CSetChaInvenNum(int nChaNum, WORD wInvenLine);
    virtual ~CSetChaInvenNum() {} ;
    virtual int Execute(CServer* pServer);

protected:
    int m_nChaNum;
    WORD m_wInvenLine;
};


/**
 * �ش� ������� �����̾� �Ⱓ�� �����Ѵ�.
 */
class CSetPremiumTime : public CDbAction
{
public:
    CSetPremiumTime(int nUserNum, __time64_t tPremiumTime);
    virtual ~CSetPremiumTime() {} ;
    virtual int Execute(CServer* pServer);
protected:
    int m_nUserNum;
    __time64_t m_tPremiumTime;
};

/**
 * ������ȯ ī�� ����� ���� �������� �����Ѵ�.
 */
class CSetLastCallPos : public CDbAction
{
public:
    CSetLastCallPos(int nChaNum, DWORD dwMapID, D3DXVECTOR3 vPos);
    virtual ~CSetLastCallPos() {} ;
    virtual int Execute(CServer* pServer);

protected:
    int m_nChaNum;
    DWORD m_dwMapID;
    D3DXVECTOR3 m_vPos;
};

/**
 * ���ο� Ŭ���� �����Ѵ�.
 */
class CCreateClub : public CDbAction
{
public:
	/**
	* ���ο� Ŭ���� �����Ѵ�.
	* \param dwClientID Client ��ȣ
	* \param strClubName Ŭ���̸�
	* \param dwChaNum Ŭ���� �����Ϸ��� ĳ���� ��ȣ (�� ĳ���Ͱ� Ŭ���� �����Ͱ� �ȴ�)
	* \return -1:�̹� Ŭ�������ͷ� ��ϵǾ� ���� �����Ұ�.
	* \n      -2:Ŭ�� ������ �����߻� (�ߺ��ȱ���̸�)
	* \n       0�̻�:������ Ŭ����ȣ��
	*/
    CCreateClub ( DWORD dwClientID, CString strClubName, DWORD dwChaNum);
    virtual ~CCreateClub() {} ;
    virtual int Execute(CServer* pServer);

protected:
    CString m_strClubName;
    DWORD m_dwChaNum;
};

/**
 * Ŭ���� �����Ѵ�.
 */
class CDeleteClub : public CDbAction
{
public:
	/**
	* Ŭ���� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwChaNum Ŭ���� �����Ϸ��� ĳ���͹�ȣ(ĳ���� ��ȣ�� Ŭ�� �����Ͱ� �ƴϸ� ������ ���� �ʴ´�)
	* \return 0:Ŭ�� ���� ����
    * \n      1:Ŭ�� ���� ����
	*/
    CDeleteClub(DWORD dwClub, DWORD dwChaNum);
    virtual ~CDeleteClub() {};
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
    DWORD m_dwChaNum;
};

/**
 * Ŭ����ũ�� �����Ѵ�.
 */
class CSetClubRank : public CDbAction
{
public:
	/**
	* Ŭ����ũ�� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwRank ��ŷ
	* \return 
	*/
    CSetClubRank(DWORD dwClub, DWORD dwRank);
    virtual ~CSetClubRank() {};
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
    DWORD m_dwRank;
};

class CWriteClubMarkImage : public CDbAction
{
public:
	/**
	* Ŭ�� ��ũ �̹����� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwMarkVer ��ũ����
	* \param pData �̹��������� ������
	* \param nSize �̹��������� ������
	* \return 
	*/
    CWriteClubMarkImage(DWORD dwClub, DWORD dwMarkVer, BYTE* pData, int nSize);
    virtual ~CWriteClubMarkImage();
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
    DWORD m_dwMarkVer;
    BYTE  m_aryMark[sizeof(DWORD)*EMCLUB_MARK_SX*EMCLUB_MARK_SY];
    int   m_nSize;
};

/**
 * Ŭ�� ��ü�ð��� �����Ѵ�.
 */
class CSetClubDissolutionTime : public CDbAction
{
public:
	/**
	* Ŭ�� ��ü�ð��� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDiss ��ü�ð�
	* \return 
	*/
    CSetClubDissolutionTime(DWORD dwClub, __time64_t tDiss);
    virtual ~CSetClubDissolutionTime() {} ;
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
    __time64_t m_tDiss;
};

/**
* Ŭ���� ����Ż�� Ȥ�� �����ð��� ���Ѵ�.
*/
class CSetClubAllianceSec : public CDbAction
{
public:
	/**
	* Ŭ���� ����Ż�� Ȥ�� �����ð��� ���Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tSec �ð�
	* \return 
	*/
    CSetClubAllianceSec(DWORD dwClub, __time64_t tSec);
    virtual ~CSetClubAllianceSec() {} ;
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
    __time64_t m_tSec;
};

/**
* Ŭ���� �����ػ� �ð��� ���Ѵ�.
*/
class CSetClubAllianceDis : public CDbAction
{
public:
	/**
	* Ŭ���� �����ػ� �ð��� ���Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDis �ػ�ð�
	* \return 
	*/
    CSetClubAllianceDis(DWORD dwClub, __time64_t tDis);
    virtual ~CSetClubAllianceDis() {} ;
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
    __time64_t m_tDis;
};

/**
* Ŭ���� ������ ���� �ð��� �����Ѵ�.
*/
class CSetClubAuthorityTime : public CDbAction
{
public:
	/**
	* Ŭ���� ������ ���� �ð��� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDis ������ ���� �ð�
	* \return 
	*/
    CSetClubAuthorityTime(DWORD dwClub, __time64_t tAuthority);
    virtual ~CSetClubAuthorityTime() {} ;
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
    __time64_t m_tAuthority;
};




/**
 * ���ο� ĳ���͸� Ŭ���� ���Խ�Ų��
 */ 
class CAddClubMember : public CDbAction
{
public:
	/**
	* ���ο� ĳ���͸� Ŭ���� ���Խ�Ų��
	* \param dwClub Ŭ����ȣ
	* \param dwChaNum ���Խ�Ű���� ĳ���͹�ȣ
	* \return 
	*/
    CAddClubMember(DWORD dwClub, DWORD dwChaNum);
    virtual ~CAddClubMember() {} ;
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
    DWORD m_dwChaNum;
};

/**
 * ���ԵǾ� �ִ� ĳ���͸� Ŭ������ Ż���Ų��
 */
class CDeleteClubMember : public CDbAction
{
public:
	/**
	* ���ԵǾ� �ִ� ĳ���͸� Ŭ������ Ż���Ų��
	* \param dwChaNum ĳ���͹�ȣ
	* \return 
	*/
    CDeleteClubMember(DWORD dwChaNum);
    virtual ~CDeleteClubMember() {} ;
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwChaNum;
};

/**
 * �ش� ĳ������ ����ġ�� �����Ѵ�.
 */
class CSetChaExp : public CDbAction
{
public:
	/**
	* �ش� ĳ������ ����ġ�� �����Ѵ�.
	* \n ����
    * \n ĳ���͹�ȣ�� 0 ���� Ŀ���Ѵ�.
    * \n ����ġ�� 0 ���� Ŀ���Ѵ�.
	* \param nChaNum ĳ���͹�ȣ
	* \param llExp ����ġ
	* \return 
	*/
    CSetChaExp(int nChaNum, LONGLONG llExp);
    virtual ~CSetChaExp() {};
    virtual int Execute(CServer* pServer);

protected:
    int m_nChaNum;
    LONGLONG m_llExp;
};

/**
 * �ش� ĳ������ ����ġ�� �����´�.
 */
class CGetChaExp : public CDbAction
{
public:

	/**
	* �ش� ĳ������ ����ġ�� �����´�.
	* \n ���� 
    * \n ĳ���͹�ȣ�� 0 ���� Ŀ���Ѵ�.
    * \n ����ġ�� 0 ���� ������ �����̴�.
	* \param nChaNum ĳ���͹�ȣ
	* \return ����ġ
	*/
    CGetChaExp(int nChaNum);
    virtual ~CGetChaExp() {};
    virtual int Execute(CServer* pServer);

protected:
    int m_nChaNum;
};

/**
 * ���ӳ����� �����̳� �Ű��� �Ű��� ������ DB�� ����Ѵ�.
 */
class CLogAppeal : public CDbAction
{
public:
	/**
	* ���ӳ����� �����̳� �Ű��� �Ű��� ������ DB�� ����Ѵ�.
	* \param nSGNum �����׷�
	* \param nChaNum �Ű��� ĳ���͹�ȣ
	* \param strMsg �Ű����� (�ִ�ũ��� CHAT_MSG_SIZE)
	* \return 
	*/
    CLogAppeal(int nSGNum, int nChaNum, CString strMsg);
    virtual ~CLogAppeal() {};
    virtual int Execute(CServer* pServer);

protected:
    int m_nSGNum;
    int m_nChaNum;
    CString m_strMsg;
};

/**
 * ������ �������� ������ Ŭ�� ������ ���Ѵ�.
 */ 
class CSetClubRegion : public CDbAction
{
public:
	/**
	* ������ �������� ������ Ŭ�� ������ ���Ѵ�.
	* \param dwRegionID ���� ID
	* \param dwClub Ŭ�� ID
	* \param fTax ����
	* \return 
	*/
	CSetClubRegion(DWORD dwRegionID, DWORD dwClub, float fTax);
	virtual ~CSetClubRegion() {} ;
	virtual int Execute(CServer* pServer);
	
protected:
	DWORD m_dwRegionID;
	DWORD m_dwClub;
	float m_fTax;
};

//add sw buff
class CSetSwRegion : public CDbAction
{
public:
	/**
	* ������ �������� ������ Ŭ�� ������ ���Ѵ�.
	* \param dwRegionID ���� ID
	* \param dwClub Ŭ�� ID
	* \param fTax ����
	* \return 
	*/
	CSetSwRegion(DWORD dwSwID, DWORD dwSwNum);
	virtual ~CSetSwRegion() {} ;
	virtual int Execute(CServer* pServer);
	
protected:
	DWORD m_dwSwID;
	DWORD m_dwSwNum;
};
/**
 * �ش� ������ ���� ���¸� �����Ѵ�.
 */
class CDelClubRegion : public CDbAction
{
public:
	/**
	* �ش� ������ ���� ���¸� �����Ѵ�.
	* \n ����
	* \n �ش����� ID �� Ŭ�� ID �� DB ������ ��Ȯ�� ��ġ���� ������ ��ҵȴ�.
	* \param dwRegionID ���� ID
	* \param dwClub Ŭ�� ID
	* \return 
	*/
	CDelClubRegion(DWORD dwRegionID, DWORD dwClub);
	virtual ~CDelClubRegion() {} ;
	virtual int Execute(CServer* pServer);
	
protected:
	DWORD m_dwRegionID;
	DWORD m_dwClub;
};

/**
 * �ش�Ŭ���� �����ݾ��� �����Ѵ�. (Storage)
 */
class CSetClubMoney : public CDbAction
{
public:
	/**
	* �ش�Ŭ���� �����ݾ��� �����Ѵ�. (Storage)
	* \param dwClub Ŭ����ȣ
	* \param llMoney �����ݾ� (�ݾ��� >= 0)
	* \return 
	*/
	CSetClubMoney(DWORD dwClub, LONGLONG llMoney);
	virtual ~CSetClubMoney() {} ;
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	LONGLONG m_llMoney;
};

/**
*Club War Points
*/
class CSetCWarSG : public CDbAction
{
public:
	CSetCWarSG(DWORD dwClub, DWORD dwClubWarPoints);
	virtual ~CSetCWarSG() {} ;
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	DWORD m_dwClubWarPoints;
};

class CSetCWarMP : public CDbAction
{
public:
	CSetCWarMP(DWORD dwClub, DWORD dwClubWarPoints);
	virtual ~CSetCWarMP() {} ;
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	DWORD m_dwClubWarPoints;
};

class CSetCWarPHX : public CDbAction
{
public:
	CSetCWarPHX(DWORD dwClub, DWORD dwClubWarPoints);
	virtual ~CSetCWarPHX() {} ;
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	DWORD m_dwClubWarPoints;
};

class CSetCWarTH : public CDbAction
{
public:
	CSetCWarTH(DWORD dwClub, DWORD dwClubWarPoints);
	virtual ~CSetCWarTH() {} ;
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	DWORD m_dwClubWarPoints;
};

/**
*Club War Premium Points
*/
class CSetClubWarVoteP : public CDbAction
{
public:
	CSetClubWarVoteP(DWORD dwClub, LONGLONG llClubWarVoteP);
	virtual ~CSetClubWarVoteP() {} ;
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	LONGLONG m_llClubWarVoteP;
};


/**
*Club War Gold Points
*/
class CSetClubWarGoldP : public CDbAction
{
public:
	CSetClubWarGoldP(DWORD dwClub, LONGLONG llClubWarGoldP);
	virtual ~CSetClubWarGoldP() {} ;
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	LONGLONG m_llClubWarGoldP;
};
/**
 * �ش�Ŭ���� ���Աݾ��� �����Ѵ�. (Income money)
 */
class CSetClubIncomeMoney : public CDbAction
{
public:
	/**
	* �ش�Ŭ���� ���Աݾ��� �����Ѵ�. (Income money)
	* \param dwClub Ŭ����ȣ
	* \param llMoney ���Աݾ� (�ݾ��� >= 0)
	* \return 
	*/
	CSetClubIncomeMoney(DWORD dwClub, LONGLONG llMoney);
	virtual ~CSetClubIncomeMoney() {} ;
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	LONGLONG m_llMoney;
};

/**
 * �ش�Ŭ���� Ŭ��â���� �����Ѵ�. 
 */
class CWriteClubStorage : public CDbAction
{
public:
	/**
	* �ش�Ŭ���� Ŭ��â���� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param pData â�������� ������
	* \param nSize ����ũ��
	* \return 
	*/
	CWriteClubStorage(DWORD dwClub, const BYTE* pData, int nSize);
	virtual ~CWriteClubStorage();
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	BYTE* m_pData;
	int   m_nSize;
};

class CGetClubStorage : public CDbAction
{
public:
	CGetClubStorage( DWORD _dwClientID, DWORD _dwCharID, DWORD _dwClubID );
	virtual ~CGetClubStorage() {};
	virtual int Execute( CServer* pServer );

protected:
	DWORD m_dwClientID;
	DWORD m_dwCharID;
	DWORD m_dwClubID;
};

/**
* �ҹ����α׷� ��볻���� ����Ѵ�.
*/
class CLogHackProgram : public CDbAction
{
	enum { MAX_INFO = 512 };

public:
	/**
	* �ҹ����α׷� ��볻���� ����Ѵ�.
	* \param nUserNum ������ȣ
	* \param nChaNum ĳ���͹�ȣ
	* \param nHackProgramNum ��ŷ���α׷� ��ȣ
	* \param *szINFO �޸�Ʈ...���ڿ�
	* \return 
	*/
	CLogHackProgram(
		int nUserNum,
		int nChaNum,
		int nHackProgramNum,
		TCHAR* szINFO );
	virtual ~CLogHackProgram() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nUserNum;
	int m_nChaNum;
	int m_nHackProgramNum;
	TCHAR m_szINFO[CLogHackProgram::MAX_INFO];
};

/**
* �� Ŭ������ �����Ѵ�
*/
class CSetClubMasterFlags : public CDbAction
{
public:
	/**
	*
	* dwClub : Ŭ����ȣ
    * dwChaNum : ĳ���͹�ȣ
    * dwSubMasterFlags : �÷��� (0 ���� ������ �Ϲݱ����� �ȴ�)
	* \param dwClub 
	* \param dwChaNum 
	* \param dwSubMasterFlags 
	* \return 
	*/
	CSetClubMasterFlags(DWORD dwClub, DWORD dwChaNum, DWORD dwSubMasterFlags);
	virtual ~CSetClubMasterFlags() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	DWORD m_dwChaNum;
	DWORD m_dwSubMasterFlags;
};

/**
* ���� Ŭ���� �Ἲ�Ѵ�.
*/
class CSetClubAlliance : public CDbAction
{
public:
	/**
	* ���� Ŭ���� �Ἲ�Ѵ�.
	* \n ����
    * \n db �� Ŭ���� ���Ἲ�� üũ���� �ʴ´�.
    * \n A �� B �� ������ ���¿��� 
    * \n B �� C �� ������ ����
    * \n C �� A �� ������ ���� ������ ü�� ������ �߻��Ѵ�.
	* \param dwClubP �� Ŭ����ȣ (���� Ŭ���� ��)
	* \param dwClubS ���� Ŭ����ȣ (�� Ŭ�� ������ ���� ���� Ŭ��)
	* \return 
	*/
	CSetClubAlliance(DWORD dwClubP, DWORD dwClubS);
	virtual ~CSetClubAlliance() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClubP;
	DWORD m_dwClubS;
};

/**
* ���� Ŭ���� ��ü�Ѵ�.
*/
class CDelClubAlliance : public CDbAction
{
public:
	/**
	* ���� Ŭ���� ��ü�Ѵ�.
	* \param dwClubP �� Ŭ����ȣ (���� Ŭ���� ��)
	* \param dwClubS ���� Ŭ����ȣ (�� Ŭ�� ������ ���� ���� Ŭ��)
	* \return 
	*/
	CDelClubAlliance(DWORD dwClubP, DWORD dwClubS);
	virtual ~CDelClubAlliance() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClubP;
	DWORD m_dwClubS;
};

class CSetClubBattle : public CDbAction
{
public:
	/**
	* Ŭ����Ʋ�� �����Ѵ�.
	* \param dwClubP �� Ŭ����ȣ (�ڱ�Ŭ��)
	* \param dwClubS ���� Ŭ����ȣ (���Ŭ��)
	* \return 
	*/
	CSetClubBattle(DWORD dwClubP, DWORD dwClubS, DWORD dwEndTime, bool bAlliance = false );
	virtual ~CSetClubBattle() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClubP;
	DWORD m_dwClubS;
	DWORD m_dwEndTime;
	bool m_bAlliance;
};

class CEndClubBattle : public CDbAction
{
public:
	/**
	* Ŭ����Ʋ�� �����Ѵ�.
	* \param dwClubP �� Ŭ����ȣ (�ڱ�Ŭ��)
	* \param dwClubS ���� Ŭ����ȣ (���Ŭ��)
	* \param nFlag		���� Flag
	* \param nGuKillNum	 ų��
	* \param nGuDeathNum ������
	* \return 
	*/
	CEndClubBattle(DWORD dwClubP, DWORD dwClubS, int nFlag, int nGuKillNum, int nGuDeathNum, bool bAlliance = false );
	virtual ~CEndClubBattle() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD	m_dwClubP;
	DWORD	m_dwClubS;
	int		m_nFlag;
	int		m_nGuKillNum;
	int		m_nGuDeathNum;
	bool	m_bAlliance;
};

class CSaveClubBattle : public CDbAction
{
public:
	/**
	* Ŭ����Ʋ�� �����Ȳ�� �����Ѵ�.
	* \param dwClubP �� Ŭ����ȣ (�ڱ�Ŭ��)
	* \param dwClubS ���� Ŭ����ȣ (���Ŭ��)
	* \param nGuKillNum	 ų��
	* \param nGuDeathNum ������
	* \return 
	*/
	CSaveClubBattle(DWORD dwClubP, DWORD dwClubS, int nGuKillNum, int nGuDeathNum );
	virtual ~CSaveClubBattle() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD	m_dwClubP;
	DWORD	m_dwClubS;
	int		m_nGuKillNum;
	int		m_nGuDeathNum;
};


/**
* ���͹�Ʋ ������ �ʱ�ȭ ��Ų��.
*/
class CReSetAllianceBattle : public CDbAction
{
public:
	/**
	* ���͹�Ʋ ������ �ʱ�ȭ ��Ų��.	
	* \param dwClub Ŭ����ȣ
	* \return 
	*/
    CReSetAllianceBattle( DWORD dwClub );
    virtual ~CReSetAllianceBattle() {} ;
    virtual int Execute(CServer* pServer);

protected:
    DWORD m_dwClub;
};


/**
* �ش������� ä�ú��Ͻð��� �����´�.
*/
class CGetChatBlockTime : public CDbAction
{
public:
	/**
	* �ش������� ä�ú��Ͻð��� �����´�.
	* \param nUserNum ����ڹ�ȣ
	* \return 
	*/
	CGetChatBlockTime(int nUserNum);
	virtual ~CGetChatBlockTime() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nUserNum;
};

/*
* �ش������� ä�ú��Ͻð��� �����Ѵ�.
*/
class CSetChatBlockTime : public CDbAction
{
public:
	/**
	* �ش������� ä�ú��Ͻð��� �����Ѵ�.
	* \param nUserNum ����ڹ�ȣ
	* \param tBlockTime ä�� ���� ����ð�
	* \return 
	*/
	CSetChatBlockTime(int nUserNum, __time64_t tBlockTime);
	virtual ~CSetChatBlockTime() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nUserNum;
	__time64_t m_tBlockTime;
};

/**
* Ŭ���� ���������� �����Ѵ�.
*/
class CSetClubNotice : public CDbAction
{
public:
	/**
	* Ŭ���� ���������� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param szClubNotice ��������
	* \return 
	*/
	CSetClubNotice(DWORD dwClub, const char* szClubNotice);
	virtual ~CSetClubNotice() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	char  m_szNotice[EMCLUB_NOTICE_LEN+1];
};

/**
* Ŭ���� ����Ŭ�� ���� �븮�ڸ� �����Ѵ�.
*/
class CSetClubDeputy : public CDbAction
{
public:
	/**
	* Ŭ���� ����Ŭ�� ���� �븮�ڸ� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwDeputy �����븮�� ĳ���͹�ȣ
	* \return 
	*/
	CSetClubDeputy(DWORD dwClub, DWORD dwDeputy);
	virtual ~CSetClubDeputy() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	DWORD m_dwDeputy;
};

/**
* Ŭ���� ������ ������ �����Ѵ�.
*/
class CSetClubAuthority : public CDbAction
{
public:
	/**
	* Ŭ���� ����Ŭ�� ���� �븮�ڸ� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwDeputy �����븮�� ĳ���͹�ȣ
	* \return 
	*/
	CSetClubAuthority(DWORD dwClub, DWORD dwMasterID);
	virtual ~CSetClubAuthority() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwClub;
	DWORD m_dwMasterID;
};

/**
* ĳ���� Ȱ�� �α׸� ����Ѵ�.
*/
class CLogAction : public CDbAction
{
public:
	/**
	* ĳ���� Ȱ�� �α׸� ����Ѵ�.
	* \param &_sLOG �αױ���ü
	* \return 
	*/
	CLogAction ( const SLOGACTION &_sLOG );
	virtual ~CLogAction() {};
    virtual int Execute (CServer* pServer);

protected:
	SLOGACTION m_sLOG;
};

/**
* ĳ���� �̸��� �����Ѵ�.
* m_dwChaNum ĳ���͹�ȣ
* m_szCharName ������ ĳ�����̸�
*/
class CRenameCharacter : public CDbAction
{
public:
	CRenameCharacter (DWORD dwClientID,
		              DWORD dwChaNum, 
					  const char* szCharName,
					  WORD wPosX,
					  WORD wPosY);
	virtual ~CRenameCharacter() {};
	virtual int Execute (CServer* pServer);	

protected:
	DWORD m_dwChaNum;	
	char  m_szCharName[CHR_ID_LENGTH];
	WORD  m_wPosX;
	WORD  m_wPosY;
};

//! ĳ������ �Ӹ����� �����Ѵ�.
//! m_dwChaNum : ĳ���͹�ȣ
//! m_nHairColor : �Ӹ�����
class CSetChaHairColor : public CDbAction
{
public:
	CSetChaHairColor(DWORD dwChaNum, 
		                   int nHairColor);
	virtual ~CSetChaHairColor() {};
	virtual int Execute(CServer* pServer);	

protected:
	DWORD m_dwChaNum;
	int   m_nHairColor;
};

//! ĳ������ ��Ÿ���� �����Ѵ�.
//! m_dwChaNum : ĳ���� ��ȣ
//! m_nHairStyle : ��Ÿ��
class CSetChaHairStyle : public CDbAction
{
public:
	CSetChaHairStyle(DWORD dwChaNum,
		                   int nHairStyle);
	virtual ~CSetChaHairStyle() {};
	virtual int Execute(CServer* pServer);	

protected:
	DWORD m_dwChaNum;
	int m_nHairStyle;
};

//! ĳ������ �󱼽�Ÿ���� �����Ѵ�.
//! m_dwChaNum : ĳ���� ��ȣ
//! m_nFaceStyle : �󱼽�Ÿ��
class CSetChaFaceStyle : public CDbAction
{
public:
	CSetChaFaceStyle(DWORD dwChaNum,
		                   int nFaceStyle);
	virtual ~CSetChaFaceStyle() {};
	virtual int Execute(CServer* pServer);	

protected:
	DWORD m_dwChaNum;
	int m_nFaceStyle;
};


// ĳ���� ������ �����Ѵ�.
class CSetChaGenderChange : public CDbAction
{
public:
	CSetChaGenderChange( DWORD dwChaNum, int nClass, int nSex, int nFace, int nHair, int nHairColor );
	virtual ~CSetChaGenderChange() {};
	virtual int Execute(CServer* pServer);	

protected:
	DWORD m_dwChaNum;
	int m_nClass;
	int	m_nSex;
	int m_nFace;
	int m_nHair;
	int m_nHairColor;
};

//! ĳ������ ��ġ������ �����Ѵ�.
class CSaveChaPos : public CDbAction
{
public:
	//! ĳ������ ��ġ������ �����Ѵ�.
	//! \param dwChaNum ĳ���� ��ȣ
	//! \param dwStartmapID �����ϴ� �� ��ȣ
	//! \param dwStartGate �����ϴ� ����Ʈ ��ȣ
	//! \param fStartPosX �����ϴ� X ��ǥ
	//! \param fStartPosY �����ϴ� Y ��ǥ
	//! \param fStartPosZ �����ϴ� Z ��ǥ
	//! \param dwSaveMapID ���������� �÷����� �� ��ȣ
	//! \param fSavePosX ���������� �÷����� X ��ǥ
	//! \param fSavePosY ���������� �÷����� Y ��ǥ
	//! \param fSavePosZ ���������� �÷����� Z ��ǥ
	//! \param dwLastCallMapID ���� ��ȯī�忡 ����� �� ��ȣ
	//! \param fLastCallPosX ���� ��ȯī�忡 ����� X ��ǥ
	//! \param fLastCallPosY ���� ��ȯī�忡 ����� Y ��ǥ
	//! \param fLastCallPosZ ���� ��ȯī�忡 ����� Z ��ǥ	
	CSaveChaPos(DWORD dwClientID,
				DWORD dwChaNum,
				DWORD dwFieldServer,
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
				FLOAT fLastCallPosZ);
	virtual ~CSaveChaPos() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwChaNum;
	DWORD m_dwStartMapID;
	DWORD m_dwFieldServer;
	DWORD m_dwStartGate;
	FLOAT m_fStartPosX;
	FLOAT m_fStartPosY;
	FLOAT m_fStartPosZ;
	DWORD m_dwSaveMapID;
	FLOAT m_fSavePosX;
	FLOAT m_fSavePosY;
	FLOAT m_fSavePosZ;
	DWORD m_dwLastCallMapID;
	FLOAT m_fLastCallPosX;
	FLOAT m_fLastCallPosY;
	FLOAT m_fLastCallPosZ;
};

/**
* ���ο� ���� �����Ѵ�.
*/
class CCreatePet : public CDbAction
{
public:
	/**
	* ���ο� ���� �����Ѵ�.
	* \param nChaNum  �� ������ ĳ���� ��ȣ
	* \param szPetName ���̸�
	* \param nPetType ���� Ÿ��
	* \param nPetMID �� MID
	* \param nPetSid �� SID
	* \param nPetStyle ���� ��Ÿ��
	* \param nPetColor ���� �÷�
	* \return DB_ERROR �Ǵ� ������������ Pet �� ������ȣ
	*/
	CCreatePet(
		DWORD dwClientID,
		int nChaNum,
		const TCHAR* szPetName,
		int nPetType,
		int nPetMID,
		int nPetSID,
		int nPetStyle,
		int nPetColor,
		WORD wPosX,
		WORD wPosY,
		int nPetCardMID,
		int nPetCardSID
		);
	virtual ~CCreatePet() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	CString m_strPetName;
	int m_nPetType;
	int m_nPetMID;
	int m_nPetSID;
	int m_nPetStyle;
	int m_nPetColor;
	
	WORD m_wPosX;
	WORD m_wPosY;

	int m_nPetCardMID;
	int m_nPetCardSID;
};

/**
* ���� �̸��� �����Ѵ�.
*/
class CRenamePet : public CDbAction
{
public:
	/**
	* ���� �̸��� �����Ѵ�.
	* \param nPetNum ���� ������ȣ
	* \param szPetName ���̸�	
	*/
	CRenamePet(DWORD dwClientID, int nChaNum, int nPetNum, const TCHAR* szPetName, WORD wPosX, WORD wPosY);
	virtual ~CRenamePet() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	int m_nPetNum;
	CString m_strPetName;
	WORD m_wPosX;
	WORD m_wPosY;
};

/**
* ���� �������� �����Ѵ�.
*/
class CExchangePet : public CDbAction
{
public:
	/**
	* ���� �������� �����Ѵ�.
	* \param nChaNum ���� ���ο� ������
	* \param nPetNum �������� ������ �� ��ȣ
	*/
	CExchangePet(int nChaNum, int nPetNum);
	virtual ~CExchangePet() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	int m_nPetNum;
};


/**
* ���� �÷��� �����Ѵ�.
*/
class CSetPetColor : public CDbAction
{
public:
	/**
	* ���� �÷��� �����Ѵ�.
	* \param nPetNum �÷��� �����Ϸ��� ���� ��ȣ
	* \param nPetColor ���� �÷�
	*/
	CSetPetColor(int nChaNum, int nPetNum, int nPetColor);
	virtual ~CSetPetColor() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	int m_nPetNum;
	int m_nPetColor;
};

/**
* ���� ��Ÿ���� �����Ѵ�.
*/
class CSetPetStyle : public CDbAction
{
public:
	/**
	* ���� ��Ÿ���� �����Ѵ�.
	* \param nPetNum ��Ÿ���� �����Ϸ��� ���� ��ȣ
	* \param nPetStyle ���� ��Ÿ��
	*/
	CSetPetStyle(int nChaNum, int nPetNum, int nPetStyle);
	virtual ~CSetPetStyle() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	int m_nPetNum;
	int m_nPetStyle;
};

/**
* ���� �������� �����Ѵ�. �������� 0-100 ����
*/
class CSetPetFull : public CDbAction
{
public:
	/**
	* ���� �������� �����Ѵ�. �������� 0-100 ����
	* \param nPetNum �������� ������ ���� ��ȣ
	* \param fPetFull ������
	*/
	CSetPetFull(int nChaNum, int nPetNum, int nPetFull);
	virtual ~CSetPetFull() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	int m_nPetNum;
	int m_nPetFull;
};

/**
* ���� ��ų ������Ʈ
*/
class CSetPetSkill : public CDbAction
{
public:
	CSetPetSkill(
		int nChaNum,
		int nPetNum,
		PETSKILL& PetSkill,
		bool bActive,
		WORD wSkillOrder);
	virtual ~CSetPetSkill() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	int m_nPetNum;
	PETSKILL m_PetSkill;
	bool m_bActive;
	WORD m_wSkillOrder;
};

/**
* ���� �κ��丮 ������Ʈ
*  
*/
class CSetPetInven : public CDbAction
{
public:
	CSetPetInven(
		int nChaNum,
		int nPetNum,
		GLPET* pPet);
	virtual ~CSetPetInven() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	int m_nPetNum;
	CByteStream m_ByteStream;
};



/**
* ���� �����Ѵ�.
*/
class CDeletePet : public CDbAction
{
public:
	/**
	* ���� �����Ѵ�.
	* \param nPetNum ������ ���� ��ȣ
	*/
    CDeletePet(int nChaNum, int nPetNum);
	virtual ~CDeletePet() {};
	virtual int Execute(CServer* pServer);

protected:
	int m_nChaNum;
	int m_nPetNum;
};

class CSetPetDualSkill : public CDbAction
{
public:
	CSetPetDualSkill(int nChaNum, int nPetNum);
	virtual ~CSetPetDualSkill(){};
	virtual int Execute(CServer* pServer);
protected:
	int m_nChaNum;
	int m_nPetNum;
};
/**
* ���� ������ �����´�.
*/
class CGetPet : public CDbAction
{
public:
    CGetPet(GLPET* pPet, DWORD dwPetNum, DWORD dwClientID, DWORD dwCharNum, WORD wPosX, WORD wPosY, bool bLMTItemCheck=false, bool bCardInfo=false, bool bTrade=false );
	virtual ~CGetPet() {};
	virtual int Execute(CServer* pServer);

protected:
	GLPET* m_pPet;
	DWORD  m_dwPetNum;
	bool   m_bLMTItemCheck;
	bool   m_bCardInfo;
	bool   m_bTrade;
	DWORD  m_dwCharNum;
	WORD   m_wPosX;
	WORD   m_wPosY;
};

/**
* ���� �������� �����´�
* \param dwPetNum �� ��ȣ
* \return ���� ������ or DB_ERROR
*/
class CGetPetFull : public CDbAction
{
public:
    CGetPetFull( int nChaNum, DWORD dwPetNum, DWORD dwClientID, WORD wCureVolume, BOOL bRatio, SNATIVEID sID, PETTYPE emType );
	virtual ~CGetPetFull() {};
	virtual int Execute( CServer* pServer );

protected:
	int	m_nChaNum;
	DWORD m_dwPetNum;
	int m_nFull;
	WORD m_wCureVolume;
	BOOL m_bRatio;
	SNATIVEID m_sNativeID;
	PETTYPE m_emType;
};

/**
* �Ҹ�� ���� ����Ʈ�� �����´�.
* \param nChaNum �Ҹ�� �� ����Ʈ�� ����Ϸ��� ĳ���� ��ȣ
* \reutrn DB_OK or DB_ERROR
*/
class CGetRestorePetList : public CDbAction
{
public:
	CGetRestorePetList( int nChaNum, DWORD dwClientID );
	virtual ~CGetRestorePetList() {};
	virtual int Execute( CServer* pServer );
    
protected:
	int m_nChaNum;
};

/**
* ���� ��Ȱ��Ų��.
* \param nPetNum ��Ȱ�� �� ��ȣ
* \param nChaNum ��Ȱ�� �䱸�� ĳ���� ��ȣ
* \return �� ��ȣ or DB_ERROR
*/
class CRestorePet : public CDbAction
{
public:
	CRestorePet( DWORD dwPetNum, DWORD dwClientID, int nChaNum, WORD wPosX, WORD wPosY );
	virtual ~CRestorePet() {};
	virtual int Execute( CServer* pServer);

protected:
	int		m_dwPetNum;
	int		m_nChaNum;
	WORD    m_wPosX;
	WORD    m_wPosY;
};


/**
* �� ��Ų ���� ī�带 �����Ų��.
* \param nPetNum ��Ȱ�� �� ��ȣ
* \param nChaNum ��Ȱ�� �䱸�� ĳ���� ��ȣ
* \return �� ��ȣ or DB_ERROR
*/
class CPetSkinPack : public CDbAction
{
public:
	CPetSkinPack( int nChaNum, DWORD dwPetNum, int nPetSkinMID, int nPetSkinSID, int nPetSkinScale, int nPetSkinTime );
	virtual ~CPetSkinPack() {};
	virtual int Execute( CServer* pServer);

protected:
	int   m_nChaNum;
	DWORD m_dwPetNum;
	int   m_nPetSkinMID;
	int   m_nPetSkinSID;
	int   m_nPetSkinScale;
	int   m_nPetSkinTime;
};

/**
* SMS ���ڸ޽����� �߼��Ѵ�.
*/
class CSendSMS : public CDbAction
{
public:
	/**
	* SMS ���ڸ޽����� �߼��Ѵ�.
	* \param nSendChaNum SMS �� �߼��ϴ� ĳ���� ��ȣ
	* \param szSendUserID SMS �� �߼��ϴ� ����� ID
	* \param nReceiveChaNum SMS �� ���� ĳ���� ��ȣ
	* \param szReceivePhone SMS �� ���� ĳ������ ��ȭ��ȣ
	* \param szSmsMsg SMS �޽��� ����
	*/
	CSendSMS(
		DWORD dwClientNum,
		DWORD dwSendChaNum,
		DWORD dwReceiveChaNum,
		TCHAR* szReceivePhone,
		TCHAR* szSendUserID,
		TCHAR* szSmsMsg,
		WORD wPosX,
		WORD wPosY );
	virtual ~CSendSMS() {};
	virtual int Execute( CServer* pServer );

protected:
	DWORD m_dwSendChaNum;
	DWORD m_dwReceiveChaNum;
	TCHAR m_szReceivePhone[SMS_RECEIVER];	
	TCHAR m_szSmsMsg[SMS_LENGTH];
	bool m_bSMS;
	WORD m_wPosX;
	WORD m_wPosY;
};

/**
* ĳ������ �޴��� ��ȣ�� ����/������Ʈ�Ѵ�.
*/
class CSetChaPhoneNumber : public CDbAction
{
public:
	/**
	* ĳ������ �޴��� ��ȣ�� ����/������Ʈ�Ѵ�.
	* \param nChaNum ĳ���� ��ȣ
	* \param szPhoneNumber ��ȭ��ȣ NULL ���� ���� 14�� SMS_RECEIVER
	*/
    CSetChaPhoneNumber(
		DWORD dwClientNum,
		DWORD dwChaNum,
		const TCHAR* szPhoneNumber );
	virtual ~CSetChaPhoneNumber() {};
	virtual int Execute( CServer* pServer );

protected:
	DWORD m_dwChaNum;
	TCHAR m_szPhoneNumber[SMS_RECEIVER];
	bool m_bSMS;
};


/**
* ���ο� Ż���� �����Ѵ�.
* \param nChaNum  Ż�� ������ ĳ���� ��ȣ
* \param szVehicleName Ż�� �̸�
* \param nVehicleType Ż���� Ÿ��
* \param nVehicleCardMID Ż���� ������ MID
* \param nVehicleCardSID Ż���� ������ SID
* \return DB_ERROR �Ǵ� ������������ Ż���� ������ȣ
*/
class CCreateVehicle : public CDbAction
{
public:
	CCreateVehicle( 
		DWORD dwClientID,
		DWORD dwCharNum,
		const TCHAR* szVehicleName,
		int nVehicleType,
		int nVehicleCardMID,
		int nVehicleCardSID
		);
	virtual ~CCreateVehicle() {};
	virtual int Execute(CServer* pServer);

protected:

	DWORD m_dwCharNum;
	CString m_strVehicleName;
	int m_nVehicleType;
	int m_nVehicleCardMID;
	int m_nVehicleCardSID;
};


/**
* Ż���� ���͸��� �����Ѵ�. ���͸��� 0-1000 ����
* \param nVehicleBttery ���͸��� ������ Ż���� ��ȣ
* \param nVehicleBttery ���͸�
*/
class CSetVehicleBattery : public CDbAction
{
public:
	CSetVehicleBattery(
		DWORD dwClientID,
		DWORD dwCharNum,
		int nVehicleNum,
		int nVehicleBttery
		);
	virtual ~CSetVehicleBattery() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwCharNum;
	int m_nVehicleNum;
	int m_nVehicleBttery;
};

class CSetVehicleColor : public CDbAction //add bike color
{
public:
	CSetVehicleColor(
		DWORD dwClientID,
		DWORD dwCharNum,
		int nVehicleNum,
		int nVehicleColor,
		int nVehicleColor1,
		int nVehicleColor2,
		int nVehicleColor3,
		int nVehicleColor4,
		int nVehicleColor5
		);
	virtual ~CSetVehicleColor() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD m_dwCharNum;
	int m_nVehicleNum;
	int m_nVehicleColor;
	int m_nVehicleColor1;
	int m_nVehicleColor2;
	int m_nVehicleColor3;
	int m_nVehicleColor4;
	int m_nVehicleColor5;
};



/**
* Ż���� �������� �����´�
* \param nVehicleNum Ż�� ��ȣ
* \return Ż�� ������ or DB_ERROR
*/

class CGetVehicleBattery : public CDbAction
{
public:
	CGetVehicleBattery( 
		DWORD dwClientID, 
		DWORD dwCharNum,
		int	  nVehicleNum, 
		SNATIVEID	sItemID,
		WORD  wCureVolume, 
		BOOL  bRatio, 
		SNATIVEID sBatteryID,
		VEHICLE_TYPE emType );

	virtual ~CGetVehicleBattery() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD		m_dwCharNum;
	int			m_nVehicleNum;
	SNATIVEID	m_sItemID;
	int			m_nVehicleBttery;
	WORD		m_wCureVolume;
	BOOL		m_bRatio;
	SNATIVEID	m_sBatteryID;
	VEHICLE_TYPE m_emType;
};

/**
* Ż���� �����Ѵ�.
* \param nVehicle ������ Ż���� ��ȣ
*/
class CDeleteVehicle : public CDbAction
{
public:
	CDeleteVehicle( 
		DWORD dwClientID,
		DWORD dwCharNum,
		int nVehicleNum
		);

	virtual ~CDeleteVehicle() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD	m_dwCharNum;
	int		m_nVehicleNum;	
};

/**
* Ż���� ������ �����´�.
*/
class CGetVehicle : public CDbAction
{
public:
	CGetVehicle(
		GLVEHICLE* pVehicle,
		DWORD dwVehicleNum,
		DWORD dwClientID,
		DWORD dwCharNum,
		bool bLMTItemCheck=false, 
		bool bCardInfo=false, 
		bool bTrade=false
		);

	virtual ~CGetVehicle() {};
	virtual int Execute(CServer* pServer);

protected:
	GLVEHICLE* m_pVehicle;
	DWORD m_dwVehicleNum;
	DWORD	m_dwCharNum;
	bool  m_bLMTItemCheck;
	bool  m_bCardInfo;
	bool  m_bTrade;
};

/**
* Ż�� �κ��丮 ������Ʈ
*  
*/
class CSetVehicleInven : public CDbAction
{
public:
	CSetVehicleInven(
		DWORD dwClientID,
		DWORD dwCharNum,
		int nVehicleNum,		
		GLVEHICLE* pVehicle
		);
	virtual ~CSetVehicleInven() {};
	virtual int Execute(CServer* pServer);

protected:
	DWORD	m_dwCharNum;
	int m_nVehicleNum;
	CByteStream m_ByteStream;
};

/**
* Ż���� �׼� �α׸� �����.
* \param _sLOG ��׼Ƿα� ����ü
*/
class CLogVehicleAction : public CDbAction
{
public:
	CLogVehicleAction( const SLOGVEHICLEACTION& sLOG );
	virtual ~CLogVehicleAction() {};
	virtual int Execute( CServer* pServer );

protected:
	SLOGVEHICLEACTION m_sLOG;
};


class CInsertUserAttend : public CDbAction
{
public:
	CInsertUserAttend( DWORD dwUserID, int nComboAttend, int nAttendReward );
	virtual ~CInsertUserAttend() {};
	virtual int Execute( CServer* pServer );

protected:
	DWORD m_dwUserID;
	int	  m_nComboAttend;
	int	  m_nAttendReward;
};

class CInsertAttendItem : public CDbAction
{
public:
	CInsertAttendItem( const CString& strPurKey, const CString& strUserID, int nItemMID, int nItemSID );
	virtual ~CInsertAttendItem() {};
	virtual int Execute ( CServer* pServer );

protected:
	CString m_strPurKey;
	CString m_strUserID;
	int	  m_nItemMID;
	int	  m_nItemSID;
};

class CThaiCafeClassCheck : public CDbAction
{
public:
	CThaiCafeClassCheck ( const DWORD dwClientID, const DWORD dwCClassType, const TCHAR *pszIP );
	virtual ~CThaiCafeClassCheck() {};
	virtual int Execute ( CServer* pserver );
protected:
	DWORD m_dwClientID;
	DWORD m_dwCClassType;
	TCHAR m_szIp[MAX_IP_LENGTH+1];
};

class CMyCafeClassCheck : public CDbAction
{
public:
	CMyCafeClassCheck ( const DWORD dwClientID, const DWORD dwCClassType, const TCHAR *pszIP );
	virtual ~CMyCafeClassCheck() {};
	virtual int Execute ( CServer* pserver );
protected:
	DWORD m_dwClientID;
	DWORD m_dwCClassType;
	TCHAR m_szIp[MAX_IP_LENGTH+1];
};

class CUpdateUserMoneyAdd : public CDbAction
{
public:
	CUpdateUserMoneyAdd ( const DWORD dwUserID, const LONGLONG lnUserMoney );
	virtual ~CUpdateUserMoneyAdd() {};
	virtual int Execute ( CServer* pserver );
protected:
	DWORD m_dwUserID;
	LONGLONG m_lnUserMoney;
};

class CUpdateUserLastInfoAdd : public CDbAction
{
public:
	CUpdateUserLastInfoAdd ( const DWORD dwUserID, const LONGLONG lnUserMoney );
	virtual ~CUpdateUserLastInfoAdd() {};
	virtual int Execute ( CServer* pserver );
protected:
	DWORD m_dwUserID;
	LONGLONG m_lnUserMoney;
};

#endif // DBACTIONLOGIC_H_