/*****************************************************************************
 *
 * File			: NtlCoreStruct.h
 * Author		: Hong Sungbock
 * Copyright	: (аж)NTL
 * Date			: 2008. 9. 16	
 * Abstract		: 
 *****************************************************************************
 * Desc         : widely usable and corable struct description
 *
 *****************************************************************************/

#ifndef __NTL_CORE_DESCRIPTION_H__
#define __NTL_CORE_DESCRIPTION_H__

struct sVERSTION
{
	RwReal		fLastestVersion;
	RwReal		fCurrentWorkVerstion;


	sVERSTION(RwReal fNewest)
	:fLastestVersion(fNewest),
	fCurrentWorkVerstion(fNewest)
	{}
};

#endif