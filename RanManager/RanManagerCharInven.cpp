// RanManagerCharInven.cpp : implementation file
//

#include "stdafx.h"
#include "RanManager.h"
#include "RanManagerCharInven.h"
#include "RanManagerCharTab.h"

#include "COdbc.h"
#include "CCfg.h"
#include "s_NetGlobal.h"
#include "Calendar.h"
#include "LogScript.h"
#include "ItemSearch.h"
#include "EtcFunction.h"
#include "RanManagerComments.h"
#include "../NetServerLib/s_DbBase.h"
#include "../EngineLib/Common/ByteStream.h"
#include "GLDefine.h"
#include ".\ranmanagercharinven.h"

// CRanManagerCharInven dialog

IMPLEMENT_DYNAMIC(CRanManagerCharInven, CPropertyPage)
CRanManagerCharInven::CRanManagerCharInven(LOGFONT logfont)
	: CPropertyPage(CRanManagerCharInven::IDD)
	,m_pFont(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
	nActiveItem_X = 65535;
	nActiveItem_Y = 65535;
}

CRanManagerCharInven::~CRanManagerCharInven()
{
}

void CRanManagerCharInven::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GENTYPE, m_cGenType );
	DDX_Control(pDX, IDC_COMBO_OPTION1, m_cRandomOp1 );
	DDX_Control(pDX, IDC_COMBO_OPTION2, m_cRandomOp2 );
	DDX_Control(pDX, IDC_COMBO_OPTION3, m_cRandomOp3 );
	DDX_Control(pDX, IDC_COMBO_OPTION4, m_cRandomOp4 );
}


BEGIN_MESSAGE_MAP(CRanManagerCharInven, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_0, OnBnClickedButtonInven00)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_1, OnBnClickedButtonInven01)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_2, OnBnClickedButtonInven02)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_3, OnBnClickedButtonInven03)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_4, OnBnClickedButtonInven04)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_5, OnBnClickedButtonInven05)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_6, OnBnClickedButtonInven06)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_7, OnBnClickedButtonInven07)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_8, OnBnClickedButtonInven08)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_0_9, OnBnClickedButtonInven09)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_0, OnBnClickedButtonInven10)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_1, OnBnClickedButtonInven11)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_2, OnBnClickedButtonInven12)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_3, OnBnClickedButtonInven13)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_4, OnBnClickedButtonInven14)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_5, OnBnClickedButtonInven15)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_6, OnBnClickedButtonInven16)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_7, OnBnClickedButtonInven17)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_8, OnBnClickedButtonInven18)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_1_9, OnBnClickedButtonInven19)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_0, OnBnClickedButtonInven20)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_1, OnBnClickedButtonInven21)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_2, OnBnClickedButtonInven22)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_3, OnBnClickedButtonInven23)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_4, OnBnClickedButtonInven24)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_5, OnBnClickedButtonInven25)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_6, OnBnClickedButtonInven26)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_7, OnBnClickedButtonInven27)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_8, OnBnClickedButtonInven28)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_2_9, OnBnClickedButtonInven29)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_0, OnBnClickedButtonInven30)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_1, OnBnClickedButtonInven31)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_2, OnBnClickedButtonInven32)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_3, OnBnClickedButtonInven33)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_4, OnBnClickedButtonInven34)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_5, OnBnClickedButtonInven35)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_6, OnBnClickedButtonInven36)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_7, OnBnClickedButtonInven37)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_8, OnBnClickedButtonInven38)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_3_9, OnBnClickedButtonInven39)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_0, OnBnClickedButtonInven40)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_1, OnBnClickedButtonInven41)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_2, OnBnClickedButtonInven42)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_3, OnBnClickedButtonInven43)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_4, OnBnClickedButtonInven44)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_5, OnBnClickedButtonInven45)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_6, OnBnClickedButtonInven46)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_7, OnBnClickedButtonInven47)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_8, OnBnClickedButtonInven48)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_4_9, OnBnClickedButtonInven49)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_0, OnBnClickedButtonInven50)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_1, OnBnClickedButtonInven51)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_2, OnBnClickedButtonInven52)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_3, OnBnClickedButtonInven53)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_4, OnBnClickedButtonInven54)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_5, OnBnClickedButtonInven55)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_6, OnBnClickedButtonInven56)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_7, OnBnClickedButtonInven57)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_8, OnBnClickedButtonInven58)
	ON_BN_CLICKED(IDC_BUTTON_INVEN_5_9, OnBnClickedButtonInven59)
	ON_BN_CLICKED(IDC_BUTTON_ITEMSEARCH_M, OnBnClickedButtonItemsearchM)
	ON_BN_CLICKED(IDC_BUTTON_ITEMSEARCH_COS, OnBnClickedButtonItemsearchCos)
	ON_BN_CLICKED(IDC_BTN_EDIT_ITEM_DELETE, OnBnClickedBtnEditItemDelete)
	ON_BN_CLICKED(IDC_BTN_EDIT_ITEM_UPDATE, OnBnClickedBtnEditItemUpdate)
	ON_BN_CLICKED(IDC_BUTTON_RESET_COS_EXPIRE, OnBnClickedButtonResetCosExpire)
	ON_CBN_SELCHANGE(IDC_COMBO_GENTYPE, OnCbnSelchangeComboGentype)
	ON_BN_CLICKED(IDC_BUTTON_INVENCANCEL, OnBnClickedButtonInvencancel)
	ON_BN_CLICKED(IDC_BUTTON_INVENSAVE, OnBnClickedButtonInvensave)
	ON_BN_CLICKED(IDC_BUTTON_INVENWIPE, OnBnClickedButtonInvenwipe)
END_MESSAGE_MAP()


