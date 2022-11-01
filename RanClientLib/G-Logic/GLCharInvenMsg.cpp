#include "stdafx.h"
#include "./GLChar.h"
#include "./GLGaeaServer.h"
#include "./GLItemLMT.h"
#include "./GLItemMan.h"
#include "GLogicData.h"
#include "GLCharacter.h"
#include "../enginelib/Common/StringUtils.h"
#include "RanFilter.h"
#include "./GLItemMixMan.h"
#include "./GLogicEx.h"
#include "../RanClientUILib/Interface/InnerInterface.h"
#include "../RanClientUILib/Interface/UITextControl.h"

#include "./GLQuest.h"
#include "./GLQuestMan.h"
#include "./GLSchoolFreePK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	Note : �κ��������� �Ҹ��Ŵ.
//		2. �Ҹ� Ŭ���̾�Ʈ�� �˸�.
//		3. b2AGENT ������ Ŭ���̾�Ʈ�� �˸��� �ʵ���� ���Ἲ�� ��Ȯ�� �� ��츦 ����Ͽ�
//			������Ʈ�� ���ؼ� �˸��� �ֵ��� ��.
bool GLChar::DoDrugInvenItem ( WORD wPosX, WORD wPosY, bool b2AGENT )
{
	//	�κ��� ������ Ȯ��.
	SINVENITEM *pINVENITEM = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pINVENITEM )		return false;

	//	������ ���� ������.
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pItem )			return false;

	if ( pItem->sDrugOp.bInstance )
	{
		if ( pINVENITEM->sItemCustom.wTurnNum > 0 )		pINVENITEM->sItemCustom.wTurnNum--;
	
		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( pINVENITEM->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, 1 );

		if ( pINVENITEM->sItemCustom.wTurnNum==0 )
		{
			//	������ ����.
			m_cInventory.DeleteItem ( wPosX, wPosY );

			//	[�ڽſ���] �ι꿡 ������ ����.
			GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
			NetMsg_Inven_Delete.wPosX = wPosX;
			NetMsg_Inven_Delete.wPosY = wPosY;
			if ( b2AGENT )	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID,&NetMsg_Inven_Delete);
			else			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);
		}
		else
		{
			//	[�ڽſ���] �ι� ������ �Ҹ��.
			GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg_Inven_Update;
			NetMsg_Inven_Update.wPosX = wPosX;
			NetMsg_Inven_Update.wPosY = wPosY;
			NetMsg_Inven_Update.wTurnNum = pINVENITEM->sItemCustom.wTurnNum;

			if ( b2AGENT )	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID,&NetMsg_Inven_Update);
			else			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Update);
		}
	}


	// ��Ÿ�� ����
	if ( pItem->sBasicOp.IsCoolTime() ) 
	{
		SetCoolTime( pINVENITEM->sItemCustom.sNativeID , pItem->sBasicOp.emCoolType );
	}

	return true;
}

bool GLChar::DoDrugSlotItem ( EMSLOT emSLOT,  WORD wNum )
{
	if ( emSLOT >= SLOT_TSIZE )		return false;

	SITEMCUSTOM& sSLOTITEM = m_PutOnItems[emSLOT];
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sSLOTITEM.sNativeID );
	if ( !pItem )					return false;

	// �Ҹ� �������� ���
	if ( pItem->ISINSTANCE() )
	{
		if ( sSLOTITEM.wTurnNum >= wNum )		sSLOTITEM.wTurnNum -= wNum;

		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( sSLOTITEM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, sSLOTITEM.wTurnNum );

		if ( sSLOTITEM.wTurnNum==0 )
		{
			//	HOLD ������ ����.
			RELEASE_SLOT_ITEM ( emSLOT );

			//	[�ڽſ���] ���Կ� �־��� ������ ����.
			GLMSG::SNETPC_PUTON_RELEASE NetMsg_Release(emSLOT);
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Release);
		}
		else
		{
			//	[�ڽſ���] �������� �������� Ƚ�� ����.
			GLMSG::SNETPC_PUTON_DRUG_UPDATE NetMsg_Update;
			NetMsg_Update.emSlot = emSLOT;
			NetMsg_Update.wTurnNum = sSLOTITEM.wTurnNum;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Update);
		}
	}
	else
	{
		//	���� �������� ���
		GLITEMLMT::GetInstance().ReqItemRoute ( sSLOTITEM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, 0 );
	}

	// ��Ÿ�� ����
	if ( pItem->sBasicOp.IsCoolTime() ) 
	{
		SetCoolTime( sSLOTITEM.sNativeID , pItem->sBasicOp.emCoolType );
	}

	return true;
}

bool GLChar::IsInsertToInven ( PITEMDROP pItemDrop, bool bVietnamInven )
{
	GASSERT(pItemDrop&&"GLChar::IsItemToInven()");
	if ( !pItemDrop )	return false;

	SITEM *pItem = GLItemMan::GetInstance().GetItem(pItemDrop->sItemCustom.sNativeID);
	if ( !pItem )		return false;

	if ( pItem->ISPILE() )
	{
		WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
		WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

		//	��ħ �������� ���.
		WORD wPILENUM = pItem->sDrugOp.wPileNum;
		SNATIVEID sNID = pItem->sBasicOp.sNativeID;

		//	�ֱ� ��û�� �����ۼ�. ( �ܿ���. )
		WORD wREQINSRTNUM = ( pItemDrop->sItemCustom.wTurnNum );

		BOOL bITEM_SPACE = TRUE;
		if( bVietnamInven )
			bITEM_SPACE = m_cVietnamInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, wINVENX, wINVENY );
		else
			bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, wINVENX, wINVENY );

		if ( !bITEM_SPACE )		return false;
	}
	else
	{
		WORD wPosX, wPosY;
		BOOL bOk = TRUE;
		if( bVietnamInven )
			bOk = m_cVietnamInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		else
			bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );

		if ( !bOk )				return false;
	}

	return true;
}

/* ��Ȱ��ȭ �κ��丮���� ���� ������ �ִ�.*/
bool GLChar::IsInsertToInvenEx ( PITEMDROP pItemDrop )
{
	GASSERT(pItemDrop&&"GLChar::IsItemToInven()");
	if ( !pItemDrop )	return false;

	SITEM *pItem = GLItemMan::GetInstance().GetItem(pItemDrop->sItemCustom.sNativeID);
	if ( !pItem )		return false;

	if ( pItem->ISPILE() )
	{
		WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
		WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

		//	��ħ �������� ���.
		WORD wPILENUM = pItem->sDrugOp.wPileNum;
		SNATIVEID sNID = pItem->sBasicOp.sNativeID;

		//	�ֱ� ��û�� �����ۼ�. ( �ܿ���. )
		WORD wREQINSRTNUM = ( pItemDrop->sItemCustom.wTurnNum );

		BOOL bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, wINVENX, wINVENY, true );
		if ( !bITEM_SPACE )		return false;
	}
	else
	{
		WORD wPosX, wPosY;
		BOOL bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY, true );
		if ( !bOk )				return false;
	}

	return true;
}

HRESULT GLChar::InsertToVNGainInven( PITEMDROP pItemDrop )
{

	GASSERT(pItemDrop&&"GLChar::InsertToInven()");
	if ( !pItemDrop )	return E_FAIL;

	SITEM *pItem = GLItemMan::GetInstance().GetItem(pItemDrop->sItemCustom.sNativeID);
	if ( !pItem )		return E_FAIL;

 	bool bITEM_SPACE = IsInsertToInven ( pItemDrop, TRUE );
	if ( !bITEM_SPACE )
	{
		//	�ι��� �������� �������� ������ �����ϴ�.
		GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
		NetMsg.emCrow = CROW_ITEM;
		NetMsg.emTakeFB = EMTAKE_FB_INVEN_ERROR;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
		return E_FAIL;
	}


	if ( pItem->ISPILE() )
	{
		WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
		WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

		//	��ħ �������� ���.
		WORD wPILENUM = pItem->sDrugOp.wPileNum;
		SNATIVEID sNID = pItem->sBasicOp.sNativeID;

		//	�ֱ� ��û�� �����ۼ�. ( �ܿ���. )
		WORD wREQINSRTNUM = ( pItemDrop->sItemCustom.wTurnNum );



		//	Note : �� �������� �ִ� �������� �ִ��� �˻��Ͽ� �κ��� �����ϴ�
		//		������ ���� ���ڸ� ���ҽ�Ŵ.
		GLInventory::CELL_MAP &ItemMap = *m_cVietnamInventory.GetItemList();
		GLInventory::CELL_MAP_ITER iter = ItemMap.begin();
		for ( ; iter!=ItemMap.end(); ++iter )
		{
			SINVENITEM &sINVENITEM = *(*iter).second;
			SITEMCUSTOM &sITEMCUSTOM = sINVENITEM.sItemCustom;
			if ( sITEMCUSTOM.sNativeID != sNID )		continue;
			if ( sITEMCUSTOM.wTurnNum>=wPILENUM )		continue;

			//	���� ������ ��ħ ������.
			WORD wSURPLUSNUM = wPILENUM - sITEMCUSTOM.wTurnNum;

			if ( wREQINSRTNUM > wSURPLUSNUM )
			{
				//	Note : ���� �������� ��ħ�� ����. ( ���� )
				sITEMCUSTOM.wTurnNum = wPILENUM;

				//	Note : Ŭ���̾�Ʈ�� ���� ������ �˸�.
				GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
				NetMsg.bVietnamInven = TRUE;
				NetMsg.wPosX = sINVENITEM.wPosX;
				NetMsg.wPosY = sINVENITEM.wPosY;
				NetMsg.wTurnNum = sITEMCUSTOM.wTurnNum;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

				//	Note : �ܿ� ���� ����.
				wREQINSRTNUM -= wSURPLUSNUM;
			}
			else
			{
				//	Note : ���� �����ۿ� ��ġ�� �� �����ε� ���� ���� �������� ����� ��.
				sITEMCUSTOM.wTurnNum += wREQINSRTNUM;

				//	Note : Ŭ���̾�Ʈ�� ���� ������ �˸�.
				GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
				NetMsg.bVietnamInven = TRUE;
				NetMsg.wPosX = sINVENITEM.wPosX;
				NetMsg.wPosY = sINVENITEM.wPosY;
				NetMsg.wTurnNum = sITEMCUSTOM.wTurnNum;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

				//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
				//
				GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
				if ( pParty )
				{
					GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
					NetMsg.dwGaeaID = m_dwGaeaID;
					NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
					pParty->SENDMSG2NEAR ( this, &NetMsg );
				}

				//	Note : ������ Land���� ����.
				m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );

				return S_OK;
			}
		}

		//	Note : ������ �κ��� ������ �������� ���� �ľ��� �κ��� �� ������ �ִ��� �˻�.

		//	�ִ��ħ������ ����.
		WORD wONENUM = wREQINSRTNUM / wPILENUM;
		WORD wITEMNUM = wONENUM;

		//	���а�ħ�������� ��ħ��.
		WORD wSPLITNUM = wREQINSRTNUM % wPILENUM;
		if ( wSPLITNUM > 0 )				wITEMNUM += 1;
		if ( wSPLITNUM==0 && wITEMNUM>=1 )	wSPLITNUM = wPILENUM;

		for ( WORD i=0; i<wITEMNUM; ++i )
		{
			WORD wInsertPosX(0), wInsertPosY(0);
			BOOL bSPACE = m_cVietnamInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );
			GASSERT(bSPACE&&"���� ������ �̸� üũ�� �ϰ� �������� �־����� ������ ������.");
			if ( !bSPACE )			return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

			//	Note : ���ο� �������� �־���.
			//
			SITEMCUSTOM sITEMCUSTOM = pItemDrop->sItemCustom;
			if ( wITEMNUM==(i+1) )	sITEMCUSTOM.wTurnNum = wSPLITNUM;	//	������ �������� �ܿ���.
			else					sITEMCUSTOM.wTurnNum = wPILENUM;	//	�ƴ� ���� ������.

			//	�ι��丮�� �ֽ��ϴ�.
			sITEMCUSTOM.bVietnamGainItem = true;
			m_cVietnamInventory.InsertItem ( sITEMCUSTOM, wInsertPosX, wInsertPosY );
			SINVENITEM *pInvenItem = m_cVietnamInventory.GetItem ( wInsertPosX, wInsertPosY );
			if ( !pInvenItem )		return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

			//	�ι꿡 ������ �־��ִ� �޽���.
			GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
			NetMsgInven.bVietnamInven = TRUE;
			NetMsgInven.Data = *pInvenItem;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);
		}

		//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
		//
		GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
		if ( pParty )
		{
			GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
			NetMsg.dwGaeaID = m_dwGaeaID;
			NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
			pParty->SENDMSG2NEAR ( this, &NetMsg );
		}

		//	Note : ������ ����� �ڵ� ����Ʈ ���� ����.
		//  ���߿� ������� �Ұ� 
		//QuestStartFromGetITEM ( pItemDrop->sItemCustom.sNativeID );

		//	Note : PILE ��ħ �������̹Ƿ� �α� �ۼ� �Ұ���.
		//

		//	Note : ������ Land���� ����.
		m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );
	}
	else
	{
		WORD wPosX, wPosY;
		BOOL bOk = m_cVietnamInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )			return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

		//	Note : ������ �ݱ� ����.
		pItemDrop->sItemCustom.bVietnamGainItem = true;
		m_cVietnamInventory.InsertItem ( pItemDrop->sItemCustom, wPosX, wPosY );
		SINVENITEM *pInvenItem = m_cVietnamInventory.GetItem ( wPosX, wPosY );
		if ( !pInvenItem )	return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

		//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
		//
		GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
		if ( pParty )
		{
			GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
			NetMsg.dwGaeaID = m_dwGaeaID;
			NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
			pParty->SENDMSG2NEAR ( this, &NetMsg );
		}



		//	[�ڽſ���] �޽��� �߻�.
		GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
		NetMsg_Inven.bVietnamInven = TRUE;
		NetMsg_Inven.Data = *pInvenItem;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);

		//	Note : ������ ����� �ڵ� ����Ʈ ���� ����.
		//  ���߿� ������� �Ұ� 
		//QuestStartFromGetITEM ( pItemDrop->sItemCustom.sNativeID );

		//	Note : ������ Land���� ����.
		m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );
	}


	return S_OK;
}

HRESULT GLChar::InsertToInven ( PITEMDROP pItemDrop )
{
	//NeedToSaveValue _C

	GASSERT(pItemDrop&&"GLChar::InsertToInven()");
	if ( !pItemDrop )	return E_FAIL;

	SITEM *pItem = GLItemMan::GetInstance().GetItem(pItemDrop->sItemCustom.sNativeID);
	if ( !pItem )		return E_FAIL;

	bool bITEM_SPACE = IsInsertToInven ( pItemDrop );
	if ( !bITEM_SPACE )
	{
		//	�ι��� �������� �������� ������ �����ϴ�.
		GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
		NetMsg.emCrow = CROW_ITEM;
		NetMsg.emTakeFB = EMTAKE_FB_INVEN_ERROR;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
		return E_FAIL;
	}

	if ( pItem->ISPILE() )
	{
		WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
		WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

		//	��ħ �������� ���.
		WORD wPILENUM = pItem->sDrugOp.wPileNum;
		SNATIVEID sNID = pItem->sBasicOp.sNativeID;

		//	�ֱ� ��û�� �����ۼ�. ( �ܿ���. )
		WORD wREQINSRTNUM = ( pItemDrop->sItemCustom.wTurnNum );

		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( pItemDrop->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pItemDrop->sItemCustom.wTurnNum );

		//	Note : �� �������� �ִ� �������� �ִ��� �˻��Ͽ� �κ��� �����ϴ�
		//		������ ���� ���ڸ� ���ҽ�Ŵ.
		GLInventory::CELL_MAP &ItemMap = *m_cInventory.GetItemList();
		GLInventory::CELL_MAP_ITER iter = ItemMap.begin();
		for ( ; iter!=ItemMap.end(); ++iter )
		{
			SINVENITEM &sINVENITEM = *(*iter).second;
			SITEMCUSTOM &sITEMCUSTOM = sINVENITEM.sItemCustom;
			if ( sITEMCUSTOM.sNativeID != sNID )		continue;
			if ( sITEMCUSTOM.wTurnNum>=wPILENUM )		continue;
		
			//	���� ������ ��ħ ������.
			WORD wSURPLUSNUM = wPILENUM - sITEMCUSTOM.wTurnNum;

			if ( wREQINSRTNUM > wSURPLUSNUM )
			{
				//	Note : ���� �������� ��ħ�� ����. ( ���� )
				sITEMCUSTOM.wTurnNum = wPILENUM;

				//	Note : Ŭ���̾�Ʈ�� ���� ������ �˸�.
				GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
				NetMsg.wPosX = sINVENITEM.wPosX;
				NetMsg.wPosY = sINVENITEM.wPosY;
				NetMsg.wTurnNum = sITEMCUSTOM.wTurnNum;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

				//	Note : �ܿ� ���� ����.
				wREQINSRTNUM -= wSURPLUSNUM;
			}
			else
			{
				//	Note : ���� �����ۿ� ��ġ�� �� �����ε� ���� ���� �������� ����� ��.
				sITEMCUSTOM.wTurnNum += wREQINSRTNUM;

				//	Note : Ŭ���̾�Ʈ�� ���� ������ �˸�.
				GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
				NetMsg.wPosX = sINVENITEM.wPosX;
				NetMsg.wPosY = sINVENITEM.wPosY;
				NetMsg.wTurnNum = sITEMCUSTOM.wTurnNum;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

				//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
				//
				GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
				if ( pParty )
				{
					GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
					NetMsg.dwGaeaID = m_dwGaeaID;
					NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
					pParty->SENDMSG2NEAR ( this, &NetMsg );
				}

				GLMSG::SNETPC_PICKUP_ITEM NetMsgPickUp;
				NetMsgPickUp.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgPickUp);

				//	Note : ������ Land���� ����.
				m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );

				return S_OK;
			}
		}

		//	Note : ������ �κ��� ������ �������� ���� �ľ��� �κ��� �� ������ �ִ��� �˻�.

		//	�ִ��ħ������ ����.
		WORD wONENUM = wREQINSRTNUM / wPILENUM;
		WORD wITEMNUM = wONENUM;

		//	���а�ħ�������� ��ħ��.
		WORD wSPLITNUM = wREQINSRTNUM % wPILENUM;
		if ( wSPLITNUM > 0 )				wITEMNUM += 1;
		if ( wSPLITNUM==0 && wITEMNUM>=1 )	wSPLITNUM = wPILENUM;

		for ( WORD i=0; i<wITEMNUM; ++i )
		{
			WORD wInsertPosX(0), wInsertPosY(0);
			BOOL bSPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );
			GASSERT(bSPACE&&"���� ������ �̸� üũ�� �ϰ� �������� �־����� ������ ������.");
			if ( !bSPACE )			return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

			//	Note : ���ο� �������� �־���.
			//
			SITEMCUSTOM sITEMCUSTOM = pItemDrop->sItemCustom;
			if ( wITEMNUM==(i+1) )	sITEMCUSTOM.wTurnNum = wSPLITNUM;	//	������ �������� �ܿ���.
			else					sITEMCUSTOM.wTurnNum = wPILENUM;	//	�ƴ� ���� ������.

			//	�ι��丮�� �ֽ��ϴ�.
			m_cInventory.InsertItem ( sITEMCUSTOM, wInsertPosX, wInsertPosY );
			SINVENITEM *pInvenItem = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
			if ( !pInvenItem )		return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

			//	�ι꿡 ������ �־��ִ� �޽���.
			GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
			NetMsgInven.Data = *pInvenItem;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);
		}

		//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
		//
		GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
		if ( pParty )
		{
			GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
			NetMsg.dwGaeaID = m_dwGaeaID;
			NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
			pParty->SENDMSG2NEAR ( this, &NetMsg );
		}

		GLMSG::SNETPC_PICKUP_ITEM NetMsgPickUp;
		NetMsgPickUp.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgPickUp);

		//	Note : ������ ����� �ڵ� ����Ʈ ���� ����.
		//
		QuestStartFromGetITEM ( pItemDrop->sItemCustom.sNativeID );

		//	Note : PILE ��ħ �������̹Ƿ� �α� �ۼ� �Ұ���.
		//

		//	Note : ������ Land���� ����.
		m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );
	}
	else
	{
		WORD wPosX, wPosY;
		BOOL bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )			return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

		//	Note : ������ �ݱ� ����.
		m_cInventory.InsertItem ( pItemDrop->sItemCustom, wPosX, wPosY );
		SINVENITEM *pInvenItem = m_cInventory.GetItem ( wPosX, wPosY );
		if ( !pInvenItem )	return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

		//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
		//
		GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
		if ( pParty )
		{
			GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
			NetMsg.dwGaeaID = m_dwGaeaID;
			NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
			pParty->SENDMSG2NEAR ( this, &NetMsg );
		}

		GLMSG::SNETPC_PICKUP_ITEM NetMsgPickUp;
		NetMsgPickUp.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgPickUp);

		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pInvenItem->sItemCustom.wTurnNum );

		//	[�ڽſ���] �޽��� �߻�.
		GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
		NetMsg_Inven.Data = *pInvenItem;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);

		//	Note : ������ ����� �ڵ� ����Ʈ ���� ����.
		//
		QuestStartFromGetITEM ( pItemDrop->sItemCustom.sNativeID );

		//	Note : ������ Land���� ����.
		m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );
	}

	return S_OK;
}


HRESULT GLChar::InsertToInvenEx ( PITEMDROP pItemDrop )
{
	GASSERT(pItemDrop&&"GLChar::InsertToInven()");
	if ( !pItemDrop )	return E_FAIL;

	SITEM *pItem = GLItemMan::GetInstance().GetItem(pItemDrop->sItemCustom.sNativeID);
	if ( !pItem )		return E_FAIL;

	bool bITEM_SPACE = IsInsertToInvenEx ( pItemDrop );
	if ( !bITEM_SPACE )
	{
		//	�ι��� �������� �������� ������ �����ϴ�.
		GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
		NetMsg.emCrow = CROW_ITEM;
		NetMsg.emTakeFB = EMTAKE_FB_INVEN_ERROR;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
		return E_FAIL;
	}

	if ( pItem->ISPILE() )
	{
		WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
		WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

		//	��ħ �������� ���.
		WORD wPILENUM = pItem->sDrugOp.wPileNum;
		SNATIVEID sNID = pItem->sBasicOp.sNativeID;

		//	�ֱ� ��û�� �����ۼ�. ( �ܿ���. )
		WORD wREQINSRTNUM = ( pItemDrop->sItemCustom.wTurnNum );

		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( pItemDrop->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pItemDrop->sItemCustom.wTurnNum );

		//	Note : �� �������� �ִ� �������� �ִ��� �˻��Ͽ� �κ��� �����ϴ�
		//		������ ���� ���ڸ� ���ҽ�Ŵ.
		GLInventory::CELL_MAP &ItemMap = *m_cInventory.GetItemList();
		GLInventory::CELL_MAP_ITER iter = ItemMap.begin();
		for ( ; iter!=ItemMap.end(); ++iter )
		{
			SINVENITEM &sINVENITEM = *(*iter).second;
			SITEMCUSTOM &sITEMCUSTOM = sINVENITEM.sItemCustom;
			if ( sITEMCUSTOM.sNativeID != sNID )		continue;
			if ( sITEMCUSTOM.wTurnNum>=wPILENUM )		continue;
		
			//	���� ������ ��ħ ������.
			WORD wSURPLUSNUM = wPILENUM - sITEMCUSTOM.wTurnNum;

			if ( wREQINSRTNUM > wSURPLUSNUM )
			{
				//	Note : ���� �������� ��ħ�� ����. ( ���� )
				sITEMCUSTOM.wTurnNum = wPILENUM;

				//	Note : Ŭ���̾�Ʈ�� ���� ������ �˸�.
				GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
				NetMsg.wPosX = sINVENITEM.wPosX;
				NetMsg.wPosY = sINVENITEM.wPosY;
				NetMsg.wTurnNum = sITEMCUSTOM.wTurnNum;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

				//	Note : �ܿ� ���� ����.
				wREQINSRTNUM -= wSURPLUSNUM;
			}
			else
			{
				//	Note : ���� �����ۿ� ��ġ�� �� �����ε� ���� ���� �������� ����� ��.
				sITEMCUSTOM.wTurnNum += wREQINSRTNUM;

				//	Note : Ŭ���̾�Ʈ�� ���� ������ �˸�.
				GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
				NetMsg.wPosX = sINVENITEM.wPosX;
				NetMsg.wPosY = sINVENITEM.wPosY;
				NetMsg.wTurnNum = sITEMCUSTOM.wTurnNum;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

				//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
				//
				GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
				if ( pParty )
				{
					GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
					NetMsg.dwGaeaID = m_dwGaeaID;
					NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
					pParty->SENDMSG2NEAR ( this, &NetMsg );
				}

				GLMSG::SNETPC_PICKUP_ITEM NetMsgPickUp;
				NetMsgPickUp.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgPickUp);

				//	Note : ������ Land���� ����.
				m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );

				return S_OK;
			}
		}

		//	Note : ������ �κ��� ������ �������� ���� �ľ��� �κ��� �� ������ �ִ��� �˻�.

		//	�ִ��ħ������ ����.
		WORD wONENUM = wREQINSRTNUM / wPILENUM;
		WORD wITEMNUM = wONENUM;

		//	���а�ħ�������� ��ħ��.
		WORD wSPLITNUM = wREQINSRTNUM % wPILENUM;
		if ( wSPLITNUM > 0 )				wITEMNUM += 1;
		if ( wSPLITNUM==0 && wITEMNUM>=1 )	wSPLITNUM = wPILENUM;

		for ( WORD i=0; i<wITEMNUM; ++i )
		{
			WORD wInsertPosX(0), wInsertPosY(0);

			// Ȱ��ȭ�� �κ��� �켱�� ��ġ...( ���� �˻������� ���� �켱�̶� ���� �ϱ� �ָ���. ) 
			BOOL bSPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );
			if ( !bSPACE )
			{
				bSPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY, true );
				
				GASSERT(bSPACE&&"���� ������ �̸� üũ�� �ϰ� �������� �־����� ������ ������.");
				if ( !bSPACE )			return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.
			}

			//	Note : ���ο� �������� �־���.
			//
			SITEMCUSTOM sITEMCUSTOM = pItemDrop->sItemCustom;
			if ( wITEMNUM==(i+1) )	sITEMCUSTOM.wTurnNum = wSPLITNUM;	//	������ �������� �ܿ���.
			else					sITEMCUSTOM.wTurnNum = wPILENUM;	//	�ƴ� ���� ������.

			//	�ι��丮�� �ֽ��ϴ�.
			m_cInventory.InsertItem ( sITEMCUSTOM, wInsertPosX, wInsertPosY, true );
			SINVENITEM *pInvenItem = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
			if ( !pInvenItem )		return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

			//	�ι꿡 ������ �־��ִ� �޽���.
			GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
			NetMsgInven.Data = *pInvenItem;
			NetMsgInven.bAllLine = true;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);
		}

		//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
		//
		GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
		if ( pParty )
		{
			GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
			NetMsg.dwGaeaID = m_dwGaeaID;
			NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
			pParty->SENDMSG2NEAR ( this, &NetMsg );
		}

		GLMSG::SNETPC_PICKUP_ITEM NetMsgPickUp;
		NetMsgPickUp.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgPickUp);

		//	Note : ������ ����� �ڵ� ����Ʈ ���� ����.
		//
		QuestStartFromGetITEM ( pItemDrop->sItemCustom.sNativeID );

		//	Note : PILE ��ħ �������̹Ƿ� �α� �ۼ� �Ұ���.
		//

		//	Note : ������ Land���� ����.
		m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );
	}
	else
	{
		WORD wPosX = 0, wPosY = 0;

		// Ȱ��ȭ�� �κ��� �켱�� ��ġ...( ���� �˻������� ���� �켱�̶� ���� �ϱ� �ָ���. ) 
		BOOL bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )
		{
				bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wPosX, wPosY, true );
				
				GASSERT(bOk&&"���� ������ �̸� üũ�� �ϰ� �������� �־����� ������ ������.");
				if ( !bOk )			return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.
		}

		//	Note : ������ �ݱ� ����.
		m_cInventory.InsertItem ( pItemDrop->sItemCustom, wPosX, wPosY, true );
		SINVENITEM *pInvenItem = m_cInventory.GetItem ( wPosX, wPosY );
		if ( !pInvenItem )	return E_FAIL;	//	�����δ� ������ ������ ������ �����߻� ���ɼ��� ����.

		//	Note : ��Ƽ���� ��� �ٸ� ��Ƽ������ ������ ���� �˸�.
		//
		GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty ( m_dwPartyID );
		if ( pParty )
		{
			GLMSG::SNETPC_PARTY_MBR_PICKUP_BRD NetMsg;
			NetMsg.dwGaeaID = m_dwGaeaID;
			NetMsg.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
			pParty->SENDMSG2NEAR ( this, &NetMsg );
		}

		GLMSG::SNETPC_PICKUP_ITEM NetMsgPickUp;
		NetMsgPickUp.sNID_ITEM = pItemDrop->sItemCustom.sNativeID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgPickUp);

		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pInvenItem->sItemCustom.wTurnNum );

		//	[�ڽſ���] �޽��� �߻�.
		GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
		NetMsg_Inven.Data = *pInvenItem;
		NetMsg_Inven.bAllLine = true;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);

		//	Note : ������ ����� �ڵ� ����Ʈ ���� ����.
		//
		QuestStartFromGetITEM ( pItemDrop->sItemCustom.sNativeID );

		//	Note : ������ Land���� ����.
		m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );
	}

	return S_OK;
}

void GLChar::DisableSkillFact()
{
	EMSLOT emRHand = GetCurRHand();	
	SITEM* pRightItem = GET_SLOT_ITEMDATA(emRHand);		

	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
	
        PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
		if ( !pSkill ) continue;

		// ��ų �ڽ� ����
		if ( pSkill->m_sBASIC.emIMPACT_TAR != TAR_SELF || pSkill->m_sBASIC.emIMPACT_REALM != REALM_SELF ) continue;
				
		GLITEM_ATT emSKILL_RITEM = pSkill->m_sBASIC.emUSE_RITEM;

		// ��ų ���� ���� ����
		if ( emSKILL_RITEM == ITEMATT_NOCARE )	continue;

		// ��ų �� ���Ⱑ ����ġ
		if( !pRightItem || !CHECHSKILL_ITEM(emSKILL_RITEM,pRightItem->sSuitOp.emAttack) )
		{
			DISABLESKEFF( i );
		}
	}
}

//************************************
// Method:    InsertMoney
// FullName:  GLChar::InsertMoney
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: LONGLONG lnAmount
// Explanation: ����� ��Ƽ���� �ݾ� �й�� ���Ǵ� �Լ��̴�
//************************************
void GLChar::InsertMoney ( LONGLONG lnAmount )
{
	//	�� ����.

	lnAmount = CalculateInsertmoney( lnAmount );

	CheckMoneyUpdate( m_lnMoney, lnAmount, TRUE, "Insert Party Money." );
	m_bMoneyUpdate = TRUE;

	m_lnMoney += lnAmount;

	//	Note : �ݾ� �α�.
	//
	if ( lnAmount>EMMONEY_LOG )
	{
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, 0, ID_CHAR, m_dwCharID, lnAmount, EMITEM_ROUTE_GROUND );
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
	}

//#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM)// ***Tracing Log print
	if ( m_bTracingUser )
	{
		NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
		TracingMsg.nUserNum  = GetUserID();
		StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );

		CString strTemp;
		strTemp.Format( "Insert Party Money, [%s][%s], Insert Amount:[%I64d], Have Money:[%I64d]",
				 m_szUID, m_szName,  lnAmount, m_lnMoney );

		StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

		m_pGLGaeaServer->SENDTOAGENT( m_dwClientID, &TracingMsg );
	}
//#endif

#if defined(VN_PARAM) //vietnamtest%%%
	if( lnAmount != 0 )
#endif
	{
		//	�ݾ� ��ȭ Ŭ���̾�Ʈ�� �˷���.
		GLMSG::SNETPC_UPDATE_MONEY NetMsg;
		NetMsg.lnMoney = m_lnMoney;
		//add money 2
		NetMsg.lnVoteP = m_lnVoteP;
		//add money 3
		NetMsg.lnPremP = m_lnPremP;
		//add contributionpoints by CNDev
		NetMsg.lnContribP = m_lnContribP;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
	}

#if defined(VN_PARAM) //vietnamtest%%%
	GLMSG::SNETPC_VIETNAM_GAIN_MONEY NetMsg2;
	NetMsg2.gainMoney = m_lVNGainSysMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg2 );
#endif
}

LONGLONG GLChar::CalculateInsertmoney( LONGLONG lnAmount )
{
	LONGLONG lnMoney = lnAmount;
	//	�� ����.	
	lnMoney = lnMoney * (m_pGLGaeaServer->GetMoneyGainRate(m_CHARINDEX,m_wLevel) + m_pGLGaeaServer->GetMoneyGainRate(m_wSchool) - 1);

#if defined(TH_PARAM) || defined(MYE_PARAM ) || defined(MY_PARAM) || defined(KR_PARAM) || defined( RELEASE_A )
	if( m_dwThaiCCafeClass == 1 || m_nMyCCafeClass == 1 ) // A���
	{
		lnMoney *= 2;
	}else if( m_dwThaiCCafeClass == 2 || m_nMyCCafeClass == 2 ) // B���
	{
		lnMoney *= 1.75f;
	}else if( m_dwThaiCCafeClass == 3 || m_nMyCCafeClass == 3 ) // C���
	{
		lnMoney *= 1.5f;
	}
#endif

#ifdef CH_PARAM_USEGAIN //chinaTest%%%
	if( m_ChinaGainType == GAINTYPE_HALF )
	{
		lnMoney /= 2;
	}else if( m_ChinaGainType == GAINTYPE_EMPTY )
	{
		lnMoney = 0;
	}
#endif

	//NeedToSaveValue_B
#if defined(VN_PARAM) //vietnamtest%%%
	if( m_dwVietnamGainType == GAINTYPE_HALF )
	{
		lnMoney /= 2;
		m_lVNGainSysMoney += lnMoney;
	}else if( m_dwVietnamGainType == GAINTYPE_EMPTY )
	{
		m_lVNGainSysMoney += lnMoney;
		lnMoney = 0;		
	}
#endif

	return lnMoney;
}

