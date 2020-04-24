#ifndef __GUI_HTMLTOCKENIZER_H__
#define __GUI_HTMLTOCKENIZER_H__

#include "gui_define.h"

// Pack 버전용 함수
typedef void (*CallHtmlTokenPack)(const RwChar* pFileName, void **pData, int *iSize );
extern CallHtmlTokenPack g_fnHtmlCallPack;

void LinkHtmlScriptLoad(CallHtmlTokenPack fn);
void UnLinkHtmlScriptLoad(void);

START_GUI

//////////////////////////////////////////////////////////////////////////////////////////
// Unicode

class CHtmlTokenizer
{
public:

	class CToken
	{
	public:

		enum TOKEN_TYPE
		{
			TOKEN_TEXT,
			TOKEN_TAG
		};

	public:

		CToken(int tokentype, std::wstring t, int o, int l) : tokentype(tokentype), token(t), offset(o), line(l) 
		{ 
		}

		std::wstring token;
		int tokentype;
		int offset;
		int line;
	};


	CHtmlTokenizer(const std::string &filename, bool bUsePack = true);
	CHtmlTokenizer(const wchar_t *pMemory, int nSize);
	~CHtmlTokenizer();

	bool IsSuccess(void);
	bool IsEmpty(void);
	std::wstring GetNextToken(void);
	int GetTokenType(void);
	int GetOffset(void);
	int GetLine(void);

private:

	bool Load(const char *pFileName, bool bUsePack);
	void ToLower(void);
	void Tokenize(void);
	bool IsEnd(int cur_pos);
	bool IsSpace(wchar_t c);
	bool IsTagStart(wchar_t c);
	bool IsTagEnd(wchar_t c);

private:

	std::deque<CToken> m_Tokens;

	int			m_nTokenType;
	int			m_nOffset;
	int			m_nLine;

	bool		m_bSuccess;
	wchar_t		*m_pData;
	int			m_nTotalSize;
	std::string m_strFileName;
};


END_GUI

#endif