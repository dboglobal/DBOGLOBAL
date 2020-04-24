#ifndef _NTL_TSUIFACTORY_H_
#define _NTL_TSUIFACTORY_H_


class CNtlTSUIObject;
class CNtlTSUIFactoryType_CNtlTSUIObject;


/**
	UI factory
*/


class CNtlTSUIFactory
{
// Declarations
public:
	typedef std::map<std::string, CNtlTSUIFactoryType_CNtlTSUIObject*> mapdef_UITypeList;

// Member variables
protected:
	mapdef_UITypeList					m_defUITypeList;		// 실시간 동적 생성을 지원하기 위한 타입 리스트

// Constructions and Destructions
public:
	virtual ~CNtlTSUIFactory( void );

// Methods
public:
	virtual	CNtlTSUIObject*				CreateObj( const std::string& strName );
	virtual void						DeleteObj( CNtlTSUIObject*& pObj );

	// Runtime 생성을 위한 UI type 등록
	virtual	void						RegisterUIType( void ) { return; }
	virtual	void						UnRegisterUIType( void );
};


#endif