#ifndef __DEBUGMACROS_H__
#define __DEBUGMACROS_H__

#include "profile.h"
#include "callstack.h"
#include "NtlCoreUtil.h"
#include "debugostream.h"
#include "iocoercion.h"

namespace RWS 
{
	void _IndentToCallDepth();
	void _IncCallDepth();
	void _DecCallDepth();
	int _GetCallDepth();
}
#ifdef _WIN32
	#ifdef _MSC_VER
		#ifdef NDEBUG
			#define RWS_DEBUGBREAK
		#else
			#define RWS_DEBUGBREAK __asm { int 3 }
		#endif
	#elif
		#error "Your platform specific break code here"
	#endif
#endif

// custom assert
#define GET_OUTPUT_STREAM() RWS::cdbg()

#define RWS_DEBUGSTREAM(a) \
{ \
	GET_OUTPUT_STREAM() << __FUNCTION__ << " (" << __LINE__ << ") : "; \
	GET_OUTPUT_STREAM() << a; \
	GET_OUTPUT_STREAM().flush(); \
}
/*
#define RWS_DEBUGSTREAM_WARN(a) \
{ \
	GET_OUTPUT_STREAM() << __FUNCTION__ << " (" << __LINE__ << ") : "; \
	GET_OUTPUT_STREAM() << "WARNING - " << a; \
	GET_OUTPUT_STREAM().flush(); \
}

#define RWS_DEBUGSTREAM_ERR(a) \
{ \
	GET_OUTPUT_STREAM() << __FUNCTION__ << " (" << __LINE__ << ") : "; \
	GET_OUTPUT_STREAM() << "ERROR - " << a; \
	GET_OUTPUT_STREAM().flush(); \
}
*/


#define RWS_ASSERTFAIL(msg) \
{ \
	RWS_DEBUGSTREAM( "[ASSERT] " << msg << std::endl ); \
	RWS_DEBUGBREAK; \
}

#define RWS_ASSERT(condition, msg) \
  { if (!(condition)) RWS_ASSERTFAIL(msg) }

