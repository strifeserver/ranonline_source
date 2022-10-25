#include "StdAfx.h"
#include "CharacterWindowPetRender.h"

#include "../DxCommon/DxBackUpRendTarget.h"
#include "GLGaeaClient.h"
#include "DxGlobalStage.h"
#include "DxEffcharData.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterWindowPetRender::CCharacterWindowPetRender () 
	: m_pSkinChar( NULL )
	, m_wLastCOLOR(0)
	, m_wLastSTYLE(0)
	, m_fDelay(0.0f)
	, m_vRot( 0.0f,0.0f,0.0f )
	, m_emPETTYPE( PETTYPE_NONE )
{
	D3DXMatrixIdentity ( &m_matTrans );

	for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
	{
		m_PutOnItems[i] = SITEMCUSTOM();
	}
}

CCharacterWindowPetRender::~CCharacterWindowPetRender ()
{
}

void CCharacterWindowPetRender::CreateSubControl ()
{
}

void CCharacterWindowPetRender::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

HRESULT CCharacterWindowPetRender::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;

	if ( m_pSkinChar )
		m_pSkinChar->RestoreDeviceObjects( pd3dDevice );

	hr = CUIGroup::RestoreDeviceObjects( pd3dDevice );
	return hr;
}

HRESULT CCharacterWindowPetRender::DeleteDeviceObjects ()
{
	HRESULT hr = S_OK;

	if ( m_pSkinChar )
		m_pSkinChar->DeleteDeviceObjects ();

	hr = CUIGroup::DeleteDeviceObjects ();

	return hr;
}

HRESULT CCharacterWindowPetRender::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;

	if ( !m_pSkinChar )
	{
		CreateModel( pd3dDevice );
	}else{

		if ( m_fDelay < 0.2 ) return S_OK;

		DWORD dwAlphaBlendEnable, dwSrcBlend, dwDestBlend, dwZWriteEnable, dwZEnable, dwZFunc, dwClipPlaneEnable, dwFogEnable, dwAmbient, dwLighting;
		DWORD dwColorOP, dwAlphaOP, dwMinFilter, dwMagFilter, dwMipFilter;
		D3DXMATRIX		matOrgView, matOrgProj, matOrgWorld;
		D3DVIEWPORTQ OrgViewPort;

		pd3dDevice->GetViewport(&OrgViewPort);

		pd3dDevice->GetTransform ( D3DTS_VIEW, &matOrgView );
		pd3dDevice->GetTransform ( D3DTS_PROJECTION, &matOrgProj );
		pd3dDevice->GetTransform ( D3DTS_WORLD, &matOrgWorld );

		DxBackUpRendTarget sBackupTarget ( pd3dDevice );

		pd3dDevice->GetRenderState( D3DRS_ALPHABLENDENABLE,	&dwAlphaBlendEnable );
		pd3dDevice->GetRenderState( D3DRS_SRCBLEND,			&dwSrcBlend );
		pd3dDevice->GetRenderState( D3DRS_DESTBLEND,		&dwDestBlend );
		pd3dDevice->GetRenderState( D3DRS_ZWRITEENABLE,		&dwZWriteEnable );
		pd3dDevice->GetRenderState( D3DRS_ZENABLE,			&dwZEnable );
		pd3dDevice->GetRenderState( D3DRS_ZFUNC,			&dwZFunc);
		pd3dDevice->GetRenderState( D3DRS_CLIPPLANEENABLE,	&dwClipPlaneEnable );
		pd3dDevice->GetRenderState( D3DRS_FOGENABLE,		&dwFogEnable );
		pd3dDevice->GetRenderState( D3DRS_AMBIENT,			&dwAmbient );
		pd3dDevice->GetRenderState( D3DRS_LIGHTING,			&dwLighting );

		pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,	&dwColorOP );
		pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,	&dwAlphaOP );

		pd3dDevice->GetSamplerState( 0, D3DSAMP_MINFILTER,	&dwMinFilter );
		pd3dDevice->GetSamplerState( 0, D3DSAMP_MAGFILTER,	&dwMagFilter );
		pd3dDevice->GetSamplerState( 0, D3DSAMP_MIPFILTER,	&dwMipFilter );
		
		pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;

		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );
		pd3dDevice->SetRenderState( D3DRS_ZENABLE,			D3DZB_TRUE );
		pd3dDevice->SetRenderState( D3DRS_ZFUNC,			D3DCMP_LESSEQUAL);
		pd3dDevice->SetRenderState( D3DRS_AMBIENT,			D3DCOLOR_XRGB(220,220,220) );
		pd3dDevice->SetRenderState( D3DRS_LIGHTING,			FALSE );

		D3DXMATRIX		matView, matProj, matWorld;

		D3DVIEWPORTQ ViewPort;
		ViewPort.X      = (DWORD)GetGlobalPos().left;
		ViewPort.Y      = (DWORD)GetGlobalPos().top;
		ViewPort.Width  = (DWORD)GetGlobalPos().sizeX;
		ViewPort.Height = (DWORD)GetGlobalPos().sizeY;
		ViewPort.MinZ = 0.0f;
		ViewPort.MaxZ = 1.0f;
		pd3dDevice->SetViewport(&ViewPort);

		D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 0.0f, 0.3f, -35.0f );
		D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( -0.02f, 0.0f, 5.0f );
		D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		D3DXMatrixLookAtLH ( &matView, &vFromPt, &vLookatPt, &vUpVec );
		pd3dDevice->SetTransform ( D3DTS_VIEW, &matView );

		float fFieldOfView = D3DX_PI/4.0f;
		float fAspectRatio = ((float)ViewPort.Width) / (float)ViewPort.Height;
		D3DXMatrixPerspectiveFovLH ( &matProj, fFieldOfView, fAspectRatio, 1.0f, 80.0f );
		pd3dDevice->SetTransform ( D3DTS_PROJECTION, &matProj );
		
		D3DXMatrixIdentity ( &matWorld );
		pd3dDevice->SetTransform ( D3DTS_WORLD, &matWorld );

		RenderModel( pd3dDevice );

		sBackupTarget.RestoreTarget ( pd3dDevice );

		pd3dDevice->SetTransform ( D3DTS_VIEW, &matOrgView );
		pd3dDevice->SetTransform ( D3DTS_PROJECTION, &matOrgProj );
		pd3dDevice->SetTransform ( D3DTS_WORLD, &matOrgWorld );

		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	dwAlphaBlendEnable );
		pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			dwSrcBlend );
		pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		dwDestBlend );
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		dwZWriteEnable );
		pd3dDevice->SetRenderState( D3DRS_ZENABLE,			dwZEnable );
		pd3dDevice->SetRenderState( D3DRS_ZFUNC,			dwZFunc);
		pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,	dwClipPlaneEnable );
		pd3dDevice->SetRenderState( D3DRS_FOGENABLE,		dwFogEnable );
		pd3dDevice->SetRenderState( D3DRS_AMBIENT,			dwAmbient );
		pd3dDevice->SetRenderState( D3DRS_LIGHTING,			dwLighting );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	dwColorOP );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	dwAlphaOP );

		pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	dwMinFilter );
		pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	dwMagFilter );
		pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	dwMipFilter );

		pd3dDevice->SetViewport(&OrgViewPort);	
	}

	hr = CUIGroup::Render ( pd3dDevice );
	return hr;
}

