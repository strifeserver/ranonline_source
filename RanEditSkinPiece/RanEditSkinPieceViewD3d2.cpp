#include "stdafx.h"
#include "RanEditSkinPieceView.h"
#include "RanEditSkinPiece.h"

#include "GLOGIC.h"
#include "DxEffectMan.h"
#include "RANPARAM.h"
#include "DxResponseMan.h"
#include "DxViewPort.h"
#include "DxGlowMan.h"
#include "DxShadowMap.h"
#include "DxPostProcess.h"
#include "DxInputDevice.h"
#include "profile.h"
#include "dxparamset.h"
#include "SUBPATH.h"
#include "GLChar.h"
#include "DxServerInstance.h"
#include "DxGlobalStage.h"
#include "RanEditSkinPieceDoc.h"
#include "D3DFont.h"
#include "../EngineUILib/GUInterface/Cursor.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../RanClientUiLib/Interface/GameTextControl.h"
#include "DxSkinMeshMan.h"
#include "DxDynamicVB.h"
#include "DxCubeMap.h"
#include "MainFrm.h"
#include "DxEnvironment.h"
#include "../EngineLib/DxEffect/EffProj/DxEffProj.h"
#include "../EngineLib/DxEffect/Single/DxEffGroupPlayer.h"
#include "../../EngineSoundLib/DxSound/BgmSound.h"
#include "../../EngineSoundLib/DxSound/DxSoundMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern int g_nITEMLEVEL;
extern BOOL g_bCHAR_EDIT_RUN;

HRESULT CRanEditSkinPieceView::FrameMove3DEnvironment ()
{
	HRESULT hr=S_OK;

	PROFILE_BLOCKSTART();

	if ( m_pd3dDevice )
	{
		hr = CD3DApplication::FrameMove3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CRanEditSkinPieceView::Render3DEnvironment ()
{
	HRESULT hr=S_OK;

	PROFILE_BLOCKSTART();

	if ( m_pd3dDevice )
	{
		hr = CD3DApplication::Render3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CRanEditSkinPieceView::ConfirmDevice ( D3DCAPSQ* pCaps, DWORD dwBehavior, D3DFORMAT Format )
{
	if ( dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
		return E_FAIL;

	return S_OK;
}

HRESULT CRanEditSkinPieceView::CreateObjects()
{
	DxFontMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );
	CD3DFontPar* pD3dFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	CDebugSet::InitDeviceObjects( pD3dFont9 );

	HRESULT hr = S_OK;

	hr = InitDeviceObjects();
    if( FAILED(hr) )	
	{
		CDebugSet::MsgBox( "CRanEditSkinPieceView::CreateObjects()" );
		return hr;
	}

	hr = RestoreDeviceObjects();	
    if( FAILED(hr) )	
	{
		CDebugSet::MsgBox( "CRanEditSkinPieceView::CreateObjects()" );
		return hr;
	}

	return hr;
}

HRESULT CRanEditSkinPieceView::InitDeviceObjects()
{
	HRESULT hr = S_OK;

	DxViewPort::GetInstance().InitDeviceObjects ( m_pd3dDevice, m_hWnd );
	DxResponseMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );
	GLogicData::GetInstance().InitDeviceObjects ( m_pd3dDevice );

	DXLIGHT sDirectional;
	sDirectional.SetDefault();
	sDirectional.m_Light.Diffuse = D3DXCOLOR(1.1f,1.1f,1.1f,1);
	sDirectional.m_Light.Ambient = D3DXCOLOR(1.1f,1.1f,1.1f,1);
	DxLightMan::SetDefDirect ( sDirectional );

	hr = m_pd3dDevice->GetDeviceCaps ( &m_d3dCaps );
	DxBoneCollector::GetInstance().PreLoad ( _PRELOAD_BONE, m_pd3dDevice );
	DxSkinAniMan::GetInstance().PreLoad ( _PRELOAD_ANI, m_pd3dDevice );
	DxSkinMeshMan::GetInstance().PreLoad( _PRELOAD_SKIN, m_pd3dDevice, FALSE );

	WORD wWidth = 1024; 
	WORD wHeight = 768;

	LPDIRECT3DSURFACEQ pBackBuffer=NULL;
	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

	if ( pBackBuffer )
	{
		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();

		wWidth = static_cast<WORD> ( m_d3dsdBackBuffer.Width );
		wHeight = static_cast<WORD> ( m_d3dsdBackBuffer.Height );
	}

	DxViewPort::GetInstance().SetCameraType ((CAMERA_TYPE)3);

	D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 0.0f, 70.0f, -70.0f );
	D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );

	//DxViewPort::GetInstance().SetMoveVelocity( 300 );

	DxViewPort::GetInstance().CameraJump ( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	if ( m_pPiece )
	{
		m_pPiece->InitDeviceObjects( m_pd3dDevice );
	}

	CCursor::GetInstance().InitDeviceObjects ();
	DxResponseMan::GetInstance().SetRenderState ();
	DxFogMan::GetInstance().SetFogRange ( EMFR_HIGH );

	m_DxEditMRS.OnResetDevice( m_pd3dDevice );

	return S_OK;
}

HRESULT CRanEditSkinPieceView::InvalidateDeviceObjects()
{
	DxResponseMan::GetInstance().InvalidateDeviceObjects ();

	if ( m_pPiece )
	{
		m_pPiece->InvalidateDeviceObjects();
	}

	m_DxEditMRS.OnLostDevice( m_pd3dDevice );

	return S_OK;
}

HRESULT CRanEditSkinPieceView::RestoreDeviceObjects()
{
	HRESULT hr=S_OK;

	DxResponseMan::GetInstance().RestoreDeviceObjects ();

	if ( m_pd3dDevice )
	{
		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )
		{
			m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE,		TRUE );
		}

		float fFogStart=720.0f, fFogEnd=790.0f, fFongDensity=0.0f;

		m_pd3dDevice->SetRenderState ( D3DRS_FOGSTART,		*((DWORD *)(&fFogStart)) );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGEND,		*((DWORD *)(&fFogEnd)) );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGDENSITY,	*((DWORD *)(&fFongDensity)) );

		m_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,	D3DFOG_LINEAR );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGTABLEMODE,	D3DFOG_NONE );

		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGRANGE )
		{
			m_pd3dDevice->SetRenderState ( D3DRS_RANGEFOGENABLE,	TRUE );
		}

		HRESULT hr;
		LPDIRECT3DSURFACEQ pBackBuffer;
		m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();

		DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
			(float)m_d3dsdBackBuffer.Height, 1.0f, 1500000.0f );
		
		hr = DxViewPort::GetInstance().SetViewPort ();
		if ( FAILED(hr) )	return hr;

		if ( m_pPiece )
		{
			m_pPiece->RestoreDeviceObjects( m_pd3dDevice );
		}

	}

	m_DxEditMRS.OnResetDevice( m_pd3dDevice );

	DXPARAMSET::INIT ();
	DxGlowMan::GetInstance().SetProjectActiveON();
	DxPostProcess::GetInstance().SetProjectActiveON();

	PROFILE_INIT();

	return S_OK;
}

