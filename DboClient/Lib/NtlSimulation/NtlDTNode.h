/*****************************************************************************
 *
 * File			: NtlDTNode.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 02	
 * Abstract		: direct node
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_DTNODE_H__
#define __NTL_DTNODE_H__

#include "NtlDTProp.h"

class CNtlSob;

enum EDirectLookAtType	
{
	DIRECT_LOOKAT_NOT_USE,

	// camera의 look at은 spline data에 의해 결정되고, 전혀 transform 시키지 않는다. 
	DIRECT_LOOKAT_SELF,			

	// camera의 look at은 spline data에 의해 결정되고, 
	// 연출이 시작할 때 외부에서 입력된 방향으로 spline data를 모든 축으로 rotate 한다.
	DIRECT_LOOKAT_SELF_ALLAXIS,		

	// camera의 look at은 spline data에 의해 결정되고, 
	// 연출이 시작할 때 외부에서 입력된 방향으로 spline data를 Y 축으로만 rotate 한다.
	DIRECT_LOOKAT_SELF_YAXIS,				
	
	// camera의 look at은 camera의 현재 position과 look at position 에 의해 항상 변경이 된다.
	// 연출이 시작할 때 외부에서 입력된 target position으로 spline data를 scale 및 Y 축으로만 rotate 한다.
	DIRECT_LOOKAT_DYNAMIC,

	// spline position을 ground height check를 하여, 높이에 적용한다.
	DIRECT_GROUND_MOVE,

	// spline position을 ground height check를 하지 않는다.
	DIRECT_FLY_MOVE,

	DIRECT_LOOKAT_END = DIRECT_FLY_MOVE
};

class CNtlDTNodeBase
{
protected:

	RwBool m_bFinish;

public:
	
	CNtlDTNodeBase();
	virtual ~CNtlDTNodeBase() {}

	virtual void	SetData(const CNtlDTProp *pProp, const CNtlSob *pSobObj) {}
	virtual void	Update(RwReal fElapsed) {}

	RwBool	IsFinish(void) const { return m_bFinish; }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTSplineNode : public CNtlDTNodeBase
{
private:

	CNtlDTSpline	*m_pProp;
	CNtlSob			*m_pSobObj;

	RwReal			m_fCurrTime;		/** 현재의 흐른 시간 */
	RwReal			m_fPtTime;			/** 현재 spline point의 time */
	RwInt32			m_iPtIdx;			/** 현재 curve data index */
	RwV3d			m_vPtPos;
	
	EDirectLookAtType	m_eLookAtType;
	
	RwMatrix		m_matTrans;			/** transform matrix */
	RwMatrix		m_matTransRot;		/** transform roate matrix */
	RwMatrix		m_matRot;			/** rotate temp matrix */
	RwV3d			m_vLookAtPos;		/** camera look at position */

private:

	void UniformTypeUpdate(RwReal fElapsed);
	void TimeSequencetypeUpdate(RwReal fElapsed);

	void GroundMoveUniformUpdate(RwReal fElapsed);
	void GroundMoveTimeSequencetypeUpdate(RwReal fElapsed);

	void SetLookAtSelf(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtSelfAllAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtSelfYAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtSelfScaleAllAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtSelfScaleYAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);
	void SetLookAtDynamic(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos);

public:

	CNtlDTSplineNode();
	~CNtlDTSplineNode();

	void SetData(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos, EDirectLookAtType eLookAtType);

	void SetData(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos, RwV3d vTransDir);

	void SetDataGroundMove(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos);

	void SetDataFlyMove(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos);
	
	void Update(RwReal fElapsed);
};

#endif