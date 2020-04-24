#ifndef _NTL_NAVI_WORLD_PROP_PE_H_
#define _NTL_NAVI_WORLD_PROP_PE_H_


#include "NtlNaviEntity.h"


class CNtlNaviWorld_PE;


class CNtlNaviWorldProp_PE : public CNtlNaviEntity_PE
{
// Declarations
public:
	struct sFIELD_PROPERTY
	{
		unsigned int				uiFieldID;

		unsigned int				uiFieldProp;

		float						fTileSize;
		unsigned int				uiTileCnt;
		unsigned int*				pTilePropChunk;
	};


// Member variables
protected:
	CNtlNaviWorld_PE*				m_pParent;

	unsigned int					m_uiFieldPropCnt;

	sFIELD_PROPERTY**				m_ppFieldPropList;


// Constructions and Destructions
public:
	CNtlNaviWorldProp_PE( void );
	~CNtlNaviWorldProp_PE( void );


// Operations
public:
	CNtlNaviWorld_PE*				GetParent( void );
	void							SetParent( CNtlNaviWorld_PE* pParent );

	virtual bool					ImportPathData( const char* pPath, unsigned char byLoadFlags );

	unsigned int					GetAttribute( float x, float z );
	unsigned int					GetTextAllIndex( float x, float z );
	unsigned int					GetZoneIndex( float x, float z );

	void							Destroy( void );
};


#endif