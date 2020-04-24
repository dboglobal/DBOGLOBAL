/*****************************************************************************
 *
 * File			: NtlSobAttFactory.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object attribute factory class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOB_ATTR_FACTORY_H__
#define __NTL_SOB_ATTR_FACTORY_H__


class CNtlSobAttr;

class CNtlSobAttrFactory
{
public:

	static CNtlSobAttr* CreateSobAttrFactory(RwUInt32 uiClassId);
	static void DeleteSobAttrFactory(CNtlSobAttr *pSobObj);
};

#endif
