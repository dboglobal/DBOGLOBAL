//***********************************************************************************
//	File		:	VenusFramework.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "NtlPostEffectCamera.h"

// 카메라 View 
enum EVenusCameraStaticView
{
    E_STATIC_VIEW_FRONT,
    E_STATIC_VIEW_BACK,
    E_STATIC_VIEW_UP,
    E_STATIC_VIEW_DOWN,
    E_STATIC_VIEW_LEFT,
    E_STATIC_VIEW_RIGHT,
};

/**
 * \ingroup Venus
 * \brief Venus의 카메라 클래스. PostEffectCamera로부터 상속받아서 뽀샤시 기능이 된다.
 * \date 2006-08-01
 * \author agebreak
 */
class CVenusCamera : public CNtlPostEffectCamera
{
public:
	CVenusCamera(void);
	virtual ~CVenusCamera(void);

	void	Destroy();
	virtual RwCamera* Create( RwInt32 width,
						RwInt32 height, 
						RwBool zBuffer, 
						RwReal fNearClipPlane = 1.f, 
						RwReal fFarClipPlane = 1000.f);

    void	MouseMessages(UINT uMsg, POINT point, short zDelta = 0, UINT nFlags = 0);   ///< 마우스 메시지를 전달한다.

    /// View Matrix를 초기화 한다.
    void	InitViewMatrix();
    void	InitViewMatrix(RwCamera* pCamera);

    /// View Matrix를 업데이트 한다.
    void	UpdateViewMatrix(RwV3d* pMove);
    void	UpdateViewMatrix();
    void	UpdateViewMatrix(RwCamera* pCamera);

    RwMatrix*	GetViewMatrix();        ///< 현재 카메라 Matrix를 반환한다.
    RwV3d*	    GetViewPosition();      ///< 현재 카메라 위치를 반환한다.

    RwBool	GetArrowMatrix(RwMatrix& matArrow);
    RwBool	GetClumpMatrix(RwMatrix& matClump);

    // Get/Set 메소드
    void   SetTargetPosition(const RwV3d vTargetPosition) {m_vTargetPosition = vTargetPosition;};    ///< 타겟 위치를 설정한다.
    RwV3d  GetTargetPosition() {return m_vTargetPosition;} ///< LootAt 타겟 위치를 반환한다.

    void   SetViewDistance(RwReal fViewDistance) {m_fViewDistance = fViewDistance;} ///< 시야거리를 설정한다.

    RwReal GetMoveSpeed() {return m_fMoveSpeed;}            ///< Move Speed를 반환한다.
    void   SetMoveSpeed(const RwReal fMoveSpeed) {m_fMoveSpeed = fMoveSpeed;} ///< Move Speed를 반환한다.

    RwReal GetRotationSpeed() {return m_fRotateSpeed;};     ///< Rotation Speed를 반환한다.
    void   SetRotationSpeed(const RwReal fRotationSpeed) {m_fRotateSpeed = fRotationSpeed;} ///< 현재 회전 속도를 반환한다.

    RwReal GetWheelSpeed() {return m_fWheelSpeed;}          ///< Wheel Speed를 반환한다.
    void   SetWheelSpeed(const RwReal fWheelSpeed) {m_fWheelSpeed = fWheelSpeed;} ///< Wheel Speed를 설정한다.

    void	GetMouseRay(const POINT& point, RwV3d& vRayPos, RwV3d& vRayDir);

    void    SetStaticView(EVenusCameraStaticView eStaticView);      ///< 이미 정의된 View 방향으로 카메라를 위치시킨다.

    static	CVenusCamera&	GetInstance(void);

protected:
    void	RotateViewMatrix(POINT gap, RwCamera* pCamera);
    void	MoveViewMatrix(POINT gap, RwCamera* pCamera);

protected:
    RwReal		m_fViewDistance;                ///< 시야 거리
    RwV3d		m_vTargetPosition;              ///< Look 타겟의 위치

    RwBool		m_bRButtonDown;                 ///< 마우스 오른쪽 버튼 눌림 여부
    POINT		m_RButtonDownPosition;          ///< 마우스 오른쪽 버튼이 눌렸을때의 위치

    RwBool		m_bMButtonDown;                 ///< 마우스 휠 버튼 눌림 여부
    POINT		m_MButtonDownPosition;          ///< 마우스 휠 버튼이 눌렸을때의 위치

    RwReal		m_fMoveSpeed;                   ///< 카메라 Move Speed
    RwReal		m_fRotateSpeed;                 ///< 카메라 회전 Speed
    RwReal		m_fWheelSpeed;                  ///< 카메라 휠 Speed (거리 조절)
};
