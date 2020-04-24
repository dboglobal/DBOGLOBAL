#ifndef __INC_DBOG_OBJECTCONTROLLER_H__
#define __INC_DBOG_OBJECTCONTROLLER_H__


class CObjectMsg;

class CObjectController
{

public:
	CObjectController();
	virtual	~CObjectController();

public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple) = 0;
	virtual int OnObjectMsg(CObjectMsg * pObjMsg) = 0;

};

#endif
