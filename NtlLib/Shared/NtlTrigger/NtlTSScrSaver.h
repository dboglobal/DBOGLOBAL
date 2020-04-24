#ifndef _NTL_TSSCRSAVER_H_
#define _NTL_TSSCRSAVER_H_


#include "NtlTSScrProperty.h"


class CNtlTSScrObject;
class CNtlTSScrFactory;
class CNtlTSScrFactoryType;


/** 
	Script saver
*/


class CNtlTSScrSaver
{
// Declaration
public:
	struct sOBJ_INFO
	{
		sOBJ_INFO( void ) : pObj( 0 ), pObjType( 0 ) { return; }

		CNtlTSScrObject*				pObj;
		CNtlTSScrFactoryType*			pObjType;
		CNtlTSScrProperty				Property;

		std::list< sOBJ_INFO* > 		listChildren;
	};

	typedef std::map<CNtlTSScrObject*,sOBJ_INFO> mapdef_ObjInfoList;
	typedef std::map<CNtlTSScrObject*,mapdef_ObjInfoList> mapdef_ObjInfoListMap;
	typedef std::map<CNtlTSScrObject*,sOBJ_INFO*> mapdef_Object_ObjInfo_MapList;

// Member variables
public:
	CNtlTSScrFactory*					m_pObjFactory;
	mapdef_ObjInfoListMap				m_defObjInfoListMap;
	mapdef_Object_ObjInfo_MapList		m_defObjObjInfoMapList;

// Constructions and Destructions
public:
	CNtlTSScrSaver( CNtlTSScrFactory* pObjFactory );
	~CNtlTSScrSaver( void );

// Methods
public:
	bool								Save( const std::string& strScriptName, std::vector<CNtlTSScrObject*>& vecRootObjList );

// Implementations
private:
	bool								MakeObjectInfoList( CNtlTSScrObject* pRootObj );
	bool								BuildObjectInfoTree( CNtlTSScrObject* pRootObj, mapdef_ObjInfoList& defObjInfoList );
	bool								WriteToFile( const std::string& strScriptName );
	bool								WriteObj( FILE* pFile, sOBJ_INFO* pstObjInfo, int nDepth );
};


#endif