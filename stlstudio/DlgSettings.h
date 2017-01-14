#pragma once
#include "afxwin.h"
#include "afxcmn.h"

typedef struct tagSettingsParams {
    double ws_length;
    double ws_width;
    double ws_height;
    double align_margin;
    double align_gap;
} SETTINGSPARAMS, *PSETTINGSPARAMS;



// CDlgSettings dialog

class CDlgSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSettings)

private:
    int m_row;
    int m_column;

public:
    PSETTINGSPARAMS m_params;

public:
	CDlgSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettings();

// Dialog Data
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    CEdit m_edit;
    CListCtrl m_list;
    afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnKillfocusEdit1();

private:
    void InitData(void);
    void SaveData(void);
};
