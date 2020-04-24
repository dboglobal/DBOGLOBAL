/*****************************************************************************
 *
 * File			: .h
 * Author		: HongHoDong
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 26	
 * Abstract		: Input handler 
 *****************************************************************************
 * Desc         : Device Restore CallBack Handler
 *
 *****************************************************************************/

#ifndef __NTL_DEVICE_HANDLER_H__
#define __NTL_DEVICE_HANDLER_H__

#include "NtlCallbackWidget.h"
#include "NtlDebug.h"
#include <list>

typedef void* DEVICE_RESTORE_HANDLE;

class CNtlDeviceRestoreHandler
{
private:

	typedef std::list<CNtlCallbackParam0*> ListCallBack;

	ListCallBack m_listDeviceRelease;
	ListCallBack m_listDeviceRestore;
	

public:

	CNtlDeviceRestoreHandler() {}
	~CNtlDeviceRestoreHandler();

	static CNtlDeviceRestoreHandler* GetInstance(void);

	template <class Callbackclass>	
	DEVICE_RESTORE_HANDLE LinkDeviceRelease(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
	{
		CNtlCallbackParam0 *pWidget = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
		m_listDeviceRelease.push_back(pWidget);
		return pWidget;
	}

	void UnLinkDeviceRelease(DEVICE_RESTORE_HANDLE handle);


	template <class Callbackclass>	
	DEVICE_RESTORE_HANDLE LinkDeviceRestore(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
	{
		CNtlCallbackParam0 *pWidget = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
		m_listDeviceRestore.push_back(pWidget);
		return pWidget;
	}

	void UnLinkDeviceRestore(DEVICE_RESTORE_HANDLE handle);


	void ReleaseHandler();
	void RestoreHandler();
};

#endif

