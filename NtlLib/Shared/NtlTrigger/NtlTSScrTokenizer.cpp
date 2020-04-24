#include "precomp_trigger.h"
#include "NtlTSScrTokenizer.h"
#include "NtlTSCoreDefine.h"
#include "NtlTSLog.h"


/** 
	Script tokenizer
*/


CNtlTSScrTokenizer::CNtlTSScrTokenizer( const std::string& strFileName, const char* pBuff, int nSize )
: m_nMode( eTOKENIZER_MODE_INVALID),
  m_strFileName( strFileName ),
  m_pFileBuffer( 0 ),
  m_nFileSize( 0 ),
  m_nPeekPos( 0 ),
  m_nLastLine( 0 ),
  m_bComment( false )
{
	if ( !Load( strFileName, pBuff, nSize ) )
	{
		Unload();
	}
	else
	{
		if ( !Tokenize() )
		{
			Unload();
		}
	}
}

CNtlTSScrTokenizer::~CNtlTSScrTokenizer( void )
{
	Unload();
}

bool CNtlTSScrTokenizer::IsSpace( char c )
{
	const char *pSpace = " \t\r\n";

	return ( strchr( pSpace, c ) != NULL );
}

bool CNtlTSScrTokenizer::IsOperator( char c )
{
	const char *pOperators = ",=();{}<+-*/>";

	return ( strchr( pOperators, c ) != NULL );
}

bool CNtlTSScrTokenizer::IsComment( char c, int nPos )
{
	if ( m_bComment )
	{
		if ( '\n' == c ) 
		{	
			m_bComment = false;
			return false;
		}
		return true;
	}
	else
	{
		if ( '/' == c )
		{
			if ( nPos + 1 < m_nFileSize && '/' == m_pFileBuffer[nPos + 1] )
			{
				m_bComment = true;
				return true;
			}
		}
		return false;
	}	
}

std::string CNtlTSScrTokenizer::GetNextToken( int *pnOffset )
{
	m_nPeekPos = 0;
	m_nLastLine = m_defTokens[0].m_nLine;
	std::string strToken = m_defTokens[0].m_strToken;
	if ( 0 != pnOffset ) *pnOffset = m_defTokens[0].m_nOffset;

	m_defTokens.pop_front();

	return strToken;
}

std::string CNtlTSScrTokenizer::PeekNextToken( int *pnOffset )
{
	if ( m_nPeekPos >= (int)m_defTokens.size() ) return "";
	m_nLastLine = m_defTokens[m_nPeekPos].m_nLine;
	if ( 0 != pnOffset ) *pnOffset = m_defTokens[m_nPeekPos].m_nOffset;

	return m_defTokens[m_nPeekPos++].m_strToken;
}

void CNtlTSScrTokenizer::PopToPeek( void )
{
	while ( m_nPeekPos > 0 )
	{
		m_nPeekPos--;
		m_defTokens.pop_front();
	}
}

bool CNtlTSScrTokenizer::Load( const std::string& strFileName, const char* pBuff, int nSize )
{
	if ( pBuff && 0 != nSize )
	{
		m_nMode = eTOKENIZER_MODE_OUT_BUFFER;

		m_nFileSize = nSize;
		m_pFileBuffer = (char*)pBuff;
	}
	else
	{
		m_nMode = eTOKENIZER_MODE_INNER_BUFFER;

		// 해당 파일을 연다
		FILE *pFile;
		if ( 0 != fopen_s( &pFile, strFileName.c_str(), "rb" ) )
		{
			printf( "Can not open the file. Info[%s]. [%s]", strFileName.c_str(), TS_CODE_TRACE() );
			return false;
		}

		// 파일의 크기를 알아낸다
		fseek( pFile, 0, SEEK_END );
		m_nFileSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		// 파일의 크기만큼 메모리를 할당한다
		m_pFileBuffer = (char*)malloc( m_nFileSize*sizeof(char) );
		if ( 0 == m_pFileBuffer )
		{
			printf( "Allocating a memory is failed. Info[%I64u]. [%s]", (int)m_nFileSize*sizeof(char), TS_CODE_TRACE() );
			return false;
		}

		// 할당된 메모리 영역으로 데이터를 저장한다
		fread( m_pFileBuffer, 1, m_nFileSize, pFile );

		fclose( pFile );
	}

	return true;
}

