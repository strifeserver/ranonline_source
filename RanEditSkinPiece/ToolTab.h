#ifndef __ToolTab_H_
#define __ToolTab_H_

/////////////////////////////////////////////////////////////////////////////
// CToolTab
#include "DlgMain.h"
#include "DxEffDlg/DlgEffSingle.h"
#include "DxEffDlg/DlgEffBlur.h"
#include "DxEffDlg/DlgEffCloneBlur.h"
#include "DxEffDlg/DlgEffDust.h"
#include "DxEffDlg/DlgEffShock.h"
#include "DxEffDlg/DlgEffAttribute.h"
#include "DxEffDlg/DlgEffArrow.h"
#include "DxEffDlg/DlgEffSpecular.h"
#include "DxEffDlg/DlgEffLevel.h"
#include "DxEffDlg/DlgEffEmit.h"
#include "DxEffDlg/DlgEffAlpha.h"
#include "DxEffDlg/DlgEffNeon.h"
#include "DxEffDlg/DlgEffMark.h"
#include "DxEffDlg/DlgEffNoAlpha.h"
#include "DxEffDlg/DlgEffReflection2.h"
#include "DxEffDlg/DlgEffAmbient.h"
#include "DxEffDlg/DlgEffDot3.h"
#include "DxEffDlg/DlgEffMultiTex.h"
#include "DxEffDlg/DlgEffGhosting.h"
#include "DxEffDlg/DlgEffSpecular2.h"
#include "DxEffDlg/DlgEffToon.h"
#include "DxEffDlg/DlgEffTexDiff.h"
#include "DxEffDlg/DlgEffParticle.h"
#include "DxEffDlg/DlgEffBonePos.h"
#include "DxEffDlg/DlgEffBoneList.h"
#include "DxEffDlg/DlgEffUserColor.h"
#include "DxEffDlg/DlgEffNormalMap.h"
#include "DxEffDlg/DlgEffLine2Bone.h"
#include "DxEffDlg/DlgEffAround.h"


enum EDITSHEET
{
	DLG_MAIN			= 0,
	DLG_EFF_SINGLE		= 1,
	DLG_EFF_BLUR		= 2,
	DLG_EFF_CLONEBLUR	= 3,
	DLG_EFF_DUST		= 4,
	DLG_EFF_SHOCK		= 5,
	DLG_EFF_ATTRIBUTE	= 6,
	DLG_EFF_ARROW		= 7,
	DLG_EFF_SPECULAR	= 8,
	DLG_EFF_LEVEL		= 9,
	DLG_EFF_EMIT		= 10,
	DLG_EFF_ALPHA		= 11,
	DLG_EFF_NEON		= 12,
	DLG_EFF_MARK		= 13,
	DLG_EFF_NOALPHA		= 14,
	DLG_EFF_REFLECTION2	= 15,
	DLG_EFF_AMBIENT		= 16,
	DLG_EFF_DOT3		= 17,
	DLG_EFF_MULTITEX	= 18,
	DLG_EFF_GHOSTING	= 19,
	DLG_EFF_SPECULAR2	= 20,
	DLG_EFF_TOON		= 21,
	DLG_EFF_TEXDIFF		= 22,
	DLG_EFF_PARTICLE	= 23,
	DLG_EFF_BONEPOSEFF	= 24,
	DLG_EFF_BONELISTEFF	= 25,
	DLG_EFF_USERCOLOR	= 26,
	DLG_EFF_NORMALMAP	= 27,
	DLG_EFF_LINE2BONE	= 28,
	DLG_EFF_AROUNDEFFECT	= 29,

	DLG_SIZE		= 30,
};

enum { CDF_CENTER, CDF_TOPLEFT, CDF_NONE };

class CToolTab : public CPropertySheet
{
	DECLARE_DYNAMIC(CToolTab)

public:
	CWnd*		m_pWndParent;

public:
	CDlgMain				m_DlgMain;
	CDlgEffSingle			m_DlgEffSingle;
	CDlgEffBlur				m_DlgEffBlur;
	CDlgEffCloneBlur		m_DlgEffCloneBlur;
	CDlgEffDust				m_DlgEffDust;
	CDlgEffShock			m_DlgEffShock;
	CDlgEffAttribute		m_DlgEffAttribute;
	CDlgEffArrow			m_DlgEffArrow;
	CDlgEffSpecular			m_DlgEffSpecular;
	CDlgEffLevel			m_DlgEffLevel;
	CDlgEffEmit				m_DlgEffEmit;
	CDlgEffAlpha			m_DlgEffAlpha;
	CDlgEffNeon				m_DlgEffNeon;
	CDlgEffMark				m_DlgEffMark;
	CDlgEffNoAlpha			m_DlgEffNoAlpha;
	CDlgEffReflection2		m_DlgEffReflection2;
	CDlgEffAmbient			m_DlgEffAmbient;
	CDlgEffDot3				m_DlgEffDot3;
	CDlgEffMultiTex			m_DlgEffMultiTex;
	CDlgEffGhosting			m_DlgEffGhosting;
	CDlgEffSpecular2		m_DlgEffSpecular2;
	CDlgEffToon				m_DlgEffToon;
	CDlgEffTexDiff			m_DlgEffTexDiff;
	CDlgEffParticle			m_DlgEffParticle;
	CDlgEffBonePos			m_DlgEffBonePos;
	CDlgEffBoneList			m_DlgEffBoneList;
	CDlgEffUserColor		m_DlgEffUserColor;
	CDlgEffNormalMap		m_DlgEffNormalMap;
	CDlgEffLine2Bone		m_DlgEffLine2Bone;
	CDlgEffAround			m_DlgEffAround;


	void	ClearData();

public:	
	void	ActiveDlgPage( int nPage );
	void	ActiveMainPage();
	void	EffCharEdit( DxSkinPiece* pData, DxEffChar* pEff, BOOL bNEW = FALSE );

// Construction
public:
	CToolTab(CWnd* pParentWnd = NULL);
	CToolTab(CRect& rect, LOGFONT logfont, CWnd* pParentWnd = NULL );
	virtual ~CToolTab();

// Attributes
public:
	CRect		m_Rect;
	BOOL		m_bNoTabs;
	int			m_iDistanceFromTop;
	CFont*		m_pFont;


// Get/Set
public:
   void NoTabs() { m_bNoTabs = TRUE; }
   void SetDistanceFromTop( int iDistance ) { m_iDistanceFromTop = iDistance; }
   void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTab)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolTab)
   afx_msg HBRUSH CtlColor(CDC* pDc, UINT uCtlColor);	
   //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __ToolTab_H_