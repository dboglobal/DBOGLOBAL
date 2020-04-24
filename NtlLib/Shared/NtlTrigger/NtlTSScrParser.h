#ifndef _NTL_TSSCRPARSER_H_
#define _NTL_TSSCRPARSER_H_


#include "NtlTSScrProperty.h"


class CNtlTSScrObject;
class CNtlTSScrFactory;
class CNtlTSScrFactoryType;
class CNtlTSScrTokenizer;


/** 
	Script parser
*/


class CNtlTSScrParser
{
// Declaration
public:
	struct sOBJ_INFO
	{
		sOBJ_INFO( void ) : pObj( 0 ), pObjType( 0 ) { return; }

		std::string						strName;
		std::string						strType;
		CNtlTSScrObject*				pObj;
		CNtlTSScrFactoryType*			pObjType;
		CNtlTSScrProperty				Property;

		std::list< sOBJ_INFO* > 		listChildren;
	};

	typedef std::map<std::string, sOBJ_INFO> mapdef_ObjMap;
	typedef std::multimap<int, sOBJ_INFO*> mmapdef_ObjSortMap;
	typedef std::pair<std::string, std::string> pairdef_VariablePair;
	typedef std::list<pairdef_VariablePair> listdef_GlobalList;

// Member variables
private:
	CNtlTSScrFactory*					m_pObjFactory;

	const char*							m_pBuff;
	int									m_nSize;

	mapdef_ObjMap						m_defObjMap;
	mmapdef_ObjSortMap					m_defObjSortedMap;
	listdef_GlobalList					m_defGlobalList;

// Constructions and Destructions
public:
	CNtlTSScrParser( CNtlTSScrFactory* pObjFactory ) : m_pObjFactory( pObjFactory ) { return; }
	~CNtlTSScrParser( void ) { return; }

// Methods
public:
	mapdef_ObjMap&						GetObjs( void )			{ return m_defObjMap; }
	mmapdef_ObjSortMap&					GetSortedObjs( void )	{ return m_defObjSortedMap; }
	listdef_GlobalList&					GetGlobalList( void )	{ return m_defGlobalList; }

	bool								Parsing( const std::string& strScriptName, std::vector<CNtlTSScrObject*>& vecCreatedRootObjList );
	bool								Parsing( const std::string& strScriptName, const char* pBuff, int nSize, std::vector<CNtlTSScrObject*>& vecCreatedRootObjList );

// Implementations
private:
	bool								Parsing( CNtlTSScrTokenizer* pLexer );
	bool								Building( std::vector<CNtlTSScrObject*>& vecCreatedRootObjList );
};


#endif