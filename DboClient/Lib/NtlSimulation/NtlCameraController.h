/*****************************************************************************
 *
 * File			: NtlCameraController.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 8. 08	
 * Abstract		: simulation layer camera controller
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_CAMERA_CONTROLLER_H__
#define __NTL_CAMERA_CONTROLLER_H__

#include "NtlSob.h"
#include "NtlDebug.h"

enum ECameraControlType
{
	CAMERA_CONTROL_NORMAL,
	CAMERA_CONTROL_SHAKE,
	CAMERA_CONTROL_DASH,
	CAMERA_CONTROL_NPC,
	CAMERA_CONTROL_DRAGONBALL,
    CAMERA_CONTROL_TIMEMACHINE,
	CAMERA_CONTROL_DT,					/**	direct control */
    CAMERA_CONTROL_FREE,				/** Free Camera Control */
	CAMERA_CONTROL_EXPLOSION,			/** Explosion camera control */
	CAMERA_CONTROL_TIMEMACHINE_ARRIVE,	/** Timemachine arrive camera control */
	CAMERA_CONTROL_KNOCKDOWN_MATRIX,	/** knockdown matrix 연출 camera control */
    CAMERA_CONTROL_FPS,                 /** 1인칭 카메라 컨트롤러 */
	CAMERA_CONTROL_BUS,					/** bus camera control */
	CAMERA_CONTROL_TURORIAL,			/** tutorial camera control */
    CAMERA_CONTROL_OBSERVER,            ///< 천하제일 무도회에서 사용하는 구경용 카메라
};

// controller flags
#define NTL_CAMERA_CONTROL_NORMAL_PAUSE		0x01
#define NTL_CAMERA_CONTROL_NOT_ROTATE		0x02
#define NTL_CAMERA_CONTROL_NOT_ZOOM			0x04

class CNtlCamera;
class CNtlSobActor;
class CNtl2DTimeSequence;
class CNtl3DTimeSequence;

/// 카메라 위치 정보를 담기위한 구조체
struct SCameraData                  
{
    RwReal fTime;                   ///< 시간
    RwV3d  vPos;                    ///< 카메라 위치
    RwV3d  vTargetPos;              ///< 카메라 타겟 위치        
};

typedef std::vector<SCameraData> VecCamaraData;


class CNtlCameraController
{
protected:

	RwBool m_bPause;
	RwBool m_bFinish;
	RwUInt8 m_byType;
	RwUInt8 m_byFlags;
	static RwMatrix m_matTrans;		// camera trans matrix
    static RwReal m_fTargetHeightRatio;

	float	m_fHeightRatioBonus;

protected:
    CNtlCameraController();
	virtual void UpdateCameraTrans(RwV3d *pCamPos, const RwV3d *pLookAt, RwReal fDistOffset = 0.0f);
    virtual void SetVisibleAllObject( RwBool bVisible );            //< 객체들의 보임을 설정한다.
	RwReal GetActorOffset(CNtlSobActor *pActor);


public:
    virtual ~CNtlCameraController();
	virtual void Enter(void) {}
	virtual void Exit(void) {}
	virtual void Update(RwReal fElapsed) {}
	virtual void UpdateData(RwBool bUpdate = TRUE) {}
	virtual void ZoomInOut(RwReal fDelta) {}
	virtual void MoveSmoothing(RwBool bEnable) {}
	virtual void SetActiveActor(const CNtlSobActor *pActor) {}
	virtual RwBool IsCollisionCheck(void)	{ return TRUE; }
	virtual RwBool IsInterpCheck(void)		{ return TRUE; }
	virtual void UserCameraControlEnable(RwBool bEnable) {}
	virtual void ResetCamera(void) {}    

	void SetType(RwUInt8 byType);
	RwUInt8 GetType(void) const;

	void SetFlags(RwUInt8 byFlags);
	RwUInt8 GetFlags(void) const;

	void Pause(RwBool bPause);
	RwBool IsPause(void) const;
	
	void Finish(void);
	RwBool IsFinish(void) const;

    static void SetTargetHeightRatio(RwReal fRatio) {m_fTargetHeightRatio = fRatio;}            ///< Sets the height ratio of the camera target.

	void		SetTargetHeightRatioBonus(float fRatio) { m_fHeightRatioBonus = fRatio; }
};

inline void CNtlCameraController::SetType(RwUInt8 byType)
{
	m_byType = byType;
}

inline RwUInt8 CNtlCameraController::GetType(void) const
{
	return m_byType;
}

inline void CNtlCameraController::SetFlags(RwUInt8 byFlags)
{
	m_byFlags = byFlags;
}

inline RwUInt8 CNtlCameraController::GetFlags(void) const
{
	return m_byFlags;
}

inline void CNtlCameraController::Pause(RwBool bPause)
{
	m_bPause = bPause;
}

inline RwBool CNtlCameraController::IsPause(void) const
{
	return m_bPause;
}

inline void CNtlCameraController::Finish(void)
{
	m_bFinish = TRUE;
}