void CNtlTSScrTokenizer::Unload( void )
{
	if ( eTOKENIZER_MODE_INNER_BUFFER == m_nMode )
	{
		if ( m_pFileBuffer )
		{
			free( m_pFileBuffer );
		}
	}
	m_pFileBuffer = 0;
	m_nFileSize = 0;
	m_nPeekPos = 0;
	m_nLastLine = 0;
	m_bComment = false;

	m_defTokens.clear();
}

bool CNtlTSScrTokenizer::Tokenize( void )
{
	int nCurPos = 0;
	int nCurLine = 0;
	while ( nCurPos < m_nFileSize )
	{
		// Comment, Space는 넘어간다
		while ( nCurPos < m_nFileSize && ( IsComment( m_pFileBuffer[nCurPos], nCurPos ) || IsSpace( m_pFileBuffer[nCurPos] ) ) )
		{
			if ( '\n' == m_pFileBuffer[nCurPos] ) nCurLine++;
			nCurPos++;
		}

		// nCurPos 가 파일 사이즈 하고 같다면 while을 빠져 나간다
		if ( nCurPos == m_nFileSize ) break;

		// Operator 저장
		if ( IsOperator( m_pFileBuffer[nCurPos] ) )
		{
			// Operator라면 현재 위치와 라인, 그리고 해당 Operator를 Token으로 저장한다
			m_defTokens.push_back( CToken( std::string( &m_pFileBuffer[nCurPos], 1 ), nCurPos, nCurLine ) );
			nCurPos++;
		}
		else
		{
			int nTempPos = nCurPos;

			// 문자열 처리
			if ( '"' == m_pFileBuffer[nTempPos] )
			{
				int nNumChars = 0;
				++nTempPos;

				while ( nTempPos < m_nFileSize )
				{
					if ( '"' == m_pFileBuffer[nTempPos] )
					{
						if ( (nTempPos+1 >= m_nFileSize) || ( '"' != m_pFileBuffer[nTempPos+1] ) )
						{
							break;
						}
						else
						{
							++nTempPos;
						}
					}
					++nTempPos;
					++nNumChars;
				}

				if ( nTempPos == m_nFileSize )
				{
					printf( "Parsing the string is failed. Info[%d]. [%s]", nTempPos, TS_CODE_TRACE() );
					return false;
				}

				char *pTemp = NULL;
				pTemp = (char*)malloc( (nNumChars+1)*sizeof(char) );
				if ( 0 == pTemp )
				{
					printf( "Allocating a memory is failed. Info[%I64u]. [%s]", (int)(nNumChars+1)*sizeof(char), TS_CODE_TRACE() );
					return false;
				}
				pTemp[nNumChars] = 0;

				int nTempOffset = 1;
				for( int i = 0; i < nNumChars; ++i )
				{
					pTemp[i] = m_pFileBuffer[nCurPos+nTempOffset+i];
					if ( '"' == m_pFileBuffer[nCurPos+nTempOffset+i] ) 
					{
						nTempOffset++;
					}
				}
				nCurPos = nTempPos + 1;
				m_defTokens.push_back( CToken(std::string(pTemp), nCurPos, nCurLine) );

				free( pTemp );
			}
			else
			{
				while ( nTempPos < m_nFileSize && 
						!IsSpace( m_pFileBuffer[nTempPos] ) && 
						!IsOperator( m_pFileBuffer[nTempPos] ) && 
						!IsComment( m_pFileBuffer[nTempPos], nCurPos ) )
				{
					++nTempPos;
				}

				int nTokenSize = nTempPos - nCurPos;
				m_defTokens.push_back( CToken(std::string(&m_pFileBuffer[nCurPos], nTokenSize), nCurPos, nCurLine) );
				nCurPos += nTokenSize;
			}
		}
	}

	return true;
}
