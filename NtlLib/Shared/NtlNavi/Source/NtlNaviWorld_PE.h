#ifndef _NTL_NAVI_WORLD_PE_H_
#define _NTL_NAVI_WORLD_PE_H_


#include "../NtlNaviDefine.h"
#include "NtlNaviEntity.h"
#include "NtlNaviMath.h"


class CNtlNaviWorldProp_PE;
class CNtlNaviWorldGroup_PE;


class CNtlNaviWorld_PE : public CNtlNaviEntity_PE
{
// Declarations
public:


// Member variables
protected:
	unsigned int					m_uiWorldID;

	CNaviAABB						m_WorldRgn;

	CNtlNaviWorldProp_PE*			m_pWorldProp;

	CNtlNaviWorldGroup_PE*			m_pWorldGroup;


// Constructions and Destructions
public:
	CNtlNaviWorld_PE( void );
	~CNtlNaviWorld_PE( void );


// Operations
public:
	unsigned int					GetWorldID( void );

	CNaviAABB&						GetWorldRgn( void );

	bool							PreLoad( const char* pPath );

	virtual bool					ImportPathData( const char* pPath, unsigned char byLoadFlags );

	bool							IsLoadedWorldGroup( void );

	bool							CanSupportAgent( long lAgentRadius );

	unsigned int					GetAttribute( float x, float z );
	unsigned int					GetTextAllIndex( float x, float z );
	unsigned int					GetZoneIndex( float x, float z );

	float							GetHeight( float fX, float fZ );
	bool							FindNearestPos( long lAgentRadius, CNaviVec3& vSourcePos );
	eCOL_TEST_RESULT				CollisionTest( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos );
	bool							FindPath( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos, vecdef_NaviPosList& defNaviPosList );


// Implementations
protected:
	void							Destroy( void );
};


#endif