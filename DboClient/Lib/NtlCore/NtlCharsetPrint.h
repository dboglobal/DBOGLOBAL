/*****************************************************************************
 *
 * File			: NtlCharsetPrint.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 10. 11	
 * Abstract		: screen display charset print
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_CHARSET_PRINT_H__
#define __NTL_CHARSET_PRINT_H__

#include <rwcore.h>
#include <string>

typedef RwInt32 CHARSET_HANDLE;

class CNtlCharsetPrint
{
private:

	enum { MAX_CHARSET = 20 };

	typedef struct _SCharset
	{
		RwBool bEnable;
		std::string str;
	}SCharset;

	SCharset m_Charset[MAX_CHARSET];

private:

	RwBool IsValidHandle(CHARSET_HANDLE h);

public:

    CNtlCharsetPrint();

	static CNtlCharsetPrint* GetInstance(void);

	CHARSET_HANDLE Acquire(void);
	void Release(CHARSET_HANDLE h);

	void Print(CHARSET_HANDLE h, const RwChar *pString);
	void Format(CHARSET_HANDLE h, char *format, ...);

	RwInt32 GetCount(void) { return MAX_CHARSET; }
	RwBool IsValid(RwUInt32 iIdx);
	const RwChar* GetString(RwUInt32 iIdx);
};

#endif
