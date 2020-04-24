#include "precomp_ntlpresentation.h"
#include "NtlPLPlayerName.h"
#include "NtlDebug.h"
#include "NtlPLEntityFreeList.h"
#include "NtlMath.h"
#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "gui_font.h"
#include "gui_fontmanager.h"
#include "gui_texture.h"
#include "gui_texturemanager.h"
#include "ntlworldcommon.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

using namespace gui;

#define dPLAYER_NAME_CREATE_TEXTURE_OFFSET 10

/**
* \brief Constructions
*/
CNtlPLPlayerName::CNtlPLPlayerName()
{
	m_bCameraCullIn		= FALSE;
	m_fCullDist			= PLAYERNAME_DIST;
	m_pFont				= NULL;
	m_pGuildFont		= NULL;
	m_pTitleFont = NULL;
	m_pTexture			= NULL;
	m_pEmblemTexture	= NULL;
	m_pNickTexture		= NULL;
    m_byAlpha			= 255;
	m_byFlag			= dNTL_PL_FLAG_NAME;
	m_bNameVisible		= TRUE;

	m_posNameSize.SetPos( 0, 0 );
	m_posGuildNameSize.SetPos( 0, 0 );
	m_posNickNameSize.SetPos( 0, 0 );
	m_posTitleNameSize.SetPos(0, 0);

	// World Position 초기화
	CNtlMath::MathRwV3dAssign(&m_v3WorldPosition, 0.0f, 0.0f, 0.0f);

	SetLayer(PLENTITY_LAYER_PLAYERNAME);
	SetClassType(PLENTITY_PLAYER_NAME);
	SetFlags(NTL_PLEFLAG_FRUSTUMOUT_NOTUPDATE | NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_ATTACH);

	AllVertexInit();
	VertexAssign();
}

/**
* \brief Destructions
*/
CNtlPLPlayerName::~CNtlPLPlayerName()
{
}

/**
* \brief Create
* \param pParam		(SPLEntityCreateParam*) Entity의 Create 파라메터 구조체
* \return 성공여부
*/
RwBool CNtlPLPlayerName::Create( const SPLEntityCreateParam * pParam )
{
	NTL_FUNCTION("CNtlPLPlayerName::Create");
	
	NTL_ASSERT( pParam != NULL, RWSTRING( "NULL Param" ) );

	const SPLPlayerNameCreateParam *pPlayerNameParam = reinterpret_cast<const SPLPlayerNameCreateParam*>(pParam);
	if(pPlayerNameParam->pName == NULL)
	{
		NTL_RETURN(FALSE);
	}

	// Camera curling distance
	m_fCullDist = pPlayerNameParam->fCullDist;

	// Create Name Texture
	if( !CreateNameTexture( pPlayerNameParam->pName ) )
		NTL_RETURN(FALSE);


	if (pPlayerNameParam->pTitleName != NULL)
	{
		if (wcslen(pPlayerNameParam->pTitleName) > 0)
		{
			if (CreateTitleNameTexture(pPlayerNameParam->pTitleName))
				m_byFlag |= dNTL_PL_FLAG_TITLE;

		}
	}

	if(pPlayerNameParam->pGuildName == NULL )
	{
		NTL_RETURN(FALSE);
	}
	// If the length of the guild name is greater than 0, it is determined that the guild name exists
	else if( wcslen( pPlayerNameParam->pGuildName ) > 0 )
	{
		WCHAR wchGuildName[NTL_MAX_SIZE_GUILD_NAME * 2 + 1];
		swprintf_s(wchGuildName, L"<%s>", pPlayerNameParam->pGuildName);

		if( CreateGuildNameTexture(wchGuildName) )
			m_byFlag |= dNTL_PL_FLAG_GUILD;

		if( CreateGuildEmblemTexture( pPlayerNameParam->pEmblemFactor) )
			m_byFlag |= dNTL_PL_FLAG_EMBLEM;
		
	}
	
	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CNtlPLPlayerName::Destroy(void) 
{
	NTL_FUNCTION("CNtlPLPlayerName::Destroy");
	
	if( m_pFont )
	{
		g_FontMgr.DeleteGuiFont(m_pFont);
		m_pFont = NULL;
	}

	if( m_pGuildFont )
	{
		g_FontMgr.DeleteGuiFont( m_pGuildFont );
		m_pGuildFont = NULL;
	}

	if (m_pTitleFont)
	{
		g_FontMgr.DeleteGuiFont(m_pTitleFont);
		m_pTitleFont = NULL;
	}

	if( m_pTexture )
	{
		g_TextureMgr.DeleteTexture(m_pTexture);
		m_pTexture = NULL;
	}

	if( m_pEmblemTexture )
	{
		GetEmblemMaker()->DestoyEmblem( m_pEmblemTexture );
		m_pEmblemTexture = NULL;
	}

	if( m_pNickTexture )
	{
		g_TextureMgr.DeleteTexture( m_pNickTexture );
		m_pNickTexture = NULL;
	}
		
	NTL_RETURNVOID();
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 업데이트에서 경과된 시간
*/
RwBool CNtlPLPlayerName::Update(RwReal fElapsed)
{
	// Entity가 현재 보이는 상태가 아니라면 Update하지 않는다.
	if(!IsVisible())
		return TRUE;

	// 카메라 컬링 체크
	RwSphere sphere;
	sphere.center = m_v3WorldPosition;
	sphere.radius = PLAYERNAME_CAMERA__BOUNDING_RADIUS;

	RwReal fCameraDist;
	RwFrustumTestResult ret = RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &sphere);
	if(ret == rwSPHEREOUTSIDE || ret == rwSPHEREBOUNDARY)
	{
		SetCameraCullin( FALSE );

		return TRUE;
	}
	else
	{
		RwV3d v3Dist;
		RwV3dSubMacro(&v3Dist, &sphere.center, &RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)->modelling.pos);

		fCameraDist = RwV3dLength(&v3Dist);
		if(fCameraDist > m_fCullDist)
		{
			SetCameraCullin( FALSE );

			return TRUE;
		}
		else
		{
			SetCameraCullin( TRUE );
		}
	}

	// Flag에 따라서 다른 함수를 적용한다.
	if( m_byFlag & dNTL_PL_FLAG_EMBLEM )
		EmblemVertexAssign();
	else
		NameVertexAssign();

	if( m_byFlag & dNTL_PL_FLAG_NICK )
		NickNameVertexAssign();

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
	CNtlMath::MathRwV3dAssign(&m_matWorld.pos, m_v3WorldPosition.x, m_v3WorldPosition.y, m_v3WorldPosition.z);
	CNtlMath::MathRwV3dAssign(&m_matWorld.right, -m_matWorld.right.x, -m_matWorld.right.y, -m_matWorld.right.z);

	for (RwInt32 i = 0; i < 4; ++ i)
	{
		RwV3dTransformPoint(&m_3dVertices[i].objVertex, &m_v3Pos[i], &m_matWorld);
	}

	if( m_byFlag & dNTL_PL_FLAG_EMBLEM )
	{
		for (RwInt32 i = 0; i < 4; ++ i)
		{
			RwV3dTransformPoint(&m_3dEmblemVertices[i].objVertex, &m_v3EmblemPos[i], &m_matWorld);
		}
	}

	if( m_byFlag & dNTL_PL_FLAG_NICK )
	{
		for (RwInt32 i = 0; i < 4; ++ i)
		{
			RwV3dTransformPoint(&m_3dNickVertices[i].objVertex, &m_v3NickPos[i], &m_matWorld);
		}
	}

	// Alpha 의 가중치가 1.0f 아니라면 Entity의 알파 가중치 값에 현재 엔티티의 알파값을 적용한다.
    if(GetWeightAlpha() != 1.0f)
    {
        RwUInt8 byAlpha = (RwUInt8)(m_byAlpha * GetWeightAlpha());
        for(int i = 0; i < 4; ++i)
        {
           RwIm3DVertexSetRGBA(&m_3dVertices[i], 255, 255, 255, byAlpha);        
        }
    }

	return TRUE;
}

