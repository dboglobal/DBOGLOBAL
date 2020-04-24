/*****************************************************************************
*
* File			: NtlPacketLockManager.h
* Author		: Peessi
* Copyright		: NTL-Inc.
* Date			: 2006. 10. 11	
* Abstract		: Packet Lock Manager
*****************************************************************************/

#ifndef __NTL_PACKET_LOCK_MANAGER_H__
#define __NTL_PACKET_LOCK_MANAGER_H__

#include <set>

class CNtlPacketLockManager
{
public:
	enum eLockType
	{
		LT_ITEM					= 0x01,
		LT_ZENNY				= 0x02,
		LT_WORLDCONCEPT			= 0x04,	
		LT_MAIL					= 0x08,
		LT_SKILL				= 0x10,
        LT_COMMUNITY            = 0x20,
		LT_AUCTIONHOUSE			= 0x40,
		LT_HLSHOP = 0x80,
	};

	struct sLockedPacket
	{
		eLockType	eType;
		RwUInt32	uiPacket;
		RwReal		fElapsed;

		sLockedPacket(eLockType elockType)
		:eType(elockType)
		,uiPacket(0)
		,fElapsed(0.f)
		{

		}
	};

	typedef std::map<RwInt32,RwInt32>				MAP_LOCKPACKET;

	typedef std::set<RwInt32>						SET_LOCKPACKET;

	typedef std::list<sLockedPacket>				LIST_LOCKED_TIME;
	typedef std::list<sLockedPacket>::iterator		LIST_LOCKED_TIME_ITER;
	
	CNtlPacketLockManager(VOID);
	~CNtlPacketLockManager(VOID);

	VOID	Reset(VOID);

	VOID	Update(RwReal fElapsed);

	// Operation
	RwBool	Lock( RwInt32 eLockPacket );
	RwBool	Unlock( RwInt32 eLockPacket );
	RwBool	IsLock( RwInt32 eLockPacket );	
	RwBool	IsLockType( RwInt32 eLockedType );
	RwBool	IsLock_withoutMessage( RwInt32 eLockPacket );
	RwBool	IsLockType_withoutMessage( RwInt32 eLockedType );

	// Req <-> Res 간의 Lock
	RwBool	IndividualLock( RwInt32 eLockPacket );
	RwBool	IndividualUnlock( RwInt32 eLockPacket );
	RwBool	IsIndividualLock( RwInt32 eLockPacket );

private:
	// Variable

	// LOCK TYPE 별로 분류하는 Packet
	MAP_LOCKPACKET		m_mapLockPacket;
	
	// Request 와 Result 로 구분하는 Packet
	SET_LOCKPACKET		m_setLockPacket;
	LIST_LOCKED_TIME	m_listLockedTime;
};

#endif//__NTL_PACKET_LOCK_MANAGER_H__
