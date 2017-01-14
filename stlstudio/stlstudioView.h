
// stlstudioView.h : interface of the CstlstudioView class
//

#pragma once

#include "glView.h"

class CstlstudioView : public CGLView
{
private:
    BOOL m_bIsShowWorkStation;

protected: // create from serialization only
    CstlstudioView();
    DECLARE_DYNCREATE(CstlstudioView)

public:
    virtual ~CstlstudioView();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    CstlstudioDoc* GetDocument() const;

    void Zoom_STLModel(void);

protected:
    virtual void RenderScene(COpenGLDC* pDC);
    virtual BOOL GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1);

    void DrawParts(COpenGLDC* pDC);
    void InitCfg(void);
    void SaveCfg(void);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnViewZoomIn();
    afx_msg void OnViewZoomOut();
    afx_msg void OnViewFrontView();
    afx_msg void OnViewBackView();
    afx_msg void OnViewTopView();
    afx_msg void OnViewBottomView();
    afx_msg void OnViewLeftView();
    afx_msg void OnViewRightView();
    afx_msg void OnViewSw();
    afx_msg void OnViewSe();
    afx_msg void OnViewNw();
    afx_msg void OnViewNe();
    afx_msg void OnViewZoomall();
    afx_msg void OnViewShading();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnEditSelectall();
    afx_msg void OnEditSelectnone();
    afx_msg void OnViewShowworkstation();
    afx_msg void OnUpdateViewShowworkstation(CCmdUI *pCmdUI);
    afx_msg void OnViewZoomworkstation();
    afx_msg void OnToolsSettings();
};

#ifndef _DEBUG  // debug version in stlstudioView.cpp
inline CstlstudioDoc* CstlstudioView::GetDocument() const
{ return reinterpret_cast<CstlstudioDoc*>(m_pDocument); }
#endif

