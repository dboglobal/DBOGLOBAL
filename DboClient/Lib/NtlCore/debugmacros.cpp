/*****************************************************************************
 *
 * File :     DebugMacros.cpp
 *
 * Abstract : Support for RWS debug macros.
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
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/
#include "precomp_ntlcore.h"
#include "debugmacros.h"

// #ifndef NDEBUG

namespace RWS 
{
   namespace 
   {
      int CallDepth = 0;
   }

   void _IncCallDepth() 
   {
	   ++CallDepth; 
   }
   void _DecCallDepth()
   {
	   --CallDepth; 
   }
   int _GetCallDepth() 
   {
	   return CallDepth; 
   }

   void _IndentToCallDepth()
   {
      for ( int i = 0; i < CallDepth; ++i )
      {
         RWS_DEBUGSTREAM(".");
      }
   }
}

// #endif // #ifndef NDEBUG

#ifdef RWS_TRACE_METRICS
namespace RWS 
{
   namespace 
   {
      const unsigned int OUTPUT_BUFFER_SIZE = (64 * 1024);
      
      RwChar outputbuffer[OUTPUT_BUFFER_SIZE + 1];  // 1 extra element for termination

      unsigned int uiIndex = 0;
   }

   /*
   *
   *  Function used by RWS_SEND_TRACE_METRICS macro, sends metrics data buffer back to workspace.
   *
   */
   void _SendMetricsDataBuffer(void)
   {
      RWS_FUNCTION("RWS::_SendMetricsDataBuffer");

      if (uiIndex > 0)
      {
         const RwChar MetricsData[] = RWSTRING ("RWS::MetricsData");
         RwUInt32 nMessageSize = (rwstrlen (MetricsData) + 1) * sizeof (RwChar);

         outputbuffer[uiIndex] = 0;   // Terminate packet with 0,0
         uiIndex++;

#ifdef RWS_MEMORY_PROFILING
         // Open for write can cause a memory allocation, if it does it is released when 
         // RwStreamClose is called, so it is ok to leave this out of the memory profile.
         //
         RwBool flag = MemoryProfile::Enable(FALSE);
#endif

         if (RwStream *pStream = NetStream::OpenForWrite (
                                      nMessageSize + uiIndex + ChunkHeaderSize))
         {
            RwStreamWriteChunkHeader (pStream, strfuncsend_NamedData,
                                      nMessageSize + uiIndex);
            RwStreamWrite (pStream, MetricsData, nMessageSize);
            RwStreamWrite (pStream, outputbuffer, uiIndex);
            RwStreamClose (pStream, 0);
         }

#ifdef RWS_MEMORY_PROFILING
         MemoryProfile::Enable(flag);
#endif

         uiIndex = 0;
      }

      RWS_RETURNVOID();
   }

   /*
   *
   *  Function used by RWS_TRACE_METRIC macro, collects metrics data buffer.
   *
   *  \param pName A pointer to a string containing the name of the metric, this name is used to
   *  identify the metric in the workspace.
   *
   *  \param uiTime Time stamp of metrics data, X axis of Metrics view in Workspace.
   *
   *  \param rVar Value of metric, Y axis of Metrics view in Workspace.
   *
   */
   void _TransmitMetricsData(const RwChar *const pName, RwUInt32 uiTime, RwReal rVar)
   {
      RWS_FUNCTION("RWS::_TransmitMetricsData");

      const unsigned int TEMP_BUFFER_SIZE = 255;

      RwChar buffer[TEMP_BUFFER_SIZE];

      const RwChar *ptr = pName;

      while (*ptr != 0)
      {
         if (*ptr == ' ' || *ptr == '\t') 
         {
            RWS_ASSERTFAIL("RWS_TRACE_METRIC, name cannot contain tabs or spaces.");
         }

         ptr++;
      }

      sprintf(buffer, "%s %d %f", pName, uiTime, rVar);

      RWS_ASSERT((rwstrlen(buffer) < TEMP_BUFFER_SIZE),
         "RWS_TRACE_METRIC, TEMP_BUFFER_SIZE overrun, increase the size of TEMP_BUFFER_SIZE");

      if ( (uiIndex + (rwstrlen(buffer) + 1)) >= OUTPUT_BUFFER_SIZE)
      {
         _SendMetricsDataBuffer();
      }

      rwstrcpy(&outputbuffer[uiIndex], buffer);

      uiIndex += rwstrlen(buffer) + 1;

      RWS_RETURNVOID();
   }
}
#endif


#ifdef RWS_FUNCTIONPROFILE
namespace RWS 
{

   const unsigned int PROFILE_BUFFER_SIZE = (2024);
   char profiledatabuffer[PROFILE_BUFFER_SIZE + 1];  // 1 extra element for termination
   const char ProfileData[] = "RWS::ProfilerData";
   unsigned int uiProfileIndex = 0;

