/*****************************************************************************
*
* File			: DboMovingAlgorithm.h
* Author		: JeongHo, Rho
* Copyright		: (аж) NTL
* Date			: 2009. 1. 21
* Abstract		: Moving algorithm
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __DBO_MOVING_ALGORITHM_H__
#define __DBO_MOVING_ALGORITHM_H__


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CDboDeadReckonig
{
public:
	typedef std::vector< RwV3d > vecdef_POS_List;

protected:
	vecdef_POS_List				m_defPosList;

public:
	CDboDeadReckonig( void ) { return; }
	virtual ~CDboDeadReckonig( void ) { return; }

public:
	RwBool						Refresh( RwV3d& vClientDir,
										 RwV3d& vClientPos,
										 RwReal fClientSpeed,
										 RwV3d& vServerDir,
										 RwV3d& vServerPos,
										 RwReal fServerSpeed,
										 RwReal fAccelator,
										 RwReal fLatencyTime );

	RwBool						Refresh( RwV3d& vPos0,
										 RwV3d& vPos1,
										 RwV3d& vPos2,
										 RwV3d& vPos3 );

	RwReal						GetMaxDistance( void );

	void						GetDirAndPos( RwReal fTimeRatio, RwV3d& vDir, RwV3d& vPos );

protected:
	RwReal						GetDirX( RwReal fTimeRatio );
	RwReal						GetDirZ( RwReal fTimeRatio );

	RwReal						GetPosX( RwReal fTimeRatio );
	RwReal						GetPosZ( RwReal fTimeRatio );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CDboHermiteMoveSpline
{
public:
	enum
	{
		eBUILD_MIN_NODE_SIZE = 3
	};

	struct sSPLINE_DATA
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vVel;
		RwReal			fDist;
	};

	typedef std::vector< sSPLINE_DATA > vecdef_SPLINE_DATA_LIST;

protected:
	D3DXMATRIX					m_matHermite;
	vecdef_SPLINE_DATA_LIST		m_defNodeList;
	RwReal						m_fMaxDist;

public:
	CDboHermiteMoveSpline( void );
	virtual ~CDboHermiteMoveSpline( void ) { return; }

public:
	void						Init( void );

	void						AddNodePos( RwV3d* pvPos );

	RwBool						BuildSpline( void );

public:
	RwReal						GetMaxDistance( void );

	RwBool						GetMoveDistRatio( RwReal fTime, RwReal& fMoveDistRatio );

	RwBool						GetPosAndDir( RwReal fTime, RwV3d& vDir, RwV3d& vPos );

protected:
	D3DXVECTOR3					GetStartVelocity( int nIndex );

	D3DXVECTOR3					GetEndVelocity( int nIndex);

	RwV3d						GetDirectionOnCubic( const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vStartVel, const D3DXVECTOR3& vEndPos, const D3DXVECTOR3& vEndVel, RwReal fTime );

	RwV3d						GetPositionOnCubic( const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vStartVel, const D3DXVECTOR3& vEndPos, const D3DXVECTOR3& vEndVel, RwReal fTime );
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#endif
