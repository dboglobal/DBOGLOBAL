#pragma once


class CAttr_Page;


class CAttrEntityMng
{
// Declarations
public:
	typedef std::map<std::string, CRuntimeClass*> mapdef_OBJLIST;
	typedef std::map<std::string, std::string> mapdef_OBJNAME;

// Member variables
protected:
	mapdef_OBJLIST						m_defObjList;
	mapdef_OBJNAME						m_defObjName;

// Constructions and Destructions
public:
	CAttrEntityMng( void );
	virtual ~CAttrEntityMng( void );

// Methods
public:
	CAttr_Page*							GetPage( CString strType );
	CString								GetName( CString strType );
};


extern CAttrEntityMng* GetAttrEntityMng( void );