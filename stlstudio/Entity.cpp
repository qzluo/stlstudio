#include "stdafx.h"
#include <stdio.h>
#include "Entity.h"
#include "public.h"

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

//load with STL File
int CSTLModel::LoadSTLFile(const char* filename)
{
    FILE* p_file = NULL;
    p_file = fopen(filename, "rb");
    if (!p_file)
        return -1;

    char str[80];
    CTriChip* tri = NULL;
    while(fscanf_s(p_file, "%s", str, 80) == 1) {
        if(strncmp(str, "normal", 6) == 0) {
            tri = new CTriChip();
            if (!tri)
                continue;

            fscanf_s(p_file, "%lf %lf %lf", &(tri->normal.dx), &(tri->normal.dy), &(tri->normal.dz));
            fscanf_s(p_file, "%*s %*s");
            fscanf_s(p_file, "%*s %lf %lf %lf", &(tri->vex[0].x), &(tri->vex[0].y), &(tri->vex[0].z));
            fscanf_s(p_file, "%*s %lf %lf %lf", &(tri->vex[1].x), &(tri->vex[1].y), &(tri->vex[1].z));
            fscanf_s(p_file, "%*s %lf %lf %lf", &(tri->vex[2].x), &(tri->vex[2].y), &(tri->vex[2].z));
            Add(tri);
        }
    }

    m_bModified = TRUE;

    return 0;
}