   /**
   *
   * Function used to send profiler data back to the workspace.
   *
   */
   void _SendProfileDataBuffer(const RwUInt32  id,
                               const RwBool   HasChild,
                               const RwUInt32  fps,
                               const RwReal  totaltime,
                               const char*  pParent,
                               const char*  pFunction, 
                               const RwUInt32  Calls, 
                               const RwReal  time,
                               char* pFileName,
                               const RwUInt32 LineNumber,
                               const RwReal  childTime)
   {
      RWS_FUNCTION("RWS::_SendProfileDataBuffer");

      RwUInt32 nMessageSize = (rwstrlen (ProfileData) + 1) * sizeof (char);

#ifdef SKY
      const char PlatformId[] = "PS2";
#endif

#ifdef _XBOX
      const char PlatformId[] = "Xbox";
#endif

#ifdef _WINDOWS
#if defined(D3D8_DRVMODEL_H) || defined(D3D9_DRVMODEL_H)
      const char PlatformId[] = "DirectX";
#endif

#ifdef OPENGL_DRVMODEL_H
      const char PlatformId[] = "OpenGL";
#endif
#endif

#ifdef DOLPHIN
      const char PlatformId[] = "GameCube";
#endif
      
      RwUInt32 i = 0;
      char pTempFileName[1000];
      rwstrcpy (pTempFileName, pFileName);

      // Because a file name can contain spaces  we need to replace those spaces with another character
      // or the string will get passed incorrectly by the profiler control.
      for (i = 0; i < rwstrlen(pTempFileName); i++ )
      {
         if (pTempFileName[i] == ' ')
         {
            pTempFileName[i] = '#';
         }
      }

      char pTempFunctionName[1000];
      rwstrcpy (pTempFunctionName,  pFunction);

      // The function name string passed into RWS_FUNCTION could contain spaces, so before sending the
      // data to the workspace, go through the string and put a '#' in their place.
      for ( i = 0; i < rwstrlen(pTempFunctionName); i++ )
      {
         if (pTempFunctionName[i] == ' ')
         {
            pTempFunctionName[i] = '#';
         }
      }

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	  sprintf_s (profiledatabuffer, PROFILE_BUFFER_SIZE + 1, "RwsChildData %d %d %f %s %s %d %f %s %d %d %f", 
               id, fps, totaltime, pParent, pFunction, Calls, time, pTempFileName, LineNumber, HasChild, childTime); 
#else
      sprintf (profiledatabuffer, "RwsChildData %d %d %f %s %s %d %f %s %d %d %f", 
               id, fps, totaltime, pParent, pFunction, Calls, time, pTempFileName, LineNumber, HasChild, childTime); 
#endif

      uiProfileIndex = rwstrlen(profiledatabuffer) + 1;

#ifdef RWS_MEMORY_PROFILING
      // Open for write can cause a memory allocation, if it does it is released when 
      // RwStreamClose is called, so it is ok to leave this out of the memory profile.
      //
      RwBool flag = MemoryProfile::Enable(FALSE);
#endif

//      if (RwStream *pStream = NetStream::OpenForWrite (nMessageSize + uiProfileIndex + ChunkHeaderSize))
//      {
//         RwStreamWriteChunkHeader (pStream, strfuncsend_NamedData,nMessageSize + uiProfileIndex);
//         RwStreamWrite (pStream, ProfileData, nMessageSize);
//         RwStreamWrite (pStream, profiledatabuffer, uiProfileIndex);
//         RwStreamClose (pStream, 0);
 //     }

#ifdef RWS_MEMORY_PROFILING
      MemoryProfile::Enable(flag);
#endif

      RWS_RETURNVOID();
   }


   /**
   *
   *  Function used to send a flag to the workspace informing that a new child is on its way, and to reset the 
   *  profile node list.  This is needed because data sent to the workspace is buffered up to save on sending 
   *  excess network traffic.  If the buffer is partially full when a new child is sent,  some data from a previous
   *  child will get sent and give incorrect readings in the workspace.
   *
   */
   void _SendProfileNewChildFlag( void )
   {
      RWS_FUNCTION("RWS::_SendProfileDataBuffer");

      RwUInt32 nMessageSize = (rwstrlen (ProfileData) + 1) * sizeof (char);

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
      sprintf_s (profiledatabuffer, PROFILE_BUFFER_SIZE + 1, "RwsNewChild"); 
#else
	  sprintf (profiledatabuffer, "RwsNewChild"); 
#endif

      uiProfileIndex = rwstrlen(profiledatabuffer) + 1;

#ifdef RWS_MEMORY_PROFILING
      // Open for write can cause a memory allocation, if it does it is released when 
      // RwStreamClose is called, so it is ok to leave this out of the memory profile.
      //
      RwBool flag = MemoryProfile::Enable(FALSE);
#endif

//      if (RwStream *pStream = NetStream::OpenForWrite (nMessageSize + uiProfileIndex + ChunkHeaderSize))
//      {
//         RwStreamWriteChunkHeader (pStream, strfuncsend_NamedData,nMessageSize + uiProfileIndex);
//         RwStreamWrite (pStream, ProfileData, nMessageSize);
//         RwStreamWrite (pStream, profiledatabuffer, uiProfileIndex);
//         RwStreamClose (pStream, 0);
//      }

#ifdef RWS_MEMORY_PROFILING
      MemoryProfile::Enable(flag);
#endif

      RWS_RETURNVOID();
   }
}
   
#endif


