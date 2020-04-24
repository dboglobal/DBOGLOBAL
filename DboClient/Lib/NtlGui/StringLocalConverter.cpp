#include "gui_precomp.h"
#include "StringLocalConverter.h"

// core
#include "NtlDebug.h"

// cleint
#include "StringLocalConverter_LocalBase.h"
#include "StringLocalConverter_Korean.h"

START_GUI

CStringLocalConverter* CStringLocalConverter::m_pInstance = NULL;


CStringLocalConverter* CStringLocalConverter::GetInstance()
{
	NTL_ASSERT(m_pInstance, "CStringLocalConverter::GetInstance, Not exist string converter instance");
	return m_pInstance;
}

RwBool CStringLocalConverter::CreateInstance()
{
	NTL_FUNCTION("CStringLocalConverter::CreateInstance");

	m_pInstance = NTL_NEW CStringLocalConverter;
	NTL_ASSERT( m_pInstance, "CStringLocalConverter::CreateInstance : Instance Allocate Fail" );

	CStringLocalConverter_Korean* pLocalKorean = NTL_NEW CStringLocalConverter_Korean;
	m_pInstance->m_mapLocalConverter[LOCAL_TYPE_KOREA] = pLocalKorean;

	NTL_RETURN(TRUE);
}

void CStringLocalConverter::DestroyInstance()
{
	NTL_FUNCTION("CStringLocalConverter::DestoryInstance");

	if(!m_pInstance)
		return;

	MAP_LOCAL_CONVERTER::iterator it = m_pInstance->m_mapLocalConverter.begin();
	for( ; it != m_pInstance->m_mapLocalConverter.end() ; ++it )
	{
		NTL_DELETE( it->second );
	}
	m_pInstance->m_mapLocalConverter.clear();

	NTL_DELETE(m_pInstance);

	NTL_RETURNVOID();
}

const WCHAR* CStringLocalConverter::LocalStringCovert(const WCHAR* pwcText)
{
	if( !pwcText )
		return NULL;

	RwUInt32 uiACP = GetACP();
	RwInt8 byLocalType = INVALID_LOCAL_TYPE;

	memset(m_awcBuffer, 0, dSTRING_CONVERTER_BUFFER_SIZE);

	switch(uiACP)
	{
	case 949:		byLocalType = LOCAL_TYPE_KOREA;		break;
	default:		return pwcText;
	}


	MAP_LOCAL_CONVERTER::iterator it = m_mapLocalConverter.find(byLocalType);
	if( it != m_mapLocalConverter.end() )
	{
		CStringLocalConverter_LocalBase* pLocalConverter = (CStringLocalConverter_LocalBase*)it->second;
		pLocalConverter->LocalStringCovert(m_awcBuffer, pwcText);
		return m_awcBuffer;
	}

	return pwcText;
}

END_GUI