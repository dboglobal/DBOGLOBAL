#include "platform_common/Header.h"
#include "cObstacleRenderInfo.h"

#include "libs/TestBed_Application\D3D.h"
#include "interface/Singletons_TestBed.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "platform_common/PointMultiplication.h"
#include "cTestBed.h"

static cVector2F
ApproximateIntersectionObs(const tIntersection& i)
{
	tPoint::tDistance n, d;
	i.getAxisFraction(n, d);
	cVector2F result = i.refLine().axis();
	result *= tMultiplier::asFloat(n);
	result /= tMultiplier::asFloat(d);
	result += i.refLine().start();
	return result;
}
static cVector2F
ApproximateIntersectionObs(const tLine& line1, const tLine& line2)
{
	return ApproximateIntersectionObs(tIntersection(line1, line2));
}

cObstacleRenderInfo::cObstacleRenderInfo()
: _vertexBuffer(NULL)
, _vertexCount(0)
{

}

cObstacleRenderInfo::~cObstacleRenderInfo()
{
	if(_vertexBuffer)
	{
		_vertexBuffer->Release();
	}
}

void cObstacleRenderInfo::PushVector3F( cVector3F& vector )
{
	_obstacleElements.push_back( vector );
}

void cObstacleRenderInfo::Build( const tMeshCircuits& circuits, iGraphicsInterface *gip, const cTranslationToOverlapped& translation )
{
	tMeshCircuits::const_iterator i;
	for( i = circuits.begin(); i != circuits.end(); ++i )
	{
		BuildMeshCurSequence( **i, gip, translation );
	}
}

void cObstacleRenderInfo::BuildMeshCurSequence( const tMeshCutSequence& cut_sequence, iGraphicsInterface *gip, const cTranslationToOverlapped& translation )
{
	tMeshCutSequence::const_iterator i;
	for( i = cut_sequence.begin(); i != cut_sequence.end(); ++i )
	{
		BuildMeshCut( **i, gip, translation );
	}
}

void cObstacleRenderInfo::BuildMeshCut( const cMeshCut& cut, iGraphicsInterface *gip, const cTranslationToOverlapped& translation )
{
	const cCircuitElement* e = cut.getStartElement();
	tFace_3D startFace;
	cVector2F approximateStartPoint;
	startFace.clear();
	do 
	{
		BuildCircuitElement_SI( gip, translation, *e, startFace, approximateStartPoint );
		e = e->getNext();
	}
	while( e && e != cut.getStartElement() );
}

void cObstacleRenderInfo::BuildCircuitElement_SI( iGraphicsInterface* gip, const cTranslationToOverlapped& translation, const cCircuitElement& el, tFace_3D& startFace, cVector2F& approximateStartPoint )
{
	if(!startFace.valid())
	{
		if(el.enters())
		{
			assertD(!startFace.valid());
			tEdge entryEdge = el.entryEdge();
			startFace = translation.translateEntry(entryEdge, el.refLine()).face();
			approximateStartPoint = ApproximateIntersectionObs(el.refLine(), GetEdgeLine(entryEdge));
		}
		else
		{
			tIntersection intersection;
			el.cornerIntersection(intersection);
			startFace = translation.translate(el.faceCrossed(0), intersection);
			approximateStartPoint = ApproximateIntersectionObs(intersection);
		}
	}

	tFace_3D endFace;
	cVector2F approximateEndPoint;
	if(el.exits())
	{
		tEdge exitEdge = el.exitEdge();
		endFace = translation.translateEntry(exitEdge, el.refLine().reversed()).face();
		approximateEndPoint = ApproximateIntersectionObs(el.refLine(), GetEdgeLine(exitEdge));
	}
	else
	{
		tIntersection intersection;
		el.nextCornerIntersection(intersection);
		cIntersection_PointLike pl(intersection);
		endFace = translation.translate(el.lastFaceCrossed(), pl);
		approximateEndPoint = ApproximateIntersectionObs(intersection);
	}

	while(startFace != endFace)
	{
		tEdge_3D crossed = NextEdgeCrossed_SI(tMesh_3D::beginEdge(startFace), el.refLine());
		cVector2F p = ApproximateIntersectionObs(el.refLine(), GetEdgeLine(crossed));
		BuildLineInFace(gip, startFace, approximateStartPoint, p);
		startFace = crossed.twin().face();
		assertD(!startFace->isExternal());
		approximateStartPoint = p;
	}

	BuildLineInFace(gip, startFace, approximateStartPoint, approximateEndPoint);
	approximateStartPoint = approximateEndPoint;
	startFace = endFace;
}

void cObstacleRenderInfo::BuildLineInFace( iGraphicsInterface* gip, tFace_3D f, const cVector2F& p1, const cVector2F& p2 )
{
	float z1 = nSingletons::testBed().refMesh().heightAtPointIn3DFace(f.index(), p1.x, p1.y);
	float z2 = nSingletons::testBed().refMesh().heightAtPointIn3DFace(f.index(), p2.x, p2.y);
	PushVector3F( cVector3F(p1.x, p1.y, z1) );
	PushVector3F( cVector3F(p2.x, p2.y, z2) );
}

void cObstacleRenderInfo::BuildVertexBuffer()
{
	if( _vertexBuffer == NULL )
	{
		char buf[256];
		sprintf_s( buf, 256, "BufferSize : %d byte", sizeof(LINEVERTEX) * (int)_obstacleElements.size() );
		/*nSingletons::pathEngineI().getErrorHandler()->handle(buf2, buf, NULL );*/

		HRESULT result = cD3D::device()->CreateVertexBuffer(
			sizeof(LINEVERTEX) * (int)_obstacleElements.size(),
			D3DUSAGE_DYNAMIC,
			D3DFVF_LINEVERTEX,
			D3DPOOL_DEFAULT,
			&_vertexBuffer,
			0
			);

		if(result != D3D_OK)
		{
			assertD(result == E_OUTOFMEMORY);
			nSingletons::pathEngineI().getErrorHandler()->handle(buf, "Create Vertec Buffer Failed", NULL );
			_vertexBuffer = 0;
			return;
		}

		void* ptr;
		{
			HRESULT result =
				_vertexBuffer->Lock(
				0,
				sizeof(LINEVERTEX) * (int)_obstacleElements.size(),
				&ptr,
				0); //... set some flags here
			assertD(result == D3D_OK);
		}

		LINEVERTEX* data = (LINEVERTEX*)ptr;

		_vertexCount = 0;
		for each( cVector3F vector3 in _obstacleElements )
		{
			data[_vertexCount].position.x = vector3.x();
			data[_vertexCount].position.y = vector3.y();

			// OBstacle의 높이가 맞지 않는다.
			data[_vertexCount].position.z = -vector3.z();
			_vertexCount++;
		}

		{
			HRESULT result = _vertexBuffer->Unlock();
			assertD(result == D3D_OK);
		}
	}
}

void cObstacleRenderInfo::Render()
{
	cD3D::refInstance().setRenderState(RS_LINE);
	cD3D::refInstance().setStreamSource(_vertexBuffer, sizeof(LINEVERTEX));
	cD3D::device()->DrawPrimitive(D3DPT_LINELIST, 0, _vertexCount/2);
}