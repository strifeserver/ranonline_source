#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharToon.h"

class	CToolTab;
// CDlgEffToon dialog

class CDlgEffToon : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffToon)

public:
	CDlgEffToon( LOGFONT logfont );
	virtual ~CDlgEffToon();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_TOON };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharToon	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharToon* pEff, BOOL bNEW );
	void			ShowData();
	BOOL			SaveData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDlgeffApply();
	afx_msg void OnBnClickedButtonDlgeffSave();
	afx_msg void OnBnClickedButtonDlgeffCancel();
	afx_msg void OnBnClickedButtonTexture();
	afx_msg void OnBnClickedButtonObli();
};