HRESULT GLChar::InsertToInven ( PMONEYDROP pMoneyDrop )
{
	//	�� ����.	
	if( pMoneyDrop->bDropMonster )
	{
		pMoneyDrop->lnAmount = CalculateInsertmoney( pMoneyDrop->lnAmount );
	}
	

	CheckMoneyUpdate( m_lnMoney, pMoneyDrop->lnAmount, TRUE, "Insert Money." );
	m_bMoneyUpdate = TRUE;

	m_lnMoney += pMoneyDrop->lnAmount;

	//	Note : �ݾ� �α�.
	//
	if ( pMoneyDrop->lnAmount>EMMONEY_LOG )
	{
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, 0, ID_CHAR, m_dwCharID, pMoneyDrop->lnAmount, EMITEM_ROUTE_GROUND );
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
	}

//#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if ( m_bTracingUser )
	{
		NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
		TracingMsg.nUserNum  = GetUserID();
		StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );

		CString strTemp;
		strTemp.Format( "Insert Money, [%s][%s], Insert Amount:[%I64d], Have Money:[%I64d]",
			m_szUID, m_szName, pMoneyDrop->lnAmount, m_lnMoney );

		StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

		m_pGLGaeaServer->SENDTOAGENT( m_dwClientID, &TracingMsg );
	}
//#endif

#if defined(VN_PARAM) //vietnamtest%%%
	if( pMoneyDrop->lnAmount != 0 )
#endif
	{
		GLMSG::SNETPC_PICKUP_MONEY NetMsg;
		NetMsg.lnMoney = m_lnMoney;
		NetMsg.lnPickUp = pMoneyDrop->lnAmount;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
	}

#if defined(VN_PARAM) //vietnamtest%%%
	GLMSG::SNETPC_VIETNAM_GAIN_MONEY NetMsg2;
	NetMsg2.gainMoney = m_lVNGainSysMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg2 );
#endif

	//	���� Land���� ����.
	m_pLandMan->DropOutMoney ( pMoneyDrop->dwGlobID );

	return S_OK;
}

//	Note : Field�� ������,���� Inventory �� �ֱ� �õ�.
//
HRESULT GLChar::MsgReqFieldToInven ( NET_MSG_GENERIC* nmg )
{
	if ( !m_pLandMan )	return E_FAIL;

	GLMSG::SNETPC_REQ_FIELD_TO_INVEN *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_FIELD_TO_INVEN*> ( nmg );

	

	if ( pNetMsg->emCrow==CROW_ITEM )
	{
		PITEMDROP pItemDrop = m_pLandMan->GetItem ( pNetMsg->dwID );

		if ( !pItemDrop )
		{
			//	�������� ��������ϴ�.
			GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
			NetMsg.emCrow = CROW_ITEM;
			NetMsg.emTakeFB = EMTAKE_FB_OFF;

			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
			return E_FAIL;
		}

		// ���Ȯ��
		if ( !IsValidBody() )	return E_FAIL;

		//	�Ÿ� üũ
		D3DXVECTOR3 vPos;

		if ( pNetMsg->bPet )	
		{
			PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
			if ( pMyPet )	vPos = pMyPet->m_vPos;
		}	
		else vPos = m_vPos;

		const D3DXVECTOR3 &vTarPos = pItemDrop->vPos;

		D3DXVECTOR3 vDistance = vPos - vTarPos;
		float fDistance = D3DXVec3Length ( &vDistance );

		WORD wTarBodyRadius = 4;
		WORD wTakeRange = wTarBodyRadius + GETBODYRADIUS() + 2;
		WORD wTakeAbleDis = wTakeRange + 15;

		if ( fDistance>wTakeAbleDis )
		{
			//	�Ÿ��� �ٴϴ�.
			GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
			NetMsg.emCrow = CROW_ITEM;
			NetMsg.emTakeFB = EMTAKE_FB_DISTANCE;

			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
						
			return E_FAIL;
		}

        // �ʵ忡�� �ֿ��� ��ȿ�� üũ( Ŭ����Ŀ ���� ��ó )
		{	
			GLITEMLMT & glItemmt = GLITEMLMT::GetInstance();
			GLItemMan & glItemMan = GLItemMan::GetInstance();
			SITEMCUSTOM sCUSTOM = pItemDrop->sItemCustom;
			const CTime cTIME_CUR = CTime::GetCurrentTime();
			
			SITEM *pITEM = glItemMan.GetItem ( sCUSTOM.sNativeID );
			if ( !pITEM )	return E_FAIL;

			bool bDELETE(false);

			if ( pITEM->IsTIMELMT() )
			{			
				
				CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
				CTime cTIME_LMT(sCUSTOM.tBORNTIME);
				cTIME_LMT += cSPAN;

				if ( cTIME_CUR > cTIME_LMT )
				{
					//	�ð� �������� ������ ���� �α� ����.
					glItemmt.ReqItemRoute ( sCUSTOM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, sCUSTOM.wTurnNum );

					//	�ð� �������� ������ ���� �˸�.
					GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
					NetMsgInvenDelTimeLmt.nidITEM = sCUSTOM.sNativeID;
					m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

					bDELETE = true;				

					//	Note : �ڽ��� ����.
					if ( sCUSTOM.nidDISGUISE!=SNATIVEID(false) )
					{
						SITEM *pONE = glItemMan.GetItem ( sCUSTOM.nidDISGUISE );
						if ( !pONE )		return E_FAIL;

						SITEMCUSTOM sITEM_NEW;
						sITEM_NEW.sNativeID = sCUSTOM.nidDISGUISE;
						CTime cTIME = CTime::GetCurrentTime();
						if ( sCUSTOM.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
						{
							cTIME = CTime(sCUSTOM.tDISGUISE);

							CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
							cTIME -= tLMT;
						}

						//	Note : ������ ����.
						//
						sITEM_NEW.tBORNTIME = cTIME.GetTime();
						sITEM_NEW.wTurnNum = 1;
						sITEM_NEW.cGenType = EMGEN_BILLING;
						sITEM_NEW.cChnID = (BYTE)m_pGLGaeaServer->GetServerChannel();
						sITEM_NEW.cFieldID = (BYTE)m_pGLGaeaServer->GetFieldSvrID();
						sITEM_NEW.lnGenNum = glItemmt.RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

						// ������ �ڽ����� �ٴڿ��� �ٽ� ������.
						CItemDrop cDropItem;
						cDropItem.sItemCustom = sITEM_NEW;
						m_pLandMan->DropItem ( m_vPos, &(cDropItem.sItemCustom), EMGROUP_ONE, m_dwGaeaID );

						//	Note : �и��� �ڽ��� �α�.
						glItemmt.ReqItemRoute ( sITEM_NEW, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_GROUND, sITEM_NEW.wTurnNum );												
					}
					
					m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );

					return E_FAIL;
				}
			}
			
			if ( !bDELETE && sCUSTOM.nidDISGUISE!=SNATIVEID(false) )
			{
				if ( sCUSTOM.tDISGUISE != 0 )
				{
					CTime cTIME_LMT(sCUSTOM.tDISGUISE);
					if ( cTIME_LMT.GetYear()!=1970 )
					{
						if ( cTIME_CUR > cTIME_LMT )
						{
							//	�ð� �������� ������ ���� �˸�.
							GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
							NetMsgInvenDelTimeLmt.nidITEM = sCUSTOM.nidDISGUISE;
							m_pGLGaeaServer->SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

							//	Note : �ڽ��� ���� ����.
							pItemDrop->sItemCustom.tDISGUISE = 0;
							pItemDrop->sItemCustom.nidDISGUISE = SNATIVEID(false);
						}
					}
				}
			}		
		}
		// �������� �ڽŰ� ������ �ʿ� �ִ��� ( ������ ������ ������Ŭ������ �������� ȹ���ϴ� ���� ��ó )
		if ( pItemDrop->sMapID != m_sMapID )	return E_FAIL;

		if ( !pItemDrop->IsTakeItem(m_dwPartyID,m_dwGaeaID) )
		{
			//	�������� �ٸ� ������� ����Ǿ� �ֽ��ϴ�.
			GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
			NetMsg.emCrow = CROW_ITEM;
			NetMsg.emTakeFB = EMTAKE_FB_RESERVED;

			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
			return E_FAIL;
		}

		SITEM *pItem = GLItemMan::GetInstance().GetItem(pItemDrop->sItemCustom.sNativeID);
		if ( !pItem )		return E_FAIL;

		if ( pItem->sBasicOp.emItemType==ITEM_QITEM )
		{
			TourchQItem ( pNetMsg->dwID );
			return S_OK;
		}

		//	Note : ��Ƽ���̰� �̺�Ʈ �������� �ƴҶ� ��Ƽ �������� �˻��� ó��.
		//
		GLPARTY_FIELD* pParty = GLGaeaServer::GetInstance().GetParty(m_dwPartyID);
		if ( pParty && !pItem->ISEVENTITEM() )
		{
			bool ballot = pParty->DOITEM_ALLOT ( this, pItemDrop );
			if ( ballot )		return S_OK;
		}

		//	Note : �̺�Ʈ ������ �ߺ����� �������� ����.
		//
		if ( pItem->ISEVENTITEM() && m_cInventory.HaveEventItem() )
		{
			//	�̹� �̺�Ʈ �������� ������ �ֽ��ϴ�.
			GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
			NetMsg.emCrow = CROW_ITEM;
			NetMsg.emTakeFB = EMTAKE_FB_TOO_EVENT_ITEM;

			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );

			return E_FAIL;
		}

		//	Note : ������ �κ��� ���� ( �κ��� ���� ������ ���� ���� �Ұ���. ) ��Ʈ���� ��� Ž�� �κ��丮�� ���� ���� �ִ�.
		//

#if defined(VN_PARAM) //vietnamtest%%%
		if( m_dwVietnamGainType == GAINTYPE_EMPTY )
		{
 			InsertToVNGainInven( pItemDrop );
		}else{
			InsertToInven ( pItemDrop );
		}
#else
		InsertToInven ( pItemDrop );
#endif


/*
		// ȹ���� �������� ��ī�� && ������ ���̶�� �������� �����Ѵ�.
		// �־��̵� 0 �� �ƴϸ� ��ī���̸鼭 DB�� ���� ������ ���̴�.
		if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD && pItemDrop->sItemCustom.dwPetID != 0 )
		{
			CExchangePet* pDbAction = new CExchangePet ( m_dwCharID, pItemDrop->sItemCustom.dwPetID );
			GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
			if ( pDBMan ) pDBMan->AddJob ( pDbAction );
		}
*/

	}
	else if ( pNetMsg->emCrow==CROW_MONEY )
	{
		PMONEYDROP pMoneyDrop = m_pLandMan->GetMoney ( pNetMsg->dwID );

		  
		if ( !pMoneyDrop )
		{
			//	���� ��������ϴ�.
			GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
			NetMsg.emCrow = CROW_MONEY;
			NetMsg.emTakeFB = EMTAKE_FB_OFF;

			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
			return E_FAIL;
		}

		// ���Ȯ��
		if ( !IsValidBody() )	return E_FAIL;

		//	�Ÿ� üũ
		D3DXVECTOR3 vPos;

		if ( pNetMsg->bPet )	
		{
			PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
			if ( pMyPet )	vPos = pMyPet->m_vPos;
		}	
		else vPos = m_vPos;

		const D3DXVECTOR3 &vTarPos = pMoneyDrop->vPos;

		D3DXVECTOR3 vDistance = vPos - vTarPos;
		float fDistance = D3DXVec3Length ( &vDistance );

		WORD wTarBodyRadius = 4;
		WORD wTakeRange = wTarBodyRadius + GETBODYRADIUS() + 2;
		WORD wTakeAbleDis = wTakeRange + 15;

		if ( fDistance>wTakeAbleDis )
		{
			//	�Ÿ��� �ٴϴ�.
			GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
			NetMsg.emCrow = CROW_ITEM;
			NetMsg.emTakeFB = EMTAKE_FB_DISTANCE;

			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
			return E_FAIL;			
			
		}

		if ( !pMoneyDrop->IsTakeItem(m_dwPartyID,m_dwGaeaID) )
		{
			//	���� �ٸ� ������� ����Ǿ� �ֽ��ϴ�.
			GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
			NetMsg.emCrow = CROW_MONEY;
			NetMsg.emTakeFB = EMTAKE_FB_RESERVED;

			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
			return E_FAIL;
		}


		//	Note : ��Ƽ���̰� �̺�Ʈ �������� �ƴҶ� ��Ƽ �������� �˻��� ó��.
		//
		GLPARTY_FIELD* pParty = GLGaeaServer::GetInstance().GetParty(m_dwPartyID);
		if ( pParty )
		{
			bool ballot = pParty->DOMONEY_ALLOT ( this, pMoneyDrop, m_pLandMan );
			if ( ballot )
			{
				return S_OK;
			}
		}

		//	�� �ݱ� ����.
		InsertToInven ( pMoneyDrop );
	}

	return S_OK;
}

//	Note : Field�� ������,���� �տ� ��� �õ�.	( �տ� �� �������� ���ٰ� ������. )
//
HRESULT GLChar::MsgReqFieldToHold ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_FIELD_TO_HOLD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_FIELD_TO_HOLD*> ( nmg );

	PITEMDROP pItemDrop = m_pLandMan->GetItem ( pNetMsg->dwGlobID );

	// �ʵ忡�� �������� �տ� ��°��� �κ��� ���� á�� ��� ���̴�.
	// ��Ʈ��Ž���� ��� �տ� ��°� �����ؾ� �Ѵ�.
#if defined(VN_PARAM) //vietnamtest%%%
	if( m_dwVietnamGainType == GAINTYPE_EMPTY )	return E_FAIL;
#endif

	if ( !pItemDrop )
	{
		//	�������� ��������ϴ�.
		GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
		NetMsg.emCrow = CROW_ITEM;
		NetMsg.emTakeFB = EMTAKE_FB_OFF;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
		return S_FALSE;
	}

	// �������� �ڽŰ� ������ �ʿ� �ִ��� ( ������ ������ ������Ŭ������ �������� ȹ���ϴ� ���� ��ó )
	if ( pItemDrop->sMapID != m_sMapID )	return S_FALSE;

	const SITEM *pItemData = GLItemMan::GetInstance().GetItem(pItemDrop->sItemCustom.sNativeID);
	if ( !pItemData )	return S_FALSE;

	if ( !pItemDrop->IsTakeItem(m_dwPartyID,m_dwGaeaID) )
	{
		//	�������� �ٸ� ������� ����Ǿ� �ֽ��ϴ�.
		GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
		NetMsg.emCrow = CROW_ITEM;
		NetMsg.emTakeFB = EMTAKE_FB_RESERVED;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );
		return S_FALSE;
	}

	if ( pItemData->sBasicOp.emItemType==ITEM_QITEM )
	{
		TourchQItem ( pNetMsg->dwGlobID );
		return S_OK;
	}

	//	Note : ��Ƽ���̰� �̺�Ʈ �������� �ƴҶ� ��Ƽ �������� �˻��� ó��.
	//
	GLPARTY_FIELD* pParty = GLGaeaServer::GetInstance().GetParty(m_dwPartyID);
	if ( pParty && !pItemData->ISEVENTITEM() )
	{
		bool ballot = pParty->DOITEM_ALLOT ( this, pItemDrop );
		if ( ballot )		return S_OK;
	}

	//	Note : �̺�Ʈ ������ �ߺ����� �������� ����.
	//
	if ( pItemData->ISEVENTITEM() && m_cInventory.HaveEventItem() )
	{
		//	�̹� �̺�Ʈ �������� ������ �ֽ��ϴ�.
		GLMSG::SNETPC_REQ_TAKE_FB NetMsg;
		NetMsg.emCrow = CROW_ITEM;
		NetMsg.emTakeFB = EMTAKE_FB_TOO_EVENT_ITEM;

		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );

		return S_FALSE;
	}

	//	Field�� �־��� �������� �տ� ��.
	HOLD_ITEM ( pItemDrop->sItemCustom );

	//	Note :�������� ���� ���� ��� ���.
	//
	GLITEMLMT::GetInstance().ReqItemRoute ( pItemDrop->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pItemDrop->sItemCustom.wTurnNum );

	//	[�ڽſ���] �޽��� �߻�.
	GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
	NetMsg_PutOn.emSlot = SLOT_HOLD;
	NetMsg_PutOn.sItemCustom = pItemDrop->sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);

	//	Note : ������ Land���� ����.
	m_pLandMan->DropOutItem ( pItemDrop->dwGlobID );

	return S_OK;
}

//	Note : Inventory�� �������� �տ� ��� �õ�.
//
HRESULT GLChar::MsgReqInvenToHold ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_INVEN_TO_HOLD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_INVEN_TO_HOLD*> ( nmg );
	
	static GLMSG::SNETPC_REQ_HOLD_FB NetMsg_fb(NET_MSG_GCTRL_REQ_INVEN_TO_HOLD,EMHOLD_FB_OFF);

	//	��û�� ������ ��ȿ�� �˻�.
	SINVENITEM *pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_fb);
		return S_FALSE;
	}

	//	�տ� �� �������� ������.
	if ( VALID_HOLD_ITEM() )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_fb);
		return S_FALSE;
	}

	SITEM* pITEM = GLItemMan::GetInstance().GetItem( pInvenItem->sItemCustom.sNativeID );
	if ( !pITEM ) return S_FALSE;
	if ( pITEM->sBasicOp.emItemType == ITEM_PET_CARD && m_bGetPetFromDB )
	{
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "PetCard Inven to Hold Error, m_bGetPetFromDB : %d",
															 m_bGetPetFromDB );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_fb);
		return S_FALSE;
	}

	//	�ι꿡 �ִ� ������ ���.
	SINVENITEM sInven_BackUp = *pInvenItem;

	//	�ι꿡�� ���� �������� ����.
	m_cInventory.DeleteItem ( sInven_BackUp.wPosX, sInven_BackUp.wPosY );

	//	�ι꿡 �־��� �������� �տ� ��.
	HOLD_ITEM ( sInven_BackUp.sItemCustom );

	//	[�ڽſ���] �޽��� �߻�.	- �ι꿡�� ������ ����.
	GLMSG::SNETPC_INVEN_DELETE NetMsg;
	NetMsg.wPosX = sInven_BackUp.wPosX;
	NetMsg.wPosY = sInven_BackUp.wPosY;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

	//	[�ڽſ���] �ι꿡 �־��� �������� �տ� ��.
	GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
	NetMsg_PutOn.emSlot = SLOT_HOLD;
	NetMsg_PutOn.sItemCustom = sInven_BackUp.sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);

	return S_OK;
}

HRESULT GLChar::MsgReqInvenExHold ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_INVEN_EX_HOLD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_INVEN_EX_HOLD*> ( nmg );

	if ( !VALID_HOLD_ITEM() )
	{
		return S_FALSE;
	}

	SINVENITEM *pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )
	{
		return S_FALSE;
	}

	SINVENITEM sInven_BackUp = *pInvenItem;		//	�ι꿡 �ִ� ������ ���.
	
	WORD wPosX = sInven_BackUp.wPosX;			//	������ ��ġ.
	WORD wPosY = sInven_BackUp.wPosY;

	SITEMCUSTOM sItemHold = GET_HOLD_ITEM();	//	�տ� �� ������ ���.

#if defined(VN_PARAM) //vietnamtest%%%
		if ( sItemHold.bVietnamGainItem )	return S_FALSE;
#endif

	SITEM* pInvenData = GLItemMan::GetInstance().GetItem ( sInven_BackUp.sItemCustom.sNativeID );
	SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( sItemHold.sNativeID );
	if ( !pInvenData || !pHoldData )	return S_FALSE;

	//	Note : ���� �Ҽ� �ִ� ���������� ����.
	bool bMERGE(false);
	bMERGE = (pInvenData->sBasicOp.sNativeID==pHoldData->sBasicOp.sNativeID);
	if (bMERGE)	bMERGE = ( pInvenData->ISPILE() );
	if (bMERGE) bMERGE = ( sItemHold.wTurnNum<pInvenData->sDrugOp.wPileNum );
	if (bMERGE) bMERGE = ( sInven_BackUp.sItemCustom.wTurnNum<pInvenData->sDrugOp.wPileNum );
	if ( bMERGE )
	{
		WORD wTurnNum = sInven_BackUp.sItemCustom.wTurnNum + sItemHold.wTurnNum;
		
		if ( wTurnNum <= pInvenData->sDrugOp.wPileNum )
		{
			//	Note : ��ħ �� ����.
			pInvenItem->sItemCustom.wTurnNum = wTurnNum;

			//	�� �־��� ������ ����.
			RELEASE_HOLD_ITEM ();

			//	Note : �κ��� ������ ������Ʈ.
			GLMSG::SNET_INVEN_ITEM_UPDATE NetMsgInvenItem;
			NetMsgInvenItem.wPosX = pNetMsg->wPosX;
			NetMsgInvenItem.wPosY = pNetMsg->wPosY;
			NetMsgInvenItem.sItemCustom = pInvenItem->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenItem);

			//	[�ڽſ���] �� �־��� ������ ����.
			GLMSG::SNETPC_PUTON_RELEASE NetMsg_ReleaseHold(SLOT_HOLD);
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_ReleaseHold);
		}
		else
		{
			WORD wSplit = wTurnNum - pInvenData->sDrugOp.wPileNum;
			
			//	���� ������ ���� ����.
			pInvenItem->sItemCustom.wTurnNum = pInvenData->sDrugOp.wPileNum;

			//	�տ� ��� �ִ� ������ ����.
			sItemHold.wTurnNum = wSplit;
			HOLD_ITEM ( sItemHold );

			//	Note : �տ��� ������ ����.
			GLMSG::SNETPC_PUTON_UPDATE NetMsgPutOn;
			NetMsgPutOn.emSlot = SLOT_HOLD;
			NetMsgPutOn.sItemCustom = sItemHold;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgPutOn);

			//	Note : �κ��� ������ ������Ʈ.
			GLMSG::SNET_INVEN_ITEM_UPDATE NetMsgInvenItem;
			NetMsgInvenItem.wPosX = pNetMsg->wPosX;
			NetMsgInvenItem.wPosY = pNetMsg->wPosY;
			NetMsgInvenItem.sItemCustom = pInvenItem->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenItem);
		}

		return S_OK;
	}

	//	�ι꿡 �ִ� ������ ����.
	BOOL bOk = m_cInventory.DeleteItem ( sInven_BackUp.wPosX, sInven_BackUp.wPosY );

	//	Note : ���� �տ��� �����ư� ����� �������� ũ�Ⱑ Ʋ�� ���.
	//	��� �ִ� �������� �� ������ ã�´�.
//	if ( !pInvenData->SAME_INVENSIZE ( *pHoldData ) )
//	{
		//	������ġ�� ������ �ִ��� �˻�.
		bOk = m_cInventory.IsInsertable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )
		{
			//	����ִ� ������ �ι꿡 ���� ������ �ִ��� �˻�.
			bOk = m_cInventory.FindInsrtable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, wPosX, wPosY );
			if ( !bOk )
			{
				//	roll-back : �ι꿡�� �����ߴ� �������� �ٽ� ����.
				m_cInventory.InsertItem ( sInven_BackUp.sItemCustom, sInven_BackUp.wPosX, sInven_BackUp.wPosY, true );

				return S_FALSE;
			}
		}
//}

	//	����ִ� ������ �ι꿡 ����.
	m_cInventory.InsertItem ( sItemHold, wPosX, wPosY );
	SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem(wPosX,wPosY);
	if ( !pINSERT_ITEM )
	{
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write ( "error MsgReqInvenExHold(), sItemHold, id(%d,%d) to pos(%d,%d)",
			sItemHold.sNativeID.wMainID, sItemHold.sNativeID.wSubID, wPosX, wPosY );
		return S_FALSE;
	}

	//	�ι꿡 �־��� �������� �տ� ��.
	HOLD_ITEM ( sInven_BackUp.sItemCustom );

	//	[�ڽſ���] �ι꿡 �ִ� ������ ����, �� ������ ����.
	GLMSG::SNETPC_INVEN_DEL_AND_INSERT NetMsg_Del_Insert;
	NetMsg_Del_Insert.wDelX = sInven_BackUp.wPosX;					//	������ ������.
	NetMsg_Del_Insert.wDelY = sInven_BackUp.wPosY;
	if ( pINSERT_ITEM )	NetMsg_Del_Insert.sInsert = *pINSERT_ITEM;	//	���ԵǴ� ������.

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Del_Insert);

	//	[�ڽſ���] �ι꿡 �־��� �������� �տ� ��.
	GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
	NetMsg_PutOn.emSlot = SLOT_HOLD;
	NetMsg_PutOn.sItemCustom = sInven_BackUp.sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);


	//	��ȿ�Ⱓ �ʰ� ����.	
	const SINVENITEM& sINVENITEM = *pINSERT_ITEM;
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.sNativeID );
	if ( pITEM && pITEM->IsTIMELMT() )
	{

		if ( pITEM->sDrugOp.tTIME_LMT != 0 )
		{
			const CTime cTIME_CUR = CTime::GetCurrentTime();

			CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
			CTime cTIME_LMT(sINVENITEM.sItemCustom.tBORNTIME);
			cTIME_LMT += cSPAN;

			WORD wInsertPosX = sINVENITEM.wPosX, wInsertPosY = sINVENITEM.wPosY;


			if ( cTIME_CUR > cTIME_LMT )
			{
				// Ż���� ���
				if ( pITEM->sBasicOp.emItemType == ITEM_VEHICLE && sINVENITEM.sItemCustom.dwVehicleID != 0 )
				{
					GLVEHICLE* pNewVehicle = new GLVEHICLE();
					CGetVehicle* pGetVehicle = new CGetVehicle ( pNewVehicle, 
												 sINVENITEM.sItemCustom.dwVehicleID, 
												 m_dwClientID, 
												 m_dwCharID, 
												 true );
					GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
					if ( pDBMan ) pDBMan->AddJob ( pGetVehicle );
				}

				// ��ī���� ���
				if ( pITEM->sBasicOp.emItemType == ITEM_PET_CARD && sINVENITEM.sItemCustom.dwPetID != 0 )
				{
					CDeletePet* pDbAction = new CDeletePet ( m_dwCharID, sINVENITEM.sItemCustom.dwPetID );
					GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
					if ( pDBMan ) pDBMan->AddJob ( pDbAction );

					// Ȱ�����̸� ������� ���ش�.
					PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
					if ( pMyPet && sINVENITEM.sItemCustom.dwPetID == pMyPet->m_dwPetID )
					{
						// ���� �Ǽ��縮�� �˻��Ѵ�.
						pMyPet->UpdateTimeLmtItem ( this );

						// �־��ٲ� �־��ش�.
						for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
						{
							CItemDrop cDropItem;
							cDropItem.sItemCustom = pMyPet->m_PutOnItems[i];
							if ( IsInsertToInven ( &cDropItem ) ) InsertToInven ( &cDropItem );
							else
							{
								if ( m_pLandMan )
								{
									m_pLandMan->DropItem ( m_vPos, 
														   &(cDropItem.sItemCustom), 
														   EMGROUP_ONE, 
														   m_dwGaeaID );
								}
							}
						}

						GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(pMyPet->m_dwGUID,true,false) );

						// ���� �����Ǹ� ��Ȱ������ Ŭ���̾�Ʈ�� �˷���.
						CGetRestorePetList *pDbAction = new CGetRestorePetList ( m_dwCharID, m_dwClientID );
						if ( pDBMan ) pDBMan->AddJob ( pDbAction );
					}
					else
					{
						// �Ǽ��縮�� üũ�� ���� DB���� �������ش�.
						GLPET* pNewPet = new GLPET ();
						CGetPet* pGetPet = new CGetPet ( pNewPet, 
														 sINVENITEM.sItemCustom.dwPetID, 
														 m_dwClientID, 
														 m_dwCharID,
														 sINVENITEM.wPosX,
														 sINVENITEM.wPosY,
														 true );
						GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
						if ( pDBMan ) pDBMan->AddJob ( pGetPet );
					}
				}

				//	[�ڽſ���] �ش� �������� �ι꿡�� ����.
				GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
				NetMsgInvenDel.wPosX = sINVENITEM.wPosX;
				NetMsgInvenDel.wPosY = sINVENITEM.wPosY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);

				//	�ð� �������� ������ ���� �˸�.
				GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
				NetMsgInvenDelTimeLmt.nidITEM = sINVENITEM.sItemCustom.sNativeID;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

				//	Note : ������ ����.
				//
				m_cInventory.DeleteItem ( sINVENITEM.wPosX, sINVENITEM.wPosY );
			
				//	Note : �ڽ��� ����.
				if ( sINVENITEM.sItemCustom.nidDISGUISE!=SNATIVEID(false) )
				{
					SITEM *pONE = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.nidDISGUISE );
					if ( !pONE )		return E_FAIL;

					SITEMCUSTOM sITEM_NEW;
					sITEM_NEW.sNativeID = sINVENITEM.sItemCustom.nidDISGUISE;
					CTime cTIME = CTime::GetCurrentTime();
					if ( sINVENITEM.sItemCustom.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
					{
						cTIME = CTime(sINVENITEM.sItemCustom.tDISGUISE);

						CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
						cTIME -= tLMT;
					}

					//	Note : ������ ����.
					//
					sITEM_NEW.tBORNTIME = cTIME.GetTime();
					sITEM_NEW.wTurnNum = 1;
					sITEM_NEW.cGenType = EMGEN_BILLING;
					sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
					sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
					sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

					//	Note : �κ��� �� ���� �˻�.
					BOOL bOk = m_cInventory.IsInsertable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
					if ( !bOk )			return E_FAIL;

					//	Note : �κ��� ����.
					m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
					SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

					GLMSG::SNETPC_INVEN_INSERT NetItemInsert;
					NetItemInsert.Data = *pINSERT_ITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemInsert);

					//	Note : �и��� �ڽ��� �α�.
					GLITEMLMT::GetInstance().ReqItemRoute ( pINSERT_ITEM->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pINSERT_ITEM->sItemCustom.wTurnNum );
				}
			}
		}
	}

	return S_OK;
}

HRESULT GLChar::MsgReqInvenToSlot ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_INVEN_TO_SLOT *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_INVEN_TO_SLOT*> ( nmg );
	if ( pNetMsg->emToSlot >= SLOT_TSIZE )	return S_FALSE;

	SINVENITEM *pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )	return E_FAIL;

	SITEMCUSTOM sItemCustom = pInvenItem->sItemCustom;

	BOOL bOk(FALSE);


	if( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
	{
		//	�ڽ�Ƭ�� ���� �������� ���
		bOk = ACCEPT_ITEM ( sItemCustom.sNativeID, sItemCustom.nidDISGUISE );
		if ( !bOk )	return E_FAIL;
	}
	else
	{
		//	�ڰ����ǿ� �����ϴ��� �˻�.
		bOk = ACCEPT_ITEM ( sItemCustom.sNativeID );
		if ( !bOk )	return E_FAIL;
	}

	//	�ش� ���Կ� ���� �������� �˻�.
	bOk = CHECKSLOT_ITEM ( sItemCustom.sNativeID, pNetMsg->emToSlot );
	if ( !bOk )	return E_FAIL;

	//	�ش� ������ �񿴴��� ����.
	bOk = ISEMPTY_SLOT ( sItemCustom.sNativeID, pNetMsg->emToSlot );
	if ( !bOk ) return E_FAIL;

	//	���Կ� ������ ����.
	SLOT_ITEM ( sItemCustom, pNetMsg->emToSlot );

	//	�ι꿡�� ������ ����.
	m_cInventory.DeleteItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//	[�ڽſ���] �ش� �������� �ι꿡�� ����.
	GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
	NetMsgInvenDel.wPosX = pNetMsg->wPosX;
	NetMsgInvenDel.wPosY = pNetMsg->wPosY;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);

	//	[�ڽſ���] �ش� �������� �ι꿡 �־���.
	GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
	NetMsg_PutOn.emSlot = pNetMsg->emToSlot;
	NetMsg_PutOn.sItemCustom = GET_SLOT_ITEM(pNetMsg->emToSlot);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);

	return S_OK;
}

HRESULT GLChar::MsgReqVNGainToHold(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNETPC_REQ_VNGAIN_TO_HOLD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_VNGAIN_TO_HOLD*> ( nmg );

	static GLMSG::SNETPC_REQ_HOLD_FB NetMsg_fb(NET_MSG_GCTRL_REQ_VNGAIN_TO_HOLD,EMHOLD_FB_OFF);

	//	��û�� ������ ��ȿ�� �˻�.
	SINVENITEM *pInvenItem = m_cVietnamInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_fb);
		return S_FALSE;
	}

	//	�տ� �� �������� ������.
	if ( VALID_HOLD_ITEM() )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_fb);
		return S_FALSE;
	}

	//	�ι꿡 �ִ� ������ ���.
	SINVENITEM sInven_BackUp = *pInvenItem;

	//	�ι꿡�� ���� �������� ����.
	m_cVietnamInventory.DeleteItem ( sInven_BackUp.wPosX, sInven_BackUp.wPosY );

	//	�ι꿡 �־��� �������� �տ� ��.
	HOLD_ITEM ( sInven_BackUp.sItemCustom );

	//	[�ڽſ���] �޽��� �߻�.	- �ι꿡�� ������ ����.
	GLMSG::SNETPC_INVEN_DELETE NetMsg;
	NetMsg.wPosX = sInven_BackUp.wPosX;
	NetMsg.wPosY = sInven_BackUp.wPosY;
	NetMsg.bVietnamInven = TRUE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

	//	[�ڽſ���] �ι꿡 �־��� �������� �տ� ��.
	GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
	NetMsg_PutOn.emSlot = SLOT_HOLD;
	NetMsg_PutOn.sItemCustom = sInven_BackUp.sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);
	return S_OK;
}

