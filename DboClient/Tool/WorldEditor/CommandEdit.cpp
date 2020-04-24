// CommandEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ConsoleDialog.h"
#include "CommandEdit.h"


// CCommandEdit

IMPLEMENT_DYNAMIC(CCommandEdit, CEdit)
CCommandEdit::CCommandEdit()
{
	m_pParent = NULL;
}

CCommandEdit::~CCommandEdit()
{
}


BEGIN_MESSAGE_MAP(CCommandEdit, CEdit)
END_MESSAGE_MAP()


BOOL CCommandEdit::PreTranslateMessage( MSG* pMsg )
{
	switch( pMsg->message )
	{
		case WM_KEYDOWN:
		{
			switch( pMsg->wParam )
			{
				case VK_RETURN:
				{
					TCHAR		aBuffer[512];
					ZeroMemory( aBuffer, sizeof( TCHAR ) * 512 );

					if( GetLine( 0, aBuffer, 512 ) == 0 )
					{
						m_pParent->CancelCommand();

						return TRUE;
					}

					if( m_pParent != NULL )
					{
//						UINT nCtrlID = GetDlgCtrlID();
						m_pParent->SetCommand( aBuffer );
					}

					return TRUE;
				}

				case VK_ESC:
				{
					m_pParent->CancelCommand();

					return TRUE;
				}
			}

			break;
		}
	}

	return CEdit::PreTranslateMessage( pMsg );
}

// CCommandEdit 메시지 처리기입니다.

