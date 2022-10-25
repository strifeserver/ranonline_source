// RanEditSkinPieceView.cpp : implementation of the CRanEditSkinPieceView class
//

#include "stdafx.h"
#include "RanEditSkinPiece.h"

#include "RanEditSkinPieceDoc.h"
#include "RanEditSkinPieceView.h"
#include ".\raneditskinpieceview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern int g_nITEMLEVEL;
extern BOOL g_bCHAR_EDIT_RUN;

// CRanEditSkinPieceView

IMPLEMENT_DYNCREATE(CRanEditSkinPieceView, CView)

BEGIN_MESSAGE_MAP(CRanEditSkinPieceView, CView)
	// Standard printing commands
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, FileOpen)
	ON_COMMAND(ID_FILE_SAVE, FileSave)
	ON_COMMAND(ID_FILE_NEW, FileNew)
	
	ON_COMMAND(ID_RENDEROPTIONS_RENDERTRACE, OnRenderoptionsRendertrace)
	ON_UPDATE_COMMAND_UI(ID_RENDEROPTIONS_RENDERTRACE, OnUpdateRenderoptionsRendertrace)
	ON_COMMAND(ID_COLLISIONDETECT_TYPE1, OnCollisiondetectType1)
	ON_UPDATE_COMMAND_UI(ID_COLLISIONDETECT_TYPE1, OnUpdateCollisiondetectType1)
	ON_COMMAND(ID_COLLISIONDETECT_TYPE2, OnCollisiondetectType2)
	ON_UPDATE_COMMAND_UI(ID_COLLISIONDETECT_TYPE2, OnUpdateCollisiondetectType2)
	ON_COMMAND(ID_COLLISIONDETECT_TYPE3, OnCollisiondetectType3)
	ON_UPDATE_COMMAND_UI(ID_COLLISIONDETECT_TYPE3, OnUpdateCollisiondetectType3)
	ON_COMMAND(ID_RENDEROPTIONS_RENDERSKELETON, OnRenderoptionsRenderskeleton)
	ON_UPDATE_COMMAND_UI(ID_RENDEROPTIONS_RENDERSKELETON, OnUpdateRenderoptionsRenderskeleton)
	ON_COMMAND(ID_RENDEROPTIONS_RENDERPLANE, OnRenderoptionsRenderplane)
	ON_UPDATE_COMMAND_UI(ID_RENDEROPTIONS_RENDERPLANE, OnUpdateRenderoptionsRenderplane)
	ON_COMMAND(ID_FILE_OPENRAN2CPS, OnFileOpenran2cps)
END_MESSAGE_MAP()

// CRanEditSkinPieceView construction/destruction

CRanEditSkinPieceView::CRanEditSkinPieceView()	:
	CD3DApplication(),
	m_bCreated(FALSE),
	m_hCursorDefault(NULL),
	m_pPiece( NULL ),
	m_vCOL( 0.0f, 0.0f, 0.0f ),
	m_dwCOL(COL_ERR),
	dwCOLTYPE(0),
	bRenderSkeleton( FALSE ),
	bRenderPlane( TRUE ),
	bMeshDetect( FALSE )
{
	// TODO: add construction code here
	memset(szAppPath, 0, sizeof(char) * (MAX_PATH));
	m_bUseDepthBuffer = TRUE;
	
	D3DXMatrixIdentity ( &m_matLocal );

	g_nITEMLEVEL = 4;
	g_bCHAR_EDIT_RUN = TRUE;

	bRenderTrace = TRUE;
	strBoneName = "";

	D3DXMatrixIdentity ( &matEdit );
}

CRanEditSkinPieceView::~CRanEditSkinPieceView()
{
}

BOOL CRanEditSkinPieceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRanEditSkinPieceView drawing

void CRanEditSkinPieceView::OnDraw(CDC* /*pDC*/)
{
	//CRanEditSkinPieceDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	// TODO: add draw code for native data here
}


// CRanEditSkinPieceView printing

//BOOL CRanEditSkinPieceView::OnPreparePrinting(CPrintInfo* pInfo)
//{
//	// default preparation
//	return DoPreparePrinting(pInfo);
//}
//
//void CRanEditSkinPieceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: add extra initialization before printing
//}
//
//void CRanEditSkinPieceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: add cleanup after printing
//}


// CRanEditSkinPieceView diagnostics

#ifdef _DEBUG
void CRanEditSkinPieceView::AssertValid() const
{
	CView::AssertValid();
}

void CRanEditSkinPieceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//CRanEditSkinPieceDoc* CRanEditSkinPieceView::GetDocument() const // non-debug version is inline
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRanEditSkinPieceDoc)));
//	return (CRanEditSkinPieceDoc*)m_pDocument;
//}
#endif //_DEBUG


// CRanEditSkinPieceView message handlers
CRanEditSkinPieceView * CRanEditSkinPieceView::GetView()
{
    CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

    CView * pView = pFrame->GetActiveView();

    if ( !pView )
        return NULL;

    // Fail if view is of wrong kind
    // (this could occur with splitter windows, or additional
    // views on a single document
    if ( ! pView->IsKindOf( RUNTIME_CLASS(CRanEditSkinPieceView) ) )
        return NULL;

    return (CRanEditSkinPieceView *) pView;
}

void CRanEditSkinPieceView::OnRenderoptionsRendertrace()
{
	bRenderTrace = !bRenderTrace;
}

void CRanEditSkinPieceView::OnUpdateRenderoptionsRendertrace(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	pCmdUI->SetCheck( bRenderTrace );
}

void CRanEditSkinPieceView::OnCollisiondetectType1()
{
	dwCOLTYPE = 0;
}

void CRanEditSkinPieceView::OnUpdateCollisiondetectType1(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	pCmdUI->SetCheck( dwCOLTYPE == 0 );
}

void CRanEditSkinPieceView::OnCollisiondetectType2()
{
	dwCOLTYPE = 1;
}

void CRanEditSkinPieceView::OnUpdateCollisiondetectType2(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	pCmdUI->SetCheck( dwCOLTYPE == 1 );
}

void CRanEditSkinPieceView::OnCollisiondetectType3()
{
	dwCOLTYPE = 2;
}

void CRanEditSkinPieceView::OnUpdateCollisiondetectType3(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	pCmdUI->SetCheck( dwCOLTYPE == 2 );
}

void CRanEditSkinPieceView::OnRenderoptionsRenderskeleton()
{
	bRenderSkeleton = !bRenderSkeleton;
}

void CRanEditSkinPieceView::OnUpdateRenderoptionsRenderskeleton(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	pCmdUI->SetCheck( bRenderSkeleton );
}

void CRanEditSkinPieceView::OnRenderoptionsRenderplane()
{
	bRenderPlane = !bRenderPlane;
}

void CRanEditSkinPieceView::OnUpdateRenderoptionsRenderplane(CCmdUI *pCmdUI)
{
	if ( pCmdUI )	pCmdUI->SetCheck( bRenderPlane );
}
