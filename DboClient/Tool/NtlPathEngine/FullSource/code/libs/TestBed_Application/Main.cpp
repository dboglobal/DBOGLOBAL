//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "KeyBuffer.h"
#include "SplitArgs.h"
#include "MouseDX8.h"
#include "common/interface/Assert.h"
#include "D3D.h"
#include "cOSInterface.h"
#include "cControlInterface.h"
#include "cGraphicsInterface.h"
#include "interface/ClientEntryPoint.h"
#include "../libs/TestBed_Interface/Camera.h"
#include "PlaySounds_Init.h"
#include <string.h>
#include <windows.h>
#include <stdio.h>

class cConfig
{
public:

    bool useDInput;
    tSigned32 sizeX;
    tSigned32 sizeY;

    cConfig();
};
cConfig::cConfig()
{
    useDInput = true;
    sizeX = 1024;
    sizeY = 768;
}

static cConfig gConfig;

void
DoErrorExit(char* file, int line, char* comment)
{
    char buffer[800];
    sprintf(buffer,"%s(%d): Assert(%s).",file,line,comment);
    MessageBox (NULL, buffer, "Error in render dll.", MB_OK | MB_ICONSTOP);
    DebugBreak();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    switch(uMessage)
    {

    case WM_CREATE:
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
        
    case WM_ERASEBKGND:
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        
    case WM_LBUTTONDOWN:
        cControlInterface::refInstance().leftMouseDown();
        SetCapture(hWnd);
        nKeyBuffer::keyPressed(VK_LBUTTON);
        break;
    case WM_LBUTTONUP:
        cControlInterface::refInstance().leftMouseUp();
        ReleaseCapture();
        nKeyBuffer::keyReleased(VK_LBUTTON);
        break;

    case WM_MBUTTONDOWN:
        cControlInterface::refInstance().middleMouseDown();
        SetCapture(hWnd);
        nKeyBuffer::keyPressed(VK_MBUTTON);
        break;
    case WM_MBUTTONUP:
        cControlInterface::refInstance().middleMouseUp();
        ReleaseCapture();
        nKeyBuffer::keyReleased(VK_MBUTTON);
        break;

    case WM_RBUTTONDOWN:
        cControlInterface::refInstance().rightMouseDown();
        SetCapture(hWnd);
        nKeyBuffer::keyPressed(VK_RBUTTON);
        break;
    case WM_RBUTTONUP:
        cControlInterface::refInstance().rightMouseUp();
        ReleaseCapture();
        nKeyBuffer::keyReleased(VK_RBUTTON);
        break;

    case WM_MOUSEMOVE:
        cControlInterface::refInstance().setMousePosition(
                                        LOWORD(lParam),
                                        gConfig.sizeY - 1 - static_cast<tSigned32>(HIWORD(lParam)));
        break;

    case WM_KEYDOWN:
        nKeyBuffer::keyPressed(static_cast<short>(wParam));
        break;

    case WM_KEYUP:
        nKeyBuffer::keyReleased(static_cast<short>(wParam));
        break;
	
	/*case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);

			int nWidth = rect.right - rect.left;
			int nHeight = rect.bottom - rect.top;

			cOSInterface::refInstance().setWindowSize(nWidth, nHeight);
			cD3D::refInstance().resize(nWidth, nHeight);
		}
		break;*/

    default:        
        return DefWindowProc(hWnd, uMessage, wParam, lParam);

    }
    return 0;
}

static void
InitWindowClasses(HINSTANCE hInstance)
{
    WNDCLASS wc;    
    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC) WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hCursor          = LoadCursor (NULL, IDC_ARROW);
    wc.hIcon            = LoadIcon (NULL, IDI_APPLICATION);
    wc.hbrBackground    = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "ClassName";
    if(!RegisterClass(&wc))
    {
        MessageBox(NULL, "Failed to register window class.", "Error", MB_OK | MB_ICONSTOP);
        exit(EXIT_FAILURE);
    }
}

