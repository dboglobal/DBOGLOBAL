#ifndef __DBOG_BUFFLINKEDOBJECT_H__
#define __DBOG_BUFFLINKEDOBJECT_H__


class CBuff;
#include "NtlLinkList.h"


class CBuffLinkedObject : public CNtlLinkObject
{

public:

	CBuffLinkedObject(CBuff* pBuff);
	virtual ~CBuffLinkedObject();

private:

	void					Init();

public:

	void					Destroy();

	CBuff*					GetBuffRef();

	void					SetBuffRemoved() { m_bIsBuffRemoved = true; }

	bool					IsBuffRemoved() { return m_bIsBuffRemoved; }

private:

	CBuff*					m_pBuffRef;

	bool					m_bIsBuffRemoved;

};


#endif