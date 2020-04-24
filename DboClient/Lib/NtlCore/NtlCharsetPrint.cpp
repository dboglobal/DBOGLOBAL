#include "precomp_ntlcore.h"
#include "NtlCharsetPrint.h"

CNtlCharsetPrint::CNtlCharsetPrint()
{
	for(RwInt32 i = 0; i < MAX_CHARSET; i++)
	{
		m_Charset[i].bEnable = FALSE;
	}
}

CNtlCharsetPrint* CNtlCharsetPrint::GetInstance(void)
{
	static CNtlCharsetPrint Instance;
	return &Instance;
}

RwBool CNtlCharsetPrint::IsValidHandle(CHARSET_HANDLE h)
{
	if(h < 0 || h >= MAX_CHARSET)
		return FALSE;

	return TRUE;
}

CHARSET_HANDLE CNtlCharsetPrint::Acquire(void)
{
	for(RwInt32 i = 0; i < MAX_CHARSET; i++)
	{
		if(m_Charset[i].bEnable == FALSE)
		{
			m_Charset[i].bEnable = TRUE;
			return i;
		}
	}

	return -1;
}

void CNtlCharsetPrint::Release(CHARSET_HANDLE h)
{
	if(!IsValidHandle(h))
		return;
		
	m_Charset[h].bEnable = FALSE;
	m_Charset[h].str = "";
}

void CNtlCharsetPrint::Print(CHARSET_HANDLE h, const RwChar *pString)
{
	if(!IsValidHandle(h))
		return;

	m_Charset[h].str = pString;
}

void CNtlCharsetPrint::Format(CHARSET_HANDLE h, char *format, ...)
{
	if(!IsValidHandle(h))
		return;

	char text[1024];

	va_list args;
	va_start(args, format);

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vsprintf_s(text, 1024, format, args);
#else
	vsprintf(text, format, args);
#endif
	va_end(args);

	m_Charset[h].str = text;
}

RwBool CNtlCharsetPrint::IsValid(RwUInt32 iIdx)
{
	if(!IsValidHandle(iIdx))
		return FALSE;
    
	return m_Charset[iIdx].bEnable;
}

const RwChar* CNtlCharsetPrint::GetString(RwUInt32 iIdx) 
{
	if(!IsValidHandle(iIdx))
		return NULL;
	
	return m_Charset[iIdx].str.c_str();
}
