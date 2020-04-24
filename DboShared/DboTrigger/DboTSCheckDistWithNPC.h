#ifndef _DBO_TSCHECKDISTWITHNPC_H_
#define _DBO_TSCHECKDISTWITHNPC_H_


#include "DboTSCoreDefine.h"


class CDboTSCheckDistWithNPC : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI


// Declarations
public:
	typedef std::vector<unsigned int> vecdef_NPC_INDEX_LIST;


// Member variables
protected:
	float								m_fDistance;
	vecdef_NPC_INDEX_LIST				m_defNPCIndexList;


// Constructions
public:
	CDboTSCheckDistWithNPC( void );


// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_DIST_WITH_NPC; }

	float								GetDistance( void ) const;
	void								SetDistance( float fDist );

	int									GetSizeOfNPCList( void ) const;
	unsigned int						GetNPCTableIndex( int nIdx );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

	bool								SetNPCIndexList( const std::string& strNPCIndexList );
};


inline float CDboTSCheckDistWithNPC::GetDistance( void ) const
{
	return m_fDistance;
}


inline void CDboTSCheckDistWithNPC::SetDistance( float fDist )
{
	m_fDistance = fDist;
}


inline int CDboTSCheckDistWithNPC::GetSizeOfNPCList( void ) const
{
	return (int)m_defNPCIndexList.size();
}


inline unsigned int CDboTSCheckDistWithNPC::GetNPCTableIndex( int nIdx )
{
	return m_defNPCIndexList[nIdx];
}


#endif