#include "gui_precomp.h"
#include "GuiUtil.h"
#include "gui_particlepath.h"
#include "gui_surfacemanager.h"
#include "NtlCoreUtil.h"
#include "NtlMath.h"

START_GUI

CParticlePath::CParticlePath()
{
	memset( &m_sPathInfo, 0, sizeof( SPathInfo ) );
	m_pCtrlPoint = NULL;
	m_pCurvePoint = NULL;
	m_nCurveCount = 0;
}

CParticlePath::~CParticlePath()
{
	Destroy();
}

void CParticlePath::Destroy()
{
	memset( &m_sPathInfo, 0, sizeof( SPathInfo ) );

	NTL_DELETE( m_pCtrlPoint );
	NTL_DELETE( m_pCurvePoint );
	
	m_nCurveCount = 0;
}

void CParticlePath::SetPath( SPathInfo& sInfo, SCtrlPoint* pCtrlPoint )
{
	Destroy();

	memcpy( &m_sPathInfo, &sInfo, sizeof( SPathInfo ) );

	m_pCtrlPoint = NTL_NEW SCtrlPoint[m_sPathInfo.nCtrlNum];
	memcpy( m_pCtrlPoint, pCtrlPoint, sizeof( SCtrlPoint ) * m_sPathInfo.nCtrlNum );

	// Build Spline ( or Line )
	switch( m_sPathInfo.nType )
	{
	case PATH_LINE:
		BuildCurveCount();
		BuildLinePoint();
		BuildCurveTime();
		break;
	case PATH_CATMULLROM:
		BuildCurveCount();
		BuildCurvePoint();
		BuildCurveTime();
		break;
	}
}

/**
* \brief 현재 time에 알맞는 위치를 가지고 온다.
* \param fTime		(float) 현재의 time
* \param v2dPos		(RwV2d&) 위치가 출력될 벡터 포인터
* \param bLoop		(BOOL) 루프 여부
*/
void CParticlePath::FrameMove( float fTime, RwV2d* pV2dPos, BOOL bLoop )
{
	if( bLoop )
	{
		fTime = fmod( fTime, m_pCurvePoint[m_nCurveCount - 1].fTime );
	}

	if( fTime >= m_pCurvePoint[m_nCurveCount-1].fTime )
	{
		(*pV2dPos) = m_pCurvePoint[m_nCurveCount-1].vPos;
	}
	else
	{
		int nPrevPos, nNextPos;
		for( int i = 0; i < m_nCurveCount; ++i )
		{
			if( fTime < m_pCurvePoint[i].fTime )
			{
				nPrevPos = ( i > 0 ) ? i - 1 : 0;
				nNextPos = i;
				break;
			}
		}

		if( m_pCurvePoint[nPrevPos].fTime == m_pCurvePoint[nNextPos].fTime )
		{
			(*pV2dPos) = m_pCurvePoint[nPrevPos].vPos;
		}
		else
		{
			// 현재 타임이 nPrevPos와 nNextPos 에서 가지는 보간 비율을 계산
			float fLerpValue = (float)(fTime - m_pCurvePoint[nPrevPos].fTime)  / (float)(m_pCurvePoint[nNextPos].fTime - m_pCurvePoint[nPrevPos].fTime);
			
			RwV2dLineInterpolation( pV2dPos, &m_pCurvePoint[nPrevPos].vPos, &m_pCurvePoint[nNextPos].vPos, fLerpValue, 1.0f );
		}
	}
}

void CParticlePath::BuildCurveCount()
{
	m_nCurveCount = (m_sPathInfo.nCtrlNum-1) * m_sPathInfo.nCurveSubNum + 1;

	m_pCurvePoint = NTL_NEW SCurvePoint[m_nCurveCount];
	memset( m_pCurvePoint, 0, sizeof( SCurvePoint ) * m_nCurveCount );
}

