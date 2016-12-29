#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "GeomCalc\cadbase.h"

#define	VIEW_FRONT		    0
#define	VIEW_BACK		    1
#define VIEW_TOP		    2
#define	VIEW_BOTTOM		    3
#define VIEW_RIGHT		    4
#define	VIEW_LEFT		    5
#define VIEW_SW_ISOMETRIC	6
#define VIEW_SE_ISOMETRIC	7
#define VIEW_NE_ISOMETRIC	8
#define VIEW_NW_ISOMETRIC	9

#define POINT_LEFT_DOWN     1
#define POINT_RIGHT_DOWN    2
#define POINT_LEFT_UP       3
#define POINT_RIGHT_UP      4

class GCamera
{
private:
    //eye coordinator
    CPoint3D	m_eye;
    CPoint3D	m_ref;
    CVector3D	m_vecUp;

    //viewing volume
    double		m_far, m_near;
    double	 	m_width,m_height;

    //viewport
    double			m_screen[2];

    //backgroup
    CPoint3D	m_leftdown;
    CPoint3D	m_rightdown;
    CPoint3D	m_leftup;
    CPoint3D	m_rightup;

public:
    GCamera();
    ~GCamera();

    //initailizing
    void init();

    //set viewport according to window
    void set_screen(int x, int y);

    void projection();
    void selection(int xPos,int yPos);

    //switch into a classical view
    void set_view_type(int type);

    //zooming
    void zoom(double scale);
    void zoom_all(double x0,double y0,double z0,double x1,double y1,double z1);

    //move view
    void move_view_by_percent(double dpx, double dpy);
    void move_view_by_space(double dx, double dy);

    //move view in screen space
    void move_screen(int dx, int dy);

    //set viewing volume
    void set_view_rect(double width,double height);
    void get_view_rect(double& width, double& height);

    //to draw backgroup and coord
    void calc_backgroup(void);
    int get_point_by_type(int type, double& x, double& y, double& z);

    void calc_coord(double d, CPoint3D& pt);
    double get_viewport_scale(void);

    //view angle rotation
    void turnLeft(double angle=5.0);
    void turnRight(double angle=5.0);
    void turnUp(double angle=5.0);
    void turnDown(double angle=5.0);

private:
    void update_upVec();
};

#endif  //__CAMERA_H__
