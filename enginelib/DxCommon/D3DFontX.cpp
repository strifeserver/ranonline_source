#include "stdafx.h"
#include "./D3DFont.h"
#include "./D3DFontX.h"
#include "../TextTexture/TextUtil.h"

#include "../../MfcExLib/GetWinVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	const DWORD g_dwExtentFlags = DT_SINGLELINE | DT_EXPANDTABS | DT_CALCRECT;
	const DWORD g_dwDrawFlags = DT_SINGLELINE | DT_EXPANDTABS | DT_NOCLIP;

	const POINT g_ptOffset[ 2 ] =
	{
		{ 1, 0 }, { 1, 1 }
	};

	const DWORD g_colorFontDX[ 2 ] =
	{
		D3DCOLOR_ARGB( 255, 50, 50, 50 ), D3DCOLOR_ARGB( 255, 10, 10, 10 )
	};

	const COLORREF g_colorFontDC[ 3 ] =
	{
		RGB( 50, 50, 50 ), RGB( 10, 10, 10 ), RGB( 255, 255, 255 )
	};

	const TCHAR g_szNull[ 2 ] = _T( "i" );

	const LONG g_lfWeight = FW_NORMAL;
	const BYTE g_lfQuality = ANTIALIASED_QUALITY;
}

namespace
{
	// ???
	const int g_nSungjo = 5;
	static const TCHAR g_szSungjo[ g_nSungjo ] =
	{
		0xcc, 0xd2, 0xde, 0xec, 0xf2
	};

	// ???????? ???????? ???? ????
	const int g_nExAlphabet = 17;
	static const TCHAR g_szExAlphabet[ g_nExAlphabet ] =
	{
		0xc3, 0xe3, 0xc2, 0xe2, 0xca, 0xea, 0xd4, 0xf4,
		0xd5, 0xf5, 0xdd, 0xfd, 0xd0, 0xf0, 0x91, 0x92,
        0x85
	};

	/* A, a, E, e, I, i , O, o, U, u Y, y ?????? ?????? */
	const int g_nMoum = 24;
	static const TCHAR g_szMoum[ g_nMoum ] =
	{
		0x41, 0x61, 0xc3, 0xe3, 0xc2, 0xe2, 0x45, 0x65, 0xca, 0xea,
        0x49, 0x69, 0x4f, 0x6f, 0xd4, 0xf4, 0xd5, 0xf5, 0x55, 0x75,
        0xdd, 0xfd, 0x59, 0x79
	};
}

BOOL CD3DFontX::m_bWindows98 = TRUE;

CD3DFontX::CD3DFontX( TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags, LANGFLAG emLang )
	: m_pd3dDevice( NULL )
	, m_iHeightScreen( 0 )
	, m_iOutLine( 1 )

	, m_pd3dxFont( NULL )
	, m_hd3dxDC( NULL )

	, m_hDC( NULL )
	, m_hFont( NULL )
	, m_hFontOld( NULL )
{
	_tcscpy( m_strFontName, strFontName );

	m_dwFontHeight = dwHeight;
	m_dwFontFlags = dwFlags;
	m_emLangFlag = emLang;
}

CD3DFontX::~CD3DFontX()
{
}

HRESULT CD3DFontX::Create( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( FAILED( InitDeviceObjects( pd3dDevice ) ) )
		return E_FAIL;
	if( FAILED( RestoreDeviceObjects() ) )
		return E_FAIL;

	return S_OK;
}

HRESULT CD3DFontX::InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	INT nWinVer;
	TCHAR szWinVer[ 50 ], szMajorMinorBuild[ 50 ];
	if( GetWinVer( szWinVer, &nWinVer, szMajorMinorBuild ) )
	{
		m_bWindows98 = ( nWinVer < WNTFIRST );
		if( m_bWindows98 )
            m_iOutLine = 1;
	}

	m_hDC = CreateCompatibleDC( NULL );
	SetMapMode( m_hDC, MM_TEXT );

	m_iHeightScreen = -( MulDiv( (INT)m_dwFontHeight, GetDeviceCaps( m_hDC, LOGPIXELSY ), 72 ) );

	LOGFONT logFont;
	SecureZeroMemory( &logFont, sizeof( LOGFONT ) );
	logFont.lfHeight = m_iHeightScreen;
	logFont.lfWeight = g_lfWeight;
	logFont.lfQuality = g_lfQuality;
	logFont.lfCharSet = nCharSet[ m_emLangFlag ];
	StringCchCopy( logFont.lfFaceName, LF_FACESIZE, m_strFontName );

	m_hFont = CreateFontIndirect( &logFont );

	m_hFontOld = (HFONT)SelectObject( m_hDC, m_hFont );

	return S_OK;
}

