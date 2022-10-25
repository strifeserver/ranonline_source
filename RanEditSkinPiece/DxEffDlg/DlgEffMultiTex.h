#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharMultiTex.h"

class	CToolTab;
// CDlgEffMultiTex dialog

class CDlgEffMultiTex : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffMultiTex)

public:
	CDlgEffMultiTex( LOGFONT logfont );
	virtual ~CDlgEffMultiTex();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_MULTITEX };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharMultiTex	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharMultiTex* pEff, BOOL bNEW );
	void			ShowData();
	BOOL			SaveData();

public:
	void			ShowMaterialList();

public:
	CListCtrl		m_list_Material;
	int				m_nMatEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDlgeffApply();
	afx_msg void OnBnClickedButtonDlgeffSave();
	afx_msg void OnBnClickedButtonDlgeffCancel();
	afx_msg void OnBnClickedButtonSpecularOk();
	afx_msg void OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckAllTex();
	afx_msg void OnBnClickedCheckListTex();
	afx_msg void OnBnClickedCheckUseTex1();
	afx_msg void OnBnClickedCheckUseTex2();
	afx_msg void OnBnClickedButtonTex1();
	afx_msg void OnBnClickedButtonTex2();
	afx_msg void OnBnClickedButtonTexmap();
	afx_msg void OnBnClickedButtonColor1();
	afx_msg void OnBnClickedButtonColor2();
};
