/*****************************************************************************
 *
 * File			: NtlSobEventObject.h
 * Author		: agebreak
 * Copyright	: (주)NTL
 * Date			: 2007. 9. 4	
 * Abstract		: Simulation Event object
 *****************************************************************************
 * Desc         : Trigger Object 외의 특별한 Event를 가지는 오브젝트를 생성한다.
 *				  (예. 이정표 - 포커싱될때 UI를 표시)
 *				  서버와 동기화 되지 않고 클라이언트 표시에만 사용한다. 
 *****************************************************************************/

#pragma once

#include "NtlSobActor.h"


class CNtlSobEventObject : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL( CNtlSobEventObject, NTL_DEFAULT_MEMORY_POOL )

public:
	CNtlSobEventObject(VOID);
	virtual ~CNtlSobEventObject(VOID);

	virtual RwBool  Create(VOID);
	virtual void    Destroy(VOID);
	virtual void    Update( RwReal fElapsed );
	virtual void    HandleEvents( RWS::CMsg& pMsg );

protected:	
};
