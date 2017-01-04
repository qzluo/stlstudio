#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "GeomCalc\cadbase.h"
#include "OpenGLDC.h"

#define MODEL_FILE_HEAD  "WXLSTLMODEL"


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

    int LoadSTLFile(const char* filename);
    int SaveSTLFile(const char* filename);

    //operate object
    int MoveRelative(double dx, double dy, double dz);
    int RotateAroundCenter(VECTOR3D bv, double angle);
    int Rotate(VECTOR3D bv, CPoint3D ref_pt, double angle);
    int ScaleAroundCenter(double fx, double fy, double fz);
    int Scale(CPoint3D ref_pt, double fx, double fy, double fz);

    //for select operation
    void ReverseHighLight(void);
    BOOL IsHighLight(void) { return m_bHighLight;}
    void SetHighLight(BOOL bHighLihgt) { m_bHighLight = bHighLihgt; }

protected:
    virtual void UpdateBox();
};


////////////////////////////////////////////////////////
class CPart : public CEntity
{
    //attribs
protected:
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
    int RemoveEntity(CEntity* ent);

    //attrib accessing
    BOOL IsEmpty();

    //import and export
    int LoadModel(LPCTSTR file);
    int LoadSTLFile(LPCTSTR file);
    int ExportModel(LPCTSTR file);
    int ExportSTLFile(LPCTSTR file);
    int RemoveSelectedObjects(void);

    //operate object
    int MoveObject(double dx, double dy, double dz);
    int RotateObject(VECTOR3D bv, double angle);
    int ScaleObject(double fx, double fy, double fz);

    //select object
    int GetSelectedObjectCount(void);
    CEntity* GetSelectedObject(void);
    BOOL HasHighLightObject(void);
    BOOL GetActiveBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1);
    void SetHighLightStat(BOOL bHighLight);

protected:
    virtual void UpdateBox();
};

#endif  //__ENTITY_H__