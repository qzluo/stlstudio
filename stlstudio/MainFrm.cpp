
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "stlstudio.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

static UINT display_toolbar_button[] = {
    ID_VIEW_FRONT_VIEW,
    ID_VIEW_BACK_VIEW,
    ID_VIEW_LEFT_VIEW,
    ID_VIEW_RIGHT_VIEW,
    ID_VIEW_TOP_VIEW,
    ID_VIEW_BOTTOM_VIEW,
    ID_VIEW_SW,
    ID_SEPARATOR,
    ID_VIEW_ZOOM_IN,
    ID_VIEW_ZOOM_OUT,
    ID_VIEW_ZOOMALL,
    ID_SEPARATOR,
    ID_EDIT_MOVEOBJECT,
    ID_EDIT_ROTATEOBJECT,
    ID_EDIT_SCALEOBJECT,
    ID_SEPARATOR,
    ID_VIEW_SHADING
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    //file toolbar
    CBitmap bm;
    bm.LoadBitmap(IDB_BITMAP1);
    m_fileToolImage.Create(40, 40, TRUE | ILC_COLOR24, 24, 0);
    m_fileToolImage.Add(&bm, (CBitmap*)NULL);
    bm.Detach();
    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_fileToolImage);

    if (!m_wndDisplayBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)) {
            TRACE0("Failed to create toolbar window/n");
            return -1;
    }

    SetUpToolBar(&m_wndDisplayBar, 50, 50, 40, 40, display_toolbar_button,
        sizeof(display_toolbar_button) / sizeof(display_toolbar_button[0]),
        &m_displayImage, IDB_BITMAP2);

    if (!m_wndStatusBar.Create(this)) {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }
    m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

    // TODO: Delete these three lines if you don't want the toolbar to be dockable
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    m_wndDisplayBar.EnableDocking(CBRS_ALIGN_ANY);
    DockControlBarLeftOf(&m_wndDisplayBar,&m_wndToolBar);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/*-------------------------------------------------------------------
  Purpose: set toolbar image
  Parameters:
     Bar -- toolbar to set
     cx_btn -- tool bar button width
     cy_btn -- tool bar button height
     cx_img -- tool bar image width
     cy_img -- tool bar image height
     lpBtnIDArray -- tool bar command id
     nIDCount -- tool bar command count
     pImageList -- imagelist to save the images
     nIDResource -- image resource ID
-------------------------------------------------------------------*/
void CMainFrame::SetUpToolBar(CToolBar* Bar,
                              UINT cx_btn, UINT cy_btn,
                              UINT cx_img, UINT cy_img,
                              const UINT* lpBtnIDArray, int nIDCount,
                              CImageList* pImageList, UINT nIDResource)
{
    SIZE szButton;
    szButton.cx = cx_btn;
    szButton.cy = cy_btn;
    SIZE szImage;
    szImage.cx = cx_img;
    szImage.cy = cy_img;
    Bar->SetSizes(szButton, szImage);

    Bar->SetButtons(lpBtnIDArray, nIDCount);

    //设置ToolBar的图标列表
    CBitmap bm;
    m_displayImage.Create(cx_img, cy_img, TRUE | ILC_COLOR24, 24, 0);
    bm.LoadBitmap(nIDResource);
    m_displayImage.Add(&bm, (CBitmap*)NULL);
    bm.Detach();

    Bar->GetToolBarCtrl().SetImageList(&m_displayImage);
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
    CRect rect;
    DWORD dw;
    UINT n;

    RecalcLayout();
    LeftOf->GetWindowRect(&rect);
    rect.OffsetRect(1,0);
    dw=LeftOf->GetBarStyle();
    n = 0;
    n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
    n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
    n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
    n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
    DockControlBar(Bar,n,&rect);
}