/*****************************************************************************
 *
 * File			: NtlStageManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 15	
 * Abstract		: DBO Stage Manager.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __STAGE_MANAGER__
#define __STAGE_MANAGER__

#include "NtlStageManager.h"


/**
 * \ingroup Client
 * DBO에서 사용되는 Stage Manager 객체이며, CNtlStageManager class를 상속받는다.
 * 여기에는 실제 사용된 stage를 생성하는 부분이 존재한다.
 * 
 */
class CStageManager : public CNtlStageManager
{
private:

	static CStageManager *m_pInstance;

protected:

	/**
	* 
    *  \return stage name에 해당하는 stage를 생성하여 리턴한다. 
	*/
	virtual CNtlStage* CreateStage(const char *pStageName);
	
public:

	CStageManager();
	virtual ~CStageManager();

	static CStageManager* GetInstance(void);

	virtual bool Create(void);
	virtual void Destroy(void);
};

#endif

