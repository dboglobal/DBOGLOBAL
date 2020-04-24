/*****************************************************************************
 *
 * File			: NtlBehaviorVehicleMove.h
 * Author		: JeongHo, Rho
 * Copyright	: (주)NTL
 * Date			: 2008. 11. 14	
 * Abstract		: vehicle move behavior class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIOR_VEHICLE_MOVEH__
#define __NTL_BEHAVIOR_VEHICLE_MOVEH__


#include "NtlSLLogic.h"
#include "NtlBehaviorBase.h"
#include "NtlBehaviorData.h"
#include "NtlVehicleHelper.h"
#include "NtlPLEntity.h"


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBehaviorVehicleMoveBase : public CNtlBehaviorBase, public CNtlVehicleHelper
{
public:
	enum eVEHICLE_MOVE_TYPE
	{
		eVEHICLE_MOVE_TYPE_SPEED_UP,
		eVEHICLE_MOVE_TYPE_UNIFORM_VELOCITY,
		eVEHICLE_MOVE_TYPE_SPEED_DOWN,
		eVEHICLE_MOVE_TYPE_INVALID
	};

	enum eVEHICLE_TRUN_TYPE
	{
		eVEHICLE_TRUN_TYPE_LEFT,
		eVEHICLE_TRUN_TYPE_RIGHT,
		eVEHICLE_TRUN_TYPE_INVALID
	};

	enum eVEHICLE_GROUND_TYPE
	{
		eVEHICLE_GROUND_TYPE_GROUND,
		eVEHICLE_GROUND_TYPE_WATER,
		eVEHICLE_GROUND_TYPE_INVALID
	};

	#define STOP_DISTANCE		(0.5f)

protected:
	// Stuff
	SMoveStuff					m_MoveStuff;

	// Time
	RwReal						m_fVelSumTime;

	// Accelerator
	RwReal						m_fInitAccelerator;
	RwReal						m_fDefAccelerator;

	// Velocity
	RwReal						m_fDefVelocity;
	RwReal						m_fPreVelocity;
	RwReal						m_fCurVelocity;

	// Rot, Height
	RwReal						m_fRotVelocity;
	RwReal						m_fYDirInterpRatio;

	// Move type
	eVEHICLE_MOVE_TYPE			m_eMoveType;
	eVEHICLE_TRUN_TYPE			m_eTurnType;

	// Move stop
	RwBool						m_bStop;

	// possible 변화 check
	RwReal						m_fPosChangeTime;
	RwUInt8						m_byPosChangeCnt;
	RwV3d						m_vChangeOldPos;

	// collision
	RwUInt8						m_byCollMoveImpossCnt;
	SWorldHeightStuff			m_sHStuff;

	// server 동기 위치 보정.
	RwBool						m_bServerSyncPos;
	RwV3d						m_vServerSyncDir;
	RwReal						m_fServerSyncDist;
	RwReal						m_fCurrServerSyncDist;
	RwReal						m_fDeltaServerSyncDist;

	// move sync
	RwBool						m_bSyncSendCheck;
	RwReal						m_fSyncSendTime;

	// moving effect
	RwUInt8						m_byGroundType;
	CNtlPLEntity*				m_pPLRunSmoke;
	CNtlPLEntity*				m_pPLRunSpray;

public:
	CNtlBehaviorVehicleMoveBase( void );
	virtual ~CNtlBehaviorVehicleMoveBase( void );

public:
	virtual void				Enter( void );
	virtual void				Exit( void );
	virtual void				Update( RwReal fElapsed );
	virtual RwUInt32			HandleEvents( RWS::CMsg& pMsg );
	virtual void				UpdateData( void );

protected:
	void						SetStop( void );

	virtual void				UpdateVelocity( RwReal fElapsed );
	virtual void				UpdateActorDir( RwReal fElapsed, RwV3d& vCurDir, RwV3d& vNextDir, RwV3d& vResultDir );
	virtual void				UpdateActorSlop( RwReal fElapsed, RwV3d& vNextDir, RwV3d& vNextPos, RwReal fDirY, RwV3d& vResultDir, RwV3d& vResultPos );
	virtual void				UpdatePosition( const RwV3d& vCurPos, const RwV3d& vDestPos, RwV3d& vResultPos );
	virtual void				UpdateAnim( void );

	virtual void				CheckSwimming( void );
	virtual void				UpdateSwimEffect( void );

	virtual void				SetServerSyncPosition( RwV3d vSyncPos );
	virtual void				UpdateServerSyncPosition( RwReal fElapsed );
	virtual void				SetSyncSendCheck( RwBool bSyncSendCheck );
	virtual void				UpdateSendSyncCheck( RwReal fElapsed );
	virtual RwBool				LimitPositionChangeCheck( RwReal fElapsed, RwV3d& vDestPos );

	virtual void				CreateRunSmokeEffect( void );
	virtual void				DestroyRunSmokeEffect( void );
	virtual void				CreateRunSprayEffect( void );
	virtual void				DestroyRunSprayEffect( void );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CDboHermiteMoveSpline;


class CNtlBehaviorVehicleMouseMove : public CNtlBehaviorVehicleMoveBase
{
	DECLEAR_MEMORY_POOL( CNtlBehaviorVehicleMouseMove, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_BEHAVIOR_FACTROY( CNtlBehaviorVehicleMouseMove )

protected:
	RwReal						m_fMoveDist;
	CDboHermiteMoveSpline*		m_pHermiteMoveSpline;

public:
    CNtlBehaviorVehicleMouseMove( void );
    ~CNtlBehaviorVehicleMouseMove( void );

    virtual void				Enter( void );
    virtual void				Exit( void );
    virtual void				Update( RwReal fElapsed );
    virtual RwUInt32			HandleEvents( RWS::CMsg& pMsg );
    virtual void				UpdateData( void );

protected:
	virtual void				InitTraceOfMoving( void );
	void						UpdateAnimFlag( RwV3d& vCurDir, RwV3d& vNextDir );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBehaviorVehicleKeyboardMove : public CNtlBehaviorVehicleMoveBase
{
	DECLEAR_MEMORY_POOL( CNtlBehaviorVehicleKeyboardMove, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_BEHAVIOR_FACTROY( CNtlBehaviorVehicleKeyboardMove )

public:
	CNtlBehaviorVehicleKeyboardMove( void );
	~CNtlBehaviorVehicleKeyboardMove( void );

public:
	virtual void				Enter( void );
	virtual void				Exit( void );
	virtual void				Update( RwReal fElapsed );
	virtual RwUInt32			HandleEvents( RWS::CMsg& pMsg );
	virtual void				UpdateData( void );
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBehaviorVehicleFollowMove : public CNtlBehaviorVehicleMouseMove
{
	DECLEAR_MEMORY_POOL( CNtlBehaviorVehicleFollowMove, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_BEHAVIOR_FACTROY( CNtlBehaviorVehicleFollowMove )

public:
	enum eTARGET_UPDATE_TYPE
	{
		eTARGET_UPDATE_TYPE_STOP,
		eTARGET_UPDATE_TYPE_MOVE
	};

	#define SLOW_DOWN_DIST		(10.f)

protected:
	RwV3d						m_vTargetPos;

public:
	CNtlBehaviorVehicleFollowMove( void );
	~CNtlBehaviorVehicleFollowMove( void );

public:
	virtual void				Enter( void );
	virtual void				Exit( void );
	virtual void				Update( RwReal fElapsed );
	virtual RwUInt32			HandleEvents( RWS::CMsg& pMsg );
	virtual void				UpdateData( void );

protected:
	virtual eTARGET_UPDATE_TYPE	UpdateTargetPos( RwBool& bSlowDown );
	virtual void				InitTraceOfMoving( void );
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBehaviorVehicleAdjustMove : public CNtlBehaviorVehicleMoveBase
{
	DECLEAR_MEMORY_POOL( CNtlBehaviorVehicleAdjustMove, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_BEHAVIOR_FACTROY( CNtlBehaviorVehicleAdjustMove )


protected:
	SMoveStuff					m_MoveStuff;

	RwV3d						m_vSourcePos;
	RwV3d						m_vMoveDir;
	RwReal						m_fMoveSpeed;
	RwReal						m_fMoveFinishTime;
	RwReal						m_fMoveCurTime;

	RwBool						m_bStop;

public:
	CNtlBehaviorVehicleAdjustMove( void );
	~CNtlBehaviorVehicleAdjustMove( void );

public:
	virtual void				Enter( void );
	virtual void				Exit( void );
	virtual void				Update( RwReal fElapsed );
	virtual RwUInt32			HandleEvents( RWS::CMsg& pMsg );
	virtual void				UpdateData( void );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorVehicleFalling : public CNtlBehaviorBase, public CNtlVehicleHelper
{
	DECLEAR_MEMORY_POOL( CNtlBehaviorVehicleFalling, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_BEHAVIOR_FACTROY( CNtlBehaviorVehicleFalling )

private:

	enum EFallingState
	{
		FALIINGSTATE_WALK,
		FALIINGSTATE_FALLING,
		FALIINGSTATE_MOVE_LADNDING,
		FALIINGSTATE_WATER_LADNDING,
		FALIINGSTATE_END
	};

	RwUInt8				m_byFallingState;
	RwV3d				m_vFallingPos;
	RwV3d				m_vFallingDir;
	RwReal				m_fFallingWalkSpeed;
	RwReal				m_fFallingSpeed;
	RwReal				m_fFallingHeight;
	SWorldHeightStuff	m_sHStuff;

private:

	void				SetAnimFalling( void );
	void				SetAnimMoveLanding( void );
	void				SetAnimWaterLanding( void );

	RwBool				LandingCheck( RwV3d& vPos );

	void				UpdatePositionMove( RwV3d& vPos, RwReal fElapsed );

	void				UpdateWalking( RwV3d& vPos, RwReal fElapsed );
	void				UpdateFalling( RwV3d& vPos, RwReal fElapsed );
	void				UpdateMoveLanding( RwV3d& vPos, RwReal fElapsed );
	void				UPdateWaterLanding( RwV3d& vPos, RwReal fElapsed );

	void				ChangeFallingState( RwUInt8 byFallingState );

	RwReal				GetHeightOffset( void );

public:

	CNtlBehaviorVehicleFalling( void );
	~CNtlBehaviorVehicleFalling( void );

	void				Enter( void );
	void				Exit( void );
	void				Update( RwReal fElapsed );
	void				UpdateData( void );
	RwUInt32			HandleEvents( RWS::CMsg& pMsg );
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#endif
