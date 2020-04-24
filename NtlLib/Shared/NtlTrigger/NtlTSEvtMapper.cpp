#include "precomp_trigger.h"
#include "NtlTSEvtMapper.h"
#include "NtlTSMemIO.h"


/**
	Event mapper
*/


NTL_TS_IMPLEMENT_RTTI( CNtlTSEvtMapper, CNtlTSControlObject )


const CNtlTSEvtMapper::vecdef_TID_LIST* CNtlTSEvtMapper::FindTSList( unsigned int uiId, const std::string& strKey ) const
{
	mapdef_MAPPER::const_iterator citMapper = m_defMapper.find( uiId );
	if ( citMapper == m_defMapper.end() ) return 0;

	hashdef_TS_LIST::const_iterator citTS = citMapper->second.find( strKey );
	if ( citTS == citMapper->second.end() ) return 0;

	return &citTS->second;
}

void CNtlTSEvtMapper::Clear( void )
{
	m_defMapper.clear();
}

bool CNtlTSEvtMapper::Load( const std::string& strFileName )
{
	FILE* pFile = NULL;
	fopen_s( &pFile, strFileName.c_str(), "rb" );

	if ( NULL == pFile )
	{
		return false;
	}

	m_defMapper.clear();

	//////////////////////////////////////////////////////////////////////////
	//	Mapper counter
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt;
	fread( &nMapperCnt, sizeof(nMapperCnt), 1, pFile );

	for ( int nCurMapperCnt = 0; nCurMapperCnt < nMapperCnt; ++nCurMapperCnt )
	{
		//////////////////////////////////////////////////////////////////////////
		//	ID
		//////////////////////////////////////////////////////////////////////////
		unsigned int uiID;
		fread( &uiID, sizeof(uiID), 1, pFile );

		hashdef_TS_LIST* pTSList = &m_defMapper.insert( mapdef_MAPPER::value_type( uiID, hashdef_TS_LIST() ) ).first->second;

		//////////////////////////////////////////////////////////////////////////
		//	TS list counter
		//////////////////////////////////////////////////////////////////////////
		int nTSListCnt;
		fread( &nTSListCnt, sizeof(nTSListCnt), 1, pFile );

		for ( int nCurTSListCnt = 0; nCurTSListCnt < nTSListCnt; ++nCurTSListCnt )
		{
			//////////////////////////////////////////////////////////////////////////
			//	string
			//////////////////////////////////////////////////////////////////////////
			int nStrLen;
			fread( &nStrLen, sizeof(nStrLen), 1, pFile );

			char* pStr = new char[nStrLen+1];
			fread( pStr, nStrLen, 1, pFile );
			pStr[nStrLen] = '\0';

			vecdef_TID_LIST* pTrigList = &pTSList->insert( hashdef_TS_LIST::value_type( pStr, vecdef_TID_LIST() ) ).first->second;

			delete [] pStr;
			pStr = NULL;

			//////////////////////////////////////////////////////////////////////////
			//	Trigger counter
			//////////////////////////////////////////////////////////////////////////
			int nTrigCnt;
			fread( &nTrigCnt, sizeof(nTrigCnt), 1, pFile );

			for ( int nCurTrigCnt = 0; nCurTrigCnt < nTrigCnt; ++nCurTrigCnt )
			{
				//////////////////////////////////////////////////////////////////////////
				//	Trigger
				//////////////////////////////////////////////////////////////////////////
				NTL_TS_T_ID tID;
				fread( &tID, sizeof(tID), 1, pFile );

				pTrigList->push_back( tID );
			}
		}
	}

	fclose( pFile );

	return true;
}

