#pragma once

#define AXIS_X  0
#define AXIS_Y  1
#define AXIS_Z  2

typedef struct tagRotateParams {
    int axis;
    double angle;
} ROTATEPARAMS, *PROTATEPARAMS;

// CDlgRotateObject dialog

class CDlgRotateObject : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRotateObject)

public:
    PROTATEPARAMS m_params;


public:
	CDlgRotateObject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRotateObject();

// Dialog Data
	enum { IDD = IDD_ROTATE_OBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();


private:
    void InitData(void);
    void SaveData(void);
    virtual void OnOK();
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
};
