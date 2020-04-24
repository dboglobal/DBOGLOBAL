#ifndef __NTL_NAVI_WORLD_H__
#define __NTL_NAVI_WORLD_H__


#include <string>
#include <list>
#include <map>
#include <vector>
#include "NtlNaviDataDefine.h"


//////////////////////////////////////////////////////////////////////////
//
//	Navi info
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviInfo
{
protected:
	CNtlNaviInfo*					m_pParent;

public:
	CNtlNaviInfo( void ) : m_pParent( NULL ) { return; }
	virtual ~CNtlNaviInfo( void ) { return; }

public:
	virtual eNAVI_INFO				GetType( void ) const { return eNAVI_INFO_BASE; }

	CNtlNaviInfo*					GetParent( void );
	void							SetParent( CNtlNaviInfo* pParent );

	virtual bool					Import( const char* /*pPathName*/ ) { return true; }
	virtual bool					Export( const char* /*pPathName*/ ) { return true; }
};

inline CNtlNaviInfo* CNtlNaviInfo::GetParent( void )
{
	return m_pParent;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi world info
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviWorldInfo : public CNtlNaviInfo
{
public:
	typedef std::vector< float >	vecdef_AGENCY_LIST;

protected:
	eNAVI_WORLD_TYPE				m_eWorldType;
	unsigned int					m_uiWorldID;
	std::string						m_strWorldName;
	float							m_fWorldMinPosX, m_fWorldMinPosZ;
	float							m_fWorldMaxPosX, m_fWorldMaxPosZ;
	vecdef_AGENCY_LIST				m_defAgencyList;

public:
	CNtlNaviWorldInfo( eNAVI_WORLD_TYPE eWorldType );
	virtual ~CNtlNaviWorldInfo( void );

public:
	virtual eNAVI_INFO				GetType( void ) const { return eNAVI_INFO_WORLD; }

	eNAVI_WORLD_TYPE				GetWorldType( void ) const;
	void							SetWorldType( eNAVI_WORLD_TYPE eWorldType );

	unsigned int					GetWorldID( void ) const;
	void							SetWorldID( unsigned int uiWorldID );

	const std::string&				GetWorldName( void ) const;
	void							SetWorldName( const char* pWorldName );

	void							GetWorldMinPos( float& fWorldMinPosX, float& fWorldMinPosZ ) const;
	void							SetWorldMinPos( float fWorldMinPosX, float fWorldMinPosZ );

	void							GetWorldMaxPos( float& fWorldMaxPosX, float& fWorldMaxPosZ ) const;
	void							SetWorldMaxPos( float fWorldMaxPosX, float fWorldMaxPosZ );

	int								GetAgencyCnt( void );
	float							GetAgency( int nIdx );
	void							AttachAgency( float fAgencyRadius );

	virtual bool					Import( const char* pPathName );
	virtual bool					Export( const char* pPathName );
};

inline eNAVI_WORLD_TYPE CNtlNaviWorldInfo::GetWorldType( void ) const
{
	return m_eWorldType;
}

inline unsigned int CNtlNaviWorldInfo::GetWorldID( void ) const
{
	return m_uiWorldID;
}

inline const std::string& CNtlNaviWorldInfo::GetWorldName( void ) const
{
	return m_strWorldName;
}

inline void CNtlNaviWorldInfo::GetWorldMinPos( float& fWorldMinPosX, float& fWorldMinPosZ ) const
{
	fWorldMinPosX = m_fWorldMinPosX;
	fWorldMinPosZ = m_fWorldMinPosZ;
}

inline void CNtlNaviWorldInfo::GetWorldMaxPos( float& fWorldMaxPosX, float& fWorldMaxPosZ ) const
{
	fWorldMaxPosX = m_fWorldMaxPosX;
	fWorldMaxPosZ = m_fWorldMaxPosZ;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi world out-door info
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviWorldOutDoorInfo : public CNtlNaviWorldInfo
{
protected:
	float							m_fTileSize;				// 실제 월드의 타일 사이즈
	float							m_fFieldSize;				// 실제 월드의 필드 사이즈
	unsigned int					m_uiCrossFieldCntOfGroup;	// Group 당 필드의 갯수
	float							m_fOverlapSize;

public:
	CNtlNaviWorldOutDoorInfo( void );
	virtual ~CNtlNaviWorldOutDoorInfo( void );

public:
	virtual eNAVI_INFO				GetType( void ) const { return eNAVI_INFO_WORLD_OUTDOOR; }

	float							GetTileSize( void ) const;
	void							SetTileSize( float fTileSize );

	float							GetFieldSize( void ) const;
	void							SetFieldSize( float fFieldSize );

	unsigned int					GetCrossFieldCntOfGroup( void ) const;
	void							SetCrossFieldCntOfGroup( unsigned int uiCrossFieldCntOfGroup );

	float							GetOverlapSize( void ) const;
	void							SetOverlapSize( float fOverlapSize );

	virtual bool					Import( const char* pPathName );
	virtual bool					Export( const char* pPathName );
};

inline float CNtlNaviWorldOutDoorInfo::GetTileSize( void ) const
{
	return m_fTileSize;
}

inline float CNtlNaviWorldOutDoorInfo::GetFieldSize( void ) const
{
	return m_fFieldSize;
}

inline unsigned int CNtlNaviWorldOutDoorInfo::GetCrossFieldCntOfGroup( void ) const
{
	return m_uiCrossFieldCntOfGroup;
}

inline float CNtlNaviWorldOutDoorInfo::GetOverlapSize( void ) const
{
	return m_fOverlapSize;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi world in-door info
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviWorldInDoorInfo : public CNtlNaviWorldInfo
{
protected:
	float							m_fBlockSize;

public:
	CNtlNaviWorldInDoorInfo( void );
	virtual ~CNtlNaviWorldInDoorInfo( void );

public:
	virtual eNAVI_INFO				GetType( void ) const { return eNAVI_INFO_WORLD_INDOOR; }

	float							GetBlockSize( void ) const;
	void							SetBlockSize( float fBlockSize );

	virtual bool					Import( const char* pPathName );
	virtual bool					Export( const char* pPathName );
};

inline float CNtlNaviWorldInDoorInfo::GetBlockSize( void ) const
{
	return m_fBlockSize;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi group out-door info
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviGroupOutDoorInfo : public CNtlNaviInfo
{
public:
	enum { eSECTOR_MAX = 4 };

	struct sFIELD_HEIGHT_LIST
	{
		sFIELD_HEIGHT_LIST( void )
		{
			memset( parSectorHeightList, 0, sizeof( parSectorHeightList ) );
		}

		float*						parSectorHeightList[eSECTOR_MAX];
	};

	struct sNAVI_OBJECT_LIST
	{
		std::string					strModelName;
		float						fPosX, fPosY, fPosZ;
		float						fScaleX, fScaleY, fScaleZ;
		float						fYRotVal;
	};

	typedef std::vector< sNAVI_OBJECT_LIST* > vecdef_NAVI_OBJECT_LSIT;
	typedef std::map< unsigned int, sFIELD_HEIGHT_LIST > hashdef_FIELD_HEIGHT_LIST;

protected:
	unsigned int					m_uiGroupID;
	float							m_fInvalidHeightVal;
	unsigned int					m_uiTileCntOfSector;
	unsigned int					m_uiFieldCntOfGroup;
	hashdef_FIELD_HEIGHT_LIST		m_defFieldHeightList;
	vecdef_NAVI_OBJECT_LSIT			m_defNaviMeshList;
	vecdef_NAVI_OBJECT_LSIT			m_defObsMeshList;

public:
	CNtlNaviGroupOutDoorInfo( void );
	virtual ~CNtlNaviGroupOutDoorInfo( void );

public:
	virtual eNAVI_INFO				GetType( void ) const { return eNAVI_INFO_GROUP_OUTDOOR; }

	unsigned int					GetGroupID( void ) const;
	void							SetGroupID( unsigned int uiGroupID );

	float							GetInvalidHeightVal( void ) const;
	void							SetInvalidHeightVal( float fInvalidHeightVal );

	unsigned int					GetTileCntOfSector( void ) const;
	void							SetTileCntOfSector( unsigned int uiTileCntOfSector );

	unsigned int					GetFieldCntOfGroup( void ) const;
	void							SetFieldCntOfGroup( unsigned int uiFieldCntOfGroup );

	sFIELD_HEIGHT_LIST*				GetSectorHeightList( unsigned int uiFieldID );
	void							AttachSectorHeightList( unsigned int uiFieldID, int nIdx, float* pSectorHeightList );

	int								GetNaviMeshCnt( void );
	sNAVI_OBJECT_LIST*				GetNaviMesh( int nIdx );
	void							AttachNaviMesh( const char* pModelName, float fPosX, float fPosY, float fPosZ, float fScaleX, float fScaleY, float fScaleZ, float fYRotVal );

	int								GetObsMeshCnt( void );
	sNAVI_OBJECT_LIST*				GetObsMesh( int nIdx );
	void							AttachObsMesh( const char* pModelName, float fPosX, float fPosY, float fPosZ, float fScaleX, float fScaleY, float fScaleZ, float fYRotVal );

	virtual bool					Import( const char* pPathName );
	virtual bool					Export( const char* pPathName );
};

inline unsigned int CNtlNaviGroupOutDoorInfo::GetGroupID( void ) const
{
	return m_uiGroupID;
}

inline float CNtlNaviGroupOutDoorInfo::GetInvalidHeightVal( void ) const
{
	return m_fInvalidHeightVal;
}

inline unsigned int CNtlNaviGroupOutDoorInfo::GetTileCntOfSector( void ) const
{
	return m_uiTileCntOfSector;
}

inline unsigned int CNtlNaviGroupOutDoorInfo::GetFieldCntOfGroup( void ) const
{
	return m_uiFieldCntOfGroup;
}

inline CNtlNaviGroupOutDoorInfo::sFIELD_HEIGHT_LIST* CNtlNaviGroupOutDoorInfo::GetSectorHeightList( unsigned int uiFieldID )
{
	hashdef_FIELD_HEIGHT_LIST::iterator it = m_defFieldHeightList.find( uiFieldID );
	if ( it == m_defFieldHeightList.end() ) return NULL;
	return &it->second;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi group in-door info
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviGroupInDoorInfo : public CNtlNaviInfo
{
public:
	struct sNAVI_OBJECT_LIST
	{
		std::string					strModelName;
		float						fPosX, fPosY, fPosZ;
		float						fScaleX, fScaleY, fScaleZ;
		float						fYRotVal;
	};

	typedef std::vector< sNAVI_OBJECT_LIST* > vecdef_NAVI_OBJECT_LSIT;

protected:
	std::string						m_strModelName;
	vecdef_NAVI_OBJECT_LSIT			m_defNaviMeshList;
	vecdef_NAVI_OBJECT_LSIT			m_defObsMeshList;

public:
	CNtlNaviGroupInDoorInfo( void );
	virtual ~CNtlNaviGroupInDoorInfo( void );

public:
	virtual eNAVI_INFO				GetType( void ) const { return eNAVI_INFO_GROUP_INDOOR; }

	const std::string&				GetModelName( void ) const;
	void							SetModelName( const char* pModelName );

	int								GetNaviMeshCnt( void );
	sNAVI_OBJECT_LIST*				GetNaviMesh( int nIdx );
	void							AttachNaviMesh( const char* pModelName, float fPosX, float fPosY, float fPosZ, float fScaleX, float fScaleY, float fScaleZ, float fYRotVal );

	int								GetObsMeshCnt( void );
	sNAVI_OBJECT_LIST*				GetObsMesh( int nIdx );
	void							AttachObsMesh( const char* pModelName, float fPosX, float fPosY, float fPosZ, float fScaleX, float fScaleY, float fScaleZ, float fYRotVal );

	virtual bool					Import( const char* pPathName );
	virtual bool					Export( const char* pPathName );
};

inline const std::string& CNtlNaviGroupInDoorInfo::GetModelName( void ) const
{
	return m_strModelName;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi property out-door info
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPropOutDoorInfo : public CNtlNaviInfo
{
protected:
	unsigned int					m_uiFieldID;
	unsigned int					m_uiFieldProp;
	unsigned int					m_uiTileCntOfField;
	unsigned int*					m_pTileProp;

public:
	CNtlNaviPropOutDoorInfo( void );
	virtual ~CNtlNaviPropOutDoorInfo( void );

public:
	virtual eNAVI_INFO				GetType( void ) const { return eNAVI_INFO_PROP_OUTDOOR; }

	unsigned int					GetFieldID( void ) const;
	void							SetFieldID( unsigned int uiFieldID );

	unsigned int					GetFieldProp( void ) const;
	void							SetFieldProp( unsigned int uiFieldProp );

	unsigned int					GetTileCntOfField( void ) const;
	void							SetTileCntOfField( unsigned int uiTileCntOfField );

	unsigned int*					GetTileProp( void ) const;
	void							SetTileProp( unsigned int* pTileProp );

	virtual bool					Import( const char* pPathName );
	virtual bool					Export( const char* pPathName );
};

inline unsigned int CNtlNaviPropOutDoorInfo::GetFieldID( void ) const
{
	return m_uiFieldID;
}

inline unsigned int CNtlNaviPropOutDoorInfo::GetFieldProp( void ) const
{
	return m_uiFieldProp;
}

inline unsigned int CNtlNaviPropOutDoorInfo::GetTileCntOfField( void ) const
{
	return m_uiTileCntOfField;
}

inline unsigned int* CNtlNaviPropOutDoorInfo::GetTileProp( void ) const
{
	return m_pTileProp;
}


//////////////////////////////////////////////////////////////////////////
//
//	Navi property in-door info
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPropInDoorInfo : public CNtlNaviInfo
{
public:
	enum eENTITY_TYPE
	{
		eENTITY_TYPE_SPHERE,
		eENTITY_TYPE_PLANE
	};

	struct sSPHERE
	{
		float						fX, fY, fZ;
		float						fRadius;
		unsigned int				uiProperty;
	};

	struct sPLANE
	{
		float						fX, fY, fZ;
		float						fWidth;
		float						fDepth;
		unsigned int				uiProperty;
	};

	union uENTITY_DATA
	{
		sSPHERE						sSphere;
		sPLANE						sPlane;
	};

	struct sENTITY_INFO
	{
		eENTITY_TYPE				eEntityType;
		uENTITY_DATA				uEntityData;
	};

	typedef std::vector< sENTITY_INFO* > vecdef_NAVI_ENTITY_PROP_LIST;

protected:
	unsigned int					m_uiBlockID;
	unsigned int					m_uiBlockProp;
	unsigned int					m_uiGameProp;
	vecdef_NAVI_ENTITY_PROP_LIST	m_defEntityPropList;

public:
	CNtlNaviPropInDoorInfo( void );
	virtual ~CNtlNaviPropInDoorInfo( void );

public:
	virtual eNAVI_INFO				GetType( void ) const { return eNAVI_INFO_PROP_INDOOR; }

	unsigned int					GetBlockID( void ) const;
	void							SetBlockID( unsigned int uiBlockID );

	unsigned int					GetBlockProp( void ) const;
	void							SetBlockProp( unsigned int uiBlockProp );

	unsigned int					GetGameProp( void ) const;
	void							SetGameProp( unsigned int uiGameProp );

	int								GetEntityInfoCnt( void );
	sENTITY_INFO*					GetEntityInfo( int nIdx );
	void							AttachEntityInfo_Sphere( float fPosX, float fPosY, float fPosZ, float fRadius );
	void							AttachEntityInfo_Plane( float fPosX, float fPosY, float fPosZ, float fWidth, float fDepth );

	virtual bool					Import( const char* pPathName );
	virtual bool					Export( const char* pPathName );
};

inline unsigned int CNtlNaviPropInDoorInfo::GetBlockID( void ) const
{
	return m_uiBlockID;
}

inline unsigned int CNtlNaviPropInDoorInfo::GetBlockProp( void ) const
{
	return m_uiBlockProp;
}

inline unsigned int CNtlNaviPropInDoorInfo::GetGameProp( void ) const
{
	return m_uiGameProp;
}


#endif