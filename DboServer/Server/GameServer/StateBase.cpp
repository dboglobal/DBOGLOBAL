#include "stdafx.h"
#include "StateBase.h"

CStateBase::CStateBase(BYTE stateID)
{
	this->m_stateID = stateID;
	this->m_prevStateID = INVALID_BYTE;
	this->m_nextStateID = INVALID_BYTE;
	this->m_dwTime = 0;
	this->m_bFinish = false;
	this->m_pvInitialData = NULL;
	this->m_pvReferenceDataForExiting = NULL;
}

CStateBase::~CStateBase()
{

}
