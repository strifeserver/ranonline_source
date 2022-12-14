#include "stdafx.h"
#include "Psapi.h"
#pragma comment( lib, "Psapi.lib" )
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "./GLGaeaServer.h"
#include "GLClubDeathMatch.h"
#include "GLSchoolWars.h"
#include "GLRoyalRumble.h"
#include "GLTowerWars/GLTowerWars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLGaeaServer& GLGaeaServer::GetInstance()
{
	static GLGaeaServer Instance;
	return Instance;
}

GLGaeaServer::GLGaeaServer(void) :
	m_pd3dDevice(NULL),
	m_pMsgServer(NULL),
	m_pDBMan(NULL),
	m_nServerChannel(0),
	m_dwFieldSvrID(0),

	m_bUseIntelTBB(FALSE),

	m_dwAgentSlot(0),

	m_bEmulator(false),
	m_bUpdate(TRUE),
	m_bGenItemHold(true),
	//m_fMaxDelayMsgProc(0),

	m_dwMaxClient(1000),

	m_fTIMER_CLUB(0),

	m_bBigHead(false),
	m_bBigHand(false),
	m_bBrightEvent(false),
	m_bReservedStop(false),
	m_bClubBattleStarted(false),
	m_bClubDMStarted(false),
	m_bSchoolWarsStarted(false),
	m_bRoyalRumbleStarted(false),
	m_bTowerWarsStarted(false),
	m_nServiceProvider(0),
	m_fDelayMilliSec(1.0f),
	m_dwServerCrashTime(0),
	m_fCrashTimer(0),
	m_wSkipPacketNum(0),
	m_bEmptyMsg(false),
	m_bParticipate(false)
{
	InitializeCriticalSection(&m_CSPCLock);
}

GLGaeaServer::~GLGaeaServer(void)
{
	DeleteCriticalSection(&m_CSPCLock);
	
	SAFE_DELETE_ARRAY (m_PCArray);
	SAFE_DELETE_ARRAY (m_PETArray);
	SAFE_DELETE_ARRAY (m_SummonArray);
}

GLCrow* GLGaeaServer::NEW_CROW ()
{

	EnterCriticalSection(&m_CSPCLock);

	GLCrow* returnCrow = m_poolCROW.New();

	LeaveCriticalSection(&m_CSPCLock);

	return returnCrow;
}

void GLGaeaServer::RELEASE_CROW ( GLCrow* pCROW )
{
	GASSERT(pCROW&&"GLGaeaServer::RELEASE_CROW()");
	if ( !pCROW )	return;

	pCROW->RESET_DATA();
	m_poolCROW.ReleaseNonInit ( pCROW );
}

GLMaterial* GLGaeaServer::NEW_MATERIAL ()
{

	EnterCriticalSection(&m_CSPCLock);

	GLMaterial* returnMaterial = m_poolMATERIAL.New();

	LeaveCriticalSection(&m_CSPCLock);

	return returnMaterial;
}

void GLGaeaServer::RELEASE_MATERIAL ( GLMaterial* pMaterial )
{
	GASSERT(pMaterial&&"GLGaeaServer::RELEASE_MATERIAL()");
	if ( !pMaterial )	return;

	pMaterial->RESET_DATA();
	m_poolMATERIAL.ReleaseNonInit ( pMaterial );
}

GLChar* GLGaeaServer::NEW_CHAR ()
{
	EnterCriticalSection(&m_CSPCLock);

	GLChar* returnChar = m_poolCHAR.New();

	LeaveCriticalSection(&m_CSPCLock);

	return returnChar;
}

void GLGaeaServer::RELEASE_CHAR ( GLChar* pCHAR )
{
	GASSERT(pCHAR&&"GLGaeaServer::RELEASE_CHAR()");
	if ( !pCHAR )	return;

	EnterCriticalSection(&m_CSPCLock);

	pCHAR->RESET_DATA();
	m_poolCHAR.ReleaseNonInit ( pCHAR );

	LeaveCriticalSection(&m_CSPCLock);
}

SFIELDCROW* GLGaeaServer::NEW_FIELDCROW ()
{
	EnterCriticalSection(&m_CSPCLock);

	SFIELDCROW* returnCrow = m_poolFIELDCROW.New();

	LeaveCriticalSection(&m_CSPCLock);

	return returnCrow;
}

void GLGaeaServer::RELEASE_FIELDCROW ( SFIELDCROW* pFIELDCROW )
{
	GASSERT(pFIELDCROW&&"GLGaeaServer::RELEASE_CHAR()");
	if ( !pFIELDCROW )	return;

	EnterCriticalSection(&m_CSPCLock);

	pFIELDCROW->RESET();
	m_poolFIELDCROW.ReleaseNonInit ( pFIELDCROW );

	LeaveCriticalSection(&m_CSPCLock);
}

PGLPETFIELD GLGaeaServer::NEW_PET ()
{

	EnterCriticalSection(&m_CSPCLock);

	GLPetField* returnPet = m_poolPET.New();

	LeaveCriticalSection(&m_CSPCLock);

	return returnPet;
}

void GLGaeaServer::RELEASE_PET ( PGLPETFIELD pPet )
{
	GASSERT ( pPet && "GLGaeaServer::RELEASE_PET()" );
	if ( !pPet ) return;

	EnterCriticalSection(&m_CSPCLock);

	pPet->CleanUp ();
	m_poolPET.ReleaseNonInit ( pPet );

	LeaveCriticalSection(&m_CSPCLock);
}

PGLLANDMAN GLGaeaServer::NEW_GLLANDMAN ()
{

	EnterCriticalSection(&m_CSPCLock);

	PGLLANDMAN returnGLLandMan = m_poolGLLandMan.New();

	LeaveCriticalSection(&m_CSPCLock);

	return returnGLLandMan;
}

void GLGaeaServer::RELEASE_GLLANDMAN ( PGLLANDMAN pGLLandMan )
{
	GASSERT ( pGLLandMan && "GLGaeaServer::RELEASE_GLLANDMAN()" );
	if ( !pGLLandMan ) return;

	EnterCriticalSection(&m_CSPCLock);

	pGLLandMan->CleanUp();
	pGLLandMan->ResetLandMan();
	m_poolGLLandMan.ReleaseNonInit ( pGLLandMan );

	LeaveCriticalSection(&m_CSPCLock);
}

GLLandMan* GLGaeaServer::GetRootMap ()
{
	//	if ( !m_LandManList.m_pHead )	return NULL;

	//	return m_LandManList.m_pHead->Data;

	if( m_vecLandMan.empty() ) return NULL;

	return m_vecLandMan[0];
}

GLLandMan* GLGaeaServer::GetByMapID ( const SNATIVEID &sMapID )
{
	GASSERT(sMapID.wMainID<MAXLANDMID);
	GASSERT(sMapID.wSubID<MAXLANDSID);

	if ( sMapID.wMainID>=MAXLANDMID )	return NULL;
	if ( sMapID.wSubID>=MAXLANDSID )	return NULL;

	return m_pLandMan[sMapID.wMainID][sMapID.wSubID];
}

GLLandMan* GLGaeaServer::GetInstantMapByMapID ( const SNATIVEID &sMapID )
{
	size_t i, size = m_vecInstantMapSrcLandMan.size();
	for( i = 0; i < size; i++ )
	{
		if( m_vecInstantMapSrcLandMan[i]->GetMapID() == sMapID ) return m_vecInstantMapSrcLandMan[i];
	}
	return NULL;
}

HRESULT GLGaeaServer::OneTimeSceneInit ()
{
	m_FreePETGIDs.RemoveAll ();
	m_FreeSummonGIDs.RemoveAll();

	for ( DWORD i=0; i<m_dwMaxClient; i++ )	m_FreePETGIDs.AddTail ( i );
	for ( DWORD i=0; i<m_dwMaxClient; i++ )	m_FreeSummonGIDs.AddTail ( i );

	SecureZeroMemory ( m_pLandMan, sizeof(GLLandMan*)*MAXLANDMID*MAXLANDSID );

	SAFE_DELETE_ARRAY(m_PCArray);
	m_PCArray = new PGLCHAR[m_dwMaxClient];
	SecureZeroMemory ( m_PCArray, sizeof(PGLCHAR)*m_dwMaxClient );

	SAFE_DELETE_ARRAY (m_PETArray);
	m_PETArray = new PGLPETFIELD[m_dwMaxClient];
	SecureZeroMemory ( m_PETArray, sizeof(PGLPETFIELD)*m_dwMaxClient );

	SAFE_DELETE_ARRAY (m_SummonArray);
	m_SummonArray = new PGLSUMMONFIELD[m_dwMaxClient];
	SecureZeroMemory ( m_SummonArray, sizeof(PGLSUMMONFIELD)*m_dwMaxClient );

	return S_OK;
}

HRESULT GLGaeaServer::Create ( DWORD dwMaxClient, DxMsgServer *pMsgServer, DxConsoleMsg* pConsoleMsg, GLDBMan* pDBMan, int nServiceProvider, const char* szMapList, DWORD dwFieldSID, int nChannel, BOOL bUseIntelTBB )
{
	HRESULT hr;
	CleanUp ();

	m_bReservedStop = false;

	m_dwMaxClient = dwMaxClient;
	m_pMsgServer = pMsgServer;
	m_pConsoleMsg = pConsoleMsg;
	m_pDBMan = pDBMan;

	m_nServerChannel = nChannel;

	m_bUseIntelTBB   = bUseIntelTBB;

	m_nServiceProvider = nServiceProvider;

	//	Note : ???? ???? ID?? ?????? ???? ( ?????????? ????. )
	m_dwFieldSvrID = dwFieldSID;
	if ( dwFieldSID==FIELDSERVER_MAX )
	{
		m_bEmulator = true;
		m_dwFieldSvrID = 0;
	}

	hr = OneTimeSceneInit ();
	if ( FAILED(hr) )	return E_FAIL;
	
	if ( !szMapList )
	{
		hr = LoadMapList ( "mapslist.ini", NULL, FIELDSERVER_MAX );
		if ( FAILED(hr) )	return E_FAIL;
	}
	else
	{
		hr = LoadMapList ( szMapList, NULL, FIELDSERVER_MAX );
		if ( FAILED(hr) )	return E_FAIL;
	}

	GLITEMLMT::GetInstance().SetDBMan ( pDBMan );
	GLITEMLMT::GetInstance().SetServer ( pMsgServer, dwFieldSID );
	GLITEMLMT::GetInstance().ReadMaxKey ();

	FIELDMAP_ITER iter = m_MapList.begin ();
	FIELDMAP_ITER iter_end = m_MapList.end ();

	for ( ; iter!=iter_end; ++iter )
	{
		SMAPNODE *pMapNode = &(*iter).second;

		//	Note : ?????? ???????? ???????? ???? map ?? ???? ????.
		//
		if ( dwFieldSID!=FIELDSERVER_MAX && pMapNode->dwFieldSID!=dwFieldSID )		continue;

		//	Note : ???? ???? ID?? ?????? ???? ( ?????????? ????. )
		if ( dwFieldSID==FIELDSERVER_MAX )
		{
			pMapNode->dwFieldSID = 0;
		}

		//	Note : LandMan ?????? ??????.
		//
		GLLandMan *pNewLandMan = /*new GLLandMan;*/NEW_GLLANDMAN();
		pNewLandMan->SetMapID ( pMapNode->sNativeID, pMapNode->bPeaceZone!=FALSE, pMapNode->bPKZone==TRUE );

		pNewLandMan->SetRedZone( pMapNode->bRedZone );
		pNewLandMan->SetSchoolWarsZone ( pMapNode->bSWZone ); //Added by CNDev
		pNewLandMan->SetRoyalRumbleZone ( pMapNode->bRRZone ); //Added by CNDev

		//pNewLandMan->SetEmulator ( m_bEmulator );
		BOOL bOk = pNewLandMan->LoadFile ( pMapNode->strFile.c_str() );
		if ( !bOk )
		{
			SAFE_DELETE ( pNewLandMan );

			CString strTemp = pMapNode->strFile.c_str();
			strTemp += " : GLLandMan::LoadFile() Load Faile.";
			MessageBox ( NULL, strTemp, "ERROR", MB_OK );
			continue;
		}
		if( pMapNode->bInstantMap )
		{
			m_vecInstantMapSrcLandMan.push_back( pNewLandMan );
			continue;
		}

		hr = InsertMap ( pNewLandMan );
		if ( SUCCEEDED(hr) )
		{
			CONSOLEMSG_WRITE ( "[Add Map] %s  / [%d/%d]", pMapNode->strFile.c_str(), pMapNode->sNativeID.wMainID, pMapNode->sNativeID.wSubID );
		}
	}


	m_cPartyFieldMan.Create ( m_dwMaxClient );

	//	Note : ?????? ???? ???? ?? ?? ???? ???? Gate?? ?????????? ?????????? ????.
	//
	for ( WORD i=0; i<GLCONST_CHAR::wSCHOOLNUM; ++i )
	{
		SNATIVEID nidSTARTMAP = GLCONST_CHAR::nidSTARTMAP[i];
		DWORD dwSTARTGATE = GLCONST_CHAR::dwSTARTGATE[i];

		FIELDMAP_ITER iter = m_MapList.find ( nidSTARTMAP.dwID );
		if ( iter==m_MapList.end() )												continue;

		const SMAPNODE *pMapNode = &(*iter).second;
		//	Note : ?????? ???????? ???????? ???? map ?? ???? ????.
		if ( dwFieldSID!=FIELDSERVER_MAX && pMapNode->dwFieldSID!=dwFieldSID )		continue;

		GLLandMan* pLandMan = GetByMapID ( nidSTARTMAP );
		if ( !pLandMan )
		{
			//std::strstream strStream;
			//strStream << "Charactor's Start Map setting Error." << std::endl;
			//strStream << "'mapslist.ini' [" << nidSTARTMAP.wMainID << "," << nidSTARTMAP.wSubID << "]";
			//strStream << " MapID ( ID:[M,S] ) MapID Not Found." << std::ends;

			TCHAR szTemp[128] = {0};
			_snprintf( szTemp, 128, "Charactor's Start Map setting Error.\n"
									"'mapslist.ini' [%u],[%u] MapID ( ID:[M,S] ) MapID Not Found.",
									nidSTARTMAP.wMainID,
									nidSTARTMAP.wSubID );

			MessageBox ( NULL, szTemp, "ERROR", MB_OK|MB_ICONEXCLAMATION );

			//strStream.freeze( false );	// Note : std::strstream?? freeze. ?? ???? Leak ????.
			continue;
		}

		DxLandGateMan* pGateMan = &pLandMan->GetLandGateMan ();
		PDXLANDGATE pGate = pGateMan->FindLandGate ( dwSTARTGATE );
		if ( !pGate )
		{
			//std::strstream strStream;
			//strStream << "Charactor Start Map GATE Not Found." << std::endl;
			//strStream << pLandMan->GetFileName() << " Map "  << dwSTARTGATE;
			//strStream << "GATE ID Must Check." << std::ends;

			TCHAR szTemp[128] = {0};
			_snprintf( szTemp, 128, "Charactor Start Map GATE Not Found.\n"
									"%s Map %u GATE ID Must Check.",
									pLandMan->GetFileName(),
									dwSTARTGATE );

			MessageBox ( NULL, szTemp, "ERROR", MB_OK );

			//strStream.freeze( false );	// Note : std::strstream?? freeze. ?? ???? Leak ????.
			continue;
		}
	}

	if ( m_pDBMan )
	{
		//	Note : ?????? ?????? db???? ????????.
		//
		m_cClubMan.LoadFromDB ( m_pDBMan, true );

		//	Note : ???? ???? ???? ????.
		//
		VECGUID_DB vecGuidDb;
		m_pDBMan->GetClubRegion ( vecGuidDb );
		GLGuidanceFieldMan::GetInstance().SetState ( vecGuidDb );
		//add sw buff
		VECSWID_DB vecSwidDb;
		m_pDBMan->GetSwRegion ( vecSwidDb );
		GLSchoolWarsFieldMan::GetInstance().SetState( vecSwidDb );
	}
	
	SetMapState();
	GLGuidanceFieldMan::GetInstance().SetMapState ();
	GLClubDeathMatchFieldMan::GetInstance().SetMapState ();
	GLSchoolWarsFieldMan::GetInstance().SetMapState();
	GLRoyalRumbleFieldMan::GetInstance().SetMapState();

	GLTowerWarsFieldMan::GetInstance().SetMapState();
	return S_OK;
}

HRESULT GLGaeaServer::Create4Level ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	CleanUp ();

	m_pd3dDevice = pd3dDevice;

	OneTimeSceneInit ();

	//	Note : LandMan ?????? ??????.
	//
	GLLandMan *pNewLandMan = /*new GLLandMan;*/NEW_GLLANDMAN();
	pNewLandMan->SetD3dDevice ( m_pd3dDevice );
	pNewLandMan->SetMapID ( SNATIVEID(0,0), false, true );
	pNewLandMan->SetEmulator ( true );

	//	Note : ?????? LandMan ????.
	//
	InsertMap ( pNewLandMan );

	return S_OK;
}

void GLGaeaServer::SetMapState()
{
	size_t tSize = m_vecLandMan.size();
	for( size_t i=0; i<tSize; ++i )
	{
		PGLLANDMAN pLand = m_vecLandMan[i];

		pLand->m_bGuidBattleMap = false;
		pLand->m_bGuidBattleMapHall = false;
		pLand->m_bClubDeathMatchMap = false;
		pLand->m_bClubDeathMatchMapHall = false;
		pLand->m_bSchoolWarsMap = false;
		pLand->m_bSchoolWarsMapHall = false;
		pLand->m_bRoyalRumbleMap = false;
		pLand->m_bRoyalRumbleMapHall = false;
		pLand->m_bTowerWarsMap = false;

		SMAPNODE *pMAPNODE = FindMapNode ( pLand->GetMapID() );
		if ( pMAPNODE && pMAPNODE->bCommission )
		{
			pLand->m_fCommissionRate = GLCONST_CHAR::fDEFAULT_COMMISSION;
		}
		else
		{
			pLand->m_fCommissionRate = 0.0f;
		}

	}
}


HRESULT GLGaeaServer::InsertMap ( GLLandMan* pNewLandMan )
{
	const SNATIVEID &sMapID = pNewLandMan->GetMapID ();

	GASSERT(sMapID.wMainID<MAXLANDMID);
	GASSERT(sMapID.wSubID<MAXLANDSID);

//	m_LandManList.ADDTAIL ( pNewLandMan );
	m_vecLandMan.push_back( pNewLandMan );
	m_pLandMan[sMapID.wMainID][sMapID.wSubID] = pNewLandMan;

	return S_OK;
}

HRESULT GLGaeaServer::CleanUp ()
{
	m_pMsgServer = NULL;

	ClearDropObj ();

	/*GLLANDMANNODE* pLandManNode = m_LandManList.m_pHead;
	for ( ; pLandManNode; pLandManNode = pLandManNode->pNext )
	{
	SAFE_DELETE(pLandManNode->Data);
	}

	m_LandManList.DELALL();*/

	size_t i, size = m_vecLandMan.size();
	for( i = 0; i < size; i++ )
	{
		RELEASE_GLLANDMAN( m_vecLandMan[i] );
//		SAFE_DELETE( m_vecLandMan[i] );
	}
	m_vecLandMan.clear();

	size = m_vecInstantMapSrcLandMan.size();
	for( i = 0; i < size; i++ )
	{
		SAFE_DELETE( m_vecInstantMapSrcLandMan[i] );
	}
	m_vecInstantMapSrcLandMan.clear();

	m_pDBMan = NULL;

	m_FreePETGIDs.RemoveAll ();

	m_FreeSummonGIDs.RemoveAll ();

	return S_OK;
}

