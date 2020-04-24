/* **************************************************************************
* @version $Id: CInputBox.c,v 0.1 2006/03/31 02:26:00 legolas558 Exp $
*
* File:		CInputBoxTest.c
* Content:		main source code for CInputBox class
* Notes:		compiles with C/C++ language rules depending on the
Inputbox.h macro definitions
*
* Copyright(c) 2006 by legolas558
*
* https://sourceforge.net/projects/cinputbox
*
* This software is released under a BSD license. See LICENSE.TXT
* You must accept the license before using this software.
*
* **************************************************************************/

#include "stdafx.h"
#include "inputbox.h"

#define		CIB_CLASS_NAME "CInputBox558"
#define		CIB_SPAN 10
#define		CIB_LEFT_OFFSET 6
#define		CIB_TOP_OFFSET 4
#define		CIB_WIDTH 300
#define		CIB_HEIGHT 130
#define		CIB_BTN_WIDTH 60
#define		CIB_BTN_HEIGHT 20

LRESULT CALLBACK CIB_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CInputBox	*_this;
    _this = (CInputBox *)GetWindowLong(hWnd, GWL_USERDATA);

    switch (msg) {
        case WM_CREATE:
            _this = (CInputBox *) ((CREATESTRUCT *)lParam)->lpCreateParams;
            SetWindowLong(hWnd, GWL_USERDATA, (long)_this);
            CIB_DEREF(_this, create(DCIB_OBJ(_this) hWnd));
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
        case IDOK:
            CIB_DEREF(_this, submit(DCIB_OBJ1(_this)));
        case IDCANCEL:
            CIB_DEREF(_this, close(DCIB_OBJ1(_this)));
            break;
            }
            break;
        case WM_CLOSE:
            CIB_DEREF(_this, hide(DCIB_OBJ1(_this)));
            return 0;
        case WM_DESTROY:
            CIB_DEREF(_this, destroy(DCIB_OBJ1(_this)));
            break;
    }
    return(DefWindowProc (hWnd, msg, wParam, lParam));
}

CIB_DECL CIB_CONSTRUCTOR(CIB_OBJ HINSTANCE hInst)
{
    WNDCLASSEX			wndInputBox;
    RECT rect;

    memset(this, 0, sizeof(wndInputBox));

    this->hThisInstance = hInst;

    wndInputBox.cbSize					= sizeof(wndInputBox);
    wndInputBox.lpszClassName			= CIB_CLASS_NAME;
    wndInputBox.style					= CS_HREDRAW | CS_VREDRAW;
    wndInputBox.lpfnWndProc				= CIB_WndProc;
    wndInputBox.lpszMenuName			= NULL;
    wndInputBox.hIconSm					= NULL;
    wndInputBox.cbClsExtra				= 0;
    wndInputBox.cbWndExtra				= 0;
    wndInputBox.hInstance				= hInst;
    wndInputBox.hIcon					= LoadIcon(NULL, IDI_WINLOGO);
    wndInputBox.hCursor					= LoadCursor(NULL, IDC_ARROW);
    wndInputBox.hbrBackground			= (HBRUSH)(COLOR_WINDOW);

    RegisterClassEx(&wndInputBox);

    if (this->hwndParent)
        GetWindowRect(this->hwndParent, &rect);
    else
        GetWindowRect(GetDesktopWindow(), &rect);

    this->hwndInputBox = CreateWindow(	CIB_CLASS_NAME, "",
        (WS_BORDER | WS_CAPTION), rect.left+(rect.right-rect.left-CIB_WIDTH)/2,
        rect.top+(rect.bottom-rect.top-CIB_HEIGHT)/2,
        CIB_WIDTH, CIB_HEIGHT, this->hwndParent, NULL,
        this->hThisInstance, this);
}

void	CIB_DECL destroy(CIB_OBJ1) {
    EnableWindow(this->hwndParent, true);
    SendMessage(this->hwndInputBox, WM_DESTROY, 0, 0);
}

CIB_DECL CIB_DESTRUCTOR(CIB_OBJ1)  {
    UnregisterClass(CIB_CLASS_NAME, this->hThisInstance);
}

void	CIB_DECL submit(CIB_OBJ1)
{
    this->wInputLength = (int)SendMessage(this->hwndEditBox, EM_LINELENGTH, 0, 0);
    if (this->wInputLength) {
        *((LPWORD)this->szInputText) = this->wInputMaxLength;
        this->wInputLength = (WORD)SendMessage(this->hwndEditBox, EM_GETLINE, 0, (LPARAM)this->szInputText);
    }
    this->szInputText[this->wInputLength] = '\0';
    this->bResult = true;
}

