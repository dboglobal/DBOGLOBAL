#ifndef _NTL_NAVI_ENTITY_H_
#define _NTL_NAVI_ENTITY_H_


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviEntity
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviEntity
{
public:
	CNtlNaviEntity( void ) { return; }
	virtual ~CNtlNaviEntity( void ) { return; }
};


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviEntityWE
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviEntityWE : public CNtlNaviEntity
{
public:
	CNtlNaviEntityWE( void ) { return; }
	virtual ~CNtlNaviEntityWE( void ) { return; }

public:
	virtual bool					ImportWEData( const char* pPath )							= 0;
	virtual bool					ExportPEData( const char* pPath )							= 0;
	virtual bool					ExportPEData( const char* pPath, vecdef_GroupIDList& list ) = 0;
	virtual bool					UpdateToolData( void )										= 0;
};


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviEntityPE
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviEntityPE : public CNtlNaviEntity
{
public:
	CNtlNaviEntityPE( void ) { return; }
	virtual ~CNtlNaviEntityPE( void ) { return; }

public:
	virtual bool					ImportPathData( const char* pPath, unsigned char byLoadFlags ) = 0;
	virtual bool					CheckLoadComplete( void ) = 0;
};


#endif