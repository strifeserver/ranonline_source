#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharBoneListEff.h"

class	CToolTab;
// CDlgEffBoneList dialog

class CDlgEffBoneList : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffBoneList)

public:
	CDlgEffBoneList( LOGFONT logfont );
	virtual ~CDlgEffBoneList();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_BONELIST };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharBoneListEff	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharBoneListEff* pEff, BOOL bNEW );
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
	afx_msg void OnBnClickedButtonTexture();
	afx_msg void OnNMDblclkListPos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonBoneAdd();
	afx_msg void OnBnClickedButtonBoneEdit();
	afx_msg void OnBnClickedButtonBoneUp();
	afx_msg void OnBnClickedButtonBoneDown();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonPosSave();
	afx_msg void OnBnClickedButtonScreenpos();
};
