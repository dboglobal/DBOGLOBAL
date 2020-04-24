//***********************************************************************************
//
//	File		:	NtlThread.h
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	NTL Thread Class
//
//***********************************************************************************

#ifndef __NTLTHREAD_H__
#define __NTLTHREAD_H__

#define LOKI_CLASS_LEVEL_THREADING
#include "Singleton.h"

#include "NtlLinkList.h"
#include "NtlString.h"
#include "NtlEvent.h"
#include "NtlMutex.h"


class CThreadKey;
class CThreadHelper;

//---------------------------------------------------------------------------------------
// CNtlRunObject : 쓰레드 로직 객체 클래스
//---------------------------------------------------------------------------------------
class CNtlRunObject
{
friend class CNtlThread;

public:

	CNtlRunObject()
		:m_arg(0), m_pOwner(0), m_bRunning(true) {}

	virtual ~CNtlRunObject() {}


public:

	// 쓰레드 프로그램은 이 함수를 Override하여 Application Logic을 작성
	virtual void			Run() {}

	//
	virtual void			Close() { m_bRunning = false; }


public:

	// 쓰레드 작업 종료 및 쓰레드 관련 리소스 해제
	void					Terminate();

	//
	void *					GetArg() const { return m_arg; }

	//
	void 					SetArg(void * arg) { m_arg = arg; }

	// 소유 쓰레드 반환
	CNtlThread *			GetThread() const { return m_pOwner; }

	// unconditional wait
	void					Wait();

	// conditional wait
	int						Wait(unsigned int millisecs);

	// 소유 쓰레드를 종료
	void					Exit();

	// 소유 쓰레드의 이름을 반환한다
	const char *			GetName() const;

	//
	bool					IsRunnable() { return m_bRunning; }

	//
	void					SetRunnable(bool bRunning) { m_bRunning = bRunning; }


private:

	// 소유 쓰레드
	void					SetThread(CNtlThread * pOwner) { m_pOwner = pOwner; }


protected:

	void *					m_arg;

private:

	CNtlThread *			m_pOwner;

	bool					m_bRunning;

};


//---------------------------------------------------------------------------------------
// CNtlThread : 쓰레드 객체 클래스
//---------------------------------------------------------------------------------------
class CNtlThread : public CNtlLinkObject
{
friend class CThreadkey;
friend class CThreadHelper;
friend class CNtlThreadFactory;

public:

	// 쓰레드 상태
	enum STATUS
	{
		eSTATUS_NOT_RUNNING = 0,	// 동작하지 않는 상태 ( 초기 상태 )
		eSTATUS_PREPARING_TO_RUN,	// 동작 준비 상태
		eSTATUS_RUNNING,			// 동작 상태
		eSTATUS_PAUSED,				// 멈춤 상태
		eSTATUS_DEAD,				// 종료 상태

		MAX_STATUS,
	};


public:

	// AutoDelete의 경우 RunObject는 반드시 Heap에 생성하고 종료시 삭제할 것. 
	CNtlThread(CNtlRunObject * pRunObject, const char * name = "Unknown Thread", bool bAutoDelete = false);

	virtual ~CNtlThread(void);


private:

	CNtlThread(const CNtlThread &);

	void operator=(const CNtlThread &);


public:

	// Thread를 종료하기위해 외부에서 호출되야 하는 함수
	void					Close();

public:

	// Thread 종료
	void					CleanUp();

	// 실제 Thread Loop에서 호출되는 함수, 내부적으로 Runnable의 run을 호출한다
	void					Execute();


public:

	// 초기화
	void					Init();

	// Get Next Thread ( in Current Linked List )
	CNtlThread *			GetNextThread() const { return (CNtlThread*) this->GetNext(); }

	// conditional wait
	void					Wait();

	// unconditional wait
	int						Wait(unsigned int millisecs);

	// Thread를 종료시킨다
	void					Exit();