HRESULT CD3DFontX::RestoreDeviceObjects()
{
	D3DXFONT_DESC logFont;
	SecureZeroMemory( &logFont, sizeof( D3DXFONT_DESC ) );
	logFont.Height = m_iHeightScreen;
	logFont.Weight = g_lfWeight;
	logFont.Quality = g_lfQuality;
	logFont.CharSet = nCharSet[ m_emLangFlag ];
	logFont.MipLevels = 1;
	StringCchCopy( logFont.FaceName, LF_FACESIZE, m_strFontName );

	SAFE_RELEASE( m_pd3dxFont );
	HRESULT hr = D3DXCreateFontIndirect( m_pd3dDevice, &logFont, &m_pd3dxFont );
	if ( FAILED(hr) )
		return hr;

	m_hd3dxDC = m_pd3dxFont->GetDC();

	return S_OK;
}

HRESULT CD3DFontX::InvalidateDeviceObjects()
{
	SAFE_RELEASE( m_pd3dxFont );
	m_hd3dxDC = NULL;

	return S_OK;
}

HRESULT CD3DFontX::DeleteDeviceObjects()
{
	SAFE_RELEASE( m_pd3dxFont );
	m_hd3dxDC = NULL;

	SelectObject( m_hDC, m_hFontOld );
	DeleteObject( m_hFont );
	DeleteDC( m_hDC );

	return S_OK;
}

HRESULT CD3DFontX::CleanUp()
{
	InvalidateDeviceObjects();
	DeleteDeviceObjects();

	return S_OK;
}

HRESULT CD3DFontX::DrawText( FLOAT x, FLOAT y, DWORD dwColor, const TCHAR* strText, DWORD dwFlags, BOOL bImmediately )
{
	if( bImmediately || CTextUtil::m_bUsage == FALSE )
		return DrawText( strText, dwColor, dwFlags, x, y );

	if( CTextUtil::Get()->DrawText( strText, this, dwColor, x, y ) == FALSE )
		return DrawText( strText, dwColor, dwFlags, x, y );

	return S_OK;
}

HRESULT CD3DFontX::GetTextExtent( const TCHAR* strText, SIZE &Size )
{
	if( !m_pd3dxFont || !m_hd3dxDC )
		return S_OK;

	BOOL bUsageCS = m_bUsageCS;
	if( bUsageCS )
		EnterCriticalSection( &m_csLock );

	INT iText = 1;
	BOOL bTextNull = TRUE;

	// ???? ?????? ?????? ???? ?????? ???? ???? strText ?? ?????????? NULL ????.
	// LPD3DXFONT ???? ?????? ???? ???????? ?????????? ?????????? ????????.
	if( strText )
	{
		iText = (INT)_tcslen( strText );
		if( iText )
		{
			bTextNull = FALSE;
		}
		else
		{
			strText = g_szNull;
			iText = 1;
		}
	}
	else
	{
		strText = g_szNull;
	}

	WCHAR * pwszTemp = new WCHAR[ iText+2 ];
	INT iWText = 0;
	Size.cx = Size.cy = 0;

	// ???? 98 ?????? GetTextExtentPoint ?????? ???????????? ???? ????????.
	if( m_bWindows98 )
		ConvWideAndTextExtent98( strText, iText, pwszTemp, iWText, Size, FALSE );
	else
		ConvWideAndTextExtent( strText, iText, pwszTemp, iWText, Size, FALSE );

	SAFE_DELETE_ARRAY( pwszTemp );

	if( bTextNull )
		Size.cx = 0;

	if( bUsageCS )
		LeaveCriticalSection( &m_csLock );

	return S_OK;
}

