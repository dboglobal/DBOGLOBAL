#include "precomp_navi.h"
#include "NtlNaviWorldGroup_PE.h"
#include "NtlNaviUtility.h"
#include "NtlNaviWorld_PE.h"
#include "NtlNaviFieldGroup_PE.h"
#include "NtlNaviEntityFactory.h"
#include "NtlNaviPathEngine.h"


CNtlNaviWorldGroup_PE::CNtlNaviWorldGroup_PE( void )
: m_pParent( NULL )
, m_nGroupSize( 1 )
, m_fGroupOverlap( 0.f )
, m_uiGroupCnt( 0 )
, m_ppFieldGroupList( NULL )
{
}

CNtlNaviWorldGroup_PE::~CNtlNaviWorldGroup_PE( void )
{
	Destroy();
}

CNtlNaviWorld_PE* CNtlNaviWorldGroup_PE::GetParent( void )
{
	return m_pParent;
}

void CNtlNaviWorldGroup_PE::SetParent( CNtlNaviWorld_PE* pParent )
{
	m_pParent = pParent;
}

bool CNtlNaviWorldGroup_PE::ImportPathData( const char* pPath, unsigned char byLoadFlags )
{
	if ( NULL == CNtlNaviPathEngine::GetInstance()->GetPathEngine() )
	{
		Destroy();

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// World group info
	//////////////////////////////////////////////////////////////////////////

	std::string strWorldGroupInfoFile = pPath;
	AttachBackSlash( strWorldGroupInfoFile );

	strWorldGroupInfoFile += PE_WORLD_PATHENGINE_FOLDER;
	AttachBackSlash( strWorldGroupInfoFile );

	strWorldGroupInfoFile += PE_WORLD_GROUP_INFO_FILE;

	FILE* pFile;
	fopen_s( &pFile, strWorldGroupInfoFile.c_str(), "rb" );
	if ( NULL == pFile )
	{
		Destroy();

		return false;
	}

	// Group size
	fread( &m_nGroupSize, sizeof( m_nGroupSize ), 1, pFile );

	// Group overlap size
	fread( &m_fGroupOverlap, sizeof( m_fGroupOverlap ), 1, pFile );

	// Agent count
	int nAgentCount;
	fread( &nAgentCount, sizeof( nAgentCount ), 1, pFile );

	// Agent load
	for ( int i = 0; i < nAgentCount; ++i )
	{
		long lRadius;
		fread( &lRadius, sizeof( lRadius ), 1, pFile );

		if ( !AttachAgent( lRadius ) )
		{
			fclose( pFile );

			Destroy();

			return false;
		}
	}

	// Group total count
	fread( &m_uiGroupCnt, sizeof( m_uiGroupCnt ), 1, pFile );

	fclose( pFile );

	//////////////////////////////////////////////////////////////////////////
	// Group
	//////////////////////////////////////////////////////////////////////////

	m_ppFieldGroupList = new CNtlNaviFieldGroup_PE* [m_uiGroupCnt];
	memset( m_ppFieldGroupList, 0, sizeof(CNtlNaviFieldGroup_PE*) * m_uiGroupCnt );

	for ( unsigned int uiGroupCnt = 0; uiGroupCnt < m_uiGroupCnt; ++uiGroupCnt )
	{
		CNtlNaviFieldGroup_PE* pFieldGroup = CNtlNaviEntityFactory::CreateEntity_FieldGroup_PE();

		m_ppFieldGroupList[uiGroupCnt] = pFieldGroup;

		pFieldGroup->SetParent( this );

		pFieldGroup->SetGroupID( uiGroupCnt );

		mapdef_AgentList::iterator itAgent = m_defAgentList.begin();
		for ( ; itAgent != m_defAgentList.end(); ++itAgent )
		{
			sAGENT& sAgent = itAgent->second;

			pFieldGroup->AttachAgent( sAgent.lRadius, sAgent.pShape );
		}

		if ( !pFieldGroup->ImportPathData( pPath, byLoadFlags ) )
		{
			Destroy();

			return false;
		}
	}

	return true;
}

bool CNtlNaviWorldGroup_PE::CanSupportAgent( long lAgentRadius )
{
	return m_defAgentList.find( lAgentRadius ) == m_defAgentList.end() ? false : true;
}

float CNtlNaviWorldGroup_PE::GetHeight( float fX, float fZ )
{
	if ( NULL == m_ppFieldGroupList ) return NAVI_FLT_MAX;

	CNaviAABB& clWorldRgn = m_pParent->GetWorldRgn();

	unsigned int uiGroupCrossCnt = (int)((clWorldRgn.maxPos.x - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));
	unsigned int uiGroupX = (int)((fX - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));
	unsigned int uiGroupZ = (int)((fZ - clWorldRgn.minPos.z) / (m_nGroupSize * FIELD_SIZE));

	unsigned int uiGroupIdx = uiGroupX + uiGroupZ * uiGroupCrossCnt;

	if ( uiGroupIdx >= m_uiGroupCnt ) return NAVI_FLT_MAX;

	return m_ppFieldGroupList[uiGroupIdx]->GetHeight( fX, fZ );
}

bool CNtlNaviWorldGroup_PE::FindNearestPos( long lAgentRadius, CNaviVec3& vSourcePos )
{
	if ( NULL == m_ppFieldGroupList ) return false;

	CNaviAABB& clWorldRgn = m_pParent->GetWorldRgn();

	unsigned int uiGroupCrossCnt = (int)((clWorldRgn.maxPos.x - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));
	unsigned int uiGroupX = (int)((vSourcePos.x - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));
	unsigned int uiGroupZ = (int)((vSourcePos.z - clWorldRgn.minPos.z) / (m_nGroupSize * FIELD_SIZE));

	unsigned int uiGroupIdx = uiGroupX + uiGroupZ * uiGroupCrossCnt;

	if ( uiGroupIdx >= m_uiGroupCnt ) return false;

	return m_ppFieldGroupList[uiGroupIdx]->FindNearestPos( lAgentRadius, vSourcePos );
}

eCOL_TEST_RESULT CNtlNaviWorldGroup_PE::CollisionTest( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos )
{
	if ( NULL == m_ppFieldGroupList ) return eCOL_TEST_RESULT_FAILED;

	if ( !IsSameGroup( vSourcePos, vTargetPos ) ) return eCOL_TEST_RESULT_NOT_SAME_GROUP;

	CNaviAABB& clWorldRgn = m_pParent->GetWorldRgn();

	unsigned int uiGroupCrossCnt = (int)((clWorldRgn.maxPos.x - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));
	unsigned int uiGroupX = (int)((vSourcePos.x - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));
	unsigned int uiGroupZ = (int)((vSourcePos.z - clWorldRgn.minPos.z) / (m_nGroupSize * FIELD_SIZE));

	unsigned int uiGroupIdx = uiGroupX + uiGroupZ * uiGroupCrossCnt;

	if ( uiGroupIdx >= m_uiGroupCnt ) return eCOL_TEST_RESULT_FAILED;

	return m_ppFieldGroupList[uiGroupIdx]->CollisionTest( lAgentRadius, vSourcePos, vTargetPos );
}

bool CNtlNaviWorldGroup_PE::FindPath( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos, vecdef_NaviPosList& defNaviPosList )
{
	if ( NULL == m_ppFieldGroupList ) return false;

	if ( !IsSameGroup( vSourcePos, vTargetPos ) ) return false;

	CNaviAABB& clWorldRgn = m_pParent->GetWorldRgn();

	unsigned int uiGroupCrossCnt = (int)((clWorldRgn.maxPos.x - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));
	unsigned int uiGroupX = (int)((vSourcePos.x - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));
	unsigned int uiGroupZ = (int)((vSourcePos.z - clWorldRgn.minPos.z) / (m_nGroupSize * FIELD_SIZE));

	unsigned int uiGroupIdx = uiGroupX + uiGroupZ * uiGroupCrossCnt;

	if ( uiGroupIdx >= m_uiGroupCnt ) return false;

	return m_ppFieldGroupList[uiGroupIdx]->FindPath( lAgentRadius, vSourcePos, vTargetPos, defNaviPosList );
}

void CNtlNaviWorldGroup_PE::Destroy( void )
{
	if ( m_ppFieldGroupList )
	{
		for ( unsigned int uiGroupCnt = 0; uiGroupCnt < m_uiGroupCnt; ++uiGroupCnt )
		{
			if ( m_ppFieldGroupList[uiGroupCnt] )
			{
				CNtlNaviEntityFactory::DeleteEntity_PE( m_ppFieldGroupList[uiGroupCnt] );
				m_ppFieldGroupList[uiGroupCnt] = NULL;
			}
		}

		delete [] m_ppFieldGroupList;
		m_ppFieldGroupList = NULL;
	}

	ClearAgent();
}

bool CNtlNaviWorldGroup_PE::AttachAgent( long lRadius )
{
	if ( NULL == CNtlNaviPathEngine::GetInstance()->GetPathEngine() )
	{
		return false;
	}

	tSigned32 arAgentCoord[NAVI_AGENT_COORD_COUNT];

	arAgentCoord[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( lRadius );
	arAgentCoord[1] = 0;

	arAgentCoord[2] = 0;
	arAgentCoord[3] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( lRadius );

	arAgentCoord[4] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( lRadius );
	arAgentCoord[5] = 0;

	arAgentCoord[6] = 0;
	arAgentCoord[7] = (tSigned32) WORLD_COORD_TO_PATH_COORD( lRadius );

	sAGENT sAgent;

	sAgent.lRadius = lRadius;

	sAgent.pShape = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->newShape( sizeof( arAgentCoord ) / sizeof( *arAgentCoord ) / 2, arAgentCoord );

	if ( NULL == sAgent.pShape )
	{
		return false;
	}

	m_defAgentList[lRadius] = sAgent;

	return true;
}

void CNtlNaviWorldGroup_PE::ClearAgent( void )
{
	mapdef_AgentList::iterator it = m_defAgentList.begin();
	for ( ; it != m_defAgentList.end(); ++it )
	{
		iShape* pShape = it->second.pShape;

		if ( pShape )
		{
			pShape->destroy();
		}
	}
	m_defAgentList.clear();
}

bool CNtlNaviWorldGroup_PE::IsSameGroup( CNaviVec3& vSourcePos, CNaviVec3& vDestPos )
{
	CNaviAABB& clWorldRgn = m_pParent->GetWorldRgn();

	int nGroupXIndex = (int)((vSourcePos.x - clWorldRgn.minPos.x) / (m_nGroupSize * FIELD_SIZE));

	float fGroupXMin = clWorldRgn.minPos.x + nGroupXIndex * (m_nGroupSize * FIELD_SIZE) - m_fGroupOverlap;
	float fGroupXMax = clWorldRgn.minPos.x + (nGroupXIndex + 1 ) * (m_nGroupSize * FIELD_SIZE) + m_fGroupOverlap;

	int nGroupZIndex = (int)((vSourcePos.z - clWorldRgn.minPos.z) / (m_nGroupSize * FIELD_SIZE));

	float fGroupZMin = clWorldRgn.minPos.z + nGroupZIndex * (m_nGroupSize * FIELD_SIZE) - m_fGroupOverlap;
	float fGroupZMax = clWorldRgn.minPos.z + (nGroupZIndex + 1 ) * (m_nGroupSize * FIELD_SIZE) + m_fGroupOverlap;

	if ( vDestPos.x < fGroupXMin || vDestPos.x >= fGroupXMax ||
		 vDestPos.z < fGroupZMin || vDestPos.z >= fGroupZMax )
	{
		return false;
	}

	return true;
}
