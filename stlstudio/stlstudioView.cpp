
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

#include "define.h"
#include "Entity.h"
#include "studio_data.h"

#include "DlgSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CstlstudioView

IMPLEMENT_DYNCREATE(CstlstudioView, CGLView)

BEGIN_MESSAGE_MAP(CstlstudioView, CGLView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
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
    ON_COMMAND(ID_VIEW_SHOWWORKSTATION, &CstlstudioView::OnViewShowworkstation)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWWORKSTATION, &CstlstudioView::OnUpdateViewShowworkstation)
    ON_COMMAND(ID_VIEW_ZOOMWORKSTATION, &CstlstudioView::OnViewZoomworkstation)
    ON_COMMAND(ID_TOOLS_SETTINGS, &CstlstudioView::OnToolsSettings)
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

    setDragMove(TRUE);
    InitCfg();

    return 0;
}

void CstlstudioView::OnDestroy()
{
    SaveCfg();
    CGLView::OnDestroy();

    // TODO: Add your message handler code here
}

void CstlstudioView::InitCfg(void)
{
    //read work station parameters
    CString strTemp;
    CString strFileName;
    OPENGLDCPARAMS params = {};
    STUDIOPARAMS studioparams = {};

    GetModuleFileName(NULL, strFileName.GetBuffer(256), 256);
    strFileName.ReleaseBuffer();
    int nPos = strFileName.ReverseFind( _T('\\') );
    if( nPos < 0 )
        return ;

    strFileName = strFileName.Left( nPos ) + _T("\\") + CONFIG_FILE;

    //work station
    int ret = GetPrivateProfileInt(WORKSTATION, WS_VISIBLE, 1, strFileName);
    m_bIsShowWorkStation = ret ? TRUE : FALSE;
    GetPrivateProfileInt(WORKSTATION, WS_VISIBLE, 1, strFileName);

    GetPrivateProfileString(WORKSTATION, WS_LENGTH, WS_LENGTH_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    params.ws_length = _wtof(strTemp.GetBuffer());
    studioparams.ws_length = params.ws_length;

    GetPrivateProfileString(WORKSTATION, WS_WIDTH, WS_WIDTH_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    params.ws_width = _wtof(strTemp.GetBuffer());
    studioparams.ws_width = params.ws_width;

    GetPrivateProfileString(WORKSTATION, WS_HEIGHT, WS_HEIGHT_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    params.ws_height = _wtof(strTemp.GetBuffer());
    studioparams.ws_height = params.ws_height;

    GetPrivateProfileString(WORKSTATION, WS_GAP, WS_GAP_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    params.ws_gap = _wtof(strTemp.GetBuffer());
    studioparams.ws_gap = params.ws_gap;

    //coordinate axis
    GetPrivateProfileString(COORDAXIS, COORD_GAP, COORD_GAP_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    params.coord_gap = _wtof(strTemp.GetBuffer());
    studioparams.coord_gap = params.coord_gap;

    GetPrivateProfileString(COORDAXIS, COORD_AXIS_LEN, COORD_AXIS_LEN_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    params.axis_len = _wtof(strTemp.GetBuffer());
    studioparams.axis_len = params.axis_len;

    GetPrivateProfileString(COORDAXIS, COORD_AXIS_LINEWIDTH, COORD_AXIS_LINEWIDTH_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    params.axis_linewidth = _wtof(strTemp.GetBuffer());
    studioparams.axis_linewidth = params.axis_linewidth;

    GetPrivateProfileString(ALLIGN, ALLIGN_MARGIN, ALLIGN_MARGIN_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    studioparams.align_margin = _wtof(strTemp.GetBuffer());

    GetPrivateProfileString(ALLIGN, ALLIGN_GAP, ALLIGN_GAP_DEF,
        strTemp.GetBuffer(32), 32, strFileName);
    strTemp.ReleaseBuffer();
    studioparams.align_gap = _wtof(strTemp.GetBuffer());

    SetStduioParams(&studioparams);

    m_pGLDC->SetParams(&params);
}

void CstlstudioView::SaveCfg(void)
{
    OPENGLDCPARAMS params = {};
    if (!m_pGLDC)
        return;

    m_pGLDC->GetParams(&params);

    STUDIOPARAMS studioparams = {};
    if (GetStduioParams(&studioparams))
        return;

    CString strTemp;
    CString strFileName;

    GetModuleFileName(NULL, strFileName.GetBuffer(256), 256);
    strFileName.ReleaseBuffer();
    int nPos = strFileName.ReverseFind( _T('\\') );
    if( nPos < 0 )
        return ;

    strFileName = strFileName.Left( nPos ) + _T("\\") + CONFIG_FILE;

    //work station
    int iVisible = m_bIsShowWorkStation ? 1 : 0;
    strTemp.Format(_T("%d"), iVisible);
    WritePrivateProfileString(WORKSTATION, WS_VISIBLE, strTemp, strFileName);

    strTemp.Format(_T("%.2f"), params.ws_length);
    WritePrivateProfileString(WORKSTATION, WS_LENGTH, strTemp, strFileName);

    strTemp.Format(_T("%.2f"), params.ws_width);
    WritePrivateProfileString(WORKSTATION, WS_WIDTH, strTemp, strFileName);

    strTemp.Format(_T("%.2f"), params.ws_height);
    WritePrivateProfileString(WORKSTATION, WS_HEIGHT, strTemp, strFileName);

    strTemp.Format(_T("%.2f"), params.ws_gap);
    WritePrivateProfileString(WORKSTATION, WS_GAP, strTemp, strFileName);

    //coordinate axis
    strTemp.Format(_T("%.2f"), params.coord_gap);
    WritePrivateProfileString(COORDAXIS, COORD_GAP, strTemp, strFileName);

    strTemp.Format(_T("%.2f"), params.axis_len);
    WritePrivateProfileString(COORDAXIS, COORD_AXIS_LEN, strTemp, strFileName);

    strTemp.Format(_T("%.2f"), params.axis_linewidth);
    WritePrivateProfileString(COORDAXIS, COORD_AXIS_LINEWIDTH, strTemp, strFileName);

    strTemp.Format(_T("%.2f"), studioparams.align_margin);
    WritePrivateProfileString(ALLIGN, ALLIGN_MARGIN, strTemp, strFileName);

    strTemp.Format(_T("%.2f"), studioparams.align_gap);
    WritePrivateProfileString(ALLIGN, ALLIGN_GAP, strTemp, strFileName);
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

    if(m_pGLDC) {
        //begin selection
        m_pGLDC->BeginSelection(point.x, point.y);

        //render
        DrawParts(m_pGLDC);

        //end selection
        hits = m_pGLDC->EndSelection(items);

        //handle selection
        /*
        If ctrl is entered at the same time, new hit items will be selected
        and old hitted items would stay old state.
        If ctrl is not entered at the same time, the items which are not hitted
        would be set not selected state. The items which are hitted would 
        reverse state.
        */
        if (MK_CONTROL & nFlags) {
            if (hits) {
                for (int i = 0; i < hits; i++) {
                    pStlModel = (CSTLModel*)items[i];
                    pStlModel->ReverseHighLight();
                }

                Invalidate(FALSE);
            }
        }
        else {
            //get old status
            BOOL* bNewStatus = NULL;
            if (hits) {
                ASSERT(hits > 0);
                bNewStatus = new BOOL[hits];
                for (int i = 0; i < hits; i++) {
                    pStlModel = (CSTLModel*)items[i];
                    bNewStatus[i] = !(pStlModel->IsHighLight());
                }
            }

            //clear all status
            pDoc->m_Part.SetHighLightStat(FALSE);

            //set new status
            if (hits) {
                for (int i = 0; i < hits; i++) {
                    pStlModel = (CSTLModel*)items[i];
                    pStlModel->SetHighLight(bNewStatus[i]);
                }

                delete bNewStatus;
            }
            Invalidate(FALSE);
        }
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

void CstlstudioView::OnToolsSettings()
{
    // TODO: Add your command handler code here
    OPENGLDCPARAMS gl_params = {};
    if (!m_pGLDC)
        return;

    m_pGLDC->GetParams(&gl_params);

    STUDIOPARAMS studioparams = {};
    if (GetStduioParams(&studioparams))
        return;

    CDlgSettings dlg;
    SETTINGSPARAMS params = {0};
    params.ws_length = studioparams.ws_length;
    params.ws_width = studioparams.ws_width;
    params.ws_height = studioparams.ws_height;
    params.align_margin = studioparams.align_margin;
    params.align_gap = studioparams.align_gap;

    dlg.m_params = &params;

    if (IDOK == dlg.DoModal()) {
        if (studioparams.ws_length == params.ws_length &&
            studioparams.ws_width == params.ws_width &&
            studioparams.ws_height == params.ws_height &&
            studioparams.align_margin == params.align_margin &&
            studioparams.align_gap == params.align_gap)
            return;

        studioparams.ws_length = params.ws_length;
        studioparams.ws_width = params.ws_width;
        studioparams.ws_height = params.ws_height;
        studioparams.align_margin = params.align_margin;
        studioparams.align_gap = params.align_gap;

        if (SetStduioParams(&studioparams))
            return;

        gl_params.ws_length = params.ws_length;
        gl_params.ws_width = params.ws_width;
        gl_params.ws_height = params.ws_height;

        m_pGLDC->SetParams(&gl_params);

        Invalidate();
    }
}
