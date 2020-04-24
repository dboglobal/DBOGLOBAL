#ifndef _NTL_TSCONTROLFACTORY_H_
#define _NTL_TSCONTROLFACTORY_H_


class CNtlTSControlObject;
class CNtlTSControlFactoryType_CNtlTSControlObject;


/**
	Control factory
*/


class CNtlTSControlFactory
{
// Declarations
public:
	typedef std::map<std::string, CNtlTSControlFactoryType_CNtlTSControlObject*> mapdef_CtrlTypeList;

// Member variables
protected:
	mapdef_CtrlTypeList					m_defCtrlTypeList;		// 실시간 동적 생성을 지원하기 위한 타입 리스트

// Constructions and Destructions
public:
	virtual ~CNtlTSControlFactory( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( const std::string& strName );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );

	// Runtime 생성을 위한 controller type 등록
	virtual	void						RegisterCtrlType( void ) { return; }
	virtual	void						UnRegisterCtrlType( void );
};


#endif