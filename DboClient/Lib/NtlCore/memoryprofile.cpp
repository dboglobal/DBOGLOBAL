/*****************************************************************************
*
* File :     memoryprofile.cpp
*
* Abstract : Implementation of memory profiling system.
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
#include <rwcore.h>
#include <rpworld.h>
#include "memoryhandler.h"


#ifdef RWS_MEMORY_PROFILING

//#define VERBOSE

//////////////////////////////////////////////////////////////////
// 
// System Includes
//
#include <string.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////
// 
//  RenderWare Studio Includes
//
#include "memoryprofile.h"
#include "../macros/debugmacros.h"
#include "../netstream/netstrm.h"
#include "../streamhandler/strfunc.h"

#ifdef SKY
extern "C"
{
#if defined (SN_TARGET_PS2)
   extern void *_start;
   extern void *_fbss;
   extern void *_end;
#endif

#if defined (__MWERKS__)
   extern void *_start;
   extern void *__data_start;
   extern void *__static_init;
   extern void *__static_init_end;
   extern void *_fbss;
   extern void *__bss_start;
   extern void *_end;
#endif
}
#endif

namespace 
{
   using namespace RWS;
   using namespace MemoryProfile;

   RwBool bIsOpen = FALSE; // Memory profiling is open
   
   RwBool bIsEnabled = FALSE; // Memory profiling is enabled

   RwBool bRecursionGuard = FALSE;  // Guard against unwanted recursion

   RwBool bIsConnectionLost = FALSE;   // Network connection failed, if unable to empty buffer
                                       // network is considered to have failed

   const RwUInt32 MaxStringLength = 1024;

   const char *strUnknown = "File#name#not#known.";

   const unsigned int OUTPUT_BUFFER_SIZE = (32 * 1024);

   char *outputbuffer;  // 1 extra element for termination

   unsigned int uiIndex = 0;

   TimeCallBackFunc g_TimeCallBackFunc = 0;

   RwUInt32 LastTimeDataSent = 0;

   const RwUInt32 TimeOut = 1000; // Time out in milliseconds, minimum time between sending data note
   // only checked when polled.

   RwUInt32 PacketNumber = 0;
   /*
   *
   *  Internal function used to remove the formatting from any string before sending them workspace.
   *
   */
   void ModifyString(char *ptr)
   {
      while (*ptr != 0){
         if (*ptr == ' ' || *ptr == '\t'|| *ptr == '\n') *ptr = '#';
         ptr++;
      }
   }

   /*
   *
   *  Function used by , sends memory profile data buffer back to workspace.
   *
   */
   bool _SendMemoryProfileDataBuffer(void)
   {
      RWS_ASSERT(bRecursionGuard, "Recursion guard should be true when sending data to the workspace.");

      if (outputbuffer && uiIndex > 0)
      {
         const char * const strProfile = "RWS::MemoryProfileData";
         const RwUInt32 uiProfile = (strlen (strProfile) + 1) * sizeof (char);

#ifdef _DEBUG
         // Check we don't modify uiIndex while sending the data
         //
         RwUInt32 uiIndexCheck = uiIndex + 1 ;
#endif

         // Note, NetStream::OpenForWrite can cause a malloc, this is ok as it will be
         // freed by the RwStreamClose as long as another stream isn't opened and left open
         // in between.
         //
         if (RwStream *pStream = NetStream::OpenForWrite (  
              uiProfile
            + sizeof(RwInt32)
            + uiIndex
            + 1
            + ChunkHeaderSize))
         {
            RwStreamWriteChunkHeader (pStream, strfuncsend_NamedData,
               uiProfile + sizeof(RwInt32) + uiIndex + 1);

            outputbuffer[uiIndex] = 0;   // Terminate packet with 0,0
            uiIndex++;

            RwStreamWrite (pStream, strProfile, uiProfile);

            // Packet Id, useful for debugging, note we use RwStreamWriteInt32 as there is
            // no RwStreamWriteUInt32
            //
            RwStreamWriteInt32 ( pStream,
               reinterpret_cast<const RwInt32 *>(&PacketNumber),
               sizeof(RwInt32));

            PacketNumber++;

            RWS_ASSERT(uiIndex == uiIndexCheck, "Error uiIndex modified during stream write");

            RwStreamWrite (pStream, outputbuffer, uiIndex);
            RwStreamClose (pStream, 0);

            uiIndex = 0;

            return true;
         }
      }

      return false;
   }

   /*
   *
   *  Function used by memory profiler, collects profiler data into buffer before calling 
   *  _SendMemoryProfileDataBuffer.
   *
   *  \param buffer String containing the fragment to send to the workspace.
   *
   */
   void _TransmitMemoryProfileData(const char *const buffer)
   {
      RWS_ASSERT(bRecursionGuard, "Recursion guard should be true when sending data to the workspace.");

      RwUInt32 TimeNow = 0;

      if (!outputbuffer || !g_TimeCallBackFunc)
      {
         return;
      }

      TimeNow = g_TimeCallBackFunc();

      // Check for buffer overflow
      // 
      if ( (uiIndex + (strlen(buffer) + 1)) >= OUTPUT_BUFFER_SIZE)
      {
         LastTimeDataSent = TimeNow;

         if (!_SendMemoryProfileDataBuffer())
         {
            // Disable memory profiling or this will be called again and again from
            // MemoryProfile::Comment
            //
            bIsConnectionLost = TRUE;
            RWS_WARNING(FALSE, 
               "Buffer overrun, Memory Profile data will be lost, either increase "
               "the Size of OUTPUT_BUFFER_SIZE until this warning is removed or disable"
               " memory profiling. Profiler now being disabled for this run.");
            return;
         }
      }

      strcpy(&outputbuffer[uiIndex], buffer);

      uiIndex += strlen(buffer) + 1;

      // If its been a while since we sent back profile data, send it back now.
      //
      if ( (TimeNow - LastTimeDataSent) > TimeOut)
      {
         LastTimeDataSent = TimeNow;

         _SendMemoryProfileDataBuffer();
      }
   }
}  // namespace

