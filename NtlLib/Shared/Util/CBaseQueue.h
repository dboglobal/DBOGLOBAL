//////////////////////////////////////////////////////////////////////////////
//
// BaseQueue.h : Base Queue Class Header
//
//        Programmed by June C. Kim (cynis@hotmail.com)
//
//        Last Update 2003.6.20
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDED_BASE_QUEUE_H_
#define _INCLUDED_BASE_QUEUE_H_

//////////////////
// Header Table //
//////////////////


///////////////////////////////
// Type Pre-Definition Table //
///////////////////////////////

class CBaseQueue;
typedef CBaseQueue*	LPBASEQUEUE;
typedef CBaseQueue*	LPCBASEQUEUE;


///////////////////////////////
// Constant Definition Table //
///////////////////////////////

#define BQ_SIZE_MIN					( 4 )
#define BQ_RESIZING_FACTOR			( 1 )

#define BQ_MODE_NOPUSH				( 0x0000 )
#define BQ_MODE_POP_BEFORE_PUSH		( 0x0001 )
#define BQ_MODE_RESIZE				( 0x0002 )

#define BQ_STATE_READY				( 0x0000 )
#define BQ_STATE_PUSH				( 0x0001 )
#define BQ_STATE_POP				( 0x0002 )
#define BQ_STATE_RESIZE				( 0x0010 )
#define BQ_STATE_DISABLED			( 0x8000 )


///////////////////////////
// Type Definition Table //
///////////////////////////

typedef struct QUEUEINFO
{
	WORD		wType;
	WORD		wSize;		
} QUEUEINFO, *LPQUEUEINFO;


//////////////////////
// class CBaseQueue //
//////////////////////

class CBaseQueue
{
	public:

		CBaseQueue( void );
		CBaseQueue( DWORD dwMaxSize, WORD wMode = BQ_MODE_RESIZE ) { Initialize(); Create( dwMaxSize, wMode ); }

		~CBaseQueue( void );

	public:

		bool					Create( DWORD dwMaxSize, WORD wMode = BQ_MODE_RESIZE );
		void					Destroy( void );

		bool					Push( LPQUEUEINFO lpQueueInfo, LPVOID lpvData );
		bool					Pop( LPQUEUEINFO lpQueueInfo, LPVOID lpvData );

		bool					PushFast( LPQUEUEINFO lpQueueInfo, LPVOID lpvData );
		bool					PopFast( LPQUEUEINFO lpQueueInfo, LPVOID lpvData );

		bool					Refresh( void );

		DWORD					GetSize( void ) { return m_dwSize; }

		bool					SetMaxSize( DWORD dwNewMaxSize );
		DWORD					GetMaxSize( void ) { return m_dwMaxSize; }

		bool					SetMode( WORD wNewMode );
		WORD					GetMode( void ) { return m_wMode; }

		void					SetLastError( DWORD dwErrorCode ) { m_dwErrorCode = dwErrorCode; }
		DWORD					GetLastError( void ) { return m_dwErrorCode; }

	protected:

		void					Initialize( void );
		void					DestroyBuffer( void );

		bool					AutoResize( void );

	protected:

		WORD					m_wState;						// Queue State
		WORD					m_wMode;						// Queue Mode

		DWORD					m_dwMaxSize;					// Maximum Queue Buffer Size
		DWORD					m_dwSize;						// Valid Queue Size

		DWORD					m_dwHead;						// Push Position
		DWORD					m_dwTail;						// Pop Position

		LPQUEUEINFO *			m_lpInfoQueueBuffer;			// Information Queue Buffer
		LPBYTE *				m_lpDataQueueBuffer;			// Data Queue Buffer

		DWORD					m_dwErrorCode;

	public:

		static const DWORD		SIZE_MIN;						// Minimum Queue Buffer Size for Downsizing Buffer
		static const DWORD		RESIZING_FACTOR;				// Resizing Queue Buffer Factor

		static const WORD		MODE_RESIZE;					// Auto Resize Mode
		static const WORD		MODE_POP_BEFORE_PUSH;			// Pop Head And Push Tail Mode
		static const WORD		MODE_NOP;						// No Operation Mode

		static const WORD		STATE_READY;					// Ready for Operation State
		static const WORD		STATE_PUSH;						// Push Operation State
		static const WORD		STATE_POP;						// Pop Operation State
		static const WORD		STATE_RESIZE;					// Resize Operation State
		static const WORD		STATE_DISABLED;					// Disabled State
};

#endif _INCLUDED_BASE_QUEUE_H_

//////////////////////////////////////////////////////////////////////////////
//
// End of File : BaseQueue.h
//
//////////////////////////////////////////////////////////////////////////////