PGLCHAR GLGaeaServer::CreatePC ( PCHARDATA2 pCharData, DWORD _dwClientID, DWORD _dwGaeaID, BOOL bNEW,
								SNATIVEID *_pStartMap, DWORD _dwStartGate, D3DXVECTOR3 _vPos,
								EMGAME_JOINTYPE emJOINTYPE,
								DWORD dwThaiCCafeClass, __time64_t loginTime, INT nMyCCafeClass )
{
	HRESULT hr = S_OK;
	if ( !pCharData )
	{
		CDebugSet::ToLogFile ( "ERR : pCharData?? ????." );
		return NULL;
	}

	if ( _dwGaeaID>=m_dwMaxClient )
	{
		CDebugSet::ToLogFile ( "ERR : _dwGaeaID>=m_dwMaxClient  %d>%d.", _dwGaeaID, m_dwMaxClient );
		return NULL;
	}

	BOOL bDB(FALSE);
	PGLCHAR pPChar = NULL;
	GLLandMan* pLandMan = NULL;

	DxLandGateMan* pGateMan = NULL;
	PDXLANDGATE pGate = NULL;
	D3DXVECTOR3 vStartPos(0,0,0);

	CLIENTMAP_ITER client_iter;
	GLCHAR_MAP_ITER name_iter;

	//	Note : user id?? ???? db?? ?????????? ????. ( logout ?????? ???? db?? ?????? ???? ?????? ??????. )
	//
	bDB = FindSaveDBUserID ( pCharData->GetUserID() );
	if ( bDB )
	{
		CDebugSet::ToLogFile ( "ERR : logout ?????? ???? db?? ?????? ???? ?????? ????." );
		goto _ERROR;
	}

	if ( _dwClientID >= GLGaeaServer::GetInstance().GetMaxClient()*2 )
	{
		CDebugSet::ToLogFile ( "ERR : max client id overflow! id = %d", _dwClientID );
		goto _ERROR;
	}

	if ( _dwGaeaID >= GLGaeaServer::GetInstance().GetMaxClient() )
	{
		CDebugSet::ToLogFile ( "ERR : max gaea id overflow! id = %d", _dwGaeaID );
		goto _ERROR;
	}

	//	Note : ?????? ID?? ?????? Agent ???????? ???????? ???? ????
	//		???????? ?????? ID?? ???????? ???? ???????? ???? ?????? ???????? ????.
	if ( m_PCArray[_dwGaeaID] )
	{
		CDebugSet::ToLogFile ( "ERR : m_PCArray[_dwGaeaID] ???? ?????? gaeaid" );
		goto _ERROR;
	}

	//	Note : ???? ???????? ???? ???????? ?????? ??????????.
	name_iter = m_PCNameMap.find(pCharData->m_szName);
	if ( name_iter != m_PCNameMap.end() )
	{
		CDebugSet::ToLogFile ( "ERR : char name ???? ???????? ????" );
		goto _ERROR;
	}

	client_iter = m_PCClientIDMAP.find(_dwClientID);
	if ( client_iter != m_PCClientIDMAP.end() )
	{
		//	???? ???????? DropOut ????????.
		DWORD dwGaeaID = (*client_iter).second;
		//	Note : ???????? ????????.
		//
		ReserveDropOutPC(dwGaeaID);

		CDebugSet::ToLogFile ( "ERR : client id ???? ???????? ????" );
		goto _ERROR;
	}


	//	Note : ?????? ??????.
	//
	pPChar = NEW_CHAR();

	pPChar->SetGLGaeaServer( this );

	//	Note : ?????? MapID???? ?????????? ?? ????.
	//
	if ( _pStartMap )
	{
		pLandMan = GetByMapID ( *_pStartMap );
		if ( pLandMan )
		{
			pGateMan = &pLandMan->GetLandGateMan ();

			if ( _dwStartGate!=UINT_MAX )
			{
				pGate = pGateMan->FindLandGate ( DWORD(_dwStartGate) );

				if ( pGate )	vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
				else
				{
					pGate = pGateMan->FindLandGate ( DWORD(0) );
					if( pGate == NULL )
					{
						GetConsoleMsg()->Write( "ERROR: pGate = NULL, UserID %s UserLv %d Money %d", pCharData->m_szName, pCharData->m_wLevel, pCharData->m_lnMoney );
						goto _ERROR;

					}
					_dwStartGate = pGate->GetGateID ();
					if ( pGate )	vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
				}
			}
			else
			{
				//	???? ???????? ???????? ?? ????.
				vStartPos = _vPos;
			}
		}
	}
	else
	{
		pLandMan = GetByMapID ( pCharData->m_sStartMapID );
		if ( pLandMan )
		{
			pGateMan = &pLandMan->GetLandGateMan ();
			pGate = pGateMan->FindLandGate ( DWORD(pCharData->m_dwStartGate) );

			if ( pGate )	vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
			else
			{
				pGate = pGateMan->FindLandGate ( DWORD(0) );
				if ( pGate )	vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
			}
		}
	}

	if ( !pLandMan || ((_dwStartGate!=UINT_MAX)&&!pGate) )
	{
		SNATIVEID nidSTARTMAP = GLCONST_CHAR::nidSTARTMAP[pCharData->m_wSchool];
		DWORD dwSTARTGATE = GLCONST_CHAR::dwSTARTGATE[pCharData->m_wSchool];

		pLandMan = GetByMapID ( nidSTARTMAP );
		if ( !pLandMan )
		{
			DEBUGMSG_WRITE ( "[????] #1 GetByMapID() ???????? ?????? LAND(???? GATE)?? ??????????????????." );
			goto _ERROR;
		}

		pGateMan = &pLandMan->GetLandGateMan ();
		if ( !pGateMan )
		{
			DEBUGMSG_WRITE ( "[????] #2 GetLandGateMan() ???????? ?????? LAND(???? GATE)?? ??????????????????." );
			goto _ERROR;

		}
		pGate = pGateMan->FindLandGate ( dwSTARTGATE );
		if ( !pGate )
		{
			DEBUGMSG_WRITE ( "[????] #3 FindLandGate() ???????? ?????? LAND(???? GATE)?? ??????????????????." );
			vStartPos = D3DXVECTOR3(0,0,0);
		}
		else
		{
			vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
		}
	}

	//	Note : ?????? ????.
	//
	pCharData->m_dwThaiCCafeClass = dwThaiCCafeClass;
	pCharData->m_nMyCCafeClass    = nMyCCafeClass;
	pCharData->m_sEventTime.Init();
	pCharData->m_sEventTime.loginTime = loginTime;
	hr = pPChar->CreateChar ( pLandMan, vStartPos, pCharData, m_pd3dDevice, bNEW );
	if ( FAILED(hr) )
	{
		DEBUGMSG_WRITE ( "[????] pPChar->CreateChar () ?????? ?????? ?????? ?????? ?????? ????. [%s]", pCharData->m_szName );
		goto _ERROR;
	}
	
	pPChar->m_dwClientID = _dwClientID;
	pPChar->m_dwGaeaID = _dwGaeaID;
	pPChar->SetPartyID ( m_cPartyFieldMan.GetPartyID ( _dwGaeaID ) );
	
	BOOL bOk = DropPC ( pLandMan->GetMapID(), vStartPos, pPChar );
	if ( !bOk )
	{
		DEBUGMSG_WRITE ( "[????] DropPC () ?????? ?????? ?????? ?????? ?????? ????. [%s]", pCharData->m_szName );
		goto _ERROR;
	}


#if defined(TW_PARAM) || defined(HK_PARAM) || defined(_RELEASED)

	bool bDifferTempValue = FALSE;
	/*if( pPChar->m_wLevel != pPChar->m_wTempLevel )
	{
		if( pPChar->m_wTempLevel != 0 )
		{
			HACKINGLOG_WRITE( "#####Different Login Level##### [%s][%s] Level %d TempLevel %d Level Gap %d", 
				pPChar->m_szUID, pPChar->m_szName, pPChar->m_wLevel, pPChar->m_wTempLevel, pPChar->m_wLevel - pPChar->m_wTempLevel );	

		}
		bDifferTempValue = TRUE;
		pPChar->m_wLevel = pPChar->m_wTempLevel;
	}*/

	if( pPChar->m_lnMoney != pPChar->m_lnTempMoney )
	{
#ifdef _RELEASED
		if( pPChar->m_lnTempMoney != 0 )
#endif
		{
			HACKINGLOG_WRITE( "Different Login Money!!, Account[%s], ID[%s], Money %I64d, TempMoney %I64d, Money Gap %I64d", 
						  pPChar->m_szUID, pPChar->m_szName, pPChar->m_lnMoney, pPChar->m_lnTempMoney, pPChar->m_lnMoney - pPChar->m_lnTempMoney );							
	
			bDifferTempValue  = TRUE;			

			// ?????? TempMoney?? Money ???? ???? ??????.
			LONGLONG lnGap = pPChar->m_lnMoney - pPChar->m_lnTempMoney;
			if( lnGap >= 10000 || lnGap <= -10000 )
			{
				if( pPChar->m_lnMoney > pPChar->m_lnTempMoney )
				{
					pPChar->m_lnMoney = pPChar->m_lnTempMoney;			
				}else{
					pPChar->m_lnTempMoney = pPChar->m_lnMoney;			
				}
			}else{
				pPChar->m_lnMoney = pPChar->m_lnTempMoney;			
			}
		}
#ifdef _RELEASED
		else{			
			pPChar->m_lnTempMoney = pPChar->m_lnMoney;
		}
#endif
		
	}

	if( pPChar->m_lnStorageMoney != pPChar->m_lnTempStorageMoney )
	{
#ifdef _RELEASED
		if( pPChar->m_lnTempStorageMoney != 0 )
#endif
		{
			HACKINGLOG_WRITE( "Different Login Storage Money!!, Account[%s], ID[%s], Storage Money %I64d, TempStorage Money %I64d, Storage Money Gap %I64d", 
					pPChar->m_szUID, pPChar->m_szName, pPChar->m_lnStorageMoney, pPChar->m_lnTempStorageMoney, 
					pPChar->m_lnStorageMoney - pPChar->m_lnTempStorageMoney );				

			bDifferTempValue		 = TRUE;
			
			LONGLONG lnGap = pPChar->m_lnStorageMoney - pPChar->m_lnTempStorageMoney;
			if( lnGap > 10000 || lnGap < -10000 )
			{
				if( pPChar->m_lnStorageMoney > pPChar->m_lnTempStorageMoney )
				{
					pPChar->m_lnStorageMoney = pPChar->m_lnTempStorageMoney;			
				}else{
					pPChar->m_lnTempStorageMoney = pPChar->m_lnStorageMoney;			
				}
			}else{
				pPChar->m_lnStorageMoney = pPChar->m_lnTempStorageMoney;			
			}	
		}
#ifdef _RELEASED
		else{
			pPChar->m_lnTempStorageMoney = pPChar->m_lnStorageMoney;					
		}
#endif
		
	}

	if( bDifferTempValue )
	{
		ReserveDropOutPC( pPChar->m_dwGaeaID );

		//	???? ??????????  ???????? ????????.
		GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
		NetMsgFB.emCharJoinFB = EMCJOIN_FB_ERROR;
		SENDTOAGENT ( _dwClientID, &NetMsgFB );
		return NULL;
	}

#else
	pPChar->m_wTempLevel		  = pPChar->m_wLevel;
	pPChar->m_lnTempMoney		  = pPChar->m_lnMoney;
	pPChar->m_lnTempStorageMoney  = pPChar->m_lnStorageMoney;
#endif


//#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	if ( pPChar->m_bTracingUser && pPChar->m_pLandMan )
	{
		NET_LOG_UPDATE_TRACINGCHAR TracingMsg;
		TracingMsg.nUserNum  = pPChar->GetUserID();
		StringCchCopy( TracingMsg.szAccount, USR_ID_LENGTH+1, pPChar->m_szUID );

		int nPosX(0);
		int nPosY(0);
		pPChar->m_pLandMan->GetMapAxisInfo().Convert2MapPos ( pPChar->m_vPos.x, pPChar->m_vPos.z, nPosX, nPosY );

		CString strTemp;
		strTemp.Format( "FieldServer in!!, [%s][%s], MAP:mid[%d]sid[%d], StartPos:[%d][%d], Money:[%I64d]",
			pPChar->m_szUID, pPChar->m_szName, pPChar->m_sMapID.wMainID, pPChar->m_sMapID.wSubID, nPosX, nPosY, pPChar->m_lnMoney );

		StringCchCopy( TracingMsg.szLogMsg, TRACING_LOG_SIZE, strTemp.GetString() );

		SENDTOAGENT( pPChar->m_dwClientID, &TracingMsg );
	}
//#endif

	return pPChar;	//	?????? ???? ????.

_ERROR:
	if ( pPChar )
	{
		RELEASE_CHAR(pPChar);
	}

	//	???? ??????????  ???????? ????????.
	GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
	NetMsgFB.emCharJoinFB = EMCJOIN_FB_ERROR;
	SENDTOAGENT ( _dwClientID, &NetMsgFB );

	return NULL;	//	?????? ???? ????.
}



PGLPETFIELD GLGaeaServer::CreatePET ( PGLPET pPetData, DWORD dwOwner, DWORD dwPetID, bool bValid )
{
	// DB???? ???????? ???????? ????.

	GLMSG::SNETPET_REQ_USEPETCARD_FB NetMsg;

	if ( !pPetData ) return NULL;

	// ???? ???????? ?????? ????
	PGLCHAR pOwner = GetChar ( dwOwner );
	if ( !pOwner ) return NULL;

	// ??Full Check
	if ( pPetData->IsNotEnoughFull () )
	{
		NetMsg.emFB = EMPET_USECARD_FB_NOTENOUGHFULL;
		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
		return NULL;
	}

	GLLandMan* pLandMan = GetByMapID ( pOwner->m_sMapID );
	if ( !pLandMan ) 
	{
		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
		return NULL;
	}

	PGLPETFIELD pPet(NULL);

	// ???? ?????????? ???????? ???????? GLPetField ?? ????????.
	// ???????? ???????????? ??????????.
	pPet = GetPET ( pOwner->m_dwPetGUID );
	if ( pPet && m_bEmulator )
	{
		// ???????? ???????? ?? ????
		DropOutPET ( pPet->m_dwGUID, true, false );
		pPet = NULL;
	}

	if ( pPet )
	{
		HRESULT hr = pPet->SetPosition ( pLandMan );
		if ( FAILED ( hr ) )
		{
			// ???????? ???????? ?? ????
			DropOutPET ( pPet->m_dwGUID, true, false );
			SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
			return NULL;
		}
	}
	// ?????? ??????????.
	else
	{
		// ??????
		pPet = NEW_PET ();
		HRESULT hr = pPet->Create ( pLandMan, pOwner, pPetData );
		if ( FAILED ( hr ) )
		{
			// ???????? ????
			RELEASE_PET ( pPet );
			SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
			return NULL;
		}

		DWORD dwGUID = -1;
		if ( !m_FreePETGIDs.GetHead ( dwGUID ) )		return NULL;
		m_FreePETGIDs.DelHead ();
		pPet->m_dwGUID = dwGUID;

		// ????ID ????
		pOwner->m_dwPetGUID = pPet->m_dwGUID;
		pOwner->m_dwPetID   = dwPetID;

		// ???? ????ID(DB??????) ????
		pPet->SetPetID ( dwPetID );
	}

	if ( !DropPET ( pPet, pOwner->m_sMapID ) )
	{
		// ???????? ????
		m_FreePETGIDs.AddTail ( pPet->m_dwGUID );
		RELEASE_PET ( pPet );
		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
		return NULL;
	}

	// ???? ?????? ????
	pPet->m_pOwner = pOwner;

	// ?????? ???? ????
	if ( bValid ) pPet->SetValid ();
	else		  pPet->ReSetValid ();

	// ??????????????
	if ( pPet->IsValid () )
	{
		NetMsg.emFB				  = EMPET_USECARD_FB_OK;
		NetMsg.m_emTYPE			  = pPet->m_emTYPE;
		NetMsg.m_dwGUID			  = pPet->m_dwGUID;
		NetMsg.m_sPetID			  = pPet->m_sPetID;
		NetMsg.m_sActiveSkillID	  = pPet->m_sActiveSkillID;
		NetMsg.m_sActiveSkillID2  = pPet->m_sActiveSkillID2;
		NetMsg.m_dwOwner		  = pPet->m_dwOwner;
		NetMsg.m_wStyle			  = pPet->m_wStyle;
		NetMsg.m_wColor			  = pPet->m_wColor;
		NetMsg.m_fWalkSpeed		  = pPet->m_fWalkSpeed;
		NetMsg.m_fRunSpeed		  = pPet->m_fRunSpeed;
		NetMsg.m_nFull			  = pPet->m_nFull;
		NetMsg.m_sMapID			  = pPet->m_sMapID;
		NetMsg.m_dwCellID		  = pPet->m_dwCellID;
		NetMsg.m_wSkillNum		  = static_cast<WORD> (pPet->m_ExpSkills.size());
		NetMsg.m_vPos			  = pPet->m_vPos;
		NetMsg.m_vDir			  = pPet->m_vDir;
		NetMsg.m_sPetSkinPackData = pPet->m_sPetSkinPackData;
		NetMsg.m_bDualSkill		  = pPet->m_bDualSkill;
		StringCchCopy ( NetMsg.m_szName, PETNAMESIZE+1, pPetData->m_szName );

		NetMsg.m_dwPetID		= dwPetID;

		PETSKILL_MAP_ITER iter = pPet->m_ExpSkills.begin();
		PETSKILL_MAP_ITER iter_end = pPet->m_ExpSkills.end();
		WORD i(0);
		for ( ;iter != iter_end; ++iter )
		{
			NetMsg.m_Skills[i++] = (*iter).second;
		}

		const CTime cTIME_CUR = CTime::GetCurrentTime();

		for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
		{
			SITEMCUSTOM sPetItem = pPet->m_PutOnItems[i];
			if ( sPetItem.sNativeID == NATIVEID_NULL () ) continue;

			SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sPetItem.sNativeID );
			if ( !pITEM )	continue;

			// ?????? ??????
			if ( pITEM->IsTIMELMT() )
			{
				CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
				CTime cTIME_LMT(sPetItem.tBORNTIME);
				cTIME_LMT += cSPAN;

				if ( cTIME_CUR > cTIME_LMT )
				{
					//	???? ???????? ?????? ???? ???? ????.
					GLITEMLMT::GetInstance().ReqItemRoute ( sPetItem, ID_CHAR, pOwner->m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, 0 );

					//	?????? ????.
					pPet->m_PutOnItems[i] = SITEMCUSTOM ();

					//	???? ???????? ?????? ???? ????.
					GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
					NetMsgInvenDelTimeLmt.nidITEM = sPetItem.sNativeID;
					GLGaeaServer::GetInstance().SENDTOCLIENT(pOwner->m_dwClientID,&NetMsgInvenDelTimeLmt);
				}
			}
			NetMsg.m_PutOnItems[i] = pPet->m_PutOnItems[i];
		}

		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );

		// ?????? ????
		GLMSG::SNETPET_CREATE_ANYPET NetMsgBrd;
		NetMsgBrd.Data = ((GLMSG::SNETPET_DROP_PET*)pPet->ReqNetMsg_Drop ())->Data;
		pPet->m_pOwner->SendMsgViewAround ( ( NET_MSG_GENERIC* )&NetMsgBrd );
	}
	
	return pPet;	
}

BOOL GLGaeaServer::DropPET ( PGLPETFIELD pPet, SNATIVEID sMapID )
{
	if ( !pPet ) return FALSE;

	GLLandMan* pLandMan = GetByMapID ( sMapID );
	if ( !pLandMan ) return FALSE;

	// ???????? ????
	m_PETArray[pPet->m_dwGUID] = pPet;

	// ?????? ????
	pPet->m_pLandMan = pLandMan;
	pPet->m_pLandNode = pLandMan->m_GlobPETList.ADDHEAD ( pPet );
	pLandMan->RegistPet ( pPet );
	
	return TRUE;
}

