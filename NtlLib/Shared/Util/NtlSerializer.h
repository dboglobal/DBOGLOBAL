/*****************************************************************************
*
* File			: NtlSerializer.h
* Author		: HyungSuk, Jang
* Copyright	: (аж)NTL
* Date			: 2006. 12. 09	
* Abstract		: system event definition.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_SERIALIZER__
#define __NTL_SERIALIZER__

#include <string>

class CNtlSerializer
{
protected:

	char			*m_pBuffer;			/** stream buffer */
	int				m_iCurrSize;		/** current buffer size */
	int				m_iStartPointer;	/** data start pointer */
	int				m_iEndPointer;		/** data end pointer */
	int				m_iGlowSize;		/** data glow size */

protected:

	void Allocate(void);
	void Dellocate(void);	

public:

	CNtlSerializer();
	CNtlSerializer(int iBufferSize, int iGlowSize);
	~CNtlSerializer();

	CNtlSerializer& operator = (const CNtlSerializer& s);

	//! operation

	void			Refresh(void);    

	CNtlSerializer&	In(const void *pData, int iSize);
	CNtlSerializer& Format(const char *pData /*= NULL*/, ...);
	CNtlSerializer&	Out(void *pData, int uiSize);
	CNtlSerializer&	Out(CNtlSerializer& s, int uiSize);

	void			IncrementEndPointer(int nSize);

	CNtlSerializer& operator << (char chData);
	CNtlSerializer& operator << (unsigned char byData);
	CNtlSerializer& operator << (short shData);
	CNtlSerializer& operator << (unsigned short wData);
	CNtlSerializer& operator << (int iData);
	CNtlSerializer& operator << (unsigned int uiData);
	CNtlSerializer& operator << (long lData);
    CNtlSerializer& operator << (unsigned long ulData);
	CNtlSerializer& operator << (__int64 i64Data);
	CNtlSerializer& operator << (unsigned __int64 ui64Data);
	CNtlSerializer& operator << (float fData);
	CNtlSerializer& operator << (double dData);
	CNtlSerializer& operator << (char *pData);
	CNtlSerializer& operator << (const char *pData);
	CNtlSerializer& operator << (std::string& str);

	CNtlSerializer& operator >> (char& chData);
	CNtlSerializer& operator >> (unsigned char& byData);
	CNtlSerializer& operator >> (short& shData);
	CNtlSerializer& operator >> (unsigned short& wData);
	CNtlSerializer& operator >> (int& iData);
	CNtlSerializer& operator >> (unsigned int& uiData);
	CNtlSerializer& operator >> (long& lData);
    CNtlSerializer& operator >> (unsigned long& ulData);
	CNtlSerializer& operator >> (__int64& i64Data);
	CNtlSerializer& operator >> (unsigned __int64& ui64Data);
	CNtlSerializer& operator >> (float& fData);
	CNtlSerializer& operator >> (double& dData);
	CNtlSerializer& operator >> (std::string& str);	
	CNtlSerializer& operator >> (bool& bData);

	//! attribute

	bool			IsEmpty(void);

	int				GetBufferSize(void) const;
	int				GetGlowSize(void) const;
	int				GetDataSize(void);
	int				GetStartPointer(void) const;
	int				GetEndPointer(void) const;
	const char*		GetData(void) const;

    bool CheckInBuffer(int iSize);
    bool CheckOutBuffer(int iSize);
};

inline int CNtlSerializer::GetBufferSize(void) const
{
	return m_iCurrSize; 
}

inline int CNtlSerializer::GetGlowSize(void) const
{
	return m_iGlowSize;
}

inline int CNtlSerializer::GetStartPointer(void) const
{
	return m_iStartPointer;
}

inline int CNtlSerializer::GetEndPointer(void) const
{
	return m_iEndPointer;
}

inline const char* CNtlSerializer::GetData(void) const 
{
	return m_pBuffer; 
}

#endif
