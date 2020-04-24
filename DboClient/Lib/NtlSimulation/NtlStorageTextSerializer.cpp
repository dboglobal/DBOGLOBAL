#include "precomp_ntlsimulation.h"
#include "NtlStorageTextSerializer.h"

CNtlStorageTextSerializer::CNtlStorageTextSerializer()
{
	SetStorageSerializerType( eNTL_STORAGE_SERIALIZER_TEXT );
}

CNtlStorageTextSerializer::CNtlStorageTextSerializer( int nBufferSize, int nGlowSize )
: CNtlStorageSerializer( nBufferSize, nGlowSize )
{
	SetStorageSerializerType( eNTL_STORAGE_SERIALIZER_TEXT );
}

CNtlStorageTextSerializer::~CNtlStorageTextSerializer()
{

}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(char chData)
{
	int iTypeSize = sizeof(char);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((char*)&m_pBuffer[m_iEndPointer]) = chData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(unsigned char byData)
{
	int iTypeSize = sizeof(unsigned char);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((unsigned char*)&m_pBuffer[m_iEndPointer]) = byData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(short shData)
{
	int iTypeSize = sizeof(short);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((short*)&m_pBuffer[m_iEndPointer]) = shData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(unsigned short wData)
{
	int iTypeSize = sizeof(unsigned short);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((unsigned short*)&m_pBuffer[m_iEndPointer]) = wData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(int iData)
{
	int iTypeSize = sizeof(int);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((int*)&m_pBuffer[m_iEndPointer]) = iData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(unsigned int uiData)
{
	int iTypeSize = sizeof(unsigned int);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((unsigned int*)&m_pBuffer[m_iEndPointer]) = uiData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(long lData)
{
	int iTypeSize = sizeof(long);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((long*)&m_pBuffer[m_iEndPointer]) = lData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<( unsigned long ulData ) 
{
	int iTypeSize = sizeof(unsigned long);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((unsigned long*)&m_pBuffer[m_iEndPointer]) = ulData;
	m_iEndPointer += iTypeSize;

	return *this;    
}


CNtlSerializer& CNtlStorageTextSerializer::operator<<(__int64 i64Data)
{
	int iTypeSize = sizeof(__int64);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((__int64*)&m_pBuffer[m_iEndPointer]) = i64Data;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(unsigned __int64 ui64Data)
{
	int iTypeSize = sizeof(unsigned __int64);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((unsigned __int64*)&m_pBuffer[m_iEndPointer]) = ui64Data;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(float fData)
{
	int iTypeSize = sizeof(float);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((float*)&m_pBuffer[m_iEndPointer]) = fData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(double dData)
{
	int iTypeSize = sizeof(double);

	if(!CheckInBuffer(iTypeSize))
		return *this;

	*((double*)&m_pBuffer[m_iEndPointer]) = dData;
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator<<(char *pData)
{
	int iTypeSize = (int)strlen(pData);
	if(iTypeSize == 0)
		return *this;

	if(!CheckInBuffer(iTypeSize))
		return *this;

	memcpy(&m_pBuffer[m_iEndPointer], pData, iTypeSize);
	m_iEndPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator << (const char *pData)
{
	int iTypeSize = (int)strlen(pData);
	if(iTypeSize == 0)
		return *this;

	if(!CheckInBuffer(iTypeSize))
		return *this;

	memcpy(&m_pBuffer[m_iEndPointer], pData, iTypeSize);
	m_iEndPointer += iTypeSize;

	return *this;
}


CNtlSerializer& CNtlStorageTextSerializer::operator<<(std::string	&str)
{
	int iTypeSize = (int)str.size();
	if(iTypeSize != 0)
	{
		if(!CheckInBuffer(iTypeSize))
			return *this;

		memcpy(&m_pBuffer[m_iEndPointer], str.c_str(), iTypeSize);
		m_iEndPointer += iTypeSize;
	}

	// 끝에 널문자를 넣어준다.
	/*operator<<('\0');*/

	return *this;
}


CNtlSerializer& CNtlStorageTextSerializer::operator >> (char& chData)
{
	int iTypeSize = sizeof(char);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	chData = *((char*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (unsigned char& byData)
{
	int iTypeSize = sizeof(unsigned char);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	byData = *((unsigned char*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;

}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (short& shData)
{
	int iTypeSize = sizeof(short);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	shData = *((short*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (unsigned short& wData)
{
	int iTypeSize = sizeof(unsigned short);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	wData = *((unsigned short*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (int& iData)
{
	int iTypeSize = sizeof(int);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	iData = *((int*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (unsigned int& uiData)
{
	int iTypeSize = sizeof(unsigned int);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	uiData = *((unsigned int*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (long& lData)
{
	int iTypeSize = sizeof(long);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	lData = *((long*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator>>( unsigned long& ulData ) 
{
	int iTypeSize = sizeof(unsigned long);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	ulData = *((unsigned long*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (__int64& i64Data)
{
	int iTypeSize = sizeof(__int64);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	i64Data = *((__int64*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (unsigned __int64& ui64Data)
{
	int iTypeSize = sizeof(unsigned __int64);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	ui64Data = *((unsigned __int64*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (float& fData)
{
	int iTypeSize = sizeof(float);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	fData = *((float*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator >> (double& dData)
{
	int iTypeSize = sizeof(double);

	if(!CheckOutBuffer(iTypeSize))
		return *this;

	dData = *((double*)&m_pBuffer[m_iStartPointer]);
	m_iStartPointer += iTypeSize;

	return *this;
}

CNtlSerializer& CNtlStorageTextSerializer::operator>>( std::string& str ) 
{
	str = (char*)&m_pBuffer[m_iStartPointer];
	m_iStartPointer += ((unsigned int)str.size());

	return *this;
}