// CRanManagerCharInven message handlers
BOOL CRanManagerCharInven::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_bDlgInit = TRUE;

	for ( int i = 0; i < GEN_SIZE; ++i )
	{
		m_cGenType.InsertString( i, RMComments::m_GenType[i].c_str() );
	}
	m_cGenType.SetCurSel( 0 );

	for ( int i = 0; i < EMR_OPT_SIZE; ++i )
	{
		m_cRandomOp1.InsertString( i, COMMENT::ITEM_RANDOM_OPT[i].c_str() );
		m_cRandomOp2.InsertString( i, COMMENT::ITEM_RANDOM_OPT[i].c_str() );
		m_cRandomOp3.InsertString( i, COMMENT::ITEM_RANDOM_OPT[i].c_str() );
		m_cRandomOp4.InsertString( i, COMMENT::ITEM_RANDOM_OPT[i].c_str() );
	}

	m_cRandomOp1.SetCurSel( 0 );
	m_cRandomOp2.SetCurSel( 0 );
	m_cRandomOp3.SetCurSel( 0 );
	m_cRandomOp4.SetCurSel( 0 );

	SetWin_Enable(this,IDC_BTN_EDIT_ITEM_DELETE,false);
	SetWin_Enable(this,IDC_BTN_EDIT_ITEM_UPDATE,false);

	return TRUE;  // return TRUE unless you set the focus to a control	
}

void CRanManagerCharInven::SetChar(int nChar)
{
	ReadChar (nChar);
	ReadInven (nChar);
}

void CRanManagerCharInven::ReadChar (int nChar)
{
	SQLRETURN	sReturn = 0; // SQL return value (short)	
	SQLHSTMT	hStmt; // Handle of statement (void*)

	sReturn = ::SQLAllocHandle(SQL_HANDLE_STMT, COdbc::GetInstance()->ConnectGame(), &hStmt);
	if (sReturn != SQL_SUCCESS && sReturn != SQL_SUCCESS_WITH_INFO)	return;	

	TCHAR szTemp[1024] = {0};
	_snprintf( szTemp, 1024, "SELECT ChaNum,ChaName FROM %s.dbo.ChaInfo WHERE ChaNum=%d",CToolCfg::GetInstance()->szGameDb,nChar );

	sReturn = ::SQLExecDirect(hStmt,(SQLCHAR*) szTemp,SQL_NTS);

	std::string strTIME = CDebugSet::GetCurTime ();
	LOGSCRIPT::LogToFile("CharInven.txt","%s--%s",strTIME.c_str(),szTemp);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO))	return;

	SQLINTEGER nCharNum = 0, cbCharNum=SQL_NTS;
	SQLCHAR    szChaName[CHR_ID_LENGTH+1] = {0}; SQLINTEGER cbChaName = SQL_NTS; 
	
	while (true)
	{
		sReturn = ::SQLFetch(hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)	break;

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{		
			::SQLGetData(hStmt, 1, SQL_C_LONG, &nCharNum,       0, &cbCharNum);
            ::SQLGetData(hStmt, 2, SQL_C_CHAR, szChaName   ,  CHR_ID_LENGTH+1, &cbChaName); 
            
			SetWin_Num_int(this,IDC_EDIT_CHANUM,nCharNum);
			SetWin_Text(this,IDC_EDIT_CHANAME,(const char*) szChaName);
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}
	::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);	
}

void CRanManagerCharInven::ReadInven (int nChar)
{
	SQLRETURN	sReturn = 0; // SQL return value (short)	
	SQLHSTMT	hStmt; // Handle of statement (void*)
	CByteStream ByteStream;
	
	sReturn = ::SQLAllocHandle(SQL_HANDLE_STMT, COdbc::GetInstance()->ConnectGame(), &hStmt);
	if (sReturn != SQL_SUCCESS && sReturn != SQL_SUCCESS_WITH_INFO)
	{
		return;
	}

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "SELECT ChaInven FROM  %s.dbo.ChaInfo WITH (NOLOCK) WHERE ChaNum=%u"
		,CToolCfg::GetInstance()->szGameDb,nChar );
	
	sReturn = ::SQLExecDirect(hStmt,(SQLCHAR*)szTemp,SQL_NTS);

	std::string strTIME = CDebugSet::GetCurTime ();
	LOGSCRIPT::LogToFile("CharInven.txt","%s--%s",strTIME.c_str(),szTemp);

	if ( sReturn != SQL_SUCCESS && sReturn != SQL_SUCCESS_WITH_INFO )
	{
		MessageBox("Error Reading Inven Items Binary","Error",MB_OK);
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return ;
	}

	SQLCHAR		pBinary[DB_IMAGE_BUF_SIZE] = {0};	// Declare a binary buffer to retrieve 1024 bytes of data at a time.
	SQLINTEGER  lSize=0, lTotalSize=0;	
	ByteStream.ClearBuffer();

	while ( (sReturn = ::SQLFetch(hStmt) ) != SQL_NO_DATA )
	{			
		while (1) 
		{
			::memset( pBinary, 0, DB_IMAGE_BUF_SIZE );
			lSize = 0;
			sReturn = ::SQLGetData(
							hStmt,
							1,
							SQL_C_BINARY,
							pBinary,
							DB_IMAGE_BUF_SIZE,
							&lSize );
			if (lSize > 0)
			{
				if (lSize > DB_IMAGE_BUF_SIZE) lSize = DB_IMAGE_BUF_SIZE;
				lTotalSize += lSize;
				ByteStream.WriteBuffer( (LPBYTE) pBinary, lSize );
			}
			if (sReturn == SQL_NO_DATA || lSize == 0)
				break;
		}

		if ( lTotalSize < DB_IMAGE_MIN_SIZE )		
			ByteStream.ClearBuffer ();
	}

	::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

	if ( ByteStream.IsEmpty() )
	{
		MessageBox("Binary Data Empty!","Error",MB_OK);
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return;
	}

	GLInventory cInven;
	BOOL bOk = cInven.SETITEM_BYBUFFER ( ByteStream );
	if ( !bOk )			
	{
		MessageBox("Error Inventory Buffer","Error",MB_OK);
		return;
	}

	m_cInventory.DeleteItemAll();//clear all before adding new data
	m_cInventory.SetAddLine ( 6, true );
	WORD wSIZEX = cInven.GetSizeX();
	WORD wSIZEY = cInven.GetSizeY();
	if ( wSIZEX==EM_INVENSIZE_X && wSIZEY==EM_INVENSIZE_Y )
	{
		m_cInventory.Assign ( cInven );
	}
	else
	{
		m_cInventory.CleanUp ();

		m_cInventory.SetState ( EM_INVENSIZE_X, EM_INVENSIZE_Y );

		GLInventory::CELL_MAP* pmapITEM = cInven.GetItemList();
		GLInventory::CELL_MAP_CITER pos = pmapITEM->begin();
		GLInventory::CELL_MAP_CITER end = pmapITEM->end();
		for ( ; pos!=end; ++pos )
		{
			m_cInventory.InsertItem ( (*pos).second->sItemCustom );
		}
	}

	ShowInven ();
}

