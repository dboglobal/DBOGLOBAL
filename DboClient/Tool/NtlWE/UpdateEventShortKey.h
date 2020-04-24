#pragma once

class CUpdateEventShortKey
{
public:
	CUpdateEventShortKey();
	virtual ~CUpdateEventShortKey();

	virtual void UpdateShortKey();

protected:
	virtual void UpdateShortKeyOutdoor();
	virtual void UpdateShortKeyIndoor();	

	virtual void UpdateShortKeyOther();
};