#include "StdAfx.h"
#include "ShoreLinePGenerator.h"
#include "ShoreLineCP.h"
#include "ntlworldcommon.h"
#include "NtlPLObject.h"
#include "NtlWorldDI.h"
#include "NtlWorldBrush.h"
#include "NtlWorldBrushLine.h"
#include "NtlWorldFieldManager.h"
#include "NtlWe.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLShoreLineProp.h"
#include "NtlPLRenderState.h"
#include "NtlPLShoreLine.h"


CShoreLinePGenerator::CShoreLinePGenerator(RwV3d& _SSnapPos, RwV3d& _ESnapPos)
{
	CreateShoreLineCP(_SSnapPos, _ESnapPos);
	m_IsLeft = TRUE;
	m_pNtlWorldBrushLine = NULL;
}

CShoreLinePGenerator::CShoreLinePGenerator(RwV3d& StartPos)
{
	CreateShoreLineCP(StartPos);
	m_IsLeft = TRUE;
	m_pNtlWorldBrushLine = NULL;
}

CShoreLinePGenerator::~CShoreLinePGenerator(void)	
{
	// control points
	std::list<CShoreLineCP*>::iterator The = m_listShoreLineCP.begin();
	while(The != m_listShoreLineCP.end())
	{
		CShoreLineCP* pDel = (*The);
		NTL_DELETE(pDel);
		++The;
	}

	m_listShoreLineCP.clear();

	// brush
	if(m_pNtlWorldBrushLine)
	{
		NTL_DELETE(m_pNtlWorldBrushLine);
	}
}

VOID CShoreLinePGenerator::CreateShoreLineCP(RwV3d& _SSnap, RwV3d& _ESnap)
{
	CShoreLineCP* pSCP = NTL_NEW CShoreLineCP(_SSnap);
	CShoreLineCP* pECP = NTL_NEW CShoreLineCP(_SSnap);

	RwV3d Nml = _ESnap - _SSnap;
	RwV3dNormalize(&Nml, &Nml);

	pSCP->m_Nml			= Nml;
	pSCP->m_NmlFixed	= TRUE;

	m_listShoreLineCP.push_back(pSCP);
	m_listShoreLineCP.push_back(pECP);
}

VOID CShoreLinePGenerator::CreateShoreLineCP(RwV3d& Pos)
{
	CShoreLineCP* pSCP = NTL_NEW CShoreLineCP(Pos);
	CShoreLineCP* pECP = NTL_NEW CShoreLineCP(Pos);
	m_listShoreLineCP.push_back(pSCP);
	m_listShoreLineCP.push_back(pECP);
}

