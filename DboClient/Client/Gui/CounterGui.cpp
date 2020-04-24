#include "precomp_dboclient.h"
#include "CounterGui.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLLogic.h"

#define dNUMBER_SIZE_1			0.5f
#define dNUMBER_SIZE_2			0.7f
#define dNUMBER_SIZE_3			1.0f


CCounterGui::CCounterGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pflashBackground(NULL)
,m_fElapsed(0.f)
,m_fUsableAlarmTime(0.f)
,m_fSizeRate(dNUMBER_SIZE_3)
,m_iAlarmElapsed(0)
,m_byDisplayFlag(0)
,m_eQuadrant(COUNTER_QUADRANT_4)
,m_bPause(FALSE)
,m_bAlarmSound(FALSE)
,m_bRender(FALSE)
{
}

CCounterGui::~CCounterGui()
{
}

RwBool CCounterGui::Create(eCounterDisplayType eDisplayType, RwReal fSecond, eCounterQuadrant eQuadrant /* = COUNTER_QUADRANT_4 */)
{
	NTL_FUNCTION( "CCounterGui::Create" );

	CRectangle rect(0, 0, 0, 0);

	m_pThis = NTL_NEW gui::CDialog( &rect, CNtlPLGuiManager::GetInstance()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager() );

	NTL_ASSERT(eDisplayType <= COUNTER_DISPLAY_2, "CCounterGui::Create, Invalid display type");
	m_eDisplayType = eDisplayType;

	switch(eDisplayType)
	{
		case COUNTER_DISPLAY_1:
		{
			LoadSurface_1();
		}
		break;
		case COUNTER_DISPLAY_2:
		{
			LoadSurface_2();
		}
		break;
		default:
		{
			DBO_FAIL("Wrong counter display type");
			NTL_RETURN(FALSE);
		}
	}

	SetQuadrant(eQuadrant);

	m_fElapsed		= fSecond;
	m_iAlarmElapsed	= (RwInt32)fSecond;
	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig
	m_slotMove				= m_pThis->SigMove().Connect( this, &CCounterGui::OnMove );

	if( m_eDisplayType == COUNTER_DISPLAY_1 )
		m_slotPaintDisplay_1	= m_pThis->SigPaint().Connect( this, &CCounterGui::OnPaintDisplay_1 );	

	NTL_RETURN(TRUE);
}

VOID CCounterGui::Destroy()
{
	NTL_FUNCTION("CCounterGui::Destroy");

	if( m_pflashBackground )
		m_pflashBackground->Unload();

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	// 나중에 m_pThis를 스크립트를 사용해서 생성하면 이 코드의 주석을 풀자
	//CNtlPLGui::DestroyComponents();
	//CNtlPLGui::Destroy();

	if( m_pThis )
		NTL_DELETE(m_pThis);

	NTL_RETURNVOID();
}

VOID CCounterGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	if( m_bPause )
		return;

	if (m_pflashBackground)
	{
		m_pflashBackground->Update(fElapsed);
	}

	m_fElapsed -= fElapsed;

	if( m_fElapsed < 0.f )
		m_fElapsed = 0.f;

	
	// 매 초마다 째깍 소리를 내기 위하여
	if( m_bAlarmSound )
	{
		if( m_fElapsed < m_fUsableAlarmTime )
		{
			if( m_iAlarmElapsed != (RwInt32)m_fElapsed )
			{
				m_iAlarmElapsed = (RwInt32)m_fElapsed;

				Logic_PlayGUISound(GSD_GENERAL_CLOCK_SECOND);
			}
		}
	}


	// 필요한 이펙트가 있는지 검사
	if( m_eDisplayType == COUNTER_DISPLAY_1 )
	{
		if( BIT_FLAG_TEST(m_byDisplayFlag, dCOUNTER_DISPLAY_FLAG_BIGGER) )
		{
			if( m_fElapsed >= 300.f )
			{
				if( m_fSizeRate != dNUMBER_SIZE_1 )
					SetSizeRate(dNUMBER_SIZE_1);
			}
			else if( m_fElapsed >= 60.f )
			{
				if( m_fSizeRate != dNUMBER_SIZE_2 )
					SetSizeRate(dNUMBER_SIZE_2);
			}
			else
			{
				if( m_fSizeRate != dNUMBER_SIZE_3 )
					SetSizeRate(dNUMBER_SIZE_3);
			}
		}
	}
	

	// 현재 남은 시간을 설정
	if( m_fElapsed >= 60.f )
		m_MinuteNumberGui.SetNumber((RwInt64)m_fElapsed);
	else
		m_SecondNumberGui.SetNumber((RwInt64)m_fElapsed);
}

