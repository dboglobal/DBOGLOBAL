#include "precomp_ntlpresentation.h"
#include "PathPoint.h"
#include "NtlPLSceneManager.h"
#include "NtlPLObject.h"
#include "NtlPLEntity.h"
#include "WorldPathTable.h"


CPathPoint::CPathPoint(RwV3d& _Pos)
{
	m_Pos			= _Pos;
	m_PrevDist		= 0.0f;
	m_Range			= 0.0f;
	m_Visibility	= TRUE;
	m_pDispEntity	= NULL;

	SPLEntityCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos = &m_Pos;	
	
	m_pDispEntity = GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "EFFECT_DUMMY", &PLEntityCreateParam);
	DBO_ASSERT(m_pDispEntity, "Entity create failed.");

	m_pDispEntity->SetFlags(m_pDispEntity->GetFlags() | NTL_PLEFLAG_TOOL_PICK);

	static_cast<CNtlPLObject*>(m_pDispEntity)->SetFadeEnable(FALSE);

	RwV3d Scl = { 1.0f, 1.0f, 1.0f };
	RwV3d Rot = { 0.0f, 0.0f, 0.0f };
	RwV3d Pos = m_Pos;

	static_cast<CNtlPLObject*>(m_pDispEntity)->SetScale(&Scl);
	static_cast<CNtlPLObject*>(m_pDispEntity)->SetRotate(Rot.x, Rot.y, Rot.z);
	static_cast<CNtlPLObject*>(m_pDispEntity)->SetPosition(&Pos);
}

CPathPoint::~CPathPoint(void)
{
	if(m_pDispEntity)
	{
		GetSceneManager()->DeleteEntity(m_pDispEntity);
		m_pDispEntity = NULL;
	}
}

VOID CPathPoint::SetPosition(RwV3d& _Pos)
{
	NTL_ASSERTE(m_pDispEntity);

	m_Pos = _Pos;
	static_cast<CNtlPLObject*>(m_pDispEntity)->SetPosition(&m_Pos);
}

VOID CPathPoint::SetVisibility(RwBool _Visibility)
{
	if(_Visibility)
	{
		if(!m_pDispEntity)
		{
			SPLEntityCreateParam PLEntityCreateParam;
			PLEntityCreateParam.pPos = &m_Pos;	
			m_pDispEntity = GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "EFFECT_DUMMY", &PLEntityCreateParam);
			DBO_ASSERT(m_pDispEntity, "Entity create failed.");

			static_cast<CNtlPLObject*>(m_pDispEntity)->SetFadeEnable(FALSE);

			RwV3d Scl = { 1.0f, 1.0f, 1.0f };
			RwV3d Rot = { 0.0f, 0.0f, 0.0f };
			RwV3d Pos = m_Pos;

			static_cast<CNtlPLObject*>(m_pDispEntity)->SetScale(&Scl);
			static_cast<CNtlPLObject*>(m_pDispEntity)->SetRotate(Rot.x, Rot.y, Rot.z);
			static_cast<CNtlPLObject*>(m_pDispEntity)->SetPosition(&Pos);
		}
	}
	else
	{
		if(m_pDispEntity)
		{
			GetSceneManager()->DeleteEntity(m_pDispEntity);
			m_pDispEntity = NULL;
		}	
	}
}

VOID CPathPoint::SetColor(RwRGBA& _RGBA)
{
	//NTL_ASSERTE(m_pDispEntity);
	if (m_pDispEntity)
	{
		static_cast<CNtlPLObject*>(m_pDispEntity)->SetColor(_RGBA.red, _RGBA.green, _RGBA.blue);
	}
}

VOID CPathPoint::Save(FILE* _pFile, RwReal _PrevDist)
{
	sPATH_POINT sPathPoint;
	sPathPoint._Pos.x		= m_Pos.x;
	sPathPoint._Pos.y		= m_Pos.y;
	sPathPoint._Pos.z		= m_Pos.z;
	sPathPoint._PrevDist	= _PrevDist;

	fwrite(&sPathPoint._Pos.x, sizeof(float), 1, _pFile);
	fwrite(&sPathPoint._Pos.y, sizeof(float), 1, _pFile);
	fwrite(&sPathPoint._Pos.z, sizeof(float), 1, _pFile);
	fwrite(&sPathPoint._PrevDist, sizeof(float), 1, _pFile);
}

VOID CPathPoint::Load(FILE* _pFile)
{
	CNtlVector	vTmp;
	float		fPrevDist;

	fread(&vTmp.x, sizeof(float), 1, _pFile);
	fread(&vTmp.y, sizeof(float), 1, _pFile);
	fread(&vTmp.z, sizeof(float), 1, _pFile);	
	fread(&fPrevDist, sizeof(float), 1, _pFile);
	m_Pos.x		= static_cast<RwReal>(vTmp.x);
	m_Pos.y		= static_cast<RwReal>(vTmp.y);
	m_Pos.z		= static_cast<RwReal>(vTmp.z);
	m_PrevDist	= static_cast<RwReal>(fPrevDist);

	RwV3d Scl = { 1.0f, 1.0f, 1.0f };
	RwV3d Rot = { 0.0f, 0.0f, 0.0f };
	RwV3d Pos = m_Pos;

	static_cast<CNtlPLObject*>(m_pDispEntity)->SetScale(&Scl);		
	static_cast<CNtlPLObject*>(m_pDispEntity)->SetRotate(Rot.x, Rot.y, Rot.z);
	static_cast<CNtlPLObject*>(m_pDispEntity)->SetPosition(&Pos);
}