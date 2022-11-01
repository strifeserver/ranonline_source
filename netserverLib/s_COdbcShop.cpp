#include "stdafx.h"
#include "s_COdbcManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Shop ���� ������ �������� �����´�.
// ������ �������� ������ ���� �� �� �ִ� �������� �ƴϴ�.
// �������� ���������� ������ ������ �� �ִ��� �ٽ� Ȯ���ؾ� �Ѵ�. (SetPurchaseItem)
// ���� ���� ���ſ� ��� �� ���ӳ� ĳ������ ����ȭ ����.
int COdbcManager::GetPurchaseItem(CString strUID, std::vector<SHOPPURCHASE> &v)
{
    ODBC_STMT* pConn = m_pShopDB->GetConnection();
	if (!pConn)	return DB_ERROR;    

	//std::strstream strTemp;
 //   strTemp << "SELECT Purkey, ItemMain, ItemSub FROM viewShopPurchase WHERE ";
 //   strTemp << "useruid='" << strUID.GetString() << "' AND ";
 //   strTemp << "PurFlag=0";
 //   strTemp << std::ends;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "SELECT Purkey, ItemMain, ItemSub FROM viewShopPurchase WHERE "
							"useruid='%s' AND PurFlag=0", strUID.GetString() );

	SQLCHAR    szPurKey[PURKEY_LENGTH+1] = {0}; SQLINTEGER cbPurKey = SQL_NTS;
    SQLINTEGER nItemMain = 0, cbItemMain =SQL_NTS;
    SQLINTEGER nItemSub  = 0, cbItemSub  =SQL_NTS;
    SQLRETURN  sReturn=0;

    sReturn = ::SQLExecDirect(pConn->hStmt,
							(SQLCHAR*)szTemp, 
							SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
        Print(szTemp);		
		Print(GetErrorString(pConn->hStmt));
		m_pShopDB->FreeConnection(pConn);

		//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

        return DB_ERROR;
	}

    while(true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
        {
            Print(szTemp);		
			Print(GetErrorString(pConn->hStmt));
            m_pShopDB->FreeConnection(pConn);

			//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

            return DB_ERROR;
		}

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{   
            ::SQLGetData(pConn->hStmt, 1, SQL_C_CHAR, szPurKey,   PURKEY_LENGTH+1, &cbPurKey);
            ::SQLGetData(pConn->hStmt, 2, SQL_C_LONG, &nItemMain, 0,             &cbItemMain);
            ::SQLGetData(pConn->hStmt, 3, SQL_C_LONG, &nItemSub,  0,             &cbItemSub);

            SHOPPURCHASE sTemp;

            if (cbPurKey != 0 && cbPurKey != -1)
                sTemp.strPurKey = szPurKey;
            sTemp.wItemMain = static_cast<WORD> (nItemMain);
            sTemp.wItemSub  = static_cast<WORD> (nItemSub);

            v.push_back(sTemp);
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}
	//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

	m_pShopDB->FreeConnection(pConn);
    return DB_OK;
}



