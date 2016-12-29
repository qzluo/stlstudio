#include "stdafx.h"

#include "OpenGLDC.h"


COpenGLDC::COpenGLDC(HWND hWnd):m_hWnd(hWnd)
{
    m_hRC = NULL;
    m_hDC = NULL;
    m_clr = RGB(255, 255, 255);
    m_clrHighLight = RGB(0, 255, 0);
    m_bShading = TRUE;
    m_bSelectionMode = FALSE;

    m_coord_gap = 100;
    m_axis_len = 100;
    m_axis_linewidth = 3;
}

COpenGLDC::~COpenGLDC()
{
}

BOOL COpenGLDC::InitDC()
{
    int pixelformat = 0;

    if (m_hWnd == NULL)
        return FALSE;

    m_Camera.init();

    m_hDC = ::GetDC(m_hWnd);			// Get the Device context

    //set dc pixel descriptor
    PIXELFORMATDESCRIPTOR pfdWnd = {
        sizeof(PIXELFORMATDESCRIPTOR), // Structure size.
        1,                             // Structure version number.
        PFD_DRAW_TO_WINDOW |           // Property flags.
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,                            // 24-bit color.
        0, 0, 0, 0, 0, 0,              // Not concerned with these.
        0, 0, 0, 0, 0, 0, 0,           // No alpha or accum buffer.
        32,                            // 32-bit depth buffer.
        0, 0,                          // No stencil or aux buffer.
        PFD_MAIN_PLANE,                // Main layer type.
        0,                             // Reserved.
        0, 0, 0                        // Unsupported.
    };

    if ( (pixelformat = ChoosePixelFormat(m_hDC, &pfdWnd)) == 0 ) {
        AfxMessageBox(_T("ChoosePixelFormat to wnd failed"));
        return FALSE;
    }

    if (SetPixelFormat(m_hDC, pixelformat, &pfdWnd) == FALSE)
        AfxMessageBox(_T("SetPixelFormat failed"));

    m_hRC = wglCreateContext(m_hDC);

    VERIFY(wglMakeCurrent(m_hDC, m_hRC));
    GLSetupRC();
    wglMakeCurrent(NULL,NULL);

    return m_hRC != 0;
}

void COpenGLDC::GLResize(int w,int h)
{
    wglMakeCurrent(m_hDC, m_hRC);
    m_Camera.set_screen(w, h);
    wglMakeCurrent(NULL, NULL);
}

void COpenGLDC::Ready()
{
    wglMakeCurrent(m_hDC, m_hRC);
    m_Camera.projection();
    ClearBkground();
    OnShading();
}

void COpenGLDC::Finish()
{
    glFlush();
    SwapBuffers(m_hDC);
    wglMakeCurrent(NULL, NULL);
}

void COpenGLDC::ClearBkground()
{
    BOOL bLighting = FALSE;
    double x = 0;
    double y = 0;
    double z = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bLighting = IsLighting();
    if (bLighting)
        Lighting(FALSE);

    COLORREF old_clr;
    GetColor(old_clr);

    //draw backgroud
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_Camera.calc_backgroup();

    glBegin(GL_QUADS);
    SetColor(BG_COLOR_ONE);
    m_Camera.get_point_by_type(POINT_LEFT_DOWN, x, y, z);
    glVertex3f(x, y, z);
    m_Camera.get_point_by_type(POINT_RIGHT_DOWN, x, y, z);
    glVertex3f(x, y, z);
    SetColor(BG_COLOR_TWO);
    m_Camera.get_point_by_type(POINT_RIGHT_UP, x, y, z);
    glVertex3f(x, y, z);
    m_Camera.get_point_by_type(POINT_LEFT_UP, x, y, z);
    glVertex3f(x, y, z);
    glEnd();

    //recover color and light
    SetColor(old_clr);
    if (bLighting)
        Lighting(bLighting);
}