HRESULT GLChar::MsgReqVNGainExHold(NET_MSG_GENERIC* nmg)
{

	GLMSG::SNETPC_REQ_VNGAIN_EX_HOLD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_VNGAIN_EX_HOLD*> ( nmg );

	if ( !VALID_HOLD_ITEM() )
	{
		return S_FALSE;
	}

	SINVENITEM *pInvenItem = m_cVietnamInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )
	{
		return S_FALSE;
	}

	SINVENITEM sInven_BackUp = *pInvenItem;		//	�ι꿡 �ִ� ������ ���.

	WORD wPosX = sInven_BackUp.wPosX;			//	������ ��ġ.
	WORD wPosY = sInven_BackUp.wPosY;

	SITEMCUSTOM sItemHold = GET_HOLD_ITEM();	//	�տ� �� ������ ���.

	SITEM* pInvenData = GLItemMan::GetInstance().GetItem ( sInven_BackUp.sItemCustom.sNativeID );
	SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( sItemHold.sNativeID );
	if ( !pInvenData || !pHoldData )	return S_FALSE;

	//	Note : ���� �Ҽ� �ִ� ���������� ����.
	bool bMERGE(false);
	bMERGE = (pInvenData->sBasicOp.sNativeID==pHoldData->sBasicOp.sNativeID);
	if (bMERGE)	bMERGE = ( pInvenData->ISPILE() );
	if (bMERGE) bMERGE = ( sItemHold.wTurnNum<pInvenData->sDrugOp.wPileNum );
	if (bMERGE) bMERGE = ( sInven_BackUp.sItemCustom.wTurnNum<pInvenData->sDrugOp.wPileNum );
	if ( bMERGE )
	{
		WORD wTurnNum = sInven_BackUp.sItemCustom.wTurnNum + sItemHold.wTurnNum;

		if ( wTurnNum <= pInvenData->sDrugOp.wPileNum )
		{
			//	Note : ��ħ �� ����.
			pInvenItem->sItemCustom.wTurnNum = wTurnNum;

			//	�� �־��� ������ ����.
			RELEASE_HOLD_ITEM ();

			//	Note : �κ��� ������ ������Ʈ.
			GLMSG::SNET_INVEN_ITEM_UPDATE NetMsgInvenItem;
			NetMsgInvenItem.wPosX = pNetMsg->wPosX;
			NetMsgInvenItem.wPosY = pNetMsg->wPosY;
			NetMsgInvenItem.sItemCustom = pInvenItem->sItemCustom;
			NetMsgInvenItem.bVietnamInven = TRUE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenItem);

			//	[�ڽſ���] �� �־��� ������ ����.
			GLMSG::SNETPC_PUTON_RELEASE NetMsg_ReleaseHold(SLOT_HOLD);
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_ReleaseHold);
		}
		else
		{
			WORD wSplit = wTurnNum - pInvenData->sDrugOp.wPileNum;

			//	���� ������ ���� ����.
			pInvenItem->sItemCustom.wTurnNum = pInvenData->sDrugOp.wPileNum;

			//	�տ� ��� �ִ� ������ ����.
			sItemHold.wTurnNum = wSplit;
			HOLD_ITEM ( sItemHold );

			//	Note : �տ��� ������ ����.
			GLMSG::SNETPC_PUTON_UPDATE NetMsgPutOn;
			NetMsgPutOn.emSlot = SLOT_HOLD;
			NetMsgPutOn.sItemCustom = sItemHold;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgPutOn);

			//	Note : �κ��� ������ ������Ʈ.
			GLMSG::SNET_INVEN_ITEM_UPDATE NetMsgInvenItem;
			NetMsgInvenItem.wPosX = pNetMsg->wPosX;
			NetMsgInvenItem.wPosY = pNetMsg->wPosY;
			NetMsgInvenItem.sItemCustom = pInvenItem->sItemCustom;
			NetMsgInvenItem.bVietnamInven = TRUE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenItem);
		}

		return S_OK;
	}

	//	�ι꿡 �ִ� ������ ����.
	BOOL bOk = m_cVietnamInventory.DeleteItem ( sInven_BackUp.wPosX, sInven_BackUp.wPosY );

	//	Note : ���� �տ��� �����ư� ����� �������� ũ�Ⱑ Ʋ�� ���.
	//	��� �ִ� �������� �� ������ ã�´�.
//	if ( !pInvenData->SAME_INVENSIZE ( *pHoldData ) )
	{
		//	������ġ�� ������ �ִ��� �˻�.
		BOOL bOk = m_cVietnamInventory.IsInsertable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOk )
		{
			//	����ִ� ������ �ι꿡 ���� ������ �ִ��� �˻�.
			bOk = m_cVietnamInventory.FindInsrtable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, wPosX, wPosY );
			if ( !bOk )
			{
				//	roll-back : �ι꿡�� �����ߴ� �������� �ٽ� ����.
				sInven_BackUp.sItemCustom.bVietnamGainItem = true;
				m_cVietnamInventory.InsertItem ( sInven_BackUp.sItemCustom, sInven_BackUp.wPosX, sInven_BackUp.wPosY, true );

				return S_FALSE;
			}
		}
	}

	//	����ִ� ������ �ι꿡 ����.
	sItemHold.bVietnamGainItem = true;
	m_cVietnamInventory.InsertItem ( sItemHold, wPosX, wPosY );
	SINVENITEM *pINSERT_ITEM = m_cVietnamInventory.GetItem(wPosX,wPosY);
	if ( !pINSERT_ITEM )
	{
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write ( "error MsgReqInvenExHold(), sItemHold, id(%d,%d) to pos(%d,%d)",
			sItemHold.sNativeID.wMainID, sItemHold.sNativeID.wSubID, wPosX, wPosY );
		return S_FALSE;
	}

	//	�ι꿡 �־��� �������� �տ� ��.
	HOLD_ITEM ( sInven_BackUp.sItemCustom );

	//	[�ڽſ���] �ι꿡 �ִ� ������ ����, �� ������ ����.
	GLMSG::SNETPC_INVEN_DEL_AND_INSERT NetMsg_Del_Insert;
	NetMsg_Del_Insert.wDelX = sInven_BackUp.wPosX;					//	������ ������.
	NetMsg_Del_Insert.wDelY = sInven_BackUp.wPosY;
	NetMsg_Del_Insert.bVietnamInven = TRUE;
	if ( pINSERT_ITEM )	NetMsg_Del_Insert.sInsert = *pINSERT_ITEM;	//	���ԵǴ� ������.

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Del_Insert);

	//	[�ڽſ���] �ι꿡 �־��� �������� �տ� ��.
	GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
	NetMsg_PutOn.emSlot = SLOT_HOLD;
	NetMsg_PutOn.sItemCustom = sInven_BackUp.sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);

	return S_OK;
}

HRESULT GLChar::MsgReqHoldToVNGain(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNETPC_REQ_HOLD_TO_VNGAIN *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_HOLD_TO_VNGAIN*> ( nmg );

	if ( !VALID_HOLD_ITEM() )
	{
		return S_FALSE;
	}

	SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
	if ( !pHoldData )
	{
		return E_FAIL;
	}

	//	�κ� ��ġ.
	WORD wInsertPosX = pNetMsg->wPosX, wInsertPosY = pNetMsg->wPosY;

	// ��ŷ ���� ( �������� ����� 0 �϶� )
	//pHoldData->sBasicOp.wInvenSizeX = 0, pHoldData->sBasicOp.wInvenSizeY = 0;

	BOOL bOk = m_cVietnamInventory.IsInsertable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
	if ( !bOk )
	{
		//	�ι��� ��������.
		return E_FAIL;
	}

	//	����ִ� ������ �ι꿡 ����.
	SITEMCUSTOM& sITEM_NEW = m_PutOnItems[SLOT_HOLD];
	sITEM_NEW.bVietnamGainItem = true;
	bOk = m_cVietnamInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
	if ( !bOk	 ) return E_FAIL;

	SINVENITEM sINVENITEM = *m_cVietnamInventory.FindPosItem ( wInsertPosX, wInsertPosY );

	//	������ ������ ����.
	RELEASE_HOLD_ITEM ();

	//	[�ڽſ���] �ι꿡 ������ ����.
	GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven_Insert;
	NetMsg_Inven_Insert.bVietnamInven = TRUE;
	NetMsg_Inven_Insert.Data = sINVENITEM;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Insert);

	//	[�ڽſ���] �� �־��� ������ ����.
	GLMSG::SNETPC_PUTON_RELEASE NetMsg_ReleaseHold(SLOT_HOLD);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_ReleaseHold);	

	return S_OK;
}

HRESULT GLChar::MsgReqVNInvenToInven( NET_MSG_GENERIC* nmg )
{

	if ( m_dwVietnamInvenCount <= 0 ) return E_FAIL;

	GLMSG::SNETPC_REQ_VNINVEN_TO_INVEN *pNetMsg = (GLMSG::SNETPC_REQ_VNINVEN_TO_INVEN *)nmg;

	SINVENITEM* pInvenItem(NULL);
	pInvenItem = m_cVietnamInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//	����� �ϴ� �������� �����ϴ�.
	if ( !pInvenItem )											return E_FAIL;
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )												return E_FAIL;

	WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
	WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

	WORD wInsertPosX(0), wInsertPosY(0);
	BOOL bITEM_SPACE(false);
	bITEM_SPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );

	//	�κ��� ���������� ���� ��� ��ҵ�.
	if ( !bITEM_SPACE )		return E_FAIL;

	SITEMCUSTOM sITEM_NEW = pInvenItem->sItemCustom;

	sITEM_NEW.bVietnamGainItem = false;
	m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
	m_cVietnamInventory.DeleteItem( pNetMsg->wPosX, pNetMsg->wPosY );

	SINVENITEM sINVENITEM = *m_cInventory.FindPosItem ( wInsertPosX, wInsertPosY );

	m_dwVietnamInvenCount--;
	GLMSG::SNETPC_INVEN_VIETNAM_ITEMGETNUM_UPDATE NetMsgFB;
	NetMsgFB.dwVietnamInvenCount = m_dwVietnamInvenCount;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	GLMSG::SNETPC_REQ_VNINVEN_TO_INVEN_FB NetMsg;
	NetMsg.wPosX    = pNetMsg->wPosX;
	NetMsg.wPosY    = pNetMsg->wPosY;
	NetMsg.wNewPosX = wInsertPosX;
	NetMsg.wNewPosY = wInsertPosY;
	NetMsg.Data		= sINVENITEM;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);	

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.sNativeID );
	if ( pITEM && pITEM->IsTIMELMT() )
	{

		if ( pITEM->sDrugOp.tTIME_LMT != 0 )
		{
			const CTime cTIME_CUR = CTime::GetCurrentTime();

			CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
			CTime cTIME_LMT(sINVENITEM.sItemCustom.tBORNTIME);
			cTIME_LMT += cSPAN;

			WORD wInsertPosX = sINVENITEM.wPosX, wInsertPosY = sINVENITEM.wPosY;


			if ( cTIME_CUR > cTIME_LMT )
			{
				//	[�ڽſ���] �ش� �������� �ι꿡�� ����.
				GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
				NetMsgInvenDel.wPosX = sINVENITEM.wPosX;
				NetMsgInvenDel.wPosY = sINVENITEM.wPosY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);

				//	�ð� �������� ������ ���� �˸�.
				GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
				NetMsgInvenDelTimeLmt.nidITEM = sINVENITEM.sItemCustom.sNativeID;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

				//	Note : ������ ����.
				//
				m_cInventory.DeleteItem ( sINVENITEM.wPosX, sINVENITEM.wPosY );
			
				//	Note : �ڽ��� ����.
				if ( sINVENITEM.sItemCustom.nidDISGUISE!=SNATIVEID(false) )
				{
					SITEM *pONE = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.nidDISGUISE );
					if ( !pONE )		return E_FAIL;

					SITEMCUSTOM sITEM_NEW;
					sITEM_NEW.sNativeID = sINVENITEM.sItemCustom.nidDISGUISE;
					CTime cTIME = CTime::GetCurrentTime();
					if ( sINVENITEM.sItemCustom.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
					{
						cTIME = CTime(sINVENITEM.sItemCustom.tDISGUISE);

						CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
						cTIME -= tLMT;
					}

					//	Note : ������ ����.
					//
					sITEM_NEW.tBORNTIME = cTIME.GetTime();
					sITEM_NEW.wTurnNum = 1;
					sITEM_NEW.cGenType = EMGEN_BILLING;
					sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
					sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
					sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

					//	Note : �κ��� �� ���� �˻�.
					BOOL bOk = m_cInventory.IsInsertable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
					if ( !bOk )			return E_FAIL;

					//	Note : �κ��� ����.
					m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
					SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

					GLMSG::SNETPC_INVEN_INSERT NetItemInsert;
					NetItemInsert.Data = *pINSERT_ITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemInsert);

					//	Note : �и��� �ڽ��� �α�.
					GLITEMLMT::GetInstance().ReqItemRoute ( pINSERT_ITEM->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pINSERT_ITEM->sItemCustom.wTurnNum );
				}
			}
		}
	}	

	return S_OK;
}

HRESULT GLChar::MsgReqVNGainInvenReset(NET_MSG_GENERIC* nmg)
{
	m_cVietnamInventory.DeleteItemAll();

	return S_OK;
}


HRESULT GLChar::MsgReqInvenSplit ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_INVEN_SPLIT *pNetMsg = (GLMSG::SNETPC_REQ_INVEN_SPLIT *)nmg;
	
	BOOL bOk(FALSE);

	//	Note : �и� ��û ������ ã��.
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )									return E_FAIL;

	//	Note : ������ ���� ��������.
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )													return E_FAIL;

	//	Note : �и� ������ ���������� ����.
	bool bSPLIT(false);
	bSPLIT = ( pItem->ISINSTANCE() );
	if ( !bSPLIT )													return E_FAIL;
	if ( pNetMsg->wSplit < 1 )										return E_FAIL;
	if ( pInvenItem->sItemCustom.wTurnNum <= 1 )					return E_FAIL;
	if ( pInvenItem->sItemCustom.wTurnNum <= pNetMsg->wSplit  )		return E_FAIL;

	//	Note : ���� �������� ��ȭ�� ����.
	WORD wORGIN_SPLIT = pInvenItem->sItemCustom.wTurnNum - pNetMsg->wSplit;

	//	Note : �и��Ǿ� ���� ����� ������.
	SITEMCUSTOM sNEW_ITEM = pInvenItem->sItemCustom;
	sNEW_ITEM.wTurnNum = pNetMsg->wSplit;

	//	Note : ���� ����� ������ ��ġ ã��.
	WORD wInvenPosX(0), wInvenPosY(0);
	bOk = m_cInventory.FindInsrtable ( pItem->sBasicOp.wInvenSizeX, pItem->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
	if ( !bOk )	return E_FAIL;

	//	Note : ���� ���� ������ �ֱ�.
	bOk = m_cInventory.InsertItem ( sNEW_ITEM, wInvenPosX, wInvenPosY );
	if ( !bOk )											return E_FAIL;

	//	Note : ���� ���� ������ ���� ��������.
	SINVENITEM* pNewItem = m_cInventory.GetItem ( wInvenPosX, wInvenPosY );
	if ( !bOk )											return E_FAIL;

	//	Note : ���� �������� ���� ���� ��Ŵ.
	pInvenItem->sItemCustom.wTurnNum = wORGIN_SPLIT;

	//	Note : �κ��� ���� �и��Ǿ� ��� ���� ������.
	GLMSG::SNETPC_INVEN_INSERT NetMsgNew;
	NetMsgNew.Data = *pNewItem;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgNew);

	//	Note : ���� ������ ���� ����.
	GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsgInven;
	NetMsgInven.wPosX = pNetMsg->wPosX;
	NetMsgInven.wPosY = pNetMsg->wPosY;
	NetMsgInven.wTurnNum = wORGIN_SPLIT;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

	return S_OK;
}

HRESULT GLChar::MsgReqHoldToInven ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_HOLD_TO_INVEN *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_HOLD_TO_INVEN*> ( nmg );

#if defined(VN_PARAM) //vietnamtest%%%
	if ( pNetMsg->bUseVietnamInven && m_dwVietnamInvenCount <= 0 ) return E_FAIL;
#endif

	if ( !VALID_HOLD_ITEM() )
	{
		return S_FALSE;
	}

	SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
	if ( !pHoldData )
	{
		return E_FAIL;
	}

	//	�κ� ��ġ.
	WORD wInsertPosX = pNetMsg->wPosX, wInsertPosY = pNetMsg->wPosY;

	// ��ŷ ���� ( �������� ����� 0 �϶� )
	//pHoldData->sBasicOp.wInvenSizeX = 0, pHoldData->sBasicOp.wInvenSizeY = 0;

	BOOL bOk = m_cInventory.IsInsertable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
	if ( !bOk )
	{
		//	�ι��� ��������.
		return E_FAIL;
	}

	//	����ִ� ������ �ι꿡 ����.	
#if defined(VN_PARAM) //vietnamtest%%%   
    SITEMCUSTOM& sCustom = m_PutOnItems[SLOT_HOLD];
	if ( sCustom.bVietnamGainItem && m_dwVietnamInvenCount <= 0 ) return E_FAIL;
	
	sCustom.bVietnamGainItem = false;
#endif	

	bOk = m_cInventory.InsertItem ( GET_HOLD_ITEM(), wInsertPosX, wInsertPosY );
	if ( !bOk	 ) return E_FAIL;

	SINVENITEM sINVENITEM = *m_cInventory.FindPosItem ( wInsertPosX, wInsertPosY );

	//	������ ������ ����.
	RELEASE_HOLD_ITEM ();

#if defined(VN_PARAM) //vietnamtest%%%
	if( pNetMsg->bUseVietnamInven )
	{
		m_dwVietnamInvenCount--;

		GLMSG::SNETPC_INVEN_VIETNAM_ITEMGETNUM_UPDATE NetMsgFB;
		NetMsgFB.dwVietnamInvenCount = m_dwVietnamInvenCount;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
	}
#endif

	//	[�ڽſ���] �ι꿡 ������ ����.
	GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven_Insert;
	NetMsg_Inven_Insert.Data = sINVENITEM;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Insert);

	//	[�ڽſ���] �� �־��� ������ ����.
	GLMSG::SNETPC_PUTON_RELEASE NetMsg_ReleaseHold(SLOT_HOLD);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_ReleaseHold);


	//	��ȿ�Ⱓ �ʰ� ����.
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.sNativeID );
	if ( pITEM && pITEM->IsTIMELMT() )
	{
		if ( pITEM->sDrugOp.tTIME_LMT != 0 )
		{
			const CTime cTIME_CUR = CTime::GetCurrentTime();

			CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
			CTime cTIME_LMT(sINVENITEM.sItemCustom.tBORNTIME);
			cTIME_LMT += cSPAN;

			if ( cTIME_CUR > cTIME_LMT )
			{
				// Ż���� ���
				if ( pITEM->sBasicOp.emItemType == ITEM_VEHICLE && sINVENITEM.sItemCustom.dwVehicleID != 0 )
				{
					GLVEHICLE* pNewVehicle = new GLVEHICLE();
					CGetVehicle* pGetVehicle = new CGetVehicle ( pNewVehicle, 
												 sINVENITEM.sItemCustom.dwVehicleID, 
												 m_dwClientID, 
												 m_dwCharID, 
												 true );
					GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
					if ( pDBMan ) pDBMan->AddJob ( pGetVehicle );
				}

				// ��ī���� ���
				if ( pITEM->sBasicOp.emItemType == ITEM_PET_CARD && sINVENITEM.sItemCustom.dwPetID != 0 )
				{
					CDeletePet* pDbAction = new CDeletePet ( m_dwCharID, sINVENITEM.sItemCustom.dwPetID );
					GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
					if ( pDBMan ) pDBMan->AddJob ( pDbAction );

					// Ȱ�����̸� ������� ���ش�.
					PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
					if ( pMyPet && sINVENITEM.sItemCustom.dwPetID == pMyPet->m_dwPetID )
					{
						// ���� �Ǽ��縮�� �˻��Ѵ�.
						pMyPet->UpdateTimeLmtItem ( this );

						// �־��ٲ� �־��ش�.
						for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
						{
							CItemDrop cDropItem;
							cDropItem.sItemCustom = pMyPet->m_PutOnItems[i];
							if ( IsInsertToInven ( &cDropItem ) ) InsertToInven ( &cDropItem );
							else
							{
								if ( m_pLandMan )
								{
									m_pLandMan->DropItem ( m_vPos, 
														   &(cDropItem.sItemCustom), 
														   EMGROUP_ONE, 
														   m_dwGaeaID );
								}
							}
						}

						GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(pMyPet->m_dwGUID,true,false) );

						// ���� �����Ǹ� ��Ȱ������ Ŭ���̾�Ʈ�� �˷���.
						CGetRestorePetList *pDbAction = new CGetRestorePetList ( m_dwCharID, m_dwClientID );
						if ( pDBMan ) pDBMan->AddJob ( pDbAction );
					}
					else
					{
						// �Ǽ��縮�� üũ�� ���� DB���� �������ش�.
						GLPET* pNewPet = new GLPET ();
						CGetPet* pGetPet = new CGetPet ( pNewPet, 
														 sINVENITEM.sItemCustom.dwPetID, 
														 m_dwClientID, 
														 m_dwCharID,
														 sINVENITEM.wPosX,
														 sINVENITEM.wPosY,
														 true );
						GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
						if ( pDBMan ) pDBMan->AddJob ( pGetPet );
					}
				}

				//	[�ڽſ���] �ش� �������� �ι꿡�� ����.
				GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
				NetMsgInvenDel.wPosX = sINVENITEM.wPosX;
				NetMsgInvenDel.wPosY = sINVENITEM.wPosY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);

				//	�ð� �������� ������ ���� �˸�.
				GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
				NetMsgInvenDelTimeLmt.nidITEM = sINVENITEM.sItemCustom.sNativeID;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

				//	Note : ������ ����.
				//
				m_cInventory.DeleteItem ( sINVENITEM.wPosX, sINVENITEM.wPosY );
			
				//	Note : �ڽ��� ����.
				if ( sINVENITEM.sItemCustom.nidDISGUISE!=SNATIVEID(false) )
				{
					SITEM *pONE = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.nidDISGUISE );
					if ( !pONE )		return E_FAIL;

					SITEMCUSTOM sITEM_NEW;
					sITEM_NEW.sNativeID = sINVENITEM.sItemCustom.nidDISGUISE;
					CTime cTIME = CTime::GetCurrentTime();
					if ( sINVENITEM.sItemCustom.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
					{
						cTIME = CTime(sINVENITEM.sItemCustom.tDISGUISE);

						CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
						cTIME -= tLMT;
					}

					//	Note : ������ ����.
					//
					sITEM_NEW.tBORNTIME = cTIME.GetTime();
					sITEM_NEW.wTurnNum = 1;
					sITEM_NEW.cGenType = EMGEN_BILLING;
					sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
					sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
					sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

					//	Note : �κ��� �� ���� �˻�.
					BOOL bOk = m_cInventory.IsInsertable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
					if ( !bOk )			return E_FAIL;

					//	Note : �κ��� ����.
					m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
					SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

					GLMSG::SNETPC_INVEN_INSERT NetItemInsert;
					NetItemInsert.Data = *pINSERT_ITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemInsert);

					//	Note : �и��� �ڽ��� �α�.
					GLITEMLMT::GetInstance().ReqItemRoute ( pINSERT_ITEM->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pINSERT_ITEM->sItemCustom.wTurnNum );
				}
			}
		}
	}

/*
	// ȹ���� �������� ��ī�� && ������ ���̶�� �������� �����Ѵ�.
	// �־��̵� 0 �� �ƴϸ� ��ī���̸鼭 DB�� ���� ������ ���̴�.
	if ( pHoldData->sBasicOp.emItemType == ITEM_PET_CARD && sINVENITEM.sItemCustom.dwPetID != 0 )
	{
		CExchangePet* pDbAction = new CExchangePet ( m_dwCharID, sINVENITEM.sItemCustom.dwPetID );
		GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
		if ( pDBMan ) pDBMan->AddJob ( pDbAction );
	}
*/


	return S_OK;
}

HRESULT GLChar::MsgReqHoldToField ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_HOLD_TO_FIELD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_HOLD_TO_FIELD*> ( nmg );

	if ( !VALID_HOLD_ITEM () )	return S_FALSE;

	D3DXVECTOR3 vCollisionPos;
	BOOL bCollision = m_pLandMan->IsCollisionNavi
		(
		pNetMsg->vPos + D3DXVECTOR3(0,+5,0),
		pNetMsg->vPos + D3DXVECTOR3(0,-5,0),
		vCollisionPos
		);

	if ( !bCollision )			return S_FALSE;

	const SITEMCUSTOM& sITEMCUSTOM = GET_HOLD_ITEM();

#if defined(VN_PARAM) //vietnamtest%%%
	if ( sITEMCUSTOM.bVietnamGainItem )
	{
		//	������ ����.
		RELEASE_HOLD_ITEM ();

		//	[�ڽſ���] �� �־��� ������ ����.
		GLMSG::SNETPC_PUTON_RELEASE NetMsg_ReleaseHold(SLOT_HOLD);
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_ReleaseHold);

		return S_OK;
	}
#endif



	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sITEMCUSTOM.sNativeID );
	if ( !pITEM )		return false;

	// ������ �������� ��ī�� && ���� Ȱ�������̸� ���� �� ����
	if ( pITEM->sBasicOp.emItemType == ITEM_PET_CARD )
	{
		PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
		if ( pMyPet && pMyPet->IsValid () && sITEMCUSTOM.dwPetID == pMyPet->m_dwPetID ) return FALSE;
	}
	
	//	�ŷ��ɼ�
	if ( !pITEM->sBasicOp.IsTHROW() )						return FALSE;


	// GMCharEdit �� ���� �������� ������ �Ұ����ϴ�.
	if ( sITEMCUSTOM.IsGM_GENITEM() )						return FALSE;

	//	Item Drop �õ�.
	BOOL bDrop = m_pLandMan->DropItem ( vCollisionPos, const_cast<SITEMCUSTOM*> ( &sITEMCUSTOM ) );
	if ( !bDrop )				return S_FALSE;

	//	Note :�������� ���� ���� ��� ���.
	//
	GLITEMLMT::GetInstance().ReqItemRoute ( sITEMCUSTOM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_GROUND, sITEMCUSTOM.wTurnNum );

	//	Drop �� ������ ����.
	RELEASE_HOLD_ITEM ();

	//	[�ڽſ���] �� �־��� ������ ����.
	GLMSG::SNETPC_PUTON_RELEASE NetMsg_ReleaseHold(SLOT_HOLD);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_ReleaseHold);

	return S_OK;
}

BOOL ISBOTH_HANDHELD ( SITEM *pITEM )
{
	GASSERT ( pITEM );
	return ( (pITEM->sSuitOp.emSuit==SUIT_HANDHELD) && pITEM->sSuitOp.IsBOTHHAND() );
}

HRESULT GLChar::MsgReqSlotToHold ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SLOT_TO_HOLD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_SLOT_TO_HOLD*> ( nmg );
	if ( pNetMsg->emSlot >= SLOT_TSIZE )		return S_FALSE;

	if ( VALID_HOLD_ITEM () )					return S_FALSE;
	if ( !VALID_SLOT_ITEM(pNetMsg->emSlot) )	return S_FALSE;
	if ( m_sCONFTING.IsCONFRONTING() )			return S_FALSE;

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	SITEM *pITEM_LEFT=NULL, *pITEM_RIGHT=NULL;
	if ( VALID_SLOT_ITEM(emLHand) )	pITEM_LEFT = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emLHand).sNativeID);
	if ( VALID_SLOT_ITEM(emRHand) )	pITEM_RIGHT = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emRHand).sNativeID);

	//	��� ��� ���� ���� ���� ���.	Ȱ+ȭ��, â+����. ��Ʈ�� �����ϴ� ������� ���� �̶� ��� ��������.
	//
	if ( pNetMsg->emSlot==emRHand )
	{
		if ( (pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_BOW) || 
			(pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_SPEAR) || 
			(pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_DUAL_GUN) || 
			(pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_RAIL_GUN)  || 
			(pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_PORTAL_GUN) )
		{
			if ( pITEM_LEFT )	//	ȭ��, ���� ������ ��.
			{
				SITEMCUSTOM sITEM_LHAND = GET_SLOT_ITEM(emLHand);

				WORD wInvenPosX, wInvenPosY;
				BOOL bOk = m_cInventory.FindInsrtable ( pITEM_LEFT->sBasicOp.wInvenSizeX, pITEM_LEFT->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
				if ( bOk )
				{
					//	���� ������ �ι꿡 ����.
					m_cInventory.InsertItem ( sITEM_LHAND, wInvenPosX, wInvenPosY );

					//	[�ڽſ���] �ι꿡 ������ ����.
					GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven_Insert;
					NetMsg_Inven_Insert.Data = *m_cInventory.GetItem(wInvenPosX, wInvenPosY);
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Insert);

					//	[��ο���] ���÷��� �������� �ƴѰɷ� �����Ͽ� ����.
				}
				//	Note : �κ��丮 �ֱ⿡ �����Ͽ��� ���.
				else
				{
					//	Item Drop �õ�.
					BOOL bDrop = m_pLandMan->DropItem ( m_vPos, &sITEM_LHAND );

					//	Note :�������� ���� ���� ��� ���.
					//
					GLITEMLMT::GetInstance().ReqItemRoute ( sITEM_LHAND, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_GROUND, sITEM_LHAND.wTurnNum );
				}

				//	���� ���� ������.
				RELEASE_SLOT_ITEM(emLHand);

				//	[�ڽſ���] SLOT�� �־��� ������ ����.
				GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release(emLHand);
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);
			}
		}
	}

	if ( pNetMsg->emSlot== SLOT_VEHICLE && m_bGetVehicleFromDB )
	{
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "Vehilce Slot Release Error, m_bGetVehicleFromDB : %d",
															 m_bGetVehicleFromDB );
		return S_FALSE;
	}

	//	�տ� ��� ��.
	HOLD_ITEM ( GET_SLOT_ITEM(pNetMsg->emSlot) );

	//	SLOT ������ ����.
	RELEASE_SLOT_ITEM ( pNetMsg->emSlot );

	//	������ ����� �� �� ȯ�� �ʿ���.
	INIT_DATA ( FALSE, FALSE );

	//	[�ڽſ���] SLOT�� �־��� ������ ����.
	GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release(pNetMsg->emSlot);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);

	//	[��ο���] �ڽ��� ���� ������ �ٲ��� �˸�.
	GLMSG::SNETPC_PUTON_RELEASE_BRD NetMsg_PutOn_Release_Brd;
	NetMsg_PutOn_Release_Brd.dwGaeaID = m_dwGaeaID;
	NetMsg_PutOn_Release_Brd.emSlot = pNetMsg->emSlot;
	SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsg_PutOn_Release_Brd) );

	//	[�ڽſ���] �տ� �������� ���.
	GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
	NetMsg_PutOn.emSlot = SLOT_HOLD;
	NetMsg_PutOn.sItemCustom = GET_HOLD_ITEM();
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);


	if ( pNetMsg->emSlot == SLOT_VEHICLE )
	{
		GLGaeaServer::GetInstance().SaveVehicle( m_dwClientID, m_dwGaeaID, false );
	}

	// ���⿡ ���� ������ �ʱ�ȭ �Ѵ�.
	DisableSkillFact();
	

	return S_OK;
}

HRESULT GLChar::MsgReqSlotExHold ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SLOT_EX_HOLD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_SLOT_EX_HOLD*> ( nmg );
	if ( pNetMsg->emSlot >= SLOT_TSIZE )		return S_FALSE;

	if ( !VALID_HOLD_ITEM () )					return S_FALSE;
	if ( m_sCONFTING.IsCONFRONTING() )			return S_FALSE;

	BOOL bOk(FALSE);

#if defined(VN_PARAM) //vietnamtest%%%
	if ( GET_HOLD_ITEM().bVietnamGainItem )	return S_FALSE;
