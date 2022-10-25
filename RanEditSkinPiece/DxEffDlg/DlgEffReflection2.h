#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharReflection2.h"

class	CToolTab;
// CDlgEffReflection2 dialog

class CDlgEffReflection2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffReflection2)

public:
	CDlgEffReflection2( LOGFONT logfont );
	virtual ~CDlgEffReflection2();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_REFLECTION2 };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharReflection2	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharReflection2* pEff, BOOL bNEW );
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
	afx_msg void OnBnClickedButtonSpecular();
	afx_msg void OnBnClickedButtonSpecularOk();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTexture();
};