VOID CCounterGui::SetQuadrant(eCounterQuadrant eQuadrantType)
{
	if( eQuadrantType > COUNTER_QUADRANT_4 )
	{
		DBO_FAIL("Invalid quadrant type");
		return;
	}

	m_eQuadrant = eQuadrantType;

	switch(m_eDisplayType)
	{
		case COUNTER_DISPLAY_1:
		{
			RelocatePosition_1();
			break;
		}
		case COUNTER_DISPLAY_2:
		{
			RelocatePosition_2();
			break;
		}
		default:
		{
			DBO_FAIL("Invalid display type");
			return;
		}
	}
}

VOID CCounterGui::SetSizeRate(RwReal fRate)
{
	if( false == BIT_FLAG_TEST(m_byDisplayFlag, dCOUNTER_DISPLAY_FLAG_BIGGER) )
		return;

	m_fSizeRate = fRate;
	RelocatePosition_1();
}

VOID CCounterGui::RelocatePosition_1()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iNumX, iNumY;

	if( m_fSizeRate == dNUMBER_SIZE_1 )
	{
		iNumX = 16;
		iNumY = 6;
	}
	else if( m_fSizeRate == dNUMBER_SIZE_2 )
	{
		iNumX = 22;
		iNumY = 8;
	}
	else if( m_fSizeRate == dNUMBER_SIZE_3 )
	{
		iNumX = 33;
		iNumY = 12;
	}
	else
	{
		DBO_FAIL("Wrong number size rate : " << m_fSizeRate);
		return;
	}

	m_srfNumberBack.SetSize((RwInt32)((RwReal)m_OriginalNumberBackGuiSize.x*m_fSizeRate),
							(RwInt32)((RwReal)m_OriginalNumberBackGuiSize.y*m_fSizeRate));
	
	m_MinuteNumberGui.SetRate(m_fSizeRate);
	m_SecondNumberGui.SetRate(m_fSizeRate);


	switch(m_eQuadrant)
	{
		case COUNTER_QUADRANT_1:
		{
			RwInt32 iHeight = m_MinuteNumberGui.GetHeight();

			m_MinuteNumberGui.SetPosition(iNumX, iNumY - iHeight);
			m_SecondNumberGui.SetPosition(iNumX, iNumY - iHeight);
			m_srfNumberBack.SetPositionfromParent(0, -iHeight);
			m_srfNumberBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}
		case COUNTER_QUADRANT_2:
		{
			RwInt32 iWidth = m_MinuteNumberGui.GetWidth();
			RwInt32 iHeight = m_MinuteNumberGui.GetHeight();

			m_MinuteNumberGui.SetPosition(iNumX - iWidth, iNumY - iHeight);
			m_SecondNumberGui.SetPosition(iNumX - iWidth, iNumY - iHeight);
			m_srfNumberBack.SetPositionfromParent(-iWidth, -iHeight);
			m_srfNumberBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}
		case COUNTER_QUADRANT_3:
		{
			RwInt32 iWidth = m_MinuteNumberGui.GetWidth();

			m_MinuteNumberGui.SetPosition(iNumX - iWidth, iNumY);
			m_SecondNumberGui.SetPosition(iNumX - iWidth, iNumY);
			m_srfNumberBack.SetPositionfromParent(-iWidth, 0);
			m_srfNumberBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}
		case COUNTER_QUADRANT_4:
		{
			m_MinuteNumberGui.SetPosition(iNumX, iNumY);
			m_SecondNumberGui.SetPosition(iNumX, iNumY);
			m_srfNumberBack.SetPositionfromParent(0, 0);
			m_srfNumberBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}
	}
}

VOID CCounterGui::RelocatePosition_2()
{
	RwInt32 iX = 60;
	RwInt32 iY = 18;

	switch(m_eQuadrant)
	{
		case COUNTER_QUADRANT_1:
		{
			CRectangle& rtFlash =m_pflashBackground->GetFrameResolution();
			RwInt32 iHeight = rtFlash.GetHeight();

			m_MinuteNumberGui.SetPosition(iX, iY - iHeight);
			m_SecondNumberGui.SetPosition(iX, iY - iHeight);
			m_pflashBackground->SetPosition(0, -iHeight);
			break;
		}
		case COUNTER_QUADRANT_2:
		{
			CRectangle& rtFlash =m_pflashBackground->GetFrameResolution();
			RwInt32 iWidth = rtFlash.GetWidth();
			RwInt32 iHeight = rtFlash.GetHeight();

			m_MinuteNumberGui.SetPosition(iX - iWidth, iY - iHeight);
			m_SecondNumberGui.SetPosition(iX - iWidth, iY - iHeight);
			m_pflashBackground->SetPosition(-iWidth, -iHeight);
			break;
		}
		case COUNTER_QUADRANT_3:
		{
			CRectangle& rtFlash =m_pflashBackground->GetFrameResolution();
			RwInt32 iWidth = rtFlash.GetWidth();

			m_MinuteNumberGui.SetPosition(iX - iWidth, iY);
			m_SecondNumberGui.SetPosition(iX - iWidth, iY);
			m_pflashBackground->SetPosition(-iWidth, 0);
			break;
		}
		case COUNTER_QUADRANT_4:
		{
			m_MinuteNumberGui.SetPosition(iX, iY);
			m_SecondNumberGui.SetPosition(iX, iY);
			m_pflashBackground->SetPosition(0, 0);
			break;
		}
	}
}