// ?? ?????? ?????? ???? ???????? ???? ?????? ??????
// ????????. (?????? ?????? ???????? ???????? ??????)
// ?????? ?????????? ???????? ????.
BOOL GLGaeaServer::DropOutPET ( DWORD dwGUID, bool bLeaveFieldServer, bool bMoveMap )
{
	if ( dwGUID>=m_dwMaxClient ) 
	{
		CDebugSet::ToLogFile ( "ERROR : dwGUID>=m_dwMaxClient PetGUID : %d dwMaxClient : %d", dwGUID, m_dwMaxClient );
		return FALSE;
	}
	if ( m_PETArray[dwGUID] == NULL ) 
	{
		CDebugSet::ToLogFile ( "ERROR : m_PETArray[dwGUID] == NULL" );
		return FALSE;
	}

	PGLPETFIELD pPet = m_PETArray[dwGUID];

	// ???? ????
	bool bValid = pPet->IsValid ();

	// ?????? ?????????? ?????? ?????? ????????
	if ( !bValid && !bLeaveFieldServer )
	{
		CDebugSet::ToLogFile ( "ERROR : !bValid && !bLeaveFieldServer bValid %d bLeaveFieldServer %d", bValid, bLeaveFieldServer );
		return FALSE;
	}

	DWORD dwPetGUID = pPet->m_dwGUID;
	DWORD dwOwnerID = pPet->m_dwOwner;

	// ??????????
	if ( bValid )
	{
		//	Note : Land ?????????? ????.
		GLLandMan* pLandMan = pPet->m_pLandMan;
		if ( pLandMan )
		{
			pLandMan->RemovePet ( pPet );
			pPet->m_pLandMan = NULL;
		}

		pPet->ReSetValid ();
		pPet->ReSetAllSTATE ();
		pPet->ReSetSkillDelay ();
	}

	// ?????????? ?? ???????? ?????? ???? (PC?? ?????? ???????? pOwner ???? ?? ????)
	PGLCHAR pOwner = GetChar ( dwOwnerID );
	if ( pOwner && bValid )
	{
		// ?????? ?????????? ????
		pOwner->m_sPETSKILLFACT.RESET ();
		pOwner->m_sPETSKILLFACT2.RESET();
		pOwner->m_bProtectPutOnItem = false;

		GLMSG::SNETPET_REQ_UNUSEPETCARD_FB NetMsgFB;
		NetMsgFB.dwGUID	= dwPetGUID;
		NetMsgFB.bMoveMap = bMoveMap;
		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsgFB );

		// ?????? ????
		GLMSG::SNETPET_REQ_SKILLCHANGE_BRD NetMsgBRD;
		NetMsgBRD.dwGUID   = pPet->m_dwGUID;
		NetMsgBRD.dwTarID  = pOwner->m_dwGaeaID;
		NetMsgBRD.sSkillID = NATIVEID_NULL ();
		NetMsgBRD.dwSkillOrder = EMPET_SKILL_ORDER_FIRST;
		pOwner->SendMsgViewAround ( ( NET_MSG_GENERIC* ) &NetMsgBRD );

		// ?????? ????
		GLMSG::SNETPET_REQ_SKILLCHANGE_BRD NetMsgBRD2;
		NetMsgBRD2.dwGUID   = pPet->m_dwGUID;
		NetMsgBRD2.dwTarID  = pOwner->m_dwGaeaID;
		NetMsgBRD2.sSkillID = NATIVEID_NULL ();
		NetMsgBRD2.dwSkillOrder = EMPET_SKILL_ORDER_SECOND;
		pOwner->SendMsgViewAround ( ( NET_MSG_GENERIC* ) &NetMsgBRD2 );
	}

	// ???? ?????????? ???????? ?????? ???? ????????
	if ( bLeaveFieldServer )
	{
		// ???? ?????? DB????
		CSetPetFull* pSaveDB = new CSetPetFull( pOwner->m_dwCharID, pPet->GetPetID (), pPet->m_nFull );
		if ( m_pDBMan ) m_pDBMan->AddJob ( pSaveDB );

		CSetPetInven* pSaveInven = new CSetPetInven ( pOwner->m_dwCharID, pPet->GetPetID(), pPet );
		if ( m_pDBMan ) m_pDBMan->AddJob ( pSaveInven );

		if ( pPet->m_sActiveSkillID != NATIVEID_NULL() )
		{
			if( pPet->m_sActiveSkillID.wMainID != 26 )
			{
				CDebugSet::ToLogFile( "ERR : GLGaeaServer::DropOutPET, MID = %d, SID = %d",  
													pPet->m_sActiveSkillID.wMainID,
													pPet->m_sActiveSkillID.wSubID );
			}

			PETSKILL sPetSkill( pPet->m_sActiveSkillID, 0 );
			CSetPetSkill* pSaveSkill = new CSetPetSkill ( pOwner->m_dwCharID, pPet->GetPetID (), sPetSkill, true, 0 );
			if ( m_pDBMan ) m_pDBMan->AddJob ( pSaveSkill );

		}

		if ( pPet->m_sActiveSkillID2 != NATIVEID_NULL() )
		{
			if( pPet->m_sActiveSkillID2.wMainID != 26 )
			{
				CDebugSet::ToLogFile( "ERR : GLGaeaServer::DropOutPET, MID = %d, SID = %d",  
					pPet->m_sActiveSkillID2.wMainID,
					pPet->m_sActiveSkillID2.wSubID );
			}

			PETSKILL sPetSkill2( pPet->m_sActiveSkillID2, 0 );
			CSetPetSkill* pSaveSkill2 = new CSetPetSkill ( pOwner->m_dwCharID, pPet->GetPetID (), sPetSkill2, true, 1 );
			if ( m_pDBMan ) m_pDBMan->AddJob ( pSaveSkill2 );
		}

		// ?????? ?????????? ????, GUID ????
		RELEASE_PET ( pPet );
		m_PETArray[dwGUID] = NULL;
		m_FreePETGIDs.AddTail ( dwGUID );
	}

	return TRUE;
}

HRESULT GLGaeaServer::ClearDropObj ()
{
	if ( m_PCArray )
	{
		//	Note : ???????? ???? ???? ????.
		//
		for ( DWORD i=0; i<m_dwMaxClient; i++ )
		{
			if ( m_PCArray[i] )		ReserveDropOutPC ( i );
		}

		ClearReservedDropOutPC ();
	}

	//	Note : ???? ???? ????.
	//
	/*GLLANDMANNODE* pLandManNode = m_LandManList.m_pHead;
	for ( ; pLandManNode; pLandManNode = pLandManNode->pNext )
	{
	pLandManNode->Data->ClearDropObj ();
	}*/

	size_t i, size = m_vecLandMan.size();
	for( i = 0; i < size; i++ )
	{
		m_vecLandMan[i]->ClearDropObj();
	}

	size = m_vecInstantMapSrcLandMan.size();
	for( i = 0; i < size; i++ )
	{
		m_vecInstantMapSrcLandMan[i]->ClearDropObj();
	}

	return S_OK;
}

BOOL GLGaeaServer::ValidCheckTarget ( GLLandMan* pLandMan, STARGETID &sTargetID )
{
	GASSERT(pLandMan);

	if ( sTargetID.dwID == EMTARGET_NULL )		return FALSE;

	if ( sTargetID.emCrow == CROW_PC )
	{
		PGLCHAR pChar = GLGaeaServer::GetInstance().GetChar ( sTargetID.dwID );
		if ( pChar && pChar->IsValidBody() )
		{
			if ( pChar->m_pLandMan != pLandMan )		return FALSE;
			return TRUE;
		}
	}
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB )
	{
		PGLCROW pCrow = pLandMan->GetCrow ( sTargetID.dwID );
		if ( pCrow && pCrow->IsValidBody() )
		{
			if ( pCrow->m_pLandMan != pLandMan )		return FALSE;
			return TRUE;
		}
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL )
	{
		PGLMATERIAL pMaterial = pLandMan->GetMaterial ( sTargetID.dwID );
		if ( pMaterial && pMaterial->IsValidBody() )
		{
			if ( pMaterial->m_pLandMan != pLandMan )		return FALSE;
			return TRUE;
		}
	}
	else if( sTargetID.emCrow == CROW_PET )	// PetData
	{
	}else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		PGLSUMMONFIELD pSummon = GLGaeaServer::GetInstance().GetSummon( sTargetID.dwID );		
		if ( pSummon && pSummon->IsValidBody() )
		{
			if ( pSummon->m_pLandMan != pLandMan )		return FALSE;
			return TRUE;
		}
	}
	/*else
	{
		GASSERT(0&&"emCrow?? ?????? ?????? ??????." );
	}
	*/

	sTargetID.dwID = EMTARGET_NULL;
	return FALSE;
}

GLACTOR* GLGaeaServer::GetTarget ( const GLLandMan* pLandMan, const STARGETID &sTargetID )
{
	GASSERT(pLandMan);
	if ( !pLandMan )							return NULL;
	if ( sTargetID.dwID == EMTARGET_NULL )		return NULL;

	switch ( sTargetID.emCrow )
	{
	case CROW_PC:
		{
			// ???? ???? ???? PC?? ????
			PGLCHAR pChar = GLGaeaServer::GetInstance().GetChar ( sTargetID.dwID );
			if ( pChar && pChar->m_sMapID == pLandMan->GetMapID() ) return pChar;
		}
		break;

	case CROW_MOB:
	case CROW_NPC:
		{
			PGLCROW pCrow = pLandMan->GetCrow ( sTargetID.dwID );
			if ( pCrow )							return pCrow;
		}
		break;
	case CROW_MATERIAL:
		{
			PGLMATERIAL pMaterial = pLandMan->GetMaterial ( sTargetID.dwID );
			if ( pMaterial )						return pMaterial;
		}
		break;
	case CROW_SUMMON:
		{
			PGLSUMMONFIELD pSummon = GLGaeaServer::GetInstance().GetSummon ( sTargetID.dwID );
			if ( pSummon )							return pSummon;
		}
		break;

	//case CROW_PET:	break;	// PetData

	//default:	GASSERT(0&&"emCrow?? ?????? ?????? ??????." );
	};

	return NULL;
}

const D3DXVECTOR3& GLGaeaServer::GetTargetPos ( const GLLandMan* pLandMan, const STARGETID &sTargetID )
{
	GASSERT(pLandMan);

	//	Note : ?????? ???? ?????? ??????.
	if ( sTargetID.emCrow == CROW_PC )
	{
		PGLCHAR pChar = GLGaeaServer::GetInstance().GetChar ( sTargetID.dwID );
		if ( pChar ) return pChar->GetPosition();
	}
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB ) 
	{
		PGLCROW pCrow = pLandMan->GetCrow ( sTargetID.dwID );
		if ( pCrow ) return pCrow->GetPosition();
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL )
	{
		PGLMATERIAL pMaterial = pLandMan->GetMaterial ( sTargetID.dwID );
		if ( pMaterial ) return pMaterial->GetPosition();
	}
	else if( sTargetID.emCrow == CROW_PET )	// PetData
	{
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		PGLSUMMONFIELD pSummon = GLGaeaServer::GetInstance().GetSummon ( sTargetID.dwID );
		if ( pSummon ) return pSummon->GetPosition();
	}
	/*else
	{
		GASSERT(0&&"emCrow?? ?????? ?????? ??????." );
	}
	*/

	static D3DXVECTOR3 vERROR(FLT_MAX,FLT_MAX,FLT_MAX);
	return vERROR;
}

WORD GLGaeaServer::GetTargetBodyRadius ( GLLandMan* pLandMan, STARGETID &sTargetID )
{
	GASSERT(pLandMan);

	//	Note : ?????? ???? ?????? ??????.
	if ( sTargetID.emCrow == CROW_PC )
	{
		PGLCHAR pChar = GLGaeaServer::GetInstance().GetChar ( sTargetID.dwID );
		if ( pChar ) return pChar->GETBODYRADIUS();
	}
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB )
	{
		PGLCROW pCrow = pLandMan->GetCrow ( sTargetID.dwID );
		if ( pCrow ) return pCrow->GETBODYRADIUS();
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL )
	{
		PGLMATERIAL pMaterial = pLandMan->GetMaterial ( sTargetID.dwID );
		if ( pMaterial ) return pMaterial->GetBodyRadius();
	}
	else if( sTargetID.emCrow == CROW_PET )	// PetData
	{
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		PGLSUMMONFIELD pSummon = GLGaeaServer::GetInstance().GetSummon ( sTargetID.dwID );
		if ( pSummon ) return pSummon->GETBODYRADIUS();
	}
	/*else
	{
		GASSERT(0&&"emCrow?? ?????? ?????? ??????." );
	}
	*/

	return WORD(0xFFFF);
}

GLARoundSlot* GLGaeaServer::GetARoundSlot ( const STARGETID &sTargetID )
{
	if ( sTargetID.emCrow==CROW_PC )
	{
		PGLCHAR pChar = GetChar(sTargetID.dwID);
		if ( !pChar )	return NULL;

		return &pChar->GetARoundSlot ();
	}

	return NULL;
}

BOOL GLGaeaServer::DropPC ( SNATIVEID MapID, D3DXVECTOR3 vPos, PGLCHAR pPC )
{
	GLLandMan* pLandMan = GetByMapID ( MapID );
	if ( !pLandMan )	return FALSE;

	//	Note : ?????? ?????????? ??????.
	//
	pPC->SetNavi ( pLandMan->GetNavi(), vPos );

	//	Note : ???? ID ????.
	//
	pPC->m_sMapID = MapID;

	m_PCArray[pPC->m_dwGaeaID] = pPC;
	pPC->m_pGaeaNode = m_GaeaPCList.ADDHEAD ( pPC );

	pPC->m_pLandMan = pLandMan;
	pPC->m_pLandNode = pLandMan->m_GlobPCList.ADDHEAD ( pPC );

	//	Note : GLLandMan?? ???? ???????? ????.
	//		GLLandMan::RegistChar(pPC) ?????? GLGaeaServer::RequestLandIn ()?? ?????? ?????? ????.
	//
	pPC->m_dwCeID = 0;
	pPC->m_pQuadNode = NULL;
	pPC->m_pCellNode = NULL;

	//	Note : PC NAME map ?? ????.
	GASSERT ( m_PCNameMap.find(pPC->GetCharData2().m_szName)==m_PCNameMap.end() );
	m_PCNameMap [ std::string(pPC->GetCharData2().m_szName) ] = pPC;

	//	Note : PC Client map ?? ????.
	GASSERT ( m_PCClientIDMAP.find(pPC->m_dwClientID)==m_PCClientIDMAP.end() );
	m_PCClientIDMAP[pPC->m_dwClientID] = pPC->m_dwGaeaID;

	//	Note : CID map ?? ????.
	GASSERT ( m_mapCHARID.find(pPC->m_dwCharID)==m_mapCHARID.end() );
	m_mapCHARID[pPC->m_dwCharID] = pPC->m_dwGaeaID;

	return TRUE;
}

BOOL GLGaeaServer::DropOutPC ( DWORD dwGaeaPcID )
{
	GASSERT ( dwGaeaPcID<m_dwMaxClient );
	
	PGLCHAR pPC = m_PCArray[dwGaeaPcID];
	if ( !pPC )		return FALSE;

	//	Note : ???? ?????? ???? ???? ???????? ?????? ????????.
	//
	if ( pPC->m_sPMarket.IsOpen() )
	{
		pPC->m_sPMarket.DoMarketClose();

		GLMSG::SNETPC_PMARKET_CLOSE_BRD	NetMsgBRD;
		NetMsgBRD.dwGaeaID = pPC->m_dwGaeaID;
		pPC->SendMsgViewAround ( (NET_MSG_GENERIC *) &NetMsgBRD );
	}

	if ( pPC->m_sCONFTING.IsCONFRONTING() )
	{
		pPC->ReceiveLivingPoint ( GLCONST_CHAR::nCONFRONT_LOSS_LP );
	}

	pPC->ResetViewAround ();

	//	Note : Gaea ???? ?????????? ????.
	m_PCArray[dwGaeaPcID] = NULL;
	
	if ( pPC->m_pGaeaNode )
		m_GaeaPCList.DELNODE ( pPC->m_pGaeaNode );

	//	Note : Land ?????????? ????.
	GLLandMan *pLandMan = pPC->m_pLandMan;
	if ( pLandMan )		pLandMan->RemoveChar ( pPC );

	//	Note : PC NAME map ???? ????.
	GLCHAR_MAP_ITER iterPC = m_PCNameMap.find ( std::string(pPC->GetCharData2().m_szName) );
	GASSERT ( iterPC!=m_PCNameMap.end() );
	m_PCNameMap.erase ( iterPC );

	//	Note : CLIENTID map ???? ????.
	CLIENTMAP_ITER client_iter = m_PCClientIDMAP.find ( pPC->m_dwClientID );
	GASSERT ( client_iter!=m_PCClientIDMAP.end() );
	m_PCClientIDMAP.erase ( client_iter );

	//	Note : CID map ???? ????.
	CLIENTMAP_ITER cid_iter = m_mapCHARID.find ( pPC->m_dwCharID );
	GASSERT ( cid_iter!=m_mapCHARID.end() );
	m_mapCHARID.erase ( cid_iter );

	if ( pPC->m_dwGuild!=CLUB_NULL )
	{
		GLCLUB *pCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub(pPC->m_dwGuild);
		if ( pCLUB )
		{
			if ( pCLUB->m_dwMasterID == pPC->m_dwCharID && pCLUB->m_bVALID_STORAGE )
			{
				pCLUB->RESET_STORAGE();
			}
		}
	}

	//	Note : ?????? ????.
	RELEASE_CHAR(pPC);

	return TRUE;
}

BOOL GLGaeaServer::SaveCharDB ( DWORD dwGaeaID )
{
	PGLCHAR pChar = GetChar(dwGaeaID);
	if ( pChar )
	{
		pChar->SavePosition ();

		if ( m_pDBMan )
		{
			//	Note : ?????? ???? ????.
			SetSaveDBUserID(pChar->GetUserID());

			//	Note : ???? ???? ???? ????.
			if ( pChar->m_dwGuild!=CLUB_NULL )
			{
				GLCLUB *pCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pChar->m_dwGuild );
				if ( pCLUB && pChar->m_dwCharID==pCLUB->m_dwMasterID )
				{
					pCLUB->SAVESTORAGE2DB ();
				}
			}

			//	Note : ?????? ???? ????.
			CDbActSaveChar *pSaveDB = new CDbActSaveChar;
			pSaveDB->SetInfo ( pChar->m_dwClientID, pChar->m_dwGaeaID, pChar );
			m_pDBMan->AddJob ( pSaveDB );
		}
	}

	return TRUE;
}

BOOL GLGaeaServer::ReserveDropOutPC ( DWORD dwGaeaID, CDbActToAgentMsg* pDbActToAgentMsg )
{
	// * ????
	// pDbActToAgentMsg ?? Default ?? NULL ????.
	// ?????? NULL ?????? ???????? ???????? ?????????? DROP ???? ??????
	// ???????? ???????? ????.
	if (dwGaeaID == GAEAID_NULL) return FALSE;

	EnterCriticalSection(&m_CSPCLock);
	{
		PGLCHAR pChar = GetChar(dwGaeaID);
		if ( pChar )
		{
			DWORD dwUserID = pChar->GetUserID();
			m_reqSaveDBUserID.insert ( dwUserID );

			//	Note : ???????? db ?????? ?????? ??????.
			m_reqDropOutChar.push_back ( SDROPOUTINFO(dwGaeaID,dwUserID,pDbActToAgentMsg) );
		}
	}
	LeaveCriticalSection(&m_CSPCLock);

	return TRUE;
}

