#ifndef _NTL_NAVI_WORLD_GROUP_PE_H_
#define _NTL_NAVI_WORLD_GROUP_PE_H_


#include "../NtlNaviDefine.h"
#include "NtlNaviEntity.h"
#include "NtlNaviMath.h"


class iShape;
class CNtlNaviWorld_PE;
class CNtlNaviFieldGroup_PE;


class CNtlNaviWorldGroup_PE : public CNtlNaviEntity_PE
{
// Declarations
public:
	struct sAGENT
	{
		long						lRadius;
		iShape*						pShape;
	};

	typedef std::map< long, sAGENT > mapdef_AgentList;


// Member variables
protected:
	CNtlNaviWorld_PE*				m_pParent;

	int								m_nGroupSize;

	float							m_fGroupOverlap;

	mapdef_AgentList				m_defAgentList;

	unsigned int					m_uiGroupCnt;

	CNtlNaviFieldGroup_PE**			m_ppFieldGroupList;


// Constructions and Destructions
public:
	CNtlNaviWorldGroup_PE( void );
	~CNtlNaviWorldGroup_PE( void );


// Operations
public:
	CNtlNaviWorld_PE*				GetParent( void );
	void							SetParent( CNtlNaviWorld_PE* pParent );

	virtual bool					ImportPathData( const char* pPath, unsigned char byLoadFlags );

	bool							CanSupportAgent( long lAgentRadius );

	float							GetHeight( float fX, float fZ );
	bool							FindNearestPos( long lAgentRadius, CNaviVec3& vSourcePos );
	eCOL_TEST_RESULT				CollisionTest( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos );
	bool							FindPath( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos, vecdef_NaviPosList& defNaviPosList );

	void							Destroy( void );


// Implementations
protected:
	bool							AttachAgent( long lRadius );
	void							ClearAgent( void );
	bool							IsSameGroup( CNaviVec3& vSourcePos, CNaviVec3& vDestPos );
};


#endif