#define RWS_ASSERTE(condition) \
  RWS_ASSERT( (condition), "[ASSERT] " #condition " Failed" )



#define RWS_TABLE_ASSERT_FAIL(msg) \
{ \
	RWS_DEBUGSTREAM( "[TABLE ASSERT] " << msg << std::endl ); \
	RWS_DEBUGBREAK; \
}

#define RWS_TABLE_ASSERT(condition, msg) \
{ \
	if( !condition ) \
	{ \
		RWS_DEBUGSTREAM( "[TABLE ASSERT] " << msg << std::endl ); \
		RWS_DEBUGBREAK; \
	} \
}

#define RWS_TABLE_ASSERTE(condition) \
	RWS_TABLE_ASSERT( (condition), "[TABLE ASSERT] " #condition " Failed" )



#define DEBUG_ARRAY_ASSERT_FAIL(msg) \
	DEBUG_ARRAY_MIN_ASSERT_FAIL(msg) \

#define DEBUG_ARRAY_ASSERT(max, index, msg) \
	DEBUG_ARRAY_MIN_ASSERT(0, max, index, msg)

#define DEBUG_ARRAY_ASSERTE(max, index) \
	DEBUG_ARRAY_MIN_ASSERTE(0, max, index)



#define DEBUG_ARRAY_MIN_ASSERT_FAIL(msg) \
	NTL_ASSERTFAIL(msg);

#define DEBUG_ARRAY_MIN_ASSERT(min, max, index, msg) \
	NTL_ASSERT( (min <= index) && (index < max), msg);

#define DEBUG_ARRAY_MIN_ASSERTE(min, max, index) \
	NTL_ASSERTE( (min <= index) && (index < max) )



#define GET_TRACE_STREAM() RWS::trace_dbg()



#define DEBUG_TRACE_MESSAGE(msg) \
{ \
	GET_TRACE_STREAM() << __FUNCTION__ << " (" << __LINE__ << "):  "; \
	GET_TRACE_STREAM() << msg; \
	GET_TRACE_STREAM().flush(); \
}

#define DEBUG_TRACE(condition, msg) \
{ \
	if( !(condition )) \
	{ \
		GET_TRACE_STREAM() << __FUNCTION__ << " (" << __LINE__ << "):  "; \
		GET_TRACE_STREAM() << msg; \
		GET_TRACE_STREAM().flush(); \
	} \
}

#define DEBUG_TRACEE(condition) \
{ \
	if( !(condition) ) \
	{ \
		GET_TRACE_STREAM() << __FUNCTION__ << " (" << __LINE__ << "):  "; \
		GET_TRACE_STREAM() << #condition; \
		GET_TRACE_STREAM().flush(); \
	} \
}



#define RWS_RESISTER_DEBUG_CALLBACK(callback)			RWS::s_DebugBuf.AddOutputCallback(callback);
#define RWS_RESISTER_TRACE_CALLBACK(callback)			RWS::s_TraceBuf.AddOutputCallback(callback)

#define RWS_UNRESISTER_DEBUG_CALLBACK(callback)			RWS::s_DebugBuf.RemoveOutputCallback(callback)
#define RWS_UNRESISTER_TRACE_CALLBACK(callback)			RWS::s_TraceBuf.RemoveOutputCallback(callback)



#define RWS_PRE(condition) \
  RWS_ASSERT( (condition), "Failed PRE-condition: " #condition )

#define RWS_POST(condition) \
  RWS_ASSERT( (condition), "Failed POST-condition: " #condition )



#define RWS_WARNING_MESSAGE(msg) \
	RWS_DEBUGSTREAM( "[WARNING] " << msg << std::endl ); \

#define RWS_WARNING(condition, msg) \
{ \
	if(!(condition)) \
	 {  \
	 RWS_DEBUGSTREAM( "[WARNING] " << msg << std::endl ); \
	 }  \
}

#define RWS_WARNINGE(condition) \
{ \
	if(!(condition)) \
	 {  \
	 RWS_DEBUGSTREAM( "[WARNING] " << #condition << std::endl ); \
	 }  \
}



#define RWS_TABLE_WARNING_MESSAGE(msg) \
	 RWS_DEBUGSTREAM( "[TABLE WARNING] " << msg << std::endl ); \

#define RWS_TABLE_WARNING(condition, msg) \
{ \
	if(!(condition)) \
	 {  \
	 RWS_DEBUGSTREAM( "[TABLE WARNING] " << msg << std::endl ); \
}

#define RWS_TABLE_WARNINGE(condition) \
{ \
	if(!(condition)) \
	 { \
	 RWS_DEBUGSTREAM( "[TABLE WARNING] " << #condition << std::endl ); \
	 } \
}



#ifdef RWS_FUNCTIONPROFILE
	#ifdef RWS_CALLSTACKENABLE
		#define RWS_FUNCTION(a) \
			const char _rwsfunctionstr[] = a; \
			int _rwscalldepth = RWS::_GetCallDepth(); \
			CNtlCallStack::GetInstance()->Push(_rwsfunctionstr); \
			RWS::_IncCallDepth(); \
			RWS::FunctionProfile::CProfileManager::StartProfile( a, __FILE__, __LINE__ ); 
	#else
		#define RWS_FUNCTION(a) \
			int _rwscalldepth = RWS::_GetCallDepth(); \
			RWS::_IncCallDepth(); \
			RWS::FunctionProfile::CProfileManager::StartProfile( a, __FILE__, __LINE__ ); 
	#endif

#else
	#ifdef RWS_CALLSTACKENABLE
		#define RWS_FUNCTION(a) \
			const char _rwsfunctionstr[] = a; \
			int _rwscalldepth = RWS::_GetCallDepth(); \
			CNtlCallStack::GetInstance()->Push(_rwsfunctionstr); \
			RWS::_IncCallDepth();
	#else
		#define RWS_FUNCTION(a) 
	#endif
#endif


#ifdef RWS_FUNCTIONPROFILE
	#ifdef RWS_CALLSTACKENABLE
		#define RWS_RETURN(_rwsreturnval) \
		{  \
			RWS::FunctionProfile::CProfileManager::StopProfile(); \
			RWS::_DecCallDepth(); \
			if (RWS::_GetCallDepth() != _rwscalldepth) \
			{ \
				CNtlCallStack::GetInstance()->LogFilePrint(); \
				NtlLogFilePrint("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID \n"); \
				RWS_DEBUGBREAK;  \
			} \
			return (_rwsreturnval);    \
		}
	#else
		#define RWS_RETURN(_rwsreturnval) \
		{  \
			RWS::FunctionProfile::CProfileManager::StopProfile();  \
			RWS::_DecCallDepth(); \
			if (RWS::_GetCallDepth() != _rwscalldepth) \
			{ \
				CNtlCallStack::GetInstance()->LogFilePrint(); \
				NtlLogFilePrint("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID \n"); \
				RWS_DEBUGBREAK;  \
			} \
			return (_rwsreturnval);    \
		 }
	#endif

	#ifdef RWS_CALLSTACKENABLE
		#define RWS_RETURNVOID() \
		{  \
			RWS::FunctionProfile::CProfileManager::StopProfile(); \
			RWS::_DecCallDepth(); \
			if (RWS::_GetCallDepth() != _rwscalldepth) \
			{  \
				CNtlCallStack::GetInstance()->LogFilePrint(); \
				NtlLogFilePrint("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID \n"); \
				RWS_DEBUGBREAK;  \
			} \
			return;     \
		}
	#else
		#define RWS_RETURNVOID() \
		{  \
			RWS::FunctionProfile::CProfileManager::StopProfile(); \
			RWS::_DecCallDepth(); \
			if (RWS::_GetCallDepth() != _rwscalldepth) \
			{ \
				CNtlCallStack::GetInstance()->LogFilePrint(); \n
				NtlLogFilePrint("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID \n"); \
				RWS_DEBUGBREAK;  \
			} \
			return;     \
		}
	#endif

#else

	#ifdef RWS_CALLSTACKENABLE
		#define RWS_RETURN(_rwsreturnval) \
		{  \
			RWS::_DecCallDepth(); \
			if (RWS::_GetCallDepth() != _rwscalldepth) \
			{ \
				CNtlCallStack::GetInstance()->LogFilePrint(); \
				NtlLogFilePrint("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID \n"); \
				RWS_DEBUGBREAK;  \
			} \
			return (_rwsreturnval);    \
		}
	#else
		#define RWS_RETURN(_rwsreturnval) \
		{  \
			return (_rwsreturnval);    \
		}
	#endif

	#ifdef RWS_CALLSTACKENABLE
		#define RWS_RETURNVOID() \
		{  \
			RWS::_DecCallDepth(); \
			if (RWS::_GetCallDepth() != _rwscalldepth) \
			{ \
				CNtlCallStack::GetInstance()->LogFilePrint(); \
				NtlLogFilePrint("Missing RWS_FUNCTION, RWS_RETURN or RWS_RETURNVOID \n"); \
				RWS_DEBUGBREAK;  \
			} \
			return;     \
		}
	#else
		#define RWS_RETURNVOID() \
		{  \
			return;     \
		}
	#endif
#endif

	


#define STR(str)  #str
#define STR2(str) STR(str)
#define RWS_COMP_MSG(uq_str) message (__FILE__ "(" STR2(__LINE__) ") : - MESSAGE - " #uq_str)
#define RWS_COMP_WARN(uq_str) message (__FILE__ "(" STR2(__LINE__) ") : + WARNING + " #uq_str)
#define RWS_COMP_ERR(uq_str) message (__FILE__ "(" STR2(__LINE__) ") : ## ERROR ## " #uq_str)

#endif // __DEBUGMACROS_H__

#ifdef RWS_FUNCTIONPROFILE
   namespace RWS
   {
		void _SendProfileDataBuffer(const RwUInt32  id,
			                       const RwBool  HasChild,
				                   const RwUInt32  fps,
					               const RwReal  totaltime,
						           const char*  pParent,
							       const char*  pFunction, 
								   const RwUInt32  Calls, 
								   const RwReal  time,
                                   char* pFileName,
                                   const RwUInt32 LineNumber,
                                   const RwReal  childTime);
                                 

		void _SendProfileNewChildFlag( void );
   }
#endif

