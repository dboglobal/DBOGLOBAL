#include "platform_common/Header.h"
#include "common/interface/Assert.h"
#include "cApplicationCallBacks.h"
#include "KeyBuffer.h"
#include "D3D.h"
#include "MouseDX8.h"
#include "cControlInterface.h"
#include "cGraphicsInterface.h"

cConfig::cConfig()
{
	useDInput = false;
	sizeX = 800;
	sizeY = 600;
}

cApplicationCallBacks::cApplicationCallBacks( HWND hWnd )
: hWnd( hWnd )
{

}

bool cApplicationCallBacks::isMinimised()
{
	return IsIconic( hWnd );
}

void cApplicationCallBacks::update()
{
	nKeyBuffer::clear();
	if(cConfig::GetInstance().useDInput)
	{
		tSigned32 dx, dy;
		GetMouseDeltaXY_DX8(dx, dy);
		cControlInterface::refInstance().setMouseDeltasTo(dx, -dy);
	}
	else
	{
		cControlInterface::refInstance().updateMouseDeltas();
	}

	cGraphicsInterface::refInstance().endFrame();
	cD3D::refInstance().endScene();
	cD3D::refInstance().beginScene();
}

void cApplicationCallBacks::update_DontBeginScene()
{
	nKeyBuffer::clear();
	if(cConfig::GetInstance().useDInput)
	{
		tSigned32 dx, dy;
		GetMouseDeltaXY_DX8(dx, dy);
		cControlInterface::refInstance().setMouseDeltasTo(dx, -dy);
	}
	else
	{
		cControlInterface::refInstance().updateMouseDeltas();
	}

	cGraphicsInterface::refInstance().endFrame();
	cD3D::refInstance().endScene();
}

void cApplicationCallBacks::update_Redraw()
{
	nKeyBuffer::clear();
	if(cConfig::GetInstance().useDInput)
	{
		tSigned32 dx, dy;
		GetMouseDeltaXY_DX8(dx, dy);
		cControlInterface::refInstance().setMouseDeltasTo(dx, -dy);
	}
	else
	{
		cControlInterface::refInstance().updateMouseDeltas();
	}

	cD3D::refInstance().present();
}

void cApplicationCallBacks::beginScene()
{
	cD3D::refInstance().beginScene();
}

void cApplicationCallBacks::exit()
{
	::exit(0);
}