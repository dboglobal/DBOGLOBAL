/*****************************************************************************
 *
 * File			: NtlTokenizer.h
 * Author		: HyungSuk, Jang
 * Copyright	: (ÁÖ)NTL
 * Date			: 2005. 9. 22	
 * Abstract		: language/help string table base tokenizer
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_TOKENIZER_H__
#define __NTL_TOKENIZER_H__

#include <string>
#include <deque>

typedef void (*CallTokenPack)(const char* pFileName, void **pData, int *nSize);

class CNtlTokenizer
{
private:

	class CNtlToken
	{
	public:
		CNtlToken(std::string strToken, int iOffsett, int iLine) : strToken(strToken), iOffset(iOffsett), iLine(iLine) { ; }
		std::string strToken;
		int iOffset;
		int iLine;
	};

	std::deque<CNtlToken> m_dqTokens;

	BOOL		m_bSuccess;
	int			m_iPeekPos;
	int			m_iLastLine;
	BOOL		m_bInRemark;

	char		*m_pData;
	int			m_iTotalSize;
	std::string m_strFileName;

#define NTL_TOKEN_BUFF_LEN	1024
	char		m_pTemp[NTL_TOKEN_BUFF_LEN];

	void Tokenize(void);


public:

	CNtlTokenizer(const std::string &strFileName, CallTokenPack fnCallPack = NULL);
	CNtlTokenizer(const char *pBuffer);
	~CNtlTokenizer();

	BOOL Load(const char *pFileName, CallTokenPack fnCallPack);
	BOOL IsSuccess(void);
	std::string GetNextToken(int *pOffset=0, int *pLine = 0);
	std::string PeekNextToken(int *pOffset=0, int *pLine = 0);
	void PopToPeek(void);
	
	virtual BOOL IsSpace(char c);
	virtual BOOL IsOperator(char c);
	virtual BOOL IsRemark(char c, int iPosition);

	std::string WriteError(std::string strErrMsg);
};

/**
* \brief Wide Character Çü(Unicode) Tokenizer
*/
class CNtlTokenizerW
{
private:

	class CNtlTokenW
	{
	public:
		CNtlTokenW(std::wstring wstrToken, int iOffsett, int iLine) : wstrToken(wstrToken), iOffset(iOffsett), iLine(iLine) { ; }
		std::wstring wstrToken;
		int iOffset;
		int iLine;
	};

	std::deque<CNtlTokenW> m_dqTokens;

	BOOL		m_bSuccess;
	int			m_iPeekPos;
	int			m_iLastLine;
	BOOL		m_bInRemark;

	wchar_t		*m_pData;
	int			m_iTotalSize;
	std::string m_strFileName;

#define NTL_TOKEN_BUFF_LEN	1024
	wchar_t		m_pTemp[NTL_TOKEN_BUFF_LEN];

	void Tokenize(void);


public:

	CNtlTokenizerW(const std::string &strFileName, CallTokenPack fnCallPack = NULL);
	CNtlTokenizerW(const wchar_t *pBuffer);
	~CNtlTokenizerW();

	BOOL Load(const char *pFileName, CallTokenPack fnCallPack);
	BOOL IsSuccess(void);
	std::wstring GetNextToken(int *pOffset=0, int *pLine = 0);
	std::wstring PeekNextToken(int *pOffset=0, int *pLine = 0);
	void PopToPeek(void);

	virtual BOOL IsSpace(wchar_t c);
	virtual BOOL IsOperator(wchar_t c);
	virtual BOOL IsRemark(wchar_t c, int iPosition);

	std::string WriteError(std::string strErrMsg);
};

#endif