#include "platform_common/Header.h"
#include "cEdgeRenderInfo.h"

#include "libs/TestBed_Application\D3D.h"
#include "interface/Singletons_TestBed.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "platform_common/PointMultiplication.h"
#include "cTestBed.h"

cEdgeRenderInfo::cEdgeRenderInfo()
: _vertexBuffer(NULL)
, _vertexCount(0)
{

}

cEdgeRenderInfo::~cEdgeRenderInfo()
{
	if(_vertexBuffer)
	{
		_vertexBuffer->Release();
	}
}

void cEdgeRenderInfo::PushVector3F( cVector3F& vector )
{
	_edgeElements.push_back( vector );
}

void cEdgeRenderInfo::BuildVertexBuffer()
{
	if( _vertexBuffer == NULL )
	{
		char buf[256];
		sprintf_s( buf, 256, "BufferSize : %d byte", sizeof(LINEVERTEX) * (int)_edgeElements.size() );
		/*nSingletons::pathEngineI().getErrorHandler()->handle(buf2, buf, NULL );*/

		HRESULT result = cD3D::device()->CreateVertexBuffer(
			sizeof(LINEVERTEX) * (int)_edgeElements.size(),
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
				sizeof(LINEVERTEX) * (int)_edgeElements.size(),
				&ptr,
				0); //... set some flags here
			assertD(result == D3D_OK);
		}

		LINEVERTEX* data = (LINEVERTEX*)ptr;

		_vertexCount = 0;
		for each( cVector3F vector3 in _edgeElements )
		{
			data[_vertexCount].position.x = vector3.x();
			data[_vertexCount].position.y = vector3.y();

			// OBstacle의 높이가 맞지 않는다.
			data[_vertexCount].position.z = vector3.z();
			_vertexCount++;
		}

		{
			HRESULT result = _vertexBuffer->Unlock();
			assertD(result == D3D_OK);
		}
	}
}

void cEdgeRenderInfo::Render()
{
	cD3D::refInstance().setRenderState(RS_LINE);
	cD3D::refInstance().setStreamSource(_vertexBuffer, sizeof(LINEVERTEX));
	cD3D::device()->DrawPrimitive(D3DPT_LINELIST, 0, _vertexCount/2);
}