HRESULT CRanEditSkinPieceView::FrameMove()
{
	PROFILE_BEGIN("FrameMove");

	CheckControlfp();

	DxResponseMan::GetInstance().FrameMove( m_fTime, m_fElapsedTime, m_bDefWin );

	CheckControlfp();

	DxEffProjMan::GetInstance().FrameMove( m_fElapsedTime );

	DxViewPort::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );

	if ( m_pPiece )
	{
		m_pPiece->FrameMove( m_fTime, m_fElapsedTime );
	}

	DxInputDevice &dxInputDev = DxInputDevice::GetInstance();
	DWORD dwML = dxInputDev.GetMouseState ( DXMOUSE_LEFT );
	bool bCLICK_LEFT = NULL != (dwML&(DXKEY_DOWNED|DXKEY_UP|DXKEY_DUP));

	{
		D3DXVECTOR3 vTargetPt, vFromPt;
		vFromPt = DxViewPort::GetInstance().GetFromPt ();
		BOOL bOk = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vTargetPt );
		if ( bOk )
		{
			if ( bCLICK_LEFT  )
			{
				if ( dwML&DXKEY_UP )
				{
					BOOL bValidMeshCol = false;
					BOOL bValidBoneCol = false;

					if ( m_pPiece )
					{
						if ( m_pPiece->m_pmcMesh )
						{
							if ( bRenderSkeleton )
							{
								if ( m_pPiece->m_pSkeleton )
								{

									BOOL bCollision = FALSE;
									std::string str;
									bCollision = m_pPiece->m_pSkeleton->IsCollision( vFromPt, vTargetPt, str, 1.0f );
									if ( bCollision ) strBoneName = str.c_str();
									bValidBoneCol = bCollision;
								}
							}


							if ( bMeshDetect )
							{
								if ( dwCOLTYPE  == 0 )
								{
									D3DXVECTOR3 vCollisionPos;
									DWORD dwCollisionID;
									BOOL bCollision;
									D3DXVECTOR3 vP;
									D3DXVECTOR3 vN;
									bCollision = m_pPiece->m_pmcMesh->GetClosedPoint( vFromPt, vTargetPt, vCollisionPos, dwCollisionID, vP, vN );
									if ( bCollision )	
									{
										m_vCOL = vCollisionPos;
										m_dwCOL = dwCollisionID;
									}
									
									bValidMeshCol = bCollision;
								}
								else if ( dwCOLTYPE == 1 )
								{
									D3DXVECTOR3 vCollisionPos;
									DWORD dwCollisionID;
									BOOL bCollision;
									D3DXVECTOR3 vP;
									D3DXVECTOR3 vN;
									bCollision = m_pPiece->m_pmcMesh->GetClosedPoint2( vFromPt, vTargetPt, vCollisionPos, dwCollisionID, vP, vN );
									if ( bCollision )	
									{
										m_vCOL = vCollisionPos;
										m_dwCOL = dwCollisionID;
									}
									
									bValidMeshCol = bCollision;
								}
								else if ( dwCOLTYPE == 2 )
								{
									D3DXVECTOR3 vCollisionPos;
									DWORD dwCollisionID;
									BOOL bCollision;
									bCollision = m_pPiece->m_pmcMesh->IsCollision( vFromPt, vTargetPt, vCollisionPos, dwCollisionID );
									if ( bCollision )	
									{
										m_vCOL = vCollisionPos;
										m_dwCOL = dwCollisionID;
									}
									
									bValidMeshCol = bCollision;
								}
							}
						}
					}

					if ( !bValidMeshCol )
					{
						m_vCOL = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
						m_dwCOL = COL_ERR;
					}

					if ( !bValidBoneCol )
					{
						strBoneName = "";
					}
				}
			}
		}
	}

	PROFILE_END("FrameMove");

	return S_OK;
}