VOID CD3DFontX::ConvWideAndTextExtent( const TCHAR* strText, INT iText, WCHAR* wstrText, INT &iWText, SIZE &Size, BOOL bConv )
{
	// ???? ???????? ???? ???????? ?????? ???? ???? ??????  GetTextExtentPoint ?????? ????????.
	// ???? ?????? ???? ???????? ?????? ?????? ?????? ????.

	memset( wstrText, 0, sizeof( WCHAR ) * ( iText + 2 ) );

	iWText = MultiByteToWideChar( nCodePage[ m_emLangFlag ], 0, strText, iText, wstrText, iText );

	if( iWText > 0 )
	{
		Size.cx = Size.cy = 0;
		GetTextExtentPoint32W( m_hd3dxDC, wstrText, iWText, &Size );

		if( Size.cx == 0 || Size.cy == 0 )
		{
			ConvWideAndTextExtent98( strText, iText, wstrText, iWText, Size, bConv );
		}
	}
}

VOID CD3DFontX::ConvWideAndTextExtent98( const TCHAR* strText, INT iText, WCHAR* wstrText, INT &iWText, SIZE &Size, BOOL bConv )
{
	// LPD3DXFONT::DrawText ?????????? ???? ?????? ???? ???????? ???? ????????.
	// ???? ?????? i?? ?????? ????????. ???? ?????? ???????? MultiByteToWideChar ???? ??????????.

	CString szTemp = strText;
	INT iResult = szTemp.Replace( (char)32, (char)105 );

	memset( wstrText, 0, sizeof( WCHAR ) * ( iText + 2 ) );
	iWText = MultiByteToWideChar( nCodePage[ m_emLangFlag ], 0, szTemp.GetString(), iText, wstrText, iText );

	if( iWText > 0 )
	{
		RECT rtCalc = { 0, 0, 0, 0 };
		m_pd3dxFont->DrawTextW( NULL, wstrText, iWText, &rtCalc, g_dwExtentFlags, 0L );

		Size.cx = rtCalc.right - rtCalc.left;
		Size.cy = rtCalc.bottom - rtCalc.top;
	}

	if( bConv && iResult )
	{
		memset( wstrText, 0, sizeof( WCHAR ) * ( iText+2 ) );
		iWText = MultiByteToWideChar( nCodePage[ m_emLangFlag ], 0, strText, iText, wstrText, iText );
	}
}

INT CD3DFontX::GetHeightScreen()
{
	return (INT)( (INT)abs( m_iHeightScreen ) + CTextUtil::m_iSpaceSize );
}