//setting model
void COpenGLDC::OnShading()
{
    if (m_bShading) {
        Lighting(TRUE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        Lighting(FALSE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void COpenGLDC::Shading(BOOL bShading)
{
    m_bShading = bShading;
}

BOOL COpenGLDC::IsShading()
{
    return m_bShading;
}

void COpenGLDC::GLSetupRC()
{
    GLfloat lightAmbient[] = {0.75, 0.75, 0.75, 1};
    GLfloat lightDiffuse[] = {1, 1, 1, 1};
    GLfloat lightPos[] = {1, 1, 1, 0};

    glClearColor(0.75, 0.75, 0.75, 0.0f);
    glEnable(GL_DEPTH_TEST);		//Hidden surface removal
    glEnable(GL_CULL_FACE);			//calculate inside of object to support two side of surfaces
    glFrontFace(GL_CCW);

    //set light0 property
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //enable light0
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //set material
    //SetMaterialColor(RGB(225,175,22));
    SetMaterialColor(RGB(64,64,64));

    //frame color
    SetColor(RGB(255, 255, 255));

    m_bShading = TRUE;
}

void COpenGLDC::SetColor(COLORREF clr)
{
    m_clr = clr;
    BYTE r,g,b;
    r = GetRValue(clr);
    g = GetGValue(clr);
    b = GetBValue(clr);
    glColor3ub(r,g,b);
}

void COpenGLDC::GetColor(COLORREF& clr)
{
    clr =  m_clr;
}

void COpenGLDC::SetHightLight(void)
{
    SetMaterialColor(m_clrHighLight);
}

void COpenGLDC::SetMaterialColor(COLORREF clr)
{
    GLfloat mat_amb_diff[] = {0, 0, 0, 1};
    mat_amb_diff[0] = GetRValue(clr) / 255.0;
    mat_amb_diff[1] = GetGValue(clr) / 255.0;
    mat_amb_diff[2] = GetBValue(clr) / 255.0;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
}

void COpenGLDC::Lighting(BOOL bLighting)
{
    if(bLighting)
        glEnable( GL_LIGHTING );
    else
        glDisable( GL_LIGHTING );
}

BOOL COpenGLDC::IsLighting()
{
    GLboolean bLighting;
    glGetBooleanv(GL_LIGHTING, &bLighting);
    return bLighting;
}

void COpenGLDC::BeginSelection(int xPos,int yPos)
{
    m_bSelectionMode = TRUE;
    wglMakeCurrent(m_hDC, m_hRC);

    //set selection buffer
    glSelectBuffer(BUFFER_LENGTH, m_selectBuff);
    glRenderMode(GL_SELECT);

    //setup pick scene
    m_Camera.selection(xPos, yPos);

    //init name stack
    glInitNames();
    glPushName(0);
}

/*-----------------------------------------------------------------------------
Function: end the selection mode of gl. The selection name in the selection 
buffer is reserved in items parameter.
Params: items[OUT] -- result of selected entity
Return: number of selected
-----------------------------------------------------------------------------*/
int COpenGLDC::EndSelection(UINT* items)
{
    int i = 0;
    GLint hits = 0;
    hits = glRenderMode(GL_RENDER);

    for (i = 0; i < hits; i++)
        items[i] = m_selectBuff[i * 4 + 3];

    wglMakeCurrent(NULL, NULL);
    m_bSelectionMode = FALSE;
    return hits;
}

BOOL COpenGLDC::IsSelectionMode()
{
    return m_bSelectionMode;
}

void COpenGLDC::LoadName(UINT name)
{
    glLoadName(name);
}

void COpenGLDC::DrawCoord()
{
    BOOL bLighting = FALSE;
    double len = 0;

    bLighting = IsLighting();
    if (bLighting)
        Lighting(FALSE);

    CPoint3D cPt,xPt,yPt,zPt;
    m_Camera.calc_coord(m_coord_gap, cPt);
    len = m_Camera.get_viewport_scale();
    len *= m_axis_len;

    xPt = yPt = zPt = cPt;
    xPt.x += len;
    yPt.y += len;
    zPt.z += len;

    //set linewidth
    glLineWidth(m_axis_linewidth);

    COLORREF old_clr;
    GetColor(old_clr);

    //axis-x: red
    SetColor(RGB(255,0,0));
    DrawLine(cPt,xPt);

    //axis-y: green
    SetColor(RGB(0,255,0));
    DrawLine(cPt,yPt);

    //axis-z: blue
    SetColor(RGB(0,0,255));
    DrawLine(cPt,zPt);

    //recover line width, color and light
    glLineWidth(1);
    SetColor(old_clr);
    if (bLighting)
        Lighting(bLighting);
}

void COpenGLDC::DrawLine(const CPoint3D& sp,const CPoint3D& ep)
{
    glBegin(GL_LINES);
    glVertex3f(sp.x,sp.y,sp.z);
    glVertex3f(ep.x,ep.y,ep.z);
    glEnd();
}

void COpenGLDC::DrawTriChip(double n0,double n1,double n2,double v00,double v01,double v02,
    double v10,double v11,double v12,double v20,double v21,double v22)
{
    glBegin(GL_TRIANGLES);
    glNormal3d(n0,n1,n2);
    glVertex3d(v00,v01,v02);
    glVertex3d(v10,v11,v12);
    glVertex3d(v20,v21,v22);
    glEnd();
}