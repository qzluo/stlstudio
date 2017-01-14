#pragma once

#define PLACEMENT_AXIS_ALONG_X   0
#define PLACEMENT_AXIS_ALONG_Y   1

typedef struct tagPlacementParams {
    double margin;
    double gap;
    int axis;
} PLACEMENTPARAMS, *PPLACEMENTPARAMS;

// CDlgAutoPlacement dialog

class CDlgAutoPlacement : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAutoPlacement)

public:
    PPLACEMENTPARAMS m_params;

public:
	CDlgAutoPlacement(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAutoPlacement();

// Dialog Data
	enum { IDD = IDD_ALIGN_OBJECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    void InitData(void);
    void SaveData(void);
};
