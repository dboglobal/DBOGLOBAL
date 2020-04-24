/*****************************************************************************
*
* File			: NtlConvexHull.h
* Author		: JeongHo, Rho
* Copyright		: (аж)NTL
* Date			: 2008. 11. 25
* Abstract		: convex hull
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#define CONVERT_CONVEX_RATIO						(100.f)
#define CONVERT_WORLD_TO_CONVEX( p )				((int)(p * CONVERT_CONVEX_RATIO))
#define CONVERT_CONVEX_TO_WORLD( p )				((float)(p / CONVERT_CONVEX_RATIO))


class CNtlConvexHull
{
public:
	typedef std::pair< float, float >				pairdef_POS;
	typedef std::vector< pairdef_POS >				vecdef_POS_LIST;
	typedef vecdef_POS_LIST::reverse_iterator		vecdef_POS_LIST_IT;

	typedef std::pair< int, int >					pairdef_IMP_POS;
	typedef std::vector< pairdef_IMP_POS >			vecdef_IMP_POS_LIST;
	typedef vecdef_IMP_POS_LIST::reverse_iterator	vecdef_IMP_POS_LIST_IT;

protected:
	vecdef_IMP_POS_LIST								m_defRawPosList;

	vecdef_IMP_POS_LIST								m_defTotalHull;
	vecdef_IMP_POS_LIST_IT							m_defTotalHullIT;

	pairdef_POS										m_defRetVal;


public:
    CNtlConvexHull( vecdef_POS_LIST& defRawPosList )
	{
		vecdef_POS_LIST::iterator it = defRawPosList.begin();
		for ( ; it != defRawPosList.end(); ++it )
		{
			m_defRawPosList.push_back( std::make_pair( CONVERT_WORLD_TO_CONVEX( it->first ), CONVERT_WORLD_TO_CONVEX( it->second ) ) );
		}
	}

public:

	void BuildHull( void )
    {
		m_defTotalHull.clear();

		vecdef_IMP_POS_LIST::iterator it = m_defRawPosList.begin();
		for ( ; it != m_defRawPosList.end(); ++it )
		{
			m_defTotalHull = AddPoint( *it, m_defTotalHull );
		}

		if ( m_defTotalHull.size() >= 3 )
		{
			pairdef_IMP_POS Pos1;
			pairdef_IMP_POS Pos2;
			pairdef_IMP_POS Pos3;

			vecdef_IMP_POS_LIST::iterator itTotal = m_defTotalHull.begin();
			for ( ; itTotal != m_defTotalHull.end(); )
			{
				vecdef_IMP_POS_LIST::iterator itStart = itTotal;
				vecdef_IMP_POS_LIST::iterator itNext = itTotal;

				Pos1 = *itNext;

				++itNext;
				if ( m_defTotalHull.end() == itNext ) break;

				Pos2 = *itNext;

				++itNext;
				if ( m_defTotalHull.end() == itNext ) break;

				Pos3 = *itNext;

				Pos1.first -= Pos2.first;
				Pos1.second -= Pos2.second;

				Pos3.first -= Pos2.first;
				Pos3.second -= Pos2.second;

				if ( abs( Pos1.first * Pos3.second - Pos3.first * Pos1.second ) < 0.0001f )
				{
					m_defTotalHull.erase( itNext - 1 );

					itTotal = itStart;
					if ( m_defTotalHull.end() == itTotal ) break;
				}
				else
				{
					++itTotal;
					if ( m_defTotalHull.end() == itTotal ) break;
				}
			}
		}
    }

	size_t GetHullListSize( void )
	{
		return m_defTotalHull.size();
	}

	pairdef_POS* BeginHull( void )
	{
		m_defTotalHullIT = m_defTotalHull.rbegin();

		if ( m_defTotalHullIT == m_defTotalHull.rend() ) return NULL;

		m_defRetVal.first = CONVERT_CONVEX_TO_WORLD( m_defTotalHullIT->first );
		m_defRetVal.second = CONVERT_CONVEX_TO_WORLD( m_defTotalHullIT->second );

		return &m_defRetVal;
	}

	pairdef_POS* NextHull( void )
	{
		++m_defTotalHullIT;

		if ( m_defTotalHullIT == m_defTotalHull.rend() ) return NULL;

		m_defRetVal.first = CONVERT_CONVEX_TO_WORLD( m_defTotalHullIT->first );
		m_defRetVal.second = CONVERT_CONVEX_TO_WORLD( m_defTotalHullIT->second );

		return &m_defRetVal;
	}

protected:
	vecdef_IMP_POS_LIST AddPoint( pairdef_IMP_POS& A, vecdef_IMP_POS_LIST& vSrcList )
	{
		vecdef_IMP_POS_LIST vOutList;

		size_t tSize = vSrcList.size();

		if ( tSize < 2 )
		{
			vOutList = vSrcList;
			vOutList.push_back( A );
			return vOutList;
		}

		bool bPushed = false;
		bool bPrev = IsRight( vSrcList[tSize-1], vSrcList[0], A );

		for ( size_t i = 0; i < tSize; ++i )
		{
			bool bCurr = IsRight( vSrcList[i], vSrcList[(i+1) % tSize], A );
			if ( bPrev )
			{
				if ( bCurr )
				{
					if ( !bPushed )
					{
						vOutList.push_back( A );
						bPushed = true;
					}

					continue;
				}
				else if ( !bPushed )
				{
					vOutList.push_back( A );
					bPushed = true;
				}
			}
			vOutList.push_back( vSrcList[i] );
			bPrev = bCurr;
		}

		return vOutList;
	}

	bool IsRight( pairdef_IMP_POS A, pairdef_IMP_POS B, pairdef_IMP_POS C )
	{
		B.first -= A.first;
		B.second -= A.second;

		C.first -= A.first;
		C.second -= A.second;

		return (B.first * C.second - B.second * C.first < 0) ? true : false;
	}
};
