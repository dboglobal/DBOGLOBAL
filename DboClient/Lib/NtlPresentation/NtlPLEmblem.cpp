#include "precomp_ntlpresentation.h"
#include "NtlPLEmblem.h"
#include "NtlDebug.h"
#include "NtlPLEntityFreeList.h"
#include "NtlMath.h"
#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "gui_texture.h"
#include "gui_texturemanager.h"
/*
#define EMBLEM_BOX_CX		2.0f
#define EMBLEM_BOX_CY		(EMBLEM_BOX_CX/8.0f)

#define EMBLEM_DIST							40.0f
#define EMBLEM_CAMERA__BOUNDING_RADIUS		0.1f

using namespace gui;

CNtlPLEmblem::CNtlPLEmblem()
{
	m_bCameraCullIn = FALSE;
	m_fCullDist		= EMBLEM_DIST;
	m_pTexture		= NULL;

	SetLayer(PLENTITY_LAYER_PLAYERNAME);
	SetClassType(PLENTITY_PLAYER_NAME);
	SetFlags(NTL_PLEFLAG_FRUSTUMOUT_NOTUPDATE | NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_ATTACH);

	VertexAssin();
}

CNtlPLEmblem::~CNtlPLEmblem()
{
}

RwBool CNtlPLEmblem::Create( const SPLEntityCreateParam * pParam )
{
	NTL_FUNCTION("CNtlPLEmblem::Create");
	
	NTL_ASSERT( pParam != NULL, RWSTRING( "NULL Param" ) );

	// font 생성
	m_pFont = g_FontMgr.CreateGuiFont( DEFAULT_FONT, 100, DEFAULT_FONT_ATTR );
	if(m_pFont == NULL)
	{
		NTL_RETURN(FALSE);
	}

	const SPLEmblemCreateParam *pEmblem = reinterpret_cast<const SPLEmblemCreateParam*>(pParam);

	m_fCullDist = pEmblem->fCullDist;

	
	// texture 생성(로직의 에러... 256으로 할려면.. 255가 최고다 버그를 잡자.)
	m_pTexture = g_TextureMgr.CreateTexture(255, size.y);
	if(m_pTexture == NULL)
	{
		NTL_RETURN(FALSE);
	}

	RwTextureSetFilterMode(m_pTexture->GetTexture(), rwFILTERLINEAR); 
	RwTextureSetAddressingU(m_pTexture->GetTexture(), rwTEXTUREADDRESSWRAP);
	RwTextureSetAddressingV(m_pTexture->GetTexture(), rwTEXTUREADDRESSWRAP);

	
	VertexAssin();

	NTL_RETURN(TRUE);
}

void CNtlPLEmblem::Destroy(void) 
{
	NTL_FUNCTION("CNtlPLEmblem::Destroy");
	
	if(m_pTexture)
	{
		g_TextureMgr.DeleteTexture(m_pTexture);
		m_pTexture = NULL;
	}
		
	NTL_RETURNVOID();
}

RwBool CNtlPLEmblem::Update(RwReal fElapsed)
{
	if(!IsVisible())
		return TRUE;

	return TRUE;
}

RwBool CNtlPLEmblem::Render(void)
{
	if(!IsVisible() || !m_bCameraCullIn)
		return TRUE;
	
	if(m_pTexture)
		RwD3D9SetTexture(m_pTexture->GetTexture(), 0);
	else
		RwD3D9SetTexture(NULL, 0);

	RwUInt32 nTransformFlags = rwIM3D_VERTEXUV | rwIM3D_ALLOPAQUE;

	if( RwIm3DTransform(m_3dVertices, 4, &CNtlPLGlobal::m_matIden, nTransformFlags) )
    {                         
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);  
        
        RwIm3DEnd();
    }
		
	return TRUE;
}


void CNtlPLEmblem::SetAlpha(RwUInt8 byValue)
{
	RwIm3DVertexSetRGBA(&m_3dVertices[0], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dVertices[1], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dVertices[2], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dVertices[3], 255, 255, 255, byValue);
}

void* CNtlPLEmblem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLEmblem::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_PLAYER_NAME));
}

void CNtlPLEmblem::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_PLAYER_NAME, pObj);
}


void CNtlPLEmblem::VertexAssin(void)
{
	
	m_v3Pos[0].x = EMBLEM_BOX_CX/2.f;	
	m_v3Pos[0].y = -EMBLEM_BOX_CY/2.f;	
	m_v3Pos[0].z = 0.f;	

	m_v3Pos[1].x = EMBLEM_BOX_CX/2.f;	
	m_v3Pos[1].y = EMBLEM_BOX_CY/2.f;	
	m_v3Pos[1].z = 0.f;

	m_v3Pos[2].x = -EMBLEM_BOX_CX/2.f;	
	m_v3Pos[2].y = -EMBLEM_BOX_CY/2.f;	
	m_v3Pos[2].z = 0.f;

	m_v3Pos[3].x = -EMBLEM_BOX_CX/2.f;	
	m_v3Pos[3].y = EMBLEM_BOX_CY/2.f;	
	m_v3Pos[3].z = 0.f;

	RwIm3DVertexSetU (&m_3dVertices[0], 0.0f);
	RwIm3DVertexSetV (&m_3dVertices[0], 0.0f);

	RwIm3DVertexSetU (&m_3dVertices[1], 0.0f);
	RwIm3DVertexSetV (&m_3dVertices[1], 1.0f);

	RwIm3DVertexSetU (&m_3dVertices[2], 1.0f);
	RwIm3DVertexSetV (&m_3dVertices[2], 0.0f);

	RwIm3DVertexSetU (&m_3dVertices[3], 1.0f);
	RwIm3DVertexSetV (&m_3dVertices[3], 1.0f);

	for(RwInt32 i = 0; i < 4; i++)
	{
		RwIm3DVertexSetRGBA(&m_3dVertices[i], 255, 255, 255, 255);
	}
	
	RwMatrixSetIdentity(&m_matWorld);
}


void CNtlPLEmblem::SetPosition(const RwV3d *pPos)
{
	// position을 update 안할 경우.
	if(!IsVisible())
		return;
	
    RwSphere sphere;
	sphere.center = *pPos;
	sphere.radius = EMBLEM_CAMERA__BOUNDING_RADIUS;
	
	RwReal fCameraDist;
	RwFrustumTestResult ret = RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &sphere);
	if(ret == rwSPHEREOUTSIDE || ret == rwSPHEREBOUNDARY)
	{
		m_bCameraCullIn = FALSE;
		return;
	}
	else
	{
		RwV3d v3Dist;
		RwV3dSub(&v3Dist, &sphere.center, &RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)->modelling.pos);
		
		fCameraDist = RwV3dLength(&v3Dist);
		if(fCameraDist > m_fCullDist)
		{
			m_bCameraCullIn = FALSE;
            return;
		}
		else
			m_bCameraCullIn = TRUE;
	}

	RwMatrix matInvert, matScale;
	RwMatrix *pMatrix = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
	RwMatrixInvert(&matInvert, pMatrix);

	RwReal fScale = fCameraDist/20.0f;
	if(fScale < 0)
		fScale = 0.0f;

	RwV3d vScale;
	CNtlMath::MathRwV3dAssign(&vScale, 1.0f+fScale, 1.0f+fScale, 1.0f+fScale);
	RwMatrixScale(&matScale, &vScale, rwCOMBINEREPLACE);
	RwMatrixMultiply(&m_matWorld, &matScale, &matInvert);
	CNtlMath::MathRwV3dAssign(&m_matWorld.pos, pPos->x, pPos->y, pPos->z);
	CNtlMath::MathRwV3dAssign(&m_matWorld.right, -m_matWorld.right.x, -m_matWorld.right.y, -m_matWorld.right.z);

	for (RwInt32 i = 0; i < 4; ++ i)
	{
		RwV3dTransformPoint(&m_3dVertices[i].objVertex, &m_v3Pos[i], &m_matWorld);
	}
}

RwV3d CNtlPLEmblem::GetPosition(void)
{
	return m_matWorld.pos;
}*/