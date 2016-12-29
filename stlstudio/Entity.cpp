#include "stdafx.h"
#include <stdio.h>
#include "Entity.h"


CEntity::CEntity()
{
    m_bModified = FALSE;
    m_pBox = NULL;
    m_clrEntity = RGB(64, 64, 64);
}

CEntity::~CEntity()
{
    if (m_pBox) {
        delete m_pBox;
        m_pBox = NULL;
    }
}

BOOL CEntity::GetBox(CBox3D& box)
{
    if (m_bModified)
        UpdateBox();

    if (m_pBox) {
        box = *m_pBox;
        return TRUE;
    }
    else
        return FALSE;
}

BOOL CEntity::GetCenterPoint(CPoint3D& point)
{
    CBox3D box;
    double x0, x1, y0, y1, y2, z0, z1;
    if (!GetBox(box))
        return FALSE;

    point.x = (box.x0 + box.x1) / 2;
    point.y = (box.y0 + box.y1) / 2;
    point.z = (box.z0 + box.z1) / 2;

    return TRUE;
}


///////////////////////////
//class tri chip 
//IMPLEMENT_SERIAL(CTriChip, CObject, 0)
CTriChip::CTriChip()
{
}

CTriChip::CTriChip(const CPoint3D& v0,const CPoint3D& v1,const CPoint3D& v2,const CVector3D& nor)
{
    vex[0] = v0;
    vex[1] = v1;
    vex[2] = v2;
    normal = nor;
}

CTriChip::~CTriChip()
{
}

const CTriChip& CTriChip::operator=(const CTriChip& tri)
{
    normal = tri.normal;
    for(int i = 0; i < 3; i++)
        vex[i] = tri.vex[i];
    return *this;
}

void CTriChip::Draw(COpenGLDC* pDC)
{
    pDC->DrawTriChip(normal.dx,normal.dy,normal.dz,
        vex[0].x,vex[0].y,vex[0].z,
        vex[1].x,vex[1].y,vex[1].z,
        vex[2].x,vex[2].y,vex[2].z);
}

//////////////////////////////////////////
//class  CSTLModel

CSTLModel::CSTLModel()
{
    m_bHighLight = FALSE;
}

CSTLModel::~CSTLModel()
{
    Clear();
}

void CSTLModel::Clear()
{
    for(int i=0;i<m_TriList.GetSize();i++)
        delete m_TriList[i];
    m_TriList.RemoveAll();

    m_bModified = TRUE;
}

void CSTLModel::Add(CTriChip* tri)
{
    m_TriList.Add(tri);
}

BOOL CSTLModel::IsEmpty()
{
    return m_TriList.GetSize() == 0;
}

void CSTLModel::Draw(COpenGLDC* pDC)
{
    //selection mode
    if (pDC->IsSelectionMode())
        pDC->LoadName((UINT)this);

    if (IsHighLight())
        pDC->SetHightLight();
    else
        pDC->SetMaterialColor(m_clrEntity);

    for(int i = 0; i < m_TriList.GetSize(); i++)
        m_TriList[i]->Draw(pDC);
}

//load with STL File
BOOL CSTLModel::LoadSTLFile(LPCTSTR stlfile)
{
    FILE* file;
    errno_t err;
    if( (err  = _tfopen_s( &file, stlfile, _T("r"))) !=0 )
        return FALSE;

    char str[80];
    CTriChip* tri = NULL;
    while(fscanf_s(file,"%s",str,80)==1){
        if(strncmp(str,"normal",6)==0){
            tri = new CTriChip();
            if (!tri)
                continue;

            fscanf_s(file,"%lf %lf %lf",&(tri->normal.dx),&(tri->normal.dy),&(tri->normal.dz));
            fscanf_s(file,"%*s %*s");
            fscanf_s(file,"%*s %lf %lf %lf",&(tri->vex[0].x),&(tri->vex[0].y),&(tri->vex[0].z));
            fscanf_s(file,"%*s %lf %lf %lf",&(tri->vex[1].x),&(tri->vex[1].y),&(tri->vex[1].z));
            fscanf_s(file,"%*s %lf %lf %lf",&(tri->vex[2].x),&(tri->vex[2].y),&(tri->vex[2].z));
            Add(tri);
        }
    }

    m_bModified = TRUE;

    return TRUE;
}