int CSTLModel::SaveSTLFile(const char* filename)
{
    FILE *p_file = NULL;
    CTriChip* tri = NULL;

    if (IsEmpty())
        return -1;

    p_file = fopen(filename, "wb");
    if (!p_file)
        return -1;

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
    return 0;
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
int CSTLModel::RotateAroundCenter(VECTOR3D bv, double angle)
{
    CPoint3D point;

    if (!GetCenterPoint(point))
        return -1;

    return Rotate(bv, point, angle);
}

/*-----------------------------------------------------------------------------
Function: Rotate the object refer to a refer point
Params: bv[IN] -- direction vector
        ref_pt[IN] -- refer point
        angle[IN] -- angle to rotate
Return: 0  -- modified
        -1 -- not modified
-----------------------------------------------------------------------------*/
int CSTLModel::Rotate(VECTOR3D bv, CPoint3D ref_pt, double angle)
{
    CTriChip* tri = NULL;

    if (IsEmpty())
        return -1;

    for(int i=0; i < m_TriList.GetSize(); i++) {
        tri = m_TriList[i];
        ASSERT(tri);

        for (int n = 0; n < 3; n++)
            RotatePoint(bv, angle, ref_pt, tri->vex[n]);

        RotateVector(bv, angle, tri->normal);
    }

    m_bModified = TRUE;

    return 0;
}

/*-----------------------------------------------------------------------------
Function: Scale the object around the center of the model
Params: fx[IN] -- scale factor in x direction
        fy[IN] -- scale factor in y direction
        fz[IN] -- scale factor in z direction
Return: 0  -- modified
        -1 -- not modified
-----------------------------------------------------------------------------*/
int CSTLModel::ScaleAroundCenter(double fx, double fy, double fz)
{
    CPoint3D point;

    //get reference point
    if (!GetCenterPoint(point))
        return -1;

    return Scale(point, fx, fy, fz);
}

/*-----------------------------------------------------------------------------
Function: Scale the object around a refer point
Params: ref_pt[IN] -- refer point
        fx[IN] -- scale factor in x direction
        fy[IN] -- scale factor in y direction
        fz[IN] -- scale factor in z direction
Return: 0  -- modified
        -1 -- not modified
-----------------------------------------------------------------------------*/
int CSTLModel::Scale(CPoint3D ref_pt, double fx, double fy, double fz)
{
    CTriChip* tri = NULL;

    if (IsEmpty())
        return -1;

    for(int i=0; i < m_TriList.GetSize(); i++) {
        tri = m_TriList[i];
        ASSERT(tri);

        for (int n = 0; n < 3; n++)
            ScalePoint(ref_pt, tri->vex[n], fx, fy, fz);
    }

    m_bModified = TRUE;

    return 0;
}

void CSTLModel::ReverseHighLight(void)
{
    m_bHighLight = !m_bHighLight;
}


//////////////////////////////////////////
CPart::CPart()
{
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
    m_bModified = TRUE;
}

int CPart::RemoveEntity(CEntity* ent)
{
    for(int i = 0; i < m_EntList.GetSize(); i++) {
        if (m_EntList[i] == ent) {
            m_EntList.RemoveAt(i);
            delete ent;
            return 0;
        }
    }

    return -1;
}

//attrib accessing
BOOL CPart::IsEmpty()
{
    return m_EntList.GetSize() == 0;
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

/*-------------------------------------------------------------------
Function: Load data from model file. The file should be save in the
          format:
             file head -- "WXLSTLMODEL"
             stlfile count -- int
             stlfile name1 -- char[256]
             stlfile name2 -- char[256]

          All the objects are set highlight state.

Params: file[IN] -- file name
Return: 0  -- success
        -1 -- failed
-------------------------------------------------------------------*/
int CPart::LoadModel(LPCTSTR file)
{
    FILE *p_file = NULL;
    char szFilePath[256] = {0};
    char buffer[256] = {0};
    int stlfile_count = 0;
    int ret = 0;

    RemoveAllEntity();

    w2c(file, szFilePath);

    p_file = fopen(szFilePath, "rb");
    if (!p_file)
        return -1;

    //read head
    ret = fread(buffer, strlen(MODEL_FILE_HEAD), 1, p_file);
    if (ret <= 0) {
        fclose(p_file);
        p_file = NULL;
        return -1;
    }

    if (strcmp(buffer, MODEL_FILE_HEAD)) {
        fclose(p_file);
        p_file = NULL;
        return -1;
    }

    //read stl file count
    ret = fread(&stlfile_count, sizeof(stlfile_count), 1, p_file);
    if (ret <= 0) {
        fclose(p_file);
        p_file = NULL;
        return -1;
    }

    //load each stl file
    CSTLModel* pSTLModel = NULL;
    for (int i = 0; i < stlfile_count; i++) {
        ret = fread(buffer, sizeof(buffer), 1, p_file);
        if (ret <= 0) {
            fclose(p_file);
            p_file = NULL;
            return -1;
        }

        CSTLModel* pSTLModel = new CSTLModel();
        if (!pSTLModel)
            return -1;

        pSTLModel->LoadSTLFile(buffer);
        if(pSTLModel->IsEmpty())
            delete pSTLModel;
        else {
            AddEntity(pSTLModel);
            pSTLModel->SetHighLight(TRUE);
        }
    }

    return 0;
}

int CPart::LoadSTLFile(LPCTSTR file)
{
    CSTLModel* pSTLModel = new CSTLModel();
    if (!pSTLModel)
        return -1;

    char szFilePath[256] = {0};

    w2c(file, szFilePath);
    pSTLModel->LoadSTLFile(szFilePath);

    if(pSTLModel->IsEmpty()) {
        delete pSTLModel;
        return -1;
    }
    else {
        AddEntity(pSTLModel);
        pSTLModel->SetHighLight(TRUE);
    }

    return 0;
}

/*-------------------------------------------------------------------
Function: Save data to model file. The file should be save in the
          format:
             file head -- "WXLSTLMODEL"
             stlfile count -- int
             stlfile name1 -- char[256]
             stlfile name2 -- char[256]
Params: file[IN] -- file name
Return: 0  -- success
        -1 -- failed
-------------------------------------------------------------------*/
int CPart::ExportModel(LPCTSTR file)
{
    FILE *p_file = NULL;
    char szFilePath[256] = {0};
    int ret = 0;

    if (IsEmpty())
        return 0;

    w2c(file, szFilePath);

    p_file = fopen(szFilePath, "wb");
    if (!p_file)
        return -1;

    //file head
    ret = fwrite(MODEL_FILE_HEAD, strlen(MODEL_FILE_HEAD), 1, p_file);
    if (ret <= 0) {
        fclose(p_file);
        p_file = NULL;
        return -1;
    }

    //stl file count
    int stlfile_count = m_EntList.GetSize();
    ret = fwrite(&stlfile_count, sizeof(stlfile_count), 1, p_file);
    if (ret <= 0) {
        fclose(p_file);
        p_file = NULL;
        return -1;
    }

    //get directory name
    char szDir[256] = {};
    wchar_t szWDir[256] = {};
    char* p = NULL;
    strcpy(szDir, szFilePath);
    p = strrchr(szDir, '.');
    if (p)
        *p = 0;

    c2w(szDir, szWDir);

    //clean directory
    CFileFind fileFind;
    if(fileFind.FindFile(szWDir))
        DeleteDir(szWDir);

    CreateDirectory(szWDir, NULL);

    char szSTLFileName[256] = {};
    for (int i = 0; i < stlfile_count; i++) {
        memset(szSTLFileName, 0, sizeof(szSTLFileName));
        sprintf(szSTLFileName, "%s\\%d.stl", szDir, i + 1);
        ret = fwrite(szSTLFileName, sizeof(szSTLFileName), 1, p_file);
        if (ret <= 0) {
            fclose(p_file);
            p_file = NULL;
            return -1;
        }

        ((CSTLModel*)m_EntList[i])->SaveSTLFile(szSTLFileName);
    }

    fclose(p_file);
    p_file = NULL;

    return 0;
}

int CPart::ExportSTLFile(LPCTSTR file)
{
    CSTLModel* p_stlmodel = NULL;
    char szFilePath[256] = {0};

    if (IsEmpty())
        return -1;

    if (GetSelectedObjectCount() != 1)
        return -1;

    w2c(file, szFilePath);

    p_stlmodel = (CSTLModel*)GetSelectedObject();
    ASSERT(p_stlmodel);

    return p_stlmodel->SaveSTLFile(szFilePath);
}

/*-----------------------------------------------------------------------------
Function: remove selected objects
Params: 
Return: 0  -- modified
        -1 -- not modified
-----------------------------------------------------------------------------*/
int CPart::RemoveSelectedObjects(void)
{
    CEntity* entity = NULL;
    if (IsEmpty())
        return -1;

    if (!HasHighLightObject())
        return -1;

    while (entity = GetSelectedObject())
        ASSERT(RemoveEntity(entity) == 0);

    m_bModified = TRUE;

    return 0;
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
    CSTLModel* p_stlmodel = NULL;
    int i = 0;
    int modify_flag = 0;

    if (IsEmpty())
        return -1;

    if (IS_ZERO(dx) && IS_ZERO(dy) && IS_ZERO(dz))
        return -1;

    //if no object is selected, all object will be moved
    if (!HasHighLightObject()) {
        //move all objects
        for (i = 0; i < m_EntList.GetSize(); i++) {
            p_stlmodel = (CSTLModel*)m_EntList[i];
            if (p_stlmodel->MoveRelative(dx, dy, dz) == 0)
                modify_flag = 1;
        }
    }
    else {
        for(i = 0; i < m_EntList.GetSize(); i++) {
            p_stlmodel = (CSTLModel*)m_EntList[i];
            if (p_stlmodel->IsHighLight())
                if (p_stlmodel->MoveRelative(dx, dy, dz) == 0)
                    modify_flag = 1;
        }
    }

    if (modify_flag)
        m_bModified = TRUE;

    return modify_flag ? 0 : -1;
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
    CSTLModel* p_stlmodel = NULL;
    int i = 0;
    double x0, y0, z0, x1, y1, z1;
    CPoint3D point;
    int modify_flag = 0;

    if (IsEmpty())
        return -1;

    if (IS_ZERO(angle) || (IS_ZERO(bv.dx) && IS_ZERO(bv.dy) && IS_ZERO(bv.dz)))
        return -1;

    //get refer point
    if (!GetActiveBox(x0, y0, z0, x1, y1, z1))
        return -1;

    point.x = (x0 + x1) / 2;
    point.y = (y0 + y1) / 2;
    point.z = (z0 + z1) / 2;

    //if no object is selected, all object will be rotated
    if (!HasHighLightObject()) {
        //rotate all objects
        for (i = 0; i < m_EntList.GetSize(); i++) {
            p_stlmodel = (CSTLModel*)m_EntList[i];
            if (p_stlmodel->Rotate(bv, point, angle) == 0)
                modify_flag = 1;
        }
    }
    else {
        for(i = 0; i < m_EntList.GetSize(); i++) {
            p_stlmodel = (CSTLModel*)m_EntList[i];
            if (p_stlmodel->IsHighLight())
                if (p_stlmodel->Rotate(bv, point, angle) == 0)
                    modify_flag = 1;
        }
    }

    if (modify_flag)
        m_bModified = TRUE;

    return modify_flag ? 0 : -1;
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
    CSTLModel* p_stlmodel = NULL;
    int i = 0;
    double x0, y0, z0, x1, y1, z1;
    CPoint3D point;
    int modify_flag = 0;

    if (IsEmpty())
        return -1;

    if (IS_ZERO(fx) || IS_ZERO(fy) || IS_ZERO(fz))
        return -1;

    if (IS_ZERO(fx-1) && IS_ZERO(fy-1) && IS_ZERO(fz-1))
        return -1;

    //get refer point
    if (!GetActiveBox(x0, y0, z0, x1, y1, z1))
        return -1;

    point.x = (x0 + x1) / 2;
    point.y = (y0 + y1) / 2;
    point.z = (z0 + z1) / 2;

    //if no object is selected, all object will be rotated
    if (!HasHighLightObject()) {
        //rotate all objects
        for (i = 0; i < m_EntList.GetSize(); i++) {
            p_stlmodel = (CSTLModel*)m_EntList[i];
            if (p_stlmodel->Scale(point, fx, fy, fz) == 0)
                modify_flag = 1;
        }
    }
    else {
        for(i = 0; i < m_EntList.GetSize(); i++) {
            p_stlmodel = (CSTLModel*)m_EntList[i];
            if (p_stlmodel->IsHighLight())
                if (p_stlmodel->Scale(point, fx, fy, fz) == 0)
                    modify_flag = 1;
        }
    }

    if (modify_flag)
        m_bModified = TRUE;

    return modify_flag ? 0 : -1;
}

void CPart::SetHighLightStat(BOOL bHighLight)
{
    for(int i=0;i<m_EntList.GetSize();i++)
        ((CSTLModel*)m_EntList[i])->SetHighLight(bHighLight);
}

int CPart::GetSelectedObjectCount(void)
{
    int count = 0;
    if (IsEmpty())
        return -1;

    for(int i = 0; i < m_EntList.GetSize(); i++) {
        if (((CSTLModel*)m_EntList[i])->IsHighLight())
            count++;
    }

    return count;
}

CEntity* CPart::GetSelectedObject(void)
{
    for(int i = 0; i < m_EntList.GetSize(); i++) {
        if (((CSTLModel*)m_EntList[i])->IsHighLight())
            return m_EntList[i];
    }

    return NULL;
}

BOOL CPart::HasHighLightObject(void)
{
    if (IsEmpty())
        return FALSE;

    for(int i = 0; i < m_EntList.GetSize(); i++) {
        if (((CSTLModel*)m_EntList[i])->IsHighLight())
            return TRUE;
    }

    return FALSE;
}

/*-------------------------------------------------------------------
Function: Get active object box.
          The active box is the sum of all selected highlight objects.
          If no object is selected, it is the sum of all the objects
          in the model.
Params: x0, y0, z0, x1, y1, z1[OUT] -- box position
Return: 0  -- success
        -1 -- failed
-------------------------------------------------------------------*/
BOOL CPart::GetActiveBox(double& x0, double& y0, double& z0,
                        double& x1, double& y1, double& z1)
{
    CBox3D box;
    CBox3D active_box;

    if (IsEmpty())
        return FALSE;

    if (!HasHighLightObject())
        GetBox(active_box);
    else {
        for(int i = 0; i < m_EntList.GetSize(); i++) {
            if (((CSTLModel*)m_EntList[i])->IsHighLight() &&
                m_EntList[i]->GetBox(box)) {
                    if (active_box.IsEmpty())
                        active_box = box;
                    else
                        active_box += box;
            }
        }
    }

    if (active_box.IsEmpty())
        return FALSE;

    x0 = active_box.x0;
    y0 = active_box.y0;
    z0 = active_box.z0;
    x1 = active_box.x1;
    y1 = active_box.y1;
    z1 = active_box.z1;

    return TRUE;
}
