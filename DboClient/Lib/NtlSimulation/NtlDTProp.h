/*****************************************************************************
 *
 * File			: NtlDTProp.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 02	
 * Abstract		: direct property
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_DTPROP_H__
#define __NTL_DTPROP_H__

#include <unordered_map>
#include "NtlSerializer.h"

//////////////////////////////////////////////////////////////////////////////////////////
// direct property define

#define DT_VER			0.1f			
#define DT_FILE_EXT		"dsf"


//////////////////////////////////////////////////////////////////////////////////////////
// direct property class

#define DTPROP_SERIALIZE_NAME			0
#define DTPROP_SERIALIZE_TRANSFORM		1
#define DTPROP_SERIALIZE_END			0xff

class CNtlDTProp
{
protected:
	
	std::string m_strName;	/** property key name */
	RwMatrix	m_matTrans; /** transform matrix */
	RwReal		m_fPitch;	/** x-axis angle */
	RwReal		m_fYaw;		/** y-axis angle */
	RwReal		m_fRoll;	/** z-axis angle */
	RwV3d		m_vScale;	/** scale */
	RwV3d		m_vPos;		/** position */

protected:

	void		CalcTransform(void);

public:

	CNtlDTProp();
	virtual ~CNtlDTProp() {}

	virtual RwBool Load(CNtlSerializer& s);
	virtual RwBool Save(CNtlSerializer& s);

	virtual RwBool	Update(RwReal fElapsed) { return TRUE; }

	void			SetName(const RwChar *pName);
	const RwChar*	GetName(void);

	virtual void	SetData(void *pData)	{ }
	virtual void*	GetData(void)			{ return NULL; }

	
	/**
	*	transform
	*/

	void			AddAngle(RwReal fPitch, RwReal fYaw, RwReal fRoll);
	void			AddScale(RwReal fScale);
	void			AddPosition(RwV3d vAdd);
	void			ResetTransform(void);

	const RwMatrix*	GetTransMatrix(void);

	/**
	*	point control
	*/

	virtual void	AddControlPoint(void) {}
	virtual void	RemoveControlPoint(void) {}

	virtual void	ChangeControlPointTime(RwInt32 iSx, RwInt32 iEx, RwReal fTime) {}
	virtual void	AddControlPointPosition(RwInt32 iSx, RwInt32 iEx, RwV3d vAdd) {}
	virtual void	AddCurveRotate(RwInt32 iSx, RwInt32 iEx, RwReal fFront, RwReal fYaw) {}
	virtual RwReal	GetSumPointTime(RwInt32 iSx, RwInt32 iEx) { return 0.0f; }

	virtual RwV3d	GetSplineControlPointWorldPosition(RwInt32 iIdx);


	/**
	*	build
	*/
	virtual void	Build(void)	 { }
};


//////////////////////////////////////////////////////////////////////////////////////////
// direct spline property class


enum EDTSplineShape
{
	EDT_SPLINE_SHAPE_LINE,
	EDT_SPLINE_SHAPE_CIRCLE,
	EDT_SPLINE_SHAPE_HURRICANE
};


struct SSplineCtrlPt
{
	RwV3d	vPos;
	RwV3d	vRot;
	RwReal	fTime;
};

struct SSplineCurvePt
{
	RwV3d	vPos;
	RtQuat	qRot;
	RwReal	fTime;
};

struct SSplineProp
{
	EDTSplineShape	eShape;					/** line, circle, hurricane */
	RwReal			fTime;					/** spline 총 시간 */
	RwInt32			iCtrlNum;				/** control point num */
	RwInt32			iCurveSubDivision;		/** curve sub division num */
	RwReal			fRadius;				/** circle radius */
	RwInt32			iRotNum;				/** circle rotate num */
	RwReal			fDist;					/** circle 진행 방향 최종 거리 */
};

#define SPLINE_CUSTOM_SOUND_FILNE_LEN				64

struct SSplineCustomData
{
	RwBool bSoundLoop;
	RwChar chSoundFile[SPLINE_CUSTOM_SOUND_FILNE_LEN];
};


#define DTPROP_SPLINE_SERIALIZE_SHAPE				0
#define DTPROP_SPLINE_SERIALIZE_TIME				1
#define DTPROP_SPLINE_SERIALIZE_CTRLNUM				2
#define DTPROP_SPLINE_SERIALIZE_CURVESUB			3
#define DTPROP_SPLINE_SERIALIZE_RADIUS				4
#define DTPROP_SPLINE_SERIALIZE_ROTNUM				5
#define DTPROP_SPLINE_SERIALIZE_DIST				6
#define DTPROP_SPLINE_SERIALIZE_TOTCURVECNT			7
#define DTPROP_SPLINE_SERIALIZE_CTRLDATA			8
#define DTPROP_SPLINE_SERIALIZE_CURVEDATA			9
#define DTPROP_SPLINE_SERIALIZE_STANDARDPOS_FLAG	10
#define DTPROP_SPLINE_SERIALIZE_STANDARDPOS			11
#define DTPROP_SPLINE_SERIALIZE_UNIFORMLERP			12
#define DTPROP_SPLINE_SERIALIZE_UNIFORMSPEED		13

