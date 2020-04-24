/*****************************************************************************
* File			: NtlMeshFederationContainer.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 03. 12
* Abstract		: Ntl MeshFederation Container
*****************************************************************************
* Desc:
* 
*****************************************************************************/

#ifndef _NTL_MESH_FEDERATION_CONTAINER_H_
#define _NTL_MESH_FEDERATION_CONTAINER_H_

#pragma once

#include <map>
#include <string>
#include <list>
#include "..\PathEngine\i_pathengine.h"

#include "NtlPathEngineDef.h"

class CNtlMeshFederation;

class CNtlMeshFederationContainer
{
public:
	CNtlMeshFederationContainer( int nIdx );
	~CNtlMeshFederationContainer( void );

	void		Destroy( void );

	// return world index
	int			GetWorldIndex( void );

	// add to federation header info
	void		AddFederation( int nFieldIdx );
	void		AddFederation( const char* fname );

	// get federation header at nFieldIdx
	CNtlMeshFederation* GetFederation( int nFieldIdx );
	CNtlMeshFederation* GetFederation( const char* fname );

	// get overapped federation's list
	void		GetOverlappedFederation( int nPositionX, int nPositionY, std::list< CNtlMeshFederation* >& outList );
	void		FindOverlappedMeshFederation( int nPositionX, int nPositionY, std::list < CNtlMeshFederation* >& listOverlapped );
	void		FindOverlappedMeshFederationByRange( cHorizontalRange range, std::list < CNtlMeshFederation* >& listOverlapped );

protected:
	typedef std::map< std::string , CNtlMeshFederation* >	mapdef_Federation;
	mapdef_Federation				m_mapFederation;

	int								m_nWorldIdx;
};

#endif