HRESULT CCharacterWindowPetRender::FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;

	m_fDelay += fElapsedTime;

	if ( m_pSkinChar )
	{
		m_pSkinChar->FrameMove ( fTime, fElapsedTime, TRUE, FALSE, FALSE );
	}

	GLPetClient* pCLIENT = GLGaeaClient::GetInstance().GetPetClient();
	if ( pCLIENT && pCLIENT->IsVALID() )
	{
		WORD wCOLOR = pCLIENT->m_wColor;
		WORD wSTYLE = pCLIENT->m_wStyle;

		BOOL bDIFCOLOR = wCOLOR != m_wLastCOLOR;
		BOOL bDIFSTYLE = wSTYLE != m_wLastSTYLE;
		BOOL bDIFITEM = FALSE;

		for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
		{
			if ( m_PutOnItems[i] != pCLIENT->m_PutOnItems[i] )
				bDIFITEM = TRUE;
		}

		if ( bDIFCOLOR || bDIFSTYLE || bDIFITEM )
		{
			UpdateSuit( pd3dDevice );
		}

		m_emPETTYPE = pCLIENT->m_emTYPE;
	}

	hr = CUIGroup::FrameMove( pd3dDevice, fTime, fElapsedTime );

	return hr;
}

void CCharacterWindowPetRender::CreateModel( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GLPetClient* pCLIENT = GLGaeaClient::GetInstance().GetPetClient();
	if ( !pCLIENT )
		return;

	if ( !pCLIENT ->IsVALID() )
		return;

	SNATIVEID loadPetID = pCLIENT->m_sPetID;

	if( pCLIENT->IsUsePetSkinPack() )
	{
		loadPetID = pCLIENT->m_sPetSkinPackState.petSkinMobID;
	}

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( loadPetID );
	if ( !pCrowData )
	{
		GASSERT ( pCrowData );
		return ;
	}

	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().LoadData( pCrowData->GetSkinObjFile(),pd3dDevice, TRUE );

	if ( !pSkinChar )	return;

	SAFE_DELETE(m_pSkinChar);
	m_pSkinChar = new DxSkinChar;
	m_pSkinChar->InitDeviceObjects( pd3dDevice );
	m_pSkinChar->SetCharData ( pSkinChar, pd3dDevice );
	m_pSkinChar->SELECTANI( AN_GUARD_N, AN_SUB_NONE );
	UpdateSuit( pd3dDevice );

	m_vRot = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	m_fDelay = 0.0f;
}

