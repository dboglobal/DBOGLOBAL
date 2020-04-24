#ifndef _NTL_TSSCRFACTORYTYPE_H_
#define _NTL_TSSCRFACTORYTYPE_H_


class CNtlTSScrObject;


/** 
	Script factory type
	Script를 통해 자동 생성되기 위해서는 아래 클래스를 상속받아 Factory type 클래스를 구현해야 함

	( 주의 )
	스크립트를 통해서 런타임 시 객체 생성을 지원하기 위해서는 CNtlTSScrFactoryType 을 상속받는 클래스에서
	반드시 TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE 를 선언 해야 함
*/


// 생성하고자 하는 오브젝트의 클래스 이름을 파라미터로 입력 한다
#define NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( classname ) \
	public: \
	static const char* OBJ_CLASS_NAME( void ) { return classname::RTTI.GetClassName(); }


class CNtlTSScrFactoryType
{
// Declarations
public:
	struct sPROPERTY_TYPE
	{
		// Declaration
		enum eDATA_TYPE
		{
			eDATA_TYPE_INVALID,

			eDATA_TYPE_STRING,
			eDATA_TYPE_BOOL,
			eDATA_TYPE_NUMBER
		};

		sPROPERTY_TYPE( void ) : eType( eDATA_TYPE_INVALID ) { return; }

		// Member variables
		eDATA_TYPE	eType;
	};
	typedef std::map<std::string, sPROPERTY_TYPE> hashdef_PropertyList;

	struct sPROPERTY_INFO
	{
		const std::string*				pstrPropertyName;
		const sPROPERTY_TYPE*			pstPropertyType;
	};

// Member variables
protected:
	sPROPERTY_INFO						m_stPropertyInfo;
	hashdef_PropertyList				m_defPropertyList;
	hashdef_PropertyList::iterator		m_itPropertyList;

// Constructions and Destructions
public:
	virtual ~CNtlTSScrFactoryType( void ) { return; }

// Methods
public:
	sPROPERTY_INFO*						BeginProperty( void );
	sPROPERTY_INFO*						NextProperty( void );

	bool								HasProperty( const std::string& strPropertyName );
	sPROPERTY_TYPE::eDATA_TYPE			GetPropertyType( const std::string& strPropertyName );

	virtual CNtlTSScrObject*			CreateObj( void ) = 0;
	virtual void						DeleteObj( CNtlTSScrObject*& pObj ) = 0;
};


inline CNtlTSScrFactoryType::sPROPERTY_INFO* CNtlTSScrFactoryType::BeginProperty( void )
{
	if ( m_defPropertyList.empty() ) return 0;
	m_itPropertyList = m_defPropertyList.begin();

	m_stPropertyInfo.pstrPropertyName = &m_itPropertyList->first;
	m_stPropertyInfo.pstPropertyType = &m_itPropertyList->second;

	return &m_stPropertyInfo;
}

inline CNtlTSScrFactoryType::sPROPERTY_INFO* CNtlTSScrFactoryType::NextProperty( void )
{
	++m_itPropertyList;
	if ( m_itPropertyList == m_defPropertyList.end() ) return 0;

	m_stPropertyInfo.pstrPropertyName = &m_itPropertyList->first;
	m_stPropertyInfo.pstPropertyType = &m_itPropertyList->second;

	return &m_stPropertyInfo;
}

inline bool CNtlTSScrFactoryType::HasProperty( const std::string& strPropertyName )
{
	return ( m_defPropertyList.find( strPropertyName ) != m_defPropertyList.end() );
}

inline CNtlTSScrFactoryType::sPROPERTY_TYPE::eDATA_TYPE CNtlTSScrFactoryType::GetPropertyType( const std::string& strPropertyName )
{
	hashdef_PropertyList::iterator it = m_defPropertyList.find( strPropertyName );
	if ( it == m_defPropertyList.end() ) return sPROPERTY_TYPE::eDATA_TYPE_INVALID;
	return it->second.eType;
}


#endif