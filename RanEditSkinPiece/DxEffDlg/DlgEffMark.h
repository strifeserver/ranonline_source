#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharMark.h"

class	CToolTab;
// CDlgEffMark dialog

class CDlgEffMark : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffMark)

public:
	CDlgEffMark( LOGFONT logfont );
	virtual ~CDlgEffMark();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_MARK };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharMark	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharMark* pEff, BOOL bNEW );
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
	afx_msg void OnBnClickedButtonTex1();
	afx_msg void OnBnClickedButtonTex2();
	afx_msg void OnBnClickedButtonColor1();
	afx_msg void OnBnClickedButtonColor2();
	afx_msg void OnBnClickedCheckUse1();
	afx_msg void OnBnClickedCheckUse2();
};
