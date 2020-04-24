// ERSManager.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"


// The module attribute causes WinMain to be automatically implemented for you
[ module(SERVICE, uuid = "{52AAB9A2-F144-4F17-BFB7-8FAE1B70338C}", 
		 name = "ERSManager", 
		 helpstring = "ERSManager 1.0 Type Library", 
		 resource_name="IDS_SERVICENAME") ]
class CERSManagerModule
{
public:
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
		// your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.

		return S_OK;
	}
};

