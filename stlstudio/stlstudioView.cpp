
// stlstudioView.cpp : implementation of the CstlstudioView class
//

#include "stdafx.h"

#include <gl/gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#ifndef SHARED_HANDLERS
#include "stlstudio.h"
#endif

#include "stlstudioDoc.h"
#include "stlstudioView.h"

#include "Entity.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CstlstudioView

IMPLEMENT_DYNCREATE(CstlstudioView, CGLView)

BEGIN_MESSAGE_MAP(CstlstudioView, CGLView)
    ON_WM_CREATE()
    ON_COMMAND(ID_VIEW_ZOOM_IN, &CstlstudioView::OnViewZoomIn)
    ON_COMMAND(ID_VIEW_ZOOM_OUT, &CstlstudioView::OnViewZoomOut)
    ON_COMMAND(ID_VIEW_FRONT_VIEW, &CstlstudioView::OnViewFrontView)
    ON_COMMAND(ID_VIEW_BACK_VIEW, &CstlstudioView::OnViewBackView)
    ON_COMMAND(ID_VIEW_TOP_VIEW, &CstlstudioView::OnViewTopView)
    ON_COMMAND(ID_VIEW_BOTTOM_VIEW, &CstlstudioView::OnViewBottomView)
    ON_COMMAND(ID_VIEW_LEFT_VIEW, &CstlstudioView::OnViewLeftView)
    ON_COMMAND(ID_VIEW_RIGHT_VIEW, &CstlstudioView::OnViewRightView)
    ON_COMMAND(ID_VIEW_SW, &CstlstudioView::OnViewSw)
    ON_COMMAND(ID_VIEW_SE, &CstlstudioView::OnViewSe)
    ON_COMMAND(ID_VIEW_NW, &CstlstudioView::OnViewNw)
    ON_COMMAND(ID_VIEW_NE, &CstlstudioView::OnViewNe)
    ON_COMMAND(ID_VIEW_ZOOMALL, &CstlstudioView::OnViewZoomall)
    ON_COMMAND(ID_VIEW_SHADING, &CstlstudioView::OnViewShading)
    ON_WM_LBUTTONDOWN()
    ON_COMMAND(ID_EDIT_SELECTALL, &CstlstudioView::OnEditSelectall)
    ON_COMMAND(ID_EDIT_SELECTNONE, &CstlstudioView::OnEditSelectnone)
    ON_WM_LBUTTONUP()
    ON_COMMAND(ID_VIEW_SHOWWORKSTATION, &CstlstudioView::OnViewShowworkstation)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWWORKSTATION, &CstlstudioView::OnUpdateViewShowworkstation)
    ON_COMMAND(ID_VIEW_ZOOMWORKSTATION, &CstlstudioView::OnViewZoomworkstation)
END_MESSAGE_MAP()

// CstlstudioView construction/destruction

CstlstudioView::CstlstudioView()
{
    m_bIsShowWorkStation = FALSE;
}

CstlstudioView::~CstlstudioView()
{
}

BOOL CstlstudioView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CGLView::PreCreateWindow(cs);
}


// CstlstudioView diagnostics

#ifdef _DEBUG
void CstlstudioView::AssertValid() const
{
    CView::AssertValid();
}

void CstlstudioView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CstlstudioDoc* CstlstudioView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CstlstudioDoc)));
    return (CstlstudioDoc*)m_pDocument;
}
#endif //_DEBUG

int CstlstudioView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CGLView::OnCreate(lpCreateStruct) == -1)
        return -1;

    setDragMove(FALSE);

    return 0;
}

void CstlstudioView::RenderScene(COpenGLDC* pDC)
{
    pDC->DrawCoord();
    if (m_bIsShowWorkStation)
        pDC->DrawWorkStation();
    DrawParts(pDC);
}

void CstlstudioView::DrawParts(COpenGLDC* pDC)
{
    CstlstudioDoc* pDoc = GetDocument();
    ASSERT(pDoc);

    if(!pDoc->m_Part.IsEmpty())
        pDoc->m_Part.Draw(pDC);
}

