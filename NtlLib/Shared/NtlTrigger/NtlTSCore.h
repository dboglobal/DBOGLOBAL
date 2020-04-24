#ifndef _NTL_TSCORE_H_
#define _NTL_TSCORE_H_


#include <string>
#include <vector>
#include <deque>
#include <map>
#include <stack>
#include <algorithm>


#include "NtlTSCoreDefine.h"
#include "NtlTSCoreStatic.h"


/** 
	Trigger system utility includes
*/

#include "NtlTSLog.h"

#include "NtlTSRTTI.h"

#include "NtlTSMemIO.h"

#include "NtlTSScrObject.h"
#include "NtlTSScrFactoryType.h"
#include "NtlTSScrFactory.h"
#include "NtlTSScrStream.h"
#include "NtlTSScrSaver.h"


/** 
	Trigger system entity includes
*/


#include "NtlTSEntityFactoryType.h"
#include "NtlTSEntityFactory.h"

#include "NtlTSEntity.h"
#include "NtlTSEvent.h"
#include "NtlTSCond.h"
#include "NtlTSAction.h"
#include "NtlTSCont.h"
#include "NtlTSGroup.h"
#include "NtlTSTrigger.h"


/** 
	Trigger system control includes
*/


#include "NtlTSControlObject.h"
#include "NtlTSControlFactoryType.h"
#include "NtlTSControlFactory.h"

#include "NtlTSController.h"
#include "NtlTSAgency.h"
#include "NtlTSEvtMapper.h"



/** 
	Trigger system ui includes
*/


#include "NtlTSUIObject.h"
#include "NtlTSUIFactoryType.h"
#include "NtlTSUIFactory.h"

#include "NtlTSRecv.h"
#include "NtlTSAgency.h"
#include "NtlTSMain.h"


#endif