/*****************************************************************************
 *
 * File :     DebugOstream.cpp
 *
 * Abstract : An ostream implementation for debug output (e.g. under Win32
 *            output goes to OutputDebugString)
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
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#include "precomp_ntlcore.h"
#include "debugmacros.h"
#include "debugostream.h"
#include "NtlCoreUtil.h"
#include "NtlLog.h"

namespace RWS
{
	namespace
   {
      /**
      *
      *  Default message output handler for debug_buf. Outputs a string 
      *  via the standard debug output method for a platform.
      *
      *  - PC, XBox use OutputDebugStringA().
      *  - PlayStation2 uses printf().
      *  - GameCube uses OSReport().
      *
      *  \param message Null terminated character string to output.
      *
      */
      void OutputPlatformDebugMessage( const char * message )
      {
#if defined(_MSC_VER)

         OutputDebugStringA(message);

#elif defined (SKY)

         printf("%s", message);

#elif defined (DOLPHIN)

         OSReport("%s", message);

#else
#error "You need to put your platforms debug output function here."
#endif
      }

	  // agebreak
      void OutputLogFileMessage( const char * message )
      {
		  NtlLogFilePrint(const_cast<RwChar*>(message));
//		  static CNtlLog log;
//		  static RwBool bFirst = TRUE;
//		  if (bFirst)
//		  {
//			  log.Create("defaultLog.txt");
//			  bFirst = FALSE;
//		  }
//		  log.fpdaylog(const_cast<RwChar*>(message));
      }

	  // avooo
	  void OutputTraceFileMessage( const char * message )
	  {
		  NtlTraceFilePrint(const_cast<RwChar*>(message));
	  }
   }
   

   /**
   *
   *  Constructor for debug_buf. Sets the buffer area to be used and adds the
   *  default message output handler.
   *
   *  \see OutputPlatformDebugMessage
   *
   */
   debug_buf::debug_buf() : m_OutputCBCount(0)
   {
      // Setup the buffer area 
      // (note end is one less, to allow for the null terminating char)
      setp(&m_buffer[0], (&m_buffer[0] + (sizeof(m_buffer) - 1)));      
   }

   /**
   *
   *  Destructor for debug_buf.
   *
   */
   debug_buf::~debug_buf()
   {
      sync();
   }

   /**
   *
   *  Adds a message output callback to the list of callbacks
   *  held by this object. A callback can only be added once.
   *  If it already exists it will be ignored.
   *
   *  \param OutputCB Function to add to the callback list.
   *
   */
   void debug_buf::AddOutputCallback( OutputMessageCB OutputCB )
   {
      RWS_WARNING(m_OutputCBCount < MAXOUTPUTCALLBACKS,
                  "debug_buf can only keep track of " << MAXOUTPUTCALLBACKS \
                  << " output callbacks; extra ones will be ignored.");

      if (m_OutputCBCount >= MAXOUTPUTCALLBACKS)
      {
         return;
      }

      if (OutputCB)
      {
         m_OutputCallbacks[m_OutputCBCount] = OutputCB;
         ++m_OutputCBCount;
      }
   }

   /**
   *
   *  Removes a message output callback from the list of callbacks
   *  held by this object.
   *
   *  \param OutputCB Function to remove from the callback list.
   *
   */
   void debug_buf::RemoveOutputCallback( OutputMessageCB OutputCB )
   {
      for (RwUInt32 uCount = 0; uCount < m_OutputCBCount; uCount++)
      {
         if (m_OutputCallbacks[uCount] == OutputCB)
         {
            // Just swap the last callback with this one, then set it to 0
            --m_OutputCBCount;
            m_OutputCallbacks[uCount] = m_OutputCallbacks[m_OutputCBCount];
            m_OutputCallbacks[m_OutputCBCount] = 0;
            break;
         }
      }
   }   

   void debug_buf::Clear(void)
   {
	   memset(m_buffer, 0, 4096);
   }

   char* debug_buf::GetBuffer(void)
   {
	   return m_buffer;
   }

   /**
   *
   *  Function called internally by debug_buf when outputting the 
   *  contents of its buffer. Iterates through all of the registered
   *  output callbacks and calls them with the given message.
   *
   *  \param message Null terminated character string to output.
   *
   */
   void debug_buf::OutputDebugMessage( char * szMessage )
   {
      static bool recursionguard = false;

	  if( !szMessage )
		  return;

	  if( strlen(szMessage) == 0 )
		  return;

      if (!recursionguard)
      {
         recursionguard = true;

         for (RwUInt32 uCount = 0; uCount < m_OutputCBCount; uCount++)
         {
            if (m_OutputCallbacks[uCount])
            {
               m_OutputCallbacks[uCount](szMessage);
            }
         }

         recursionguard = false;
      }
   }

   /**
   *
   *  Custom implementation of std::streambuf::sync for debug_buf.
   *
   */
   int debug_buf::sync()
   {
      // Can't use an RWS_ASSERT here, or it will recursively call this trace class
      if ( pptr() > epptr() )
      {
         OutputDebugMessage("Warning: pptr() > epptr(), debug_buf buffer has been overrun.\n");
         RWS_DEBUGBREAK;
      }

#if defined(DOLPHIN) & !defined(RWS_BROADBAND)
      // Buffer the text on GameCube, the buffering is a speed optimization due to the slow
      // USB connection to the development kit.
      //
      // Only flush the buffer if it is full - to keep the number of network packets down
      if ( pptr() == epptr() )
      {
#endif
         // null-terminate the string
         *pptr() = '\0';

         OutputDebugMessage(pbase());

         // reset the buffer pointers (note end is one less, to allow for the null terminating char)
         setp(&m_buffer[0], (&m_buffer[0] + (sizeof(m_buffer) - 1)));

#if defined(DOLPHIN) & !defined(RWS_BROADBAND)
      }
#endif

      return 0; // return -1 for failure, anything else for success
   }

   /**
   *
   *  Custom Implementation of std::streambuf::overflow for debug_buf.
   *
   */
   int debug_buf::overflow( int c )
   {
      // flush the current contents of the buffer
      if ( sync() != -1 )
      {
         // flush went ok - add the given character to the buffer
         return sputc(c);
      }
      else
      {
         // flush failed - return error
         return EOF;
      }
   }

   /**
   *
   *  Custom implementation of std::streambuf::underflow for platforms on which
   *  it is a pure virtual function. This function always returns EOF since it 
   *  is not used by debug_buf (it is only for input stream buffers).
   *
   */
   int debug_buf::underflow()
   {
      return EOF;
   }


   /**
   *
   *  Constructor for a debug_ostream. Requires a debug_buf that has already been fully constructed.
   *     Note: the PS2 version of debug_ostream uses old-style iostreams, other platforms use the newer,
   *           standards-compliant versions hence the slightly different base constructor calls.
   *
   */
   debug_ostream::debug_ostream( debug_buf * pBuf )