BOOL CSTLModel::SaveSTLFile(LPCTSTR stlfile)
{
    FILE *p_file = NULL;
    char szfilename[256] = {0};
    int nLength = 0;
    CTriChip* tri = NULL;

    if (IsEmpty())
        return FALSE;

    nLength = WideCharToMultiByte(CP_ACP, 0, stlfile, wcslen(stlfile), NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, stlfile, wcslen(stlfile), szfilename, nLength, NULL, NULL);

    p_file = fopen(szfilename, "wb");
    if (!p_file)
        return FALSE;

    fprintf(p_file, "%s\n", "solid");
    for(int i = 0; i < m_TriList.GetSize(); i++) {
        tri = m_TriList[i];
        fprintf(p_file, " facet normal %e %e %e\n", tri->normal.dx, tri->normal.dy, tri->normal.dz);
        fprintf(p_file, "  outer loop\n");
        fprintf(p_file, "   vertex %e %e %e\n", tri->vex[0].x, tri->vex[0].y, tri->vex[0].z);
        fprintf(p_file, "   vertex %e %e %e\n", tri->vex[1].x, tri->vex[1].y, tri->vex[1].z);
        fprintf(p_file, "   vertex %e %e %e\n", tri->vex[2].x, tri->vex[2].y, tri->vex[2].z);
        fprintf(p_file, "  endloop\n");
    }
    fprintf(p_file, "%s\n", "endsolid");

    fclose(p_file);
    p_file = NULL;

    return TRUE;
}

void CSTLModel::UpdateBox()
{
    if(m_pBox){
        delete m_pBox;
        m_pBox = NULL;
    }

    if(m_TriList.GetSize() == 0)
        return;

    double x0,y0,z0,x1,y1,z1;
    x0=y0=z0=10000;
    x1=y1=z1=-10000;

    CTriChip* tri;
    for(int i=0;i<m_TriList.GetSize();i++){
        tri = m_TriList[i];
        for(int n=0;n<3;n++){
            if(tri->vex[n].x<x0)  x0 = tri->vex[n].x;
            if(tri->vex[n].x>x1)  x1 = tri->vex[n].x;
            if(tri->vex[n].y<y0)  y0 = tri->vex[n].y;
            if(tri->vex[n].y>y1)  y1 = tri->vex[n].y;
            if(tri->vex[n].z<z0)  z0 = tri->vex[n].z;
            if(tri->vex[n].z>z1)  z1 = tri->vex[n].z;
        }
    }

    m_pBox = new CBox3D(x0,y0,z0,x1,y1,z1);
    m_bModified = FALSE;
}

void CSTLModel::HitHandler(void)
{
    m_bHighLight = !m_bHighLight;
}

/*-----------------------------------------------------------------------------
Function: Move in directions with a distance
Params: dx[IN] -- x direction distance
        dy[IN] -- y direction distance
        dz[IN] -- z direction distance
Return: 0  -- modified
        -1 -- not modified
-----------------------------------------------------------------------------*/
int CSTLModel::MoveRelative(double dx, double dy, double dz)
{
    CTriChip* tri = NULL;

    if (IsEmpty())
        return -1;

    for(int i=0; i < m_TriList.GetSize(); i++) {
        tri = m_TriList[i];
        ASSERT(tri);
        for(int n = 0; n < 3; n++) {
            tri->vex[n].x += dx;
            tri->vex[n].y += dy;
            tri->vex[n].z += dz;
        }
    }

    m_bModified = TRUE;

    return 0;
}

/*-----------------------------------------------------------------------------
Function: Rotate the object refer to the center point
Params: bv[IN] -- direction vector
        angle[IN] -- angle to rotate
Return: 0  -- modified
        -1 -- not modified
-----------------------------------------------------------------------------*/
int CSTLModel::Rotate(VECTOR3D bv, double angle)
{
    CTriChip* tri = NULL;
    CPoint3D point;

    //get reference point
    if (!GetCenterPoint(point))
        return -1;

    for(int i=0; i < m_TriList.GetSize(); i++) {
        tri = m_TriList[i];
        ASSERT(tri);

        for (int n = 0; n < 3; n++)
            RotatePoint(bv, angle, point, tri->vex[n]);

        RotateVector(bv, angle, tri->normal);
    }

    m_bModified = TRUE;

    return 0;
}

/*-----------------------------------------------------------------------------
Function: Scale the object
Params: fx[IN] -- scale factor in x direction
        fy[IN] -- scale factor in y direction
        fz[IN] -- scale factor in z direction
Return: 0  -- modified
        -1 -- not modified
-----------------------------------------------------------------------------*/
int CSTLModel::Scale(double fx, double fy, double fz)
{
    CTriChip* tri = NULL;
    CPoint3D point;

    //get reference point
    if (!GetCenterPoint(point))
        return 0;

    for(int i=0; i < m_TriList.GetSize(); i++) {
        tri = m_TriList[i];
        ASSERT(tri);

        for (int n = 0; n < 3; n++)
            ScalePoint(point, tri->vex[n], fx, fy, fz);
    }

    m_bModified = TRUE;

    return 0;
}


