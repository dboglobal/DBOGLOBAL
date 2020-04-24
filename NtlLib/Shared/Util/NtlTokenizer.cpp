#include "StdAfx.h"
#include "NtlTokenizer.h"

CNtlTokenizer::CNtlTokenizer(const std::string &strFileName, CallTokenPack fnCallPack /* = NULL */)
{
	m_pData		 = NULL;

	m_bSuccess = Load(strFileName.data(), fnCallPack);
	if(!m_bSuccess)
		return;

	m_strFileName = strFileName;
	m_iPeekPos = 0;
	m_iLastLine = 0;
	m_bInRemark = FALSE;

	Tokenize();
}

CNtlTokenizer::CNtlTokenizer(const char *pBuffer)
{
	m_pData		 = NULL;
	m_bSuccess	 = TRUE;

	m_iTotalSize = (int)strlen(pBuffer);
	m_pData = new char[m_iTotalSize+1];
	m_pData[m_iTotalSize] = '\0';
	memcpy(m_pData, pBuffer, m_iTotalSize);
		
	m_iPeekPos = 0;
	m_iLastLine = 0;
	m_bInRemark = FALSE;

	Tokenize();
}

CNtlTokenizer::~CNtlTokenizer()
{
	if(m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}
}

BOOL CNtlTokenizer::Load(const char *pFileName, CallTokenPack fnCallPack)
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	if( fnCallPack )
	{
		(*fnCallPack)( pFileName, (void**)&m_pData, &m_iTotalSize );
	}
	else
	{
		FILE *fp = NULL;
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		if(fopen_s(&fp, pFileName, "rb") != 0)
			return FALSE;
#else
		fp = fopen(pFileName,"rb");
		if(fp == NULL)
			return FALSE;
#endif

		fseek(fp, 0, SEEK_END);
		int iSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		m_iTotalSize = iSize;
		m_pData = new char[iSize+1];
		m_pData[iSize] = '\0';

		fread(m_pData,1,iSize,fp);
		fclose(fp);
	}

	return TRUE;
}

BOOL CNtlTokenizer::IsSuccess(void)
{
	return m_bSuccess;
}

BOOL CNtlTokenizer::IsSpace(char c)
{
	const char *pSpace = " \t\r\n";

	return strchr(pSpace, c) != NULL;
}


BOOL CNtlTokenizer::IsOperator(char c)
{
	const char *pOperators = ",=();{}<+-*/>";

	return strchr(pOperators, c) != NULL;
}


