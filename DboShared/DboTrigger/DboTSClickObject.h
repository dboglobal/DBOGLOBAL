#ifndef _DBO_TSCLICKOBJCT_H_
#define _DBO_TSCLICKOBJCT_H_


#include "DboTSCoreDefine.h"


/**
	Click object event
	Purpose: Client quest, client trigger, server quest, server trigger pc
	Description: The client, used as a foundation event that occurs when the Object is selected
	The server used for the purpose of checking that the appropriate Object is selected
*/


class CDboTSClickObject : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::map<unsigned int, unsigned int> mapdef_OBJECT_LIST;

// Member variables
protected:
	unsigned int						m_uiWorldIdx;
	mapdef_OBJECT_LIST					m_defObjIdxList;
	mapdef_OBJECT_LIST::iterator		m_defObjIdxListIT;

// Constructions
public:
	CDboTSClickObject( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_CLICK_OBJECT; }

	unsigned int						GetWorldIdx( void ) const;
	void								SetWorldIdx( unsigned int uiWorldIdx );

	bool								HasObjectIdx( unsigned int uiObjIdx );

	int									GetNumOfObjectIdx( void ) const;
	unsigned int						BeginObjectIdx( void );
	unsigned int						NextObjectIdx( void );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

	void								PackingObjectIdxList( std::string& strObjList, const mapdef_OBJECT_LIST& defObjList );
	void								UnPackingObjectIdxList( const std::string& strObjList, mapdef_OBJECT_LIST& defObjList );
};


inline unsigned int CDboTSClickObject::GetWorldIdx( void ) const
{
	return m_uiWorldIdx;
}

inline int CDboTSClickObject::GetNumOfObjectIdx( void ) const
{
	return (int)m_defObjIdxList.size();
}


#endif