#endif

	if( GET_HOLD_ITEM().nidDISGUISE!=SNATIVEID(false) )
	{
		//	�ڽ�Ƭ�� ���� �������� ���
		bOk = ACCEPT_ITEM ( GET_HOLD_ITEM().sNativeID, GET_HOLD_ITEM().nidDISGUISE );
		if ( !bOk )	return E_FAIL;
	}
	else
	{
		//	�ڰ����ǿ� �����ϴ��� �˻�.
		bOk = ACCEPT_ITEM ( GET_HOLD_ITEM().sNativeID );
		if ( !bOk )	return E_FAIL;
	}

	//	�ش� ���Կ� ���� �������� �˻�.
	bOk = CHECKSLOT_ITEM ( GET_HOLD_ITEM().sNativeID, pNetMsg->emSlot );
	if ( !bOk )	return E_FAIL;
	
	WORD wInvenPosX, wInvenPosY;
	SITEMCUSTOM sToHoldItem(NATIVEID_NULL());
	SITEMCUSTOM sToInvenItem(NATIVEID_NULL());
	SITEMCUSTOM sToSlotItem(NATIVEID_NULL());

	SITEM *pITEM_TOSLOT = GLItemMan::GetInstance().GetItem(GET_HOLD_ITEM().sNativeID);
	if ( !pITEM_TOSLOT )	return E_FAIL;

	EMSLOT emRHand = GetCurRHand();
	EMSLOT emLHand = GetCurLHand();

	//	Note : �Ϲ� ������ ���.
	//
	if ( pITEM_TOSLOT->sSuitOp.emSuit!=SUIT_HANDHELD )
	{
		if ( !VALID_SLOT_ITEM(pNetMsg->emSlot) )	return S_FALSE;

		//	���� ������ �տ� ��� �ְ�.	#1 ���
		sToHoldItem = GET_SLOT_ITEM ( pNetMsg->emSlot );

		//	������ ������ ����.
		sToSlotItem = GET_HOLD_ITEM();
	}
	//	�տ� ��� ������ ���.
	else
	{
		SITEM *pITEM_LEFT=NULL, *pITEM_RIGHT=NULL;
		if ( VALID_SLOT_ITEM(emLHand) )	pITEM_LEFT = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emLHand).sNativeID);
		if ( VALID_SLOT_ITEM(emRHand) )	pITEM_RIGHT = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emRHand).sNativeID);

		//	��� ������ �����Ϸ� �ϴ� ���.
		if ( ISBOTH_HANDHELD(pITEM_TOSLOT) )
		{
			if ( pITEM_LEFT && pITEM_RIGHT )
			{
				if( ( (pITEM_TOSLOT->sSuitOp.emAttack==ITEMATT_BOW) && (pITEM_LEFT->sBasicOp.emItemType!=ITEM_ARROW) ) ||
					( (pITEM_TOSLOT->sSuitOp.emAttack==ITEMATT_DUAL_GUN) && (pITEM_LEFT->sBasicOp.emItemType!=ITEM_BULLET) ) ||
					( (pITEM_TOSLOT->sSuitOp.emAttack==ITEMATT_RAIL_GUN) && (pITEM_LEFT->sBasicOp.emItemType!=ITEM_BULLET) ) ||
					( (pITEM_TOSLOT->sSuitOp.emAttack==ITEMATT_PORTAL_GUN) && (pITEM_LEFT->sBasicOp.emItemType!=ITEM_BULLET) ) ||
					( (pITEM_TOSLOT->sSuitOp.emAttack==ITEMATT_SPEAR) && (pITEM_LEFT->sBasicOp.emItemType!=ITEM_CHARM) ) ||
					( (pITEM_TOSLOT->sSuitOp.emAttack!=ITEMATT_BOW) && 
					(pITEM_TOSLOT->sSuitOp.emAttack!=ITEMATT_SPEAR) && 
					 (pITEM_TOSLOT->sSuitOp.emAttack!=ITEMATT_DUAL_GUN) && 
					 (pITEM_TOSLOT->sSuitOp.emAttack!=ITEMATT_RAIL_GUN) && 
					 (pITEM_TOSLOT->sSuitOp.emAttack!=ITEMATT_PORTAL_GUN) ) )
				{
					bOk = m_cInventory.FindInsrtable ( pITEM_LEFT->sBasicOp.wInvenSizeX, pITEM_LEFT->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
					if ( !bOk )	return E_FAIL;

					//	���� ������ �ι꿡 ����.	#1 ���		//	���� ���� ������.
					sToInvenItem = GET_SLOT_ITEM(emLHand);
				}

				//	���� ������ �տ� ��� �ְ�.	#2 ���
				sToHoldItem = GET_SLOT_ITEM(emRHand);

				//	������ ������ ����.
				sToSlotItem = GET_HOLD_ITEM();
			}
			else
			{
				if ( !VALID_SLOT_ITEM(pNetMsg->emSlot) )	return S_FALSE;

				//	�տ� ���.
				sToHoldItem = GET_SLOT_ITEM(pNetMsg->emSlot);

				//	������ ������ ������ ����.
				sToSlotItem = GET_HOLD_ITEM();
			}
		}
		//	�Ѽ� ������ �����Ϸ� �ϴ� ���.
		else
		{
			//	Note : �����ҷ��ϴ� ������ ȭ���̳� ������ ���.
			//
			if ( (pITEM_TOSLOT->sBasicOp.emItemType==ITEM_ARROW) || (pITEM_TOSLOT->sBasicOp.emItemType==ITEM_CHARM) || (pITEM_TOSLOT->sBasicOp.emItemType==ITEM_BULLET) )
			{
				//	�޼տ��� ���� ����.
				if ( pNetMsg->emSlot != emLHand )
					return E_FAIL;

				//	Note : ��չ����̰� Ȱ�̳� ȭ���� ��츸 ���� ����.
				//
				if ( pITEM_RIGHT && ISBOTH_HANDHELD(pITEM_RIGHT) )
				{
					if ( (pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_BOW) && (pITEM_TOSLOT->sBasicOp.emItemType!=ITEM_ARROW) )		return E_FAIL;
					if ( (pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_DUAL_GUN) && (pITEM_TOSLOT->sBasicOp.emItemType!=ITEM_BULLET) )		return E_FAIL;
					if ( (pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_RAIL_GUN) && (pITEM_TOSLOT->sBasicOp.emItemType!=ITEM_BULLET) )		return E_FAIL;
					if ( (pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_PORTAL_GUN) && (pITEM_TOSLOT->sBasicOp.emItemType!=ITEM_BULLET) )		return E_FAIL;
					if ( (pITEM_RIGHT->sSuitOp.emAttack==ITEMATT_SPEAR) && (pITEM_TOSLOT->sBasicOp.emItemType!=ITEM_CHARM) )	return E_FAIL;
				}
				else	return E_FAIL;

				if ( !VALID_SLOT_ITEM(pNetMsg->emSlot) )	return S_FALSE;

				//	���� ������ �տ� ��� �ְ�.	#1 ���
				sToHoldItem = GET_SLOT_ITEM(pNetMsg->emSlot);

				//	������ ������ ����.
				sToSlotItem = GET_HOLD_ITEM();
			}
			else 
			{
				//	���� �������� ��շ��� ���.
				if ( pITEM_RIGHT && ISBOTH_HANDHELD(pITEM_RIGHT) )
				{
					//	��� ��� ���� ���� ���� ���.	Ȱ+ȭ��, â+����. ��Ʈ�� �����ϴ� ������� ���� �̶� ��� ��������.
					//
					if ( pITEM_LEFT )
					{
						bOk = m_cInventory.FindInsrtable ( pITEM_LEFT->sBasicOp.wInvenSizeX, pITEM_LEFT->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
						if ( !bOk )	return E_FAIL;

						//	���� ������ �ι꿡 ����. #1 ���	//	���� ���� ������.
						sToInvenItem = GET_SLOT_ITEM(emLHand);
					}

					//	���� ������ �տ� ��� �ְ�.	#2 ���
					sToHoldItem = GET_SLOT_ITEM(emRHand);

					//	������ ������ ����.
					sToSlotItem = GET_HOLD_ITEM();
				}
				//	�Ϲ����� ���.
				else
				{
					if ( !VALID_SLOT_ITEM(pNetMsg->emSlot) )	return S_FALSE;

					//	���� ������ �տ� ��� �ְ�.	#1 ���
					sToHoldItem = GET_SLOT_ITEM(pNetMsg->emSlot);

					//	������ ������ ����.
					sToSlotItem = GET_HOLD_ITEM();
				}
			}
		}
	}

	//	Note : ������ �̵� ��Ŵ, �޽��� �߻�
	//

	//	Note : �κ��� ������ ������.
	//
	if ( sToInvenItem.sNativeID != NATIVEID_NULL() )
	{
		m_cInventory.InsertItem ( sToInvenItem, wInvenPosX, wInvenPosY );

		//	[�ڽſ���] �ι꿡 ������ ����.
		GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven_Insert;
		NetMsg_Inven_Insert.Data = *m_cInventory.GetItem(wInvenPosX, wInvenPosY);
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Insert);		
	}

	//	Note : �տ����.
	//

	if ( sToHoldItem.sNativeID != NATIVEID_NULL() )
	{
		HOLD_ITEM ( sToHoldItem );

		//	[�ڽſ���] �տ� �������� ���.
		GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
		NetMsg_PutOn.emSlot = SLOT_HOLD;
		NetMsg_PutOn.sItemCustom = sToHoldItem;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);

		// ���⿡ ���� ������ �ʱ�ȭ �Ѵ�.
		DisableSkillFact();
	}

	if ( pNetMsg->emSlot == SLOT_VEHICLE )
	{
		GLGaeaServer::GetInstance().SaveVehicle( m_dwClientID, m_dwGaeaID, false );		
	}

	//	Note : ���Կ� �ֱ�. ���Կ��� ���� ������ ������ ����. (�κ��� ������ ������.)
	//
	if ( sToSlotItem.sNativeID != NATIVEID_NULL() )
	{
		//	Note : ���Կ��� ������ �������� ���� ���.
		//
		EMSLOT emSlotRelease(SLOT_TSIZE);
		if ( sToInvenItem.sNativeID!=NATIVEID_NULL() )
		{
			emSlotRelease = emLHand;
			RELEASE_SLOT_ITEM(emLHand);		//	���� ���� ������.
		}

		//	���Կ� ����.
		SLOT_ITEM ( sToSlotItem, pNetMsg->emSlot );

		//	[�ڽſ���] SLOT �� �������� ����.
		//
		GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
		NetMsg_PutOn.emSlot = pNetMsg->emSlot;
		NetMsg_PutOn.sItemCustom = sToSlotItem;
		NetMsg_PutOn.emSlotRelease = emSlotRelease;					//	������ ������ ����.
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);

		//	[��ο���] �ڽ��� ���� ������ �ٲ��� �˸�.
		//
		GLMSG::SNETPC_PUTON_UPDATE_BRD NetMsg_PutOn_Update_Brd;
		NetMsg_PutOn_Update_Brd.dwGaeaID = m_dwGaeaID;
		NetMsg_PutOn_Update_Brd.emSlot = pNetMsg->emSlot;
		NetMsg_PutOn_Update_Brd.sItemClient.Assign ( sToSlotItem );
		NetMsg_PutOn_Update_Brd.emSlotRelease = emSlotRelease;		//	������ ������ ����.
		SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsg_PutOn_Update_Brd) );

		// ���⿡ ���� ������ �ʱ�ȭ �Ѵ�.
		DisableSkillFact();
	}

	//	Note : ������ ����� �� �� ȯ�� �ʿ���.
	//
	INIT_DATA ( FALSE, FALSE );

	return S_OK;
}

HRESULT GLChar::MsgReqHoldToSlot ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_HOLD_TO_SLOT *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_HOLD_TO_SLOT*> ( nmg );
	if ( pNetMsg->emSlot >= SLOT_TSIZE )	return S_FALSE;

	if ( !VALID_HOLD_ITEM () )				return S_FALSE;
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( GET_HOLD_ITEM().sNativeID );
	if ( !pItem )							return S_FALSE;
	if ( m_sCONFTING.IsCONFRONTING() )		return S_FALSE;

	const SITEMCUSTOM sItemCustom = GET_HOLD_ITEM ();

	BOOL bOk(FALSE);

#if defined(VN_PARAM) //vietnamtest%%%
	if ( sItemCustom.bVietnamGainItem )	return S_FALSE;
#endif
	
	if( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
	{
		//	�ڽ�Ƭ�� ���� �������� ���
		bOk = ACCEPT_ITEM ( sItemCustom.sNativeID, sItemCustom.nidDISGUISE );
		if ( !bOk )	return E_FAIL;
	}
	else
	{
		//	�ڰ����ǿ� �����ϴ��� �˻�.
		bOk = ACCEPT_ITEM ( sItemCustom.sNativeID );
		if ( !bOk )	return E_FAIL;
	}
	
	//	�ش� ���Կ� ���� �������� �˻�.
	bOk = CHECKSLOT_ITEM ( sItemCustom.sNativeID, pNetMsg->emSlot );
	if ( !bOk )	return E_FAIL;

	//	�ش� ������ �񿴴��� ����.
	bOk = ISEMPTY_SLOT ( sItemCustom.sNativeID, pNetMsg->emSlot );
	if ( !bOk ) return E_FAIL;

	//	���� ��� ������ ��� ������ "������ ���" ��������.
	if ( ISBOTH_HANDHELD(pItem) )	pNetMsg->emSlot = GetCurRHand();

	//	�ش� ���Կ� ������ ����.
	SLOT_ITEM ( sItemCustom, pNetMsg->emSlot );

	//	�տ� �� ������ ����
	RELEASE_HOLD_ITEM ();

	//	������ ����� �� �� ȯ�� �ʿ���.
	INIT_DATA ( FALSE, FALSE );

	//	[�ڽſ���] �տ� �־��� ������ ����.
	GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release(SLOT_HOLD);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);

	//	[�ڽſ���] �տ� �־��� �������� Slot�� ����.
	GLMSG::SNETPC_PUTON_UPDATE NetMsg_PutOn;
	NetMsg_PutOn.emSlot = pNetMsg->emSlot;
	NetMsg_PutOn.sItemCustom = sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);

	//	[��ο���] ������ �ٲ��� �˷���.
	GLMSG::SNETPC_PUTON_UPDATE_BRD NetMsg_PutOn_Brd;
	NetMsg_PutOn_Brd.dwGaeaID = m_dwGaeaID;
	NetMsg_PutOn_Brd.emSlot = pNetMsg->emSlot;
	NetMsg_PutOn_Brd.sItemClient.Assign ( sItemCustom );
	SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsg_PutOn_Brd) );

	// ���⿡ ���� ������ �ʱ�ȭ �Ѵ�.
	DisableSkillFact();

	return S_OK;
}


HRESULT GLChar::MsgReqSlotChange ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SLOT_CHANGE *pNetMsg = reinterpret_cast<GLMSG::SNETPC_REQ_SLOT_CHANGE*> ( nmg );

	if( IsUseArmSub() ) SetUseArmSub( FALSE ); // �ֹ���
	else				SetUseArmSub( TRUE );  // ��������

	//	������ ����� �� �� ȯ�� �ʿ���.
	INIT_DATA ( FALSE, FALSE, m_sCONFTING.sOption.fHP_RATE );

	//	[�ڽſ���] ���� ����
	GLMSG::SNETPC_PUTON_CHANGE NetMsg_PutOn;
	NetMsg_PutOn.bUseArmSub = IsUseArmSub();
	NetMsg_PutOn.fCONFT_HP_RATE = m_sCONFTING.sOption.fHP_RATE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn);

	//	[��ο���] ���� ������ �˸�.
	GLMSG::SNETPC_PUTON_CHANGE_BRD NetMsg_PutOn_Brd;
	NetMsg_PutOn_Brd.dwGaeaID = m_dwGaeaID;
	NetMsg_PutOn_Brd.bUseArmSub = IsUseArmSub();
	SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsg_PutOn_Brd) );

	GLMSG::SNETPC_PUTON_CHANGE_AG NetMsg_PutOn_AG;
	NetMsg_PutOn_AG.bUseArmSub = IsUseArmSub();
	GLGaeaServer::GetInstance().SENDTOAGENT ( m_dwClientID, &NetMsg_PutOn_AG );


	// ���⿡ ���� ������ �ʱ�ȭ �Ѵ�.
	DisableSkillFact();
	
	return S_OK;
}

bool GLChar::InsertPileItem ( const SITEMCUSTOM& sItemCustom, WORD wREQINSRTNUM )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
	if ( !pItem )				return false;
	if ( !pItem->ISPILE() )		return false;

	WORD wPILENUM = pItem->sDrugOp.wPileNum;
	WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
	WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

	//	Note : �� �������� �ִ� �������� �ִ��� �˻��Ͽ� �κ��� �����ϴ�
	//		������ ���� ���ڸ� ���ҽ�Ŵ.
	GLInventory::CELL_MAP &ItemMap = *m_cInventory.GetItemList();
	GLInventory::CELL_MAP_ITER iter = ItemMap.begin();
	for ( ; iter!=ItemMap.end(); ++iter )
	{
		SINVENITEM &sINVENITEM = *(*iter).second;
		SITEMCUSTOM &sITEMCUSTOM = sINVENITEM.sItemCustom;
		if ( sITEMCUSTOM.sNativeID != sItemCustom.sNativeID )	continue;
		if ( sITEMCUSTOM.wTurnNum>=wPILENUM )		continue;
	
		//	���� ������ ��ħ ������.
		WORD wSURPLUSNUM = wPILENUM - sITEMCUSTOM.wTurnNum;

		if ( wREQINSRTNUM > wSURPLUSNUM )
		{
			//	Note : ���� �������� ��ħ�� ����. ( ���� )
			sITEMCUSTOM.wTurnNum = wPILENUM;

			//	Note : Ŭ���̾�Ʈ�� ���� ������ �˸�.
			GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
			NetMsg.wPosX = sINVENITEM.wPosX;
			NetMsg.wPosY = sINVENITEM.wPosY;
			NetMsg.wTurnNum = sITEMCUSTOM.wTurnNum;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

			//	Note : �ܿ� ���� ����.
			wREQINSRTNUM -= wSURPLUSNUM;
		}
		else
		{
			//	Note : ���� �����ۿ� ��ġ�� �� �����ε� ���� ���� �������� ����� ��.
			sITEMCUSTOM.wTurnNum += wREQINSRTNUM;

			//	Note : Ŭ���̾�Ʈ�� ���� ������ �˸�.
			GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
			NetMsg.wPosX = sINVENITEM.wPosX;
			NetMsg.wPosY = sINVENITEM.wPosY;
			NetMsg.wTurnNum = sITEMCUSTOM.wTurnNum;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

			//	�ݾ� ��ȭ �޽���.
			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			//add money 2
			NetMsgMoney.lnVoteP = m_lnVoteP;
			//add money 3
			NetMsgMoney.lnPremP = m_lnPremP;
			//add contributionpoints by CNDev
			NetMsgMoney.lnContribP = m_lnContribP;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgMoney);

			return true;
		}
	}

	//	Note : ������ �κ��� ������ �������� ���� �ľ��� �κ��� �� ������ �ִ��� �˻�.

	//	�ִ��ħ������ ����.
	WORD wONENUM = wREQINSRTNUM / wPILENUM;
	WORD wITEMNUM = wONENUM;

	//	���а�ħ�������� ��ħ��.
	WORD wSPLITNUM = wREQINSRTNUM % wPILENUM;
	if ( wSPLITNUM > 0 )				wITEMNUM += 1;
	if ( wSPLITNUM==0 && wITEMNUM>=1 )	wSPLITNUM = wPILENUM;

	for ( WORD i=0; i<wITEMNUM; ++i )
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		BOOL bITEM_SPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );
		GASSERT(bITEM_SPACE&&"���� ������ �̸� üũ�� �ϰ� �������� �־����� ������ ������.");

		//	Note : ���ο� �������� �־���.
		//
		SITEMCUSTOM sITEMCUSTOM(sItemCustom.sNativeID);
		sITEMCUSTOM.tBORNTIME = sItemCustom.tBORNTIME;

		sITEMCUSTOM.cGenType = sItemCustom.cGenType;
		sITEMCUSTOM.cChnID = sItemCustom.cChnID;
		sITEMCUSTOM.cFieldID = sItemCustom.cFieldID;

		if ( wITEMNUM==(i+1) )	sITEMCUSTOM.wTurnNum = wSPLITNUM;	//	������ �������� �ܿ���.
		else					sITEMCUSTOM.wTurnNum = wPILENUM;	//	�ƴ� ���� ������.

		m_cInventory.InsertItem ( sITEMCUSTOM, wInsertPosX, wInsertPosY );	//	�ι��丮�� �ֽ��ϴ�.

		//	�ι꿡 ������ �־��ִ� �޽���.
		GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
		NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);
	}

	return true;
}

bool GLChar::DeletePileItem ( SNATIVEID sNID, WORD wREQDELNUM )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNID );
	if ( !pItem )				return false;
	if ( !pItem->ISPILE() )		return false;

	WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
	WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

	std::vector<SNATIVEID>	vecDEL;

	//	Note : �� �������� �ִ� �������� �ִ��� �˻��Ͽ� �κ��� �����ϴ�
	//		������ ���� ���ڸ� ���ҽ�Ŵ.
	GLInventory::CELL_MAP &ItemMap = *m_cInventory.GetItemList();
	GLInventory::CELL_MAP_ITER iter = ItemMap.begin();
	for ( ; iter!=ItemMap.end(); ++iter )
	{
		if ( wREQDELNUM==0 )						break;

		SINVENITEM &sINVENITEM = *(*iter).second;
		SITEMCUSTOM &sITEMCUSTOM = sINVENITEM.sItemCustom;
		if ( sITEMCUSTOM.sNativeID != sNID )		continue;

		if ( sITEMCUSTOM.wTurnNum>wREQDELNUM )
		{
			sITEMCUSTOM.wTurnNum -= wREQDELNUM;
			wREQDELNUM = 0;

			//	Note : �������� �޽���.
			GLMSG::SNETPC_INVEN_DRUG_UPDATE	NetMsgUpdate;
			NetMsgUpdate.wPosX = sINVENITEM.wPosX;
			NetMsgUpdate.wPosY = sINVENITEM.wPosY;
			NetMsgUpdate.wTurnNum = sITEMCUSTOM.wTurnNum;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgUpdate);
		}
		else
		{
			wREQDELNUM -= sITEMCUSTOM.wTurnNum;
			sITEMCUSTOM.wTurnNum = 0;

			vecDEL.push_back ( SNATIVEID(sINVENITEM.wPosX,sINVENITEM.wPosY) );
		}
	}

	if ( !vecDEL.size() )	return true;

	for ( DWORD i=0; i<vecDEL.size(); ++i )
	{
		m_cInventory.DeleteItem ( vecDEL[i].wMainID, vecDEL[i].wSubID );

		//	Note : ���� �޽���.
		GLMSG::SNETPC_INVEN_DELETE NetMsgDelete;
		NetMsgDelete.wPosX = vecDEL[i].wMainID;
		NetMsgDelete.wPosY = vecDEL[i].wSubID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgDelete);
	}

	return true;
}

// *****************************************************
// Desc: NPC �κ��� ������ ����
// *****************************************************
HRESULT GLChar::MsgReqBuyFromNpc ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_BUY_FROM_NPC *pNetMsg = (GLMSG::SNETPC_REQ_BUY_FROM_NPC *) nmg;

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pNetMsg->sNID );

	DxConsoleMsg* pConsoleMsg = GLGaeaServer::GetInstance().GetConsoleMsg();

	if ( !m_pLandMan )
	{
		return E_FAIL;
	}

	// ���� NPC ��ȿ�� üũ
	PGLCROW pCrow = m_pLandMan->GetCrow ( pNetMsg->dwNPCID );
	if ( !pCrow )					return E_FAIL;

	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-pCrow->GetPosition()) );
	float fTalkRange = (float) (pCrow->GetBodyRadius() + GETBODYRADIUS() + 30);
	float fTalkableDis = fTalkRange + 20;

	if ( fDist>fTalkableDis )		return E_FAIL;
	

	if ( !pCrowData )
	{
		//	���� NPC�� �������� �ʽ��ϴ�.
		return E_FAIL;
	}

	if ( !pCrowData->m_sNpcTalkDlg.IsMARKET() )
	{
		//	������ �ƴմϴ�.
		return E_FAIL;
	}

	if ( pCrowData->GetSaleNum() <= pNetMsg->dwChannel )		return E_FAIL;

	GLInventory *pInven = pCrowData->GetSaleInven(pNetMsg->dwChannel);
	if ( !pInven )												return E_FAIL;

	SINVENITEM* pSaleItem(NULL);
	pSaleItem = pInven->GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//	����� �ϴ� �������� �����ϴ�.
	if ( !pSaleItem )											return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pSaleItem->sItemCustom.sNativeID );
	if ( !pItem )												return E_FAIL;

	volatile float fSHOP_RATE = GET_PK_SHOP2BUY();
	if ( fSHOP_RATE==0.0f )										return E_FAIL;

	volatile float fSHOP_2RATE = fSHOP_RATE + m_pLandMan->m_fCommissionRate;
	volatile float fSHOP_2RATE_C = fSHOP_2RATE * 0.01f;


	LONGLONG dwPrice = pCrowData->GetNpcSellPrice(pItem->sBasicOp.sNativeID.dwID);
	volatile LONGLONG llPRICE2;

	//add money 2
	//no tax for vote item
	if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
	{
		llPRICE2= pItem->sBasicOp.dwBuyPrice;
	}
	//add money 3
	//no tax for premium item
	else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
	{
		llPRICE2= pItem->sBasicOp.dwBuyPrice;
	}
	//add contributionpoint by CNDev
	//no tax for contribution item
	else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
	{
		llPRICE2= pItem->sBasicOp.dwBuyPrice;
	}
	else
	{
		if( dwPrice == 0 )
		{
			dwPrice = pItem->sBasicOp.dwBuyPrice;
			llPRICE2 = (LONGLONG)(dwPrice * fSHOP_2RATE_C);
		}
		else
		{
		llPRICE2 = dwPrice;
		}
	}

	volatile LONGLONG llPRICE3 = (LONGLONG)(llPRICE2*pNetMsg->wBuyNum);

	//add money 2
	//compare price to votepoint
	if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) 
	{
		if ( m_lnVoteP < llPRICE3 )
		{
		return E_FAIL;
		}
	}
	//add money 3
	//compare price to premium point
	else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) 
	{
		if ( m_lnPremP < llPRICE3 )
		{
		return E_FAIL;
		}
	}
	//add contributionpoint by CNDev
	else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
	{
		if ( m_lnContribP < llPRICE3 )
		{
			return E_FAIL;
		}
	}
	//compare price to gold
	else
	{
		if ( m_lnMoney < llPRICE3 )
		{
		return E_FAIL;
		}
	}
	

	WORD wINVENX = pItem->sBasicOp.wInvenSizeX;
	WORD wINVENY = pItem->sBasicOp.wInvenSizeY;

	//	Note : �κ��� ���� ������ �ִ��� �˻�.
	//
	BOOL bITEM_SPACE(FALSE);
	if ( pItem->ISPILE() )
	{
		//	��ħ �������� ���.
		WORD wPILENUM = pItem->sDrugOp.wPileNum;
		SNATIVEID sNID = pItem->sBasicOp.sNativeID;

		//	�ֱ� ��û�� �����ۼ�. ( �ܿ���. )
		WORD wREQINSRTNUM = ( pNetMsg->wBuyNum * pItem->GETAPPLYNUM() );

		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, wINVENX, wINVENY );

		//	�κ��� ���������� ���� ��� ��ҵ�.
		if ( !bITEM_SPACE )		return E_FAIL;

		//	Note :�������� ���� ���� ��� ���.
		//
		SITEMCUSTOM sDB_ITEM(sNID);
		sDB_ITEM.cGenType = EMGEN_SHOP;
		sDB_ITEM.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sDB_ITEM.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		sDB_ITEM.wTurnNum = wPILENUM;
		sDB_ITEM.tBORNTIME = CTime::GetCurrentTime().GetTime();


		//add money 2
		//log vote item purchase to db
		if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) 
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( sDB_ITEM, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR_VOTE, sDB_ITEM.wTurnNum );
		}
		//add money 3
		//log premium item purchase to db
		else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) 
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( sDB_ITEM, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR_PREMIUM, sDB_ITEM.wTurnNum );
		}
		//add cotnributionpoint by CNDev
		else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( sDB_ITEM, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR_CONTRIBUTION, sDB_ITEM.wTurnNum );
		}
		else
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( sDB_ITEM, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, sDB_ITEM.wTurnNum );
		}
		//add money 3
		//record vote item purchase 
		if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) 
		{
			CheckMoneyUpdate( m_lnVoteP, llPRICE3, FALSE, "Buy From Npc." );
			m_bMoneyUpdate = TRUE;
			m_lnVoteP -= llPRICE3;	

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_VOTE_PURCHASE.txt", "%s	BUY MILTIPLE VOTE ITEM [CharID: %d][CharName:%s][Point Spent:%I64d][Money Left:%I64d][ITEM MID:%d SID:%d][Number: %d] ",
			strTIME.c_str(), m_dwCharID, m_szName, llPRICE3, m_lnVoteP, sDB_ITEM.sNativeID.wMainID, sDB_ITEM.sNativeID.wSubID,pNetMsg->wBuyNum );
		}
		//add money 2
		//record premium item purchase
		else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) 
		{
			CheckMoneyUpdate( m_lnPremP, llPRICE3, FALSE, "Buy From Npc." );
			m_bMoneyUpdate = TRUE;
			m_lnPremP -= llPRICE3;	

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_PREMIUM_PURCHASE.txt", "%s	BUY MILTIPLE PREMIUM ITEM [CharID: %d][CharName:%s][Point Spent:%I64d][Money Left:%I64d][ITEM MID:%d SID:%d][Number: %d] ",
			strTIME.c_str(), m_dwCharID, m_szName, llPRICE3, m_lnPremP, sDB_ITEM.sNativeID.wMainID, sDB_ITEM.sNativeID.wSubID,pNetMsg->wBuyNum );
		}
		//add contributionpoint by CNDev
		else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
		{
			CheckMoneyUpdate( m_lnContribP, llPRICE3, FALSE, "Buy From Npc." );
			m_bMoneyUpdate = TRUE;
			m_lnContribP -= llPRICE3;

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_CONTRIBUTION_PURCHASE.txt", "%s	BUY MILTIPLE CONTRIBUTION ITEM [CharID: %d][CharName:%s][Point Spent:%I64d][Money Left:%I64d][ITEM MID:%d SID:%d][Number: %d] ",
			strTIME.c_str(), m_dwCharID, m_szName, llPRICE3, m_lnContribP, sDB_ITEM.sNativeID.wMainID, sDB_ITEM.sNativeID.wSubID,pNetMsg->wBuyNum );
		}
		else
		{
			CheckMoneyUpdate( m_lnMoney, llPRICE3, FALSE, "Buy From Npc." );
			m_bMoneyUpdate = TRUE;
			m_lnMoney -= llPRICE3;	
		}

		//add money 2 modified
		if ( llPRICE3>EMMONEY_LOG )
		{
			if ( !(pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM || pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM || pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM)  ) 
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE3), EMITEM_ROUTE_DELETE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
			}
		}

		if ( llPRICE3>EMMONEY_LOG_PREMIUM )
		{		
			if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) 
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE3), EMITEM_ROUTE_DELETE_VOTE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVoteP, EMITEM_ROUTE_CHAR_VOTE );
			}
			else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) 
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE3), EMITEM_ROUTE_DELETE_PREMIUM );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnPremP, EMITEM_ROUTE_CHAR_PREMIUM );
			}
			//add contributionpoint by CNDev
			else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE3), EMITEM_ROUTE_DELETE_CONTRIBUTION );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnContribP, EMITEM_ROUTE_CHAR_CONTRIBUTION );
			}
		}

//#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if ( m_bTracingUser )
		{
			NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
			TracingMsg.nUserNum  = GetUserID();
			StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );

			CString strTemp;
			strTemp.Format( "Buy From Npc, [%s][%s], Buy price:[%I64d], Have Money:[%I64d]",
					 m_szUID, m_szName,  llPRICE3, m_lnMoney );

			StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

			m_pGLGaeaServer->SENDTOAGENT( m_dwClientID, &TracingMsg );
		}
//#endif

		//	Note : �������� �κ��� �־��ش�.
		//
		InsertPileItem ( sDB_ITEM, wREQINSRTNUM );
	}
	else
	{
		//	�Ϲ� �������� ���.
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );

		//	�κ��� ���������� ���� ��� ��ҵ�.
		if ( !bITEM_SPACE )		return E_FAIL;

		//	Note : ���ο� �������� �����մϴ�.
		//
		SNATIVEID sNID = pSaleItem->sItemCustom.sNativeID;
		SITEMCUSTOM sITEM_NEW = pSaleItem->sItemCustom;
		CTime cTIME = CTime::GetCurrentTime();
		sITEM_NEW.tBORNTIME = cTIME.GetTime();

		sITEM_NEW.wTurnNum = 1;
		sITEM_NEW.cGenType = (BYTE) EMGEN_SHOP;
		sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sITEM_NEW.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID();
		sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sNID, EMGEN_SHOP );

		//add ownership
		//generate ownership ID
		sITEM_NEW.dwOwnerID = m_dwCharID;

		//	���� �ɼ� �߻�.
		//
		//if( sITEM_NEW.GENERATE_RANDOM_OPT () )
		//{
		//	GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
		//}

		//	Note : ���� �����մϴ�.
		//
		
		//add money 2
		//record vote item purchase
		if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) 
		{
			CheckMoneyUpdate( m_lnVoteP, llPRICE2, FALSE, "Buy From Npc." );
			m_bMoneyUpdate = TRUE;
			m_lnVoteP -= llPRICE2;	

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_VOTE_PURCHASE.txt", "%s	BUY SINGLE VOTE ITEM [CharID: %d][CharName:%s][Point Spent:%I64d][Money Left:%I64d][ITEM MID:%d SID:%d] ",
			strTIME.c_str(), m_dwCharID, m_szName, llPRICE3, m_lnVoteP, sITEM_NEW.sNativeID.wMainID, sITEM_NEW.sNativeID.wSubID );

			//test add regenerate random option
			if( sITEM_NEW.GENERATE_RANDOM_OPT () )
			{
				GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
			}
		}
		//add money 3
		//record premium item purchase
		else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) 
		{
			CheckMoneyUpdate( m_lnPremP, llPRICE2, FALSE, "Buy From Npc." );
			m_bMoneyUpdate = TRUE;
			m_lnPremP -= llPRICE2;	

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_PREMIUM_PURCHASE.txt", "%s	BUY SINGLE PREMIUM ITEM [CharID: %d][CharName:%s][Point Spent:%I64d][Money Left:%I64d][ITEM MID:%d SID:%d] ",
			strTIME.c_str(), m_dwCharID, m_szName, llPRICE3, m_lnPremP, sITEM_NEW.sNativeID.wMainID, sITEM_NEW.sNativeID.wSubID );

			//test add regenerate random option
			if( sITEM_NEW.GENERATE_RANDOM_OPT () )
			{
				GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
			}
		}
		//add contributionpoint by CNDev
		else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM ) 
		{
			CheckMoneyUpdate( m_lnContribP, llPRICE2, FALSE, "Buy From Npc." );
			m_bMoneyUpdate = TRUE;
			m_lnContribP -= llPRICE2;	

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_PREMIUM_CONTRIBUTION.txt", "%s	BUY SINGLE CONTRIBUTION ITEM [CharID: %d][CharName:%s][Point Spent:%I64d][Money Left:%I64d][ITEM MID:%d SID:%d] ",
			strTIME.c_str(), m_dwCharID, m_szName, llPRICE3, m_lnContribP, sITEM_NEW.sNativeID.wMainID, sITEM_NEW.sNativeID.wSubID );

			//test add regenerate random option
			if( sITEM_NEW.GENERATE_RANDOM_OPT () )
			{
				GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
			}
		}
		else
		{
			CheckMoneyUpdate( m_lnMoney, llPRICE2, FALSE, "Buy From Npc." );
			m_bMoneyUpdate = TRUE;
			m_lnMoney -= llPRICE2;	
		}


		//add money 2
		//add contributionpoint by CNDev
		if ( llPRICE3>EMMONEY_LOG )
		{
			if ( !(pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM || pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM || pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM) ) 
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE2), EMITEM_ROUTE_DELETE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
			}
		}

		if ( llPRICE3>EMMONEY_LOG_PREMIUM )
		{		
			if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) 
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE2), EMITEM_ROUTE_DELETE_VOTE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVoteP, EMITEM_ROUTE_CHAR_VOTE );
			}
			else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) 
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE2), EMITEM_ROUTE_DELETE_PREMIUM );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnPremP, EMITEM_ROUTE_CHAR_PREMIUM );
			}
			//add contributionpoint by CNDev
			else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM ) 
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE2), EMITEM_ROUTE_DELETE_CONTRIBUTION );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnContribP, EMITEM_ROUTE_CHAR_CONTRIBUTION );
			}
		}

//#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
		if ( m_bTracingUser )
		{
			NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
			TracingMsg.nUserNum  = GetUserID();
			StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );

			CString strTemp;
			strTemp.Format( "Buy From Npc, [%s][%s], Buy price:[%I64d], Have Money:[%I64d]",
					 m_szUID, m_szName,  llPRICE3, m_lnMoney );

			StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

			m_pGLGaeaServer->SENDTOAGENT( m_dwClientID, &TracingMsg );
		}