#define DTPROP_SPLINE_SERIALIZE_CUSTOMDATA			100

#define DTPROP_SPLINE_SERIALIZE_END					0xff

class CNtlDTSpline : public CNtlDTProp
{
private:

	SSplineProp			m_sProp;
	SSplineCtrlPt		*m_pCtrlPt;
	SSplineCurvePt		*m_pCurvePt;
	RwInt32				m_iTotCurveCnt;

	RwBool				m_bUseWorldPos;		/** spline world stand world position 사용 유무 */
	RwBool				m_bUniformLerp;
	RwReal				m_fUniformSpeed;

	SSplineCustomData	**m_ppCustomData;

private:

	void Allocate(SSplineProp *pProp);
	void DeAllocate(void);

	void InsertCustomData(RwInt32 iNewTotNum, RwInt32 iOldTotNum, RwInt32 iBeofreIdx, RwInt32 iAfterIdx);
	void RemoveCustomData(RwInt32 iNewTotNum, RwInt32 iOldTotNum); 	
	void RemoveCustomData(RwInt32 iNewTotNum, RwInt32 iOldTotNum, RwInt32 iBeofreIdx, RwInt32 iAfterIdx);

	void ReCalcTotTime(void);

	// line
	void BuildLinePoint(void);
	// circle
	void BuildCirclePoint(void);
	// Hurricane
	void BuildHurricanePoint(void);

	void BuildCurvePoint(void);
	void BuildCurveLineRotate(void);
	void BuildCurveRotate(void);
	void BuildCurveTime(void);

public:

	CNtlDTSpline();
	~CNtlDTSpline();

	virtual RwBool Load(CNtlSerializer& s);
	virtual RwBool Save(CNtlSerializer& s);

	RwBool	Update(RwReal fElapsed);

	void	SetData(void *pData);
	void*	GetData(void);

	void	Build(void);

	/**
	*	point control
	*/

	void	SetCurveRotate(RwInt32 iSx, RwInt32 iEx, RwV3d vAt);
	void	SetCurveRotate(RwInt32 iSx, std::list<RwV3d>& listAt);
	void	SetCurvePointDirectRotate(RwInt32 iSx, RwInt32 iEx);
	void	SetCurveRotateInterpolation(RwInt32 iSx, RwInt32 iEx);
	
	void	InsertControlPoint(RwInt32 iIdx, const RwV3d *pPos, const RwV3d *pDir);
	void	RemoveControlPoint(void);
	void	RemoveControlPoint(RwInt32 iIdx);

	void	ChangeControlPointTime(RwReal fSumTime);
	void	ChangeControlPointTime(RwInt32 iSx, RwInt32 iEx, RwReal fTime);
	void	SetControlPointPosition(RwInt32 iSx, RwInt32 iEx, RwV3d vPos);
	void	SetControlPointInterpolation(RwInt32 iSx, RwInt32 iEx);
	void	AddControlPointPosition(RwInt32 iSx, RwInt32 iEx, RwV3d vDelta);
	void	AddCurveRotate(RwInt32 iSx, RwInt32 iEx, RwReal fFront, RwReal fYaw);
	RwReal	GetSumPointTime(RwInt32 iSx, RwInt32 iEx);

public:

	RwV3d					GetSplineControlPointWorldPosition(RwInt32 iIdx);
	RwV3d					GetSplineControlPointRotate(RwInt32 iIdx);
	void					GetSplineControlPointRotate(RwInt32 iIdx, std::list<RwV3d>& listRot);
	
	RwInt32					GetTotCurveCount(void) const;
	const SSplineProp*		GetSplineProp(void);
	const SSplineCtrlPt*	GetSplineControlPoint(void) const;
	const SSplineCurvePt*	GetSplineCurvePoint(void) const;
	SSplineCustomData*		GetSplineCustomData(RwInt32 iCurvePointIdx);

	void					SetEnableWorldPosition(RwBool bEnable);
	RwBool					IsUseWorldPosition(void) const;
	
	void					SetEnableUniformLerp(RwBool bEnable);
	RwBool					IsUniformLerp(void) const;

	void					SetUniformSpeed(RwReal fSpeed);
	RwReal					GetUniformSpeed(void) const;

