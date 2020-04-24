/******************************************************************************/
/* NAME              : SkinProgress.cpp                                       */
/* PURPOSE           : Place a bitmapped progress bar into the status bar     */
/* CREATED_BY        : Kochise (kochise@caramail.com)                         */
/* CREATION_DATE     : $C_DATE=YYYY/MM/DD$ 22003/05/02                        */
/* MODIFIED_BY       : Kochise (kochise@caramail.com)                         */
/* MODIFICATION_DATE : $M_DATE=YYYY/MM/DD$ 2003/07/12                         */
/* LANGUAGE          : MFC Visual C++ 6                                       */
/* MPROC_TYPE        : Microsoft Windows compatible computer                  */
/*                                                                            */
/*
$Log: E:\Projects\NoeZooArc\SkinProgressTest.zoo\SkinProgressTest\SkinProgress.cpp.adiff $
*
*   Rev 1.02 : July 12 2003 14:01:53 (Kochise)
* Base : CSkinProgress 1.32dev
*   extracting files : no error
*   parsing files : no error
*   diff engine : no error
*   modify.public.constructor:CSkinProgress
*   touch.protected.function:CreateCommon
*   modify.protected.function:ProgressInStatusBar
*   modify.protected.function:ProgressInDialog
*   modify.public.function:SetRange
*   touch.public.function:SetPos
*   modify.public.function:GetPos
*   touch.public.function:SetStart
*   modify.public.function:GetStart
*   modify.public.function:Reset
*   add.public.function:SetProgress
*   add.public.function:GetProgress
*   modify.public.function:RefreshPanes
*   touch.protected.function:CompactText
*   modify.protected.function:GetTimed
*   modify.protected.function:UpdateProgress
*   modify.protected.function:OnPaint
*   compiling project files : no error
*   packing files : no error
* http://www.codeproject.com/miscctrl/CSkinProgress.asp
*
*   Rev 1.01 : July 06 2003 22:08:37 (Kochise)
* Base : CSkinProgress 1.28 Devel
*   extracting files : no error
*   parsing files : no error
*   diff engine : no error
*   modify.public.constructor:CSkinProgress
*   add.public.constructor:CSkinProgress
*   modify.public.function:SetBitmap
*   modify.public.function:ConvBitmap
*   modify.public.function:SetRange
*   modify.public.function:SetPos
*   modify.public.function:OffsetPos
*   modify.public.function:StepIt
*   modify.public.function:SetSize
*   add.public.function:GetSize
*   add.public.function:GetPos
*   add.public.function:GetStep
*   add.public.function:GetLower
*   add.public.function:GetUpper
*   add.public.function:SetStart
*   add.public.function:GetStart
*   add.public.function:Reset
*   modify.public.function:SetText
*   mutate.function:protected.ResizeTextPane->public.RefreshPanes
*   modify.public.function:RefreshPanes
*   mutate.function:public.Clear->protected.Clear
*   add.protected.function:GetTargetRect
*   mutate.protected.function:Create->CreateCommon
*   modify.protected.function:CreateCommon
*   add.protected.function:ProgressInStatusBar
*   add.protected.function:ProgressInDialog
*   modify.protected.function:CompactText
*   add.protected.function:UpdateProgress
*   modify.protected.function:OnPaint
*   compiling project files : no error
*   packing files : no error
* http://www.codeproject.com/miscctrl/CSkinProgress.asp
*
*   Rev 1.00 : May 25 2003 13:25:12 (Kochise)
* First archived version and released onto CodeProject.
* Base : CSkinProgress 1.12 Devel
*   add.public.constructor:CSkinProgress
*   add.public.constructor:CSkinProgress
*   add.public.function:SetRange
*   add.public.function:SetText
*   add.public.function:SetSize
*   add.public.function:SetBitmap
*   add.public.function:CopyBitmap
*   add.public.function:ConvBitmap
*   add.public.function:SetPos
*   add.public.function:OffsetPos
*   add.public.function:SetStep
*   add.public.function:StepIt
*   add.public.function:Clear
*   add.public.destructor:~CSkinProgress
*   add.protected.function:Create
*   add.protected.function:GetStatusBar
*   add.protected.function:ResizeTextPane
*   add.protected.function:CompactText
*   add.protected.function:GetTimed
*   add.protected.function:OnEraseBkgnd
*   add.protected.function:OnPaint
*   add.protected.function:OnSizing
*   add.protected.function:OnSize
*   add.protected.function:OnTimer
*   compiling project files : no error
*   packing files : no error
* http://www.codeproject.com/useritems/CSkinProgress.asp
*
*/
/******************************************************************************/

// SkinProgress.cpp : implementation file
//

#include "stdafx.h"
#include "SkinProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinProgress

// g_Global      : g_ = Global
// i_Input       : i_ = Input
// no z...
// m_Member      : m_ = Member
// o_Output      : o_ = Output
// ...           : use your imagination

// aArray        : a  = Array    (array)
// bBool         : b  = Boolean  (boolean, true/false)
// cConstant     : c  = Constant (constant, whatever...)
// dDefine       : d  = Define   (simple define or defined value)
// nNumber       : n  = Number   (char, long, int, whatever...)
// oObject       : o  = Object   (C++ class)
// pPointer      : p  = Pointer  (typicaly a 32 bits ulong address)
// sStruct       : s  = Struct   (structure)
// uUnion        : u  = Union    (join two or more values of the same size under a common name)

// poRemoteClass : po = Pointer on Object
// cdConstantDef : cd = Constant Define, typicaly a constant defined value
// usUnionStruct : us = Union of Structures
// ...           : use your imagination

// o_psBitmap    : CBitmap::GetBitmap((BITMAP*) o_psBitmap); // s = BITMAP, p = *, o_ means it's an output

// <Value>
// [Function]

// TYPICAL LIFE TIME
// 
// CSkinProgress(...) -------> StepIt() ----------------------------> ~CSkinProgress()   USER
//   |                           |                                       |               SIDE
// ..|...........................|.......................................|.........................
//   |                           |                                       |
//   '-> Create(...)             |                                       '-> Clear()     OBJECT
//         |                     |                                                       SIDE
//         +-> GetStatusBar()    |
//         +-> SetRange(...)     |
//         +-> SetBitmap(...)    '----------->
//         '---------------------------------> RefreshPanes() --------,
//         ,--------------------------------->   |                    |
//         |                               ,->   |                    |
//         |                               |     |                    |
//         |                               |     +-> GetStatusBar()   |
//         |                               |     +-> GetTimed(...)    |
//         |                               |     '-> CompactText(...) |
// ........|...............................|..........................|............................
//         |                               |                          |                  WINDOWS
//       OnSizing(...) -> OnSize(...)    OnEraseBkgnd(...)            '-> OnPaint()      SIDE

#ifndef dCSP_RESOURCE_BITMAP
/*** anAQUA_BITMAP ************************************************************/
/* Purpose : The default embedded progress bar image, if resource bitmap not  */
/*           used instead.                                                    */
/* Unit    : unsigned char                                                    */
/* Range   : [0-255] - LIMITED selection                                      */
/* List    : None                                                             */
/* Example : None                                                             */
/******************************************************************************/
unsigned char anAQUA_BITMAP[5760] =
{ // B,    G,    R,    A -> Set to 0xFF for CAlphaBlendBitmap
	0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xAC, 0x9C, 0x9B, 0xFF, 0x9F, 0x60, 0x5A, 0xFF, 0x9E, 0x38, 0x2B, 0xFF,
		0xA2, 0x22, 0x00, 0xFF, 0xAA, 0x22, 0x00, 0xFF, 0xAB, 0x2B, 0x00, 0xFF, 0xAB, 0x2B, 0x00, 0xFF,
		0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
		0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
		0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
		0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
		0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF,
		0xAB, 0x2B, 0x00, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xAB, 0x38, 0x00, 0xFF, 0xA4, 0x46, 0x27, 0xFF,
		0x9E, 0x65, 0x54, 0xFF, 0x99, 0x89, 0x86, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xA9, 0x8A, 0x86, 0xFF, 0x9F, 0x40, 0x2E, 0xFF,
		0xA5, 0x1C, 0x00, 0xFF, 0xB1, 0x2E, 0x00, 0xFF, 0xC8, 0x6D, 0x42, 0xFF, 0xD8, 0x91, 0x67, 0xFF,
		0xDB, 0x9D, 0x76, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
		0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
		0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
		0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
		0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF,
		0xDD, 0xA2, 0x75, 0xFF, 0xDD, 0xA2, 0x75, 0xFF, 0xDB, 0x9D, 0x76, 0xFF, 0xD9, 0x94, 0x6D, 0xFF,
		0xC8, 0x6D, 0x42, 0xFF, 0xB1, 0x33, 0x00, 0xFF, 0xA5, 0x1C, 0x00, 0xFF, 0x97, 0x3D, 0x22, 0xFF,
		0x8E, 0x6F, 0x6B, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xB4, 0xA5, 0xA2, 0xFF,
		0xA1, 0x46, 0x2E, 0xFF, 0xAB, 0x2B, 0x00, 0xFF, 0xAF, 0x33, 0x00, 0xFF, 0xCE, 0x76, 0x42, 0xFF,
		0xE0, 0xAB, 0x81, 0xFF, 0xE5, 0xB5, 0x89, 0xFF, 0xE7, 0xBC, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE7, 0xBC, 0x95, 0xFF, 0xE5, 0xB5, 0x89, 0xFF, 0xE3, 0xAF, 0x88, 0xFF, 0xD0, 0x7F, 0x4C, 0xFF,
		0xB1, 0x33, 0x00, 0xFF, 0xAB, 0x2B, 0x00, 0xFF, 0x97, 0x3D, 0x22, 0xFF, 0x88, 0x78, 0x76, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xA6, 0x6E, 0x5A, 0xFF, 0xAF, 0x33, 0x00, 0xFF, 0xB8, 0x3D, 0x00, 0xFF,
		0xBB, 0x45, 0x00, 0xFF, 0xD4, 0x87, 0x44, 0xFF, 0xE4, 0xAD, 0x7B, 0xFF, 0xE5, 0xB3, 0x84, 0xFF,
		0xE5, 0xB5, 0x89, 0xFF, 0xE6, 0xB8, 0x8C, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF,
		0xE9, 0xBD, 0x95, 0xFF, 0xE9, 0xBD, 0x95, 0xFF, 0xE6, 0xB8, 0x8C, 0xFF, 0xE5, 0xB3, 0x84, 0xFF,
		0xE4, 0xAD, 0x7B, 0xFF, 0xD8, 0x8E, 0x4E, 0xFF, 0xBB, 0x45, 0x00, 0xFF, 0xB8, 0x3D, 0x00, 0xFF,
		0xB1, 0x33, 0x00, 0xFF, 0x89, 0x50, 0x3D, 0xFF, 0x99, 0x99, 0x99, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF,
		0xCD, 0xCD, 0xCD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xA6, 0x50, 0x27, 0xFF,
		0xBB, 0x45, 0x00, 0xFF, 0xBF, 0x4E, 0x00, 0xFF, 0xC1, 0x54, 0x00, 0xFF, 0xD4, 0x7A, 0x2E, 0xFF,
		0xE1, 0xA2, 0x6A, 0xFF, 0xE5, 0xAC, 0x77, 0xFF, 0xE7, 0xB1, 0x7F, 0xFF, 0xE9, 0xB4, 0x85, 0xFF,
		0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
		0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
		0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
		0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
		0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF, 0xE9, 0xB6, 0x88, 0xFF,
		0xE5, 0xB3, 0x84, 0xFF, 0xE4, 0xAD, 0x7B, 0xFF, 0xE1, 0xA6, 0x72, 0xFF, 0xD4, 0x80, 0x38, 0xFF,
		0xC1, 0x54, 0x00, 0xFF, 0xBF, 0x4E, 0x00, 0xFF, 0xBB, 0x45, 0x00, 0xFF, 0x9B, 0x46, 0x1C, 0xFF,
		0x82, 0x82, 0x82, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xB6, 0x4C, 0x00, 0xFF, 0xC1, 0x54, 0x00, 0xFF, 0xC5, 0x5C, 0x00, 0xFF,
		0xCA, 0x63, 0x15, 0xFF, 0xD4, 0x73, 0x22, 0xFF, 0xD9, 0x7D, 0x2E, 0xFF, 0xDA, 0x84, 0x3B, 0xFF,
		0xDF, 0x8A, 0x42, 0xFF, 0xE0, 0x8D, 0x48, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE0, 0x8D, 0x48, 0xFF, 0xDA, 0x84, 0x3B, 0xFF,
		0xD4, 0x7A, 0x2E, 0xFF, 0xCE, 0x6F, 0x22, 0xFF, 0xC5, 0x61, 0x15, 0xFF, 0xC5, 0x5C, 0x00, 0xFF,
		0xB8, 0x55, 0x00, 0xFF, 0xB6, 0x4C, 0x00, 0xFF, 0x73, 0x73, 0x73, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xD5, 0xD5, 0xD5, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xB6, 0x4C, 0x00, 0xFF,
		0xC1, 0x54, 0x00, 0xFF, 0xC5, 0x5C, 0x00, 0xFF, 0xCA, 0x63, 0x15, 0xFF, 0xD4, 0x73, 0x22, 0xFF,
		0xD9, 0x7D, 0x2E, 0xFF, 0xDA, 0x84, 0x3B, 0xFF, 0xDF, 0x8A, 0x42, 0xFF, 0xE0, 0x8D, 0x48, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xE0, 0x8D, 0x48, 0xFF, 0xDA, 0x84, 0x3B, 0xFF, 0xD4, 0x7A, 0x2E, 0xFF, 0xCE, 0x6F, 0x22, 0xFF,
		0xC5, 0x61, 0x15, 0xFF, 0xC5, 0x5C, 0x00, 0xFF, 0xB8, 0x55, 0x00, 0xFF, 0xB6, 0x4C, 0x00, 0xFF,
		0x73, 0x73, 0x73, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xB5, 0xB5, 0xB5, 0xFF, 0xB8, 0x55, 0x00, 0xFF, 0xC3, 0x5F, 0x15, 0xFF, 0xCC, 0x6A, 0x1C, 0xFF,
		0xD4, 0x73, 0x22, 0xFF, 0xDE, 0x82, 0x35, 0xFF, 0xE3, 0x8C, 0x42, 0xFF, 0xEA, 0x96, 0x4D, 0xFF,
		0xEB, 0x9B, 0x55, 0xFF, 0xED, 0x9E, 0x59, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
		0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
		0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
		0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
		0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF,
		0xF2, 0xA6, 0x61, 0xFF, 0xF2, 0xA6, 0x61, 0xFF, 0xED, 0x9E, 0x59, 0xFF, 0xE5, 0x94, 0x4D, 0xFF,
		0xDF, 0x8A, 0x42, 0xFF, 0xD4, 0x7C, 0x33, 0xFF, 0xCE, 0x72, 0x22, 0xFF, 0xCC, 0x6A, 0x1C, 0xFF,
		0xC3, 0x5F, 0x15, 0xFF, 0xB4, 0x54, 0x00, 0xFF, 0x67, 0x67, 0x67, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF,
		0xDD, 0xDD, 0xDD, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0xA9, 0x62, 0x2B, 0xFF,
		0xCC, 0x6A, 0x1C, 0xFF, 0xD4, 0x73, 0x22, 0xFF, 0xDA, 0x7F, 0x35, 0xFF, 0xE4, 0x90, 0x46, 0xFF,
		0xF1, 0x9D, 0x54, 0xFF, 0xF8, 0xA7, 0x60, 0xFF, 0xFB, 0xAC, 0x66, 0xFF, 0xFE, 0xB2, 0x6D, 0xFF,
		0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
		0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
		0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
		0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
		0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF, 0xFE, 0xB5, 0x74, 0xFF,
		0xFA, 0xAF, 0x6E, 0xFF, 0xEE, 0xA2, 0x5D, 0xFF, 0xE9, 0x96, 0x50, 0xFF, 0xDF, 0x8A, 0x42, 0xFF,
		0xD4, 0x7C, 0x33, 0xFF, 0xCE, 0x72, 0x22, 0xFF, 0xC7, 0x69, 0x1C, 0xFF, 0xA0, 0x59, 0x22, 0xFF,
		0x69, 0x69, 0x69, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0x9C, 0x73, 0x50, 0xFF, 0xCE, 0x72, 0x22, 0xFF, 0xD4, 0x7C, 0x33, 0xFF,
		0xE1, 0x89, 0x3D, 0xFF, 0xF1, 0x9D, 0x54, 0xFF, 0xFB, 0xAC, 0x66, 0xFF, 0xFE, 0xB2, 0x6D, 0xFF,
		0xFF, 0xBC, 0x74, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
		0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
		0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
		0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
		0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF,
		0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xFE, 0xB2, 0x6D, 0xFF,
		0xF5, 0xA5, 0x5E, 0xFF, 0xEA, 0x96, 0x4D, 0xFF, 0xDA, 0x84, 0x3B, 0xFF, 0xD4, 0x7C, 0x33, 0xFF,
		0xCE, 0x72, 0x22, 0xFF, 0x83, 0x5A, 0x38, 0xFF, 0x82, 0x82, 0x82, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0x9B, 0x91, 0x88, 0xFF,
		0xA3, 0x6A, 0x3B, 0xFF, 0xD4, 0x80, 0x38, 0xFF, 0xE4, 0x90, 0x46, 0xFF, 0xFB, 0xA6, 0x5A, 0xFF,
		0xFF, 0xB8, 0x6E, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xFF, 0xD0, 0x87, 0xFF, 0xFF, 0xCD, 0x8D, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xCD, 0x8D, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xFE, 0xB2, 0x6D, 0xFF, 0xF1, 0xA0, 0x57, 0xFF,
		0xE3, 0x8C, 0x42, 0xFF, 0xD4, 0x80, 0x38, 0xFF, 0x9E, 0x63, 0x33, 0xFF, 0x66, 0x5C, 0x54, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF, 0x8E, 0x7B, 0x6D, 0xFF, 0xA8, 0x6F, 0x40, 0xFF,
		0xE4, 0x90, 0x46, 0xFF, 0xF9, 0xA8, 0x5F, 0xFF, 0xFF, 0xBC, 0x74, 0xFF, 0xFF, 0xCC, 0x82, 0xFF,
		0xFF, 0xD5, 0x8C, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
		0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
		0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
		0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
		0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF,
		0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xE6, 0x9F, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xCC, 0x82, 0xFF,
		0xFF, 0xBC, 0x74, 0xFF, 0xF5, 0xA5, 0x5E, 0xFF, 0xE4, 0x90, 0x46, 0xFF, 0xA3, 0x6A, 0x3B, 0xFF,
		0x70, 0x5D, 0x4E, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF,
		0xF2, 0xF2, 0xF2, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF,
		0xAC, 0xAC, 0xAC, 0xFF, 0x85, 0x7C, 0x74, 0xFF, 0x8C, 0x6A, 0x4C, 0xFF, 0xB8, 0x80, 0x50, 0xFF,
		0xEC, 0xA3, 0x66, 0xFF, 0xFF, 0xBD, 0x7A, 0xFF, 0xFF, 0xC7, 0x84, 0xFF, 0xFF, 0xCD, 0x8D, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF, 0xFF, 0xDE, 0x97, 0xFF,
		0xFF, 0xD5, 0x8C, 0xFF, 0xFF, 0xC4, 0x7C, 0xFF, 0xEC, 0xA3, 0x66, 0xFF, 0xB8, 0x80, 0x50, 0xFF,
		0x85, 0x63, 0x44, 0xFF, 0x6A, 0x60, 0x59, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xB5, 0xB5, 0xB5, 0xFF,
		0x91, 0x91, 0x91, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 0x69, 0x69, 0x69, 0xFF,
		0x67, 0x67, 0x67, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
		0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
		0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
		0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
		0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
		0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF, 0x5C, 0x5C, 0x5C, 0xFF,
		0x67, 0x67, 0x67, 0xFF, 0x69, 0x69, 0x69, 0xFF, 0x73, 0x73, 0x73, 0xFF, 0x91, 0x91, 0x91, 0xFF,
		0xC3, 0xC3, 0xC3, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF,
		0xED, 0xED, 0xED, 0xFF, 0xED, 0xED, 0xED, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xDD, 0xDD, 0xDD, 0xFF, 0xCD, 0xCD, 0xCD, 0xFF, 0xBD, 0xBD, 0xBD, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xB5, 0xB5, 0xB5, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
		0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
		0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
		0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
		0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
		0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xAC, 0xAC, 0xAC, 0xFF,
		0xBD, 0xBD, 0xBD, 0xFF, 0xD5, 0xD5, 0xD5, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
		0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF, 0xE4, 0xE4, 0xE4, 0xFF,
};

