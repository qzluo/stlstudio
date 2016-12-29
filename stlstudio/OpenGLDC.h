#ifndef __OPENGLDC_H__
#define __OPENGLDC_H__

#include <gl/gl.h>
#include "camera.h"

#define BUFFER_LENGTH 64

#define BG_COLOR_ONE   RGB(128, 128, 128)
#define BG_COLOR_TWO   RGB(255, 255, 255)

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

    //to draw coordinate axis
    double m_coord_gap;
    double m_axis_len;
    double m_axis_linewidth;

public:
    GCamera     m_Camera;

public:
    //initialize
    BOOL InitDC();

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
};

#endif  //__OPENGLDC_H__