/*****************************************************************************
*
* File			: CNtlSobDynamicObjectProxy
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 12. 16
* Abstract		: Simulation Dynamic object proxy
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_SOB_DYNAMIC_OBJECT_PROXY_H__
#define __NTL_SOB_DYNAMIC_OBJECT_PROXY_H__

#include "NtlSLDef.h"
#include "NtlSobProxy.h"
#include "NtlPLObject.h"
#include "NtlPLPlayerName.h"

class CNtlShareTargetMark;

class CNtlSobDynamicObjectProxy : public CNtlSobProxy
{
	public:

	DECLEAR_MEMORY_POOL( CNtlSobDynamicObjectProxy, NTL_DEFAULT_MEMORY_POOL )

	CNtlSobDynamicObjectProxy(void);
	virtual ~CNtlSobDynamicObjectProxy(void);

	virtual RwBool			Create( RwUInt32 uiCompType );
	virtual void			Destroy( void );
	virtual void			Update( RwReal fElapsed );
	virtual void			HandleEvents( RWS::CMsg& msg );

public:
	virtual void			SetPosition( const RwV3d* pPos );
	virtual void			SetDirection( const RwV3d* pDir );
	virtual RwV3d			GetDirection( void );
	virtual void			SetAngleY( RwReal fAngle );

public:
	virtual CNtlPLEntity*	GetPLMainEntity( void );
	virtual RwReal			GetPLEntityWidth( void );
	virtual RwReal			GetPLEntityHeight( void );
	virtual RwReal			GetPLEntityDepth( void );

	// 이름
	void					EnableNameVisible( RwBool bVisible );
	virtual void			SetNameColor( const WCHAR* pwcName, COLORREF color );
	

protected:
	CNtlPLObject*			CreateDynamicObject( const RwV3d& vLoc, RwUInt32 uiDynamicObjectTblIdx );
	void					DestroyDynamicObject();

	void					CreatePLObjectName( void );
	void					DestroyPLObjectName( void );

	void					CreatePLTargetMark( void );
	void					DestroyPLTargetMark( void );

    /// 공유 타겟 마크
    void                    SobShareTargetSelectHandler(RWS::CMsg& pMsg);                   ///< 공유 타겟 마크 설정 이벤트 처리
    void                    SobShareTargetReleaseHandler(RWS::CMsg& pMsg);                  ///< 공유 타겟 마크 해제 이벤트 처리        
    void                    CreateShareTargetMark(RwUInt8 bySlot, RwUInt32 type);
    void                    DeleteShareTargetMark();    

	void					UpdateState( RwUInt8 byState );


protected:
	CNtlPLObject*			m_pPLDynamicObject;				///< Dynamic object 
	CNtlPLPlayerName*		m_pPLDynamicObjectName;			///< Dynamic object name
	CNtlPLEntity*			m_pTargetMark;					///< Dynamic object target mark
    CNtlShareTargetMark*    m_pShareTargetMark;             ///< 공유 타겟 마크

	RwReal					m_fObjectWidth;					///< Object Width
	RwReal					m_fObjectHeight;				///< Object Height
	RwReal					m_fObjectDepth;					///< Object depth

	RwUInt8					m_byMainState;
};


inline CNtlPLEntity* CNtlSobDynamicObjectProxy::GetPLMainEntity( void ) 
{
	return reinterpret_cast<CNtlPLEntity*>( m_pPLDynamicObject );
}


#endif