/**
* \brief Render
* 이름과 Emblem을 렌더링한다.
*/
RwBool CNtlPLPlayerName::Render(void)
{
	if(!IsVisible() || !m_bCameraCullIn)
		return TRUE;

	RenderName();
	RenderEmblem();
	RenderNick();
		
	return TRUE;
}

/**
* \brief PlayerName Entity에 알파를 지정한다.
* \param byValue	(RwUInt8) 알파값
*/
void CNtlPLPlayerName::SetAlpha(RwUInt8 byValue)
{
	RwIm3DVertexSetRGBA(&m_3dVertices[0], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dVertices[1], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dVertices[2], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dVertices[3], 255, 255, 255, byValue);

	RwIm3DVertexSetRGBA(&m_3dEmblemVertices[0], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dEmblemVertices[1], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dEmblemVertices[2], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dEmblemVertices[3], 255, 255, 255, byValue);

	RwIm3DVertexSetRGBA(&m_3dNickVertices[0], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dNickVertices[1], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dNickVertices[2], 255, 255, 255, byValue);
	RwIm3DVertexSetRGBA(&m_3dNickVertices[3], 255, 255, 255, byValue);

    m_byAlpha = byValue;
}

/**
* \brief new 연산자 오버로딩
* \param size	(size_t) 메모리 사이즈
* \return (void*) CNtlPLEntityFreeList의 Alloc의 리턴값
*/
void* CNtlPLPlayerName::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLPlayerName::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_PLAYER_NAME));
}

/**
* \brief delte 연산자 오버로딩
* \param pObj	(void*) CNtlPLEntityFreeList의 Free로 해제해줘야 하는 객체의 void pointer
*/
void CNtlPLPlayerName::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_PLAYER_NAME, pObj);
}

/**
* \brief 모든 버텍스 정보를 0으로 초기화한다.
*/
void CNtlPLPlayerName::AllVertexInit( void ) 
{
	memset( &m_v3Pos, 0, sizeof(RwV3d) * PLAYERNAME_VERTEX_COUNT );
	memset( &m_v3EmblemPos, 0, sizeof(RwV3d) * PLAYERNAME_VERTEX_COUNT );
	memset( &m_v3NickPos, 0, sizeof(RwV3d) * PLAYERNAME_VERTEX_COUNT );
	memset( &m_3dVertices, 0, sizeof(RwIm3DVertex) * PLAYERNAME_VERTEX_COUNT );
	memset( &m_3dEmblemVertices, 0, sizeof(RwIm3DVertex) * PLAYERNAME_VERTEX_COUNT );
	memset( &m_3dNickVertices, 0, sizeof(RwIm3DVertex) * PLAYERNAME_VERTEX_COUNT );
}