//////////////////////////////////////////
CPart::CPart()
{
    m_StlModelSelected = NULL;
}

CPart::~CPart()
{
    RemoveAllEntity();
}

//draw
void CPart::Draw(COpenGLDC* pDC)
{
    CEntity *pEnt = NULL;
    for(int i=0;i<m_EntList.GetSize();i++){
        pEnt = m_EntList[i];
        pEnt->Draw(pDC);
    }
}

//operation
void CPart::AddEntity(CEntity* ent)
{
    m_EntList.Add(ent);
    m_bModified = TRUE;
}

void CPart::RemoveAllEntity()
{
    for(int i=0;i<m_EntList.GetSize();i++)
        delete m_EntList[i];
    m_EntList.RemoveAll();
    m_StlModelSelected = NULL;
    m_bModified = TRUE;
}

void CPart::UpdateBox()
{
    if (m_pBox) {
        delete m_pBox;
        m_pBox = NULL;
    }

    CBox3D box;
    for(int i=0;i<m_EntList.GetSize();i++){
        if(m_EntList[i]->GetBox(box)){
            if(m_pBox)
                *m_pBox += box;
            else{
                m_pBox = new CBox3D();
                *m_pBox = box;
            }
        }
    }
    m_bModified = FALSE;
}

int CPart::ExportSTLFile(LPCTSTR file)
{
    if (IsEmpty())
        return -1;

    if (!m_StlModelSelected)
        return -1;

    ASSERT(m_StlModelSelected);

    return m_StlModelSelected->SaveSTLFile(file) ? 0 : -1;
}

//attrib accessing
BOOL CPart::IsEmpty()
{
    return m_EntList.GetSize() == 0;
}

void CPart::SetHighLightStat(BOOL bHighLight)
{
    for(int i=0;i<m_EntList.GetSize();i++)
        ((CSTLModel*)m_EntList[i])->SetHighLight(bHighLight);
}

void CPart::SetSelectModel(CSTLModel* model)
{
    m_StlModelSelected = model;
}

BOOL CPart::GetSelectedBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1)
{
    if (!m_StlModelSelected)
        return FALSE;

    CBox3D box;
    if(!m_StlModelSelected->GetBox(box))
        return FALSE;

    x0 = box.x0;
    y0 = box.y0;
    z0 = box.z0;
    x1 = box.x1;
    y1 = box.y1;
    z1 = box.z1;

    return TRUE;
}

/*-----------------------------------------------------------------------------
Function: Move object
Params: dx[IN] -- x direction distance
        dy[IN] -- y direction distance
        dz[IN] -- z direction distance
Return: 0  -- move action
        -1 -- not moved
-----------------------------------------------------------------------------*/
int CPart::MoveObject(double dx, double dy, double dz)
{
    if (IsEmpty())
        return -1;

    if (!m_StlModelSelected)
        return -1;

    if (IS_ZERO(dx) && IS_ZERO(dy) && IS_ZERO(dz))
        return -1;

    if (m_StlModelSelected->MoveRelative(dx, dy, dz))
        return -1;

    m_bModified = TRUE;

    return 0;
}

/*-----------------------------------------------------------------------------
Function: Rotate the object refer to the center point
Params: bv[IN] -- direction vector
        angle[IN] -- angle to rotate
Return: 0  -- rotate action
        -1 -- not rotated
-----------------------------------------------------------------------------*/
int CPart::RotateObject(VECTOR3D bv, double angle)
{
    if (IsEmpty())
        return -1;

    if (!m_StlModelSelected)
        return -1;

    if (IS_ZERO(angle) || (IS_ZERO(bv.dx) && IS_ZERO(bv.dy) && IS_ZERO(bv.dz)))
        return -1;

    if (m_StlModelSelected->Rotate(bv, angle))
        return -1;

    m_bModified = TRUE;

    return 0;
}

/*-----------------------------------------------------------------------------
Function: Scale object
Params: fx[IN] -- scale factor in x direction
        fy[IN] -- scale factor in y direction
        fz[IN] -- scale factor in z direction
Return: 0  -- scale action
        -1 -- not scaled
-----------------------------------------------------------------------------*/
int CPart::ScaleObject(double fx, double fy, double fz)
{
    if (IsEmpty())
        return -1;

    if (!m_StlModelSelected)
        return -1;

    if (IS_ZERO(fx) || IS_ZERO(fy) || IS_ZERO(fz))
        return -1;

    if (IS_ZERO(fx-1) && IS_ZERO(fy-1) && IS_ZERO(fz-1))
        return -1;

    if (m_StlModelSelected->Scale(fx, fy, fz))
        return -1;

    m_bModified = TRUE;

    return 0;
}