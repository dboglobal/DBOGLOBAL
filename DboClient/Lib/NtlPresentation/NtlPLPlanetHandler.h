#pragma once


#include "ceventhandler.h"
#include "NtlPLPlanet.h"


struct sNTL_FIELD_PROP;


class CNtlPLPlanetHandler : public RWS::CEventHandler
{
public:
	CNtlPLPlanetHandler(void);
	virtual ~CNtlPLPlanetHandler(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

protected:
	CNtlPLPlanet*		m_pPlanetGroupReal[dNTLPL_PLANET_MAX_CNT];
	CNtlPLPlanet*		m_pPlanetGroupDumm[dNTLPL_PLANET_MAX_CNT];

protected:
	RwBool	IsInterpolationNeeded(sNTL_FIELD_PROP* _pNewFieldProp);

public:
	void	Update(RwReal _ElapsedTime);
	void	SetVisible(RwBool bVisible);


protected:
	RwBool	m_bVisible;
};
