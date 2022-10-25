#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharDot3.h"

class	CToolTab;
// CDlgEffDot3 dialog

class CDlgEffDot3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffDot3)

public:
	CDlgEffDot3( LOGFONT logfont );
	virtual ~CDlgEffDot3();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_DOT3 };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharDot3	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharDot3* pEff, BOOL bNEW );
	void			ShowData();
	BOOL			SaveData();

public:
	void			AddTexture( int nID );


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDlgeffApply();
	afx_msg void OnBnClickedButtonDlgeffSave();
	afx_msg void OnBnClickedButtonDlgeffCancel();
	afx_msg void OnBnClickedButtonTexture();
	afx_msg void OnBnClickedButtonSrcTex1();
	afx_msg void OnBnClickedButtonSrcTex2();
	afx_msg void OnBnClickedButtonSrcTex3();
	afx_msg void OnBnClickedButtonTexture1();
	afx_msg void OnBnClickedButtonTexture2();
	afx_msg void OnBnClickedButtonTexture3();
	afx_msg void OnBnClickedButtonDotTexture1();
	afx_msg void OnBnClickedButtonDotTexture2();
	afx_msg void OnBnClickedButtonDotTexture3();
};
