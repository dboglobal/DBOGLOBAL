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
	CPatcher( void );
	~CPatcher( void );

public:
	bool					DoPatch( CString strFullPatchFileName, CString strDestPatchPath );

protected:
	static LPVOID CALLBACK	CallBack( UINT Id, LPVOID lpParm );
};
