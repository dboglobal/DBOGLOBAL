/*****************************************************************************
*
* File :     IdFromString.h
*
* Abstract : Generates a fairly unique Id from a string, useful where many string
*            compares can be rejected using the Id.
*
*****************************************************************************
*
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on Criterion Software Ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software Ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

namespace RWS
{
  /**
   *
   *  Utility functions used by the Game Framework core.
   *
   */
   RwUInt32 GenerateUIntIdFromString(const RwChar *cptr);
}
