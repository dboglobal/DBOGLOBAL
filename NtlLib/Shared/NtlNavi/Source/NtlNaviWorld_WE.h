#ifndef _NTL_NAVI_WORLD_WE_H_
#define _NTL_NAVI_WORLD_WE_H_


#include "NtlNaviEntity.h"
#include "NtlNaviMath.h"


class CNtlNaviWorldProp_WE;
class CNtlNaviFieldGroup_WE;


class CNtlNaviWorld_WE : public CNtlNaviEntity_WE
{
// Declarations
public:
	typedef std::list< long > listdef_AgentList;


// Member variables
protected:
	unsigned int					m_uiWorldID;

	CNaviAABB						m_WorldRgn;

	float							m_fWorldTileSize;

	int								m_nGroupSize;

	float							m_fGroupOverlap;

	listdef_AgentList				m_defAgentList;

	CNtlNaviWorldProp_WE*			m_pWorldProp;

	CNtlNaviFieldGroup_WE*			m_pFieldGroup;


// Constructions and Destructions
public:
	CNtlNaviWorld_WE( void );
	~CNtlNaviWorld_WE( void );


// Operations
public:
	unsigned int					GetWorldID( void );
	void							SetWorldID( unsigned int uiWorldID );

	CNaviAABB&						GetWorldRgn( void );
	void							SetWorldRgn( const CNaviVec3& clPos1, const CNaviVec3& clPos2 );

	float							GetWorldTileSize( void );
	void							SetWorldTileSize( float fWorldTileSize );

	int								GetGroupSize( void );
	void							SetGroupSize( int nGroupSize );

	float							GetGroupOverlap( void );
	void							SetGroupOverlap( float fGroupOverlap );

	void							AttachAgent( long lRadius );
	void							ClearAgentList( void );

	CNtlNaviWorldProp_WE*			GetWorldProperty( void );
	void							SetWorldProperty( CNtlNaviWorldProp_WE* pWorldProp );

	CNtlNaviFieldGroup_WE*			GetFieldGroup( void );
	void							SetFieldGroup( CNtlNaviFieldGroup_WE* pFieldGroup );

	virtual bool					ImportWorldData( const char* pPath );
	virtual bool					ExportPathData( const char* pPath );


// Implementations
protected:
	void							Destroy( void );
};


#endif