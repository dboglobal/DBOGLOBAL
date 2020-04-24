#pragma once


#include "Changer.h"


class CFinder : public CChanger
{
public:
	typedef std::map< CString, CString > mapdef_FindList;

	struct sFIND_ENTITY
	{
		NTL_TS_T_ID			tId;
		NTL_TS_TG_ID		tgId;
		NTL_TS_TC_ID		tcId;
		NTL_TS_TA_ID		taId;
		CString				strType;
		CString				strAttr;
	};

	typedef std::vector< sFIND_ENTITY > vecdef_FindResultList;

protected:
	mapdef_FindList				m_defFindList;
	vecdef_FindResultList		m_defFindResultList;


public:
	CFinder( CTSTrigger* pTSTrigger );
	~CFinder( void );


public:
	virtual CString			Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr );

public:
	void					AddFindEntity( CString& strType );

	vecdef_FindResultList*	GetFindResult( void ) { return &m_defFindResultList; }
};