void CRanManagerCharInven::ShowInven ()
{
	SetWin_Num_int(this,IDC_EDIT_TOTAL,m_cInventory.CountItemTotal() );
	int nNum = 0;
	TCHAR szTemp[128] = {0};
	for (int X = 0; X < EM_INVENSIZE_X; X++ )
	{
		for (int Y = 0; Y < EM_INVENSIZE_Y; Y++ )
		{
			SINVENITEM* sItem = m_cInventory.FindPosItem(X,Y);
			if (sItem)	_snprintf( szTemp, 128, "%d_%d",sItem->sItemCustom.sNativeID.wMainID,sItem->sItemCustom.sNativeID.wSubID );
			else	_snprintf( szTemp, 128, "%s","Empty" );

			switch (nNum)
			{
				case	0:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_0,(const TCHAR*)szTemp );
				case	1:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_1,(const TCHAR*)szTemp );
				case	2:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_2,(const TCHAR*)szTemp );
				case	3:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_3,(const TCHAR*)szTemp );
				case	4:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_4,(const TCHAR*)szTemp );
				case	5:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_5,(const TCHAR*)szTemp );
				case	6:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_6,(const TCHAR*)szTemp );
				case	7:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_7,(const TCHAR*)szTemp );
				case	8:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_8,(const TCHAR*)szTemp );
				case	9:	SetWin_Text ( this, IDC_BUTTON_INVEN_0_9,(const TCHAR*)szTemp );

				case	10:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_0,(const TCHAR*)szTemp );
				case	11:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_1,(const TCHAR*)szTemp );
				case	12:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_2,(const TCHAR*)szTemp );
				case	13:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_3,(const TCHAR*)szTemp );
				case	14:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_4,(const TCHAR*)szTemp );
				case	15:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_5,(const TCHAR*)szTemp );
				case	16:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_6,(const TCHAR*)szTemp );
				case	17:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_7,(const TCHAR*)szTemp );
				case	18:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_8,(const TCHAR*)szTemp );
				case	19:	SetWin_Text ( this, IDC_BUTTON_INVEN_1_9,(const TCHAR*)szTemp );

				case	20:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_0,(const TCHAR*)szTemp );
				case	21:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_1,(const TCHAR*)szTemp );
				case	22:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_2,(const TCHAR*)szTemp );
				case	23:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_3,(const TCHAR*)szTemp );
				case	24:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_4,(const TCHAR*)szTemp );
				case	25:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_5,(const TCHAR*)szTemp );
				case	26:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_6,(const TCHAR*)szTemp );
				case	27:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_7,(const TCHAR*)szTemp );
				case	28:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_8,(const TCHAR*)szTemp );
				case	29:	SetWin_Text ( this, IDC_BUTTON_INVEN_2_9,(const TCHAR*)szTemp );

				case	30:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_0,(const TCHAR*)szTemp );
				case	31:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_1,(const TCHAR*)szTemp );
				case	32:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_2,(const TCHAR*)szTemp );
				case	33:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_3,(const TCHAR*)szTemp );
				case	34:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_4,(const TCHAR*)szTemp );
				case	35:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_5,(const TCHAR*)szTemp );
				case	36:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_6,(const TCHAR*)szTemp );
				case	37:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_7,(const TCHAR*)szTemp );
				case	38:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_8,(const TCHAR*)szTemp );
				case	39:	SetWin_Text ( this, IDC_BUTTON_INVEN_3_9,(const TCHAR*)szTemp );

				case	40:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_0,(const TCHAR*)szTemp );
				case	41:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_1,(const TCHAR*)szTemp );
				case	42:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_2,(const TCHAR*)szTemp );
				case	43:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_3,(const TCHAR*)szTemp );
				case	44:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_4,(const TCHAR*)szTemp );
				case	45:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_5,(const TCHAR*)szTemp );
				case	46:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_6,(const TCHAR*)szTemp );
				case	47:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_7,(const TCHAR*)szTemp );
				case	48:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_8,(const TCHAR*)szTemp );
				case	49:	SetWin_Text ( this, IDC_BUTTON_INVEN_4_9,(const TCHAR*)szTemp );

				case	50:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_0,(const TCHAR*)szTemp );
				case	51:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_1,(const TCHAR*)szTemp );
				case	52:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_2,(const TCHAR*)szTemp );
				case	53:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_3,(const TCHAR*)szTemp );
				case	54:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_4,(const TCHAR*)szTemp );
				case	55:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_5,(const TCHAR*)szTemp );
				case	56:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_6,(const TCHAR*)szTemp );
				case	57:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_7,(const TCHAR*)szTemp );
				case	58:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_8,(const TCHAR*)szTemp );
				case	59:	SetWin_Text ( this, IDC_BUTTON_INVEN_5_9,(const TCHAR*)szTemp );
			};
			nNum++;
		}
	}
}