VOID CCounterGui::LoadSurface_1()
{	
	LoadNumberSurface();

	m_srfNumberBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfNumberBack" ));	
	m_OriginalNumberBackGuiSize.x = m_srfNumberBack.GetWidth();
	m_OriginalNumberBackGuiSize.y = m_srfNumberBack.GetHeight();
}

VOID CCounterGui::LoadSurface_2()
{
	LoadNumberSurface();

	m_pflashBackground = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());

	m_slotPaintDisplay_2 = m_pflashBackground->SigPaint().Connect(this, &CCounterGui::OnPaintDisplay_2);
	m_slotFrameCallbackDisplay2 = m_pflashBackground->SigFSCallBack().Connect(this, &CCounterGui::OnFrameCallback_Display2);

	m_pflashBackground->Load("TMQ_result_limit_time.swf");
	m_pflashBackground->SetFrameCallback(TRUE);

	m_pflashBackground->PlayMovie(TRUE);

	CRectangle rtFlash = m_pflashBackground->GetFrameResolution();
	m_pflashBackground->SetSize(rtFlash.GetWidth(), rtFlash.GetHeight());
}

VOID CCounterGui::LoadNumberSurface()
{
	m_MinuteNumberGui.Create(ND_CENTER);
	m_MinuteNumberGui.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_0" ));
	m_MinuteNumberGui.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_1" ));
	m_MinuteNumberGui.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_2" ));
	m_MinuteNumberGui.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_3" ));
	m_MinuteNumberGui.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_4" ));
	m_MinuteNumberGui.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_5" ));
	m_MinuteNumberGui.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_6" ));
	m_MinuteNumberGui.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_7" ));
	m_MinuteNumberGui.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_8" ));
	m_MinuteNumberGui.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_9" ));
	m_MinuteNumberGui.SetPosition(34, 8);
	m_MinuteNumberGui.SetNumGap(-5);

	m_SecondNumberGui.Create(ND_CENTER);
	m_SecondNumberGui.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_0" ));
	m_SecondNumberGui.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_1" ));
	m_SecondNumberGui.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_2" ));
	m_SecondNumberGui.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_3" ));
	m_SecondNumberGui.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_4" ));
	m_SecondNumberGui.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_5" ));
	m_SecondNumberGui.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_6" ));
	m_SecondNumberGui.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_7" ));
	m_SecondNumberGui.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_8" ));
	m_SecondNumberGui.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSecondNumber_9" ));	
	m_SecondNumberGui.SetNumGap(-5);
}

VOID CCounterGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	switch(m_eDisplayType)
	{
		case COUNTER_DISPLAY_1:
		{
			RelocatePosition_1();
			break;
		}
		case COUNTER_DISPLAY_2:
		{
			RelocatePosition_2();
			break;
		}
		default:
		{
			DBO_FAIL("Invalid display type");
			return;
		}
	}

	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_MinuteNumberGui.SetParentPos(rtScreen.left, rtScreen.top);
	m_SecondNumberGui.SetParentPos(rtScreen.left, rtScreen.top);

	if( m_eDisplayType == COUNTER_DISPLAY_1 )
		m_srfNumberBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CCounterGui::OnPaintDisplay_1()
{
	m_srfNumberBack.Render();

	if( m_fElapsed >= 60.f )
		m_MinuteNumberGui.Render();
	else
		m_SecondNumberGui.Render();
}

VOID CCounterGui::OnPaintDisplay_2()
{
	if( !m_bRender )
		return;

	if( m_fElapsed >= 60.f )
		m_MinuteNumberGui.Render();
	else
		m_SecondNumberGui.Render();
}

VOID CCounterGui::OnFrameCallback_Display2(const char* pcParam, const char* pcParam2)
{
	m_bRender = TRUE;
}