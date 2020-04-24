#ifndef __SERVER_AISONODE_CONDITION_HIMSELFCHECK__
#define __SERVER_AISONODE_CONDITION_HIMSELFCHECK__


#include "ControlScriptNodeCondition.h"


class CAISNodeCondition_HimselfCheck : public CControlScriptNodeCondition
{

public:
	CAISNodeCondition_HimselfCheck(const char* lpszName);
	virtual ~CAISNodeCondition_HimselfCheck();

	enum eMAXCOUNT_HIMSELF
	{
		MAX_COUNT_HIMSELF_SKILLHIT = 0xA,
	};

	enum eAIS_HIMSELF_CHECKTYPE
	{
		AIS_EVENT_CHECKTYPE_LP,
		AIS_EVENT_CHECKTYPE_SKILLHIT,
		MAX_AIS_HIMSELF_CHECKTYPE,
		INVALID_AIS_HIMSELF_CHECKTYPE,
	};

	enum eAIS_HIMSELF_VALUETYPE
	{
		AIS_EVENT_VALUETYPE_PERCENT,
		AIS_EVENT_VALUETYPE_SKILLTBLIDX,
		MAX_AIS_HIMSELF_VALUETYPE,
		INVALID_AIS_HIMSELF_VALUETYPE,
	};


	virtual sPARAMETER_INFO*	GetParameterMap();


public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	eAIS_HIMSELF_CHECKTYPE		m_eCheckType;

	eAIS_HIMSELF_VALUETYPE		m_eValueType;

	float						m_fVal;

	TBLIDX						m_tblidxSkill[10];

	BYTE						m_byTblidxSkillCount;


};


#endif
