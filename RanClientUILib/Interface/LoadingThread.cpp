#include "StdAfx.h"
#include <process.h>
#include "LoadingThread.h"

#include "SubPath.h"
#include "DxGrapUtils.h"
#include "DxResponseMan.h"
#include "UITextControl.h"
#include "GameTextControl.h"


#include "../../EngineLib/DxCommon/TextureManager.h"
#include "../../EngineLib/DxCommon/DxFontMan.h"
#include "../../EngineLib/G-Logic/GLogic.h"

#include "..\enginelib\Common\BaseString.h"
#include "..\ranclientlib\g-logic\glogicdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NLOADINGTHREAD;
std::vector<CString> m_vecTip;
int nIndex = NULL;
int MAX_INDEX =NULL;

//	-------------------- [ CUSTOM VERTEX 설정 ] --------------------
const	DWORD TEXTUREVERTEXFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
struct TEXTUREVERTEX
{
	union
	{
		struct { D3DXVECTOR4 vPos; };
		struct { float x, y, z, rhw; };
	};

	DWORD Diffuse;

	union
	{
		struct { D3DXVECTOR2 vUV; };
		struct { float tu, tv; };
	};

	TEXTUREVERTEX () :
		x(0.0f),
		y(0.0f),
		z(0.0f),
		rhw(1.0f),
		Diffuse(0xFFFFFFFF),
		tu(0.0f),
		tv(0.0f)
	{		
	}

	TEXTUREVERTEX ( float _x, float _y, float _tu, float _tv ) :
		x(_x),
		y(_y),
		z(0.0f),
		rhw(1.0f),
		Diffuse(0xFFFFFFFF),
		tu(_tu),
		tv(_tv)
	{		
	}

	void	SetPos ( float _x, float _y )
	{
		x = _x;
		y = _y;
	}

	void	SetTexPos ( float _tu, float _tv )
	{
		tu = _tu;
		tv = _tv;
	}
};

//	-------------------- [ CUSTOM VERTEX 제어 메소드 설정 ] --------------------