BOOL CNtlTokenizer::IsRemark(char c, int iPosition)
{
	if (m_bInRemark)
	{
		if (c == '\n') 
		{	
			m_bInRemark = FALSE;
		}
		return TRUE;
	}
	else
	{
		if (c == '/')
		{
			if (
				iPosition+1 < m_iTotalSize &&
				m_pData[iPosition+1] == '/')
			{
				m_bInRemark = TRUE;
				return TRUE;
			}
		}
		/*
		else if(c == '-')
		{
			if (
				iPosition+1 < m_iTotalSize &&
				m_pData[iPosition+1] == '-')
			{
				m_bInRemark = TRUE;
				return TRUE;
			}
		}
		*/
		else if (c == '#')
		{
			m_bInRemark = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}




void CNtlTokenizer::Tokenize(void)
{
	int iCurPos = 0;
	int iCurLine = 0;
	while (iCurPos < m_iTotalSize)
	{
		while (
			iCurPos < m_iTotalSize && 
			(
				IsRemark(m_pData[iCurPos], iCurPos) ||
				IsSpace(m_pData[iCurPos])
			))
		{
			if (m_pData[iCurPos] == '\n') iCurLine++;
			iCurPos++;
		}
		if (iCurPos == m_iTotalSize) break;

		if (IsOperator(m_pData[iCurPos]))
		{
			m_dqTokens.push_back(CNtlToken(std::string(&m_pData[iCurPos], 1), iCurPos, iCurLine));
			iCurPos++;
		}
		else
		{
			int iTempPos = iCurPos;
			if (m_pData[iTempPos] == '"')
			{
				int iNumChars = 0;
				iTempPos++;
				while (iTempPos < m_iTotalSize)
				{
					if (m_pData[iTempPos] == '"')
					{
						if (iTempPos+1>=m_iTotalSize || m_pData[iTempPos+1] != '"') break;
						else
						{
							iTempPos++;
						}
					}
					iTempPos++;
					iNumChars++;
				}
				if (iTempPos == m_iTotalSize)
				{
					WriteError("Missing '""' following '""'-begin");
					break;
				}

				_ASSERTE(iNumChars < NTL_TOKEN_BUFF_LEN);
//				char *temp = new char[iNumChars+1];
				m_pTemp[iNumChars] = 0;
				int ofs = 1;
				for (int i=0;i<iNumChars;i++)
				{
					m_pTemp[i] = m_pData[iCurPos+ofs+i];
					if (m_pData[iCurPos+ofs+i] == '"') 
					{
						ofs++;
					}
				}
				iCurPos = iTempPos+1;

				m_dqTokens.push_back(CNtlToken(std::string(m_pTemp), iCurPos, iCurLine));
//				delete temp;
			}
			else
			{
				while (iTempPos < m_iTotalSize &&
					!IsSpace(m_pData[iTempPos]) &&
					!IsOperator(m_pData[iTempPos]) &&
					!IsRemark(m_pData[iTempPos], iCurPos))
				{
					iTempPos++;
				}

				int iTokSize = iTempPos-iCurPos;
				m_dqTokens.push_back(CNtlToken(std::string(&m_pData[iCurPos], iTokSize), iCurPos, iCurLine));
				iCurPos += iTokSize;
			}
		}
	}
}

std::string CNtlTokenizer::PeekNextToken(int *pOffset/*=NULL*/, int *pLine /*= 0*/)
{
	if(m_iPeekPos >= (int) m_dqTokens.size())
	{
		return "";
	}
	if(pOffset != NULL) 
		*pOffset = m_dqTokens[m_iPeekPos].iOffset;
	if(pLine != NULL)
		*pLine = m_dqTokens[m_iPeekPos].iLine;

	return m_dqTokens[m_iPeekPos++].strToken;
}


std::string CNtlTokenizer::GetNextToken(int *pOffset/*=NULL*/, int *pLine /*= 0*/)
{
	m_iPeekPos = 0;
	m_iLastLine = m_dqTokens[0].iLine;
	std::string token = m_dqTokens[0].strToken;
	if(pOffset != NULL)
		*pOffset = m_dqTokens[0].iOffset;

	if(pLine != NULL)
		*pLine = m_dqTokens[0].iLine;

	m_dqTokens.pop_front();

	return token;
}

void CNtlTokenizer::PopToPeek(void)
{
	while (m_iPeekPos > 0)
	{
		m_iPeekPos--;
		m_dqTokens.pop_front();
	}
}


std::string CNtlTokenizer::WriteError(std::string strErrMsg)
{
	std::string str = "core tokenizer Tokenize() error";
	return str;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicode Tokenizer
//////////////

CNtlTokenizerW::CNtlTokenizerW(const std::string &strFileName, CallTokenPack fnCallPack /* = NULL */)
{
	m_pData		 = NULL;

	m_bSuccess = Load(strFileName.data(), fnCallPack);
	if(!m_bSuccess)
		return;

	m_strFileName = strFileName;
	m_iPeekPos = 0;
	m_iLastLine = 0;
	m_bInRemark = FALSE;

	Tokenize();
}

CNtlTokenizerW::CNtlTokenizerW(const wchar_t *pBuffer)
{
	m_pData		 = NULL;
	m_bSuccess	 = TRUE;

	m_iTotalSize = (int)wcslen(pBuffer);
	m_pData = new wchar_t[m_iTotalSize+1];
	m_pData[m_iTotalSize] = '\0';
	memcpy(m_pData, pBuffer, m_iTotalSize*2);

	m_iPeekPos = 0;
	m_iLastLine = 0;	
	m_bInRemark = FALSE;

	Tokenize();
}

CNtlTokenizerW::~CNtlTokenizerW()
{
	if(m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}
}

BOOL CNtlTokenizerW::Load(const char *pFileName, CallTokenPack fnCallPack)
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}

	// Kell's Comment : UTF-16/UCS2 만 검사한다. 이외의 방식은 MultiByte로 읽는다.
	// UTF-16의 Byte-order Mark는 2byte 인 FF FE 를 사용하고 이 방식은 윈도우의
	// 메모장이나 Word에서 사용하는 방식이다. (주의)

	const BYTE byBomOffset = 2;
	const BYTE abyUnicodeBom[2] = { 0xFF, 0xFE };
	
	if( fnCallPack )
	{
		CHAR* pData = NULL;
		INT iSize = 0;

		(*fnCallPack)( pFileName, (VOID**)&pData, &iSize );

		if( abyUnicodeBom[0] == (BYTE)pData[0] &&
			abyUnicodeBom[1] == (BYTE)pData[1] )
		{// UTF-16(Little-Endian)
			WCHAR* pwData = (WCHAR*)pData;
			pwData++;
			m_iTotalSize = ( iSize - 2 ) / 2;
			m_pData = new WCHAR[m_iTotalSize+1];
			m_pData[m_iTotalSize] = '\0';

			memcpy( m_pData, pwData, m_iTotalSize * 2 );			
		}
		else
		{// Ansi
			m_iTotalSize = iSize;
			m_pData = new WCHAR[m_iTotalSize+1];
			m_pData[m_iTotalSize] = '\0';

			::MultiByteToWideChar( GetACP(), 0, pData, -1, m_pData, m_iTotalSize + 1 );
		}

		delete [] pData;
	}
	else
	{
		FILE *fp = NULL;
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		if(fopen_s(&fp, pFileName, "rb") != 0)
			return FALSE;
#else
		fp = fopen(pFileName,"rb");
		if(fp == NULL)
			return FALSE;
#endif
		// 앞의 2 Byte만 읽어온다.
		BYTE abyFileBom[2] = { 0, };
		fread( abyFileBom, 1, 2, fp );

		// 현재는 MultiByte와 Unicode, 2개 다 읽어들어야 한다.

		// UTF-16(Little-Endian)
		if( abyUnicodeBom[0] == abyFileBom[0] &&
			abyUnicodeBom[1] == abyFileBom[1] )
		{
			fseek(fp, byBomOffset, SEEK_END);
			int nSize = ftell(fp);

			// Cut BOM ( Byte Order Mark ) 만큼 짤라준다.
			fseek(fp, 2, SEEK_SET);

			// Unicode는 2byte당 1character를 사용한다.
			int nStrLen = (nSize/2);
			m_iTotalSize = nSize/2;
			m_pData = new wchar_t[nStrLen];
			m_pData[nStrLen-1] = L'\0';

			fread(m_pData, nSize , 1, fp);
		}
		// Ansi
		else 
		{
			fseek(fp, 0, SEEK_END);
			int nSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char* pData = new char[nSize+1];
			pData[nSize] = 0;

			fread(pData, nSize, 1, fp);

			m_pData = new wchar_t[nSize];
			m_iTotalSize = nSize;

			// Wide Character로 변환
			::MultiByteToWideChar( GetACP(), 0,pData, -1, m_pData, nSize );

			delete [] pData;
			pData = NULL;
		}

		fclose(fp);
	}

	return TRUE;
}

BOOL CNtlTokenizerW::IsSuccess(void)
{
	return m_bSuccess;
}

BOOL CNtlTokenizerW::IsSpace(wchar_t c)
{
	const wchar_t *pSpace = L" \t\r\n";

	return wcschr(pSpace, c) != NULL;
}


BOOL CNtlTokenizerW::IsOperator(wchar_t c)
{
	const wchar_t *pOperators = L",=();{}<+-*/>";

	return wcschr(pOperators, c) != NULL;
}


BOOL CNtlTokenizerW::IsRemark(wchar_t c, int iPosition)
{
	if (m_bInRemark)
	{
		if (c == L'\n') 
		{	
			m_bInRemark = FALSE;
		}
		return TRUE;
	}
	else
	{
		if (c == L'/')
		{
			if (
				iPosition+1 < m_iTotalSize &&
				m_pData[iPosition+1] == L'/')
			{
				m_bInRemark = TRUE;
				return TRUE;
			}
		}
		/*
		else if(c == L'-')
		{
		if (
		iPosition+1 < m_iTotalSize &&
		m_pData[iPosition+1] == L'-')
		{
		m_bInRemark = TRUE;
		return TRUE;
		}
		}
		*/
		else if (c == L'#')
		{
			m_bInRemark = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}




void CNtlTokenizerW::Tokenize(void)
{
	int iCurPos = 0;
	int iCurLine = 0;
	while (iCurPos < m_iTotalSize)
	{
		while (
			iCurPos < m_iTotalSize && 
			(
			IsRemark(m_pData[iCurPos], iCurPos) ||
			IsSpace(m_pData[iCurPos])
			))
		{
			if (m_pData[iCurPos] == L'\n') iCurLine++;
			iCurPos++;
		}
		if (iCurPos == m_iTotalSize) break;

		if (IsOperator(m_pData[iCurPos]))
		{
			m_dqTokens.push_back(CNtlTokenW(std::wstring(&m_pData[iCurPos], 1), iCurPos, iCurLine));
			iCurPos++;
		}
		else
		{
			int iTempPos = iCurPos;
			if (m_pData[iTempPos] == L'"')
			{
				int iNumChars = 0;
				iTempPos++;
				while (iTempPos < m_iTotalSize)
				{
					if (m_pData[iTempPos] == L'"')
					{
						if (iTempPos+1>=m_iTotalSize || m_pData[iTempPos+1] != L'"') break;
						else
						{
							iTempPos++;
						}
					}
					iTempPos++;
					iNumChars++;
				}
				if (iTempPos == m_iTotalSize)
				{
					WriteError("Missing '""' following '""'-begin");
					break;
				}

				_ASSERTE(iNumChars < NTL_TOKEN_BUFF_LEN);
				//				char *temp = new char[iNumChars+1];
				m_pTemp[iNumChars] = 0;
				int ofs = 1;
				for (int i=0;i<iNumChars;i++)
				{
					m_pTemp[i] = m_pData[iCurPos+ofs+i];
					if (m_pData[iCurPos+ofs+i] == '"') 
					{
						ofs++;
					}
				}
				iCurPos = iTempPos+1;

				m_dqTokens.push_back(CNtlTokenW(std::wstring(m_pTemp), iCurPos, iCurLine));
				//				delete temp;
			}
			else
			{
				while (iTempPos < m_iTotalSize &&
					!IsSpace(m_pData[iTempPos]) &&
					!IsOperator(m_pData[iTempPos]) &&
					!IsRemark(m_pData[iTempPos], iCurPos))
				{
					iTempPos++;
				}

				int iTokSize = iTempPos-iCurPos;
				m_dqTokens.push_back(CNtlTokenW(std::wstring(&m_pData[iCurPos], iTokSize), iCurPos, iCurLine));
				iCurPos += iTokSize;
			}
		}
	}
}

std::wstring CNtlTokenizerW::PeekNextToken(int *pOffset/*=NULL*/, int *pLine /*= 0*/)
{
	if(m_iPeekPos >= (int) m_dqTokens.size())
	{
		return L"";
	}
	if(pOffset != NULL) 
		*pOffset = m_dqTokens[m_iPeekPos].iOffset;
	if(pLine != NULL)
		*pLine = m_dqTokens[m_iPeekPos].iLine;

	return m_dqTokens[m_iPeekPos++].wstrToken;
}


std::wstring CNtlTokenizerW::GetNextToken(int *pOffset/*=NULL*/, int *pLine /*= 0*/)
{
	m_iPeekPos = 0;
	m_iLastLine = m_dqTokens[0].iLine;
	std::wstring token = m_dqTokens[0].wstrToken;
	if(pOffset != NULL)
		*pOffset = m_dqTokens[0].iOffset;

	if(pLine != NULL)
		*pLine = m_dqTokens[0].iLine;

	m_dqTokens.pop_front();

	return token;
}

void CNtlTokenizerW::PopToPeek(void)
{
	while (m_iPeekPos > 0)
	{
		m_iPeekPos--;
		m_dqTokens.pop_front();
	}
}


std::string CNtlTokenizerW::WriteError(std::string strErrMsg)
{
	std::string str = "core tokenizer Tokenize() error";
	return str;
}