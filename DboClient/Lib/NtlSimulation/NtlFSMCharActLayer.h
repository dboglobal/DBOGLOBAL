/*****************************************************************************
 *
 * File			: NtlFSMCharActLayer.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: fsm character act layer
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_CAHRACT_LAYER_H__
#define __NTL_FSM_CAHRACT_LAYER_H__

#include "NtlFSMLayer.h"

class CNtlBehaviorCharPush;
class CNtlBehaviorCharShock;
class CNtlBehaviorCharKnockDown;

class CNtlFSMCharActLayer : public CNtlFSMLayer
{
	DECLEAR_MEMORY_POOL(CNtlFSMCharActLayer, NTL_DEFAULT_MEMORY_POOL)

private:

	CNtlBehaviorCharPush *m_pBePush;
	CNtlBehaviorCharShock *m_pBeShock;
	CNtlBehaviorCharKnockDown *m_pKnownDown;

private:

	void CreateController(RwUInt32 uiController);

public:

	CNtlFSMCharActLayer();
	~CNtlFSMCharActLayer();

	/**
    *  이 class가 생성된 후 바로 호출하는 함수
    *
    */
	virtual RwBool Create(RwUInt32 uiController);

	/**
    *  이 class가 소멸되기 전에 호출하는 함수
    *
    */
	virtual void Destroy(void);

	/**
    *  layer update 함수.
    *
    */
	virtual void Update(RwReal fElapsed);

	/**
    *  layer event handler 함수.
    *
    */
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
};

#endif
