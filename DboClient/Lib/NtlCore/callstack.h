/*****************************************************************************
 *
 * File :     CallStack.h
 *
 *****************************************************************************/

#ifndef __CALLSTACK_H__
#define __CALLSTACK_H__

class CNtlCallStack
{
private:

	enum { ECsDepth	= 128 };
	enum { ECSLen = 1024 };

	char m_Depth[ECsDepth][ECSLen];

public:

	CNtlCallStack();

	static CNtlCallStack* GetInstance(void);

	void Push(const char *pString);
	void LogFilePrint(void);
};

static CNtlCallStack* GetNtlCallStack(void)
{
	return CNtlCallStack::GetInstance();
}


#endif