/*****************************************************************************
*
* File :     IdFromString.cpp
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

#include "precomp_ntlcore.h"
#include <rwcore.h>
#include <rpworld.h>
// Çü¼®
// #include "memoryhandler.h"


//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

#include "debugmacros.h"

namespace RWS
{
   /**
   *
   *  Generate a fairly unique Id from a string,useful where many string
   *  compares can be rejected using the Id, where the Id's match the strings
   *  are then compared. i.e. two identical strings share the same Id, two different
   *  strings may share the same Id but it is unlikely.
   *
   *  \param cptr A pointer to a string.
   *
   *  \return Returns an Id generated from the string.
   *
   */
   RwUInt32 GenerateUIntIdFromString(const RwChar *cptr)
   {
      RWS_FUNCTION("RWS::GenerateUIntIdFromString");

      if (!cptr) RWS_RETURN(0);

      RwChar *ptr = const_cast<RwChar*>(cptr);

      RwUInt32 id = 0;

      while (*ptr != 0)
      {
         id = (id<<1) ^ (static_cast<RwUInt32>(*ptr));

         ptr ++;
      };

	  RWS_RETURN(id);
   }
}