inline RwBool CNtlCameraController::IsFinish(void) const
{
	return m_bFinish;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlCameraNormalController : public CNtlCameraController
{

private:

	enum ECameraNormalFlag
	{
		CAMERA_NORAML_NONE				= 0x00,
		CAMERA_NORAML_ROTATE			= 0x01,
		CAMERA_NORAML_ZOOM_INOUT		= 0x02,
	};

	enum ECameraSMootingFlg
	{
		CAMERA_NORAML_SMOOTHING_NONE			= 0x00,
		CAMERA_NORAML_SMOOTHING_LERP			= 0x01,
		CAMERA_NORAML_SMOOTHING_FOLLOW_START	= 0x02,
		CAMERA_NORAML_SMOOTHING_FOLLOW_ADJUST	= 0x03,
		CAMERA_NORAML_SMOOTHING_FOLLOW			= 0x04,
		CAMERA_NORAML_SMOOTHING_STOP			= 0x05
	};

	RwUInt8 m_byUpdateFlags;		// camera의 update를 나타내는 flag.
	RwUInt8	m_bySmoothingFlag;		// camera의 update smoothing 보간.
	RwReal	m_fSmoothTime;			// smooth time.
	RwReal	m_fSmoothDist;			// smooth distance
	RwBool	m_bSmoothLookAtEnable;	// smooth look at enable
	RwV3d	m_vSmoothLookAt;		// smooth look at position
	RwV3d	m_vPrevActorPos;		// previous actor position.
	
	RwReal	m_fDesiredDist;		// 스프링 효과를 위해 썼다.
	RwReal	m_fDeltaDist;		// 마우스 wheel delta distance.
	RwV3d	m_vLerpPos;			// 카메라 보정 offset.
	RwReal	m_fLerpSpeed;		// 카메라 보정 speed.

	CNtlSobActor *m_pActor;

private:

	void EnableRotate(RwBool bEnable);
	void EnableZoomInOut(RwBool bEnable);
	void EnableSmoothingLerp(RwBool bEnable);

	RwV3d	GetLookAt(CNtlSobActor *pActor);
	RwBool	UpdateSmoothLookAt(RwV3d& vLookAt, RwReal fElapsed, RwReal fSpeed);

	void	UpdateZoomInOut(RwReal fElapsed);
	void	UpdateSmoothSmoothNone(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed);
	void	UpdateSmoothLerp(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed);
	void	UpdateSmoothFollowStart(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed);
	void	UpdateSmoothFollowAdjust(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed);
	void	UpdateSmoothSmoothFollow(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed);
	void	UpdateSmoothSmoothStop(RwV3d& vCamPos, RwV3d& vLookAt, RwReal& fDistOffset, RwReal fElapsed);
	
public:
	CNtlCameraNormalController();
	~CNtlCameraNormalController();

	virtual void Update(RwReal fElapsed);
	virtual void UpdateData(RwBool bUpdate = TRUE);
	virtual void ZoomInOut(RwReal fDelta);
	virtual void MoveSmoothing(RwBool bEnable);
	virtual void ResetCamera(void);
	virtual void SetActiveActor(const CNtlSobActor *pActor);
	const CNtlSobActor* GetActiveActor(void);
};

//////////////////////////////////////////////////////////////////////////
// Create by agebreak 2008.02.11
// 1인칭 카메라 컨트롤러
class CNtlCameraFPSController : public CNtlCameraController
{
public:
    CNtlCameraFPSController();
    virtual ~CNtlCameraFPSController();

    virtual void Enter();
    virtual void Exit();
    virtual void Update(RwReal fElapsed);
    virtual void SetActiveActor(const CNtlSobActor *pActor);
    virtual RwBool IsCollisionCheck(void)	{ return FALSE;}
	virtual RwBool IsInterpCheck(void)		{ return FALSE; }
    virtual void ZoomInOut(RwReal fDelta);
    virtual void HandleEvents(RWS::CMsg &pMsg); 
    
    void    SetLookAtDir(RwV3d& vCurrLookAt, RwV3d& vTargetLookAt);
    void    EnableZoomInOut(RwBool bEnable);               ///< 줌이 가능한지 설정한다 (스카우터는 줌이 안된다)    
    void    AddPitch(RwReal fDeltaPitch);
    void    AddYaw(RwReal fDeltaYaw);
protected:

protected:
    CNtlSobActor *m_pActor;                 ///< 메인이 되는 Actor
    RwBool        m_bEnableZoom;            ///< 줌 가능 플래그
	RwReal		  m_fOldDist;				///< 이전 카메라 distance    
    RwV3d         m_vStartPos;              ///< 원래 카메라 위치
    RwV3d         m_vTargetPos;             ///< 변화될 1인칭 카메라 위치
    RwV3d         m_vCurrLookAt;            ///< 원래 보고 있는 방향
    RwV3d         m_vTargetLookAt;          ///< 변경될 보고 있는 방향
    RwReal        m_fInterTime;             ///< 보간되는 시간 누적    
    RwBool        m_bInter;                 ///< 현재 보간중인지 여부 플래그
    RwReal        m_fOrgPitch;              ///< 원래 카메라 Pitch 값
    RwReal        m_fOrgYaw;                ///< 원래 카메라 Yaw 값
    RwUInt32      m_uiMoveFlag;             ///< 키보드 이동 Flag
};

//////////////////////////////////////////////////////////////////////////
/// Create by agebreak 2007.11.27
/// 자유롭게 이동할 수 있는 프리 카메라 컨트롤러 (DT 에디터에서 사용한다.)
class CNtlFreeCameraController : public CNtlCameraController
{
public:
    CNtlFreeCameraController();
    virtual ~CNtlFreeCameraController();

    virtual void	Enter(void);
    virtual void	Exit(void);
    virtual void    Update(RwReal fElapsed);    
    virtual RwBool  IsCollisionCheck(void)	{ return FALSE; }           ///< 충돌체크를 하지 않는다.
	virtual RwBool	IsInterpCheck(void)		{ return FALSE; }

    
    void    Move(RwUInt32 moveType, RwReal fSpeed);          ///< 카메라를 이동시킨다.    
    void    Stop();
    void    UpdateRotate(const RwV3d& vCamPos);

    static void    SetMoveSpeed(RwReal fSpeed) {m_fMoveSpeed = fSpeed;} ///< 카메라 이동 속도를 세팅한다.
    static RwReal  GetMoveSpeed() {return m_fMoveSpeed;}                ///< 카메라 이동 속도를 반환한다.
    
protected:
    RwUInt32        m_vPrevMoveFlag;                        ///< 이전 무브 플래그를 저장한다. (키를 눌러도 지속적으로 플래그가 날라오지 않기때문에)
    RwReal          m_fPrevSpeed;                           ///< 이전 속도를 저장한다.
    static RwReal   m_fMoveSpeed;                           ///< 카메라 이동 속도
    RwBBox          m_box;                                  ///< 카메라 이동 한계    
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct SObserverCameraData
{
    RwInt32 nIndex;
    RwV3d   vPos;
    RwV3d   vTarget;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlCameraShakeController : public CNtlCameraController
{
private:

	RwReal	m_fShakeElapsedTime;
	static RwReal	m_fShakeFactor;
	static RwReal	m_fShakeMaxHeightFactor;

public:

	CNtlCameraShakeController();
	~CNtlCameraShakeController();

	virtual void Update(RwReal fElapsed);

	static void SetShakeFactor(RwReal fWeightValue, RwReal fHeightValue);
	static void GetShakeFactor(RwReal& fWeightValue, RwReal& fHeightValue);
	static void ResetShakeFactor(void);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlCameraDashController : public CNtlCameraController
{
	enum ECameraDashState
	{
		CAMERA_DASH_STATE_START,
		CAMERA_DASH_STATE_LOOP,
		CAMERA_DASH_STATE_END
	};

private:

	RwUInt8			m_byCameraDashState;

	RwReal			m_fTime;
	RwReal			m_fStartTime;
	RwReal			m_fDashSpeed;
	RwBool			m_bArrival;
	
	RwV3d			m_vStartPos;
	RwV3d			m_vDestPos;
	
	CNtlSobActor	*m_pActor;

private:

	void ResetData(void);

	void CalcCameraTrans(RwReal fSpeed, RwReal fLimitLen, RwReal fElapsed);
	void CalcCameraEndTrans(RwReal fElapsed);

	void UpdateStart(RwReal fElapsed);
	void UpdateLoop(RwReal fElapsed);
	void UpdateEnd(RwReal fElapsed);

public:

	CNtlCameraDashController();
	~CNtlCameraDashController();

	virtual void Enter(void);
	
	virtual void Exit(void);

	virtual void Update(RwReal fElapsed);

	virtual void UpdateData(RwBool bUpdate = TRUE);

	virtual void SetActiveActor(const CNtlSobActor *pActor);

public:

	void SetDashSpeed(RwReal fDashSpeed);

	void SetDashDest(const RwV3d *pDest);
};

inline void CNtlCameraDashController::SetDashSpeed(RwReal fDashSpeed)
{
	m_fDashSpeed = fDashSpeed;
}

inline void CNtlCameraDashController::SetDashDest(const RwV3d *pDest)
{
	RwV3dAssignMacro(&m_vDestPos, pDest);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \ingroup NtlSimulation
 * \brief 상인 NPC를 클릭할 때 사용되는 카메라
 *
 * \date 2009-08-18
 * \author agebreak
 */
class CNtlCameraNpcController : public CNtlCameraController
{
	enum ECameraNpcState
	{
		CAMERA_NPC_STATE_NONE,
		CAMERA_NPC_STATE_START,
		CAMERA_NPC_STATE_LOOP,
		CAMERA_NPC_STATE_END
	};

public:

	CNtlCameraNpcController();
	~CNtlCameraNpcController();

	virtual void		Enter(void);
	virtual void		Exit(void);
	virtual void		Update(RwReal fElapsed);
	virtual void		UpdateData(RwBool bUpdate = TRUE);
	virtual void		SetActiveActor(const CNtlSobActor *pActor);
	virtual RwBool		IsCollisionCheck(void)	{ return FALSE; }
	virtual RwBool		IsInterpCheck(void)		{ return FALSE; }

private:
    void				UpdateStart(RwReal fElapsed);
    void				UpdateLoop(RwReal fElapsed);
    void				UpdateEnd(RwReal fElapsed);
    void				ChangeState(RwInt32 iState);    

private:
    ECameraNpcState		m_eState;
    SERIAL_HANDLE		m_hLookAtSerialId;
    CNtl2DTimeSequence	*m_pSeq;
    RwV3d				m_vStartPos;
    RwV3d				m_vIdleOffsetPos;

    RwV3d				m_vDir;
    RwMatrix			m_matRot;
    RwReal				m_fTime;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 그래픽팀으로 받은 데이터를 이용하는 카메라 컨트롤러 추상 클래스. 
// 관련 함수들이 구현되어 있다.
class CNtlCameraFromArtData : public CNtlCameraController
{
public:
    CNtlCameraFromArtData();
    ~CNtlCameraFromArtData();

    virtual void		Enter(void) = 0;	
    virtual void		Exit(void) = 0;
    virtual void		Update(RwReal fElapsed) = 0;	    
    virtual void		ChangeState(RwInt32 iState) = 0;
    virtual void		SetActiveActor(const CNtlSobActor *pActor);
    virtual RwBool		IsCollisionCheck(void)	{ return FALSE; }
	virtual RwBool		IsInterpCheck(void)		{ return FALSE; }

protected:
    virtual void        AddCameraData(RwReal fTime, RwReal posX, RwReal posY, RwReal posZ, RwReal targetX, RwReal targetY, RwReal targetZ, VecCamaraData* pVecCameraData);
    virtual void        CreateSequenceData(CNtl3DTimeSequence** ppSeqPos, CNtl3DTimeSequence** ppSeqTarget, VecCamaraData* pVecCamaraData);              ///< 시퀀스 데이터를 생성한다.
    virtual void        UpdateCameraTrans(RwV3d *pCamPos, const RwV3d *pLookAt, RwReal fDistOffset = 0.0f);

protected:
    RwReal				m_fTime;
    CNtlSobActor*       m_pActor;
    RwMatrix            m_matDir;
    RwV3d               m_ActPos;    
};


class CNtlCameraDragonBallController : public CNtlCameraFromArtData
{
public:
	enum ECameraDBState
	{
		CAMERA_DB_STATE_NONE,
		CAMERA_DB_STATE_START,
		CAMERA_DB_STATE_LOOP,
        CAMERA_DB_STATE_SCATTER,
		CAMERA_DB_STATE_END
	};    

public:

    CNtlCameraDragonBallController();
    ~CNtlCameraDragonBallController();

    virtual void		Enter(void);	
    virtual void		Exit(void);
    virtual void		Update(RwReal fElapsed);	
    virtual void		SetActiveActor(const CNtlSobActor *pActor);    
    void				ChangeState(RwInt32 iState);

protected:    
    void				UpdateStart(RwReal fElapsed);
    void				UpdateLoop(RwReal fElapsed);
    void                UpdateScatter(RwReal fElapsed);         ///< 카메라 흩어질때의 카메라 연출
    void				UpdateEnd(RwReal fElapsed);    
    void                CreateScatterData();    

protected:
	ECameraDBState		m_eState;    
	CNtl3DTimeSequence* m_pSeqPos;
    CNtl3DTimeSequence* m_pSeqTarget;		
    VecCamaraData       m_vecData;
    CNtl3DTimeSequence* m_pScatterSeqPos;
    CNtl3DTimeSequence* m_pScatterSeqTarget;
    VecCamaraData       m_vecScatterData;
    SCameraData         m_DataIdle;
    
    RwReal              m_fOrgFov;              ///< 원래 FOV값
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 타임머신 연출용 카메라 컨트롤러
class CNtlCameraTimeMachineController : public CNtlCameraFromArtData
{
public:
    enum ECameraTMState
    {
        CAMERA_TM_STATE_NONE,
        CAMERA_TM_STATE_START,        
        CAMERA_TM_STATE_END,
    };

public:
    CNtlCameraTimeMachineController();
    ~CNtlCameraTimeMachineController();

    virtual void		Enter(void);	
    virtual void		Exit(void);
    virtual void		Update(RwReal fElapsed);	
    virtual void		SetActiveActor(const CNtlSobActor *pActor);
    virtual RwBool		IsCollisionCheck(void)	{ return FALSE; }
	virtual RwBool		IsInterpCheck(void)		{ return FALSE; }
    void				ChangeState(RwInt32 iState);

protected:
    void                UpdateStart(RwReal fElapsed);
    void                UpdateEnd(RwReal fElspase);

protected:
    ECameraTMState      m_eState;
    VecCamaraData       m_vecCameraData;
    CNtl3DTimeSequence* m_pSeqPos;
    CNtl3DTimeSequence* m_pSeqTarget;    
};

//////////////////////////////////////////////////////////////////////////


class CNtlDTNodeBase;
class CNtlDTProp;

class CNtlSobCameraNode : public CNtlSob
{
protected:

	typedef std::list<CNtlSobCameraNode*> ListChildNode;

	CINEMATIC_HANDLE	m_hCinematic;
	ListChildNode		m_listChildNode;

protected:

	CNtlSobCameraNode();

public:
	
	virtual ~CNtlSobCameraNode();
	
	virtual void	Update(RwReal fElapsed);

	virtual void	Enter(void);

	virtual void	Exit(void)	{}

	virtual RwBool	IsFinish();

	void			SetCinematicInstance(CINEMATIC_HANDLE hCinematic);

	void			AddChildNode(CNtlSobCameraNode *pChildNode);
};

class CNtlSobCameraActorDist : public CNtlSobCameraNode
{
private:

	RwReal			m_fCurrTime;
	RwReal			m_fLifeTime;

	RwBool			m_bDynamicLookAt;
	SERIAL_HANDLE	m_hLookAtSerialId;
	RwReal			m_fHeightOffset;
	RwReal			m_fAngleX;
	RwReal			m_fAngleY;
	RwReal			m_fDistance;
	RwV3d			m_vLookAt;

public:

	CNtlSobCameraActorDist();
	virtual ~CNtlSobCameraActorDist();

	virtual	void	Update(RwReal fElapsed);

	virtual void	Enter(void);

	virtual RwBool	IsFinish(void);

	void			SetData(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
							RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt);
	void			SetDynamicLookAtData(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
										RwReal fDistance, RwReal fLifeTime);
};

class CNtlSobCameraRotate : public CNtlSobCameraNode
{
private:

	enum	
	{
		CAMERA_DT_AXIS_X_ROTATE		= 0x01,
		CAMERA_DT_AXIS_Y_ROTATE		= 0x02,
		CAMERA_DT_AXIS_Z_ROTATE		= 0x04,
	};

	RwUInt8			m_byRotType;

	RwReal			m_fCurrTime;
	RwReal			m_fInterTime;
	RwReal			m_fLifeTime;

	RwReal			m_fCurrAngleX;
	RwReal			m_fCurrAngleY;
	RwReal			m_fCurrAngleZ;
	RwReal			m_fLastAngleX;
	RwReal			m_fLastAngleY;
	RwReal			m_fLastAngleZ;
	RwReal			m_fIncAngleX;
	RwReal			m_fIncAngleY;
	RwReal			m_fIncAngleZ;

public:

	CNtlSobCameraRotate();
	virtual ~CNtlSobCameraRotate();

	virtual	void	Update(RwReal fElapsed);

	virtual void	Enter(void);

	virtual void	Exit(void);

	virtual RwBool	IsFinish(void);

	void			SetAxisXData(RwReal fLastAngleX, RwReal fInterTime, RwReal fLifeTime);
	void			SetAxisYData(RwReal fLastAngleY, RwReal fInterTime, RwReal fLifeTime);
	void			SetAxisZData(RwReal fLastAngleZ, RwReal fInterTime, RwReal fLifeTime);

	void			SetAxisXYZData(RwReal fLastAngleX, RwReal fLastAngleY, RwReal fLastAngelZ, RwReal fInterTime, RwReal fLifeTime);
};

class CNtlSobCameraFov : public CNtlSobCameraNode
{
private:

	RwReal	m_fCurrTime;
	RwReal	m_fInterTime;

	RwReal	m_fCurrFov;
	RwReal	m_fIncFov;
	RwReal	m_fLastFov;
	
public:

	CNtlSobCameraFov();
	~CNtlSobCameraFov();

	virtual	void	Update(RwReal fElapsed);

	virtual void	Enter(void);

	virtual void	Exit(void);

	virtual RwBool	IsFinish(void);

	void			SetFovData(RwReal fLastFov, RwReal fInterTime);
};

class CNtlSobCameraPauseNode : public CNtlSobCameraNode
{
private:

	RwReal	m_fCurrTime;
	RwReal	m_fLifeTime;

	RwBool	m_bLoop;

public:

	CNtlSobCameraPauseNode();
	virtual ~CNtlSobCameraPauseNode();

	virtual	void	Update(RwReal fElapsed);

	virtual RwBool	IsFinish(void);

	void			SetData(RwReal fLifeTime, RwBool bLoop = FALSE); 
};


class CNtlSobCameraSplineNode : public CNtlSobCameraNode
{
public:

	enum EDTSplineMode
	{
		DT_SPLINE_POS_CAMERA_LOOKAT_ACTOR,
		DT_SPLINE_POS_ACTOR_LOOKAT_ACTOR,
		DT_SPLINE_POS_ACTOR_LOOKAT_TWO_ACTOR,
		DT_SPLINE_POS_LOOKAT_ACTOR,
		DT_SPLINE_SELF,
		DT_SPLINE_ACTOR_REF							// 명령이 동작되는 시점의 actor의 정보.
	};

	struct SSplineNodeLookAtActor
	{
		SERIAL_HANDLE	hLookAtSerialId;
		RwReal			fLookAtHeightOffset;
	};

private:

	std::string		m_strPropKey;
	CNtlDTNodeBase	*m_pDTNode;
	CNtlDTProp		*m_pDTProp;

	RwReal			m_fCurrTime;
	RwReal			m_fLifeTime;

	EDTSplineMode			m_eMode;
	SERIAL_HANDLE			m_hPosSerialId;
	SSplineNodeLookAtActor	m_sLookAtActor;
	RwV3d					m_vStartPos;
	RwReal					m_fAngleX;
	RwReal					m_fAngleY;
	RwReal					m_fAngleZ;

private:

	void			UpdateLookAtActorMode(RwReal fElapsed);

	void			EnterPosCameraLookAtActorMode(void);
	void			EnterPosActorLookAtActorMode(void);
	void			EnterPosActorLookAtTwoActorMode(void);
	void			EnterPosLookAtActorMode(void);
	void			EnterSelfMode(void);
	void			EnterSelfModeActorRef(void);

public:
	
	CNtlSobCameraSplineNode();
	~CNtlSobCameraSplineNode();

	virtual void	Update(RwReal fElapsed);

	virtual void	Enter(void);

	virtual RwBool	IsFinish();

	void			SetPosition(const RwV3d *pPos);
	void			SetDirection(const RwV3d *pDir);

	void			SetDataPosCameraLookAtActor(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime);
	void			SetDataPosActorLookAtActor(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime);
	void			SetDataPosActorLookAtTwoActor(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime);
	void			SetDataPosLookAtActor(RwV3d vPos, RwReal fAngleY, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime);
	void			SetDataSelf(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fLifeTime);
	void			SetDataSelfBeginActorRef(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, RwReal fLifeTime);
	void			SetDataSelfActorRef(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, RwReal fLifeTime);
};


class CNtlCameraDTController : public CNtlCameraController
{
private:

	typedef std::list<CNtlSobCameraNode*> ListNode;
	typedef std::list<CNtlSobCameraPauseNode*> ListPauseNode;
	
	RwReal				m_fCurrFov;
	RwBool				m_bAttributeRestore;
	
	ListNode			m_listNode;
	ListPauseNode		m_listLoopPause;

private:

	void		AddListNode(CNtlSobCameraNode *pNode, RwBool AttachNode);

public:

	CNtlCameraDTController();
	~CNtlCameraDTController();

	virtual void	Enter(void);
	
	virtual void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	virtual void	UpdateData(RwBool bUpdate = TRUE);

	virtual RwBool  IsCollisionCheck(void)	{ return FALSE; }           ///< 충돌체크를 하지 않는다.

	virtual RwBool	IsInterpCheck(void)		{ return FALSE; }

	virtual void	SetActiveActor(const CNtlSobActor *pActor);

public:

	void	SetAttributeRestore(RwBool bRestore);

	void	AddDTSoftActorDistNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
									RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt, RwBool AttachNode, CINEMATIC_HANDLE hCinematic);
	void	AddDTSoftActorDynamicLookAtDistNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
												RwReal fDistance, RwReal fLifeTime, RwBool AttachNode, CINEMATIC_HANDLE hCinematic);
	void	AddDTSoftCurrentRotateX(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwBool AttachNode, CINEMATIC_HANDLE hCinematic);
	void	AddDTSoftCurrentRotateY(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwBool AttachNode, CINEMATIC_HANDLE hCinematic);
	void	AddDTSoftCurrentRotateZ(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwBool AttachNode, CINEMATIC_HANDLE hCinematic);
	void	AddDTSoftCurrentRotateXYZ(RwReal fLastAngleX, RwReal fLastAngleY, RwReal fLastAngleZ, RwReal fInterTime, RwReal fLifeTime, RwBool AttachNode, CINEMATIC_HANDLE hCinematic);
	void	AddDTSoftCurrentFov(RwReal fLastFov, RwReal fInterTime, RwBool AttachNode, CINEMATIC_HANDLE hCinematic);
	void	AddDTSoftPauseNode(RwReal fLifeTime, RwBool bLoop, RwBool AttachNode, CINEMATIC_HANDLE hCinematic);
	void	AddDTSplineCurrentDynamicLookAtNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwBool bAttach, CINEMATIC_HANDLE hCinematic);
	void	AddDTSplineActorPositionDynamicLookAtNode(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwBool bAttach, CINEMATIC_HANDLE hCinematic);
	void	AddDTSplineActorPositionDynamicLookAtCenterNode(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwBool bAttach, CINEMATIC_HANDLE hCinematic);
	void	AddDTSplinePositionDynamicLookAtNode(RwV3d vPos, RwReal fAngleY, SERIAL_HANDLE hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwBool bAttach, CINEMATIC_HANDLE hCinematic);
	void	AddSplineSelfNode(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwBool bAttach, CINEMATIC_HANDLE hCinematic);
	void	AddSplineSelfNodeBeginActorRef(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, RwBool bAttach, CINEMATIC_HANDLE hCinematic);
	void	AddSplineSelfNodeActorRef(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, RwBool bAttach, CINEMATIC_HANDLE hCinematic);
	void	ClearLoopPauseNode(void);
};


/**
* \ingroup NtlSimulation
* \brief 천하제일 무도회에서 사용하는 옵저버용 고정 카메라 컨트롤러
*
* \date 2008-09-02
* \author agebreak
*/
class CNtlCameraObserver : public CNtlCameraDTController
{
public:
    CNtlCameraObserver();
    virtual ~CNtlCameraObserver();

    virtual void	Enter(void);
    virtual void	Exit(void);
    virtual void    Update(RwReal fElapsed);    
    virtual RwBool  IsCollisionCheck(void)	{ return FALSE; }           ///< 충돌체크를 하지 않는다.
    virtual RwBool	IsInterpCheck(void)		{ return FALSE; }

    void    SetIndex(RwInt32 nType, RwInt32 nIndex);                    ///< 카메라의 인덱스를 설정한다.

protected:
    RwBool  LoadObserverCameraData();                                   ///< XML 파일로부터 카메라 데이터를 로드한다.    

protected:
    RwInt32 m_nIndex;
    std::map<RwInt32, SObserverCameraData> m_mapPos;                    ///< 옵저버 카메라 위치 정보 벡터

};



class CNtlSobCameraExplosion : public CNtlCameraController
{

// Declarations
public:
	#define EXPLOSION_MIN_DISTANCE (25.f)
	#define EXPLOSION_MAX_DISTANCE (60.f)

	enum EXPLOSION_AXIS_TYPE
	{
		EXPLOSION_AXIS_TYPE_X,
		EXPLOSION_AXIS_TYPE_Y,
		EXPLOSION_AXIS_TYPE_Z,

		EXPLOSION_AXIS_TYPE_COUNT,
		EXPLOSION_AXIS_TYPE_INVALID		= 0xffffffff
	};

	class CExplosionAlgorithm
	{
	protected:
		EXPLOSION_AXIS_TYPE				eAxisType;

	public:
		CExplosionAlgorithm( EXPLOSION_AXIS_TYPE eAxisType ) : eAxisType( eAxisType ) { return; }
		virtual ~CExplosionAlgorithm( void ) { return; }

	public:
		virtual bool IsActive( void ) = 0;
		virtual void GetResult( RwMatrix& matResult ) = 0;
		virtual bool Update( RwReal fElapsed ) = 0;
	};

	class CExplosionAlgorithm_Sin : public CExplosionAlgorithm
	{
	public:
		// Parameters
		RwReal fLifeTime;
		RwReal fFrequancy;
		RwReal fPhase;
		RwReal fAmp;
		RwReal fDescFactor;

	protected:
		// Updated Informations
		bool bActive;
		RwReal fCurTime;
		RwReal fCurResult;

	public:
		CExplosionAlgorithm_Sin( EXPLOSION_AXIS_TYPE eAxisType )
			: CExplosionAlgorithm( eAxisType )
		{
			fLifeTime	= 0.f;
			fFrequancy	= 1.f;
			fPhase		= 0.f;
			fAmp		= 1.f;
			fDescFactor	= 1.f;

			bActive		= true;
			fCurTime	= 0.f;
			fCurResult	= 0.f;
		}

		virtual bool IsActive( void )
		{
			return bActive;
		}

		virtual void GetResult( RwMatrix& matResult )
		{
			if ( !IsActive() ) return;

			switch ( eAxisType )
			{
			case EXPLOSION_AXIS_TYPE_X:
				matResult.pos.x += fCurResult;
				break;
			case EXPLOSION_AXIS_TYPE_Y:
				matResult.pos.y += fCurResult;
				break;
			case EXPLOSION_AXIS_TYPE_Z:
				matResult.pos.z += fCurResult;
				break;
			}
		}

		virtual bool Update( RwReal fElapsed )
		{
			if ( !IsActive() ) return false;

			if ( fCurTime + fElapsed <= fLifeTime )
			{
				fCurTime += fElapsed;

				CalculateSinExplosionAlgorithm();
			}
			else
			{
				bActive = false;
			}

			return bActive;
		}

	protected:

		void CalculateSinExplosionAlgorithm( void )
		{
			RwReal fDescValue = ((fDescFactor - fCurTime < 0.f) ? 0.f : fDescFactor - fCurTime);
			fCurResult = sinf( 2.f * rwPI * fCurTime * fFrequancy + fPhase ) * fAmp * fDescValue/ fDescFactor;
		}
	};

	class CExplosionAlgorithm_Layer
	{
	public:
		typedef std::list< CExplosionAlgorithm* > listdef_AlgorithmList;

	protected:
		bool							bActive;
		listdef_AlgorithmList			defAlgorithmList;

	public:
		CExplosionAlgorithm_Layer( void ) { bActive = true; }
		~CExplosionAlgorithm_Layer( void ) { DeleteAllAlgorithms(); }

	public:
		void AddAlgorithm( CExplosionAlgorithm* pAlgorithm )
		{
			defAlgorithmList.push_back( pAlgorithm );
		}

		void DeleteAllAlgorithms( void )
		{
			listdef_AlgorithmList::iterator it = defAlgorithmList.begin();
			for ( ; it != defAlgorithmList.end(); ++it )
			{
				NTL_DELETE( *it );
			}
			defAlgorithmList.clear();
		}

		void GetResult( RwMatrix& matResult )
		{
			if ( !bActive ) return;

			listdef_AlgorithmList::iterator it = defAlgorithmList.begin();
			for ( ; it != defAlgorithmList.end(); ++it )
			{
				(*it)->GetResult( matResult );
			}
		}

		bool Update( RwReal fElapsed )
		{
			if ( !bActive ) return false;

			bool bActiveRet = true;

			listdef_AlgorithmList::iterator it = defAlgorithmList.begin();
			for ( ; it != defAlgorithmList.end(); ++it )
			{
				bActiveRet &= (*it)->Update( fElapsed );
			}

			bActive = bActiveRet;

			return bActive;
		}
	};

	typedef std::map< RwUInt32, CExplosionAlgorithm_Layer > mapdef_AlgorithmLayerList;

// Member variables
protected:
	CNtlSobActor*						m_pActor;
	mapdef_AlgorithmLayerList			m_defAlgorithmLayerList;

// Constructions and Destructions
public:
	CNtlSobCameraExplosion();
	~CNtlSobCameraExplosion();

// Operations
public:
	virtual void SetActiveActor(const CNtlSobActor *pActor);    

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwBool  IsCollisionCheck(void)	{ return FALSE; }           ///< 충돌체크를 하지 않는다.
	virtual RwBool	IsInterpCheck(void)		{ return FALSE; }

	void AddSinExplosionAlgorithm_X( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor );
	void AddSinExplosionAlgorithm_Y( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor );
	void AddSinExplosionAlgorithm_Z( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor );

// Implementations
protected:
	void UpdateCamera( RwMatrix& matResult );
};


class CNtlCameraTimeMachineArriveController : public CNtlCameraController
{

// Declarations
public:
	enum eTMARRIVE_STATE
	{
		eTMARRIVE_STATE_START,
		eTMARRIVE_STATE_RUN,
		eTMARRIVE_STATE_END,

		eTMARRIVE_STATE_INVALID = 0xffffffff
	};

// Member variables
protected:
	eTMARRIVE_STATE		m_eState;
	CNtlSobActor		*m_pTimemachineActor;

	RwReal				m_fOldFOV;

	RwReal				m_fFOV;
	RwV3d				m_v3Pos;
	RwV3d				m_v3Target;

// Constructions and Destructions
public:
	CNtlCameraTimeMachineArriveController( void );
	~CNtlCameraTimeMachineArriveController( void );

// Operations
public:
	virtual void		Enter( void );	
	virtual void		Exit( void );
	virtual void		Update( RwReal fElapsed );	
	virtual void		SetActiveActor( const CNtlSobActor *pActor );
	virtual RwBool		IsCollisionCheck( void )	{ return FALSE; }
	virtual RwBool		IsInterpCheck(void)		{ return FALSE; }

	void				Start();
	void				End();

// Implementations
protected:
	void				Run( void );
};


class CNtlCameraKnockDownMatrixController : public CNtlCameraController
{
// Declarations
public:

	enum ECameraKnockDownMatrixState
	{
		ECKDM_STATE_ZOOM,
		ECKDM_STATE_LOOK
	};

	enum ECameraKnockDownMatrixBlur
	{
		ECKDM_BLUR_NONE,
		ECKDM_BLUR_ENTER,
		ECKDM_BLUR_LOOP,
		ECKDM_BLUR_END
	};


// Member variables
protected:

	ECameraKnockDownMatrixState	m_eState;
	RwReal						m_fZoomSpeed;
	SERIAL_HANDLE				m_hTargetSerialId;
	RwReal						m_fTargetHeight;
	RwReal						m_fTargetRound;
	RwReal						m_fCurrTime;
	RwReal						m_fLifeTime;

	RwUInt8						m_byBlurState;
	RwReal						m_fBlurTime;

private:

	void				UpdateZoom(RwReal fElapsed);
	void				UpdateLook(RwReal fElasped);

// Constructions and Destructions
public:
	CNtlCameraKnockDownMatrixController( void );
	~CNtlCameraKnockDownMatrixController( void );

// Operations
public:
	
	virtual void		Enter( void );	
	virtual void		Exit( void );
	virtual void		Update( RwReal fElapsed );	
	virtual void		SetActiveActor(const CNtlSobActor *pActor);
	virtual RwBool		IsCollisionCheck(void)	{ return FALSE; }           ///< 충돌체크를 하지 않는다.
	virtual RwBool		IsInterpCheck(void)		{ return FALSE; }

	void				SetData(RwReal fLifeTime);
};

class CNtlCameraBusController : public CNtlCameraNormalController
{
public:
	enum ECameraBusState
	{
		EBS_STATE_START,
		EBS_STATE_USER_CONTROLL,
		EBS_STATE_TRACE_BUSBACK,

		EBS_STATE_NONE,
	};

public:
	struct sBusCameraStart
	{
		RwReal			fOriginalMinDist;
		RwReal			fOriginalMaxDist;
		RwReal			fOriginalCurDist;
		RwReal			fOriginalAngleX;
		RwReal			fOriginalAngleY;
		RwReal			fOriginalFov;

		RwReal			fInterpolationElaped;
		RwReal			fDestAngleY;
	};

	struct sUserCamera
	{
		RwReal			fChangeDelayTime;

		RwBool			bChange;
	};

	struct sBusTrace
	{
		RwReal			fInterpolationTime;
		RwReal			fInterpolationElaped;
		RwReal			fOriginalAngleX;
		RwReal			fOriginalAngleY;
		RwReal			fDestAngleY;
		RwV3d			v3BusDir;
		RwBool			bTracing;
	};

	CNtlCameraBusController();
	virtual ~CNtlCameraBusController();

	virtual void		Enter( void );	
	virtual void		Exit( void );
	virtual void		Update( RwReal fElapsed );
	virtual RwBool		IsCollisionCheck(void)	{ return TRUE; }           ///< 충돌체크를 하지 않는다.
	virtual RwBool		IsInterpCheck(void)		{ return FALSE; }

	virtual void		UserCameraControlEnable(RwBool bEnable);

protected:
	void				ChangeState( ECameraBusState eState);

	void				InitTraceCamera();

	void				Update_Start( RwReal fElapsed );
	void				Update_UserCamera( RwReal fElapsed );
	void				Update_TraceBusBack( RwReal fElapsed );

	RwReal				GetDeltaYaw(RwReal fOriginalAngleY, RwBool* pbLeftTurn = NULL);

protected:
	ECameraBusState				m_eState;

	sBusCameraStart				m_BusStartCamera;
	sUserCamera					m_UserCamera;
	sBusTrace					m_BusTraceInfo;
};

class CNtlCameraTutorialController : public CNtlCameraController
{
// Declarations
public:
	enum
	{
		EYAW_FINISH		= 0x01,
		EPITCH_FINISH	= 0x02
	};

// Member variables
protected:
	CNtlSobActor*		m_pActor;

	RwUInt32			m_uiType;
	RwUInt32			m_uiTblIdx;

	RwV3d				m_vTargetPos;

	RwUInt8				m_byFinishFlag;

	RwReal				m_fOldYaw, m_fCurYaw, m_fOffsetYaw;
	RwReal				m_fOldPitch, m_fCurPitch, m_fOffsetPitch;

// Constructions and Destructions
public:
	CNtlCameraTutorialController( void );
	~CNtlCameraTutorialController( void );

// Operations
public:
	virtual void		Enter( void );	
	virtual void		Exit( void );
	virtual void		Update( RwReal fElapsed );
	virtual void		SetActiveActor( const CNtlSobActor *pActor );
	virtual RwBool		IsCollisionCheck(void)	{ return FALSE; }           ///< 충돌체크를 하지 않는다.
	virtual RwBool		IsInterpCheck(void)		{ return FALSE; }

	void				SetData( RwUInt32 uiType, RwUInt32 uiTblIdx, RwV3d& vTargetPos );

// Implementations
protected:
	void				UpdateTargetPos( void );

	void				UpdateYaw( RwReal fElapsed );
	void				UpdatePitch( RwReal fElapsed );

	RwReal				GetActorOffset( void );
};

#endif
