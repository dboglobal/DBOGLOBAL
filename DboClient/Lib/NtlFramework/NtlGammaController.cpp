#include "precomp_ntlframework.h"
#include "NtlGammaController.h"
#include "NtlCoreUtil.h"

#include "NtlApplication.h"

// SysEvent
#include "NtlSysEvent.h"
#include "NtlCoreApi.h"

// Gamma 근사치
#define dNTL_GAMMA_OFFSET		0.01f

CNtlGammaController::CNtlGammaController()
: m_bGammaOffset(FALSE)
, m_bCreate(FALSE)
{
	
}

CNtlGammaController::~CNtlGammaController()
{
	Destroy();
}

RwBool CNtlGammaController::Create()
{
	if( m_bCreate )
		return FALSE;

	LinkMsg( g_EventToggleFullScreen, 0 );

	m_hRestore = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &CNtlGammaController::RestoreHandler);

	m_bCreate = TRUE;

	BackupBackGroundGamma();

	return TRUE;
}

RwBool CNtlGammaController::Create( RwReal fGamma )
{
	if( m_bCreate )
		return FALSE;

	LinkMsg( g_EventToggleFullScreen, 0 );
	
	m_hRestore = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &CNtlGammaController::RestoreHandler);

	m_bCreate = TRUE;

	BackupBackGroundGamma();

	return TRUE;
}

void CNtlGammaController::Destroy()
{
	if( m_bCreate )
	{
		RestoreBackGroundGamma();

		UnLinkMsg( g_EventToggleFullScreen );		

		CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore( m_hRestore );

		m_bCreate = FALSE;
	}
}

void CNtlGammaController::BackupBackGroundGamma()
{
	if( m_bCreate )
	{
		HWND				hWnd = CNtlApplication::GetInstance()->GetHWnd();
		HDC					hDC = GetDC( hWnd );

		GetDeviceGammaRamp( hDC, &m_rampBackGroundBackup );

		ReleaseDC( hWnd, hDC );
	}
}

void CNtlGammaController::RestoreBackGroundGamma()
{
	if( m_bCreate )
	{
		HWND				hWnd = CNtlApplication::GetInstance()->GetHWnd();
		HDC					hDC = GetDC( hWnd );

		SetDeviceGammaRamp( hDC, &m_rampBackGroundBackup );				

		ReleaseDC( hWnd, hDC );
	}
}

void CNtlGammaController::SetGamma( RwReal fGamma )
{
	if( !CNtlApplication::GetInstance()->IsFullScreen() )
	{
		m_fGamma = fGamma;
		return;
	}

	if( m_bCreate )
	{
		HWND				hWnd = CNtlApplication::GetInstance()->GetHWnd();
		HDC					hDC	= GetDC( hWnd );

		// 저장되어 있는 감마값이 새로 들어온 감마값과 똑같다면 근사값을 줘서 적용한다.
		if( m_fGamma == fGamma )
		{
			if( m_bGammaOffset )
			{
				fGamma += dNTL_GAMMA_OFFSET;
			}
			else
			{
				fGamma -= dNTL_GAMMA_OFFSET;
			}

			m_bGammaOffset = !m_bGammaOffset;
		}

		GetGammaRamp(&m_rampCurrent, fGamma);
		SetDeviceGammaRamp(hDC, &m_rampCurrent);
		m_fGamma = fGamma;

		ReleaseDC(hWnd, hDC); 
	}
}

/**
* \brief GAMMA RAMP값을 만든다.
* \param pGammaRamp			unsigned short형의 256*3 크기의 메모리 공간
* \param fGamma				감마값 ( 0 ~ 4 )
* \param fOverBright		밝기
* \param fContrast			대비
*/
void CNtlGammaController::GetGammaRamp( void* pGammapRamp, RwReal fGamma, RwReal fOverBright /*= 1.0f*/, RwReal fContrast /*= 1.0f*/ )
{
	if( m_bCreate )
	{
		WORD* p = (WORD*)pGammapRamp;
		for (int i = 0; i < 256; ++i)
		{
			RwReal f = (255.0f * pow((float)i / 256.0f, 1.0f / fGamma)) * fContrast + fOverBright;

			if (f < 0)
			{
				f = 0.0f;
			}
			else if (f > 255)
			{
				f = 255.0f;
			}
			p[i] = p[256 + i] = p[512 + i] = ((WORD)f) << 8;
		}
	}
}

void CNtlGammaController::HandleEvents(	RWS::CMsg &pMsg )
{
	if( m_bCreate )
	{
		if( pMsg.Id == g_EventToggleFullScreen )
		{
			bool* pbFullScreen = reinterpret_cast<bool*>( pMsg.pData );

			if( !(*pbFullScreen) )
			{
				// 바탕화면의 Gamma를 복원시킨다.
				RestoreBackGroundGamma();
			}
			else
			{
				// 어플리케이션의 감마로 복귀
				SetCurrentGamma();
			}
		}
	}
}

void CNtlGammaController::SetCurrentGamma()
{
	if( m_bCreate )	
		SetGamma( m_fGamma );
}

int CNtlGammaController::RestoreHandler()
{
	if( m_bCreate )
		SetGamma( m_fGamma );

	return TRUE;
}