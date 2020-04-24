#include "stdafx.h"
#include "CallBack.h"

void CallbackFP::operator()()
{
	mycallback();
}

void CallbackFP::execute()
{
	mycallback();
}

CallbackFP* CallbackFP::Create()
{
	return new CallbackFP(0);
}
