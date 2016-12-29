#pragma once

typedef struct tagMoveParams {
    double x;
    double y;
    double z;
    double dx;
    double dy;
    double dz;
} MOVEPARAMS, *PMOVEPARAMS;

// CDlgMoveObject dialog

class CDlgMoveObject : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgMoveObject)

public:
    PMOVEPARAMS m_params;

private:
    void InitData();
    void SaveData();

public:
    CDlgMoveObject(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgMoveObject();

    // Dialog Data
    enum { IDD = IDD_MOVE_OBJECT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
};
