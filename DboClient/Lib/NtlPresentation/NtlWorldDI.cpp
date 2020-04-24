//------------------------------------------------------------------------------
//
// Headers
//
//------------------------------------------------------------------------------
#include "NtlWorldDI.h"

////////////////////////////////////////////////////////////////////////////////
//
// Globals
//
////////////////////////////////////////////////////////////////////////////////
UCHAR g_szKeyBuf[ 256 ]; // The keyboard's key buffer
UCHAR g_szOldKeyBuf[ 256 ]; // The keyboard's key buffer
UCHAR g_szPKeyBuf[ 256 ]; // The keyboard's key buffer
DIMOUSESTATE g_dims; // The mouse's state structure

////////////////////////////////////////////////////////////////////////////////
//
// Classes
//
////////////////////////////////////////////////////////////////////////////////
CDI::CDI()
{
	m_pDI = NULL;
	m_pDIDK = NULL;
}

CDI::~CDI(){}

void CDI::Init(HWND hMainFrmWnd)
{
	CurWindowHandle = hMainFrmWnd;
	Create();
}

void CDI::Free()
{
	Destroy();
}

bool CDI::Create()
{
	HRESULT hr;	

    if(FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL)))
	{
		::MessageBox(CurWindowHandle, "CDI::Create, DirectInput8Create", "Failed", MB_OK);
		return false;
	}

	// Create a keyboard device
	if(FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDK, NULL)))
	{
		Destroy();
		::MessageBox(CurWindowHandle, "CDI::Create, CreateDevice", "Failed", MB_OK);
		return false;
	}
	if(FAILED(m_pDIDK->SetDataFormat(&c_dfDIKeyboard)))
	{
		Destroy();
		::MessageBox(CurWindowHandle, "CDI::Create, SetDataFormat", "Failed", MB_OK);
		return false;
	}
	if(FAILED(m_pDIDK->SetCooperativeLevel(CurWindowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		Destroy();
		::MessageBox(CurWindowHandle, "Could not set keyboard's cooperation level", "Error", MB_OK);
		return false;
	}
	m_pDIDK->Acquire();
	
	return true;
}
		
void CDI::Destroy()
{
	// First, since we create the keyboard last, we need to 'destroy' it first.
	if(m_pDIDK)
	{
		// Unacquire the keyboard
		m_pDIDK->Unacquire();
		m_pDIDK->Release();
		m_pDIDK = NULL;
	}

	// Now the main DirectInput object
	if(m_pDI)
	{
		m_pDI->Release();
		m_pDI = NULL;
	}
}

void CDI::InvalidateDeviceObjects()
{
	Destroy();
}

void CDI::RestoreDeviceObjects()
{
	Create();
}

void CDI::Update()
{
	HRESULT hr;
	
	// Update keyboard
    if(FAILED(hr = m_pDIDK->GetDeviceState(sizeof(UCHAR[256]), (LPVOID)&g_szKeyBuf)))
	{ 
        if(hr == DIERR_INPUTLOST || hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED) 
		{
			m_pDIDK->Acquire();
			ZeroMemory(g_szKeyBuf, 256);
		}
	}

	// It's pretty weird. g_szKeyBuf is damned by GetDeviceState() for the first try so this flag is temporary.
	static bool bFirstRun = true;
	if(bFirstRun)
	{
		ZeroMemory(g_szKeyBuf, 256);
		ZeroMemory(g_szOldKeyBuf, 256);
		ZeroMemory(g_szPKeyBuf, 256);
		bFirstRun = false;
	}
	
	for(int i = 0 ; i < 256; i++)
	{
		// if pressed now, but not pressed previously;
		if ((g_szKeyBuf[i] & 0x80) && !(g_szOldKeyBuf[i] & 0x80))
			g_szPKeyBuf[i] = TRUE;
		else
			g_szPKeyBuf[i] = FALSE;
	}

	memcpy(g_szOldKeyBuf, g_szKeyBuf, 256);
}
