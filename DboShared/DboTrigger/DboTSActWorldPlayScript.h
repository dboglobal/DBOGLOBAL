#ifndef _DBO_WORLDPLAYSCRIPT_H_
#define _DBO_WORLDPLAYSCRIPT_H_


#include "DboTSCoreDefine.h"


class CDboTSActWorldPlayScript : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	bool								m_bStart;			// Startup / Shutdown
	bool								m_bUserOutFailed;	// At the end of the game Failed user process
	unsigned int						m_uiScriptID;		// World play script ID


// Constructions
public:
	CDboTSActWorldPlayScript( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_WORLDPLAYSCRIPT; }

	bool								IsStart( void ) const;
	void								SetStart( bool bStart );

	bool								IsUserOutFailed( void ) const;
	void								SetUserOutFailed( bool bUserOutFailed );

	unsigned int						GetScriptID( void ) const;
	void								SetScriptID( unsigned int uiScriptID );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline bool CDboTSActWorldPlayScript::IsStart( void ) const
{
	return m_bStart;
}

inline bool CDboTSActWorldPlayScript::IsUserOutFailed( void ) const
{
	return m_bUserOutFailed;
}

inline unsigned int CDboTSActWorldPlayScript::GetScriptID( void ) const
{
	return m_uiScriptID;
}


#endif