namespace RWS
{
   namespace MemoryProfile
   {

      RwUInt32 MallocCount = 0;
      RwUInt32 ReAllocCount = 0;
      RwUInt32 CallocCount = 0;
      RwUInt32 FreeCount = 0;

      /**
      *
      * Return the number of calls to malloc.
      *
      */
      RwUInt32 GetMallocCount(void)
      {
         return MallocCount;
      }

      /**
      *
      * Return the number of calls to Calloc.
      *
      */
      RwUInt32 GetCallocCount(void)
      {
         return CallocCount;
      }

      /**
      *
      * Return the number of calls to ReAlloc.
      *
      */
      RwUInt32 GetReAllocCount(void)
      {
         return ReAllocCount;
      }

      /**
      *
      * Return the number of calls to Free.
      *
      */
      RwUInt32 GetFreeCount(void)
      {
         return FreeCount;
      }

      /**
      *
      *  Periodically check if the memory profiling buffer needs to be sent back to the workspace.
      *
      */
      void Poll(void)
      {
         if (bRecursionGuard) return;

         bRecursionGuard = TRUE;

         RwUInt32 TimeNow = 0;

         if (!g_TimeCallBackFunc)
         {
            bRecursionGuard = FALSE;

            return;
         }

         TimeNow = g_TimeCallBackFunc();

         // If its been a while since we sent back profile data, send it back now.
         //
         if ( (TimeNow - LastTimeDataSent) > TimeOut)
         {
            LastTimeDataSent = TimeNow;

            _SendMemoryProfileDataBuffer();
         }

         bRecursionGuard = FALSE;
      }