BOOL CstlstudioView::GetBox(double& x0, double& y0, double& z0, double& x1, double& y1, double& z1)
{
    CstlstudioDoc* pDoc = GetDocument();
    ASSERT(pDoc);

    if(!pDoc->m_Part.IsEmpty()){
        CBox3D box;
        if(pDoc->m_Part.GetBox(box)) {
            x0 = box.x0; y0 = box.y0;	z0 = box.z0;
            x1 = box.x1; y1 = box.y1;   z1 = box.z1;
            return TRUE;
        }
    }

    return FALSE;
}

void CstlstudioView::Zoom_STLModel(void)
{
    CstlstudioDoc* pDoc = GetDocument();
    ASSERT(pDoc);

    double x0, y0, z0, x1, y1, z1;
    if (pDoc->m_Part.GetActiveBox(x0, y0, z0, x1, y1, z1)) {
        m_pGLDC->m_Camera.zoom_all(x0, y0, z0, x1, y1, z1);
        Invalidate();
    }
}

void CstlstudioView::OnViewZoomIn()
{
    Zoom(0.9);
}

void CstlstudioView::OnViewZoomOut()
{
    // TODO: Add your command handler code here
    Zoom(1.1);
}

void CstlstudioView::OnViewFrontView()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_FRONT);
}

void CstlstudioView::OnViewBackView()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_BACK);
}

void CstlstudioView::OnViewTopView()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_TOP);
}

void CstlstudioView::OnViewBottomView()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_BOTTOM);
}

void CstlstudioView::OnViewLeftView()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_LEFT);
}

void CstlstudioView::OnViewRightView()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_RIGHT);
}

void CstlstudioView::OnViewSw()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_SW_ISOMETRIC);
}

void CstlstudioView::OnViewSe()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_SE_ISOMETRIC);
}

void CstlstudioView::OnViewNw()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_NW_ISOMETRIC);
}

void CstlstudioView::OnViewNe()
{
    // TODO: Add your command handler code here
    OnViewType(VIEW_NE_ISOMETRIC);
}

void CstlstudioView::OnViewZoomall()
{
    // TODO: Add your command handler code here
    ZoomAll();
}

void CstlstudioView::OnViewShading()
{
    // TODO: Add your command handler code here
    m_pGLDC->Shading(!m_pGLDC->IsShading());
    Invalidate();
}

void CstlstudioView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    int hits = 0;
    UINT items[16] = {};
    CSTLModel* pStlModel = NULL;
    CstlstudioDoc* pDoc = GetDocument();
    ASSERT(pDoc);

    setDragMove(TRUE);

    if(m_pGLDC) {
        //begin selection
        m_pGLDC->BeginSelection(point.x, point.y);

        //render
        DrawParts(m_pGLDC);

        //end selection
        hits = m_pGLDC->EndSelection(items);

        //handle selection
        if (hits) {
            for (int i = 0; i < hits; i++) {
                pStlModel = (CSTLModel*)items[i];
                pStlModel->ReverseHighLight();
            }
        }
        else
            pDoc->m_Part.SetHighLightStat(FALSE);

        Invalidate(FALSE);
    }

    CGLView::OnLButtonDown(nFlags, point);
}

void CstlstudioView::OnEditSelectall()
{
    // TODO: Add your command handler code here
    CstlstudioDoc* pDoc = GetDocument();
    ASSERT(pDoc);

    if(!pDoc->m_Part.IsEmpty()) {
        pDoc->m_Part.SetHighLightStat(TRUE);
        Invalidate();
    }
}

void CstlstudioView::OnEditSelectnone()
{
    // TODO: Add your command handler code here
    CstlstudioDoc* pDoc = GetDocument();
    ASSERT(pDoc);

    if(!pDoc->m_Part.IsEmpty()) {
        pDoc->m_Part.SetHighLightStat(FALSE);
        Invalidate();
    }
}

void CstlstudioView::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    setDragMove(FALSE);

    CGLView::OnLButtonUp(nFlags, point);
}

void CstlstudioView::OnViewShowworkstation()
{
    // TODO: Add your command handler code here
    m_bIsShowWorkStation = !m_bIsShowWorkStation;
    Invalidate();
}

void CstlstudioView::OnUpdateViewShowworkstation(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->SetCheck(m_bIsShowWorkStation);
}


void CstlstudioView::OnViewZoomworkstation()
{
    // TODO: Add your command handler code here
    if(m_pGLDC) {
        m_pGLDC->ZoomWorkStation();
        Invalidate();
    }
}
