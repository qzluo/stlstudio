
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class CMainFrame : public CFrameWnd
{

protected: // create from serialization only
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

public:
    virtual ~CMainFrame();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
    CToolBar          m_wndToolBar;
    CToolBar          m_wndDisplayBar;
    CStatusBar        m_wndStatusBar;
    CImageList        m_fileToolImage;
    CImageList        m_displayImage;

private:
    void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
    void SetUpToolBar(CToolBar* Bar, UINT cx_btn, UINT cy_btn,
        UINT cx_img, UINT cy_img, const UINT* lpBtnIDArray,
        int nIDCount, CImageList* pImageList, UINT nIDResource);

    // Generated message map functions
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};

