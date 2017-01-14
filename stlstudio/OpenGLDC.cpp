#include "stdafx.h"

#include "OpenGLDC.h"


COpenGLDC::COpenGLDC(HWND hWnd):m_hWnd(hWnd)
{
    m_hRC = NULL;
    m_hDC = NULL;
    m_clr = RGB(255, 255, 255);
    m_clrHighLight = RGB(0, 48, 0x00);
    m_bShading = TRUE;
    m_bSelectionMode = FALSE;

    m_coord_gap = 100;
    m_axis_len = 100;
    m_axis_linewidth = 3;

    m_ws_length = 400;
    m_ws_width = 400;
    m_ws_height = 300;
    m_ws_gap = 10;
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

void COpenGLDC::GLSetupRC()
{
    GLfloat lightAmbient[] = {0.75, 0.75, 0.75, 1};
    GLfloat lightDiffuse[] = {1, 1, 1, 1};
    GLfloat lightPos[] = {-1, 1, 1, 0};
    GLfloat lightPos1[] = {1, 1, -1, 0};
    GLfloat lightPos2[] = {1, 1, 1, 0};

    glClearColor(0.75, 0.75, 0.75, 0.0f);
    glEnable(GL_DEPTH_TEST);		//Hidden surface removal
    glEnable(GL_CULL_FACE);			//calculate inside of object to support two side of surfaces
    glFrontFace(GL_CCW);

    //set light0 property
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //set light1 property
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    //set light2 property
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);

    //enable light0
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    //set material
    //SetMaterialColor(RGB(225,175,22));
    SetMaterialColor(RGB(30, 30, 30));

    //frame color
    SetColor(RGB(255, 255, 255));

    m_bShading = TRUE;
}

int COpenGLDC::SetParams(POPENGLDCPARAMS params)
{
    if (!params)
        return -1;

    if (params->coord_gap < 0)
        return -1;

    if (params->axis_len <= 0)
        return -1;

    if (params->axis_linewidth < 0)
        return -1;

    if (params->ws_length <= 0)
        return -1;

    if (params->ws_width <= 0)
        return -1;

    if (params->ws_height <= 0)
        return -1;

    if (params->ws_gap <= 0)
        return -1;

    m_coord_gap = params->coord_gap;
    m_axis_len = params->axis_len;
    m_axis_linewidth = params->axis_linewidth;

    //to draw work station
    m_ws_length = params->ws_length;
    m_ws_width = params->ws_width;
    m_ws_height = params->ws_height;
    m_ws_gap = params->ws_gap;

    return 0;
}

int COpenGLDC::GetParams(POPENGLDCPARAMS params)
{
    if (!params)
        return -1;

    params->coord_gap = m_coord_gap;
    params->axis_len = m_axis_len;
    params->axis_linewidth = m_axis_linewidth;

    //to draw work station
    params->ws_length = m_ws_length;
    params->ws_width = m_ws_width;
    params->ws_height = m_ws_height;
    params->ws_gap = m_ws_gap;

    return 0;
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

void COpenGLDC::DrawWorkStation(void)
{
    //get old setting
    int polymode = 0;
    BOOL bLighting = FALSE;
    bLighting = IsLighting();
    if (bLighting)
        Lighting(FALSE);

    COLORREF old_clr;
    GetColor(old_clr);
    glLineWidth(2);
    polymode = GetPolyMode();

    //draw frame
    DrawWSFrame();

    //draw down pane grid
    DrawWSPaneGrid();

    //recover old setting
    SetPolyMode(polymode);
    glLineWidth(1);
    SetColor(old_clr);
    if (bLighting)
        Lighting(bLighting);
}

void COpenGLDC::DrawWSFrame(void)
{
    SetColor(RGB(0,0,0));
    GLfloat point_a[3] = {0, 0, 0};
    GLfloat point_b[3] = {m_ws_length, 0, 0};
    GLfloat point_c[3] = {m_ws_length, 0, m_ws_height};
    GLfloat point_d[3] = {0, 0, m_ws_height};

    GLfloat point_e[3] = {m_ws_length, m_ws_width, 0};
    GLfloat point_f[3] = {m_ws_length, m_ws_width, m_ws_height};
    GLfloat point_g[3] = {0, m_ws_width, m_ws_height};
    GLfloat point_h[3] = {0, m_ws_width, 0};

    glBegin(GL_LINE_LOOP);
    glVertex3fv(point_a);
    glVertex3fv(point_b);
    glVertex3fv(point_c);
    glVertex3fv(point_d);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3fv(point_b);
    glVertex3fv(point_e);
    glVertex3fv(point_f);
    glVertex3fv(point_c);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3fv(point_e);
    glVertex3fv(point_h);
    glVertex3fv(point_g);
    glVertex3fv(point_h);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3fv(point_h);
    glVertex3fv(point_a);
    glVertex3fv(point_d);
    glVertex3fv(point_g);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3fv(point_a);
    glVertex3fv(point_h);
    glVertex3fv(point_e);
    glVertex3fv(point_b);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3fv(point_c);
    glVertex3fv(point_f);
    glVertex3fv(point_g);
    glVertex3fv(point_d);
    glEnd();

    //draw bottom pane
    SetColor(RGB(0xAD, 0xD8, 0xE6));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glVertex3fv(point_a);
    glVertex3fv(point_b);
    glVertex3fv(point_e);
    glVertex3fv(point_h);
    glEnd();
}

void COpenGLDC::DrawWSPaneGrid(void)
{
    double pos = 0;

    SetColor(RGB(0x99, 0x99, 0x66));
    while (pos <= m_ws_width) {
        glBegin(GL_LINES);
        glVertex3f(0, pos, 0.001);
        glVertex3f(m_ws_length, pos, 0.001);
        glEnd();
        pos += m_ws_gap;
    }

    pos = 0;
    while (pos <= m_ws_length) {
        glBegin(GL_LINES);
        glVertex3f(pos, 0, 0.001);
        glVertex3f(pos, m_ws_width, 0.001);
        glEnd();
        pos += m_ws_gap;
    }
}

void COpenGLDC::ZoomWorkStation(void)
{
    double factor = 0.5;
    double x0, y0, z0, x1, y1, z1;
    x0 = - m_ws_length * factor / 2;
    y0 = - m_ws_width * factor / 2;
    z0 = - m_ws_height * factor / 2;
    x1 = m_ws_length + m_ws_length * factor / 2;
    y1 = m_ws_width + m_ws_width * factor / 2;
    z1 = m_ws_height + m_ws_height * factor / 2;

    m_Camera.zoom_all(x0, y0, z0, x1, y1, z1);
}

int COpenGLDC::GetPolyMode(void)
{
    glGetIntegerv(GL_POLYGON_MODE, &m_polymode);
    return m_polymode;
}

void COpenGLDC::SetPolyMode(int polymode)
{
    m_polymode = polymode;
    glPolygonMode(GL_FRONT_AND_BACK, m_polymode);
}

void COpenGLDC::DrawLine(const CPoint3D& sp,const CPoint3D& ep)
{
    glBegin(GL_LINES);
    glVertex3f(sp.x, sp.y, sp.z);
    glVertex3f(ep.x, ep.y, ep.z);
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