#ifndef __SERVER_ACTIONPATTERN_MANAGER__
#define __SERVER_ACTIONPATTERN_MANAGER__


#include "NtlSingleton.h"
#include "ContainerActionPatternSet.h"


class CActionPatternSystem : public CNtlSingleton<CActionPatternSystem>
{

public:

	CActionPatternSystem();
	virtual ~CActionPatternSystem();

public:

	bool						Create();
	void						Destroy();

	CContainerActionPatternSet*	GetContainerActionPatternSet() { return &m_cContainerActionPatternSet; }

private:

	CContainerActionPatternSet	m_cContainerActionPatternSet;

};

#define GetActionPatternManager()		CActionPatternSystem::GetInstance()
#define g_pActionPatternSystem			GetActionPatternManager()

#endif