#ifndef __GLVIEW_H__
#define __GLVIEW_H__

#include "OpenGLDC.h"

class CGLView : public CView
{
protected:
    COpenGLDC*	m_pGLDC;
    BOOL		m_bDragMove;	//if on, drag move is activated to response mouse move
    CPoint		m_lastPoint;	//cache for the most recent mouse position

protected: // create from serialization only
    CGLView();
    DECLARE_DYNCREATE(CGLView)

public:
    virtual ~CGLView();

        //{{AFX_VIRTUAL(CGLView)
    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

    virtual void RenderScene(COpenGLDC* pDC);
    virtual BOOL GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1);

        // Generated message map functions
protected:
    //{{AFX_MSG(CGLView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    void ZoomAll();
    void Zoom(double dScale);
    void OnViewType(UINT type);
    void setDragMove(BOOL bOn);
};

#endif  //__GLVIEW_H__