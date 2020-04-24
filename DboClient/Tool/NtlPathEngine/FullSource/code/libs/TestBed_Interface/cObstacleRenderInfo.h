//**********************************************************************
//
// Copyright (c) 2002-2006
// NTL TestBed
// Haesung, Cho
//
// All Rights Reserved
//
//**********************************************************************

#ifndef OBSTACLE_RENDER_INFO_INCLUDED
#define OBSTACLE_RENDER_INFO_INCLUDED

#include "libs/Geometry/interface/Vector3F.h"
#include "common/STL/vector.h"
#include "common/interface/Assert.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/MeshPair/interface/TranslationToOverlapped.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/TestBed_Application/VertexTypes.h"

class iGraphicsInterface;

class cObstacleRenderInfo
{
public:
	cObstacleRenderInfo();
	virtual ~cObstacleRenderInfo();

	void PushVector3F( cVector3F& vector );

	void Build( const tMeshCircuits& circuits, iGraphicsInterface *gip, const cTranslationToOverlapped& translation );
	void BuildMeshCurSequence( const tMeshCutSequence& cut_sequence, iGraphicsInterface *gip, const cTranslationToOverlapped& translation );
	void BuildMeshCut(const cMeshCut& cut, iGraphicsInterface *gip, const cTranslationToOverlapped& translation);
	void BuildCircuitElement_SI( iGraphicsInterface* gip, const cTranslationToOverlapped& translation,
		const cCircuitElement& el,
		tFace_3D& startFace, cVector2F& approximateStartPoint );
	void BuildLineInFace( iGraphicsInterface* gip, tFace_3D f, const cVector2F& p1, const cVector2F& p2 );

	void BuildVertexBuffer();

	void Render();

protected:
	std::vector<cVector3F> _obstacleElements;
	IDirect3DVertexBuffer9* _vertexBuffer;
	unsigned int			_vertexCount;
};

#endif