void	CIB_DECL create(CIB_OBJ HWND hwndNew)
{
    HFONT	myFont;
    this->hwndInputBox = hwndNew;
    myFont = (HFONT )GetStockObject(DEFAULT_GUI_FONT);

    //	SetWindowPos(hwndInputBox, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    this->hwndQuery =	CreateWindow("Static", "", WS_CHILD | WS_VISIBLE,
        CIB_LEFT_OFFSET, CIB_TOP_OFFSET,
        CIB_WIDTH-CIB_LEFT_OFFSET*2, CIB_BTN_HEIGHT*2,
        this->hwndInputBox, NULL,
        this->hThisInstance, NULL);
    this->hwndEditBox = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT, CIB_LEFT_OFFSET,
        CIB_TOP_OFFSET + CIB_BTN_HEIGHT*2, CIB_WIDTH-CIB_LEFT_OFFSET*3, CIB_BTN_HEIGHT,
        this->hwndInputBox,	NULL,
        this->hThisInstance, NULL);
    this->hwndOk =		CreateWindow("Button", "OK", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        CIB_WIDTH/2 - CIB_SPAN*2 - CIB_BTN_WIDTH, CIB_HEIGHT - CIB_TOP_OFFSET*4 - CIB_BTN_HEIGHT*2,
        CIB_BTN_WIDTH, CIB_BTN_HEIGHT, this->hwndInputBox, (HMENU)IDOK,
        this->hThisInstance, NULL);
    this->hwndCancel =	CreateWindow("Button", "Cancel",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        CIB_WIDTH/2 + CIB_SPAN, CIB_HEIGHT - CIB_TOP_OFFSET*4 - CIB_BTN_HEIGHT*2,  CIB_BTN_WIDTH, CIB_BTN_HEIGHT,
        this->hwndInputBox, (HMENU)IDCANCEL,
        this->hThisInstance, NULL);

    //	SendMessage(this->hwndInputBox,WM_SETFONT,(WPARAM)myFont,FALSE);
    SendMessage(this->hwndQuery,WM_SETFONT,(WPARAM)myFont,FALSE);
    SendMessage(this->hwndEditBox,WM_SETFONT,(WPARAM)myFont,FALSE);
    SendMessage(this->hwndOk,WM_SETFONT,(WPARAM)myFont,FALSE);
    SendMessage(this->hwndCancel,WM_SETFONT,(WPARAM)myFont,FALSE);
}

void	CIB_DECL close(CIB_OBJ1) {
    PostMessage(this->hwndInputBox, WM_CLOSE, 0, 0);
}

void	CIB_DECL hide(CIB_OBJ1) {
    ShowWindow(this->hwndInputBox, SW_HIDE);
}

void	CIB_DECL show(CIB_OBJ LPCSTR lpszTitle, LPCSTR	lpszQuery) {
    SetWindowText(this->hwndInputBox, lpszTitle);
    SetWindowText(this->hwndEditBox, this->szInputText);
    SetWindowText(this->hwndQuery, lpszQuery);
    SendMessage(this->hwndEditBox, EM_LIMITTEXT, this->wInputMaxLength, 0);
    SendMessage(this->hwndEditBox, EM_SETSEL, 0, -1);
    SetFocus(this->hwndEditBox);
    ShowWindow(this->hwndInputBox, SW_NORMAL);
}

int CIB_DECL ShowInputBox(CIB_OBJ HWND hwndParentWindow, LPCTSTR lpszTitle, LPCTSTR lpszQuery, LPSTR szResult, WORD wMax)
{
    MSG	msg;
    BOOL	bRet;
    this->hwndParent = hwndParentWindow;
    this->szInputText = szResult;
    this->wInputMaxLength = wMax;

    this->bResult = false;

    //	EnableWindow(hwndParent, false);

    CIB_DEREF(this, show(DCIB_OBJ(this) lpszTitle, lpszQuery));

    while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
    {
        if (msg.message==WM_KEYDOWN) {
            switch (msg.wParam) {
            case VK_RETURN:
                CIB_DEREF(this, submit(DCIB_OBJ1(this)));
            case VK_ESCAPE:
                CIB_DEREF(this, close(DCIB_OBJ1(this)));
                break;
            default:
                TranslateMessage(&msg);
                break;
            }
        } else
            //		if (!IsDialogMessage(hwndInputBox, &msg)) {
            TranslateMessage(&msg);
        //		}
        DispatchMessage(&msg);	
        if (msg.message == WM_CLOSE)
            break;
    }

    //	EnableWindow(hwndParent, true);

    return this->bResult;
}
