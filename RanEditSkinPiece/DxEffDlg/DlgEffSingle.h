#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharSingle.h"

class	CToolTab;
// CDlgEffSingle dialog

class CDlgEffSingle : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffSingle)

public:
	CDlgEffSingle( LOGFONT logfont );
	virtual ~CDlgEffSingle();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_SINGLE };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharSingle *m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharSingle* pEff, BOOL bNEW );
	void			ShowData();
	BOOL			SaveData();

public:
	CListCtrl		m_list_Dot1;
	CListCtrl		m_list_Dot2;
	D3DXMATRIX		matEff;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1dot();
	afx_msg void OnBnClickedCheck2dots();
	afx_msg void OnBnClickedCheckMatrix();
	afx_msg void OnBnClickedButtonFilename();
	afx_msg void OnBnClickedButtonDlgeffApply();
	afx_msg void OnNMDblclkListDot1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListDot2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDlgeffSave();
	afx_msg void OnBnClickedButtonDlgeffCancel();
};
