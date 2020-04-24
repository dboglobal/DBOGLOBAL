#ifndef _NTL_NAVI_PATHENGINE_H_
#define _NTL_NAVI_PATHENGINE_H_


//////////////////////////////////////////////////////////////////////////
//
//	CPathDataOutStream
//
//////////////////////////////////////////////////////////////////////////


class CPathDataOutStream : public iOutputStream
{
// Member variables
protected:
	FILE*						m_pFile;


// Constructions and Destructions
public:
	CPathDataOutStream( const char* pFileName );
	virtual ~CPathDataOutStream( void );


// Operations
public:
	virtual void put( const char* data, tUnsigned32 dataSize );
};


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPathEngine
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPathEngine : public iErrorHandler
{
// Instance
public:
	static CNtlNaviPathEngine*	GetInstance( void );


// Declarations
public:
	typedef iPathEngine* (__stdcall* tGetInterfaceFunction) ( iErrorHandler* );


// Member variables
protected:
	HINSTANCE					m_hInstance;
	iPathEngine*				m_pPathEngine;


// Constructions and Destructions
public:
	CNtlNaviPathEngine( void );
	~CNtlNaviPathEngine( void );


// Operations
public:
	iPathEngine*				GetPathEngine( void );

	bool						Create( const char* pPathDllName );
	void						Delete( void );

	virtual eAction				handle( const char* type, const char* description, const char *const* attributes );
};


#endif