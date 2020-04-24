#ifndef __NTL_PLSKYSPHERE_H__
#define __NTL_PLSKYSPHERE_H__


#include "NtlPLSky.h"


class CNtlPLSkySphere : public CNtlPLSky
{
public:
	CNtlPLSkySphere(void);
	virtual	~CNtlPLSkySphere(void);

	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();
	virtual RwBool	Update(RwReal fElapsed);
	virtual void	HandleEvents(RWS::CMsg &pMsg);
	virtual	void	UpdateFieldVariation(RwReal fElapsed);
};


#endif //__NTL_PLSKYSPHERE_H__