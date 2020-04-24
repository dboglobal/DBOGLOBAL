//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "MouseDX8.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

LPDIRECTINPUT8       g_pDI    = 0;         
LPDIRECTINPUTDEVICE8 g_pMouse = 0;
     
HRESULT
InitMouseDX8(HWND hDlg)
{
    HRESULT hr;
    if(FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL)))
    {
        return hr;
    }
    if(FAILED(hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pMouse, NULL)))
    {
        return hr;
    }
    if(FAILED(hr = g_pMouse->SetDataFormat(&c_dfDIMouse2)))
    {
        return hr;
    }

    DWORD dwCoopFlags;
    dwCoopFlags = DISCL_NONEXCLUSIVE;
    dwCoopFlags |= DISCL_BACKGROUND;

    hr = g_pMouse->SetCooperativeLevel(hDlg, dwCoopFlags);
    if(FAILED(hr))
    {
        return hr;
    }

    // Acquire the newly created device
    hr = g_pMouse->Acquire();
    if(FAILED(hr))
    {
        return hr;
    }
    return S_OK;
}

void GetMouseDeltaXY_DX8(tSigned32& x, tSigned32& y)
{
    HRESULT hr;
    DIMOUSESTATE2 dims2;

    if( NULL == g_pMouse ) 
    {
        return;
    }
    
    ZeroMemory(&dims2, sizeof(dims2));
    hr = g_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &dims2);
    if(FAILED(hr)) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hr = g_pMouse->Acquire();
        while(hr == DIERR_INPUTLOST)
        {
            hr = g_pMouse->Acquire();
        }
        return;
    }
    
    x = dims2.lX;
    y = dims2.lY;
    return;
}
