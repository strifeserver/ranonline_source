#pragma once

#include "DxSkinPieceContainer.h"
#include "DxEffCharBonePosEff.h"

class	CToolTab;
// CDlgEffBonePos dialog

class CDlgEffBonePos : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgEffBonePos)

public:
	CDlgEffBonePos( LOGFONT logfont );
	virtual ~CDlgEffBonePos();

// Dialog Data
	enum { IDD = IDD_DLG_EFF_BONEPOS };

protected:
	CToolTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

	DxSkinPiece		*m_pPiece;
	DxEffCharBonePosEff	*m_pEff;
	BOOL			bNEWEFF;

public:
	void			SetToolTab		( CToolTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetData( DxSkinPiece* pData, DxEffCharBonePosEff* pEff, BOOL bNEW );
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
	afx_msg void OnBnClickedButtonBoneSelect();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonTexture();
	afx_msg void OnBnClickedButtonGetFromScreen();
};