HRESULT CD3DFontX::DrawText( const TCHAR* strText, DWORD dwColor, DWORD dwFlags, FLOAT x, FLOAT y )
{
	if( !m_pd3dxFont || !m_hd3dxDC || !strText )
		return S_OK;

	INT iText = (INT)_tcslen( strText );
	if( !iText )
		return S_OK;

	// ?????????? ???????? ?????? ?????? ???? ????.
	CString szTemp = strText;
	if( szTemp.Replace( (char)32, (char)105 ) == iText )
		return S_OK;

	BOOL bUsageCS = m_bUsageCS;
	if( bUsageCS )
		EnterCriticalSection( &m_csLock );

	WCHAR * pwszTemp = new WCHAR[ iText+2 ];
	INT iWText = 0;
	SIZE Size = { 0, 0 };

	TCHAR strNewText[MAX_PATH] = {0};
	_tcscpy( strNewText, strText );


		CheckVietnam( strText, strNewText );
		iText = (INT)_tcslen( strNewText );
	

	// ???? 98 ?????? GetTextExtentPoint ?????? ???????????? ???? ????????.
	if( m_bWindows98 )
		ConvWideAndTextExtent98( strNewText, iText, pwszTemp, iWText, Size, TRUE );
	else
		ConvWideAndTextExtent( strNewText, iText, pwszTemp, iWText, Size, TRUE );

	// ???? ?????? ???????? ?????? ?? ????.
	if( iWText < 1 || Size.cx == 0 || Size.cy == 0 )
	{
		SAFE_DELETE_ARRAY( pwszTemp );

		if( bUsageCS )
			LeaveCriticalSection( &m_csLock );

		return S_OK;
	}

	Size.cx += (LONG)x;
	Size.cy += (LONG)y;
	RECT rectText = { (LONG)x, (LONG)y, (LONG)Size.cx, (LONG)Size.cy };

	if( DxFontMan::g_pSprite )
		DxFontMan::g_pSprite->Begin( D3DXSPRITE_ALPHABLEND );

	if( m_dwFontFlags & D3DFONT_SHADOW )
	{
		for( LONG lX = -m_iOutLine; lX <= m_iOutLine; ++lX )
		{
			for( LONG lY = -m_iOutLine; lY <= m_iOutLine; ++lY )
			{
				if( !lX && !lY )
					continue;

				RECT rtOutline =
				{
					rectText.left + lX,
					rectText.top + lY,
					rectText.right + lX,
					rectText.bottom + lY
				};

				m_pd3dxFont->DrawTextW
				(
					DxFontMan::g_pSprite,
					pwszTemp,
					iWText,
					&rtOutline,
					g_dwDrawFlags,
					g_colorFontDX[ 1 ]
				);
			}
		}
	}
	else if( m_dwFontFlags & D3DFONT_SHADOW_EX )
	{
		for( INT i=0; i<2; ++i )
		{
			RECT rtOutline =
			{
				rectText.left + g_ptOffset[ i ].x,
				rectText.top + g_ptOffset[ i ].y,
				rectText.right + g_ptOffset[ i ].x,
				rectText.bottom + g_ptOffset[ i ].y
			};

			m_pd3dxFont->DrawTextW
			(
				DxFontMan::g_pSprite,
				pwszTemp,
				iWText,
				&rtOutline,
				g_dwDrawFlags,
				g_colorFontDX[ 1 ]
			);
		}
	}

	// ???? ????
	m_pd3dxFont->DrawTextW
	(
		DxFontMan::g_pSprite,
		pwszTemp,
		iWText,
		&rectText,
		g_dwDrawFlags,
		dwColor
	);

	if( DxFontMan::g_pSprite )
		DxFontMan::g_pSprite->End();

	SAFE_DELETE_ARRAY( pwszTemp );

	if( bUsageCS )
		LeaveCriticalSection( &m_csLock );

	return S_OK;
}

VOID CD3DFontX::PushText( const TCHAR* strText )
{
	if( CTextUtil::m_bUsage )
		CTextUtil::Get()->PushText( strText, this );
}

