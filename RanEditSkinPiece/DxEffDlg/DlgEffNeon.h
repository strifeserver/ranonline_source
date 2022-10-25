#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharNeon.h"

class	CToolTab;
// CDlgEffNeon dialog

class CDlgEffNeon : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffNeon)

public:
	CDlgEffNeon( LOGFONT logfont );
	virtual ~CDlgEffNeon();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_NEON };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharNeon	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharNeon* pEff, BOOL bNEW );
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
	afx_msg void OnBnClickedCheckGlowDef();
	afx_msg void OnBnClickedCheckGlowBurn();
	afx_msg void OnBnClickedCheckTextureAll();
	afx_msg void OnBnClickedCheckTextureSelection();
	afx_msg void OnBnClickedCheckColorSource();
	afx_msg void OnBnClickedCheckColorSource2();
	afx_msg void OnBnClickedButtonText();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonSpecularOk();
	afx_msg void OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult);
};
