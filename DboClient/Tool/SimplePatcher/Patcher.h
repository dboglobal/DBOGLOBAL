/*****************************************************************************
 *
 * File			: Patcher.h
 * Author		:
 * Copyright	: (аж)NTL
 * Date			:
 * Abstract		: 
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/


#pragma once


#include "patchwin.h"


//////////////////////////////////////////////////////////////////////////
//
// CPatcher
//
//////////////////////////////////////////////////////////////////////////


class CPatcher
{
public:
	static bool				s_bCancelPatch;

public:
	CPatcher( void );
	~CPatcher( void );

public:
	bool					DoPatch( CString strDestPatchPath, CString strCurFullPatchFileName );

	static LPVOID CALLBACK	CallBack( UINT Id, LPVOID lpParm );
};