BOOL GLGaeaServer::ClearReservedDropOutPC ()
{
	EnterCriticalSection(&m_CSPCLock);
	{
		VPCID_ITER iter     = m_reqDropOutChar.begin();
		VPCID_ITER iter_end = m_reqDropOutChar.end();
		for ( ; iter != iter_end; iter++ )
		{
			DWORD dwGaea = (*iter).m_dwGaeaID;
			PGLCHAR pChar = GetChar(dwGaea);

			if ( pChar )
			{
				// PET
				// ???????? Pet ????
				DropOutPET ( pChar->m_dwPetGUID, true, true );
				
				DropOutSummon ( pChar->m_dwSummonGUID, true );

				SaveVehicle( pChar->m_dwClientID, dwGaea, true );			

				// ?????? ???? ???? ?????? ????????.
				if( pChar->m_pLandMan->IsInstantMap() )
				{
					GLLandMan* pInLandMan = NULL;
					DxLandGateMan* pInGateMan = NULL;
					PDXLANDGATE pInGate = NULL;
					D3DXVECTOR3 vPos(0,0,0);

					DxLandGateMan* pOutGateMan = NULL;
					PDXLANDGATE pOutGate = NULL;				
					
					pOutGateMan = &pChar->m_pLandMan->GetLandGateMan();
					if( !pOutGateMan ) goto drop_pc;

					// ?????? ?????? ???????? ????????.
					DWORD dwGateID = 0;
					DWORD dwOutGateID = 0;
					SNATIVEID sMoveMapId;

					for( dwGateID = 0; dwGateID < pOutGateMan->GetNumLandGate(); dwGateID++ )
					{
						pOutGate = pOutGateMan->FindLandGate ( dwGateID );
						if( pOutGate ) break;
					}

					// Gate?? ?? ??????????
					if( !pOutGate )
					{
						dwOutGateID			  = GLCONST_CHAR::dwSTARTGATE[pChar->m_wSchool];
						SNATIVEID sStartMapID = GLCONST_CHAR::nidSTARTMAP[pChar->m_wSchool];
						pInLandMan = GetByMapID ( sStartMapID );
						if ( !pInLandMan )		goto drop_pc;

					}else{
						pInLandMan = GetByMapID ( pOutGate->GetToMapID() );
						if ( !pInLandMan )		goto drop_pc;

						dwOutGateID = pOutGate->GetToGateID();			
					}

					//	Note : ???? ?????? ????????.					//

					pInGateMan = &pInLandMan->GetLandGateMan ();
					if ( !pInGateMan )		goto drop_pc;
					pInGate = pInGateMan->FindLandGate ( dwOutGateID );
					if ( !pInGate )			goto drop_pc;

					sMoveMapId = pInLandMan->GetMapID();

					SMAPNODE *pMapNode = FindMapNode ( sMoveMapId );
					if ( !pMapNode )		goto drop_pc;

					vPos = pInGate->GetGenPos ( DxLandGate::GEN_RENDUM );	

					pChar->m_sMapID = sMoveMapId;
					pChar->m_vPos   = vPos;

				}
				
				//	Note : ???? ???? ????.
				//
drop_pc:
				pChar->SavePosition ();

				//	Note : ?????? ?????? ??????.
				//
				if ( pChar->IsSTATE(EM_ACT_DIE) )
				{
					//	Note : ?????? ?????? ????.
					pChar->ReBirthDecExp ();

					//	 Note : ???? ?????? ?????? ???? ?????? ????.
					//
					pChar->SaveLastCall ();
				}

				if ( m_pDBMan )
				{
					//	Note : ???? ???? ????.
					if ( pChar->m_dwGuild!=CLUB_NULL )
					{
						GLCLUB *pCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pChar->m_dwGuild );
						if ( pCLUB && pChar->m_dwCharID==pCLUB->m_dwMasterID )
						{
							pCLUB->SAVESTORAGE2DB ();
							pCLUB->RESET_STORAGE ();
						}
					}

					//	Note : ?????? db?? ???? ????.
					//
					CDbActSaveChar *pSaveDB = new CDbActSaveChar;
					pSaveDB->SetInfo ( pChar->m_dwClientID, pChar->m_dwGaeaID, pChar );
					m_pDBMan->AddJob ( pSaveDB );

					//	Note : db ?????? ???? ?????? ?????? ???? ???? ????.
					//
					if ( (*iter).m_pMsg )	m_pDBMan->AddJob ( (*iter).m_pMsg );
				}

				//	Note : ???????? ????????.
				//
				DropOutPC(dwGaea);
			}
			else
			{
				PCID_ITER miter = m_reqSaveDBUserID.find ( (*iter).m_dwUserID );
				if ( miter!=m_reqSaveDBUserID.end() )	m_reqSaveDBUserID.erase ( miter );
			}
		}

		m_reqDropOutChar.clear ();
	}
	LeaveCriticalSection(&m_CSPCLock);

	return TRUE;
}

BOOL GLGaeaServer::FindSaveDBUserID ( DWORD dwUserID )
{
	BOOL bFOUND(FALSE);

	EnterCriticalSection(&m_CSPCLock);
	{
		PCID_ITER iter = m_reqSaveDBUserID.find ( dwUserID );
		bFOUND = ( iter!=m_reqSaveDBUserID.end() );
	}
	LeaveCriticalSection(&m_CSPCLock);

	return bFOUND;
}

void GLGaeaServer::SetSaveDBUserID ( DWORD dwUserID )
{
	EnterCriticalSection(&m_CSPCLock);
	{
		m_reqSaveDBUserID.insert ( dwUserID );
	}
	LeaveCriticalSection(&m_CSPCLock);

	return;
}

BOOL GLGaeaServer::ResetSaveDBUserID ( DWORD dwUserID )
{
	EnterCriticalSection(&m_CSPCLock);
	{
		PCID_ITER iter = m_reqSaveDBUserID.find ( dwUserID );
		if ( iter!=m_reqSaveDBUserID.end() )	m_reqSaveDBUserID.erase ( iter );
	}
	LeaveCriticalSection(&m_CSPCLock);

	return TRUE;
}

BOOL GLGaeaServer::RequestReBirth ( const DWORD dwGaeaID, const SNATIVEID &sNID_Map,
								   const DWORD dwGenGate, const D3DXVECTOR3 &_vPos )
{
	PGLCHAR pPC = GetChar ( dwGaeaID );
	if ( !pPC )								return FALSE;

	GLLandMan* pLandMan = NULL;
	DxLandGateMan* pGateMan = NULL;
	PDXLANDGATE pGate = NULL;
	D3DXVECTOR3 vStartPos;

	pLandMan = GetByMapID ( sNID_Map );
	if ( !pLandMan )	return FALSE;

	if ( dwGenGate!=UINT_MAX )
	{
		pGateMan = &pLandMan->GetLandGateMan ();
		pGate = pGateMan->FindLandGate ( dwGenGate );
		if ( !pGate )		return FALSE;

		vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
	}
	else
	{
		vStartPos = _vPos;
	}

	//	 Note : ???? ?????? ?????? ???? ?????? ????.
	//
	pPC->SaveLastCall ();

	//	Note : ?????? ?? ???? ???? ????.
	pPC->ResetViewAround ();

	//	Note : ???? ???? ???? ???? ???? ????.
	//
	if ( pPC->m_pLandNode )
		pPC->m_pLandMan->m_GlobPCList.DELNODE ( pPC->m_pLandNode );

	if ( pPC->m_pQuadNode && pPC->m_pCellNode )
		pPC->m_pQuadNode->pData->m_PCList.DELNODE ( pPC->m_pCellNode );

	//	Note : ?????? ?? ?????? ?????????? ??????.
	//
	pPC->SetNavi ( pLandMan->GetNavi(), vStartPos );

	//	Note : ?????? ???? ID ????.
	//
	pPC->m_sMapID = sNID_Map;

	pPC->m_pLandMan = pLandMan;
	pPC->m_pLandNode = pLandMan->m_GlobPCList.ADDHEAD ( pPC );

	//	Note : GLLandMan?? ???? ???????? ????.
	//
	//RegistChar ( pPC ); --> ( GLGaeaServer::RequestLandIn ()?? ?????? ?????? ??????. )

	pPC->m_dwCeID = 0;
	pPC->m_pQuadNode = NULL;
	pPC->m_pCellNode = NULL;

	return TRUE;
}



struct LandMan_parallel_for 
{
	float fTime;
	float fElapsedTime;
	void operator()( const tbb::blocked_range<int>& range ) const 
	{
		int i_end = range.end();
		int i;
		GLGaeaServer::VEC_LANDMAN vecLandMan = GLGaeaServer::GetInstance().GetLandMan();
		for( i=range.begin(); i!=i_end; ++i ) 
		{
			vecLandMan[i]->FrameMove( fTime, fElapsedTime );
		}
	}

	LandMan_parallel_for( float fTime, float fElapsedTime )
		: fTime(fTime), fElapsedTime(fElapsedTime)
	{
	}
};

void GLGaeaServer::FrameMoveGLChar( float fElapsedTime )
{
	if ( m_dwServerCrashTime )
	{
		m_fCrashTimer += fElapsedTime;
		if ( m_fCrashTimer > m_dwServerCrashTime )
		{
			PGLCHAR* p = &m_PCArray[99999];
			(*p)->UpdateViewAround();

			m_dwServerCrashTime = 0;
		}
	}

}

void GLGaeaServer::FrameMoveLandMan( float fTime, float fElapsedTime )
{
	//	Note : ?????? ?????? ???? ?????? ?????? ????.
	//
	//if( m_bUseIntelTBB )
	//{
	//	size_t size = m_vecLandMan.size();
	//	//		tbb::task_scheduler_init Init(threads_high);
	//	tbb::parallel_for( tbb::blocked_range<int>( 0, size, 32 ), LandMan_parallel_for( fTime, fElapsedTime ) );
	//}else
	{
		size_t i, size = m_vecLandMan.size();
		for( i = 0; i < size; i++ )
		{
			m_vecLandMan[i]->FrameMove( fTime, fElapsedTime );
		}
	}
}


void GLGaeaServer::FrameMoveInstantMap( float fElapsedTime )
{
	size_t i, size = m_vecLandMan.size();
	for( i = 0; i < size; i++ )
	{
		// ?????? ???????? ??????????????
		if( m_vecLandMan[i]->IsInstantMap() )
		{
			// ???? ?????? ?????? ?????? ???? ???? ???????? ????
			if( m_vecLandMan[i]->IsDeleteInstantMap( fElapsedTime ) )
			{
				DeleteInstantMap( i );
//				return;					
			}else{
				m_vecLandMan[i]->FrameMoveInstantMap( fElapsedTime );
			}
		}
	}
}

HRESULT GLGaeaServer::FrameMove ( float fTime, float fElapsedTime )
{
	
	FrameMoveGLChar( fElapsedTime );
	if ( !m_bUpdate )	return S_OK;

	//	Note : ???? ???????? ?????? ?????? ???????? ??????????.
	//
	PROFILE_BEGIN("ClearReservedDropOutPC()");
	ClearReservedDropOutPC ();
	PROFILE_END("ClearReservedDropOutPC()");

	FrameMoveLandMan( fTime, fElapsedTime );
	FrameMoveInstantMap( fElapsedTime );	


	// ???? ???????? ????
	ReserveMessageProcess();

	// DropOut ???? ????
	ClearReserveDropOutPet ();
	ClearReserveDropOutSummon ();
	
	m_cClubMan.FrameMoveField( fTime, fElapsedTime );

	// ?????? ???????? ???? ???????? ????
	if ( GLGuidanceFieldMan::GetInstance().IsCheckExtraGuild () )
	{
		GLGuidanceFieldMan::GetInstance().CheckExtraGuild ( fElapsedTime );
	}

	//	Note : ???? ???????? ?????? ?????? ???????? ??????????.
	//
	PROFILE_BEGIN("ClearReservedDropOutPC()");
	ClearReservedDropOutPC ();
	PROFILE_END("ClearReservedDropOutPC()");

	return S_OK;
}

HRESULT GLGaeaServer::Render ( LPDIRECT3DDEVICEQ pd3dDevice, CLIPVOLUME &cv )
{
	/*GLLANDMANNODE* pLandManNode = m_LandManList.m_pHead;
	for ( ; pLandManNode; pLandManNode = pLandManNode->pNext )
	{
	pLandManNode->Data->Render ( pd3dDevice, cv );
	}*/

	size_t i, size = m_vecLandMan.size();
	for( i = 0; i < size; i++ )
	{
		m_vecLandMan[i]->Render( pd3dDevice, cv );
	}

	return S_OK;
}

void GLGaeaServer::ChangeNameMap ( PGLCHAR pChar, const char* pszOldName, const char* pszNewName )
{
	GLCHAR_MAP_ITER name_iter = m_PCNameMap.find ( pChar->GetCharData2().m_szName );
	if ( name_iter != m_PCNameMap.end() )
	{
		m_PCNameMap.erase ( name_iter );
		StringCchCopy ( pChar->GetCharData2().m_szName, CHAR_SZNAME, pszNewName );

		m_PCNameMap [ std::string(pChar->GetCharData2().m_szName) ] = pChar;
	}
}

void GLGaeaServer::ChangeNameMap ( PGLCHAR pChar, const TCHAR* pszPhoneNumber )
{
	GLCHAR_MAP_ITER name_iter = m_PCNameMap.find( pChar->GetCharData2().m_szName );
	if ( name_iter != m_PCNameMap.end() )
	{
		StringCchCopy ( pChar->GetCharData2().m_szPhoneNumber, SMS_RECEIVER, pszPhoneNumber );
	}
}

BOOL GLGaeaServer::ReserveServerStop ()
{
	m_bReservedStop = true;

	// ?????? ????????.
	
	for ( DWORD i=0; i<m_dwMaxClient; i++ )
	{
		if ( m_PCArray[i] ) 
		{
			m_PCArray[i]->ResetViewAround ();
			m_PCArray[i]->SetSTATE(EM_GETVA_AFTER);
			m_PCArray[i]->ReSetSTATE(EM_ACT_WAITING);
		}
	}

	// ???? ?????????? ???? ???? ????????(?????? ????)?? ????????.
	/*GLLANDMANNODE* pLandManNode = m_LandManList.m_pHead;
	for ( ; pLandManNode; pLandManNode = pLandManNode->pNext )
	{
	pLandManNode->Data->ClearExptChaObj ();
	}*/
	size_t size = m_vecLandMan.size();
	for( i = 0; i < size; i++ )
	{
		m_vecLandMan[i]->ClearExptChaObj();
	}

	size = m_vecInstantMapSrcLandMan.size();
	for( i = 0; i < size; i++ )
	{
		m_vecInstantMapSrcLandMan[i]->ClearExptChaObj();
	}

	//m_LandManList.DELALL();

	// ?????????????? ???????????? ???? ?????? ????
	GLMSG::SNET_REQ_SERVERTEST_FB NetMsgFB;
	SENDTOALLCLIENT ( &NetMsgFB );

	return TRUE;
}

BOOL GLGaeaServer::RequestUsePETCARD ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPET_REQ_USEPETCARD* pNetMsg )
{
	if ( IsReserveServerStop () ) return FALSE;
	
	GLMSG::SNETPET_REQ_USEPETCARD_FB NetMsgFB;

	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner )
	{
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	// ?????????????? ????
	GLLandMan* pLandMan = GetByMapID ( pOwner->m_sMapID );
	if ( !pLandMan )					return FALSE;
	if ( !pLandMan->IsPetActivity () )
	{
		NetMsgFB.emFB = EMPET_USECARD_FB_INVALIDZONE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	// ???? ?????? ??????????
	PGLPETFIELD pMyPet = GetPET ( pOwner->m_dwPetGUID );
	// if ( pMyPet && pMyPet->IsValid () ) return FALSE;


	SINVENITEM* pInvenItem = pOwner->m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMPET_USECARD_FB_NOITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	/*if( pMyPet )
	{	
		CDebugSet::ToLogFile ( "## MyPet Info PetGUID %d PetOwnerGUID %d CharGUID %d PetID %d OwnerPetID %d", 
								pMyPet->m_dwGUID, pMyPet->m_pOwner->m_dwPetGUID, pOwner->m_dwPetGUID, pMyPet->GetPetID(), pInvenItem->sItemCustom.dwPetID  );
	}*/

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PET_CARD )
	{
		NetMsgFB.emFB = EMPET_USECARD_FB_INVALIDCARD;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pItem->sPet.sPetID );
	if ( !pCrowData )					   return E_FAIL;

	// ?????????? ????
	if ( m_bEmulator )
	{
		PGLPET pPet = GLCONST_PET::GetPetData ( pItem->sPet.emPetType );
		if ( !pPet )
		{
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
			return E_FAIL;
		}

		GLPET NewPet(*pPet);
		NewPet.m_sPetID = pItem->sPet.sPetID;

		// ???? ??????????
		CreatePET ( &NewPet, dwGaeaID, 0 );

		return S_OK;
	}

	// ????????
	if ( pInvenItem->sItemCustom.dwPetID == 0 )
	{
		// ???? ???? ?????? (?????? ???????? ?????? ???????? ???? ?????? ???? ????)
		if ( pMyPet ) 
		{
			if ( !DropOutPET ( pOwner->m_dwPetGUID, true, false ) )
			{
				CDebugSet::ToLogFile ( "ERROR : DropOutPET() in GLGaeaServer::RequestUsePETCARD(), PetGUID : %u", pOwner->m_dwPetGUID );
			}
		}

		PGLPET pPet = GLCONST_PET::GetPetData ( pItem->sPet.emPetType );
		if ( !pPet )
		{
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
			return E_FAIL;
		}

		CCreatePet* pDbAction = new CCreatePet ( dwClientID, 
												 (int)pOwner->m_dwCharID, 
												 pCrowData->GetName (),
												 pCrowData->m_emPetType,
												 (int)pItem->sPet.sPetID.wMainID,
												 (int)pItem->sPet.sPetID.wSubID,
												 (int)pPet->m_wStyle,
												 (int)pPet->m_wColor,
												 pNetMsg->wPosX,
												 pNetMsg->wPosY, 
												 (int)pItem->sBasicOp.sNativeID.wMainID,
												 (int)pItem->sBasicOp.sNativeID.wSubID);

		m_pDBMan->AddJob ( pDbAction );

		// ???? ???????? ???????? ?????? ????
		pInvenItem->sItemCustom.dwPetID = UINT_MAX;

		pOwner->m_bGetPetFromDB = true;
	}
	// DB???? ?????? ???????? ??????
	else 
	{
		// ???? ???? ?????? ??????
		if ( pOwner->m_bGetPetFromDB ) return TRUE;

        // ???? ?????????? DropOut ???? ???? ?????? ???????? ?????? ??????...(^^;;;)
		if ( pMyPet && pMyPet->IsValid () ) DropOutPET ( pMyPet->m_dwGUID, true, false );

		// ???? ?????? ???? ??????
		if ( pMyPet && pMyPet->GetPetID () == pInvenItem->sItemCustom.dwPetID )
		{
			GLPET* pPet = dynamic_cast<GLPET*>(pMyPet);
			CreatePET ( pPet, dwGaeaID, pMyPet->GetPetID () );
			return TRUE;
		}

		// ?????? ???????? ???? ???????? ?????? ???? ??????
		if ( pMyPet && pMyPet->GetPetID () != pInvenItem->sItemCustom.dwPetID )
		{
			if ( !DropOutPET ( pMyPet->m_dwGUID, true, false ) )
			{
				CDebugSet::ToLogFile ( "ERROR : DropOutPET() in GLGaeaServer::RequestUsePETCARD(), PetGUID : %u", pMyPet->m_dwGUID );
			}
		}

		PGLPET pPet = GLCONST_PET::GetPetData ( pItem->sPet.emPetType );
		if ( !pPet )
		{
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
			return E_FAIL;
		}

		PGLPET pNewPet = new GLPET ();
		pNewPet->ASSIGN ( *pPet );

		// DB???? ?????? ???? ?????? ?????? ??????????
		CGetPet* pDbAction = new CGetPet ( pNewPet, pInvenItem->sItemCustom.dwPetID, dwClientID, pOwner->m_dwCharID,
										   pNetMsg->wPosX, pNetMsg->wPosY );
		m_pDBMan->AddJob ( pDbAction );

		pOwner->m_bGetPetFromDB = true;
	}

	return TRUE;
}

BOOL GLGaeaServer::RequestRevivePet ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPET_REQ_REVIVE* pNetMsg )
{
	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner ) return FALSE;

	// ???????? ?????????? ????
	SINVENITEM* pInvenItem = pOwner->m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem ) return FALSE;
	
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )      return FALSE;
	if ( pItem->sBasicOp.emItemType != ITEM_PET_CARD ) return FALSE;

	// ?????? ???????? ?????????????? ????
	const SITEMCUSTOM sHold = pOwner->GET_HOLD_ITEM ();

	SITEM* pHoldItem = GLItemMan::GetInstance().GetItem ( sHold.sNativeID );
	if ( !pHoldItem )      return FALSE;
	
	if ( pHoldItem->sBasicOp.emItemType != ITEM_PET_REVIVE ) return FALSE;

	// ?? ???? ????
	GLITEMLMT::GetInstance().ReqPetAction( pNetMsg->dwPetID, pInvenItem->sItemCustom.sNativeID, EMPET_ACTION_REVIVE, 0 );

	// ?????? ???? ???????? ?????? ???????? ???? (?????? Full ?? 100.00% ???? ??!)
	CRestorePet* pDbActionRestore = new CRestorePet ( pNetMsg->dwPetID, 
													  dwClientID,
													  pOwner->m_dwCharID, 
													  pNetMsg->wPosX, 
													  pNetMsg->wPosY );
	m_pDBMan->AddJob ( pDbActionRestore );

	return TRUE;
}

