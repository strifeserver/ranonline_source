#include "stdafx.h"
#include "Crypt.h"
#include "SUBPATH.h"
#include "GLOGIC.h"
#include "../Common/IniLoader.h"
#include "../Common/GLTexFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace CCrypt
{
	BOOL	bCryptRCC = TRUE;
	BOOL	bEngineDebug = TRUE;
	BOOL	bEngineDebugLog = TRUE;

	void  LoadSetting( const TCHAR *szRootPath )
	{
		CString strFileName = szRootPath;
		strFileName += "\\GameSet.ini";

		CIniLoader cFILE;

		DWORD	dwKey = 0;
			
		if( cFILE.open( strFileName.GetString(), true) )
		{
			cFILE.SetUseErrorMsgBox( FALSE );

			cFILE.getflag ( "GAME_SET", "dwCryptKey", 0, 1, dwKey );
			cFILE.getflag ( "GAME_SET", "bEngineDebug", 0, 1, bEngineDebug );
			cFILE.getflag ( "GAME_SET", "bEngineDebugLog", 0, 1, bEngineDebugLog );
		}
		else
		{
			CDebugSet::ToLogFile("CCrypt::LoadSetting Cant Load Setting File");
		}
#if defined( RELEASE_A )
		if ( dwKey == 31311 )	bCryptRCC = FALSE;
#elif defined( RELEASE_B )
		if ( dwKey == 23858 )	bCryptRCC = FALSE;
#elif defined( RELEASE_C )
		if ( dwKey == 35193 )	bCryptRCC = FALSE;
#elif defined( RELEASE_D )
		if ( dwKey == 47448 )	bCryptRCC = FALSE;
#else
		if ( dwKey == 987654 )	bCryptRCC = FALSE;
#endif
	}

	void Encryption(BYTE *B ,int dwSize)
	{
		for( int i = 0 ; i < dwSize ; i ++ )
		{
			B[i] = B[i] ^ EN;
			B[i] += ( EN ^ EN2 );
		};
		return ;
	}
	void Decryption(BYTE *B,int dwSize)
	{
		for( int i = 0 ; i < dwSize ; i ++ )
		{
			B[i] -= ( EN ^ EN2 );
			B[i] = B[i] ^ EN;
		};
		return ;
	}
};
