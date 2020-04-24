#ifndef _DBO_TSCHECKOBJITEM_H_
#define _DBO_TSCHECKOBJITEM_H_


#include "DboTSCoreDefine.h"


/**
	Check obejct item condition
*/


class CDboTSCheckObjItem : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	NTL_TS_T_ID							m_tQuestId;

// Constructions and Destructions
public:
	CDboTSCheckObjItem( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_OBJITEM; }

	NTL_TS_T_ID							GetQuestId( void );
	void								SetQuestId( NTL_TS_T_ID tId );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline NTL_TS_T_ID CDboTSCheckObjItem::GetQuestId( void )
{
	return m_tQuestId;
}


#endif