RwBool CShoreLinePGenerator::GetSnapVert(RwV3d& _CurPos, RwV3d& SSnapVert, RwV3d& ESSnapVert)
{
	RwReal					SortDist	= RwRealMAXVAL;
	RwBool					PickOne		= FALSE;
	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	RwInt32					CurFieldIdx = pMgr->GetFieldIdx(_CurPos);
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	int						l, m;
	int						SectorIdx;
	RwV3d					SectorSPos;
	RwV3d					SPos;
	CNtlWorldSector*		pNtlWorldSector;
	vector<RwInt32>			vecIdxFields;
	RwReal					CurMinDist = RwRealMAXVAL;

	pMgr->GetNeighborFields(CurFieldIdx, vecIdxFields);
	vecIdxFields.push_back(CurFieldIdx);


	// get the closest vert. for starting point
	for(RwUInt32 i = 0; i < vecIdxFields.size(); ++i)
	{
		if(vecIdxFields[i] == -1)
		{
			continue;
		}

		SPos = pFields[vecIdxFields[i]].GetSPos();

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x	= (RwReal)m;
				SectorSPos.z	= (RwReal)l;
				SectorIdx		= pMgr->GetSectorIdx(SectorSPos);
				pNtlWorldSector = &pMgr->m_pSectors[SectorIdx];

				if(!pNtlWorldSector->m_pNtlPLEntityShoreLine)
				{
					continue;
				}
				else
				{
					RwIm3DVertex*	pVB		= static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine)->GetVB();
					RwInt32			VNum	= static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine)->GetVN();

					for(RwInt32 j = 0; j < VNum; j ++)
					{
						RwReal CurDist = CNtlMath::MathRwV3dSquaredLength(&(_CurPos - pVB[j].objVertex));

						if(CurDist < CurMinDist)
						{
							CurMinDist	= CurDist;
							SSnapVert	= pVB[j].objVertex;
						}
					}
				}
			}
		}
	}

	if(CurMinDist > (RwRealMAXVAL - 1))
	{
		return FALSE;
	}

	CurMinDist = RwRealMAXVAL;

	// get the next closest vert. for ending point
	for(RwUInt32 i = 0; i < vecIdxFields.size(); ++i)
	{
		if(vecIdxFields[i] == -1)
		{
			continue;
		}

		SPos = pFields[vecIdxFields[i]].GetSPos();

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x	= (RwReal)m;
				SectorSPos.z	= (RwReal)l;
				SectorIdx		= pMgr->GetSectorIdx(SectorSPos);
				pNtlWorldSector = &pMgr->m_pSectors[SectorIdx];

				if(!pNtlWorldSector->m_pNtlPLEntityShoreLine)
				{
					continue;
				}
				else
				{
					RwIm3DVertex*	pVB		= static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine)->GetVB();
					RwInt32			VNum	= static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine)->GetVN();

					for(RwInt32 j = 0; j < VNum; j ++)
					{
						RwReal CurDist = CNtlMath::MathRwV3dSquaredLength(&(_CurPos - pVB[j].objVertex));

						if(	static_cast<RwInt32>(pVB[j].objVertex.x) == static_cast<RwInt32>(SSnapVert.x) &&
							static_cast<RwInt32>(pVB[j].objVertex.y) == static_cast<RwInt32>(SSnapVert.y) &&
							static_cast<RwInt32>(pVB[j].objVertex.z) == static_cast<RwInt32>(SSnapVert.z))
						{
							continue;
						}

						if(CurDist < CurMinDist)
						{
							CurMinDist	= CurDist;
							ESSnapVert	= pVB[j].objVertex;
						}
					}
				}
			}
		}
	}

	if(CurMinDist > (RwRealMAXVAL - 1))
	{
		return FALSE;
	}

	vecIdxFields.clear();

	return TRUE;
}

VOID CShoreLinePGenerator::SetHeight(RwReal _Value, RwBool _AddFlag/* = TRUE*/)
{
	if(m_listShoreLineCP.size())
	{
		std::list<CShoreLineCP*>::iterator The = m_listShoreLineCP.begin();
		while(The != m_listShoreLineCP.end())
		{
			RwV3d Pos = static_cast<CShoreLineCP*>(*The)->GetPosition();

			if(_AddFlag)
			{
				Pos.y += _Value;
			}
			else
			{
				Pos.y = _Value;
			}

			static_cast<CShoreLineCP*>(*The)->SetPosition(Pos);

			++The;
		}
	}
}