//#endif

		//	Note : �κ��丮�� �־��ݴϴ�.
		//
		BOOL bOK = m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
		if ( !bOK )		return S_OK;

		//	Note :�������� ���� ���� ��� ���.
		//
		//add money 2
		//item log
		if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) 
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( sITEM_NEW, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR_VOTE, sITEM_NEW.wTurnNum );
		}
		//add money 3
		else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) 
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( sITEM_NEW, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR_PREMIUM, sITEM_NEW.wTurnNum );
		}
		//add contributionpoint by CNDev
		else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM ) 
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( sITEM_NEW, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR_CONTRIBUTION, sITEM_NEW.wTurnNum );
		}
		else
		{
			GLITEMLMT::GetInstance().ReqItemRoute ( sITEM_NEW, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, sITEM_NEW.wTurnNum );
		}

		//	�ι꿡 ������ �־��ִ� �޽���.
		GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
		NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);
	}

	if ( pCrow->m_bNpcRecall )
	{
		volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;
		volatile LONGLONG llPRICE1 = (LONGLONG)( dwPrice * fSHOP_RATE_C );
		volatile LONGLONG llCommission = llPRICE2 - llPRICE1;

		llCommission = LONGLONG ( llCommission * GLCONST_CHAR::fEARNING_RATE_NPC + 0.1f );

		pCrow->m_lnCommission += llCommission;
	}
	else if ( m_pLandMan->m_dwGuidClubID != CLUB_NULL )
	{
		GLGuidance *pGuid = m_pLandMan->m_pGuidance;
		if ( pGuid && !pGuid->m_bBattle )
		{
			//	Note : Ŭ���� ���� �߻�.
			volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;
			volatile LONGLONG llPRICE1 = (LONGLONG)( dwPrice * fSHOP_RATE_C );
			volatile LONGLONG llCommission = llPRICE2 - llPRICE1;

			GLClubMan &cClubMan = GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB *pCLUB = cClubMan.GetClub ( m_pLandMan->m_dwGuidClubID );
			if ( pCLUB )
			{
				pCLUB->m_lnIncomeMoney += llCommission;

				// Ŭ�� ������ 1�� �̻��� ��� �α׸� �����.
				if( llCommission > 	EMCLUBINCOMEMONEY_LOG )
					GLITEMLMT::GetInstance().ReqMoneyExc( ID_CHAR, 0, 
														  ID_CLUB, pCLUB->m_dwID,
														  llCommission,
														  EMITEM_ROUTE_CLUBINCOME );

				CDebugSet::ToFile ( "club_income_char.txt", "{BuyFromNpc}, ClubID[%u], CharID[%u], Commission[%I64d], IncomeMoney[%I64d]",
									pCLUB->m_dwID, m_dwCharID, llCommission, pCLUB->m_lnIncomeMoney );
			}
		}
	}

	//	�ݾ� ��ȭ �޽���.
	GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
	NetMsgMoney.lnMoney = m_lnMoney;
	//add money 2
	NetMsgMoney.lnVoteP = m_lnVoteP;
	//add money 3
	NetMsgMoney.lnPremP = m_lnPremP;
	//add contributionpoints by CNDev
	NetMsgMoney.lnContribP = m_lnContribP;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgMoney);

	return S_OK;
}

// *****************************************************
// Desc: NPC ���� ������ �Ǹ�
// *****************************************************
HRESULT GLChar::MsgReqSaleToNpc ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_SALE_TO_NPC *pNetMsg = (GLMSG::SNETPC_REQ_SALE_TO_NPC *) nmg;
	
	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pNetMsg->sNID );

	DxConsoleMsg* pConsoleMsg = GLGaeaServer::GetInstance().GetConsoleMsg();

	// ���� NPC ��ȿ�� üũ
	PGLCROW pCrow = m_pLandMan->GetCrow ( pNetMsg->dwNPCID );
	if ( !pCrow )						return E_FAIL;
	

	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-pCrow->GetPosition()) );
	float fTalkRange = (float) (pCrow->GetBodyRadius() + GETBODYRADIUS() + 30);
	float fTalkableDis = fTalkRange + 20;

	if ( fDist>fTalkableDis )			return E_FAIL;
	

	if ( !pCrowData )
	{
		//	���� NPC�� �������� �ʽ��ϴ�.
		return E_FAIL;
	}

	if ( !pCrowData->m_sNpcTalkDlg.IsMARKET() )
	{
		//	������ �ƴմϴ�.
		return E_FAIL;
	}

	if ( !VALID_HOLD_ITEM() )
	{
		//	�ȷ��� �ϴ� �������� �����ϴ�.
		return E_FAIL;
	}

	const SITEMCUSTOM& sHOLD_ITEM = GET_HOLD_ITEM();
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sHOLD_ITEM.sNativeID );
	if ( pItem==NULL )						return E_FAIL;

	// �־��̵�(DB���ٿ�)
	DWORD dwPetID = sHOLD_ITEM.dwPetID;
	DWORD dwVehicleID = sHOLD_ITEM.dwVehicleID;

	//	�ŷ��ɼ�
	if ( !pItem->sBasicOp.IsSALE() )
	{
		//	�ȱ� �������� ����.
		return E_FAIL;
	}

//  Ŭ���̾�Ʈ�� ���� ������ ��� ������ Ʋ���� ������ 
/* 
	volatile float fSHOP_SALE_RATE = GET_PK_SHOP2SALE();
	if ( fSHOP_SALE_RATE==0.0f )			return E_FAIL;

	volatile LONGLONG llPRICE = LONGLONG( pItem->GETPRICE(sHOLD_ITEM.wTurnNum) * GLCONST_CHAR::fDISPRICE );

	volatile float fSHOP_SALE_2RATE = fSHOP_SALE_RATE - m_pLandMan->m_fCommissionRate;
	volatile float fSHOP_SALE_2RATE_C = (DOUBLE(fSHOP_SALE_2RATE) * 0.01f );
	volatile LONGLONG llDisPRICE2 = LONGLONG( llPRICE * fSHOP_SALE_2RATE_C );
*/

	volatile float fSHOP_SALE_RATE = GET_PK_SHOP2SALE();
	if ( fSHOP_SALE_RATE==0.0f )			return E_FAIL;
	
	volatile LONGLONG dwPrice = pItem->GETSELLPRICE ( sHOLD_ITEM.wTurnNum );  

	volatile float fSHOP_SALE_2RATE = fSHOP_SALE_RATE - m_pLandMan->m_fCommissionRate;
	volatile float fSHOP_SALE_2RATE_C = fSHOP_SALE_2RATE * 0.01f;

	volatile LONGLONG llDisPRICE2 = LONGLONG( dwPrice * fSHOP_SALE_2RATE_C );


	if ( pCrow->m_bNpcRecall )
	{
		volatile float fSHOP_RATE_C = fSHOP_SALE_RATE * 0.01f;
		volatile LONGLONG llDisPRICE1 = LONGLONG ( dwPrice * fSHOP_RATE_C );
		volatile LONGLONG llCommission = llDisPRICE1-llDisPRICE2;

		llCommission = LONGLONG ( llCommission * GLCONST_CHAR::fEARNING_RATE_NPC + 0.1f );

		pCrow->m_lnCommission += llCommission;
	}
	else if ( m_pLandMan->m_dwGuidClubID != CLUB_NULL )
	{
		GLGuidance *pGuid = m_pLandMan->m_pGuidance;
		if ( pGuid && !pGuid->m_bBattle )
		{
			//	Note : Ŭ���� ���� �߻�.
			volatile float fSHOP_RATE_C = fSHOP_SALE_RATE * 0.01f;
			volatile LONGLONG llDisPRICE1 = LONGLONG ( dwPrice * fSHOP_RATE_C );
			volatile LONGLONG llCommission = llDisPRICE1-llDisPRICE2;
		
			GLClubMan &cClubMan = GLGaeaServer::GetInstance().GetClubMan();
			GLCLUB *pCLUB = cClubMan.GetClub ( m_pLandMan->m_dwGuidClubID );
			if ( pCLUB )
			{
				pCLUB->m_lnIncomeMoney += llCommission;

				// Ŭ�� ������ 1�� �̻��� ��� �α׸� �����.
				if( llCommission > 	EMCLUBINCOMEMONEY_LOG )
					GLITEMLMT::GetInstance().ReqMoneyExc(	ID_CHAR, m_dwCharID, 
															ID_CLUB, pCLUB->m_dwID,
															llCommission,
															EMITEM_ROUTE_CLUBINCOME );

				CDebugSet::ToFile ( "club_income_char.txt", "{SaleToNpc}, ClubID[%u], CharID[%u], Commission[%I64d], IncomeMoney[%I64d]",
									pCLUB->m_dwID, m_dwCharID, llCommission, pCLUB->m_lnIncomeMoney );
			}
		}
	}

	//	�ݾ� ��ȭ.
	CheckMoneyUpdate( m_lnMoney, llDisPRICE2, TRUE, "Sale To Npc." );
	m_bMoneyUpdate = TRUE;

	if ( pItem->sBasicOp.IsPointRecover() )
	{//add and modified for //add point sell
		if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
		{
			m_lnVoteP += dwPrice;

			if ( dwPrice>EMMONEY_LOG_PREMIUM )
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, 0, ID_CHAR, m_dwCharID, dwPrice, EMITEM_ROUTE_CHAR_VOTE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVoteP, EMITEM_ROUTE_CHAR_VOTE );
			}

			GLITEMLMT::GetInstance().ReqItemRoute ( sHOLD_ITEM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_SHOP_VOTE, sHOLD_ITEM.wTurnNum );

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_VOTE_SELL.txt", "%s	SELL VOTE ITEM [CharID: %d][CharName:%s][Item Price:%I64d][Money After:%I64d][ITEM MID:%d SID:%d][Number: %d] ",
			strTIME.c_str(), m_dwCharID, m_szName, dwPrice, m_lnVoteP, pItem->sBasicOp.sNativeID.wMainID, pItem->sBasicOp.sNativeID.wSubID,sHOLD_ITEM.wTurnNum );
		}

		else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
		{
			m_lnPremP += dwPrice;

			if ( dwPrice>EMMONEY_LOG_PREMIUM )
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, 0, ID_CHAR, m_dwCharID, dwPrice, EMITEM_ROUTE_CHAR_PREMIUM );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnPremP, EMITEM_ROUTE_CHAR_PREMIUM );
			}

			GLITEMLMT::GetInstance().ReqItemRoute ( sHOLD_ITEM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_SHOP_PREMIUM, sHOLD_ITEM.wTurnNum );

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_PREMIUM_SELL.txt", "%s	SELL PREMIUM ITEM [CharID: %d][CharName:%s][Item Price:%I64d][Money After:%I64d][ITEM MID:%d SID:%d][Number: %d] ",
			strTIME.c_str(), m_dwCharID, m_szName, dwPrice, m_lnPremP, pItem->sBasicOp.sNativeID.wMainID, pItem->sBasicOp.sNativeID.wSubID,sHOLD_ITEM.wTurnNum );
		}
		//add contributionpoint by CNDev
		else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
		{
			m_lnContribP += dwPrice;

			if ( dwPrice>EMMONEY_LOG_CONTRIBUTION )
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, 0, ID_CHAR, m_dwCharID, dwPrice, EMITEM_ROUTE_CHAR_CONTRIBUTION );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnContribP, EMITEM_ROUTE_CHAR_CONTRIBUTION );
			}

			GLITEMLMT::GetInstance().ReqItemRoute ( sHOLD_ITEM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_SHOP_CONTRIBUTION, sHOLD_ITEM.wTurnNum );

			std::string strTIME = CDebugSet::GetCurTime ();
			CDebugSet::ToFile ( "ITEM_CONTRIBUTION_SELL.txt", "%s	SELL CONTRIBUTION ITEM [CharID: %d][CharName:%s][Item Price:%I64d][Money After:%I64d][ITEM MID:%d SID:%d][Number: %d] ",
			strTIME.c_str(), m_dwCharID, m_szName, dwPrice, m_lnContribP, pItem->sBasicOp.sNativeID.wMainID, pItem->sBasicOp.sNativeID.wSubID,sHOLD_ITEM.wTurnNum );
		}

	}
	else
	{
		m_lnMoney += llDisPRICE2;

		//	�� �α�.
		if ( llDisPRICE2>EMMONEY_LOG )
		{
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, 0, ID_CHAR, m_dwCharID, llDisPRICE2, EMITEM_ROUTE_CHAR );
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
		}

		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( sHOLD_ITEM, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_SHOP, sHOLD_ITEM.wTurnNum );
	}

//#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if ( m_bTracingUser )
	{
		NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
		TracingMsg.nUserNum  = GetUserID();
		StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );

		CString strTemp;
		strTemp.Format( "Sale To Npc, [%s][%s], Sale price:[%I64d], Have Money:[%I64d]",
				 m_szUID, m_szName,  llDisPRICE2, m_lnMoney );

		StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

		m_pGLGaeaServer->SENDTOAGENT( m_dwClientID, &TracingMsg );
	}
//#endif

	//	������ ����.
	RELEASE_HOLD_ITEM();

	//	�ݾ� ��ȭ �޽���.
	GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
	NetMsgMoney.lnMoney = m_lnMoney;
	//add money 2
	NetMsgMoney.lnVoteP = m_lnVoteP;
	//add money 3
	NetMsgMoney.lnPremP = m_lnPremP;
	//add contributionpoints by CNDev
	NetMsgMoney.lnContribP = m_lnContribP;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgMoney);

	//	[�ڽſ���] �տ� �־��� ������ ����.
	GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release(SLOT_HOLD);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);

	// ��ī���� ��� �������� ������ �ȸ� ��DB ����
	if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD && dwPetID != 0 )
	{
		// ���� ������� ���ش�.
		GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(m_dwPetGUID,true,false) );

		CDeletePet* pDbAction = new CDeletePet ( m_dwCharID, dwPetID );
		GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
		if ( pDBMan ) pDBMan->AddJob ( pDbAction );

		// ���� �����Ǹ� ��Ȱ������ Ŭ���̾�Ʈ�� �˷���.
		CGetRestorePetList *pDbAction1 = new CGetRestorePetList ( m_dwCharID, m_dwClientID );
		if ( pDBMan ) pDBMan->AddJob ( pDbAction1 );
	}

	if ( pItem->sBasicOp.emItemType == ITEM_VEHICLE && dwVehicleID != 0 )
	{
		CDeleteVehicle* pDbAction = new CDeleteVehicle ( m_dwClientID, m_dwCharID, dwVehicleID );
		GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
		if ( pDBMan ) pDBMan->AddJob ( pDbAction );		
	}

	return S_OK;
}


HRESULT GLChar::MsgReqFireCracker ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_FIRECRACKER_BRD *pNetMsg = (GLMSG::SNETPC_REQ_FIRECRACKER_BRD *) nmg;

	GLMSG::SNETPC_REQ_FIRECRACKER_FB NetMsgFB;

	if ( !VALID_HOLD_ITEM () )
	{
		NetMsgFB.emFB = EMREQ_FIRECRACKER_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( m_sTrade.Valid() )
	{
		NetMsgFB.emFB = EMREQ_FIRECRACKER_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pITEM = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pITEM )
	{
		NetMsgFB.emFB = EMREQ_FIRECRACKER_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pITEM->sBasicOp.sNativeID ) ) return E_FAIL;

	SITEMCUSTOM &sCUSTOM = m_PutOnItems[SLOT_HOLD];

	if ( pITEM->sBasicOp.emItemType!=ITEM_FIRECRACKER )
	{
		NetMsgFB.emFB = EMREQ_FIRECRACKER_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pITEM->sBasicOp.strTargetEffect.empty() )
	{
		NetMsgFB.emFB = EMREQ_FIRECRACKER_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	//	Note : ���� ���.
	//
	GLMSG::SNETPC_REQ_FIRECRACKER_BRD NetMsgBrd;
	NetMsgBrd.nidITEM = sCUSTOM.sNativeID;
	NetMsgBrd.vPOS = pNetMsg->vPOS;
	SNDMSGAROUND ( (NET_MSG_GENERIC*) &NetMsgBrd );

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgBrd);

	//	Note : ���� ����� �˸�.
	//
	NetMsgFB.emFB = EMREQ_FIRECRACKER_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	//	Note : ������ �Ҹ�.
	//
	DoDrugSlotItem (SLOT_HOLD );

	return S_OK;
}

HRESULT GLChar::MsgInvenVietnamGet ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;
//	if( m_dwVietnamGainType == GAINTYPE_EMPTY ) return E_FAIL;
//	if( m_dwVietnamGainType == GAINTYPE_HALF ) return E_FAIL;

	GLMSG::SNETPC_INVEN_VIETNAM_INVENGET *pNetMsg = (GLMSG::SNETPC_INVEN_VIETNAM_INVENGET *) nmg;


	SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )	return E_FAIL;

	if ( CheckCoolTime( pInvenItem->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sDrugOp.wCureVolume == 0 )	return E_FAIL;

    if( pNetMsg->bGetExp )
	{
		// Ȯ�� 
		if ( pItem->sDrugOp.bRatio ) 
		{
			LONGLONG lAddExp;
			LONGLONG lAddMoney;

			float fRate = (float)pItem->sDrugOp.wCureVolume / 100.0f;

			lAddExp = m_lVNGainSysExp * fRate;
			lAddMoney = m_lVNGainSysMoney * fRate;

			if ( lAddExp > m_lVNGainSysExp ) lAddExp = m_lVNGainSysExp;
			if ( lAddMoney > m_lVNGainSysMoney ) lAddMoney = m_lVNGainSysMoney;

			m_lVNGainSysExp -= lAddExp;
			m_lVNGainSysMoney -= lAddMoney;

			m_sExperience.lnNow += lAddExp;
			m_lnMoney			+= lAddMoney;

			m_bVietnamLevelUp = TRUE; 
		}
		else 
		{
			LONGLONG lAddExp;
			if( m_lVNGainSysExp < pItem->sDrugOp.wCureVolume )
			{
				lAddExp = m_lVNGainSysExp;
				m_lVNGainSysExp = 0;
			}else{
				lAddExp = pItem->sDrugOp.wCureVolume;
				m_lVNGainSysExp -= pItem->sDrugOp.wCureVolume;
			}
			m_sExperience.lnNow += lAddExp;
			m_lnMoney			+= m_lVNGainSysMoney;

			m_lVNGainSysMoney = 0;
			m_bVietnamLevelUp = TRUE; 
		}

		DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

		GLMSG::SNETPC_INVEN_VIETNAM_EXPGET_FB NetMsgFB;
		NetMsgFB.lnVnMoney = m_lVNGainSysMoney;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

		//	Note : Ŭ���̾�Ʈ�� �� �׼� ��ȭ�� �˷���.
		GLMSG::SNETPC_UPDATE_MONEY NetMsg;
		NetMsg.lnMoney = m_lnMoney;
		//add money 2
		NetMsg.lnVoteP = m_lnVoteP;
		//add money 3
		NetMsg.lnPremP = m_lnPremP;
		//add contributionpoints by CNDev
		NetMsg.lnContribP = m_lnContribP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

		//	Note : �ݾ� �α�.
		//
		/*if ( lnAmount>EMMONEY_LOG )
		{
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, 0, ID_CHAR, m_dwCharID, lnAmount, EMITEM_ROUTE_VNINVEN );
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
		}*/
	}else{
		m_dwVietnamInvenCount += pItem->sDrugOp.wCureVolume;
		DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );
		GLMSG::SNETPC_INVEN_VIETNAM_ITEMGET_FB NetMsgFB;
		NetMsgFB.dwVietnamInvenCount = m_dwVietnamInvenCount;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	}

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.


	return S_OK;
}

HRESULT GLChar::MsgReqInvenDrug ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_INVENDRUG *pNetMsg = (GLMSG::SNETPC_REQ_INVENDRUG *) nmg;

	SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )	return E_FAIL;

	if ( CheckCoolTime( pInvenItem->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sDrugOp.emDrug==ITEM_DRUG_NUNE )	return E_FAIL;

	if ( m_bAntiPot )
	{
		GLMSG::SNET_ITEM_CANNOT_POT NetMsg;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

		return E_FAIL;
	}

	if ( m_sCONFTING.IsFIGHTING() )
	{
		if ( !m_sCONFTING.IsRECOVE() )
		{
			//	Note : ȸ���� ��� ���� ���� Ŭ���̾�Ʈ�� �˸�.
			//
			GLMSG::SNETPC_CONFRONT_RECOVE NetMsg;
			NetMsg.wRECOVER = m_sCONFTING.wRECOVER;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

			return E_FAIL;
		}

		if ( m_sCONFTING.sOption.wRECOVER!=USHRT_MAX )
		{
			m_sCONFTING.COUNTRECOVE();

			//	Note : ȸ���� ��� ���� ���� Ŭ���̾�Ʈ�� �˸�.
			//
			GLMSG::SNETPC_CONFRONT_RECOVE NetMsg;
			NetMsg.wRECOVER = m_sCONFTING.wRECOVER;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);
		}
	}

	//	Note : pk ����� ���θ� ��� �̻��� ��� ȸ������ ����� ���´�.
	//
	DWORD dwPK_LEVEL = GET_PK_LEVEL();
	if ( dwPK_LEVEL != UINT_MAX && dwPK_LEVEL>GLCONST_CHAR::dwPK_DRUG_ENABLE_LEVEL )
	{
		return E_FAIL;
	}

	switch ( pItem->sDrugOp.emDrug )
	{
	case ITEM_DRUG_HP:
		if ( m_sHP.dwNow == m_sHP.dwMax ) return E_FAIL;
		m_sHP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nHP_Potion_Rate); //add directval
		break;

	case ITEM_DRUG_MP:
		if ( m_sMP.dwNow == m_sMP.dwMax ) return E_FAIL;
		m_sMP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nMP_Potion_Rate); //add directval
		break;

	case ITEM_DRUG_SP:
		if ( m_sSP.dwNow == m_sSP.dwMax ) return E_FAIL;
		m_sSP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nSP_Potion_Rate); //add directval
		break;

	case ITEM_DRUG_CP:
		if ( m_sCP.dwNow == m_sCP.dwMax ) return E_FAIL;
		m_sCP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ); //add cp
		break;

	case ITEM_DRUG_HP_MP:
		if ( m_sHP.dwNow == m_sHP.dwMax && m_sMP.dwNow == m_sMP.dwMax ) return E_FAIL;
		m_sHP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nHP_Potion_Rate); //add directval
		m_sMP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nMP_Potion_Rate); //add directval
		break;

	case ITEM_DRUG_MP_SP:
		if ( m_sMP.dwNow == m_sMP.dwMax && m_sSP.dwNow == m_sSP.dwMax ) return E_FAIL;
		m_sMP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nMP_Potion_Rate); //add directval
		m_sSP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nSP_Potion_Rate); //add directval
		break;

	case ITEM_DRUG_HP_MP_SP:
		if ( m_sHP.dwNow == m_sHP.dwMax && m_sMP.dwNow == m_sMP.dwMax && m_sSP.dwNow == m_sSP.dwMax ) return E_FAIL;
		m_sHP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nHP_Potion_Rate); //add directval
		m_sMP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nMP_Potion_Rate); //add directval
		m_sSP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nSP_Potion_Rate); //add directval
		break;

	//case ITEM_DRUG_HP:
	//case ITEM_DRUG_MP:
	//case ITEM_DRUG_SP:
	//case ITEM_DRUG_HP_MP:
	//case ITEM_DRUG_MP_SP:
	//case ITEM_DRUG_HP_MP_SP:
	//	RECEIVE_DRUGFACT(pItem->sDrugOp.emDrug,pItem->sDrugOp.wCureVolume,pItem->sDrugOp.bRatio);
	//	break;

	case ITEM_DRUG_CURE:
	case ITEM_DRUG_HP_CURE:
	case ITEM_DRUG_HP_MP_SP_CURE:
		{
			BOOL bSTATEBLOW = ISSTATEBLOW();

			if( pItem->sDrugOp.emDrug == ITEM_DRUG_HP_CURE )
			{
				if ( m_sHP.dwNow == m_sHP.dwMax && !bSTATEBLOW ) return E_FAIL;
				m_sHP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nHP_Potion_Rate); //add directval
				bSTATEBLOW = TRUE;
			}

			if( pItem->sDrugOp.emDrug == ITEM_DRUG_HP_MP_SP_CURE )
			{
				if ( m_sHP.dwNow == m_sHP.dwMax && m_sMP.dwNow == m_sMP.dwMax 
					&& m_sSP.dwNow == m_sSP.dwMax && !bSTATEBLOW ) return E_FAIL;
				m_sHP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nHP_Potion_Rate); //add directval
				m_sMP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nMP_Potion_Rate); //add directval
				m_sSP.INCREASE ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio ,m_nSP_Potion_Rate); //add directval
				bSTATEBLOW = TRUE;
			}

			if ( !bSTATEBLOW ) return E_FAIL;

			GLMSG::SNETPC_CURESTATEBLOW_BRD	NetMsgBrd;
			for ( int i=0; i<EMBLOW_MULTI; ++i )
			{
				if ( m_sSTATEBLOWS[i].emBLOW==EMBLOW_NONE )		continue;

				EMDISORDER emDIS = STATE_TO_DISORDER(m_sSTATEBLOWS[i].emBLOW);
				if ( !(pItem->sDrugOp.dwCureDISORDER&emDIS) )	continue;

				//	�����̻� ���ӽð��� �����Ŵ.
				m_sSTATEBLOWS[i].fAGE = 0.0f;
			}

			//	�����̻� ġ����� �ڽſ��� �˸�.
			NetMsgBrd.emCrow = CROW_PC;
			NetMsgBrd.dwID = m_dwGaeaID;
			NetMsgBrd.dwCUREFLAG = pItem->sDrugOp.dwCureDISORDER;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgBrd);

			//	�����̻� ġ����� ��ο��� �˸�.
			SNDMSGAROUND ( (NET_MSG_GENERIC*)&NetMsgBrd );
		}
		break;
	};


	//	Note : ������ �Ҹ�.
	//
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//	Note : ü�� ��ġ ��ȭ�� [�ڽ�,��Ƽ��,����]�� Ŭ���̾�Ʈ�鿡 �˸�.
	MsgSendUpdateState ();

	return S_OK;
}



HRESULT GLChar::MsgGetPoints ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;
	GLMSG::SNETPC_RETRIEVE_POINTS_FB *pNetMsg = (GLMSG::SNETPC_RETRIEVE_POINTS_FB *) nmg;
	
	GLMSG::SNETPC_RETRIEVE_POINTS_FB pNetMsgFB;
	pNetMsgFB.emFB = pNetMsg->emFB;
	pNetMsgFB.PPoints = pNetMsg->PPoints;
	pNetMsgFB.VPoints = pNetMsg->VPoints;
	//CONSOLEMSG_WRITE ( " send MsgGetPoints ");
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&pNetMsgFB);
	return S_OK;
}
HRESULT GLChar::MsgRetrievePoints ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;
	GLMSG::SNETPC_RETRIEVE_POINTS *pNetMsg = (GLMSG::SNETPC_RETRIEVE_POINTS *) nmg;
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CGetPoints* pDbAction = new CGetPoints ( m_dwCharID, m_dwUserID , m_dwClientID );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
		//if ( pDbAction ) CONSOLEMSG_WRITE ( " send MsgRetrievePoints ");
	}

	return S_OK;
}

HRESULT GLChar::MsgReqInvenItemShopInfo ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;
	GLMSG::SNET_GET_ITEMSHOP_FROMDB* pNetMsg = (GLMSG::SNET_GET_ITEMSHOP_FROMDB*) nmg;

	CString strUID = m_szUID;
    std::vector<ITEMSHOP> vItem;

	// DB���� ���������� ���� ��������

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CGetItemShop* pDbAction = new CGetItemShop (m_dwCharID);
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
		//if ( pDbAction ) CONSOLEMSG_WRITE ( " send MsgReqInvenItemShopInfo ");
	}
		
	return S_OK;
}


HRESULT GLChar::MsgBuyItemShop ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;
	GLMSG::SNET_ITEMSHOP_ITEM_BUY *pNetMsg = (GLMSG::SNET_ITEMSHOP_ITEM_BUY *) nmg;
	GLMSG::SNET_ITEMSHOP_ITEM_BUY NetMsg;
	NetMsg.bBuy = pNetMsg->bBuy;
	NetMsg.dwUserID = pNetMsg->dwUserID;
	//CONSOLEMSG_WRITE ( " send ");
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

	return S_OK;
}

HRESULT GLChar::MsgReqInvenItemShop ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_ITEMSHOP_BUY *pNetMsg = (GLMSG::SNET_ITEMSHOP_BUY *) nmg;
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CItemShop_Get *pDbAction = new CItemShop_Get( m_dwClientID, m_dwUserID, pNetMsg->szPurKey );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
		//if ( pDbAction ) CONSOLEMSG_WRITE ( " send MsgReqInvenItemShop ");
	}
	
	return S_OK;
}



HRESULT GLChar::MsgReqInvenBoxInfo ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_GET_CHARGEDITEM_FROMDB* pNetMsg = (GLMSG::SNET_GET_CHARGEDITEM_FROMDB*) nmg;

	CString strUID = m_szUID;
    std::vector<SHOPPURCHASE> vItem;

	// DB���� ���������� ���� ��������

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CGetPurchaseItem* pDbAction = new CGetPurchaseItem ( strUID, m_dwCharID );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
	}
		
	return S_OK;
}

HRESULT GLChar::MsgReqInvenBoxOpen ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_INVEN_BOXOPEN *pNetMsg = (GLMSG::SNET_INVEN_BOXOPEN *) nmg;

	GLMSG::SNET_INVEN_BOXOPEN_FB NetMsgFB;

	WORD wPosX = pNetMsg->wPosX;
	WORD wPosY = pNetMsg->wPosY;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMREQ_BOXOPEN_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_BOX )
	{
		NetMsgFB.emFB = EMREQ_BOXOPEN_FB_NOBOX;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( !pItem->sBox.VALID() )
	{
		NetMsgFB.emFB = EMREQ_BOXOPEN_FB_EMPTY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	//	Note : �κ��� ���� ���� ����.
	//
	GLInventory cInvenTemp;
	cInvenTemp.Assign ( m_cInventory );

	for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		SITEMCUSTOM sCUSTOM;
		sCUSTOM.sNativeID = pItem->sBox.sITEMS[i].nidITEM;
		if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
		if ( !pITEM )
		{
			NetMsgFB.emFB = EMREQ_BOXOPEN_FB_INVALIDITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		BOOL bOK = cInvenTemp.InsertItem ( sCUSTOM );
		if ( !bOK )
		{
			//	Note : �κ��� ������ ���� ������ �Ǵܵ�.
			//
			NetMsgFB.emFB = EMREQ_BOXOPEN_FB_NOTINVEN;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}
	}

	//	Note : ���ھȿ� �ִ� �������� ����� �ִ´�.
	//
	for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		SITEMCUSTOM sITEM_NEW;
		sITEM_NEW.sNativeID = pItem->sBox.sITEMS[i].nidITEM;
		DWORD dwAMOUNT = pItem->sBox.sITEMS[i].dwAMOUNT;
		if ( sITEM_NEW.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sITEM_NEW.sNativeID );
		if ( !pITEM )											return E_FAIL;

		//	Note : ������ ����.
		//
		CTime cTIME = CTime::GetCurrentTime();
		sITEM_NEW.tBORNTIME = cTIME.GetTime();

		sITEM_NEW.wTurnNum = (WORD) dwAMOUNT;
		sITEM_NEW.cGenType = pInvenItem->sItemCustom.cGenType;
		sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

		// ������ ����Ʈ���� �Է��� ���� ��� ���� ( ���� )
		sITEM_NEW.cDAMAGE = (BYTE)pITEM->sBasicOp.wGradeAttack;
		sITEM_NEW.cDEFENSE = (BYTE)pITEM->sBasicOp.wGradeDefense;

		//	���� �ɼ� ����.
		if( sITEM_NEW.GENERATE_RANDOM_OPT() )
		{
			GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
		}

		//	Note : �κ��� ���� ��ġ ã��.
		WORD wInsertPosX, wInsertPosY;
		BOOL bOk = m_cInventory.FindInsrtable ( pITEM->sBasicOp.wInvenSizeX, pITEM->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
		if ( !bOk )		return S_OK;	//	�κ� ���� �������� �ǵ��� ����.

		//	������ ������ �ι꿡 ����.
		m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
		SINVENITEM *pInsertItem = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

		GLITEMLMT::GetInstance().ReqItemRoute ( pInsertItem->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pInsertItem->sItemCustom.wTurnNum );

		//	[�ڽſ���] �޽��� �߻�.
		GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
		NetMsg_Inven.Data = *pInsertItem;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);
	}

	//	���� ���� �α�.
	GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pInvenItem->sItemCustom.wTurnNum );

	//	Note : ���� ������ ����.
	//
	m_cInventory.DeleteItem ( wPosX, wPosY );

	//	[�ڽſ���] �ι꿡 ������ ����.
	GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
	NetMsg_Inven_Delete.wPosX = wPosX;
	NetMsg_Inven_Delete.wPosY = wPosY;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);

	//	Note : ���ڸ� ���������� ������.
	//
	NetMsgFB.emFB = EMREQ_BOXOPEN_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}


