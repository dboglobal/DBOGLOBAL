#ifndef _NTL_NAVI_FIELD_GROUP_PE_H_
#define _NTL_NAVI_FIELD_GROUP_PE_H_


#include "../NtlNaviDefine.h"
#include "NtlNaviEntity.h"
#include "NtlNaviMath.h"

class iShape;
class iMesh;
class CNtlNaviWorldGroup_PE;


class CNtlNaviFieldGroup_PE : public CNtlNaviEntity_PE
{
// Declarations
public:
	typedef std::map< long, iShape* > mapdef_AgentList;

	struct sFILE_CHUNK_INFO
	{
		sFILE_CHUNK_INFO( void )
		{
		}

		sFILE_CHUNK_INFO( long lChunkSize, char* pChunk )
		{
			this->lChunkSize = lChunkSize;
			this->pChunk = pChunk;
		}

		long						lChunkSize;
		char*						pChunk;
	};


// Member variables
protected:
	CNtlNaviWorldGroup_PE*			m_pParent;

	unsigned int					m_uiGroupID;

	mapdef_AgentList				m_defAgentList;

	iMesh*							m_pMesh;


// Constructions and Destructions
public:
	CNtlNaviFieldGroup_PE( void );
	~CNtlNaviFieldGroup_PE( void );


// Operations
public:
	CNtlNaviWorldGroup_PE*			GetParent( void );
	void							SetParent( CNtlNaviWorldGroup_PE* pParent );

	unsigned int					GetGroupID( void );
	void							SetGroupID( unsigned int uiGroupID );

	void							AttachAgent( long lRadius, iShape* pShape );
	void							ClearAgent( void );

	virtual bool					ImportPathData( const char* pPath, unsigned char byLoadFlags );

	float							GetHeight( float fX, float fZ );
	bool							FindNearestPos( long lAgentRadius, CNaviVec3& vSourcePos );
	eCOL_TEST_RESULT				CollisionTest( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos );
	bool							FindPath( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos, vecdef_NaviPosList& defNaviPosList );


// Implementations
protected:
	void							Destroy( void );

	sFILE_CHUNK_INFO				LoadFileChunk( const char* pFileName );
	void							UnloadFileChunk( sFILE_CHUNK_INFO& sFileChunkInfo );
};


#endif