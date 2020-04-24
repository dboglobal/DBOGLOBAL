/*****************************************************************************
 *
 * File			: InputHandler.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 26	
 * Abstract		: Input handler 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

#include "NtlCallbackWidget.h"
#include "NtlDebug.h"
#include "NtlActionMapDef.h"

typedef void* INPUT_HANDLE;

enum EMouseBtn
{
	MOUSE_LBTN,
	MOUSE_MBTN,
	MOUSE_RBTN
};

enum EMouseFlag
{
	MF_LBUTTON = 0x0001,		// mouse left button
	MF_RBUTTON = 0x0002,		// mouse right button
	MF_SHIFT = 0x0004,			// keyboard shift key
	MF_CONTROL = 0x0008,		// keyboard control key
	MF_MBUTTON = 0x0010			// mouse middle button
};

struct SKeyData
{
	unsigned int uiChar;
	unsigned int uiRepCount;
};

struct SMouseData
{
	unsigned int uiFlags;
	int iPosX;
	int iPosY;
	short zDelta;
	char chBtnType;
	bool bDblClick;
	unsigned int uiDownDeltaTime;
};

class CInputHandler
{
private:

	std::list<CNtlCallbackParam1*> m_listKeyDown;
	std::list<CNtlCallbackParam1*> m_listKeyUp;
	std::list<CNtlCallbackParam1*> m_listMouseDown;
	std::list<CNtlCallbackParam1*> m_listMouseUp;
	std::list<CNtlCallbackParam1*> m_listMouseMove;
	std::list<CNtlCallbackParam1*> m_listMouseWheel;

	RwBool m_bActive;

public:

	CInputHandler() { m_bActive = TRUE; }
	~CInputHandler();

	static CInputHandler* GetInstance(void);

	RwBool IsActive( void )
	{
		return m_bActive;
	}

	void SetActive( RwBool bActive )
	{
		m_bActive = bActive;
	}


	template <class Callbackclass>	
	INPUT_HANDLE LinkKeyDown(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiKeyData))
	{
		CNtlCallbackParam1 *pWidget = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
		m_listKeyDown.push_back(pWidget);
		return pWidget;
	}
    
	template <class Callbackclass>	
	INPUT_HANDLE LinkKeyUp(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiKeyData))
	{
		CNtlCallbackParam1 *pWidget = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
		m_listKeyUp.push_back(pWidget);
		return pWidget;
	}

	template <class Callbackclass>	
	INPUT_HANDLE LinkMouseDown(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMouseData))
	{
		CNtlCallbackParam1 *pWidget = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
		m_listMouseDown.push_back(pWidget);
		return pWidget;
	}

	template <class Callbackclass>	
	INPUT_HANDLE LinkMouseUp(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMouseData))
	{
		CNtlCallbackParam1 *pWidget = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
		m_listMouseUp.push_back(pWidget);
		return pWidget;
	}

	template <class Callbackclass>	
	INPUT_HANDLE LinkMouseMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMouseData))
	{
		CNtlCallbackParam1 *pWidget = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
		m_listMouseMove.push_back(pWidget);
		return pWidget;
	}

	template <class Callbackclass>
	INPUT_HANDLE LinkMouseWheel(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMouseData))
	{
		CNtlCallbackParam1 *pWidget = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
		m_listMouseWheel.push_back(pWidget);
		return pWidget;
	}

	void UnLinkKeyDown(INPUT_HANDLE handle);
	void UnLinkKeyUp(INPUT_HANDLE handle);
	void UnLinkMouseDown(INPUT_HANDLE handle);
	void UnLinkMouseUp(INPUT_HANDLE handle);
	void UnLinkMouseMove(INPUT_HANDLE handle);
	void UnLinkMouseWheel(INPUT_HANDLE handle);

	void KeyDownHandler(unsigned int uiChar, unsigned int uiRepCount);
	void KeyUpHandler(unsigned int uiChar, unsigned int uiRepCount);
	void MouseDownHandler(unsigned int uiFlags, int nPosX, int nPosY, char chBtnType, bool bDblClick);
	void MouseUpHandler(unsigned int uiFlags, int nPosX, int nPosY, char chBtnType, unsigned int uiDeltaTime);
	void MouseMoveHandler(unsigned int uiFlags, int nPosX, int nPosY);
	void MouseWheelHandler(unsigned int uiFlags, short shDelta, int nPosX, int nPosY);
};

#endif