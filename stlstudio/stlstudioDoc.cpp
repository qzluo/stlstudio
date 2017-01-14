
// stlstudioDoc.cpp : implementation of the CstlstudioDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "stlstudio.h"
#endif

#include "stlstudioDoc.h"
#include "stlstudioView.h"

#include "DlgMoveObject.h"
#include "DlgRotateObject.h"
#include "DlgScaleObject.h"
#include "DlgAutoPlacement.h"

#include "studio_data.h"


#include <propkey.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CstlstudioDoc

IMPLEMENT_DYNCREATE(CstlstudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CstlstudioDoc, CDocument)
    ON_COMMAND(ID_FILE_NEW, &CstlstudioDoc::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CstlstudioDoc::OnFileOpen)
    ON_COMMAND(ID_FILE_SAVE, &CstlstudioDoc::OnFileSave)
    ON_COMMAND(ID_EDIT_MOVEOBJECT, &CstlstudioDoc::OnEditMoveobject)
    ON_COMMAND(ID_EDIT_ROTATEOBJECT, &CstlstudioDoc::OnEditRotateobject)
    ON_COMMAND(ID_EDIT_SCALEOBJECT, &CstlstudioDoc::OnEditScaleobject)
    ON_COMMAND(ID_FILE_OPENSTL_PART, &CstlstudioDoc::OnFileOpenstlPart)
    ON_COMMAND(ID_FILE_SAVESTL_PART, &CstlstudioDoc::OnFileSavestlPart)
    ON_COMMAND(ID_FILE_REMOVESTLPART, &CstlstudioDoc::OnFileRemovestlpart)
    ON_COMMAND(ID_UNITCONVERSION_MMTOINCH, &CstlstudioDoc::OnUnitconversionMmtoinch)
    ON_COMMAND(ID_UNITCONVERSION_INCHTOMM, &CstlstudioDoc::OnUnitconversionInchtomm)
    ON_COMMAND(ID_EDIT_ALIGNOBJECT, &CstlstudioDoc::OnEditAlignobject)
    ON_COMMAND(ID_EXPORTSTLPART_ASSTL, &CstlstudioDoc::OnExportstlpartAsstl)
END_MESSAGE_MAP()


// CstlstudioDoc construction/destruction

CstlstudioDoc::CstlstudioDoc()
{
	// TODO: add one-time construction code here

}

CstlstudioDoc::~CstlstudioDoc()
{
}

BOOL CstlstudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CstlstudioDoc serialization

void CstlstudioDoc::Serialize(CArchive& ar)
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

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CstlstudioDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CstlstudioDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CstlstudioDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CstlstudioDoc diagnostics

#ifdef _DEBUG
void CstlstudioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CstlstudioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CstlstudioDoc commands

void CstlstudioDoc::OnFileNew()
{
    // TODO: Add your command handler code here
    m_Part.RemoveAllEntity();
    UpdateAllViews(NULL);
}

void CstlstudioDoc::OnFileOpen()
{
    // TODO: Add your command handler code here
    CFileDialog dlg(TRUE,_T("mdl"),NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        _T("Model File(*.mdl)|*.mdl||"), NULL );

    if (dlg.DoModal()==IDOK) {
        CString strName = dlg.GetPathName();
        m_Part.LoadModel(strName);

        //show all the stlmodel in the window
        CstlstudioView* p_view = (CstlstudioView*)GetView(RUNTIME_CLASS(CstlstudioView));
        if (p_view)
            p_view->OnViewZoomall();

        UpdateAllViews(NULL);
    }
}

void CstlstudioDoc::OnFileSave()
{
    // TODO: Add your command handler code here
    if (m_Part.IsEmpty())
        return;

    CFileDialog dlg(FALSE, _T("mdl"), _T("default.mdl"),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        _T("Model File(*.mdl)|*.mdl||"), NULL );

    CString filePath;

    if(dlg.DoModal()==IDOK) {
        filePath = dlg.GetPathName();
        m_Part.ExportModel(filePath);
    }
}

CView* CstlstudioDoc::GetView(CRuntimeClass* pClass)
{
    CView* pView = NULL;
    POSITION pos = GetFirstViewPosition();
    while (pos != NULL) {
        pView = GetNextView(pos);
        if(!pView->IsKindOf(pClass))
            break;
    }

    return pView;
}

void CstlstudioDoc::OnEditMoveobject()
{
    if (m_Part.IsEmpty())
        return;

    //get positon
    double x0,y0,z0,x1,y1,z1;
    if (!m_Part.GetActiveBox(x0,y0,z0,x1,y1,z1))
        return ;

    CDlgMoveObject dlg;
    MOVEPARAMS params = {};
    params.x = x0;
    params.y = y0;
    params.z = z0;

    dlg.m_params = &params;
    if (IDOK == dlg.DoModal()) {
        if (m_Part.MoveObject(params.dx, params.dy, params.dz) == 0)
            UpdateAllViews(NULL);
    }
}

