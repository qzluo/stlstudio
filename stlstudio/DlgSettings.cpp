// DlgSettings.cpp : implementation file
//

#include "stdafx.h"
#include "stlstudio.h"
#include "DlgSettings.h"
#include "afxdialogex.h"


// CDlgSettings dialog

IMPLEMENT_DYNAMIC(CDlgSettings, CDialogEx)

CDlgSettings::CDlgSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSettings::IDD, pParent)
{

}

CDlgSettings::~CDlgSettings()
{
}

void CDlgSettings::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_edit);
    DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDlgSettings, CDialogEx)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgSettings::OnDblclkList1)
    ON_EN_KILLFOCUS(IDC_EDIT1, &CDlgSettings::OnKillfocusEdit1)
END_MESSAGE_MAP()


// CDlgSettings message handlers


BOOL CDlgSettings::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    InitData();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettings::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    SaveData();

    CDialogEx::OnOK();
}

void CDlgSettings::InitData(void)
{
    CString strTemp;

    DWORD dwStyle = m_list.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_list.SetExtendedStyle(dwStyle);

    //set column
    m_list.InsertColumn(0, _T("Property"), LVCFMT_LEFT, 320);
    m_list.InsertColumn(1, _T("Setting"), LVCFMT_LEFT, 320);

    //
    m_list.InsertItem(0, _T("Work Station Length"));
    strTemp.Format(_T("%.0f"), m_params->ws_length);
    m_list.SetItemText(0, 1, strTemp);

    m_list.InsertItem(1, _T("Work Station Width"));
    strTemp.Format(_T("%.0f"), m_params->ws_width);
    m_list.SetItemText(1, 1, strTemp);

    m_list.InsertItem(2, _T("Work Station Height"));
    strTemp.Format(_T("%.0f"), m_params->ws_height);
    m_list.SetItemText(2, 1, strTemp);

    m_list.InsertItem(3, _T("Align Margin"));
    strTemp.Format(_T("%.0f"), m_params->align_margin);
    m_list.SetItemText(3, 1, strTemp);

    m_list.InsertItem(4, _T("Align Gap"));
    strTemp.Format(_T("%.0f"), m_params->align_gap);
    m_list.SetItemText(4, 1, strTemp);

    m_edit.ShowWindow(SW_HIDE);

    m_row = -1;
    m_column = -1;
}

void CDlgSettings::SaveData(void)
{
    CString strText;

    m_list.GetItemText(0, 1, strText.GetBuffer(256), 256);
    m_params->ws_length = _wtof(strText.GetBuffer());

    m_list.GetItemText(1, 1, strText.GetBuffer(256), 256);
    m_params->ws_width = _wtof(strText.GetBuffer());

    m_list.GetItemText(2, 1, strText.GetBuffer(256), 256);
    m_params->ws_height = _wtof(strText.GetBuffer());

    m_list.GetItemText(3, 1, strText.GetBuffer(256), 256);
    m_params->align_margin = _wtof(strText.GetBuffer());

    m_list.GetItemText(4, 1, strText.GetBuffer(256), 256);
    m_params->align_gap = _wtof(strText.GetBuffer());
}

void CDlgSettings::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CRect rc;
    CRect rcList;
    CRect rcDlgClt;
    CListCtrl* p_listctrl = (CListCtrl*)GetDlgItem(IDC_LIST1);
    if (pNMListView->iItem != -1) {
        m_row = pNMListView->iItem;
        m_column = pNMListView->iSubItem;

        if (m_column <= 0) {
            *pResult = 0;
            return;
        }

        //get offset rect
        m_list.GetWindowRect(&rcList);
        GetClientRect(&rcDlgClt);
        ClientToScreen(&rcDlgClt);
        POINT pt;
        pt.x = rcList.left - rcDlgClt.left;
        pt.y = rcList.top - rcDlgClt.top;

        m_list.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem, LVIR_LABEL, rc);
        rc.OffsetRect(pt);
        rc.left += 3;
        rc.top += 2 ;
        rc.right += 3;
        rc.bottom += 2 ;

        CString strText;
        m_list.GetItemText(pNMListView->iItem, pNMListView->iSubItem, strText.GetBuffer(256), 256);
        m_edit.SetWindowText(strText);
        m_edit.ShowWindow(SW_SHOW);
        m_edit.MoveWindow(&rc);
        m_edit.SetFocus();
        m_edit.CreateSolidCaret(1, rc.Height() - 5);
        m_edit.ShowCaret();
        m_edit.SetSel(-1);
    }

    *pResult = 0;
}


void CDlgSettings::OnKillfocusEdit1()
{
    // TODO: Add your control notification handler code here
    CString str;
    m_edit.GetWindowText(str);
    m_list.SetItemText(m_row, m_column, str);
    m_edit.ShowWindow(SW_HIDE);
}

