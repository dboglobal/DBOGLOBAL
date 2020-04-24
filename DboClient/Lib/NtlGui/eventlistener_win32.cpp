#include "gui_precomp.h"
#include "eventlistener_win32.h"

// std::list<CEventListenerSlot*> CEventListener_Win32::m_CallbackListener;
// std::list<CListenerHandle> CEventListener_Win32::m_RemoveListener;

CEventListenerSlot* CEventListener_Win32::m_pListener[MAX_LISTENER] = {NULL, };

