
// stlstudioDoc.h : interface of the CstlstudioDoc class
//


#pragma once

#include "Entity.h"


class CstlstudioDoc : public CDocument
{
protected: // create from serialization only
	CstlstudioDoc();
	DECLARE_DYNCREATE(CstlstudioDoc)

// Attributes
public:
    CPart	m_Part;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CstlstudioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CView* GetView(CRuntimeClass* pClass);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
    afx_msg void OnFileNew();
    afx_msg void OnFileOpen();
    afx_msg void OnFileSave();
    afx_msg void OnEditMoveobject();
    afx_msg void OnEditRotateobject();
    afx_msg void OnEditScaleobject();
    afx_msg void OnFileOpenstlPart();
    afx_msg void OnFileSavestlPart();
    afx_msg void OnFileRemovestlpart();
};
