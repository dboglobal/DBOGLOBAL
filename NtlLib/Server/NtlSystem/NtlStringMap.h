//***********************************************************************************
//
//	File		:	NtlStringMap.h
//
//	Begin		:	2006-01-10
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	String Map Utility Class ( Not Stable )
//
//***********************************************************************************

#ifndef __NTLSTRINGMAP_H__
#define __NTLSTRINGMAP_H__


//#include <ext/hash_map>
//#include <ext/stl_hash_fun.h>
#include <map>
#include "NtlString.h"

template<class TYPE> class NtlStringMap
{
public:
//	struct eqstr
//	{
//		bool operator()(const char* s1, const char* s2) const
//		{
//			return strcmp(s1, s2) == 0;
//		}
//	};
	

	typedef std::map<CNtlString, TYPE>		MAP;    
	typedef MAP::value_type					MAPVALUE;
	typedef MAP::iterator					MAPIT;


public:

	NtlStringMap() {}
	virtual ~NtlStringMap() {}


public:

    bool				Add(const char * strName, TYPE t);
	
	TYPE				Find(const char * strFind);

	void				Remove(const char * strName);

    
private:

    MAP					m_strMap;

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template<class TYPE>
inline bool NtlStringMap<TYPE>::Add(const char * strName, TYPE t)
{
	if( false == m_strMap.insert( MAPVALUE(CNtlString(strName), t)).second )
		return false;
	
	return true;		
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template<class TYPE>
inline TYPE NtlStringMap<TYPE>::Find(const char * strFind)
{
	MAPIT it = m_strMap.find( CNtlString(strFind) );
	if( it == m_strMap.end() )
		return NULL;
	
	return it->second;		
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template<class TYPE>
void NtlStringMap<TYPE>::Remove(const char * strName)
{
	m_strMap.erase( CNtlString(strName) );
}


#endif // __NTLSTRINGMAP_H__
