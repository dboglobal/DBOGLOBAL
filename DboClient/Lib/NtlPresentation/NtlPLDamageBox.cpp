#include "precomp_ntlpresentation.h"
#include "NtlPLDamageBox.h"
#include "NtlDebug.h"
#include "NtlPLEntityFreeList.h"
#include "NtlMath.h"
#include "NtlPLResourceManager.h"

#define NTL_DANAGEBOX_WIDTH					0.1f
#define NTL_DAMAGEBOX_HEIGHT				0.1f
#define NTL_DAMAGEBOX_INTERVAL				0.6f
#define NTL_DAMAGEBOX_MOVE_SPEED			1.0f
#define NTL_DAMAGEBOX_START_SCALE_RATIO		2.0f
#define NTL_DAMAGEBOX_SCALE_RATIO			6.0f
#define NTL_DAMAGEBOX_LIFE_TIME				1.0f


RwTexture* CNtlPLDamageBox::m_ppTexture[MAX_DAMAGEBOX_COLOR][MAX_DAMAGEBOX_IMAGE_COUNT];


CNtlPLDamageBox::CNtlPLDamageBox()
{
	m_fLifeTime	= 0.f;
	m_iImageCount = 0;

	SetLayer(PLENTITY_LAYER_DAMAGEBOX);
	SetClassType(PLENTITY_DAMAGE_BOX);
	SetFlags(NTL_PLEFLAG_FRUSTUMOUT_NOTUPDATE | NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_ATTACH);

	VertexAssin();
}

CNtlPLDamageBox::~CNtlPLDamageBox()
{
}

void CNtlPLDamageBox::VertexAssin(void)
{
	for(RwInt32 i = 0; i < MAX_DAMAGEBOX_IMAGE_COUNT; ++ i)
	{
		RwIm3DVertexSetU(&m_Vertices[i][0], 0.0f);		
		RwIm3DVertexSetV(&m_Vertices[i][0], 0.0f);

		RwIm3DVertexSetU(&m_Vertices[i][1], 0.0f);		
		RwIm3DVertexSetV(&m_Vertices[i][1], 1.0f);

		RwIm3DVertexSetU(&m_Vertices[i][2], 1.0f);		
		RwIm3DVertexSetV(&m_Vertices[i][2], 0.0f);

		RwIm3DVertexSetU(&m_Vertices[i][3], 1.0f);		
		RwIm3DVertexSetV(&m_Vertices[i][3], 1.0f);

        RwIm3DVertexSetRGBA(&m_Vertices[i][0], 255, 255, 255, 255);
        RwIm3DVertexSetRGBA(&m_Vertices[i][1], 255, 255, 255, 255);
        RwIm3DVertexSetRGBA(&m_Vertices[i][2], 255, 255, 255, 255);
        RwIm3DVertexSetRGBA(&m_Vertices[i][3], 255, 255, 255, 255);

		RwIm3DVertexSetNormal(&m_Vertices[i][0], 0, 1, 0);
		RwIm3DVertexSetNormal(&m_Vertices[i][1], 0, 1, 0);
		RwIm3DVertexSetNormal(&m_Vertices[i][2], 0, 1, 0);
		RwIm3DVertexSetNormal(&m_Vertices[i][3], 0, 1, 0);

		CNtlMath::MathRwV3dAssign(&m_vPos[i][0], NTL_DANAGEBOX_WIDTH/2.f, -NTL_DAMAGEBOX_HEIGHT/2.f, 0.0f);
		CNtlMath::MathRwV3dAssign(&m_vPos[i][1], NTL_DANAGEBOX_WIDTH/2.f, NTL_DAMAGEBOX_HEIGHT/2.f, 0.0f);
		CNtlMath::MathRwV3dAssign(&m_vPos[i][2], -NTL_DANAGEBOX_WIDTH/2.f, -NTL_DAMAGEBOX_HEIGHT/2.f, 0.0f);
		CNtlMath::MathRwV3dAssign(&m_vPos[i][3], -NTL_DANAGEBOX_WIDTH/2.f, NTL_DAMAGEBOX_HEIGHT/2.f, 0.0f);
	}
}

