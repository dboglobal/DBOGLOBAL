#ifndef _DBO_TSCHECKATTACHOBJ_H_
#define _DBO_TSCHECKATTACHOBJ_H_


#include "DboTSCoreDefine.h"


/**
	Check attach object
*/


class CDboTSCheckAttachObj : public CNtlTSCond
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
	CDboTSCheckAttachObj( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_ATTACH_OBJ; }

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


inline unsigned int CDboTSCheckAttachObj::GetWorldIdx( void ) const
{
	return m_uiWorldIdx;
}

inline int CDboTSCheckAttachObj::GetNumOfObjectIdx( void ) const
{
	return (int)m_defObjIdxList.size();
}


#endif