#include "precomp_ntlpresentation.h"
#include "PathList.h"
#include "NtlDebug.h"
#include "PathPoint.h"
#include <algorithm>
#include "ntlworldcommon.h"
#include "C2DAABB.h"
#include "NtlMath.h"


CPathList::CPathList(RwInt32 _UID, RwReal _fRadius, RwChar* _pMemo)
{
	m_UID			= _UID;
	m_fRadius		= _fRadius;
	m_pAABB			= NULL;
	m_Visibility	= TRUE;
	m_LineColor		= 0xffffffff;
	strcpy_s(m_Memo, sizeof(RwChar) * 255, _pMemo);
}

CPathList::~CPathList(void)
{
	tLIST_PP_ITER The = m_lstPathPoint.begin();
	while(The != m_lstPathPoint.end())
	{
		CPathPoint* pDel = (*The);
		NTL_DELETE(pDel);

		The = m_lstPathPoint.erase(The);
	}
	m_lstPathPoint.clear();

	NTL_DELETE(m_pAABB);
}

RwBool CPathList::CompareUID(RwInt32 _UID)
{
	return (m_UID == _UID);
}

VOID CPathList::Insert(CPathPoint* _pPathPoint, RwBool _PushBack/* = TRUE*/)
{
	if(_PushBack)
	{
		m_lstPathPoint.push_back(_pPathPoint);
	}
	else
	{
		m_lstPathPoint.push_front(_pPathPoint);
	}

	// recalculate AABB
	if(!m_pAABB)
	{
		m_pAABB = NTL_NEW C2DAABB;
	}

	RwV2d NewPos;
	NewPos.x = _pPathPoint->GetPosition().x;
	NewPos.y = _pPathPoint->GetPosition().z;
	m_pAABB->SetNewMinMax(NewPos);
}

VOID CPathList::InsertN(CPathPoint* _pPathPoint, CPathPoint* _pBasePathPoint, RwBool _PushBack)
{
	tLIST_PP_ITER The = m_lstPathPoint.begin();
	while(The != m_lstPathPoint.end())
	{
		if ((*The) == _pBasePathPoint)
		{
			if (!_PushBack)
			{
				--The;
			}
			m_lstPathPoint.insert(The, _pPathPoint);
		}

		++The;
	}

	// recalculate AABB
	if(!m_pAABB)
	{
		m_pAABB = NTL_NEW C2DAABB;
	}

	RwV2d NewPos;
	NewPos.x = _pPathPoint->GetPosition().x;
	NewPos.y = _pPathPoint->GetPosition().z;
	m_pAABB->SetNewMinMax(NewPos);
}

VOID CPathList::Delete(CPathPoint* _pPathPoint)
{
	tLIST_PP_ITER The = std::find(m_lstPathPoint.begin(), m_lstPathPoint.end(), _pPathPoint);

	if(The != m_lstPathPoint.end())
	{
		CPathPoint* pDel = *The;
		NTL_DELETE(pDel);

		m_lstPathPoint.erase(The);
	}

	if(!m_lstPathPoint.size())
	{
		m_lstPathPoint.clear();
	}

	RefreshAABB();
}

VOID CPathList::RefreshAABB()
{
	RwV2d			NewPos;
	tLIST_PP_ITER	The = m_lstPathPoint.begin();

	if(m_lstPathPoint.size())
	{
		if(m_pAABB)
		{
			NTL_DELETE(m_pAABB);			
		}

        m_pAABB = NTL_NEW C2DAABB;        

		while(The != m_lstPathPoint.end())
		{
			NewPos.x = (*The)->GetPosition().x;
			NewPos.y = (*The)->GetPosition().z;
			m_pAABB->SetNewMinMax(NewPos);	

			++The;
		}
	}
}

VOID CPathList::Move(CPathPoint* _pPathPoint)
{

}

VOID CPathList::Cull(C2DAABB* _pCurFieldAABB)
{
	RwV2d			Pos;
	tLIST_PP_ITER	The = m_lstPathPoint.begin();

	if(_pCurFieldAABB->CollisionTest(*m_pAABB))
	{
		m_Visibility = TRUE;

		// cull the proper path points
		while(The != m_lstPathPoint.end())
		{
			Pos.x  = (*The)->GetPosition().x;
			Pos.y  = (*The)->GetPosition().z;

			if(_pCurFieldAABB->CollisionTest(Pos))
			{
				(*The)->SetVisibility(TRUE);
			}
			else
			{
				(*The)->SetVisibility(FALSE);
			}		

			++The;
		}
	}
	else
	{
		m_Visibility = FALSE;

		// cull all path points
		while(The != m_lstPathPoint.end())
		{
			(*The)->SetVisibility(FALSE);
			++The;
		}
	}
}

CPathPoint* CPathList::Pick(RwInt32 ScreenPosX, RwInt32 ScreenPosY)
{
	tLIST_PP_ITER	The			= m_lstPathPoint.begin();
	CPathPoint*		pPathPoint	= NULL;

	while(The != m_lstPathPoint.end())
	{
		if((*The)->GetVisibility())
		{
			RwSphere Sphere;
			Sphere.center = (*The)->GetPosition();
			Sphere.radius = 2.0f;

			RwReal fDistance;
			RwBool bResult = CNtlMath::LineSphereCollisionTestPixelRay(CNtlPLGlobal::m_RwCamera, &Sphere, ScreenPosX, ScreenPosY, 1024.0f, fDistance);
			if(bResult)
			{
				pPathPoint = (*The);
				break;
			}
			else
			{
				pPathPoint = NULL;
			}
		}

		++The;
	}

	return pPathPoint;
}

