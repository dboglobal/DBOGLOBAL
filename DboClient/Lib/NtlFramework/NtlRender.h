/*****************************************************************************
 *
 * File			: NtlRender.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: Main Render.
 *****************************************************************************
 * 설명 : 
 *
 *****************************************************************************/

#ifndef __NTL_RENDER__
#define __NTL_RENDER__

#include "ceventhandler.h"
#include "NtlSysEvent.h"

/**
 * \ingroup Client
 * client의 rendering 객체에 system event를 보내는 객체이다.
 * 모든 rendering 객체를 update 한다.
 *
 */

class CNtlRender
{
private:
    
	bool m_bEnabled;

	float m_fFreeElapsed;		/**< free sync tick*/

public:

	CNtlRender();

	// CNtlRender 객체가 생성된 다음 곧바로 호출된다.
    bool Create(void);

	// CNtlRender 객체가 해제되기 전에 바로 호출된다.
	void Destroy(void);

	/**
	* \ingroup Client
	* client의 rendering 객체에 tick event를 보낸다.
	*
	*/
	void Update(float fElapsed);

	// rendering enable 및 disable
	void SetEnabled(bool bEnabled); 
};


inline void CNtlRender::SetEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
}


#endif
