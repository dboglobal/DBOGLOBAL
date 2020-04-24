#ifndef _DBO_TSENTITYFASTFINDER_H_
#define _DBO_TSENTITYFASTFINDER_H_


#include "NtlSharedType.h"


struct sWORLD_TBLDAT;


enum eTS_ENTITY_TYPE
{
	eTS_ENTITY_TYPE_NPC,
};


struct sTSEntity
{
	sTSEntity( eTS_ENTITY_TYPE eTSEntityType, RwReal fX, RwReal fZ, RwUInt32 uiTSEntityID )
		: eTSEntityType( eTSEntityType ), fX( fX ), fZ( fZ ), uiTSEntityID( uiTSEntityID )
	{ return; }

	eTS_ENTITY_TYPE			eTSEntityType;
	RwReal					fX, fZ;
	RwUInt32				uiTSEntityID;
};


class CDboTSWorldMng
{
public:
	typedef std::list< sTSEntity > listdef_EntityList;
	typedef listdef_EntityList SectorInfo;
	typedef listdef_EntityList* PSectorInfo;

	typedef std::list<PSectorInfo> listdef_SectorGarbageList;

protected:
	RwReal						m_fWorldMinX, m_fWorldMaxX;
	RwReal						m_fWorldMinZ, m_fWorldMaxZ;
	RwReal						m_fSectorSize;

	RwInt32						m_nWorldSectorWidthCnt;

	listdef_SectorGarbageList	m_defGarvageList;
	PSectorInfo					*m_pSectorInfo;

public:
	CDboTSWorldMng( void );
	~CDboTSWorldMng( void );

public:
	RwBool Create( RwReal fWorldMinX, RwReal fWorldMaxX, RwReal fWorldMinZ, RwReal fWorldMaxZ, RwReal fSectorSize );
	void Delete( void );

	RwBool AddNPC( RwReal fX, RwReal fZ, RwUInt32 uiTSEntityID );

	RwBool IsEmptyEntity( void );
	void FindEntity( RwReal fX, RwReal fZ, RwReal fRadius, std::list<sTSEntity*>& FindEntityList );

protected:
	RwBool ConvertPosToIndex( RwReal fX, RwReal fZ, RwInt32& nX, RwInt32& nZ );
	void ConvertPosToIndexX( RwReal fX, RwInt32& nX );
	void ConvertPosToIndexZ( RwReal fZ, RwInt32& nZ );
};


class CDboTSEntityFastFinder
{
// Declarations
public:
	#define SECTOR_SIZE			(512.f)

	typedef std::map< TBLIDX, CDboTSWorldMng* > mapdef_WorldList;

// Member variables
protected:
	mapdef_WorldList			m_defWorldList;

// Constructions and Destructions
public:
	CDboTSEntityFastFinder( void );
	~CDboTSEntityFastFinder( void );

// Operations
public:
	RwBool						Create( void );
	void						Delete( void );

	CDboTSWorldMng*				CreateWorldMng( sWORLD_TBLDAT* pWorldTblIdx );
	void						DeleteWorldMng( CDboTSWorldMng* pWorldMng );
	CDboTSWorldMng*				FindWorldMng( TBLIDX WorldTblIdx );
};


#endif