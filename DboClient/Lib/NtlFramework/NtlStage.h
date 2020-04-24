/*****************************************************************************
 *
 * File			: NtlStage.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: Stage.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_STAGE__
#define __NTL_STAGE__

#include "ceventhandler.h"
#include "NtlSysEvent.h"

/**
 * \ingroup Client
 * client 모든 객체를 게임 상태 단위 별로 그룹하여 정보를 가지고 있는 base class이다.
 * MMORPG에서는 Login Stage, Lobby Stage, Character Create Stage, Character Select Stage, Game Stage등등 존재할 수 있다.
 */

class CNtlStage 
{
private:

	std::string m_strName;
	
public:

	CNtlStage(const char *pStageName);
	virtual ~CNtlStage();

	// stage 객체가 생성된 다음 곧바로 호출된다. 이는 하위 객체에서 상속받아 적절한 처리를 한다.
	virtual bool Create(void);

	// stage 객체가 삭제되기 전에 호출된다. 이는 하위 객체에서 상속받아 적절한 처리를 한다.
	virtual void Destroy(void);

	virtual void PostRender(void);

	// stage name을 얻어온다.
	const char* GetName(void);
};


inline const char* CNtlStage::GetName(void)
{
	return m_strName.c_str();
}

#endif