static HRESULT	CreateVB ( LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DVERTEXBUFFERQ& pTextureVB, TEXTUREVERTEX VertexArray[6] )
{	
	HRESULT hr = S_OK;
	hr = pd3dDevice->CreateVertexBuffer( 6*sizeof(TEXTUREVERTEX), 0, TEXTUREVERTEXFVF,
											D3DPOOL_MANAGED, &pTextureVB, NULL );
    if( FAILED ( hr ) )	return hr;    

    VOID* pVertices;
	hr = pTextureVB->Lock( 0, sizeof ( TEXTUREVERTEX ) * 6, (VOID**)&pVertices, 0 );
    if( FAILED ( hr ) ) return hr;

    memmove( pVertices, VertexArray, sizeof ( TEXTUREVERTEX ) * 6 );

    hr = pTextureVB->Unlock();
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

static void SetVertexPos ( TEXTUREVERTEX VertexArray[6], float LEFT, float TOP, float SIZEX, float SIZEY )
{
	float RIGHT = LEFT + SIZEX;
	float BOTTOM = TOP + SIZEY;

	VertexArray[0].SetPos ( LEFT,	TOP );
	VertexArray[1].SetPos ( RIGHT,	TOP);
	VertexArray[2].SetPos ( LEFT,	BOTTOM );

	VertexArray[3].SetPos ( LEFT,	BOTTOM );
	VertexArray[4].SetPos ( RIGHT,	TOP);
	VertexArray[5].SetPos ( RIGHT,	BOTTOM );
}

static void SetTexturePos ( TEXTUREVERTEX VertexArray[6], float LEFT, float TOP, float SIZEX, float SIZEY, float TEX_SIZEX, float TEX_SIZEY )
{
	float RIGHT = LEFT + SIZEX;
	float BOTTOM = TOP + SIZEY;

	VertexArray[0].SetTexPos ( LEFT / TEX_SIZEX,	TOP / TEX_SIZEY );
	VertexArray[1].SetTexPos ( RIGHT / TEX_SIZEX,	TOP / TEX_SIZEY);
	VertexArray[2].SetTexPos ( LEFT / TEX_SIZEX,	BOTTOM / TEX_SIZEY );

	VertexArray[3].SetTexPos ( LEFT / TEX_SIZEX,	BOTTOM / TEX_SIZEY );
	VertexArray[4].SetTexPos ( RIGHT / TEX_SIZEX,	TOP / TEX_SIZEY );
	VertexArray[5].SetTexPos ( RIGHT / TEX_SIZEX,	BOTTOM / TEX_SIZEY );
}

static HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DTEXTUREQ pLoadingTexture, LPDIRECT3DVERTEXBUFFERQ pTextureVB )
{
	HRESULT hr = S_OK;

	hr = pd3dDevice->SetTexture ( 0, pLoadingTexture );
	if ( FAILED ( hr ) ) return hr;

    hr = pd3dDevice->SetStreamSource( 0, pTextureVB, 0, sizeof(TEXTUREVERTEX) );
	if ( FAILED ( hr ) ) return hr;

	hr = pd3dDevice->SetFVF( TEXTUREVERTEXFVF );
	if ( FAILED ( hr ) ) return hr;

	hr = pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
	if ( FAILED ( hr ) ) return hr;

	hr = pd3dDevice->SetTexture ( 0, NULL );
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

BOOL MessagePump()
{
   MSG msg;

   while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
   {
       if(AfxGetApp() -> PumpMessage())
       {
             ::PostQuitMessage(0);
             return FALSE;
       }
   }

   return TRUE;
}

unsigned int WINAPI	LoadingThread( LPVOID pData )
{

	if( n_strTextureName.IsEmpty() || n_strTextureName == _T("null") )
	{
		srand ( (UINT)time( NULL ) );
		int nIndex = (rand() % 2) + 11;

		n_strTextureName.Format( "loading_%03d.dds", nIndex );	
	}

	TCHAR szTexture[256] = {0};
	StringCchPrintf ( szTexture, 256, n_strTextureName.GetString(), n_szAppPath, SUBPATH::TEXTURE_FILE_ROOT );

	LPDIRECT3DDEVICEQ& pd3dDevice = *n_ppd3dDevice;
	LPDIRECT3DTEXTUREQ pLoadingTexture = NULL;
	LPDIRECT3DTEXTUREQ pCopyRightTex = NULL;
	LPDIRECT3DTEXTUREQ pLoadingStepTex = NULL;
	LPDIRECT3DTEXTUREQ pLoadingBackTex = NULL;
	LPDIRECT3DTEXTUREQ pLoadingUnderTex = NULL;
	LPDIRECT3DTEXTUREQ pLoadingTopTex = NULL;
	LPDIRECT3DTEXTUREQ pHintIconTex = NULL;
	LPDIRECT3DTEXTUREQ pMapNameBackTex = NULL;

	LPD3DXSPRITE pSprite = NULL;
	int          ldstep = NULL;

	std::string	  m_strLoadingStepTex= "loading_st.dds";
	std::string	  m_strLoadingBackTex = "ld_back.dds";
	std::string	  m_strLoadingUnderTex = "ld_under.dds";
	std::string	  m_strLoadingTopTex = "ld_top.dds";
	std::string	  m_strHintIconTex = "HintIcon.dds";
	std::string	  m_strMapNameBackTex = "mapnamebackimg.dds";

	HRESULT hr = S_OK;
	hr = TextureManager::LoadTexture( szTexture, pd3dDevice, pLoadingTexture, 0, 0 );
	if ( FAILED ( hr ) )
	{		
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	hr = TextureManager::LoadTexture( m_strLoadingStepTex.c_str(), pd3dDevice, pLoadingStepTex, 0, 0 );
	if ( FAILED ( hr ) )
	{		
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	hr = TextureManager::LoadTexture( m_strLoadingBackTex.c_str(), pd3dDevice, pLoadingBackTex, 0, 0 );
	if ( FAILED ( hr ) )
	{		
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	hr = TextureManager::LoadTexture( m_strLoadingUnderTex.c_str(), pd3dDevice, pLoadingUnderTex, 0, 0 );
	if ( FAILED ( hr ) )
	{		
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	hr = TextureManager::LoadTexture( m_strLoadingTopTex.c_str(), pd3dDevice, pLoadingTopTex, 0, 0 );
	if ( FAILED ( hr ) )
	{		
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	hr = TextureManager::LoadTexture( m_strHintIconTex.c_str(), pd3dDevice, pHintIconTex, 0, 0 );
	if ( FAILED ( hr ) )
	{		
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	hr = TextureManager::LoadTexture( m_strMapNameBackTex.c_str(), pd3dDevice, pMapNameBackTex, 0, 0 );
	if ( FAILED ( hr ) )
	{		
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	//	[TOGGLE SWITCH] 텍스쳐 로딩완료
	DeWait ();
	///////////////////////////////////////////////////////////////////////////////

	RECT rect;
	GetClientRect ( n_hWnd, &rect );
	
	const float fWidth = float(rect.right - rect.left);
	const float fHeight = float(rect.bottom - rect.top);

	const float	fRealImageX = 1024.0f;
	const float	fRealImageY = 768.0f;
	const float	fImageSize = 1023.0f;

	const float fWidthRatio = fRealImageX / fImageSize;
	const float fHeightRatio= fRealImageY / fImageSize;

	const float LeftPos  = 0.0f;
	const float TopPos   = 0.0f;
	const float RightPos = fWidth;
	const float BottomPos= fHeight;

	float ROOT_LEFT		= LeftPos;
	float ROOT_TOP		= TopPos;
	float ROOT_SIZEX	= fWidth;
	float ROOT_SIZEY	= fHeight;

	const D3DXVECTOR2 vld_topTex ( 0, 0 );
	const D3DXVECTOR2 vld_topSize ( 1024, 256 );
	const D3DXVECTOR2 vld_topTexPos ( 0, 0 );

	const D3DXVECTOR2 vld_midTex ( 0, 0 );
	const D3DXVECTOR2 vld_midSize ( 1024, 512 );
	const D3DXVECTOR2 vld_midTexPos ( 0, 128 );

	const D3DXVECTOR2 vld_underTex ( 0, 0 );
	const D3DXVECTOR2 vld_underSize ( 1024, 256 );
	const D3DXVECTOR2 vld_underTexPos ( 0, 635 );

	const D3DXVECTOR2 vld_backTex ( 0, 0 );
	const D3DXVECTOR2 vld_backSize ( 128, 128);

	const D3DXVECTOR2 vhinticonTex ( 0, 0 );
	const D3DXVECTOR2 vhinticonSize ( 128, 64);

	const D3DXVECTOR2 vmapnamebackTex ( 0, 0 );
	const D3DXVECTOR2 vmapnamebackSize ( 330, 64);

	D3DXVECTOR2 vCopyrightAlign;
	vCopyrightAlign.x = 15.0f;
	vCopyrightAlign.y = fHeight - 30.0f;
	
	D3DXVECTOR2 vld_topAlignSize;
	vld_topAlignSize.x = static_cast<float>(floor(vld_topSize.x * fWidth / fRealImageX));
	vld_topAlignSize.y = static_cast<float>(floor(vld_topSize.y * fHeight/ fRealImageY));

	D3DXVECTOR2 vld_topAlign;
	vld_topAlign.x = (fWidth - vld_topAlignSize.x)/2.0f;
	vld_topAlign.y = static_cast<float>(floor(vld_topTexPos.y * fHeight/ fRealImageY));

	D3DXVECTOR2 vld_midAlignSize;
	vld_midAlignSize.x = static_cast<float>(floor(vld_midSize.x * fWidth / fRealImageX));
	vld_midAlignSize.y = static_cast<float>(floor(vld_midSize.y * fHeight/ fRealImageY));

	D3DXVECTOR2 vld_midAlign;
	vld_midAlign.x = (fWidth - vld_midAlignSize.x)/2.0f;
	vld_midAlign.y = static_cast<float>(floor(vld_midTexPos.y * fHeight/ fRealImageY));

	D3DXVECTOR2 vld_underAlignSize;
	vld_underAlignSize.x = static_cast<float>(floor(vld_underSize.x * fWidth / fRealImageX));
	vld_underAlignSize.y = static_cast<float>(floor(vld_underSize.y * fHeight/ fRealImageY));

	D3DXVECTOR2 vld_underAlign;
	vld_underAlign.x = (fWidth - vld_underAlignSize.x)/2.0f;
	vld_underAlign.y = static_cast<float>(floor(vld_underTexPos.y * fHeight/ fRealImageY));

	D3DXVECTOR2 vld_backAlign;
	vld_backAlign.x = fWidth - (vld_backSize.x+15.0f);
	vld_backAlign.y = fHeight -(vld_backSize.y+5.0f);

	D3DXVECTOR2 vhinticonAlign;
	vhinticonAlign.x = 15.0f;
	vhinticonAlign.y = fHeight -( vhinticonSize.y+25.0f);

	D3DXVECTOR2 vmapnamebackAlign;
	D3DXVECTOR2 vmapnamebackAlign2;
	vmapnamebackAlign2.x = fWidth / 2;
	vmapnamebackAlign2.y = fHeight / 7;

	vmapnamebackAlign.x = vmapnamebackAlign2.x - ( vmapnamebackSize.x / 2 );
	vmapnamebackAlign.y = vmapnamebackAlign2.y - ( vmapnamebackSize.y / 2 );

	LPDIRECT3DVERTEXBUFFERQ pld_topVB;
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_topAlign.x, ROOT_TOP + vld_topAlign.y, vld_topAlignSize.x, vld_topAlignSize.y  );
		SetTexturePos( VertexArray, vld_topTex.x, vld_topTex.y, vld_topSize.x, vld_topSize.y, vld_topSize.x, vld_topSize.y );

		if ( FAILED ( CreateVB ( pd3dDevice, pld_topVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	LPDIRECT3DVERTEXBUFFERQ pld_underVB;
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_underAlign.x, ROOT_TOP + vld_underAlign.y, vld_underAlignSize.x, vld_underAlignSize.y  );
		SetTexturePos( VertexArray, vld_underTex.x, vld_underTex.y, vld_underSize.x, vld_underSize.y, vld_underSize.x, vld_underSize.y);

		if ( FAILED ( CreateVB ( pd3dDevice, pld_underVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	LPDIRECT3DVERTEXBUFFERQ pTextureVB        = NULL; // Buffer to hold vertices
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_midAlign.x, ROOT_TOP + vld_midAlign.y, vld_midAlignSize.x, vld_midAlignSize.y );
		SetTexturePos( VertexArray, vld_midTex.x, vld_midTex.y, vld_midSize.x, vld_midSize.y, vld_midSize.x, vld_midSize.y );

		if ( FAILED ( CreateVB ( pd3dDevice, pTextureVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	LPDIRECT3DVERTEXBUFFERQ pld_backVB;
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_backAlign.x, ROOT_TOP + vld_backAlign.y, vld_backSize.x, vld_backSize.y  );
		SetTexturePos( VertexArray, vld_backTex.x, vld_backTex.y, vld_backSize.x, vld_backSize.y, vld_backSize.x, vld_backSize.y);

		if ( FAILED ( CreateVB ( pd3dDevice, pld_backVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	LPDIRECT3DVERTEXBUFFERQ phinticonVB;
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vhinticonAlign.x, ROOT_TOP + vhinticonAlign.y, vhinticonSize.x, vhinticonSize.y  );
		SetTexturePos( VertexArray, vhinticonTex.x, vhinticonTex.y, vhinticonSize.x, vhinticonSize.y, vhinticonSize.x, vhinticonSize.y);

		if ( FAILED ( CreateVB ( pd3dDevice, phinticonVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	LPDIRECT3DVERTEXBUFFERQ pmapnamebackVB;
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vmapnamebackAlign.x, ROOT_TOP + vmapnamebackAlign.y, vmapnamebackSize.x, vmapnamebackSize.y  );
		SetTexturePos( VertexArray, vmapnamebackTex.x, vmapnamebackTex.y, vmapnamebackSize.x, vmapnamebackSize.y, vmapnamebackSize.x, vmapnamebackSize.y);

		if ( FAILED ( CreateVB ( pd3dDevice, pmapnamebackVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	// CopyRight 출력용 폰트에 크리티컬 섹션 사용 설정 ( 준혁 )
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont12 = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 12, _DEFAULT_FONT_FLAG );
	if( pFont )
		pFont->UsageCS( TRUE );
	if( pFont12 )
		pFont12->UsageCS( TRUE );

	//	뿌리기
	while ( n_bRender )
	{
		if( pSprite == NULL )
		{
			if ( FAILED (D3DXCreateSprite(pd3dDevice,&pSprite)))
			{
				MessageBoxA(NULL,"Cannot Create Sprite",0,0);
			}

		}

		if( m_vecTip.size() ==0 )
		{
			LoadTip(n_szAppPath);
		}

        MAX_INDEX = static_cast<int>( m_vecTip.size() );
		if( MAX_INDEX > 0 )
		{
			srand ( (UINT)time( NULL ) );
			if( nIndex == NULL )
			{
				nIndex = rand () % MAX_INDEX;
			}
		}

		HRESULT hr;
		if( FAILED( hr = pd3dDevice->TestCooperativeLevel() ) )
		{
			// If the device was lost, do not render until we get it back
			if( D3DERR_DEVICELOST == hr )
			{
				CDebugSet::ToListView ( "[ERROR] D3DERR_DEVICELOST _ LoadingThread() FAILED" );
				continue;
			}

			// Check if the device needs to be resized.
			if( D3DERR_DEVICENOTRESET == hr )
				continue;
		}

		hr = pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L );

		// Begin the scene
		if( SUCCEEDED( pd3dDevice->BeginScene() ) )
		{
			//	밑 바닥
			hr = Render ( pd3dDevice, pLoadingTexture, pTextureVB );

			DWORD dwAlphaBlendEnable;
			pd3dDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable );

			pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
			pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

			DWORD dwAlphaOP;
			pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwAlphaOP );
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

			DWORD dwMin, dwMag, dwMip;
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MINFILTER,	&dwMin );
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MAGFILTER,	&dwMag );
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MIPFILTER,	&dwMip );

			pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	D3DTEXF_POINT );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	D3DTEXF_POINT );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	D3DTEXF_NONE );

			hr = Render ( pd3dDevice, pLoadingTopTex, pld_topVB );
			hr = Render ( pd3dDevice, pLoadingUnderTex, pld_underVB );
			hr = Render ( pd3dDevice, pLoadingBackTex, pld_backVB );
			hr = Render ( pd3dDevice, pHintIconTex, phinticonVB );
			
			RECT srcRect;
			srcRect.top    = (( ldstep / 4 ) * 105);
			srcRect.left   = (( ldstep % 4 ) * 105);
			srcRect.bottom = (( ldstep / 4 ) * 105) + 105;
			srcRect.right  = (( ldstep % 4 ) * 105) + 105;

			D3DXVECTOR3 vCenter( 0.0f, 0.0f, 0.0f );
			D3DXVECTOR3 vPosition(vld_backAlign.x + 12.0f,vld_backAlign.y + 12.0f, 0.0f );
			pSprite->Begin( D3DXSPRITE_ALPHABLEND );
			pSprite->Draw( pLoadingStepTex,&srcRect,&vCenter,&vPosition,D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f) );

			if( !n_strMapName.IsEmpty() || n_strMapName != _T("null") )
			{
				hr = Render ( pd3dDevice, pMapNameBackTex, pmapnamebackVB );
				if( pFont12 )
				{
					CString strTemp;
					strTemp.Format("< %s >", n_strMapName.GetString() );
					float flength = (float)strTemp.GetLength();
					float xAlign = vmapnamebackSize.x * ( flength / 100 ) ;
					float xAlignA =  ( vmapnamebackSize.x /2 ) -  ( xAlign + flength );
					pFont12->DrawText( vmapnamebackAlign.x  + xAlignA , vmapnamebackAlign.y + 20.0f, NS_UITEXTCOLOR::WHITE, strTemp.GetString() );
				}
			}

			if( nIndex > 0 && nIndex< m_vecTip.size() )
			{
				pFont->DrawText( vhinticonAlign.x+85.0f,vhinticonAlign.y+20.0f,NS_UITEXTCOLOR::WHITE, m_vecTip[ nIndex ] );	
			}

			int nIndex1 = nIndex + 1;
			if ( nIndex1 > 0 && nIndex1 < m_vecTip.size() )
			{
				pFont->DrawText(vhinticonAlign.x+85.0f,vhinticonAlign.y+35.0f,NS_UITEXTCOLOR::WHITE, m_vecTip[ nIndex1 ] );
			}

			pSprite->End();

			if( pFont )
			{
				pFont->DrawText( vCopyrightAlign.x, vCopyrightAlign.y, NS_UITEXTCOLOR::WHITE, ID2GAMEWORD("COPYRIGHT_COMPANY_LOAD") );
			}
	
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwAlphaOP );
			pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable );

			pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	dwMin );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	dwMag );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	dwMip );

			// End the scene.
			pd3dDevice->EndScene();
			pd3dDevice->Present( NULL, NULL, NULL, NULL );
		}
		{
			++ldstep;
			if( ldstep > 3 )
				ldstep = 0;
		}
		Sleep(500);
	}

	if( pFont )
		pFont->UsageCS( FALSE );
	if( pFont12 )
		pFont12->UsageCS( FALSE );

	SAFE_RELEASE ( pTextureVB );
	SAFE_RELEASE ( pld_topVB );
	SAFE_RELEASE ( pld_underVB );
	SAFE_RELEASE ( pld_backVB );
	SAFE_RELEASE ( phinticonVB );
	SAFE_RELEASE ( pmapnamebackVB );
	SAFE_RELEASE ( pSprite );
	
	TextureManager::ReleaseTexture( szTexture, pLoadingTexture );
	TextureManager::ReleaseTexture( m_strLoadingStepTex.c_str(), pLoadingStepTex );
	TextureManager::ReleaseTexture( m_strLoadingBackTex.c_str(), pLoadingBackTex );
	TextureManager::ReleaseTexture( m_strLoadingUnderTex.c_str(), pLoadingUnderTex );
	TextureManager::ReleaseTexture( m_strLoadingTopTex.c_str(), pLoadingTopTex );
	TextureManager::ReleaseTexture( m_strHintIconTex.c_str(), pHintIconTex );
	TextureManager::ReleaseTexture( m_strMapNameBackTex.c_str(), pMapNameBackTex );

	nIndex = rand () % MAX_INDEX;
	n_strMapName = _T("");

	///////////////////////////////////////////////////////////////////////////////
	//	[TOGGLE SWITCH] 로더 스레드 종료
	DeWait ();
	//////////////////////////////////////////////////////////////////////////////
	n_ExitState = eNORMAL;

	return 0;
}

namespace	NLOADINGTHREAD
{
	DWORD				n_dwThreadID;
	LPDIRECT3DDEVICEQ*	n_ppd3dDevice;
	HWND				n_hWnd;
	BOOL				n_bWait;
	BOOL				n_bRender;
	bool				n_bOVER15;
	char				n_szAppPath[MAX_PATH] = {0};
	int					n_ExitState;
	int					n_Step = 0;
	HANDLE				n_hThread = NULL;
	CString				n_strTextureName;
	CString				n_strMapName;

	HRESULT	StartThreadLOAD(LPDIRECT3DDEVICEQ* ppd3dDevice, 
							HWND hWnd, 
							char* szAppPath, 
							const CString & strTextureName, 
							const CString & strMapName, 
							bool bOVER15 )
	{
		n_bOVER15 = bOVER15;
		n_ppd3dDevice = ppd3dDevice;
		n_hWnd = hWnd;
		StringCchCopy ( n_szAppPath, MAX_PATH, szAppPath );
		n_strTextureName = strTextureName.GetString();
		n_strMapName = strMapName.GetString();
		
		n_bRender = TRUE;	//	렌더
		EnWait ();
		n_ExitState = eNORMAL;

		//n_hThread = CreateThread( NULL, 0, LoadingThread, NULL, 0, &n_dwThreadID );
		n_hThread = (HANDLE) ::_beginthreadex(
									NULL,
									0,
									LoadingThread,
									NULL,
									0,
									(unsigned int*) &n_dwThreadID );
		if ( !n_hThread )
		{
			DeWait();
			return E_FAIL;
		}

		return S_OK;
	}

	void	WaitThread ()
	{
		while ( GetWait () )
		{		
			Sleep( 1 );
		}
		EnWait ();
	}

	void	EndThread ()
	{		
		n_bRender = FALSE;

		if ( n_ExitState == eNORMAL )
		{
			WaitThread ();
		}

		CloseHandle( n_hThread );
		n_hThread = NULL;

		n_strTextureName.Empty();
	}

	BOOL	GetWait ()
	{
		return n_bWait;
	}

	void	DeWait  ()
	{
		n_bWait = FALSE;
	}

	void	EnWait ()
	{
		n_bWait = TRUE;
	}

	void	SetStep ( int step )
	{
		n_Step = step;
	}

	int		GetStep ()
	{
		return n_Step;
	}

	void  LoadTip( char* szAppPath)
	{
		CString strFileName = szAppPath;
		strFileName += SUBPATH::GLOGIC_FILE;
		strFileName += ID2GAMEWORD("TIP_FILE");

		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath( strFileName.GetString(), NULL, NULL, fname, ext );
		StringCchCat ( fname, _MAX_EXT, ext );

		std::auto_ptr<CBaseString> pBString(GLOGIC::openfile_basestring(GLOGIC::bGLOGIC_ZIPFILE, 
			GLOGIC::strGLOGIC_ZIPFILE.c_str(), 
			strFileName.GetString(), 
			fname,
			false,
			GLOGIC::bGLOGIC_PACKFILE ) );

		if ( !pBString.get() )
		{
			return ;
		}

		CBaseString &cBString = *pBString;

		CString strLine;
		CString strFullText;
		while ( cBString.GetNextLine(strLine) )
		{
			if ( strLine == ';' )
			{
				int nLength = strFullText.GetLength ();
				if ( 2<nLength && strFullText[nLength-2] == '\r' && strFullText[nLength-1] == '\n' )
					strFullText = strFullText.Left(nLength-2);

				nLength = strFullText.GetLength ();
				if ( nLength )
				{
					m_vecTip.push_back ( strFullText );
					strFullText.Empty ();
				}
			}
			else
			{			
				strLine += "\r\n";
				strFullText += strLine;
			}
		}
		if ( strFullText.GetLength () )	m_vecTip.push_back ( strFullText );
	}
};