// DlgAutoPlacement.cpp : implementation file
//

#include "stdafx.h"
#include "stlstudio.h"
#include "DlgAutoPlacement.h"
#include "afxdialogex.h"


// CDlgAutoPlacement dialog

IMPLEMENT_DYNAMIC(CDlgAutoPlacement, CDialogEx)

CDlgAutoPlacement::CDlgAutoPlacement(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAutoPlacement::IDD, pParent)
{
    m_params = NULL;
}

CDlgAutoPlacement::~CDlgAutoPlacement()
{
}

void CDlgAutoPlacement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAutoPlacement, CDialogEx)
END_MESSAGE_MAP()


// CDlgAutoPlacement message handlers


BOOL CDlgAutoPlacement::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    InitData();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAutoPlacement::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    SaveData();

    CDialogEx::OnOK();
}

void CDlgAutoPlacement::InitData(void)
{
    CString strTemp;
    ASSERT(m_params);

    strTemp.Format(_T("%.02f"), m_params->margin);
    SetDlgItemText(IDC_EDIT_MARGIN, strTemp);
    strTemp.Format(_T("%.02f"), m_params->gap);
    SetDlgItemText(IDC_EDIT_GAP, strTemp);

    ASSERT(m_params->axis >= PLACEMENT_AXIS_ALONG_X &&
        m_params->axis <= PLACEMENT_AXIS_ALONG_Y);

    m_params->axis == PLACEMENT_AXIS_ALONG_X ?
    ((CButton*)GetDlgItem(IDC_RADIO_ALONG_X))->SetCheck(TRUE) :
    ((CButton*)GetDlgItem(IDC_RADIO_ALONG_Y))->SetCheck(TRUE);
}

void CDlgAutoPlacement::SaveData(void)
{
    CString strTemp;

    ASSERT(m_params);

    GetDlgItemText(IDC_EDIT_MARGIN, strTemp);
    m_params->margin = _tstof(strTemp);

    GetDlgItemText(IDC_EDIT_GAP, strTemp);
    m_params->gap = _tstof(strTemp);

    m_params->axis = ((CButton*)GetDlgItem(IDC_RADIO_ALONG_X))->GetCheck() ?
        PLACEMENT_AXIS_ALONG_X : PLACEMENT_AXIS_ALONG_Y;
}