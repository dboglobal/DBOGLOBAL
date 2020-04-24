#ifndef _NTL_TSSCRSTREAM_H_
#define _NTL_TSSCRSTREAM_H_


class CNtlTSScrFactory;
class CNtlTSScrObject;


/** 
	Script stream
*/


class CNtlTSScrStream
{
// Member variables
protected:
	CNtlTSScrFactory*					m_pFactory;
	std::string							m_strFileName;
	const char*							m_pBuff;
	int									m_nSize;
	std::vector<CNtlTSScrObject*>		m_defObjList;

// Constructions and Destructions
public:
	CNtlTSScrStream( CNtlTSScrFactory* pFactory, const std::string& strFileName, const char* pBuff = 0, int nSize = 0 );
	virtual ~CNtlTSScrStream( void );

// Methods
public:
	std::vector<CNtlTSScrObject*>&		GetObjList( void );

	bool								Load( void );
	bool								Save( std::vector<CNtlTSScrObject*>& defObjList );
};


#endif