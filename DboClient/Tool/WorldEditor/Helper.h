//*****************************************************************************
// File       : Helper.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __HELPER_H__
#define __HELPER_H__

#include "BaseType.h"

RwBool		CreateCone( SImediateRenderInfo * pRenderInfo, RwV2d * pSize, RwUInt32 iDirection = 1 );
RwBool		RenderCone( SImediateRenderInfo * pRenderInfo, RwMatrix * pMatrix );

#endif //__HELPER_H__

//*****************************************************************************
//
// End of File : Helper.h
//
//*****************************************************************************