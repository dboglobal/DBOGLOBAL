#ifndef _DBO_TSCOLOBJCT_H_
#define _DBO_TSCOLOBJCT_H_


#include "DboTSCoreDefine.h"


/**
	Collsion object event
	용도 : Client quest, client trigger, server quest, server trigger pc
	설명 : 클라이언트에서는 Object가 충돌되었을 때 발생하는 이벤터로써 사용됨
		   서버에서는 해당 Object가 충돌되었는지( 해당 영역에 들어왔는지 ) 검사하는 용도로 사용됨
*/


class CDboTSColObject : public CNtlTSEvent
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
	CDboTSColObject( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_COL_OBJECT; }

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


inline unsigned int CDboTSColObject::GetWorldIdx( void ) const
{
	return m_uiWorldIdx;
}

inline int CDboTSColObject::GetNumOfObjectIdx( void ) const
{
	return (int)m_defObjIdxList.size();
}


#endif