VOID CShoreLinePGenerator::Update()
{
	// height
	if(m_listShoreLineCP.size())
	{
		if(dKEY(DIK_PGUP))
		{
			SetHeight(dHEIGHT_MOVE_LEN);
		}
		else if(dKEY(DIK_PGDN))
		{
			SetHeight(-dHEIGHT_MOVE_LEN);
		}
	}

	// append
	if(m_listShoreLineCP.size())
	{
		std::list<CShoreLineCP*>::iterator TheEND = m_listShoreLineCP.end(); --TheEND;
		std::list<CShoreLineCP*>::iterator TheBEG = m_listShoreLineCP.begin();

		RwV3d PrevPos = static_cast<CShoreLineCP*>(*TheBEG)->m_pCPUIObj->GetPosition();
		RwV3d CurrPos = PrevPos;

		CurrPos.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
		CurrPos.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

		if(dGET_MOUSE()->GetLBOnce())
		{
			if(dKEY(DIK_LSHIFT))
			{
				RwV3d SSnapVert, ESSnapVert;
				
				if(GetSnapVert(CurrPos, SSnapVert, ESSnapVert))
				{
					RwV3d Nml = ESSnapVert - SSnapVert;
					RwV3dNormalize(&Nml, &Nml);
					
					static_cast<CShoreLineCP*>(*TheEND)->m_Nml		= Nml;
					static_cast<CShoreLineCP*>(*TheEND)->m_NmlFixed	= TRUE;
					static_cast<CShoreLineCP*>(*TheEND)->m_pCPUIObj->SetPosition(&SSnapVert);

					CShoreLineCP* pNew_SLCP = NTL_NEW CShoreLineCP(CurrPos);
					m_listShoreLineCP.push_back(pNew_SLCP);
				}
				else
				{
					AfxMessageBox("There're no shoreline client meshes for snapping around here.", MB_OK);
				}
			}
			else
			{
				CShoreLineCP* pNew_SLCP = NTL_NEW CShoreLineCP(CurrPos);
				m_listShoreLineCP.push_back(pNew_SLCP);
				static_cast<CShoreLineCP*>(*TheEND)->m_pCPUIObj->SetPosition(&CurrPos);
			}
		}
		else
		{
			static_cast<CShoreLineCP*>(*TheEND)->m_pCPUIObj->SetPosition(&CurrPos);
		}
	}

	// erase one at a time
	if(m_listShoreLineCP.size() > 2)
	{
		if(dOKEY(DIK_SPACE))
		{
			std::list<CShoreLineCP*>::iterator The = m_listShoreLineCP.end();
			--The;

			CShoreLineCP* pDel = (*The);
			NTL_DELETE(pDel);

			m_listShoreLineCP.erase(The);
		}
	}

	// change direction
	if(dOKEY(DIK_TAB))
	{
		m_IsLeft = !m_IsLeft;
	}

	// recalculate normal for the rest control points
	std::list<CShoreLineCP*>::iterator The = m_listShoreLineCP.begin();
	std::list<CShoreLineCP*>::iterator SThe, EThe;
	while(The != m_listShoreLineCP.end())
	{
		SThe = The;
		EThe = The;
		++EThe;

		if(EThe == m_listShoreLineCP.end())
		{
			break;
		}
		else
		{
			RwV3d SPos = static_cast<CShoreLineCP*>(*SThe)->m_pCPUIObj->GetPosition();
			RwV3d EPos = static_cast<CShoreLineCP*>(*EThe)->m_pCPUIObj->GetPosition();
			RwV3d SFront = EPos - SPos;	RwV3dNormalize(&SFront, &SFront);
			RwV3d EFront = SPos - EPos;	RwV3dNormalize(&SFront, &SFront);
			RwV3d SLeft;
			RwV3d ELeft;

			RwV3dCrossProduct(&SLeft, &CNtlPLGlobal::m_vYAxisV3, &SFront);
			RwV3dCrossProduct(&ELeft, &CNtlPLGlobal::m_vYAxisV3, &EFront);
			RwV3dNormalize(&SLeft, &SLeft);
			RwV3dNormalize(&ELeft, &ELeft);

			if(m_IsLeft)
			{
				if(!static_cast<CShoreLineCP*>(*SThe)->m_NmlFixed)
				{
					static_cast<CShoreLineCP*>(*SThe)->m_Nml = SLeft;
				}
			}
			else
			{
				if(!static_cast<CShoreLineCP*>(*SThe)->m_NmlFixed)
				{
					static_cast<CShoreLineCP*>(*SThe)->m_Nml = SLeft * -1.0f;
				}
			}
		}

		++The;
	}
}

VOID CShoreLinePGenerator::RenderLine(RwReal _Line)
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// line
	std::list<CShoreLineCP*>::iterator The = m_listShoreLineCP.begin();
	std::list<CShoreLineCP*>::iterator SThe, EThe;
	while(The != m_listShoreLineCP.end())
	{
		SThe = The;
		EThe = The;
		++EThe;

		if(EThe == m_listShoreLineCP.end())
		{
			break;
		}
		else
		{
			if(!m_pNtlWorldBrushLine)
			{
				m_pNtlWorldBrushLine = NTL_NEW CNtlWorldBrush;
				CNtlWorldBrushController *pController = NTL_NEW CNtlWB_Line<CNtlWorldFieldManager>(eNBT_LINE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
				m_pNtlWorldBrushLine->SetController(pController);
			}

			CNtlWB_Line<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Line<CNtlWorldFieldManager>*>(m_pNtlWorldBrushLine->GetController());

			RwV3d SPos = static_cast<CNtlPLObject*>(static_cast<CShoreLineCP*>(*SThe)->m_pCPUIObj)->GetPosition();
			RwV3d EPos = static_cast<CNtlPLObject*>(static_cast<CShoreLineCP*>(*EThe)->m_pCPUIObj)->GetPosition();

			pCurBrush->SetPosS(SPos);
			pCurBrush->SetPosE(EPos);
			pCurBrush->Render();
		}

		++The;
	}

	// normal
	The		= m_listShoreLineCP.begin();
	EThe	= m_listShoreLineCP.end(); --EThe;

	RwReal					Size	= 0.0f;
	CNtlPLShoreLineProp*	pSLProp	= static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));

	while(The != m_listShoreLineCP.end())
	{
		if(EThe == The)
		{
			break;
		}

		CShoreLineCP* pCP = (*The);

		if(!m_pNtlWorldBrushLine)
		{
			m_pNtlWorldBrushLine = NTL_NEW CNtlWorldBrush;
			CNtlWorldBrushController *pController = NTL_NEW CNtlWB_Line<CNtlWorldFieldManager>(eNBT_LINE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
			m_pNtlWorldBrushLine->SetController(pController);
		}

		CNtlWB_Line<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Line<CNtlWorldFieldManager>*>(m_pNtlWorldBrushLine->GetController());

		RwV3d SPos = pCP->m_pCPUIObj->GetPosition();
		RwV3d EPos = SPos;

		RwV3dIncrementScaled(&EPos, &pCP->m_Nml, _Line);

		pCurBrush->SetPosS(SPos);
		pCurBrush->SetPosE(EPos);
		pCurBrush->Render();

		++The;
	}

}

