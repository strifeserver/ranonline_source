#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharAmbient.h"

class	CToolTab;
// CDlgEffAmbient dialog

class CDlgEffAmbient : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffAmbient)

public:
	CDlgEffAmbient( LOGFONT logfont );
	virtual ~CDlgEffAmbient();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_AMBIENT };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharAmbient	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharAmbient* pEff, BOOL bNEW );
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
	afx_msg void OnBnClickedButtonColor();
};
