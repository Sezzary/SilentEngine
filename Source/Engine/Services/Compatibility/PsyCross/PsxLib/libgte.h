#pragma once

struct MATRIX
{
    short m[3][3]; // 3x3 rotation matrix.
    int   t[3];    // Transfer vector.
};

// 3D `int` vector.
struct VECTOR
{
    int vx, vy, vz, pad;
};

// 3D `short` vector.
struct SVECTOR
{
    short vx, vy, vz, pad;
};

// Color vector.
struct CVECTOR
{
    uchar r, g, b, cd;
};

// 2D `short` vector.
struct DVECTOR
{
    short vx, vy;
};

void InitGeom();
void SetGeomOffset(int ofx, int ofy);
void SetGeomScreen(int h);
void SetRotMatrix(MATRIX* m);
void SetLightMatrix(MATRIX* m);
void SetColorMatrix(MATRIX* m);
void SetTransMatrix(MATRIX* m);
void PushMatrix();
void PopMatrix();

VECTOR*  ApplyMatrix(MATRIX* m, SVECTOR* v0, VECTOR* v1);
VECTOR*  ApplyRotMatrix(SVECTOR* v0, VECTOR* v1);
VECTOR*  ApplyRotMatrixLV(VECTOR* v0, VECTOR* v1);
SVECTOR* ApplyMatrixSV(MATRIX* m, SVECTOR* v0, SVECTOR* v1);
VECTOR*  ApplyMatrixLV(MATRIX* m, VECTOR* v0, VECTOR* v1);

void RotTrans(SVECTOR* v0, VECTOR* v1, long* flag);
void RotTransSV(SVECTOR* v0, SVECTOR* v1, long* flag);

int RotTransPers(SVECTOR* v0, long* sxy, long* p, long* flag);
int RotTransPers3(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, long* sxy0, long* sxy1, long* sxy2, long* p, long* flag);
int RotTransPers4(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, SVECTOR* v3, long* sxy0, long* sxy1, long* sxy2, long* sxy3, long* p, long* flag);

void NormalColor(SVECTOR* v0, CVECTOR* v1);
void NormalColor3(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, CVECTOR* v3, CVECTOR* v4, CVECTOR* v5);
void NormalColorDpq(SVECTOR* v0, CVECTOR* v1, int p, CVECTOR* v2);
void NormalColorCol(SVECTOR* v0, CVECTOR* v1, CVECTOR* v2);
void NormalColorCol3(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, CVECTOR* v3, CVECTOR* v4, CVECTOR* v5, CVECTOR* v6);

void LocalLight(SVECTOR* v0, VECTOR* v1);
int  RotAverageNclip4(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, SVECTOR* v3, long* sxy0, long* sxy1, long* sxy2, long* sxy3, long* p, long* otz, long* flag);

MATRIX* MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2);
MATRIX* MulMatrix(MATRIX* m0, MATRIX* m1);
MATRIX* MulMatrix2(MATRIX* m0, MATRIX* m1);

void SetBackColor(int rbk, int gbk, int bbk);
void SetFarColor(int rfc, int gfc, int bfc);

MATRIX* RotMatrix(SVECTOR* r, MATRIX* m);
MATRIX* RotMatrixYXZ(SVECTOR* r, MATRIX* m);
MATRIX* RotMatrixZYX_gte(SVECTOR* r, MATRIX* m);
MATRIX* RotMatrixX(int r, MATRIX* m);
MATRIX* RotMatrixY(int r, MATRIX* m);
MATRIX* RotMatrixZ(int r, MATRIX* m);
MATRIX* TransMatrix(MATRIX* m, VECTOR* v);
MATRIX* CompMatrix(MATRIX* m0, MATRIX* m1, MATRIX* m2);
MATRIX* ScaleMatrix(MATRIX* m, VECTOR* v);
MATRIX* MulRotMatrix(MATRIX* m0);

void ColorDpq(VECTOR* v0, CVECTOR* v1, int p, CVECTOR* v2);
void ColorCol(VECTOR* v0, CVECTOR* v1, CVECTOR* v2);

int  NormalClip(int sxy0, int sxy1, int sxy2);
void SetDQA(int iDQA);
void SetDQB(int iDQB);
void SetFogNear(int a, int h);
void SetFogNearFar(int a, int b, int h);

int SquareRoot0(int a);
int rsin(int a);
int rcos(int a);
int ratan2(int y, int x);
