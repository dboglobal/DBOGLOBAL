#ifndef _NTL_NAVI_PATH_DATA_LAODER_H_
#define _NTL_NAVI_PATH_DATA_LAODER_H_


#include "../NtlNaviDefine.h"


class CNtlNaviWorld_PE;


class CNtlNaviPathDataLoader
{
// Declarations
public:
	typedef std::map< unsigned int, CNtlNaviWorld_PE* > mapdef_NaviWorldList;


// Member variables
protected:
	mapdef_NaviWorldList		m_defNaviWorldList;


// Constructions and Destructions
public:
	CNtlNaviPathDataLoader( void );
	~CNtlNaviPathDataLoader( void );


// Operations
public:
	bool						Update( void ) { return true; }
	bool						Load( const char* pRootFolder, vecdef_WorldIDList& defWorldIDList, unsigned char byLoadFlags );

	CNtlNaviWorld_PE*			FindNaviWorld( unsigned int uiWorldID );


// Implementations
protected:
	void						Destroy( void );
};


#endif