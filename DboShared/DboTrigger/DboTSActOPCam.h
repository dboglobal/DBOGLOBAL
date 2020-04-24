#ifndef _DBO_TSACTOPCAM_H_
#define _DBO_TSACTOPCAM_H_


#include "DboTSCoreDefine.h"


/**
	Operation camera action
	용도 : Client quest, client trigger
	설명 : 클라이언트 전용으로 사용되는 카메라 조작 액션
*/


class CDboTSActOPCam : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	std::string							m_strFuncName;

// Constructions
public:
	CDboTSActOPCam( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_OPCAM; }

	const std::string&					GetFuncName( void ) const;
	void								SetFuncName( const std::string& strFName );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline const std::string& CDboTSActOPCam::GetFuncName( void ) const
{
	return m_strFuncName;
}


#endif