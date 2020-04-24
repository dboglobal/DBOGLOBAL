#ifndef _DBO_TSCLICKNPC_H_
#define _DBO_TSCLICKNPC_H_


#include "DboTSCoreDefine.h"


/**
	Click NPC event
	용도 : Client quest, client trigger, server quest, server trigger pc
	설명 : 클라이언트에서는 NPC가 선택되었을 때 발생하는 이벤터로써 사용됨
		   서버에서는 해당 NPC가 선택되었는지 검사하는 용도로 사용됨
*/


class CDboTSClickNPC : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

public:

	typedef std::list<unsigned int> listdef_NPCIdx;



// Member variables
protected:

	listdef_NPCIdx					m_defNPCIdx;
	listdef_NPCIdx::iterator		m_itNPCIdx;

// Constructions
public:
	CDboTSClickNPC( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_CLICK_NPC; }

	
	int								GetNumOfNPCIdx( void ) const;
	unsigned int					GetNPCIdx( void );//BeginNextNPCIdx( void );
	unsigned int					NextNPCIdx( void );
	bool							AttachNPCIdx( unsigned int NPCId );
	void							DetachNPCIdx( unsigned int NPCId );
	bool							ParsingNPCIdx( const std::string& strContents );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};

inline int CDboTSClickNPC::GetNumOfNPCIdx(void) const
{
	return (int)m_defNPCIdx.size();
}




#endif