#ifndef __INC_DBOG_CHARATTRIBUTE_BOT_H__
#define __INC_DBOG_CHARATTRIBUTE_BOT_H__


#include "CharacterAtt.h"

class CNpc;

class CCharacterAttBot : public CCharacterAtt
{

public:
	CCharacterAttBot();
	virtual	~CCharacterAttBot();

public:

	bool				Create(CNpc* pBot);

	virtual void		CalculateAtt();


public:

	virtual float		GetWalkSpeed();


private:

	CNpc*				m_pBotRef;

};

#endif