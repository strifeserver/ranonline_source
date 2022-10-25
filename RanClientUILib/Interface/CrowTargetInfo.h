#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "BasicLineBoxEx.h"

class	CD3DFontPar;
class	CBasicTextBox;
class	CBasicProgressBar;
class	CBasicTextBoxEx;

class	CCrowTargetInfo : public CUIGroup
{
protected:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	CCrowTargetInfo ();
	virtual	~CCrowTargetInfo ();
public:
	void	CreateSubControl ();
	CUIControl*	CreateControl ( const char* szControl, WORD wAlignFlag );
	CBasicLineBoxEx*  m_pTargetBox;
public:
	void	SetTargetInfo ( int fNow, int fMax, int fPercent,DWORD Level , CString strName );
	const	CString& GetTargetName () const			{ return m_strName; }
public:
	CString		m_strName;
private:
	CBasicTextBox*		m_pNameBox;
	CBasicTextBox*		m_pHPBox;
	CUIControl*			m_pNameBoxDummy;
private:
	CD3DFontPar*		m_pFont8;
};