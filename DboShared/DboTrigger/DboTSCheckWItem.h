#ifndef _DBO_TSCHECKWITEM_H_
#define _DBO_TSCHECKWITEM_H_


#include "DboTSCoreDefine.h"


/**
	Check wear item condition
	용도 : Client quest, client trigger, server quest, server trigger pc
*/


class CDboTSCheckWItem : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiItemIdx;

// Constructions
public:
	CDboTSCheckWItem( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_WITEM; }

	unsigned int						GetItemIdx( void ) const;
	void								SetItemIdx( unsigned int uiItemIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSCheckWItem::GetItemIdx( void ) const
{
	return m_uiItemIdx;
}


#endif