/**
* \brief PlayerName에서 쓰이는 모든 버텍스들의 UV와 RGB값을 셋팅
*/
void CNtlPLPlayerName::VertexAssign( void ) 
{
	// UV Setting
	RwIm3DVertexSetU (&m_3dVertices[0], 0.0f);
	RwIm3DVertexSetV (&m_3dVertices[0], 0.0f);

	RwIm3DVertexSetU (&m_3dVertices[1], 0.0f);
	RwIm3DVertexSetV (&m_3dVertices[1], 1.0f);

	RwIm3DVertexSetU (&m_3dVertices[2], 1.0f);
	RwIm3DVertexSetV (&m_3dVertices[2], 0.0f);

	RwIm3DVertexSetU (&m_3dVertices[3], 1.0f);
	RwIm3DVertexSetV (&m_3dVertices[3], 1.0f);

	// RGBA Setting
	for(RwInt32 i = 0; i < 4; i++)
	{
		RwIm3DVertexSetRGBA(&m_3dVertices[i], 255, 255, 255, 255);
	}

	// UV Setting
	RwIm3DVertexSetU (&m_3dEmblemVertices[0], 0.0f);
	RwIm3DVertexSetV (&m_3dEmblemVertices[0], 0.0f);

	RwIm3DVertexSetU (&m_3dEmblemVertices[1], 0.0f);
	RwIm3DVertexSetV (&m_3dEmblemVertices[1], 1.0f);

	RwIm3DVertexSetU (&m_3dEmblemVertices[2], 1.0f);
	RwIm3DVertexSetV (&m_3dEmblemVertices[2], 0.0f);

	RwIm3DVertexSetU (&m_3dEmblemVertices[3], 1.0f);
	RwIm3DVertexSetV (&m_3dEmblemVertices[3], 1.0f);

	// RGBA Setting
	for(RwInt32 i = 0; i < 4; i++)
	{
		RwIm3DVertexSetRGBA(&m_3dEmblemVertices[i], 255, 255, 255, 255);
	}

	// UV Setting
	RwIm3DVertexSetU (&m_3dNickVertices[0], 0.0f);
	RwIm3DVertexSetV (&m_3dNickVertices[0], 0.0f);

	RwIm3DVertexSetU (&m_3dNickVertices[1], 0.0f);
	RwIm3DVertexSetV (&m_3dNickVertices[1], 1.0f);

	RwIm3DVertexSetU (&m_3dNickVertices[2], 1.0f);
	RwIm3DVertexSetV (&m_3dNickVertices[2], 0.0f);

	RwIm3DVertexSetU (&m_3dNickVertices[3], 1.0f);
	RwIm3DVertexSetV (&m_3dNickVertices[3], 1.0f);

	// RGBA Setting
	for(RwInt32 i = 0; i < 4; i++)
	{
		RwIm3DVertexSetRGBA(&m_3dNickVertices[i], 255, 255, 255, 255);
	}

	RwMatrixSetIdentity(&m_matWorld);
}

/**
* \brief PlayerName 이나 Guild Name 까지만 출력할때 사용되는 버텍스 정렬
*/
void CNtlPLPlayerName::NameVertexAssign(void) 
{
	if( m_pTexture == NULL )
		return;

	RwReal fWidth = GetAdjustPolygonWidth( m_pTexture->GetWidth() );
	RwReal fHeight = GetAdjustPolygonHeight( m_pTexture->GetHeight() );

	// Name Texture Local Position
	m_v3Pos[0].x = (fWidth/2.f);
	m_v3Pos[0].y = (-fHeight/2.f);
	m_v3Pos[0].z = 0.f;

	m_v3Pos[1].x = (fWidth/2.f);
	m_v3Pos[1].y = (fHeight/2.f);
	m_v3Pos[1].z = 0.f;

	m_v3Pos[2].x = (-fWidth/2.f);
	m_v3Pos[2].y = (-fHeight/2.f);	
	m_v3Pos[2].z = 0.f;

	m_v3Pos[3].x = (-fWidth/2.f);
	m_v3Pos[3].y = (fHeight/2.f);	
	m_v3Pos[3].z = 0.f;
	
}

