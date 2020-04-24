//***********************************************************************************
//
//	File		:	NtlPairedMap.h
//
//	Begin		:	2006-11-25
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include <map>

template <typename TYPE1, typename TYPE2>
class CNtlPairedMap
{
public:
	CNtlPairedMap(void) {}

	virtual ~CNtlPairedMap(void) {}

public:


	bool Add(TYPE1 val1, TYPE2 val2)
	{
		if( false == m_firstList.insert( std::map<TYPE1, TYPE2>::value_type( val1, val2 ) ).second )
		{
			return false;
		}

		if( false == m_secondList.insert( std::map<TYPE1, TYPE2>::value_type( val2, val1 ) ).second )
		{
			m_firstList.erase( val1 );
			return false;
		}

		return true;
	}


	void RemoveWithFirst(TYPE1 val1)
	{
		std::map<TYPE1, TYPE2>::iterator it = m_firstList.find( val1 );
		if( it != m_firstList.end() )
		{
			m_secondList.erase( it->second );

			m_firstList.erase( it );
		}
	}


	void RemoveWithSecond(TYPE2 val2)
	{
		std::map<TYPE2, TYPE1>::iterator it = m_secondList.find( val2 );
		if( it != m_firstList.end() )
		{
			m_firstList.erase( it->second );

			m_secondList.erase( it );
		}
	}


	DWORD GetCount() { return m_firstList.count(); }



protected:

	void						Init()
	{

	}


protected:


	std::map<TYPE1, TYPE2>			m_firstList;
	std::map<TYPE2, TYPE1>			m_secondList;
};