void CParticlePath::BuildCurveTime()
{
	RwInt32	nIdx = 0;
	RwInt32 i0, i1;

	for (RwInt32 i = 0; i < m_sPathInfo.nCtrlNum; ++i) 
	{
		// For each subdivision
		for(RwInt32 j = 0; j < m_sPathInfo.nCurveSubNum; ++j) 
		{
			if (nIdx >= m_nCurveCount)
			{
				continue;
			}

			i0 = i;
			i1 = i+1;

			float fLerp = (float)j / (float)m_sPathInfo.nCurveSubNum;

			m_pCurvePoint[nIdx].fTime = GetResultLineInterpolation( fLerp, 1.0f, m_pCtrlPoint[i0].fTime,  
				m_pCtrlPoint[i1].fTime );

			nIdx++;
		}
	}
}

void CParticlePath::BuildLinCount()
{
	m_nCurveCount = (m_sPathInfo.nCtrlNum-1) * m_sPathInfo.nCurveSubNum + 1;

	m_pCurvePoint = NTL_NEW SCurvePoint[m_nCurveCount];
	memset( m_pCurvePoint, 0, sizeof( SCurvePoint ) * m_nCurveCount );
}

void CParticlePath::BuildLinePoint()
{
	int nIdx = 0;
	int i0, i1;

	for (int i = 0; i < m_sPathInfo.nCtrlNum; ++i) 
	{
		// For each subdivision
		for(int j = 0; j < m_sPathInfo.nCurveSubNum; ++j) 
		{
			if (nIdx >= m_nCurveCount)
			{
				continue;
			}

			RwReal fWeightValue = 0.5f;
			if(m_sPathInfo.nCurveSubNum > 0)
			{
				fWeightValue = (RwReal)j / m_sPathInfo.nCurveSubNum;
			}

			i0 = i;
			i1 = (i+1 >= m_sPathInfo.nCtrlNum) ? m_sPathInfo.nCtrlNum-1 : i+1;

			if(i0 < 0) i0 = 0;
			if(i1 < 0) i1 = 0;
			
			gui::RwV2dLineInterpolation( &m_pCurvePoint[nIdx].vPos, &m_pCtrlPoint[i0].vPos, &m_pCtrlPoint[i1].vPos, fWeightValue, 1.0f );

			++ nIdx;
		}
	}
}

void CParticlePath::BuildCurvePoint()
{
	int	nIdx = 0;
	int i0, i1, i2, i3;

	for (int i = -1; i < m_sPathInfo.nCtrlNum; ++i) 
	{
		// For each subdivision
		for(int j = 0; j < m_sPathInfo.nCurveSubNum; ++j) 
		{
			if (nIdx >= m_nCurveCount)
			{
				continue;
			}

			RwReal fWeightValue = 0.5f;
			if(m_sPathInfo.nCurveSubNum > 0)
				fWeightValue = (RwReal)j / m_sPathInfo.nCurveSubNum;

			i0 = i;
			i1 = (i+1 >= m_sPathInfo.nCtrlNum) ? m_sPathInfo.nCtrlNum-1 : i+1;
			i2 = (i+2 >= m_sPathInfo.nCtrlNum) ? m_sPathInfo.nCtrlNum-1 : i+2;
			i3 = (i+3 >= m_sPathInfo.nCtrlNum) ? m_sPathInfo.nCtrlNum-1 : i+3;

			if(i0 < 0) i0 = 0;
			if(i1 < 0) i1 = 0;
			if(i2 < 0) i2 = 0;
			if(i3 < 0) i3 = 0;

			D3DXVec3CatmullRom( (D3DXVECTOR3*)&m_pCurvePoint[nIdx].vPos,  
				(D3DXVECTOR3*)&m_pCtrlPoint[i0].vPos,
				(D3DXVECTOR3*)&m_pCtrlPoint[i1].vPos,
				(D3DXVECTOR3*)&m_pCtrlPoint[i2].vPos,
				(D3DXVECTOR3*)&m_pCtrlPoint[i3].vPos,
				fWeightValue );		

			++ nIdx;
		}
	}
}

int CParticlePath::GetCurveCount()
{
	return m_nCurveCount;
}

SCurvePoint* CParticlePath::GetCurvePoint()
{
	if( m_pCurvePoint )
		return m_pCurvePoint;
	else
		return NULL;
}



END_GUI