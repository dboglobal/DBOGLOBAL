/*****************************************************************************
*
* File			: SayFilter.h
* Author		: JinSung, Kim ( Modified Haesung, Cho )
* Copyright		: (주)NTL
* Date			: 2007. 9. 4
* Abstract		: Say message filtering manager
*****************************************************************************
* Desc			:
*
* DBO 문자열 필터링
* 현재는 테이블이 로드되지 않았어도 게임 플레이가 가능 (필터되지 않음)
* (정책적으로 상의해야 할 사항들)
*****************************************************************************/

#ifndef _SAYFILTER_H_
#define _SAYFILTER_H_

#pragma once

// include
#include "stdafx.h"
#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include "NtlDebug.h"


/**
* \ingroup Client
* 필터될 단어들을 한 글자씩 분리하여 맵의 키 값에 삽입
*/
template <typename T>
class CFilterWord
{
public:
	typedef std::map<T, CFilterWord<T>*>					FILTERMAP;
	typedef std::char_traits<T>								TCHAR_TRAITS;
	typedef std::allocator<T>								TALLOCATOR;
	typedef	std::basic_string<T, TCHAR_TRAITS, TALLOCATOR>	TSTRING;

public:
	// 대체할 텍스트를 저장
	struct SResult
	{
		TSTRING strText;
		UINT	uPosStart;
		UINT	uPosEnd;
		int		nLink;
	};

public:
	CFilterWord();
	~CFilterWord();

	int				GetNumFilter();

	void			AddText(int& nIndex, const T* pText, const int& iSize, int& nLink);
	int				FindText(const T* pText, const int& iSize, std::vector<SResult>& vecResult);

	bool			IsEnd() { return m_bEndText; }

protected:
	CFilterWord<T>*	FindFilter(const T& Text);
	void			_AddText(int& nIndex, const T* pText, const int& iSize, int& nPos, int& nLink );
	int				_FindText(const T* pText, const int& iSize, int& iIndex, TSTRING& vecResult, int& nLink);

protected:
	FILTERMAP		m_mapFilter;
	bool			m_bEndText;
	int				m_nIndex;
	int				m_nLink;
};

template <typename T>
CFilterWord<T>::CFilterWord()
{
	 m_bEndText = FALSE;
	 m_nLink = -1;
	 m_nIndex = 0;
}

template <typename T>
CFilterWord<T>::~CFilterWord()
{
	while (!m_mapFilter.empty())
	{
		CFilterWord* pTextFilter = m_mapFilter.begin()->second;
		m_mapFilter.erase(m_mapFilter.begin());

		NTL_DELETE( pTextFilter );
	}
}

template <typename T>
void CFilterWord<T>::AddText(int& nIndex, const T* pText, const int& nSize, int& nLink)
{
	int nPos	= 0;
	_AddText(nIndex, pText, nSize, nPos, nLink);
}

template <typename T>
void CFilterWord<T>::_AddText(int& nIndex, const T* pText, const int& nSize, int& nPos, int& nLink)
{
	if (nSize == nPos || pText[nPos] == NULL)
	{
		m_bEndText = true;
		m_nIndex = nIndex;
		m_nLink = nLink;
		return;
	}

	CFilterWord* pFilter = FindFilter(pText[nPos]);	
	if (pFilter == NULL)
	{
		m_mapFilter[pText[nPos]] = pFilter = NTL_NEW CFilterWord;		
	}
	pFilter->_AddText(nIndex, pText, nSize, ++nPos, nLink);	
}

template <typename T>
int CFilterWord<T>::FindText(const T* pText, const int& nSize, std::vector<SResult>& vecResult)
{
	int iResult = 0;
	TSTRING	strTemp;
	for (int i = 0; i < nSize; ++i)
	{
		int iIndex = i;
		strTemp.clear();

		int iIndexException = iIndex;
		int nLink			= 0;
		int iTemp			= _FindText(pText, nSize, iIndex, strTemp, nLink);
		int iIndexFilter	= iIndex;

		if (iTemp > 0)
		{
			SResult resultTemp;
			resultTemp.strText		= strTemp;
			resultTemp.uPosStart	= iIndexException;
			resultTemp.uPosEnd		= iIndexFilter;
			resultTemp.nLink		= nLink;

			vecResult.push_back(resultTemp);
		}

		iResult += iTemp;

		if (iIndex != i)
		{
			i = iIndex;
		}
	}
	return iResult;
}

template <typename T>
int	CFilterWord<T>::_FindText(const T* pText, const int& nSize, int& nPos, TSTRING& vecResult, int& nLink)
{
	int i		= nPos;
	int iResult	= 0;

	CFilterWord<T>* pFilter = FindFilter(pText[i]);
	if (pFilter != NULL)
	{	
		vecResult += pText[i];
        iResult = pFilter->_FindText(pText, nSize, ++i, vecResult, nLink);
		if (iResult)
		{
			nPos = i;
			return iResult;
		}
	}

	if (m_bEndText == true)
	{
		--nPos;
		nLink = m_nLink;
		return 1;
	}
	return 0;
}

template <typename T>
CFilterWord<T>* CFilterWord<T>::FindFilter(const T& Text)
{
	FILTERMAP::iterator it = m_mapFilter.find(Text);
	if (it == m_mapFilter.end())
	{
		return NULL;
	}
	return it->second;
}


template <typename T>
int CFilterWord<T>::GetNumFilter()
{
	int iNum = 0;
	for (FILTERMAP::iterator it = m_mapFilter.begin(); it != m_mapFilter.end(); ++it)
	{
		CFilterWord<T>* pFilter = it->second;
		iNum += pFilter->GetNumFilter();
	}

	if (m_bEndText)
	{
		++iNum;
	}
	return iNum;
}

/**
* \ingroup Client
* 메시지 필터링의 클래스
*/
class CFilterManager
{
public:
	typedef CFilterWord<wchar_t> CWFILTERWORD;

public:
	// 초기화 & 해제
	void InitOnce();
	void Destroy();

	// 테이블 로드
	void Create( void );

	void AddFilter( int nIndex, wchar_t* pText, int nLink );		///< 필터링 단어를 등록

	std::wstring Filtering( std::wstring& pText );					///< 필터링 (wstring)
	std::string Filtering( std::string& pText );					///< 필터링 (string)
	std::wstring Filtering( const wchar_t* pText );						///< 필터링 (wchar_t*)

	/*std::wstring FilterName( */

	RwBool IsAnyInvalidText( const WCHAR* pText );

	RwBool IsSlang( std::wstring& pText );							///< 욕설인지 체크(wstring), 욕설이라면 TRUE 리턴
	RwBool IsSlang( std::string& pText );							///< 욕설인지 체크(string)
	RwBool IsSlang( const wchar_t* pText );

	// 싱글톤의 인스턴스
	static CFilterManager* GetInstance(void)
	{
		static CFilterManager instance;

		return &instance;
	}

protected:
	CWFILTERWORD*			m_pFilterWord;
	std::wstring			m_defaultReplace;
	RwBool					m_bIsEmpty;

	// 생성자 & 소멸자
	CFilterManager();
	~CFilterManager();
};

static CFilterManager* GetChattingFilter(void)
{
	return CFilterManager::GetInstance(); 
}

#endif