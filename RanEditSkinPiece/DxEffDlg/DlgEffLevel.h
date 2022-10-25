#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharLevel.h"

class	CToolTab;
// CDlgEffLevel dialog

class CDlgEffLevel : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffLevel)

public:
	CDlgEffLevel( LOGFONT logfont );
	virtual ~CDlgEffLevel();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_LEVEL };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharLevel	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharLevel* pEff, BOOL bNEW );
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
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonSpecular();
	afx_msg void OnBnClickedButtonSpecularOk();
	afx_msg void OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult);
};
