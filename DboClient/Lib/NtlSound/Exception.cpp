#include "precomp_ntlsound.h"
#include "exception.h"
#include "fmod_errors.h"

BOOL ErrorCheck(FMOD_RESULT ret)
{
    if( ret != FMOD_OK )
	{
		return FALSE;
	}

	return TRUE;
}