int COdbcManager::GetItemShop(std::vector<ITEMSHOP> &v)
{
    ODBC_STMT* pConn = m_pShopDB->GetConnection();
	if (!pConn)	return DB_ERROR;    

	TCHAR szTemp[256] = {0};
	_snprintf( szTemp, 256, "SELECT ProductNum, ItemMain, ItemSub , Itemstock , ItemPrice , ItemCtg , ItemSec , isHide FROM viewShopItemMap where Itemstock > 0 AND isHide = 0");

	SQLCHAR    szPurKey[PURKEY_LENGTH+1] = {0}; SQLINTEGER cbPurKey = SQL_NTS;
	SQLINTEGER nItemMain = 0, cbItemMain =SQL_NTS;
    SQLINTEGER nItemSub  = 0, cbItemSub  =SQL_NTS;
    SQLINTEGER nItemStock  = 0, cbItemStock  =SQL_NTS;
    SQLINTEGER nItemPrice  = 0, cbItemPrice  =SQL_NTS;
    SQLINTEGER nItemCtg  = 0, cbItemCtg  =SQL_NTS;
	SQLINTEGER nItemCurrency = 0, cbItemCurrency =SQL_NTS;
    SQLRETURN  sReturn=0;

    sReturn = ::SQLExecDirect(pConn->hStmt,
							(SQLCHAR*)szTemp, 
							SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
        Print(szTemp);		
		Print(GetErrorString(pConn->hStmt));
		m_pShopDB->FreeConnection(pConn);

		//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

        return DB_ERROR;
	}

    while(true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
        {
            Print(szTemp);		
			Print(GetErrorString(pConn->hStmt));
            m_pShopDB->FreeConnection(pConn);

			//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

            return DB_ERROR;
		}

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{   
			::SQLGetData(pConn->hStmt, 1, SQL_C_CHAR, szPurKey,   PURKEY_LENGTH+1, &cbPurKey);
            ::SQLGetData(pConn->hStmt, 2, SQL_C_LONG, &nItemMain, 0,             &cbItemMain);
            ::SQLGetData(pConn->hStmt, 3, SQL_C_LONG, &nItemSub,  0,             &cbItemSub);
            ::SQLGetData(pConn->hStmt, 4, SQL_C_LONG, &nItemStock,  0,             &cbItemStock);
            ::SQLGetData(pConn->hStmt, 5, SQL_C_LONG, &nItemPrice,  0,             &cbItemPrice);
            ::SQLGetData(pConn->hStmt, 6, SQL_C_LONG, &nItemCtg,  0,             &cbItemCtg);
            ::SQLGetData(pConn->hStmt, 7, SQL_C_LONG, &nItemCurrency,  0,             &cbItemCurrency);


            ITEMSHOP sTemp;
			if (cbPurKey != 0 && cbPurKey != -1)
                sTemp.strItemNum = szPurKey;
            sTemp.sID.wMainID = static_cast<WORD> (nItemMain);
            sTemp.sID.wSubID  = static_cast<WORD> (nItemSub);
            sTemp.wItemStock  = static_cast<WORD> (nItemStock);
			sTemp.wItemPrice  = static_cast<WORD> (nItemPrice);
			sTemp.wItemCtg  = static_cast<WORD> (nItemCtg);
			sTemp.wItemCurrency = static_cast<WORD> (nItemCurrency);

            v.push_back(sTemp);
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}
	//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

	m_pShopDB->FreeConnection(pConn);
    return DB_OK;
}



int COdbcManager::SetItemShop(CString strItemNum, DWORD dwUserID)
{
	
	TCHAR szTemp[256] = {0};
	_snprintf( szTemp, 256, "{call sp_InsertItem2Bank('%s',%d,?)}", strItemNum.GetString() , dwUserID );
	int nReturn = m_pShopDB->ExecuteSpInt(szTemp);
	//CString strText;
	//strText.Format(" returned %d",nReturn);
	//MessageBox(NULL,strText,NULL,NULL);
	return nReturn;
	
}

///////////////////////////////////////////////////////////////////////////////
// ������ ������ �� �ִ� ��ǰ���� Ȯ���Ѵ�.
// �Է°�
// nFlag  : (0 : ����, 1 : ���ſϷ�, 2 : ������ҽ�û, 3 : �������ó��)
// ��°�
// 1 : �������� ����
// �̿��ǰ� : �������� ����
int COdbcManager::SetPurchaseItem(CString strPurKey, int nFlag)
{
	//std::strstream strTemp;
	//strTemp << "{call sp_purchase_change_state(";	
	//strTemp << "'" << strPurKey.GetString() << "',";		
	//strTemp << nFlag << ",?)}";
	//strTemp << std::ends;

	TCHAR szTemp[256] = {0};
	_snprintf( szTemp, 256, "{call sp_purchase_change_state('%s',%d,?)}", strPurKey.GetString(), nFlag );

	int nReturn = m_pShopDB->ExecuteSpInt(szTemp);
	//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

	return nReturn;
}