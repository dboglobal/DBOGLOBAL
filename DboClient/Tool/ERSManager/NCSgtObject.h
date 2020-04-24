#pragma once

#include <Windows.h>
#include <assert.h>


template <class T> class NCSgtObject;


// NCSgtObject<classname>와 동일.
#if !defined (NCSGT)
#define NCSGT(classname) \
	Sgt##classname
#endif /* NCSGT */


//-------------------------------------------------------------
/*! '싱글톤-오브젝트' 관련 매크로 정의.
# NCDeclareSGT	: 해당클래스 앞에 Sgt를 붙인 이름을
#				 '싱글톤-오브젝트'로 명한다.
#
# NCDefInitSGT,	
# NCInitSGT		: '싱글톤-오브젝트'를 초기화하는 함수를 호출.
# NCDestroySGT	: '싱글톤-오브젝트'를 해제시키는 함수를 호출.
#
# NCGetSGT		: '싱글톤-오브젝트'를 참조하는데 사용.
*/
//------------------------------------------------------------
#if !defined (NCDeclareSGT)
#define NCDeclareSGT(classname) \
class classname; \
	typedef NCSgtObject<classname> NCSGT(classname)
#endif /* NCDeclareSGT */


#if !defined (NCDefInitSGT) 
#define NCDefInitSGT(classname) \
	NCSGT(classname)::Init() // default constructor
#endif /* NCDefInitSGT */

#if !defined (NCInitSGT)
#define NCInitSGT(classname, argu) \
{ \
	NCSGT(classname)::Init( false ); \
	assert( !NCSGT(classname)::ms_pObject ); \
	NCSGT(classname)::ms_pObject = NCNEW classname(argu); \
} 
#endif /* NCInitSGT */


#if !defined (NCDestroySGT)
#define NCDestroySGT(classname) \
	NCSGT(classname)::Destroy()
#endif /* NCDestroySGT */


#if !defined (NCGetSGT)
#define NCGetSGT(classname) \
	NCSGT(classname)::Get()
#endif /* NCGetSGT */

#if !defined (NCIsInitedSGT)
#define NCIsInitedSGT(classname) \
	NCSGT(classname)::IsInit()
#endif /* NCIsInitedSGT */


//----------------------------------------------------
/*! '싱글톤(Singleton)-오브젝트' 클래스
# 만약, 클래스의 T의 생성자와 소멸자 둘 중 하나라도
#public 되어있지 않을 경우,
# 싱글톤-오브젝트로 사용할 수 없다.
* \brief 싱글톤-오브젝트 클래스
*/
//----------------------------------------------------
template <class T> class NCSgtObject
{ 
public:
	/*! 싱글톤-오브젝트를 초기화(메모리 할당)시킨다.
	# 만약, 이미 할당되어 있을 경우에는 메모리를 재할당하지 않는다.
	*/
	static void Init();
	static void Init( bool binit );

	/*! 싱글톤-오브젝트를 해제시킨다.
	# 초기화 된 오브젝트는 반드시 Destory()를 호출해 주어야 한다.
	*/
	static void Destroy();

	/*! 싱글톤-오브젝트를 참조한다.
	# 반드시 초기화(Init) 후 호출한다.
	*/
	static T* Get();


	static bool IsInit();


	//---------------
	// 생성자/소멸자
	//---------------
private:
	NCSgtObject( bool binit = true );

public:
	virtual ~NCSgtObject();

	//----------
	// 맴버변수
	//----------
public:
	static T* ms_pObject;
private:
	static NCSgtObject<T>* ms_pThis;
};







// inline
#include "NCSgtObject.inl"