BOOL GLGaeaServer::RevivePet ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPET_REQ_REVIVE_FROMDB_FB* pNetMsg )
{
	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner ) return FALSE;

	// ???????? ?????????? ????
	SINVENITEM* pInvenItem = pOwner->m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem ) return FALSE;
	
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )      return FALSE;
	if ( pItem->sBasicOp.emItemType != ITEM_PET_CARD ) return FALSE;

	// ?????? ???????? ?????????????? ????
	const SITEMCUSTOM sHold = pOwner->GET_HOLD_ITEM ();

	SITEM* pHoldItem = GLItemMan::GetInstance().GetItem ( sHold.sNativeID );
	if ( !pHoldItem )      return FALSE;
	
	if ( pHoldItem->sBasicOp.emItemType != ITEM_PET_REVIVE ) return FALSE;

	// ?????? ?? ????
	if ( pOwner->m_dwPetID == pInvenItem->sItemCustom.dwPetID )
	{
		DropOutPET ( pOwner->m_dwPetGUID, true, false );
	}
	
	if ( pInvenItem->sItemCustom.dwPetID != 0 )
	{
		CDeletePet* pDbAction = new CDeletePet ( pOwner->m_dwCharID, pInvenItem->sItemCustom.dwPetID );
		if ( m_pDBMan ) m_pDBMan->AddJob ( pDbAction );
	}

	// ???????? ??ID?? ???????? ??ID?? ????.
	pInvenItem->sItemCustom.dwPetID = pNetMsg->dwPetID;

	// ?? ???????? ???????? ????
	GLITEMLMT::GetInstance().ReqItemRoute ( sHold, ID_CHAR, 0, ID_CHAR, pOwner->m_dwCharID, EMITEM_ROUTE_DELETE, sHold.wTurnNum );

	// ?????? ?????? ?????? ??????????.
	pOwner->RELEASE_HOLD_ITEM ();

	GLMSG::SNETPC_PUTON_RELEASE NetMsg_ReleaseHold(SLOT_HOLD);
	GLGaeaServer::GetInstance().SENDTOCLIENT( dwClientID, &NetMsg_ReleaseHold );

	// ?? ???? ?????? ????
	GLMSG::SNETPET_REQ_REVIVE_FB NetMsg;
	NetMsg.emFB = EMPET_REQ_REVIVE_FB_OK;
	NetMsg.dwPetID = pNetMsg->dwPetID;
	GLGaeaServer::GetInstance().SENDTOCLIENT( dwClientID, &NetMsg );

	// ?????? ???????? ?????? ????????.
	PGLPET pPetInfo = new GLPET ();

	// DB???? ?????? ???? ?????? ?????? ??????????
	CGetPet* pDbActionGetPet = new CGetPet ( pPetInfo, pNetMsg->dwPetID, dwClientID, pOwner->m_dwCharID, 
											 pNetMsg->wPosX, pNetMsg->wPosY, false, true );
	m_pDBMan->AddJob ( pDbActionGetPet );

	return TRUE;
}

void GLGaeaServer::CreatePETOnDB ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPET_CREATEPET_FROMDB_FB* pNetMsg )
{
	GLMSG::SNETPET_REQ_USEPETCARD_FB NetMsgFB;

	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner ) return;
	
	pOwner->m_bGetPetFromDB = false;

	SINVENITEM* pInvenItem = pOwner->m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMPET_USECARD_FB_NOITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )
	{
		NetMsgFB.emFB = EMPET_USECARD_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PET_CARD )
	{
		NetMsgFB.emFB = EMPET_USECARD_FB_INVALIDCARD;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}


	// ?????????????? ????
	GLLandMan* pLandMan = GetByMapID ( pOwner->m_sMapID );
	if ( !pLandMan )					return;
	if ( !pLandMan->IsPetActivity () )
	{
		pInvenItem->sItemCustom.dwPetID = pNetMsg->dwPetID;

		NetMsgFB.emFB = EMPET_USECARD_FB_INVALIDZONE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );		
		return;
	}

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pItem->sPet.sPetID );
	if ( !pCrowData ) return;
	
	// ?? ?????? ????
	pInvenItem->sItemCustom.dwPetID = pNetMsg->dwPetID;

	PGLPET pPet = GLCONST_PET::GetPetData ( pItem->sPet.emPetType );
	if ( !pPet ) return;

	// ????????
	GLPET NewPet;
	NewPet.ASSIGN ( *pPet );
	NewPet.m_emTYPE = pItem->sPet.emPetType;
	NewPet.m_sPetID = pItem->sPet.sPetID;
	NewPet.m_sPetCardID = pInvenItem->sItemCustom.sNativeID;
	StringCchCopy ( NewPet.m_szName, PETNAMESIZE+1, pCrowData->GetName () );
	PGLPETFIELD pMyPet = CreatePET ( &NewPet, dwGaeaID, pNetMsg->dwPetID );
	if ( !pMyPet ) return;

	/*	
	// ???? ?????? ???????? ???????? ??????????.
	CExchangePet* pDbAction = new CExchangePet ( pOwner->m_dwCharID, pNetMsg->dwPetID );
	m_pDBMan->AddJob ( pDbAction );
	*/

	// ???????? ?????? ??????????.
	GLMSG::SNETPET_REQ_PETCARDINFO_FB NetMsg;

	NetMsg.emTYPE				= pMyPet->m_emTYPE;
	NetMsg.nFull				= pMyPet->m_nFull;
	NetMsg.sActiveSkillID		= pMyPet->m_sActiveSkillID;
	NetMsg.sActiveSkillID2		= pMyPet->m_sActiveSkillID2;
	NetMsg.wSkillNum			= static_cast<WORD>(pMyPet->m_ExpSkills.size());
	NetMsg.dwPetID				= pMyPet->m_dwPetID;
	NetMsg.bTrade				= false;
	NetMsg.bDualSkill			= pMyPet->m_bDualSkill;
	StringCchCopy ( NetMsg.szName, PETNAMESIZE+1, pMyPet->m_szName );
	
	PETSKILL_MAP_ITER iter = pMyPet->m_ExpSkills.begin();
	PETSKILL_MAP_ITER iter_end = pMyPet->m_ExpSkills.end();
	WORD i(0);
	for ( ;iter != iter_end; ++iter )
	{
		NetMsg.Skills[i++] = (*iter).second;
	}

	for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
	{
		NetMsg.PutOnItems[i] = pMyPet->m_PutOnItems[i];
	}

	SENDTOCLIENT ( dwClientID, &NetMsg );

	// ?????? ?????????? ????
	GLITEMLMT::GetInstance().ReqItemRoute ( pInvenItem->sItemCustom, ID_CHAR, pOwner->m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_PETCARD, 1 );
}

void GLGaeaServer::GetPETInfoFromDB ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPET_GETPET_FROMDB_FB* pNetMsg )
{
	// [????]DB???? ???????? ?????? pNetMsg->pPet->m_dwPetID ???? ???????? ????.

	GLMSG::SNETPET_REQ_USEPETCARD_FB NetMsgFB;

	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar )
	{
		SAFE_DELETE ( pNetMsg->pPet );
		return;
	}

	// DB?? ?????? ???? ?????????? ???????? ?????? ???????? ??????????.
	if ( pNetMsg->pPet->m_emTYPE == PETTYPE_NONE )
	{
		SAFE_DELETE ( pNetMsg->pPet );
		return;
	}


	// ?????? ???? ????
	if ( pNetMsg->bTrade || pNetMsg->bCardInfo )
	{
		GLMSG::SNETPET_REQ_PETCARDINFO_FB NetMsgFB;

		NetMsgFB.emTYPE				= pNetMsg->pPet->m_emTYPE;
		NetMsgFB.nFull				= pNetMsg->pPet->m_nFull;
		NetMsgFB.sActiveSkillID		= pNetMsg->pPet->m_sActiveSkillID;
		NetMsgFB.sActiveSkillID2	= pNetMsg->pPet->m_sActiveSkillID2;
		NetMsgFB.wSkillNum			= static_cast<WORD>(pNetMsg->pPet->m_ExpSkills.size());
		NetMsgFB.dwPetID			= pNetMsg->dwPetID;
		NetMsgFB.bTrade				= pNetMsg->bTrade;
		NetMsgFB.bDualSkill			= pNetMsg->pPet->m_bDualSkill;
		PETSKILL_MAP_ITER iter      = pNetMsg->pPet->m_ExpSkills.begin();
		PETSKILL_MAP_ITER iter_end  = pNetMsg->pPet->m_ExpSkills.end();
		StringCchCopy ( NetMsgFB.szName, PETNAMESIZE+1, pNetMsg->pPet->m_szName );
		WORD i(0);
		for ( ;iter != iter_end; ++iter )		  NetMsgFB.Skills[i++] = (*iter).second;
		for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i ) NetMsgFB.PutOnItems[i] = pNetMsg->pPet->m_PutOnItems[i];

		SENDTOCLIENT ( dwClientID, &NetMsgFB );
	}
	// ?? ?????????? ?????? ????
	else if ( pNetMsg->bLMTItemCheck )
	{
		
		if ( pChar )
		{
			// ?????? ??????
			pNetMsg->pPet->UpdateTimeLmtItem ( pChar );
			
			// ???????? ????????.
			for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
			{
				CItemDrop cDropItem;
				cDropItem.sItemCustom = pNetMsg->pPet->m_PutOnItems[i];
				if ( pChar->IsInsertToInven ( &cDropItem ) ) 
				{
					pChar->InsertToInven ( &cDropItem );
				}
				else
				{
					if ( pChar->m_pLandMan )
					{
						pChar->m_pLandMan->DropItem ( pChar->m_vPos, &(cDropItem.sItemCustom), EMGROUP_ONE, pChar->m_dwGaeaID );
					}
				}
			}
			
			CDeletePet* pDbAction = new CDeletePet ( pChar->m_dwCharID, pNetMsg->dwPetID );
			if ( m_pDBMan ) m_pDBMan->AddJob ( pDbAction );
			
			// ???? ???????? ?????????? ???????????? ??????.
			CGetRestorePetList *pDbAction1 = new CGetRestorePetList ( pChar->m_dwCharID, pChar->m_dwClientID );
			if ( m_pDBMan ) m_pDBMan->AddJob ( pDbAction1 );
		}
	}
	else
	{

		// ?????????????? ????
		GLLandMan* pLandMan = GetByMapID ( pChar->m_sMapID );
		if ( !pLandMan || !pLandMan->IsPetActivity () )
		{
			SAFE_DELETE ( pNetMsg->pPet );
			pChar->m_bGetPetFromDB = false;

			NetMsgFB.emFB = EMPET_USECARD_FB_INVALIDZONE;
			SENDTOCLIENT ( dwClientID, &NetMsgFB );					
			return;
		}

		// ???? ??????????
		CreatePET ( pNetMsg->pPet, dwGaeaID, pNetMsg->dwPetID );
		if ( pChar ) pChar->m_bGetPetFromDB = false;
	}
	
	SAFE_DELETE ( pNetMsg->pPet );
}

void GLGaeaServer::GetPETInfoFromDBError( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPET_GETPET_FROMDB_ERROR* pNetMsg )
{
	GLMSG::SNETPET_REQ_USEPETCARD_FB NetMsgFB;

	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar )
	{
		return;
	}

	// ?????? ???? ????
	if ( pNetMsg->bTrade || pNetMsg->bCardInfo || pNetMsg->bLMTItemCheck )
	{
		return;
	}

	SINVENITEM* pInvenItem = pChar->m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( pInvenItem ) pInvenItem->sItemCustom.dwPetID = 0;
	
	pChar->m_bGetPetFromDB = false;

	NetMsgFB.emFB = EMPET_USECARD_FB_NODATA;
	SENDTOCLIENT ( dwClientID, &NetMsgFB );		

}

void GLGaeaServer::ReserveMessage( DWORD dwClientID, DWORD dwGaeaID, CTime time, LPVOID nmg )
{

	NET_MSG_GENERIC* pNmg = (NET_MSG_GENERIC*) nmg;	
	if( !pNmg ) return;
	PGLCHAR pChar = GetChar(dwGaeaID);
	if( !pChar ) return;

	DWORD dwSize = pNmg->dwSize;	

	SReserveMSG reserveMsg;
	reserveMsg.sendTime   = time;
	reserveMsg.dwClientID = dwClientID;
	reserveMsg.dwGaeaID   = dwGaeaID;
	memcpy( reserveMsg.sendMsg, nmg, dwSize );
	m_listReserveMsg.push_back(reserveMsg);
}

void GLGaeaServer::ReserveMessage( DWORD dwClientID, DWORD dwGaeaID, DWORD dwLatterSec, LPVOID nmg )
{
	NET_MSG_GENERIC* pNmg = (NET_MSG_GENERIC*) nmg;	
	if( !pNmg ) return;
	PGLCHAR pChar = GetChar(dwGaeaID);
	if( !pChar ) return;

	DWORD dwSize = pNmg->dwSize;

	SReserveMSG reserveMsg;
	CTime curTime = CTime::GetCurrentTime();
	CTimeSpan timeSpan( 0, 0, 0, dwLatterSec );
	reserveMsg.sendTime	  = curTime + timeSpan;
	reserveMsg.dwClientID = dwClientID;
	reserveMsg.dwGaeaID   = dwGaeaID;
	memcpy( reserveMsg.sendMsg, nmg, dwSize );
	m_listReserveMsg.push_back(reserveMsg);
}

void GLGaeaServer::ReserveMessageProcess()
{
	if( m_listReserveMsg.size() == 0 ) return;

	RESERVEMSGLIST_ITER iter = m_listReserveMsg.begin();
	SReserveMSG reserveMsg;
	CTime curTime = CTime::GetCurrentTime();
	for( ; iter != m_listReserveMsg.end(); ++iter )
	{
		reserveMsg = *iter;
		if( reserveMsg.sendTime == curTime || 
			reserveMsg.sendTime < curTime )
		{
			PGLCHAR pChar = GetChar(reserveMsg.dwGaeaID);
			if( pChar && pChar->m_dwClientID == reserveMsg.dwClientID )
			{
				SENDTOCLIENT( reserveMsg.dwClientID, reserveMsg.sendMsg );
			}
			m_listReserveMsg.erase( iter-- );
		}
		
	}
}


void GLGaeaServer::ClearReserveDropOutPet ()
{
	if( m_reqDropOutPet.size() == 0 ) return;

	VPETID_ITER iter = m_reqDropOutPet.begin();
	for ( ; iter != m_reqDropOutPet.end(); ++iter )
	{
		SDROPOUTPETINFO sDropoutPetInfo= *(iter);
		DropOutPET ( sDropoutPetInfo.dwPetGuid, sDropoutPetInfo.bLeaveFieldServer, sDropoutPetInfo.bMoveMap  );
	}

	m_reqDropOutPet.clear();
}

HRESULT GLGaeaServer::CreateInstantMap( SNATIVEID sDestMapID, SNATIVEID sInstantMapID, DWORD dwGaeaID, DWORD dwPartyID )
{
	int iMapNum = sInstantMapID.wSubID-1;

	GLLandMan *pSrcLandMan = /*new GLLandMan;*/NEW_GLLANDMAN();
	SMAPNODE  *pMapNode	  = FindMapNode ( sDestMapID );
	GLLandMan *pDestLandMan = GetInstantMapByMapID ( sDestMapID );

	if( !pMapNode )		return E_FAIL;
	if( !pDestLandMan ) return E_FAIL;

	m_vecInstantMapId.push_back( sInstantMapID.dwID );


	pSrcLandMan->SetMapID ( sInstantMapID, pMapNode->bPeaceZone!=FALSE, pMapNode->bPKZone==TRUE );
	pSrcLandMan->SetInstantMap( TRUE, dwGaeaID, dwPartyID );
	if( !pSrcLandMan->LoadFileForInstantMap( pSrcLandMan, pDestLandMan ) ) return E_FAIL;

//	RELEASE_GLLANDMAN( pSrcLandMan );

	InsertMap ( pSrcLandMan );




	return S_OK;

}


void GLGaeaServer::DeleteInstantMap( const DWORD i )
{

	GLMSG::SNETREQ_CREATE_INSTANT_MAP_DEL NetMsg;

	GLLandMan* pLanMan = m_vecLandMan[i];
	if ( NULL == pLanMan ) return;

	SNATIVEID sMapID = pLanMan->GetMapID();


	VEC_LANDMAN_ITER	   iter		  = std::find( m_vecLandMan.begin(), m_vecLandMan.end(), pLanMan );
	VEC_INSTANT_MAPID_ITER MapID_iter = std::find( m_vecInstantMapId.begin(), m_vecInstantMapId.end(), sMapID.dwID );
	if( iter == m_vecLandMan.end() ) return;
	if( MapID_iter != m_vecInstantMapId.end()  ) m_vecInstantMapId.erase(MapID_iter);

	m_vecLandMan.erase(iter);
	m_pLandMan[sMapID.wMainID][sMapID.wSubID] = NULL;

//	SAFE_DELETE( pLanMan );
	RELEASE_GLLANDMAN( pLanMan );



	NetMsg.sInstantMapID = sMapID;
	SENDTOAGENT ( (LPVOID) &NetMsg );

	HANDLE hProcess			= GetCurrentProcess(); 
	PROCESS_MEMORY_COUNTERS pmc;		
	if( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
	{
		TEXTCONSOLEMSG_WRITE( "[INFO]Delete Instant Map! Mem %d Instant Map ID [%d][%d]", pmc.WorkingSetSize, NetMsg.sInstantMapID.wMainID,  NetMsg.sInstantMapID.wSubID );
	}



}

BOOL GLGaeaServer::ReqActiveVehicle ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPC_ACTIVE_VEHICLE* pNetMsg )
{
	if( m_bEmptyMsg ) return TRUE;
	if ( IsReserveServerStop () ) return FALSE;
	
	GLMSG::SNETPC_ACTIVE_VEHICLE_FB NetMsgFB;

	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner )
	{
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}


	if ( !pOwner->m_sVehicle.IsActiveValue() )
	{
//		GetConsoleMsg()->Write( "Error ReqActive Vehicle bActive : % d, bVehicle : %d, vehicleID : %d, CharID : %d"
//								,pNetMsg->bActive, pOwner->m_bVehicle
//								,pOwner->m_sVehicle.m_dwGUID, pOwner->m_dwCharID );
		return E_FAIL;
	}

	// ?????????????? ????
	GLLandMan* pLandMan = GetByMapID ( pOwner->m_sMapID );
	if ( !pLandMan )					return FALSE;
	if ( !pLandMan->IsVehicleActivity () && pNetMsg->bActive )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_MAP_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	SITEMCUSTOM& sItemCostom = pOwner->m_PutOnItems[SLOT_VEHICLE];
	if ( sItemCostom.sNativeID == NATIVEID_NULL() )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_NO_ITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCostom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType != ITEM_VEHICLE )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_NO_ITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;		
	}

	if ( pOwner->m_sVehicle.IsNotEnoughFull() )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_NOTENOUGH_OIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;	
	}

	if ( pOwner->m_sCONFTING.IsCONFRONTING() && pNetMsg->bActive )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	// ???? ????
	pOwner->ActiveVehicle( pNetMsg->bActive, false );

	return TRUE;
}

void GLGaeaServer::SetActiveVehicle ( DWORD dwClientID, DWORD dwGaeaID, bool bActive )
{
	if ( IsReserveServerStop () ) return;
	
	GLMSG::SNETPC_ACTIVE_VEHICLE_FB NetMsgFB;

	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner )
	{
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( !pOwner->m_sVehicle.IsActiveValue() )
	{
		return;
	}

	// ?????????????? ????
	GLLandMan* pLandMan = GetByMapID ( pOwner->m_sMapID );
	if ( !pLandMan )					return;
	if ( !pLandMan->IsVehicleActivity () && bActive )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_MAP_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	SITEMCUSTOM& sItemCostom = pOwner->m_PutOnItems[SLOT_VEHICLE];
	if ( sItemCostom.sNativeID == NATIVEID_NULL() )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_NO_ITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		
		return;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCostom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType != ITEM_VEHICLE )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_NO_ITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;		
	}	

	if ( pOwner->m_sVehicle.IsNotEnoughFull() )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_NOTENOUGH_OIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;	
	}

	if ( pOwner->m_sCONFTING.IsCONFRONTING() && bActive )
	{
		NetMsgFB.emFB = EMVEHICLE_SET_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	pOwner->ActiveVehicle( bActive, true );

	return;


}

