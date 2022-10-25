#include "stdafx.h"
#include "GLUpgradeColor.h"
#include "./GLogicData.h"

#include "../../EngineLib/Common/GLTexFile.h"
#include "../../EngineLib/G-Logic/GLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLUpgradeColor::GLUpgradeColor()
{
	for ( int i=0; i<COLOR_ARRAY; ++i )
	{
		G_A[i] = 0;
		G_R[i] = 0;
		G_G[i] = 0;
		G_B[i] = 0;

		A_A[i] = 0;
		A_R[i] = 0;
		A_G[i] = 0;
		A_B[i] = 0;

		R_A[i] = 0;
		R_R[i] = 0;
		R_G[i] = 0;
		R_B[i] = 0;
		bEnable[i] = true;
	}
};
GLUpgradeColor::~GLUpgradeColor()
{
};

GLUpgradeColor& GLUpgradeColor::GetInstance ()
{
	static GLUpgradeColor Instance;
	return Instance;
}

BOOL GLUpgradeColor::LoadUpgradeColor()
{
	std::string strPath;
	strPath = GLOGIC::GetPath();
	strPath += "UpgradeColor.ini";

	gltexfile cFILE;
	cFILE.reg_sep ( '\t' );
	cFILE.reg_sep ( ' ' );
	cFILE.reg_sep ( ',' );
	cFILE.reg_sep ( '{' );
	cFILE.reg_sep ( '}' );
	cFILE.reg_sep ( '[' );
	cFILE.reg_sep ( ']' );
	cFILE.reg_sep ( '(' );
	cFILE.reg_sep ( ')' );
	cFILE.reg_sep ( '|' );
	cFILE.reg_sep ( '\"' );

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	if( !cFILE.open( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ToLogFile ( "ERROR Cannot Load Upgrade Color Set File");
		return FALSE;
	}

	for ( int i=6; i<COLOR_ARRAY; ++i )
	{
		CString strTempG = "";
		CString strTempA = "";
		CString strTempR = "";
		CString strTempEnable = "";
		strTempG.Format("G%02d",i+4);
		strTempA.Format("A%02d",i+4);
		strTempR.Format("R%02d",i+4);
		strTempEnable.Format("bEnable%02d",i+4);

		cFILE.getflag( strTempG.GetString(), 1, 4, G_A[i] );	
		cFILE.getflag( strTempG.GetString(), 2, 4, G_R[i] );	
		cFILE.getflag( strTempG.GetString(), 3, 4, G_G[i] );	
		cFILE.getflag( strTempG.GetString(), 4, 4, G_B[i] );	
		cFILE.getflag( strTempA.GetString(), 1, 4, A_A[i] );	
		cFILE.getflag( strTempA.GetString(), 2, 4, A_R[i] );	
		cFILE.getflag( strTempA.GetString(), 3, 4, A_G[i] );	
		cFILE.getflag( strTempA.GetString(), 4, 4, A_B[i] );
		cFILE.getflag( strTempR.GetString(), 1, 4, R_A[i] );	
		cFILE.getflag( strTempR.GetString(), 2, 4, R_R[i] );	
		cFILE.getflag( strTempR.GetString(), 3, 4, R_G[i] );	
		cFILE.getflag( strTempR.GetString(), 4, 4, R_B[i] );
		cFILE.getflag( strTempEnable.GetString(), 1, 1, bEnable[i] );
	}
	return TRUE;	
};

