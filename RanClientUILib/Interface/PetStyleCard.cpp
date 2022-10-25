#include "StdAfx.h"
#include "PetStyleCard.h"

#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "GameTextControl.h"
#include "BasicTextButton.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"
#include "GLogicData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BOUND_OF_INDEX	-1

INT nPET_STYLE_INDEX_A[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_B[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_C[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_D[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_E[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_F[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_G[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_H[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_I[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_J[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_K[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_L[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_M[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_N[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_O[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_P[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_Q[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_R[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_S[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_T[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_U[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_V[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_W[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_X[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_Y[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_Z[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};

TCHAR * szPETSTYLECARD_NAME_A = _T("PETSTYLECARD_NAME_A");
TCHAR * szPETSTYLECARD_NAME_B = _T("PETSTYLECARD_NAME_B");
TCHAR * szPETSTYLECARD_NAME_C = _T("PETSTYLECARD_NAME_C");
TCHAR * szPETSTYLECARD_NAME_D = _T("PETSTYLECARD_NAME_D");
TCHAR * szPETSTYLECARD_NAME_E = _T("PETSTYLECARD_NAME_E");
TCHAR * szPETSTYLECARD_NAME_F = _T("PETSTYLECARD_NAME_F");
TCHAR * szPETSTYLECARD_NAME_G = _T("PETSTYLECARD_NAME_G");
TCHAR * szPETSTYLECARD_NAME_H = _T("PETSTYLECARD_NAME_H");
TCHAR * szPETSTYLECARD_NAME_I = _T("PETSTYLECARD_NAME_I");
TCHAR * szPETSTYLECARD_NAME_J = _T("PETSTYLECARD_NAME_J");
TCHAR * szPETSTYLECARD_NAME_K = _T("PETSTYLECARD_NAME_K");
TCHAR * szPETSTYLECARD_NAME_L = _T("PETSTYLECARD_NAME_L");
TCHAR * szPETSTYLECARD_NAME_M = _T("PETSTYLECARD_NAME_M");
TCHAR * szPETSTYLECARD_NAME_N = _T("PETSTYLECARD_NAME_N");
TCHAR * szPETSTYLECARD_NAME_O = _T("PETSTYLECARD_NAME_O");
TCHAR * szPETSTYLECARD_NAME_P = _T("PETSTYLECARD_NAME_P");
TCHAR * szPETSTYLECARD_NAME_Q = _T("PETSTYLECARD_NAME_Q");
TCHAR * szPETSTYLECARD_NAME_R = _T("PETSTYLECARD_NAME_R");
TCHAR * szPETSTYLECARD_NAME_S = _T("PETSTYLECARD_NAME_S");
TCHAR * szPETSTYLECARD_NAME_T = _T("PETSTYLECARD_NAME_T");
TCHAR * szPETSTYLECARD_NAME_U = _T("PETSTYLECARD_NAME_U");
TCHAR * szPETSTYLECARD_NAME_V = _T("PETSTYLECARD_NAME_V");
TCHAR * szPETSTYLECARD_NAME_W = _T("PETSTYLECARD_NAME_W");
TCHAR * szPETSTYLECARD_NAME_X = _T("PETSTYLECARD_NAME_X");
TCHAR * szPETSTYLECARD_NAME_Y = _T("PETSTYLECARD_NAME_Y");
TCHAR * szPETSTYLECARD_NAME_Z = _T("PETSTYLECARD_NAME_Z");

CPetStyleCard::CPetStyleCard ()
	: m_pStyleName(NULL)
	, m_pPetStylePos(NULL)
	, m_pId2GameText(NULL)
	, m_wPet(0)
	, m_wPetBack(0)
	, m_emTYPE(PETTYPE_NONE)
	, m_bOKButton(FALSE)
{
	SecureZeroMemory( m_pPetStyle, sizeof( m_pPetStyle ) );
}

CPetStyleCard::~CPetStyleCard ()
{
}

CBasicTextBox* CPetStyleCard::CreateStaticControl( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

CUIControl* CPetStyleCard::CreateControl( const char * szControl )
{
	GASSERT( szControl );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub( this, szControl );
	pControl->SetVisibleSingle( FALSE );
	RegisterControl( pControl );

	return pControl;
}

void CPetStyleCard::CreateSubControl ()
{
	CD3DFontPar * pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9 );

	CUIWindowEx::CreateControl( "HAIRSTYLECARD_TITLE_BACK_L" );
	CUIWindowEx::CreateControl( "HAIRSTYLECARD_TITLE_BACK_R" );
	m_pStyleName = CreateStaticControl( "HAIRSTYLECARD_TITLE", pFont, TEXT_ALIGN_CENTER_X );

	{ // Note : 스타일 변경 버튼
		CBasicButton* pButton = new CBasicButton;
		pButton->CreateSub( this, "HAIRSTYLECARD_LEFT", UI_FLAG_DEFAULT, PETSTYLECARD_LEFT );
		pButton->CreateFlip( "HAIRSTYLECARD_LEFT_FLIP", CBasicButton::MOUSEIN_FLIP );
		RegisterControl( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub( this, "HAIRSTYLECARD_RIGHT", UI_FLAG_DEFAULT, PETSTYLECARD_RIGHT );
		pButton->CreateFlip( "HAIRSTYLECARD_RIGHT_FLIP", CBasicButton::MOUSEIN_FLIP );
		RegisterControl( pButton );
	}

	{ // Note : 버튼
		CBasicTextButton* pButton = new CBasicTextButton;
		pButton->CreateSub( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, PETSTYLECARD_OK );
		pButton->CreateBaseButton( "HAIRSTYLECARD_OK", CBasicTextButton::SIZE14, 
									CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD( "MODAL_BUTTON", 0 ) );
		pButton->SetShortcutKey( DIK_RETURN, DIK_NUMPADENTER );
		RegisterControl( pButton );

		pButton = new CBasicTextButton;
		pButton->CreateSub( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, PETSTYLECARD_CANCEL );
		pButton->CreateBaseButton( "HAIRSTYLECARD_CANCEL", CBasicTextButton::SIZE14, 
									CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD( "MODAL_BUTTON", 1 ) );
		pButton->SetShortcutKey( DIK_ESCAPE );
		RegisterControl( pButton );
	}

	{
		CString strCombine;

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_A].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_A_%02d", i );
			m_pPetStyle[PETTYPE_A][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_B].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_B_%02d", i );
			m_pPetStyle[PETTYPE_B][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_C].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_C_%02d", i );
			m_pPetStyle[PETTYPE_C][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_D].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_D_%02d", i );
			m_pPetStyle[PETTYPE_D][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_E].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_E_%02d", i );
			m_pPetStyle[PETTYPE_E][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_F].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_F_%02d", i );
			m_pPetStyle[PETTYPE_F][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_G].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_G_%02d", i );
			m_pPetStyle[PETTYPE_G][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_H].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_H_%02d", i );
			m_pPetStyle[PETTYPE_H][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_I].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_I_%02d", i );
			m_pPetStyle[PETTYPE_I][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_J].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_J_%02d", i );
			m_pPetStyle[PETTYPE_J][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_K].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_K_%02d", i );
			m_pPetStyle[PETTYPE_K][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_L].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_L_%02d", i );
			m_pPetStyle[PETTYPE_L][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_M].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_M_%02d", i );
			m_pPetStyle[PETTYPE_M][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_N].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_N_%02d", i );
			m_pPetStyle[PETTYPE_N][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_O].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_O_%02d", i );
			m_pPetStyle[PETTYPE_O][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_P].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_P_%02d", i );
			m_pPetStyle[PETTYPE_P][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_Q].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_Q_%02d", i );
			m_pPetStyle[PETTYPE_Q][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_R].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_R_%02d", i );
			m_pPetStyle[PETTYPE_R][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_S].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_S_%02d", i );
			m_pPetStyle[PETTYPE_S][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_T].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_T_%02d", i );
			m_pPetStyle[PETTYPE_T][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_U].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_U_%02d", i );
			m_pPetStyle[PETTYPE_U][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_V].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_V_%02d", i );
			m_pPetStyle[PETTYPE_V][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_W].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_W_%02d", i );
			m_pPetStyle[PETTYPE_W][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_X].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_X_%02d", i );
			m_pPetStyle[PETTYPE_X][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_Y].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_Y_%02d", i );
			m_pPetStyle[PETTYPE_Y][i] = CreateControl( strCombine.GetString() );
		}
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PETTYPE_Z].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_Z_%02d", i );
			m_pPetStyle[PETTYPE_Z][i] = CreateControl( strCombine.GetString() );
		}
	}
}

void CPetStyleCard::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case PETSTYLECARD_LEFT:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				INT nBackPos = *m_pPetStylePos;

				--m_pPetStylePos;

				if( *m_pPetStylePos == BOUND_OF_INDEX )	++m_pPetStylePos;
				else
				{
					m_pPetStyle[m_emTYPE][nBackPos]->SetVisibleSingle( FALSE );
					m_pPetStyle[m_emTYPE][*m_pPetStylePos]->SetVisibleSingle( TRUE );

					m_pStyleName->SetText( ID2GAMEINTEXT( m_pId2GameText, *m_pPetStylePos ) );


					--m_wPet;
					GLGaeaClient::GetInstance().GetPetClient()->StyleChange( m_wPet );
				}
			}
		}
		break;

	case PETSTYLECARD_RIGHT:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				INT nBackPos = *m_pPetStylePos;

				++m_pPetStylePos;

				if( *m_pPetStylePos == BOUND_OF_INDEX )	--m_pPetStylePos;
				else
				{
					m_pPetStyle[m_emTYPE][nBackPos]->SetVisibleSingle( FALSE );
					m_pPetStyle[m_emTYPE][*m_pPetStylePos]->SetVisibleSingle( TRUE );

					m_pStyleName->SetText( ID2GAMEINTEXT( m_pId2GameText, *m_pPetStylePos ) );

					++m_wPet;
					GLGaeaClient::GetInstance().GetPetClient()->StyleChange( m_wPet );
				}
			}
		}
		break;

	case PETSTYLECARD_OK:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				m_bOKButton = TRUE;
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}
		break;

	case PETSTYLECARD_CANCEL:
	case ET_CONTROL_BUTTON:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );
}

void CPetStyleCard::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		m_emTYPE = GLGaeaClient::GetInstance().GetPetClient()->m_emTYPE;

		switch( m_emTYPE )
		{
		case PETTYPE_A:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_A;
				m_pId2GameText = szPETSTYLECARD_NAME_A;
			}
			break;
		case PETTYPE_B:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_B;
				m_pId2GameText = szPETSTYLECARD_NAME_B;
			}
			break;
		case PETTYPE_C:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_C;
				m_pId2GameText = szPETSTYLECARD_NAME_C;
			}
			break;
		case PETTYPE_D:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_D;
				m_pId2GameText = szPETSTYLECARD_NAME_D;
			}
			break;

		case PETTYPE_E:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_E;
				m_pId2GameText = szPETSTYLECARD_NAME_E;
			}
			break;

		case PETTYPE_F:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_F;
				m_pId2GameText = szPETSTYLECARD_NAME_F;
			}
			break;

		case PETTYPE_G:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_G;
				m_pId2GameText = szPETSTYLECARD_NAME_G;
			}
			break;

		case PETTYPE_H:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_H;
				m_pId2GameText = szPETSTYLECARD_NAME_H;
			}
			break;

		case PETTYPE_I:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_I;
				m_pId2GameText = szPETSTYLECARD_NAME_I;
			}
			break;

		case PETTYPE_J:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_J;
				m_pId2GameText = szPETSTYLECARD_NAME_J;
			}
			break;

		case PETTYPE_K:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_K;
				m_pId2GameText = szPETSTYLECARD_NAME_K;
			}
			break;
		case PETTYPE_L:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_L;
				m_pId2GameText = szPETSTYLECARD_NAME_L;
			}
			break;
		case PETTYPE_M:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_M;
				m_pId2GameText = szPETSTYLECARD_NAME_M;
			}
			break;
		case PETTYPE_N:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_N;
				m_pId2GameText = szPETSTYLECARD_NAME_N;
			}
			break;
		case PETTYPE_O:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_O;
				m_pId2GameText = szPETSTYLECARD_NAME_O;
			}
			break;
		case PETTYPE_P:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_P;
				m_pId2GameText = szPETSTYLECARD_NAME_P;
			}
			break;
		case PETTYPE_Q:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_Q;
				m_pId2GameText = szPETSTYLECARD_NAME_Q;
			}
			break;
		case PETTYPE_R:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_R;
				m_pId2GameText = szPETSTYLECARD_NAME_R;
			}
			break;
		case PETTYPE_S:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_S;
				m_pId2GameText = szPETSTYLECARD_NAME_S;
			}
			break;
		case PETTYPE_T:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_T;
				m_pId2GameText = szPETSTYLECARD_NAME_T;
			}
			break;
		case PETTYPE_U:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_U;
				m_pId2GameText = szPETSTYLECARD_NAME_U;
			}
			break;
		case PETTYPE_V:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_V;
				m_pId2GameText = szPETSTYLECARD_NAME_V;
			}
			break;
		case PETTYPE_W:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_W;
				m_pId2GameText = szPETSTYLECARD_NAME_W;
			}
			break;
		case PETTYPE_X:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_X;
				m_pId2GameText = szPETSTYLECARD_NAME_X;
			}
			break;
		case PETTYPE_Y:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_Y;
				m_pId2GameText = szPETSTYLECARD_NAME_Y;
			}
			break;
		case PETTYPE_Z:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_Z;
				m_pId2GameText = szPETSTYLECARD_NAME_Z;
			}
			break;
		default:
			GASSERT( 0 && "PETTYPE::PETTYPE_NONE" );
			return;
		}

		m_wPet = GLGaeaClient::GetInstance().GetPetClient()->m_wStyle;
		m_wPetBack = m_wPet;

		m_pPetStylePos += m_wPet + 1;

		m_pStyleName->AddText( ID2GAMEINTEXT( m_pId2GameText, *m_pPetStylePos ) );
		m_pPetStyle[m_emTYPE][*m_pPetStylePos]->SetVisibleSingle( TRUE );

		GLGaeaClient::GetInstance().GetPetClient()->StyleInitData();
	}
	else
	{
		if( m_bOKButton )
		{
			GLGaeaClient::GetInstance().GetPetClient()->ReqChangeStyle( m_wPet );
			m_bOKButton = FALSE;
		}
		else
		{
			GLGaeaClient::GetInstance().GetPetClient()->StyleChange( m_wPetBack );
		}

		m_pStyleName->ClearText();
		m_pPetStyle[m_emTYPE][*m_pPetStylePos]->SetVisibleSingle( FALSE );
		
		GLGaeaClient::GetInstance().GetPetClient()->StyleEnd();
	}
}