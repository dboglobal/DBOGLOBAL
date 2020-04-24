#include "StdAfx.h"
#include "MTPropertyContainer.h"
#include "NtlDebug.h"

CMTPropertyContainer::CMTPropertyContainer()
{

}

CMTPropertyContainer::~CMTPropertyContainer()
{

}

RwBool CMTPropertyContainer::Load()
{
	NTL_FUNCTION("__FUNCTION__");

	//////////////////////////////////////////////////////////////////////////
	// 월드용 로드
	if(!LoadNtlWorld(""))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadWater(".\\script\\waterproperty.xml"))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadShoreLine(".\\script\\ShoreLineProp.xml"))
	{
		NTL_RETURN(FALSE);
	}
	//////////////////////////////////////////////////////////////////////////

	if(!LoadEffect(".\\Effect\\visualeffect_01.eff"))
	{
		NTL_RETURN(FALSE);
	}
	if (!LoadEffect(".\\Effect\\visualeffect_02.eff"))
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}
