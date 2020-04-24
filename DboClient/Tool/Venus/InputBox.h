/* **************************************************************************
 * @version $Id: InputBox.h,v 0.1 2006/03/31 02:26:00 legolas558 Exp $
 *
 * File:		InputBox.h
 * Content:		main include file for CInputBox class
 * Notes:		declares C or C++ class depending on the current language
 *
 * Copyright(c) 2006 by legolas558
 *
 * https://sourceforge.net/projects/cinputbox
 *
 * This software is released under a BSD license. See LICENSE.TXT
 * You must accept the license before using this software.
 *
 * **************************************************************************/

#pragma once

#include <windows.h>

#define	CIB_DECL		CInputBox::
#define	CIB_OBJ
#define	CIB_OBJ1
#define	CIB_DESTRUCTOR	~CInputBox
#define	CIB_CONSTRUCTOR	CInputBox
#define	DCIB_OBJ1(t)
#define	DCIB_OBJ(t)
#define	CIB_DEREF(t, code)	t->code
#define	DCIB_PUB	public:

class CInputBox
{
private:
    HWND				hwndParent,
        hwndInputBox,
        hwndQuery,
        hwndOk,
        hwndCancel,
        hwndEditBox;
    LPSTR				szInputText;
    WORD				wInputMaxLength, wInputLength;
    bool				bRegistered,
        bResult;

    HINSTANCE			hThisInstance;

    DCIB_PUB

        void	create(CIB_OBJ HWND hwndNew);
    void	show(CIB_OBJ LPCSTR lpszTitle, LPCSTR	lpszQuery);
    void	hide(CIB_OBJ1);
    void	submit(CIB_OBJ1);
    void	close(CIB_OBJ1);
    void	destroy(CIB_OBJ1);

    CIB_CONSTRUCTOR(CIB_OBJ HINSTANCE hInst);
    CIB_DESTRUCTOR(CIB_OBJ1);

    int ShowInputBox(CIB_OBJ HWND hParentWindow, LPCTSTR lpszTitle, LPCTSTR lpszQuery, LPSTR szResult, WORD wMax);
};

