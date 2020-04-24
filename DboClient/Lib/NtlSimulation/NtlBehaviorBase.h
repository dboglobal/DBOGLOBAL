/*****************************************************************************
 *
 * File			: NtlBehaviorBase.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: actor behavior class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIORBASE_H__
#define __NTL_BEHAVIORBASE_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlBehaviorDef.h"
#include "NtlDumpTarget.h"
#include "NtlDebug.h"


class CNtlPLEntity;

class CNtlSobActor;

///////////////////////////////////////////////

#define DECLEAR_BEHAVIOR_FACTROY(class_name) \
public: \
	CNtlBehaviorBase* CreateBehaviorFactory(void) \
	{ \
		return NTL_NEW class_name; \
	} \
	void DestroyBehaviorFactory(CNtlBehaviorBase *pBehavior) \
	{ \
		NTL_DELETE( pBehavior ); \
	}

///////////////////////////////////////////////

#define CREATE_BEHAVIOR_FACTORY(behavior_inst) \
	behavior_inst->CreateBehaviorFactory(); 
	
#define DELETE_BEHAVIOR_FACTORY(behavior_inst) \
	behavior_inst->DestroyBehaviorFactory(behavior_inst);

///////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CNtlBehaviorBase
{
protected:

	RwUInt32 m_uiBehaviorId;
	RwChar m_strName[NTL_MAX_BEHAVIOR_NAME];
	RwBool m_bFinish;

	std::list<CNtlPLEntity*> m_listPLEntity;

	CNtlSobActor *m_pActor;

	CNtlBehaviorBase();

private:

	void RemoveAllPLEntity(void);

public:

	CNtlBehaviorBase& operator = (const CNtlBehaviorBase& Behavior);
	virtual ~CNtlBehaviorBase();

	virtual CNtlBehaviorBase* CreateBehaviorFactory(void) = 0;
	virtual	void DestroyBehaviorFactory(CNtlBehaviorBase *pBehavior) = 0;

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed) {}
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void Dump(CNtlDumpTarget& dump);

	virtual void UpdateData(void) {}

	virtual void Clone(CNtlBehaviorBase *pBehavior);

	void SetBehaviorId(RwUInt32 uiBehaviorId);
	RwUInt32 GetBehaviorId(void) const;

	void SetBehaviorName(const RwChar *pName);
	const RwChar* GetBehaviorName(void) const;

	virtual void SetActor(const CNtlSobActor *pActor);
	CNtlSobActor* GetActor(void) const;

	/**
	* visual 
	*/
	void AddPLEntity(CNtlPLEntity *pPLEntity);

	void Finish(void);
	RwBool IsFinish(void) const;
};

inline void CNtlBehaviorBase::SetBehaviorId(RwUInt32 uiBehaviorId)
{
	m_uiBehaviorId = uiBehaviorId;
}

inline RwUInt32 CNtlBehaviorBase::GetBehaviorId(void) const
{
	return m_uiBehaviorId;
}

inline void CNtlBehaviorBase::SetBehaviorName(const RwChar *pName)
{
	if(pName)
	{
		strcpy_s(m_strName, NTL_MAX_BEHAVIOR_NAME, pName);
	}
}

inline const RwChar* CNtlBehaviorBase::GetBehaviorName(void) const
{
	return m_strName;
}

inline void CNtlBehaviorBase::SetActor(const CNtlSobActor *pActor)
{
	m_pActor = const_cast<CNtlSobActor*>(pActor);
}

inline CNtlSobActor* CNtlBehaviorBase::GetActor(void) const
{
	return m_pActor;
}

inline void CNtlBehaviorBase::Finish(void)
{
	m_bFinish = TRUE;
}

inline RwBool CNtlBehaviorBase::IsFinish(void) const
{
	return m_bFinish;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CNtlBehaviorFrame : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorFrame, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorFrame)

private:

	std::list<CNtlBehaviorBase*> m_listChild;

public:

	CNtlBehaviorFrame();
	virtual ~CNtlBehaviorFrame();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void Dump(CNtlDumpTarget& dump);

	virtual void SetActor(const CNtlSobActor *pActor);

	void AddChild(CNtlBehaviorBase *pBehavior);
};

#endif