VOID CD3DFontX::DrawText( const TCHAR* strText, DWORD dwFlags, RECT* rtBound, HDC hDC )
{
	if( !m_hFont || !hDC )
		return;

	if( !strText || !rtBound )
		return;

	INT iText = (INT)_tcslen( strText );
	if( !iText )
		return;

	// ?????? ?????? ???????? ?????? ?? ????.
	if( ( rtBound->right - rtBound->left <= 0 ) || ( rtBound->bottom - rtBound->top <= 0 ) )
		return;

	// ?????????? ???????? ?????? ?????? ???? ????.
	CString szTemp = strText;
	if( szTemp.Replace( (char)32, (char)105 ) == iText )
		return;

	BOOL bUsageCS = m_bUsageCS;
	if( bUsageCS )
		EnterCriticalSection( &m_csLock );

	WCHAR * pwszTemp = new WCHAR[ iText  ];
	memset( pwszTemp, 0, sizeof( WCHAR ) * ( iText  ) );
	INT iWText = MultiByteToWideChar( nCodePage[ m_emLangFlag ], 0, strText, iText, pwszTemp, iText );

	// ???? ?????? ???????? ?????? ?? ????.
	if( iWText < 1 )
	{
		SAFE_DELETE_ARRAY( pwszTemp );

		if( bUsageCS )
			LeaveCriticalSection( &m_csLock );

		return;
	}

	HFONT hFontOld = (HFONT)SelectObject( hDC, m_hFont );

	SetTextColor( hDC, g_colorFontDC[ 1 ] );

	if( m_dwFontFlags & D3DFONT_SHADOW )
	{
		for( LONG lX = -m_iOutLine; lX <= m_iOutLine; ++lX )
		{
			for( LONG lY = -m_iOutLine; lY <= m_iOutLine; ++lY )
			{
				if( !lX && !lY )
					continue;

				POINT ptTemp =
				{
					rtBound->left + lX,
					rtBound->top + lY
				};

				ExtTextOutW
				(
					hDC,
					ptTemp.x,
					ptTemp.y,
					ETO_OPAQUE,
					NULL,
					pwszTemp,
					iWText,
					NULL
				);
			}
		}
	}
	else if( m_dwFontFlags & D3DFONT_SHADOW_EX )
	{
		for( INT i=0; i<2; ++i )
		{
			POINT ptTemp =
			{
				rtBound->left + g_ptOffset[ i ].x,
				rtBound->top + g_ptOffset[ i ].y
			};

			ExtTextOutW
			(
				hDC,
				ptTemp.x,
				ptTemp.y,
				ETO_OPAQUE,
				NULL,
				pwszTemp,
				iWText,
				NULL
			);
		}
	}

	// ???? ????
	if( dwFlags & EM_DC_TEXT )
	{
		SetTextColor( hDC, g_colorFontDC[ 2 ] );
		ExtTextOutW
		(
			hDC,
			rtBound->left,
			rtBound->top,
			ETO_OPAQUE,
			NULL,
			pwszTemp,
			iWText,
			NULL
		);
	}

	SelectObject( hDC, hFontOld );

	SAFE_DELETE_ARRAY( pwszTemp );

	if( bUsageCS )
		LeaveCriticalSection( &m_csLock );
}

VOID CD3DFontX::CheckVietnam( const TCHAR* strText, TCHAR* strNewText )
{
	INT nCurrent = 0;
	TCHAR chBefore = '\0';

	INT size = _tcslen( strText );
	if( size < 1 )
		return;

	for ( INT i = 0; i < size; ++i )
	{
		if ( CheckCharSunjo( strText[ i ] ) && !CheckCharMoum( chBefore ) )
		{
//			CDebugSet::ToLogFile ( "sungjo Error %d %c %c %02x strText : %s\n", i, strText[i], chBefore, chBefore, strText );
			continue;
		}

		chBefore = strText[ i ];
		strNewText[ nCurrent ] = strText[ i ];
		++nCurrent;
	}
	strNewText[ nCurrent ] = _T('\0');

// ?????? ?????????? ???????? ?????? ?????? ???????? ???? ???? //
/*
	size = _tcslen( strNewText );

	for ( INT i = 0; i < size; ++i )
	{
		if ( strText[ i ] < 0 || strText[ i ] > 127 ) // ???? ?????? ?????? ????
		{
			if ( !CheckCharExAlphabet( strText[ i ] ) && !CheckCharSunjo( strText[ i ] ) ) // ????,?????? ?????? ????? ????
			{
				strNewText[ i ] = '*';
				CDebugSet::ToLogFile ( "Check Error %d, %d %02x %c strText : %s\n", i,strText[i], strText[i], strText[i], strText );
				CDebugSet::ToLogFile ( "strNewText : %s\n", strNewText );
			}
		}
	}
*/
}

BOOL CD3DFontX::CheckCharSunjo( TCHAR chText )
{
	for ( int i = 0; i < g_nSungjo; ++i )
	{
		if ( chText == g_szSungjo[ i ] )
			return true;
	}

	return false;
}

BOOL CD3DFontX::CheckCharExAlphabet( TCHAR chText )
{
	for ( int i = 0; i < g_nExAlphabet; ++i )
	{
		if ( chText == g_szExAlphabet[ i ] )
			return true;
	}

	return false;
}

BOOL CD3DFontX::CheckCharMoum( TCHAR chText )
{
	for ( int i = 0; i < g_nMoum; ++i )
	{
		if ( chText == g_szMoum[ i ] )
			return true;
	}

	return false;
}