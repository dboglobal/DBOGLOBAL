/*****************************************************************************
 *
 * File			: NtlMath.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 21	
 * Abstract		: Math관련 함수들
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _NTL_MATH_
#define _NTL_MATH_

#include <rwcore.h>

#define NTL_EPSILON 0.0001f
#define DBO_PI	3.1415926535897932384626433832795028841971693993751f
#define NTL_DEG2RAD(_x) ((_x) * ( ((RwReal)DBO_PI) / ((RwReal)180) ))
#define NTL_RAD2DEG(_x) ((_x) * ( ((RwReal)180) / ((RwReal)DBO_PI) ))

static const RwV3d  XAxis = { 1.0f, 0.0f, 0.0f };
static const RwV3d  YAxis = { 0.0f, 1.0f, 0.0f };
static const RwV3d  ZAxis = { 0.0f, 0.0f, 1.0f };
static const RwV3d  ZeroAxis = { 0.0f, 0.0f, 0.0f };
static const RwV3d  UnitAxis = {1.0f, 1.0f, 1.0f};

inline RwInt32	NtlRandomNumber(RwInt32 iMin, RwInt32 iMax)
{
	if(iMin > iMax)
	{
		RwInt32 iTemp = iMin;
		iMin = iMax;
		iMax = iTemp;
	}

	if (iMin == iMax) 
		return(iMin);
	return((rand() % (abs(iMax-iMin)+1))+iMin);
}

inline RwReal NtlRandomNumber(RwReal fMin, RwReal fMax)
{
	if(fMin > fMax)
	{
		RwReal fTemp = fMin;
		fMin = fMax;
		fMax = fTemp;
	}
	
	if (fMin == fMax) 
		 return(fMin);
	RwReal fRandom = (RwReal)rand() / (RwReal)RAND_MAX;
	return((fRandom * (RwReal)fabs(fMax-fMin))+fMin);
}


inline RwV3d NtlRandomNumber(RwV3d vMin, RwV3d vMax)
{
	RwV3d v;
	v.x = NtlRandomNumber(vMin.x, vMax.x);
	v.y = NtlRandomNumber(vMin.y, vMax.y);
	v.z = NtlRandomNumber(vMin.z, vMax.z);

	return v;
}

class CNtlPlane
{
public:

	union 
	{
		struct 
        {
			RwReal a, b, c, d;
		};
        struct 
        {
            RwV3d n;
            RwReal d;
        };
		RwV4d v4d;
		RwReal m[4];
	};
	
public:

	CNtlPlane(RwReal A, RwReal B, RwReal C, RwReal D);
	CNtlPlane();
	CNtlPlane(const RwV3d& bNormal, RwReal fConstant);
	
	void Normalize(void);

	void Set(RwReal A, RwReal B, RwReal C, RwReal D) { a = A; b = B; c = C; d = D; }
	
    void PlaneFromPoints(const RwV3d *pV1, const RwV3d *pV2, const RwV3d *pV3 );
    void PlaneFromPointNormal(const RwV3d *pPoint, const RwV3d *pNormal);

    RwReal DotProduct(RwV3d *pV);
	RwReal DistanceTo (const RwV3d *pPoint) const;
	RwV3d SplitPoint(const RwV3d *pFrom,const RwV3d *pAt);

	RwV3d&	GetNormal(void) { return n; }
	const RwV3d& GetNormal(void) const { return n; }
	
	RwReal& GetConstant(void) { return d; }
	const RwReal& GetConstant(void) const { return d; }
};

class CNtlOBB
{
public:
	CNtlOBB();
	CNtlOBB(RwMatrix* pMatrix, RwBBox* pAABB);
	~CNtlOBB();

	RwV3d	vCenterPos;
	RwV3d	vAxisDir[3];
	RwReal	fAxisLen[3];

	static void		CalcOBBData(CNtlOBB* pOBB, RwMatrix* pMatrix, RwBBox* pAABB);
	static RwBool	IntersectionOBBvsOBB(CNtlOBB* pA, CNtlOBB* pB);
};


// 

/**
 * \ingroup Core
 * Math관련 함수들의 집합
 * 
 */
class CNtlMath
{
public:
	CNtlMath();
	~CNtlMath();

public:

	static void     MathRwV2dAssign(RwV2d *pOut, RwReal fX, RwReal fY);
	static void     MathRwV2dAdd(RwV2d *pOut, RwReal fX, RwReal fY);
	static RwReal   MathRwV2dSquaredLength(const RwV2d *pIn);
	static void     MathRwV3dAssign(RwV3d *pOut, RwReal fX, RwReal fY, RwReal fZ); 
	static void     MathRwV3dAdd(RwV3d *pOut, RwReal fX, RwReal fY, RwReal fZ);
	static void     MathRwMatrixAssign(RwMatrix *pOut, const RwV3d *pPos, const RwV3d *pDir, const RwV3d *pScale = NULL);
	static void     MathRwV3dAddMul(RwV3d *pOut, const RwV3d *pIn, RwReal fValue);
	static void     MathRwV3dSubMul(RwV3d *pOut, const RwV3d *pIn, RwReal fValue);
	static RwReal   MathRwV3dSquaredLength(const RwV3d *pIn);
	static RwReal   GetLineAngle(const RwV3d &curPos, const RwV3d &desPos);
    static RwReal   GetLength(const RwV3d& v1, const RwV3d& v2);
    static RwReal   GetLength(const RwV2d& v1, const RwV2d& v2);

    // X-Axis Find the Euler angle.
	static RwReal LineToAngleX(const RwV3d *pLine);

	// X-Axis Find the Euler angle. Increase the speed by inserting the Y-axis rotation angle value.
	static RwReal LineToAngleX(const RwV3d *pLine, RwReal fAxisAngleY);

	// Find the Y-Axis Euler angle.
	static RwReal LineToAngleY(const RwV3d *pLine);

	// Find X-Axis and Y-Axis Euler angles.
	static void LineToAngleXY(const RwV3d *pLine, RwReal& fAxisAngleX, RwReal& fAxisAngleY);

	static void MathGetMatrixEulerAngle(const RwMatrix *pMatrix, RwV3d *pvAngle);
	static void MathGetRotationMatrix(RwMatrix *pMatrix, const RwV3d* pvUnit_A, const RwV3d* pvUnit_B);
	static void MathGetMatrix(RwMatrix *pMatrix, const RwV3d* pvScale, const RwV3d* pvRot, const RwV3d* pvPos);
	
	static DWORD	FtoDW(FLOAT f) { return *((DWORD*)&f); }
	static RwReal	DWtoF(DWORD w) { return *((FLOAT*)&w); }

    
    static RwRGBA Interpolation(const RwRGBA& startColor, const RwRGBA& endColor, const RwReal fDelta);    // RwRGBA값의 보간값을 구한다 (delta : 0.0 ~ 1.0 까지의 값)
    static RwReal Interpolation(const RwReal fStart, const RwReal fEnd, const RwReal fDelta);               // Real값의 보간값을 구한다. (delta : 0.0 ~ 1.0 까지의 값)
    static RwV3d  Interpolation(const RwV3d& vStart, const RwV3d& vEnd, const RwReal fDelta);               // RwV3d값의 선형 보간값을 구한다.

	static RwBool LineSphereCollisionTestPixelRay(RwCamera *pCamera, RwSphere *pSphere, RwInt32 iPixelX, RwInt32 iPixelY, RwReal fRayDist, RwReal& fDistance); 

	static RwBool BBoxBBoxCollision(const RwBBox *pBox1, const RwBBox *pBox2);

    // 값을 범위내로 잘라서 반환한다. 
    static RwReal Range(const RwReal fValue, const RwReal fMin, const RwReal fMax);

    // 행렬로부터 스케일값을 추출한다. (균등 스케일인경우에만 적용된다)
    static RwReal ScaleFromRwMatrix(const RwMatrix& matrix);
    
    // 벡터V의  1 / V를 계산한다.
    static RwV3d RwV3dInverse(const RwV3d& v3d);

	static RwInt32 GetSafeIdx3D(RwV3d& vPos, RwInt32 iHalfSize, RwInt32 iChunkSize, RwInt32 iChunkNum);
};