VOID CShoreLinePGenerator::RenderGeom(RwReal _Size, RwRGBA _RGBA)
{
	std::list<CShoreLineCP*>::iterator	The		= m_listShoreLineCP.begin();
	std::list<CShoreLineCP*>::iterator	TheLast = m_listShoreLineCP.end(); --TheLast;
	std::list<CShoreLineCP*>::iterator	SThe, EThe;

	while(The != m_listShoreLineCP.end())
	{
		SThe = The;
		EThe = The;
		++EThe;

		if(EThe == TheLast)
		{
			break;
		}


		CShoreLineCP* pCP = (*The);

		// define vertices and create geometries
		RwV3d	Pos0;
		RwV3d	Pos1; 
		RwV3d	Pos2; 
		RwV3d	Pos3; 

		Pos2 = static_cast<CShoreLineCP*>(*SThe)->m_pCPUIObj->GetPosition();
		Pos3 = Pos2;
		RwV3dIncrementScaled(&Pos3, &static_cast<CShoreLineCP*>(*SThe)->m_Nml, _Size);

		Pos0 = static_cast<CShoreLineCP*>(*EThe)->m_pCPUIObj->GetPosition();
		Pos1 = Pos0;
		RwV3dIncrementScaled(&Pos1, &static_cast<CShoreLineCP*>(*EThe)->m_Nml, _Size);

		//
		// In case of the right
		// 
		// 1----3
		// |  / |
		// | /  |
		// 0----2
		//
		// (1, 0, 3), (3, 0, 2)
		//
		//
		// In case of the left
		//
		// 0----2 
		// |  / |
		// | /  |
		// 1----3
		//
		// (0, 1, 2), (2, 1, 3)
		//

		if(m_IsLeft)
		{
			RwIm3DVertexSetU(&m_VB[0], 0.0f);		
			RwIm3DVertexSetV(&m_VB[0], 1.0f);
			RwIm3DVertexSetU(&m_VB[1], 0.0f);		
			RwIm3DVertexSetV(&m_VB[1], 0.0f);
			RwIm3DVertexSetU(&m_VB[2], 1.0f);		
			RwIm3DVertexSetV(&m_VB[2], 1.0f);
			RwIm3DVertexSetRGBA(&m_VB[0], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetRGBA(&m_VB[1], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetRGBA(&m_VB[2], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetPos(&m_VB[0], Pos0.x, Pos0.y, Pos0.z);
			RwIm3DVertexSetPos(&m_VB[1], Pos1.x, Pos1.y, Pos1.z);
			RwIm3DVertexSetPos(&m_VB[2], Pos2.x, Pos2.y, Pos2.z);

			RwIm3DVertexSetU(&m_VB[3], 1.0f);		
			RwIm3DVertexSetV(&m_VB[3], 1.0f);
			RwIm3DVertexSetU(&m_VB[4], 0.0f);		
			RwIm3DVertexSetV(&m_VB[4], 0.0f);
			RwIm3DVertexSetU(&m_VB[5], 1.0f);		
			RwIm3DVertexSetV(&m_VB[5], 0.0f);	
			RwIm3DVertexSetRGBA(&m_VB[3], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetRGBA(&m_VB[4], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetRGBA(&m_VB[5], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetPos(&m_VB[3], Pos2.x, Pos2.y, Pos2.z);
			RwIm3DVertexSetPos(&m_VB[4], Pos1.x, Pos1.y, Pos1.z);
			RwIm3DVertexSetPos(&m_VB[5], Pos3.x, Pos3.y, Pos3.z);
		}
		else
		{
			RwIm3DVertexSetU(&m_VB[0], 0.0f);		
			RwIm3DVertexSetV(&m_VB[0], 0.0f);
			RwIm3DVertexSetU(&m_VB[1], 0.0f);		
			RwIm3DVertexSetV(&m_VB[1], 1.0f);
			RwIm3DVertexSetU(&m_VB[2], 1.0f);		
			RwIm3DVertexSetV(&m_VB[2], 0.0f);
			RwIm3DVertexSetRGBA(&m_VB[0], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetRGBA(&m_VB[1], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetRGBA(&m_VB[2], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetPos(&m_VB[0], Pos1.x, Pos1.y, Pos1.z);
			RwIm3DVertexSetPos(&m_VB[1], Pos0.x, Pos0.y, Pos0.z);
			RwIm3DVertexSetPos(&m_VB[2], Pos3.x, Pos3.y, Pos3.z);

			RwIm3DVertexSetU(&m_VB[3], 1.0f);		
			RwIm3DVertexSetV(&m_VB[3], 0.0f);
			RwIm3DVertexSetU(&m_VB[4], 0.0f);		
			RwIm3DVertexSetV(&m_VB[4], 1.0f);
			RwIm3DVertexSetU(&m_VB[5], 1.0f);		
			RwIm3DVertexSetV(&m_VB[5], 1.0f);	
			RwIm3DVertexSetRGBA(&m_VB[3], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetRGBA(&m_VB[4], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetRGBA(&m_VB[5], _RGBA.red, _RGBA.green, _RGBA.blue, 255);
			RwIm3DVertexSetPos(&m_VB[3], Pos3.x, Pos3.y, Pos3.z);
			RwIm3DVertexSetPos(&m_VB[4], Pos0.x, Pos0.y, Pos0.z);
			RwIm3DVertexSetPos(&m_VB[5], Pos2.x, Pos2.y, Pos2.z);
		}

		if(RwIm3DTransform(m_VB, 6, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
			RwIm3DEnd();
		}

		++The;
	}
}

VOID CShoreLinePGenerator::Render()
{
	RwReal								Size	= 0.0f;
	CNtlPLShoreLineProp*				pSLProp	= static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));
	CNtlWorldFieldManager*				pMgr	= dGETMGR();
	std::list<CShoreLineCP*>::iterator	The		= m_listShoreLineCP.begin();
	std::list<CShoreLineCP*>::iterator	TheLast = m_listShoreLineCP.end(); --TheLast;
	std::list<CShoreLineCP*>::iterator	SThe, EThe;

	// verify valid script is selected
	if(pSLProp->GetCurType() == eST_NOTHING)
	{
		return;
	}

	BeginLightState(FALSE);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	// render UI
	RenderLine(Size);

	RwRGBA Clr = {255, 255, 255, 255};
	Clr.alpha	= 255;
	Clr.red		-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);
	Clr.green	-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);
	Clr.blue	-= static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight);

	// set size, textures
	switch(pSLProp->GetCurType())
	{
	case eST_UV_SINGLE:
		{
			RwD3D9SetTexture(pSLProp->GetUVSContainer()->m_pTex, 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &pSLProp->GetUVSContainer()->m_mTex);

			RenderGeom(pSLProp->GetUVSContainer()->m_Size, Clr);
		}
		break;

	case eST_UV_DUAL:
		{
			RwD3D9SetTexture(pSLProp->GetUVDContainer()->m_pTex[0], 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &pSLProp->GetUVDContainer()->m_mTex[0]);
			RenderGeom(pSLProp->GetUVDContainer()->m_Size, Clr);

			RwD3D9SetTexture(pSLProp->GetUVDContainer()->m_pTex[1], 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &pSLProp->GetUVDContainer()->m_mTex[1]);
			RenderGeom(pSLProp->GetUVDContainer()->m_Size, Clr);
		}
		break;

	case eST_SEQUENCE:
		{
			D3DXMATRIXA16 mIdentity;
			D3DXMatrixIdentity(&mIdentity);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);

			RwD3D9SetTexture(pSLProp->GetSeqContainer()->m_ppTexPack[pSLProp->GetSeqContainer()->m_FrameIdx], 0);
			RenderGeom(pSLProp->GetSeqContainer()->m_Size, Clr);
		}
		break;
	}

	// restore
	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	
	EndLightState();
}