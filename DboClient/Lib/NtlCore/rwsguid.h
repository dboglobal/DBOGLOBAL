/*****************************************************************************
*
* File :     rwsguid.h
*
* Abstract : Definition of a struct holding a globally unique identifier.
*            Used to identify attribute handlers (behaviors) and resources.
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
* Copyright (c) 2003 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#if !defined(RWSGUID_H_)
#define RWSGUID_H_

namespace RWS
{
   /**
   *
   *  This struct represents an integer representation of the GUID strings
   *  used to uniquely identify RenderWare Studio database objects in the
   *  workspace. 
   *
   *  When an entity or asset is built into the stream sent to the framework,
   *  it's GUID string is converted into the format used here.
   *
   *  This servers two purposes: 
   *
   *  1) Save memory by using a single 128bit value rather than a 38 character
   *     string.
   *  2) Speed up the searching for an object by Id, by using an integer 
   *     compare instead of a string compare.
   *
   *  \note The GUID string in the workspace is of the form 
   *        "{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}" where each X represents
   *        a hexadecimal character. 
   *        E.g. "{274352f6-2c97-485b-a46f-96fe5a7acc5}"
   *
   */
   class RWSGUID
   {
   public:
      /**
      *
      *  Sets the value of GUID to 0.
      *
      */
      void Clear()
      {
         m_Data[0] = 0;
         m_Data[1] = 0;
         m_Data[2] = 0;
         m_Data[3] = 0;
      }

      /**
      *
      *  operator == for the RWSGUID type
      *
      */
      bool operator == ( const RWSGUID & Guid ) const
      {
         return (m_Data[0] == Guid.m_Data[0]
                 && m_Data[1] == Guid.m_Data[1]
                 && m_Data[2] == Guid.m_Data[2]
                 && m_Data[3] == Guid.m_Data[3]);
      }
      
      /**
      *
      * operator !=
      *
      */

      bool operator != ( const RWSGUID & Guid) const
      {
         return(!(*this == Guid));
      }

      /**
      *
      *  operator < for the RWSGUID type
      *
      */
      bool operator < ( const RWSGUID & Guid ) const
      {
         if (m_Data[0] != Guid.m_Data[0])
         {
            return (m_Data[0] < Guid.m_Data[0]);
         }
         else
         {
            if (m_Data[1] != Guid.m_Data[1])
            {
               return (m_Data[1] < Guid.m_Data[1]);
            }
            else
            {
               if (m_Data[2] != Guid.m_Data[2])
               {
                  return (m_Data[2] < Guid.m_Data[2]);
               }
               else
               {
                  return (m_Data[3] < Guid.m_Data[3]);
               }
            }
         }
      }

   public:
      RwUInt32 m_Data[4];
   };
}


///////////////////////////////////////////////////////////////////////////////
// Inline << operators so that RWSGUIDs can be used with RWS_TRACE.
#if !defined(NDEBUG)

#include "../macros/debugmacros.h"

inline std::ostream & operator << ( std::ostream & s, const RWS::RWSGUID & Guid )
{
   s << "{" << RWS_HEX(Guid.m_Data[0]) 
     << ", " << RWS_HEX(Guid.m_Data[1])
     << ", " << RWS_HEX(Guid.m_Data[2]) 
     << ", " << RWS_HEX(Guid.m_Data[3]) << "}";

   return s;
}

inline std::ostream & operator << ( std::ostream & s, const RWS::RWSGUID * pGuid )
{
   s << "{" << RWS_HEX(pGuid->m_Data[0]) 
     << ", " << RWS_HEX(pGuid->m_Data[1])
     << ", " << RWS_HEX(pGuid->m_Data[2]) 
     << ", " << RWS_HEX(pGuid->m_Data[3]) << "}";

   return s;
}

#endif // !NDEBUG


#endif // RWSGUID_H_