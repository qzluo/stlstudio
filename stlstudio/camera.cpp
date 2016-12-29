#include "stdafx.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include "camera.h"

GCamera::GCamera(void)
{
}

GCamera::~GCamera()
{
}

void GCamera::init()
{
    m_eye = CPoint3D(0, 0, 10000);
    m_ref = CPoint3D(0, 0, 0);
    m_vecUp = CVector3D(0,1,0);

    m_far  = 20000;
    m_near = 1;
    m_width  = 2400.0;
    m_height = 2400.0;

    m_screen[0] = 400;
    m_screen[1] = 400;
}

void GCamera::set_screen(int x, int y)
{
    glViewport(0, 0, x, y);
    if (y == 0) y = 1;

    double ratio = (double)x / (double)y;
    m_height *= (double)y / m_screen[1];
    m_width =  m_height * ratio;
    m_screen[0] = x;
    m_screen[1] = y;
}

void GCamera::projection()
{
    //switch to projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glRenderMode(GL_RENDER);

    //apply projective matrix
    double left		= - m_width / 2.0;
    double right	= m_width / 2.0;
    double bottom	= - m_height / 2.0;
    double top		= m_height / 2.0;

    glOrtho(left, right, bottom, top, m_near, m_far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x, m_eye.y, m_eye.z,
        m_ref.x, m_ref.y, m_ref.z,
        m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);
}

//根据鼠标位置，设置拾取场景
void GCamera::selection(int xPos,int yPos)
{
    GLint vp[4] = {};
    glGetIntegerv(GL_VIEWPORT, vp);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(xPos, vp[3] - yPos, 1, 1, vp);

    double left		= - m_width / 2.0;
    double right	= m_width / 2.0;
    double bottom	= - m_height / 2.0;
    double top		= m_height / 2.0;

    glOrtho(left, right, bottom, top, m_near, m_far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x, m_eye.y, m_eye.z,
        m_ref.x, m_ref.y, m_ref.z,
        m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);
}

void GCamera::set_view_type( int type )
{
    double r;
    CVector3D vec;

    vec = m_ref - m_eye;
    r = vec.GetLength();

    if(IS_ZERO(r)) r = 50.0;
    if( r > 10000)  r = 10000;

    switch(type) {
    case VIEW_FRONT:
        m_eye = m_ref + CVector3D(0,-r,0);
        m_vecUp = CVector3D(0,0,1);
        break;

    case VIEW_BACK:
        m_eye = m_ref + CVector3D(0,r,0);
        m_vecUp = CVector3D(0,0,1);
        break;

    case VIEW_TOP:
        m_eye = m_ref + CVector3D(0,0,r);
        m_vecUp = CVector3D(0,1,0);
        break;

    case VIEW_BOTTOM:
        m_eye = m_ref + CVector3D(0,0,-r);
        m_vecUp = CVector3D(0,1,0);
        break;

    case VIEW_RIGHT:
        m_eye = m_ref + CVector3D(r,0,0);
        m_vecUp = CVector3D(0,0,1);
        break;

    case VIEW_LEFT:
        m_eye = m_ref + CVector3D(-r,0,0);
        m_vecUp = CVector3D(0,0,1);
        break;

    case VIEW_SW_ISOMETRIC:
        m_eye = m_ref + CVector3D(-1,-1,1).GetNormal()*r;
        update_upVec();
        break;

    case VIEW_SE_ISOMETRIC:
        m_eye = m_ref + CVector3D(1,-1,1).GetNormal()*r;
        update_upVec();
        break;

    case VIEW_NE_ISOMETRIC:
        m_eye = m_ref + CVector3D(1,1,1).GetNormal()*r;
        update_upVec();
        break;

    case VIEW_NW_ISOMETRIC:
        m_eye = m_ref + CVector3D(-1,1,1).GetNormal()*r;
        update_upVec();
        break;

    default:
        break;
    }
}

void GCamera::zoom(double scale)
{
    ASSERT(scale > 0.0);
    m_width *= scale;
    m_height *= scale;
}

void GCamera::zoom_all(double x0,double y0,double z0,double x1,double y1,double z1)
{
    double width,height;
    double	xl, yl, zl;
    xl = x1-x0;
    yl = y1-y0;
    zl = z1-z0;

    width = max(max(xl,yl),zl);
    height= max(max(xl,yl),zl);

    set_view_rect(width, height);

    CVector3D vec = m_eye - m_ref;
    m_ref.x = (x0+x1)/2;
    m_ref.y = (y0+y1)/2;
    m_ref.z = (z0+z1)/2;
    m_eye = m_ref + vec;
}

void GCamera::move_view_by_percent(double dpx, double dpy)
{
    CVector3D vec;
    CVector3D xUp, yUp;

    vec = m_ref - m_eye;
    vec.Normalize();
    xUp = vec * m_vecUp;
    xUp.Normalize();
    yUp = xUp * vec;

    m_eye -= xUp * m_width * dpx + yUp * m_height * dpy;
    m_ref -= xUp * m_width * dpx + yUp * m_height * dpy;
}

