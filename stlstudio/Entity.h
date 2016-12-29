#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "GeomCalc\cadbase.h"
#include "OpenGLDC.h"


class CEntity : public CObject  
{
protected:
    BOOL        m_bModified;
    CBox3D*     m_pBox;
    COLORREF    m_clrEntity;

public:
    //constructor and destructor
    CEntity();
    virtual ~CEntity();

    //display
    virtual void Draw(COpenGLDC* pDC) = 0;

    //attrib accessing 
    BOOL GetBox(CBox3D& box);
    BOOL GetCenterPoint(CPoint3D& point);

protected:
    virtual void UpdateBox() = 0;
};

//triangle chip
class CTriChip:public CObject
{
public:
    //attribs
    CPoint3D  vex[3];
    CVector3D normal;

public:
    //constructor && destructor
    CTriChip();
    CTriChip(const CPoint3D& v0,const CPoint3D& v1,const CPoint3D& v2,const CVector3D& norm);
    virtual ~CTriChip();

    //display
    virtual void Draw(COpenGLDC* pDC);

    //operator
    const CTriChip& operator=(const CTriChip&);
};

//tessellated model
class CSTLModel: public CEntity
{
private:
    BOOL m_bHighLight;

public:
    CTypedPtrArray<CObArray,CTriChip*> m_TriList;

public:
    //constructor && destructor
    CSTLModel();
    virtual ~CSTLModel();

    void Clear();

    void Add(CTriChip* tri);
    BOOL IsEmpty();

    //display
    void Draw(COpenGLDC* pDC);

    BOOL LoadSTLFile(LPCTSTR file);
    BOOL SaveSTLFile(LPCTSTR file);

    void HitHandler(void);

    BOOL IsHighLight(void) { return m_bHighLight;}
    void SetHighLight(BOOL bHighLihgt) { m_bHighLight = bHighLihgt; }

    //operate object
    int MoveRelative(double dx, double dy, double dz);
    int Rotate(VECTOR3D bv, double angle);
    int Scale(double fx, double fy, double fz);

protected:
    virtual void UpdateBox();
};


////////////////////////////////////////////////////////
class CPart : public CEntity
{
    //attribs
protected:
    CSTLModel* m_StlModelSelected;    //for export object
    CTypedPtrArray<CObArray,CEntity*> m_EntList;

public:
    //constructor && destructor
    CPart();
    virtual ~CPart();

    //draw
    virtual void Draw(COpenGLDC* pDC);

    //operation
    void AddEntity(CEntity* ent);
    void RemoveAllEntity();

    int ExportSTLFile(LPCTSTR file);

    //attrib accessing
    BOOL IsEmpty();

    void SetHighLightStat(BOOL bHighLight);

    //select object
    void SetSelectModel(CSTLModel* model);
    CSTLModel* GetSelectModel(void) {return m_StlModelSelected;}
    BOOL GetSelectedBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1);

    //operate object
    int MoveObject(double dx, double dy, double dz);
    int RotateObject(VECTOR3D bv, double angle);
    int ScaleObject(double fx, double fy, double fz);

protected:
    virtual void UpdateBox();
};

#endif  //__ENTITY_H__