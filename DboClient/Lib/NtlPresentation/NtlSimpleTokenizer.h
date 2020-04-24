/*****************************************************************************
 *
 * File			: NTLApplication
 * Author		: HongHoDong
 * Copyright	: (аж)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: NTl Application
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_SIMPLE_TOKENIZER__
#define __NTL_SIMPLE_TOKENIZER__

#include	<string>
#include	<strstream>
#include	<vector>
#include	<iostream>
#include	<fstream>

using namespace	std;
/*----------------------------------------------------------------------------------/
 ::CTokenizer
/----------------------------------------------------------------------------------*/
class   CNtlSimpleTokenizer
{
public:
    CNtlSimpleTokenizer();
    CNtlSimpleTokenizer(const string &a);
    void    SetString(const string &a);
	void	MakeLower();
	void	MakeUpper();
    void    operator << (const string &a);
    string &operator [] (int a);
    int     NumTokens();
	int		GetInt(int a) { return	atoi(m_token[a].c_str()); }
	float	GetFloat(int a) { return (float)atof(m_token[a].c_str()); }
	void	GetLine(istrstream &str);
	void	SetDelim(char a);

private:
    vector<string>  m_token;
protected:
	bool isDelim(char a);
	char m_cDelim;
};

#include "NtlSimpleTokenizer.inl"

#endif//__UTIL_H__
