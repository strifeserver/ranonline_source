// RanEditSkinPieceDoc.h : interface of the CRanEditSkinPieceDoc class
//


#pragma once

class CRanEditSkinPieceDoc : public CDocument
{
protected: // create from serialization only
	CRanEditSkinPieceDoc();
	DECLARE_DYNCREATE(CRanEditSkinPieceDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CRanEditSkinPieceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