void CRanManagerCharInven::OnBnClickedButtonInven00()	{	ViewItem(0,0);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_0 );	}
void CRanManagerCharInven::OnBnClickedButtonInven01()	{	ViewItem(0,1);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_1 );	}
void CRanManagerCharInven::OnBnClickedButtonInven02()	{	ViewItem(0,2);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_2 );	}
void CRanManagerCharInven::OnBnClickedButtonInven03()	{	ViewItem(0,3);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_3 );	}
void CRanManagerCharInven::OnBnClickedButtonInven04()	{	ViewItem(0,4);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_4 );	}
void CRanManagerCharInven::OnBnClickedButtonInven05()	{	ViewItem(0,5);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_5 );	}
void CRanManagerCharInven::OnBnClickedButtonInven06()	{	ViewItem(0,6);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_6 );	}
void CRanManagerCharInven::OnBnClickedButtonInven07()	{	ViewItem(0,7);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_7 );	}
void CRanManagerCharInven::OnBnClickedButtonInven08()	{	ViewItem(0,8);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_8 );	}
void CRanManagerCharInven::OnBnClickedButtonInven09()	{	ViewItem(0,9);	SetInvenItemState ( this,IDC_BUTTON_INVEN_0_9 );	}
void CRanManagerCharInven::OnBnClickedButtonInven10()	{	ViewItem(1,0);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_0 );	}
void CRanManagerCharInven::OnBnClickedButtonInven11()	{	ViewItem(1,1);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_1 );	}
void CRanManagerCharInven::OnBnClickedButtonInven12()	{	ViewItem(1,2);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_2 );	}
void CRanManagerCharInven::OnBnClickedButtonInven13()	{	ViewItem(1,3);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_3 );	}
void CRanManagerCharInven::OnBnClickedButtonInven14()	{	ViewItem(1,4);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_4 );	}
void CRanManagerCharInven::OnBnClickedButtonInven15()	{	ViewItem(1,5);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_5 );	}
void CRanManagerCharInven::OnBnClickedButtonInven16()	{	ViewItem(1,6);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_6 );	}
void CRanManagerCharInven::OnBnClickedButtonInven17()	{	ViewItem(1,7);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_7 );	}
void CRanManagerCharInven::OnBnClickedButtonInven18()	{	ViewItem(1,8);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_8 );	}
void CRanManagerCharInven::OnBnClickedButtonInven19()	{	ViewItem(1,9);	SetInvenItemState ( this,IDC_BUTTON_INVEN_1_9 );	}
void CRanManagerCharInven::OnBnClickedButtonInven20()	{	ViewItem(2,0);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_0 );	}
void CRanManagerCharInven::OnBnClickedButtonInven21()	{	ViewItem(2,1);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_1 );	}
void CRanManagerCharInven::OnBnClickedButtonInven22()	{	ViewItem(2,2);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_2 );	}
void CRanManagerCharInven::OnBnClickedButtonInven23()	{	ViewItem(2,3);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_3 );	}
void CRanManagerCharInven::OnBnClickedButtonInven24()	{	ViewItem(2,4);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_4 );	}
void CRanManagerCharInven::OnBnClickedButtonInven25()	{	ViewItem(2,5);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_5 );	}
void CRanManagerCharInven::OnBnClickedButtonInven26()	{	ViewItem(2,6);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_6 );	}
void CRanManagerCharInven::OnBnClickedButtonInven27()	{	ViewItem(2,7);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_7 );	}
void CRanManagerCharInven::OnBnClickedButtonInven28()	{	ViewItem(2,8);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_8 );	}
void CRanManagerCharInven::OnBnClickedButtonInven29()	{	ViewItem(2,9);	SetInvenItemState ( this,IDC_BUTTON_INVEN_2_9 );	}
void CRanManagerCharInven::OnBnClickedButtonInven30()	{	ViewItem(3,0);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_0 );	}
void CRanManagerCharInven::OnBnClickedButtonInven31()	{	ViewItem(3,1);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_1 );	}
void CRanManagerCharInven::OnBnClickedButtonInven32()	{	ViewItem(3,2);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_2 );	}
void CRanManagerCharInven::OnBnClickedButtonInven33()	{	ViewItem(3,3);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_3 );	}
void CRanManagerCharInven::OnBnClickedButtonInven34()	{	ViewItem(3,4);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_4 );	}
void CRanManagerCharInven::OnBnClickedButtonInven35()	{	ViewItem(3,5);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_5 );	}
void CRanManagerCharInven::OnBnClickedButtonInven36()	{	ViewItem(3,6);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_6 );	}
void CRanManagerCharInven::OnBnClickedButtonInven37()	{	ViewItem(3,7);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_7 );	}
void CRanManagerCharInven::OnBnClickedButtonInven38()	{	ViewItem(3,8);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_8 );	}
void CRanManagerCharInven::OnBnClickedButtonInven39()	{	ViewItem(3,9);	SetInvenItemState ( this,IDC_BUTTON_INVEN_3_9 );	}
void CRanManagerCharInven::OnBnClickedButtonInven40()	{	ViewItem(4,0);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_0 );	}
void CRanManagerCharInven::OnBnClickedButtonInven41()	{	ViewItem(4,1);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_1 );	}
void CRanManagerCharInven::OnBnClickedButtonInven42()	{	ViewItem(4,2);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_2 );	}
void CRanManagerCharInven::OnBnClickedButtonInven43()	{	ViewItem(4,3);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_3 );	}
void CRanManagerCharInven::OnBnClickedButtonInven44()	{	ViewItem(4,4);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_4 );	}
void CRanManagerCharInven::OnBnClickedButtonInven45()	{	ViewItem(4,5);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_5 );	}
void CRanManagerCharInven::OnBnClickedButtonInven46()	{	ViewItem(4,6);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_6 );	}
void CRanManagerCharInven::OnBnClickedButtonInven47()	{	ViewItem(4,7);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_7 );	}
void CRanManagerCharInven::OnBnClickedButtonInven48()	{	ViewItem(4,8);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_8 );	}
void CRanManagerCharInven::OnBnClickedButtonInven49()	{	ViewItem(4,9);	SetInvenItemState ( this,IDC_BUTTON_INVEN_4_9 );	}
void CRanManagerCharInven::OnBnClickedButtonInven50()	{	ViewItem(5,0);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_0 );	}
void CRanManagerCharInven::OnBnClickedButtonInven51()	{	ViewItem(5,1);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_1 );	}
void CRanManagerCharInven::OnBnClickedButtonInven52()	{	ViewItem(5,2);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_2 );	}
void CRanManagerCharInven::OnBnClickedButtonInven53()	{	ViewItem(5,3);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_3 );	}
void CRanManagerCharInven::OnBnClickedButtonInven54()	{	ViewItem(5,4);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_4 );	}
void CRanManagerCharInven::OnBnClickedButtonInven55()	{	ViewItem(5,5);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_5 );	}
void CRanManagerCharInven::OnBnClickedButtonInven56()	{	ViewItem(5,6);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_6 );	}
void CRanManagerCharInven::OnBnClickedButtonInven57()	{	ViewItem(5,7);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_7 );	}
void CRanManagerCharInven::OnBnClickedButtonInven58()	{	ViewItem(5,8);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_8 );	}
void CRanManagerCharInven::OnBnClickedButtonInven59()	{	ViewItem(5,9);	SetInvenItemState ( this,IDC_BUTTON_INVEN_5_9 );	}

