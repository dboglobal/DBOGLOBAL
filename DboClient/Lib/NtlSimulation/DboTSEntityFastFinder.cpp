#include "precomp_ntlsimulation.h"
#include "DboTSEntityFastFinder.h"
#include "WorldTable.h"
#include "NtlDebug.h"


CDboTSWorldMng::CDboTSWorldMng( void )
{
	m_fWorldMinX = RwRealMAXVAL;
	m_fWorldMaxX = RwRealMINVAL;
	m_fWorldMinZ = RwRealMAXVAL;
	m_fWorldMaxZ = RwRealMINVAL;
	m_fSectorSize = RwRealMAXVAL;

	m_nWorldSectorWidthCnt = 0;

	m_pSectorInfo = NULL;
}


CDboTSWorldMng::~CDboTSWorldMng( void )
{
	Delete();
}


RwBool CDboTSWorldMng::Create( RwReal fWorldMinX, RwReal fWorldMaxX, RwReal fWorldMinZ, RwReal fWorldMaxZ, RwReal fSectorSize )
{
	Delete();

	m_fWorldMinX = fWorldMinX;
	m_fWorldMaxX = fWorldMaxX;
	m_fWorldMinZ = fWorldMinZ;
	m_fWorldMaxZ = fWorldMaxZ;
	m_fSectorSize = fSectorSize;

	const int nXSize = (RwInt32)((m_fWorldMaxX - m_fWorldMinX) / m_fSectorSize);
	const int nZSize = (RwInt32)((m_fWorldMaxZ - m_fWorldMinZ) / m_fSectorSize);

	m_pSectorInfo = NTL_NEW PSectorInfo[nXSize * nZSize];
	if ( NULL == m_pSectorInfo )
	{
		return FALSE;
	}
	memset( m_pSectorInfo, 0, sizeof(PSectorInfo) * nXSize * nZSize );

	m_nWorldSectorWidthCnt = nXSize;

	return TRUE;
}


void CDboTSWorldMng::Delete( void )
{
	m_fWorldMinX = RwRealMAXVAL;
	m_fWorldMaxX = RwRealMINVAL;
	m_fWorldMinZ = RwRealMAXVAL;
	m_fWorldMaxZ = RwRealMINVAL;
	m_fSectorSize = RwRealMAXVAL;

	m_nWorldSectorWidthCnt = 0;

	listdef_SectorGarbageList::iterator it = m_defGarvageList.begin();
	for ( ; it != m_defGarvageList.end(); ++it ) 
	{
		NTL_DELETE( *it );
	}
	m_defGarvageList.clear();

	NTL_ARRAY_DELETE( m_pSectorInfo );
}


RwBool CDboTSWorldMng::AddNPC( RwReal fX, RwReal fZ, RwUInt32 uiTSEntityID )
{
	RwInt32 nX, nZ;
	if ( !ConvertPosToIndex( fX, fZ, nX, nZ ) )
	{
		return FALSE;
	}

	PSectorInfo pSectorInfo = m_pSectorInfo[nZ * m_nWorldSectorWidthCnt + nX];
	if ( NULL == pSectorInfo )
	{
		pSectorInfo = NTL_NEW SectorInfo;

		m_defGarvageList.push_back( pSectorInfo );

		m_pSectorInfo[nZ * m_nWorldSectorWidthCnt + nX] = pSectorInfo;
	}

	pSectorInfo->push_back( sTSEntity( eTS_ENTITY_TYPE_NPC, fX, fZ, uiTSEntityID ) );

	return TRUE;
}


RwBool CDboTSWorldMng::IsEmptyEntity( void )
{
	return m_defGarvageList.empty() ? TRUE : FALSE;
}

void CDboTSWorldMng::FindEntity( RwReal fX, RwReal fZ, RwReal fRadius, std::list<sTSEntity*>& FindEntityList )
{
	if (!m_pSectorInfo)
		return;

	FindEntityList.clear();

	RwInt32 nStartX, nEndX;
	ConvertPosToIndexX( fX - fRadius, nStartX );
	ConvertPosToIndexX( fX + fRadius, nEndX );

	RwInt32 nStartZ, nEndZ;
	ConvertPosToIndexZ( fZ - fRadius, nStartZ );
	ConvertPosToIndexZ( fZ + fRadius, nEndZ );

	sTSEntity* pEntity;
	PSectorInfo pSectorInfo;
	// by daneos: changed <= to < to fix crash issue when teleporting to dungeon
	for ( RwInt32 nZ = nStartZ; nZ </*=*/ nEndZ; ++nZ )
	{
		for ( RwInt32 nX = nStartX; nX </*=*/ nEndX; ++nX )
		{
			pSectorInfo = m_pSectorInfo[nZ * m_nWorldSectorWidthCnt + nX];

			if ( pSectorInfo )
			{
				for (SectorInfo::iterator it = pSectorInfo->begin(); it != pSectorInfo->end(); ++it )
				{
					pEntity = &*it;

					if ( (pEntity->fX - fX) * (pEntity->fX - fX) + (pEntity->fZ - fZ) * (pEntity->fZ - fZ) < fRadius * fRadius )
					{
						FindEntityList.push_back( pEntity );
					}
				}
			}
		}
	}
}