/*** sAQUA_BITMAP *************************************************************/
/* Purpose : anAQUA_BITMAP's image header for CBitmap::CreateBitmapIndirect   */
/* Unit    : BITMAP                                                           */
/* Range   : structure list                                                   */
/* List    :  0 - int            bmType       : bitmap type (0)               */
/*            1 - int            bmWidth      : width  in pixels (>0)         */
/*            2 - int            bmHeight     : height in pixels (>0)         */
/*            3 - int            bmWidthBytes ; number of bytes per line (%2) */
/*            4 - unsigned char  bmPlanes     ; number of color planes        */
/*            5 - unsigned char  bmBitsPixel  : number of bits per pixel      */
/*            6 - unsigned char* bmBits       : pointer to an array of shorts */
/* Example : None                                                             */
/* WARNING : I said SHORT for <bmBits> because the number of bytes must be    */
/*           divible by 2 ! But in facts, it can be bytes, longs, whatever,   */
/*           as long as the size of the array remains divisible by 2...       */
/******************************************************************************/
BITMAP sAQUA_BITMAP =
{
	0,            // bmType
		90,           // bmWidth
		16,           // bmHeight
		360,          // bmWidthBytes
		1,            // bmPlanes
		32,           // bmBitsPixel
		anAQUA_BITMAP // bmBits
};
#endif // dCSP_RESOURCE_BITMAP

/*** anThreeDots **************************************************************/
/* Purpose : dots for reduced string                                          */
/* Unit    : char                                                             */
/* Range   : [0-127] - LIMITED selection                                      */
/* List    : None                                                             */
/* Example : None                                                             */
/******************************************************************************/
const char anThreeDots[] = "...";

