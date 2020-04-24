#pragma once
//-- include ------------------------------------------------------------------------
#include "NtlSharedType.h"
//-----------------------------------------------------------------------------------

#define BOTAP_IDLE1_SECOND ( 500 )				// 시간 단위 : 0.5초
#define BOTAP_IDLE2_SECOND ( 1000 )				// 시간 단위 : 1초 [12/13/2007 SGpro]

typedef TBLIDX ACTIONPATTERNTBLIDX;
typedef BYTE ACTIONPATTERNID;   //eACTIONPATTERNID를 저장

typedef BYTE ACTIONPATTERN_FIELDSIZE; //테이블에 저장되어 있는 Pattern에 수를 저장
typedef BYTE ACTIONPATTERN_FIELD;
#define INVALID_ACTIONPATTERN_FIELD (-1)

//typedef BYTE ACTIONPATTERN_RECORDSIZE; //테이블에 저장되어 있는 Pattern에 수를 저장
//typedef BYTE ACTIONPATTERN_RECORD;

enum eACTIONPATTERNID
{
	 BOTAP_MOVE = 0		//Move 패킷을 보냄
	,BOTAP_IDLE1		//0.5초짜리
	,BOTAP_IDLE2		//1초짜리
	,BOTAP_MIDLE1
	,BOTAP_MIDLE2

	,COUNT_ACTIONPATTERNID
	,INVALID_ACTIONPATTERNID = -1
};

enum eACTIONPATTERN_LIMIT
{
	 MAX_SAVECOUNT_ACTIONPATTERN_FIELD	= 64	//한개의 Pattern이 가질 수 있는 Action의 최대 필드 수	
	,MAX_SAVECOUNT_ACTIONPATTERN_RECORD = 100	//테이블에 저장할 수 있는 최대 레코드
};