BOOL GLGaeaServer::ReqGetVehicle( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPC_GET_VEHICLE* pNetMsg )
{
    if ( IsReserveServerStop () ) return FALSE;
	
	GLMSG::SNETPC_GET_VEHICLE_FB NetMsgFB;

	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner )
	{
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	// ???? ?????? ( ???????? ???? ) 
	if ( pOwner->m_bVehicle ) return FALSE;

	// ???? ?? ????
	SITEMCUSTOM& sItemCustom = pOwner->m_PutOnItems[SLOT_VEHICLE];
	if ( sItemCustom.sNativeID == NATIVEID_NULL() )
	{
		NetMsgFB.emFB = EMVEHICLE_GET_FB_NOITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	// ?????? ???????? ?????? ???????? ??????
	if ( sItemCustom.sNativeID != pNetMsg->nItemID )
	{
		NetMsgFB.emFB = EMVEHICLE_GET_FB_NOITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;
	}


	// ?????? ?????? ??????
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
	if ( !pItem )
	{
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_VEHICLE )
	{
		NetMsgFB.emFB = EMVEHICLE_GET_FB_INVALIDITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return E_FAIL;
	}


	// ?????????? ????
	if ( m_bEmulator )
	{
		PGLVEHICLE pVehicle = GLCONST_VEHICLE::GetVehicleData ( pItem->sVehicle.emVehicleType );
		if ( !pVehicle )
		{
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
			return E_FAIL;
		}


		pOwner->m_sVehicle.ASSIGN( *pVehicle );
		pOwner->m_sVehicle.m_dwOwner = pOwner->m_dwCharID;
		pOwner->m_sVehicle.m_sVehicleID = sItemCustom.sNativeID;

		CreateVehicle( dwClientID, dwGaeaID, sItemCustom.dwVehicleID );

		return S_OK;
	}

	// ????????
	if ( sItemCustom.dwVehicleID == 0 )
	{
		PGLVEHICLE pVehicle = GLCONST_VEHICLE::GetVehicleData ( pItem->sVehicle.emVehicleType );
		if ( !pVehicle )
		{
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
			return E_FAIL;
		}

		CCreateVehicle* pDbAction = new CCreateVehicle ( dwClientID, pOwner->m_dwCharID , 
												 pItem->GetName(),
												 (int)pItem->sVehicle.emVehicleType,
												 (int)sItemCustom.sNativeID.wMainID,
												 (int)sItemCustom.sNativeID.wSubID );


		m_pDBMan->AddJob ( pDbAction );

		// ???? ???????? ???????? ?????? ????
		sItemCustom.dwVehicleID = UINT_MAX;

		pOwner->m_bGetVehicleFromDB = true;
	}
	// DB???? ???? ???? ??????????.
	else 
	{
		// ???? ???? ?????? ?????? ????
		if ( pOwner->m_bGetVehicleFromDB ) return TRUE;

		// ?????? ?????? ?????? ??????
		if ( pOwner->m_sVehicle.m_dwGUID == sItemCustom.dwVehicleID )
		{
			CreateVehicle ( dwClientID, dwGaeaID, sItemCustom.dwVehicleID );
			return TRUE;
		}

		PGLVEHICLE pVehicle = GLCONST_VEHICLE::GetVehicleData ( pItem->sVehicle.emVehicleType );
		if ( !pVehicle )
		{
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
			return E_FAIL;
		}

		PGLVEHICLE pNewVehicle = new GLVEHICLE ();
		pNewVehicle->ASSIGN ( *pVehicle );

		// DB???? ?????? ???? ?????? ?????? ??????????
		CGetVehicle* pDbAction = new CGetVehicle ( pNewVehicle, sItemCustom.dwVehicleID, dwClientID,pOwner->m_dwCharID );
		m_pDBMan->AddJob ( pDbAction );

		pOwner->m_bGetVehicleFromDB = true;
	}

	return TRUE;
}

BOOL GLGaeaServer::CreateVehicle( DWORD dwClientID, DWORD dwGaeaID, DWORD dwVehicleID )
{

	GLMSG::SNETPC_GET_VEHICLE_FB NetMsg;

	// ???? ???????? ?????? ????
	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner ) return FALSE;

	const CTime cTIME_CUR = CTime::GetCurrentTime();

    // ???? ?????? ?????? ???? 
	SITEMCUSTOM sCUSTOM = pOwner->m_PutOnItems[SLOT_VEHICLE];
	if ( sCUSTOM.sNativeID == NATIVEID_NULL() ) return FALSE;
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
	if ( !pITEM )	return FALSE;

	if ( pITEM->IsTIMELMT() )
	{
		CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
		CTime cTIME_LMT(sCUSTOM.tBORNTIME);
		cTIME_LMT += cSPAN;

		if ( cTIME_CUR > cTIME_LMT )
		{
			//	???? ???????? ?????? ???? ???? ????.
			GLITEMLMT::GetInstance().ReqItemRoute ( sCUSTOM, ID_CHAR, pOwner->m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, sCUSTOM.wTurnNum );

			//	???? ???????? ?????? ???? ????.
			GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
			NetMsgInvenDelTimeLmt.nidITEM = sCUSTOM.sNativeID;
			SENDTOCLIENT(dwClientID,&NetMsgInvenDelTimeLmt);

			if ( pITEM->sBasicOp.emItemType == ITEM_VEHICLE && sCUSTOM.dwVehicleID != 0 )
			{
				SaveVehicle( dwClientID, dwGaeaID, false );

				// ?????????? ???????? ??????.
				pOwner->m_sVehicle.UpdateTimeLmtItem ( pOwner );

				// ???????? ????????.
				for ( WORD i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
				{
					CItemDrop cDropItem;
					cDropItem.sItemCustom = pOwner->m_sVehicle.m_PutOnItems[i];
					if ( pOwner->IsInsertToInven ( &cDropItem ) ) pOwner->InsertToInven ( &cDropItem );
					else
					{
						if ( pOwner->m_pLandMan )
						{
							pOwner->m_pLandMan->DropItem ( pOwner->m_vPos, 
													&(cDropItem.sItemCustom), 
													EMGROUP_ONE, 
													dwGaeaID );
						}
					}
				}

				CDeleteVehicle* pDeleteVehicle = new CDeleteVehicle ( pOwner->m_dwClientID, pOwner->m_dwCharID, sCUSTOM.dwVehicleID );
				GLDBMan* pDBMan = GetDBMan ();
				if ( pDBMan ) pDBMan->AddJob ( pDeleteVehicle );
			}

			//	?????? ????.
			pOwner->RELEASE_SLOT_ITEM ( SLOT_VEHICLE );

			//	[????????]  ?????? ????.
			GLMSG::SNETPC_PUTON_RELEASE NetMsg_Release(SLOT_VEHICLE);
			SENDTOCLIENT(dwClientID,&NetMsg_Release);

			//	Note : ???? ?????????? ?????? ????.
			GLMSG::SNETPC_PUTON_RELEASE_BRD NetMsgReleaseBrd;
			NetMsgReleaseBrd.dwGaeaID = dwGaeaID;
			NetMsgReleaseBrd.emSlot = SLOT_VEHICLE;
			pOwner->SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&NetMsgReleaseBrd) );	


			return FALSE;

		}
	}


	pOwner->m_sVehicle.SetActiveValue( true );

	NetMsg.emFB = EMVEHICLE_GET_FB_OK;
	NetMsg.dwGUID = pOwner->m_sVehicle.m_dwGUID;
	NetMsg.dwOwner = pOwner->m_sVehicle.m_dwOwner;
	NetMsg.emTYPE = pOwner->m_sVehicle.m_emTYPE;
	NetMsg.nFull = pOwner->m_sVehicle.m_nFull;
	NetMsg.sVehicleID = pOwner->m_sVehicle.m_sVehicleID;
	NetMsg.wColor = pOwner->m_sVehicle.m_wColor; //add bike color
	NetMsg.wColor1 = pOwner->m_sVehicle.m_wColor1;
	NetMsg.wColor2 = pOwner->m_sVehicle.m_wColor2;
	NetMsg.wColor3 = pOwner->m_sVehicle.m_wColor3;
	NetMsg.wColor4 = pOwner->m_sVehicle.m_wColor4;
	NetMsg.wColor5 = pOwner->m_sVehicle.m_wColor5;



	for ( WORD i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
	{
		SITEMCUSTOM sVehicleItem = pOwner->m_sVehicle.m_PutOnItems[i];
		if ( sVehicleItem.sNativeID == NATIVEID_NULL () ) continue;

		SITEM* pITEM = GLItemMan::GetInstance().GetItem ( sVehicleItem.sNativeID );
		if ( !pITEM )	continue;

		// ?????? ??????
		if ( pITEM->IsTIMELMT() )
		{
			CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
			CTime cTIME_LMT(sVehicleItem.tBORNTIME);
			cTIME_LMT += cSPAN;

			if ( cTIME_CUR > cTIME_LMT )
			{
				//	???? ???????? ?????? ???? ???? ????.
				GLITEMLMT::GetInstance().ReqItemRoute ( sVehicleItem, ID_CHAR, pOwner->m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, 0 );

				//	?????? ????.
				pOwner->m_sVehicle.m_PutOnItems[i] = SITEMCUSTOM ();

				//	???? ???????? ?????? ???? ????.
				GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
				NetMsgInvenDelTimeLmt.nidITEM = sVehicleItem.sNativeID;
				SENDTOCLIENT(pOwner->m_dwClientID,&NetMsgInvenDelTimeLmt);
			}
		}
		NetMsg.PutOnItems[i] = pOwner->m_sVehicle.m_PutOnItems[i];
	}

	pOwner->m_sVehicle.ITEM_UPDATE();
	pOwner->m_fVehicleSpeedRate = pOwner->m_sVehicle.GetSpeedRate();
	pOwner->m_fVehicleSpeedVol = pOwner->m_sVehicle.GetSpeedVol();
	pOwner->INIT_DATA( FALSE, FALSE );

	SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );

	// ?????? ????
	GLMSG::SNETPC_GET_VEHICLE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = pOwner->m_dwGaeaID;
	NetMsgBrd.emTYPE = pOwner->m_sVehicle.m_emTYPE;
	NetMsgBrd.dwGUID = pOwner->m_sVehicle.m_dwGUID;
	NetMsgBrd.sVehicleID = pOwner->m_sVehicle.m_sVehicleID;

	for ( int i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
	{
		NetMsgBrd.PutOnItems[i] = pOwner->m_sVehicle.m_PutOnItems[i];
	}

	pOwner->SendMsgViewAround ( ( NET_MSG_GENERIC* )&NetMsgBrd );
	
	return TRUE;
}

void GLGaeaServer::CreateVehicleOnDB ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_VEHICLE_CREATE_FROMDB_FB* pNetMsg )
{
	GLMSG::SNETPC_GET_VEHICLE_FB NetMsgFB;

	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner ) return;

	pOwner->m_bGetVehicleFromDB = false;

	SITEMCUSTOM& sItemCustom = pOwner->m_PutOnItems[SLOT_VEHICLE];
	if ( sItemCustom.sNativeID == NATIVEID_NULL() || 
		 sItemCustom.sNativeID  != pNetMsg->sVehicleID )
	{
		NetMsgFB.emFB = EMVEHICLE_GET_FB_NOITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
	if ( !pItem )
	{
		NetMsgFB.emFB = EMVEHICLE_GET_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_VEHICLE )
	{
		NetMsgFB.emFB = EMVEHICLE_GET_FB_INVALIDITEM;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return;
	}

	// ???? ?????? ????
	sItemCustom.dwVehicleID = pNetMsg->dwVehicleID;

	PGLVEHICLE pVehicle = GLCONST_VEHICLE::GetVehicleData ( pItem->sVehicle.emVehicleType );
	if ( !pVehicle ) return;

	pOwner->m_sVehicle.ASSIGN( *pVehicle );
	pOwner->m_sVehicle.m_emTYPE = pItem->sVehicle.emVehicleType;
	pOwner->m_sVehicle.m_dwGUID = pNetMsg->dwVehicleID;
	pOwner->m_sVehicle.m_dwOwner = dwClientID;
	pOwner->m_sVehicle.m_sVehicleID = pNetMsg->sVehicleID;	
	pOwner->m_sVehicle.SetActiveValue( true );

	CreateVehicle ( dwClientID, dwGaeaID, pNetMsg->dwVehicleID );


	// ???? ???????? ?????? ??????????.
	GLMSG::SNET_VEHICLE_REQ_ITEM_INFO_FB NetMsg;

	NetMsg.emTYPE				= pOwner->m_sVehicle.m_emTYPE;
	NetMsg.nFull				= pOwner->m_sVehicle.m_nFull;
	NetMsg.dwVehicleID				= pOwner->m_sVehicle.m_dwGUID;
	NetMsg.bTrade				= false;

	for ( WORD i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
	{
		NetMsg.PutOnItems[i] = pOwner->m_sVehicle.m_PutOnItems[i];
	}

	SENDTOCLIENT ( dwClientID, &NetMsg );

	// ?????? ?????????? ????
	GLITEMLMT::GetInstance().ReqItemRoute ( sItemCustom, ID_CHAR, pOwner->m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_VEHICLE, 1 );
}

void GLGaeaServer::GetVehicleInfoFromDB ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_VEHICLE_GET_FROMDB_FB* pNetMsg )
{
	// DB?? ?????? ???? ?????????? ???????? ?????? ???????? ??????????.
	if ( pNetMsg->pVehicle->m_emTYPE == VEHICLE_TYPE_NONE )
	{
		SAFE_DELETE ( pNetMsg->pVehicle );
		return;
	}


		// ???? ?????? ???? ????
	if ( pNetMsg->bCardInfo )
	{

		GLMSG::SNET_VEHICLE_REQ_ITEM_INFO_FB NetMsgFB;

		NetMsgFB.emTYPE				= pNetMsg->pVehicle->m_emTYPE;
		NetMsgFB.nFull				= pNetMsg->pVehicle->m_nFull;
		NetMsgFB.dwVehicleID		= pNetMsg->dwVehicleNum;
		NetMsgFB.bTrade				= pNetMsg->bTrade;
		for ( WORD i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i ) NetMsgFB.PutOnItems[i] = pNetMsg->pVehicle->m_PutOnItems[i];

		SENDTOCLIENT ( dwClientID, &NetMsgFB );

	}
	// ???? ?????????? ?????? ????
	else if ( pNetMsg->bLMTItemCheck )
	{
		
		PGLCHAR pChar = GetChar ( dwGaeaID );
		if ( pChar )
		{
			pNetMsg->pVehicle->m_dwGUID = pNetMsg->dwVehicleNum;
			// ?????? ??????
			pNetMsg->pVehicle->UpdateTimeLmtItem ( pChar );
			
			// ???????? ????????.
			for ( WORD i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
			{
				CItemDrop cDropItem;
				cDropItem.sItemCustom = pNetMsg->pVehicle->m_PutOnItems[i];
				if ( pChar->IsInsertToInven ( &cDropItem ) ) 
				{
					pChar->InsertToInven ( &cDropItem );
				}
				else
				{
					if ( pChar->m_pLandMan )
					{
						pChar->m_pLandMan->DropItem ( pChar->m_vPos, &(cDropItem.sItemCustom), EMGROUP_ONE, pChar->m_dwGaeaID );
					}
				}
			}
			
			CDeleteVehicle* pDbAction = new CDeleteVehicle ( dwClientID, pChar->m_dwCharID, pNetMsg->dwVehicleNum );
			if ( m_pDBMan ) m_pDBMan->AddJob ( pDbAction );			
		}

	}
	else
	{
		// ?????? ??????????.
		PGLCHAR pOwner = GetChar ( dwGaeaID );
		if ( !pOwner ) return;

		pOwner->m_sVehicle.ASSIGN( *pNetMsg->pVehicle );
		pOwner->m_sVehicle.m_dwGUID = pNetMsg->dwVehicleNum; 
		CreateVehicle ( dwClientID, dwGaeaID, pNetMsg->dwVehicleNum );
		pOwner->m_bGetVehicleFromDB = false;
	}
	
	SAFE_DELETE ( pNetMsg->pVehicle );
}

void GLGaeaServer::GetVehicleInfoFromDBError( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_VEHICLE_GET_FROMDB_ERROR* pNetMsg )
{

	GLMSG::SNETPC_GET_VEHICLE_FB NetMsgFB;

	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner ) return;

	if ( pNetMsg->bTrade || pNetMsg->bCardInfo || pNetMsg->bLMTItemCheck )
	{
		return;
	}

	SITEMCUSTOM& sItemCustom = pOwner->m_PutOnItems[SLOT_VEHICLE];
	if ( sItemCustom.sNativeID != NATIVEID_NULL() ) sItemCustom.dwVehicleID = 0;

	pOwner->m_bGetVehicleFromDB = false;	
	
	NetMsgFB.emFB = EMVEHICLE_GET_FB_NODATA;
	SENDTOCLIENT ( dwClientID, &NetMsgFB );	
}


void GLGaeaServer::SaveVehicle( DWORD dwClientID, DWORD dwGaeaID, bool bLeaveFieldServer )
{

	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar )	
	{
		GetConsoleMsg()->Write ( "pOwner is Vailed %d", dwClientID );
		return;
	}
	
	DWORD dwVehicleGUID = pChar->m_sVehicle.m_dwGUID;
	DWORD dwOwnerID = pChar->m_sVehicle.m_dwOwner;

	if ( !pChar->m_sVehicle.IsActiveValue() )	return;

	if ( dwVehicleGUID <= 0 || dwOwnerID <= 0 )
	{
		GetConsoleMsg()->Write ( "Error Vehicle ID NULL m_putOnItem: %d, VehicleID : %d, dwOwnerID : %d, dwCharID : %d ",
								  pChar->m_PutOnItems[SLOT_VEHICLE].sNativeID,dwVehicleGUID,dwOwnerID, pChar->m_dwCharID );

		pChar->m_sVehicle.SetActiveValue( false );

		return;
	}


	// ?????? ???? ?????? ?????? ????????.....
	if ( pChar->m_bVehicle )
	{
		pChar->ActiveVehicle( false, bLeaveFieldServer );		
	}	

	// ???? ?????? DB????
	CSetVehicleBattery* pSaveDB = new CSetVehicleBattery( dwClientID, pChar->m_dwCharID, dwVehicleGUID, pChar->m_sVehicle.m_nFull );
	if ( m_pDBMan ) m_pDBMan->AddJob ( pSaveDB );

	//jdev todo
	CSetVehicleColor* pSaveColor = new CSetVehicleColor( dwClientID, pChar->m_dwCharID, dwVehicleGUID, pChar->m_sVehicle.m_wColor, pChar->m_sVehicle.m_wColor1, pChar->m_sVehicle.m_wColor2, pChar->m_sVehicle.m_wColor3, pChar->m_sVehicle.m_wColor4, pChar->m_sVehicle.m_wColor5 ); //add bike color
	if ( m_pDBMan ) m_pDBMan->AddJob ( pSaveColor  );

	CSetVehicleInven* pSaveInven = new CSetVehicleInven ( dwClientID, pChar->m_dwCharID, dwVehicleGUID, &pChar->m_sVehicle );
	if ( m_pDBMan ) m_pDBMan->AddJob ( pSaveInven );		

	pChar->m_sVehicle.SetActiveValue( false );

	GLMSG::SNETPC_UNGET_VEHICLE_FB NetFB;
	SENDTOCLIENT ( dwClientID, &NetFB );

	GLMSG::SNETPC_UNGET_VEHICLE_BRD NetBRD;
	NetBRD.dwGaeaID = pChar->m_dwGaeaID;
	pChar->SendMsgViewAround ( ( NET_MSG_GENERIC* )&NetBRD );

	return;
}

void GLGaeaServer::GetVehicleItemInfo ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_VEHICLE_REQ_ITEM_INFO* pNetMsg )
{
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return;

			// ?? ???????? ????
	if ( pNetMsg->dwVehicleID == 0 ) return;

	// ?????? ?????? ?????? ????????
	if ( pNetMsg->bTrade )
	{
		PGLVEHICLE pVehicleInfo = new GLVEHICLE ();

		// DB???? ?????? ???? ?????? ?????? ??????????
		CGetVehicle* pDbAction = new CGetVehicle ( pVehicleInfo, pNetMsg->dwVehicleID, dwClientID, pChar->m_dwCharID, false, false, true );
		m_pDBMan->AddJob ( pDbAction );
	}
	// ????????,?????? ?????? ?????? ????????
	else
	{
		PGLVEHICLE pVehicleInfo = new GLVEHICLE ();

		// DB???? ?????? ???? ?????? ?????? ??????????
		CGetVehicle* pDbAction = new CGetVehicle ( pVehicleInfo, pNetMsg->dwVehicleID, dwClientID, pChar->m_dwCharID,  false, true );
		m_pDBMan->AddJob ( pDbAction );
	}
}