	// 호출자는 Thread가 종료될때까지 블락된다
	void					Join();

	// 쓰레드 생성뒤에 반드시 호출하여야 동작한다. ( 실제 Thread 생성 )
	void					Start();

	// Thread이름 반환
	const char *			GetName() { return m_strName.c_str(); }

	// Thread에 연결된 Runnable 객체 반환
	CNtlRunObject *			GetRunObject() { return m_pRunObject; }

	// attribute 설정
	void					SetArg(void *arg) { m_pRunObject->SetArg(arg); }

	// attribute 반환
	void *					GetArg() { return m_pRunObject->GetArg(); }

	//
	bool					IsAutoDelete() { return m_bAutoDelete; }

	// 상태체크
	bool					IsStatus(STATUS status) { return status == m_status; }

	//
	const char *			GetStatusString();

	//
	void					SetSignaled(bool bSignaled) { m_bSignaled = bSignaled; }

	//
	bool					IsSignaled() { return m_bSignaled; }




	// 해당되는 Thread에 시그널을 보낸다 ( static )
	static void				Notify(CNtlThread * pThread);

	// 현재 Thread를 반환한다 ( static )
	static CNtlThread *		GetCurrentThread();

	// Main Thread를 반환한다
	static CNtlThread *		GetMainThread() { return m_pMainThread; }

	//
	static void				UnitTest();
	

protected:


	CNtlString				m_strName;

	STATUS					m_status;


	HANDLE					m_hThread;

	unsigned				m_threadID;

	CNtlEvent				m_event;

	bool					m_bSignaled;

	bool					m_bAutoDelete;

	CNtlRunObject *			m_pRunObject;


protected:

	static CThreadKey *		m_pThreadKey;

	static CNtlThread *		m_pMainThread;
};




//---------------------------------------------------------------------------------------
// NtlThreadFactory : 쓰레드 생성 및 소멸 처리를 담당하는 Factory 클래스
//---------------------------------------------------------------------------------------
class CNtlThreadFactory
{
friend class CNtlThread;
friend class CThreadHelper;	// 클래스 내부용
friend class CNtlRunObject;

public:

	CNtlThreadFactory();

	virtual ~CNtlThreadFactory();


public:


	void					GarbageCollect(bool bShutDown = false);

	void					SingleGarbageCollect(CNtlThread* pGarbageThread);

	//close


public:
	
	void					Shutdown();

	void					CloseAll();

	void					JoinAll();

	CNtlThread *			CreateThread(CNtlRunObject * pRunObject, const char * name = "Unnamed Thread", bool bAutoDelete = false);
	
	void					AllThreadDump(); //new


protected:

	CNtlLinkList 		m_ThreadList;

	CNtlMutex 			m_Mutex;

	bool				m_bClosed;

};

typedef Loki::SingletonHolder<CNtlThreadFactory, Loki::CreateUsingNew, Loki::DefaultLifetime, Loki::ClassLevelLockable> tThreadFactory;


inline void CNtlRunObject::Wait()
{
	m_pOwner->Wait();
}

inline void CNtlRunObject::Terminate()
{
	Close();

	if (GetThread())
	{
		if (GetThread()->IsStatus(CNtlThread::eSTATUS_RUNNING) ||
			GetThread()->IsStatus(CNtlThread::eSTATUS_PAUSED) ||
			GetThread()->IsStatus(CNtlThread::eSTATUS_DEAD))
		{
			GetThread()->Join();
		}

		tThreadFactory::Instance().SingleGarbageCollect(GetThread());
	}
}

inline int CNtlRunObject::Wait(unsigned int millisecs)
{
	return m_pOwner->Wait( millisecs );
}

inline void CNtlRunObject::Exit()
{
	m_pOwner->Exit();
}

inline const char * CNtlRunObject::GetName() const
{
	return m_pOwner->GetName();
}


#endif // __NTLTHREAD_H__