//////////////////////////////////////////////////////////////////////////
// 연산자 오버로딩 (2006.09.13 by agebreak)
//////////////////////////////////////////////////////////////////////////
const RwV3d operator+(const RwV3d& lhs, const RwV3d& rhs);
const RwV3d operator-(const RwV3d& lhs, const RwV3d& rhs);
const void  operator+=(RwV3d& lhs, const RwV3d& rhs);
const void  operator-=(RwV3d& lhs, const RwV3d& rhs);
const RwV3d operator*(const RwV3d& lhs, const RwReal& rhs);
const void	operator*=(RwV3d& lhs, const RwReal& rhs);
const RwV3d operator/(const RwV3d& lhs, const RwReal& rhs);
const RwV3d operator*(const RwReal& lhs, const RwV3d& rhs);
const RwV3d operator/(const RwReal& lhs, const RwV3d& rhs);
const RwBool operator==(const RwV3d& lhs, const RwV3d& rhs);
const RwBool operator!=(const RwV3d& lhs, const RwV3d& rhs);
const RwV3d operator-(const RwV3d& rhs);
const RwMatrix operator*(const RwMatrix& lhs, const RwMatrix& rhs);
    
const RwV2d operator+(const RwV2d& lhs, const RwV2d& rhs);
const RwV2d operator-(const RwV2d& lhs, const RwV2d& rhs);
const RwV2d operator*(const RwV2d& lhs, const RwReal& rhs);
const RwV2d operator/(const RwV2d& lhs, const RwReal& rhs);
const void	operator*=(RwV2d& lhs, const RwReal& rhs);

/************************************************************************/
/* Spline 보간 계산 함수                                                */
/************************************************************************/

// u : Weight 값
// u2 : Weight의 제곱값
// u3 : Weight의 삼제곱값
// Cntrl : 컨트롤 포인트들 

#define _XSL_B_SPLINE(u, u_2, u_3, cntrl0, cntrl1, cntrl2, cntrl3)		\
    (																	\
    (																	\
    (-1.0f*u_3 + 3.0f*u_2 - 3.0f*u + 1.0f) * (cntrl0) +					\
    ( 3.0f*u_3 - 6.0f*u_2 + 0.0f*u + 4.0f) * (cntrl1) +					\
    (-3.0f*u_3 + 3.0f*u_2 + 3.0f*u + 1.0f) * (cntrl2) +					\
    ( 1.0f*u_3 + 0.0f*u_2 + 0.0f*u + 0.0f) * (cntrl3)					\
    ) / 6.0f															\
    ) 

#define _XSL_CATMULL_ROM_SPLINE(u, u_2, u_3, cntrl0, cntrl1, cntrl2, cntrl3)	\
    (																    		\
    ( 																			\
    (-1.0f*u_3 + 2.0f*u_2 - 1.0f*u + 0.0f) * (cntrl0) + 						\
    ( 3.0f*u_3 - 5.0f*u_2 + 0.0f*u + 2.0f) * (cntrl1) + 						\
    (-3.0f*u_3 + 4.0f*u_2 + 1.0f*u + 0.0f) * (cntrl2) + 						\
    ( 1.0f*u_3 - 1.0f*u_2 + 0.0f*u + 0.0f) * (cntrl3)   						\
    ) / 2.0f																	\
    ) 

#define HERMITE_SPLINE(u, u_2, u_3, cntrl0, cntrl1, cntrl2, cntrl3)				\
    ( 																				\
    ( 2.0f*u_3 - 3.0f*u_2 + 0.0f*u + 1.0f) * (cntrl0) + 						\
    (-2.0f*u_3 + 3.0f*u_2 + 0.0f*u + 0.0f) * (cntrl1) + 						\
    ( 1.0f*u_3 - 2.0f*u_2 + 1.0f*u + 0.0f) * (cntrl2) + 						\
    ( 1.0f*u_3 - 1.0f*u_2 + 0.0f*u + 0.0f) * (cntrl3)   						\
    )

#define BEZIER_SPLINE(u, u_2, u_3, cntrl0, cntrl1, cntrl2, cntrl3)				\
    (																				\
    (-1.0f*u_3 + 3.0f*u_2 - 3.0f*u + 1.0f) * (cntrl0) + 						\
    ( 3.0f*u_3 - 6.0f*u_2 + 3.0f*u + 0.0f) * (cntrl1) + 						\
    (-3.0f*u_3 + 3.0f*u_2 + 0.0f*u + 0.0f) * (cntrl2) + 						\
    ( 1.0f*u_3 + 0.0f*u_2 + 0.0f*u + 0.0f) * (cntrl3)     						\
    )

#endif