#if defined(SKY)
      : std::ostream ( pBuf ),
#else
      : std::basic_ostream<char, std::char_traits<char> > ( pBuf ),
#endif
        m_pBuf(pBuf)
   {
   }

   /**
   *
   *  Destructor for debug_ostream.
   *
   */
   debug_ostream::~debug_ostream()
   {
   }
   
   /**
   *
   *  Custom implementation of std::ostream::rdbuf() that
   *  returns a pointer to a debuf_buf rather than a
   *  std::streambuf.
   *
   *  \return Pointer to the debug_buf within a debug_ostream.
   */
   debug_buf * debug_ostream::rdbuf() const
   {
      return m_pBuf;
   }


   debug_buf s_DebugBuf;			// debug log 파일용 stream buffer
   debug_buf s_TraceBuf;			// tracd log 파일용 stream buffer

   /**
   *
   *  Global accessor function for getting at the global debug_ostream.
   *
   */
   debug_ostream& cdbg() 
   {
	  static RwBool bSetCallback = false;
	  if( !bSetCallback )
	  {
		  bSetCallback = !bSetCallback;

		  // Setup the default output callback
		  s_DebugBuf.AddOutputCallback(OutputPlatformDebugMessage);
		  s_DebugBuf.AddOutputCallback(OutputLogFileMessage);
	  }

      static debug_ostream str(&s_DebugBuf);

      return str;
   }

   /**
   *
   *  Global accessor function for getting at the global trace_ostream.
   *
   */
   debug_ostream& trace_dbg() 
   {
	   static RwBool bSetCallback = false;
	   if( !bSetCallback )
	   {
		   bSetCallback = !bSetCallback;

		   // Setup the default output callback
		   s_TraceBuf.AddOutputCallback(OutputTraceFileMessage);
	   }

	   static debug_ostream str(&s_TraceBuf);

	   return str;
   }
}