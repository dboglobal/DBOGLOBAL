#ifndef _NTL_NAVI_WORLD_PROP_WE_H_
#define _NTL_NAVI_WORLD_PROP_WE_H_


#include "NtlNaviEntity.h"


class CNtlNaviWorld_WE;
class CNtlNaviFieldProp_WE;


class CNtlNaviWorldProp_WE : public CNtlNaviEntity_WE
{
// Declarations
public:
	typedef std::list< CNtlNaviFieldProp_WE* > listdef_FieldPropList;


// Member variables
protected:
	CNtlNaviWorld_WE*				m_pParent;

	listdef_FieldPropList			m_defFieldPropList;


// Constructions and Destructions
public:
	CNtlNaviWorldProp_WE( void );
	~CNtlNaviWorldProp_WE( void );


// Operations
public:
	CNtlNaviWorld_WE*				GetParent( void );
	void							SetParent( CNtlNaviWorld_WE* pParent );

	virtual bool					ImportWorldData( const char* pPath );
	virtual bool					ExportPathData( const char* pPath );


// Implementations
protected:
	void							Destroy( void );
};


#endif