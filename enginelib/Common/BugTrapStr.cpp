#include "stdafx.h"
#include "./BugTrapStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace BUG_TRAP
{
	TCHAR szEMail[MAX_PATH]			= _T("");
#if defined( RELEASE_A )
	TCHAR szCountry[MAX_PATH]		= _T("OR");
	TCHAR szServerAddr[MAX_PATH]	= _T("164587");
	TCHAR szSupportURL[MAX_PATH]	= _T("CNDEVelopments");
#elif defined( RELEASE_B )
	TCHAR szCountry[MAX_PATH]		= _T("VG");
	TCHAR szServerAddr[MAX_PATH]	= _T("287933");
	TCHAR szSupportURL[MAX_PATH]	= _T("CNDEVelopments");
#elif defined( RELEASE_C )
	TCHAR szCountry[MAX_PATH]		= _T("UN");
	TCHAR szServerAddr[MAX_PATH]	= _T("300389");
	TCHAR szSupportURL[MAX_PATH]	= _T("CNDEVelopments");
#elif defined( RELEASE_D )
	TCHAR szCountry[MAX_PATH]		= _T("TR");
	TCHAR szServerAddr[MAX_PATH]	= _T("423735");
	TCHAR szSupportURL[MAX_PATH]	= _T("CNDEVelopments");
#else
	TCHAR szCountry[MAX_PATH]		= _T("CR");
	TCHAR szServerAddr[MAX_PATH]	= _T("546181");
	TCHAR szSupportURL[MAX_PATH]	= _T("CNDEVelopments");
#endif		
	SHORT nPort						= 8888;
};
