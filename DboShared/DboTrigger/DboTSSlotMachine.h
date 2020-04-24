#ifndef _DBO_TSSLOTMACHINE_H_
#define _DBO_TSSLOTMACHINE_H_


#include "DboTSCoreDefine.h"


/**
	Slot machine
*/


class CDboTSSlotMachine : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI


protected:

	int						m_iSt;


// Constructions
public:
	CDboTSSlotMachine( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_SLOTMACHINE; }

	int									GetSt( void ) const;

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline int CDboTSSlotMachine::GetSt( void ) const
{
	return m_iSt;
}


#endif