RwBool CDboTSWorldMng::ConvertPosToIndex( RwReal fX, RwReal fZ, RwInt32& nX, RwInt32& nZ )
{
	fX -= m_fWorldMinX; fX /= m_fSectorSize; nX = (RwInt32)fX;
	fZ -= m_fWorldMinZ; fZ /= m_fSectorSize; nZ = (RwInt32)fZ;

	if ( nX < 0 || nX >= (m_fWorldMaxX - m_fWorldMinX) / m_fSectorSize ||
		 nZ < 0 || nZ >= (m_fWorldMaxZ - m_fWorldMinZ) / m_fSectorSize )
	{
		return FALSE;
	}

	return TRUE;
}


void CDboTSWorldMng::ConvertPosToIndexX( RwReal fX, RwInt32& nX )
{
	fX -= m_fWorldMinX; fX /= m_fSectorSize; nX = (RwInt32)fX;

	if ( nX < 0 )
	{
		nX = 0;
	}
	else if ( nX >= (m_fWorldMaxX - m_fWorldMinX) / m_fSectorSize )
	{
		nX = (RwInt32)((m_fWorldMaxX - m_fWorldMinX) / m_fSectorSize) - 1;
	}
}


void CDboTSWorldMng::ConvertPosToIndexZ( RwReal fZ, RwInt32& nZ )
{
	fZ -= m_fWorldMinZ; fZ /= m_fSectorSize; nZ = (RwInt32)fZ;

	if ( nZ < 0 )
	{
		nZ = 0;
	}
	else if ( nZ >= (m_fWorldMaxZ - m_fWorldMinZ) / m_fSectorSize )
	{
		nZ = (RwInt32)((m_fWorldMaxZ - m_fWorldMinZ) / m_fSectorSize) - 1;
	}
}


CDboTSEntityFastFinder::CDboTSEntityFastFinder( void )
{
}

CDboTSEntityFastFinder::~CDboTSEntityFastFinder( void )
{
	Delete();
}

RwBool CDboTSEntityFastFinder::Create( void )
{
	Delete();

	return TRUE;
}

void CDboTSEntityFastFinder::Delete( void )
{
	mapdef_WorldList::iterator it = m_defWorldList.begin();
	for ( ; it != m_defWorldList.end(); ++it )
	{
		NTL_DELETE( it->second );
	}
	m_defWorldList.clear();
}

CDboTSWorldMng* CDboTSEntityFastFinder::CreateWorldMng( sWORLD_TBLDAT* pWorldTblIdx )
{
	CDboTSWorldMng* pWorldMng = FindWorldMng( pWorldTblIdx->tblidx );
	if ( NULL == pWorldMng )
	{
		pWorldMng = NTL_NEW CDboTSWorldMng;

		RwReal fTemp;
		RwV2d vSortStart, vSortEnd;

		vSortStart.x = pWorldTblIdx->vStart.x;
		vSortStart.y = pWorldTblIdx->vStart.z;

		vSortEnd.x = pWorldTblIdx->vEnd.x;
		vSortEnd.y = pWorldTblIdx->vEnd.z;

		if ( vSortStart.x > vSortEnd.x )
		{
			fTemp = vSortStart.x;
			vSortStart.x = vSortEnd.x;
			vSortEnd.x = fTemp;
		}

		if ( vSortStart.y > vSortEnd.y )
		{
			fTemp = vSortStart.y;
			vSortStart.y = vSortEnd.y;
			vSortEnd.y = fTemp;
		}

		if ( !pWorldMng->Create( vSortStart.x, vSortEnd.x, vSortStart.y, vSortEnd.y, SECTOR_SIZE ) )
		{
			NTL_DELETE( pWorldMng );
			return NULL;
		}

		m_defWorldList[pWorldTblIdx->tblidx] = pWorldMng;
	}

	return pWorldMng;
}


void CDboTSEntityFastFinder::DeleteWorldMng( CDboTSWorldMng* pWorldMng )
{
	mapdef_WorldList::iterator it = m_defWorldList.begin();
	for ( ; it != m_defWorldList.end(); ++it )
	{
		if ( it->second == pWorldMng )
		{
			pWorldMng->Delete();

			NTL_DELETE( pWorldMng );

			m_defWorldList.erase( it );

			return;
		}
	}
}


CDboTSWorldMng* CDboTSEntityFastFinder::FindWorldMng( TBLIDX WorldTblIdx )
{
	mapdef_WorldList::iterator it = m_defWorldList.find( WorldTblIdx );
	if ( it == m_defWorldList.end() )
	{
		return NULL;
	}

	return it->second;
}
