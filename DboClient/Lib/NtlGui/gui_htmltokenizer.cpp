#include "gui_precomp.h"
#include "gui_htmltokenizer.h"

CallHtmlTokenPack g_fnHtmlCallPack = NULL;

void LinkHtmlScriptLoad( CallHtmlTokenPack fn )
{
	g_fnHtmlCallPack = fn;
}

void UnLinkHtmlScriptLoad( void )
{
	g_fnHtmlCallPack = NULL;
}

START_GUI

//////////////////////////////////////////////////////////////////////////////////////////////
// Unicode



CHtmlTokenizer::CHtmlTokenizer(const std::string &filename, bool bUsePack)
{
	m_pData		 = NULL;

	m_bSuccess = Load(filename.data(), bUsePack);
	if(!m_bSuccess)
		return;

	//	대소문자 구분 안하게 만든다.
	//	ToLower();

	m_strFileName = filename;

	Tokenize();
}

CHtmlTokenizer::CHtmlTokenizer(const wchar_t *pMemory, int nSize)
{
	m_pData	= NULL;
	m_bSuccess = true;
	m_nTotalSize = nSize;
	m_pData = NTL_NEW wchar_t[nSize+1];
	m_pData[nSize] = '\0';

	wcscpy_s( m_pData, nSize+1 , pMemory );

	// 대소문자 구분 안하게 만든다.
	//	ToLower();

	Tokenize();
}

CHtmlTokenizer::~CHtmlTokenizer()
{
	NTL_ARRAY_DELETE( m_pData );
}

bool CHtmlTokenizer::IsSuccess(VOID)
{
	return m_bSuccess;
}

bool CHtmlTokenizer::Load(const char *pFileName, bool bUsePack )
{
	NTL_ARRAY_DELETE( m_pData );

	const BYTE byBomOffset = 2;
	const BYTE abyUnicodeBom[2] = { 0xFF, 0xFE };
	BYTE abyHtml_Bom[2] = { 0, };

	if( g_fnHtmlCallPack && bUsePack )
	{
		char* pData = NULL;
		int nSize;

		// 일단 버퍼에 담아서 읽어온다.
		(*g_fnHtmlCallPack)(pFileName, (void**)&pData, &nSize );
		if( pData == NULL )
			return false;

		char* pBuffer = NTL_NEW char[nSize+1];
		memcpy( pBuffer, pData, sizeof(char) * nSize );
		pBuffer[nSize] = '\0';
	
		// // UTF - 16일 경우에
		if( abyUnicodeBom[0] == pBuffer[0] &&
			abyUnicodeBom[1] == pBuffer[1] )
		{
			size_t nStrLen = strlen( pBuffer+2 );
			// Wide형의 배열은 2Byte 당 하나임.
			// 앞의 BOM 코드를 뺀 것의 2를 나눠준 만큼 복사한다.
			m_nTotalSize = (int)nStrLen;
			m_pData = NTL_NEW WCHAR[m_nTotalSize+1];
			memcpy( m_pData, pBuffer+2, nSize-2 );
			m_pData[m_nTotalSize] = L'\0';
		}
		// Ansi
		else
		{
			size_t nStrLen = strlen( pBuffer );
			m_nTotalSize = (int)nStrLen;
			m_pData = NTL_NEW WCHAR[m_nTotalSize+1];
			
			::MultiByteToWideChar( GetACP(), 0, pBuffer, -1, m_pData, m_nTotalSize);
			m_pData[m_nTotalSize] = L'\0';
		}

		// 담아놓은 버퍼를 삭제한다.
		NTL_ARRAY_DELETE( pData );
		NTL_ARRAY_DELETE( pBuffer );
	}
	else
	{
		FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		if(fopen_s(&fp, pFileName, "rb") != 0)
			return false;
#else
		fp = fopen(pFileName, "rb");
		if(fp == NULL)
			return false;
#endif

		// Kell's Comment : UTF-16/UCS2 만 검사한다. 이외의 방식은 MultiByte로 읽는다.
		// UTF-16의 Byte-order Mark는 2byte 인 FF FE 를 사용하고 이 방식은 윈도우의
		// 메모장이나 Word에서 사용하는 방식이다. (주의)

		// 앞의 2 Byte만 읽어온다.
		fread( abyHtml_Bom, 2, 1, fp );

		// Unicode(LE)가 아닌 경우 Assert가 걸린다.
		// 현재는 MultiByte와 Unicode, 2개 다 읽어들어야 한다.
		// assert( abyUnicodeBom == abyHtml_Bom );

		// UTF-16(Little-Endian)
		if( abyUnicodeBom[0] == abyHtml_Bom[0] &&
			abyUnicodeBom[1] == abyHtml_Bom[1] )
		{
			fseek(fp, byBomOffset, SEEK_END);
			int nSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			// Unicode는 2byte당 1character를 사용한다.
			int nStrLen = m_nTotalSize = (nSize/2);
			m_pData = NTL_NEW wchar_t[nStrLen];
			m_pData[nStrLen-1] = L'\0';

			fread(m_pData,  nSize, 1, fp);
		}
		// Ansi
		else 
		{
			fseek(fp, 0, SEEK_END);
			int nSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			m_nTotalSize = nSize;
			char* pData = NTL_NEW char[nSize+1];
			pData[nSize] = 0;

			fread(pData, nSize, 1, fp);

			m_pData = NTL_NEW wchar_t[nSize];

			// Wide Character로 변환
			::MultiByteToWideChar( GetACP(), 0,pData, -1, m_pData, nSize );

			NTL_ARRAY_DELETE( pData );
		}
		fclose(fp);
	}


	return true;
}