/**
* \brief Emblem Vertex alignment used when outputting images
*/
void CNtlPLPlayerName::EmblemVertexAssign(void) 
{
	if( m_pEmblemTexture == NULL )
		return;

	// The size of the polygon that the name occupies
	RwReal fNameWidth = GetAdjustPolygonWidth( m_pTexture->GetWidth() );
	RwReal fNameHeight = GetAdjustPolygonHeight( m_pTexture->GetHeight() );
	
	// The size of the polygon occupied by the emblem
	/*RwReal fEmblemWidth = GetAdjustPolygonWidth( m_pEmblemTexture->raster->width );
	RwReal fEmblemHeight = GetAdjustPolygonHeight( m_pEmblemTexture->raster->height );*/

	RwReal fEmblemWidth = (RwReal)m_pEmblemTexture->raster->width * PLAYERNAME_EMBLEM_BASIS_CX;
	RwReal fEmblemHeight =(RwReal)m_pEmblemTexture->raster->height * PLAYERNAME_EMBLEM_BASIS_CY;
	
	// Actual texture size of name
	RwInt32 nNameHeight = m_posGuildNameSize.y + PLAYERNAME_LINE_INTERVAL_Y;
	RwInt32 nNameWidth = m_posNameSize.x + m_posTitleNameSize.x > m_posGuildNameSize.x ? m_posNameSize.x + m_posTitleNameSize.x : m_posGuildNameSize.x;

	// Actual polygon size of name
	RwReal fNameAdjustWidth = fNameWidth * ( (RwReal)nNameWidth/(RwReal)m_pTexture->GetWidth() );
	RwReal fNameAdjustHeight = fNameHeight * ( (RwReal)nNameHeight/(RwReal)m_pTexture->GetHeight() );

	RwReal fTotalWidth = fEmblemWidth + fNameWidth;
	RwReal fTotalHeight = fEmblemHeight + fNameHeight;

	RwReal fTotalAdjustWidth = fEmblemWidth +  fNameAdjustWidth;
	RwReal fTotalAdjustHeight = fEmblemWidth + fNameAdjustHeight;

	// Name Offset
	RwReal fOffsetNameWidth = (fEmblemWidth)/2.f;
	RwReal fOffsetNameHeight = (fNameHeight-fNameAdjustHeight)/2.f;
	
	RwReal fOffsetExtraWidth = (fNameWidth - fNameAdjustWidth)/2.f;

	// Emblem Offset

	RwReal fOffsetEmblemExtraWidth = (fNameWidth - fNameAdjustWidth)/2.f;
	RwReal fOffsetEmblemWidth = (fTotalWidth/2.f) - (fEmblemWidth/2.f);
	
	// Name Texture Local Position
	m_v3Pos[0].x = (fNameWidth/2.f) - fOffsetNameWidth - fOffsetExtraWidth - PLAYERNAME_BOX_OFFSET;
	m_v3Pos[0].y = (-fNameHeight/2.f) + fOffsetNameHeight;
	m_v3Pos[0].z = 0.f;

	m_v3Pos[1].x = (fNameWidth/2.f) - fOffsetNameWidth - fOffsetExtraWidth - PLAYERNAME_BOX_OFFSET;
	m_v3Pos[1].y = (fNameHeight/2.f) + fOffsetNameHeight;
	m_v3Pos[1].z = 0.f;

	m_v3Pos[2].x = (-fNameWidth/2.f) - fOffsetNameWidth - fOffsetExtraWidth - PLAYERNAME_BOX_OFFSET;
	m_v3Pos[2].y = (-fNameHeight/2.f) + fOffsetNameHeight;	
	m_v3Pos[2].z = 0.f;

	m_v3Pos[3].x = (-fNameWidth/2.f) - fOffsetNameWidth - fOffsetExtraWidth - PLAYERNAME_BOX_OFFSET;
	m_v3Pos[3].y = (fNameHeight/2.f) + fOffsetNameHeight;	
	m_v3Pos[3].z = 0.f;

	// Emblem Texture Local Position
	m_v3EmblemPos[0].x = (fEmblemWidth/2.f) + (fOffsetEmblemWidth) - (fOffsetExtraWidth) + PLAYERNAME_BOX_OFFSET;
	m_v3EmblemPos[0].y = (-fEmblemHeight/2.f);
	m_v3EmblemPos[0].z = 0.f;	

	m_v3EmblemPos[1].x = (fEmblemWidth/2.f) + (fOffsetEmblemWidth) - (fOffsetExtraWidth) + PLAYERNAME_BOX_OFFSET;
	m_v3EmblemPos[1].y = (fEmblemHeight/2.f);
	m_v3EmblemPos[1].z = 0.f;

	m_v3EmblemPos[2].x = (-fEmblemWidth/2.f) + (fOffsetEmblemWidth) - (fOffsetExtraWidth) + PLAYERNAME_BOX_OFFSET;
	m_v3EmblemPos[2].y = (-fEmblemHeight/2.f);
	m_v3EmblemPos[2].z = 0.f;

	m_v3EmblemPos[3].x = (-fEmblemWidth/2.f) + (fOffsetEmblemWidth) - (fOffsetExtraWidth) + PLAYERNAME_BOX_OFFSET;
	m_v3EmblemPos[3].y = (fEmblemHeight/2.f);
	m_v3EmblemPos[3].z = 0.f;
}

/**
* \brief Set the position and size of the vertex to which the nickname will be printed
*/
void CNtlPLPlayerName::NickNameVertexAssign( void )
{
	if( m_pNickTexture == NULL )
		return;

	RwReal fWidth = GetAdjustPolygonWidth( m_pNickTexture->GetWidth() );
	RwReal fHeight = GetAdjustPolygonHeight( m_pNickTexture->GetHeight() );

	m_v3NickPos[0].x = (fWidth/2.f);
	m_v3NickPos[0].y = (-fHeight/2.f) - m_v3Pos[1].y - PLAYERNAME_NICK_OFFSET;
	m_v3NickPos[0].z = 0.f;

	m_v3NickPos[1].x = (fWidth/2.f);
	m_v3NickPos[1].y = (fHeight/2.f) - m_v3Pos[1].y - PLAYERNAME_NICK_OFFSET;
	m_v3NickPos[1].z = 0.f;

	m_v3NickPos[2].x = (-fWidth/2.f);
	m_v3NickPos[2].y = (-fHeight/2.f) - m_v3Pos[1].y - PLAYERNAME_NICK_OFFSET;	
	m_v3NickPos[2].z = 0.f;

	m_v3NickPos[3].x = (-fWidth/2.f);
	m_v3NickPos[3].y = (fHeight/2.f) - m_v3Pos[1].y - PLAYERNAME_NICK_OFFSET;	
	m_v3NickPos[3].z = 0.f;
}

/**
* \brief Texture 사이즈가 가장 잘 보일만한 폴리곤의 사이즈를 리턴한다.
* \param nSize	(RwInt32) 텍스쳐의 가로 사이즈
* \return (RwReal) 폴리곤의 사이즈
*/
RwReal CNtlPLPlayerName::GetAdjustPolygonWidth( RwInt32 nSize ) 
{
	// Texture Size 1 을 기준으로 구하는 폴리곤 사이즈
	return nSize * PLAYERNAME_BOX_BASIS_CX;
}

/**
* \brief Texture 사이즈가 가장 잘 보일만한 폴리곤의 사이즈를 리턴한다.
* \param nSize	(RwInt32) 텍스쳐의 높이 사이즈
* \return (RwReal) 폴리곤의 사이즈
*/
RwReal CNtlPLPlayerName::GetAdjustPolygonHeight( RwInt32 nSize ) 
{
	return nSize * PLAYERNAME_BOX_BASIS_CY;
}

