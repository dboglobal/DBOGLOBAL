#ifndef _DBO_TSACTOBJCONV_H_
#define _DBO_TSACTOBJCONV_H_


#include "DboTSCoreDefine.h"


/**
	Object conversation action
*/


class CDboTSActObjConv : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiWorldTblIdx;
	unsigned int						m_uiObjIdx;
	eOBJCONV_TYPE						m_eObjConvType;
	unsigned int						m_uiObjConv;

// Constructions
public:
	CDboTSActObjConv( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_OBJCONV; }

	unsigned int						GetWorldTblIdx( void ) const;
	void								SetWorldTblIdx( unsigned int uiWorldTblIdx );

	unsigned int						GetObjIdx( void ) const;
	void								SetObjIdx( unsigned int uiObjIdx );

	eOBJCONV_TYPE						GetObjConvType( void ) const;
	void								SetObjConvType( eOBJCONV_TYPE eObjConvType );

	unsigned int						GetObjConv( void ) const;
	void								SetObjConv( unsigned int uiObjConv );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSActObjConv::GetWorldTblIdx( void ) const
{
	return m_uiWorldTblIdx;
}

inline unsigned int CDboTSActObjConv::GetObjIdx( void ) const
{
	return m_uiObjIdx;
}

inline eOBJCONV_TYPE CDboTSActObjConv::GetObjConvType( void ) const
{
	return m_eObjConvType;
}

inline unsigned int CDboTSActObjConv::GetObjConv( void ) const
{
	return m_uiObjConv;
}


#endif