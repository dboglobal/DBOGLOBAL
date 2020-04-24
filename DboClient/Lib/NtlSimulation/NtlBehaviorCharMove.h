/*****************************************************************************
 *
 * File			: NtlBehaviorCharMove.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: character move behavior class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIOR_CHAR_MOVEH__
#define __NTL_BEHAVIOR_CHAR_MOVEH__


#include "NtlBehaviorBase.h"
#include "NtlBehaviorData.h"
#include "NtlSLLogicDef.h"

class CNtlDTNodeBase;
class CNtlDTProp;
class CNtlInstanceEffect;
struct SNtlEventSobMoveSync;

// move
#define SLOW_MOVE_RANGE						1.0f
#define SLOW_MOVE_SPEED						2.0f
#define SLOW_MOVE_TIME						0.5f
#define FOLLOW_ERROR_RANGE					0.90f
#define MOVE_POSITION_CHANGE_CHECK_TIME		0.5f
#define MOVE_POSITION_CHANGE_LIMIT_RANGE	0.2f
#define COLLISION_MOVE_IMPOSSIBLE_COUNT		3

// dash 
#define DASH_LANDING_TIME					0.166f  // 0.8초에 DASH_LANDING_DIST 만큼 간다.

// jump
#define JUMP_GRAVITY_SPEED					11.0f
#define JUMP_UP_VELOCITY					6.0f

// falling
#define FALLING_CHECK_LEN					1.0f
#define FALLING_WALK_LEN					1.0f

// move sync send
#define MOVE_SYNC_SEND_TIME					0.2f
#define MOVE_SYNC_SPEED                     1.0f
#define MOVE_SYNC_GAP                       0.3f


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharMove : public CNtlBehaviorBase
{
public:
    CNtlBehaviorCharMove();
	virtual ~CNtlBehaviorCharMove();

	RwBool	IsEmptyPathNode(void);
	void	AddPathNode(RwV3d vPos);
	RwBool	PopPathNode(RwV3d& vPos);
	void	ClearPathNode(void);
    RwBool  IsMoveSyncState();                                  ///< MoveSync을 맞춰야하는 상태인지를 반환한다 (다른 캐릭터용)
    RwBool  IsMoveSyncSend();                                   ///< MoveSync 패킷을 보내야하는 상태인지를 반환한다 (아바타용)

    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);    

protected:    
    virtual void	 UpdateAnimSpeed(void);    
    virtual RwReal   GetMoveSpeed(RwUInt8 byMoveFlag);          ///< Move Flag에 맞는 속도를 반환한다.
    virtual RwBool   IsMoveSyncEmpty();                         ///< 현재 MoveSyncQ에 Sync할 데이터가 있는지를 반환한다.

protected:    

    typedef std::list<RwV3d>    ListPathNode;
    ListPathNode                m_listPathNode;
    RwReal				        m_fAnimSpeed;
    RwUInt8                     m_byPrevMoveFlag;        
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharSplineMove : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharSplineMove, NTL_SMALL_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharSplineMove)

private:

	CNtlDTNodeBase	*m_pDTNode;
	CNtlDTProp		*m_pDTProp;

public:

	CNtlBehaviorCharSplineMove();
	~CNtlBehaviorCharSplineMove();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBehaviorCharFalling : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharFalling, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharFalling)

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
	BYTE				m_byMoveDirection;
	RwV3d				m_vFallingPos;
	RwV3d				m_vFallingDir;
	RwReal				m_fFallingWalkSpeed;
	RwReal				m_fFallingSpeed;
	RwReal				m_fFallingHeight;
	SWorldHeightStuff	m_sHStuff;					/** world height stuff */
	
