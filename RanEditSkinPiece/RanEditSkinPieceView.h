// RanEditSkinPieceView.h : interface of the CRanEditSkinPieceView class
//
#include "D3DApp.h"
#include "DxSkinPieceContainer.h"
#include "DxObjectMRS.h"

#pragma once


class CRanEditSkinPieceView : public CView, public CD3DApplication
{
protected: // create from serialization only
	CRanEditSkinPieceView();
	DECLARE_DYNCREATE(CRanEditSkinPieceView)

// Attributes
public:
	//CRanEditSkinPieceDoc* GetDocument() const;
	static	CRanEditSkinPieceView * GetView();

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	char	szAppPath[MAX_PATH];
	BOOL	m_bCreated;
	HCURSOR	m_hCursorDefault;

public:
	LPDIRECT3DDEVICEQ Get3DDevice()	{	return m_pd3dDevice;	}

	public:
	void SetActive ( BOOL bActive );

	HRESULT ReSizeWindow ( int cx, int cy )
	{
		if ( cx < 10 )	cx = 10;
		if ( cy < 10 )	cy = 10;

		m_d3dpp.BackBufferWidth  = cx;
		m_d3dpp.BackBufferHeight = cy;

		return Resize3DEnvironment();
	}

	HRESULT OneTimeSceneInit();
	HRESULT ConfirmDevice( D3DCAPSQ*,DWORD,D3DFORMAT );
	HRESULT CreateObjects();
	HRESULT InitDeviceObjects();
	HRESULT FrameMove();
	HRESULT Render();
	HRESULT RenderText();
	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT FinalCleanup();

public:
	virtual void OnInitialUpdate();
	virtual HRESULT FrameMove3DEnvironment();
	virtual HRESULT Render3DEnvironment();

protected:
	virtual void PostNcDestroy();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

// Implementation
public:
	virtual ~CRanEditSkinPieceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	DxSkinPiece*	m_pPiece;
	D3DXMATRIX		m_matLocal;
	DxObjectMRS		m_DxEditMRS;

public:
	D3DXVECTOR3		m_vCOL;
	DWORD			m_dwCOL;

	BOOL	bRenderTrace;
	DWORD	dwCOLTYPE;

	BOOL	bRenderSkeleton;
	std::string strBoneName;

	BOOL	bRenderPlane;
	BOOL	bMeshDetect;

	D3DXMATRIX	matEdit;

public:
	void	SetEditMatrix( D3DXMATRIX mat, bool bDisable = false );

public:
	void	FileNew();
	void	FileOpen();
	void	FileSave();
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);	
	afx_msg void OnRenderoptionsRendertrace();
	afx_msg void OnUpdateRenderoptionsRendertrace(CCmdUI *pCmdUI);
	afx_msg void OnCollisiondetectType1();
	afx_msg void OnUpdateCollisiondetectType1(CCmdUI *pCmdUI);
	afx_msg void OnCollisiondetectType2();
	afx_msg void OnUpdateCollisiondetectType2(CCmdUI *pCmdUI);
	afx_msg void OnCollisiondetectType3();
	afx_msg void OnUpdateCollisiondetectType3(CCmdUI *pCmdUI);
	afx_msg void OnRenderoptionsRenderskeleton();
	afx_msg void OnUpdateRenderoptionsRenderskeleton(CCmdUI *pCmdUI);
	afx_msg void OnRenderoptionsRenderplane();
	afx_msg void OnUpdateRenderoptionsRenderplane(CCmdUI *pCmdUI);
	afx_msg void OnFileOpenran2cps();
};

//#ifndef _DEBUG  // debug version in RanEditSkinPieceView.cpp
//inline CRanEditSkinPieceDoc* CRanEditSkinPieceView::GetDocument() const
//   { return reinterpret_cast<CRanEditSkinPieceDoc*>(m_pDocument); }
//#endif

