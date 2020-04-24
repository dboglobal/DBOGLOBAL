#ifndef _NTL_NAVI_FIELD_PROP_WE_H_
#define _NTL_NAVI_FIELD_PROP_WE_H_


#include "NtlNaviEntity.h"


class CNtlNaviWorldProp_WE;


class CNtlNaviFieldProp_WE : public CNtlNaviEntity_WE
{
// Declarations
public:


// Member variables
protected:
	CNtlNaviWorldProp_WE*			m_pParent;

	unsigned int					m_uiFieldID;

	unsigned int					m_uiFieldProp;

	float							m_fTileSize;

	unsigned int					m_uiTileCnt;

	// ( unsigned int * m_uiTileCnt )
	// - unsigned int ( upper 2 byte : 老馆 甘 加己, lower 2 byte : 漂荐 甘 加己 )
	unsigned int*					m_pTilePropChunk;


// Constructions and Destructions
public:
	CNtlNaviFieldProp_WE( void );
	~CNtlNaviFieldProp_WE( void );


// Operations
public:
	CNtlNaviWorldProp_WE*			GetParent( void );
	void							SetParent( CNtlNaviWorldProp_WE* pParent );

	unsigned int					GetFieldID( void );
	void							SetFieldID( unsigned int uiFieldID );

	unsigned int					GetFieldProperty( void );
	void							SetFieldProperty( unsigned int uiFieldProp );

	float							GetTileSize( void );
	void							SetTileSize( float fTileSize );

	unsigned int					GetTileCount( void );
	void							SetTileCount( unsigned int uiTileCnt );

	unsigned int*					GetTilePropChunk( void );
	void							SetTilePropChunk_PointerForward( unsigned int* pTilePropChunk );

	virtual bool					ImportWorldData( const char* pPath );
	virtual bool					ExportPathData( const char* pPath );


// Implementations
protected:
	void							Destroy( void );
};


#endif