      /**
      *  Reports information about code/data Size to workspace
      */
      void ReportCodeMetrics()
      {
#ifdef SKY
#if defined (SN_TARGET_PS2)
         Malloc(reinterpret_cast<void*>(&_start),
            reinterpret_cast<RwUInt32>(&_fbss) - reinterpret_cast<RwUInt32>(&_start),
            "ReportCodeMetrics _start", 0, RWS_MEMORY_TYPE_USER);

         Malloc(reinterpret_cast<void*>(&_fbss),
            reinterpret_cast<RwUInt32>(&_end) - reinterpret_cast<RwUInt32>(&_fbss),
            "ReportCodeMetrics _fbss", 0, RWS_MEMORY_TYPE_USER);
#endif

#if defined (__MWERKS__)
         Malloc(reinterpret_cast<void*>(&_start),
            reinterpret_cast<RwUInt32>(&__data_start) - reinterpret_cast<RwUInt32>(&_start),
            "_start to __data_start", 0, RWS_MEMORY_TYPE_USER);

         Malloc(reinterpret_cast<void*>(&__data_start),
            reinterpret_cast<RwUInt32>(&__static_init) - reinterpret_cast<RwUInt32>(&__data_start),
            "__data_start to __static_init", 0, RWS_MEMORY_TYPE_USER);

         Malloc(reinterpret_cast<void*>(&__static_init),
            reinterpret_cast<RwUInt32>(&__static_init_end) - reinterpret_cast<RwUInt32>(&__static_init),
            "__static_init to __static_init_end", 0, RWS_MEMORY_TYPE_USER);

         Malloc(reinterpret_cast<void*>(&__static_init_end),
            reinterpret_cast<RwUInt32>(&_fbss) - reinterpret_cast<RwUInt32>(&__static_init_end),
            "__static_init_end to _fbss", 0, RWS_MEMORY_TYPE_USER);

         Malloc(reinterpret_cast<void*>(&_fbss),
            reinterpret_cast<RwUInt32>(&__bss_start) - reinterpret_cast<RwUInt32>(&_fbss),
            "_fbss to __bss_start", 0, RWS_MEMORY_TYPE_USER);

         Malloc(reinterpret_cast<void*>(&__bss_start),
            reinterpret_cast<RwUInt32>(&_end) - reinterpret_cast<RwUInt32>(&__bss_start),
            "__bss_start to _end", 0, RWS_MEMORY_TYPE_USER);
#endif
#endif
      }

      /**
      *
      * \ingroup MemProfiler
      *
      *  Open Memory Profiling Module. Note calls to this module prior to opening the module will be ignored.
      *
      *  \param _TimeCallBackFunc Used to provide a GetTime function to the memory profiler module.
      *  \param StartEnabled StartEnabled Used to enable profiling as soon as the memory profiling
      *         module is opened.
      */
      void Open(TimeCallBackFunc _TimeCallBackFunc, RwBool StartEnabled)
      {
         RWS_PRE(!bIsOpen);

         g_TimeCallBackFunc = _TimeCallBackFunc;

         bIsOpen = TRUE;

         outputbuffer = NTL_NEW char[OUTPUT_BUFFER_SIZE + 1];  // *TRUE ALLOC*

         bIsEnabled = StartEnabled;

         // Note, the NTL_NEW above is reported as the memory profiler can't profile until after this point.
         // this malloc fakes a report for the NTL_NEW above, see *TRUE ALLOC*
         //
         Malloc(outputbuffer,
            OUTPUT_BUFFER_SIZE + 1, __FILE__, __LINE__, RWS_MEMORY_TYPE_RWS);
      }

      /**
      *
      *  Close Memory Profiling Module.
      *
      */
      void Close()
      {
         RWS_PRE(bIsOpen)

		 NTL_ARRAY_DELETE( outputbuffer );

         bIsEnabled = FALSE;

         bIsOpen = FALSE;
      }

      /**
      *
      * \ingroup MemProfiler
      *
      *  Enable/Disable memory profiling.
      *
      *  \param flag Flag specifies new state of profiling module, TRUE enables profiling and FALSE disable
      *  profiling.           
      *
      *  \return The previous state of the memory profiler.
      */
      RwBool Enable(RwBool flag)
      {
         RWS_PRE(bIsOpen)

         RwBool bIsEnabledOld = bIsEnabled;

         bIsEnabled = flag;

         return bIsEnabledOld;
      }

      /**
      *
      * \ingroup MemProfiler
      *
      *  Used to completely disable memory profiling when connection with workspace is lost.
      *
      */
      void ConnectionLost(void)
      {
         bIsConnectionLost = TRUE;
      }

