inline	CNtlSimpleTokenizer :: CNtlSimpleTokenizer(const string &a) 
{
	SetString(a);
}

inline	void    CNtlSimpleTokenizer :: operator << (const string &a)
{
	SetString(a);
}

inline	string &CNtlSimpleTokenizer :: operator [](int a)
{
	if (a >= (int)m_token.size()) 
	{
		cout << "Error in CTokenizer :: operator []";
		for (int i=0; i<(int)m_token.size(); i++)
			cerr << m_token[i].c_str();
	}
	return	m_token[a];
}

inline	int     CNtlSimpleTokenizer :: NumTokens()
{
	return	(int)m_token.size();
}

inline void	CNtlSimpleTokenizer::SetDelim(char a)
{
	m_cDelim = a;
}

inline	int		random(int range)
{
	if(range==0) 
		return 0;
	return	rand()%range;
}