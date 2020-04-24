#ifndef __WORLD_FACTORY_H__
#define __WORLD_FACTORY_H__


class CWorld;

class CWorldFactory
{

public:

	CWorldFactory();
	virtual ~CWorldFactory();

public:

	virtual CWorld*		Create(); //not sure
	virtual void		Destroy(CWorld *pWorld);
	

};


#endif