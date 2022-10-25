#pragma	once

#include "UIWindowEx.h"

class CBasicProgressBar;
class CBasicTextBox;
class CBasicTextButton;
class CUIEditBox;

class CBikeColorCard : public CUIWindowEx
{
	static const int nLIMIT_NUMBER_CHAR;
	static const int nMAX_COLOR;
	static const float fRGB_RATIO;

	enum
	{
		BIKECOLORCARD_R_PROGRESS = ET_CONTROL_NEXT,
		BIKECOLORCARD_G_PROGRESS,
		BIKECOLORCARD_B_PROGRESS,

		BIKECOLORCARD_R_EDITBOX,
		BIKECOLORCARD_G_EDITBOX,
		BIKECOLORCARD_B_EDITBOX,

		BIKECOLORCARD_OK,
		BIKECOLORCARD_CANCEL,
		BIKECOLOR_PART_SEL0,
		BIKECOLOR_PART_SEL1,
		BIKECOLOR_PART_SEL2,
		BIKECOLOR_PART_SEL3,
		BIKECOLOR_PART_SEL4,
		BIKECOLOR_PART_SEL5,
	};

public:
	CBikeColorCard ();
	virtual	~CBikeColorCard ();

public:
	void CreateSubControl();

	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle( BOOL bVisible );

private:
	CBasicTextBox * CreateStaticControl( char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	BOOL UpdateProgressBar( CBasicProgressBar * pBar, int x );
	void UpdateProgressBarToEditBox( CBasicProgressBar * pBar, CUIEditBox * pEditBox, INT & nColorVal );
	BOOL UpdateEditBoxToProgressBar( CUIEditBox * pEditBox, CBasicProgressBar * pBar, INT & nColorVal );

	void TranslateProgressBarMsg( CBasicProgressBar * pBar, DWORD dwMsg );
	void TranslateEditBoxMsg( CUIEditBox * pEditBox, DWORD dwMsg );

	void InitUIControl();
	void InitUIControlDummy();
	void ClearEditBox();
	void RegulateRGB();

private:
	CBasicProgressBar * m_pRProgressBar;
	CBasicProgressBar * m_pGProgressBar;
	CBasicProgressBar *	m_pBProgressBar;

	CUIEditBox * m_pREditBox;
	CUIEditBox * m_pGEditBox;
	CUIEditBox * m_pBEditBox;

	CUIControl * m_RGBControl;
	CUIControl * m_RGBControl1;
	CUIControl * m_RGBControl2;
	CUIControl * m_RGBControl3;
	CUIControl * m_RGBControl4;
	CUIControl * m_RGBControl5;

	INT m_nRDummy;
	INT m_nGDummy;
	INT m_nBDummy;

	INT m_nR;
	INT m_nG;
	INT m_nB;
	INT m_nR1;
	INT m_nG1;
	INT m_nB1;
	INT m_nR2;
	INT m_nG2;
	INT m_nB2;
	INT m_nR3;
	INT m_nG3;
	INT m_nB3;
	INT m_nR4;
	INT m_nG4;
	INT m_nB4;
	INT m_nR5;
	INT m_nG5;
	INT m_nB5;

	INT m_nRBack;
	INT m_nGBack;
	INT m_nBBack;
	INT m_nRBack1;
	INT m_nGBack1;
	INT m_nBBack1;
	INT m_nRBack2;
	INT m_nGBack2;
	INT m_nBBack2;
	INT m_nRBack3;
	INT m_nGBack3;
	INT m_nBBack3;
	INT m_nRBack4;
	INT m_nGBack4;
	INT m_nBBack4;
	INT m_nRBack5;
	INT m_nGBack5;
	INT m_nBBack5;

	INT m_nRegR;
	INT m_nRegG;
	INT m_nRegB;
	INT m_nRegR1;
	INT m_nRegG1;
	INT m_nRegB1;
	INT m_nRegR2;
	INT m_nRegG2;
	INT m_nRegB2;
	INT m_nRegR3;
	INT m_nRegG3;
	INT m_nRegB3;
	INT m_nRegR4;
	INT m_nRegG4;
	INT m_nRegB4;
	INT m_nRegR5;
	INT m_nRegG5;
	INT m_nRegB5;

	BOOL m_bOKButton;

	int nPartSelect;
};