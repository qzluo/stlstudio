// DlgRotateObject.cpp : implementation file
//

#include "stdafx.h"
#include "stlstudio.h"
#include "DlgRotateObject.h"
#include "afxdialogex.h"

#include "math_public.h"

// CDlgRotateObject dialog

IMPLEMENT_DYNAMIC(CDlgRotateObject, CDialogEx)

CDlgRotateObject::CDlgRotateObject(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRotateObject::IDD, pParent)
{
    m_params = NULL;
}

CDlgRotateObject::~CDlgRotateObject()
{
}

void CDlgRotateObject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRotateObject, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgRotateObject::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CDlgRotateObject::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CDlgRotateObject::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CDlgRotateObject::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDlgRotateObject message handlers


BOOL CDlgRotateObject::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    InitData();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRotateObject::InitData(void)
{
    CString strTemp;
    strTemp.Format(_T("%d"), 0);
    SetDlgItemText(IDC_EDIT_DEGREE, strTemp);
    ((CButton*)GetDlgItem(IDC_RADIO_Z))->SetCheck(TRUE);
}

void CDlgRotateObject::SaveData(void)
{
    int degree = 0;

    if (!m_params)
        return;

    ASSERT(m_params);

    degree = GetDlgItemInt(IDC_EDIT_DEGREE);
    degree %= 360;
    m_params->angle = degree * PI / 180.0;

    if (((CButton*)GetDlgItem(IDC_RADIO_X))->GetCheck())
        m_params->axis = AXIS_X;
    else if (((CButton*)GetDlgItem(IDC_RADIO_Y))->GetCheck())
        m_params->axis = AXIS_Y;
    else if (((CButton*)GetDlgItem(IDC_RADIO_Z))->GetCheck())
        m_params->axis = AXIS_Z;
}

void CDlgRotateObject::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    SaveData();

    CDialogEx::OnOK();
}


void CDlgRotateObject::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    SetDlgItemInt(IDC_EDIT_DEGREE, 45);
}


void CDlgRotateObject::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    SetDlgItemInt(IDC_EDIT_DEGREE, 90);
}


void CDlgRotateObject::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
    SetDlgItemInt(IDC_EDIT_DEGREE, 180);
}


void CDlgRotateObject::OnBnClickedButton4()
{
    // TODO: Add your control notification handler code here
    SetDlgItemInt(IDC_EDIT_DEGREE, 270);
}
