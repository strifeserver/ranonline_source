#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharSpecular2.h"

class	CToolTab;
// CDlgEffSpecular2 dialog

class CDlgEffSpecular2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffSpecular2)

public:
	CDlgEffSpecular2( LOGFONT logfont );
	virtual ~CDlgEffSpecular2();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_SPECULAR2 };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharSpecular2	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharSpecular2* pEff, BOOL bNEW );
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
	afx_msg void OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSpecular();
	afx_msg void OnBnClickedButtonSpecularOk();
	afx_msg void OnBnClickedButtonColor();
};
