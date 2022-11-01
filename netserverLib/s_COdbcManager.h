#ifndef _CODBCMANAGER_H_
#define _CODBCMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <stdio.h>
#include <odbcinst.h>
#include <sqlext.h> // ODBC library

#include <vector>
//#include <strstream>
#include <sstream>
#include <hash_map>

#include "s_NetGlobal.h"
#include "s_CCfg.h"
#include "s_COdbcSupervisor.h"
#include "s_CLock.h"
#include "s_CConsoleMessage.h"

#include "../EngineLib/G-Logic/GLDBMan.h"

#include "../RanClientLib/G-Logic/GLCharData.h"
#include "../RanClientLib/G-Logic/GLContrlMsg.h"
#include "../RanClientLib/G-Logic/GLCharAG.h"
#include "../RanClientLib/G-Logic/GLClubMan.h"
#include "../RanClientLib/G-Logic/GLGuidance.h"
#include "../RanClientLib/G-Logic/GLPet.h" // Pet Define

/**
 * \ingroup NetServerLib
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
 * \date 2002-05-28
 *
 * \author jgkim
 *
 * \par license
 * Copyright 2002-2005 (c) Mincoms. All rights reserved. 
 * \todo 
 * 
 * \n Board Database ���õ� �Լ��� ���� ����. (�������� ����)
 *
 * \bug 
 *
 */
class COdbcManager : public GLDBMan, public CLock
{
public:	
	static COdbcManager* GetInstance();
	static void ReleaseInstance();

private:
// public:
	COdbcManager(void);
	~COdbcManager(void);
	static COdbcManager* SelfInstance;

protected:
   	COdbcSupervisor* m_pGameDB;
	COdbcSupervisor* m_pUserDB;
	COdbcSupervisor* m_pBoardDB;
	COdbcSupervisor* m_pLogDB;
	COdbcSupervisor* m_pShopDB;
	COdbcSupervisor* m_pTerraDB;
	COdbcSupervisor* m_pGspDB;
	COdbcSupervisor* m_pThaiDB;
	COdbcSupervisor* m_pKorDB;
	COdbcSupervisor* m_pMyDB;			 // �����̽þ� PC�� �̺�Ʈ
	int              m_nServiceProvider; ///< ���� ��������

public:
	void SetServiceProvider(int nServiceProvider);

