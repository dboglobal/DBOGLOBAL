//***********************************************************************************
//	File		:	VenusGrid.h
//	Desc		:	
//	Begin		:	2004.9.30
//	Copyright	:	ⓒ 2004 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "rwcore.h"


/**
 * \ingroup Venus
 * \brief 화면에 Grid를 그리는 클래스
 * \date 2006-08-01
 * \author agebreak
 */
class CVenusGrid
{
public:
	CVenusGrid(void);
	~CVenusGrid(void);

	static	CVenusGrid&	GetInstance(void);

	RwBool	Create();
	RwBool	Destroy();

	RwBool	CreateGrid(RwInt32 nTileCount);                     ///< 그리드를 생성한다.
	

	RwBool	Render();                                           ///< 그리드를 화면에 렌더링 한다.

    /// Intersection된 포인터를 찾는다.
    RwBool	Intersection3dPoint(RwV3d* pIntersectionPoint, const RwV3d* pCenter, const RwV3d* pRayPos, const RwV3d* pRayDir);
    RwBool	IntersectionStart(POINT point, const RwV3d* pCenter);
    void	IntersectionEnd();
    RwBool	IntersectionMove(RwV3d* pMove, POINT point, const RwV3d* pCenter);

    // Get/Set 메소드
    void    SetTileCount(RwInt32 nTileCont) {m_nTileCount = nTileCont;} ///< 타일 개수를 설정한다.
    RwInt32 GetTileCount() {return m_nTileCount;}                       ///< 타일 개수를 반환한다.

    RwBool	SetGridWidth(RwReal fGridWidth);                            ///< 그리드의 넓이를 설정한다.
    RwReal  GetGridWidth() {return m_fGridWidth;}                       ///< 그리디의 넓이를 반환한다.

    RwBool  GetIntersection() {return m_bIntersection;}                 ///< Intersection 유무를 반환한다.

protected:
    RwInt32					m_nTileCount;                       ///< 타일 개수
    RwInt32					m_nVertexCount;                     ///< 버텍스 개수
    RwReal					m_fGridWidth;                       ///< Grid 넓이
    RwIm3DVertex*			m_pVertex;                          ///< 버텍스 버퍼의 포인터
    RwBool					m_bIntersection;                    ///< Intersection 유무
    RwV3d					m_vOldIntersectionPoint;            ///< 이전 Intersection 위치
    RwIm3DVertex			m_IntersectionPlaneVertex[4];       ///< Intersection된 Plane의 버텍스
    RwV3d					m_vOldCenter;                       ///< 이전 중심점
};
