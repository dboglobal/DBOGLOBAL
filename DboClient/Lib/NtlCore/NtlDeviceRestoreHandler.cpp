#include "precomp_ntlcore.h"
#include "NtlDeviceRestoreHandler.h"

CNtlDeviceRestoreHandler::~CNtlDeviceRestoreHandler()
{
}

CNtlDeviceRestoreHandler* CNtlDeviceRestoreHandler::GetInstance(void)
{
	static CNtlDeviceRestoreHandler Instance;
	return &Instance;
}

void CNtlDeviceRestoreHandler::UnLinkDeviceRelease(DEVICE_RESTORE_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkDeviceRelease");
	
	CNtlCallbackParam0 *pWidget = reinterpret_cast<CNtlCallbackParam0*>(handle);

	ListCallBack::iterator it;
	for(it = m_listDeviceRelease.begin(); it != m_listDeviceRelease.end(); ++it)
	{
		CNtlCallbackParam0 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listDeviceRelease.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}


void CNtlDeviceRestoreHandler::UnLinkDeviceRestore(DEVICE_RESTORE_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkDeviceRestore");
	
	CNtlCallbackParam0 *pWidget = reinterpret_cast<CNtlCallbackParam0*>(handle);

	ListCallBack::iterator it;
	for(it = m_listDeviceRestore.begin(); it != m_listDeviceRestore.end(); ++it)
	{
		CNtlCallbackParam0 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listDeviceRestore.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}


void CNtlDeviceRestoreHandler::ReleaseHandler()
{
	NTL_FUNCTION("CNtlDeviceRestoreHandler::ReleaseHandler");
	
	ListCallBack::iterator it;
	for(it = m_listDeviceRelease.begin(); it != m_listDeviceRelease.end(); ++it)
	{
		(*it)->Call();
	}

	NTL_RETURNVOID();
}

void CNtlDeviceRestoreHandler::RestoreHandler()
{
	NTL_FUNCTION("CNtlDeviceRestoreHandler::DeviceRestoreHandler");
	
	ListCallBack::iterator it;
	for(it = m_listDeviceRestore.begin(); it != m_listDeviceRestore.end(); ++it)
	{
		(*it)->Call();
	}

	NTL_RETURNVOID();
}
