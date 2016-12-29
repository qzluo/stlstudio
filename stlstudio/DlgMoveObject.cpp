// DlgMoveObject.cpp : implementation file
//

#include "stdafx.h"
#include "stlstudio.h"
#include "DlgMoveObject.h"
#include "afxdialogex.h"


// CDlgMoveObject dialog

IMPLEMENT_DYNAMIC(CDlgMoveObject, CDialogEx)

CDlgMoveObject::CDlgMoveObject(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMoveObject::IDD, pParent)
{
    m_params = NULL;
}

CDlgMoveObject::~CDlgMoveObject()
{
}

void CDlgMoveObject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMoveObject, CDialogEx)
END_MESSAGE_MAP()


// CDlgMoveObject message handlers


BOOL CDlgMoveObject::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    InitData();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMoveObject::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    SaveData();

    CDialogEx::OnOK();
}

void CDlgMoveObject::InitData()
{
    CString strTemp;
    ASSERT(m_params);

    strTemp.Format(_T("X: %.2f \\ Y: %.2f \\ Z: %.2f"),
        m_params->x, m_params->y, m_params->z);
    SetDlgItemText(IDC_STATIC_POS, strTemp);

    strTemp.Format(_T("%.02f"), 0.0);
    SetDlgItemText(IDC_EDIT_X, strTemp);
    SetDlgItemText(IDC_EDIT_Y, strTemp);
    SetDlgItemText(IDC_EDIT_Z, strTemp);
}

void CDlgMoveObject::SaveData()
{
    CString strTemp;

    ASSERT(m_params);

    GetDlgItemText(IDC_EDIT_X, strTemp);
    m_params->dx = _tstof(strTemp);

    GetDlgItemText(IDC_EDIT_Y, strTemp);
    m_params->dy = _tstof(strTemp);

    GetDlgItemText(IDC_EDIT_Z, strTemp);
    m_params->dz = _tstof(strTemp);

    if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()) {
        m_params->dx -= m_params->x;
        m_params->dy -= m_params->y;
        m_params->dz -= m_params->z;
    }
}