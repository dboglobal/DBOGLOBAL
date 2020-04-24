/*****************************************************************************
 *
 * File			: NtlSob.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 10	
 * Abstract		: Simulation object entity base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBENTITY_H__
#define __NTL_SOBENTITY_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlDumpTarget.h"

class CNtlPLEntity;

class CNtlSob;
class CNtlSobAttr;
class CNtlSobProxy;
class CNtlSobIcon;
class CNtlSobElapsedController;


////////////////////////////////////////////////////////////////////////////////////////////////////
// simualtion object factory macro
////////////////////////////////////////////////////////////////////////////////////////////////////

// 잠시 주석처리(장형석)
/*
class CNtlSLFactory
{
public:

	static CNtlSLFactory *m_pFactory[MAX_SLCLASS];

public:

	CNtlSLFactory() {} 
	virtual CNtlSob* CreateFactory(void) const { return NULL; } 
};
	


#define DECLEAR_SLSOB_FACTORY(class_name, classid) \
protected: \
	class class_name##Factory : public CNtlSLFactory \
	{ \
	public: \
		class_name##Factory() \
		{ \
			m_pFactory[classid] = this; \
		} \
		CNtlSob* CreateFactory(void) const { return NTL_NEW class_name; } \
	};\
public:\
	static class_name##Factory m_##class_name##Factory; 



#define DEFINITION_SLSOB_FACTORY(class_name)\
	class_name::class_name##Factory class_name::m_##class_name##Factory;
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSob : public RWS::CEventHandler
{
protected:

	RwUInt32 m_uiClassID;					/** class id */
	RwChar m_strName[MAX_SLCLASS_NAME];		/** class name */	
	RwUInt32 m_uiFlags;						/** flags */	
	
	SERIAL_HANDLE m_hSerialID;				/** unique serial id */
	SERIAL_HANDLE m_hOwnerID;				/** owner id */
	SERIAL_HANDLE m_hTargetID;				/** target id */
	SERIAL_HANDLE m_hVehicleID;				/** vehicle id */
	SERIAL_HANDLE m_hLastAttackerID;		/** LastAttacker id */
	RwUInt32 m_uiVisualLuaID;				/** visual lua id */

	CNtlSob *m_pSobParent;					/** owner id와는 다르다 */
	CNtlSobAttr *m_pSobAttr;				/** simulation object attribute */
	CNtlSobProxy *m_pSobProxy;				/** simulation object rendering proxy */

	// coordinate
	RwV3d m_vPos;							/** position */
	RwV3d m_vDirection;						/** direction */

	// weight elapsed time
	RwReal m_fWeightElapsedTime;			/** 가중치 elapsed time */

	RwBool m_bEventHook;

	// 생성자.
	CNtlSob(); 

public:

	virtual ~CNtlSob();

//	static CNtlSob* CreateSobFactory(RwUInt32 uiClassId);
//	static void DeleteSobFactory(CNtlSob *pSobObj);

	virtual RwBool Create(void);
	virtual void PostCreate(void);
	virtual void Destroy(void);
	virtual void ServerChangeOut(void);
	virtual void ServerChangeIn(void);

	virtual void Update(RwReal fElapsed);
	virtual void HandleEvents(RWS::CMsg &pMsg) { }
	virtual void Dump(CNtlDumpTarget& dump);

	virtual RwBool IsFinish(void) { return FALSE; }
	    
public:

	/////////////////////////////////////////////////////
	// sob identity
	void SetClassID(const RwUInt32 uiClassID);
	RwUInt32 GetClassID(void) const;

	void SetClassName(const RwChar *pName);
	const RwChar* GetClassName(void) const;

	void SetFlags(RwUInt32 uiFlags);
	RwUInt32 GetFlags(void) const;

	void SetSerialID(const SERIAL_HANDLE uiSerialID);
	SERIAL_HANDLE GetSerialID(void) const;

	void SetOwnerID(const SERIAL_HANDLE uiOwnerID);
	SERIAL_HANDLE GetOwnerID(void) const;

	void SetTargetID(const SERIAL_HANDLE uiTargetID);
	SERIAL_HANDLE GetTargetID(void) const;

	void SetVehicleID(const SERIAL_HANDLE hVehicleID);
	SERIAL_HANDLE GetVehicleID(void) const;

	void SetLastAttackerID(const SERIAL_HANDLE hLastAttackerID);
	SERIAL_HANDLE GetLastAttackerID(void) const;

	void SetVisualLuaID(const RwUInt32 uiLuaID);
	RwUInt32 GetVisualLuaID(void) const;

	void SetSobParent(const CNtlSob *pSobObj);
	CNtlSob* GetSobParent(void) const;

	/////////////////////////////////////////////////////
	// coordinate
	virtual void SetPosition(const RwV3d *pPos);
	RwV3d GetPosition(void) const;

	virtual void SetDirection(const RwV3d *pDir);
	RwV3d GetDirection(void) const;

	void SetAngleY(RwReal fAngle);
	void SetDeltaAngleY(RwReal fAngle);

	void SetAngleX(RwReal fAngle);
	void SetDeltaAngleX(RwReal fAngle);

	/////////////////////////////////////////////////////
	// weight elapsed time
	void SetWeightElapsedTime(RwReal fWeightValue);
	RwReal GetWeightElapsedTime(void) const;

	/////////////////////////////////////////////////////
	// event hook
	void SetEventHook(RwBool bEventHook);
	RwBool GetEventHook(void) const;

	/////////////////////////////////////////////////////
	// sob add/remove
	virtual void AddSob(CNtlSob *pSobObj) {}
	virtual void RemoveSob(CNtlSob *pSobObj) {}

	CNtlSobAttr* GetSobAttr(void) const;
	CNtlSobProxy* GetSobProxy(void) const;
	virtual CNtlSobIcon* GetIcon(void) const;

