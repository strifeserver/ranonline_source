#pragma once

#include "./GLInventory.h"
#include "GLItemMan.h"
#include "GLItem.h"
#include "GLItemDef.h"
// CRanManagerCharInven dialog
class	CRanManagerCharTab;
class CRanManagerCharInven : public CPropertyPage
{
	DECLARE_DYNAMIC(CRanManagerCharInven)

public:
	CRanManagerCharInven(LOGFONT logfont);
	virtual ~CRanManagerCharInven();

// Dialog Data
	enum { IDD = IDD_RM_CHAR_INVEN };

protected:
	CRanManagerCharTab*		m_pToolTab;
	CFont*			m_pFont;
	int				m_CallPage;
	BOOL			m_bDlgInit;

public:
	GLInventory				m_cInventory;
	CComboBox		m_cGenType;
	CComboBox		m_cRandomOp1;
	CComboBox		m_cRandomOp2;
	CComboBox		m_cRandomOp3;
	CComboBox		m_cRandomOp4;
	int				nActiveItem_X;
	int				nActiveItem_Y;

public:
	void			SetToolTab		( CRanManagerCharTab *pToolTab ) { m_pToolTab = pToolTab; }
	void			SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void			SetChar (int nChar);
	void			ReadChar (int nChar);
	void			ReadInven (int nChar);
	void			ShowInven ();
	void			ViewItem(WORD X,WORD Y);
	void			SetInvenItemState ( CDialog *pDlg,int nID );
	void			ResetInvenItemState ( CDialog *pDlg );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonInven00();
	afx_msg void OnBnClickedButtonInven01();
	afx_msg void OnBnClickedButtonInven02();
	afx_msg void OnBnClickedButtonInven03();
	afx_msg void OnBnClickedButtonInven04();
	afx_msg void OnBnClickedButtonInven05();
	afx_msg void OnBnClickedButtonInven06();
	afx_msg void OnBnClickedButtonInven07();
	afx_msg void OnBnClickedButtonInven08();
	afx_msg void OnBnClickedButtonInven09();
	afx_msg void OnBnClickedButtonInven10();
	afx_msg void OnBnClickedButtonInven11();
	afx_msg void OnBnClickedButtonInven12();
	afx_msg void OnBnClickedButtonInven13();
	afx_msg void OnBnClickedButtonInven14();
	afx_msg void OnBnClickedButtonInven15();
	afx_msg void OnBnClickedButtonInven16();
	afx_msg void OnBnClickedButtonInven17();
	afx_msg void OnBnClickedButtonInven18();
	afx_msg void OnBnClickedButtonInven19();
	afx_msg void OnBnClickedButtonInven20();
	afx_msg void OnBnClickedButtonInven21();
	afx_msg void OnBnClickedButtonInven22();
	afx_msg void OnBnClickedButtonInven23();
	afx_msg void OnBnClickedButtonInven24();
	afx_msg void OnBnClickedButtonInven25();
	afx_msg void OnBnClickedButtonInven26();
	afx_msg void OnBnClickedButtonInven27();
	afx_msg void OnBnClickedButtonInven28();
	afx_msg void OnBnClickedButtonInven29();
	afx_msg void OnBnClickedButtonInven30();
	afx_msg void OnBnClickedButtonInven31();
	afx_msg void OnBnClickedButtonInven32();
	afx_msg void OnBnClickedButtonInven33();
	afx_msg void OnBnClickedButtonInven34();
	afx_msg void OnBnClickedButtonInven35();
	afx_msg void OnBnClickedButtonInven36();
	afx_msg void OnBnClickedButtonInven37();
	afx_msg void OnBnClickedButtonInven38();
	afx_msg void OnBnClickedButtonInven39();
	afx_msg void OnBnClickedButtonInven40();
	afx_msg void OnBnClickedButtonInven41();
	afx_msg void OnBnClickedButtonInven42();
	afx_msg void OnBnClickedButtonInven43();
	afx_msg void OnBnClickedButtonInven44();
	afx_msg void OnBnClickedButtonInven45();
	afx_msg void OnBnClickedButtonInven46();
	afx_msg void OnBnClickedButtonInven47();
	afx_msg void OnBnClickedButtonInven48();
	afx_msg void OnBnClickedButtonInven49();
	afx_msg void OnBnClickedButtonInven50();
	afx_msg void OnBnClickedButtonInven51();
	afx_msg void OnBnClickedButtonInven52();
	afx_msg void OnBnClickedButtonInven53();
	afx_msg void OnBnClickedButtonInven54();
	afx_msg void OnBnClickedButtonInven55();
	afx_msg void OnBnClickedButtonInven56();
	afx_msg void OnBnClickedButtonInven57();
	afx_msg void OnBnClickedButtonInven58();
	afx_msg void OnBnClickedButtonInven59();
	afx_msg void OnBnClickedButtonItemsearchM();
	afx_msg void OnBnClickedButtonItemsearchCos();
	afx_msg void OnBnClickedBtnEditItemDelete();
	afx_msg void OnBnClickedBtnEditItemUpdate();
	afx_msg void OnBnClickedButtonResetCosExpire();
	afx_msg void OnCbnSelchangeComboGentype();
	afx_msg void OnBnClickedButtonInvencancel();
	afx_msg void OnBnClickedButtonInvensave();
	afx_msg void OnBnClickedButtonInvenwipe();
};
