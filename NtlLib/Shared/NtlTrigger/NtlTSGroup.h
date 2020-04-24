#ifndef _NTL_TSGROUP_H_
#define _NTL_TSGROUP_H_


#include "NtlTSEntity.h"


class CNtlTSScrObject;
class CNtlTSCont;


/**
	Group entity
*/


class CNtlTSGroup : public CNtlTSEntity
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::map<NTL_TS_TG_ID, CNtlTSCont*> mapdef_ContList;

// Member variables
protected:
	NTL_TS_TG_ID						m_tgID;

	mapdef_ContList						m_defChildContList;
	mapdef_ContList::iterator			m_itChildContList;

// Constructions and Destructions
public:
	CNtlTSGroup( void );
	virtual ~CNtlTSGroup( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return NTL_TSENTITY_TYPE_GROUP_BEGIN; }

	// Group id
	NTL_TS_TG_ID						GetID( void ) const;
	NTL_TS_TG_ID						SetID( NTL_TS_TG_ID uID );

	// Child container
	int									GetNumOfChildCont( void ) const;
	CNtlTSCont*							GetChildCont( NTL_TS_TC_ID tcID );
	CNtlTSCont*							GetBeginChildCont( void );
	CNtlTSCont*							GetNextChildCont( void );
	void								AttachChildCont( CNtlTSCont* pChildCont );
	CNtlTSCont*							DetachChildCont( NTL_TS_TC_ID tcID );
	CNtlTSCont*							DetachChildCont( CNtlTSCont* pChildCont );

	// Navigation and run against his and child Entity
	virtual	NTL_TSRESULT				SearchAll( CNtlTSRecv* pTSRecv, void* pParam );
	NTL_TSRESULT						SearchTarget( NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam );

	NTL_TSRESULT						RunTarget( NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam );

// Implementations
protected:
	// Child-related functions used in the script
	virtual	CNtlTSScrObject*			BeginChildForScript( void );
	virtual	CNtlTSScrObject*			NextChildForScript( void );
	virtual	void						AttachChildForScript( CNtlTSScrObject* pChild );

	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

public:
	virtual void						MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList );
};


inline NTL_TS_TG_ID CNtlTSGroup::GetID( void ) const
{
	return m_tgID;
}

inline int CNtlTSGroup::GetNumOfChildCont( void ) const
{
	return (int)m_defChildContList.size();
}


#endif