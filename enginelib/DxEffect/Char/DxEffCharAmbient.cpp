#include "stdafx.h"
#include "../Common/SerialFile.h"

#include "../DxCommon/DxLightMan.h"
#include "../DxCommon/DxViewPort.h"

#include "./DxEffCharAmbient.h"

#include "Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD		DxEffCharAmbient::TYPEID			= EMEFFCHAR_AMBIENT;
DWORD		DxEffCharAmbient::VERSION			= 0x0104;
DWORD		DxEffCharAmbient::VERSION_SAVE		= 0x0103;
char		DxEffCharAmbient::NAME[MAX_PATH]	= "EffCharAmbient";

DxEffCharAmbient::DxEffCharAmbient(void) :
	DxEffChar(),
	m_dwOldAmbient(0L),
	m_dwOldTexFactor(0L),
	m_dwOldColorARG1(0L),
	m_dwOldColorARG2(0L),
	m_dwOldColorOP(0L),
	m_fElapsedTime(0.f)
{
}

DxEffCharAmbient::~DxEffCharAmbient(void)
{
}

DxEffChar* DxEffCharAmbient::CloneInstance ( LPDIRECT3DDEVICEQ pd3dDevice, DxCharPart* pCharPart, DxSkinPiece* pSkinPiece )
{
	HRESULT hr;
	DxEffCharAmbient *pEffChar = new DxEffCharAmbient;
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



HRESULT DxEffCharAmbient::CreateDevice ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	return S_OK;
}

HRESULT DxEffCharAmbient::ReleaseDevice ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	return S_OK;
}

HRESULT DxEffCharAmbient::FrameMove ( float fTime, float fElapsedTime )
{
	m_fElapsedTime += fElapsedTime;

	return S_OK;
}

HRESULT	DxEffCharAmbient::SettingState ( LPDIRECT3DDEVICEQ pd3dDevice, SKINEFFDATA& sSKINEFFDATA )
{
	if( m_dwFlage & USETEXCOLOR )
	{
		DWORD dwTexFactor = 0xff000000 + ((DWORD)(m_cColor.r*255)<<16) + ((DWORD)(m_cColor.g*255)<<8) + (DWORD)(m_cColor.b*255);
		pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_dwOldTexFactor );

		pd3dDevice->GetRenderState( D3DRS_TEXTUREFACTOR,		&m_dwOldTexFactor );
		pd3dDevice->GetTextureStageState( 1, D3DTSS_COLORARG1,	&m_dwOldColorARG1 );
		pd3dDevice->GetTextureStageState( 1, D3DTSS_COLORARG2,	&m_dwOldColorARG2 );
		pd3dDevice->GetTextureStageState( 1, D3DTSS_COLOROP,	&m_dwOldColorOP );

		pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,		dwTexFactor );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1,	D3DTA_TFACTOR );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2,	D3DTA_CURRENT );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_MODULATE );
	}
	else
	{
		D3DLIGHTQ	pLight;
		pLight = DxLightMan::GetInstance()->GetDirectLight()->m_Light;

		pLight.Ambient.r = 0.f;
		pLight.Ambient.g = 0.f;
		pLight.Ambient.b = 0.f;

		pd3dDevice->SetLight ( 0, &pLight );

		pd3dDevice->GetRenderState ( D3DRS_AMBIENT, &m_dwOldAmbient );
		pd3dDevice->SetRenderState ( D3DRS_AMBIENT, D3DCOLOR_XRGB((int)(m_cColor.r*255),(int)(m_cColor.g*255),(int)(m_cColor.b*255)) );
	}

	return S_OK;
}

HRESULT	DxEffCharAmbient::RestoreState ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( m_dwFlage & USETEXCOLOR )
	{
		pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,		m_dwOldTexFactor );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1,	m_dwOldColorARG1 );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2,	m_dwOldColorARG2 );
		pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	m_dwOldColorOP );
	}
	else
	{
		pd3dDevice->SetRenderState ( D3DRS_AMBIENT, m_dwOldAmbient );

		pd3dDevice->SetLight ( 0, &DxLightMan::GetInstance()->GetDirectLight()->m_Light );

		//	Note : ?????? ?????? ????
		//
		if ( !(m_dwFlage&USEALL) && m_fElapsedTime > m_fFullTime )
		{
			m_fElapsedTime = m_fFullTime;
			SetEnd();
		}
	}

	return S_OK;
}

HRESULT	DxEffCharAmbient::LoadFile ( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice ,bool bTool,bool bRan2 )
{
	HRESULT hr;
	DWORD dwVer, dwSize;

	SFile >> dwVer;
	SFile >> dwSize;

	//	Note : ?????? ?????? ????. 
	//
	if ( dwVer == VERSION )
	{
		SFile >> m_dwFlage;
		SFile >> m_fFullTime;
		SFile >> m_cColor;
		SFile >> m_fSrcColor;
		SFile >> m_dwColorOP;

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
	else if ( dwVer == 0x0103 )
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
	else if ( dwVer == 0x0102 )
	{
		EFFCHAR_PROPERTY_AMBIENT_102	pProp;

		//	Note : ???????? Property ?? ????.
		//
		SFile.ReadBuffer ( &pProp, sizeof(EFFCHAR_PROPERTY_AMBIENT_102) );

		m_dwColorOP	= pProp.m_dwColorOP;
		m_fFullTime = 0.06f;
		m_fSrcColor	= 0.4f;
		m_cColor	= D3DCOLOR_ARGB(0,255,0,0);

		m_dwFlage	= pProp.m_dwFlage;

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
	else if ( dwVer == 0x0101 )
	{
		EFFCHAR_PROPERTY_AMBIENT_101	pProp;

		//	Note : ???????? Property ?? ????.
		//
		SFile.ReadBuffer ( &pProp, sizeof(EFFCHAR_PROPERTY_AMBIENT_101) );

		m_dwColorOP	= pProp.m_dwColorOP;
		m_fFullTime = 0.06f;
		m_fSrcColor	= 0.4f;
		m_cColor	= D3DCOLOR_ARGB(0,255,0,0);

		m_dwFlage	= 0;

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
		EFFCHAR_PROPERTY_AMBIENT_100	pProp;

		//	Note : ???????? Property ?? ????.
		//
		SFile.ReadBuffer ( &pProp, sizeof(EFFCHAR_PROPERTY_AMBIENT_100) );

		m_dwColorOP	= pProp.m_dwColorOP;
		m_fFullTime = 0.06f;
		m_fSrcColor	= 0.4f;
		m_cColor	= D3DCOLOR_ARGB(0,255,0,0);

		m_dwFlage	= 0;

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
		strTemp.Format( "DxEffCharAmbient::LoadFile Error! Name:%s Version:%d Size:%d Current Version:%d" ,SFile.GetFileName(),dwVer,dwSize,VERSION);
		if ( CCrypt::bEngineDebug )  MessageBox (NULL,strTemp,"Need New Version",MB_OK);
		if ( CCrypt::bEngineDebugLog )  CDebugSet::ToFile ( "EngineLoadError.txt",strTemp );
		strTemp.Empty();

		SFile.SetOffSet ( SFile.GetfTell()+dwSize );
		
		return E_FAIL;
	}

	return S_OK;
}

HRESULT	DxEffCharAmbient::SaveFile ( basestream &SFile ,bool bTool)
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


