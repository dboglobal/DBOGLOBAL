/*****************************************************************************
 *
 * File			: NtlLoadingController.cpp
 * Author		: JeongHo,Rho
 * Copyright	: (аж)NTL
 * Date			: 2007. 2. 13	
 * Abstract		: 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#include "precomp_ntlcore.h"
#include "NtlLoadingController.h"



CNtlLoadingController* CNtlLoadingController::GetInstance( void )
{
	static CNtlLoadingController clInstance;
	return &clInstance;
}