bool CNtlTSEvtMapper::Load( const char* pData, int nDataSize )
{
	CNtlTSMemInput clMemInput( pData, nDataSize );

	m_defMapper.clear();

	//////////////////////////////////////////////////////////////////////////
	//	Mapper counter
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt;
	if ( !clMemInput.Read( &nMapperCnt, sizeof(nMapperCnt) ) )
	{
		m_defMapper.clear();
		return false;
	}

	for ( int nCurMapperCnt = 0; nCurMapperCnt < nMapperCnt; ++nCurMapperCnt )
	{
		//////////////////////////////////////////////////////////////////////////
		//	ID
		//////////////////////////////////////////////////////////////////////////
		unsigned int uiID;
		if ( !clMemInput.Read( &uiID, sizeof(uiID) ) )
		{
			m_defMapper.clear();
			return false;
		}

		hashdef_TS_LIST* pTSList = &m_defMapper.insert( mapdef_MAPPER::value_type( uiID, hashdef_TS_LIST() ) ).first->second;

		//////////////////////////////////////////////////////////////////////////
		//	TS list counter
		//////////////////////////////////////////////////////////////////////////
		int nTSListCnt;
		if ( !clMemInput.Read( &nTSListCnt, sizeof(nTSListCnt) ) )
		{
			m_defMapper.clear();
			return false;
		}

		for ( int nCurTSListCnt = 0; nCurTSListCnt < nTSListCnt; ++nCurTSListCnt )
		{
			//////////////////////////////////////////////////////////////////////////
			//	string
			//////////////////////////////////////////////////////////////////////////
			int nStrLen;
			if ( !clMemInput.Read( &nStrLen, sizeof(nStrLen) ) )
			{
				m_defMapper.clear();
				return false;
			}

			char* pStr = new char[nStrLen+1];

			if ( !clMemInput.Read( pStr, nStrLen ) )
			{
				m_defMapper.clear();
				return false;
			}

			pStr[nStrLen] = '\0';

			vecdef_TID_LIST* pTrigList = &pTSList->insert( hashdef_TS_LIST::value_type( pStr, vecdef_TID_LIST() ) ).first->second;

			delete [] pStr;
			pStr = NULL;

			//////////////////////////////////////////////////////////////////////////
			//	Trigger counter
			//////////////////////////////////////////////////////////////////////////
			int nTrigCnt;
			if ( !clMemInput.Read( &nTrigCnt, sizeof(nTrigCnt) ) )
			{
				m_defMapper.clear();
				return false;
			}

			for ( int nCurTrigCnt = 0; nCurTrigCnt < nTrigCnt; ++nCurTrigCnt )
			{
				//////////////////////////////////////////////////////////////////////////
				//	Trigger
				//////////////////////////////////////////////////////////////////////////
				NTL_TS_T_ID tID;
				if ( !clMemInput.Read( &tID, sizeof(tID) ) )
				{
					m_defMapper.clear();
					return false;
				}

				pTrigList->push_back( tID );
			}
		}
	}

	return true;
}

bool CNtlTSEvtMapper::Save( const std::string& strFileName )
{
	FILE* pFile = NULL;
	fopen_s( &pFile, strFileName.c_str(), "wb" );

	if ( NULL == pFile )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Mapper counter
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt = (int)m_defMapper.size();
	fwrite( &nMapperCnt, sizeof(nMapperCnt), 1, pFile );

	mapdef_MAPPER::iterator itMapper = m_defMapper.begin();
	for ( ; itMapper != m_defMapper.end(); ++itMapper )
	{
		//////////////////////////////////////////////////////////////////////////
		//	ID
		//////////////////////////////////////////////////////////////////////////
		unsigned int uiID = itMapper->first;
		fwrite( &uiID, sizeof(uiID), 1, pFile );

		//////////////////////////////////////////////////////////////////////////
		//	TS list counter
		//////////////////////////////////////////////////////////////////////////
		int nTSListCnt = (int)itMapper->second.size();
		fwrite( &nTSListCnt, sizeof(nTSListCnt), 1, pFile );

		hashdef_TS_LIST::iterator itTSList = itMapper->second.begin();
		for ( ; itTSList != itMapper->second.end(); ++itTSList )
		{
			//////////////////////////////////////////////////////////////////////////
			//	string
			//////////////////////////////////////////////////////////////////////////
			int nStrLen = (int)itTSList->first.size();
			const char* pStr = itTSList->first.c_str();
			fwrite( &nStrLen, sizeof(nStrLen), 1, pFile );
			fwrite( pStr, nStrLen, 1, pFile );

			//////////////////////////////////////////////////////////////////////////
			//	Trigger counter
			//////////////////////////////////////////////////////////////////////////
			int nTrigCnt = (int)itTSList->second.size();
			fwrite( &nTrigCnt, sizeof(nTrigCnt), 1, pFile );

			vecdef_TID_LIST::iterator itTList = itTSList->second.begin();
			for ( ; itTList != itTSList->second.end(); ++itTList )
			{
				//////////////////////////////////////////////////////////////////////////
				//	Trigger
				//////////////////////////////////////////////////////////////////////////
				NTL_TS_T_ID tID = *itTList;
				fwrite( &tID, sizeof(tID), 1, pFile );
			}
		}
	}

	fclose( pFile );

	return true;
}

bool CNtlTSEvtMapper::AddBuildData( const std::string&, const CNtlTSMain::mapdef_TLIST&, void*, void* )
{
	return false;
}

bool CNtlTSEvtMapper::DelBuildData( const std::string& )
{
	return false;
}