	void					SetSplineCustomSoundData(RwInt32 iCurvePointIdx, const RwChar *pFileName, RwBool bLoop); 
	void					ClearSplineCustomData(RwInt32 iCurvePointIdx);
	RwInt32					GetValidSplineCustomDataCount(void);
};

inline void* CNtlDTSpline::GetData(void)
{
	return &m_sProp; 
}

inline RwInt32 CNtlDTSpline::GetTotCurveCount(void) const 
{
	return m_iTotCurveCnt;
}


inline const SSplineProp* CNtlDTSpline::GetSplineProp(void) 
{
	return &m_sProp;
}

inline const SSplineCtrlPt*	CNtlDTSpline::GetSplineControlPoint(void) const
{
	return m_pCtrlPt;
}

inline const SSplineCurvePt* CNtlDTSpline::GetSplineCurvePoint(void) const
{
	return m_pCurvePt;
}

inline SSplineCustomData* CNtlDTSpline::GetSplineCustomData(RwInt32 iCurvePointIdx)
{
	if(m_ppCustomData == NULL)
		return NULL;

	if(iCurvePointIdx < 0 || iCurvePointIdx >= m_iTotCurveCnt)
		return NULL;

	return m_ppCustomData[iCurvePointIdx];
}

inline void CNtlDTSpline::SetEnableWorldPosition(RwBool bEnable)
{
	m_bUseWorldPos = bEnable;
}

inline RwBool CNtlDTSpline::IsUseWorldPosition(void) const
{
	return m_bUseWorldPos;
}

inline void	CNtlDTSpline::SetEnableUniformLerp(RwBool bEnable)
{
	m_bUniformLerp = bEnable;
}

inline RwBool CNtlDTSpline::IsUniformLerp(void) const
{
	return m_bUniformLerp;
}

inline void	CNtlDTSpline::SetUniformSpeed(RwReal fSpeed)
{
	m_fUniformSpeed = fSpeed;
}

inline RwReal CNtlDTSpline::GetUniformSpeed(void) const
{
	return m_fUniformSpeed;
}


//////////////////////////////////////////////////////////////////////////////////////////
// direct property container class


#define DTCONTAINER_SERIALIZE_VER			0
#define DTCONTAINER_SERIALIZE_PROPKEY		1
#define DTCONTAINER_SERIALIZE_PROPDATA		2
#define DTCONTAINER_SERIALIZE_END			0xff

typedef std::unordered_map<std::string, CNtlDTProp*> HMapDTProp;

class CNtlDTContainer
{
private:

	HMapDTProp	m_hmapDTProp;


	
public:

	CNtlDTContainer();
	~CNtlDTContainer();

	static CNtlDTContainer* GetInstance(void);

	RwBool Create(const RwChar *pFileName);
	void Destroy(void);

	RwBool Load(const RwChar *pFileName);
	RwBool Save(const RwChar *pFileName);

	CNtlDTProp* CreateSplineProp(SSplineProp& sProp);
	void DeleteProp(CNtlDTProp *pProp);
	void RenameProp(CNtlDTProp *pProp, const RwChar *pNewName);
	void Append(CNtlDTContainer& container);
	
	CNtlDTProp* Find(const RwChar *pName);

	HMapDTProp& Gets(void);
};


//////////////////////////////////////////////////////////////////////////////////////////
// direct property rendering class


#define DT_RENDER_MARK
#define DT_RENDER_MARK_NONE					0x00
#define DT_RENDER_MARK_CONTROL_POINT		0x01
#define DT_RENDER_MARK_CURVE_POINT			0x02
#define DT_RENDER_MARK_CURVE_ROTATE			0x04
#define DT_RENDER_MARK_ALL					0x07


class CNtlDTRender
{
private:

	static RwUInt8 m_byDTRenderMark;

	static void SplineRender(CNtlDTSpline *pDTSpline, RwMatrix* pWorldMatrix, int nSx, int nEx, RwReal fBoxSize);

public:

	static void		IncludeRenderMarkFlag(RwUInt8 byMarkFlag);
	static void		ExcludeRenderMarkFlag(RwUInt8 byMarkFlag);
	static RwUInt8	GetRenderMarkFlags(void);

	static void DTPropRender(CNtlDTProp *pDTProp, RwMatrix* pWorldMatrix, RwReal fBoxSize, int nSx = -1, int nEx = -1);
	static void DTPropFocusPointRender(CNtlDTProp *pDTProp, RwMatrix* pWorldMatrix, int nIdx, RwReal fBoxSize);
	static void DTPropValidCustomDataRender(CNtlDTProp *pDTProp, RwMatrix* pWorldMatrix, RwReal fBoxSize);
};



#endif
