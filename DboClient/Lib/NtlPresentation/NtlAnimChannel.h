#ifndef __NTL_ANIM_CHANNEL_H__
#define __NTL_ANIM_CHANNEL_H__

#include "NtlInstanceAnimData.h"
#include "NtlDebug.h"

/*!
 * \Animation
 * Animation Channel(하나)를 처리 한다.
 * 
 */
class CNtlAnimChannel
{
public:
	CNtlAnimChannel() : m_pHierarchy(NULL), m_pAnimData(NULL), m_bLoop(FALSE), m_bAnimEnd(FALSE) {};
	virtual ~CNtlAnimChannel() {};

protected:
	RpHAnimHierarchy	*m_pHierarchy;							///< Hierarchy
	SInstanceAnimData	*m_pAnimData;							///< Animation Instance Data Reference

	RwBool				m_bLoop;								///< Loop Animation
	RwBool				m_bAnimEnd;								///< Loop가 아닐경우 Animation이 끝났는지

public:
	RwBool	Create(RpHAnimHierarchy *pBaseHierarchy);
	RwBool	Update(RwReal fElapsed);
	void	Destroy();
	
	RpHAnimHierarchy *GetHierarchy();							///< Get Hierarchy
	
	void	SetAnimData(SInstanceAnimData *pInstanceAnimData);	///< Set AnimData
	SInstanceAnimData *GetAnimData();							///< Get AnimData

	void	SetLoop(RwBool bLoop) { m_bLoop = bLoop; };
	RwBool	GetLoop() { return m_bLoop; };

	void	SetAnimEnd(RwBool bAnimEnd) { m_bAnimEnd = bAnimEnd; }
	RwBool	GetAnimEnd() { return m_bAnimEnd; }
};

#endif