HRESULT GLChar::MsgReqInvenGMItem ( NET_MSG_GENERIC* nmg ) //add itemcmd
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_INVEN_GMITEM *pNetMsg = (GLMSG::SNET_INVEN_GMITEM *) nmg;

	GLMSG::SNET_INVEN_GMITEM_FB NetMsgFB;

	if ( pNetMsg->wPass != GLCONST_CHAR::wGMItemPass )
	{
		NetMsgFB.emFB = EMREQ_GMITEM_FB_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sItemID );
	if ( !pItem  )
	{
		NetMsgFB.emFB = EMREQ_GMITEM_FB_NOTITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pNetMsg->wNum >= GLCONST_CHAR::wGMItemMax )
	{
		NetMsgFB.emFB = EMREQ_GMITEM_FB_MAX;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	GLInventory cInvenTemp;
	cInvenTemp.Assign ( m_cInventory );

	WORD	wNum =  pNetMsg->wNum;
	for ( int i=0; i<wNum; ++i )
	{
		SITEMCUSTOM sCUSTOM;
		sCUSTOM.sNativeID = pItem->sBasicOp.sNativeID;

		if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
		if ( !pITEM )
		{
			NetMsgFB.emFB = EMREQ_GMITEM_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		BOOL bOK = cInvenTemp.InsertItem ( sCUSTOM );
		if ( !bOK )
		{
			NetMsgFB.emFB = EMREQ_GMITEM_FB_INFAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}
	}

	for ( int i=0; i<wNum; ++i )
	{
		SITEMCUSTOM sITEM_NEW;
		sITEM_NEW.sNativeID = pItem->sBasicOp.sNativeID;
		//DWORD dwAMOUNT = pItem->sBox.sITEMS[i].dwAMOUNT;
		if ( sITEM_NEW.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sITEM_NEW.sNativeID );
		if ( !pITEM )											return E_FAIL;

		CTime cTIME = CTime::GetCurrentTime();
		sITEM_NEW.tBORNTIME = cTIME.GetTime();

		//sITEM_NEW.wTurnNum = (WORD) 1;
		sITEM_NEW.cGenType = EMGEN_SYSTEM;
		sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

		sITEM_NEW.cDAMAGE = (BYTE)9; //item is +9
		sITEM_NEW.cDEFENSE = (BYTE)9;//item is +9

		sITEM_NEW.dwOwnerID = m_dwCharID; //item have owner id of the one who request

		if( sITEM_NEW.GENERATE_RANDOM_OPT() )
		{
			GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
		}

		WORD wInsertPosX, wInsertPosY;
		BOOL bOk = m_cInventory.FindInsrtable ( pITEM->sBasicOp.wInvenSizeX, pITEM->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
		if ( !bOk )		return S_OK;	

		m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
		SINVENITEM *pInsertItem = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

		GLITEMLMT::GetInstance().ReqItemRoute ( pInsertItem->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pInsertItem->sItemCustom.wTurnNum );

		GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
		NetMsg_Inven.Data = *pInsertItem;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);
	}

	NetMsgFB.emFB = EMREQ_GMITEM_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}
HRESULT GLChar::MsgReqInvenRandomBoxOpen ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_INVEN_RANDOMBOXOPEN *pNetMsg = (GLMSG::SNET_INVEN_RANDOMBOXOPEN *) nmg;

	GLMSG::SNET_INVEN_RANDOMBOXOPEN_FB NetMsgFB;

	WORD wPosX = pNetMsg->wPosX;
	WORD wPosY = pNetMsg->wPosY;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMINVEN_RANDOMBOXOPEN_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_RANDOMITEM )
	{
		NetMsgFB.emFB = EMINVEN_RANDOMBOXOPEN_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( !pItem->sRandomBox.VALID() )
	{
		NetMsgFB.emFB = EMINVEN_RANDOMBOXOPEN_FB_EMPTY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	float fNowRate = seqrandom::getpercent();

	float fLOW = 0;
	ITEM::SRANDOMITEM sGENITEM;
	sGENITEM.nidITEM = SNATIVEID(false);
	for ( DWORD i=0; i<pItem->sRandomBox.vecBOX.size(); ++i )
	{
		ITEM::SRANDOMITEM sITEM = pItem->sRandomBox.vecBOX[i];

		if ( fLOW <= fNowRate && fNowRate < (fLOW+sITEM.fRATE) )
		{
			sGENITEM = sITEM;
			break;
		}

		fLOW += sITEM.fRATE;
	}

	if ( sGENITEM.nidITEM == SNATIVEID(false) )
	{
		//	���� ������ ���� �α� ����.
		GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pInvenItem->sItemCustom.wTurnNum );

		//	Note : ���� ������ ����.
		//
		m_cInventory.DeleteItem ( wPosX, wPosY );

		//	[�ڽſ���] �ι꿡 ������ ����.
		GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
		NetMsg_Inven_Delete.wPosX = wPosX;
		NetMsg_Inven_Delete.wPosY = wPosY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);

		//	Note : ó�� ��� ����.
		NetMsgFB.emFB = EMINVEN_RANDOMBOXOPEN_FB_MISS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM *pITEM_DATA = GLItemMan::GetInstance().GetItem ( sGENITEM.nidITEM );
	if ( !pITEM_DATA )
	{
		NetMsgFB.emFB = EMINVEN_RANDOMBOXOPEN_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	//	Note : ������ �߻�.
	//
	WORD wINSERTX(0), wINSERTY(0);
	BOOL bOK = m_cInventory.FindInsrtable ( pITEM_DATA->sBasicOp.wInvenSizeX, pITEM_DATA->sBasicOp.wInvenSizeY, wINSERTX, wINSERTY );
	if ( !bOK )
	{
		NetMsgFB.emFB = EMINVEN_RANDOMBOXOPEN_FB_NOINVEN;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	//	Note : ������ ����.
	//
	SITEMCUSTOM sITEM_NEW;
	sITEM_NEW.sNativeID = sGENITEM.nidITEM;

	CTime cTIME = CTime::GetCurrentTime();
	sITEM_NEW.tBORNTIME = cTIME.GetTime();

	sITEM_NEW.wTurnNum = pITEM_DATA->GETAPPLYNUM();
	sITEM_NEW.cGenType = pInvenItem->sItemCustom.cGenType;
	sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
	sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
	sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

	// ������ ����Ʈ���� �Է��� ���� ��� ���� ( ���� )
	sITEM_NEW.cDAMAGE = (BYTE)pITEM_DATA->sBasicOp.wGradeAttack;
	sITEM_NEW.cDEFENSE = (BYTE)pITEM_DATA->sBasicOp.wGradeDefense;

	//	���� �ɼ� ����.
	if( sITEM_NEW.GENERATE_RANDOM_OPT() )
	{
		GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
	}

	//	������ ������ �ι꿡 ����.
	m_cInventory.InsertItem ( sITEM_NEW, wINSERTX, wINSERTY );
	SINVENITEM *pInsertItem = m_cInventory.GetItem ( wINSERTX, wINSERTY );

	if (!pInsertItem) return E_FAIL;


	//	[�ڽſ���] �޽��� �߻�.
	GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
	NetMsg_Inven.Data = *pInsertItem;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);

	//	���� ������ ���� �α� ����.
	GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pInvenItem->sItemCustom.wTurnNum );

	//	���� ������ ���� �α� ����.
	GLITEMLMT::GetInstance().ReqItemRoute ( pInsertItem->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pInvenItem->sItemCustom.wTurnNum );

	//	Note : ���� ������ ����.
	//
	m_cInventory.DeleteItem ( wPosX, wPosY );

	//	[�ڽſ���] �ι꿡 ������ ����.
	GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
	NetMsg_Inven_Delete.wPosX = wPosX;
	NetMsg_Inven_Delete.wPosY = wPosY;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);

	//	Note : ���ڸ� ���������� ������.
	//
	NetMsgFB.emFB = EMINVEN_RANDOMBOXOPEN_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqInvenDisJunction ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_INVEN_DISJUNCTION *pNetMsg = (GLMSG::SNET_INVEN_DISJUNCTION *) nmg;

	GLMSG::SNET_INVEN_DISJUNCTION_FB NetMsgFB;

	WORD wPosX = pNetMsg->wPosX;
	WORD wPosY = pNetMsg->wPosY;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMINVEN_DISJUNCTION_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )
	{
		NetMsgFB.emFB = EMINVEN_DISJUNCTION_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	if ( CheckCoolTime( pHold->sBasicOp.sNativeID ) ) return E_FAIL;

	if ( pHold->sBasicOp.emItemType!=ITEM_DISJUNCTION )
	{
		NetMsgFB.emFB = EMINVEN_DISJUNCTION_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	if ( pInvenItem->sItemCustom.nidDISGUISE==SNATIVEID(false) )
	{
		NetMsgFB.emFB = EMINVEN_DISJUNCTION_FB_NONEED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	SITEMCUSTOM sITEM_NEW;
	sITEM_NEW.sNativeID = pInvenItem->sItemCustom.nidDISGUISE;
	if ( sITEM_NEW.sNativeID==SNATIVEID(false) )
	{
		NetMsgFB.emFB = EMINVEN_DISJUNCTION_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM *pONE = GLItemMan::GetInstance().GetItem ( sITEM_NEW.sNativeID );
	if ( !pONE )
	{
		NetMsgFB.emFB = EMINVEN_DISJUNCTION_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	CTime cTIME = CTime::GetCurrentTime();

	if ( pInvenItem->sItemCustom.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
	{
		cTIME = CTime(pInvenItem->sItemCustom.tDISGUISE);

		CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
		cTIME -= tLMT;
	}

	//	Note : ������ ����.
	//
	sITEM_NEW.tBORNTIME = cTIME.GetTime();

	sITEM_NEW.wTurnNum = 1;
	sITEM_NEW.cGenType = EMGEN_BILLING;
	sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
	sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
	sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

	//	Note : hold ������ �κ��� �ǵ���.
	WORD wInsertPosX, wInsertPosY;
	BOOL bOk = m_cInventory.FindInsrtable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
	if ( !bOk )
	{
		NetMsgFB.emFB = EMINVEN_DISJUNCTION_FB_NOINVEN;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;	//	�κ� ���� �������� �ǵ��� ����.
	}

	//	����ִ� ������ �ι꿡 ����.
	m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
	SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

	//	[�ڽſ���] �޽��� �߻�.
	GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
	NetMsg_Inven.Data = *pINSERT_ITEM;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);

	//	Note : ������ ��Ź�� ��.
	//
	pInvenItem->sItemCustom.tDISGUISE = 0;
	pInvenItem->sItemCustom.nidDISGUISE = SNATIVEID(false);

	GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
	NetItemUpdate.wPosX = pNetMsg->wPosX;
	NetItemUpdate.wPosY = pNetMsg->wPosY;
	NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemUpdate);

	//	Note : �и��� �ڽ��� �α�.
	GLITEMLMT::GetInstance().ReqItemRoute ( pINSERT_ITEM->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pINSERT_ITEM->sItemCustom.wTurnNum );

	//	Note : �Ϲ� ���� �������� ��Ź�ϴ� ���� �α� ����.
	//
	GLITEMLMT::GetInstance().ReqItemConversion ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID );

	//	Note : ������ �Ҹ�.
	//
	DoDrugSlotItem ( SLOT_HOLD );

	NetMsgFB.emFB = EMINVEN_DISJUNCTION_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}


// *****************************************************
// Desc: �ڽ�Ƭ ����
// *****************************************************
HRESULT GLChar::MsgReqInvenDisguise ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_INVEN_DISGUISE *pNetMsg = (GLMSG::SNET_INVEN_DISGUISE *) nmg;

	GLMSG::SNET_INVEN_DISGUISE_FB NetMsgFB;

	WORD wPosX = pNetMsg->wPosX;
	WORD wPosY = pNetMsg->wPosY;

	// ��ȿ�� �κ��丮 ���� �˻�
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	// ��ȿ�� ���������� �˻�
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	// �տ� ��� �ִ� �������� ��ȿ���� �˻�
	const SITEMCUSTOM& sITEM_HOLD = GET_SLOT_ITEM(SLOT_HOLD);
	if ( sITEM_HOLD.sNativeID==SNATIVEID(false) )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	// �տ��� �������� ���� ���� ����
	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	// �տ��� �������� �ڽ�Ƭ���� �˻�
	if ( !pHold->sBasicOp.IsDISGUISE() )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_NODISGUISE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	// �κ��丮 �������� ������������ �˻�
	if ( pItem->sBasicOp.IsDISGUISE() )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	// ���� Ŭ���� ���� �˻�
	if ( ( pHold->sBasicOp.dwReqCharClass & pItem->sBasicOp.dwReqCharClass ) == NULL )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_DEFSUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	// ��������� �˻�
	if ( pHold->sBasicOp.emItemType != ITEM_SUIT )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_NOTSUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_SUIT )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_NOTSUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	// ���� ������ �������� �˻�
	if ( pHold->sSuitOp.emSuit != pItem->sSuitOp.emSuit )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_DEFSUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	// �κ��丮�� ������ �ڽ�Ƭ�� ������ �ִ��� �˻�
	if ( pInvenItem->sItemCustom.nidDISGUISE!=SNATIVEID(false) )
	{
		NetMsgFB.emFB = EMREQ_DISGUISE_FB_ALREADY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	//	Note : ���� �ڽ����� ���.
	//
	if ( pHold->sDrugOp.tTIME_LMT != 0 )
	{
		__time64_t &tTIME = pInvenItem->sItemCustom.tDISGUISE;

		CTime tCurTime = sITEM_HOLD.tBORNTIME;
		CTime tSeedTime(tTIME);
		CTimeSpan tLMT (pHold->sDrugOp.tTIME_LMT);
		tLMT += CTimeSpan(0,0,05,0);

		//	Note : �ð� ����.
		tSeedTime = tCurTime + tLMT;

		tTIME = tSeedTime.GetTime();
	}

	//	Note : �ڽ��� ��Ų ����.
	//
	pInvenItem->sItemCustom.nidDISGUISE = pHold->sBasicOp.sNativeID;

	GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
	NetItemUpdate.wPosX = pNetMsg->wPosX;
	NetItemUpdate.wPosY = pNetMsg->wPosY;
	NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemUpdate);

	//	Note : �Ϲ� ���� �������� �ڽ��� ��Ų���� ��ȯ�Ǵ� �� �α� ����.
	//
	GLITEMLMT::GetInstance().ReqItemConversion ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID );

	//	Note :�������� ���� ���� ��� ���.
	//
	GLITEMLMT::GetInstance().ReqItemRoute ( GET_HOLD_ITEM(), ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, GET_HOLD_ITEM().wTurnNum );

	//	Note : �տ��� ������ ����.
	//
	RELEASE_SLOT_ITEM ( SLOT_HOLD );

	//	[�ڽſ���] SLOT�� �־��� ������ ����.
	GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release(SLOT_HOLD);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);

	//	Note : �ڽ��� ��Ų���� ��ȯ ���� �˸�.
	//
	NetMsgFB.emFB = EMREQ_DISGUISE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqInvenCleanser ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_INVEN_CLEANSER *pNetMsg = (GLMSG::SNET_INVEN_CLEANSER *) nmg;

	GLMSG::SNET_INVEN_CLEANSER_FB NetMsgFB;

	WORD wPosX = pNetMsg->wPosX;
	WORD wPosY = pNetMsg->wPosY;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMREQ_CLEANSER_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}	

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		NetMsgFB.emFB = EMREQ_CLEANSER_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )
	{
		NetMsgFB.emFB = EMREQ_CLEANSER_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	if ( CheckCoolTime( pHold->sBasicOp.sNativeID ) ) return E_FAIL;

	if ( pInvenItem->sItemCustom.nidDISGUISE==SNATIVEID(false) )
	{
		NetMsgFB.emFB = EMREQ_CLEANSER_FB_NONEED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;
	}

	if ( pHold->sBasicOp.emItemType!=ITEM_CLEANSER )
	{
		NetMsgFB.emFB = EMREQ_CLEANSER_FB_NONEED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_FALSE;	
	}

	//	Note : ������ ��Ź�� ��.
	//
	pInvenItem->sItemCustom.tDISGUISE = 0;
	pInvenItem->sItemCustom.nidDISGUISE = SNATIVEID(false);

	GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
	NetItemUpdate.wPosX = pNetMsg->wPosX;
	NetItemUpdate.wPosY = pNetMsg->wPosY;
	NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemUpdate);

	//	Note : �Ϲ� ���� �������� ��Ź�ϴ� ���� �α� ����.
	//
	GLITEMLMT::GetInstance().ReqItemConversion ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID );

	//	Note : ������ �Ҹ�.
	//
	DoDrugSlotItem ( SLOT_HOLD );

	//	Note : �ڽ��� ��Ų ���� ��ȯ ���� �˸�.
	//
	NetMsgFB.emFB = EMREQ_CLEANSER_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqMoneyToField ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_MONEY_TO_FIELD *pNetMsg = (GLMSG::SNETPC_REQ_MONEY_TO_FIELD *)nmg;

	if ( !GLCONST_CHAR::bMONEY_DROP2FIELD )	return S_FALSE;
	if ( pNetMsg->lnMoney < 0 )				return S_FALSE;
	if ( m_lnMoney < pNetMsg->lnMoney )		return S_FALSE;

	D3DXVECTOR3 vCollisionPos;
	BOOL bCollision = m_pLandMan->IsCollisionNavi
	(
		pNetMsg->vPos + D3DXVECTOR3(0,+5,0),
		pNetMsg->vPos + D3DXVECTOR3(0,-5,0),
		vCollisionPos
	);

	if ( !bCollision )			return S_FALSE;

	//	Note : ������ �ִ� ���� �׼��� ����.
	CheckMoneyUpdate( m_lnMoney, pNetMsg->lnMoney, FALSE, "Drop Money." );
	m_bMoneyUpdate = TRUE;

	m_lnMoney -= pNetMsg->lnMoney;

	//	Note : �ݾ� �α�.
	//
	if ( pNetMsg->lnMoney > EMMONEY_LOG )
	{
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, pNetMsg->lnMoney, EMITEM_ROUTE_GROUND );
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
	}

//#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if ( m_bTracingUser )
	{
		NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
		TracingMsg.nUserNum  = GetUserID();
		StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, m_szUID );

		CString strTemp;
		strTemp.Format( "DropMoney!!, [%s][%s], Drop Amount:[%I64d], Have Money:[%I64d]",
			m_szUID, m_szName,  pNetMsg->lnMoney, m_lnMoney );

		StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

		m_pGLGaeaServer->SENDTOAGENT( m_dwClientID, &TracingMsg );
	}
//#endif

	//	Note : �ٴڿ� ����Ʈ����.
	m_pLandMan->DropMoney ( vCollisionPos, pNetMsg->lnMoney, FALSE );

	//	Note : Ŭ���̾�Ʈ�� �� �׼� ��ȭ�� �˷���.
	GLMSG::SNETPC_UPDATE_MONEY NetMsg;
	NetMsg.lnMoney = m_lnMoney;
	//add money 2
	NetMsg.lnVoteP = m_lnVoteP;
	//add money 3
	NetMsg.lnPremP = m_lnPremP;
	//add contributionpoints by CNDev
	NetMsg.lnContribP = m_lnContribP;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);
	
	return S_OK;
}

// *****************************************************
// Desc: ������ ��þƮ ��û ó��
// *****************************************************
HRESULT GLChar::MsgReqInvenGrinding ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_GRINDING *pNetMsg = (GLMSG::SNET_INVEN_GRINDING *)nmg;

	if ( !VALID_HOLD_ITEM() )							return S_FALSE;

	SITEMCUSTOM& sHOLDITEM = m_PutOnItems[SLOT_HOLD];
	SITEM* pHold = GLItemMan::GetInstance().GetItem ( sHOLDITEM.sNativeID );
	if ( !pHold )										return E_FAIL;

	if ( pHold->sBasicOp.emItemType != ITEM_GRINDING )			return S_FALSE;

	if ( CheckCoolTime( pHold->sBasicOp.sNativeID ) ) return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )									return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )										return S_FALSE;

	if ( pItem->sSuitOp.gdDamage == GLPADATA(0,0) )
	{
		if ( pHold->sGrindingOp.emCLASS != EMGRINDING_CLASS_CLOTH )	return S_FALSE;
	}
	else
	{
		if ( pHold->sGrindingOp.emCLASS != EMGRINDING_CLASS_ARM )	return S_FALSE;
	}

	BOOL bGrinding = pItem->sBasicOp.emItemType==ITEM_SUIT && pItem->sSuitOp.wReModelNum>0;
	if ( !bGrinding )									return E_FAIL;

	if ( pHold->sGrindingOp.emTYPE == EMGRINDING_DAMAGE || pHold->sGrindingOp.emTYPE == EMGRINDING_DEFENSE )
	{
		if ( pInvenItem->sItemCustom.GETGRADE(pHold->sGrindingOp.emTYPE)>=GLCONST_CHAR::wGRADE_MAX )
		{
			return S_FALSE;
		}
	}
	else
	{
		if ( pInvenItem->sItemCustom.GETGRADE(pHold->sGrindingOp.emTYPE)>=GLCONST_CHAR::wGRADE_MAX_REGI )
		{
			return S_FALSE;
		}
	}
	
	//	Note : ������ ���, ���� �������� ���� ���� �Ұ���
	//
	
	BYTE cGrade = 0;

	cGrade = pInvenItem->sItemCustom.GETGRADE(pHold->sGrindingOp.emTYPE);

	if ( cGrade >= GRADE_HIGH && pHold->sGrindingOp.emGRINDER_TYPE != EMGRINDER_TOP )
	{
		return E_FAIL;
	}
	else if ( cGrade >=GRADE_NORMAL && pHold->sGrindingOp.emGRINDER_TYPE < EMGRINDER_HIGH )
	{
		return E_FAIL;
	}

	// �ֻ��� ��ޱ��� ��æƮ �Ҽ� �ִ� ���������� ����
	if ( cGrade >= GRADE_HIGH && pItem->sGrindingOp.emGRINDER_TYPE != EMGRINDER_TOP )
	{
		return S_FALSE;
	}


	//	Note : ������ ���� Ȯ��
	WORD wGradeNum = 1;	
	
	if ( cGrade >= GRADE_HIGH )
	{
		wGradeNum = GLCONST_CHAR::wUSE_GRADE_NUM[cGrade-GRADE_HIGH];

		if ( wGradeNum > sHOLDITEM.wTurnNum )
		{
			return E_FAIL;
		}

	}

	//	Note : ���� �������� ��� GRADE_NORMAL ��� �̸� ���� �Ұ���.
	//
	//if ( pInvenItem->sItemCustom.GETGRADE(pHold->sGrindingOp.emTYPE)<GRADE_NORMAL && pHold->sGrindingOp.bHIGH )
	//{
	//	return E_FAIL;
	//}

	//	Note : ���� ����.
	//
	bool bRESET(false), bTERMINATE(false);
	EMANTIDISAPPEAR emANTIDISAPPEAR(EMANTIDISAPPEAR_OFF);

	// �ҹ��ְ� �ִٸ� bANTIDISAPPEAR�� 1�� �����ؼ� �������� �Ҹ�Ǵ°��� �����Ѵ�.
	SITEM* pITEM = GET_SLOT_ITEMDATA(SLOT_NECK);
	if ( pITEM && pITEM->sBasicOp.emItemType == ITEM_ANTI_DISAPPEAR )
	{
		if ( !IsCoolTime( pITEM->sBasicOp.sNativeID ) )	emANTIDISAPPEAR = EMANTIDISAPPEAR_ON;
	}

	bool bNoFail = pHold->sBasicOp.IsNoFail();//add nofailups

	BOOL bSucceeded = DOGRINDING ( pInvenItem->sItemCustom, pHold, bRESET, bTERMINATE, bNoFail, emANTIDISAPPEAR ); //add nofailups

	//	Note : �Ҹ�� ������ ó��.
	//
	DoDrugSlotItem(SLOT_HOLD, wGradeNum);

	//	Note :	�Ҹ�� �ҹ��� ó��.
	//			bANTIDISAPPEAR�� false�� �ҹ��ְ� ���Ǿ���.
	if( emANTIDISAPPEAR == EMANTIDISAPPEAR_USE )
	{
		DoDrugSlotItem(SLOT_NECK);
	}

	//	Note : �α� ���.
	//
	GLITEMLMT::GetInstance().ReqItemConversion ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID );

	//	Note : ���� ���� ���� ��ȯ.
	//
	GLMSG::SNET_INVEN_GRINDING_FB NetMsgFB;
	NetMsgFB.bRESET = bRESET;
	NetMsgFB.bTERMINATE = bTERMINATE;
	NetMsgFB.emANTIDISAPPEAR = emANTIDISAPPEAR;
	NetMsgFB.emGrindFB = bSucceeded ? EMGRINDING_SUCCEED : EMGRINDING_FAIL;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	//	Note : �������� �Ӽ��� Ŭ���̾�Ʈ�� �˸�.
	//
	if ( !bTERMINATE )
	{
		GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
		NetItemUpdate.wPosX = pNetMsg->wPosX;
		NetItemUpdate.wPosY = pNetMsg->wPosY;
		NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemUpdate);
	}
	else
	{
		// ���� �ڽ�Ƭ�� �߶��� ������ �ڽ�Ƭ�� �и��ϰ� �����Ѵ�.
		if ( pInvenItem->sItemCustom.nidDISGUISE!=SNATIVEID(false) )
		{
			GLITEMLMT & glItemmt = GLITEMLMT::GetInstance();			
			GLItemMan & glItemMan = GLItemMan::GetInstance();

			SITEM *pONE = glItemMan.GetItem ( pInvenItem->sItemCustom.nidDISGUISE );
			if ( !pONE )		return E_FAIL;

			SITEMCUSTOM sITEM_NEW;
			sITEM_NEW.sNativeID = pInvenItem->sItemCustom.nidDISGUISE;
			CTime cTIME = CTime::GetCurrentTime();
			if ( pInvenItem->sItemCustom.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
			{
				cTIME = CTime(pInvenItem->sItemCustom.tDISGUISE);

				CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
				cTIME -= tLMT;
			}

			//	Note : ������ ����.
			//
			sITEM_NEW.tBORNTIME = cTIME.GetTime();
			sITEM_NEW.wTurnNum = 1;
			sITEM_NEW.cGenType = EMGEN_BILLING;
			sITEM_NEW.cChnID = (BYTE)m_pGLGaeaServer->GetServerChannel();
			sITEM_NEW.cFieldID = (BYTE)m_pGLGaeaServer->GetFieldSvrID();
			sITEM_NEW.lnGenNum = glItemmt.RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

			//	Note : hold ������ �κ��� �ǵ���.
			WORD wInsertPosX, wInsertPosY;
			BOOL bOk = m_cInventory.FindInsrtable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
			if ( !bOk )
			{
				// ������ �ڽ����� �ٴڿ��� �ٽ� ������.
				CItemDrop cDropItem;
				cDropItem.sItemCustom = sITEM_NEW;
				m_pLandMan->DropItem ( m_vPos, &(cDropItem.sItemCustom), EMGROUP_ONE, m_dwGaeaID );

				//	Note : �и��� �ڽ��� �α�.
				glItemmt.ReqItemRoute ( sITEM_NEW, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_GROUND, sITEM_NEW.wTurnNum );
			}else{
				//	����ִ� ������ �ι꿡 ����.
				m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
				SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

				//	[�ڽſ���] �޽��� �߻�.
				GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
				NetMsg_Inven.Data = *pINSERT_ITEM;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);
			}			
		}

		//	Note :�������� ���� ���� ��� ���.
		//
		GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pInvenItem->sItemCustom.wTurnNum );

		//	[�ڽſ���] �ι꿡 ������ ����.
		GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
		NetMsg_Inven_Delete.wPosX = pNetMsg->wPosX;
		NetMsg_Inven_Delete.wPosY = pNetMsg->wPosY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);

		//	������ ����.
		m_cInventory.DeleteItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqInvenResetSkSt ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_RESET_SKST *pNetMsg = (GLMSG::SNET_INVEN_RESET_SKST *)nmg;

	GLMSG::SNET_INVEN_RESET_SKST_FB	MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMREQ_RESET_SKST_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;



	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_SKP_STAT_RESET )
	{
		MsgFB.emFB = EMREQ_RESET_SKST_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	// ���� ���� Ȯ��
	if ( !SIMPLE_CHECK_ITEM( pITEM->sBasicOp.sNativeID ) )
	{		
		MsgFB.emFB = EMREQ_RESET_SKST_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);		
		return E_FAIL;
	}

	enum
	{
		EMGRADE				= 31,
		EMMARBLE_MID		= 104,

		EMGRADE_MIN			= 4,
	};

	WORD wNUM(0);
	WORD wGRADE[EMGRADE];
	memset(wGRADE,0,sizeof(WORD)*EMGRADE);

	SKILL_MAP_ITER pos = m_ExpSkills.begin();
	SKILL_MAP_ITER end = m_ExpSkills.end();
	for ( ; pos!=end; ++pos )
	{
		SNATIVEID sNID ( (*pos).first );

		PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData ( sNID );
		if ( !pSKILL )									continue;
		if ( EMGRADE_MIN > pSKILL->m_sBASIC.dwGRADE )	continue;

		wGRADE[pSKILL->m_sBASIC.dwGRADE] += 1;
		++wNUM;
	}

	BOOL bINSERT(TRUE);
	GLInventory cInven;
	cInven.Assign ( m_cInventory );
	for ( int i=EMGRADE_MIN; i<EMGRADE; ++i )
	for ( int j=0; j<wGRADE[i]; ++j )
	{
		SITEM *pITEM_SKILL = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
		if ( !pITEM_SKILL )		continue;

		SITEMCUSTOM sITEM_NEW;
		sITEM_NEW.sNativeID = SNATIVEID(EMMARBLE_MID,i);

		//	�������� ��� Ƚ�� ǥ��. ( �Ҹ�ǰ�� ��� x ��, �Ϲ� ��ǰ 1 )
		sITEM_NEW.wTurnNum = pITEM_SKILL->GETAPPLYNUM();

		BOOL bOK = cInven.InsertItem ( sITEM_NEW );
		if ( !bOK )
		{
			bINSERT = FALSE;
			break;
		}
	}

	if ( !bINSERT )
	{
		MsgFB.wITEM_NUM = wNUM;
		MsgFB.emFB = EMREQ_RESET_SKST_FB_NOINVEN;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	for ( i=EMGRADE_MIN; i<EMGRADE; ++i )
	for ( int j=0; j<wGRADE[i]; ++j )
	{
		SITEM *pITEM_SKILL = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
		if ( !pITEM_SKILL )		continue;

		SITEMCUSTOM sITEM_NEW;
		CTime cTIME = CTime::GetCurrentTime();

		//	������ �߻�.
		sITEM_NEW.sNativeID = SNATIVEID(EMMARBLE_MID,i);
		sITEM_NEW.tBORNTIME = cTIME.GetTime();

		//	�������� ��� Ƚ�� ǥ��. ( �Ҹ�ǰ�� ��� x ��, �Ϲ� ��ǰ 1 )
		sITEM_NEW.wTurnNum = pITEM_SKILL->GETAPPLYNUM();

		//	�߻��� ���� ���.
		sITEM_NEW.cGenType = EMGEN_INIT;
		sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, EMGEN_INIT );

		BOOL bOK;
		WORD wPosX(0), wPosY(0);
		bOK = m_cInventory.FindInsrtable ( pITEM_SKILL->sBasicOp.wInvenSizeX, pITEM_SKILL->sBasicOp.wInvenSizeY, wPosX, wPosY );
		if ( !bOK )		continue;

		bOK = m_cInventory.InsertItem ( sITEM_NEW, wPosX, wPosY );
		if ( !bOK )		continue;

		SINVENITEM *pInvenItem = m_cInventory.GetItem ( wPosX, wPosY );
		if (!pInvenItem) continue;

		//	������ ȹ�� �α� ����.
		GLITEMLMT::GetInstance().ReqItemRoute ( sITEM_NEW, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, sITEM_NEW.wTurnNum );

		//	[�ڽſ���] �޽��� �߻�.
		GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
		NetMsg_Inven.Data = *pInvenItem;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);

	}

	//	Note : ��ų, ���� ����.
	//
	RESET_STATS_SKILL ( pITEM->sDrugOp.wCureVolume );

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//	Note : ���� �޽��� ����.
	//
	MsgFB.emFB = EMREQ_RESET_SKST_FB_OK;
	MsgFB.wITEM_NUM = wNUM;
	MsgFB.wSTATS_POINT = pITEM->sDrugOp.wCureVolume;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	return S_OK;
}


HRESULT GLChar::MsgReqCharCard ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_CHARCARD *pNetMsg = (GLMSG::SNET_INVEN_CHARCARD *)nmg;

	GLMSG::SNET_INVEN_CHARCARD_FB	MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMREQ_CHARCARD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_CHARACTER_CARD )
	{
		MsgFB.emFB = EMREQ_CHARCARD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//	Note : �ɸ��� ���� ���� ����.
	//
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CItemChaCreateNumIncrease *pDbAction = new CItemChaCreateNumIncrease ( m_dwUserID );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
	}

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//	Note : ó�� ���� �˸�.
	MsgFB.emFB = EMREQ_CHARCARD_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqStorageCard ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_STORAGECARD *pNetMsg = (GLMSG::SNET_INVEN_STORAGECARD *)nmg;

	GLMSG::SNET_INVEN_STORAGECARD_FB	MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMREQ_STORAGECARD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_STORAGE_CARD )
	{
		MsgFB.emFB = EMREQ_STORAGECARD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( pNetMsg->wSTORAGE < EMSTORAGE_CHANNEL_SPAN || pNetMsg->wSTORAGE>=(EMSTORAGE_CHANNEL_SPAN+EMSTORAGE_CHANNEL_SPAN_SIZE) )
	{
		MsgFB.emFB = EMREQ_STORAGECARD_FB_INVNUM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//	Note : â�� ���� �ð� ����.
	//
	int nINDEX = pNetMsg->wSTORAGE-EMSTORAGE_CHANNEL_SPAN;
	__time64_t &tTIME = m_tSTORAGE[nINDEX];

	CTime tCurTime = CTime::GetCurrentTime();
	CTime tSeedTime(tTIME);
	CTimeSpan tLMT (pITEM->sDrugOp.tTIME_LMT);
	CTimeSpan tADD(0,1,30,0);

	//	Note : ���� �ʱ�ȭ ���� �ʾҰų�, �����ð� �ʰ��Ǿ��� ���.
	if ( tTIME == 0 || tSeedTime < tCurTime )
	{
		tSeedTime = tCurTime + tLMT + tADD;
	}
	//	Note : �����ð��� ���� ������ ������.
	else
	{
		tSeedTime = tSeedTime + tLMT + tADD;
	}

	//	Note : ���ð� ������, ��� �������� ����.
	tTIME = tSeedTime.GetTime();	//	m_tSTORAGE[nINDEX] = time;
	m_bSTORAGE[nINDEX] = true;

	//	Note : DB�� ���丮�� �����ð� ����.
	//
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CSetChaStorageDate *pDbAction = new CSetChaStorageDate ( m_dwUserID, nINDEX+EMSTORAGE_CHANNEL_SPAN, tTIME );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
	}

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//	Note : ó�� ��� �˸�.
	CTimeSpan tDX = tSeedTime - tCurTime;
	MsgFB.emFB = EMREQ_STORAGECARD_FB_OK;
	MsgFB.tSTORAGE_LIMIT = tSeedTime.GetTime();
	MsgFB.tSPAN = tDX.GetTimeSpan();
	MsgFB.wSTORAGE = pNetMsg->wSTORAGE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqInvenLine ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_INVENLINE *pNetMsg = (GLMSG::SNET_INVEN_INVENLINE *)nmg;

	GLMSG::SNET_INVEN_INVENLINE_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMREQ_INVENLINE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_INVEN_CARD )
	{
		MsgFB.emFB = EMREQ_INVENLINE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( m_wINVENLINE >= (EM_INVENSIZE_Y - EM_INVEN_DEF_SIZE_Y - EM_INVEN_PREMIUM_SIZE) )
	{
		MsgFB.emFB = EMREQ_INVENLINE_FB_MAXLINE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//	Note : �κ� ���� ����.
	//
	m_wINVENLINE += 1;

	//	Note : ���� Ȱ��ȭ�� �κ� ���� ����.
	//
	m_cInventory.SetAddLine ( GetOnINVENLINE(), true );

	//	Note : DB�� ����.
	//
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CSetChaInvenNum *pDbAction = new CSetChaInvenNum ( m_dwCharID, m_wINVENLINE );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
	}

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//	Note : ó�� ��� �˸�.
	MsgFB.emFB = EMREQ_INVENLINE_FB_OK;
	MsgFB.wINVENLINE = m_wINVENLINE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	return S_OK;
}

