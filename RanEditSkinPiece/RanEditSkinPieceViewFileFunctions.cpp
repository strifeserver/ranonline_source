// RanEditSkinPieceView.cpp : implementation of the CRanEditSkinPieceView class
//

#include "stdafx.h"
#include "RanEditSkinPiece.h"
#include "RanEditSkinPieceDoc.h"
#include "RanEditSkinPieceView.h"
#include "DxSkinMeshMan.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRanEditSkinPieceView

void CRanEditSkinPieceView::FileNew()
{
	m_pPiece = NULL;
	DxBoneCollector::GetInstance().CleanUp();
	DxSkinMeshMan::GetInstance().CleanUp( m_pd3dDevice );

	m_pPiece = new DxSkinPiece;
	m_pPiece->InitDeviceObjects( m_pd3dDevice );
	
	m_vCOL = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_dwCOL = COL_ERR;

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	if ( pFrame )
	{
		pFrame->SetWindowText( "New" );
	}

	CMainFrame	*pMainFrame = (CMainFrame*) AfxGetMainWnd();
	if ( pMainFrame )
	{
		pMainFrame->DataUpdate();
	}
}

void CRanEditSkinPieceView::FileOpen()
{
	CString szFilterInput = "Piece (*.cps,*.aps,*.vps)|*.cps;*.aps;*.vps|";
	CFileDialog dlgInput(true,"*.*",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilterInput,this);
	dlgInput.m_ofn.lpstrInitialDir = DxSkinPieceContainer::GetInstance().GetPath();
	if ( dlgInput.DoModal() != IDOK ) return;

	m_pPiece = NULL;
	DxBoneCollector::GetInstance().CleanUp();
	DxSkinMeshMan::GetInstance().CleanUp( m_pd3dDevice );

	m_pPiece = new DxSkinPiece;
	m_pPiece->InitDeviceObjects( m_pd3dDevice );
	HRESULT hr = m_pPiece->LoadPiece ( dlgInput.GetFileName(), m_pd3dDevice, false );
	if ( FAILED( hr ) )
	{
		SAFE_DELETE( m_pPiece );
		CDebugSet::MsgBox( "Error Loading Piece file :%s", dlgInput.GetFileName() );

		CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
		if ( pFrame )
		{
			pFrame->SetWindowText( "" );
		}

		return;
	}

	m_vCOL = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_dwCOL = COL_ERR;

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	if ( pFrame )
	{
		pFrame->SetWindowText( dlgInput.GetFileName() );
	}

	CMainFrame	*pMainFrame = (CMainFrame*) AfxGetMainWnd();
	if ( pMainFrame )
	{
		pMainFrame->DataUpdate();
	}
}

void CRanEditSkinPieceView::FileSave()
{
	if ( m_pPiece )
	{
		CString szFilterInput = "Piece (*.cps,*.aps,*.vps)|*.cps;*.aps;*.vps|";
		CFileDialog dlgInput(false,"*.*", m_pPiece->m_szFileName ,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilterInput,this);
		dlgInput.m_ofn.lpstrInitialDir = DxSkinPieceContainer::GetInstance().GetPath();
		if ( dlgInput.DoModal() != IDOK ) return;

		HRESULT hr = m_pPiece->SavePiece( dlgInput.GetFileName() );
		if ( FAILED( hr ) )
		{
			CDebugSet::MsgBox( "Error Saving file :%s", dlgInput.GetFileName() );
			return;
		}
	}
}

void CRanEditSkinPieceView::OnFileOpenran2cps()
{
}
