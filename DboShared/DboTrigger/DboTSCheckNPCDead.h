#ifndef _DBO_TSCHECKNPCDEAD_H_
#define _DBO_TSCHECKNPCDEAD_H_


#include "DboTSCoreDefine.h"


class CDboTSCheckNPCDead : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::vector<unsigned int> vecdef_NPC_INDEX_LIST;


// Member variables
protected:
	vecdef_NPC_INDEX_LIST				m_defNPCIndexList;


// Constructions
public:
	CDboTSCheckNPCDead( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_NPCDEAD; }

	int									GetSizeOfNPCList( void ) const;
	unsigned int						GetNPCTableIndex( int nIdx );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

	bool								SetNPCIndexList( const std::string& strNPCIndexList );
};


inline int CDboTSCheckNPCDead::GetSizeOfNPCList( void ) const
{
	return (int)m_defNPCIndexList.size();
}


inline unsigned int CDboTSCheckNPCDead::GetNPCTableIndex( int nIdx )
{
	return m_defNPCIndexList[nIdx];
}


#endif