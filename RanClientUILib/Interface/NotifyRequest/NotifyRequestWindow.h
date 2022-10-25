#pragma	once
#include "../UIWindowEx.h"
#include "GLCharDefine.h"

class CBasicTextBoxEx;
class CBasicScrollBarEx;
class CBasicTextBox;
class CNotifyRequestWindow : public CUIWindowEx
{
public:
	struct REQUEST_LIST_DATE_DATA
	{
		int			nYear;
		int			nMonth;
		int			nDay;

		REQUEST_LIST_DATE_DATA()
			: nYear(0)
			, nMonth(0)
			, nDay(0)
		{
		}

		void SET_DATE( int nY, int nM, int nD )
		{
			nYear = nY;
			nMonth = nM;
			nDay = nD;
		}
	};

	struct REQUEST_LIST
	{
		DWORD					dwGaeaID;
		WORD					wType;

		CString					strName;

		SPARTY_OPT				sPartyOption;
		DWORD					dwPartyID;
		DWORD					dwMasterID;

		DWORD					dwClubMasterID;
		CString					strClubName;
		CString					strClubMaster;

		REQUEST_LIST_DATE_DATA	reqDate;

		REQUEST_LIST()
			: dwGaeaID(0)
			, wType(-1)
			, strName("")
			, dwPartyID(0)
			, dwMasterID(0)
			, dwClubMasterID(0)
			, strClubName("")
			, strClubMaster("")
		{
		}
	};

protected:
	enum
	{
		REQUEST_LIST_TEXTBOX = ET_CONTROL_NEXT,
		REQUEST_LIST_SCROLLBAR
	};

public:
	CNotifyRequestWindow ();
	virtual ~CNotifyRequestWindow ();

public:
	void	CreateSubControl ();
	virtual void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CUIControl*			m_pBackGround;
	CBasicTextBoxEx*	m_pListText;
	CBasicScrollBarEx*	m_pListScrollBar;
	CBasicTextBox*		m_pHeaderText;

	typedef std::map<DWORD, REQUEST_LIST >	REQUEST_LIST_MAP;
	REQUEST_LIST_MAP mapReq;

public:
	void	LoadRequest();
	void	AddRequest( DWORD dwID, REQUEST_LIST rList ) { mapReq[dwID] = rList; }
	int		GetRequestSize() { return mapReq.size(); }

	void	RemoveAllPartyReq();
};