void CRanManagerCharInven::SetInvenItemState ( CDialog *pDlg,int nID )
{
	ResetInvenItemState (this);
	CButton *pBtn;
	switch (nID)
	{
	case IDC_BUTTON_INVEN_0_0:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_0 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_1:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_1 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_2:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_2 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_3:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_3 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_4:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_4 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_5:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_5 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_6:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_6 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_7:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_7 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_8:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_8 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_0_9:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_9 );	pBtn->SetState(TRUE);	break;

	case IDC_BUTTON_INVEN_1_0:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_0 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_1:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_1 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_2:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_2 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_3:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_3 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_4:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_4 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_5:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_5 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_6:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_6 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_7:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_7 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_8:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_8 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_1_9:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_9 );	pBtn->SetState(TRUE);	break;

	case IDC_BUTTON_INVEN_2_0:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_0 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_1:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_1 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_2:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_2 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_3:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_3 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_4:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_4 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_5:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_5 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_6:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_6 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_7:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_7 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_8:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_8 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_2_9:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_9 );	pBtn->SetState(TRUE);	break;

	case IDC_BUTTON_INVEN_3_0:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_0 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_1:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_1 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_2:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_2 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_3:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_3 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_4:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_4 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_5:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_5 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_6:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_6 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_7:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_7 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_8:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_8 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_3_9:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_9 );	pBtn->SetState(TRUE);	break;

	case IDC_BUTTON_INVEN_4_0:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_0 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_1:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_1 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_2:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_2 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_3:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_3 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_4:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_4 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_5:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_5 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_6:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_6 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_7:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_7 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_8:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_8 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_4_9:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_9 );	pBtn->SetState(TRUE);	break;

	case IDC_BUTTON_INVEN_5_0:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_0 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_1:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_1 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_2:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_2 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_3:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_3 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_4:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_4 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_5:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_5 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_6:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_6 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_7:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_7 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_8:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_8 );	pBtn->SetState(TRUE);	break;
	case IDC_BUTTON_INVEN_5_9:	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_9 );	pBtn->SetState(TRUE);	break;
	};
}

void CRanManagerCharInven::ResetInvenItemState ( CDialog *pDlg )
{
	CButton *pBtn;
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_0 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_1 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_2 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_3 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_4 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_5 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_6 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_7 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_8 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_0_9 );	pBtn->SetState(FALSE);

	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_0 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_1 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_2 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_3 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_4 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_5 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_6 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_7 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_8 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_1_9 );	pBtn->SetState(FALSE);

	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_0 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_1 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_2 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_3 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_4 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_5 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_6 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_7 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_8 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_2_9 );	pBtn->SetState(FALSE);

	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_0 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_1 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_2 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_3 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_4 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_5 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_6 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_7 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_8 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_3_9 );	pBtn->SetState(FALSE);

	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_0 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_1 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_2 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_3 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_4 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_5 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_6 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_7 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_8 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_4_9 );	pBtn->SetState(FALSE);

	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_0 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_1 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_2 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_3 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_4 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_5 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_6 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_7 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_8 );	pBtn->SetState(FALSE);
	pBtn =  (CButton*)pDlg->GetDlgItem ( IDC_BUTTON_INVEN_5_9 );	pBtn->SetState(FALSE);

}

