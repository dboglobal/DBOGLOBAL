// stdafx.h : 잘 변경되지 않고 자주 사용하는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
#ifndef	_STD_AFX_H_
#define _STD_AFX_H_

#pragma once

#pragma warning (disable:4786)	// template parameter length waning
#pragma warning (disable:4018)	// sign/unsign Waning
#pragma warning (disable:4244)	// data converting warning
#pragma warning (disable:4805)	// bool/BOOL converting warning
#pragma warning (disable:4311)	// convert warning
#pragma warning (disable:4312)	// convert warning
#pragma warning (disable:4275)	// dll parent class warning
#pragma warning (disable:4100)	// unreferenced formal parameter warning

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows 헤더에서 거의 사용되지 않는 내용을 제외시킵니다.
#endif

// 아래 지정된 플랫폼보다 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0400		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT 0x0400		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINDOWS		// Windows 98 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0410 // Windows Me 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 4.0 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0400	// IE 5.0 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소
#include <afxext.h>         // MFC 익스텐션
#include <afxdisp.h>        // MFC 자동화 클래스

#include <afxdtctl.h>		// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

// STL

#include <list>
#include <vector>
#include <hash_map>
using std::list;
using std::vector;
using stdext::hash_map;

// Windows API

#include <WinSock2.h> 
#include <Windowsx.h> 
#include <WinNls.h>

#define CRTDBG_MAP_ALLOC

#include <StdDef.h>

// Direct X

#include <D3D9.h>

// Randerware

//#include <RwPlCore.h>
#include <RwCore.h>

#include <RpWorld.h>
#include <RpCollis.h>
#include <RpMatFx.h>
#include <RpSkin.h>
#include <RpHAnim.h>
#include <RpUVAnim.h>
#include <RpRandom.h>
#include <RpLtMap.h>
#include <RpPvs.h>

#include <RtImport.h>
#include <RtIntsec.h>
#include <RtPick.h>
#include <RtCharse.h>
#include <RtFSyst.h>
#include <RtDict.h>
#include <RtQuat.h> 
#include <RtAnim.h>
#include <RtLtMap.h>

// NTL Common Library

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"

// NTL Core Layer

#include "NtlDebug.h"
#include "NtlText.h"

// NTL Presentation Layer

#include "NtlPlResource.h"
#include "NtlPlResourceManager.h"
#include "NtlPLProperty.h"
#include "NtlPLObjectProperty.h"
#include "NtlPLTerrainProperty.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLApi.h"

// NTL Framework Layer

#include "NtlApplication.h"
#include "NtlCamera.h"

// Etcs

//#include "DebugEx.h"
#include "PropTree.h"
#include "WorldMonitor.h"
#include "CLogMessageManager.h"
#include "UserDefinedAssert.h"

#endif //_STD_AFX_H_