static HWND
CreateMainWindow(HINSTANCE hInstance, int width, int height, int showCommand)
{
    int xpos = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int ypos = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
    int captionHeight;
    int borderX, borderY;
    captionHeight = GetSystemMetrics(SM_CYCAPTION);
    borderX = GetSystemMetrics(SM_CXFIXEDFRAME);
    borderY = GetSystemMetrics(SM_CYFIXEDFRAME);
    HWND hWnd = CreateWindow("ClassName", "NTLNaviViewer", 
		/*WS_OVERLAPPEDWINDOW, */
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        xpos, ypos, width + borderX * 2, height + captionHeight + borderY * 2,
        0, 0, hInstance, 0);
    if(!hWnd)
    {
        MessageBox(NULL, "Failed to create window.", "Error", MB_OK | MB_ICONSTOP);
        exit(EXIT_FAILURE);
    }
    ShowWindow(hWnd, showCommand);
    return hWnd;
}

bool ProcessMessages()
{
    bool exitRequested = false;
    MSG msg;
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        if(msg.message == WM_QUIT)
        {
            exitRequested = true;
        }   
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return exitRequested;
}

class cApplicationCallBacks : public iApplicationCallBacks
{
    HWND hWnd;

public:

    cApplicationCallBacks(HWND hWnd) :
      hWnd(hWnd)
    {
    }

    bool isMinimised()
    {
        return IsIconic(hWnd) != 0;
    }

    void update()
    {
        nKeyBuffer::clear();
        bool exitRequested = ProcessMessages();
        if(exitRequested)
        {
            ::exit(0);
        }
        if(gConfig.useDInput)
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
    void update_DontBeginScene()
    {
        nKeyBuffer::clear();
        bool exitRequested = ProcessMessages();
        if(exitRequested)
        {
            ::exit(0);
        }
        if(gConfig.useDInput)
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
    void update_Redraw()
    {
        nKeyBuffer::clear();
        bool exitRequested = ProcessMessages();
        if(exitRequested)
        {
            ::exit(0);
        }
        if(gConfig.useDInput)
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
    void beginScene()
    {
        cD3D::refInstance().beginScene();
    }
     void exit()
    {
//        PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
        PostMessage(hWnd, WM_CLOSE, 0, 0);
        while(!ProcessMessages())
        {
        }
        ::exit(0);
    }
};

static bool
ProcessArgument(char* arg, cConfig& config)
{
    if(_stricmp(arg,"notdinput")==0)
    {
        config.useDInput = false;
        return true;
    }
    if(_strnicmp(arg, "windowsize=", 11) == 0)
    {
        char* ptr;
        config.sizeX = strtol(arg + 11, &ptr, 10);
        assertD(*ptr == ',');
        config.sizeY = strtol(ptr + 1, &ptr, 10);
        assertD(*ptr == 0);
        return true;
    }
    return false;
}

int PASCAL
WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
    LPTSTR cmdLine, int showCommand)
{
    char** argV;
    int argC;
    SplitArgs(cmdLine, argV, argC);

    char** nextArgV = argV;
    int argsLeft = argC;
    while(argsLeft && ProcessArgument(*nextArgV, gConfig))
    {
        argsLeft--;
        nextArgV++;
    }

    InitWindowClasses(hInstance);
    HWND hWnd = CreateMainWindow(hInstance, gConfig.sizeX, gConfig.sizeY, showCommand);

	{
		HRESULT result = cD3D::refInstance().init(hWnd, gConfig.sizeX, gConfig.sizeY);
		if(result != S_OK)
		{
			_exit(1);
		}
	}
	cD3D::refInstance().beginScene();
    InitDSound(hWnd);

    if(gConfig.useDInput)
    {
        HRESULT result = InitMouseDX8(hWnd);
        if(result != S_OK)
        {
            MessageBox(0, "Failed to setup direct input.", "Error", MB_OK | MB_ICONSTOP);
            exit(EXIT_FAILURE);
        }
    }

    ProcessMessages();

    cOSInterface::refInstance().setWindowSize(gConfig.sizeX, gConfig.sizeY);
    cApplicationCallBacks callBacks(hWnd);
    cOSInterface::refInstance().setApplicationCallBacks(&callBacks);

    ClientEntryPoint(&cOSInterface::refInstance(), argsLeft, nextArgV, hWnd);

//    PostMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
    PostMessage(hWnd, WM_CLOSE, 0, 0);
    while(!ProcessMessages())
    {
    }

    cGraphicsInterface::refInstance().endFrame();
    cD3D::refInstance().endScene();
    cD3D::refInstance().cleanup();
    ShutDownDSound();

    DeleteArgs(argV, argC);
    return 0;
}

