#ifndef _DBO_TSENTITYFACTORY_H_
#define _DBO_TSENTITYFACTORY_H_


/**
	Entity factory
*/


class CDboTSEntityFactory : public CNtlTSEntityFactory
{
// Methods
public:
	// Runtime 생성을 위한 Entity type 등록
	virtual	void						RegisterEntityType( void );
};


#endif