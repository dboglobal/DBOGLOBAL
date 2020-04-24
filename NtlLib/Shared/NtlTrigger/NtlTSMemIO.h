#ifndef _NTL_TS_MEM_IO_H_
#define _NTL_TS_MEM_IO_H_


class CNtlTSMemInput
{
// Member variables
protected:
	const char*			m_pBuf;
	int					m_nBufSize;

	int					m_nCurPos;


// Constructions and Destructions
public:
	CNtlTSMemInput( const char* pBuf, int nBufSize );
	virtual ~CNtlTSMemInput( void );


// Operations
public:
	bool				Read( void* pData, int nSize );
};


#endif