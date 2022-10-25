// RanEditSkinPiece.h : main header file for the RanEditSkinPiece application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CRanEditSkinPieceApp:
// See RanEditSkinPiece.cpp for the implementation of this class
//

class CRanEditSkinPieceApp : public CWinApp
{
public:
	CRanEditSkinPieceApp();

public:
	BOOL		m_bIsActive;
	CString		m_strAppPath;
	char		m_szAppPath[MAX_PATH];

public:
	void		SetActive(BOOL bActive) { m_bIsActive = bActive; };


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int Run();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRanEditSkinPieceApp theApp;