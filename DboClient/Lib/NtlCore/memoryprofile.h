/*****************************************************************************
*
* File :     memoryprofile.h
*
* Abstract : Interface definition of memory profiling system.
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

#ifndef __MEMORYPROFILE_H_
#define __MEMORYPROFILE_H_

/**
*
*  \ingroup MemProfiler
*
*  \page MemProfilerOverview Overview
*
*  The Memory Profile module hooks into the memory handler and feeds back data relating to dynamic memory
*  allocations to the workspace. This data can be visualized using the RWSMemoryProfiler control in the 
*  workspace. Line and file number information is passed back to the workspace if available, \see RWS_NEW.
*
*  \note The profiler needs to send a continuous stream of data to the workspace from startup, disconnecting
*  will interrupt this stream. Any subsequent data cannot be interpreted by the workspace. Therefore on a 
*  disconnect the memory profiler stops sending data.
*/

namespace RWS
{
   /**
   *
   * \ingroup MemProfiler
   *
   * Memory Profiling.
   */
   namespace MemoryProfile
   {
      typedef RwUInt32 (*TimeCallBackFunc)(void); 

#ifdef RWS_MEMORY_PROFILING

      RwUInt32 GetMallocCount(void);
      RwUInt32 GetCallocCount(void);
      RwUInt32 GetReAllocCount(void);
      RwUInt32 GetFreeCount(void);

      void Open(TimeCallBackFunc _TimeCallBackFunc, RwBool StartEnabled);

      void Close();

      RwBool Enable(RwBool flag);

      void ConnectionLost(void);

      void Poll();

      void Malloc(   const void *Address,
                     const RwUInt32 Size,
                     const char *Filename,
                     const RwUInt32 LineNumber,
                     const RwUInt32 UserTypeNum);
      
      void Calloc(   const void *Address,
                     const RwUInt32 Size,
                     const char *Filename,
                     const RwUInt32 LineNumber,
                     const RwUInt32 UserTypeNum);

      void Realloc(  const void *OrgAddress,
                     const void *Address,
                     const RwUInt32 Size,
                     const char *Filename,
                     const RwUInt32 LineNumber,
                     const RwUInt32 UserTypeNum);

      void Free(const void *Address);

      void Comment(const char *CommentText);

      void ReportCodeMetrics(void);
#else 
      // This section contains dummy functions which are will optimize out when
      // RWS_MEMORY_PROFILING is not defined.
      //
      inline void Open(TimeCallBackFunc _TimeCallBackFunc, RwBool StartEnabled) {}

      inline void Close() {}

      inline RwBool Enable(RwBool flag) { return FALSE; }

      inline void Poll() {}

      inline void ConnectionLost() {}

      inline void Malloc(   const void *Address,
                     const RwUInt32 Size,
                     const char *Filename,
                     const RwUInt32 LineNumber,
                     const RwUInt32 UserTypeNum) {}
      
      inline void Calloc(   const void *Address,
                     const RwUInt32 Size,
                     const char *Filename,
                     const RwUInt32 LineNumber,
                     const RwUInt32 UserTypeNum) {}

      inline void Realloc(  const void *OrgAddress,
                     const void *Address,
                     const RwUInt32 Size,
                     const char *Filename,
                     const RwUInt32 LineNumber,
                     const RwUInt32 UserTypeNum) {}

      inline void Free(const void *Address) {}

      inline void Comment(const char *CommentText) {}

      inline void ReportCodeMetrics(void) {}
#endif   // #ifdef RWS_MEMORY_PROFILING
   }
}

#endif   // #ifndef __MEMORYPROFILE_H_