private:

	void		SetAnimFalling(void);
	void		SetAnimMoveLanding(void);
	void		SetAnimWaterLanding(void);

	RwBool		LandingCheck(RwV3d& vPos);

	void		UpdatePositionMove(RwV3d& vPos, RwReal fElapsed);

	void		UpdateWalking(RwV3d& vPos, RwReal fElapsed);
	void		UpdateFalling(RwV3d& vPos, RwReal fElapsed);
	void		UpdateMoveLanding(RwV3d& vPos, RwReal fElapsed);
	void		UPdateWaterLanding(RwV3d& vPos, RwReal fElapsed);
		
	void		ChangeFallingState(RwUInt8 byFallingState);

public:

	CNtlBehaviorCharFalling();
	~CNtlBehaviorCharFalling();

	void		Enter(void);
	void		Exit(void);
	void		Update(RwReal fElapsed);
	void		UpdateData(void);
	RwUInt32	HandleEvents(RWS::CMsg &pMsg);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBehaviorBusMove : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL( CNtlBehaviorBusMove, NTL_DEFAULT_MEMORY_POOL )
	DECLEAR_BEHAVIOR_FACTROY( CNtlBehaviorBusMove )

// Declarations
protected:
	struct SInputData
	{
		RwUInt8		byFormFlag;
		D3DXVECTOR3 vCurPos;
		D3DXVECTOR3 vDestPos;
		D3DXVECTOR3 vSecondPos;
	};

	class RNS
	{
	public:
		#define BUILD_MIN_NODE_SIZE		(3)

		enum ERNS_MOVE_TYPE
		{
			ERNS_MOVE_TYPE_PRE_DEST,
			ERNS_MOVE_TYPE_DEST,
			ERNS_MOVE_TYPE_SECOND_DEST,

			ERNS_MOVE_TYPE_INVALID
		};

		struct SSplineData
		{
			D3DXVECTOR3 vPos;
			D3DXVECTOR3 vVel;
			RwReal fDist;
		};
		typedef std::vector<SSplineData> vecdef_SplineData_List;

	protected:
		D3DXMATRIX				m_matHermite;
		vecdef_SplineData_List	m_defNodeList;
		RwReal					m_fMaxDist;
		RwBool					m_bLastNodeInfoSetting;

	public:
		void					Init( void );

		RwBool					IsLastNodeInfoSetting( void );
		void					SetLastNodeInfoSetting( RwBool bLastNodeInfoSetting );

		int						GetNodeSize( void );

		RwReal					GetMaxDistance( void );

		void					GetMoveDistRatio( RwReal fTime, ERNS_MOVE_TYPE& eMoveType, RwReal& fMoveDistRatio );
		D3DXVECTOR3				GetPosition( RwReal fTime );
		void					AddNodePos( const D3DXVECTOR3& vPos );

		void					BuildSpline( void );

	protected:
		D3DXVECTOR3				GetPositionOnCubic( const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vStartVel, const D3DXVECTOR3& vEndPos, const D3DXVECTOR3& vEndVel, RwReal fTime );
		D3DXVECTOR3				GetStartVelocity( int nIndex );
		D3DXVECTOR3				GetEndVelocity( int nIndex);
	};

	#define	ROT_PER_SEC_MIN	(0.f)
	#define ROT_PER_SEC_MAX	(1.f)

// Member variables
protected:
	RwBool						m_bWalkMode;
	RwReal						m_fDefMoveSpeed;

	SInputData					m_sInputData;
	RNS							m_RNSData;
	RwReal						m_fRNSMovingTime;
	D3DXVECTOR3					m_vRNSCurPos;

	RwBool						m_bRot;
	RwReal						m_fRotSpeed;

// Constructions and Destructions
public:
	CNtlBehaviorBusMove( void );
	~CNtlBehaviorBusMove( void );

// Operations
public:
	virtual void Enter(void);
	virtual void Update( RwReal fElapsed );
	virtual RwUInt32 HandleEvents( RWS::CMsg &pMsg );

// Implementations
protected:
	void BuildData( void );
	void UpdateActorDirPos( RwReal fElapsed, RwV3d& vResultDir, RwV3d& vResultPos );
	RwReal GetRotSpeed( RwReal fElapsed );

	void SetAnim( void );
};


#endif
