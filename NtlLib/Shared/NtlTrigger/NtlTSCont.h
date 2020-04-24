#ifndef _NTL_TSCONT_H_
#define _NTL_TSCONT_H_


#include "NtlTSEntity.h"
#include <list>


class CNtlTSRecv;
class CNtlTSScrObject;


/**
	Container entity
*/


class CNtlTSCont : public CNtlTSEntity
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::deque<CNtlTSEntity*> vecdef_EntityList;
	typedef std::list<NTL_TS_TC_ID> listdef_ContList;

// Member variables
protected:
	NTL_TS_TC_ID						m_tcID;					// 자신의 고유 아이디 값

	// 자신이 실행 할 수 있는 자식 Entity 리스트
	// CTSEvent, CTSCondition, CTSAction에서 상속 받은 객체만이
	// Container의 자식이 될 수 있다
	vecdef_EntityList					m_defChildEntityList;
	vecdef_EntityList::iterator			m_itChildEntityList;

	listdef_ContList					m_defPrevLink;			// 이전 링크
	listdef_ContList::iterator			m_itLink;

// Constructions and Destructions
public:
	CNtlTSCont( void );
	virtual ~CNtlTSCont( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return NTL_TSENTITY_TYPE_CONT_BEGIN; }

	// Container id
	NTL_TS_TC_ID						GetID( void ) const;
	NTL_TS_TC_ID						SetID( NTL_TS_TC_ID tcID );

	// Child entity
	int									GetNumOfChildEntity( void ) const;
	CNtlTSEntity*						GetChildEntity( int nIdx );
	CNtlTSEntity*						FindChildEntity( NTL_TS_TA_ID taId );
	CNtlTSEntity*						GetEventEntity( void );
	bool								AttachChildEntity( CNtlTSEntity* pEntity );
	void								DetachChildEntity( CNtlTSEntity* pEntity );

	// Previous link container
	int									GetNumOfPrevLink( void ) const;
	NTL_TS_TC_ID						BeginPrevLink( void );
	NTL_TS_TC_ID						NextPrevLink( void );
	bool								AttachPrevLink( NTL_TS_TC_ID tcId );
	void								DetachPrevLink( NTL_TS_TC_ID tcId );

	// 자신 및 자식 Entity들에 대한 탐색 및 실행
	virtual	NTL_TSRESULT				SearchAll( CNtlTSRecv* pTSRecv, void* pParam );
	virtual	NTL_TSRESULT				RunAll( CNtlTSRecv* pTSRecv, void* pParam );

// Implementations
protected:
	// 스크립트에서 사용되는 자식 관련 함수들
	virtual	CNtlTSScrObject*			BeginChildForScript( void );
	virtual	CNtlTSScrObject*			NextChildForScript( void );
	virtual	void						AttachChildForScript( CNtlTSScrObject* pChild );

public:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

public:
	virtual void						MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList );
	bool								ParsingLinkInfo( const std::string& strContents );
};


inline NTL_TS_TC_ID CNtlTSCont::GetID( void ) const
{
	return m_tcID;
}

inline int CNtlTSCont::GetNumOfChildEntity( void ) const
{
	return (int)m_defChildEntityList.size();
}

inline CNtlTSEntity* CNtlTSCont::GetChildEntity( int nIdx )
{
	return m_defChildEntityList[nIdx];
}

inline int CNtlTSCont::GetNumOfPrevLink( void ) const
{
	return (int)m_defPrevLink.size();
}


#endif