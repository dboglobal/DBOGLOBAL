//***********************************************************************************
//
//	File		:	NtlEvent.h
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Event 동기화 오브젝트 클래스
//
//***********************************************************************************

#ifndef __NTLEVENT_H__
#define __NTLEVENT_H__

#include <process.h>

class CNtlEvent
{
public:

	CNtlEvent();

	virtual ~CNtlEvent(void);


	//
	CNtlEvent(const CNtlEvent & ev);

	CNtlEvent & operator=(const CNtlEvent &ev);



	// 이벤트 초기화
	void			Reset();

	//  이벤트 통보
	void			Notify();

	// 이벤트 대기 ( unconditional )
	void			Wait();

	// 이벤트 대기 ( conditional )
	int				Wait( unsigned int millisecs );


private:

	HANDLE			m_hEvent;
};

#endif // __NTLEVENT_H__
