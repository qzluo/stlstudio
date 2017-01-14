#ifndef __OPENGLDC_H__
#define __OPENGLDC_H__

#include <gl/gl.h>
#include "camera.h"

#define BUFFER_LENGTH 64

#define BG_COLOR_ONE   RGB(128, 128, 128)
#define BG_COLOR_TWO   RGB(255, 255, 255)

#define WS_COLOR_BOTTOM   RGB(0xAD, 0xD8, 0xE6)
#define WS_COLOR_GRID     RGB(0x99, 0x99, 0x66)

typedef struct tagOpenGLDCParams {
    double coord_gap;
    double axis_len;
    double axis_linewidth;
    double ws_length;
    double ws_width;
    double ws_height;
    double ws_gap;
} OPENGLDCPARAMS, *POPENGLDCPARAMS;

class COpenGLDC
{
public:
    COpenGLDC(HWND hWnd);
    virtual ~COpenGLDC();

private:
    HWND        m_hWnd;
    HGLRC       m_hRC;
    HDC         m_hDC;

    COLORREF    m_clr;          //Polygon Color for unshading
    COLORREF    m_clrHighLight; //High light color
    BOOL        m_bShading;     //use material property

    //selection
    BOOL        m_bSelectionMode;
    GLuint      m_selectBuff[BUFFER_LENGTH];

    //polymode
    GLint       m_polymode;

    //to draw coordinate axis
    double m_coord_gap;
    double m_axis_len;
    double m_axis_linewidth;

    //to draw work station
    double m_ws_length;
    double m_ws_width;
    double m_ws_height;
    double m_ws_gap;

public:
    GCamera     m_Camera;

public:
    //initialize
    BOOL InitDC();
    int SetParams(POPENGLDCPARAMS params);
    int GetParams(POPENGLDCPARAMS params);

    void GLResize(int cx,int cy);

    //for drawing
    void Ready();
    void Finish();

    void Shading(BOOL bShading);
    BOOL IsShading();

    //frame material
    void SetColor(COLORREF rgb);
    void GetColor(COLORREF& rgb);

    void SetHightLight(void);
    void SetMaterialColor(COLORREF rgb);

    //selection mode
    void BeginSelection(int xPos,int yPos);
    int EndSelection(UINT* items);
    BOOL IsSelectionMode();

    void LoadName(UINT name);

/////////////////// Drawing ////////////////////
    void DrawCoord();
    void DrawWorkStation(void);
    void ZoomWorkStation(void);

    void DrawLine(const CPoint3D& sp,const CPoint3D& ep);

    //drawing surface
    void DrawTriChip(double n0,double n1,double n2,double v00,double v01,double v02,
        double v10,double v11,double v12,double v20,double v21,double v22);

private:
    void GLSetupRC();
    void ClearBkground();
    void OnShading();

    void Lighting(BOOL bLighting);
    BOOL IsLighting();

    //to draw a work station frame
    void DrawWSFrame(void);
    void DrawWSPaneGrid(void);

    int GetPolyMode(void);
    void SetPolyMode(int polymode);
};

#endif  //__OPENGLDC_H__