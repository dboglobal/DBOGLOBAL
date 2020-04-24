#include "precomp_ntlsimulation.h"
#include "InputHandler.h"

CInputHandler::~CInputHandler()
{
}

CInputHandler* CInputHandler::GetInstance(void)
{
	static CInputHandler Instance;
	return &Instance;
}


void CInputHandler::UnLinkKeyDown(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkKeyDown");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listKeyDown.begin(); it != m_listKeyDown.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listKeyDown.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkKeyUp(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkKeyUp");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listKeyUp.begin(); it != m_listKeyUp.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listKeyUp.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkMouseDown(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkMouseDown");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseDown.begin(); it != m_listMouseDown.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listMouseDown.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkMouseUp(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkMouseUp");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseUp.begin(); it != m_listMouseUp.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listMouseUp.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkMouseMove(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkMouseMove");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseMove.begin(); it != m_listMouseMove.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listMouseMove.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkMouseWheel(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkMouseWheel");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseWheel.begin(); it != m_listMouseWheel.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listMouseWheel.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}



void CInputHandler::KeyDownHandler(unsigned int uiChar, unsigned int uiRepCount)
{
	NTL_FUNCTION("CInputHandler::KeyDownHandler");

	if ( !m_bActive ) return;

	SKeyData Key = { uiChar, uiRepCount};

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listKeyDown.begin(); it != m_listKeyDown.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<unsigned int>(&Key) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::KeyUpHandler(unsigned int uiChar, unsigned int uiRepCount)
{
	NTL_FUNCTION("CInputHandler::KeyUpHandler");

	if ( !m_bActive ) return;

	SKeyData Key = { uiChar, uiRepCount};

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listKeyUp.begin(); it != m_listKeyUp.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<unsigned int>(&Key) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::MouseDownHandler(unsigned int uiFlags, int nPosX, int nPosY, char chBtnType, bool bDblClick)
{
	NTL_FUNCTION("CInputHandler::MouseDownHandler");

	if ( !m_bActive ) return;

	SMouseData Mouse = { uiFlags, nPosX, nPosY, 0, chBtnType, bDblClick };

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseDown.begin(); it != m_listMouseDown.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<unsigned int>(&Mouse) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::MouseUpHandler(unsigned int uiFlags, int nPosX, int nPosY, char chBtnType, unsigned int uiDeltaTime)
{
	NTL_FUNCTION("CInputHandler::MouseUpHandler");

	if ( !m_bActive ) return;

	SMouseData Mouse = { uiFlags, nPosX, nPosY, 0, chBtnType, 0, uiDeltaTime };

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseUp.begin(); it != m_listMouseUp.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<unsigned int>(&Mouse) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::MouseMoveHandler(unsigned int uiFlags, int nPosX, int nPosY)
{
	NTL_FUNCTION("CInputHandler::MouseMoveHandler");

	if ( !m_bActive ) return;

	SMouseData Mouse = { uiFlags, nPosX, nPosY, 0, 0 };

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseMove.begin(); it != m_listMouseMove.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<unsigned int>(&Mouse) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::MouseWheelHandler(unsigned int uiFlags, short shDelta, int nPosX, int nPosY)
{
	NTL_FUNCTION("CInputHandler::MouseWheelHandler");

	if ( !m_bActive ) return;

	SMouseData Mouse = { uiFlags, nPosX, nPosY, shDelta, MOUSE_LBTN, 0 };

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseWheel.begin(); it != m_listMouseWheel.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<unsigned int>(&Mouse) );
	}

	NTL_RETURNVOID();
}