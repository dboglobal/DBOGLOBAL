#include "Precomp_NtlPathEngine.h"
#include "NtlMeshFederationContainer.h"

// Standard
#include <stdlib.h>
#include <sstream>

// NtlPathEngine
#include "NtlPathEngine.h"
#include "NtlPathEngineLog.h"

// Federation
#include "NtlMeshFederation.h"
#include "NtlMeshFederationBuilder.h"

// Func
#include "NtlPathEngineHelper.h"

CNtlMeshFederationContainer::CNtlMeshFederationContainer( int nIdx )
: m_nWorldIdx(nIdx)
{
}

CNtlMeshFederationContainer::~CNtlMeshFederationContainer( void )
{
	Destroy();
}

void CNtlMeshFederationContainer::Destroy( void )
{
	for each( std::pair< std::string, CNtlMeshFederation* > pair in m_mapFederation )
	{
		if( pair.second )
		{
			delete pair.second;
			pair.second = NULL;
		}
	}

	m_mapFederation.clear();
}

int CNtlMeshFederationContainer::GetWorldIndex()
{
	return m_nWorldIdx;
}

void CNtlMeshFederationContainer::AddFederation( int nFieldIdx )
{
	char acFieldIdx[NTLPE_MAX_FILENAME_LENGTH+1];
	_itoa_s( nFieldIdx, acFieldIdx, NTLPE_MAX_FILENAME_LENGTH+1, 10 );

	mapdef_Federation::iterator it = m_mapFederation.find( acFieldIdx );
	if( it == m_mapFederation.end() )
	{
		CNtlMeshFederation* federation = new CNtlMeshFederation();

		char acWorldIndex[NTLPE_MAX_FILENAME_LENGTH+1];
		_itoa_s( m_nWorldIdx, acWorldIndex, NTLPE_MAX_FILENAME_LENGTH+1, 10 );

		std::ostringstream filename;
		filename << acWorldIndex;
		filename << "/";
		filename << acFieldIdx;
		filename << NTLPE_FEDERATION_APPEND_HEADER;
		filename << NTLPE_DATA_EXTENSION;

		federation->SetFieldIndex( nFieldIdx );

		char* buffer;
		unsigned long size;
		buffer = NTLPE_LoadBinary_AllocatedBuffer( filename.str().c_str(), size );
		iMeshFederation* mf = GetNtlPathEngine()->GetPathEngine()->loadFederation( NTLPE_DATA_FORMAT, buffer, size );
		federation->SetMeshFederation( mf );
		NTLPE_FreeBuffer(buffer);

		// Insert
		m_mapFederation.insert( std::make_pair( filename.str(), federation ) );
	}
}

void CNtlMeshFederationContainer::AddFederation( const char* fname )
{
	// 중복된 Federation Header 정보가 아닌지 확인한다.
	mapdef_Federation::iterator it = m_mapFederation.find( fname );
	if( it == m_mapFederation.end() )
	{
		CNtlMeshFederationBuilder ntlFederation;
		CNtlMeshFederation *federation = new CNtlMeshFederation();

		// World Index로 Path를 지정해준다.
		char acWorldIndex[NTLPE_MAX_FILENAME_LENGTH+1];
		_itoa_s( m_nWorldIdx, acWorldIndex, NTLPE_MAX_FILENAME_LENGTH+1, 10 );
		
		std::ostringstream filename;
		filename << acWorldIndex;
		filename << "/";
		filename << fname;
		filename << NTLPE_FEDERATION_APPEND_HEADER;
		filename << NTLPE_DATA_EXTENSION;

		federation->SetFieldIndex( atoi( fname ) );
		
		char* buffer;
		unsigned long size;
		buffer = NTLPE_LoadBinary_AllocatedBuffer( filename.str().c_str(), size );
		iMeshFederation* mf = GetNtlPathEngine()->GetPathEngine()->loadFederation( NTLPE_DATA_FORMAT, buffer, size );;
		federation->SetMeshFederation( mf );
		NTLPE_FreeBuffer(buffer);

		// Insert
		m_mapFederation.insert( std::make_pair( filename.str(), federation ) );
	}
}

CNtlMeshFederation* CNtlMeshFederationContainer::GetFederation( int nFieldIdx )
{
	char acFieldIdx[NTLPE_MAX_FILENAME_LENGTH+1];
	_itoa_s( nFieldIdx, acFieldIdx, NTLPE_MAX_FILENAME_LENGTH+1, 10 );

	return GetFederation( acFieldIdx );
}

CNtlMeshFederation* CNtlMeshFederationContainer::GetFederation( const char* fname )
{
	mapdef_Federation::iterator it = m_mapFederation.find( fname );
	if( it == m_mapFederation.end() )
		return NULL;

	return (CNtlMeshFederation*)(*it).second;
}

// 포함되는 영역의 리스트를 포함해서 리턴한다.
void CNtlMeshFederationContainer::GetOverlappedFederation( int nPositionX, int nPositionY, std::list< CNtlMeshFederation* >& outList )
{
	for each( std::pair< std::string, CNtlMeshFederation* > pair in m_mapFederation )
	{		
		CNtlMeshFederation* federation = pair.second;
		if( federation->IsInRangeAtPosition( nPositionX, nPositionY ) )
		{		
			outList.push_back( federation );
			continue;
		}
	}
}

void CNtlMeshFederationContainer::FindOverlappedMeshFederation( int nPositionX, int nPositionY, std::list < CNtlMeshFederation* >& listOverlapped )
{
	for each( std::pair< std::string, CNtlMeshFederation* > pair in m_mapFederation )
	{		
		CNtlMeshFederation* federation = pair.second;
		if( federation->IsInRangeAtPosition( nPositionX, nPositionY ) )
		{		
			listOverlapped.push_back( federation );
			continue;
		}
	}
}

void CNtlMeshFederationContainer::FindOverlappedMeshFederationByRange( cHorizontalRange range, std::list < CNtlMeshFederation* >& listOverlapped )
{
	for each( std::pair < std::string, CNtlMeshFederation* > pair in m_mapFederation )
	{
		CNtlMeshFederation* federation = pair.second;

		for( int i =0; i < federation->GetTileCount(); ++i )
		{
			cHorizontalRange tileRange;
			federation->getRepresentedRegion_World( i, tileRange );

			if( NTLPE_RangesOverlap( range, tileRange ) )
			{
				listOverlapped.push_back( federation );
				break;
			}
		}
	}
}