void CHtmlTokenizer::ToLower(void)
{
	for(INT i = 0; i < m_nTotalSize; i++)
	{
		m_pData[i] = tolower(m_pData[i]);
	}
}

bool CHtmlTokenizer::IsEnd(int cur_pos)
{
	if(cur_pos >= m_nTotalSize)
		return true;
	return false;
}

bool CHtmlTokenizer::IsSpace(wchar_t c)
{
	const wchar_t *pSpace = L" \t\r\n";

	return wcschr(pSpace, c) != NULL;
}

bool CHtmlTokenizer::IsTagStart(wchar_t c)
{
	const wchar_t *pOperators = L"[";

	return wcschr(pOperators, c) != NULL;
}

bool CHtmlTokenizer::IsTagEnd(wchar_t c)
{
	const wchar_t *pOperators = L"]";

	return wcschr(pOperators, c) != NULL;
}

VOID CHtmlTokenizer::Tokenize(VOID)
{
	int cur_pos = 0;
	int cur_line = 0;
	int start_tag_pos = 0;
	int start_tag_line = 0;
	bool blank = false;

	int buff_pos = 0;

	// TOKEN_TEXT 타입을 만들기 위해 사용되는 버퍼
#define HTML_BUFFER_SIZE 100000
	wchar_t *buffer = NTL_NEW wchar_t [HTML_BUFFER_SIZE];

	enum
	{
		HTML_STRING,
		HTML_TAG
	};

	int state = HTML_STRING;

	while ( !IsEnd(cur_pos) )
	{
		while ( !IsEnd(cur_pos) )
		{
			if (m_pData[cur_pos] == '\n') 
			{
				cur_line++;
				cur_pos++;
			}
			else
			{
				break;
			}
		}

		if(IsEnd(cur_pos))
			break;

		// html tag
		if (IsTagStart(m_pData[cur_pos]))
		{
			if(buff_pos > 0)
				m_Tokens.push_back( CToken(CToken::TOKEN_TEXT, std::wstring(buffer, buff_pos), cur_pos, cur_line) );

			start_tag_pos = cur_pos;
			start_tag_line = cur_line;
			state = HTML_TAG;

			blank = false;  
			buff_pos = 0;
		}
		else if(IsTagEnd(m_pData[cur_pos]))
		{
			m_Tokens.push_back(CToken(CToken::TOKEN_TAG, std::wstring(&m_pData[start_tag_pos], cur_pos-start_tag_pos+1), start_tag_pos, start_tag_line));
			state = HTML_STRING;

			blank = false;  
			buff_pos = 0;
		}
		else
		{
			if(state == HTML_STRING)
			{
				if(IsSpace(m_pData[cur_pos]))
				{
					if(!blank)
						buffer[buff_pos++] = ' ';
					blank = true;  
				}
				else
				{
					buffer[buff_pos++] = m_pData[cur_pos];
					blank = false;  
				}
			}
		}

		cur_pos++;
	}

	if(buff_pos > 0)
		m_Tokens.push_back(CToken(CToken::TOKEN_TEXT, std::wstring(buffer, buff_pos), cur_pos, cur_line));

	NTL_ARRAY_DELETE( buffer );
}

bool CHtmlTokenizer::IsEmpty(void)
{
	if (m_Tokens.size() == 0)
		return true;

	return false;
}

std::wstring CHtmlTokenizer::GetNextToken(void)
{
	if (m_Tokens.size() == 0)
		return L"";

	m_nTokenType = m_Tokens[0].tokentype;
	m_nOffset = m_Tokens[0].offset;
	m_nLine = m_Tokens[0].line;
	std::wstring token = m_Tokens[0].token;

	m_Tokens.pop_front();

	return token;
}

int CHtmlTokenizer::GetTokenType(void)
{
	return m_nTokenType;
}

int CHtmlTokenizer::GetOffset(void)
{
	return m_nOffset;
}

int CHtmlTokenizer::GetLine(void)
{
	return m_nLine;
}


END_GUI

