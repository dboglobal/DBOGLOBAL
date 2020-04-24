/*****************************************************************************
*
* File			: NtlDNController
* Author		: Peessi
* Copyright		: (주)NTL
* Date			: 2006. 8. 17	
* Abstract		: Presentation Day And Night Control
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_DN_CONTROLLER_H__
#define __NTL_DN_CONTROLLER_H__

class CNtlDNController;

class CDNNode
{
public:
	enum { NOT_START = 0xffffffff };
	
	CDNNode( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucStartValue, RwUInt8 ucFinishValue, RwReal fBlendFactor, CNtlDNController* pController );
	CDNNode( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucFinishValue, RwReal fBlendFactor, CNtlDNController* pController );// Current Start 생성자.
	
	~CDNNode(VOID);

	RwReal GetBlendFactor(VOID) { return m_fBlendFactor; }
	RwBool IsFinished(VOID);

	RwInt32 Update( RwReal fElapsedTime );	

	RwReal   m_fBeginTime;
	RwReal   m_fDurationTime;
	RwUInt8  m_ucStartValue;
	RwUInt8  m_ucFinishValue;

	RwReal   m_fCurrentTime;
	RwReal	 m_fBlendFactor;

	RwBool	 m_bCurrentStart;

	CNtlDNController* m_pController;
};

class CNtlDNController
{
private:
	CNtlDNController(VOID);
public:
	~CNtlDNController(VOID);

	RwBool CreateInstance(VOID);
	VOID   DeleteInstance(VOID);

	VOID   ClearNodes(VOID);
	
	static CNtlDNController* GetInstance(VOID);

	// operation
	VOID AddDNNode( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucStartValue, RwUInt8 ucFinishValue, RwReal fBlendFactor = 1.0f );
	VOID AddDNNodeCurrentStart( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucFinishValue, RwReal fBlendFactor = 1.0f );
	VOID AddDNNodeCurrentEnd( RwReal fBeginTime, RwReal fDurationTime, RwUInt8 ucStartValue, RwReal fBlendFactor = 1.0f );

	BOOL IsDNEffectApplied(VOID) { return m_bSetDNEffect; }

	RwUInt32 GetLastValue(VOID) { return m_ucLastValue; }

	VOID Update( RwReal fElapsedTime );

private:
	// Implementation
	
	// Variables
	typedef std::list<CDNNode*>				DNLIST;
	typedef std::list<CDNNode*>::iterator	DNLIST_IT;

	DNLIST	 m_listDNNode;		
	RwUInt8  m_ucLastValue;
	BOOL	 m_bSetDNEffect;		// DNEffect가 적용되는 상태를 알기위한 것. TRUE : 적용중.
};

static CNtlDNController* GetDnController(VOID)
{
	return CNtlDNController::GetInstance();
}

#endif//__NTL_DN_CONTROLLER_H__