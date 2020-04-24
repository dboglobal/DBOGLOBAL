/*****************************************************************************
 *
 * File			: NtlLogic.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: Main Logic.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_LOGIC__
#define __NTL_LOGIC__

#include "ceventhandler.h"
#include "NtlSysEvent.h"


//RWS_DEFINE_EVENT(iMsgUpdateTick, 0, "Sent just before each MsgUpateTick.");

/**
 * \ingroup Client
 * client의 모든 simulation object에 system event를 보내는 객체이다.
 * 여기에는 free tick, 10 frame sync tick, 20 frame sync tick, 10 frame sync tick이 존재한다.
 *
 */

class CNtlLogic
{
private:
    
	bool m_bEnabled;			/**< tick의 enable 및 disable flag*/

	float m_fFreeElapsed;		/**< free sync tick*/
	float m_fSync30Elapsed;		/**< 30 frame sync tick에 사용되는 delta time 변수*/
	float m_fSync20Elapsed;		/**< 20 frame sync tick에 사용되는 delta time 변수*/
	float m_fSync10Elapsed;		/**< 10 frame sync tick에 사용되는 delta time 변수*/
	
public:

	CNtlLogic();

	bool Create(void);
	void Destroy(void);

	/**
	* \ingroup Client
	* client의 rendering 객체에 여러가지 sync의 tick event를 보낸다.
	* 모든 simulation object는 tick event를 받아서 update를 한다.
	*
	*/
	void Update(float fElapsed);

	// simulation object tick의 enable 및 disable
	void SetEnabled(bool bEnabled);
};

inline void CNtlLogic::SetEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
}

#endif
