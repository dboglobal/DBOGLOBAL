#ifndef __TQS_ALGO_DBOG_ACTION_DIRECTION_INDICATION_H__
#define __TQS_ALGO_DBOG_ACTION_DIRECTION_INDICATION_H__

#include "TqsAlgoAction_Base.h"

enum eDIRECTION_INDICATE_TYPE;
class CNtlVector;


class CTqsAlgoAction_Direction_Indicate : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_Direction_Indicate(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_Direction_Indicate();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bIndicate;

	eDIRECTION_INDICATE_TYPE	m_eIndicateType;

	bool						m_bAutoHide;

	TBLIDX						m_tblidx;

	CNtlVector					m_vLoc;

};

#endif