void GCamera::move_view_by_space(double dx, double dy)
{
    CVector3D vec;
    CVector3D xUp, yUp;

    vec = m_ref - m_eye;
    vec.Normalize();
    xUp = vec * m_vecUp;
    xUp.Normalize();
    yUp = xUp * vec;

    m_eye -= xUp * dx + yUp * dy;
    m_ref -= xUp * dx + yUp * dy;
}

void GCamera::move_screen(int dx,int dy)
{
    double dxvw = dx * m_width / m_screen[0];
    double dyvw = dy * m_height / m_screen[1];
    move_view_by_space(dxvw, dyvw);
}

void GCamera::set_view_rect(double width,double height)
{
    m_height = height;
    double aspect = m_screen[0]/m_screen[1];
    m_width =  m_height * aspect;
}

void GCamera::get_view_rect(double& width,double& height)
{
    width = m_width;
    height = m_height;
}

void GCamera::turnLeft(double angle)
{
    angle /= 180/PI;
    CMatrix3D mat = CMatrix3D::CreateRotateMatrix(angle, m_vecUp);
    CVector3D vec = m_eye - m_ref;
    vec = vec * mat;
    m_eye = m_ref + vec;
}

void GCamera::turnRight(double angle)
{
    angle /= 180/PI;
    CMatrix3D mat = CMatrix3D::CreateRotateMatrix(-angle, m_vecUp);
    CVector3D vec = m_eye - m_ref;
    vec = vec * mat;
    m_eye = m_ref + vec;
}

void GCamera::turnUp(double angle)
{
    angle /= 180/PI;

    CVector3D xDir = (m_ref - m_eye) * m_vecUp;
    CMatrix3D mat = CMatrix3D::CreateRotateMatrix(angle, xDir);

    CVector3D vec = m_eye - m_ref;
    vec = vec * mat;
    m_eye = m_ref + vec;
    m_vecUp = vec * xDir;
    m_vecUp.Normalize();
}

void GCamera::turnDown(double angle)
{
    angle /= 180/PI;

    CVector3D xDir = (m_ref - m_eye) * m_vecUp;
    CMatrix3D mat = CMatrix3D::CreateRotateMatrix(-angle, xDir);

    CVector3D vec = m_eye - m_ref;
    vec = vec * mat;
    m_eye = m_ref + vec;
    m_vecUp = vec * xDir;
    m_vecUp.Normalize();
}

void GCamera::update_upVec()
{
    CVector3D vec = m_ref - m_eye;
    CVector3D zVec(0,0,1);
    CVector3D vec0;

    vec.Normalize();
    vec0 = vec*zVec;
    m_vecUp = vec0*vec;
    m_vecUp.Normalize();
}

void GCamera::calc_backgroup(void)
{
    CVector3D vecDir;
    CVector3D xUp, yUp;
    CPoint3D center;

    vecDir = m_ref - m_eye;
    vecDir.Normalize();

    xUp = vecDir * m_vecUp;
    xUp.Normalize();
    yUp = xUp * vecDir;

    //backgroup center
    center = m_eye + vecDir * (m_far - 10);
    m_leftdown = center - xUp * m_width / 2 - yUp * m_height / 2;
    m_rightdown = center + xUp * m_width / 2 - yUp * m_height / 2;
    m_leftup = center - xUp * m_width / 2 + yUp * m_height / 2;
    m_rightup = center + xUp * m_width / 2 + yUp * m_height / 2;
}

int GCamera::get_point_by_type(int type, double& x, double& y, double& z)
{
    CPoint3D* p_point = NULL;
    if (type < POINT_LEFT_DOWN || type > POINT_RIGHT_UP)
        return -1;

    switch (type) {
    case POINT_LEFT_DOWN:
        p_point = &m_leftdown;
        break;

    case POINT_RIGHT_DOWN:
        p_point = &m_rightdown;
        break;

    case POINT_LEFT_UP:
        p_point = &m_leftup;
        break;

    case POINT_RIGHT_UP:
        p_point = &m_rightup;
        break;

    default:
        break;
    }

    x = p_point->x;
    y = p_point->y;
    z = p_point->z;

    return 0;
}

/*-----------------------------------------------------------------------------
Function: to calculate a point in that we would draw a coord in later
Params: d[IN] -- distance from the bottom edge of screen view
        pt[OUT] -- the point to get
Return:
-----------------------------------------------------------------------------*/
void GCamera::calc_coord(double d, CPoint3D& pt)
{
    CVector3D vecDir;
    CVector3D xUp, yUp;
    CPoint3D center;

    vecDir = m_ref - m_eye;
    vecDir.Normalize();

    xUp = vecDir * m_vecUp;
    xUp.Normalize();
    yUp = xUp * vecDir;

    //backgroup center
    center = m_eye + vecDir * (m_near + 500);
    pt = center - xUp * m_width / 2 - yUp * m_height / 2;
    pt = pt + xUp * d * m_width / m_screen[0] + 
        yUp * d * m_height / m_screen[1];
}

double GCamera::get_viewport_scale(void)
{
    ASSERT(m_screen[0]);
    return m_width / m_screen[0];
}