void CNtlPLDamageBox::CreateResource(void)
{
	NTL_ASSERT(m_ppTexture[0][0] == NULL, "CNtlPLDamageBox::CreateResource => resource created!!!");

	std::string strTexName[MAX_DAMAGEBOX_COLOR][MAX_DAMAGEBOX_IMAGE_COUNT] = 
	{ 
		{"pc_0.png", "Pc_1.png", "Pc_2.png", "Pc_3.png", "Pc_4.png", "Pc_5.png", "Pc_6.png", "Pc_7.png", "Pc_8.png", "Pc_9.png"}, 
		{"Mob_0.png", "Mob_1.png", "Mob_2.png", "Mob_3.png", "Mob_4.png", "Mob_5.png", "Mob_6.png", "Mob_7.png", "Mob_8.png", "Mob_9.png"}, 
		{"PcHeal_0.png", "PcHeal_1.png", "PcHeal_2.png", "PcHeal_3.png", "PcHeal_4.png", "PcHeal_5.png", "PcHeal_6.png", "PcHeal_7.png", "PcHeal_8.png", "PcHeal_9.png"}, 
		{"MobHeal_0.png", "MobHeal_1.png", "MobHeal_2.png", "MobHeal_3.png", "MobHeal_4.png", "MobHeal_5.png", "MobHeal_6.png", "MobHeal_7.png", "MobHeal_8.png", "MobHeal_9.png"}
	};

	for(RwInt32 j = 0; j < MAX_DAMAGEBOX_COLOR; j++)
	{
		for(RwInt32 i = 0; i < MAX_DAMAGEBOX_IMAGE_COUNT; i++)
		{
			m_ppTexture[j][i] = GetNtlResourceManager()->LoadTexture(strTexName[j][i].c_str(), PATH_EFFECT_TEXTURE, CNtlPLResourceManager::RES_TEX_EFFECT_DICT);
		}
	}
}

void CNtlPLDamageBox::DestroyResource(void)
{
	for(RwInt32 j = 0; j < MAX_DAMAGEBOX_COLOR; j++)
	{
		for(RwInt32 i = 0; i < MAX_DAMAGEBOX_IMAGE_COUNT; i++)
		{
			if(m_ppTexture[j][i])
				GetNtlResourceManager()->UnLoadTexture(m_ppTexture[j][i]);

			m_ppTexture[j][i] = NULL;
		}
	}
}

RwBool CNtlPLDamageBox::Create( const SPLEntityCreateParam * pParam /*= NULL*/ )
{
	NTL_FUNCTION("CNtlPLDamageBox::Create");
	
	NTL_ASSERT( pParam != NULL, RWSTRING( "NULL Param" ) );

	const SPLDamageBoxCreateParam *pDamageBoxParam = reinterpret_cast<const SPLDamageBoxCreateParam*>(pParam);

	RwV3dAssign(&m_vCurrPos, pDamageBoxParam->pPos);

	m_vCurrPos.x += NtlRandomNumber(-1.5f, 1.5f);

	RwChar chBuff[MAX_DAMAGEBOX_IMAGE_COUNT];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	_itoa_s(pDamageBoxParam->uiDamage, chBuff, MAX_DAMAGEBOX_IMAGE_COUNT, 10);
#else
	itoa(pDamageBoxParam->uiDamage, chBuff, IMAGE_MAX_COUNT);
#endif
	m_iImageCount = (RwInt32)strlen(chBuff);

	RwReal fStart = (m_iImageCount - 1) * (NTL_DANAGEBOX_WIDTH * 0.5f);

	for (RwInt32 i = 0; i < m_iImageCount; ++ i)
	{
		m_byImageIdx[i] = chBuff[i] - 48;

		m_vPos[i][0].x += fStart-NTL_DANAGEBOX_WIDTH * NTL_DAMAGEBOX_INTERVAL * i;
		m_vPos[i][1].x += fStart-NTL_DANAGEBOX_WIDTH * NTL_DAMAGEBOX_INTERVAL * i;
		m_vPos[i][2].x += fStart-NTL_DANAGEBOX_WIDTH * NTL_DAMAGEBOX_INTERVAL * i;
		m_vPos[i][3].x += fStart-NTL_DANAGEBOX_WIDTH * NTL_DAMAGEBOX_INTERVAL * i;
	}

	m_byColorType = pDamageBoxParam->iClolorType;

	NTL_RETURN(TRUE);
}

