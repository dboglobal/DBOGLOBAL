#ifndef _DBO_TSACTOPOBJECT_H_
#define _DBO_TSACTOPOBJECT_H_


#include "DboTSCoreDefine.h"


class CDboTSActOPObject : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	bool								m_bApply;				// True 적용, False 해제
	unsigned int						m_uiOperateTime;		// 밀리 세컨드
	unsigned int						m_uiDirectionTblIdx;	// 디렉션 테이블 인덱스

// Constructions
public:
	CDboTSActOPObject( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_OPERATEOBJECT; }

	bool								IsApply( void ) const;
	void								SetApply( bool bApply );

	unsigned int						GetOperateTime( void ) const;
	void								SetOperateTime( unsigned int uiOperateTime );

	unsigned int						GetDirectionTableIndex( void ) const;
	void								SetDirectionTableIndex( unsigned int uiDirTblIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline bool CDboTSActOPObject::IsApply( void ) const
{
	return m_bApply;
}

inline unsigned int CDboTSActOPObject::GetOperateTime( void ) const
{
	return m_uiOperateTime;
}

inline unsigned int CDboTSActOPObject::GetDirectionTableIndex( void ) const
{
	return m_uiDirectionTblIdx;
}


#endif