// *****************************************************
// Desc: ���� ��� �̿�ī�� ��û ( ī�常 �Ҹ��Ŵ )
// *****************************************************
HRESULT GLChar::MsgReqInvenRemodelOpen ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_REMODELOPEN *pNetMsg = (GLMSG::SNET_INVEN_REMODELOPEN *)nmg;

	GLMSG::SNET_INVEN_REMODELOPEN_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMREQ_REMODELOPEN_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_REMODEL )
	{
		MsgFB.emFB = EMREQ_REMODELOPEN_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	m_ItemRemodelPosX = pNetMsg->wPosX;
	m_ItemRemodelPosY = pNetMsg->wPosY;

	MsgFB.emFB = EMREQ_REMODELOPEN_FB_OK;

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	return S_OK;
}


HRESULT GLChar::MsgReqInvenGarbageOpen ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_GARBAGEOPEN *pNetMsg = (GLMSG::SNET_INVEN_GARBAGEOPEN *)nmg;

	GLMSG::SNET_INVEN_GARBAGEOPEN_FB MsgFB;

	if ( !pNetMsg->bInven )
	{
		SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
		if ( !pINVENITEM )
		{
			MsgFB.emFB = EMREQ_GARBAGEOPEN_FB_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
		if ( pITEM->sBasicOp.emItemType!=ITEM_GARBAGE_CARD )
		{
			MsgFB.emFB = EMREQ_GARBAGEOPEN_FB_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		m_ItemGarbagePosX = pNetMsg->wPosX;
		m_ItemGarbagePosY = pNetMsg->wPosY;
	}
	MsgFB.emFB = EMREQ_GARBAGEOPEN_FB_OK;

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	return S_OK;
}

// *****************************************************
// Desc: ��� â�� �̿�ī�� ��û ( ī�常 �Ҹ��Ŵ )
// *****************************************************
HRESULT GLChar::MsgReqInvenStorageOpen ( NET_MSG_GENERIC* nmg )
{
	// â������ī�带 ������̸�
	if ( m_bUsedStorageCard ) return E_FAIL;

	GLMSG::SNET_INVEN_STORAGEOPEN *pNetMsg = (GLMSG::SNET_INVEN_STORAGEOPEN *)nmg;

	GLMSG::SNET_INVEN_STORAGEOPEN_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMREQ_STORAGEOPEN_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_STORAGE_CONNECT )
	{
		MsgFB.emFB = EMREQ_STORAGEOPEN_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	MsgFB.emFB = EMREQ_STORAGEOPEN_FB_OK;
	MsgFB.wPosX = pNetMsg->wPosX;
	MsgFB.wPosY = pNetMsg->wPosY;

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	// â������ī�� ��뿩��
	m_bUsedStorageCard = true;

	return S_OK;
}

// *****************************************************
// Desc: ��� â�� ��� �Ϸ�
// *****************************************************
HRESULT GLChar::MsgReqInvenStorageClose ( NET_MSG_GENERIC* nmg )
{
	// â������ī�� ��뿩��
	m_bUsedStorageCard = false;

	return S_OK;
}

HRESULT GLChar::MsgReqInvenPremiumSet ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_PREMIUMSET *pNetMsg = (GLMSG::SNET_INVEN_PREMIUMSET *)nmg;

	GLMSG::SNET_INVEN_PREMIUMSET_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMREQ_PREMIUMSET_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_PREMIUMSET )
	{
		MsgFB.emFB = EMREQ_PREMIUMSET_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//	Note : �κ��� ���� ���� ����.
	//
	GLInventory cInvenTemp;
	cInvenTemp.SetAddLine ( m_cInventory.GETAddLine(), true );
	cInvenTemp.Assign ( m_cInventory );

	for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		SITEMCUSTOM sCUSTOM;
		sCUSTOM.sNativeID = pITEM->sBox.sITEMS[i].nidITEM;
		if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pONE = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
		if ( !pONE )
		{
			MsgFB.emFB = EMREQ_PREMIUMSET_FB_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		BOOL bOK = cInvenTemp.InsertItem ( sCUSTOM );
		if ( !bOK )
		{
			//	Note : �κ��� ������ ���� ������ �Ǵܵ�.
			//
			MsgFB.emFB = EMREQ_PREMIUMSET_FB_NOTINVEN;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}
	}

	//	Note : ���ھȿ� �ִ� �������� ����� �ִ´�.
	//
	for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		SITEMCUSTOM sITEM_NEW;
		sITEM_NEW.sNativeID = pITEM->sBox.sITEMS[i].nidITEM;
		DWORD dwAMOUNT = pITEM->sBox.sITEMS[i].dwAMOUNT;
		if ( sITEM_NEW.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pONE = GLItemMan::GetInstance().GetItem ( sITEM_NEW.sNativeID );
		if ( !pONE )											return E_FAIL;

		//	Note : ������ ����.
		//
		CTime cTIME = CTime::GetCurrentTime();
		sITEM_NEW.tBORNTIME = cTIME.GetTime();

		sITEM_NEW.wTurnNum = (WORD) dwAMOUNT;
		sITEM_NEW.cGenType = pINVENITEM->sItemCustom.cGenType;
		sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

		// ������ ����Ʈ���� �Է��� ���� ��� ���� ( ���� )
		sITEM_NEW.cDAMAGE = (BYTE)pONE->sBasicOp.wGradeAttack;
		sITEM_NEW.cDEFENSE = (BYTE)pONE->sBasicOp.wGradeDefense;

		//	���� �ɼ� ����.
		if( sITEM_NEW.GENERATE_RANDOM_OPT() )
		{
			GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
		}

		//	Note :.
		WORD wInsertPosX, wInsertPosY;
		BOOL bOk = m_cInventory.FindInsrtable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
		if ( !bOk )		return S_OK;	//	�κ� ���� �������� �ǵ��� ����.

		//	.
		m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
		SINVENITEM *pInvenItem = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		if ( !pInvenItem )	return E_FAIL;

		//	Note : �α� ����.
		GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pInvenItem->sItemCustom.wTurnNum );

		//	[�ڽſ���] �޽��� �߻�.
		GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
		NetMsg_Inven.Data = *pInvenItem;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);
	}

	//	Note : �����̾� ����.
	//
	CTime tPREMIUM(m_tPREMIUM);
	CTime tCurTime = CTime::GetCurrentTime();
	CTimeSpan tLMT(pITEM->sDrugOp.tTIME_LMT);
	CTimeSpan tADD(0,1,30,0);

	if ( m_tPREMIUM==0 || tPREMIUM < tCurTime )
	{
		tPREMIUM = tCurTime + tLMT + tADD;
	}
	else
	{
		tPREMIUM = tPREMIUM + tLMT + tADD;
	}

	//	Note : �����̾� ��� ON.
	//
	m_tPREMIUM = tPREMIUM.GetTime();
	m_bPREMIUM = true;

	//	Note : ���� Ȱ��ȭ�� �κ� ���� ����.
	//
	m_cInventory.SetAddLine ( GetOnINVENLINE(), true );

	//	Note : DB�� ����.
	//
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CSetPremiumTime *pDbAction = new CSetPremiumTime ( m_dwUserID, m_tPREMIUM );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
	}

	// Note : CSetPremiumTime Ŭ�������� ��� ��츦 ó���ϵ��� �����
	/*
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CDaumSetPremiumTime *pDbAction = new CDaumSetPremiumTime ( m_dwUserID, m_tPREMIUM );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
	}
	*/
	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );


	//	Note : ������Ʈ�� ���ļ� �޽��� ����.
	//		( ������Ʈ�� ��ϵǾ� �ִ� �����̾� �Ⱓ�� ���� ������� �Ѵ�. )
	//
	CTimeSpan tDX = tPREMIUM - tCurTime;
	MsgFB.emFB = EMREQ_PREMIUMSET_FB_OK;
	MsgFB.tLMT = m_tPREMIUM;
	MsgFB.tSPAN = tDX.GetTimeSpan();
	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID,&MsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqInvenHairChange ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_HAIR_CHANGE *pNetMsg = (GLMSG::SNETPC_INVEN_HAIR_CHANGE *)nmg;

	GLMSG::SNETPC_INVEN_HAIR_CHANGE_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_HAIR )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	bool bVALIDCLASS = (NULL!=(pITEM->sBasicOp.dwReqCharClass&m_emClass));
	if ( !bVALIDCLASS )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_BADCLASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	WORD wHAIR = pITEM->sSuitOp.wReModelNum;

	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	const GLCONST_CHARCLASS& sCHARCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];
	if ( sCHARCONST.dwHAIRNUM<=wHAIR || MAX_HAIR<=wHAIR )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( sCHARCONST.strHAIR_CPS[wHAIR].empty() )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//	Note : �� ��ȭ ��Ŵ.
	//
	m_wHair = wHAIR;

	//	Note : Ŭ���̾�Ʈ�� ó�� ��� ����.
	MsgFB.dwID = m_wHair;
	MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	//	Note : ��� ������ �ٸ� ����鿡�� �˸�.
	GLMSG::SNETPC_INVEN_HAIR_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.dwID = m_wHair;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	return S_OK;
}

HRESULT GLChar::MsgReqInvenHairStyleChange ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_HAIRSTYLE_CHANGE *pNetMsg = (GLMSG::SNETPC_INVEN_HAIRSTYLE_CHANGE *)nmg;

	GLMSG::SNETPC_INVEN_HAIR_CHANGE_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	
	if ( pITEM->sBasicOp.emItemType!=ITEM_HAIR_STYLE )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}


	bool bVALIDCLASS = (NULL!=(pITEM->sBasicOp.dwReqCharClass&m_emClass));
	if ( !bVALIDCLASS )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_BADCLASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//WORD wHAIR = pITEM->sSuitOp.wReModelNum;

	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	const GLCONST_CHARCLASS& sCHARCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];
	if ( sCHARCONST.dwHAIRNUM<=pNetMsg->wHairStyle || MAX_HAIR<=pNetMsg->wHairStyle )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	m_wHair = pNetMsg->wHairStyle;

	//	Note : Ŭ���̾�Ʈ�� ó�� ��� ����.
	MsgFB.dwID = m_wHair;
	MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	//	Note : ��� ������ �ٸ� ����鿡�� �˸�.
	GLMSG::SNETPC_INVEN_HAIR_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.dwID = m_wHair;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	// DB�� ����
	CSetChaHairStyle* pDBAction = new CSetChaHairStyle ( m_dwCharID, m_wHair );

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDBAction );
	}


	return S_OK;
}

HRESULT GLChar::MsgReqInvenHairColorChange ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_HAIRCOLOR_CHANGE *pNetMsg = (GLMSG::SNETPC_INVEN_HAIRCOLOR_CHANGE *)nmg;

	GLMSG::SNETPC_INVEN_HAIRCOLOR_CHANGE_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );

	if ( pITEM->sBasicOp.emItemType!=ITEM_HAIR_COLOR )
	{
		MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//	Note : �� ��ȭ ��Ŵ.
	//
	m_wHairColor = pNetMsg->wHairColor;

	//	Note : Ŭ���̾�Ʈ�� ó�� ��� ����.
	MsgFB.wHairColor = m_wHairColor;
	MsgFB.emFB = EMINVEN_HAIR_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	//	Note : ��� ������ �ٸ� ����鿡�� �˸�.
	GLMSG::SNETPC_INVEN_HAIRCOLOR_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.wHairColor = m_wHairColor;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	// DB�� ����
	CSetChaHairColor* pDBAction = new CSetChaHairColor ( m_dwCharID, m_wHairColor );

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDBAction );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqInvenBikeColorChange ( NET_MSG_GENERIC* nmg ) //add bike color
{
	GLMSG::SNETPC_INVEN_BIKECOLOR_CHANGE *pNetMsg = (GLMSG::SNETPC_INVEN_BIKECOLOR_CHANGE *)nmg;

	GLMSG::SNETPC_INVEN_BIKECOLOR_CHANGE_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_BIKE_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.emFB = EMINVEN_BIKE_CHANGE_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );

	if ( pITEM->sBasicOp.emItemType!=ITEM_BIKE_COLOR )
	{
		MsgFB.emFB = EMINVEN_BIKE_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	m_sVehicle.m_wColor = pNetMsg->wBikeColor;
	m_sVehicle.m_wColor1 = pNetMsg->wBikeColor1;
	m_sVehicle.m_wColor2 = pNetMsg->wBikeColor2;
	m_sVehicle.m_wColor3 = pNetMsg->wBikeColor3;
	m_sVehicle.m_wColor4 = pNetMsg->wBikeColor4;
	m_sVehicle.m_wColor5 = pNetMsg->wBikeColor5;

	MsgFB.wBikeColor = m_sVehicle.m_wColor;
	MsgFB.wBikeColor1 = m_sVehicle.m_wColor1;
	MsgFB.wBikeColor2 = m_sVehicle.m_wColor2;
	MsgFB.wBikeColor3 = m_sVehicle.m_wColor3;
	MsgFB.wBikeColor4 = m_sVehicle.m_wColor4;
	MsgFB.wBikeColor5 = m_sVehicle.m_wColor5;
	MsgFB.emFB = EMINVEN_BIKE_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	GLMSG::SNETPC_INVEN_BIKECOLOR_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.wBikeColor = m_sVehicle.m_wColor;
	NetMsgBrd.wBikeColor1 = m_sVehicle.m_wColor1;
	NetMsgBrd.wBikeColor2 = m_sVehicle.m_wColor2;
	NetMsgBrd.wBikeColor3 = m_sVehicle.m_wColor3;
	NetMsgBrd.wBikeColor4 = m_sVehicle.m_wColor4;
	NetMsgBrd.wBikeColor5 = m_sVehicle.m_wColor5;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	// DB�� ���� //jdev todo save color after change
	CSetVehicleColor* pDBAction = new CSetVehicleColor ( m_dwClientID, m_dwCharID, m_sVehicle.m_dwGUID ,m_sVehicle.m_wColor,m_sVehicle.m_wColor1,m_sVehicle.m_wColor2,m_sVehicle.m_wColor3,m_sVehicle.m_wColor4,m_sVehicle.m_wColor5 );

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDBAction );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqSlotItemColorChange ( NET_MSG_GENERIC* nmg ) //add itemcolor
{
	GLMSG::SNETPC_SLOT_ITEMCOLOR_CHANGE *pNetMsg = (GLMSG::SNETPC_SLOT_ITEMCOLOR_CHANGE *)nmg;

	GLMSG::SNETPC_SLOT_ITEMCOLOR_CHANGE_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_ITEMCOLOR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	SITEMCUSTOM& sSLOTITEM = m_PutOnItems[pNetMsg->emSlot];
	SITEM* pSlot = GLItemMan::GetInstance().GetItem ( sSLOTITEM.sNativeID );
	if ( !pSlot )
	{
		MsgFB.emFB = EMINVEN_ITEMCOLOR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.emFB = EMINVEN_ITEMCOLOR_CHANGE_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );

	if ( pITEM->sBasicOp.emItemType!=ITEM_ITEM_COLOR )
	{
		MsgFB.emFB = EMINVEN_ITEMCOLOR_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	m_PutOnItems[pNetMsg->emSlot].wColor = pNetMsg->wItemColor;

	MsgFB.wItemColor = m_PutOnItems[pNetMsg->emSlot].wColor;
	MsgFB.emSlot = pNetMsg->emSlot;
	MsgFB.emFB = EMINVEN_ITEMCOLOR_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	GLMSG::SNETPC_SLOT_ITEMCOLOR_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.wItemColor = m_PutOnItems[pNetMsg->emSlot].wColor;
	NetMsgBrd.emSlot = pNetMsg->emSlot;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	GLITEMLMT::GetInstance().ReqItemConversion ( sSLOTITEM, ID_CHAR, m_dwCharID );

	return S_OK;
}


HRESULT GLChar::MsgReqInvenFaceChange ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_FACE_CHANGE *pNetMsg = (GLMSG::SNETPC_INVEN_FACE_CHANGE *)nmg;

	GLMSG::SNETPC_INVEN_FACE_CHANGE_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);		
		return E_FAIL;
	}

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_FACE )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	bool bVALIDCLASS = (NULL!=(pITEM->sBasicOp.dwReqCharClass&m_emClass));
	if ( !bVALIDCLASS )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_BADCLASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	WORD wFACE = pITEM->sSuitOp.wReModelNum;

	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	const GLCONST_CHARCLASS& sCHARCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];
	if ( sCHARCONST.dwHEADNUM<=wFACE || MAX_HEAD<=wFACE )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( sCHARCONST.strHEAD_CPS[wFACE].empty() )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//	Note : ���� ��ȭ ��Ŵ.
	//
	m_wFace = wFACE;

	//	Note : Ŭ���̾�Ʈ�� ó�� ��� ����.
	MsgFB.dwID = m_wFace;
	MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	//	Note : �� ������ �ٸ� ����鿡�� �˸�.
	GLMSG::SNETPC_INVEN_FACE_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.dwID = m_wFace;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	return S_OK;
}

HRESULT GLChar::MsgReqInvenFaceStyleChange ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_FACESTYLE_CHANGE *pNetMsg = (GLMSG::SNETPC_INVEN_FACESTYLE_CHANGE *)nmg;

	GLMSG::SNETPC_INVEN_FACE_CHANGE_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{		
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	
	if ( pITEM->sBasicOp.emItemType!=ITEM_FACE_STYLE )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}


	bool bVALIDCLASS = (NULL!=(pITEM->sBasicOp.dwReqCharClass&m_emClass));
	if ( !bVALIDCLASS )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_BADCLASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//WORD wHAIR = pITEM->sSuitOp.wReModelNum;

	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	const GLCONST_CHARCLASS& sCHARCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];
	if ( sCHARCONST.dwHEADNUM<=pNetMsg->wFaceStyle || MAX_HEAD<=pNetMsg->wFaceStyle )
	{
		MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	m_wFace = pNetMsg->wFaceStyle;

	//	Note : Ŭ���̾�Ʈ�� ó�� ��� ����.
	MsgFB.dwID = m_wFace;
	MsgFB.emFB = EMINVEN_FACE_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	//	Note : ��� ������ �ٸ� ����鿡�� �˸�.
	GLMSG::SNETPC_INVEN_FACE_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.dwID = m_wFace;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	// DB�� ����
	CSetChaFaceStyle* pDBAction = new CSetChaFaceStyle ( m_dwCharID, m_wFace );

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDBAction );
	}


	return S_OK;

}


HRESULT GLChar::MsgReqInvenGenderChange( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_GENDER_CHANGE *pNetMsg = (GLMSG::SNETPC_INVEN_GENDER_CHANGE *)nmg;
	GLMSG::SNETPC_INVEN_GENDER_CHANGE_FB MsgFB;


	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_GENDER_CHANGE_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	
	if ( pITEM->sBasicOp.emItemType!=ITEM_GENDER_CHANGE )
	{
		MsgFB.emFB = EMINVEN_GENDER_CHANGE_FB_ITEMTYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}


	//fix change gender
	//if ( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_EXTREME_W )
	//{
	//	MsgFB.emFB = EMINVEN_GENDER_CHANGE_FB_BADCLASS;
	//	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
	//	return E_FAIL;
	//}

	//fix for proper class reverse
	EMCHARCLASS emClass;
    	
	//if ( m_emClass > GLCC_EXTREME_M &&  m_emClass < GLCC_SCIENCE_M )	emClass = (EMCHARCLASS) (m_emClass / 64 );
	//else if ( m_emClass < GLCC_EXTREME_M ) emClass = (EMCHARCLASS) (m_emClass * 64);

	if ( m_emClass == GLCC_FIGHTER_M || m_emClass == GLCC_ARMS_M || m_emClass == GLCC_ARCHER_W || m_emClass == GLCC_SPIRIT_W )  emClass = (EMCHARCLASS) (m_emClass * 64);
	else if ( m_emClass == GLCC_FIGHTER_W || m_emClass == GLCC_ARMS_W || m_emClass == GLCC_ARCHER_M || m_emClass == GLCC_SPIRIT_M )  emClass = (EMCHARCLASS) (m_emClass / 64);
	else if ( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_SCIENCE_M || m_emClass == GLCC_ASSASIN_M || m_emClass == GLCC_TESTING_M || m_emClass == GLCC_SHAPER_M )  emClass = (EMCHARCLASS) (m_emClass * 2 );
	else if ( m_emClass == GLCC_EXTREME_W || m_emClass == GLCC_SCIENCE_W || m_emClass == GLCC_ASSASIN_W || m_emClass == GLCC_TESTING_W || m_emClass == GLCC_SHAPER_W )  emClass = (EMCHARCLASS) (m_emClass / 2 );


	EMCHARINDEX emIndex = CharClassToIndex(emClass);
	const GLCONST_CHARCLASS& sCHARCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];

	if ( pNetMsg->wFace >= sCHARCONST.dwHEADNUM || pNetMsg->wHair >= sCHARCONST.dwHAIRNUM_SELECT )
	{
		MsgFB.emFB = EMINVEN_GENDER_CHANGE_FB_NOTVALUE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

    m_emClass = emClass;
	m_wHair = pNetMsg->wHair;
	m_wFace = pNetMsg->wFace;	
	m_wHairColor = HAIRCOLOR::GetHairColor( emIndex, m_wHair );

	m_wSex = m_wSex ? 0 : 1;


	//	Note : Ŭ���̾�Ʈ�� ó�� ��� ����.
	MsgFB.emClass	= m_emClass;
	MsgFB.wSex		= m_wSex;
	MsgFB.wFace = m_wFace;
	MsgFB.wHair = m_wHair;
	MsgFB.wHairColor = m_wHairColor;

	MsgFB.emFB = EMINVEN_GENDER_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	// DB�� ����
	CSetChaGenderChange* pDBAction = new CSetChaGenderChange ( m_dwCharID, m_emClass, m_wSex, m_wFace, m_wHair, m_wHairColor );

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDBAction );
	}

	return S_OK;
}

HRESULT GLChar::MsgReqInvenChargedItem ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_CHARGED_ITEM_GET *pNetMsg = (GLMSG::SNET_CHARGED_ITEM_GET *) nmg;

	GLMSG::SNET_CHARGED_ITEM_GET_FB NetMsgFB;

	MAPSHOP_ITER iter = m_mapCharged.find ( pNetMsg->szPurKey );
	if ( iter==m_mapCharged.end() )
	{
		NetMsgFB.emFB = EMCHARGED_ITEM_GET_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SHOPPURCHASE &sPURCHASE = (*iter).second;
	
	SNATIVEID nidITEM(sPURCHASE.wItemMain,sPURCHASE.wItemSub);
	
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( nidITEM );
	if ( !pITEM )
	{
		NetMsgFB.emFB = EMCHARGED_ITEM_GET_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	WORD wPosX, wPosY;
	BOOL bOK = m_cInventory.FindInsrtable ( pITEM->sBasicOp.wInvenSizeX, pITEM->sBasicOp.wInvenSizeY, wPosX, wPosY );
	if ( !bOK )
	{
		NetMsgFB.emFB = EMCHARGED_ITEM_GET_FB_NOINVEN;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	//	Note : ������ ������. DB Action.
	//
	CPurchaseItem_Get *pDbAction = new CPurchaseItem_Get(m_dwClientID,
		                                                 m_dwUserID,
														 pNetMsg->szPurKey,
														 nidITEM,pNetMsg->dwID);
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
	}

	return S_OK;
}

// *****************************************************
// Desc: �̸������û ó��
// *****************************************************
HRESULT GLChar::MsgReqInvenRename ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_RENAME *pNetMsg = (GLMSG::SNETPC_INVEN_RENAME*) nmg;

	GLMSG::SNETPC_INVEN_RENAME_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_RENAME_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_RENAME )
	{
		MsgFB.emFB = EMINVEN_RENAME_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}
	
	// ĳ���͸�
	CString strChaName(pNetMsg->szName);
	strChaName.Trim(_T(" ")); // �յ� ��������

	// �����̽� ã��, ĳ�����̸� 4���� ���� ����, ĳ���� ���� �� ����.
	if ((strChaName.FindOneOf(" ") != -1) || (strChaName.GetLength() < 4)) 
	{
		MsgFB.emFB = EMINVEN_RENAME_FB_LENGTH;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	BOOL bFILTER0 = STRUTIL::CheckString( strChaName );
	BOOL bFILTER1 = CRanFilter::GetInstance().NameFilter( strChaName );
	if ( bFILTER0 || bFILTER1 )
	{
		MsgFB.emFB = EMINVEN_RENAME_FROM_DB_BAD;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &MsgFB );
		return S_FALSE;			
	}

#ifdef TH_PARAM	
	// �±��� ���� ���� üũ 
	if ( !m_pCheckString ) return E_FAIL;

	if ( !m_pCheckString(strChaName) )
	{
		MsgFB.emFB = EMINVEN_RENAME_FB_THAICHAR_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}
#endif

#ifdef VN_PARAM
	// ��Ʈ�� ���� ���� üũ 
	if( STRUTIL::CheckVietnamString( strChaName ) )
	{
		MsgFB.emFB = EMINVEN_RENAME_FB_VNCHAR_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

#endif 

	// DB�� �����û
	CRenameCharacter* pDBAction = new CRenameCharacter(m_dwClientID, 
		                                               m_dwCharID,
													   pNetMsg->szName,
													   pNetMsg->wPosX,
													   pNetMsg->wPosY);
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDBAction );
	}

	return S_OK;
}

// *****************************************************
// Desc: �̸����� ��� ó��
// *****************************************************
HRESULT GLChar::MsgInvenRename ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_RENAME_FROM_DB *pNetMsg = (GLMSG::SNETPC_INVEN_RENAME_FROM_DB *)nmg;
	
	// Ŭ���̾�Ʈ�� ó�� ��� ����.
	GLMSG::SNETPC_INVEN_RENAME_FB MsgFB;

	if( pNetMsg->emFB == EMINVEN_RENAME_FROM_DB_BAD )
	{
		MsgFB.emFB = EMINVEN_RENAME_FROM_DB_BAD;
		StringCchCopy ( MsgFB.szName, CHAR_SZNAME, pNetMsg->szName );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return S_FALSE;
	}
	else if ( pNetMsg->emFB == EMINVEN_RENAME_FROM_DB_FAIL )
	{
		MsgFB.emFB = EMINVEN_RENAME_FROM_DB_FAIL;
		StringCchCopy ( MsgFB.szName, CHAR_SZNAME, pNetMsg->szName );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return S_FALSE;
	}

	StringCchCopy ( MsgFB.szName, CHAR_SZNAME, pNetMsg->szName );
	MsgFB.emFB = EMINVEN_RENAME_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	// �ֺ��� �̸������� �˸�.
	GLMSG::SNETPC_INVEN_RENAME_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	StringCchCopy ( NetMsgBrd.szName, CHAR_SZNAME, pNetMsg->szName );
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	// GLLandMan �̸��� ����.
	GLMSG::SNETPC_CHANGE_NAMEMAP NetMsgNameMap;
	NetMsgNameMap.dwGaeaID = m_dwGaeaID;
	StringCchCopy ( NetMsgNameMap.szOldName, CHAR_SZNAME, m_szName );
	StringCchCopy ( NetMsgNameMap.szNewName, CHAR_SZNAME, pNetMsg->szName );
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgNameMap );

	// �̸������� ģ���� Ŭ�� �ɹ��鿡�� �˸�
	GLMSG::SNETPC_INVEN_RENAME_AGTBRD NetMsgAgt;
	NetMsgAgt.dwID = m_dwGaeaID;
	StringCchCopy ( NetMsgAgt.szOldName, CHAR_SZNAME, m_szName );
	StringCchCopy ( NetMsgAgt.szNewName, CHAR_SZNAME, pNetMsg->szName );
	GLGaeaServer::GetInstance().SENDTOAGENT ( m_dwClientID, &NetMsgAgt );

	GLGaeaServer::GetInstance().ChangeNameMap ( this, m_szName, pNetMsg->szName );

	//	[�ڽſ���] �ι꿡 ������ ����.
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	return S_OK;
}

HRESULT GLChar::MsgReqSendSMS( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_SEND_SMS *pNetMsg = (GLMSG::SNETPC_SEND_SMS*) nmg;

	GLMSG::SNETPC_SEND_SMS_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wItemPosX, pNetMsg->wItemPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMSMS_SEND_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( pITEM->sBasicOp.emItemType!=ITEM_SMS )
	{
		MsgFB.emFB = EMSMS_SEND_FB_BADITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	// DB�� �����û
	CSendSMS* pDBAction = new CSendSMS( m_dwClientID,
										m_dwCharID,
										pNetMsg->dwReceiveChaNum,
										pNetMsg->szPhoneNumber,
										m_szName,
										pNetMsg->szSmsMsg,
										pNetMsg->wItemPosX,
										pNetMsg->wItemPosY );

	if( GLGaeaServer::GetInstance().GetDBMan() )
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob( pDBAction );
	}

	return S_OK;
}

// *****************************************************
// Desc: SMS ���� ��� ó��
// *****************************************************
HRESULT GLChar::MsgSendSMS( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_SEND_SMS_FROM_DB *pNetMsg = (GLMSG::SNETPC_SEND_SMS_FROM_DB *)nmg;

	// Ŭ���̾�Ʈ�� ó�� ��� ����.
	GLMSG::SNETPC_SEND_SMS_FB MsgFB;

	if ( pNetMsg->emFB == EMSMS_SEND_FROM_DB_FAIL )
	{
		MsgFB.emFB = EMSMS_SEND_FROM_DB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return S_FALSE;
	}

	MsgFB.emFB = EMSMS_SEND_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	return TRUE;
}

HRESULT GLChar::MsgLoudSpeaker ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_CHAT_LOUDSPEAKER *pNetMsg = (GLMSG::SNETPC_CHAT_LOUDSPEAKER *) nmg;

	GLMSG::SNETPC_CHAT_LOUDSPEAKER_FB	NetMsgFB;

	bool bchannel_all(false);

	if ( m_dwUserLvl < USER_GM3 )
	{
		SINVENITEM *pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
		if ( !pINVENITEM )
		{
			NetMsgFB.emFB = EMCHAT_LOUDSPEAKER_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_OK;
		}

		if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
		if ( !pITEM )
		{
			NetMsgFB.emFB = EMCHAT_LOUDSPEAKER_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_OK;
		}

		if ( pITEM->sBasicOp.emItemType!=ITEM_LOUDSPEAKER )
		{
			NetMsgFB.emFB = EMCHAT_LOUDSPEAKER_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			return S_OK;
		}

		if ( IsCHATBLOCK() )	return S_FALSE;

		bchannel_all = pITEM->sBasicOp.IsCHANNEL_ALL();

		if ( m_sPMarket.IsOpen() ) 
		{
			DWORD wTurn = m_cInventory.CountTurnItem( pINVENITEM->sItemCustom.sNativeID );
			DWORD wMarketTurn = m_sPMarket.GetItemTurnNum( pINVENITEM->sItemCustom.sNativeID ) ;

			if ( wTurn <= wMarketTurn ) 
			{
				NetMsgFB.emFB = EMCHAT_LOUDSPEAKER_NOITEM;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
				return S_OK;
			}
		}

		//	[�ڽſ���] �ι� ������ �Ҹ��Ŵ.
		DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	}

	//	Note : Ȯ���� ���.
	//
	GLMSG::SNETPC_CHAT_LOUDSPEAKER_AGT NetMsgAgt;
	StringCbCopy( NetMsgAgt.szMSG, CHAT_MSG_SIZE+1, pNetMsg->szMSG);
	NetMsgAgt.bchannel_all = bchannel_all;
	NetMsgAgt.sITEMLINK = pNetMsg->sITEMLINK;
	GLGaeaServer::GetInstance().SENDTOAGENT ( m_dwClientID, &NetMsgAgt );

	return S_OK;
}

