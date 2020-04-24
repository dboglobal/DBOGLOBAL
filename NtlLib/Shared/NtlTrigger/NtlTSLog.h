#ifndef _NTL_TSLOG_H_
#define _NTL_TSLOG_H_


/**
	Log
	TS에서 발생하는 로고를 게임쪽에서 사용하기 위해서는
	아래 클래스를 상속해야서 구현 후, 전역 함수 Register를 통해 등록해야 함
	상속 받아 구현한 로고 객체는 사용 완료 후 반드시 Unregister를 통해 제거 해야 함
*/


class CNtlTSLog
{
// Declarations
public:
	enum
	{
		LOG_MAX_BUFFER					= 2048
	};

// Static variables
protected:
	static CNtlTSLog*					s_pLog;
	static char							s_szStrBuf[LOG_MAX_BUFFER];

// Static methods
public:
	static void							Register( CNtlTSLog* pLog );
	static void							Unregister( void );
	static void							Log( const char* pFormat, ... );

// Constructions and Destructions
public:
	virtual ~CNtlTSLog( void ) { return; }

// Implementations
protected:
	virtual void						OnLogEvent( const char* pLog );
};


#endif