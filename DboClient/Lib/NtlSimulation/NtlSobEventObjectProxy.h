/*****************************************************************************
*
* File			: CNtlSobEventObjectProxy
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2007. 9. 4	
* Abstract		: Simulation Event object Proxy
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#pragma once

class CNtlPLObject;

#include "NtlSLDef.h"
#include "NtlSobProxy.h"

class CNtlSobEventObjectProxy : public CNtlSobProxy
{
	DECLEAR_MEMORY_POOL( CNtlSobEventObjectProxy, NTL_DEFAULT_MEMORY_POOL )
public:

    CNtlSobEventObjectProxy(VOID);
    virtual ~CNtlSobEventObjectProxy(VOID);

    virtual RwBool Create(RwUInt32 uiCompType);
    virtual void Destroy(VOID);
    virtual void Update(RwReal fElapsed);
    virtual void HandleEvents( RWS::CMsg& msg );

    //-------------------------------------------------------------------------------
    // event handler
    //-------------------------------------------------------------------------------
    void SobCreateEventHandler(RWS::CMsg &pMsg);

    //---------------------------------------------------
    // main presentation entity에 관련된 함수 
    //---------------------------------------------------

    /**
    * transform에 관현된 함수.
    */

    virtual void SetPosition(const RwV3d *pPos);
    virtual void SetDirection(const RwV3d *pDir);
    virtual void SetAngleY(RwReal fAngle);        

    /**
    * entity 정보에 관한 함수.
    */
    CNtlPLEntity*			GetPLMainEntity(void);      ///< presentation main entity    
    virtual RwReal			GetPLEntityWidth(void);     ///< pl entity의 폭을 얻어온다.    
    virtual RwReal			GetPLEntityHeight(void);    ///< pl entity의 높이를 얻어온다.    
    virtual RwReal			GetPLEntityDepth(void);     ///< pl entity의 depth을 얻어온다.

protected:

	/** 
	* trigger object 이름 생성.
	*/
	void    CreatePLObjectName(void);
	void    DeletePLObjectName(void);

protected:
        CNtlPLObject*           m_pPLObject;					/** trigger object */
        RwReal					m_fObjectWidth;					/** trigger object width */
        RwReal					m_fObjectHeight;				/** trigger object height */
        RwReal					m_fObjectDepth;					/** trigger object depth */        
};

inline CNtlPLEntity* CNtlSobEventObjectProxy::GetPLMainEntity(VOID) 
{
	return reinterpret_cast<CNtlPLEntity*>( m_pPLObject );
}