void CRanManagerCharInven::ViewItem(WORD X,WORD Y)
{
	nActiveItem_X = X;
	nActiveItem_Y = Y;

	SetWin_Enable(this,IDC_BTN_EDIT_ITEM_UPDATE,true);

	SINVENITEM* sItem = m_cInventory.FindPosItem(X,Y);
	if (sItem) 
	{
		SetWin_Enable(this,IDC_BTN_EDIT_ITEM_DELETE,true);
		
		SetWin_Num_int ( this, IDC_EDIT_POS_X,sItem->wPosX);
		SetWin_Num_int ( this, IDC_EDIT_POS_Y,sItem->wPosY);

		SITEM* pItem = GLItemMan::GetInstance().GetItem(sItem->sItemCustom.sNativeID);
		if (pItem ) 
		{
			SetWin_Num_int ( this, IDC_EDIT_MID,pItem->sBasicOp.sNativeID.wMainID);
			SetWin_Num_int ( this, IDC_EDIT_SID,pItem->sBasicOp.sNativeID.wSubID);
			SetWin_Text ( this, IDC_EDIT_NAME,pItem->GetName());
		}
		else
		{
			SetWin_Num_int ( this, IDC_EDIT_MID,sItem->sItemCustom.sNativeID.wMainID);
			SetWin_Num_int ( this, IDC_EDIT_SID,sItem->sItemCustom.sNativeID.wSubID);
			SetWin_Text ( this, IDC_EDIT_NAME,"Unknown Item");
		}

		SetWin_Num_int ( this, IDC_EDIT_MID_COS,sItem->sItemCustom.nidDISGUISE.wMainID);
		SetWin_Num_int ( this, IDC_EDIT_SID_COS,sItem->sItemCustom.nidDISGUISE.wSubID);

		CTimeSpan cSPAN(sItem->sItemCustom.tDISGUISE);
		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_1, (int)cSPAN.GetDays() );
		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_2, (int)cSPAN.GetHours() );
		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_3, (int)cSPAN.GetMinutes() );

		SITEM* pCosItem = GLItemMan::GetInstance().GetItem(sItem->sItemCustom.nidDISGUISE);
		if (pCosItem)	SetWin_Text ( this, IDC_EDIT_NAME_COS,pCosItem->GetName());
		else	SetWin_Text ( this, IDC_EDIT_NAME_COS,"Unknown Item");

		SetWin_Num_int ( this, IDC_EDIT_TURN_NUM,sItem->sItemCustom.wTurnNum );
		m_cGenType.SetCurSel((int)sItem->sItemCustom.cGenType);

		TCHAR Gen[100];
		_snprintf( Gen, 100, "%I64d",sItem->sItemCustom.lnGenNum );
		SetWin_Text ( this, IDC_EDIT_GEN_NUM,Gen);

		SetWin_Num_int ( this, IDC_EDIT_CHID,sItem->sItemCustom.cChnID );
		SetWin_Num_int ( this, IDC_EDIT_FLDID,sItem->sItemCustom.cFieldID );

		SetWin_Num_int ( this, IDC_EDIT_DAMAGE,sItem->sItemCustom.cDAMAGE);
		SetWin_Num_int ( this, IDC_EDIT_DEFENSE,sItem->sItemCustom.cDEFENSE);

		SetWin_Num_int ( this, IDC_EDIT_FIRE,sItem->sItemCustom.cRESIST_FIRE);
		SetWin_Num_int ( this, IDC_EDIT_ICE,sItem->sItemCustom.cRESIST_ICE);
		SetWin_Num_int ( this, IDC_EDIT_ELE,sItem->sItemCustom.cRESIST_ELEC);
		SetWin_Num_int ( this, IDC_EDIT_POI,sItem->sItemCustom.cRESIST_POISON);
		SetWin_Num_int ( this, IDC_EDIT_SPI,sItem->sItemCustom.cRESIST_SPIRIT);

		m_cRandomOp1.SetCurSel((int)sItem->sItemCustom.cOptTYPE1);
		m_cRandomOp2.SetCurSel((int)sItem->sItemCustom.cOptTYPE2);
		m_cRandomOp3.SetCurSel((int)sItem->sItemCustom.cOptTYPE3);
		m_cRandomOp4.SetCurSel((int)sItem->sItemCustom.cOptTYPE4);

		SetWin_Num_int ( this, IDC_EDIT_OPTION_VAL1,(int)sItem->sItemCustom.nOptVALUE1);
		SetWin_Num_int ( this, IDC_EDIT_OPTION_VAL2,(int)sItem->sItemCustom.nOptVALUE2);
		SetWin_Num_int ( this, IDC_EDIT_OPTION_VAL3,(int)sItem->sItemCustom.nOptVALUE3);
		SetWin_Num_int ( this, IDC_EDIT_OPTION_VAL4,(int)sItem->sItemCustom.nOptVALUE4);

		SetWin_Num_int ( this, IDC_EDIT_PET_ID,(int)sItem->sItemCustom.dwPetID);
		SetWin_Num_int ( this, IDC_EDIT_VEHICLE_ID,(int)sItem->sItemCustom.dwVehicleID);
		SetWin_Num_int ( this, IDC_EDIT_OWNER_ID,(int)sItem->sItemCustom.dwOwnerID);
		SetWin_Num_int ( this, IDC_EDIT_ITEMCOLOR,(int)sItem->sItemCustom.wColor);
	}
	else
	{
		SetWin_Enable(this,IDC_BTN_EDIT_ITEM_DELETE,false);

		SetWin_Num_int ( this, IDC_EDIT_POS_X,X);
		SetWin_Num_int ( this, IDC_EDIT_POS_Y,Y);
		SetWin_Num_int ( this, IDC_EDIT_MID,65535);
		SetWin_Num_int ( this, IDC_EDIT_SID,65535);
		SetWin_Text ( this, IDC_EDIT_NAME,"NULL");

		SetWin_Num_int ( this, IDC_EDIT_MID_COS,65535);
		SetWin_Num_int ( this, IDC_EDIT_SID_COS,65535);

		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_1, 0 );
		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_2, 0 );
		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_3, 0 );

		SetWin_Text ( this, IDC_EDIT_NAME_COS,"NULL");

		SetWin_Num_int ( this, IDC_EDIT_TURN_NUM,0 );
		m_cGenType.SetCurSel(0);

		SetWin_Text ( this, IDC_EDIT_GEN_NUM,"0");

		SetWin_Num_int ( this, IDC_EDIT_CHID,0 );
		SetWin_Num_int ( this, IDC_EDIT_FLDID,0 );

		SetWin_Num_int ( this, IDC_EDIT_DAMAGE,0);
		SetWin_Num_int ( this, IDC_EDIT_DEFENSE,0);

		SetWin_Num_int ( this, IDC_EDIT_FIRE,0);
		SetWin_Num_int ( this, IDC_EDIT_ICE,0);
		SetWin_Num_int ( this, IDC_EDIT_ELE,0);
		SetWin_Num_int ( this, IDC_EDIT_POI,0);
		SetWin_Num_int ( this, IDC_EDIT_SPI,0);

		m_cRandomOp1.SetCurSel(0);
		m_cRandomOp2.SetCurSel(0);
		m_cRandomOp3.SetCurSel(0);
		m_cRandomOp4.SetCurSel(0);

		SetWin_Num_int ( this, IDC_EDIT_OPTION_VAL1,0);
		SetWin_Num_int ( this, IDC_EDIT_OPTION_VAL2,0);
		SetWin_Num_int ( this, IDC_EDIT_OPTION_VAL3,0);
		SetWin_Num_int ( this, IDC_EDIT_OPTION_VAL4,0);

		SetWin_Num_int ( this, IDC_EDIT_PET_ID,0);
		SetWin_Num_int ( this, IDC_EDIT_VEHICLE_ID,0);
		SetWin_Num_int ( this, IDC_EDIT_OWNER_ID,0);
		SetWin_Num_int ( this, IDC_EDIT_ITEMCOLOR,32767);
	}
}

