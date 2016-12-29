// DlgScaleObject.cpp : implementation file
//

#include "stdafx.h"
#include "stlstudio.h"
#include "DlgScaleObject.h"
#include "afxdialogex.h"


// DlgScaleObject dialog

IMPLEMENT_DYNAMIC(DlgScaleObject, CDialogEx)

DlgScaleObject::DlgScaleObject(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgScaleObject::IDD, pParent)
{
    m_params = NULL;
}

DlgScaleObject::~DlgScaleObject()
{
}

void DlgScaleObject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgScaleObject, CDialogEx)
    ON_BN_CLICKED(IDC_CHECK1, &DlgScaleObject::OnBnClickedCheck1)
    ON_EN_CHANGE(IDC_EDIT_FX, &DlgScaleObject::OnChangeEditFx)
END_MESSAGE_MAP()


// DlgScaleObject message handlers


BOOL DlgScaleObject::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    InitData();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgScaleObject::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    SaveData();

    CDialogEx::OnOK();
}

void DlgScaleObject::InitData(void)
{
    CString strTemp;
    ASSERT(m_params);

    strTemp.Format(_T("%.02f"), 1.0);
    SetDlgItemText(IDC_EDIT_FX, strTemp);
    SetDlgItemText(IDC_EDIT_FY, strTemp);
    SetDlgItemText(IDC_EDIT_FZ, strTemp);

    ((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
    GetDlgItem(IDC_EDIT_FY)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_FZ)->EnableWindow(FALSE);
}

void DlgScaleObject::SaveData(void)
{
    CString strTemp;

    ASSERT(m_params);

    GetDlgItemText(IDC_EDIT_FX, strTemp);
    m_params->fx = _tstof(strTemp);

    GetDlgItemText(IDC_EDIT_FY, strTemp);
    m_params->fy = _tstof(strTemp);

    GetDlgItemText(IDC_EDIT_FZ, strTemp);
    m_params->fz = _tstof(strTemp);
}

void DlgScaleObject::OnBnClickedCheck1()
{
    // TODO: Add your control notification handler code here
    CString strTemp;

    if ( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()) {
        GetDlgItemText(IDC_EDIT_FX, strTemp);
        SetDlgItemText(IDC_EDIT_FY, strTemp);
        SetDlgItemText(IDC_EDIT_FZ, strTemp);

        GetDlgItem(IDC_EDIT_FY)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_FZ)->EnableWindow(FALSE);
    }
    else {
        GetDlgItem(IDC_EDIT_FY)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_FZ)->EnableWindow(TRUE);
    }
}

void DlgScaleObject::OnChangeEditFx()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    CString strTemp;

    if ( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()) {
        GetDlgItemText(IDC_EDIT_FX, strTemp);
        SetDlgItemText(IDC_EDIT_FY, strTemp);
        SetDlgItemText(IDC_EDIT_FZ, strTemp);
    }
}
