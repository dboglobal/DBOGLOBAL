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

     
/*----------------------------------------------------------------------------------/
 Include 
/----------------------------------------------------------------------------------*/
#include "NtlSimpleTokenizer.h"

/*----------------------------------------------------------------------------------/
 ::lexMode
/----------------------------------------------------------------------------------*/
enum	lexMode 
{
	lm_quote, lm_read, lm_skip,
};

/*----------------------------------------------------------------------------------/
 CTokenizer Implementation
/----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------/
 function:
 
 
/----------------------------------------------------------------------------------*/
CNtlSimpleTokenizer :: CNtlSimpleTokenizer() 
{
	m_cDelim	= ' ';
}


/*----------------------------------------------------------------------------------/
 function:
 
 
/----------------------------------------------------------------------------------*/
void CNtlSimpleTokenizer::SetString(const string& a)
{
    int     len = (int)a.length();
    int     curIndex = 0;
    int     curOffset = 0;
	lexMode	curMode = lm_read;
    char    buffer[1024];

	m_token.erase(m_token.begin(), m_token.end());

    for (int i=0; i<len; i++)
	{
		switch (curMode)
		{
		case lm_quote :
			if (a[i] == '"') 
			{
				buffer[curOffset] = 0;
				m_token.push_back(string(buffer));
				curMode = lm_skip;
				curOffset = 0;
			}
			else
			{
				buffer[curOffset++] = a[i];
			}
			break;
		case lm_skip :
			if (!(isDelim(a[i])&&a[i]>=0)) 
			{
				curMode = lm_read;
				i--;
			}
			break;
		case lm_read :
			if (a[i] == '/')
				if (a[i+1] == '/')
					return;
			if ((isDelim(a[i])&&a[i]>=0))
			{
				if (i!=0)
				{
					buffer[curOffset] = 0;
					m_token.push_back(string(buffer));
	                curOffset = 0;
		        }
				curMode = lm_skip;
				break;
			}
			if (a[i] == '"')
			{
				curOffset = 0;
				curMode = lm_quote;
				break;
			}
			buffer[curOffset++] = a[i];
			break;
        }
    }

    if (curMode != lm_skip && i) 
	{
        buffer[curOffset] = 0;
        m_token.push_back(string(buffer));
    }
}

/*----------------------------------------------------------------------------------/
 function:
 
 
/----------------------------------------------------------------------------------*/
void CNtlSimpleTokenizer::MakeLower()
{
	//	for every string
	for (int i=0; i<(int)m_token.size(); i++)
	{
		string &	s = m_token[i];
		//	for every character
		for (int j=0; j<(int)s.size(); j++)
		{
			if (s[j]>0)
			{
				if (isupper(s[j]))
					s[j] += ('a'-'A');
			} 
			// 2-byte char, skip one more char
			else
				j++;
		}
	}
}

/*----------------------------------------------------------------------------------/
 function:
 
 
/----------------------------------------------------------------------------------*/
void CNtlSimpleTokenizer::MakeUpper()
{
	//	for every string
	for (int i=0; i<(int)m_token.size(); i++)
	{
		string &	s = m_token[i];
		//	for every character
		for (int j=0; j<(int)s.size(); j++)
		{
			if (s[j]>0)
			{
				if (islower(s[j]))
					s[j] -= ('a'-'A');
			}
			// 2-byte char, skip one more char
			else
				j++;
		}
	}
}

/*----------------------------------------------------------------------------------/
 function:
 
 
/----------------------------------------------------------------------------------*/
void CNtlSimpleTokenizer::GetLine(istrstream &stream)
{
	char	buf[1024];
	stream.getline(buf, 1024);
    SetString(string(buf));
}

/*----------------------------------------------------------------------------------/
 function:
 
 
/----------------------------------------------------------------------------------*/
bool CNtlSimpleTokenizer::isDelim(char a)
{
	bool	ret = false;
	if(a == m_cDelim || isspace(a))
		ret = true;

	return ret;
}