	int	OpenGameDB(
			TCHAR* szOdbcName, 
			TCHAR* szUsrID, 
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseGameDB();

	int	OpenUserDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID,
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseUserDB();

	int	OpenBoardDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID, 
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseBoardDB();

	int	OpenLogDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID,
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseLogDB();

	int OpenShopDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID,
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseShopDB();
	
	int	OpenTerraDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID, 
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseTerraDB();

	int	OpenGspDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID, 
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseGspDB();

	int	OpenThaiDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID, 
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseThaiDB();

	int	OpenKorDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID, 
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseKorDB();

	// �����̽þ� PC�� �̺�Ʈ
	int	OpenMyDB(
			TCHAR* szOdbcName,
			TCHAR* szUsrID,
			TCHAR* szUsrPasswd,
			TCHAR* szDbName,
			int nPoolSize = DB_POOL_SIZE,
			int nDBTimeOut = DB_RESPONSE_TIME );
	void CloseMyDB();


	void Print(const CString & strMsg);
	CString	GetErrorString(SQLHSTMT hStmt);
	void CheckConnection();

	/**************************************************************************
	* Game Database
    */
	//! ����� �κ��丮���� ����Ÿ�� �о�´�.
	int	ReadUserInven(
			int SGNum,
			DWORD dwUserNum,
			CByteStream &ByteStream );

	//! �����κ��丮�� �ִ��� üũ�Ѵ�.
	bool CheckInven( int SGNum, DWORD dwUserNum );
	int	MakeUserInven( int SGNum, DWORD dwUserNum );
	//! ����� �κ��丮�� ����Ÿ�� �����Ѵ�.
	int	WriteUserInven( 
			int SGNum, 
			LONGLONG llMoney /* LONGLONG __int64 */,
			DWORD dwUserNum,
			BYTE* pData,
			int nSize);
	
	// ���ο� ĳ���͸� �����Ѵ�.
	int	CreateNewCharacter( SCHARDATA2* pCharData2 );
	// �븸,ȫ��
	// ĳ���� ������ Temp Table�� ĳ���� ������ �ݾ� ������ ��ȣȭ �ؼ� ������ �д�.
	int	InsertCharInfoTemp( int nChaNum );

	virtual int ReadUserInven( SCHARDATA2* pChaData2 );

	//! ĳ���͸� �����Ѵ�
	virtual int	SaveCharacter( LPVOID _pbuffer );
	virtual void AddJob    ( CDbAction *pDbActJob );
	virtual void AddLogJob ( CDbAction *pDbActJob );
	virtual void AddUserJob( CDbAction *pDbActJob );
	virtual void AddWebJob ( CDbAction *pDbActJob );

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
	//! \return DB_OK, DB_ERROR
	int	SaveChaPos(
			DWORD dwChaNum,
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
			FLOAT fLastCallPosZ );

	/**
	* ĳ���� �̸��� �����Ѵ�.
	* \param dwChaNum ĳ���͹�ȣ
	* \param szCharName ������ ĳ�����̸�
	* \return DB_OK, DB_ERROR
	*/
	int RenameCharacter(
			DWORD dwChaNum, 
			TCHAR* szCharName );

	/**
	* ĳ���� ������ �����´� (���̳ʸ� ���� ����Ÿ)
	* \param nUserNum ����ڹ�ȣ
	* \param nChaNum ĳ���͹�ȣ
	* \return ĳ��������
	*/
	/*
	SCHARDATA2*	GetCharacter(
					int nUserNumber,
					int nChaNum );
	*/

	int GetChaPhoneNumber(
			int nChaNum,
			TCHAR* szPhoneNumber);
	/**	
	* ĳ���� ������ �����´�. ĳ������ ������� (���̳ʸ� ����)
	* \param nUserNum ����ڹ�ȣ
	* \param nChaNum ĳ���͹�ȣ
	* \param pChaData2 ĳ���� �����͸� ���� ����Ʈ
	* \return DB_OK, DB_ERROR
	*/
	int GetCharacterInfo(
			int nUserNumber,
		    int nChaNum,
			SCHARDATA2* pChaData2 );
	/**
	* ĳ���Ͱ� ������ ��, ����Ʈ, ��ġ�� �����´�
	* \param nUserNum ����ڹ�ȣ
	* \param nChaNum ĳ���͹�ȣ
	* \param pChaData ĳ���� �����͸� ���� ����Ʈ
	* \return DB_OK, DB_ERROR
	*/
	int	GetCharacterInfo(
			int nUserNumber,
		    int nChaNum,
			GLCHARAG_DATA* pChaData );
	
	//! �ش� ������� ĳ���� ������ �����´�.
	int	GetChaAllInfo( int nUsrNum );
	int	GetChaBAInfo( int nUsrNum, int nSvrGrp, NET_CHA_BBA_INFO* ncbi );
	int	GetChaBInfo( int nUserNum, int nChaNum, SCHARINFO_LOBBY* sci );
	
	// ĳ���� ����	
	int	DelCharacter( int nUsrNum, int nChaNum, const TCHAR* szPass2 );
	// ĳ���� ����
	int	DaumDelCharacter( int nUsrNum, int nChaNum );
	// ĳ���� ����
	int	TerraDelCharacter( int nUsrNum, int nChaNum );
	// GSP ĳ���� ����
	int	GspDelCharacter( int nUsrNum, int nChaNum );
	// ĳ���� ����
	int	ExciteDelCharacter( int nUsrNum, int nChaNum );
	// Japan ĳ���� ����
	int	JapanDelCharacter( int nUsrNum, int nChaNum );
	// ĳ���� ����	
	int	GsDelCharacter( int nUsrNum, int nChaNum );
	// ���� ĳ���� ���� �Լ�
	int	DeleteCharacter( int nUsrNum, int nChaNum );


	// ��Ʈ�� Ž������ �ý����� �߰� ���� �ҷ����� �κ� �߰�
	int COdbcManager::GetVTCharInfo( int nChaNum, SCHARDATA2* pChaData2 );
	// �븸 ��ŷ ���� �����ؼ� �ֱ� �α׾ƿ� �������� ������ DB���� �о��.
	int COdbcManager::GetLastCharInfo( int nUserNum, int nChaNum,SCHARDATA2* pChaData2 );

	/**
	* ĳ������ ģ�������� �����´�.
	*/
	int	GetChaFriend(
			int nChaNum,
			std::vector<CHA_FRIEND> &vFriend );

	int	DelChaFriend(
			int nChaP,
			int nChaS );
	int AddChaFriend(
			int nChaP,
			int nChaS );

	int SetChaFriend(
			int nChaP,
			int nChaS,
			int nFlag );

	int	LogPartyMatch(
			int nSGNum,
			int nSvrNum,
			WORD wWin,
			WORD wLost ); // �б�vs�б� ��Ƽ��ð������
        
    /**
    * �ش� ĳ������ ����ġ�� �����Ѵ�.    
    * ���� 
    * ĳ���͹�ȣ�� 0 ���� Ŀ���Ѵ�.
    * ����ġ�� 0 ���� Ŀ���Ѵ�. 
	* \param nChaNum ĳ���͹�ȣ 
	* \param llExp ����ġ
	* \return 
	*/
    int SetChaExp(int nChaNum, LONGLONG llExp);
    
    /**
    * �ش� ĳ������ ����ġ�� �����´�.    
	* ����
	* ĳ���͹�ȣ�� 0 ���� Ŀ���Ѵ�.
	* ����ġ�� 0 ���� ������ �����̴�.
    * \param nChaNum  ĳ���͹�ȣ
    * \return (LONGLONG) ����ġ
    */
    LONGLONG GetChaExp(int nChaNum);

    /**
    * �ش� ��Ŀ ����ð��� ���Ѵ�.
    * 1 �� 5 �� ��Ŀ�� ���� �� ����.
    * 2, 3, 4 �� ��Ŀ�� ����ð��� ���� �� �ִ�.
    * �Է�
    * \param nUserNum ����ڰ�����ȣ
    * \param nStorageNum ���丮�� ��ȣ
    * \param tTime ����ð�
    * \return -1 DB_ERROR 0 : DB_OK
	*/
	int SetChaStorageDate(int nUserNum, int nStorageNum, __time64_t tTime);

    /**
    * �ش� ĳ������ �κ��丮 �߰� �ټ��� �����Ѵ�.
    * \n ���� 
    * \n ���� 0 ���� ���� �߰��� 1 �� �Է� 
    * \n ���� ���� ���¿��� �� ���� �߰��� 2 �� �Է� (���� �ټ��� �Է��Ѵ�)
    * \n ���� ���� ���¿��� ���� �߰��� 3 �� �Է� 
    * \n �ִ� 3������ �Է°���.
	* \param nChaNum ĳ���͹�ȣ
    * \param wInvenLine �߰��ټ� 	
	* \return 
	*/
	int SetChaInvenNum(int nChaNum, WORD wInvenLine=0);

	/**
	* ������ȯ ī�� ����� ���� �������� �����Ѵ�.
	* \param nChaNum ĳ���͹�ȣ
	* \param dwMapID �� ID
	* \param vPos    ĳ���� ��ġ
	* \return 
	*/
    int SetLastCallPos(int nChaNum, DWORD dwMapID, D3DXVECTOR3 vPos);
 
	/**
	* ���ο� Ŭ���� �����Ѵ�.
	* \param strClubName Ŭ���̸�
	* \param dwChaNum    Ŭ���� �����Ϸ��� ĳ���� ��ȣ (�� ĳ���Ͱ� Ŭ���� �����Ͱ� �ȴ�)
	* \return 
	*/
    int CreateClub(CString strClubName, DWORD dwChaNum);

	/**
	* Ŭ���� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwChaNum Ŭ���� �����Ϸ��� ĳ���͹�ȣ(ĳ���� ��ȣ�� Ŭ�� �����Ͱ� �ƴϸ� ������ ���� �ʴ´�)
	* \return 
	*/
    int DeleteClub(DWORD dwClub, DWORD dwChaNum);    

	/**
	* Ŭ����ŷ�� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwRank ��ŷ
	* \return 
	*/
    int SetClubRank(DWORD dwClub, DWORD dwRank);    

	/**
	* Ŭ����ũ �̹����� �����´�.
	* \param dwClub Ŭ����ȣ
	* \param &ByteStream ����
	* \return 
	*/
    int ReadClubMarkImage(DWORD dwClub, CByteStream &ByteStream);    

	/**
	* Ŭ����ũ �̹����� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwMarkVer Ŭ����ũ����
	* \param pData Ŭ���̹��� �ּ�
	* \param nSize Ŭ���̹��� ������
	* \return 
	*/
    int	WriteClubMarkImage(
			DWORD dwClub,
			DWORD dwMarkVer,
			BYTE* pData,
			int nSize );
	
	/**
	* �ش�Ŭ���� �����ݾ��� �����Ѵ�. (Storage)
	* \param dwClub Ŭ����ȣ
	* \param llMoney �����ݾ� (�ݾ��� >= 0)
	* \return 
	*/
    int	SetClubMoney(DWORD dwClub, LONGLONG llMoney);    

	/**
	* �ش�Ŭ���� �����ݾ��� �����´�. (Storage)
	* \param dwClub Ŭ����ȣ
	* \return ���ϰ��� >= 0, 0 ���� ������ ERROR
	*/
    LONGLONG GetClubMoney(DWORD dwClub);

	/**
	* �ش�Ŭ���� ���Աݾ��� �����Ѵ�. (Income money)
	* \param dwClub Ŭ����ȣ
	* \param llMoney ���Աݾ� (�ݾ��� >= 0)
	* \return 
	*/
    int SetClubIncomeMoney(DWORD dwClub, LONGLONG llMoney);    
    
	/**
	* �ش�Ŭ���� ���Աݾ��� �����´�. (Income money)
	* \param dwClub Ŭ����ȣ
	* \return >= 0, 0 ���� ������ ERROR
	*/
    LONGLONG GetClubIncomeMoney(DWORD dwClub);    

	/**
	* �ش�Ŭ���� Ŭ��â���� �о�´�.
	* \param dwClub Ŭ����ȣ
	* \param &ByteStream ����
	* \return 
	*/
    int ReadClubStorage(DWORD dwClub, CByteStream &ByteStream);    

	/**
	* �ش�Ŭ���� Ŭ��â���� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param pData â����ġ
	* \param nSize ���� ��üũ��
	* \return 
	*/
    int	WriteClubStorage(DWORD dwClub, BYTE* pData, int nSize);

	/**
	* Ŭ�� ��ü�ð��� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDiss ��ü�ð�
	* \return 
	*/
    int SetClubDissolutionTime(DWORD dwClub, __time64_t tDiss);

	/**
	* Ŭ���� ����Ż�� Ȥ�� �����ð��� ���Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tSec �ð�
	* \return 
	*/
	int SetClubAllianceSec(DWORD dwClub, __time64_t tSec);

	/**
	* Ŭ���� �����ػ� �ð��� ���Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDis �ػ�ð�
	* \return 
	*/
	int SetClubAllianceDis(DWORD dwClub, __time64_t tDis);

	/**
	* Ŭ���� ���������� �ð��� ���Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param tDis ������ ���ӽð�
	* \return 
	*/
	int SetClubAuthorityTime(DWORD dwClub, __time64_t tAuthority);

	/**
	* ������ ��� Ŭ�������� �����´�.
	* \param &vClub Ŭ������
	* \return 
	*/
    int GetClubInfo(std::vector<GLCLUBINFO> &vClub);
	
	/**
	* Ŭ���� ���������� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param szClubNotice ��������
	* \return 
	*/
	int SetClubNotice(DWORD dwClub, const TCHAR* szClubNotice);

	/**
	* Ŭ���� ����Ŭ�� ���� �븮�ڸ� �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwDeputy �븮������ ĳ���͹�ȣ
	* \return 
	*/
	int SetClubDeputy(DWORD dwClub, DWORD dwDeputy);

	/**
	* Ŭ���� ������ ������ �����Ѵ�.
	* \param dwClub Ŭ����ȣ
	* \param dwMasterID ������ ���� ������ �ɸ���
	* \return 
	*/

	int SetClubAuthority (DWORD dwClub, DWORD dwMasterID);

	/**
	* �ش� Ŭ���� ����� �����´�
	* \param dwClub Ŭ����ȣ
	* \param &vMember Ŭ�����
	* \return 
	*/
    int GetClubMember(DWORD dwClub, std::vector<GLCLUBMEMBER> &vMember);
    
	/**
	* ���ο� ĳ���͸� Ŭ���� ���Խ�Ų��
	* \param dwClub Ŭ����ȣ
	* \param dwChaNum ���Խ�Ű���� ĳ���͹�ȣ
	* \return 
	*/
    int AddClubMember(DWORD dwClub, DWORD dwChaNum);

	/**
	* ���ԵǾ� �ִ� ĳ���͸� Ŭ������ Ż���Ų��
	* \param dwChaNum ĳ���͹�ȣ
	* \return 
	*/
    int DeleteClubMember(DWORD dwChaNum);

	/**
	* ĳ������ Ŭ����ȣ�� �����´�.
	* \param dwChaNum ĳ���͹�ȣ
	* \return 
	*/
    int GetChaGuildNum(DWORD dwChaNum);    

	/**
	* ������ �������� ������ Ŭ�� ������ ���Ѵ�.
	* \param dwRegionID ���� ID
	* \param dwClub Ŭ�� ID
	* \param fTax ����
	* \return 
	*/
    int SetClubRegion(DWORD dwRegionID, DWORD dwClub=0, float fTax=0);

	//add sw buff
	int SetSwRegion(DWORD dwSwID, DWORD dwSwNum=3);
	/**
	* �ش� ������ ���� ���¸� �����Ѵ�.
	* \param dwRegionID ���� ID
	* \param dwClub Ŭ�� ID (�⺻ 0 �̴�)
	* \return 
	*/
    int DelClubRegion(DWORD dwRegionID, DWORD dwClub=0);    
    
	/**
	* ���� ��������� ���� Ŭ�������͸� �����´�.
	* \n ����:���ʿ��� ������ ���� ����.
	* \param &vGUID_DB 
	* \return 
	*/
    int GetClubRegion(std::vector<GLGUID_DB> &vGUID_DB);

	//add sw buff
	int GetSwRegion(std::vector<GLSWID> &vSWID_DB);
	/**
	* �� Ŭ������ �����Ѵ�
	* \param dwClub Ŭ����ȣ
	* \param dwChaNum ĳ���͹�ȣ
	* \param dwSubMasterFlags �÷��� (0 ���� ������ �Ϲݱ����� �ȴ�)
	* \return 
	*/
	int	SetClubMasterFlags(DWORD dwClub, DWORD dwChaNum, DWORD dwSubMasterFlags);

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
	int	SetClubAlliance(DWORD dwClubP, DWORD dwClubS);

	/**
	* ���� Ŭ���� ��ü�Ѵ�.
	* \param dwClubP �� Ŭ����ȣ (���� Ŭ���� ��)
	* \param dwClubS ���� Ŭ����ȣ (�� Ŭ�� ������ ���� ���� Ŭ��)
	* \return 
	*/
	int	DelClubAlliance(DWORD dwClubP, DWORD dwClubS);

	/**
	* ���� Ŭ�������� �����´�.
	* \n ���� ���� Logic�ʿ��� ������ �ȵȰ���� �����Ǿ� ���� �ʴ�.
	* \return 
	*/
	int	GetClubAlliance ( std::vector< std::pair<DWORD,DWORD> > &vecAlliance );
	
	/**
	* Ŭ�� ��Ʋ�� ��û�Ѵ�.
	* \param dwClubP ��Ŭ����ȣ( Ŭ�� ��Ʋ�� ��û�� ���� �� )
	* \param dwClubS ���� Ŭ����ȣ( Ŭ�� ��Ʋ�� ��û�� �� Ŭ�� )
	* \return
	*/
	int SetClubBattle( DWORD dwClubP, DWORD dwClubS, DWORD dwEndTime, int nAlliance );

	/**
	* Ŭ�� ��Ʋ�� �����Ѵ�.
	* \param dwClubP ��Ŭ�� ��ȣ( Ŭ�� ��Ʋ�� ��û�� ���� �� )
	* \param dwClubS ���� Ŭ�� ��ȣ( Ŭ�� ��Ʋ�� ��û�� �� Ŭ�� )
	* \param nGuFlag ���п���
	* \param nGuKillNum Ŭ�� ��Ʋ ų��
	* \return
	*/
	int EndClubBattle( DWORD dwClubP, DWORD dwClubS, int nGuFlag, int nGuKillNum, int nGuDeathNum, bool bAlliance );

	/**
	* Ŭ�� ��Ʋ������ �����Ѵ�,( Ŭ�� ��Ʋ�� ����Ǳ��� ����������� �ʿ��Ѱ�� ��Ʋ ������ ������ �ʿ��� )
	* \param dwClubP ��Ŭ�� ��ȣ( Ŭ�� ��Ʋ�� ��û�� ���� ��)
	* \param dwClubS ���� Ŭ�� ��ȣ( Ŭ�� ��Ʋ�� ��û�� �� Ŭ�� )
	* \param nGuKillNum Ŭ�� ��Ʋ ų��
	* \return
	*/
	int SaveClubBattle( DWORD dwClubP, DWORD dwClubS, int nGuKillNum, int nGuDeathNum );

	/**
	* ���͹�Ʋ ������ �ʱ�ȭ ��Ű���.
	* \param dwClub Ŭ�� ��ȣ
	* \return
	*/
	int ReSetAllianceBattle( DWORD dwClub );

	/**
	* �ش� Ŭ���� ��Ʋ���� Ŭ�� ������ �����´�.
	* \param dwClub Ŭ����ȣ
	* \param &vMember Ŭ����Ʋ ����
	* \return 
	*/
    int GetClubBattleInfo( DWORD dwClub, std::vector<GLCLUBBATTLE> &vBattleInfo );
	
	/**
	* ĳ���͸� �¶��� ���·� �����.
	* \param nChaNum ĳ���͹�ȣ
	* \return 
	*/
	int SetCharacterOnline(int nChaNum);

	/**
	* ĳ���͸� �������� ���·� �����.
	* \param nChaNum ĳ���͹�ȣ
	* \return 
	*/
	int SetCharacterOffline(int nChaNum);

	/**
	* ��� ĳ���͸� �������� ���·� �����.
	* \return 
	*/
	int SetAllCharacterOffline();

	/**
	* ĳ������ �Ӹ����� �����Ѵ�.
	* \return DB_OK, DB_ERROR
	*/
	int SetChaHairColor(DWORD dwChaNum, int nHairColor);

	/**
	* ĳ������ ��Ÿ���� �����Ѵ�.
	* \return DB_OK, DB_ERROR
	*/
	int SetChaHairStyle(DWORD dwChaNum, int nHairStyle);

	/**
	* ĳ������ �󱼽�Ÿ���� �����Ѵ�.
	* \return DB_OK, DB_ERROR
	*/
	int SetChaFaceStyle(DWORD dwChaNum, int nFaceStyle);



	/**
	* ĳ������ ������ �����Ѵ�.
	* \return DB_OK, DB_ERROR
	*/
	int SetChaGenderChange( DWORD dwChaNum, int nClass, int nSex, int nFace, int nHair, int nHairColor);

	//----------------------------------------------------------------------------
	// PET
	//----------------------------------------------------------------------------
	enum DB_PET_INVEN_TYPE
	{
		TYPEA = 1,
		TYPEB = 2,
		SKILL = 3,
		SKILL2 = 4,
	};

	// typedef std::pair < DWORD, PETSKILL> PETSKILL_PAIR;

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
	int CreatePet(
		int nChaNum,
		const TCHAR* szPetName,
		int nPetType,
		int nPetMID,
		int nPetSID,
		int nPetStyle,
		int nPetColor,
		int nPetCardMID,
		int nPetCardSID
		);

	/**
	* ���ο� ���� �����Ѵ�.
	* \param nPetNum ���� ������ȣ
	* \param szPetName ���̸�
	* \return DB_ERROR, DB_OK
	*/
	int RenamePet(int nChaNum, int nPetNum, const TCHAR* szPetName);

	/**
	* ���� �������� �����Ѵ�.
	* \param nChaNum ���� ���ο� ������
	* \param nPetNum �������� ������ �� ��ȣ
	*/
	int ExchangePet(int nChaNum, int nPetNum);

	/**
	* ���� �÷��� �����Ѵ�.
	* \param nPetNum �÷��� �����Ϸ��� ���� ��ȣ
	* \param nPetColor ���� �÷�
	*/
	int SetPetColor(int nChaNum, int nPetNum, int nPetColor);

	/**
	* ���� ��Ÿ���� �����Ѵ�.
	* \param nPetNum ��Ÿ���� �����Ϸ��� ���� ��ȣ
	* \param nPetStyle ���� ��Ÿ��
	*
	*/
	int SetPetStyle(int nChaNum, int nPetNum, int nPetStyle);

	/**
	* ���� �������� �����Ѵ�. �������� 0-100 ����
	* \param nPetNum �������� ������ ���� ��ȣ
	* \param nPetFull ������
	*/
	int SetPetFull( int nChaNum, int nPetNum, int nPetFull );

	/**
	* ���� �κ��丮 ������Ʈ
	*/
	int SetPetSkill(
		int nChaNum,
		int nPetNum,
		int nPetInvenType,
		int nPetInvenMID,
		int nPetInvenSID,
		int nPetInvenCMID,
		int nPetInvenCSID,
		int nPetInvenAvailable);

	/**
	* ���� �κ��丮 ������Ʈ
	*/
	int SetPetInven(
		int nPetChaNum,
		int nPetNum,
		CByteStream &ByteStream );

	/**
	* ���� �����Ѵ�.
	* \param nPetNum ������ ���� ��ȣ
	*/
	int	DeletePet(int nChaNum, int nPetNum);
	
	/**
	* ���� ��ų�� �����´�.
	* \param nPetNum ���� ��ȣ
	* \param vSkill �� ��ų�� ���� vector
	*/
	int GetPetSkill( int nChaNum, GLPET* pPet, DWORD dwPetNum );

	/**
	* ���� �κ��丮 ������ �����´�
	*/
	int GetPetInven( int nChaNum, GLPET* pPet, DWORD dwPetNum );

	/**
	* ���� ������ �����´�.
	*/
	int GetPet( GLPET* pPet, DWORD dwPetNum, DWORD dwClientID );

	/**
	* ���� �������� �����´�
	* \param dwPetNum �� ��ȣ
	* \return ���� ������ or DB_ERROR
	*/
	int GetPetFull( int nChaNum, DWORD dwPetNum );

	/**
	* �Ҹ�� ���� ����Ʈ�� �����´�.
	* \param nChaNum �Ҹ�� �� ����Ʈ�� ����Ϸ��� ĳ���� ��ȣ
	* \param vecPetReList �Ҹ�� �� ����Ʈ�� ���� ����
	*/
	int GetRestorePetList( int nChaNum, std::vector<PET_RELIST> &vecPetReList );

	/**
	* ���� ��Ȱ��Ų��.
	* \param nPetNum ��Ȱ�� �� ��ȣ
	* \param nChaNum ��Ȱ�� �䱸�� ĳ���� ��ȣ
	*/
	int RestorePet( DWORD dwPetNum, int nChaNum );

	/**
	* ���� ��Ų���� ������Ʈ �Ѵ�.
	* \param nChaNum		�ּ��� ĳ���� ��ȣ
	* \param dwPetNum		�� ��ȣ
	* \param nPetSkinMID	�� ��Ų MID
	* \param nPetSkinSID	�� ��Ų SID
	* \param nPetSkinScale	�� ��Ų Scale
	* \param nPetSkinTime	�� ��Ų ����ð�
	*/
	int UpdatePetSkin( int nChaNum,
					   DWORD dwPetNum,
					   int nPetSkinMID,
					   int nPetSkinSID,
					   int nPetSkinScale,
					   int nPetSkinTime );

	int SetPetDualSkill(int nChaNum, int nPetNum);
	/**************************************************************************
	* User Database
	*/

	/**
	* Taiwan / HongKong 
	* �α����Ϸ��� ����ڸ� üũ�Ѵ�.	
	* \param szUsrID ����� ID
	* \param szPasswd ����� �н�����
	* \param szUsrIP ������� IP Address
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return DB ������ �����
	*/
	int	UserCheck(
		const TCHAR* szUsrID,
		const TCHAR* szPasswd,
		const TCHAR* szUsrIP,
		const TCHAR* szRandomPasswd,
		int nRandomNum,
		int nSvrGrp,
		int nSvrNum);

	/**
	* Thailand
	* �α����Ϸ��� ����ڸ� üũ�Ѵ�.	
	* \param szUsrID ����� ID
	* \param szPasswd ����� �н�����
	* \param szUsrIP ������� IP Address
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return DB ������ �����
	*/
	int	ThaiUserCheck(
			const TCHAR* szUsrID,
		    const TCHAR* szPasswd,
			const TCHAR* szUsrIP,
			int nSvrGrp,
			int nSvrNum);

	/**
	* Daum Game (Korea)
	* \param szDaumGID ����� GID
	* \param szDaumUID ����� UID
	* \param szDaumSSNHEAD �ֹι�ȣ ���ڸ�
	* \param szDaumSEX ����
	* \param szUsrIP IP Address
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return 
	*/
	/*
	int	DaumUserCheck(
			const TCHAR* szDaumGID, 
			const TCHAR* szDaumUID,
			const TCHAR* szDaumSSNHEAD,
			const TCHAR* szDaumSEX,
			const TCHAR* szUsrIP, 
			int nSvrGrp, 
			int nSvrNum);
	*/

	/**
	* Daum Game (Korea)	
	* \param 
	* \param 
	* \param szUsrIP �α����Ϸ��� IP Address
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return
	*/
	int DaumUserCheck(
			TCHAR* szDaumGID,
		    const TCHAR* szUUID,
			const TCHAR* szUserIP,
			int nSvrGrp,
			int nSvrNum );

	/**
	* GSP
	* \n �α����Ϸ��� ����ڸ� üũ�Ѵ�.
	* \param 
	* \param 
	* \param szUsrIP �α����Ϸ��� IP Address
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return 
	*/
	int GspUserCheck(
			TCHAR* szGspUserID,
		    const TCHAR* szUUID,
			const TCHAR* szUserIP,
			int nSvrGrp,
			int nSvrNum );
	/**
	* Terra (Malaysia)
	* \n �α����Ϸ��� ����ڸ� üũ�Ѵ�.
	* \param szTerraLoginName Terra TLoginName varchar(15)
	* \param szTerraTID Web ���� �Ѿ�� ���ڿ� (ENCODING ���ڿ�)
	* \param szUsrIP �α����Ϸ��� IP Address
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return 
	*/
	int TerraUserCheck(
			TCHAR* szTerraLoginName,
		    const TCHAR* szTerraDecodedTID,
			const TCHAR* szUsrIP,
			int nSvrGrp,
			int nSvrNum );
	/**
	* Excite (Japan)
	* \param szUserID �����ID
	* \param szUsrIP IP
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return 
	*/
	int ExciteUserCheck(
			const TCHAR* szUserID,
			const TCHAR* szUserIP,
			int nSvrGrp, 
			int nSvrNum );

	/**
	* Gonzo (Japan)
	* \param szUserID �����ID
	* \param szUsrIP IP
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return 
	*/
	int JapanUserCheck(
			const TCHAR* szUserID,
			const TCHAR* szUserIP,
			int nSvrGrp, 
			int nSvrNum,
			int nUserNum);


	/**
	* �۷ι� ����
	* �α����Ϸ��� ����ڸ� üũ�Ѵ�.	
	* \param szUsrID ����� ID
	* \param szPasswd ����� �н�����
	* \param szUsrIP ������� IP Address
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return DB ������ �����
	*/
	int	GsUserCheck(
		const TCHAR* szUsrID,
		const TCHAR* szPasswd,
		const TCHAR* szUsrIP,
		int nSvrGrp,
		int nSvrNum);


	/**
	* �ش����ڸ� Logout �Ѵ�
	* \param szUsrID ����� ID
	* \param nUsrNum ����ڹ�ȣ
	* \param nGameTime �����÷��̽ð� 
	* \param nChaNum ĳ���͹�ȣ
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return 
	*/
	int	UserLogout(
			const TCHAR* szUsrID,
			int nUsrNum=0,
			int nGameTime=0,
			int nChaNum=0,
			int nSvrGrp=0,
			int nSvrNum=0,
			int nTotalGameTime=0,
			int nOfflineTime=0 );

	/**
	* Terra (Malaysia)
	* �ش����ڸ� Logout �Ѵ�
	* \param szTID Terra User ID
	* \param nUsrNum ����ڹ�ȣ
	* \param nGameTime �����÷��̽ð�
	* \param nChaNum ĳ���͹�ȣ
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return 
	*/
	int	TerraUserLogout(
			const TCHAR* szTID,
			int nUsrNum=0,
			int nGameTime=0,
			int nChaNum=0,
			int nSvrGrp=0,
			int nSvrNum=0 );

	/**
	* Maya (Taiwan/Hongkong)
	* \n ����ڸ� �α׾ƿ� ��Ų��
	* \param szUsrID ����� ID
	* \return 
	*/
	int	UserLogoutSimple( const TCHAR* szUsrID );

	/**
	* Maya (Taiwan/Hongkong)
	* \n ����ڸ� �α׾ƿ� ��Ų��.
	* \param nUserNum ����ڹ�ȣ
	* \return 
	*/
	int UserLogoutSimple2( int nUserNum );
	
	/**
	* Feya (Taiwan/Hongkong)
	* �ش� ���Ӽ����� ��ü ������ �α׾ƿ� ��Ų��.
	* \param nSvrGrp �����׷�
	* \param nSvrNum ������ȣ
	* \return 
	*/
	int	UserLogoutSvr(int nSvrGrp, int nSvrNum);
	
	/**
	* Feya (Taiwan/Hongkong)
	* \n DB ����� ������ ������ �÷����� ĳ���� �̸��� �����Ѵ�.
	* \param nUsrNum ����ڹ�ȣ
	* \param szChaName ĳ�����̸�
	* \return 
	*/
	int	UserUpdateCha(int nUsrNum, const TCHAR* szChaName);		
	
	/**	
	* �ش� ������� ����� ��ȣ�� �����´�.
	* \param szUsrId ����� ID
	* \return ����ڹ�ȣ
	*/
	int	GetUserNum(const TCHAR* szUsrId);

	int GetUserChaRemain(int nUserNum);
	int	GetUserChaTestRemain(int nUserNum);
	int	UserChaNumDecrease(int nUserNum);
	/*
	* TEST ���� ĳ���� �������� ����
	*/
	int	UserTestChaNumDecrease(int nUserNum);

	/**
	* ĳ���� ����, ĳ���ͻ��� ���� ����
	*/
	int UserChaNumIncrease(int nUserNum);

	/**
	* TEST ���� ĳ���� ����, ���� ����
	*/
	int UserTestChaNumIncrease(int nUserNum);
	
	/**
	* �ش� ������� Ÿ���� �����´�.
	* \param nUserNum ����ڹ�ȣ
	* \return �����Ÿ��
	*/
	int	GetUserType(int nUserNum);

	/**	
	* �ش� ������� �����̾� �Ⱓ�� �����´�.
	* \param nUserNum ����ڹ�ȣ
	* \return �����̾��Ⱓ(������)
	*/
    __time64_t GetPremiumTime(int nUserNum);	
    
	/**	
	* �ش� ������� �����̾� �Ⱓ�� �����Ѵ�.
	* \param nUserNum ����ڹ�ȣ
	* \param tPremiumTime �����̾� ����ð�
	* \return 
	*/
    int SetPremiumTime(int nUserNum, __time64_t tPremiumTime);	

	/**
	* �ش� ������� ä�� ���Ͻð��� �����´�
	* \param nUserNum ����ڹ�ȣ
	* \return ä�ú��� ����ð�
	*/
	__time64_t GetChatBlockTime(int nUserNum);

	/**
	* �ش� ������� ä�� ���Ͻð��� �����Ѵ�
	* \param nUserNum ����ڹ�ȣ
	* \param tBlockTime ä�ú��� ����ð�
	* \return 
	*/
	int SetChatBlockTime(int nUserNum, __time64_t tBlockTime);

	int AddBlockIPList( TCHAR* szIP, TCHAR* szReason );
	int	AddLogServerState(int nSGNum=0, int nSvrNum=0, int nUserNum=0, int nUserMax=1);

	USER_INFO_BASIC GetUserInfo( const TCHAR* szUsrId, const TCHAR* szPasswd );
	int DaumGetUserInfo( const TCHAR* szGID, DAUM_NET_LOGIN_FEEDBACK_DATA2* pMsg );
	
	/**
	* ����� ������ �����´�
	* Taiwan / HongKong
	* \param szUserID 
	* \param pMsg 
	* \return 
	*/
	int GetUserInfo( const TCHAR* szUserID, NET_LOGIN_FEEDBACK_DATA2* pMsg );

	/**
	* ����� ������ �����´�
	* Thailand
	* \param szUserID 
	* \param pMsg 
	* \return 
	*/
	int ThaiGetUserInfo( const TCHAR* szUserID, NET_LOGIN_FEEDBACK_DATA2* pMsg );

	/**
	* Terra ����� ������ �����´�.
	* \param szTLoginName TLoginName varchar(15)
	* \param pMsg
	* \return 
	*/
	int TerraGetUserInfo( const TCHAR* szTLoginName, TERRA_NET_LOGIN_FEEDBACK_DATA2* pMsg );

	/**
	* GSP ����� ������ �����´�.
	* \param szUserID UserID varchar(50)
	* \param pMsg
	* \return 
	*/
	int GspGetUserInfo( const TCHAR* szUserID, GSP_NET_LOGIN_FEEDBACK_DATA2* pMsg );

	/**
	* Excite ����� ������ �����´�.
	* \param szUserID UserID varchar(20)
	* \param pMsg
	* \return 
	*/
	int ExciteGetUserInfo( const TCHAR* szUserID, EXCITE_NET_LOGIN_FEEDBACK_DATA2* pMsg );

	/**
	* Japan(Gonzo) ����� ������ �����´�.
	* \param szUserID UserID varchar(16)
	* \param pMsg
	* \return 
	*/
	int JapanGetUserInfo( const TCHAR* szUserID, JAPAN_NET_LOGIN_FEEDBACK_DATA2* pMsg );


	/**
	* �۷ι����� ����� ������ �����´�.
	* \param szUserID UserID varchar(16)
	* \param pMsg
	* \return 
	*/
	int GsGetUserInfo( const TCHAR* szUserID, GS_NET_LOGIN_FEEDBACK_DATA2* pMsg );
	
	/**
	* �ش����ڸ� ���Ͻ�Ų��.
	* \param nUserNum ����ڹ�ȣ
	* \param nDay �����ϼ� 1 - 15 ����
	* \return 
	*/
    int UserBlock( int nUserNum, int nDay );

	/**
	* �ش����ڸ� ���Ͻ�Ų��.
	* \param nUserNum ����ڹ�ȣ
	* \param nHour �ð�
	* \return 
	*/
    int UserBlockDate( int nUserNum, __time64_t tBlockTime );
	
	/**
	* �ش� ���Ӽ����� ��ü ������ �α׾ƿ� ��Ų��.
	* \param nSvrGrp 
	* \return 
	*/
	int AllServerUserLogout(int nSvrGrp);

	/**
	* ��ü ������ �α׾ƿ� ��Ų��.
	* \return 
	*/
	int	AllUserLogout();

	/**
	* TERRA
	* ������ �н����带 �ְų� üũ�Ѵ�.
	* \param szTID Terra UserID
	* \param szPasswd ����� �н�����
	* \return 
	*/
	int TerraUserPassCheck(
				const TCHAR* szTID,
				const TCHAR* szPasswd,
				int	nCheckFlag );

	/**
	* Daum
	* ������ �н����带 �ְų� üũ�Ѵ�.
	* \param szDaumGID Daum UserGID
	* \param szDaumPass ����� �н�����
	* \return 
	*/
	int DaumUserPassCheck(
				const TCHAR* szDaumGID,
				const TCHAR* szDaumPasswd,
				int	nCheckFlag );


	/**
	* Excite
	* ������ �н����带 �ְų� üũ�Ѵ�.
	* \param szUID Excite UserID
	* \param szUserID Excite 2�� UserID
	* \param szPasswd ����� �н�����
	* \return 
	*/
	int ExciteUserPassCheck(
					const TCHAR* szUID,
					const TCHAR* szUserID,
					const TCHAR* szPasswd,
					int	nCheckFlag );


	/**
	* Thailand
	* ������� PC�� Ŭ������ üũ�Ѵ�.	
	* \param szUsrIP ������� IP Address
	* \return DB ������ �����
	*/
	int	ThaiUserClassCheck(			
			const TCHAR* szUsrIP );

	/**
	* Malaysia
	* ������� PC�� Ŭ������ üũ�Ѵ�.	
	* \param szUsrIP ������� IP Address
	* \return DB ������ �����
	*/
	int	MyUserClassCheck(
			const TCHAR* szUsrIP );
	
	/**
	* �ش� ������� ���� �����÷��� �ð�(��)�� �����´�.
	* Thailand
	* \param nUserNum 
	* \return 
	*/
	WORD ThaiGetTodayPlayTime (int nUserNum);

	/**************************************************************************
	* Log Database
    */

	// int LogServerState(int nSGNum=0, int nSvrNum=0, int nUserNum=0, int nUserMax=0);
	int	LogChaDeleted(
			int nSGNum=0,
			int nSvrNum=0,
			int nUserNum=0,
			int nChaNum=0 );

	int	LogItemExchange(
			const SLOGITEMEXCHANGE &sEXCHANGE );

	int	LogMoneyExchange(
			const SLOGMONEYEXCHANGE &_sLOG );

	int LogItemConversion(
			const SLOGITEMCONVERSION &_sLOG );

    int	LogAction(
			const SLOGACTION &_sLOG );

	/**
	* �������� �����ɼ� �α׸� �����.
	* \param _sLOG �����ɼ� ����ü
	*/
	int LogRandomItem( const SLOGRANDOMOPTION &_sLOG );

	/**
	* ���� �׼Ƿα׸� �����.
	* \param _sLOG ��׼Ƿα� ����ü
	*/
	int LogPetAction( const SLOGPETACTION &_sLOG );

	// �������� PC �� ���θ�� ���ǰ���'��������!!' �̺�Ʈ
	int	LogLottery(
			CString strLottery, 
		    CString strIP, 
			CString strUserUID, 
			int nUserNum );

	// �������� PC ���θ�� ������ '����' �̺�Ʈ
	int LogPungPungCheck(
			CString strIP, 
		    int nUserNum );
    
	// �������� PC ���θ�� ������ '����' �̺�Ʈ
	int LogPungPungWrite(
			CString strIP, 
		    CString strUserUID, 
			int nUserNum, 
			int nPrize );

    LONGLONG GetItemMaxNum(
				int nSGNum, 
		        int nSvrNum, 
				int nFldNum, 
				std::vector<VIEWLOGITEMEXCHANGEMAX> &v );

	/**
	* ���ӳ����� �����̳� �Ű��� �Ű��� ������ DB�� ����Ѵ�.
	* \param nSGNum �����׷�
	* \param nChaNum �Ű��� ĳ���͹�ȣ
	* \param strMsg �Ű����� (�ִ�ũ��� CHAT_MSG_SIZE)
	* \return 
	*/
    int LogAppeal(int nSGNum, int nChaNum, CString strMsg);

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
	int	LogHackProgram(
		int nSGNum, 
		int nSvrNum,
		int nUserNum,
		int nChaNum,
		int nHackProgramNum,
		const TCHAR* szComment );

	/**
	* SMS ���ڸ޽����� �߼��Ѵ�.
	*
	* \param nSendChaNum SMS �� �߼��ϴ� ĳ���� ��ȣ	
	* \param nReceiveChaNum SMS �� ���� ĳ���� ��ȣ
	* \param szReceivePhone SMS �� ���� ĳ������ ��ȭ��ȣ
	* \param szSmsMsg SMS �޽��� ����
	*/
	int LogSMS(
		int nSendChaNum,		
		int nReceiveChaNum,
		const TCHAR* szReceivePhone,
		const TCHAR* szSmsMsg );

	/**
	* ĳ������ �޴��� ��ȣ�� �����Ѵ�.
	* \param nChaNum ĳ���� ��ȣ
	* \param szPhoneNumber ��ȭ��ȣ NULL ���� ���� 14�� SMS_RECEIVER
	*/
	int SetChaPhoneNumber(
			int nChaNum,
			const TCHAR* szPhoneNumber );

	/**
	* ĳ������ ģ�������� �����´�.
	* \param nChaNum ģ�������� ������ ĳ���� ��ȣ
	* \param vFriend ģ�������� ���� vector
	*/
	int	GetChaFriendSMS(
			int nChaNum,
			std::vector<CHA_FRIEND_SMS> &vFriend );

	/**************************************************************************
	* Shop Database
    */

	/**
	* Item Shop ���� ������ �������� �����´�.
	*/
	int GetPurchaseItem(
			CString strUID,
			std::vector<SHOPPURCHASE> &v );
	int GetItemShop(std::vector<ITEMSHOP> &v );
    
	/**
	* ������ ������ �� �ִ� ��ǰ���� Ȯ���Ѵ�.
	* \param strPurKey ����Ű
	* \param nFlag  (0:����, 1:���ſϷ�, 2:������ҽ�û, 3:�������ó��)
	* \return 1:�������� ���� \n �̿��ǰ�:�������� ����
	*/
    int SetPurchaseItem(
			CString strPurKey,
		    int nFlag=1 );
	int SetItemShop(
			CString strItemNum, DWORD dwUserID );

	/**************************************************************************
	* Vehicle Database
    */

	// VEHICLE
	//----------------------------------------------------------------------------
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
	int CreateVehicle(
		int nCharNum,
		const TCHAR* szVehicleName,
		int nVehicleType,
		int nVehicleCardMID,
		int nVehicleCardSID
		);


	/**
	* Ż���� ���͸��� �����Ѵ�. ���͸��� 0-1000 ����
	* \param nVehicleBttery ���͸��� ������ Ż���� ��ȣ
	* \param nVehicleBttery ���͸�
	*/
	int SetVehicleBattery(
		int nVehicleNum,
		int nCharNum,
		int nVehicleBttery
		);

	int SetVehicleColor( //add bike color
		int nVehicleNum,
		int nCharNum,
		int nVehicleColor,
		int nVehicleColor1,
		int nVehicleColor2,
		int nVehicleColor3,
		int nVehicleColor4,
		int nVehicleColor5
		);


	int GetVehicleBattery(
		int nVehicleNum,
		int nCharNum
		);

	
	/**
	* Ż���� �׼Ƿα׸� �����.
	* \param _sLOG Ż�;׼Ƿα� ����ü
	*/
	int LogVehicleAction( const SLOGVEHICLEACTION &_sLOG );




	/**
	* Ż���� �����Ѵ�.
	* \param nVehicle ������ Ż���� ��ȣ
	*/
	int DeleteVehicle(
		int nVehicle, 
		int nCharNum);

	/**
	* Ż���� ������ �����´�.
	*/
	int GetVehicle(
		GLVEHICLE* pVehicle,
		int nVehicleNum,
		int nCharNum);


	/**
	* Ż���� �κ��丮 ������ �����´�
	*/
	int GetVehicleInven( 
		GLVEHICLE* pVehicle,
		int nVehicleNum,
		int nCharNum );

	/**
	*Ż�� �κ��丮 ������Ʈ
	*/
	int SetVehicleInven(
		int nVehicle,
		int nCharNum,
		CByteStream &ByteStream );	


	// �⼮�� �̺�Ʈ 
	int LogUserAttend( int nUserNum, std::vector<USER_ATTEND_INFO> &vecUserAttend );

	int InsertUserAttend( int nUserNum, int nCount, int nAttendReward );

	int InsertAttendItem( CString strPurKey, CString strUserID, int nItemMID, int nItemSID );

	//	UserMoney Add
	int UpdateUserMoneyAdd( DWORD dwUserID, LONGLONG lnUserMoney );
	
	//	UserLastInfo Add
	int UpdateUserLastInfoAdd( DWORD dwUserID, LONGLONG lnUserMoney );



	/**************************************************************************
	* etc
    **************************************************************************/
	int	Command( TCHAR* szCmdStr );
};

#endif // _CODBCMANAGER_H_