HRESULT CRanEditSkinPieceView::Render()
{
	if ( !m_pd3dDevice )	return S_FALSE;

	PROFILE_BEGIN("Render");

	D3DCOLOR colorClear = D3DCOLOR_XRGB(120,120,120);

	HRESULT hr = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,colorClear, 1.0f, 0L );

	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		DxGlowMan::GetInstance().RenderTex ( m_pd3dDevice );
		DxLightMan::GetInstance()->Render ( m_pd3dDevice );
		DXLIGHT &Light = *DxLightMan::GetInstance()->GetDirectLight ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT, (float*)&Light.m_Light.Direction, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIFFUSE, (float*)&Light.m_Light.Diffuse, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTAMBIENT, (float*)&Light.m_Light.Ambient, 1 );
		D3DXVECTOR3 &vFromPt = DxViewPort::GetInstance().GetFromPt ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_CAMERAPOSITION, (float*)&vFromPt, 1 );
		
		D3DLIGHTQ	pLight;
		D3DXVECTOR4	vPointPos;
		D3DXVECTOR3	vPointDiff;
		for ( int i=0; i<7; i++ )
		{
			if ( DxLightMan::GetInstance()->GetClosedLight(i+1) )
			{
				pLight = DxLightMan::GetInstance()->GetClosedLight(i+1)->m_Light;
				vPointDiff = D3DXVECTOR3 ( pLight.Diffuse.r, pLight.Diffuse.g, pLight.Diffuse.b );
				vPointPos.x = pLight.Position.x;
				vPointPos.y = pLight.Position.y;
				vPointPos.z = pLight.Position.z;
				vPointPos.w = pLight.Range;
			}else{
				vPointPos = D3DXVECTOR4 ( 0.f, 0.f, 0.f, 0.1f );
				vPointDiff = D3DXVECTOR3 ( 0.f, 0.f, 0.f );
			}
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTPOS01, (float*)&vPointPos, 1);
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTDIFF01, (float*)&vPointDiff, 1);
		}

		D3DXMATRIX matView = DxViewPort::GetInstance().GetMatView();
		D3DXMATRIX matProj = DxViewPort::GetInstance().GetMatProj();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_SKIN_DEFAULT, (float*)&D3DXVECTOR4 (1.f, 0.5f, 0.f, 765.01f), 1 );
		D3DXVECTOR3	vLightVector = DxLightMan::GetInstance()->GetDirectLight()->m_Light.Direction;
		D3DXVec3TransformNormal ( &vLightVector, &vLightVector, &matView );
		D3DXVec3Normalize ( &vLightVector, &vLightVector);
		vLightVector = -vLightVector;
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT_VIEW, (float*)&vLightVector, 1 );
		D3DXMatrixTranspose( &matView, &matView );
		D3DXMatrixTranspose( &matProj, &matProj );
		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity( &matIdentity );
		m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE );	

		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )		
		m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, FALSE );

		CLIPVOLUME &CV = DxViewPort::GetInstance().GetClipVolume ();

		if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )	
		{
			m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, FALSE );
		}

		if ( bRenderPlane )
		{
			D3DXVECTOR3 vCenter ( 0.0f, 0.f, 0.0f );
			D3DXVECTOR3 vSize ( 20.0f, 0.f, 20.0f );
			D3DXVECTOR3 vSnap ( 5.0f, 0, 5.0f );

			DWORD dwZFunc;
			m_pd3dDevice->GetRenderState ( D3DRS_ZFUNC,		&dwZFunc );
			m_pd3dDevice->SetRenderState ( D3DRS_ZFUNC,		D3DCMP_ALWAYS );
			EDITMESHS::RENDERPLANE ( m_pd3dDevice, vCenter, vSize, vSnap );
			m_pd3dDevice->SetRenderState ( D3DRS_ZFUNC,		dwZFunc );
		}

		{
			if ( m_pPiece )
			{
				m_pPiece->Render( m_pd3dDevice, m_matLocal, NULL );
				m_pPiece->RenderTEST( m_pd3dDevice, m_matLocal, NULL);

				if ( bRenderTrace )
				{
					m_pPiece->DrawTraceVert( m_pd3dDevice );
				}
			}

			D3DXVECTOR3 vVALID = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			if ( m_vCOL != vVALID && m_pPiece && m_dwCOL != COL_ERR )
			{
				EDITMESHS::RENDERSPHERE ( m_pd3dDevice, m_vCOL, 0.3f, NULL, 0xff00ff00 );
			}

			DxEffGroupPlayer::GetInstance().Render ( m_pd3dDevice );
			OPTMManager::GetInstance().Render( m_pd3dDevice );

			if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )		
			{
				m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, FALSE );
			}

			DxCubeMap::GetInstance().Render ( m_pd3dDevice );
			DxGlowMan::GetInstance().RenderTex ( m_pd3dDevice );
			DxGlowMan::GetInstance().Render ( m_pd3dDevice );
			DxGlowMan::GetInstance().RenderBURN( m_pd3dDevice );
			DxPostProcess::GetInstance().Render( m_pd3dDevice );
			DxEffProjMan::GetInstance().Render( m_pd3dDevice );

			if ( m_pPiece )
			{
				if ( bRenderSkeleton )
				{
					if ( m_pPiece->m_pSkeleton )
					{
						DWORD dwZFunc;
						m_pd3dDevice->GetRenderState ( D3DRS_ZFUNC,		&dwZFunc );
						m_pd3dDevice->SetRenderState ( D3DRS_ZFUNC,		D3DCMP_ALWAYS );
						m_pPiece->m_pSkeleton->EditLineSphere( m_pd3dDevice );
						m_pPiece->m_pSkeleton->CheckSphere( m_pd3dDevice, strBoneName.c_str() );
						m_pd3dDevice->SetRenderState ( D3DRS_ZFUNC,		dwZFunc );
					}
				}
			}

			m_DxEditMRS.Render( m_pd3dDevice );
		}


		RenderText();
		CCursor::GetInstance().Render ( m_pd3dDevice, DxInputDevice::GetInstance().GetMouseLocateX(), DxInputDevice::GetInstance().GetMouseLocateY() );

		m_pd3dDevice->EndScene();
	}

	PROFILE_END("Render");
	return S_OK;
}

HRESULT CRanEditSkinPieceView::DeleteDeviceObjects()
{
#ifdef _DEBUG
	_CrtCheckMemory();
#endif //_DEBUG

	GLogicData::GetInstance().DeleteDeviceObjects ();

	if ( m_pPiece )
	{
		m_pPiece->DeleteDeviceObjects();
	}

	DxResponseMan::GetInstance().DeleteDeviceObjects ();
	CCursor::GetInstance().DeleteDeviceObjects ();


	return S_OK;
}

HRESULT CRanEditSkinPieceView::FinalCleanup()
{
	DxResponseMan::GetInstance().FinalCleanup ();
	GLogicData::GetInstance().ClearData ();


	return S_OK;
}