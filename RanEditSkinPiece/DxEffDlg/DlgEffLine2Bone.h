#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharLine2BoneEff.h"

class	CToolTab;
// CDlgEffLine2Bone dialog

class CDlgEffLine2Bone : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffLine2Bone)

public:
	CDlgEffLine2Bone( LOGFONT logfont );
	virtual ~CDlgEffLine2Bone();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_LINE2BONE };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharLine2BoneEff	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharLine2BoneEff* pEff, BOOL bNEW );
	void			ShowData();
	BOOL			SaveData();

public:
	void			UpdateBoneList();

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
	afx_msg void OnBnClickedButtonColor1();
	afx_msg void OnBnClickedButtonColor2();
	afx_msg void OnBnClickedButtonTex1();
	afx_msg void OnBnClickedButtonTex2();
	afx_msg void OnBnClickedButtonBone1();
	afx_msg void OnBnClickedButtonBone2();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnNMDblclkListPos(NMHDR *pNMHDR, LRESULT *pResult);
};