/**
* \brief 이름의 텍스쳐를 생성한다. (이름의 크기만 고려)
* \param pName	(const WCHAR*) 이름의 문자열 포인터
* \return (RwBool) 성공 여부
*/
RwBool CNtlPLPlayerName::CreateNameTexture( const WCHAR* pName ) 
{
	// If you already have a texture, you will need to delete it and recreate it. Reason: Resize
	if( m_pTexture )
	{
		g_TextureMgr.DeleteTexture( m_pTexture );
		m_pTexture = NULL;
	}

	// Create Font
	m_pFont = g_FontMgr.CreateGuiFont( DEFAULT_FONT, dNTL_PLAYERNAME_FONTSIZE_NAME, DEFAULT_FONT_ATTR ); 
	if(m_pFont == NULL)
	{
		NTL_RETURN(FALSE);
	}

	// If there is no string in the Name buffer, it is assumed that there is no character and FALSE is returned.
	if( (RwInt32)wcslen( pName ) <= 0 )
	{
		NTL_RETURN(FALSE);
	}
	
	// Get the pixel size of the Text, save it, and create the texture.
	CPos size = m_pFont->GetTextWSize( pName, (int)wcslen( pName )); 	
	m_posNameSize.SetPos( size.x, size.y );
	m_pTexture = g_TextureMgr.CreateTexture( m_posNameSize.x + dPLAYER_NAME_CREATE_TEXTURE_OFFSET, m_posNameSize.y + PLAYERNAME_LINE_INTERVAL_Y);
	if(m_pTexture == NULL)
	{
		NTL_RETURN(FALSE);
	}

	RwTextureSetFilterMode(m_pTexture->GetTexture(), rwFILTERLINEAR);
	RwTextureSetAddressingU(m_pTexture->GetTexture(), rwTEXTUREADDRESSWRAP);
	RwTextureSetAddressingV(m_pTexture->GetTexture(), rwTEXTUREADDRESSWRAP);

	//In World Tool Mode, SetNameColor () is not called separately.
#ifdef dNTL_WORLD_TOOL_MODE
	COLORREF color = RGB(255, 255, 255);

	m_pFont->TextOutW(L"               ", 0, 0);
	m_pFont->TextOutW(pName, 0, 0);

	CRectangle rect;
	rect.left = (m_pTexture->GetWidth()-size.x)/2;
	rect.top = (m_pTexture->GetHeight()-size.y)/2;
	rect.right = rect.left + size.x;
	rect.bottom = rect.top + size.y;

	// texture에 string copy
	g_FontMgr.BitmapToTexture(m_pTexture, rect, color, TE_OUTLINE, 0, 2);
#endif

	NTL_RETURN(TRUE);
}


RwBool CNtlPLPlayerName::CreateTitleNameTexture(const WCHAR * pTitleName)
{
	// Create a font with the title name.
	m_pTitleFont = g_FontMgr.CreateGuiFont(DEFAULT_FONT, dNTL_PLAYERNAME_FONTSIZE_TITLE, DEFAULT_FONT_ATTR);
	if (m_pTitleFont == NULL)
	{
		NTL_RETURN(FALSE);
	}

	// Texture Resize ( Delete & Create )
	if (m_pTexture)
	{
		g_TextureMgr.DeleteTexture(m_pTexture);
		m_pTexture = NULL;
	}

	if ((RwInt32)wcslen(pTitleName) <= 0)
	{
		NTL_RETURN(FALSE);
	}

	// Save the size of the text and create a texture for the size.
	CPos size = m_pTitleFont->GetTextWSize(pTitleName, (int)wcslen(pTitleName));
	m_posTitleNameSize.SetPos(size.x, size.y);

	RwInt32 nMaxWidth = m_posNameSize.x > m_posGuildNameSize.x ? m_posNameSize.x : m_posGuildNameSize.x;

	m_pTexture = g_TextureMgr.CreateTexture(nMaxWidth + m_posTitleNameSize.x + dPLAYER_NAME_CREATE_TEXTURE_OFFSET, m_posTitleNameSize.y + PLAYERNAME_LINE_INTERVAL_Y);
	if (m_pTexture == NULL)
		NTL_RETURN(FALSE);

	RwTextureSetFilterMode(m_pTexture->GetTexture(), rwFILTERLINEAR);
	RwTextureSetAddressingU(m_pTexture->GetTexture(), rwTEXTUREADDRESSWRAP);
	RwTextureSetAddressingV(m_pTexture->GetTexture(), rwTEXTUREADDRESSWRAP);

	NTL_RETURN(TRUE);
}

/**
* \brief 길드 이름까지 출력하는 텍스쳐를 만든다.
* \param pGuildName	(const WCHAR*) 길드이름의 문자열
* \return 성공여부
*/
RwBool CNtlPLPlayerName::CreateGuildNameTexture(const WCHAR* pGuildName) 
{
	// Create a font with the guild name.
	m_pGuildFont = g_FontMgr.CreateGuiFont( DEFAULT_FONT, dNTL_PLAYERNAME_FONTSIZE_GUILD, DEFAULT_FONT_ATTR ); 
	if(m_pGuildFont == NULL)
	{
		NTL_RETURN(FALSE);
	}

	// Texture Resize ( Delete & Create )
	if( m_pTexture )
	{
		g_TextureMgr.DeleteTexture(m_pTexture);
		m_pTexture = NULL;
	}

	if ((RwInt32)wcslen(pGuildName) <= 0)
	{
		NTL_RETURN(FALSE);
	}
	

	// Save the size of the text and create a texture for the size.
	CPos size = m_pGuildFont->GetTextWSize(pGuildName, (int)wcslen(pGuildName));
	m_posGuildNameSize.SetPos( size.x, size.y );

	RwInt32 nMaxWidth = (m_posNameSize.x + m_posTitleNameSize.x + dPLAYER_NAME_CREATE_TEXTURE_OFFSET > m_posGuildNameSize.x) ? m_posNameSize.x + m_posTitleNameSize.x + dPLAYER_NAME_CREATE_TEXTURE_OFFSET : m_posGuildNameSize.x + dPLAYER_NAME_CREATE_TEXTURE_OFFSET;

	m_pTexture = g_TextureMgr.CreateTexture( nMaxWidth, m_posNameSize.y + m_posGuildNameSize.y + PLAYERNAME_LINE_INTERVAL_Y );
	if( m_pTexture == NULL )
		NTL_RETURN(FALSE);

	RwTextureSetFilterMode(m_pTexture->GetTexture(), rwFILTERLINEAR);
	RwTextureSetAddressingU(m_pTexture->GetTexture(), rwTEXTUREADDRESSWRAP);
	RwTextureSetAddressingV(m_pTexture->GetTexture(), rwTEXTUREADDRESSWRAP);

	NTL_RETURN(TRUE);
}

