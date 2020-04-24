/*****************************************************************************
 *
 * File			: NtlCameraManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 8. 08	
 * Abstract		: simulation layer camera manager
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_CAMERA_MANAGER_H__
#define __NTL_CAMERA_MANAGER_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"

#define NTL_CAMERA_DEFAULT_FOV 40.0
#define NTL_CAMERA_FOV_MAX_BONUS 30.0

class CNtlCamera;
class CNtlCameraController;
class CNtlSobActor;


class CNtlGameCameraManager : public RWS::CEventHandler
{
public:

	enum EActiveCamera
	{
		ACTIVE_BASE_THIRD,
		ACTIVE_BASE_FREE
	};

private:
	class CCAMInterpolation
	{
	public:
		enum eCAM_INTERP_TYPE
		{
			eCAM_INTERP_TYPE_RUN_FRONT,
			eCAM_INTERP_TYPE_RUN_BACK,
			eCAM_INTERP_TYPE_IDLE,

			eCAM_INTERP_TYPE_MAX
		};

		typedef void (CNtlGameCameraManager::CCAMInterpolation::* pINTERP_FUNC)( RwReal, RwV3d&, RwV3d& );

	private:
		RwReal				m_fRadius;

		pINTERP_FUNC		m_parStateFunc[eCAM_INTERP_TYPE_MAX][eCAM_INTERP_TYPE_MAX];

		eCAM_INTERP_TYPE	m_eCAMInterpType;

		RwReal				m_fOriginOldCamDist;
		RwReal				m_fOriginCurCamDist;

		RwReal				m_fSourceCamDist;
		RwReal				m_fTargetCamDist;

	public:
		CCAMInterpolation( void );

	public:
		void SetRadius( RwReal fRadius ) { m_fRadius = fRadius; }

		void ChangeInterpState( eCAM_INTERP_TYPE eInterpType, RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt, RwReal fOriginCamDist );

		void Update( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt );

	private:
		void OnMoveIdleToFront( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt );
		void OnMoveFrontToFront( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt );
		void OnMoveBack( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt );
		void OnMoveIdle( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt );

		RwReal CalCamDist( const RwV3d& vCamPos, const RwV3d& vLookAt );
		void Interpolation( RwReal fElapsed, RwV3d& vCamPos, RwV3d& vLookAt, RwReal fSpeed );
	};


private:

	static CNtlGameCameraManager *m_pInstance;

	// ntl camera
	CNtlCamera *m_pCamera;

	// camera control attribute
	RwReal m_fMinDist;			// minimun distance.
	RwReal m_fMaxDist;			// maximun distance.	
	RwReal m_fDist;				// camera 현재 거리.

	RwReal m_fMinPitch;			// camera min pitch.
	RwReal m_fMaxPitch;			// camera max pitch.	
	RwReal m_fPitch;			// camera current pitch.	
	RwReal m_fYaw;				// camera current yaw.	
	RwReal m_fRoll;				// camera current roll;
	
	RwV3d m_vCamPos;			// camera position
	RwV3d m_vLookAt;			// camera look at
	RwV3d m_vUp;				// camera up	
	RwV3d m_vOffset;			// camera position offset
	RwV3d m_vLookAtOffset;		// camera position offset
	
	// camera controller
	RwBool m_bActorAlpha;
	CNtlSobActor *m_pActor;

	EActiveCamera			m_eActiveCamera;

	CNtlCameraController	*m_pThirdController;
	CNtlCameraController	*m_pFreeController;
	CNtlCameraController	*m_pBaseController;

	typedef std::list<CNtlCameraController*> ListCameraController;
	ListCameraController m_listController;

	RwBool m_bDTNodeAttach;
	RwBool m_bDTAttrRestore;

	RwBool m_bCollisionCheck;
	RwBool m_bInterpCheck;

	RwBool m_bFinishLock;
	RwBool m_bUpdateData;
    RwReal m_fCollisionObjCheckTime;
	RwBool m_bEnableCollisionObj;

	CCAMInterpolation		m_clCAMInterp;

	float	m_fHeightRatioBonus;

private:

	void AddController(CNtlCameraController *pController);
	RwBool RemoveController(RwUInt8 byType);
	
	void CheckNormalControlPause(void);
	RwBool CheckRotatePossible(void);
	RwBool CheckZoomPossible(void);

	RwBool WorldHeight(RwV3d& vCamPos, RwV3d& vLookAt);
	RwBool UpdateCameraForWorld(RwV3d& vCamPos, RwV3d& vLookAt, RwReal fRadius);

	void CollisionActor(RwV3d& vCamPos, RwV3d& vLookAt);
    void CollisionObj();
//    void CullingEffect();                                       ///< 이펙트 컬링 유무를 테스트한다.
	
public:

	CNtlGameCameraManager();
	~CNtlGameCameraManager();

	static CNtlGameCameraManager* GetInstance(void);
	
	RwBool					Create(void);
	void					Destroy(void);
	void					Update(RwReal fElapsed);
	virtual void			HandleEvents(RWS::CMsg &pMsg); 
	void					SetActiveCamera(const CNtlCamera *pCamera);
	void					SetActiveActor(const CNtlSobActor *pSobActor);
	CNtlCameraController*	FindController(RwUInt8 byType);

    // Free Camera를 위한 인터페이스
    RwBool                  IsActiveFreeCamera();
    void                    FreeCameraMove(RwUInt32 moveType, RwReal fSpeed = 0.0f);    
    void                    FreeCameraMoveStop();
    RwReal                  FreeCameraGetMoveSpeed();
    void                    FreeCameraSetMoveSpeed(RwReal fSpeed);

public:

	RwV3d&			GetYAxis( void );

	void			SetMinDist(RwReal fMinDist);
	RwReal			GetMinDist(void) const;

	void			SetMaxDist(RwReal fMaxDist);
	RwReal			GetMaxDist(void) const;

	void			SetDist(RwReal fDist);
	RwReal			GetDist(void) const;

	void			SetMinPitch(RwReal fMinPitch);
	RwReal			GetMinPitch(void) const;

	void			SetMaxPitch(RwReal fMaxPitch);
	RwReal			GetMaxPitch(void) const;

	void			SetPitch(RwReal fPitch);
	RwReal			GetPitch(void) const;

	void			SetYaw(RwReal fYaw);
	RwReal			GetYaw(void) const;

	void			SetRoll(RwReal fRoll);
	RwReal			GetRoll(void) const;

	void			AddPitch(RwReal fDeltaPitch);
	void			AddYaw(RwReal fDeltaPitch);

	void			ZoomInOut(RwReal fDelta);

	void			MoveSmoothing(RwBool bEnable);

	void			EnableUpdateData(RwBool bUpdateData);
    void            EnableCollisionObj(RwBool bEnable) {m_bEnableCollisionObj = bEnable;}   ///< Set camera collision alpha handling for objects other than avatars

	void			UserCameraControlEnable(RwBool bEnable);

public:

	void			Reset(void);
	void			ResetAttribute(void);
	void			ResetRotate(void);

	void			ResetCamera(void);

	void			SetFov(RwReal fFov);
	RwReal			GetFov(void) const;

	void			SetHeightRatioBonus(float fBonus);
	float			GetHeightRatioBonus();

	void			SetLookAt(const RwV3d *pLookAt);
    RwV3d           GetLookAt() {return m_vLookAt;}

	void			SetCameraPos(const RwV3d *pPos);
	const RwV3d*	GetCameraPos(void);

	const RwV3d*	GetCameraDir(void);

	void			AddCameraOffset(const RwV3d *pPos);
	const RwV3d*	GetCameraOffset(void);

	void			AddCameraLookAtOffset(const RwV3d *pPos);
	const RwV3d*	GetCameraLookAtOffset(void);

	void			SetCaptureScreenShot();

	CNtlCamera*		GetCamera(void) const;

public:

	void			SetCollisionCheck(RwBool bCollisionCheck);

	RwBool			IsCollisionCheck(void);

	void			SetInterpCheck(RwBool bInterpCheck);

	void			SetDTNodeAttach(RwBool bAttach);

	void			SetDTAttributeRestore(RwBool bRestore);

	void			AddDTSoftActorDistNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
											RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);
	void			AddDTSoftActorDynamicLookAtDistNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
														RwReal fDistance, RwReal fLifeTime, CINEMATIC_HANDLE = INVALID_CINEMATIC_ID);

	void			AddDTSoftCurrentRotateX(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSoftCurrentRotateY(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSoftCurrentRotateZ(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSoftCurrentFov(RwReal fLastFov, RwReal fInterTime, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSoftPauseNode(RwReal fLifeTime, RwBool bLoop, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSplineSelfNode(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSplineSelfNodeBeginActor(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSplineSelfNodeActor(SERIAL_HANDLE hSerialId, const RwChar *pPropKey, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);
	
	void			AddDTSplineCurrentDynamicLookAtNode(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSplineActorPositionDynamicLookAtNode(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE  hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSplineActorPositionDyncmicLookAtCenterNode(SERIAL_HANDLE hPosSerialId, SERIAL_HANDLE  hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			AddDTSplinePositionDynamicLookAtNode(RwV3d vStartPos, RwReal fAngleY, SERIAL_HANDLE  hLookAtSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, CINEMATIC_HANDLE hCinematic = INVALID_CINEMATIC_ID);

	void			ClearLoopPauseNode(void);

	void			FinishLock(void);

	void			FinishUnLock(void);

	RwBool			IsFinishLock(void);

// Explosion camera 관련 함수들
public:
	
	void			AddSinExplosionAlgorithm_XAxis( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor );

	void			AddSinExplosionAlgorithm_YAxis( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor );

	void			AddSinExplosionAlgorithm_ZAxis( RwUInt32 uiExcuteID, RwReal fLifeTime, RwReal fFrequancy, RwReal fPhase, RwReal fAmp, RwReal fDescFactor );

};

static CNtlGameCameraManager* GetNtlGameCameraManager(void)
{
	return CNtlGameCameraManager::GetInstance();
}

inline CNtlCamera* CNtlGameCameraManager::GetCamera(void) const
{
	return m_pCamera;
}

inline RwV3d& CNtlGameCameraManager::GetYAxis( void )
{
	return m_vUp;
}

inline void CNtlGameCameraManager::SetMinDist(RwReal fMinDist)
{
	m_fMinDist = fMinDist;    
}

inline RwReal CNtlGameCameraManager::GetMinDist(void) const
{
	return m_fMinDist;
}

inline void CNtlGameCameraManager::SetMaxDist(RwReal fMaxDist)
{
	m_fMaxDist = fMaxDist;
}

inline RwReal CNtlGameCameraManager::GetMaxDist(void) const
{
	return m_fMaxDist;
}

inline RwReal CNtlGameCameraManager::GetDist(void) const
{
	return m_fDist;
}

inline void CNtlGameCameraManager::SetMinPitch(RwReal fMinPitch)
{
	m_fMinPitch = fMinPitch;
}

inline RwReal CNtlGameCameraManager::GetMinPitch(void) const
{
	return m_fMinPitch;
}

inline void CNtlGameCameraManager::SetMaxPitch(RwReal fMaxPitch)
{
	m_fMaxPitch = fMaxPitch;
}

inline RwReal CNtlGameCameraManager::GetMaxPitch(void) const
{
	return m_fMaxPitch;
}

inline RwReal CNtlGameCameraManager::GetPitch(void) const
{
	return m_fPitch;
}

inline RwReal CNtlGameCameraManager::GetYaw(void) const
{
	return m_fYaw;
}

inline RwReal CNtlGameCameraManager::GetRoll(void) const
{
	return m_fRoll;
}

inline void CNtlGameCameraManager::SetLookAt(const RwV3d *pLookAt)
{
	RwV3dAssignMacro(&m_vLookAt, pLookAt);
}

inline void CNtlGameCameraManager::AddCameraOffset(const RwV3d *pOffset)
{
	RwV3dAdd(&m_vOffset, &m_vOffset, pOffset);
}

inline const RwV3d* CNtlGameCameraManager::GetCameraOffset(void)
{
	return &m_vOffset;
}

inline void CNtlGameCameraManager::AddCameraLookAtOffset(const RwV3d *pOffset)
{
	RwV3dAdd(&m_vLookAtOffset, &m_vLookAtOffset, pOffset);
}

inline const RwV3d* CNtlGameCameraManager::GetCameraLookAtOffset(void)
{
	return &m_vLookAtOffset;
}

inline RwBool CNtlGameCameraManager::IsCollisionCheck(void)
{
	return m_bCollisionCheck;
}

#endif
