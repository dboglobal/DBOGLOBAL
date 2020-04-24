#ifndef _NTL_TSSCRTOKENIZER_H_
#define _NTL_TSSCRTOKENIZER_H_


/** 
	Script tokenizer
*/


class CNtlTSScrTokenizer
{
// Declaration
protected:
	enum { eTOKENIZER_MODE_INVALID, eTOKENIZER_MODE_OUT_BUFFER, eTOKENIZER_MODE_INNER_BUFFER };

	class CToken
	{
	public:
		CToken( const std::string& strToken, int nOffset, int nLine ) 
			: m_strToken( strToken ), m_nOffset ( nOffset ), m_nLine( nLine ) { return; }
	public:
		std::string						m_strToken;
		int								m_nOffset;
		int								m_nLine;		
	};

	typedef std::deque<CToken> deqdef_TokenList;

// Member variables
protected:
	int									m_nMode;

	deqdef_TokenList					m_defTokens;

	std::string							m_strFileName;

	char*								m_pFileBuffer;
	int									m_nFileSize;	

	int									m_nPeekPos;
	int									m_nLastLine;

	bool								m_bComment;

// Constructions and Destructions
public:
	CNtlTSScrTokenizer( const std::string& strFileName, const char* pBuff, int nSize );
	~CNtlTSScrTokenizer( void );	

// Methods
public:
	// ' ' '\t' '\r' '\n' 를 공백으로 간주한다
	bool								IsSpace( const char c );

	// ',' '=' '(' ')' ';' '{' '}' '<' '+' '-' '*' '/' '>' 를 연산자로 간주한다
	bool								IsOperator( const char c );

	// '//' 를 주석으로 간주한다
	bool								IsComment( const char c, int pos );
	
	// Token 을 제거하면서 가져온다
	std::string							GetNextToken( int* pnOffSet = 0 );

	// Token 을 제거하지 않고 가져온다
	// 추후 PopToPeek 함수가 호출되었을 때 PeekNextToken 을 사용해서 가져온 토큰들이 일괄적으로 제거 된다
	std::string							PeekNextToken( int* pnOffSet = 0 );
	void								PopToPeek( void );

// Implementations
protected:
	bool								Load( const std::string& strFileName, const char* pBuff, int nSize );
	void								Unload( void );
	bool								Tokenize( void );
};

#endif