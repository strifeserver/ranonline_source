#pragma once

namespace	NLOADINGTHREAD
{
	extern	LPDIRECT3DDEVICEQ*	n_ppd3dDevice;
	extern	HWND				n_hWnd;
	extern	BOOL				n_bWait;
	extern	BOOL				n_bRender;
	extern	char				n_szAppPath[MAX_PATH];
	extern	int					n_ExitState;
	extern	bool				n_bOVER15;
	extern	CString				n_strTextureName;
	extern	CString				n_strMapName;

	enum
	{
		eNOSTART,
		eNORMAL,
		eERROR
	};

	HRESULT	StartThreadLOAD (	LPDIRECT3DDEVICEQ* ppd3dDevice, 
								HWND hWnd, 
								char* szAppPath, 
								const CString & strTextureName,
								const CString & strMapName,
								bool bOVER15 = false );
	void	WaitThread ();
	void	EndThread ();

	BOOL	GetWait ();
	void	DeWait ();
	void	EnWait ();

	void	SetStep ( int step );
	int		GetStep ();

	void	LoadTip( char* szAppPath);
};