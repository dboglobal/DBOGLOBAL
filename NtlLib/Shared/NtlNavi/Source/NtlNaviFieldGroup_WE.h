#ifndef _NTL_NAVI_FIELD_GROUP_WE_H_
#define _NTL_NAVI_FIELD_GROUP_WE_H_


#include "../NtlNaviDefine.h"
#include "../PathEngine/i_pathengine.h"
#include "NtlNaviMath.h"
#include "NtlNaviEntity.h"

#include <vector>

class CNtlNaviWorld_WE;
class CNtlNaviFieldHeight_WE;


class CNtlNaviFieldGroup_WE : public CNtlNaviEntity_WE
{
// Declarations
public:
	struct sAGENT
	{
		long						lRadius;
		iShape*						pShape;
	};

	typedef std::list< sAGENT > listdef_AgentList;

	typedef std::map< unsigned int, CNtlNaviFieldHeight_WE* > mapdef_FieldHeightList;

	class CFieldGroup
	{
	public:
		typedef std::list< CNtlNaviFieldHeight_WE* > listdef_NaviFieldHeightList;

	protected:
		CNaviAABB					m_clWorldRgn;
		float						m_fWorldTileSize;
		int							m_nFieldCrossCnt;
		int							m_nFieldCnt;
		CNtlNaviFieldHeight_WE**	m_ppNaviFieldHeightList;
		listdef_NaviFieldHeightList m_defFieldHeightList;

	public:
		CFieldGroup( void );
		~CFieldGroup( void );

	public:
		bool						Create( const CNaviAABB& clWorldRgn, float fWorldTileSize );
		void						Delete( void );

	public:
		bool						IsValid( float fX, float fZ );
		float						GetHeight( float fX, float fZ );

		bool						AttachField( CNtlNaviFieldHeight_WE* pNaviFieldHeight );
		void						ClearField( void );
	};

	class CGroupEntity : public iFaceVertexMesh
	{
	public:
		struct sFace
		{
			sFace( void ) { return; }
			sFace( tSigned32 ver0, tSigned32 ver1, tSigned32 ver2 ) { ver[0] = ver0; ver[1] = ver1; ver[2] = ver2; }
			sFace( const sFace& clVal ) { ver[0] = clVal.ver[0]; ver[1] = clVal.ver[1]; ver[2] = clVal.ver[2]; }

			tSigned32				ver[3];
		};
		typedef std::vector< sFace > FaceList;

	public:
		CNaviAABB					m_clGroupRgn;
		float						m_fWorldTileSize;
		CFieldGroup*				m_pFieldGroup;

	protected:
		int							m_nVertCrossCnt;
		int							m_nVertVerticalCnt;
		int							m_nVertCnt;
		FaceList					m_defFaceList;

	public:
		void						Build( void );

		bool						IsValid( tSigned32 vertex );

	public:
		CGroupEntity( void );
		virtual ~CGroupEntity( void );

	public:
		virtual tSigned32 faces( void ) const
		{
			return (tSigned32)m_defFaceList.size();
		}

		virtual tSigned32 vertices( void ) const
		{
			return (tSigned32)m_nVertCnt;
		}

		virtual tSigned32 vertexIndex( tSigned32 face, tSigned32 vertexInFace ) const
		{
			return m_defFaceList[face].ver[vertexInFace];
		}

		virtual tSigned32 vertexX( tSigned32 vertex ) const
		{
			return (tSigned32) WORLD_COORD_TO_PATH_COORD( (vertex % m_nVertCrossCnt) * m_fWorldTileSize + m_clGroupRgn.minPos.x );
		}

		virtual tSigned32 vertexY( tSigned32 vertex ) const
		{
			return (tSigned32) WORLD_COORD_TO_PATH_COORD( (vertex / m_nVertCrossCnt) * m_fWorldTileSize + m_clGroupRgn.minPos.z );
		}

		virtual float vertexZ( tSigned32 vertex ) const
		{
			float fX = (vertex % m_nVertCrossCnt) * m_fWorldTileSize + m_clGroupRgn.minPos.x;
			float fZ = (vertex / m_nVertCrossCnt) * m_fWorldTileSize + m_clGroupRgn.minPos.z;

			return (float) WORLD_COORD_TO_PATH_COORD( m_pFieldGroup->GetHeight( fX, fZ ) );
		}

		virtual tSigned32 faceAttribute( tSigned32 /*face*/, tSigned32 /*attributeIndex*/ ) const
		{
			return -1;
		}
	};


// Member variables
protected:
	CNtlNaviWorld_WE*				m_pParent;

	int								m_nGroupSize;

	float							m_fGroupOverlap;

	listdef_AgentList				m_defAgentList;

	mapdef_FieldHeightList			m_defFieldHeightList;


// Constructions and Destructions
public:
	CNtlNaviFieldGroup_WE( void );
	~CNtlNaviFieldGroup_WE( void );


// Operations
public:
	CNtlNaviWorld_WE*				GetParent( void );
	void							SetParent( CNtlNaviWorld_WE* pParent );

	int								GetGroupSize( void );
	void							SetGroupSize( int nGroupSize );

	float							GetGroupOverlap( void );
	void							SetGroupOverlap( float fGroupOverlap );

	bool							AttachAgent( long lRadius );
	void							ClearAgent( void );

	bool							AttachFieldHeight( CNtlNaviFieldHeight_WE* pFieldHeight );
	void							ClearFieldHeightList( void );

	virtual bool					ImportWorldData( const char* pPath );
	virtual bool					ExportPathData( const char* pPath );


// Implementations
protected:
	void							Destroy( void );
};


#endif