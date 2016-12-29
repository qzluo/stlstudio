#pragma once

typedef struct tagScaleParams {
    double fx;
    double fy;
    double fz;
} SCALEPARAMS, *PSCALEPARAMS;

// DlgScaleObject dialog

class DlgScaleObject : public CDialogEx
{
	DECLARE_DYNAMIC(DlgScaleObject)

public:
    PSCALEPARAMS m_params;

public:
	DlgScaleObject(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgScaleObject();

// Dialog Data
	enum { IDD = IDD_SCALE_OBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnBnClickedCheck1();

private:
    void InitData(void);
    void SaveData(void);
public:
    afx_msg void OnChangeEditFx();
};