void CstlstudioDoc::OnEditRotateobject()
{
    if (m_Part.IsEmpty())
        return;

    VECTOR3D bv = {0, 0, 0};
    CDlgRotateObject dlg;
    ROTATEPARAMS params = {0, 0};
    dlg.m_params = &params;

    if (IDOK == dlg.DoModal()) {
        ASSERT(params.axis >= AXIS_X && params.axis <= AXIS_Z);

        if (params.axis == AXIS_X)
            bv.dx = 1;
        else if (params.axis == AXIS_Y)
            bv.dy = 1;
        else if (params.axis == AXIS_Z)
            bv.dz = 1;

        if (m_Part.RotateObject(bv, params.angle) == 0)
            UpdateAllViews(NULL);
    }
}

void CstlstudioDoc::OnEditScaleobject()
{
    if (m_Part.IsEmpty())
        return;

    DlgScaleObject dlg;
    SCALEPARAMS params;
    dlg.m_params = &params;

    if (IDOK == dlg.DoModal()) {
        if (m_Part.ScaleObject(params.fx, params.fy, params.fz) == 0)
            UpdateAllViews(NULL);
    }
}

void CstlstudioDoc::OnFileOpenstlPart()
{
    // TODO: Add your command handler code here
    CFileDialog dlg(TRUE,_T("stl"), NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        _T("Stereo Lithograpic File(*.stl)|*.stl||"), NULL);

    if(dlg.DoModal()==IDOK){
        CString strName = dlg.GetPathName();

        if (m_Part.LoadSTLFile(strName) == 0) {
            CstlstudioView* p_view = (CstlstudioView*)GetView(RUNTIME_CLASS(CstlstudioView));
            if (p_view)
                p_view->Zoom_STLModel();
            UpdateAllViews(NULL);
        }
    }
}

void CstlstudioDoc::OnFileSavestlPart()
{
    // TODO: Add your command handler code here
    ExportStlFile(STL_FILE_FORMAT_ASCII);
}

void CstlstudioDoc::OnExportstlpartAsstl()
{
    // TODO: Add your command handler code here
    ExportStlFile(STL_FILE_FORMAT_BIN);
}

void CstlstudioDoc::ExportStlFile(int format)
{
    // TODO: Add your command handler code here
    if (m_Part.IsEmpty())
        return;

    if (m_Part.GetSelectedObjectCount() != 1) {
        MessageBox(NULL, _T("To export stl part, one part and only one should be selected."),
            _T("Export STL"), MB_OK );
        return;
    }

    CFileDialog dlg(FALSE, _T("stl"), _T("default.stl"),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        _T("Stereo Lithograpic File(*.stl)|*.stl||"), NULL );

    CString filePath;

    if(dlg.DoModal()==IDOK) {
        filePath = dlg.GetPathName();
        m_Part.ExportSTLFile(filePath, format);
    }
}

void CstlstudioDoc::OnFileRemovestlpart()
{
    // TODO: Add your command handler code here
    int count = m_Part.GetSelectedObjectCount();
    if (count < 1)
        return;

    ASSERT(count >= 1);

    CString strMsg;
    strMsg = count == 1 ? 
        _T("Are you sure to remove the selected object?") :
        _T("Are you sure to remove the selected objects?");

    int ret = MessageBox(NULL, strMsg, _T("Remove Objects"), MB_YESNO);

    if (ret == IDYES) {
        if (m_Part.RemoveSelectedObjects() == 0)
            UpdateAllViews(NULL);
    }
}

void CstlstudioDoc::OnUnitconversionMmtoinch()
{
    // TODO: Add your command handler code here
    if (m_Part.UnitConvertion(1 / 25.4) == 0)
        UpdateAllViews(NULL);
}

void CstlstudioDoc::OnUnitconversionInchtomm()
{
    // TODO: Add your command handler code here
    if (m_Part.UnitConvertion(25.4) == 0)
        UpdateAllViews(NULL);
}

void CstlstudioDoc::OnEditAlignobject()
{
    // TODO: Add your command handler code here
    if (m_Part.IsEmpty())
        return;

    STUDIOPARAMS studioparams = {};
    GetStduioParams(&studioparams);

    CDlgAutoPlacement dlg;
    PLACEMENTPARAMS params = {};
    params.margin = studioparams.align_margin;
    params.gap = studioparams.align_gap;
    params.axis = PLACEMENT_AXIS_ALONG_Y;

    dlg.m_params = &params;

    if (IDOK == dlg.DoModal()) {
        if (m_Part.AlignPartsAlong(studioparams.ws_length, studioparams.ws_width,
            params.margin, params.gap, params.axis) != 0)
            MessageBox(NULL, _T("Not all the objects can by placed in the work station."), 
            _T("Align Object"),NULL);

        UpdateAllViews(NULL);
    }
}

