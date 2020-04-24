#pragma once

#include "NtlWorldConceptController.h"

/**
 * \ingroup NtlSimulation
 * \brief 개인 Trade시의 제어 클래스 (현재는 아무것도 제어하지 않는다)
 * \date 2007-06-14
 * \author agebreak
 */
class CNtlWorldConceptTrade : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptTrade(void);
	~CNtlWorldConceptTrade(void);

	virtual void		ChangeState( RwInt32 eState );

	VOID				SetOtherPCHandle(SERIAL_HANDLE hHandle);
	SERIAL_HANDLE		GetOtherPCHandler();

protected:
	SERIAL_HANDLE		m_hOtherPC;
};

inline VOID CNtlWorldConceptTrade::SetOtherPCHandle(SERIAL_HANDLE hHandle)
{
	m_hOtherPC = hHandle;
}

inline SERIAL_HANDLE CNtlWorldConceptTrade::GetOtherPCHandler()
{
	return m_hOtherPC;
}