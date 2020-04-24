/*****************************************************************************
*
* File			: CNtlSobWorldItemProxy
* Author		: Peessi
* Copyright		: (주)NTL
* Date			: 2006. 7. 12	
* Abstract		: Simulation World Item Proxy
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_SOB_WORLDITEM_PROXY_H__
#define __NTL_SOB_WORLDITEM_PROXY_H__

class CNtlPLPlayerName;
class CNtlPLDecal;
class CNtlPLItem;

#include "NtlSLDef.h"
#include "NtlSobProxy.h"

class CNtlSobWorldItemProxy : public CNtlSobProxy
{
	DECLEAR_MEMORY_POOL( CNtlSobWorldItemProxy, NTL_DEFAULT_MEMORY_POOL )

private:

	CNtlPLItem*			m_pWorldItem;					/** Item **/
	
protected:

	/** 
	* character 생성
	*/
	VOID CreatePLWorldItem( RWS::CMsg& msg );
	VOID DeletePLWorldItem(VOID);
		
public:

	CNtlSobWorldItemProxy(VOID);
	virtual ~CNtlSobWorldItemProxy(VOID);

	virtual RwBool Create(RwUInt32 uiCompType);
	virtual VOID Destroy(VOID);
	virtual VOID HandleEvents( RWS::CMsg& msg );

public:

	//---------------------------------------------------
	// main presentation entity에 관련된 함수 
	//---------------------------------------------------

	/**
	* world에 add 및 remove 관련된 함수.
	*/

	virtual void	AddWorld(void);
	virtual void	RemoveWorld(void);

	/**
	* transform에 관현된 함수.
	*/

	virtual void	SetPosition(const RwV3d *pPos);
	virtual void	SetDirection(const RwV3d *pDir);
	virtual void	SetAngleY(RwReal fAngle);


	// animation
	void			SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop = TRUE, RwReal fStartTime = 0.0f);

	// 
	virtual void	EnableVisible(RwBool bEnable);

	// presentation main entity
	CNtlPLEntity*	GetPLMainEntity(VOID);
};

inline CNtlPLEntity* CNtlSobWorldItemProxy::GetPLMainEntity(VOID) 
{
	return reinterpret_cast<CNtlPLEntity*>( m_pWorldItem );
}


#endif