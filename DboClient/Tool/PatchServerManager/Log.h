#ifndef _NTL_LOG_H_
#define _NTL_LOG_H_


class CLog
{
// Member variables
protected:
	CString								m_strPath;
	std::wofstream						m_Stream;

// Operations
public:
	void								SetPath( const CString& strPath );

	bool								B( void );
	std::wofstream&						S( void );
	void								E( void );
};


extern CLog g_clLog;


#define LOG_OUT( s )	\
{	\
	std::wcout << s << std::endl;	\
	if ( g_clLog.B() )	\
	{	\
		g_clLog.S() << s << _T("\t\t\t[") << __FILEW__ << _T(", ") << __FUNCTIONW__ << _T(", ") << __LINE__ << _T("]") << std::endl;	\
		g_clLog.E();	\
	}	\
}


#define LOG_ERROR( s )	\
{	\
	{	\
		std::wcout << s << std::endl;	\
		if ( g_clLog.B() )	\
		{	\
			g_clLog.S() << _T("[ERROR] ") << s << _T("\t\t\t[") << __FILEW__ << _T(", ") << __FUNCTIONW__ << _T(", ") << __LINE__ << _T("]") << std::endl;	\
			g_clLog.E();	\
		}	\
	}	\
	_ASSERTE( !#s );	\
}


#endif