void CRanManagerCharInven::OnBnClickedButtonItemsearchM()
{
	CItemSearch sItemSearch;
	sItemSearch.SetMultiple(false);
	if ( IDOK == sItemSearch.DoModal () )
	{
		SITEM*	pItemCheck = GLItemMan::GetInstance().GetItem(sItemSearch.sSelectedItem);
		if ( pItemCheck )
		{
			SetWin_Num_int ( this, IDC_EDIT_MID,sItemSearch.sSelectedItem.wMainID);
			SetWin_Num_int ( this, IDC_EDIT_SID,sItemSearch.sSelectedItem.wSubID);
			SetWin_Text( this,IDC_EDIT_NAME,pItemCheck->GetName() );
		}
	}
}

void CRanManagerCharInven::OnBnClickedButtonItemsearchCos()
{
	CItemSearch sItemSearch;
	sItemSearch.SetMultiple(false);
	if ( IDOK == sItemSearch.DoModal () )
	{
		SITEM*	pItemCheck = GLItemMan::GetInstance().GetItem(sItemSearch.sSelectedItem);
		if ( pItemCheck )
		{
			SetWin_Num_int ( this, IDC_EDIT_MID_COS,sItemSearch.sSelectedItem.wMainID);
			SetWin_Num_int ( this, IDC_EDIT_SID_COS,sItemSearch.sSelectedItem.wSubID);
			SetWin_Text( this,IDC_EDIT_NAME_COS,pItemCheck->GetName() );
		}
	}
}

void CRanManagerCharInven::OnBnClickedBtnEditItemDelete()
{
	if ( MessageBox ( "Do You Want to Delete this Item?, Continue?", "Question", MB_YESNO ) == IDYES )
	{
		int X = GetWin_Num_int(this,IDC_EDIT_POS_X);
		int Y = GetWin_Num_int(this,IDC_EDIT_POS_Y);
		m_cInventory.DeleteItem(X,Y);
		ShowInven ();
		ViewItem(X,Y);
	}
}

void CRanManagerCharInven::OnBnClickedBtnEditItemUpdate()
{
	if ( MessageBox ( "Update Changes to This Item?", "Question", MB_YESNO ) == IDYES )
	{
		SITEM*  sItemCheck = GLItemMan::GetInstance().GetItem(GetWin_Num_int(this,IDC_EDIT_MID),GetWin_Num_int(this,IDC_EDIT_SID));
		if (!sItemCheck)
		{
			MessageBox("Invalid Item ID! Recheck Item MID and SID","Item Error",MB_OK);
			return;
		}

		SITEMCUSTOM	pItemUpdate;
		pItemUpdate.sNativeID = sItemCheck->sBasicOp.sNativeID;
		SNATIVEID sCosID;
		sCosID.wMainID = GetWin_Num_int(this,IDC_EDIT_MID_COS);
		sCosID.wSubID = GetWin_Num_int(this,IDC_EDIT_SID_COS);
		pItemUpdate.nidDISGUISE = sCosID;
		CTime cTIME_CUR;
		pItemUpdate.tBORNTIME = cTIME_CUR.GetTime();

		CTimeSpan cSPAN(
			GetWin_Num_int(this,IDC_EDIT_LOCKER_COS_1),
			GetWin_Num_int(this,IDC_EDIT_LOCKER_COS_2),
			GetWin_Num_int(this,IDC_EDIT_LOCKER_COS_3),
			0);
		pItemUpdate.tDISGUISE = cSPAN.GetTimeSpan();
		pItemUpdate.lnGenNum = _strtoi64(GetWin_Text(this,IDC_EDIT_GEN_NUM ),NULL,10);
		pItemUpdate.wTurnNum = (WORD)GetWin_Num_int(this,IDC_EDIT_TURN_NUM);
		pItemUpdate.cGenType = (BYTE)m_cGenType.GetCurSel();
		if ( pItemUpdate.cGenType == EMGEN_GMEDIT || pItemUpdate.cGenType == EMGEN_GMEDIT2)	SetWin_Num_int(this, IDC_EDIT_FLDID ,255 );
		pItemUpdate.cChnID = (BYTE)GetWin_Num_int(this,IDC_EDIT_CHID);
		pItemUpdate.cFieldID = (BYTE)GetWin_Num_int(this,IDC_EDIT_FLDID);
		pItemUpdate.cDAMAGE = (BYTE)GetWin_Num_int(this,IDC_EDIT_DAMAGE);
		pItemUpdate.cDEFENSE = (BYTE)GetWin_Num_int(this,IDC_EDIT_DEFENSE);
		pItemUpdate.cRESIST_FIRE = (BYTE)GetWin_Num_int(this,IDC_EDIT_FIRE);
		pItemUpdate.cRESIST_ICE = (BYTE)GetWin_Num_int(this,IDC_EDIT_ICE);
		pItemUpdate.cRESIST_ELEC = (BYTE)GetWin_Num_int(this,IDC_EDIT_ELE);
		pItemUpdate.cRESIST_POISON = (BYTE)GetWin_Num_int(this,IDC_EDIT_POI);
		pItemUpdate.cRESIST_SPIRIT = (BYTE)GetWin_Num_int(this,IDC_EDIT_SPI);
		pItemUpdate.cOptTYPE1 = (BYTE)m_cRandomOp1.GetCurSel();
		pItemUpdate.cOptTYPE2 = (BYTE)m_cRandomOp2.GetCurSel();
		pItemUpdate.cOptTYPE3 = (BYTE)m_cRandomOp3.GetCurSel();
		pItemUpdate.cOptTYPE4 = (BYTE)m_cRandomOp4.GetCurSel();
		pItemUpdate.nOptVALUE1 = (short)GetWin_Num_int(this,IDC_EDIT_OPTION_VAL1);
		pItemUpdate.nOptVALUE2 = (short)GetWin_Num_int(this,IDC_EDIT_OPTION_VAL2);
		pItemUpdate.nOptVALUE3 = (short)GetWin_Num_int(this,IDC_EDIT_OPTION_VAL3);
		pItemUpdate.nOptVALUE4 = (short)GetWin_Num_int(this,IDC_EDIT_OPTION_VAL4);
		pItemUpdate.dwPetID = GetWin_Num_int(this,IDC_EDIT_PET_ID);
		pItemUpdate.dwVehicleID = GetWin_Num_int(this,IDC_EDIT_VEHICLE_ID);
		pItemUpdate.dwOwnerID = GetWin_Num_int(this,IDC_EDIT_OWNER_ID);
		pItemUpdate.wColor = GetWin_Num_int(this,IDC_EDIT_ITEMCOLOR);

		m_cInventory.DeleteItem(nActiveItem_X,nActiveItem_Y);
		if (m_cInventory.InsertItem(pItemUpdate,nActiveItem_X,nActiveItem_Y) )	MessageBox("Done Updating Item","Info",MB_OK);
		else	MessageBox("Error! Updating Item","Info",MB_OK);	

		ShowInven ();
		ViewItem(nActiveItem_X,nActiveItem_Y);
	}
}

