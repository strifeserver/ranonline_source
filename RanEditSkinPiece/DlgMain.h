#pragma once

#include "DxSkinPieceContainer.h"

class	CToolTab;
// CDlgMain dialog

class CDlgMain : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgMain)

public:
	CDlgMain( LOGFONT logfont );
	virtual ~CDlgMain();

// Dialog Data
	enum { IDD = IDD_DLG_MAIN };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	int				m_nMaterialSelected;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece*	pData );
	void			ShowData();
	BOOL			SaveData();

public:
	CListCtrl		m_list_Strace;
	CListCtrl		m_list_CharEff;
	CListCtrl		m_list_Mesh;
	CListCtrl		m_list_Material;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFindskin();
	afx_msg void OnBnClickedButtonFindskeleton();
	afx_msg void OnBnClickedButtonSetskinskeleton();
	afx_msg void OnBnClickedButtonSetmesh();
	afx_msg void OnBnClickedButtonTraceAdd();
	afx_msg void OnBnClickedButtonTraceDel();
	afx_msg void OnBnClickedButtonEffAdd();
	afx_msg void OnBnClickedButtonEffDel();
	afx_msg void OnBnClickedButtonEffEdit();
	afx_msg void OnNMClickListMaterial(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMaterialSize();
	afx_msg void OnNMClickListTrace(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListTrace(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonEffOk();
	afx_msg void OnCbnSelchangeComboPieceType();
	afx_msg void OnCbnSelchangeComboWeaponwhereback();
};