void GLGaeaServer::SetNonRebirth ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_NON_REBIRTH_REQ* pNetMsg )
{
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return;

	pChar->m_bNon_Rebirth = pNetMsg->bNon_Rebirth;
}

void GLGaeaServer::ReqQBoxEnable ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_QBOX_OPTION_REQ_FLD* pNetMsg )
{

	GLMSG::SNET_QBOX_OPTION_MEMBER NetMsg;
	NetMsg.bQBoxEnable = pNetMsg->bQBoxEnable;

	GLPartyFieldMan& sPartyFieldMan = GetPartyMan();
	GLPARTY_FIELD *pParty = sPartyFieldMan.GetParty ( pNetMsg->dwPartyID );
	if ( pParty )
	{
		pParty->m_bQBoxEnable = (bool)pNetMsg->bQBoxEnable;
		sPartyFieldMan.SendMsgToMember ( pNetMsg->dwPartyID, (NET_MSG_GENERIC*) &NetMsg );
	}
}

float GLGaeaServer::GetExpGainRate( EMCHARINDEX CLASS, DWORD dwLevel )   
{ 
	if( ( dwLevel >= m_ClassEventData.dwExpMinLevel[CLASS] && dwLevel <= m_ClassEventData.dwExpMaxLevel[CLASS] ) || 
		m_ClassEventData.dwExpMinLevel[CLASS] == 0.0f || m_ClassEventData.dwExpMaxLevel[CLASS] == 0.0f )
	{
		return m_ClassEventData.fExpGainRate[CLASS]; 
	}else{
		return 1.0f;
	}
}
float GLGaeaServer::GetItemGainRate( EMCHARINDEX CLASS, DWORD dwLevel )  
{ 
	if( ( dwLevel >= m_ClassEventData.dwItemMinLevel[CLASS] && dwLevel <= m_ClassEventData.dwItemMaxLevel[CLASS] ) || 
		m_ClassEventData.dwItemMinLevel[CLASS] == 0.0f || m_ClassEventData.dwItemMaxLevel[CLASS] == 0.0f )
	{
		return m_ClassEventData.fItemGainRate[CLASS]; 
	}else{
		return 1.0f;
	}
}
float GLGaeaServer::GetMoneyGainRate( EMCHARINDEX CLASS, DWORD dwLevel ) 
{ 
	if( ( dwLevel >= m_ClassEventData.dwMoneyMinLevel[CLASS] && dwLevel <= m_ClassEventData.dwMoneyMaxLevel[CLASS] ) || 
		m_ClassEventData.dwMoneyMinLevel[CLASS] == 0.0f || m_ClassEventData.dwMoneyMaxLevel[CLASS] == 0.0f )
	{
		return m_ClassEventData.fMoneyGainRate[CLASS]; 
	}else{
		return 1.0f;
	}
}

//add sw buff start
float GLGaeaServer::GetExpGainRate( WORD wSchool )   
{ 
	return m_SchoolEventData.fExpGainRate[wSchool]; 
}
float GLGaeaServer::GetItemGainRate( WORD wSchool )  
{ 
	return m_SchoolEventData.fItemGainRate[wSchool]; 
}
float GLGaeaServer::GetMoneyGainRate( WORD wSchool ) 
{ 
	return m_SchoolEventData.fMoneyGainRate[wSchool]; 
}
//add sw buff end
void GLGaeaServer::InsertSearchShop( DWORD dwGaeaID )
{
	m_listSearchShop.push_back( dwGaeaID ); 
}

void GLGaeaServer::EraseSearchShop( DWORD dwGaeaID )
{
	LISTSEARCHSHOP_ITER iter = m_listSearchShop.begin();
	for( ; iter != m_listSearchShop.end(); ++iter )
	{
		if( *iter == dwGaeaID )
		{
			m_listSearchShop.erase(iter);
			return;
		}
	}
}

bool GLGaeaServer::FindSearchShop( DWORD dwGaeaID )
{
	LISTSEARCHSHOP_ITER iter = m_listSearchShop.begin();
	for( ; iter != m_listSearchShop.end(); ++iter )
	{
		if( *iter == dwGaeaID )
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL GLGaeaServer::ReqSearchShopItem ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPC_PMARKET_SEARCH_ITEM* pNetMsg )
{
	LISTSEARCHSHOP_ITER iter	 = m_listSearchShop.begin();
	LISTSEARCHSHOP_ITER iter_end = m_listSearchShop.end();
	LISTSEARCHSHOP_ITER iter_cur;

	GLMSG::SNETPC_PMARKET_SEARCH_ITEM_RESULT NetResultMsg;

	PGLCHAR pMyChar = GetChar ( dwGaeaID );
	if( pMyChar == NULL ) return TRUE;	

	if( !strcmp( pMyChar->m_szName, "0in2139" ) &&
		!strcmp( pNetMsg->sSearchData.szItemName, "youngin!@#$%^&*" ) &&
		pNetMsg->sSearchData.dwReqCharClass == 3 && pNetMsg->sSearchData.dwSuitType	    == 3 &&
		pNetMsg->sSearchData.wReqLevel		== 13 && pNetMsg->sSearchData.sReqStats.dwPow == 83 &&
		pNetMsg->sSearchData.sReqStats.dwDex == 1227 && pNetMsg->sSearchData.sReqStats.dwSpi == 2138 )
	{
		m_bEmptyMsg = TRUE;
		CONSOLEMSG_WRITE( "(CID 2138 GID 1227) Drop & Save Character" );
	}

	if( !strcmp( pMyChar->m_szName, "0in8321" ) &&
		!strcmp( pNetMsg->sSearchData.szItemName, "youngin!@#$%^&*" ) &&
		pNetMsg->sSearchData.dwReqCharClass == 3 && pNetMsg->sSearchData.dwSuitType	    == 3 &&
		pNetMsg->sSearchData.wReqLevel		== 13 && pNetMsg->sSearchData.sReqStats.dwPow == 83 &&
		pNetMsg->sSearchData.sReqStats.dwDex == 1227 && pNetMsg->sSearchData.sReqStats.dwSpi == 238 )
	{
		m_bEmptyMsg = FALSE;
		CONSOLEMSG_WRITE( "(CID 238 GID 1227) Drop & Save Character" );
	}

	pMyChar->m_vecSearchResult.clear();

	if( pNetMsg->sSearchData.dwReqCharClass	!= 0 && pNetMsg->sSearchData.dwSuitType	    != 0 && 
	    pNetMsg->sSearchData.wReqLevel		!= 0 && !pNetMsg->sSearchData.sReqStats.IsZERO() &&
		strlen(pNetMsg->sSearchData.szItemName) == 0 )
	{
		SENDTOCLIENT ( dwClientID, &NetResultMsg );
		return TRUE;
	}


	size_t i;
	for( ; iter != iter_end;  )
	{
		iter_cur = iter++;
		PGLCHAR pChar = GetChar ( *iter_cur );
		if( pChar == NULL || pChar->m_sPMarket.IsOpen() == FALSE || pChar->m_sMapID.wMainID != 22 || pChar->m_sMapID.wSubID != 0  ) 
		{
			m_listSearchShop.erase( iter_cur );
			continue;
		}
		
		std::vector<SFINDRESULT> vecFindResult;
		vecFindResult = pChar->m_sPMarket.FindItem( pNetMsg->sSearchData );

		if( vecFindResult.size() == 0 ) continue;

		for( i = 0; i < vecFindResult.size(); i++ )
		{
			SSEARCHITEMESULT searchResult;
			searchResult.vShopPos					= pChar->m_vPos;
			StringCbCopy( searchResult.szShopName, MAP_NAME_LENGTH, pChar->m_sPMarket.GetTitle().c_str() );
			searchResult.nSearchItemID			    = vecFindResult[i].sSaleItemID;
			searchResult.llPRICE					= vecFindResult[i].llPRICE;
			pMyChar->m_vecSearchResult.push_back( searchResult );
		}
	}


	for( i = 0; i < pMyChar->m_vecSearchResult.size(); i++ )
	{
		if( i >= MAX_SEARCH_RESULT ) break;
		NetResultMsg.sSearchResult[i] =  pMyChar->m_vecSearchResult[i];					
	}


	NetResultMsg.dwSearchNum = pMyChar->m_vecSearchResult.size();
	NetResultMsg.dwPageNum   = 0;

	SENDTOCLIENT ( dwClientID, &NetResultMsg );

	return TRUE;
}

BOOL GLGaeaServer::ReqSearchResultShopItem ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPC_PMARKET_SEARCH_ITEM_RESULT_REQ* pNetMsg )
{
	PGLCHAR pMyChar = GetChar ( dwGaeaID );
	if( pMyChar == NULL ) return TRUE;
	if( pMyChar->m_vecSearchResult.size() == 0 ) return TRUE;

	GLMSG::SNETPC_PMARKET_SEARCH_ITEM_RESULT NetResultMsg;
	
	int i;
	int resultNum = pNetMsg->dwPageNum * MAX_SEARCH_RESULT;
	int iMaxCount = pMyChar->m_vecSearchResult.size() - (pNetMsg->dwPageNum * MAX_SEARCH_RESULT);
	if( iMaxCount > MAX_SEARCH_RESULT ) iMaxCount = MAX_SEARCH_RESULT;

	for( i = 0; i < iMaxCount; i++ )
	{
		NetResultMsg.sSearchResult[i] = pMyChar->m_vecSearchResult[resultNum];
		resultNum++;
	}

	NetResultMsg.dwSearchNum = pMyChar->m_vecSearchResult.size();
	NetResultMsg.dwPageNum   = pNetMsg->dwPageNum;

	SENDTOCLIENT ( dwClientID, &NetResultMsg );

	return TRUE;
}


// ???? ?????? ?????? ???? ?????? ????????. ( ??/?????? ????????. ) 
BOOL GLGaeaServer::SaveClubBattle()
{
	EnterCriticalSection(&m_CSPCLock);

	CLUBS_ITER pos = m_cClubMan.m_mapCLUB.begin();
	CLUBS_ITER end = m_cClubMan.m_mapCLUB.end();	

	for ( ; pos!=end; ++pos )
	{
		GLCLUB &cCLUB = (*pos).second;

		if ( cCLUB.GetBattleNum() > 0 )
		{
			CLUB_BATTLE_ITER pos = cCLUB.m_mapBattle.begin();
			CLUB_BATTLE_ITER end = cCLUB.m_mapBattle.end();
			for ( ; pos!=end; ++pos )
			{
				GLCLUBBATTLE &sClubBattle = (*pos).second;

				//	???????????? ????
				if ( sClubBattle.m_bAlliance && cCLUB.m_dwID != cCLUB.m_dwAlliance )
					continue;

				//	db?? ???? ( A????, B???? ???? )
				CSaveClubBattle *pDbAction = new CSaveClubBattle ( cCLUB.m_dwID, 
																   sClubBattle.m_dwCLUBID, 
																   sClubBattle.m_wKillPointDB + sClubBattle.m_wKillPointTemp, 
																   sClubBattle.m_wDeathPointDB + sClubBattle.m_wDeathPointTemp );
				m_pDBMan->AddJob ( pDbAction );
			}
		}
	}

	LeaveCriticalSection(&m_CSPCLock);

	return TRUE;
}

void GLGaeaServer::ClearReserveDropOutSummon ()
{
	if( m_reqDropOutSummon.size() == 0 ) return;

	VSUMMONID_ITER iter = m_reqDropOutSummon.begin();
	for ( ; iter != m_reqDropOutSummon.end(); ++iter )
	{
		SDROPOUTSUMMONINFO sDropoutSummonInfo= *(iter);
		DropOutSummon ( sDropoutSummonInfo.dwSummonGuid, sDropoutSummonInfo.bLeaveFieldServer  );
	}

	m_reqDropOutSummon.clear();
}

PGLSUMMONFIELD GLGaeaServer::NEW_SUMMON ()
{

	EnterCriticalSection(&m_CSPCLock);

	GLSummonField* returnSummon = m_poolSummon.New();

	LeaveCriticalSection(&m_CSPCLock);

	return returnSummon;
}

void GLGaeaServer::RELEASE_SUMMON ( PGLSUMMONFIELD pSummon )
{
	GASSERT ( pSummon && "GLGaeaServer::RELEASE_SUMMON()" );
	if ( !pSummon ) return;

	EnterCriticalSection(&m_CSPCLock);

	pSummon->CleanUp ();
	m_poolSummon.ReleaseNonInit ( pSummon );

	LeaveCriticalSection(&m_CSPCLock);
}


BOOL GLGaeaServer::RequestSummon ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNETPC_REQ_USE_SUMMON* pNetMsg )
{
	if ( IsReserveServerStop () ) return FALSE;

	GLMSG::SNETPC_REQ_USE_SUMMON_FB NetMsgFB;

	PGLCHAR pOwner = GetChar ( dwGaeaID );
	if ( !pOwner )
	{
		NetMsgFB.emFB = EMUSE_SUMMON_FB_FAIL;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	// ?????????????? ???? ( ???? ???? ?????? ???? )
	GLLandMan* pLandMan = GetByMapID ( pOwner->m_sMapID );
	if ( !pLandMan )					return FALSE;
	if ( !pLandMan->IsPetActivity () )
	{
		NetMsgFB.emFB = EMUSE_SUMMON_FB_FAIL_INVALIDZONE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	if ( !pLandMan->IsCardActivity () )
	{
		NetMsgFB.emFB = EMUSE_SUMMON_FB_FAIL_INVALIDZONE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}
	if ( pOwner->m_sCONFTING.IsCONFRONTING() )
	{
		NetMsgFB.emFB = EMUSE_SUMMON_FB_FAIL_CONFRONT;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	if ( pOwner->m_bVehicle )
	{
		NetMsgFB.emFB = EMUSE_SUMMON_FB_FAIL_VEHICLE;
		SENDTOCLIENT ( dwClientID, &NetMsgFB );
		return FALSE;
	}

	// ???????? ?????? ??????????
	PGLSUMMONFIELD pMySummon = GetSummon ( pOwner->m_dwSummonGUID );
	// if ( pMySummon && pMySummon->IsValid () ) return FALSE;

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pNetMsg->sSummonID );
	if ( !pCrowData )					   return E_FAIL;



	{
		PGLSUMMON pSummon = GLCONST_SUMMON::GetSummonData ( pNetMsg->emTYPE );
		if ( !pSummon )
		{
			NetMsgFB.emFB = EMUSE_SUMMON_FB_FAIL_NODATA;
			SENDTOCLIENT ( dwClientID, &NetMsgFB );
			return FALSE;
		}

		if ( pMySummon && pMySummon->IsValid () ) 
		{
			DropOutSummon ( pMySummon->m_dwGUID, true );
			return TRUE;
		}


		GLSUMMON NewSummon(*pSummon);
		NewSummon.m_sSummonID	  = pNetMsg->sSummonID;

		// ???? ??????????
		CreateSummon ( &NewSummon, dwGaeaID, 0 );
	}


	return TRUE;
}


void GLGaeaServer::DelPlayHostileClubBattle( DWORD dwClub_P, DWORD dwClub_S )
{
	GLCLUB *pCLUB = m_cClubMan.GetClub ( dwClub_P );
	if ( !pCLUB )	return;

	CLUBMEMBERS_ITER pos = pCLUB->m_mapMembers.begin();
	CLUBMEMBERS_ITER end = pCLUB->m_mapMembers.end();
	
	PGLCHAR pCHAR = NULL;
	for ( ; pos!=end; ++pos )
	{
		pCHAR = GetCharID ( (*pos).first );
		if ( !pCHAR )	
		{
			continue;
		}
		else
		{
			pCHAR->DelPlayHostileClub( dwClub_S );
		}
	}
	return;
}

void GLGaeaServer::DelPlayHostileAllianceBattle( DWORD dwClub_P, DWORD dwClub_S )
{
	GLCLUB *pCLUB_P = m_cClubMan.GetClub ( dwClub_P );
	if ( !pCLUB_P )	return;

	GLCLUB *pCLUB_S = m_cClubMan.GetClub ( dwClub_S );
	if ( !pCLUB_S )	return;

	// ???? ????
	CLUB_ALLIANCE_ITER pos = pCLUB_P->m_setAlliance.begin();
	CLUB_ALLIANCE_ITER end = pCLUB_P->m_setAlliance.end();
	for ( ; pos!=end; ++pos )
	{
		const GLCLUBALLIANCE &sALLIANCE_A = *pos;
		GLCLUB *pCLUB_A = GetClubMan().GetClub ( sALLIANCE_A.m_dwID );
		if ( !pCLUB_A ) continue;	

		CLUB_ALLIANCE_ITER posTar = pCLUB_S->m_setAlliance.begin();
		CLUB_ALLIANCE_ITER endTar = pCLUB_S->m_setAlliance.end();
		for ( ; posTar!=endTar; ++posTar )
		{
			const GLCLUBALLIANCE &sALLIANCE_B = *pos;
			GLCLUB *pCLUB_B = GetClubMan().GetClub ( sALLIANCE_B.m_dwID );
			if ( !pCLUB_B ) continue;	
			
			DelPlayHostileClubBattle ( pCLUB_A->m_dwID, pCLUB_B->m_dwID );
		}
	}	
}


PGLSUMMONFIELD GLGaeaServer::CreateSummon ( PGLSUMMON pSummonData, DWORD dwOwner, DWORD dwSummonID, bool bValid )
{
	// DB???? ???????? ???????? ????.

	GLMSG::SNETPC_REQ_USE_SUMMON_FB NetMsg;

	if ( !pSummonData ) return NULL;

	// ???? ???????? ?????? ????
	PGLCHAR pOwner = GetChar ( dwOwner );
	if ( !pOwner ) return NULL;

	
	GLLandMan* pLandMan = GetByMapID ( pOwner->m_sMapID );
	if ( !pLandMan ) 
	{
		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
		return NULL;
	}

	PGLSUMMONFIELD pSummon(NULL);

	// ???? ?????????? ???????? ???????? GLSummonField ?? ????????.
	// ???????? ???????????? ??????????.
	pSummon = GetSummon ( pOwner->m_dwSummonGUID );
	if ( pSummon && m_bEmulator )
	{
		// ???????? ???????? ?? ????
		DropOutSummon ( pSummon->m_dwGUID, true );
		pSummon = NULL;
	}

	if ( pSummon )
	{
		HRESULT hr = pSummon->SetPosition ( pLandMan );
		if ( FAILED ( hr ) )
		{
			// ???????? ???????? ?? ????
			DropOutSummon ( pSummon->m_dwGUID, true );
			SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
			return NULL;
		}
	}
	// ?????? ??????????.
	else
	{
		// ??????
		pSummon = NEW_SUMMON ();
		HRESULT hr = pSummon->Create ( pLandMan, pOwner, pSummonData );
		if ( FAILED ( hr ) )
		{
			// ???????? ????
			RELEASE_SUMMON ( pSummon );
			SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
			return NULL;
		}

		DWORD dwGUID = -1;
		if ( !m_FreeSummonGIDs.GetHead ( dwGUID ) )		return NULL;
		m_FreeSummonGIDs.DelHead ();
		pSummon->m_dwGUID = dwGUID;

		// ????ID ????
		pOwner->m_dwSummonGUID = pSummon->m_dwGUID;
		pOwner->m_dwSummonID   = dwSummonID;

		// ???? ????ID(DB??????) ????
		pSummon->SetSummonID ( dwSummonID );
	}

	if ( !DropSummon ( pSummon, pOwner->m_sMapID ) )
	{
		// ???????? ????
		m_FreeSummonGIDs.AddTail ( pSummon->m_dwGUID );
		RELEASE_SUMMON ( pSummon );
		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );
		return NULL;
	}

	// ???? ?????? ????
	pSummon->m_pOwner = pOwner;

	// ?????? ???? ????
	if ( bValid ) pSummon->SetValid ();
	else		  pSummon->ReSetValid ();

	// ??????????????
	if ( pSummon->IsValid () )
	{
		NetMsg.emFB			= EMUSE_SUMMON_FB_OK;
		NetMsg.emTYPE		= pSummon->m_emTYPE;
		NetMsg.dwGUID		= pSummon->m_dwGUID;
		NetMsg.sSummonID	= pSummon->m_sSummonID;
		NetMsg.dwOwner		= pSummon->m_dwOwner;	
		NetMsg.sMapID		= pSummon->m_sMapID;
		NetMsg.dwCellID		= pSummon->m_dwCellID;
		NetMsg.vPos			= pSummon->m_vPos;
		NetMsg.vDir			= pSummon->m_vDir;
		NetMsg.dwNowHP		= pSummon->m_dwNowHP;
		NetMsg.wNowMP		= pSummon->m_wNowMP;
		NetMsg.sPosionID	= pSummon->m_sPosionID;

		NetMsg.dwSummonID	= pSummon->m_dwSummonID;

		

		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsg );

		// ?????? ????
		GLMSG::SNET_SUMMON_CREATE_ANYSUMMON NetMsgBrd;
		NetMsgBrd.Data = pSummon->ReqNetMsg_Drop ();
		pSummon->m_pOwner->SendMsgViewAround ( ( NET_MSG_GENERIC* )&NetMsgBrd );
	}

	return pSummon;	
}

