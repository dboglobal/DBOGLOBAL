#ifndef _NTL_TSSCRFACTORY_H_
#define _NTL_TSSCRFACTORY_H_


class CNtlTSScrObject;
class CNtlTSScrFactoryType;


/** 
	Script factory
	Script 를 통해서 객체를 자동생성하기 위해서는 아래 클래스를 상속받아 Factory 클래스를 구현해야 함
*/


class CNtlTSScrFactory
{
// Constructions and Destructions
public:
	virtual ~CNtlTSScrFactory( void ) { return; }

// Methods
public:
	// 생성하고자 하는 클래스 이름을 파라미터로 입력하면,
	// 해당 클래스를 생성할 수 있는 FactoryType이 리턴된다
	virtual	CNtlTSScrFactoryType*		GetFactoryType( const std::string& strClassName ) = 0;

	virtual	CNtlTSScrObject*			CreateObj( const std::string& strTypeName ) = 0;
	virtual void						DeleteObj( CNtlTSScrObject*& pObj ) = 0;
};


#endif