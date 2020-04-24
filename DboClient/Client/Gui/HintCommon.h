/******************************************************************************
* File			: HintCommon.h
* Author		: Cho Haesung
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 
*****************************************************************************
* Desc			: PoPo Icon을 통하여 출력되는 힌트들
*****************************************************************************/

#ifndef __HINT_COMMON_H__
#define __HINT_COMMON_H__

#pragma once

#define MAX_HINT_STRING_TITLE_LEN 64		// Popo Notify 제목의 길이 ( 32글자 )
#define MAX_HINT_STRING_BUFFER_LEN 128		// Popo Notify 내용의 길이 ( 64글자 )

/**
* \ingroup client
* \brief Hint data base class
*/
class CHint
{
public:
	enum eHintType
	{
		eHINT_TYPE_HELP,			// 도움말 힌트
		eHINT_TYPE_GUIDE,			// 안내 힌트
		eHINT_TYPE_STRING,			// 문자열 힌트
		eHINT_TYPE_NUMS,			// 힌트 타입 갯수
		eHINT_TYPE_INVALID = eHINT_TYPE_NUMS
	};

	CHint();
	virtual ~CHint();

	eHintType	GetType();
	VOID		SetType(eHintType uiHintType);

protected:
	eHintType m_uiHintType;
};

/**
* \ingroup client
* \brief HelpHint
*/
class CHelpHint : public CHint
{
public:
	CHelpHint(RwUInt32 uiEventType, RwUInt32 uiTableIndex, eHintType byType = CHint::eHINT_TYPE_HELP );
	virtual ~CHelpHint();

	RwUInt32	GetEventType();
	RwUInt32	GetTableIndex();

protected:
	RwUInt32	m_uiEventType;
	RwUInt32	m_uiTableIndex;
};

/**
* \ingroup client
* \brief String Hint ( Popo Notify )
*/
class CStringHint : public CHint
{
public:
	CStringHint( const WCHAR* pwcTitle, const WCHAR* pwcBuffer, RwBool bImmediate = FALSE,
		RwReal fLifeTime = 0.0f, eHintType byType = CHint::eHINT_TYPE_STRING);
	virtual ~CStringHint();

	const WCHAR* GetTitle();
	const WCHAR* GetBuffer();
	RwBool		 GetImmdiate();
	RwReal		 GetLifeTime();

protected:
	WCHAR		m_awcTitle[MAX_HINT_STRING_TITLE_LEN];
	WCHAR		m_awcBuffer[MAX_HINT_STRING_BUFFER_LEN];
	RwBool		m_bImmediate;
	RwReal		m_fLifeTime;
};

#endif//__HINT_COMMON_H__