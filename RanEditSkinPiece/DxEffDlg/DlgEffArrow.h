#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharArrow.h"

class	CToolTab;
// CDlgEffArrow dialog

class CDlgEffArrow : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffArrow)

public:
	CDlgEffArrow( LOGFONT logfont );
	virtual ~CDlgEffArrow();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_ARROW };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharArrow	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharArrow* pEff, BOOL bNEW );
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
	afx_msg void OnBnClickedButtonEff1();
	afx_msg void OnBnClickedButtonEff2();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonMesh();
};
