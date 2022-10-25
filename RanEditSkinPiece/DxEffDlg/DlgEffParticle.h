#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharParticle.h"

class	CToolTab;
// CDlgEffParticle dialog

class CDlgEffParticle : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffParticle)

public:
	CDlgEffParticle( LOGFONT logfont );
	virtual ~CDlgEffParticle();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_PARTICLE };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharParticle	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharParticle* pEff, BOOL bNEW );
	void			ShowData();
	BOOL			SaveData();

public:
	void			UpdatePositionList();

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
	afx_msg void OnBnClickedButtonTexture();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonPosAdd();
	afx_msg void OnBnClickedButtonPosDel();
	afx_msg void OnBnClickedButtonPosUp();
	afx_msg void OnBnClickedButtonPosDown();
	afx_msg void OnNMDblclkListPos(NMHDR *pNMHDR, LRESULT *pResult);
};
