// RanEditSkinPieceDoc.cpp : implementation of the CRanEditSkinPieceDoc class
//

#include "stdafx.h"
#include "RanEditSkinPiece.h"

#include "RanEditSkinPieceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRanEditSkinPieceDoc

IMPLEMENT_DYNCREATE(CRanEditSkinPieceDoc, CDocument)

BEGIN_MESSAGE_MAP(CRanEditSkinPieceDoc, CDocument)
END_MESSAGE_MAP()


// CRanEditSkinPieceDoc construction/destruction

CRanEditSkinPieceDoc::CRanEditSkinPieceDoc()
{
	// TODO: add one-time construction code here

}

CRanEditSkinPieceDoc::~CRanEditSkinPieceDoc()
{
}

BOOL CRanEditSkinPieceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CRanEditSkinPieceDoc serialization

void CRanEditSkinPieceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CRanEditSkinPieceDoc diagnostics

#ifdef _DEBUG
void CRanEditSkinPieceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRanEditSkinPieceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRanEditSkinPieceDoc commands
