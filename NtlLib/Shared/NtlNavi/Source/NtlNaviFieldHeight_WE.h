#ifndef _NTL_NAVI_FIELD_HEIGHT_WE_H_
#define _NTL_NAVI_FIELD_HEIGHT_WE_H_


#include "NtlNaviMath.h"
#include "NtlNaviEntity.h"


class CNtlNaviFieldHeight_WE : public CNtlNaviEntity_WE
{
// Declarations
public:


// Member variables
protected:
	CNtlNaviEntity*					m_pParent;

	std::string						m_strPath;

	unsigned int					m_uiFieldID;

	CNaviAABB						m_clFieldRgn;

	float							m_fTileSize;

	float							m_fInvalidHeightVal;

	unsigned int					m_uiFieldHeightCnt;

	float*							m_pFieldHeightList;


// Constructions and Destructions
public:
	CNtlNaviFieldHeight_WE( void );
	~CNtlNaviFieldHeight_WE( void );


// Operations
public:
	CNtlNaviEntity*					GetParent( void );
	void							SetParent( CNtlNaviEntity* pParent );

	unsigned int					GetFieldID( void );
	void							SetFieldID( unsigned int uiFieldID );

	CNaviAABB&						GetFieldRgn( void );
	void							SetFieldRgn( const CNaviVec3& clFieldMinPos, const CNaviVec3& clFieldMaxPos );

	float							GetTileSize( void );
	void							SetTileSize( float fTileSize );

	float							GetInvalidHeightValue( void );
	void							SetInvalidHeightValue( float fInvalidHeightValue );

	unsigned int					GetFieldHeightCnt( void );
	void							SetFieldHeightCnt( unsigned int uiFieldHeightCnt );

	virtual bool					ImportWorldData( const char* pPath );
	virtual bool					ExportPathData( const char* pPath );

	bool							Load( void );
	void							Unload( void );

	bool							IsValid( float fX, float fZ );
	float							GetHeight( float fX, float fZ );


// Implementations
protected:
	bool							PreLoad( const char* pFileName );

	void							Destroy( void );
};


#endif