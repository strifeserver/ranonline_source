#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharBlur.h"

class	CToolTab;
// CDlgEffBlur dialog

class CDlgEffBlur : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffBlur)

public:
	CDlgEffBlur( LOGFONT logfont );
	virtual ~CDlgEffBlur();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_BLUR };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharBlur	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharBlur* pEff, BOOL bNEW );
	void			ShowData();
	BOOL			SaveData();

public:
	CListCtrl		m_list_Dot1;
	CListCtrl		m_list_Dot2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDlgeffApply();
	afx_msg void OnBnClickedButtonDlgeffSave();
	afx_msg void OnBnClickedButtonDlgeffCancel();
	afx_msg void OnBnClickedButtonColor1();
	afx_msg void OnBnClickedButtonColor2();
	afx_msg void OnNMDblclkListDot1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListDot2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFilename();
};
