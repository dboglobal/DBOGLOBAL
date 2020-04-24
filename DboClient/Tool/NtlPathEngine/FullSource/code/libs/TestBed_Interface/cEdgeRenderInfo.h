//**********************************************************************
//
// Copyright (c) 2002-2006
// NTL TestBed
// Haesung, Cho
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EDGE_RENDER_INFO_INCLUDED
#define EDGE_RENDER_INFO_INCLUDED

#include "libs/Geometry/interface/Vector3F.h"
#include "common/STL/vector.h"
#include "common/interface/Assert.h"
#include "libs/TestBed_Application/VertexTypes.h"

class iGraphicsInterface;

class cEdgeRenderInfo
{
public:
	cEdgeRenderInfo();
	virtual ~cEdgeRenderInfo();

	void PushVector3F( cVector3F& vector );

	void BuildVertexBuffer();

	void Render();

protected:
	std::vector<cVector3F> _edgeElements;
	IDirect3DVertexBuffer9* _vertexBuffer;
	unsigned int			_vertexCount;
};

#endif