/**
* \brief 길드 엠블렘의 텍스쳐를 만든다. ( EmblemMaker에서 만들어진 텍스쳐의 포인터만 가지고 온다.)
* \param pEmblemFactor	(sEmblemFactor*) 엠블렘 정보 구조체
* \return 성공여부
*/
RwBool CNtlPLPlayerName::CreateGuildEmblemTexture(sEmblemFactor *pEmblemFactor) 
{
	// If there is an existing EmblemTexture, ask the emblem maker to reduce the reference count by one.
	if( m_pEmblemTexture )
	{
		GetEmblemMaker()->DestoyEmblem( m_pEmblemTexture );
		m_pEmblemTexture = NULL;
	}

	// If Emblem Texture returns NULL, the emblem is not registered.
	m_pEmblemTexture = GetEmblemMaker()->CreateEmblem( pEmblemFactor );
	if( m_pEmblemTexture == NULL )
		return FALSE;

	return TRUE;
}

RwBool CNtlPLPlayerName::CreateNickNameTexture( const WCHAR* pNickName )
{
	// 길드 이름의 폰트를 만든다.
	m_pNickFont = g_FontMgr.CreateGuiFont( DEFAULT_FONT, dNTL_PLAYERNAME_FONTSIZE_NICK, DEFAULT_FONT_ATTR ); 
	if(m_pNickFont == NULL)
	{
		NTL_RETURN(FALSE);
	}

	// Texture Resize ( Delete & Create )
	if( m_pNickTexture )
	{
		g_TextureMgr.DeleteTexture(m_pNickTexture);
		m_pNickTexture = NULL;
	}

	// 글자의 사이즈를 저장하고 사이즈에 알맞은 텍스쳐를 만든다.
	CPos size = m_pNickFont->GetTextWSize( pNickName, (int)wcslen(pNickName)); 
	m_posNickNameSize.SetPos( size.x, size.y );

	m_pNickTexture = g_TextureMgr.CreateTexture( m_posNickNameSize.x + dPLAYER_NAME_CREATE_TEXTURE_OFFSET , m_posNickNameSize.y );
	if( m_pNickTexture == NULL )
		NTL_RETURN(FALSE);

	RwTextureSetFilterMode(m_pNickTexture->GetTexture(), rwFILTERLINEAR);
	RwTextureSetAddressingU(m_pNickTexture->GetTexture(), rwTEXTUREADDRESSWRAP);
	RwTextureSetAddressingV(m_pNickTexture->GetTexture(), rwTEXTUREADDRESSWRAP);

	NTL_RETURN(TRUE);
}

/**
* \brief Renders the name or guild name.
* \return Success
*/
RwBool CNtlPLPlayerName::RenderName( void ) 
{
	if(m_pTexture)
		RwD3D9SetTexture(m_pTexture->GetTexture(), 0);
	else
	{
		RwD3D9SetTexture(NULL, 0);
		return FALSE; // m_pTexture가 NULL이면 렌더링하지 않는다.
	}

	RwUInt32 nTransformFlags = rwIM3D_VERTEXUV | rwIM3D_ALLOPAQUE;

	if( RwIm3DTransform(m_3dVertices, 4, &CNtlPLGlobal::m_matIden, nTransformFlags) )
	{                         
		RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);  

		RwIm3DEnd();
	}

	return TRUE;
}

/**
* \brief 엠블렘을 렌더링한다.
* \param 성공 여부
*/
RwBool CNtlPLPlayerName::RenderEmblem( void ) 
{
	if( !(m_byFlag & dNTL_PL_FLAG_EMBLEM) )
		return FALSE;

	if(m_pEmblemTexture)
		RwD3D9SetTexture(m_pEmblemTexture, 0);
	else
	{
		RwD3D9SetTexture(NULL, 0);
		return FALSE; // m_pTexture가 NULL이면 렌더링하지 않는다.
	}

	RwUInt32 nTransformFlags = rwIM3D_VERTEXUV | rwIM3D_ALLOPAQUE;

	if( RwIm3DTransform(m_3dEmblemVertices, 4, &CNtlPLGlobal::m_matIden, nTransformFlags) )
	{                         
		RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);  

		RwIm3DEnd();
	}

	return TRUE;
}

RwBool CNtlPLPlayerName::RenderNick( void )
{
	if(m_pNickTexture)
		RwD3D9SetTexture(m_pNickTexture->GetTexture(), 0);
	else
	{
		RwD3D9SetTexture(NULL, 0);
		return FALSE; // m_pNickTexture가 NULL이면 렌더링하지 않는다.
	}

	RwUInt32 nTransformFlags = rwIM3D_VERTEXUV | rwIM3D_ALLOPAQUE;

	if( RwIm3DTransform(m_3dNickVertices, 4, &CNtlPLGlobal::m_matIden, nTransformFlags) )
	{                         
		RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);  

		RwIm3DEnd();
	}

	return TRUE;
}

/**
* \brief 이름과 엠블렘 포지션을 업데이트 한다.
* \param pPos	(const RwV3d) 정렬의 기준이 될 포지션
*/
void CNtlPLPlayerName::SetPosition(const RwV3d *pPos)
{
	// position을 update 안할 경우.
	if(!IsVisible())
		return;

	// World Position을 지정
	m_v3WorldPosition = *pPos;
}

/**
* \brief 현재 월드 행렬의 좌표를 리턴
* \return 월드 행렬의 좌표 ( 카메라의 역행렬 )
*/
RwV3d CNtlPLPlayerName::GetPosition(void)
{
	return m_matWorld.pos;
}
	