VOID CPathList::DeleteDummyEntities()
{
	tLIST_PP_ITER The = m_lstPathPoint.begin();
	while(The != m_lstPathPoint.end())
	{
		CPathPoint* pCurPP = static_cast<CPathPoint*>(*The);
		pCurPP->SetVisibility(FALSE);

		++The;
	}
}

VOID CPathList::Save(FILE* _pFile, RwBool OnlyForTool)
{
	TBLIDX	WorldTblIdx		= static_cast<TBLIDX>(m_UID);
	float	fRadius			= static_cast<float>(m_fRadius);
	BYTE	ByPointCount	= static_cast<BYTE>(m_lstPathPoint.size());

	fwrite(&WorldTblIdx, sizeof(TBLIDX), 1, _pFile);
	fwrite(&fRadius, sizeof(float), 1, _pFile);
	fwrite(&ByPointCount, sizeof(BYTE), 1, _pFile);

	if(OnlyForTool)
	{
		fwrite(m_Memo, sizeof(RwChar) * 255, 1, _pFile);
	}

	RwReal			PrevDist	= 0.0f;
	tLIST_PP_ITER	The			= m_lstPathPoint.begin();
	tLIST_PP_ITER	TheNext;
	RwV3d			ThePos, TheNextPos;

	while(The != m_lstPathPoint.end())
	{
		static_cast<CPathPoint*>(*The)->Save(_pFile, PrevDist);

		TheNext = The;
		++TheNext;

		if(TheNext != m_lstPathPoint.end())
		{
			ThePos		= (*The)->GetPosition();
			TheNextPos	= (*TheNext)->GetPosition();
			PrevDist	= ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&(TheNextPos - ThePos)));
		}

		++The;
	}
}

VOID CPathList::Load(FILE* _pFile, BYTE _ByPointCount)
{
	RwV3d PosDummy = {-999.0f, -999.0f, -999.0f};

	for(BYTE i = 0; i < _ByPointCount; ++i)
	{
		CPathPoint* pPathPoint = NTL_NEW CPathPoint(PosDummy);
		pPathPoint->Load(_pFile);
		Insert(pPathPoint, TRUE);
	}
}

VOID CPathList::Export(FILE* _pFile)	
{
	fprintf(_pFile, "%d\t%f\t%s\n", m_UID, m_fRadius, m_Memo);
}

VOID CPathList::Render()
{
	CPathPoint* pSPathPoint;
	CPathPoint* pEPathPoint;
	sLINE3D	PathLine[2];

	tLIST_PP_ITER The = m_lstPathPoint.begin();
	tLIST_PP_ITER Tmp;
	while(The != m_lstPathPoint.end())
	{
		Tmp = The;
		++Tmp;

		pSPathPoint = NULL;
		pEPathPoint = NULL;

		pSPathPoint = (*The);

		if(Tmp != m_lstPathPoint.end())
		{
			pEPathPoint = (*Tmp);
		}		

		if(pSPathPoint && pEPathPoint)
		{
			if(pSPathPoint->GetVisibility() && pEPathPoint->GetVisibility())
			{
				PathLine[0].Col = m_LineColor;
				PathLine[1].Col = m_LineColor;
				PathLine[0].Pos.x = pSPathPoint->GetPosition().x;
				PathLine[0].Pos.y = pSPathPoint->GetPosition().y + 1.0f;
				PathLine[0].Pos.z = pSPathPoint->GetPosition().z;
				PathLine[1].Pos.x = pEPathPoint->GetPosition().x;
				PathLine[1].Pos.y = pEPathPoint->GetPosition().y + 1.0f;
				PathLine[1].Pos.z = pEPathPoint->GetPosition().z;

				RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, PathLine, sizeof(sLINE3D));
			}
		}

		++The;
	}
}

/*
s << (char)PLGUI_SERIALIZE_END;
return TRUE;
}

const CRectangle& rt = m_pThis->GetPosition();

s << (char)PLGUI_SERIALIZE_POS_X;
s << rt.left;
s << (char)PLGUI_SERIALIZE_POS_Y;

if (serializer.GetDataSize() < GetDataSize())
{
return false;
}

serializer.Out(GetDataAddress(), GetDataSize());

// file save
NtlFileAttrReadOnlyRelease((RwChar*)pFileName);

FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
if(fopen_s(&fp, pFileName, "wb") != 0)
return FALSE;
#else
fp = fopen(g_chLogPath, "wb");
if(fp == NULL)
return FALSE;
#endif

fwrite(s.GetData(), s.GetDataSize(), 1, fp);

fclose(fp);
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMergePathList::CMergePathList(RwInt32 _UID, RwReal _fRadius, RwChar* _pMemo)
: CPathList(_UID, _fRadius, _pMemo)
{
}

CMergePathList::~CMergePathList(void)
{
}