public:

	void EnableInput(RwBool bEnable);
	void EnableVisible(RwBool bEnable);
	void EnableWeightElapsedTime(RwBool bEnable);

	RwBool IsInput(void) const;
	RwBool IsVisible(void) const;
	RwBool IsWeightElapsedTime(void) const;

public:
	
	// 움직일 수 있는가?
	virtual RwBool CanMove(void);

	// 공격할 수 있는가?
	virtual RwBool CanAttack(void);

	// 공격 받을 수 있는가?
	virtual RwBool CanHurted(void);
};

inline void CNtlSob::SetClassID(const RwUInt32 uiClassID)
{
	m_uiClassID = uiClassID;
}

inline RwUInt32 CNtlSob::GetClassID(void) const
{
	return m_uiClassID;
}

inline void CNtlSob::SetClassName(const RwChar *pName)
{
	if(pName)
	{
		strcpy_s(m_strName, MAX_SLCLASS_NAME, pName);
	}
}

inline const RwChar* CNtlSob::GetClassName(void) const
{
	return m_strName;
}

inline void CNtlSob::SetFlags(RwUInt32 uiFlags)
{
	m_uiFlags = uiFlags;
}

inline RwUInt32 CNtlSob::GetFlags(void) const
{
	return m_uiFlags;
}

inline void CNtlSob::SetSerialID(const SERIAL_HANDLE hSerialID)
{
	m_hSerialID = hSerialID;
}

inline SERIAL_HANDLE CNtlSob::GetSerialID(void) const
{
	return m_hSerialID;
}

inline void CNtlSob::SetOwnerID(const SERIAL_HANDLE hOwnerID)
{
	m_hOwnerID = hOwnerID;
}

inline SERIAL_HANDLE CNtlSob::GetOwnerID(void) const
{
	return m_hOwnerID;
}

inline void CNtlSob::SetTargetID(const SERIAL_HANDLE hTargetID)
{
	m_hTargetID = hTargetID;
}

inline SERIAL_HANDLE CNtlSob::GetTargetID(void) const
{
	return m_hTargetID;
}

inline void CNtlSob::SetVehicleID(const SERIAL_HANDLE hVehicleID)
{
	m_hVehicleID = hVehicleID;
}

inline SERIAL_HANDLE CNtlSob::GetVehicleID(void) const
{
	return m_hVehicleID;
}

inline void CNtlSob::SetLastAttackerID(const SERIAL_HANDLE hLastAttackerID)
{
	m_hLastAttackerID = hLastAttackerID;
}

inline SERIAL_HANDLE CNtlSob::GetLastAttackerID(void) const
{
	return m_hLastAttackerID;
}

inline void CNtlSob::SetVisualLuaID(const RwUInt32 uiLuaID)
{
	m_uiVisualLuaID = uiLuaID;
}

inline RwUInt32 CNtlSob::GetVisualLuaID(void) const
{
	return m_uiVisualLuaID;
}


inline void CNtlSob::SetSobParent(const CNtlSob *pSobObj)
{
	m_pSobParent = const_cast<CNtlSob*>(pSobObj);
}

inline CNtlSob* CNtlSob::GetSobParent(void) const
{
	return m_pSobParent;
}

inline CNtlSobAttr* CNtlSob::GetSobAttr(void) const
{
	return m_pSobAttr;
}

inline CNtlSobProxy* CNtlSob::GetSobProxy(void) const
{
	return m_pSobProxy;
}


inline RwV3d CNtlSob::GetPosition(void) const
{
	return m_vPos;
}

inline RwV3d CNtlSob::GetDirection(void) const
{
	return m_vDirection;
}

inline RwReal CNtlSob::GetWeightElapsedTime(void) const
{
	return m_fWeightElapsedTime;
}

inline RwBool CNtlSob::GetEventHook(void) const
{
	return m_bEventHook;
}

inline RwBool CNtlSob::CanMove(void)
{
	return FALSE;
}

inline RwBool CNtlSob::CanAttack(void)
{
	return FALSE;
}

inline RwBool CNtlSob::CanHurted(void)
{
	return FALSE;
}

inline CNtlSobIcon* CNtlSob::GetIcon(void) const
{
	return NULL;
}

#endif