void CNtlPLPlayerName::SetNameColor(const WCHAR* pwcName, COLORREF color)
{
	if( m_pTexture )
		m_pTexture->FillClear();

	m_pFont->TextOutW(L"                ");
	m_pFont->TextOutW(pwcName);

	if( m_pTexture )
	{
		CRectangle rect;
		CPos size = m_pFont->GetTextWSize(pwcName, (int)wcslen(pwcName));

		rect.left	= (m_pTexture->GetWidth()-size.x)/2;
		rect.top	= (m_pTexture->GetHeight()-size.y)/2;
		rect.right	= rect.left + size.x;
		rect.bottom = rect.top + size.y;

		// texture에 string copy
		g_FontMgr.BitmapToTexture(m_pTexture, rect, color, TE_OUTLINE, 0, 2);
	}
}

/**
* \brief 이름과 컬러를 지정해준다.
* \param pwcName		(const WCHAR*) 이름
* \param color			(COLORREF) 이름의 색상
* \param pwcGuildName	(const WCHAR*) 길드 이름
* \param guildcolor		(COLORREF) 길드 이름의 색상
* \param pEmblemFactor	(sEmblemFactor*) 엠블렘 정보의 구조체
*/
void CNtlPLPlayerName::SetNameColor( const WCHAR* pwcName, COLORREF color, const WCHAR* pwcGuildName, COLORREF guildcolor
									,sEmblemFactor* pEmblemFactor, const WCHAR* pwcTitleName, COLORREF titlecolor)
{
	if( m_pTexture )
		m_pTexture->FillClear();

	// Judge name validity
	if( pwcName == NULL )
		return;
	else if( (RwInt32)wcslen( pwcName ) <= 0 )
		return;

	if (pwcTitleName == NULL || wcslen(pwcTitleName) == 0)
	{
		m_posTitleNameSize.SetPos(0, 0);
		m_byFlag &= ~dNTL_PL_FLAG_TITLE;
	}

	WCHAR wchGuildName[NTL_MAX_SIZE_GUILD_NAME * 2 + 1];

	// judgment
	if( pwcGuildName == NULL )
	{
		if( CreateNameTexture( pwcName ) )
			m_byFlag = dNTL_PL_FLAG_NAME;

		if (pwcTitleName && wcslen(pwcTitleName) > 0)
		{
			if (CreateTitleNameTexture(pwcTitleName))
				m_byFlag |= dNTL_PL_FLAG_TITLE;
		}
	}
	else if( (RwInt32)wcslen( pwcGuildName ) > 0 )
	{
		if (pwcTitleName && wcslen(pwcTitleName) > 0)
		{
			if (CreateTitleNameTexture(pwcTitleName))
				m_byFlag |= dNTL_PL_FLAG_TITLE;
		}

		swprintf_s(wchGuildName, L"<%s>", pwcGuildName);

		// If the guild name is successfully created, the emblem is judged.
		if( CreateGuildNameTexture(wchGuildName) )
		{
			m_byFlag |= dNTL_PL_FLAG_GUILD;

			if( CreateGuildEmblemTexture( pEmblemFactor ) )
				m_byFlag |= dNTL_PL_FLAG_EMBLEM;
		}
	}
	// If the guild name pointer is NULL and the buffer is 0, only the name is printed.
	else
	{
		if( CreateNameTexture( pwcName ) )
			m_byFlag = dNTL_PL_FLAG_NAME;

		if (pwcTitleName && wcslen(pwcTitleName) > 0)
		{
			if (CreateTitleNameTexture(pwcTitleName))
				m_byFlag |= dNTL_PL_FLAG_TITLE;
		}
	}

	if( m_pNickTexture )
		m_byFlag |= dNTL_PL_FLAG_NICK;

	if( m_pTexture )
	{
		CRectangle rect;

		if( m_byFlag & dNTL_PL_FLAG_EMBLEM )
		{
			// create guild name
			m_pGuildFont->TextOutW(L"                                ");
			m_pGuildFont->TextOutW(wchGuildName);

			CPos size2 = m_pGuildFont->GetTextWSize(wchGuildName, (int)wcslen(wchGuildName));

			rect.left = 1;
			rect.top = 0;
			rect.right = rect.left + size2.x;
			rect.bottom = rect.top + size2.y;

			g_FontMgr.BitmapToTexture(m_pTexture, rect, guildcolor, TE_OUTLINE, 0, 2);

			LONG lLeft = 1;

			if (m_byFlag & dNTL_PL_FLAG_TITLE)
			{
				m_pTitleFont->TextOutW(L"                                ");
				m_pTitleFont->TextOutW(pwcTitleName);

				CPos size3 = m_pTitleFont->GetTextWSize(pwcTitleName, (int)wcslen(pwcTitleName));

				rect.left = 1;
				rect.top = size2.y + PLAYERNAME_LINE_INTERVAL_Y;
				rect.right = rect.left + size3.x;
				rect.bottom = rect.top + size3.y;

				lLeft = rect.right + dPLAYER_NAME_CREATE_TEXTURE_OFFSET;

				g_FontMgr.BitmapToTexture(m_pTexture, rect, titlecolor, TE_OUTLINE, 0, 2);
			}

			m_pFont->TextOutW(L"                ");
			m_pFont->TextOutW(pwcName);

			CPos size = m_pFont->GetTextWSize(pwcName, (int)wcslen(pwcName));

			rect.left = lLeft;
			rect.top = size2.y + PLAYERNAME_LINE_INTERVAL_Y;
			rect.right = rect.left + size.x;
			rect.bottom = rect.top + size.y;

			g_FontMgr.BitmapToTexture(m_pTexture, rect, color, TE_OUTLINE, 0, 2);
		}
		else if( m_byFlag & dNTL_PL_FLAG_GUILD )
		{
			// create guild name
			m_pGuildFont->TextOutW(L"                                ");
			m_pGuildFont->TextOutW(wchGuildName);

			CPos size2 = m_pGuildFont->GetTextWSize(wchGuildName, (int)wcslen(wchGuildName));

			rect.left = (m_pTexture->GetWidth() - size2.x) / 2;
			rect.top = 0;
			rect.right = rect.left + size2.x;
			rect.bottom = rect.top + size2.y;

			g_FontMgr.BitmapToTexture(m_pTexture, rect, guildcolor, TE_OUTLINE, 0, 2);

			CPos size = m_pFont->GetTextWSize(pwcName, (int)wcslen(pwcName));

			LONG lLeft = (m_pTexture->GetWidth() - size.x) / 2;

			if (m_byFlag & dNTL_PL_FLAG_TITLE)
			{
				m_pTitleFont->TextOutW(L"                                ");
				m_pTitleFont->TextOutW(pwcTitleName);

				CPos size3 = m_pTitleFont->GetTextWSize(pwcTitleName, (int)wcslen(pwcTitleName));

				rect.left = (m_pTexture->GetWidth() - (size.x+size3.x)) / 2;
				rect.top = size2.y + PLAYERNAME_LINE_INTERVAL_Y;
				rect.right = rect.left + size3.x;
				rect.bottom = rect.top + size3.y;

				lLeft = rect.right + dPLAYER_NAME_CREATE_TEXTURE_OFFSET;

				g_FontMgr.BitmapToTexture(m_pTexture, rect, titlecolor, TE_OUTLINE, 0, 2);
			}

			m_pFont->TextOutW(L"                ");
			m_pFont->TextOutW(pwcName);

			rect.left = lLeft;
			rect.top = size2.y + PLAYERNAME_LINE_INTERVAL_Y;
			rect.right	= rect.left + size.x;
			rect.bottom = rect.top + size.y;

			// create texture
			g_FontMgr.BitmapToTexture(m_pTexture, rect, color, TE_OUTLINE, 0, 2);
		}
		else
		{
			CPos size = m_pFont->GetTextWSize(pwcName, (int)wcslen(pwcName));

			LONG lLeft = (m_pTexture->GetWidth() - size.x) / 2;

			if (m_byFlag & dNTL_PL_FLAG_TITLE)
			{
				m_pTitleFont->TextOutW(L"                                ");
				m_pTitleFont->TextOutW(pwcTitleName);

				CPos size3 = m_pTitleFont->GetTextWSize(pwcTitleName, (int)wcslen(pwcTitleName));

				rect.left = (m_pTexture->GetWidth() - (size.x + size3.x)) / 2;
				rect.top = (m_pTexture->GetHeight() - size.y) / 2;
				rect.right = rect.left + size3.x;
				rect.bottom = rect.top + size3.y;

				lLeft = rect.right + dPLAYER_NAME_CREATE_TEXTURE_OFFSET;

				g_FontMgr.BitmapToTexture(m_pTexture, rect, titlecolor, TE_OUTLINE, 0, 2);
			}

			m_pFont->TextOutW(L"                ");
			m_pFont->TextOutW(pwcName);

			rect.left = lLeft;
			rect.top = (m_pTexture->GetHeight() - size.y) / 2;
			rect.right = rect.left + size.x;
			rect.bottom = rect.top + size.y;

			// create texture
			g_FontMgr.BitmapToTexture(m_pTexture, rect, color, TE_OUTLINE, 0, 2);
		}
	}
}

