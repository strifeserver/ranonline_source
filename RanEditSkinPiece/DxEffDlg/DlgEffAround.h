#pragma once

#include "DxSkinPieceContainer.h"
#include "DxCharAroundEff.h"

class	CToolTab;
// CDlgEffAround dialog

class CDlgEffAround : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffAround)

public:
	CDlgEffAround( LOGFONT logfont );
	virtual ~CDlgEffAround();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_AROUND };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxCharAroundEff	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxCharAroundEff* pEff, BOOL bNEW );
	void			ShowData();
	BOOL			SaveData();

public:
	void			UpdateList();

public:
	CListCtrl		m_list_POS;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDlgeffApply();
	afx_msg void OnBnClickedButtonDlgeffSave();
	afx_msg void OnBnClickedButtonDlgeffCancel();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonTex();
	afx_msg void OnNMDblclkListPos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGet();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonDel();
};