      /**
      *
      * \ingroup MemProfiler
      *
      *  Add a alloc memory event to the memory profile.
      *
      *  \param Address Memory address returned by the calloc function.
      *  \param Size Size of the allocated memory in bytes.
      *  \param Filename String containing the name of the file of the calling function, typically generated
      *         using __FILE__.
      *
      *  \param LineNumber Line number of the file containing the calling function, typically generated using
      *         __LINE__.
      *
      *  \param UserTypeNum User type is used to broadly identify the area of code calling this function see
      *  RWS_MEMORY_TYPE in memoryhandler.h
      *
      */
      void Malloc(const void *Address,
         RwUInt32 Size,
         const char *Filename,
         RwUInt32 LineNumber,
         RwUInt32 userTypeNum)
      {  
         MallocCount++;

         if (bRecursionGuard) return;

         bRecursionGuard = TRUE;

#ifdef VERBOSE
         RWS_DEBUGSTREAM(  "MALLOC  " << 
                           RWS_HEX(Address) << 
                           " " << 
                           RWS_VALIDSTRING(Filename) << 
                           " Line " << 
                           LineNumber << 
                           " Size " << 
                           Size << std::endl);
#endif

         if (!bIsEnabled || bIsConnectionLost) 
         {
            bRecursionGuard = FALSE;

            return;
         }

         if (!bIsOpen) 
         {
            bRecursionGuard = FALSE;

            return;
         }

         char tbuffer[MaxStringLength];

         if (Filename) 
         {
            strcpy(tbuffer, Filename);

            ModifyString(tbuffer);
         }
         else
         {
            strcpy(tbuffer, strUnknown);
         }

         char buffer[MaxStringLength];

         sprintf( buffer,
            "A %x %x %s %u %u",
            reinterpret_cast<RwUInt32>(Address),
            Size, tbuffer, LineNumber, userTypeNum);

         RWS_ASSERT(strlen(buffer) < (MaxStringLength - 1), "Error increase size of MaxStringLength");

         _TransmitMemoryProfileData(buffer);
   
         bRecursionGuard = FALSE;
      }

      /**
      *
      * \ingroup MemProfiler
      *
      *  Add a calloc memory event to the memory profile.
      *
      *  \param Address Memory address returned by the calloc function.
      *  \param Size Size of the allocated memory in bytes.
      *  \param Filename String containing the name of the file of the calling function, typically generated
      *  using __FILE__.
      *
      *  \param LineNumber Line number of the file containing the calling function, typically generated
      *  using __LINE__.
      *
      *  \param UserTypeNum User type is used to broadly identify the area of code calling this function
      *  see RWS_MEMORY_TYPE in memoryhandler.h
      *
      */
      void Calloc(   const void *Address,
         const RwUInt32 Size,
         const char *Filename,
         const RwUInt32 LineNumber,
         const RwUInt32 UserTypeNum)
      {
         CallocCount++;

         if (bRecursionGuard) return;

         bRecursionGuard = TRUE;

#ifdef VERBOSE
         RWS_DEBUGSTREAM(  "CALLOC  " << 
                           RWS_HEX(Address) << 
                           " " << 
                           RWS_VALIDSTRING(Filename) << 
                           " Line " << 
                           LineNumber << 
                           " Size " << 
                           Size << std::endl);
#endif

         if (!bIsEnabled || bIsConnectionLost) 
         {
            bRecursionGuard = FALSE;
            return;
         }

         if (!bIsOpen) 
         {
            bRecursionGuard = FALSE;
            return;
         }

         char tbuffer[MaxStringLength];

         if (Filename) 
         {
            strcpy(tbuffer, Filename);

            ModifyString(tbuffer);
         }
         else
         {
            strcpy(tbuffer, strUnknown);
         }

         char buffer[MaxStringLength];

         sprintf( buffer,
            "C %x %x %s %u %u",
            reinterpret_cast<RwUInt32>(Address),
            Size, tbuffer, LineNumber, UserTypeNum);

         RWS_ASSERT(strlen(buffer) < (MaxStringLength - 1), "Error increase size of MaxStringLength");

         _TransmitMemoryProfileData(buffer);

         bRecursionGuard = FALSE;
      }

