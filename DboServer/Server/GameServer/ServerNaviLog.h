#ifndef __DBOG_SERVER_NAVI_LOG_H__
#define __DBOG_SERVER_NAVI_LOG_H__

#include "NtlNavi.h"

class CServerNaviLog : public INtlNaviLog
{

public:

	CServerNaviLog(void);
	~CServerNaviLog(void);

public:

	bool					Create();

	void					Destroy();

protected:

	void					Init();

public:

	virtual void			Log(const char* pLog, ...);
};


#endif