void CCharacterWindowPetRender::RenderModel( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( m_pSkinChar )
	{
		CalculatePetMatrix();
		m_pSkinChar->Render( pd3dDevice, m_matTrans/*, FALSE, FALSE, FALSE */);
	}
}

void CCharacterWindowPetRender::UpdateSuit( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GLPetClient* pCLIENT = GLGaeaClient::GetInstance().GetPetClient();
	if ( !pCLIENT )
		return;

	if ( !pCLIENT->IsVALID() )
		return;

	if ( !m_pSkinChar )	return;

	if( pCLIENT->IsUsePetSkinPack() ) return;

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pCLIENT->m_sPetID );
	if ( !pCrowData ) return;

	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().LoadData( pCrowData->GetSkinObjFile(), pd3dDevice, TRUE );

	if ( !pSkinChar ) return;

	const PETSTYLE &sSTYLE = GLCONST_PET::GetPetStyle ( pCLIENT->m_emTYPE );

	{
		if ( sSTYLE.wSTYLENum > pCLIENT->m_wStyle )
		{
			std::string strSTYLE_CPS = sSTYLE.strSTYLE_CPS[pCLIENT->m_wStyle];

			PDXCHARPART pCharPart = NULL;
			pCharPart = m_pSkinChar->GetPiece(PIECE_HAIR);

			if ( pCharPart && strcmp(strSTYLE_CPS.c_str(),pCharPart->m_szFileName) )
			{
				m_pSkinChar->SetPiece ( strSTYLE_CPS.c_str(), pd3dDevice );
			}
		}
	}

	if ( !pCLIENT->m_bEnableColor ) m_pSkinChar->SetHairColor( pCLIENT->m_wColor );

	m_wLastCOLOR = pCLIENT->m_wColor;
	m_wLastSTYLE = pCLIENT->m_wStyle;

	for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
	{
		m_PutOnItems[i] = pCLIENT->m_PutOnItems[i];

		SITEM* pSlotItem = GLItemMan::GetInstance().GetItem ( pCLIENT->m_PutOnItems[i].sNativeID );
		if ( pSlotItem ) m_pSkinChar->SetPiece ( pSlotItem->GetPetWearingFile (), pd3dDevice, FG_MUSTNEWLOAD );
		else
		{
			PDXSKINPIECE pSkinPiece = NULL;
			PDXCHARPART pCharPart = NULL;

			EMPIECECHAR emPiece = pCLIENT->GetPieceFromSlot ( (PET_ACCESSORYTYPE)i );
			if ( emPiece != PIECE_SIZE ) 
			{
				pSkinPiece = pSkinChar->GetPiece(emPiece);
				pCharPart = m_pSkinChar->GetPiece(emPiece);

				if ( pSkinPiece )
				{
					if ( pCharPart && strcmp(pSkinPiece->m_szFileName,pCharPart->m_szFileName) )
					{
						m_pSkinChar->SetPiece ( pSkinPiece->m_szFileName, pd3dDevice, 0X0, 0, TRUE );
					}
				}else{
					m_pSkinChar->ResetPiece ( emPiece );
				}
			}
		}
	}
}

void CCharacterWindowPetRender::ClearRender()
{
	if ( m_pSkinChar )
		SAFE_DELETE ( m_pSkinChar );

	m_fDelay = 0.0f;
	m_wLastCOLOR = 0;
	m_wLastSTYLE = 0;
	m_emPETTYPE = PETTYPE_NONE;

	for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
	{
		m_PutOnItems[i] = SITEMCUSTOM();
	}
}

void CCharacterWindowPetRender::CalculatePetMatrix()
{
	float fScale = 2.0f;
	float fTransY = -11.0f;
	

	switch( m_emPETTYPE )
	{
		//case	PETTYPE_A: // fairy
		//case	PETTYPE_B:

		case	PETTYPE_C: //white tiger
		case	PETTYPE_D: //bear
			{
				fScale = 2.4f;
			}break;

		case	PETTYPE_E: //owl
		case	PETTYPE_F: //parrot
			{
				fScale = 2.2f;
				fTransY = -45.0f;
			}break;
        case	PETTYPE_G: //miku
	    case	PETTYPE_H: //monkey
		case	PETTYPE_L: //dinasour
		{
				fScale = 1.5f;
				//fTransY = -45.0f;
			}break;
		case	PETTYPE_J: //fairy
		case	PETTYPE_K: //fox
			{
				fTransY = -18.0f;
			}break;

		case	PETTYPE_I: //leopard
			{
				fScale = 1.5f;
				fTransY = -9.0f;
			}break;
	};

	D3DXMATRIX matTrans, matScale, matRot;

	D3DXMatrixScaling( &matScale, fScale, fScale, fScale );
	D3DXMatrixTranslation( &matTrans, 0.0f, fTransY, 0.0f );
	D3DXMatrixRotationYawPitchRoll ( &matRot, m_vRot.x, m_vRot.y, m_vRot.z );

	m_matTrans = matScale*matRot*matTrans;
}