      /**
      *
      * \ingroup MemProfiler
      *
      *  Add a realloc memory event to the memory profile.
      *
      *  \param OrgAddress Original memory address, typically address passed to realloc function.
      *  \param Address Memory address returned by the realloc function.
      *  \param Size Size of the reallocated memory in bytes.
      *  \param Filename String containing the name of the file of the calling function, typically
      *         generated using __FILE__.
      *
      *  \param LineNumber Line number of the file containing the calling function, typically
      *         generated using __LINE__.
      *
      *  \param UserTypeNum User type is used to broadly identify the area of code calling this
      *  function see RWS_MEMORY_TYPE in memoryhandler.h
      *
      */
      void Realloc(  const void *OrgAddress,
         const void *Address,
         const RwUInt32 Size,
         const char *Filename,
         const RwUInt32 LineNumber,
         const RwUInt32 UserTypeNum)
      {
         ReAllocCount++;

         if (bRecursionGuard) return;

         bRecursionGuard = TRUE;

#ifdef VERBOSE
         RWS_DEBUGSTREAM(  "REALLOC " << 
                           RWS_HEX(OrgAddress) << 
                           " " << 
                           RWS_HEX(Address) << 
                           " " << 
                           RWS_VALIDSTRING(Filename) << 
                           " Line " << 
                           LineNumber << 
                           " Size" 
                           << Size << std::endl);
#endif

         if (!bIsEnabled || bIsConnectionLost)
         {
            bRecursionGuard = FALSE;
            return;
         }

         if (!bIsOpen)
         {
            bRecursionGuard = FALSE;
            return;
         }

         char tbuffer[MaxStringLength];

         if (Filename) 
         {
            strcpy(tbuffer, Filename);

            ModifyString(tbuffer);
         }
         else
         {
            strcpy(tbuffer, strUnknown);
         }

         RWS_ASSERT(strlen(tbuffer) < (MaxStringLength - 1), "Error increase size of MaxStringLength");

         char buffer[MaxStringLength];

         sprintf( buffer,
            "R %x %x %x %s %u %u",
            reinterpret_cast<RwUInt32>(OrgAddress),
            reinterpret_cast<RwUInt32>(Address),
            Size, tbuffer, LineNumber, UserTypeNum);

         RWS_ASSERT(strlen(buffer) < (MaxStringLength - 1), "Error increase size of MaxStringLength");

         _TransmitMemoryProfileData(buffer);

         bRecursionGuard = FALSE;
      }

      /**
      *
      * \ingroup MemProfiler
      *
      *  Add a free memory event to the memory profile.
      *
      *  \param Address Pointer to the address being freed.
      *
      */
      void Free(const void *Address)
      {
         FreeCount++;

         if (bRecursionGuard) return;

         bRecursionGuard = TRUE;

#ifdef VERBOSE
         RWS_DEBUGSTREAM(  "FREE    " << 
                           RWS_HEX(Address) << 
                           " " << std::endl);
#endif

         if (!bIsEnabled || bIsConnectionLost)
         {
            bRecursionGuard = FALSE;
            return;
         }

         if (!bIsOpen) 
         {
            bRecursionGuard = FALSE;
            return;
         }

         char buffer[MaxStringLength];

         sprintf(buffer, "F %x", reinterpret_cast<RwUInt32>(Address));

         RWS_ASSERT(strlen(buffer) < (MaxStringLength - 1), "Error increase size of MaxStringLength");

         _TransmitMemoryProfileData(buffer);

         bRecursionGuard = FALSE;
      }

      /**
      *
      *  Add a comment memory event to the memory profile.
      *
      *  \param CommentText A string containing the comment.
      *
      */
      void Comment(const char *CommentText)
      {
         if (bRecursionGuard) return;

         bRecursionGuard = TRUE;

         if (!bIsEnabled || bIsConnectionLost) 
         {
            bRecursionGuard = FALSE;

            return;
         }

         if (!bIsOpen) 
         {
            bRecursionGuard = FALSE;

            return;
         }

         char buffer[MaxStringLength];

         sprintf(buffer, "# %s",CommentText);

         RWS_ASSERT(strlen(buffer) < (MaxStringLength - 1), "Error increase size of MaxStringLength");

         ModifyString(buffer);

         _TransmitMemoryProfileData(buffer);

         bRecursionGuard = FALSE;
      }
   }  // namespace memoryprofile
}  // namespace RWS

#endif
