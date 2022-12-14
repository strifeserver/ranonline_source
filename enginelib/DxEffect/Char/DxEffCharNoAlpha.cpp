#include "stdafx.h"
#include "../DxEffect/Single/DxEffSingle.h"
#include "../Single/DxEffSinglePropGMan.h"
#include "../Common/SerialFile.h"

#include "../DxCommon/DxViewPort.h"
#include "../DxEffect/DxEffectMan.h"

#include "./DxEffCharNoAlpha.h"

#include "Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD		DxEffCharNoAlpha::TYPEID			= EMEFFCHAR_NOALPHA;
DWORD		DxEffCharNoAlpha::VERSION			= 0x0101;
DWORD		DxEffCharNoAlpha::VERSION_SAVE		= 0x0101;
char		DxEffCharNoAlpha::NAME[MAX_PATH]	= "EffCharNoAlpha";

LPDIRECT3DSTATEBLOCK9		DxEffCharNoAlpha::m_pSavedStateBlock	= NULL;
LPDIRECT3DSTATEBLOCK9		DxEffCharNoAlpha::m_pEffectStateBlock	= NULL;

DxEffCharNoAlpha::DxEffCharNoAlpha(void) :
	DxEffChar()
{
}

DxEffCharNoAlpha::~DxEffCharNoAlpha(void)
{
	CleanUp ();
}

DxEffChar* DxEffCharNoAlpha::CloneInstance ( LPDIRECT3DDEVICEQ pd3dDevice, DxCharPart* pCharPart, DxSkinPiece* pSkinPiece )
{
	HRESULT hr;
	DxEffCharNoAlpha *pEffChar = new DxEffCharNoAlpha;
	pEffChar->SetLinkObj ( pCharPart, pSkinPiece );
	pEffChar->SetProperty ( &m_Property );

	hr = pEffChar->Create ( pd3dDevice );
	if ( FAILED(hr) )
	{
		SAFE_DELETE(pEffChar);
		return NULL;
	}

	return pEffChar;
}

HRESULT DxEffCharNoAlpha::CreateDevice ( LPDIRECT3DDEVICEQ pd3dDevice )
{	
	for( UINT which=0; which<2; which++ )
	{
		pd3dDevice->BeginStateBlock();

		pd3dDevice->SetRenderState ( D3DRS_ZWRITEENABLE,		TRUE );
		pd3dDevice->SetRenderState ( D3DRS_FOGENABLE,			FALSE );

		pd3dDevice->SetRenderState ( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState ( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA );

		pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,		FALSE );

		pd3dDevice->SetTextureStageState ( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
		pd3dDevice->SetTextureStageState ( 0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );

		if( which==0 )	pd3dDevice->EndStateBlock( &m_pSavedStateBlock );
		else			pd3dDevice->EndStateBlock( &m_pEffectStateBlock );
	}

	return S_OK;
}

HRESULT DxEffCharNoAlpha::ReleaseDevice ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	SAFE_RELEASE( m_pSavedStateBlock );
	SAFE_RELEASE( m_pEffectStateBlock );

	return S_OK;
}

HRESULT DxEffCharNoAlpha::FrameMove ( float fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;

	return S_OK;
}

HRESULT	DxEffCharNoAlpha::SettingState ( LPDIRECT3DDEVICEQ pd3dDevice, SKINEFFDATA& sSKINEFFDATA )
{
	m_pSavedStateBlock->Capture();
	m_pEffectStateBlock->Apply();

	return S_OK;
}

HRESULT	DxEffCharNoAlpha::RestoreState ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	m_pSavedStateBlock->Apply();

	return S_OK;
}

HRESULT	DxEffCharNoAlpha::LoadFile ( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice ,bool bTool,bool bRan2 )
{
	HRESULT hr;
	DWORD dwVer, dwSize;

	SFile >> dwVer;
	SFile >> dwSize;

	//	Note : ?????? ?????? ????. 
	//
	if ( dwVer == VERSION )
	{
		//	Note : ???????? Property ?? ????.
		//
		SFile.ReadBuffer ( &m_Property, sizeof(m_Property) );

		//	Note : Device ?????? ????????.
		//
		if (!bTool)
		{
			hr = Create( pd3dDevice );
			if ( FAILED(hr) )	return hr;
		}
		/*hr = Create ( pd3dDevice );
		if ( FAILED(hr) )	return hr;*/
	}
	else if ( dwVer == 0x0100 )
	{
		//	Note : ???????? Property ?? ????.
		//
		EFFCHAR_PROPERTY_NOALPHA_100	sProperty;
		SFile.ReadBuffer ( &sProperty, sizeof(EFFCHAR_PROPERTY_NOALPHA_100) );

		//	Note : Device ?????? ????????.
		//
		if (!bTool)
		{
			hr = Create( pd3dDevice );
			if ( FAILED(hr) )	return hr;
		}
		/*hr = Create ( pd3dDevice );
		if ( FAILED(hr) )	return hr;*/
	}
	else
	{
		CString strTemp = "";
		strTemp.Format( "DxEffCharNoAlpha::LoadFile Error! Name:%s Version:%d Size:%d Current Version:%d" ,SFile.GetFileName(),dwVer,dwSize,VERSION);
		if ( CCrypt::bEngineDebug )  MessageBox (NULL,strTemp,"Need New Version",MB_OK);
		if ( CCrypt::bEngineDebugLog )  CDebugSet::ToFile ( "EngineLoadError.txt",strTemp );
		strTemp.Empty();
		//
		SFile.SetOffSet ( SFile.GetfTell()+dwSize );
		
		return E_FAIL;
	}

	return S_OK;
}

HRESULT	DxEffCharNoAlpha::SaveFile ( basestream &SFile ,bool bTool)
{
	SFile << TYPEID;
	SFile << VERSION_SAVE;

	//	Note : DATA?? ???????? ????????. Load ???? ?????? ???? ???? ??????.
	//
	SFile << (DWORD) ( sizeof(m_Property) );

	//	Note : ???????? Property ?? ????.
	//
	SFile.WriteBuffer ( &m_Property, sizeof(m_Property) );

	return S_OK;
}