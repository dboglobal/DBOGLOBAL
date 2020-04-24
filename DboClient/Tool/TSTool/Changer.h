#pragma once


class CTSTrigger;
class CAttr_Page;


class CAttrData
{
protected:
	CString					m_strAttr;

public:
	void					SetAttr( CString strAttr ) { m_strAttr = strAttr; }
	CString					GetAttr( void ) { return m_strAttr; }
	bool					HasAttr( CString strKey );
	CString					FindAttr( CString strKey );
	bool					ChangeAttr( CString strKey, CString strVal );
	CString					MakeAttr( CString& strKey, CString& strVal );
};


class CChanger
{
protected:
	CTSTrigger*				m_pTSTrigger;

public:
	CChanger( CTSTrigger* pTSTrigger );
	~CChanger( void );

public:
	void					Run( void );

public:
	virtual CString			Trigger( NTL_TS_T_ID tID, CAttrData* pAttr );
	virtual CString			Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr );
};