HRESULT GLChar::MsgReqRebuildResult( NET_MSG_GENERIC* nmg ) //sealed card NaJDeV
{
	GLMSG::SNET_REBUILD_RESULT* pNetMsg = (GLMSG::SNET_REBUILD_RESULT*)nmg;

	switch( pNetMsg->emResult )
	{
	case EMREBUILD_RESULT_OPEN:
		{
			InitRebuildData();
			OpenRebuild();
		}
		break;

	case EMREBUILD_RESULT_SUCCESS:	
		{
			if( m_i64RebuildInput != m_i64RebuildCost || m_lnMoney < m_i64RebuildCost )
			{
				pNetMsg->emResult = EMREBUILD_RESULT_MONEY;
				GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, pNetMsg );
				return S_OK;
			}

			SITEMCUSTOM sRebuildItem = GET_REBUILD_ITEM();
			SITEM* pItem = GLItemMan::GetInstance().GetItem( sRebuildItem.sNativeID );
			if( !pItem )		return E_FAIL;

			SRANDOM_GEN* pRANDOM_SET = GLItemMan::GetInstance().GetItemRandomOpt( sRebuildItem.sNativeID );
			if( !pRANDOM_SET )	return E_FAIL;

			if( m_ItemRemodelPosX < EM_INVENSIZE_X && m_ItemRemodelPosY < EM_INVENSIZE_Y )
			{
				GLMSG::SNET_INVEN_REMODELOPEN_FB MsgFB;

				SINVENITEM* pINVENITEM = m_cInventory.GetItem ( m_ItemRemodelPosX, m_ItemRemodelPosY );
				if ( !pINVENITEM )
				{
					MsgFB.emFB = EMREQ_REMODELOPEN_FB_NOITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
					return E_FAIL;
				}

				if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

				SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
				if ( pITEM->sBasicOp.emItemType!=ITEM_REMODEL )
				{
					MsgFB.emFB = EMREQ_REMODELOPEN_FB_NOITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
					return E_FAIL;
				}

				DoDrugInvenItem ( m_ItemRemodelPosX, m_ItemRemodelPosY );

				if ( pNetMsg->Data1.nOption != -1 || pNetMsg->Data2.nOption != -1 )
					DoDrugInvenItem( m_sRebuildSeal.wPosX, m_sRebuildSeal.wPosY );

				SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( m_sRebuildSeal.wPosX, m_sRebuildSeal.wPosY );
				if ( !pInvenItem )
					m_sRebuildSeal.RESET();
			}
			else
			{
				PGLCROW pCrow = m_pLandMan->GetCrow ( pNetMsg->dwNPCID );
				if ( !pCrow )	
					return E_FAIL;

				float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-pCrow->GetPosition()) );
				float fTalkRange = (float) (pCrow->GetBodyRadius() + GETBODYRADIUS() + 30);
				float fTalkableDis = fTalkRange + 20;

				if ( fDist>fTalkableDis )
					return E_FAIL;
			}

			{
				CheckMoneyUpdate( m_lnMoney, m_i64RebuildCost, FALSE, "Rebuild Cost." );
				m_bMoneyUpdate = TRUE;

				m_lnMoney -= m_i64RebuildCost;

				GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
				NetMsgMoney.lnMoney = m_lnMoney;
				NetMsgMoney.lnVoteP = m_lnVoteP;
				NetMsgMoney.lnPremP = m_lnPremP;
				GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgMoney );

				GLITEMLMT::GetInstance().ReqMoneyExc( ID_CHAR, m_dwCharID, ID_CHAR, 0, -m_i64RebuildCost, EMITEM_ROUTE_NPCREMAKE );
				GLITEMLMT::GetInstance().ReqMoneyExc( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
			}

			float fDestroyRate = seqrandom::getpercent();
			if( fDestroyRate < pRANDOM_SET->fD_point )
			{
				GLITEMLMT::GetInstance().ReqItemRoute( sRebuildItem, ID_CHAR, m_dwCharID, ID_CHAR, m_dwCharID, EMITEM_ROUTE_DELETE, sRebuildItem.wTurnNum );

				m_cInventory.DeleteItem( m_sRebuildItem.wPosX, m_sRebuildItem.wPosY );

				GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
				NetMsg_Inven_Delete.wPosX = m_sRebuildItem.wPosX;
				NetMsg_Inven_Delete.wPosY = m_sRebuildItem.wPosY;
				GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg_Inven_Delete );

				pNetMsg->emResult = EMREBUILD_RESULT_DESTROY;
				GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, pNetMsg );
			}
			else
			{
				SINVENITEM* pInvenItem = m_cInventory.GetItem( m_sRebuildItem.wPosX, m_sRebuildItem.wPosY );
				if( pInvenItem )
				{
					SITEMSEALDATA sData1 = pNetMsg->Data1;
					SITEMSEALDATA sData2 = pNetMsg->Data2;
					if ( sData1.nOption != -1 || sData2.nOption != -1 )
					{
						SITEMCUSTOM sCustom = pInvenItem->sItemCustom;

						SITEMCUSTOM sITEM_NEW;
						sITEM_NEW.sNativeID 	= sCustom.sNativeID;
						sITEM_NEW.nidDISGUISE	= sCustom.nidDISGUISE;
					
						sITEM_NEW.tBORNTIME 	= sCustom.tBORNTIME;
						sITEM_NEW.tDISGUISE 	= sCustom.tDISGUISE;

						sITEM_NEW.cGenType 		= sCustom.cGenType;
						sITEM_NEW.cChnID 		= sCustom.cChnID;
						sITEM_NEW.cFieldID 		= sCustom.cFieldID;
						sITEM_NEW.lnGenNum 		= sCustom.lnGenNum;

						sITEM_NEW.cDAMAGE 		= sCustom.cDAMAGE;
						sITEM_NEW.cDEFENSE 		= sCustom.cDEFENSE;
						
						sITEM_NEW.cRESIST_FIRE 		= sCustom.cRESIST_FIRE;
						sITEM_NEW.cRESIST_ICE 		= sCustom.cRESIST_ICE;
						sITEM_NEW.cRESIST_ELEC 		= sCustom.cRESIST_ELEC;
						sITEM_NEW.cRESIST_POISON 		= sCustom.cRESIST_POISON;
						sITEM_NEW.cRESIST_SPIRIT 		= sCustom.cRESIST_SPIRIT;
						
						sITEM_NEW.dwPetID 			= sCustom.dwPetID;
						sITEM_NEW.dwVehicleID 		= sCustom.dwVehicleID;
						sITEM_NEW.bVietnamGainItem  = sCustom.bVietnamGainItem;
						sITEM_NEW.dwOwnerID 		= sCustom.dwOwnerID;
						sITEM_NEW.wColor 			= sCustom.wColor;

						if( sITEM_NEW.GENERATE_RANDOM_OPT() )
						{
							if ( sData1.nOption != -1 )
								{
									sITEM_NEW.cOptTYPE1 	= sData1.emOpt;
									sITEM_NEW.nOptVALUE1 	= sData1.fValue;

									if( sITEM_NEW.cOptTYPE1 == EMR_OPT_DAMAGE_RATE || sITEM_NEW.cOptTYPE1 == EMR_OPT_DEFENSE_RATE || sITEM_NEW.cOptTYPE1 == EMR_OPT_HIT_RATE || sITEM_NEW.cOptTYPE1 == EMR_OPT_AVOID_RATE )
										sITEM_NEW.nOptVALUE1 = sData1.fValue * 100.0f;
									
									if( sITEM_NEW.cOptTYPE1 == sITEM_NEW.cOptTYPE2)
									{
										sITEM_NEW.cOptTYPE2 = 0;
										sITEM_NEW.nOptVALUE2 = 0;
									}
									if( sITEM_NEW.cOptTYPE1 == sITEM_NEW.cOptTYPE3)
									{
										sITEM_NEW.cOptTYPE3 = 0;
										sITEM_NEW.nOptVALUE3 = 0;
									}
									if( sITEM_NEW.cOptTYPE1 == sITEM_NEW.cOptTYPE4)
									{
										sITEM_NEW.cOptTYPE4 = 0;
										sITEM_NEW.nOptVALUE4 = 0;
									}
								}

								if ( sData2.nOption != -1 )
								{
									sITEM_NEW.cOptTYPE2 	= sData2.emOpt;
									sITEM_NEW.nOptVALUE2 	= sData2.fValue;

									if( sITEM_NEW.cOptTYPE2 == EMR_OPT_DAMAGE_RATE || sITEM_NEW.cOptTYPE2 == EMR_OPT_DEFENSE_RATE || sITEM_NEW.cOptTYPE2 == EMR_OPT_HIT_RATE || sITEM_NEW.cOptTYPE2 == EMR_OPT_AVOID_RATE )
										sITEM_NEW.nOptVALUE2 = sData2.fValue * 100.0f;
									
									if( sITEM_NEW.cOptTYPE2 == sITEM_NEW.cOptTYPE3)
									{
										sITEM_NEW.cOptTYPE3 = 0;
										sITEM_NEW.nOptVALUE3 = 0;
									}
									if( sITEM_NEW.cOptTYPE2 == sITEM_NEW.cOptTYPE4)
									{
										sITEM_NEW.cOptTYPE4 = 0;
										sITEM_NEW.nOptVALUE4 = 0;
									}
								}

							GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
						}

						GLITEMLMT::GetInstance().ReqItemRoute( sRebuildItem, ID_CHAR, m_dwCharID, ID_CHAR, m_dwCharID, EMITEM_ROUTE_DELETE, sRebuildItem.wTurnNum );

						m_cInventory.DeleteItem( m_sRebuildItem.wPosX, m_sRebuildItem.wPosY );

						GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
						NetMsg_Inven_Delete.wPosX = m_sRebuildItem.wPosX;
						NetMsg_Inven_Delete.wPosY = m_sRebuildItem.wPosY;
						GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg_Inven_Delete );

						m_cInventory.InsertItem ( sITEM_NEW, m_sRebuildItem.wPosX, m_sRebuildItem.wPosY );
						SINVENITEM *pInsertItem = m_cInventory.GetItem ( m_sRebuildItem.wPosX, m_sRebuildItem.wPosY );

						GLITEMLMT::GetInstance().ReqItemRoute ( pInsertItem->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pInsertItem->sItemCustom.wTurnNum );

						GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
						NetMsg_Inven.Data = *pInsertItem;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);

						pNetMsg->emResult = EMREBUILD_RESULT_SUCCESS;
						GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, pNetMsg );
					}
					else
					{
						pInvenItem->sItemCustom.GENERATE_RANDOM_OPT( false );

						GLITEMLMT::GetInstance().ReqRandomItem( sRebuildItem );

						GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
						NetItemUpdate.wPosX = m_sRebuildItem.wPosX;
						NetItemUpdate.wPosY = m_sRebuildItem.wPosY;
						NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
						GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetItemUpdate );

						pNetMsg->emResult = EMREBUILD_RESULT_SUCCESS;
						GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, pNetMsg );
					}
				}
			}
		}
		break;

	case EMREBUILD_RESULT_FAIL:
	case EMREBUILD_RESULT_DESTROY:	
	case EMREBUILD_RESULT_MONEY:	
	case EMREBUILD_RESULT_CLOSE:	
		{
			InitRebuildData();
			CloseRebuild();

			m_ItemRemodelPosX = EM_INVENSIZE_X;
			m_ItemRemodelPosY = EM_INVENSIZE_Y;
		}
		break;
	}

	return S_OK;
}

HRESULT GLChar::MsgReqGarbageResult( NET_MSG_GENERIC* nmg )	// ITEMREBUILD_MARK
{
	GLMSG::SNET_GARBAGE_RESULT* pNetMsg = (GLMSG::SNET_GARBAGE_RESULT*)nmg;

	//if( m_ItemGarbagePosX < EM_INVENSIZE_X && m_ItemGarbagePosY < EM_INVENSIZE_Y )
	{
		GLMSG::SNET_GARBAGE_RESULT_FB MsgFB;
		
		SINVENITEM* pINVENITEM_DEL = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );

		if ( !pINVENITEM_DEL )
		{
			MsgFB.emResult = EMGARBAGE_RESULT_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		SITEM* pItem = GLItemMan::GetInstance().GetItem( pINVENITEM_DEL->sItemCustom.sNativeID );
		if( !pItem ) //|| !pItem->sBasicOp.IsGarbage() )		
		{
			MsgFB.emResult = EMGARBAGE_RESULT_FB_ITEMTYPE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		// ��ī���̸鼭 ���� Ȱ��ȭ �Ǿ� ������ �����Ҽ� ����.
		if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD )
		{
			PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
			if ( pMyPet && pMyPet->IsValid () && pINVENITEM_DEL->sItemCustom.dwPetID == pMyPet->m_dwPetID ) 
			{
				MsgFB.emResult = EMGARBAGE_RESULT_FB_FAIL;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
				return E_FAIL;
			}
		}
		
		/*SINVENITEM* pINVENITEM = m_cInventory.GetItem ( m_ItemGarbagePosX, m_ItemGarbagePosY );
		if ( !pINVENITEM )
		{
			MsgFB.emResult = EMGARBAGE_RESULT_FB_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )	return E_FAIL;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
		if ( !pITEM || pITEM->sBasicOp.emItemType!=ITEM_GARBAGE_CARD )
		{
			MsgFB.emResult = EMGARBAGE_RESULT_FB_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}*/

		DoDrugInvenItem ( m_ItemGarbagePosX, m_ItemGarbagePosY );

		GLITEMLMT::GetInstance().ReqItemRoute( pINVENITEM_DEL->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, m_dwCharID, EMITEM_ROUTE_GARBAGE, pINVENITEM_DEL->sItemCustom.wTurnNum );

        
		{
			// �� ī�� �ϰ��
			if ( pItem->sBasicOp.emItemType == ITEM_PET_CARD && pINVENITEM_DEL->sItemCustom.dwPetID != 0 )
			{
				PGLPETFIELD pMyPet = m_pGLGaeaServer->GetPET ( m_dwPetGUID );

				if ( pMyPet && pINVENITEM_DEL->sItemCustom.dwPetID == pMyPet->m_dwPetID ) 
				{
					m_pGLGaeaServer->ReserveDropOutPet ( SDROPOUTPETINFO(pMyPet->m_dwGUID,true,false) );

						// ���� �Ǽ��縮�� �˻��Ѵ�.
					pMyPet->UpdateTimeLmtItem ( this );

						// �־��ٲ� �־��ش�.
					for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
					{
						CItemDrop cDropItem;
						cDropItem.sItemCustom = pMyPet->m_PutOnItems[i];
						if ( IsInsertToInven ( &cDropItem ) ) InsertToInven ( &cDropItem );
						else
						{
							if ( m_pLandMan )
							{
								m_pLandMan->DropItem ( m_vPos, 
														&(cDropItem.sItemCustom), 
														EMGROUP_ONE, 
														m_dwGaeaID );
							}
						}
					}

					CDeletePet* pDeletePet = new CDeletePet ( m_dwCharID, pINVENITEM_DEL->sItemCustom.dwPetID );
					GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
					if ( pDBMan ) pDBMan->AddJob ( pDeletePet );
					
					// ���� �����Ǹ� ��Ȱ������ Ŭ���̾�Ʈ�� �˷���.
					CGetRestorePetList *pGetRestorePetList = new CGetRestorePetList ( m_dwCharID, m_dwClientID );
					if ( pDBMan ) pDBMan->AddJob ( pGetRestorePetList );
				}
				else
				{
					// �Ǽ��縮�� üũ�� ���� DB���� �������ش�.
					GLPET* pNewPet = new GLPET ();
					CGetPet* pGetPet = new CGetPet ( pNewPet, 
														pINVENITEM_DEL->sItemCustom.dwPetID, 
														m_dwClientID, 
														m_dwCharID,
														pINVENITEM_DEL->wPosX,
														pINVENITEM_DEL->wPosY,
														true );
					GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
					if ( pDBMan ) pDBMan->AddJob ( pGetPet );
				}
			}
			// Ż���� ���
			if ( pItem->sBasicOp.emItemType == ITEM_VEHICLE && pINVENITEM_DEL->sItemCustom.dwVehicleID != 0 )
			{
				// �Ǽ��縮�� üũ�� Ż���� DB���� �������ش�.
				GLVEHICLE* pNewVehicle = new GLVEHICLE();
				CGetVehicle* pGetVehicle = new CGetVehicle ( pNewVehicle, 
												pINVENITEM_DEL->sItemCustom.dwVehicleID, 
												m_dwClientID,
												m_dwCharID, 
												true );
				GLDBMan* pDBMan = m_pGLGaeaServer->GetDBMan ();
				if ( pDBMan ) pDBMan->AddJob ( pGetVehicle );		
			}
		}

		// �κ��丮���� ������ ����
		m_cInventory.DeleteItem( pNetMsg->wPosX, pNetMsg->wPosY );

		MsgFB.emResult = EMGARBAGE_RESULT_FB_OK;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

		// ������ ������ ����
		GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
		NetMsg_Inven_Delete.wPosX = pNetMsg->wPosX;
		NetMsg_Inven_Delete.wPosY = pNetMsg->wPosY;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg_Inven_Delete );	
		
		m_ItemGarbagePosX = EM_INVENSIZE_X;
		m_ItemGarbagePosY = EM_INVENSIZE_Y;

	}	

	return S_OK;
}

HRESULT GLChar::MsgReqRebuildMoveItem( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_REBUILD_MOVE_ITEM* pNetMsg = (GLMSG::SNET_REBUILD_MOVE_ITEM*)nmg;
	GLMSG::SNET_REBUILD_MOVE_ITEM NetMsg;

	m_sRebuildItem.RESET();
	m_sRebuildSeal.RESET();
	m_wSealType = 0;

	if( pNetMsg->wPosX != USHRT_MAX && pNetMsg->wPosY != USHRT_MAX )
	{
		SINVENITEM* pResistItem = m_cInventory.GetItem( pNetMsg->wPosX, pNetMsg->wPosY );
		if( pResistItem )
		{
			// ���� �������̾�� �ٲ� �� �ִ�
			SITEM* pItem = GLItemMan::GetInstance().GetItem( pResistItem->sItemCustom.sNativeID );
			if( pItem && pItem->sBasicOp.emItemType == ITEM_SUIT )
			{
				// �����ɼ� ������ �����Ǿ� �־�� �����ϴ�
				if( strlen( pItem->sRandomOpt.szNAME ) > 3 )
				{
					NetMsg.wPosX = pNetMsg->wPosX;
					NetMsg.wPosY = pNetMsg->wPosY;

					m_sRebuildItem.SET( NetMsg.wPosX, NetMsg.wPosY );
				}
			}
		}
	}
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg );

	// ���� ��� ����
	MsgReqRebuildCostMoney();

	return S_OK;
}

HRESULT GLChar::MsgReqRebuildCostMoney()
{
	GLMSG::SNET_REBUILD_COST_MONEY NetMsg;

	// ���� �������� �����Ƿ� 0�� �����Ѵ�
	if( !m_sRebuildItem.VALID() )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg );
		return S_OK;
	}

	// ����â�� ������ �ø��� ���������Ƿ� �̷� ���� �߻����� ���ƾ��Ѵ�
	SITEMCUSTOM sRebuildItem = GET_REBUILD_ITEM();
	SITEM* pItem = GLItemMan::GetInstance().GetItem( sRebuildItem.sNativeID );
	if( !pItem )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg );
		return S_OK;
	}

	// ����â�� ������ �ø��� ���������Ƿ� �̷� ���� �߻����� ���ƾ��Ѵ�
	SRANDOM_GEN* pRANDOM_SET = GLItemMan::GetInstance().GetItemRandomOpt( sRebuildItem.sNativeID );
	if( !pRANDOM_SET )
	{
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg );
		return S_OK;
	}

	m_i64RebuildCost = (LONGLONG)( pItem->sBasicOp.dwBuyPrice * pRANDOM_SET->fS_value );

	NetMsg.i64CostMoney = m_i64RebuildCost;

	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg );

	return S_OK;
}

HRESULT GLChar::MsgReqRebuildInputMoney( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_REBUILD_INPUT_MONEY* pNetMsg = (GLMSG::SNET_REBUILD_INPUT_MONEY*)nmg;

	m_i64RebuildInput = (LONGLONG)max( 0, pNetMsg->i64InputMoney );
	m_i64RebuildInput = (LONGLONG)min( m_i64RebuildInput, m_lnMoney );

	pNetMsg->i64InputMoney = m_i64RebuildInput;

	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, pNetMsg );

	return S_OK;
}

HRESULT	GLChar::MsgGiveFood ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPET_REQ_GIVEFOOD* pNetMsg = ( GLMSG::SNETPET_REQ_GIVEFOOD* ) nmg;
	
	GLMSG::SNETPET_REQ_GIVEFOOD_FB NetMsgFB;
	SNATIVEID sNativeID;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem ) 
	{
		NetMsgFB.emFB = EMPET_REQ_GIVEFOOD_FB_INVALIDCARD;
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		NetMsgFB.emFB = EMPET_REQ_GIVEFOOD_FB_INVALIDCARD;
		return E_FAIL;
	}

	// ��ī�� ���� üũ
	if ( pItem->sBasicOp.emItemType != ITEM_PET_CARD )	
	{
		NetMsgFB.emFB = EMPET_REQ_GIVEFOOD_FB_INVALIDCARD;
		return E_FAIL;
	}

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold ) 
	{
		NetMsgFB.emFB = EMPET_REQ_GIVEFOOD_FB_INVALIDFOOD;
		return E_FAIL;
	}

	if ( CheckCoolTime( pHold->sBasicOp.sNativeID ) ) return E_FAIL;

	// ��� ���� üũ
	if ( pHold->sBasicOp.emItemType != ITEM_PET_FOOD )	
	{
		NetMsgFB.emFB = EMPET_REQ_GIVEFOOD_FB_INVALIDFOOD;
		return E_FAIL;
	}

	// ������ ������ ���� Ʋ���� �������� �ʴ´�.
	SITEM* pPutOnItem = GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
	if( pHold != pPutOnItem )
	{
		NetMsgFB.emFB = EMPET_REQ_GIVEFOOD_FB_INVALIDFOOD;
		return E_FAIL;
	}

	sNativeID = pHold->sBasicOp.sNativeID;

	switch ( pHold->sDrugOp.emDrug )
	{
	case ITEM_DRUG_HP:
		{
			PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );

			// Logic �� ���� �ִٸ�
			if ( pMyPet && pMyPet->m_dwPetID == pInvenItem->sItemCustom.dwPetID )
			{
				// �� ���Ա� ���� �α�
				GLITEMLMT::GetInstance().ReqPetAction(	pMyPet->m_dwPetID, 
														pInvenItem->sItemCustom.sNativeID, 
														EMPET_ACTION_FOOD_BEFORE, 
														pMyPet->m_nFull );

				bool bOK = pMyPet->IncreaseFull ( pHold->sDrugOp.wCureVolume, pHold->sDrugOp.bRatio );
				if ( !bOK ) return E_FAIL;
				
				if ( pMyPet->IsValid () )	pMyPet->m_fTIMER = 0.0f;

				DoDrugSlotItem ( SLOT_HOLD );

				// �� ���Ա� ���� �α�
				GLITEMLMT::GetInstance().ReqPetAction(	pMyPet->m_dwPetID, 
														pInvenItem->sItemCustom.sNativeID, 
														EMPET_ACTION_FOOD_AFTER, 
														pMyPet->m_nFull );

				// �ֿ��� ���̸� �ش�.
				NetMsgFB.emFB	   = EMPET_REQ_GIVEFOOD_FB_OK;
				NetMsgFB.dwPetID   = pMyPet->m_dwPetID;
				NetMsgFB.sNativeID = sNativeID;
				NetMsgFB.nFull	   = pMyPet->m_nFull;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
			}
			// ���� Logic �� ���� ���ٸ�
			else
			{
				// �� �������� Ȯ��
				if (pInvenItem->sItemCustom.dwPetID <= 0 )
				{
					GLGaeaServer::GetInstance().GetConsoleMsg()->Write ( LOG_TEXT_CONSOLE, "Invalid ID, PetID %d dwClientID %d", 
						pInvenItem->sItemCustom.dwPetID, m_dwClientID );
					return E_FAIL;
				}

				// ���� �������� �����ͼ� �����ϰ� �ٽ� DB�� �����ϴ� �ڵ�
				CGetPetFull* pDbAction = new CGetPetFull ( m_dwCharID, 
														   pInvenItem->sItemCustom.dwPetID, 
														   m_dwClientID,
														   pHold->sDrugOp.wCureVolume, 
														   pHold->sDrugOp.bRatio,
														   sNativeID,
														   pNetMsg->emPetTYPE );

				GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
				if ( pDBMan ) pDBMan->AddJob ( pDbAction );
			}
		}
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT GLChar::MsgGetPetFullFromDB ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPET_REQ_GETFULL_FROMDB_FB* pIntMsg = ( GLMSG::SNETPET_REQ_GETFULL_FROMDB_FB* ) nmg;
	switch ( pIntMsg->emFB )
	{
	case EMPET_REQ_GETFULL_FROMDB_FAIL:
		{
			GLMSG::SNETPET_REQ_GIVEFOOD_FB NetMsgFB;
			NetMsgFB.emFB = EMPET_REQ_GETFULL_FROMDB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		}
		break;

	case EMPET_REQ_GETFULL_FROMDB_OK:
		{
			//	�տ� �� ������ Ȯ���Ѵ�.
			//	�������� �ٲ�� ��찡 �ִ�.
			SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
			if ( !pHold ) 
			{
				GLGaeaServer::GetInstance().GetConsoleMsg()->Write ( LOG_TEXT_CONSOLE, 
					"Pet Give Full Error CharID : %d", m_dwCharID );
					
				return E_FAIL;
			}

			if (  pHold->sBasicOp.emItemType != ITEM_PET_FOOD || pHold->sBasicOp.sNativeID != pIntMsg->sNativeID ) 
			{
				GLGaeaServer::GetInstance().GetConsoleMsg()->Write ( LOG_TEXT_CONSOLE, 
					"Pet Give Full Error CharID : %d, Item %d/%d", m_dwCharID,
					pHold->sBasicOp.sNativeID.wMainID, pHold->sBasicOp.sNativeID.wSubID  );
					
				return E_FAIL;

			}

			// �� ������ ���� �α�
			GLITEMLMT::GetInstance().ReqPetAction(	pIntMsg->dwPetID, 
													pIntMsg->sNativeID,
													EMPET_ACTION_FOOD_BEFORE, 
													pIntMsg->nFull );

			int nFull = pIntMsg->nFull;
			int nMaxFull = GLCONST_PET::pGLPET[pIntMsg->emType]->m_nFull;
			if ( nFull >= nMaxFull ) break;

			if ( pIntMsg->bRatio )
			{
				nFull += ( nMaxFull*pIntMsg->wCureVolume )/100;
				if ( nFull > nMaxFull ) nFull = nMaxFull;
			}
			else
			{
				nFull += pIntMsg->wCureVolume;
				if ( nFull > nMaxFull ) nFull = nMaxFull;
			}

			CSetPetFull* pDbAction = new CSetPetFull( m_dwCharID, pIntMsg->dwPetID, nFull );
			GLDBMan* pDbMan = GLGaeaServer::GetInstance().GetDBMan ();
			if ( pDbMan ) pDbMan->AddJob ( pDbAction );
			DoDrugSlotItem ( SLOT_HOLD );

			// �� ������ ���� �α�
			GLITEMLMT::GetInstance().ReqPetAction(	pIntMsg->dwPetID,
													pIntMsg->sNativeID,
													EMPET_ACTION_FOOD_AFTER,
													nFull );

			GLMSG::SNETPET_REQ_GIVEFOOD_FB NetMsgFB;
			NetMsgFB.emFB	   = EMPET_REQ_GIVEFOOD_FB_OK;
			NetMsgFB.dwPetID   = pIntMsg->dwPetID;
			NetMsgFB.sNativeID = pIntMsg->sNativeID;
			NetMsgFB.nFull	   = nFull;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
		}
	}

	return S_OK;
}

HRESULT GLChar::MsgReqMGameOddEvenEvent( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg = ( GLMSG::SNETPC_MGAME_ODDEVEN* )nmg;
	GLMSG::SNETPC_MGAME_ODDEVEN_FB NetMsgFB;

	// Ȧ¦����NPC ��ȿ�� üũ
	PGLCROW pCrow = m_pLandMan->GetCrow ( pNetMsg->dwNPCID );
	if ( !pCrow )
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-pCrow->GetPosition()) );
	float fTalkRange = (float) (pCrow->GetBodyRadius() + GETBODYRADIUS() + 30);
	float fTalkableDis = fTalkRange + 20;

	// Ȧ¦����NPC �Ÿ� üũ
	if ( fDist>fTalkableDis )
	{
		NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgFB );

		return E_FAIL;
	}

	switch( pNetMsg->emEvent )
	{
	case EMMGAME_ODDEVEN_OK:		return MGOddEvenOK(pNetMsg);
	case EMMGAME_ODDEVEN_CANCEL:	return MGOddEvenCANCEL(pNetMsg);
	case EMMGAME_ODDEVEN_SELECT:	return MGOddEvenSelect(pNetMsg);
	case EMMGAME_ODDEVEN_AGAIN_OK:	return MGOddEvenAGAIN(pNetMsg);
	case EMMGAME_ODDEVEN_SHUFFLE:	return MGOddEvenShuffle(pNetMsg);
	case EMMGAME_ODDEVEN_FINISH:	return MGOddEvenFinish(pNetMsg);
	default:
		{
			// ������ �̺�Ʈ�� �ƴ϶� ���� ���
			NetMsgFB.emResult = EMMGAME_ODDEVEN_FB_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgFB );

			return S_FALSE;
		}
		break;
	}

	return S_OK;
}


HRESULT GLChar::MsgReqItemMix( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_ITEM_MIX* pNetMsg = (GLMSG::SNET_INVEN_ITEM_MIX*)nmg;
	GLMSG::SNET_INVEN_ITEM_MIX_FB	NetMsgFB;


	SINVENITEM* pInvenItem[ITEMMIX_ITEMNUM];

	//	NPCȮ��
		// NPC üũ 
	
	
	//	���� Ȯ��
	const ITEM_MIX* pItemMix = GLItemMixMan::GetInstance().GetItemMix( pNetMsg->dwKey );
	if ( !pItemMix ) 
	{
		NetMsgFB.emFB = EMITEM_MIX_FB_NOMIX;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	//	�ݾ� Ȯ�� 
	if( m_lnMoney < pItemMix->dwPrice )
	{
		NetMsgFB.emFB = EMITEM_MIX_FB_NOMONEY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	//	�κ� ������ Ȯ��
	for ( int i =0; i < ITEMMIX_ITEMNUM; ++i ) 
	{
		if( !pNetMsg->sInvenPos[i].VALID() ) continue;
		
		pInvenItem[i] = m_cInventory.GetItem ( pNetMsg->sInvenPos[i].wPosX, pNetMsg->sInvenPos[i].wPosY );
		if ( !pInvenItem[i] ) 
		{
			NetMsgFB.emFB = EMITEM_MIX_FB_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		SITEM* pItem = GLItemMan::GetInstance().GetItem( pInvenItem[i]->sItemCustom.sNativeID );

		if ( !pItem ) 
		{
			NetMsgFB.emFB = EMITEM_MIX_FB_NOITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}
	}


	for ( int i =0; i < ITEMMIX_ITEMNUM; ++i ) 
	{
		if ( pItemMix->sMeterialItem[i].sNID == NATIVEID_NULL() ) continue;

		if ( pItemMix->sMeterialItem[i].nNum != pInvenItem[i]->sItemCustom.wTurnNum )
		{
			NetMsgFB.emFB = EMITEM_MIX_FB_NOMIXNUM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}	
	}


	//	��� ������ üũ
	SITEM* pItemResult = GLItemMan::GetInstance().GetItem( pItemMix->sResultItem.sNID );
	if ( !pItemResult ) 
	{
		NetMsgFB.emFB = EMITEM_MIX_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}


	WORD wInsertPosX, wInsertPosY;
	BOOL bOk = m_cInventory.FindInsrtable ( pItemResult->sBasicOp.wInvenSizeX, pItemResult->sBasicOp.wInvenSizeY, wInsertPosX, wInsertPosY );
	if ( !bOk )
	{
		//	�Ұ����� ��� �Դϴ�.
		NetMsgFB.emFB = EMITEM_MIX_FB_NOINVEN;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;	//	�κ� ���� �������� �ǵ��� ����.
	}

	GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;

	//	���� ������ ����
	for ( int i =0; i < ITEMMIX_ITEMNUM; ++i )
	{
		if( !pNetMsg->sInvenPos[i].VALID() ) continue;

		GLITEMLMT::GetInstance().ReqItemRoute( pInvenItem[i]->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, 
			EMITEM_ROUTE_ITEMMIX, pInvenItem[i]->sItemCustom.wTurnNum );
		
		m_cInventory.DeleteItem( pInvenItem[i]->wPosX, pInvenItem[i]->wPosY );
		
		NetMsg_Inven_Delete.wPosX = pNetMsg->sInvenPos[i].wPosX;
		NetMsg_Inven_Delete.wPosY = pNetMsg->sInvenPos[i].wPosY;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg_Inven_Delete );	
	}

	//	������ 
	if ( pItemMix->dwPrice > 0 )
	{
		CheckMoneyUpdate( m_lnMoney, pItemMix->dwPrice, FALSE, "ITEM_MIX" );
		m_bMoneyUpdate = TRUE;

		m_lnMoney -= pItemMix->dwPrice;

		GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
		NetMsgMoney.lnMoney = m_lnMoney;
		//add money 2
		NetMsgMoney.lnVoteP = m_lnVoteP;
		//add money 3
		NetMsgMoney.lnPremP = m_lnPremP;
		//add contributionpoints by CNDev
		NetMsgMoney.lnContribP = m_lnContribP;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgMoney );
	}

	//	�������� ���
	float fRate = (float)pItemMix->dwRate;
	if ( !RANDOM_GEN( fRate ) )
	{
		NetMsgFB.emFB = EMITEM_MIX_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return S_OK;	
	}

	//	��� ������ ����
	SITEMCUSTOM sITEM_NEW;
	sITEM_NEW.sNativeID = pItemMix->sResultItem.sNID;
	sITEM_NEW.tBORNTIME = CTime::GetCurrentTime().GetTime();
	sITEM_NEW.wTurnNum = pItemMix->sResultItem.nNum;
	sITEM_NEW.cGenType = EMGEN_ITEMMIX;
	sITEM_NEW.cChnID = (BYTE)m_pGLGaeaServer->GetServerChannel();
	sITEM_NEW.cFieldID = (BYTE)m_pGLGaeaServer->GetFieldSvrID();
	sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

	//	���� �ɼ� ����.
	if( sITEM_NEW.GENERATE_RANDOM_OPT() )
	{
		GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
	}

	//	����ִ� ������ �ι꿡 ����.
	m_cInventory.InsertItem ( sITEM_NEW, wInsertPosX, wInsertPosY );
	SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem ( wInsertPosX, wInsertPosY );

	if ( !pINSERT_ITEM )
	{
		NetMsgFB.emFB = EMITEM_MIX_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	GLITEMLMT::GetInstance().ReqItemRoute ( pINSERT_ITEM->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, 
		EMITEM_ROUTE_ITEMMIX, pINSERT_ITEM->sItemCustom.wTurnNum );

	//	[�ڽſ���] �޽��� �߻�.
	GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven;
	NetMsg_Inven.Data = *pINSERT_ITEM;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven);


	NetMsgFB.emFB = EMITEM_MIX_FB_OK;
	NetMsgFB.sNativeID	= pINSERT_ITEM->sItemCustom.sNativeID;
	NetMsgFB.wTurnNum = pINSERT_ITEM->sItemCustom.wTurnNum;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
	
	return S_OK;
}

HRESULT GLChar::MsgReqRebuildMoveSeal( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_REBUILD_MOVE_SEAL* pNetMsg = (GLMSG::SNET_REBUILD_MOVE_SEAL*)nmg;
	GLMSG::SNET_REBUILD_MOVE_SEAL NetMsg;

	m_sRebuildSeal.RESET();
	m_wSealType = 0;

	if( pNetMsg->wPosX != USHRT_MAX && pNetMsg->wPosY != USHRT_MAX )
	{
		SINVENITEM* pResistItem = m_cInventory.GetItem( pNetMsg->wPosX, pNetMsg->wPosY );
		if( pResistItem )
		{
			SITEM* pItem = GLItemMan::GetInstance().GetItem( pResistItem->sItemCustom.sNativeID );
			if( pItem && pItem->sBasicOp.emItemType == ITEM_SEALED_CARD )
			{
				NetMsg.wPosX = pNetMsg->wPosX;
				NetMsg.wPosY = pNetMsg->wPosY;

				m_sRebuildSeal.SET( NetMsg.wPosX, NetMsg.wPosY );
				m_wSealType = pItem->sSuitOp.wReModelNum;
			}
		}
	}

	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg );

	return S_OK;
}