// *** CONSTRUCTORS ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CSkinProgress                                              */
/* Role          : Default constructor  for status bar usage                  */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : Sets the default values to                                 */
/*                   i_poStrMessage = "Work in progress..."                   */
/*                   i_nUpper       = 100          - from 0 to 100            */
/*                   i_nProgress    = cSPT_PERCENT - DISPLAY PERCENT          */
/*                   i_nPane        = 0            - Share the text pane      */
/*                   i_nSize        = 200          - 200 pixels FORCED WIDTH  */
/*                   i_poBitmap     = NULL         - USE EMBEDDED BITMAP      */
/*                <> i_bReverse     = false        - normal display           */
/* Behavior      : A - Create the progress bar using default parameters       */
/*----------------------------------------------------------------------------*/
/* PROC CSkinProgress                                                         */
/*                                                                            */
/* A..... Create a status progress bar with "Work in progress..." as text     */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
CSkinProgress::CSkinProgress
( // Default constructor
 )
{

	// Init

	// Process

	// A..... Create a default progress bar in Status Bar with "Work in progress..." as text
	ProgressInStatusBar("Work in progress");
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CSkinProgress                                              */
/* Role          : Default constructor with parameters for status bar usage   */
/* Type          : public                                                     */
/* Interface     : RETURN (pointer to value, direct value)                    */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poStrMessage : The text message to display in pane 0   */
/*                   i_nUpper       : Upper limit to reach 100% (0 at 0%)     */
/*                   i_nProgress    : Progress completion message type        */
/*                   i_nPane        : Pane number for the progress bar        */
/*                   i_nSize        : Progress bar width in pixel IF pane 0   */
/*                   i_poBitmap     : Replacement progress bar bitmap         */
/*                <> i_bReverse     : Reverse display of the progress bar     */
/* Pre-condition : None                                                       */
/* Constraints   : At least, <i_poStrMessage> must be provided                */
/* Behavior      : A - Create the progress bar using user's parameters        */
/*----------------------------------------------------------------------------*/
/* PROC CSkinProgress                                                         */
/*                                                                            */
/* A..... Create the progress bar using user's parameters                     */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
CSkinProgress::CSkinProgress
( // Default constructor with parameters for status bar usage
 LPCTSTR     i_poStrMessage, // Text to display
 int         i_nUpper,       // = 100          : Default range from 0 to i_nUpper
 int         i_nProgress,    // = cSPT_PERCENT : Message type to add to the text
 int         i_nPane,        // = 0            : Pane number in which display the progress bar
 int         i_nSize,        // = 200          : Size of the progress bar if in pane 0
 CBitmap*    i_poBitmap      // = NULL         : Pointer to a user bitmap
#ifdef dCSP_SLIDERBAR_METHOD
 ,BOOL        i_bReverse      // = false        : Reverse display of the progress bar
#endif // dCSP_SLIDERBAR_METHOD
 )
{

	// Init

	// Process

	// A..... Create the progress bar using user's parameters
	ProgressInStatusBar
		(
		i_poStrMessage,
		i_nUpper,
		i_nProgress,
		i_nPane,
		i_nSize,
		i_poBitmap
#ifdef dCSP_SLIDERBAR_METHOD
		,i_bReverse
#endif // dCSP_SLIDERBAR_METHOD
		);
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CSkinProgress                                              */
/* Role          : Default constructor with parameters for dialog usage       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poWndProgress : The CWnd where to display the progress */
/*                   i_nUpper        : Upper limit to reach 100% (0 at 0%)    */
/*                   i_poBitmap      : Replacement progress bar bitmap        */
/*                <> i_bReverse      : Reverse display of the progress bar    */
/*                   i_poWndMessage  : The CWnd where to display the text     */
/*                   i_poStrMessage  : The text message to display            */
/*                   i_nProgress     : Progress completion message type       */
/* Pre-condition : None                                                       */
/* Constraints   : At least, <i_poStrMessage> must be provided                */
/* Behavior      : A - Create the dialog progress bar using user's parameters */
/*----------------------------------------------------------------------------*/
/* PROC CSkinProgress                                                         */
/*                                                                            */
/* A..... Create the dialog progress bar using user's parameters              */
/*----------------------------------------------------------------------------*/
/* The most simple dialog CSkinProgress constructor :                         */
/*   CSkinProgress oCspBar(&oCStatic); // Use 100 as upper limit              */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
#ifdef dCSP_DIALOG_PROGRESS
CSkinProgress::CSkinProgress
( // Default constructor with parameters for dialog usage
 CWnd*       i_poWndProgress, // Pointer to the anchor CWnd to use for the progress bar
 int         i_nUpper,        // = 100,      : Default range from 0 to i_nUpper
 CBitmap*    i_poBitmap,      // = NULL      : Pointer to a user bitmap
#ifdef dCSP_SLIDERBAR_METHOD
 BOOL        i_bReverse,      // = false,    : Reverse display of the progress bar
#endif // dCSP_SLIDERBAR_METHOD
 CWnd*       i_poWndMessage,  // = NULL,     : Pointer to the anchor CWnd to use for the text pane
 LPCTSTR     i_poStrMessage,  // = NULL      : Text to display,
 int         i_nProgress      // = cSPT_NONE : Message type to add to the text
 )
{

	// Init

	// Process

	// A..... Create the progress bar using user's parameters
	ProgressInDialog
		(
		i_poWndProgress,
		i_nUpper, 
		i_poBitmap,
#ifdef dCSP_SLIDERBAR_METHOD
		i_bReverse,
#endif // dCSP_SLIDERBAR_METHOD
		i_poWndMessage,
		i_poStrMessage,
		i_nProgress
		);
}
#endif // dCSP_DIALOG_PROGRESS

// *** INITIALIZATORS ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CreateCommon                                               */
/* Role          : Initialize the progress with its basic parameters          */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error during progress bar creation        */
/*                          true  : Progress bar created                      */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poStrMessage  : The text message to display in pane 0  */
/*                   i_nUpper        : Upper limit to reach 100% (0 at 0%)    */
/*                   i_nProgress     : Progress completion message type       */
/*                   i_poBitmap      : Replacement progress bar bitmap        */
/*                <> i_bReverse      : Reverse display of the progress bar    */
/* Pre-condition : None                                                       */
/* Constraints   : Base of progress bar is fixed to 0, must use               */
/*                 [SetRange(...)] to change it...                            */
/* Behavior      : A - Set base values                                        */
/*                 B - Set the bitmap                                         */
/*                 C - Set the progress bar values                            */
/*                 D - Get the creation date and time of the progress bar     */
/*                 E - Set the refresh timer                                  */
/*                 F - Resize the text and display the whole things           */
/*----------------------------------------------------------------------------*/
/* PROC CreateCommon                                                          */
/*                                                                            */
/* A..... Set base values                                                     */
/* B..... Set the bitmap                                                      */
/* C..... Set the progress bar values                                         */
/* D..... Get the creation date and time of the progress bar                  */
/* E..... Set the refresh timer to 500 milliseconds                           */
/* F..... Resize the text and display the whole things                        */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::CreateCommon
(
 LPCTSTR  i_poStrMessage,
 int      i_nUpper,       // = 100
 int      i_nProgress,    // = cSPT_PERCENT
 CBitmap* i_poBitmap      // = NULL
#ifdef dCSP_SLIDERBAR_METHOD
 ,BOOL     i_bReverse      // = false
#endif // dCSP_SLIDERBAR_METHOD
 )
{
	// Init

	// A..... Set base values
	m_poBaseBmp       = NULL;
	m_poCompatibleBmp = NULL;
	m_poStretchBmp    = NULL;
	m_poProgressBmp   = NULL;
	m_oStrPrevMessage = "";
	m_oRectPane.SetRect(0, 0, 0, 0);

	// Process + Return

	// B..... Set the bitmap
	if(
		!SetBitmap
		(
		i_poBitmap
#ifndef dCSP_TIMED_REDRAW
		,false
#endif // dCSP_TIMED_REDRAW
		)
		)
	{
		return false;
	}
	else
	{
		// C..... Set the progress bar values
		// Set the progress text type
		m_nProgressText = i_nProgress;

		// Set range and step
		m_nLower = 0; // Set first parameters for the resampling
		ASSERT(i_nUpper > 0); // m_nLower is 0, m_nUpper CANNOT BE BEFORE m_nLower quite now
		m_nUpper = i_nUpper;
#ifdef dCSP_SLIDERBAR_METHOD
		m_nLeft  = 0;
#endif // dCSP_SLIDERBAR_METHOD
		m_nRight = 0;

		SetRange
			(
			0,
			i_nUpper,
			1,
			false
#ifndef dCSP_TIMED_REDRAW
			,false // Don't resample range, and avoid display there !
#endif // dCSP_TIMED_REDRAW
			);

		// Set and save additional values
		m_oStrMessage = i_poStrMessage;
#ifdef dCSP_SLIDERBAR_METHOD
		m_bReverse    = i_bReverse;
#endif // dCSP_SLIDERBAR_METHOD

		// D..... Get the creation date and time of the progress bar and set its position to the beginning
		Reset
			(
#ifndef dCSP_TIMED_REDRAW
			false
#endif // dCSP_TIMED_REDRAW
			);

		// E..... Set the refresh timer to 500 milliseconds (or 20ms/50Hz in <dCSP_TIMED_REDRAW> mode)
#ifndef dCSP_TIMED_REDRAW
		SetTimer((UINT) this, 500, NULL); // Use object's unique address as timer identifier
#else
		SetTimer((UINT) this,  20, NULL); // Use object's unique address as timer identifier
#endif // dCSP_TIMED_REDRAW

		// F..... Resize the text and display the whole things
		RefreshPanes(); // Set the text in pane 0 and do the first drawing of the SkinProgress bar

		return true;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : ProgressInStatusBar                                        */
/* Role          : Create the progress in status bar with basic parameters    */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error during progress bar creation        */
/*                          true  : Progress bar created                      */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poStrMessage  : The text message to display in pane 0  */
/*                   i_nUpper        : Upper limit to reach 100% (0 at 0%)    */
/*                   i_nProgress     : Progress completion message type       */
/*                   i_nPane         : Pane number for the progress bar       */
/*                   i_nSize         : Progress bar width in pixel IF pane 0  */
/*                   i_poBitmap      : Replacement progress bar bitmap        */
/*                <> i_bReverse      : Reverse display of the progress bar    */
/* Pre-condition : None                                                       */
/* Constraints   : Base of progress bar is fixed to 0, must use [SetRange] to */
/*                 change it...                                               */
/* Behavior      : A - Get status bar handler                                 */
/*                 B - Create a CStatic object upon the target pane           */
/*                 C - Specific Status Bar initialization                     */
/*                 D - Create and initialize the progress bar                 */
/*----------------------------------------------------------------------------*/
/* PROC ProgressInStatusBar                                                   */
/*                                                                            */
/* A..... Get status bar handler                                              */
/* [IF there is a status bar]                                                 */
/* : B..... Create a CStatic object upon the target status bar pane           */
/* : [IF CStatic creation OK]                                                 */
/* : : C..... Specific Status Bar initialization                              */
/* : : D..... Create and initialize the progress bar                          */
/* : [ENDIF]                                                                  */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::ProgressInStatusBar
( // The initialization routine
 LPCTSTR     i_poStrMessage,  // Text to display
 int         i_nUpper,        // = 100,         : Default range from 0 to i_nUpper
 int         i_nProgress,     // = cSPT_PERCENT : Message type to add to the text
 int         i_nPane,         // = 0,           : Pane number in which display the progress bar
 int         i_nSize,         // = 200,         : Size of the progress bar if in pane 0
 CBitmap*    i_poBitmap       // = NULL         : Pointer to a user bitmap
#ifdef dCSP_SLIDERBAR_METHOD
 ,BOOL        i_bReverse       // = false
#endif // dCSP_SLIDERBAR_METHOD
 )
{
	BOOL        bSuccess;
	CStatusBar* poStatusBar;
	DWORD       nStyleEx;
	DWORD       nStyle;
	CRect       oRectPane;

	LPCSTR      poStrClass;
	UINT        nClassStyle;

	// Init

	bSuccess          = false;
#ifdef dCSP_DIALOG_PROGRESS
	m_poWndProgress   = NULL;
	m_poWndMessage    = NULL;
#endif // dCSP_DIALOG_PROGRESS

	// Process

	// A..... Get status bar handler
	poStatusBar = GetStatusBar();
	if(poStatusBar != NULL)
	{
		// B..... Create a CStatic object upon the target status bar pane
		// Set the style for the custom progress bas
		nClassStyle = 0; // CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
		nStyleEx   = WS_EX_STATICEDGE;
		nStyle     = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE;

		// Get CRect coordinates for requested status bar pane
		poStatusBar->GetItemRect(i_nPane, &oRectPane);

		// Create the progress bar
		poStrClass = AfxRegisterWndClass(nClassStyle, AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		bSuccess   = CStatic::CreateEx(nStyleEx, poStrClass, "", nStyle, oRectPane, poStatusBar, (UINT) this); // Use object's unique address as child window identifier

		ASSERT(bSuccess);
		if(!bSuccess)
		{
			return false;
		}
		else
		{
#ifdef dCSP_VERTICAL_BAR
#ifndef dCSP_DISPLAY_STRETCH
			m_bVertical = false; // *ALWAYS false IN CSkinProgress while in StatusBar mode*
#else
			m_bVertical = true;  // *FORCE* vertical for m_poStretchBmp analysis
#endif // dCSP_DISPLAY_STRETCH
#endif // dCSP_VERTICAL_BAR

			// C..... Specific Status Bar initialization
			SetSize
				(
				i_nSize
#ifndef dCSP_TIMED_REDRAW
				,false // Set the size, and avoid display there !
#endif // dCSP_TIMED_REDRAW
				);
			m_nPane        = i_nPane;
			m_oStrPrevText = poStatusBar->GetPaneText(m_nPane); // Get the previous pane's text

			// D..... Create and initialize the progress bar
			bSuccess = CreateCommon
				(
				i_poStrMessage,
				i_nUpper,
				i_nProgress,
				i_poBitmap
#ifdef dCSP_SLIDERBAR_METHOD
				,i_bReverse
#endif // dCSP_SLIDERBAR_METHOD
				);

			return bSuccess;
		}
	}
	else
	{
		return false;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : ProgressInDialog                                           */
/* Role          : Create the progress bar in a dialog with basic parameters  */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error during progress bar creation        */
/*                          true  : Progress bar created                      */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poWndProgress : The CWnd where to display the progress */
/*                   i_nUpper        : Upper limit to reach 100% (0 at 0%)    */
/*                   i_poBitmap      : Replacement progress bar bitmap        */
/*                <> i_bReverse      : Reverse display of the progress bar    */
/*                   i_poWndMessage  : The CWnd where to display the text     */
/*                   i_poStrMessage  : The text message to display in pane 0  */
/*                   i_nProgress     : Progress completion message type       */
/* Pre-condition : None                                                       */
/* Constraints   : Base of progress bar is fixed to 0, must use [SetRange] to */
/*                 change it...                                               */
/* Behavior      : A - Get status bar handler                                 */
/*                 B - Create a CStatic object upon the target pane           */
/*                 C - Set the <m_bVertical> flag                             */
/*                 D - Specific Status Bar initialization                     */
/*                 E - Create and initialize the progress bar                 */
/*----------------------------------------------------------------------------*/
/* PROC ProgressInDialog                                                      */
/*                                                                            */
/* A..... Specific Dialog initialization                                      */
/* B..... Create a CStatic object upon the target CWnd progress bar pane      */
/* [IF CStatic creation OK]                                                   */
/* : C..... Set the <m_bVertical> flag if the progress bar pane is vertical   */
/* : [IF there is a CWnd anchor for a text pane to create]                    */
/* : : D..... Specific Status Bar initialization                              */
/* : [ENDIF]                                                                  */
/* : E..... Create and initialize the progress bar                            */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/* Well, sorry, I put <i_bReverse> in the middle of the function call, but    */
/* there is an obvious reason : If you want to bypass the message's Cwnd,     */
/* you can leave the ending parameters empty ! But if I had put <i_bReverse>  */
/* on the end, you had to force every text related parameter to NULL before   */
/* being able to set the <i_bReverse> value.                                  */
/*                                                                            */
/* You can also use yourself the <dCSP_SLIDERBAR_METHOD> define to compile    */
/* your own code according to your needs (as long as you include the .h) !    */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
#ifdef dCSP_DIALOG_PROGRESS
BOOL CSkinProgress::ProgressInDialog
( // The initialization routine
 CWnd*       i_poWndProgress,
 int         i_nUpper,        // = 100,      : Default range from 0 to i_nUpper
 CBitmap*    i_poBitmap,      // = NULL      : Pointer to a user bitmap
#ifdef dCSP_SLIDERBAR_METHOD
 BOOL        i_bReverse,      // = false
#endif // dCSP_SLIDERBAR_METHOD
 CWnd*       i_poWndMessage,  // = NULL
 LPCTSTR     i_poStrMessage,  // = NULL      : Text to display
 int         i_nProgress      // = cSPT_NONE : Message type to add to the text
 )
{
	BOOL        bSuccess;
	DWORD       nStyleEx;
	DWORD       nStyle;
	CRect       oRectPane;

	LPCSTR      poStrClass;
	UINT        nClassStyle;

	// Init

	bSuccess        = false;

	// A..... Specific Dialog initialization
	m_poWndProgress = i_poWndProgress;
	m_poWndMessage  = i_poWndMessage;

	// Process + Return

	// B..... Create a CStatic object upon the target CWnd progress bar pane
	// Set the style for the custom progress bas
	nClassStyle = 0; // CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
	nStyleEx    = WS_EX_STATICEDGE;
	nStyle      = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE;

	// Get CRect coordinates for requested target CWnd bar pane
	GetTargetRect(&oRectPane);

	// Create the progress bar
	poStrClass = AfxRegisterWndClass(nClassStyle, AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	bSuccess   = CStatic::CreateEx
		(
		nStyleEx,
		poStrClass,
		"",
		nStyle,
		oRectPane,
		m_poWndProgress->GetParent(), // Attach it not the the CWnd anchor (thus getting in its client area) but to its parent to overlap it
		(UINT) this // Use object's unique address as child window identifier
		);

	ASSERT(bSuccess);
	if(!bSuccess)
	{
		return false;
	}
	else
	{
		// C..... Set the <m_bVertical> flag if the progress bar pane is vertical
#ifdef dCSP_VERTICAL_BAR
#ifndef dCSP_DISPLAY_STRETCH
		if(oRectPane.Height() > oRectPane.Width())
		{ // VERTICAL
			m_bVertical = true;
		}
		else
		{
			m_bVertical = false;
		}
#else
		m_bVertical = true;  // *FORCE* vertical for <m_poStretchBmp> analysis
#endif // dCSP_DISPLAY_STRETCH
#endif // dCSP_VERTICAL_BAR

		if(m_poWndMessage != NULL)
		{
			// D..... Specific Status Bar initialization
			m_poWndMessage->GetWindowText(m_oStrPrevText); // Get the previous pane's text
		}else{}

		if(bSuccess)
		{
			// E..... Create and initialize the progress bar
			bSuccess = CreateCommon
				(
				i_poStrMessage,
				i_nUpper,
				i_nProgress,
				i_poBitmap
#ifdef dCSP_SLIDERBAR_METHOD
				,i_bReverse
#endif // dCSP_SLIDERBAR_METHOD
				);
		}else{}

		return bSuccess;
	}
}
#endif // dCSP_DIALOG_PROGRESS

// *** DESCTRUCTOR ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : ~CSkinProgress                                             */
/* Role          : Destructor, restore the inital states and used memory      */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Clear the progress bar                                 */
/*                 B - Delete the refresh timer                               */
/*                 C - Delete the base bitmap                                 */
/*                 C - Delete the client DC compatible base bitmap            */
/*                 D - Delete the stretched for image list bitmap             */
/*                 E - Delete the displayed progress bar bitmap               */
/*----------------------------------------------------------------------------*/
/* PROC ~CSkinProgress                                                        */
/*                                                                            */
/* A..... Clear the progress bar                                              */
/* B..... Delete the refresh timer                                            */
/* C..... Delete the <m_poBaseBmp> base bitmap                                */
/* D..... Delete the <m_poCompatibleBmp> client DC compatible base bitmap     */
/* E..... Delete the <m_poStretchBmp> stretched for image list bitmap         */
/* F..... Delete the <m_poProgressBmp> displayed progress bar bitmap          */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
CSkinProgress::~CSkinProgress
( // Destructor
 )
{

	// Init

	// Process

	// A..... Clear the progress bar
	Clear();

	// B..... Delete the refresh timer
	if(IsWindow(this->m_hWnd)) // Test first if <this> is the correct address of the object before using it as the right timer identifier
	{
		KillTimer((UINT) this); // Use object's unique address as timer identifier
	}else{}

	// C..... Delete the <m_poBaseBmp> base bitmap
	if(m_poBaseBmp != NULL)
	{ // Delete the attached base bitmap
		delete m_poBaseBmp;
		m_poBaseBmp = NULL;
	}else{}

	// D..... Delete the <m_poCompatibleBmp> client DC compatible base bitmap
	if(m_poCompatibleBmp != NULL)
	{ // Delete the attached client DC compatible base bitmap
		delete m_poCompatibleBmp;
		m_poCompatibleBmp = NULL;
	}else{}

	// E..... Delete the <m_poStretchBmp> stretched for image list bitmap
	if(m_poStretchBmp != NULL)
	{ // Delete the attached stretched image list bitmap
		delete m_poStretchBmp;
		m_poStretchBmp = NULL;
	}else{}

	// F..... Delete the <m_poProgressBmp> displayed progress bar bitmap
	if(m_poProgressBmp != NULL)
	{ // Delete the attached progress bitmap
		delete m_poProgressBmp;
		m_poProgressBmp	= NULL;
	}else{}
}

// *** CLEANER ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : Clear                                                      */
/* Role          : Erase the progress                                         */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Delete the progress bar                                */
/*                 B - Set IDLE message in pane 0                             */
/*                 C - Set previous message in user pane                      */
/*                 D - Display the changes                                    */
/*                 E - Set previous message                                   */
/*----------------------------------------------------------------------------*/
/* PROC Clear                                                                 */
/*                                                                            */
/* [IF the object is still alive]                                             */
/* : A..... Delete the progress bar                                           */
/* : [IF status bar]                                                          */
/* : : B..... Set IDLE message in pane 0                                      */
/* : : C..... Set previous message in user pane                               */
/* : : D..... Display the changes                                             */
/* : [ELSE]                                                                   */
/* : : [IF there is a text CWnd]                                              */
/* : : : E..... Set previous message                                          */
/* : : [ENDIF]                                                                */
/* : [ENDIF]                                                                  */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::Clear
( // Deletion of the progress bar, restoration of the context
 )
{
	CStatusBar* poStatusBar;

	//

	if(IsWindow(GetSafeHwnd()))
	{
		// A..... Delete the progress bar
		ModifyStyle(WS_VISIBLE, 0); // Clear the attached CStatic progress bar
		DestroyWindow(); // Destroy it

#ifdef dCSP_DIALOG_PROGRESS
		if(m_poWndProgress == NULL)
		{ // If in Status Bar, reset pane 0 and delete the progress bar
#endif // dCSP_DIALOG_PROGRESS
			poStatusBar = GetStatusBar();
			if(poStatusBar != NULL)
			{
				// B..... Set IDLE message in pane 0
				m_oStrMessage.LoadString(AFX_IDS_IDLEMESSAGE); // Get the IDLE_MESSAGE and place ? in the status bar 
				poStatusBar->SetPaneText(0, m_oStrMessage);

				// C..... Set previous message in user pane
				if(m_nPane != 0)
				{ // If not the text pane, restore previous text in the pane
					poStatusBar->SetPaneText(m_nPane, m_oStrPrevText);
				}else{}

				// D..... Display the changes
				poStatusBar->InvalidateRect(NULL);
			}else{}
#ifdef dCSP_DIALOG_PROGRESS
		}
		else
		{
			if(m_poWndMessage != NULL)
			{
				// E..... Set previous message
				m_poWndMessage->SetWindowText(m_oStrPrevText); // Set the previous pane's text
			}else{}

			m_poWndProgress->InvalidateRect(NULL);
		} // NOTHING TO RESET IN DIALOG MODE !!!
#endif // dCSP_DIALOG_PROGRESS
	}else{}
}

// *** SYSTEM INTERFACE ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetStatusBar                                               */
/* Role          : Get status bar's window handle                             */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   CStatusBar* : Window handle, else NULL if no status bar  */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get the main window handle                             */
/*                 B - Retrieve the status bar handle                         */
/*----------------------------------------------------------------------------*/
/* PROC GetStatusBar                                                          */
/*                                                                            */
/* A..... Get the main window handle                                          */
/* B..... Retrieve the status bar handle                                      */
/* [IF main window is based on CFrameWnd]                                     */
/* : BA.... Return status bar's handle from CFrameWnd main window type        */
/* :[ELSE]                                                                    */
/* : BB.... Return status bar's handle from other main window type            */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
CStatusBar* CSkinProgress::GetStatusBar
( // Get the pointer on the current status bar
 )
{
	CWnd* poMainWnd;
	CWnd* poMessageBar;

	// Init

	// A..... Get the main window handle
	poMainWnd = AfxGetMainWnd();

	// Process + Return

	// B..... Retrieve the status bar handle
	if(poMainWnd != NULL)
	{
		if(poMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		{ // If main window is a frame window, use normal methods
			poMessageBar = ((CFrameWnd*) poMainWnd)->GetMessageBar();

			// BA.... Return status bar's handle from CFrameWnd main window type
			return DYNAMIC_DOWNCAST(CStatusBar, poMessageBar);
		}
		else
		{ // otherwise traverse children to try and find the status bar
			// BB.... Return status bar's handle from other main window type
			return DYNAMIC_DOWNCAST(CStatusBar, poMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR));
		}
	}
	else
	{
		return NULL;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetTargetRect                                              */
/* Role          : Get the target CRect of the progress bar                   */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error in getting the target rect          */
/*                          true  : CRect returned is valid                   */
/*                 OUTPUT (pointer to value)                                  */
/*                   o_poProgressRect : Pointer on an existing CRect          */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get the progress bar's CWnd anchor rectangle           */
/*                 B - Get the status bar selected pane rectangle             */
/*----------------------------------------------------------------------------*/
/* PROC GetTargetRect                                                         */
/*                                                                            */
/* [IF there is a progress bar CWnd anchor specified]                         */
/* : A..... Get the progress bar's CWnd anchor rectangle                      */
/* [ELSE]                                                                     */
/* : B..... Get the status bar selected pane rectangle                        */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::GetTargetRect
( // Get the target CRect for the [SetBitmap(...)]
 CRect* o_poProgressRect // Pointer on an existing CRect
 )
{
	CStatusBar* poStatusBar;

	// Init

	// Process + Return

#ifdef dCSP_DIALOG_PROGRESS
	if(m_poWndProgress != NULL)
	{ // If dialog mode
		// A..... Get the progress bar's CWnd anchor rectangle
		m_poWndProgress->GetWindowRect(o_poProgressRect); // Return CWnd parent's screen absolute coordinates 
		m_poWndProgress->GetParent()->ScreenToClient(o_poProgressRect); // Transform into parent relative coordinates

		return true;
	}
	else
	{
#endif // dCSP_DIALOG_PROGRESS
		poStatusBar = GetStatusBar();
		if(poStatusBar != NULL)
		{
			// B..... Get the status sar selected pane rectangle
			poStatusBar->GetItemRect(m_nPane, o_poProgressRect); // Already working in parent (the status bar) relative coordinates

			return true;
		}
		else
		{
			return false;
		}
#ifdef dCSP_DIALOG_PROGRESS
	}
#endif // dCSP_DIALOG_PROGRESS
}

// *** BITMAP PROCESS ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetBitmap                                                  */
/* Role          : Create an image list for the progress bar painting         */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error in image list creation              */
/*                          true  : Image list created and ready to use       */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poCwnd   : CWnd anchor to get the bitmap size to use   */
/*                   i_poBitmap : Image to use, use default bitmap if NULL    */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get the base bitmap for the image list creation        */
/*                 B - Create the stretched bitmap used for the image list    */
/*                 C - Create an image list compatible bitmap                 */
/*                 D - Create the image list                                  */
/*                 E - Display the new bitmap                                 */
/*----------------------------------------------------------------------------*/
/* PROC SetBitmap                                                             */
/*                                                                            */
/* A..... Get the base bitmap for the <m_oBarImgLst> image list creation      */
/*   AA.... Create the base and the device compatible bitmaps                 */
/*   AB.... Load the <i_poBitmap> base bitmap                                 */
/*   AC.... Convert the <m_poBaseBmp> base bitmap in a compatible format      */
/* B..... Create the stretched bitmap used for the image list creation        */
/*   BA.... Create the bitmap, and delete the previous one if there was one   */
/*   BB.... Create a bitmap with mixed dimensions from the base and the pane  */
/*   BC.... Set the copy parameters for the stretching                        */
/* C..... Modify the base bitmap to create an image list compatible bitmap    */
/* D..... Create the <m_oBarImgLst> image list                                */
/*   DA.... Get the stretched bitmap size                                     */
/*   DB.... Calculate the size of every element of the image list             */
/*   DC.... Delete the previous image list the create a new one from scratch  */
/*   DD.... Create the new image list in 32 bits for maximun throughput       */
/*   DE.... Add the stretched bitmap in the image list                        */
/* E..... Display the new bitmap                                              */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetBitmap
( // Change of progress bar image
 CBitmap* i_poBitmap  // = NULL : Pointer to an existing bitmap
#ifndef dCSP_TIMED_REDRAW
 ,BOOL     i_bDisplay  // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{ 
	BITMAP      sBmpSrc;
#ifdef dCSP_VERTICAL_BAR
	CBitmap     oBmpVertical;
#endif // dCSP_VERTICAL_BAR
	CBitmap*    poOldBitmap;
#ifdef dCSP_VERTICAL_BAR
	CBitmap*    poOldVertical;
#endif // dCSP_VERTICAL_BAR
	CBitmap*    poOldStretch;
	CRect       oRectBar;
	CDC         oDcBitmap;
#ifdef dCSP_VERTICAL_BAR
	CDC         oDcVertical;
#endif // dCSP_VERTICAL_BAR
	CDC         oDcStretch;

	// Init

	m_oRectPaint.SetRect(0, 0, 0, 0); // Force redraw if the bitmap is changed while the SkinProgress object exist
#ifdef dCSP_VERTICAL_BAR
	poOldVertical = NULL; // AVOID /W4 -> BC.... *STUPID COMPILER*
#endif // dCSP_VERTICAL_BAR

	// Process

	if(GetTargetRect(&oRectBar))
	{ // If the target rectangle is valid
		CClientDC oDC(this); // Get the current DC

		// A..... Get the <i_poBitmap> base bitmap for the <m_oBarImgLst> image list creation
		// AA.... Create the base and the device compatible bitmaps
		if(m_poBaseBmp != NULL)
		{ // *IF* m_poBaseBmp already exist
			delete m_poBaseBmp;
			m_poBaseBmp = NULL;
		}else{}
		m_poBaseBmp = new CBitmap; // BEWARE : DON'T INITIALIZE IT YET !

		if(m_poCompatibleBmp != NULL)
		{ // *IF* m_poCompatibleBmp already exist
			delete m_poCompatibleBmp;
			m_poCompatibleBmp = NULL;
		}else{}
		m_poCompatibleBmp = new CBitmap; // BEWARE : DON'T INITIALIZE IT YET !

		// AB.... Load the <i_poBitmap> base bitmap
		// Check if i_poBitmap is valid, otherwise use the resource's bitmap
		if(i_poBitmap == NULL)
		{ // If no bitmap provided, takes the default bitmap
#ifndef dCSP_RESOURCE_BITMAP
			m_poBaseBmp->CreateBitmapIndirect(&sAQUA_BITMAP);
#else
			m_poBaseBmp->LoadBitmap(IDB_AQUABAR); // Defaut bitmap, might have to be changed !
#endif // dCSP_RESOURCE_BITMAP
		}
		else
		{ // Copy the <i_poBitmap> input bitmap as the new <m_poBaseBmp> base bitmap
			CopyBitmap(m_poBaseBmp, i_poBitmap);
		}

		// AC.... Convert the <m_poBaseBmp> base bitmap in a device compatible format
		ConvBitmap(m_poCompatibleBmp, m_poBaseBmp, &oDC);

		// Get bitmap width and height for the image list creation
		m_poCompatibleBmp->GetBitmap(&sBmpSrc);
		m_nBmpWidth  = sBmpSrc.bmWidth / cSPB_EnumElements; // cSPB_EnumElements == 9, the bitmap is actually composed of 9 parts
		m_nBmpHeight = sBmpSrc.bmHeight;

		// B..... Create the <m_poStretchBmp> stretched bitmap used for the <m_oBarImgLst> image list creation
		// BA.... Create the bitmap, and if a previous one was already existing, delete it first
		if(m_poStretchBmp != NULL)
		{ // *IF* m_poStretchBmp already exist
			delete m_poStretchBmp;
			m_poStretchBmp = NULL;
		}else{}
		m_poStretchBmp = new CBitmap;

		// BB.... Create a bitmap with the base bitmap width, and the despination pane height minus 2
#ifdef dCSP_VERTICAL_BAR
		if(m_bVertical == false)
		{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifndef dCSP_CREATE_BITMAP_FILE
			m_poStretchBmp->CreateCompatibleBitmap(&oDC, sBmpSrc.bmWidth, oRectBar.Height() - 2); // Minus 2, with don't count the contour of the box
#else
			m_poStretchBmp->CreateCompatibleBitmap(&oDC, sBmpSrc.bmWidth, sBmpSrc.bmHeight); // BITMAP CREATION IN CURRENT VIDEO MODE !
#endif //dCSP_CREATE_BITMAP_FILE
#ifdef dCSP_VERTICAL_BAR
		}
		else
		{ // Vertical bar
#ifndef dCSP_DISPLAY_STRETCH
			oBmpVertical.CreateCompatibleBitmap(&oDC, sBmpSrc.bmWidth, oRectBar.Width() - 2); // Minus 2, with don't count the contour of the box
			m_poStretchBmp->CreateCompatibleBitmap(&oDC, (oRectBar.Width() - 2) * cSPB_EnumElements, m_nBmpWidth); // Minus 2, with don't count the contour of the box
#else
			oBmpVertical.CreateCompatibleBitmap(&oDC, sBmpSrc.bmWidth, oRectBar.Height() - 2); // Minus 2, with don't count the contour of the box
			m_poStretchBmp->CreateCompatibleBitmap(&oDC, (oRectBar.Height() - 2) * cSPB_EnumElements, m_nBmpWidth); // Minus 2, with don't count the contour of the box
#endif //dCSP_DISPLAY_STRETCH
		}
#endif // dCSP_VERTICAL_BAR

		// BC.... Set the copy parameters for the stretching
		// Set the DC
		oDcBitmap.CreateCompatibleDC(&oDC);
		poOldBitmap  = oDcBitmap.SelectObject(m_poCompatibleBmp); // Source
		oDcStretch.CreateCompatibleDC(&oDC);
		poOldStretch = oDcStretch.SelectObject(m_poStretchBmp); // Destination
#ifdef dCSP_VERTICAL_BAR
		if(m_bVertical != false)
		{ // Vertical bar
			oDcVertical.CreateCompatibleDC(&oDC);
			poOldVertical = oDcVertical.SelectObject(&oBmpVertical); // Destination
			oDcVertical.SetStretchBltMode(HALFTONE);
		}
		else
		{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
			oDcStretch.SetStretchBltMode(HALFTONE);
#ifdef dCSP_VERTICAL_BAR
		}
#endif // dCSP_VERTICAL_BAR

		// C..... Copy the <m_poCompatibleBmp> base bitmap and apply modifications to create an image list compatible bitmap
		// Copy the picture
#ifdef dCSP_VERTICAL_BAR
		if(m_bVertical == false)
		{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifndef dCSP_CREATE_BITMAP_FILE
			oDcStretch.StretchBlt(0, 0, sBmpSrc.bmWidth, oRectBar.Height() - 2, &oDcBitmap, 0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, SRCCOPY);
#else
			oDcStretch.BitBlt(0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, &oDcBitmap, 0, 0, SRCCOPY); // BITMAP CREATION IN CURRENT VIDEO MODE, PLEASE USE 32 BITS, AVOID MAPPED !
#endif //dCSP_CREATE_BITMAP_FILE
#ifdef dCSP_VERTICAL_BAR
		}
		else
		{ // Vertical bar
#ifndef dCSP_DISPLAY_STRETCH
			oDcVertical.StretchBlt(0, 0, sBmpSrc.bmWidth, oRectBar.Width()  - 2, &oDcBitmap, 0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, SRCCOPY);
#else
			oDcVertical.StretchBlt(0, 0, sBmpSrc.bmWidth, oRectBar.Height() - 2, &oDcBitmap, 0, 0, sBmpSrc.bmWidth, sBmpSrc.bmHeight, SRCCOPY);
#endif // dCSP_DISPLAY_STRETCH

			// Copy the stretched vertical picture into the m_poStretchBmp with part flip
#ifndef dCSP_DISPLAY_STRETCH
			for(int y = 0; y < oRectBar.Width() - 2; y += 1)
#else
			for(int y = 0; y < oRectBar.Height() - 2; y += 1)
#endif // dCSP_DISPLAY_STRETCH
			{ // Scan each line
				for(int x = 0; x < sBmpSrc.bmWidth; x += 1)
				{ // Scan every pixel of the line
#ifndef dCSP_DISPLAY_STRETCH
					oDcStretch.SetPixel(y + ((oRectBar.Width()  - 2) * (x / m_nBmpWidth)), x % m_nBmpWidth, oDcVertical.GetPixel(x, y));
#else
					oDcStretch.SetPixel(y + ((oRectBar.Height() - 2) * (x / m_nBmpWidth)), x % m_nBmpWidth, oDcVertical.GetPixel(x, y));
#endif // dCSP_DISPLAY_STRETCH
				}
			}
		}
#endif // dCSP_VERTICAL_BAR

		// Release the DC
#ifdef dCSP_VERTICAL_BAR
		if(m_bVertical != false)
		{ // Vertical bar
			oDcVertical.SelectObject(poOldVertical);
			oDcVertical.DeleteDC();
		}else{}
#endif // dCSP_VERTICAL_BAR
		oDcStretch.SelectObject(poOldStretch);
		oDcStretch.DeleteDC();
		oDcBitmap.SelectObject(poOldBitmap);
		oDcBitmap.DeleteDC();

		// D..... Create the <m_oBarImgLst> image list
		// DA.... Get the stretched bitmap size
		// Get bitmap width and height for the image list creation
		m_poStretchBmp->GetBitmap(&sBmpSrc);

#ifdef dCSP_CREATE_BITMAP_FILE
		// Fast and dirty routine to save the stretched bitmap in C format, for the people who wants to recreate the AQUA_BITMAP array with their own default bitmap, instead to provide it to the constructor
		FILE*          sFileHandle;
		unsigned char* panDataBmp;
		int            nCount = sBmpSrc.bmWidthBytes * sBmpSrc.bmHeight;
		unsigned char  nBufferHi;
		unsigned char  nBufferLo;

		panDataBmp = new unsigned char[nCount];

		if(panDataBmp != NULL)
		{
			sFileHandle = fopen("C:\\aqua_bitmap.cpp", "wt"); // Change the path here
			if(sFileHandle != NULL)
			{
				m_poStretchBmp->GetBitmapBits(nCount, panDataBmp);

				fprintf(sFileHandle, "unsigned char anAQUA_BITMAP[%d] =\n{\n ", nCount);
				for(int nPointer = 0; nPointer < nCount; nPointer += 1)
				{
					fputs(" 0x", sFileHandle);
					nBufferHi = panDataBmp[nPointer];
					nBufferLo = nBufferHi;

					nBufferHi >>= 4;
					nBufferLo  &= 0x0F;

					if(nBufferHi < 10)
					{
						nBufferHi += '0';
					}
					else
					{
						nBufferHi += 'A' - 10;
					}

					if(nBufferLo < 10)
					{
						nBufferLo += '0';
					}
					else
					{
						nBufferLo += 'A' - 10;
					}

					fputc(nBufferHi, sFileHandle);
					fputc(nBufferLo, sFileHandle);

					fputs(",", sFileHandle);
					if((nPointer % 16) == 15)
					{ // 16 pixels per line
						fputs("\n ", sFileHandle);
					}else{}
				}
				fputs("};\n\n", sFileHandle);

				fputs("BITMAP sAQUA_BITMAP =\n{\n", sFileHandle);
				fprintf(sFileHandle, "  %d,   // bmType\n",     sBmpSrc.bmType);
				fprintf(sFileHandle, "  %d,  // bmWidth\n",     sBmpSrc.bmWidth);
				fprintf(sFileHandle, "  %d,  // bmHeight\n",    sBmpSrc.bmHeight);
				fprintf(sFileHandle, "  %d, // bmWidthBytes\n", sBmpSrc.bmWidthBytes);
				fprintf(sFileHandle, "  %d,   // bmPlanes\n",   sBmpSrc.bmPlanes);
				fprintf(sFileHandle, "  %d,  // bmBitsPixel\n", sBmpSrc.bmBitsPixel);
				fputs("  anAQUA_BITMAP // bmBits\n", sFileHandle);
				fputs("};\n", sFileHandle);

				fclose(sFileHandle);
			}else{}

			delete[] panDataBmp;
			panDataBmp = NULL;
		}else{}
#endif //dCSP_CREATE_BITMAP_FILE

		// DB.... Calculate the size of every element of the image list
		m_nBmpWidth  = sBmpSrc.bmWidth / cSPB_EnumElements; // cSPB_EnumElements == 5, the bitmap is yet composed of 5 parts
		m_nBmpHeight = sBmpSrc.bmHeight;

		// DC.... Delete the previous image list the create a new one from scratch (useful when providing a new bitmap during SkinProgress life-time)
		// Image list creation
		m_oBarImgLst.DeleteImageList(); // Delete the previous ImageList and recreate it with the new bitmap
		// DD.... Create the new image list in 32 bits to be sure it will accept any kind of bitmap in input (if 8 bits bitmap provided, will be converted in 32 bits)
		if(!m_oBarImgLst.Create(m_nBmpWidth, m_nBmpHeight, ILC_COLOR32, 0, 0)) // ILC_COLOR32 to be sure to accept any BitMap format passed through i_poBitmap
		{
			return false;
		}
		else
		{
			// DE.... Add the stretched bitmap in the image list
			m_oBarImgLst.Add(m_poStretchBmp, RGB(0, 255, 255));

			// E..... Display the new bitmap
#ifndef dCSP_TIMED_REDRAW
			if(i_bDisplay != false)
			{
				RefreshPanes();
			}else{}
#endif // dCSP_TIMED_REDRAW

			return true;
		}
	}
	else
	{
		return false;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CopyBitmap                                                 */
/* Role          : Copy a bitmap                                              */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error in copy                             */
/*                          true  : Image copied                              */
/*                 OUTPUT (pointer to value)                                  */
/*                   o_poBitmap    : Image where to copy                      */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poBitmap    : Image to copy                            */
/* Pre-condition : Both output and input pointer must be provided !           */
/*                 Also, the output bitmap MUST BE CREATED, BUT NOT           */
/*                 INITIALIZED                                                */
/* Constraints   : Do not handle mapped bitmap yet...                         */
/* Behavior      : A - Create a remote data buffer to get the source data     */
/*                 B - Get the source data                                    */
/*                 C - Copy the bitmap                                        */
/*----------------------------------------------------------------------------*/
/* PROC CopyBitmap                                                            */
/*                                                                            */
/* A..... Create a remote data buffer to get the source data                  */
/* [IF the remote buffer is successfully created]                             */
/* : B..... Get the source data                                               */
/* : C..... Copy the bitmap                                                   */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/* As this STUPID [CBitmap::GetBitmap((BITMAP*) o_psBitmap)] returns NULL in  */
/* <o_psBitmap.bmBits>, we cannot reuse immediatly the returned BITMAP for a  */
/* [CBitmap::CreateBitmapIndirect((BITMAP*) i_psBitmap)], we have to get the  */
/* address of the data buffer by creating it by ourself, then get the datas   */
/* with [CBitmap::GetBitmapBits((DWORD) i_nDataCount, (LPVOID) i_panDataBmp)] */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::CopyBitmap
( // Copy a bitmap
 CBitmap* o_poBitmap, // Pointer to an EXISTING but NOT INITIALIZED bitmap
 CBitmap* i_poBitmap  // Pointer to the source bitmap
 )
{ 
	BITMAP         sBmpSrc;
	unsigned char* panDataBmp;
	DWORD          nDataCount;
	BOOL           bResult;

	// Init

	// Process

	if(
		(o_poBitmap == NULL)
		|| (i_poBitmap == NULL)
		)
	{
		return false;
	}
	else
	{
		// A..... Create a remote data buffer to get the source data
		i_poBitmap->GetBitmap(&sBmpSrc);
		nDataCount = sBmpSrc.bmWidthBytes * sBmpSrc.bmHeight;
		panDataBmp = new unsigned char[nDataCount];

		if(panDataBmp != NULL)
		{
			// B..... Get the source data
			i_poBitmap->GetBitmapBits(nDataCount, panDataBmp);
			sBmpSrc.bmBits = panDataBmp; // Just complete the BITMAP structure

			// C..... Copy the bitmap
			bResult = o_poBitmap->CreateBitmapIndirect(&sBmpSrc);

			delete[] panDataBmp;
			panDataBmp = NULL;

			return bResult;
		}
		else
		{
			return false;
		}
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : ConvBitmap                                                 */
/* Role          : Convert a bitmap to a specified device context             */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error in conversion                       */
/*                          true  : Image converted                           */
/*                 OUTPUT (pointer to value)                                  */
/*                   o_poBitmap    : Image where to copy/transform            */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poBitmap    : Image to copy/transform                  */
/*                   i_poDC        : The DC to use for the conversion         */
/* Pre-condition : Both output and input pointer must be provided !           */
/*                 Also, the output bitmap MUST BE CREATED, BUT NOT           */
/*                 INITIALIZED                                                */
/* Constraints   : Do not handle mapped bitmap yet...                         */
/* Behavior      : A - Get the current DC                                     */
/*                 B - Copy the bitmap                                        */
/*                 C - Create a remote data buffer to get the source data     */
/*                 D - Get the source data                                    */
/*                 E - Initialize the bitmap                                  */
/*                 F - Convert the data                                       */
/*                 G - Copy the converted bitmap                              */
/*----------------------------------------------------------------------------*/
/* PROC CopyBitmap                                                            */
/*                                                                            */
/* [IF pointers on bitmap valids]                                             */
/* : [IF no DC provided]                                                      */
/* : : A..... Get the current DC                                              */
/* : [ELSE]                                                                   */
/* : [IF same color format]                                                   */
/* : : B..... Copy the bitmap                                                 */
/* : [ELSE]                                                                   */
/* : : C..... Create a remote data buffer to get the source data              */
/* : : [IF buffer created]                                                    */
/* : : : D..... Get the source data                                           */
/* : : : E..... Initialize the bitmap                                         */
/* : : : F..... Convert the data                                              */
/* : : : G..... Copy the converted bitmap                                     */
/* : : [ELSE]                                                                 */
/* : [ENDIF]                                                                  */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::ConvBitmap
( // Convert a bitmap to a specified device context
 CBitmap*    o_poBitmap, // Pointer to an EXISTING but NOT INITIALIZED bitmap
 CBitmap*    i_poBitmap, // Pointer to the source bitmap
 CDC*        i_poDC      // = NULL : Pointer to the DC to use for the conversion, if NULL use the current DC
 )
{ 
	BITMAP         sBmpSrc;
	BITMAPINFO     sBmpInfoSrc;
	HBITMAP        DIB;
	unsigned char* panDataBmp;
	DWORD          nDataCount;
	int            nResult;

	// Init

	panDataBmp = NULL;

	// Process

	if(
		(o_poBitmap != NULL)
		&& (i_poBitmap != NULL)
		)
	{
		if(i_poDC == NULL)
		{ // If no target device context provided, use the current one
			// A..... Get the current DC
			CClientDC oDC(this);
			i_poDC = &oDC;
		}else{}

		i_poBitmap->GetBitmap(&sBmpSrc); // Get some infos on the source bitmap

		if(sBmpSrc.bmBitsPixel == GetDeviceCaps(*i_poDC, BITSPIXEL))
		{ // If it's the same bitmap format, just copy it instead to waste some time to convert the picture (what will fail :/ )
			// B..... Copy the bitmap
			nResult = (int) CopyBitmap(o_poBitmap, i_poBitmap);
		}
		else
		{
			// C..... Create a remote data buffer to get the source data
			nDataCount = sBmpSrc.bmWidthBytes * sBmpSrc.bmHeight;
			panDataBmp = new unsigned char[nDataCount];

			if(panDataBmp != NULL)
			{
				// D..... Get the source data
				i_poBitmap->GetBitmapBits(nDataCount, panDataBmp);
				sBmpSrc.bmBits = panDataBmp;

				// E..... Initialize the bitmap

				sBmpInfoSrc.bmiColors[0].rgbBlue      = 0;
				sBmpInfoSrc.bmiColors[0].rgbGreen     = 0;
				sBmpInfoSrc.bmiColors[0].rgbRed       = 0;
				sBmpInfoSrc.bmiColors[0].rgbReserved  = 0;

				sBmpInfoSrc.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
				sBmpInfoSrc.bmiHeader.biWidth         = sBmpSrc.bmWidth;
				sBmpInfoSrc.bmiHeader.biHeight        = 0 - sBmpSrc.bmHeight; // 0 - HEIGHT : TOP-DOWN DIB, origin is upper/left corner
				sBmpInfoSrc.bmiHeader.biPlanes        = sBmpSrc.bmPlanes;
				sBmpInfoSrc.bmiHeader.biBitCount      = sBmpSrc.bmBitsPixel;
				sBmpInfoSrc.bmiHeader.biCompression   = BI_RGB;
				sBmpInfoSrc.bmiHeader.biSizeImage     = 0; // 0 : BI_RGB
				sBmpInfoSrc.bmiHeader.biXPelsPerMeter = 0;
				sBmpInfoSrc.bmiHeader.biYPelsPerMeter = 0;
				sBmpInfoSrc.bmiHeader.biClrUsed       = 0;
				sBmpInfoSrc.bmiHeader.biClrImportant  = 0;

				// F..... Convert the data
				DIB = CreateDIBitmap
					(
					i_poDC->m_hDC,
					&sBmpInfoSrc.bmiHeader,
					CBM_INIT,
					panDataBmp,
					&sBmpInfoSrc,
					DIB_RGB_COLORS
					);

				// G..... Copy the converted bitmap
				nResult = (int) CopyBitmap(o_poBitmap, CBitmap::FromHandle(DIB));

				DeleteObject(DIB);
				delete[] panDataBmp;
				panDataBmp = NULL;

				if(nResult != 0)
				{
					return true;
				}else{}
			}else{}
		}
	}else{}

	return false;
}

// *** PROGRESS INTERFACE ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetRange                                                   */
/* Role          : None                                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error while trying to change the range    */
/*                          true  : New range set                             */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nLower    : Minimum value possible                     */
/*                   i_nUpper    : Maximum value possible                     */
/*                   i_nStep     : Step increment                             */
/*                   i_bResamble : Resample the current position              */
/*                   i_bDisplay  : Display the changes                        */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Resample the postion                                   */
/*                 B - Set the new range                                      */
/*                 C - Display the new ranged progress bar                    */
/*----------------------------------------------------------------------------*/
/* PROC SetRange                                                              */
/*                                                                            */
/* A..... Resample the postion from the previous range to the new one         */
/* B..... Set the new range values                                            */
/* C..... Display the new ranged progress bar                                 */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetRange
( // Set the new range
 int  i_nLower,    // Minimum limit
 int  i_nUpper,    // Maximum limit
 int  i_nStep,     // = 1     : Step increment
 BOOL i_bResamble  // = false : Resample the current position from the new Lower and Upper values
#ifndef dCSP_TIMED_REDRAW
 ,BOOL i_bDisplay   // = true  : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{

	// Init

	// Process + Return

	if(IsWindow(GetSafeHwnd()))
	{
		ASSERT((-0x7FFFFFFF <= i_nLower) && (i_nLower <= 0x7FFFFFFF));
		ASSERT((-0x7FFFFFFF <= i_nUpper) && (i_nUpper <= 0x7FFFFFFF));
		ASSERT(i_nLower < i_nUpper); // Seems to be stupid, but avoid zero divide problem in percent calculation just below

		// A..... Resample the postion from the previous range to the new one
		if(i_bResamble != false)
		{ // Resample positions to readapt them to the new range
#ifdef dCSP_SLIDERBAR_METHOD
			// Recalculate start
			m_nLeft  = (int) (
				(
				( 
				(LONGLONG) // Turns the whole calculation in 64 bits
				(m_nLeft  - m_nLower) // Old start
				* (i_nUpper - i_nLower) // New range
				)
				/ (m_nUpper - m_nLower)     // Old range
				)
				+ i_nLower                      // New start
				);
			m_nPrevLeft  = m_nLeft;
#endif // dCSP_SLIDERBAR_METHOD

			// Recalculate end
			m_nRight = (int) (
				(
				( 
				(LONGLONG) // Turns the whole calculation in 64 bits
				(m_nRight - m_nLower) // Old end
				* (i_nUpper - i_nLower) // New range
				)
				/ (m_nUpper - m_nLower)     // Old range
				)
				+ i_nLower                      // New end 
				);
			m_nPrevRight = m_nRight;
		}else{}

		// B..... Set the new range values
		// Sets new parameters for good
		m_nLower = i_nLower;
		m_nUpper = i_nUpper;
		m_nStep  = i_nStep;

		// C..... Display the new ranged progress bar
#ifndef dCSP_TIMED_REDRAW
		if(i_bDisplay != false)
		{
			RefreshPanes();
		}else{}
#endif // dCSP_TIMED_REDRAW

		return true;
	}
	else
	{
		return false;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetPos                                                     */
/* Role          : None                                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nRight> position, 0 if error           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nPos     : Select the new position                     */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Jump to a specified position                           */
/*                 B - Display the changes                                    */
/*                 C - Return the previous position                           */
/*----------------------------------------------------------------------------*/
/* PROC SetPos                                                                */
/*                                                                            */
/* A..... Jump to a specified <i_nEnd> position                               */
/* B..... Display the changes                                                 */
/* C..... Return the previous <m_nRight> position                             */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::SetPos
( // Set <m_nRight> value
 int  i_nPos     // Set a new current position
#ifndef dCSP_TIMED_REDRAW
 ,BOOL i_bDisplay // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{

	// Init

	// Process + Return

	if(IsWindow(GetSafeHwnd()))
	{
		ASSERT((-0x7FFFFFFF <= i_nPos) && (i_nPos <= 0x7FFFFFFF));

		if(i_nPos > m_nUpper)
		{ // Avoid problems
			i_nPos = m_nUpper;
		}else{}

		if(i_nPos < m_nLower)
		{ // Avoid problems
			i_nPos = m_nLower;
		}else{}

#ifdef dCSP_SLIDERBAR_METHOD
		if(i_nPos < m_nLeft)
		{ // Avoid problems
			i_nPos = m_nLeft;
		}else{}
#endif // dCSP_SLIDERBAR_METHOD

		m_nPrevRight = m_nRight;
		// A..... Jump to a specified <i_nEnd> position
		m_nRight     = i_nPos;

		// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
		if(i_bDisplay != false)
		{
			RefreshPanes();
		}else{}
#endif // dCSP_TIMED_REDRAW

		// C..... Return the previous <m_nRight> position
		return m_nPrevRight;
	}
	else
	{
		return 0;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OffsetPos                                                  */
/* Role          : None                                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nRight> position, 0 if error           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nOffset  : Offset the current position                 */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Add an offset to the current position                  */
/*                 B - Display the changes                                    */
/*                 C - Return the previous position                           */
/*----------------------------------------------------------------------------*/
/* PROC OffsetPos                                                             */
/*                                                                            */
/* A..... Add a <i_nEnd> offset to the current <m_nRight> position            */
/* B..... Display the changes                                                 */
/* C..... Return the previous <m_nRight> position                             */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::OffsetPos
( // Forward of <i_nOffset> value
 int  i_nOffset  // Add the offset to the current position (can be negative)
#ifndef dCSP_TIMED_REDRAW
 ,BOOL i_bDisplay // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{ 

	// Init

	// Process + Return

	if(IsWindow(GetSafeHwnd()))
	{
		m_nPrevRight  = m_nRight;
		// A..... Add a <i_nEnd> offset to the current <m_nRight> position
		m_nRight     += i_nOffset;

		if(m_nRight > m_nUpper)
		{ // Avoid problems
			m_nRight = m_nUpper;
		}else{}

		if(m_nRight < m_nLower)
		{ // Avoid problems
			m_nRight = m_nLower;
		}else{}

#ifdef dCSP_SLIDERBAR_METHOD
		if(m_nRight < m_nLeft)
		{ // Avoid problems
			m_nRight = m_nLeft;
		}else{}
#endif // dCSP_SLIDERBAR_METHOD

		// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
		if(i_bDisplay != false)
		{
			RefreshPanes();
		}else{}
#endif // dCSP_TIMED_REDRAW

		// C..... Return the previous <m_nRight> position
		return m_nPrevRight;
	}
	else
	{
		return 0;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetStep                                                    */
/* Role          : None                                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nStep> setting, 0 if error             */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nStep : Set the new step increment                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Set the step increment value                           */
/*                 B - Return the previous step increment value               */
/*----------------------------------------------------------------------------*/
/* PROC SetStep                                                               */
/*                                                                            */
/* A..... Set the <m_nStep> step increment value                              */
/* C..... Return the previous <m_nStep> step increment value                  */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::SetStep
( // Set <m_nStep> value
 int i_nStep // Set the step increment
 )             
{ 
	int PrevStep;

	// Init

	// Process + Return

	if(IsWindow(GetSafeHwnd()))
	{
		PrevStep  = m_nStep;
		// A..... Set the <m_nStep> step increment value
		m_nStep   = i_nStep;

		// B..... Return the previous <m_nStep> step increment value
		return PrevStep;
	}
	else
	{
		return 0;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : StepIt                                                     */
/* Role          : None                                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nRight> position, 0 if error           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Step the position                                      */
/*                 B - Display the changes                                    */
/*                 C - Return the previous position                           */
/*----------------------------------------------------------------------------*/
/* PROC StepIt                                                                */
/*                                                                            */
/* A..... Step the <m_nRight> position from the <m_nStep> increment value     */
/* B..... Display the changes                                                 */
/* C..... Return the previous <m_nRight> position                             */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::StepIt
( // Forward of <m_nStep> value
#ifndef dCSP_TIMED_REDRAW
 BOOL i_bDisplay // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 ) 
{ 

	// Init

	// Process + Return

	if(IsWindow(GetSafeHwnd()))
	{
		m_nPrevRight  = m_nRight;
		// A..... Step the <m_nRight> position from the <m_nStep> step increment value
		m_nRight     += m_nStep;

		if(m_nRight > m_nUpper)
		{ // Avoid problems
			m_nRight = m_nUpper;
		}else{}

		if(m_nRight < m_nLower)
		{ // Avoid problems
			m_nRight = m_nLower;
		}else{}

#ifdef dCSP_SLIDERBAR_METHOD
		if(m_nRight < m_nLeft)
		{ // Avoid problems
			m_nRight = m_nLeft;
		}else{}
#endif // dCSP_SLIDERBAR_METHOD

		// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
		if(i_bDisplay != false)
		{
			RefreshPanes();
		}else{}
#endif // dCSP_TIMED_REDRAW

		// C..... Return the previous <m_nRight> position
		return m_nPrevRight;
	}
	else
	{
		return 0;
	}
}

// *** EXTENDED PROGRESS INTERFACE ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetSize                                                    */
/* Role          : Change the size of the progress bar if in pane 0           */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : NEVER                                     */
/*                          true  : ALWAYS                                    */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nSize    : The new size of the progress bar in pane 0  */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Set the new size of the progress bar                   */
/*                 B - Display the resized progress bar                       */
/*----------------------------------------------------------------------------*/
/* PROC SetSize                                                               */
/*                                                                            */
/* A..... Set the new size of the progress bar                                */
/* B..... Display the resized progress bar                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetSize
( // Set size of the progress bar in pane 0 
 int  i_nSize     // Set the size of the progress bar
#ifndef dCSP_TIMED_REDRAW
 ,BOOL i_bDisplay // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{

	// Init

	// A..... Set the new size of the progress bar
	if(i_nSize > 0)
	{ // Due to pane anti-disclosure system and progress bar contour, must readapt the size
		i_nSize += 5;
	}
	else if(i_nSize < 0)
	{
		i_nSize -= 2;
	}else{}

	m_nSize = i_nSize;

	// Process

	// B..... Display the resized progress bar
#ifndef dCSP_TIMED_REDRAW
	if(i_bDisplay != false)
	{
		RefreshPanes();
	}else{}
#endif // dCSP_TIMED_REDRAW

	// Return

	return true;
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetSize                                                    */
/* Role          : Get width or height of the progress bar                    */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : The width or height in pixel of the progress bar   */
/*                     0 : Error                                              */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Return the current <m_nSize> value                     */
/*                 B - Return the CWnd based pane size                        */
/*----------------------------------------------------------------------------*/
/* PROC SetStart                                                              */
/*                                                                            */
/* [IF progress bar in status bar]                                            */
/* : A..... Return the current <m_nSize> value                                */
/* [ELSE]                                                                     */
/* : [IF get the target pane rectangle]                                       */
/* : : B..... Return the CWnd based pane size                                 */
/* : : [IF the bar is not vertical]                                           */
/* : : : BA.... Return the width                                              */
/* : : [ELSE]                                                                 */
/* : : : BB.... Return the height                                             */
/* ; : [ENDIF]                                                                */
/* : [ELSE]                                                                   */
/* :   [ERROR]                                                                */
/* ; [ENDIF]                                                                  */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::GetSize
( // Get width or height of the progress bar in pixel, mostly used in CSkinSlider
 )
{
#ifdef dCSP_DIALOG_PROGRESS
	CRect oRectPane;

	// Init

	// Process + Return

	if(m_poWndProgress == NULL)
	{
#endif // dCSP_DIALOG_PROGRESS
		// A..... Return the current <m_nSize> value
		return m_nSize;
#ifdef dCSP_DIALOG_PROGRESS
	}
	else
	{
		// B..... Return the CWnd based pane size
		if(GetTargetRect(&oRectPane))
		{
#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{
#endif // dCSP_VERTICAL_BAR
				return oRectPane.Width();
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{
				return oRectPane.Height();
			}
#endif // dCSP_VERTICAL_BAR
		}
		else
		{
			return 0;
		}
	}
#endif // dCSP_DIALOG_PROGRESS
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetPos                                                     */
/* Role          : Get <m_nRight> value                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Current <m_nRight> position                        */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_bPercent : Return the position in percent              */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Return the current <m_nRight> position                 */
/*                 B - Return the current <m_nRight position in percent       */
/*----------------------------------------------------------------------------*/
/* PROC SetStart                                                              */
/*                                                                            */
/* [IF normal mode]                                                           */
/* : A..... Return the current <m_nRight> position                            */
/* [ELSE]                                                                     */
/* : B..... Return the current <m_nRight> position in percent                 */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/* If return in percent, 90% gives 23040, divided by 256 (>> 8) returns 90 !  */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::GetPos
( // Get <m_nRight> value
 BOOL i_bPercent // = false
 )
{
	if(i_bPercent == false)
	{
		// A..... Return the current <m_nRight> position
		return m_nRight;
	}
	else
	{
		// B..... Return the current <m_nRight> position in 8-bits fixed point percent value ('>> 8' to get the 'int' value)
		return (int) ( // 100% *256 to keep a pseudo 8-bits fixed point value (0.00390625 - 1/256 - resolution)
			(
			(LONGLONG) // Turns the whole calculation in 64 bits
			(m_nRight - m_nLower)
			* 25600
			)
			/ (m_nUpper - m_nLower)
			);
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetStep                                                    */
/* Role          : Get <m_nStep> value                                        */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Current <m_nStep> value                            */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Return the current <m_nStep> value                     */
/*----------------------------------------------------------------------------*/
/* PROC SetStart                                                              */
/*                                                                            */
/* A..... Return the current <m_nStep> value                                  */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::GetStep
( // Get <m_nStep> value
 )
{
	// A..... Return the current <m_nStep> value
	return m_nStep;
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetLower                                                   */
/* Role          : Get <m_nLower> value                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Current <m_nLower> value                           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Return the current <m_nLower> value                    */
/*----------------------------------------------------------------------------*/
/* PROC SetStart                                                              */
/*                                                                            */
/* A..... Return the current <m_nLower> value                                 */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::GetLower
( // Get <m_nLower> value
 )
{
	// A..... Return the current <m_nLower> value
	return m_nLower;
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetUpper                                                   */
/* Role          : Get <m_nUpper> value                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Current <m_nUpper> value                           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Return the current <m_nUpper> value                    */
/*----------------------------------------------------------------------------*/
/* PROC SetStart                                                              */
/*                                                                            */
/* A..... Return the current <m_nUpper> value                                 */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::GetUpper
( // Get <m_nUpper> value
 )
{
	// A..... Return the current <m_nUpper> value
	return m_nUpper;
}

#ifdef dCSP_SLIDERBAR_METHOD
/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetStart                                                   */
/* Role          : None                                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Previous <m_nLeft> position, 0 if error            */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nStart   : Select the new start position               */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Jump to a specified position                           */
/*                 B - Display the changes                                    */
/*                 C - Return the previous position                           */
/*----------------------------------------------------------------------------*/
/* PROC SetStart                                                              */
/*                                                                            */
/* A..... Jump to a specified <i_nStart> position                             */
/* B..... Display the changes                                                 */
/* C..... Return the previous <m_nLeft> position                              */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::SetStart
( // Set <m_nLeft> value
 int  i_nStart   // Set a new start position
#ifndef dCSP_TIMED_REDRAW
 ,BOOL i_bDisplay // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{

	// Init

	// Process + Return

	if(IsWindow(GetSafeHwnd()))
	{
		ASSERT((-0x7FFFFFFF <= i_nStart) && (i_nStart <= 0x7FFFFFFF));

		if(i_nStart > m_nUpper)
		{ // Avoid problems
			i_nStart = m_nUpper;
		}else{}

		if(i_nStart < m_nLower)
		{ // Avoid problems
			i_nStart = m_nLower;
		}else{}

		if(i_nStart > m_nRight)
		{ // Avoid problems
			i_nStart = m_nRight;
		}else{}

		m_nPrevLeft = m_nLeft;
		// A..... Jump to a specified <i_nEnd> position
		m_nLeft     = i_nStart;

		// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
		if(i_bDisplay != false)
		{
			RefreshPanes();
		}else{}
#endif // dCSP_TIMED_REDRAW

		// C..... Return the previous <m_nPrevLeft> position
		return m_nPrevLeft;
	}
	else
	{
		return 0;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetStart                                                   */
/* Role          : Get <m_nLeft> value                                        */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : Current <m_nLeft> position                         */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_bPercent : Return the position in percent              */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Return the current <m_nLeft> position                  */
/*                 B - Return the current <m_nLeft> position in percent       */
/*----------------------------------------------------------------------------*/
/* PROC SetStart                                                              */
/*                                                                            */
/* [IF normal mode]                                                           */
/* : A..... Return the current <m_nLeft> position                             */
/* [ELSE]                                                                     */
/* : B..... Return the current <m_nLeft> position in percent                  */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/* If return in percent, 90% gives 23040, divided by 256 (>> 8) returns 90 !  */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::GetStart
( // Get <m_nLeft> value
 BOOL i_bPercent // = false
 )
{
	if(i_bPercent == false)
	{
		// A..... Return the current <m_nLeft> position
		return m_nLeft;
	}
	else
	{
		// B..... Return the current <m_nLeft> position in 8-bits fixed point percent value ('>> 8' to get the 'int' value)
		return (int) ( // 100% *256 to keep a pseudo 8-bits fixed point value (0.00390625 - 1/256 - resolution)
			(
			(LONGLONG) // Turns the whole calculation in 64 bits
			(m_nLeft - m_nLower)
			* 25600
			)
			/ (m_nUpper - m_nLower)
			);
	}
}
#endif // dCSP_SLIDERBAR_METHOD

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : Reset                                                      */
/* Role          :  // Restart the progress bar                               */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_bDisplay : Display the changes                         */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get the creation date and time of the progress bar     */
/*                 B - Set the position of the bar to its beginning           */
/*----------------------------------------------------------------------------*/
/* PROC StepIt                                                                */
/*                                                                            */
/* A..... Get the creation date and time of the progress bar                  */
/* B..... Set the position of the bar to its beginning                        */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::Reset
( // Restart the progress bar
#ifndef dCSP_TIMED_REDRAW
 BOOL i_bDisplay // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{
	FILETIME sStartTimeFile;

	// Init

	// Process

	// A..... Get the creation date and time of the progress bar
	//  GetSystemTime(&m_sStartTimeSystem);
	GetLocalTime(&m_sStartTimeSystem);
	SystemTimeToFileTime(&m_sStartTimeSystem, &sStartTimeFile);
	m_nStartTimeLongLong = (
		(
		(LONGLONG) // Turns the whole calculation in 64 bits
		sStartTimeFile.dwHighDateTime
		<< 32
		)
		| sStartTimeFile.dwLowDateTime
		);

	// B..... Set the position of the bar to its beginning
	SetPos
		(
		m_nLower
#ifndef dCSP_TIMED_REDRAW
		,i_bDisplay
#endif // dCSP_TIMED_REDRAW
		);
}

// *** TEXT INTERFACE ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetText                                                    */
/* Role          : Change the text                                            */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : NEVER                                     */
/*                          true  : ALWAYS                                    */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poStrMessage : New text to display                     */
/*                   i_bDisplay    : Display the changes                      */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Set the new text                                       */
/*                 B - Display the new text                                   */
/*----------------------------------------------------------------------------*/
/* PROC SetText                                                               */
/*                                                                            */
/* A..... Set the new text                                                    */
/* B..... Display the changes                                                 */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetText
( // Set the new text
 LPCTSTR i_poStrMessage  // New text to display      
#ifndef dCSP_TIMED_REDRAW
 ,BOOL    i_bDisplay      // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{ 

	// Init

	// A..... Set the new text
	m_oStrMessage = i_poStrMessage;

	// Process

	// B..... Display the changes
#ifndef dCSP_TIMED_REDRAW
	if(i_bDisplay != false)
	{
		RefreshPanes();
	}else{}
#endif // dCSP_TIMED_REDRAW

	// Return

	return true;
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : SetProgress                                                */
/* Role          : Change the completion status text                          */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : NEVER                                     */
/*                          true  : ALWAYS                                    */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_nProgress : Any <eProgressSkinText> entry value        */
/*                   i_bDisplay  : Display the changes                        */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Set the new progress text                              */
/*                 B - Display the new progress text                          */
/*----------------------------------------------------------------------------*/
/* PROC SetProgress                                                           */
/*                                                                            */
/* A..... Set the new progress text                                           */
/* B..... Display the new progress text                                       */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::SetProgress
( // Set <m_nProgressText> value
 int  i_nProgress  // Set progress text
#ifndef dCSP_TIMED_REDRAW
 ,BOOL i_bDisplay   // = true : Display the changes
#endif // dCSP_TIMED_REDRAW
 )
{

	// Init

	// A..... Set the new progress text
	m_nProgressText = i_nProgress;

	// Process

	// B..... Display the resized progress bar
#ifndef dCSP_TIMED_REDRAW
	if(i_bDisplay != false)
	{
		RefreshPanes();
	}else{}
#endif // dCSP_TIMED_REDRAW

	// Return

	return true;
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetProgress                                                */
/* Role          : Get the completion status message currently used           */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   int : The current <m_nProgressText> value                */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Return the current <m_nProgressText> value             */
/*----------------------------------------------------------------------------*/
/* PROC GetProgress                                                           */
/*                                                                            */
/* A..... Return the current <m_nProgressText> value                          */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::GetProgress
( // Get <m_nProgressText> value
 )
{

	// Init

	// Process

	// Return

	// A..... Return the current <m_nProgressText> value
	return m_nProgressText;
}

// *** UPDATE PROCESS ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : RefreshPanes                                               */
/* Role          : None                                                       */
/* Type          : public                                                     */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : Error while trying to resize the text     */
/*                          true  : Process completed without error           */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get the text pane pointer and dimension                */
/*                 B - Progress pane size calculation                         */
/*                 C - Get optional message length                            */
/*                 D - Compact the text in the remaining space                */
/*                 E - Create the complete information message                */
/*                 F - Modify the progress bar size according of its size     */
/*                 G - Refresh the progress bar                               */
/*                 H - Display the complete information message               */
/*----------------------------------------------------------------------------*/
/* PROC RefreshPanes                                                          */
/*                                                                            */
/* A..... Get the text pane pointer and dimension                             */
/* [IF it is in dialog mode, in a CWnd target pane]                           */
/* : [IF there is a CWnd text pane]                                           */
/* : : AA.... Dialog CWnd text anchor                                         */
/* : [ENDIF]                                                                  */
/* [ELSE]                                                                     */
/* : [IF there is a status bar]                                               */
/* : : AB.... Status bar pane 0                                               */
/* : [ENDIF]                                                                  */
/* [ENDIF]                                                                    */
/* B..... Progress pane size calculation                                      */
/* [IF the progress bar dimension is valid]                                   */
/* : BA.... Set the size of the progress bar to the full width of its pane    */
/* : [IF there is a text pane]                                                */
/* : : BB.... Get text pane's DC                                              */
/* : : [IF text in status bar]                                                */
/* : : : BC.... Calculate the fixed size of the progress bar                  */
/* : : : [IF progress in pane 0 and have a fixed size]                        */
/* : : : : [IF size > 0]                                                      */
/* : : : : : BCA... Calculate the remaining size for the text                 */
/* : : : : [ELSE]                                                             */
/* : : : : : BCB... Calculate the size of the text to use                     */
/* : : : : [ENDIF]                                                            */
/* : : : [ELSE]                                                               */
/* : : : : BCC... Use whole width of pane 0 for the text                      */
/* : : : [ENDIF]                                                              */
/* : : [ELSE]                                                                 */
/* : : : BD.... Use all the space of the CWnd text anchor                     */
/* : : [ENDIF]                                                                */
/* : : [IF text is visble]                                                    */
/* : : : C..... Get optional message length                                   */
/* : : : [IF there is a message to add]                                       */
/* : : : : CA.... Get the length of the cSPT_PERCENT message                  */
/* : : : : [IF the message is at least TIMED]                                 */
/* : : : : : CB.... Get the length of the cSPT_TIMED message                  */
/* : : : : [ENDIF]                                                            */
/* : : : [ENDIF]                                                              */
/* : : : CC.... Get the length of the text to display                         */
/* : : : D..... Compact the text in the remaining space                       */
/* : : : [IF there is no message]                                             */
/* : : : : DA.... Compact the text according to just its length               */
/* : : : [ELSE]                                                               */
/* : : : : DB.... Compact the text minus the length of the cSPT_PERCENT       */
/* : : : [ELSE]                                                               */
/* : : : : DC.... Compact the text minus the length of the cSPT_TIMED         */
/* : : : [ELSE]                                                               */
/* : : : : DD.... Compact the text according to the remaining place           */
/* : : : : [IF the message is at least TIMED]                                 */
/* : : : : : DDA... Try to compact the text with cSPT_TIMED at first          */
/* : : : : [ELSE]                                                             */
/* : : : : : [IF the message is at least TIMED]                               */
/* : : : : : : DDB... Reset the text                                          */
/* : : : : : : DDC... Calculate the length of the three dots                  */
/* : : : : : : DDD... Compact the text minus the three doted cSPT_PERCENT     */
/* : : : : : : DDE... Indicate the message used is cRTP_PERCENT               */
/* : : : : : [ELSE]                                                           */
/* : : : : : : DDF... Indicate the message used is cSPT_TIMED                 */
/* : : : : : [ENDIF]                                                          */
/* : : : : [ENDIF]                                                            */
/* : : : [ELSE]                                                               */
/* : : : : DE.... Compact the text according to just its length               */
/* : : : [ENDIF]                                                              */
/* : : : [IF there is no message]                                             */
/* : : : : DF.... Indicate the text is completely compacted,                  */
/* : : : [ENDIF]                                                              */
/* : : : [IF there is no message]                                             */
/* : : : : DG.... Indicate that there is no text, but perhaps a message       */
/* : : : [ENDIF]                                                              */
/* : : : E..... Create the complete information message                       */
/* : : : [IF there is no message]                                             */
/* : : : : EA.... Add the cSPT_PERCENT message                                */
/* : : : [ELSE]                                                               */
/* : : : : EB.... Add the cSPT_TIMED message                                  */
/* : : : [ELSE]                                                               */
/* : : : : [IF the message is at least TIMED]                                 */
/* : : : : : EC.... Add the cSPT_PERCENT message                              */
/* : : : : [ELSE]                                                             */
/* : : : : : ED.... Add the cSPT_TIMED message                                */
/* : : : : [ENDIF]                                                            */
/* : : : [ENDIF]                                                              */
/* : : : EE.... Scrub the text to delete trailing characters                  */
/* : : : F..... Modify the progress bar size according of its size            */
/* : : : [IF there is no message]                                             */
/* : : : : FA.... Get the length of the complete text plus its margin         */
/* : : : [ENDIF]                                                              */
/* : : : [IF there is no message]                                             */
/* : : : : FB.... Get the maximum length of the text pane                     */
/* : : : [ENDIF]                                                              */
/* : : : FC.... Set the new progress bar rectangle                            */
/* : : : FD.... Move the left side of the progress bar after the text         */
/* : : : FE.... Move the right side of the text to the beginning of the bar   */
/* : : [ENDIF]                                                                */
/* : : FF.... Make sure the previous text is erased                           */
/* : [ENDIF]                                                                  */
/* : G..... Refresh the progress bar                                          */
/* : [IF the progress bar is visible]                                         */
/* : : [IF text is visble]                                                    */
/* : : : GA.... Move the progress bar                                         */
/* : : [ELSE]                                                                 */
/* : : : [IF there is no message]                                             */
/* : : : : GB.... Display the modified progress bar                           */
/* : : : [ENDIF]                                                              */
/* : : [ENDIF]                                                                */
/* : [ENDIF]                                                                  */
/* : H..... Display the complete information message                          */
/* : [IF the progress bar is visible]                                         */
/* : : [IF text is visble]                                                    */
/* : : : [IF there is no message]                                             */
/* : : : : HA.... Text in pane 0 of the status bar                            */
/* : : : [ELSE]                                                               */
/* : : : : HB.... Text in the CWnd text pane                                  */
/* : : : [ENDIF]                                                              */
/* : : [ENDIF]                                                                */
/* : [ENDIF]                                                                  */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
#define cRTP_BLOCK   (           1 << 0)
#define cRTP_NOTEXT  (cRTP_BLOCK   << 1)
#define cRTP_PERCENT (cRTP_NOTEXT  << 1)
#define cRTP_TIMED   (cRTP_PERCENT << 1)
BOOL CSkinProgress::RefreshPanes
( // Resize the text pane and the progress bar
 ) 
{
	CStatusBar* poStatusBar;
	CWnd*       poTextCWnd;
	CFont*      poFontOld;

	CString     oStrMessage;    // Working buffer for m_oStrMessage
	CString     oStrPercent;    // Percent of completion level
	CString     oStrTimed;      // Timed completion level

	int         nProgressText;  // Absolute value of <m_nProgressText>

	int         nLenMessage;    // Len of m_oStrMessage in pixel
	int         nLenPercent;    // Len of oStrPercent in pixel
	int         nLenTimed;      // Len of oStrTimed in pixel
	int         nLenMargin;     // Len of 2*' ' in pixel
	int         nLenCompact;    // Number of character packed

	int         nLenText;       // Allowed len of the text (used to wrap it)
	int         nLenPane;       // 
	int         nLenBlock;      // Flags for computation (text wrapping)

	CRect       oRectText;      // Rect of pane 0 -> oRectPane at the end of the process if m_nPane == 0
	CRect       oRectPane;      // Rect of pane m_nPane, can be also pane 0

	// Init

	poStatusBar   = NULL; // AVOID /W4 -> HA.... *STUPID COMPILER*
	poTextCWnd    = NULL; // AVOID /W4 -> BA.... *STUPID COMPILER*

	nLenPercent   = 0;    // AVOID /W4 -> DB.... *STUPID COMPILER*
	nLenTimed     = 0;    // AVOID /W4 -> DDA... *STUPID COMPILER*
	nLenText      = 0;    // AVOID /W4 -> DE.... *STUPID COMPILER*
	nLenBlock     = 0;    // AVOID /W4 -> F..... *STUPID COMPILER*

	if(m_nProgressText < 0)
	{
		nProgressText = 0 - m_nProgressText; // Make it positive
	}
	else
	{
		nProgressText = m_nProgressText;
	}

	// Process + Return


	if(IsWindow(GetSafeHwnd()))
	{
		// A..... Get the text pane pointer and dimension
#ifdef dCSP_DIALOG_PROGRESS
		if(m_poWndProgress != NULL)
		{
			// AA.... Dialog CWnd text anchor
			if(m_poWndMessage != NULL)
			{
				poTextCWnd = m_poWndMessage;
				poTextCWnd->GetWindowRect(oRectText);
			}else{}
		}
		else
		{
#endif // dCSP_DIALOG_PROGRESS
			poStatusBar = GetStatusBar();
			if(poStatusBar != NULL)
			{
				// AB.... Status bar pane 0
				poTextCWnd = poStatusBar;
				poStatusBar->GetItemRect(0, oRectText);
			}
			else
			{
				return false;
			}
#ifdef dCSP_DIALOG_PROGRESS
		}
#endif // dCSP_DIALOG_PROGRESS

		// B..... Progress pane size calculation
		if(GetTargetRect(&oRectPane))
		{
			// BA.... Set the size of the progress bar to the full width of its current pane
			nLenPane = oRectPane.Width();

			if(poTextCWnd != NULL)
			{
				// BB.... Get text pane's DC
				CClientDC oDC(poTextCWnd);
				poFontOld = oDC.SelectObject(poTextCWnd->GetFont());

#ifdef dCSP_DIALOG_PROGRESS
				if(m_poWndProgress == NULL)
				{
#endif // dCSP_DIALOG_PROGRESS
					// BC.... Calculate the fixed size of the progress bar
					if(
						(m_nPane == 0)
						&& (m_nSize != 0)
						)
					{ // If the text pane is shared with the progress bar, calculate how much space the text takes up
						if(m_nSize > 0)
						{ // Force progress bar size
							// BCA... Calculate the remaining size for the text once removed the fixed size of the progress bar
							nLenPane = oRectText.Width() - (m_nSize - 3); // Minus 3 to keep the same spacing with the others ways just below, to resize the text
							nLenText = nLenPane; // Use the remaining space left by the resized progress bar
						}
						else
						{ // Resize the progress bar if the text is too long
							// BCB... Calculate the size of the text to use (here the whole width of pane 0)
							nLenPane = oRectText.Width() + m_nSize; // *BEWARE* : m_nSize < 0 -> Get the optimal width of the progress bar before resizing it if necessary
							nLenText = oRectText.Width() - 3; // Use the whole space of pane 0, minus 3 to avoid a complete pane 0 disclosure leading to a windows bug
						}
					}
					else
					{ // Resize the text if it is too long (even in pane 0 if m_nSize == 0)
						// BCC... Use whole width of pane 0 for the text, and what remains for the progress bar
						nLenPane = 0; // Full length for the progress bar, might be resized for pane 0
						nLenText = oRectText.Width() - 3; // Use the whole space of pane 0, minus 3 to avoid a complete pane 0 disclosure leading to a windows bug
					}
#ifdef dCSP_DIALOG_PROGRESS
				}
				else
				{
					// BD.... Use all the space of the CWnd text anchor
					if(poTextCWnd != NULL)
					{ // Use the whole lenght of the CWnd text pane
						nLenText = oRectText.Width();
					}else{}
				}
#endif // dCSP_DIALOG_PROGRESS

				if(poTextCWnd->IsWindowVisible())
				{ // Redraw the text window
					// C..... Get optional message length
					if(nProgressText != cSPT_NONE)
					{ // Calculate progress text
						// CA.... Get the length of the cSPT_PERCENT message
						oStrPercent.Format
							(
							"%d%%",
							(int) // Result on 'int'
							( // From 0 to 100
							(
							(LONGLONG) // Turns the whole calculation in 64 bits
							(m_nRight - m_nLower)
							* 100
							)
							/ (m_nUpper - m_nLower)
							)
							);

						if(m_nProgressText > 0)
						{
							oStrPercent = "(" + oStrPercent + ")";
						}else{}

						oStrPercent = " " + oStrPercent;
						nLenPercent = oDC.GetTextExtent(oStrPercent).cx; // Length of Percent

						if(nProgressText >= cSPT_TIMED)
						{
							// CB.... Get the length of the cSPT_TIMED message
							GetTimed(&oStrTimed);
							nLenTimed = oDC.GetTextExtent(oStrTimed).cx; // Length of Timed
						}else{}
					}else{}

					// CC.... Get the length of the text to display
					oStrMessage = m_oStrMessage;
					nLenMessage = oDC.GetTextExtent(oStrMessage).cx; // Length of Message
					nLenMargin  = oDC.GetTextExtent(" ").cx << 1; // Text margin

					// D..... Compact the text in the remaining space
					// Dompact the text
					switch(nProgressText)
					{
					case cSPT_NONE :
						// DA.... Compact the text according to just its length
						nLenCompact = CompactText(&oStrMessage, &oDC, nLenText, nLenMargin);
						break;
					case cSPT_PERCENT :
						// DB.... Compact the text according to its length minus the length of the cSPT_PERCENT message
						nLenCompact = CompactText(&oStrMessage, &oDC, nLenText, nLenMargin + nLenPercent);
						break;
					case cSPT_TIMED :
						// DC.... Compact the text according to its length minus the length of the cSPT_TIMED message
						nLenCompact = CompactText(&oStrMessage, &oDC, nLenText, nLenMargin + nLenTimed);
						break;
					case cSPT_AUTOSIZE :
						// DD.... Compact the text according to the most usable remaining place between the text and the progress bar
						// DDA... Try to compact the text with the cSPT_TIMED message at first
						nLenCompact = CompactText(&oStrMessage, &oDC, nLenText, nLenMargin + nLenTimed);
						if(
							(nLenCompact != 0)
							&& (nLenCompact != oStrMessage.GetLength())
							)
						{ // If the message was compacted, try with Percent
							// DDB... Reset the text
							oStrMessage = m_oStrMessage; // Restore the message

							// DDC... Calculate the length of the three dots that will be displayed after the cSPT_PERCENT message
							// In case of Timed wrapping, use Percent instead, but add "..." to show that the Timed information was wrapped
							oStrPercent += "...";
							nLenPercent = oDC.GetTextExtent(oStrPercent).cx; // Length of Percent

							// DDD... Compact the text according to its length minus the length of the cSPT_PERCENT message and the added three dots
							// Dompact the Percent + "..." information
							nLenCompact = CompactText(&oStrMessage, &oDC, nLenText, nLenMargin + nLenPercent);
							// DDE... Indicate the message used is cRTP_PERCENT
							nLenBlock |= cRTP_PERCENT;
						}
						else
						{ // There was enough place to add the Timed information
							// DDF... Indicate the message used is cSPT_TIMED
							nLenBlock |= cRTP_TIMED;
						}
						break;
					default :
						// DE.... Compact the text according to just its length
						nLenCompact = CompactText(&oStrMessage, &oDC, nLenText, nLenMargin);
					}

#ifdef dCSP_DIALOG_PROGRESS
					if(m_poWndProgress == NULL)
					{
#endif // dCSP_DIALOG_PROGRESS
						// Block the text wrapping if there is nothing more to wrap
						if(
							(nLenCompact < 0)
							|| (
							(oStrMessage.GetLength() <= 4)
							&& (oStrMessage.Right(3) == "...")
							)
							)
						{
							// DF.... Indicate the text is completely compacted, 
							nLenBlock |= cRTP_BLOCK;
						}else{}

						if(oStrMessage == "")
						{
							// DG.... Indicate that there is no text, but there might be a message
							nLenBlock |= cRTP_NOTEXT;
						}else{}
#ifdef dCSP_DIALOG_PROGRESS
					}else{}
#endif // dCSP_DIALOG_PROGRESS

					// E..... Create the complete information message with the user text and the completion report
					// Add the information
					switch(nProgressText)
					{
					case cSPT_NONE :
						break;
					case cSPT_PERCENT :
						// EA.... Add the cSPT_PERCENT message
						oStrMessage += oStrPercent;
						break;
					case cSPT_TIMED :
						// EB.... Add the cSPT_TIMED message
						oStrMessage += oStrTimed;
						break;
					case cSPT_AUTOSIZE :
						if((nLenBlock & cRTP_PERCENT) != 0)
						{ // If the message was compacted, try with Percent
							// EC.... Add the cSPT_PERCENT message
							oStrMessage += oStrPercent;
						}
						else
						{ // There was enough place to add the Timed information
							// ED.... Add the cSPT_TIMED message
							oStrMessage += oStrTimed;
						}
						break;
					default :
						break;
					}

					// EE.... Scrub the text to delete trailing characters
					oStrMessage.TrimLeft();

#ifdef dCSP_DIALOG_PROGRESS
					if(m_poWndProgress == NULL)
					{
#endif // dCSP_DIALOG_PROGRESS
						// F..... Modify the progress bar size according of its size
						if(m_nPane == 0)
						{ // If the text pane is shared with the progress bar, calculate how much space the text takes up to
							nLenMessage = oDC.GetTextExtent(oStrMessage).cx; // Length of the compacted Message

							if(
								(
								(
								(nLenMessage + nLenMargin) // Lenght of Message + Margin
							> (nLenPane)                 // Lenght of remaining space for text
							)
							&& (
							(m_nSize <= 0) // Resize of the progress bar if the text is longer, also used for m_nSize == 0
							|| (
							(nLenMessage > 0)
							&& ((nLenBlock & cRTP_NOTEXT) != 0)
							)
							)
							)
							|| ((nLenBlock & cRTP_BLOCK) != 0) // Avoid paning if the text is wrapped to the max (when only "...'+information remains)
							)
							{ // *IF* the packed text is larger than the space left by the resized progress bar *AND* the resizing of the progress bar is allowed *OR* the wrapping is blocked
								// FA.... Get the length of the complete text plus its margin
								nLenPane = nLenMessage;

								if(nLenMessage > 0)
								{
									nLenPane += nLenMargin;
								}else{}
							}else{}

							if(nLenPane > oRectText.Width())
							{ // Make sure the length will stay positive
								// FB.... Get the maximum length of the text pane
								nLenPane = oRectText.Width();
							}else{}

							if(nLenPane < 2)
							{
								nLenPane = 2;
							}else{}

							// FC.... Set the new progress bar rectangle
							// This is the pane to display
							oRectPane = oRectText;

							// FD.... Move the left side of the progress bar to the right of the text
							// Move left edge of the progress bar to the right, to leave some place for the text
							oRectPane.left = nLenPane;

							// FE.... Move the right side of the text to the beginning of the bar
							oRectText.right = nLenPane;
						}else{}
#ifdef dCSP_DIALOG_PROGRESS
					}else{}
#endif // dCSP_DIALOG_PROGRESS
				}else{}

				// FF.... Make sure the previous text is erased, so display at least a space character
				if(oStrMessage == "")
				{
					oStrMessage = " "; // If no text, force the pane to be cleared
				}else{}

				// Release DC
				oDC.SelectObject(poFontOld);
			}else{}

			if(IsWindowVisible())
			{
				// G..... Refresh the progress bar
				if(
					(m_oRectPane != oRectPane)
					&& ( // Avoid pane disclosure, so that you can reopen the window and the pane won't stay closed
					(oRectPane.Width()  > 2)
					&& (oRectPane.Height() > 2)
					)
					)
				{
					// GA.... Move the progress bar
					m_oRectPane = oRectPane; // The new progress rectangle
					MoveWindow(&oRectPane);
					Invalidate(); // Display the progress bar on its new position
				}
				else
				{
					if(UpdateProgress())
					{
						// GB.... Display the modified progress bar
						Invalidate(); // Display the changes
					}else{}
				}
			}else{}

			if(poTextCWnd != NULL)
			{
				if(m_oStrPrevMessage != oStrMessage)
				{ // If the new message has changed from the previous, set it
					// H..... Display the complete information message
#ifdef dCSP_DIALOG_PROGRESS
					if(m_poWndProgress == NULL)
					{
#endif // dCSP_DIALOG_PROGRESS
						// HA.... Text in pane 0 of the status bar
						poStatusBar->SetPaneText(0, oStrMessage); // Force text in pane 0
						m_oStrPrevMessage = oStrMessage;
#ifdef dCSP_DIALOG_PROGRESS
					}
					else
					{
						if(m_poWndMessage != NULL)
						{
							// HB.... Text in the CWnd text pane
							m_poWndMessage->SetWindowText(oStrMessage);
							m_oStrPrevMessage = oStrMessage;
						}else{}
					}
#endif // dCSP_DIALOG_PROGRESS
				}else{}
			}else{}

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

// *** DATA PROCESS ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : CompactText                                                */
/* Role          : None                                                       */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   int : > 0 = Number of character packed                   */
/*                           0 = Nothing packed                               */
/*                         < 0 = Pack blocked                                 */
/*                 OUTPUT (pointer to value)                                  */
/*                   io_poString  : Packed and three-doted text               */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   io_poString  : Text to compact                           */
/*                   i_poDC       : Target DC                                 */
/*                   i_nMaxWidth  : Width in pixel the text should fits in    */
/*                   i_nAddWidth  : Additional width for some purposes        */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get the current DC                                     */
/*                 B - Remove any ending dots                                 */
/*                 C - Verify there is something to do                        */
/*                 D - Check if the text can be compacted                     */
/*                 E - Compact the string                                     */
/*                 F - Add the three dots at the end of the string            */
/*----------------------------------------------------------------------------*/
/* PROC CompactText                                                           */
/*                                                                            */
/* [IF a string is provided to process]                                       */
/* : [IF no DC specified]                                                     */
/* : : A..... Get the current DC                                              */
/* : [ENDIF]                                                                  */
/* : [IF there is ending dots]                                                */
/* : : B..... Remove any ending dots                                          */
/* : [ENDIF]                                                                  */
/* : C..... Verify there is something to do                                   */
/* : [IF something have to be done]                                           */
/* : : D..... Check if the text can be compacted                              */
/* : : [IF the text can be compacted]                                         */
/* : : : E..... Compact the string as long as it does'nt fit                  */
/* : : : D..... Add the three dots at the end of the string                   */
/* : : [ENDIF]                                                                */
/* : [ENDIF]                                                                  */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/* Quite easy and fun to use... No trailing blank characters !                */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
int CSkinProgress::CompactText
(
 CString* io_poString, 
 CDC*     i_poDC,
 int      i_nMaxWidth,
 int      i_nAddWidth
 )
{
	int     nLenString;
	int     nLenDots;
	int     nCountChar;
	TCHAR   nFirstChar;

	// Init

	if(io_poString != NULL)
	{
		if(i_poDC == NULL)
		{ // If no target device context provided, use the current one
			// A..... Get the current DC
			CClientDC oDC(this);
			i_poDC = &oDC;
		}else{}

		nLenString = io_poString->GetLength();
		if(io_poString->Right(3) == "...")
		{
			// B..... Remove any ending dots
			nLenString  -= 3;
			*io_poString = io_poString->Left(nLenString);
		}else{}

		// Process + Return

		// C..... Verify there is something to do
		if(
			(nLenString == 0) // No text
			|| ( // Enough space for the unmodified text and the margin
			(i_poDC->GetTextExtent(*io_poString, nLenString).cx + i_nAddWidth)
			<= i_nMaxWidth
			)
			)
		{
			return 0;
		}
		else
		{
			// D..... Check if the three doted first character is longer than the whole string (VERY few characters only)
			nFirstChar = io_poString->GetAt(0);
			nLenDots   = i_poDC->GetTextExtent(anThreeDots, sizeof(anThreeDots)).cx;
			if(
				i_poDC->GetTextExtent(*io_poString, nLenString).cx
				<= (i_poDC->GetTextExtent(&nFirstChar, sizeof(nFirstChar)).cx + nLenDots)
				)
			{
				return (0 - nLenString);
			}
			else
			{
				nCountChar = 0; 

				// E..... Compact the string as long as it does'nt fit in the desired space
				while( // As long as something remains *AND* the stuff is bigger than the allowed space
					(nLenString > 1) // Leading character must remain at least
					&& (
					(i_poDC->GetTextExtent(*io_poString, nLenString).cx + (nLenDots >> 1) + i_nAddWidth)
				> i_nMaxWidth
				)
				)
				{
					*io_poString = io_poString->Left(io_poString->GetLength() - 1); // BEWARE : Modify the original object
					io_poString->TrimRight();               // Kill spaces 
					nLenString  = io_poString->GetLength(); // Get the real length after trim
					nCountChar += 1;
				}

				// F..... Add the three dots at the end of the string
				*io_poString += anThreeDots;

				return nCountChar;
			}
		}
	}
	else
	{
		return 0;
	}
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : GetTimed                                                   */
/* Role          : Get a complete and accurate timed text message             */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : NEVER                                     */
/*                          true  : ALWAYS                                    */
/*                 OUTPUT (pointer to value)                                  */
/*                   o_poTimed : Timed message                                */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get current date and time                              */
/*                 B - Add start date in timed message if necessary           */
/*                 C - Add start time int timed message                       */
/*                 D - Add the time elapsed from start                        */
/*                 E - Add the current progress completion status             */
/*                 F - Add the remaining progress completion                  */
/*                 G - Get the remaining time before end                      */
/*                 H - Display the expected date and time of the end          */
/*----------------------------------------------------------------------------*/
/* PROC GetTimed                                                              */
/*                                                                            */
/* A..... Get current date and time                                           */
/* B..... Add start date in timed message if necessary                        */
/* C..... Add start time int timed message                                    */
/* D..... Add the time elapsed from start                                     */
/* E..... Add the current progress completion status                          */
/* F..... Add the remaining progress completion                               */
/* G..... Get the remaining time before end                                   */
/* H..... Display the expected date and time of the end                       */
/*----------------------------------------------------------------------------*/
/* I will in the next update recode a bit this mess because if the progress   */
/* is not linear, the end time is far from being accurate. You might see that */
/* when grabing the bottom-right corner of the main window to resize it. It   */
/* block the progress, but the timer continue to elapse, so the end time      */
/* grows fast. Then, once you release the corner, the progress restart, but   */
/* then the time to finish is quite short so the remaining time decrease very */
/* fast, instead to readapt to show a 1:1 linear timer till the end.          */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::GetTimed
(
 CString* o_poTimed
 )
{
	CString       oStrTempo;
	unsigned long nTempo;

	SYSTEMTIME    sCurrentTimeSystem;
	SYSTEMTIME    sTempoTimeSystem;
	FILETIME      sTempoTimeFile;
	LONGLONG      nTempoTimeLongLong;
	LONGLONG      nDeltaTimeLongLong;

	// Init

	// A..... Get current date and time
	//  GetSystemTime(&nCurrentTime);
	GetLocalTime(&sTempoTimeSystem);
	sCurrentTimeSystem = sTempoTimeSystem; // For the Date of End and Time of End
	SystemTimeToFileTime(&sTempoTimeSystem, &sTempoTimeFile);
	nTempoTimeLongLong = (
		(
		(LONGLONG) // Turns the whole calculation in 64 bits
		sTempoTimeFile.dwHighDateTime
		<< 32
		)
		| sTempoTimeFile.dwLowDateTime
		);

	// Process

	*o_poTimed = " ";
	if(m_nProgressText > 0)
	{
		*o_poTimed += "(";
	}else{}

	// B..... Add start date in timed message if necessary
	if( // *IF* Date of Start is different from Current Date
		(sTempoTimeSystem.wDay   != m_sStartTimeSystem.wDay)   // Check what change first, days
		|| (sTempoTimeSystem.wMonth != m_sStartTimeSystem.wMonth)
		|| (sTempoTimeSystem.wYear  != m_sStartTimeSystem.wYear)
		)
	{ // Date of Start
		oStrTempo.Format
			(
			"%04d/%02d/%02d/%02d ",
			m_sStartTimeSystem.wYear,
			m_sStartTimeSystem.wMonth,
			m_sStartTimeSystem.wDay,
			m_sStartTimeSystem.wDayOfWeek + 1
			);
		*o_poTimed += oStrTempo;
	}else{}

	// C..... Add start time in timed message
	// Time of Start
	oStrTempo.Format
		(
		"%02d:%02d:%02d - ",
		m_sStartTimeSystem.wHour,
		m_sStartTimeSystem.wMinute,
		m_sStartTimeSystem.wSecond
		);
	*o_poTimed += oStrTempo;

	// D..... Add the time elapsed from start
	// Calculate time from start
	nDeltaTimeLongLong            = nTempoTimeLongLong - m_nStartTimeLongLong;
	nTempoTimeLongLong            = nDeltaTimeLongLong; // Keep nDeltaTimeLongLong safe
	sTempoTimeFile.dwLowDateTime  = (DWORD) nTempoTimeLongLong;
	sTempoTimeFile.dwHighDateTime = (DWORD) ((LONGLONG) nTempoTimeLongLong >> 32);
	FileTimeToSystemTime(&sTempoTimeFile, &sTempoTimeSystem);
	sTempoTimeSystem.wYear       -= 1600; // Starts from 1601, so if 1601 you get year 1

	// Day from start
	if(
		(sTempoTimeSystem.wYear  != 1)
		|| (sTempoTimeSystem.wMonth != 1)
		|| (sTempoTimeSystem.wDay   != 1)
		)
	{ // If not the same day, calculate the number of days elapsed from start
		nTempo = (unsigned long) (
			nTempoTimeLongLong // Already in 64 bits, so the calculation will be done in 64 bits
			/ (nDeltaTimeLongLong / 864000000000) // Number of days given in 100 nanosecond (0.1us)
			);

		if(nTempo != 0)
		{ // If there is some days elapsed
			oStrTempo.Format("%d:", nTempo);
			*o_poTimed += oStrTempo;
		}else{}
	}else{}

	// Time from start
	oStrTempo.Format
		(
		"%02d:%02d:%02d - ",
		sTempoTimeSystem.wHour,
		sTempoTimeSystem.wMinute,
		sTempoTimeSystem.wSecond
		);
	*o_poTimed += oStrTempo;

	// E..... Add the current progress completion status
	// Elapsed percent
#ifndef dCSP_SLIDERBAR_METHOD
	nTempo = (int) ( // 100% *256 to keep a pseudo 8-bits fixed point value (0.00390625 - 1/256 - resolution)
		(
		(LONGLONG) // Turns the whole calculation in 64 bits
		(m_nRight - m_nLower)
		* 25600
		)
		/ (m_nUpper - m_nLower)
		);
#else
	nTempo = (int) ( // 100% *256 to keep a pseudo 8-bits fixed point value (0.00390625 - 1/256 - resolution)
		(
		(LONGLONG) // Turns the whole calculation in 64 bits
		(m_nRight -  m_nLeft)
		* 25600
		)
		/ (m_nUpper - m_nLower)
		);
#endif // dCSP_SLIDERBAR_METHOD
	oStrTempo.Format("%d%% / ", nTempo >> 8); // '>> 8' == '/ 256'
	*o_poTimed += oStrTempo;

	// F..... Add the remaining progress completion
	// Remaining percent
	oStrTempo.Format("%d%% - ", 100 - (nTempo >> 8));
	*o_poTimed += oStrTempo;

	// G..... Get the remaining time before end
	// Remaining time
	if(nTempo == 0)
	{ // Avoid zero divide
		nTempoTimeLongLong = 0; // No more time to wait
	}
	else
	{
		nTempoTimeLongLong = ((nDeltaTimeLongLong * 25600) / nTempo) - nDeltaTimeLongLong; // The remaining time, 100% - Elasped (100% = Elapsed / Percent)
	}
	sTempoTimeFile.dwLowDateTime  = (DWORD) nTempoTimeLongLong;
	sTempoTimeFile.dwHighDateTime = (DWORD) ((LONGLONG) nTempoTimeLongLong >> 32);
	FileTimeToSystemTime(&sTempoTimeFile, &sTempoTimeSystem);
	sTempoTimeSystem.wYear       -= 1600;

	// Day to end
	if(
		(sTempoTimeSystem.wDay   != 1) // Check what change first, days
		|| (sTempoTimeSystem.wMonth != 1)
		|| (sTempoTimeSystem.wYear  != 1)
		)
	{ // If not the same day, calculate the number of days elapsed from start
		nTempo = (unsigned long) (nDeltaTimeLongLong / 864000000000); // Number of days given in 100 nanosecond (0.1us)

		if(nTempo != 0)
		{ // If there is some days elapsed
			oStrTempo.Format("%d:", nTempo);
			*o_poTimed += oStrTempo;
		}else{}
	}else{}

	// Time to end
	oStrTempo.Format("%02d:%02d:%02d - ", sTempoTimeSystem.wHour, sTempoTimeSystem.wMinute, sTempoTimeSystem.wSecond);
	*o_poTimed += oStrTempo;

	// H..... Display the expected date and time of the end
	// Date of End
	if(nTempo == 0)
	{ // Avoid zero divide
		nTempoTimeLongLong = m_nStartTimeLongLong + nDeltaTimeLongLong; // Current time, Delta = Tempo - Start, see above
	}
	else
	{
		nTempoTimeLongLong = m_nStartTimeLongLong + ((nDeltaTimeLongLong * 25600) / nTempo); // Start + 100% (100% = Elapsed / Percent)
	}
	sTempoTimeFile.dwLowDateTime  = (DWORD) nTempoTimeLongLong;
	sTempoTimeFile.dwHighDateTime = (DWORD) ((LONGLONG) nTempoTimeLongLong >> 32);
	FileTimeToSystemTime(&sTempoTimeFile, &sTempoTimeSystem);

	if(
		(sTempoTimeSystem.wYear  != sCurrentTimeSystem.wYear)
		&& (sTempoTimeSystem.wMonth != sCurrentTimeSystem.wMonth)
		&& (sTempoTimeSystem.wDay   != sCurrentTimeSystem.wDay)
		)
	{
		oStrTempo.Format
			(
			"%04d/%02d/%02d/%02d ",
			sTempoTimeSystem.wYear,
			sTempoTimeSystem.wMonth,
			sTempoTimeSystem.wDay,
			sTempoTimeSystem.wDayOfWeek + 1
			);
		*o_poTimed += oStrTempo;
	}else{}

	// Time of End
	oStrTempo.Format
		(
		"%02d:%02d:%02d",
		sTempoTimeSystem.wHour,
		sTempoTimeSystem.wMinute,
		sTempoTimeSystem.wSecond
		);
	*o_poTimed += oStrTempo;

	if(m_nProgressText > 0)
	{
		*o_poTimed += ")";
	}else{}

	// Return

	return true;
}

// *** UPDATE PROCESS ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : UpdateProgress                                             */
/* Role          : None                                                       */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   BOOL = false : The progress bar has not changed          */
/*                          true  : The display needs to be updated           */
/*                 OUTPUT (pointer to value)                                  */
/*                   o_poRectPaint : Return the progress bar client rect      */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Get the current progress bar dimension                 */
/*                 B - Set variables for calculation                          */
/*                 C - Calculate the elements' position in pixel              */
/*                 D - Reverse the left and right element position            */
/*                 E - On progress bar change, return the need to refresh it  */
/*----------------------------------------------------------------------------*/
/* PROC UpdateProgress                                                        */
/*                                                                            */
/* A..... Get the current progress bar dimension                              */
/* B..... Set variables for calculation                                       */
/* C..... Calculate the elements' position                                    */
/*   CA.... Get the position of the end of the progress bar                   */
/*   CB.... Get the length in pixel of the prgress bar                        */
/*   CC.... Get the position of the left end of the progress bar              */
/*   [IF the lenght of the progress bar exceed the end position]              */
/*   : CD... Reset the length of the progress bar to its maximum size         */
/*   [ENDIF]                                                                  */
/*   [IF the left element is located after the right element]                 */
/*   : CE... Set the start before the end                                     */
/*   [ENDIF]                                                                  */
/* [IF the display is reversed]                                               */
/* : D..... Reverse the left and right elements position                      */
/* [ENDIF]                                                                    */
/* E..... On progress bar change, return the need to refresh it               */
/*----------------------------------------------------------------------------*/
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::UpdateProgress
( // Update of the progress bar values
 CRect* o_poRectPaint // = NULL
 ) 
{
	CRect    oRectPaint;
#ifdef dCSP_SLIDERBAR_METHOD
	int      nCurrentPos; // Pos of the current calculated position
#endif // dCSP_SLIDERBAR_METHOD
	int      nLenPos;
	int      nStepPos;

	// Init

	// Init

	// A..... Get the current progress bar dimension
	GetClientRect(&oRectPaint); // the CStatic currently being repaint
	if(o_poRectPaint != NULL)
	{
		*o_poRectPaint = oRectPaint;
	}else{}
	//  GetParent()->GetClientRect(&rcStatusBar); // For old test purpose, just stay here to recall me the syntax
	//  GetParent()->ClientToScreen(&rcStatusBar);
	//  ScreenToClient(&rcStatusBar);

	// B...... Set variables for calculation
#ifdef dCSP_VERTICAL_BAR
	if(m_bVertical == false)
	{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
		nLenPos  = oRectPaint.Width();
		nStepPos = m_nBmpWidth;
#ifdef dCSP_VERTICAL_BAR
	}
	else
	{ // Vertical bar
		nLenPos  = oRectPaint.Height();
		nStepPos = m_nBmpHeight;
	}
#endif // dCSP_VERTICAL_BAR

	// Process

	// m_nEndAff ----------------------------------------------------,
	// m_nRightAff ----------------------------,                     |
	// m_nLeftAff ---------,                   | m_nBmpWidth --,-,   |
	//                     |<------ bar ------>|               | |   |
	// ProgressBar = [ : : :(:=:=:=:=:=:=:=:=:=:): : : : : : : : : : : ]
	//               | |                                           | |
	//               | '- 0% --------- m_nRightAff --------- 100% -+-'
	//               '- 0% --------- m_nLeftAff ------------ 100% -'

	// C..... Calculate the elements' position in pixel
	// CA.... Get the position of the end of the progress bar
	m_nEndAff   = nLenPos - nStepPos; // Position of the cSPB_RIGHT element

	// CB.... Get the position of the right end of the progress bar
	m_nRightAff = (int) ( // Position of the cSPB_RIGHT element
		(
		(LONGLONG) // Turns the whole calculation in 64 bits
		(m_nRight - m_nLower)
		* (nLenPos  - (nStepPos << 1))
		)
		/ (m_nUpper - m_nLower)
		) + nStepPos;

#ifdef dCSP_SLIDERBAR_METHOD
	// CC.... Get the position of the left end of the progress bar
	m_nLeftAff  = (int) ( // Position of the cSPB_LEFT element
		(
		(LONGLONG) // Turns the whole calculation in 64 bits
		(m_nLeft - m_nLower)
		* (nLenPos - (nStepPos << 1)) 
		)
		/ (m_nUpper - m_nLower)
		);
#endif // dCSP_SLIDERBAR_METHOD

	if(m_nRightAff > m_nEndAff)
	{ // Cannot be be bigger than the bar itselves
		// CD.... Reset the length of the progress bar to its maximum size
		m_nRightAff = m_nEndAff;
	}else{}

#ifdef dCSP_SLIDERBAR_METHOD
	if(m_nLeftAff > (m_nRightAff - nStepPos))
	{ // Cannot be be bigger than the bar itselves
		// CE.... Set the start before the end
		m_nLeftAff = m_nRightAff - nStepPos;
	}else{}
#endif // dCSP_SLIDERBAR_METHOD

	// D..... Reverse the left and right elements position
#ifdef dCSP_SLIDERBAR_METHOD
	if(m_bReverse != false)
	{ // If reversing the display, just invert the positions in dCSP_SLIDERBAR_METHOD mode, TRICKY ISN'T IT !
		nCurrentPos = nLenPos - m_nRightAff - nStepPos; // Will become m_nLeftAff
		m_nRightAff = nLenPos - m_nLeftAff  - nStepPos; // m_nLeftAff  becomes m_nRightAff
		m_nLeftAff  = nCurrentPos;                      // m_nRightAff becomes m_nLeftAff
	}else{}
#endif // dCSP_SLIDERBAR_METHOD

	// Return

	// E..... On progress bar change, return the need to refresh it
	if(
		(m_nPrevEndAff   != m_nEndAff)
		|| (m_nPrevRightAff != m_nRightAff)
#ifdef dCSP_SLIDERBAR_METHOD
		|| (m_nPrevLeftAff  != m_nLeftAff)
#endif // dCSP_SLIDERBAR_METHOD
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// *** WINDOWS MAPPED PROCESSING ***

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnEraseBkgnd                                               */
/* Role          : Erase the background (with a filled rect) before [OnPaint] */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   i_poDC : current DC                                      */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Erase the background                                   */
/*                 B - Resize and display the text                            */
/*----------------------------------------------------------------------------*/
/* PROC OnEraseBkgnd                                                          */
/*                                                                            */
/* A..... Erase the background                                                */
/* B..... Resize and display the text                                         */
/*----------------------------------------------------------------------------*/
/* This function is called prior to [OnSizing] because we cannot over-ride    */
/* the status bar [OnSizing] function from here. And we also cannot do it for */
/* the main window also, which is the parent window of the status bar. So, in */
/* order to resize the text in real-time while it is moved/resized, just call */
/* the [RefreshPanes] function when the status bar is asked to be erased.     */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
BOOL CSkinProgress::OnEraseBkgnd
(
 CDC* i_poDC
 )
{
	BOOL bResult;

	// Init

	// A..... Erase the background
	bResult = CStatic::OnEraseBkgnd(i_poDC);

	// Process

	// B..... Resize and display the text
	RefreshPanes();

	// Return

	return bResult;
}

BEGIN_MESSAGE_MAP(CSkinProgress, CStatic)
	//{{AFX_MSG_MAP(CSkinProgress)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinProgress message handlers

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnPaint                                                    */
/* Role          : Repaint the progress bar according to its new size         */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   None                                                     */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Update values, get dimension and check refresh         */
/*                 B - Set variables for calculation                          */
/*                 C - Draw the progress bar                                  */
/*                 D - Display the progress bar                               */
/*----------------------------------------------------------------------------*/
/* PROC OnPaint                                                               */
/*                                                                            */
/* A..... Update values, get dimension and check refresh                      */
/* [IF the progress bar needs to be refreshed]                                */
/* : B..... Set variables for calculation                                     */
/* : C..... Draw the progress bar                                             */
/* : [IF the progress bar size is different from the previous time]           */
/* : : CA.... Redraw the complete bar from scratch                            */
/* : : [IF the progress bar was existing]                                     */
/* : : : CAA... Delete the previous progress bar bitmap                       */
/* : : [ENDIF]                                                                */
/* : : CAB... Create the new progress bar bitmap with the new dimension       */
/* : [ENDIF]                                                                  */
/* : CB.... Select the DC on the progress bar bitmap                          */
/* : [IF the progress bar size is different from the previous time]           */
/* : : CC.... Start the complete redraw of the progress bar from the end      */
/* : :   CCA... Draw the background element                                   */
/* : :     CCAA.. Draw the background element at the most right position      */
/* : :     [UNTIL the right end of the progress bar is not reached]           */
/* : :     : CCAB.. Draw background element until the right end of the bar    */
/* : :     [NEXT]                                                             */
/* : :   CCB... Save the previous position of the end of the progress bar     */
/* : :   CCC... Draw the bar element                                          */
/* : :     CCCA.. Reset the drawing position on a base of an element's width  */
/* : :     [UNTIL the beginning is not reached]                               */
/* : :     : CCCB.. Draw the bar element until the left end of the bar        */
/* : :     [NEXT]                                                             */
/* : :   CCD... Draw the center element                                       */
/* : :   [IF not under the start element]                                     */
/* : :   : CCE... Draw the before-left element                                */
/* : :   [ENDIF]                                                              */
/* : :   [IF not under the end element]                                       */
/* : :   : CCF... Draw the after-right element                                */
/* : :   [ENDIF]                                                              */
/* : :   CCG... Draw the start element                                        */
/* : :   CCH... Draw the end element                                          */
/* : :   CCI... Draw the left element of the bar                              */
/* : :   CCJ... Draw the right element of the bar                             */
/* : [ELSE]                                                                   */
/* : : CD.... Modify the moved elements without redrawing everything          */
/* : :   CDA... Start to update the progress bar from the previous position   */
/* : :   [IF going backward (Oh my God, could it be possible ?)]              */
/* : :   : CDB... Going backward                                              */
/* : :   :   CDBA.. Draw background to replace the end of the progress bar    */
/* : :   :   CDBB.. Reset drawing position on a base of an element's width    */
/* : :   :   [UNTIL the right end of the progress bar is not reached]         */
/* : :   :   : CDBC.. Draw the background until the right end of the bar      */
/* : :   :   [NEXT]                                                           */
/* : :   [ELSE]                                                               */
/* : :   : CDC... Going forward                                               */
/* : :   :   CDCA.. Draw the bar element to replace the progress bar end      */
/* : :   :   CDCB.. Reset drawing position on a base of an element's width    */
/* : :   :   [UNTIL the right end of the progress bar is not reached]         */
/* : :   :   : CDCC.. Draw the progress bar until the right end of the bar    */
/* : :   :   [NEXT]                                                           */
/* : :   [ENDIF]                                                              */
/* : :   CDD... Draw the center element                                       */
/* : :   [IF moved AND not under the start element]                           */
/* : :   : CDE... Draw the before-left element                                */
/* : :   [ENDIF]                                                              */
/* : :   [IF moved AND not under the end element]                             */
/* : :   : CDF... Draw the after-right element                                */
/* : :   [ENDIF]                                                              */
/* : :   [IF modifed by BEFORE or LEFT or CENTER]                             */
/* : :   : CDG... Draw the start element                                      */
/* : :   [ENDIF]                                                              */
/* : :   [IF modifed by CENTER or RIGHT or AFTER]                             */
/* : :   : CDH... Draw the end element                                        */
/* : :   [ENDIF]                                                              */
/* : :   [IF LEFT moved OR RIGHT too close from LEFT]                         */
/* : :   : CDI... Draw the left element of the bar                            */
/* : :   [ENDIF]                                                              */
/* : :   [IF RIGHT moved OR LEFT too close from RIGHT]                        */
/* : :   : CDJ... Draw the right element of the bar                           */
/* : :   [ENDIF]                                                              */
/* : [ENDIF]                                                                  */
/* : D..... Display the progress bar                                          */
/* [ENDIF]                                                                    */
/*----------------------------------------------------------------------------*/
/* Specific details about the dCSP_SLIDERBAR_METHOD or the dCSP_VERTICAL_BAR  */
/* are not provided because beyon the scope of the class. The code is also    */
/* fully functionnal, but is not yet intented to be used. It will surely be   */
/* a near update. But right now, anyone who wants to understand how slider    */
/* bars and/or vertical bars works just have to understand first how the      */
/* progress bars works. Hope there is enough comments for this purpose ;p     */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::OnPaint
( // On redraw event
 ) 
{
	CPaintDC oDC(this);   // Device context for painting - Do not call CStatic::OnPaint() for painting messages

	CDC      oDcProgress; // CompatibleDC
	CBitmap* poOldBitmap; // oDC's previous bitmap

	CRect    oRectPaint;
	int      nCurrentPos; // Pos of the current calculated position
	int      nStepPos;

	//  LPPAINTSTRUCT sPaintStruct;

	// Init

	// Process

	// A..... Update the progress bar elements position and get the progress bar dimension
	UpdateProgress(&oRectPaint); // the CStatic currently being repaint

	//  BeginPaint(sPaintStruct);

	// B...... Set variables for calculation
#ifdef dCSP_VERTICAL_BAR
	if(m_bVertical == false)
	{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
		nStepPos = m_nBmpWidth;
#ifdef dCSP_VERTICAL_BAR
	}
	else
	{ // Vertical bar
		nStepPos = m_nBmpHeight;
	}
#endif // dCSP_VERTICAL_BAR

	// C..... Draw the progress bar
	if( // If the SIZE (don't mind the position) of the ProgressBar has changed
		(oRectPaint.Width()  != m_oRectPaint.Width())
		|| (oRectPaint.Height() != m_oRectPaint.Height())
		)
	{ // If the progress bar size has changed
		// CA.... Redraw the complete bar from scratch
		if(m_poProgressBmp != NULL)
		{ // If the ProgressBitmap already exists, delete it and create a new one with the new dimension of the pane
			// CAA... Delete the previous progress bar bitmap
			delete m_poProgressBmp;
			m_poProgressBmp = NULL;
		}else{}

		// CAB... Create the new progress bar bitmap with the new dimension
		m_poProgressBmp = new CBitmap;
		m_poProgressBmp->CreateCompatibleBitmap(&oDC, oRectPaint.Width(), oRectPaint.Height()); // *ALWAYS* use '&oDC', *NEVER* '&oDcProgress' otherwise you'll get a monochrom image
	}else{}

	// CB.... Select the DC on the progress bar bitmap
	oDcProgress.CreateCompatibleDC(&oDC);
	poOldBitmap = oDcProgress.SelectObject(m_poProgressBmp);

	if( // If the SIZE (don't mind the position) of the ProgressBar has changed
		(oRectPaint.Width()  != m_oRectPaint.Width())
		|| (oRectPaint.Height() != m_oRectPaint.Height())
		)
	{ // If the ProgressBar has changed, redraw it completly
		// CC.... Start the complete redraw of the progress bar from the end
		// Background
		// CCA... Draw the background element
		// CCAA.. Draw the background element from the most right position
		nCurrentPos = m_nEndAff - (m_nEndAff % nStepPos);
		for(; nCurrentPos > m_nRightAff; nCurrentPos -= nStepPos)
		{
			// CCAB.. Draw the background element until the right end of the bar
#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
			}
#endif // dCSP_VERTICAL_BAR
		}

		// CCB... Save the previous position of the end of the progress bar
		// Position of the end of the bar
		m_nPrevRightAff = m_nRightAff;

		// CCC... Draw the bar element
		// Bar
		// CCCA.. Reset the drawing position on a base of the image list element's width
		nCurrentPos -= (nCurrentPos % nStepPos);
#ifndef dCSP_SLIDERBAR_METHOD
		for(; nCurrentPos >= nStepPos;   nCurrentPos -= nStepPos) // For m_nLeft-less progress bar routine
#else
		for(; nCurrentPos >= m_nLeftAff; nCurrentPos -= nStepPos)
#endif // dCSP_SLIDERBAR_METHOD
		{
			// CCCB.. Draw the bar element until the left end of the bar
#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(0, nCurrentPos), ILD_NORMAL);
			}
#endif // dCSP_VERTICAL_BAR
		}

#ifdef dCSP_SLIDERBAR_METHOD
		// Position of the beginning of the bar
		m_nPrevLeftAff = m_nLeftAff;

		// Background
		nCurrentPos -= (nCurrentPos % nStepPos);
		for(; nCurrentPos >= nStepPos; nCurrentPos -= nStepPos)
		{
#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
			}
#endif // dCSP_VERTICAL_BAR
		}
#endif // dCSP_SLIDERBAR_METHOD

		// CCD... Draw the center element
		// Center
#ifdef dCSP_VERTICAL_BAR
		if(m_bVertical == false)
		{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifndef dCSP_SLIDERBAR_METHOD
			m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(m_nRightAff >> 1, 0), ILD_NORMAL);
#else
			m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint((m_nLeftAff + m_nRightAff) >> 1, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD
#ifdef dCSP_VERTICAL_BAR
		}
		else
		{ // Vertical bar
#ifndef dCSP_SLIDERBAR_METHOD
			m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(0, m_nRightAff >> 1), ILD_NORMAL);
#else
			m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(0, (m_nLeftAff + m_nRightAff) >> 1), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD
		}
#endif // dCSP_VERTICAL_BAR

		// CCE... Draw the before-left element
		// Before
#ifdef dCSP_SLIDERBAR_METHOD
		if(m_nLeftAff > nStepPos)
		{
#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BEFORE, CPoint(m_nLeftAff - m_nBmpWidth, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BEFORE, CPoint(0, m_nLeftAff - m_nBmpHeight), ILD_NORMAL);
			}
#endif // dCSP_VERTICAL_BAR
		}else{}
#endif // dCSP_SLIDERBAR_METHOD

		// CCF... Draw the after-right element
		// After
		if(m_nRightAff < (m_nEndAff - nStepPos))
		{
#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
				m_oBarImgLst.Draw(&oDcProgress, cSPB_AFTER, CPoint(m_nRightAff + m_nBmpWidth, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
				m_oBarImgLst.Draw(&oDcProgress, cSPB_AFTER, CPoint(0, m_nRightAff + m_nBmpHeight), ILD_NORMAL);
			}
#endif // dCSP_VERTICAL_BAR
		}else{}

		// CCG... Draw the start element
		// Start
#ifdef dCSP_SLIDERBAR_METHOD
		m_oBarImgLst.Draw(&oDcProgress, cSPB_START, CPoint(0, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD

		// CCH... Draw the end element
		// End
#ifdef dCSP_VERTICAL_BAR
		if(m_bVertical == false)
		{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
			m_oBarImgLst.Draw(&oDcProgress, cSPB_END, CPoint(m_nEndAff, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
		}
		else
		{ // Vertical bar
			m_oBarImgLst.Draw(&oDcProgress, cSPB_END, CPoint(0, m_nEndAff), ILD_NORMAL);
		}
#endif // dCSP_VERTICAL_BAR

		// CCI... Draw the left element of the bar
		// Left
#ifdef dCSP_VERTICAL_BAR
		if(m_bVertical == false)
		{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifndef dCSP_SLIDERBAR_METHOD
			m_oBarImgLst.Draw(&oDcProgress, cSPB_LEFT, CPoint(0, 0), ILD_NORMAL); // For m_nLeft-less progress bar routine
#else
			m_oBarImgLst.Draw(&oDcProgress, cSPB_LEFT, CPoint(m_nLeftAff, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD
#ifdef dCSP_VERTICAL_BAR
		}
		else
		{ // Vertical bar
#ifndef dCSP_SLIDERBAR_METHOD
			m_oBarImgLst.Draw(&oDcProgress, cSPB_TOP, CPoint(0, 0), ILD_NORMAL); // For m_nLeft-less progress bar routine
#else
			m_oBarImgLst.Draw(&oDcProgress, cSPB_TOP, CPoint(0, m_nLeftAff), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD
		}
#endif // dCSP_VERTICAL_BAR

		// CCJ// Car... Draw the right element of the bar
		// Right
#ifdef dCSP_VERTICAL_BAR
		if(m_bVertical == false)
		{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
			m_oBarImgLst.Draw(&oDcProgress, cSPB_RIGHT, CPoint(m_nRightAff, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
		}
		else
		{ // Vertical bar
			m_oBarImgLst.Draw(&oDcProgress, cSPB_BOTTOM, CPoint(0, m_nRightAff), ILD_NORMAL);
		}
#endif // dCSP_VERTICAL_BAR

		m_nPrevEndAff = m_nEndAff;
		m_oRectPaint  = oRectPaint;
	}
	else
	{
		// CD.... Modify the moved elements without redrawing the complete progress bar
		if(m_nPrevRightAff != m_nRightAff)
		{
			// CDA... Start to update the progress bar from the previous position
			nCurrentPos = m_nPrevRightAff;

			if(m_nRightAff < m_nPrevRightAff)
			{ // If going backward, draw BACKGROUND and last with RIGHT
				// CDB... Going backward
				// CDBA.. Draw the background element to replace the end of the progress bar
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
				// CDBB.. Reset the drawing position on a base of the image list element's width
				nCurrentPos -= (nCurrentPos % nStepPos);
				for(; nCurrentPos > m_nRightAff; nCurrentPos -= nStepPos)
				{
					// CDBC.. Draw the background element until the right end of the bar
#ifdef dCSP_VERTICAL_BAR
					if(m_bVertical == false)
					{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
						m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
					}
					else
					{ // Vertical bar
						m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
					}
#endif // dCSP_VERTICAL_BAR

				}
			}
			else
			{ // If going forward, draw BAR and last with RIGHT
				// CDC... Going forward
				// CDCA.. Draw the progress bar element to replace the end of the progress bar
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
				// CDCB.. Reset the drawing position on a base of the image list element's width
				nCurrentPos += nStepPos;
				nCurrentPos -= (nCurrentPos % nStepPos);
				for(; nCurrentPos < m_nRightAff; nCurrentPos += nStepPos)
				{
					// CDCC.. Draw the progress bar element until the right end of the bar
#ifdef dCSP_VERTICAL_BAR
					if(m_bVertical == false)
					{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
						m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
					}
					else
					{ // Vertical bar
						m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(0, nCurrentPos), ILD_NORMAL);
					}
#endif // dCSP_VERTICAL_BAR
				}
			}
		}else{}

#ifdef dCSP_SLIDERBAR_METHOD
		if(m_nLeftAff!= m_nPrevLeftAff)
		{
			nCurrentPos = m_nPrevLeftAff;

			if(m_nLeftAff < m_nPrevLeftAff)
			{ // If going backward, draw BAR and last with LEFT
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
				nCurrentPos -= (nCurrentPos % nStepPos);
				for(; nCurrentPos > m_nLeftAff; nCurrentPos -= nStepPos)
				{
#ifdef dCSP_VERTICAL_BAR
					if(m_bVertical == false)
					{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
						m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(nCurrentPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
					}
					else
					{ // Vertical bar
						m_oBarImgLst.Draw(&oDcProgress, cSPB_BAR, CPoint(0, nCurrentPos), ILD_NORMAL);
					}
#endif // dCSP_VERTICAL_BAR
				}
			}
			else
			{ // If going forward, draw BACKGROUND and last with LEFT
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
				nCurrentPos += nStepPos;
				nCurrentPos -= (nCurrentPos % nStepPos);
				for(; nCurrentPos < m_nLeftAff; nCurrentPos += nStepPos)
				{
#ifdef dCSP_VERTICAL_BAR
					if(m_bVertical == false)
					{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
						m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(nCurrentPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
					}
					else
					{ // Vertical bar
						m_oBarImgLst.Draw(&oDcProgress, cSPB_BACKGROUND, CPoint(0, nCurrentPos), ILD_NORMAL);
					}
#endif // dCSP_VERTICAL_BAR
				}
			}
		}else{}
#endif // dCSP_SLIDERBAR_METHOD

		// CDD... Draw the center element
		if(
			(m_nPrevRightAff != m_nRightAff)
#ifdef dCSP_SLIDERBAR_METHOD
			|| (m_nPrevLeftAff  != m_nLeftAff)
#endif // dCSP_SLIDERBAR_METHOD
			)
		{
#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
#ifndef dCSP_SLIDERBAR_METHOD
				m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(m_nRightAff >> 1, 0), ILD_NORMAL);
#else
				m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint((m_nLeftAff + m_nRightAff) >> 1, 0), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
#ifndef dCSP_SLIDERBAR_METHOD
				m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(0, m_nRightAff >> 1), ILD_NORMAL);
#else
				m_oBarImgLst.Draw(&oDcProgress, cSPB_CENTER, CPoint(0, (m_nLeftAff + m_nRightAff) >> 1), ILD_NORMAL);
#endif // dCSP_SLIDERBAR_METHOD
			}
#endif // dCSP_VERTICAL_BAR
		}

		// CDE... Draw the before-left element
		// Before
#ifdef dCSP_SLIDERBAR_METHOD
		if(m_nPrevLeftAff != m_nLeftAff)
		{
			if(m_nLeftAff > nStepPos)
			{
#ifdef dCSP_VERTICAL_BAR
				if(m_bVertical == false)
				{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
					m_oBarImgLst.Draw(&oDcProgress, cSPB_BEFORE, CPoint(m_nLeftAff - nStepPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
				}
				else
				{ // Vertical bar
					m_oBarImgLst.Draw(&oDcProgress, cSPB_BEFORE, CPoint(0, m_nLeftAff - nStepPos), ILD_NORMAL);
				}
#endif // dCSP_VERTICAL_BAR
			}else{}
		}else{}
#endif // dCSP_SLIDERBAR_METHOD

		// CDF... Draw the after-right element
		// After
		if(m_nRightAff != m_nPrevRightAff)
		{
			if(m_nRightAff < (m_nEndAff - nStepPos))
			{
#ifdef dCSP_VERTICAL_BAR
				if(m_bVertical == false)
				{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
					m_oBarImgLst.Draw(&oDcProgress, cSPB_AFTER, CPoint(m_nRightAff + nStepPos, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
				}
				else
				{ // Vertical bar
					m_oBarImgLst.Draw(&oDcProgress, cSPB_AFTER, CPoint(0, m_nRightAff + nStepPos), ILD_NORMAL);
				}
#endif // dCSP_VERTICAL_BAR
			}else{}
		}else{}

		// CDG... Draw the start element
		// Start
#ifdef dCSP_SLIDERBAR_METHOD
		if(m_nPrevLeftAff  != m_nLeftAff)
#else
		if(m_nPrevRightAff != m_nRightAff) // In non-<dCSP_SLIDERBAR_METHOD> mode, only the right end can move
#endif // dCSP_SLIDERBAR_METHOD
		{
#ifdef dCSP_SLIDERBAR_METHOD
			if(m_nLeftAff  < (nStepPos << 1))
#else
			if(m_nRightAff < (nStepPos << 1)) // In non-<dCSP_SLIDERBAR_METHOD> mode, only the right end can move
#endif // dCSP_SLIDERBAR_METHOD
			{ // If the START element was over-written, refresh it
				m_oBarImgLst.Draw(&oDcProgress, cSPB_START, CPoint(0, 0), ILD_NORMAL);
			}else{}
		}else{}

		// CDH... Draw the end element
		// End
		if(m_nPrevRightAff != m_nRightAff)
		{
			if(m_nRightAff > (m_nEndAff - (nStepPos << 1)))
			{ // If the END element was over-written, refresh it
#ifdef dCSP_VERTICAL_BAR
				if(m_bVertical == false)
				{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
					m_oBarImgLst.Draw(&oDcProgress, cSPB_END, CPoint(m_nEndAff, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
				}
				else
				{ // Vertical bar
					m_oBarImgLst.Draw(&oDcProgress, cSPB_END, CPoint(0, m_nEndAff), ILD_NORMAL);
				}
#endif // dCSP_VERTICAL_BAR
			}else{}
		}else{}

		// CDI... Draw the left element of the bar
#ifdef dCSP_SLIDERBAR_METHOD
		if( // In case of slider display, the closeness of the RIGHT element from the LEFT can cause an over-write of the LEFT element with BAR and/or CENTER
			(m_nPrevLeftAff != m_nLeftAff)
			|| (m_nRightAff < (m_nLeftAff + (nStepPos << 1)))
			)
		{
			// Position of the start of the bar
			m_nPrevLeftAff = m_nLeftAff;

#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
				// Left
				m_oBarImgLst.Draw(&oDcProgress, cSPB_LEFT, CPoint(m_nLeftAff, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
				// Top
				m_oBarImgLst.Draw(&oDcProgress, cSPB_TOP, CPoint(0, m_nLeftAff), ILD_NORMAL);
			}
#endif // dCSP_VERTICAL_BAR
		}else{}
#else
		// As long as the RIGHT element is too close from the LEFT element, the CENTER element over-writes the LEFT element, thus we have to refresh the LEFT element
		if(m_nRightAff < (nStepPos << 1))
		{
#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
				// Left
				m_oBarImgLst.Draw(&oDcProgress, cSPB_LEFT, CPoint(0, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
				// Top
				m_oBarImgLst.Draw(&oDcProgress, cSPB_TOP, CPoint(0, 0), ILD_NORMAL);
			}
#endif // dCSP_VERTICAL_BAR
		}else{}
#endif // dCSP_SLIDERBAR_METHOD

		// CDJ... Draw the right element of the bar
		if( // In case of slider display, the closeness of the LEFT element from the RIGHT can cause an over-write of the RIGHT element with BAR and/or CENTER
			(m_nPrevRightAff != m_nRightAff)
#ifdef dCSP_SLIDERBAR_METHOD
			|| (m_nLeftAff> (m_nRightAff - (nStepPos << 1)))
#endif // dCSP_SLIDERBAR_METHOD
			)
		{
			// Position of the end of the bar
			m_nPrevRightAff = m_nRightAff;

#ifdef dCSP_VERTICAL_BAR
			if(m_bVertical == false)
			{ // Horizontal bar
#endif // dCSP_VERTICAL_BAR
				// Right
				m_oBarImgLst.Draw(&oDcProgress, cSPB_RIGHT, CPoint(m_nRightAff, 0), ILD_NORMAL);
#ifdef dCSP_VERTICAL_BAR
			}
			else
			{ // Vertical bar
				// Cottom
				m_oBarImgLst.Draw(&oDcProgress, cSPB_BOTTOM, CPoint(0, m_nRightAff), ILD_NORMAL);
			}
#endif // dCSP_VERTICAL_BAR
		}else{}

		if(m_nPrevEndAff != m_nEndAff)
		{
			m_nPrevEndAff = m_nEndAff;
		}else{}
	}

	// D..... Display the progress bar
	// Copy the progress bitmap each time the object have to be refreshed
#ifndef dCSP_DISPLAY_STRETCH
	oDC.BitBlt(0, 0, oRectPaint.Width(), oRectPaint.Height(), &oDcProgress, 0, 0, SRCCOPY);
#else
	CDC      oDcStretch;   // CompatibleDC
	CBitmap* poOldStretch; // oDC's previous bitmap
	BITMAP   sStretchBmp;

	oDcStretch.CreateCompatibleDC(&oDC);
	poOldStretch = oDcStretch.SelectObject(m_poStretchBmp);

	m_poStretchBmp->GetBitmap(&sStretchBmp);
	oDC.BitBlt(0, 0, sStretchBmp.bmWidth, sStretchBmp.bmHeight, &oDcStretch, 0, 0, SRCCOPY);

	oDcStretch.SelectObject(poOldStretch);
	oDcStretch.DeleteDC();
#endif // dCSP_DISPLAY_STRETCH

	// Release the DC
	oDcProgress.SelectObject(poOldBitmap);
	oDcProgress.DeleteDC();

		//if(m_poProgressBmp != NULL)
		//{ // If the ProgressBitmap already exists, delete it and create a new one with the new dimension of the pane
		//	// CAA... Delete the previous progress bar bitmap
		//	delete m_poProgressBmp;
		//	m_poProgressBmp = NULL;
		//}else{}

	//  EndPaint(sPaintStruct);
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnSizing                                                   */
/* Role          : Before things are resized                                  */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   fwSide : Edge of window to be moved                      */
/*                   pRect  : New rectangle                                   */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Resize the object                                      */
/*                 B - Resize the text                                        */
/*----------------------------------------------------------------------------*/
/* PROC OnSizing                                                              */
/*                                                                            */
/* A..... Resize the object                                                   */
/* B..... Resize the text                                                     */
/*----------------------------------------------------------------------------*/
/* In normal cases, this is NEVER called                                      */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::OnSizing
(
 UINT   fwSide, // Edge of window to be moved
 LPRECT pRect   // New rectangle
 )
{
	// A..... Resize the object
	CStatic::OnSizing(fwSide, pRect);

	// Process

	// B..... Resize the text
	RefreshPanes(); // Recalculate text layout on sizing the window
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnSize                                                     */
/* Role          : Once things were resized                                   */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   nType : Type of resizing requested                       */
/*                   cx    : Width                                            */
/*                   cy    : Height                                           */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Transmit the new dimensions                            */
/*----------------------------------------------------------------------------*/
/* PROC OnSize                                                                */
/*                                                                            */
/* A..... Transmit the new dimensions                                         */
/*----------------------------------------------------------------------------*/
/* Just over-ridden, ready to use for various purposes ;)                     */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::OnSize
(
 UINT nType, // Type of resizing requested 
 int  cx,    // Width
 int  cy     // Height
 )
{
	// A..... Transmit the new dimensions
	CStatic::OnSize(nType, cx, cy);
}

/*--- START FUNCTION HEADER --------------------------------------------------*/
/* Name          : OnTimer                                                    */
/* Role          : The object timer has elapsed                               */
/* Type          : PROTECTED                                                  */
/* Interface     : RETURN (direct value)                                      */
/*                   None                                                     */
/*                 OUTPUT (pointer to value)                                  */
/*                   None                                                     */
/*                 INPUT  (pointer to value, direct/default value)            */
/*                   nIDEvent : The Timer handle that elapsed                 */
/* Pre-condition : None                                                       */
/* Constraints   : None                                                       */
/* Behavior      : A - Refresh the text and the progress bar                  */
/*                 B - Transmit the Timer handle to the parent                */
/*----------------------------------------------------------------------------*/
/* PROC OnTimer                                                               */
/*                                                                            */
/* A..... Refresh the text and the progress bar                               */
/* B..... Transmit the Timer handle to the parent                             */
/*----------------------------------------------------------------------------*/
/* Just used to refresh the timed message when things are going too slow...   */
/* Also used when on time redrawing the progress bar, to limit to 50 refresh  */
/* per second...                                                              */
/*--- END FUNCTION HEADER ----------------------------------------------------*/
void CSkinProgress::OnTimer
( // On a Timer event
 UINT nIDEvent
 )
{
	if(nIDEvent == (UINT) this) // Use object's unique address as timer identifier
	{
		// A..... Refresh the text and the progress bar
		RefreshPanes(); // Refresh the whole stuff, each 500 ms or 20 ms if in dCSP_TIMED_REDRAW mode
	}else{}

	// B..... Transmit the Timer handle to the parent	
	CStatic::OnTimer(nIDEvent);
}
