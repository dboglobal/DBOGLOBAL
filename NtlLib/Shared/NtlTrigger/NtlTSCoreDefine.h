#ifndef _NTLTSCORETDEFINE_H_
#define _NTLTSCORETDEFINE_H_

#include <list>

/** 
	Code position trace 
*/

#define LINE_IMP(l)				#l
#define LINE(l)					LINE_IMP( l )
#define TS_CODE_TRACE()			"[" __FILE__ "] [" __FUNCTION__ "] [" LINE( __LINE__ ) "]"


/** 
	Result types
*/


typedef unsigned int NTL_TSRESULT;

#define NTL_TSRESULT_TYPE_SUCCESS				(0x00000000)	// 성공
#define NTL_TSRESULT_TYPE_ERROR					(0x10000000)	// 프로그램적인 실패

#define NTL_TSRESULT_TYPE_COND_CAN_PROGRESS		(0x00000001)	// 조건 검사에서 조건은 만족하지 못했지만 앞으로 만족할 가능성 있음
#define NTL_TSRESULT_TYPE_COND_CANT_PROGRESS	(0x00000002)	// 조건 검사에서 앞으로 조건 검사를 해도 무조건 False 가 나오는 상황


/** 
	Trigger system entity type
*/

// Invalid entity type
#define NTL_TSENTITY_TYPE_INVALID				(0xffffffff)

// Event ( 0x00000000 <= Range < 0x0FFFFFFF )
#define NTL_TSENTITY_TYPE_EVENT_BEGIN			(0x00000000)
#define NTL_TSENTITY_TYPE_EVENT_NEXT			(NTL_TSENTITY_TYPE_EVENT_BEGIN+1)
#define NTL_TSENTITY_TYPE_EVENT_END				(0x0FFFFFFF)

// Condition ( 0x10000000 <= Range < 0x1FFFFFFF )
#define NTL_TSENTITY_TYPE_COND_BEGIN			(0x10000000)
#define NTL_TSENTITY_TYPE_COND_NEXT				(NTL_TSENTITY_TYPE_COND_BEGIN+1)
#define NTL_TSENTITY_TYPE_COND_END				(0x1FFFFFFF)

// Action ( 0x20000000 <= Range < 0x2FFFFFFF )
#define NTL_TSENTITY_TYPE_ACT_BEGIN				(0x20000000)
#define NTL_TSENTITY_TYPE_ACT_NEXT				(NTL_TSENTITY_TYPE_ACT_BEGIN+1)
#define NTL_TSENTITY_TYPE_ACT_END				(0x2FFFFFFF)

// Container ( 0x30000000 <= Range < 0x3FFFFFFF )
#define NTL_TSENTITY_TYPE_CONT_BEGIN			(0x30000000)
#define NTL_TSENTITY_TYPE_CONT_NEXT				(NTL_TSENTITY_TYPE_CONT_BEGIN+1)
#define NTL_TSENTITY_TYPE_CONT_END				(0x3FFFFFFF)

// Group ( 0x40000000 <= Range < 0x4FFFFFFF )
#define NTL_TSENTITY_TYPE_GROUP_BEGIN			(0x40000000)
#define NTL_TSENTITY_TYPE_GROUP_NEXT			(NTL_TSENTITY_TYPE_GROUP_BEGIN+1)
#define NTL_TSENTITY_TYPE_GROUP_END				(0x4FFFFFFF)

// Trigger ( 0x50000000 <= Range < 0x5FFFFFFF )
#define NTL_TSENTITY_TYPE_TRIGGER_BEGIN			(0x50000000)
#define NTL_TSENTITY_TYPE_TRIGGER_NEXT			(NTL_TSENTITY_TYPE_TRIGGER_BEGIN+1)
#define NTL_TSENTITY_TYPE_TRIGGER_END			(0x5FFFFFFF)


/** 
	Trigger
*/


typedef unsigned short NTL_TS_T_ID;

#define NTL_TS_T_ID_INVALID						(0xFFFF)


/** 
	Trigger group
*/


typedef unsigned char NTL_TS_TG_ID;

#define NTL_TS_TG_ID_INVALID					(0xFF)


/** 
	Trigger container
*/


typedef unsigned char NTL_TS_TC_ID;

#define NTL_TS_TC_ID_INVALID					(0xFF)


/** 
	Trigger action
*/


typedef unsigned char NTL_TS_TA_ID;

#define NTL_TS_TA_ID_INVALID					(0xFF)


/** 
	Trigger Event
*/
typedef unsigned int NTL_TS_EVENT_ID;	

#define NTL_TS_EVENT_ID_INVALID					(0xFFFFFFFF)


#endif