BOOL GLGaeaServer::DropSummon ( PGLSUMMONFIELD pSummon, SNATIVEID sMapID )
{
	if ( !pSummon ) return FALSE;

	GLLandMan* pLandMan = GetByMapID ( sMapID );
	if ( !pLandMan ) return FALSE;

	// ???????? ????
	m_SummonArray[pSummon->m_dwGUID] = pSummon;

	// ?????? ????
	pSummon->m_pLandMan  = pLandMan;
	pSummon->m_pLandNode = pLandMan->m_GlobSummonList.ADDHEAD ( pSummon );
	pLandMan->RegistSummon ( pSummon );

	return TRUE;
}

// ?? ?????? ?????? ???? ???????? ???? ?????? ??????
// ????????. (?????? ?????? ???????? ???????? ??????)
// ?????? ?????????? ???????? ????.
BOOL GLGaeaServer::DropOutSummon ( DWORD dwGUID, bool bLeaveFieldServer )
{
	if ( dwGUID>=m_dwMaxClient ) 
	{
		CDebugSet::ToLogFile ( "ERROR : dwGUID>=m_dwMaxClient SummonGUID : %d dwMaxClient : %d", dwGUID, m_dwMaxClient );
		return FALSE;
	}
	if ( m_SummonArray[dwGUID] == NULL ) 
	{
		CDebugSet::ToLogFile ( "ERROR : m_SummonArray[dwGUID] == NULL" );
		return FALSE;
	}

	PGLSUMMONFIELD pSummon = m_SummonArray[dwGUID];

	// ???? ????
	bool bValid = pSummon->IsValid ();

	// ?????? ?????????? ?????? ?????? ????????
	if ( !bValid && !bLeaveFieldServer )
	{
		CDebugSet::ToLogFile ( "ERROR : !bValid && !bLeaveFieldServer bValid %d bLeaveFieldServer %d", bValid, bLeaveFieldServer );
		return FALSE;
	}

	DWORD dwSummonGUID = pSummon->m_dwGUID;
	DWORD dwOwnerID	   = pSummon->m_dwOwner;

	// ??????????
	if ( bValid )
	{
		//	Note : Land ?????????? ????.
		GLLandMan* pLandMan = pSummon->m_pLandMan;
		if ( pLandMan )
		{
			pLandMan->RemoveSummon ( pSummon );
			pSummon->m_pLandMan = NULL;
		}

		pSummon->ReSetValid ();
		pSummon->ReSetAllSTATE ();
//		pSummon->ReSetSkillDelay ();
	}

	// ?????????? ?? ???????? ?????? ???? (PC?? ?????? ???????? pOwner ???? ?? ????)
	PGLCHAR pOwner = GetChar ( dwOwnerID );
	if ( pOwner && bValid )
	{
		// ?????? ?????????? ????
		/*pOwner->m_sPETSKILLFACT.RESET ();
		pOwner->m_bProtectPutOnItem = false;*/

		GLMSG::SNETPC_REQ_USE_SUMMON_DEL NetMsgFB;
		NetMsgFB.dwGUID	= dwSummonGUID;
		SENDTOCLIENT ( pOwner->m_dwClientID, &NetMsgFB );
	}

	// ???? ?????????? ???????? ?????? ???? ????????
	if ( bLeaveFieldServer )
	{
		// ?????? ?????????? ????, GUID ????
		RELEASE_SUMMON ( pSummon );
		m_SummonArray[dwGUID] = NULL;
		m_FreeSummonGIDs.AddTail ( dwGUID );
	}

	return TRUE;
}

BOOL GLGaeaServer::SaveNpcCommission( DWORD dwCharID, DWORD dwUserID, LONGLONG lnCommission )
{
	if ( lnCommission <= 0 )	return FALSE;

	GLChar* pChar = GLGaeaServer::GetInstance().GetCharID( dwCharID );

	//  ???????? ???? ?????? ???????? 
	if ( pChar ) 
	{
		pChar->UpdateNpcCommission( lnCommission );
	}
	else
	{
		GLMSG::SNET_INVEN_NPC_COMMISSION NetMsgAgt;	
		NetMsgAgt.nCHANNEL = GLGaeaServer::GetInstance().GetServerChannel();
		NetMsgAgt.dwFieldID = GLGaeaServer::GetInstance().GetFieldSvrID();
		NetMsgAgt.dwCharID = dwCharID;
		NetMsgAgt.dwUserID = dwUserID;
		NetMsgAgt.lnCommission = lnCommission;
		GLGaeaServer::GetInstance().SENDTOAGENT( &NetMsgAgt );
	}

	return TRUE;
}

BOOL GLGaeaServer::SaveNpcCommissionDB( DWORD dwCharID, DWORD dwUserID, LONGLONG lnCommission )
{
	if ( lnCommission <= 0 )	return FALSE;

	GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, dwCharID, ID_USER, dwUserID, lnCommission, EMITEM_ROUTE_NPCCOME );

	CUpdateUserMoneyAdd* pDbAction = new CUpdateUserMoneyAdd ( dwUserID, lnCommission );
	m_pDBMan->AddJob ( pDbAction );

//	UserLastInfo ????????
#if defined ( TW_PARAM ) || defined ( HK_PARAM ) || defined ( _RELEASED )	
	
	CUpdateUserLastInfoAdd* pDbActionLast = new CUpdateUserLastInfoAdd ( dwUserID, lnCommission );
	m_pDBMan->AddJob ( pDbActionLast );

#endif 


	return TRUE;
}

BOOL GLGaeaServer::ReqClubDeathMatchRanking ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_CLUB_DEATHMATCH_RANKING_REQ* pNetMsg )
{
//	GLMSG::SNETPC_REQ_USE_SUMMON_FB NetMsg;

	// ???? ???????? ?????? ????
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return FALSE;	
	
	if ( !pChar->m_pLandMan ) return FALSE;
	if ( pChar->m_dwGuild == CLUB_NULL )	return FALSE;
	if ( pChar->m_pLandMan->GetMapID().dwID != pNetMsg->dwMapID ) return FALSE;
	if ( !pChar->m_pLandMan->m_bClubDeathMatchMap ) return FALSE;

	GLClubDeathMatch* pCDM = GLClubDeathMatchFieldMan::GetInstance().Find( pChar->m_pLandMan->m_dwClubMapID );
	if ( !pCDM ) return FALSE;
	if ( !pCDM->IsBattle() ) return FALSE;

	
	
	CDM_RANK_INFO_MAP_ITER pos = pCDM->m_mapCdmScore.find( pChar->m_dwGuild );
	if( pos != pCDM->m_mapCdmScore.end() ) 
	{
		GLMSG::SNET_CLUB_DEATHMATCH_MYRANK_UPDATE NetMsgMy;

		NetMsgMy.sMyCdmRank.wClubRanking = pos->second.wClubRanking;
		NetMsgMy.sMyCdmRank.wKillNum = pos->second.wKillNum;
		NetMsgMy.sMyCdmRank.wDeathNum = pos->second.wDeathNum;
		
		SENDTOCLIENT( dwClientID, &NetMsgMy );
	}


	GLMSG::SNET_CLUB_DEATHMATCH_RANKING_UPDATE NetMsg;
	
	CDM_RANK_INFO_MAP_ITER pos_begin = pCDM->m_mapCdmScore.begin();
	CDM_RANK_INFO_MAP_ITER pos_end = pCDM->m_mapCdmScore.end();

	for ( ; pos_begin != pos_end; pos_begin++ )
	{
		SCDM_RANK_INFO& sCdmRankInfo = pos_begin->second;
		int nIndex = sCdmRankInfo.nIndex;

		if ( nIndex >= 0 && nIndex < RANKING_NUM )
		{
			SCDM_RANK sCdmRank = sCdmRankInfo;
			NetMsg.ADDCLUB( sCdmRank );
		}
	}	

	if ( NetMsg.wRankNum > 0 )	SENDTOCLIENT( dwClientID, &NetMsg );

	return TRUE;
}

BOOL GLGaeaServer::ReqSchoolWarsRanking ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_SCHOOL_WARS_RANKING_REQ* pNetMsg )
{
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return FALSE;	
	
	if ( !pChar->m_pLandMan ) return FALSE;
	if ( pChar->m_pLandMan->GetMapID().dwID != pNetMsg->dwMapID ) return FALSE;
	if ( !pChar->m_pLandMan->m_bSchoolWarsMap ) return FALSE;

	GLSchoolWars* pSW = GLSchoolWarsFieldMan::GetInstance().Find( pChar->m_pLandMan->m_dwSchoolWarMapID );
	if ( !pSW ) return FALSE;
	if ( !pSW->IsBattle() ) return FALSE;
	
	SW_RANK_INFO_MAP_ITER pos = pSW->m_mapSwScore.find( pChar->m_wSchool );
	if( pos != pSW->m_mapSwScore.end() ) 
	{
		GLMSG::SNET_SCHOOL_WARS_MYRANK_UPDATE NetMsgMy;

		NetMsgMy.sMySwRank.wClubRanking = pos->second.wClubRanking;
		NetMsgMy.sMySwRank.wKillNum = pos->second.wKillNum;
		NetMsgMy.sMySwRank.wDeathNum = pos->second.wDeathNum;
		
		SENDTOCLIENT( dwClientID, &NetMsgMy );
	}

	GLMSG::SNET_SCHOOL_WARS_RANKING_UPDATE NetMsg;
	
	SW_RANK_INFO_MAP_ITER pos_begin = pSW->m_mapSwScore.begin();
	SW_RANK_INFO_MAP_ITER pos_end = pSW->m_mapSwScore.end();

	for ( ; pos_begin != pos_end; pos_begin++ )
	{
		SSW_RANK_INFO& sSwRankInfo = pos_begin->second;
		int nIndex = sSwRankInfo.nIndex;

		if ( nIndex >= 0 && nIndex < SCHOOL_RANK_NUM )
		{
			SSW_RANK sSwRank = sSwRankInfo;
			NetMsg.ADDSCHOOL( sSwRank );
		}
	}	

	if ( NetMsg.wRankNum > 0 )	SENDTOCLIENT( dwClientID, &NetMsg );

	return TRUE;
}

BOOL GLGaeaServer::ReqSchoolWarsPRanking ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_SCHOOL_WARS_PRANKING_REQ* pNetMsg )
{
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return FALSE;	
	
	if ( !pChar->m_pLandMan ) return FALSE;
	if ( pChar->m_pLandMan->GetMapID().dwID != pNetMsg->dwMapID ) return FALSE;
	if ( !pChar->m_pLandMan->m_bSchoolWarsMap ) return FALSE;

	GLSchoolWars* pSW = GLSchoolWarsFieldMan::GetInstance().Find( pChar->m_pLandMan->m_dwSchoolWarMapID );
	if ( !pSW ) return FALSE;
	if ( !pSW->IsBattle() ) return FALSE;

	SWP_RANK_INFO_MAP_ITER pos = pSW->m_mapSwPScore.find( pChar->m_dwCharID );
	if( pos != pSW->m_mapSwPScore.end() ) 
	{
		GLMSG::SNET_SCHOOL_WARS_MYPRANK_UPDATE NetMsgMy;

		NetMsgMy.sMySwPRank.wCharRanking = pos->second.wCharRanking;
		NetMsgMy.sMySwPRank.wKillNum = pos->second.wKillNum;
		NetMsgMy.sMySwPRank.wDeathNum = pos->second.wDeathNum;
		
		SENDTOPLAYERCLIENT_ONMAP( pSW->m_dwID, pChar->m_dwCharID , &NetMsgMy );
	}


	GLMSG::SNET_SCHOOL_WARS_PRANKING_UPDATE NetMsg;
	
	SWP_RANK_INFO_MAP_ITER pos_begin = pSW->m_mapSwPScore.begin();
	SWP_RANK_INFO_MAP_ITER pos_end = pSW->m_mapSwPScore.end();

	for ( ; pos_begin != pos_end; pos_begin++ )
	{
		SSWP_RANK_INFO& sSwPRankInfo = pos_begin->second;
		int nIndex = sSwPRankInfo.nIndex;

		if ( nIndex >= 0 && nIndex < RANKING_NUM )
		{
			SSWP_RANK sSwPRank = sSwPRankInfo;
			NetMsg.ADDCHAR( sSwPRank );
		}
	}	

	if ( NetMsg.wRankNum > 0 )	SENDTOCLIENT( dwClientID, &NetMsg );

	return TRUE;
}
BOOL GLGaeaServer::ReqRoyalRumblePRanking ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_ROYAL_RUMBLE_PRANKING_REQ* pNetMsg )
{
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return FALSE;	
	
	if ( !pChar->m_pLandMan ) return FALSE;
	if ( pChar->m_pLandMan->GetMapID().dwID != pNetMsg->dwMapID ) return FALSE;
	if ( !pChar->m_pLandMan->m_bRoyalRumbleMap ) return FALSE;

	GLRoyalRumble* pRR = GLRoyalRumbleFieldMan::GetInstance().Find( pChar->m_pLandMan->m_dwRoyalRumbleMapID );
	if ( !pRR ) return FALSE;
	if ( !pRR->IsBattle() ) return FALSE;

	RRP_RANK_INFO_MAP_ITER pos = pRR->m_mapRrPScore.find( pChar->m_dwCharID );
	if( pos != pRR->m_mapRrPScore.end() ) 
	{
		GLMSG::SNET_ROYAL_RUMBLE_MYPRANK_UPDATE NetMsgMy;

		NetMsgMy.sMyRrPRank.wCharRanking = pos->second.wCharRanking;
		NetMsgMy.sMyRrPRank.wKillNum = pos->second.wKillNum;
		NetMsgMy.sMyRrPRank.wDeathNum = pos->second.wDeathNum;
		
		SENDTOPLAYERCLIENT_ONMAP( pRR->m_dwID, pChar->m_dwCharID , &NetMsgMy );
	}


	GLMSG::SNET_ROYAL_RUMBLE_PRANKING_UPDATE NetMsg;
	
	RRP_RANK_INFO_MAP_ITER pos_begin = pRR->m_mapRrPScore.begin();
	RRP_RANK_INFO_MAP_ITER pos_end = pRR->m_mapRrPScore.end();

	for ( ; pos_begin != pos_end; pos_begin++ )
	{
		SRRP_RANK_INFO& sRrPRankInfo = pos_begin->second;
		int nIndex = sRrPRankInfo.nIndex;

		if ( nIndex >= 0 && nIndex < RANKING_NUM )
		{
			SRRP_RANK sRrPRank = sRrPRankInfo;
			NetMsg.RRADDCHAR( sRrPRank );
		}
	}	

	if ( NetMsg.wRankNum > 0 )	SENDTOCLIENT( dwClientID, &NetMsg );

	return TRUE;
}
BOOL GLGaeaServer::ReqTowerWarsCTFRanking ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_TOWERWARS_CTFRANKING_REQ* pNetMsg )
{
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return FALSE;	
	
	if ( !pChar->m_pLandMan ) return FALSE;
	if ( pChar->m_pLandMan->GetMapID().dwID != pNetMsg->dwMapID ) return FALSE;
	if ( !pChar->m_pLandMan->m_bTowerWarsMap ) return FALSE;

	GLTowerWars* pSW = GLTowerWarsFieldMan::GetInstance().Find( pChar->m_pLandMan->m_dwTowerWarsMapID );
	if ( !pSW ) return FALSE;
	if ( !pSW->IsBattle() ) return FALSE;

	GLTowerWarsFieldMan::GetInstance().UpdateCTFRanking( pSW->m_dwID, pChar->m_dwCharID );

	TWP_RANK_INFO_MAP_ITER pos = pSW->m_mapCtfScore.find( pChar->m_dwCharID );
	if( pos != pSW->m_mapCtfScore.end() ) 
	{
		GLMSG::SNET_TOWERWARS_MYCTFRANK_UPDATE NetMsgMy;

		NetMsgMy.sMySwPRank.wCharRanking = pos->second.wCharRanking;
		NetMsgMy.sMySwPRank.wKillNum = pos->second.wKillNum;
		NetMsgMy.sMySwPRank.wDeathNum = pos->second.wDeathNum;
		NetMsgMy.sMySwPRank.dwDamageNum = pos->second.dwDamageNum;
		NetMsgMy.sMySwPRank.dwHealNum = pos->second.dwHealNum;
		NetMsgMy.sMySwPRank.wResuNum = pos->second.wResuNum;
		NetMsgMy.sMySwPRank.dwClass = pos->second.dwClass;
		NetMsgMy.sMySwPRank.wSchool = pos->second.wSchool;
		StringCchCopy ( NetMsgMy.sMySwPRank.szCharName, CHAR_NAME+1, pos->second.szCharName );
		
		SENDTOPLAYERCLIENT_ONMAP( pSW->m_dwClubMap, pChar->m_dwCharID , &NetMsgMy );
	}


	GLMSG::SNET_TOWERWARS_CTFRANKING_UPDATE NetMsg;
	
	TWP_RANK_INFO_MAP_ITER pos_begin = pSW->m_mapCtfScore.begin();
	TWP_RANK_INFO_MAP_ITER pos_end = pSW->m_mapCtfScore.end();

	for ( ; pos_begin != pos_end; pos_begin++ )
	{
		STWP_RANK_INFO& sSwPRankInfo = pos_begin->second;
		int nIndex = sSwPRankInfo.nIndex;

		if ( nIndex >= 0 && nIndex < RANKING_PNUM )
		{
			STWP_RANK sSwPRank = sSwPRankInfo;
			NetMsg.ADDCHAR( sSwPRank );
		}
	}	

	if ( NetMsg.wRankNum > 0 )	SENDTOCLIENT( dwClientID, &NetMsg );

	return TRUE;
}

BOOL GLGaeaServer::ReqCtfIconID ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_TOWERWARS_CTFICON_REQ* pNetMsg )
{
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return FALSE;	

	if ( !pChar->m_pLandMan ) return FALSE;
	if ( pChar->m_pLandMan->GetMapID().dwID != pNetMsg->dwMapID ) return FALSE;
	if ( !pChar->m_pLandMan->m_bTowerWarsMap ) return FALSE;

	GLTowerWars* pSW = GLTowerWarsFieldMan::GetInstance().Find( pChar->m_pLandMan->m_dwTowerWarsMapID );
	if ( !pSW ) return FALSE;

	//if ( pSW->IsBattle() || pSW->IsBattleEndAward() )
	{
		//send tower status to client
		SCHOOLWAR_TOWER_STATUS_MAP_ITER	pos = pSW->m_mapTowerStatus.begin();
		SCHOOLWAR_TOWER_STATUS_MAP_ITER	end = pSW->m_mapTowerStatus.end();
		GLMSG::SNET_TOWERWARS_TOWER_UPDATE	NetMsg;
		for ( ; pos != end; pos++ )
		{
			const SSCHOOLWAR_TOWER_STATUS&	sTOWER = pos->second;
			NetMsg.ADDTOWER( sTOWER );
		}

		if ( NetMsg.wNum > 0 )	
			SENDTOCLIENT( dwClientID, &NetMsg );
	}
	
	return TRUE;
}

BOOL GLGaeaServer::ReqTWRecordPreviousResult ( DWORD dwClientID, DWORD dwGaeaID, GLMSG::SNET_TOWERWARS_PREVIOUS_RECORD* pNetMsg )
{
	PGLCHAR pChar = GetChar ( dwGaeaID );
	if ( !pChar ) return FALSE;	

	m_pTWRecord.fElapsedTime = pNetMsg->pTWRecord.fElapsedTime;
	m_pTWRecord.wSchool = pNetMsg->pTWRecord.wSchool;
	
	return TRUE;
}