void CRanManagerCharInven::OnBnClickedButtonResetCosExpire()
{
	if ( MessageBox ( "This Will Remove the costume, Continue?", "Question", MB_YESNO ) == IDYES )
	{
		SetWin_Num_int ( this, IDC_EDIT_MID_COS,65535);
		SetWin_Num_int ( this, IDC_EDIT_SID_COS,65535);

		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_1, 0 );
		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_2, 0 );
		SetWin_Num_int ( this, IDC_EDIT_LOCKER_COS_3, 0 );
		SetWin_Text ( this, IDC_EDIT_NAME_COS,"Unknown Item");
	}
}

void CRanManagerCharInven::OnCbnSelchangeComboGentype()
{
}

void CRanManagerCharInven::OnBnClickedButtonInvencancel()
{
	m_pToolTab->ActivePage(CHARPAGE_MAIN,0);
}

void CRanManagerCharInven::OnBnClickedButtonInvensave()
{
	CByteStream ByteStream;
	LPBYTE pBuffer = NULL;
	DWORD dwSize = 0;
	TCHAR szTemp[128] = {0};
	int nChaNum = GetWin_Num_int(this,IDC_EDIT_CHANUM);

	SQLRETURN	sReturn = 0; // SQL return value (short)	
	SQLHSTMT	hStmt; // Handle of statement (void*)

	sReturn = ::SQLAllocHandle(SQL_HANDLE_STMT, COdbc::GetInstance()->ConnectGame(), &hStmt);
	if (sReturn != SQL_SUCCESS && sReturn != SQL_SUCCESS_WITH_INFO)	
	{
		MessageBox("Error Connecting to Game DB!","error",MB_OK);
		return;
	}
	
	ByteStream.ClearBuffer ();
	m_cInventory.GETITEM_BYBUFFER ( ByteStream );
	ByteStream.GetBuffer(pBuffer, dwSize);

	//write image
	if (pBuffer != NULL)
	{
		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInven=? WHERE (ChaNum=%u)", (DWORD)nChaNum );

		std::string strTIME = CDebugSet::GetCurTime ();
		LOGSCRIPT::LogToFile("CharInven.txt","%s--%s",strTIME.c_str(),szTemp);

		sReturn = ::SQLPrepare(hStmt, (SQLCHAR*)szTemp, SQL_NTS);
		if (sReturn != SQL_SUCCESS && sReturn != SQL_SUCCESS_WITH_INFO) 
		{	
			MessageBox("Error Sql Prepare","error",MB_OK);
			::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			return;
		}

		SQLINTEGER cbBinaryParam = SQL_LEN_DATA_AT_EXEC(0);

		sReturn = ::SQLBindParameter(hStmt, 
								1, 
								SQL_PARAM_INPUT,
								SQL_C_BINARY, 
								SQL_LONGVARBINARY,
								dwSize, 
								0, 
								(SQLPOINTER) 2, 
								0, 
								&cbBinaryParam);
		if (sReturn != SQL_SUCCESS && sReturn != SQL_SUCCESS_WITH_INFO) 
		{
			MessageBox("Error SQLBind","error",MB_OK);
			return;
		}

		sReturn = ::SQLExecute(hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
		{
			MessageBox("Error Saving CharInven Binary!","error",MB_OK);
			return;
		}
	
		SQLPOINTER pToken = NULL;
		while (sReturn == SQL_NEED_DATA) 
		{
			sReturn = ::SQLParamData(hStmt, &pToken);
			if (sReturn == SQL_NEED_DATA) 
			{				
				::SQLPutData(hStmt, pBuffer, (SQLINTEGER) dwSize);
			}
			Sleep( 0 );
		}	
	}

	::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	MessageBox("Done Saving CharInven!","Info",MB_OK);

	pBuffer = NULL;
	ShowInven ();
}

void CRanManagerCharInven::OnBnClickedButtonInvenwipe()
{
	if ( MessageBox ( "This Will Remove All The Character Items In The Inventory!, Continue?", "Question", MB_YESNO ) == IDYES )
	{
		m_cInventory.DeleteItemAll();
		ShowInven ();
		ViewItem(65535,65535);
	}
}
