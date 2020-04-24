#ifndef _PATH_ENGINE_H_
#define _PATH_ENGINE_H_


#include "NtlNavi.h"

class CPathEngine : public INtlNaviLog
{
// Static
public:
	static CPathEngine*					GetInstance( void );


// Member variables
protected:


// Constructions and Destructions
public:
	CPathEngine( void );
	virtual ~CPathEngine( void );


// Operations
public:
	bool								Create( const char* pPathDllName );

	void								Delete( void );

	bool								Update( void );

	NAVI_INST_HANDLE					CreateInstanceHandler( unsigned int uiWorldID );

	// Instance handler Á¦°Å
	void								DeleteInstanceHandler( NAVI_INST_HANDLE hHandle );

	bool								ImportWorldData( const char* pPath );

	void								GetListImportedWorldIDList( vecdef_WorldIDList& vecOut );

	bool								ExportPathData( const char* pPath );

	bool								ExportPathDataSelect( const char* pPath, mapdef_ExportList& list );

	bool								LoadPathData( const char* pPath );

	int									GetWorldVertexCount( NAVI_INST_HANDLE hHandle );

	int									GetWorldVertexCount( unsigned int uiWorldID );
	
	bool								GetWorldVertexToBuffer( NAVI_INST_HANDLE hHandle, sNAVI_VERTEX* pBuffer );

	bool								GetWorldVertexToBuffer( unsigned int uiWorldID, sNAVI_VERTEX* pBuffer );

	iMesh*								GetNearstMesh( NAVI_INST_HANDLE hHandle, sNAVI_POS& vPos );

	bool								GetWorldVertexList( sNAVI_VERTEX* pVertex, int* pVertexCount );

	unsigned long						GetTotalMemory( void );


// Implementation
protected:
	virtual void						Log( const char* pLog, ... );
};


#endif