void CNtlPLDamageBox::Destroy(void)
{
	
}

RwBool CNtlPLDamageBox::Update(RwReal fElapsed)
{
	m_fLifeTime += fElapsed;
	if (m_fLifeTime > NTL_DAMAGEBOX_LIFE_TIME)
		return FALSE;

	m_vCurrPos.y += fElapsed * NTL_DAMAGEBOX_MOVE_SPEED;

	RwInt32 alpha = 255 - (RwInt32)((m_fLifeTime / NTL_DAMAGEBOX_LIFE_TIME) * 255);
	
	RwMatrix matTemp, matScale;
	RwMatrix *pMatrix = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
	RwMatrixInvert(&matTemp, pMatrix);

	RwReal fScale = NTL_DAMAGEBOX_START_SCALE_RATIO + (m_fLifeTime / NTL_DAMAGEBOX_LIFE_TIME) * (NTL_DAMAGEBOX_SCALE_RATIO - NTL_DAMAGEBOX_START_SCALE_RATIO);
	
	RwV3d vScale;
	CNtlMath::MathRwV3dAssign(&vScale, fScale, fScale, fScale);
	
	RwMatrixScale(&matScale, &vScale, rwCOMBINEREPLACE);

	RwMatrixMultiply(&m_matWorld, &matScale, &matTemp);

	CNtlMath::MathRwV3dAssign(&m_matWorld.pos, m_vCurrPos.x, m_vCurrPos.y, m_vCurrPos.z);
	CNtlMath::MathRwV3dAssign(&m_matWorld.right, -m_matWorld.right.x, -m_matWorld.right.y, -m_matWorld.right.z);

	for (RwInt32 i = 0; i < m_iImageCount; ++ i)
	{
		RwIm3DVertexSetRGBA( &m_Vertices[i][0], 255, 255, 255, 255);
		RwIm3DVertexSetRGBA( &m_Vertices[i][1], 255, 255, 255, 255);
		RwIm3DVertexSetRGBA( &m_Vertices[i][2], 255, 255, 255, 255);
		RwIm3DVertexSetRGBA( &m_Vertices[i][3], 255, 255, 255, 255);

		RwV3dTransformPoint((RwV3d*)&m_Vertices[i][0].objVertex, &m_vPos[i][0], &m_matWorld);
		RwV3dTransformPoint((RwV3d*)&m_Vertices[i][1].objVertex, &m_vPos[i][1], &m_matWorld);
		RwV3dTransformPoint((RwV3d*)&m_Vertices[i][2].objVertex, &m_vPos[i][2], &m_matWorld);
		RwV3dTransformPoint((RwV3d*)&m_Vertices[i][3].objVertex, &m_vPos[i][3], &m_matWorld);
	}

	return TRUE;
}

RwBool CNtlPLDamageBox::Render(void)
{
	for (RwInt32 i = 0; i < m_iImageCount; ++ i)
	{
		RwD3D9SetTexture(m_ppTexture[m_byColorType][m_byImageIdx[i]], 0);

		if( RwIm3DTransform(m_Vertices[i], 4, NULL, rwIM3D_VERTEXUV | rwIM3D_ALLOPAQUE) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
			RwIm3DEnd();
		}
	}

	return TRUE;
}

RwBool CNtlPLDamageBox::SetProperty(const CNtlPLProperty *pData)
{
	return TRUE;
}
	
void* CNtlPLDamageBox::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLDamageBox::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_DAMAGE_BOX));
}

void CNtlPLDamageBox::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_DAMAGE_BOX, pObj);
}



