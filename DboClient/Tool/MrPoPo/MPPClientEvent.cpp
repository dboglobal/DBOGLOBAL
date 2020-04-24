#include "StdAfx.h"
#include "MPPClientEvent.h"
#include "MPPLogView.h"


CMPPClientEvent::CMPPClientEvent(void)
{
    m_hDBOClient = NULL;
}

CMPPClientEvent::~CMPPClientEvent(void)
{

}

BOOL CMPPClientEvent::CheckClientConnect() 
{
    m_hDBOClient = FindWindow(L"DragonBall", L"DragonBallOnline");
    if(!m_hDBOClient)
    {
        MessageBox(NULL, L"Can't Connect DBO Client", NULL, MB_OK);
        AddLogView(L"Can't Connect DBO Client");
        return FALSE;
    }            

    return TRUE;
}

void CMPPClientEvent::SendEvent( int nMsgCode, DWORD param ) 
{
    if(!CheckClientConnect())
        return;

    SendMessage(m_hDBOClient, MSG_FROM_MRPOPO, nMsgCode, param);
}
