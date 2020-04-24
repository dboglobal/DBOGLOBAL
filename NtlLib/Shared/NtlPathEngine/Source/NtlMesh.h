/*****************************************************************************
* File			: NtlMesh.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 03. 14
* Abstract		: Pathengine iMesh Wrapping class
*****************************************************************************
* Desc:
* 
*****************************************************************************/

#ifndef _NTL_MESH_H_
#define _NTL_MESH_H_

#pragma once

#include "../PathEngine/i_pathengine.h"
#include <map>

class CNtlMesh
{
public:
	CNtlMesh();
	CNtlMesh( int nWorldIndex, int nFieldIndex, int nTileIndex, 
		cHorizontalRange& rangeWorld, 
		cHorizontalRange& rangeLocal,
		int nCenterAtWorldX, int nCenterAtWorldY,
		iMesh* pMesh = NULL, iCollisionContext* pContext = NULL);
	virtual ~CNtlMesh();

	virtual void	Destroy();

	virtual void	SetWorldIndex( int nWorldIndex );
	virtual void	SetFieldIndex( int nFieldIndex );
	virtual void	SetTileIndex( int nTileIndex );
	virtual void	SetMesh( iMesh* pMesh );
	virtual void 	SetCollisionContext( iCollisionContext* pCollisionContext );
	virtual void	SetRangeAtWorld( cHorizontalRange& range );

	virtual void	AddAgent( int nRadiusSize );

	virtual int					GetWorldIndex();
	virtual int					GetFieldIndex();
	virtual int					GetTileIndex();
	virtual iMesh*				GetMesh();
	virtual iCollisionContext*	GetCollisionContext();
	virtual cHorizontalRange	GetRangeAtWorld();
	virtual int					GetTileCenterAtWorldX();
	virtual int					GetTileCenterAtWorldY();

	virtual bool				IsRangeOverlapped( cHorizontalRange range );

	// Wrapping
	virtual iAgent*				PlaceAgent( iShape* shape, const cPosition& position ) const;
	virtual cPosition			FindClosestUnobstructedPosition( iShape* shape, iCollisionContext* context,
		const cPosition& position, long maximumDistance ) const;
	virtual iPath*				FindShortestPath( int nShapeRadius, const cPosition& start, const cPosition& goal);


	// PreProcess
public:
	virtual bool				LoadMesh( const char* fileName );

	virtual void				BuildPreProcessFor( const char* filename, int nAgentSize );

protected:
	int					m_nWorldIdx;
	int					m_nFieldIdx;
	int					m_nTileIdx;
	iMesh*				m_pMesh;
	iCollisionContext*	m_pCollisionContext;

	cHorizontalRange	m_rangeRepresentWorld;
	cHorizontalRange	m_rangeRepresentLocal;

	int					m_nCenterAtWorldX;
	int					m_nCenterAtWorldY;

	typedef std::map< int, iShape* > mapdef_Shape;
	mapdef_Shape		m_mapAgentShape;
};

#endif