RwBool CNtlPLPlayerName::IsExistEmblemTexture()
{
	if( m_pEmblemTexture )
		return true;

	return false;
}

void CNtlPLPlayerName::SetVisible( RwBool bVisible )
{
	if( bVisible )
	{
		// SetVisible(TRUE) 를 했을 때 NameVisible 이 TRUE 라면 이름을 보여준다.
		if( m_bNameVisible )
		{
			CNtlPLEntity::SetVisible( bVisible );
		}
	}
	else
	{
		// SetVisible(FALSE) 를 했을 때는 무조건 꺼준다.
		CNtlPLEntity::SetVisible( bVisible );
	}
}

void CNtlPLPlayerName::EnablePlayerNameVisible( RwBool bVisible )
{
	SetVisible( bVisible );

	m_bNameVisible = bVisible;
}

RwBool CNtlPLPlayerName::IsEnablePlayerNameVisible()
{
	return m_bNameVisible;
}

void CNtlPLPlayerName::SetCameraCullin( RwBool bCullin )
{
	m_bCameraCullIn = bCullin;
}

void CNtlPLPlayerName::SetNickNameColor( const WCHAR* pwcTitle, COLORREF color )
{
	if( m_pNickTexture )
		m_pNickTexture->FillClear();

	// 이름 유효성 판단
	if( pwcTitle == NULL || (RwInt32)wcslen( pwcTitle ) <= 0 )
	{
		if( m_pNickTexture )
		{
			g_TextureMgr.DeleteTexture(m_pNickTexture);
			m_pNickTexture = NULL;
		}

		return;
	}	

	// 판단
	if( CreateNickNameTexture( pwcTitle ) )
	{
		m_byFlag |= dNTL_PL_FLAG_NICK;
	}
	

	m_pNickFont->TextOutW(L"                ");
	m_pNickFont->TextOutW(pwcTitle);

	if( m_pNickTexture )
	{
		CRectangle rect;
		CPos size = m_pNickFont->GetTextWSize(pwcTitle, (int)wcslen(pwcTitle));

		rect.left	= (m_pNickTexture->GetWidth()-size.x)/2;
		rect.top	= (m_pNickTexture->GetHeight()-size.y)/2;
		rect.right	= rect.left + size.x;
		rect.bottom = rect.top + size.y;

		// texture에 string copy
		g_FontMgr.BitmapToTexture(m_pNickTexture, rect, color, TE_OUTLINE, 0, 2);
	}
}