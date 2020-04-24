#ifndef __GUI_COMPONENTTOCKENIZER_H__
#define __GUI_COMPONENTTOCKENIZER_H__

#include "gui_define.h"


typedef void (*CallComponentTokenPack)(const RwChar *pFileName, void **pData, unsigned int *uiSize);
extern CallComponentTokenPack g_fnCallPack;

void LinkGuiScriptLoad(CallComponentTokenPack fn);
void UnLinkGuiScriptLoad(void);

START_GUI

// Kell Comment : CComponentTokenizer는 리소스 스크립트도 파싱하기 때문에 HtmlMetaTag에서 사용하기 위하여
// Unicode용 클래스를 추가로 만듬

// MultiByte
class CComponentTokenizer
{
public:

	CComponentTokenizer(const std::string &filename, bool bUsePack = true);
	CComponentTokenizer(const char *pMemory, int nSize);
	~CComponentTokenizer();

	bool Load(const char *pFileName, bool bUsePack);
	bool IsSuccess(VOID);
	bool IsSpace(char c);
	bool IsOperator(char c);
	bool IsRemark(char c, int position);
	std::string GetNextToken(int *offset=NULL);
	std::string PeekNextToken(int *offset=NULL);
	VOID PopToPeek(VOID);
	std::string WriteError(std::string err_msg);

private:

	class CToken
	{
	public:
		CToken(std::string t, int o, int l) : token(t), offset(o), line(l) { ; }
		std::string token;
		int offset;
		int line;
	};

	std::deque<CToken> m_Tokens;

	bool		m_bSuccess;
	int			m_nPeekPos;
	int			m_nLastLine;
	bool		m_bInRemark;

	char		*m_pData;
	int			m_nTotalSize;
	std::string m_strFileName;

#define TOKEN_BUFF_LEN	1024
	char		m_pTemp[TOKEN_BUFF_LEN];

	VOID Tokenize(VOID);

};

// Unicode
class CComponentTokenizerW
{
public:
	CComponentTokenizerW(const std::string &filename, bool bUsePack = true);
	CComponentTokenizerW(const wchar_t *pMemory, int nSize);
	~CComponentTokenizerW();

	bool Load(const char *pFileName, bool bUsePack);
	bool IsSuccess(VOID);
	bool IsSpace(wchar_t c);
	bool IsOperator(wchar_t c);
	bool IsRemark(wchar_t c, int position);
	std::wstring GetNextToken(int *offset=NULL);
	std::wstring PeekNextToken(int *offset=NULL);
	VOID PopToPeek(VOID);
	std::wstring WriteError(std::wstring err_msg);

private:
	class CToken
	{
	public:
		CToken(std::wstring t, int o, int l) : token(t), offset(o), line(l) { ; }
		std::wstring token;
		int offset;
		int line;
	};

	std::deque<CToken> m_Tokens;

	bool		m_bSuccess;
	int			m_nPeekPos;
	int			m_nLastLine;
	bool		m_bInRemark;

	wchar_t		*m_pData;
	int			m_nTotalSize;
	std::string m_strFileName;

	wchar_t		m_pTemp[TOKEN_BUFF_LEN];

	VOID Tokenize(VOID);
};


END_GUI

#endif