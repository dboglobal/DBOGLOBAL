#include "precomp_ntlsimulation.h"
#include "NtlSLLogic.h"

// core
#include "NtlDebug.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

#include "NtlPLOptionManager.h"
#include "NtlInstanceEffect.h"
#include "NtlPLCharacter.h"


// Simulation
#include "NtlSobManager.h"
#include "NtlApplication.h"
#include "NtlPostEffectCamera.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSobCharProxyDecoration.h"
#include "NtlCameraManager.h"

// Consider applying it according to Flags.

bool Logic_ApplyNtlStorageUnit( CNtlStorageUnit* pUnit, unsigned int uiFlags )
{
	eNTL_STORAGE_TYPE eType = pUnit->GetType();

	switch( eType )
	{
		case eNTL_STORAGE_GRAPHIC:
		{
			// This flag is not checked by eNTL_STRORAG_APPLY_ALL.
			if( uiFlags & eNTL_STORAGE_APPLY_PRESENTATION_INITONCE )
			{
				// 텍스텨 해상도 조절

				RwInt32 nTextureLevel = pUnit->GetIntData( dSTORAGE_GRAPHIC_TEXTURE_LEVEL );
				RwUInt32 uiTextureQualityFactor = 0 /* Default value */ ;

				switch( nTextureLevel )
				{
				case 0: uiTextureQualityFactor = 2; break;		// Set any value
				case 1: uiTextureQualityFactor = 1; break;
				case 2: uiTextureQualityFactor = 0; break;
				default: uiTextureQualityFactor = 0; break;
				}

				// Todo : Genuine variant
				if( GetNtlPLOptionManager()->GetTextureQuality() != uiTextureQualityFactor )
					GetNtlPLOptionManager()->SetTextureQuality( uiTextureQualityFactor );
				
			}

			if( uiFlags & eNTL_STORAGE_APPLY_PRESENTATION )
			{
				// Hdr
				if(pUnit->GetBoolData( dSTORAGE_GRAPHIC_SHADER_HDR ))
					CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
				else
					CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);

				// 지형 시야거리 ( 1lv : 200, 2lv : 300, 3lv : 400, 4lv : 512 )
				RwInt32 nTerrainRangeLevel = pUnit->GetIntData( dSTORAGE_GRAPHIC_TERRAIN_RANGE );
				RwReal fTerranRange = 512.f;
				switch( nTerrainRangeLevel )
				{
					case 0: fTerranRange = 200.f; break;
					case 1: fTerranRange = 300.f; break;
					case 2: fTerranRange = 400.f; break;
					case 3: fTerranRange = 512.f; break;
					default: fTerranRange = 512.f; break;
				}
				if( GetNtlPLOptionManager()->GetTerrainFar() != fTerranRange )
					GetNtlPLOptionManager()->SetTerrainFar( fTerranRange );

				// 사물 시야거리 : 200, 300, 400, 512 )
				RwInt32 nObjectRangeLevel = pUnit->GetIntData( dSTORAGE_GRAPHIC_OBJECT_RANGE );
				RwReal fObjectRange = 512.f;
				switch( nObjectRangeLevel )
				{
					case 0: fObjectRange = 200.f; break;
					case 1: fObjectRange = 300.f; break;
					case 2: fObjectRange = 400.f; break;
					case 3: fObjectRange = 512.f; break;
					default: fObjectRange = 512.f; break;
				}

				if( GetNtlPLOptionManager()->GetObjectFar() != fObjectRange )
					GetNtlPLOptionManager()->SetObjectFar( fObjectRange );


				// 지형 그림자
				RwBool bTerrainShadow = pUnit->GetBoolData( dSTORAGE_GRAPHIC_TERRAIN_SHADOW );
				if( GetNtlPLOptionManager()->GetTerrainShadow() != bTerrainShadow )
					GetNtlPLOptionManager()->SetTerrainShadow( bTerrainShadow );

				// 물 효과
				RwBool bWaterEffect = pUnit->GetBoolData( dSTORAGE_GRAPHIC_WATER_EFFECT );
				if( GetNtlPLOptionManager()->GetWaterSpecular() != bWaterEffect )
					GetNtlPLOptionManager()->SetWaterSpecular( bWaterEffect );

				// 캐릭터 업그레이드 효과
				/*RwBool bCharacterUpgrade = pUnit->GetBoolData( dSTORAGE_GRAPHIC_CHARACTER_EFFECT );
				CNtlPLCharacter::ToggleEMUVAni(bCharacterUpgrade);*/

				// 캐릭터 외곽선
				RwBool bCharacterEdge = pUnit->GetBoolData( dSTORAGE_GRAPHIC_CHARACTER_EDGE );
				if( bCharacterEdge )
					CNtlPLCharacter::SetSkipEdge( FALSE );
				else
					CNtlPLCharacter::SetSkipEdge( TRUE );

				// Magic Detail Effect
				RwInt32 nMagicEffectLevel = pUnit->GetIntData( dSTORAGE_GRAPHIC_MAGIC_EFFECT );
				RwReal fParticleRatio = 1.f;
				RwBool bMesh = TRUE;
				RwBool bDecal = TRUE;
				switch( nMagicEffectLevel )
				{
				case 0: fParticleRatio = 0.1f; bMesh = FALSE; bDecal = FALSE; break;
				case 1: fParticleRatio = 0.4f; bMesh = TRUE; bDecal = FALSE; break;
				case 2: fParticleRatio = 0.7f; bMesh = TRUE; bDecal = FALSE; break;
				case 3: fParticleRatio = 1.f; bMesh = TRUE; bDecal = TRUE; break;
				default: fParticleRatio = 1.f; bMesh = TRUE; bDecal = TRUE; break;
				}
				if( CNtlInstanceEffect::GetLowSpecRatio() != fParticleRatio )
					CNtlInstanceEffect::SetLowSpecRatio(fParticleRatio);

				if( CNtlInstanceEffect::GetRenderMeshSystem() != bMesh )
					CNtlInstanceEffect::SetRenderMeshSystem(bMesh);

				if( CNtlInstanceEffect::GetRenderDecalSystem() != bDecal )
					CNtlInstanceEffect::SetRenderDecalSystem(bDecal);

				// Weather effects
				RwInt32 nWeatherEffect = pUnit->GetIntData( dSTORAGE_GRAPHIC_WEATHER_EFFECT );
				RwUInt32 uiWeatherLevel = 0;
				switch( nWeatherEffect )
				{
					case 0: uiWeatherLevel = 1; break;
					case 1: uiWeatherLevel = 0; break;
					default: uiWeatherLevel = 0; break;
				}

				if( GetNtlPLOptionManager()->GetWeatherLevel() != uiWeatherLevel )
					GetNtlPLOptionManager()->SetWeatherLevel( uiWeatherLevel );
			}

			if( uiFlags & eNTL_STORAGE_APPLY_SIMULATION )
			{
				//// 캐릭터 시야거리 ( 20, 60, 100, 150 )
				//RwInt32 nCharacterRangeLevel = pUnit->GetIntData( dSTORAGE_GRAPHIC_CHARACTER_RANGE );
				//RwReal fCharacterRange = 150.f;
				//switch( nCharacterRangeLevel )
				//{
				//case 0: fCharacterRange = 20.f; break;
				//case 1: fCharacterRange = 60.f; break;
				//case 2: fCharacterRange = 100.f; break;
				//case 3: fCharacterRange = 150.f; break;
				//default: fCharacterRange = 150.f; break;
				//}
				//if( CNtlSobManager::GetOptionOutRange() != fCharacterRange )
				//	CNtlSobManager::SetOptionOutRange(fCharacterRange);

				// Character Shadow
				RwBool bCharacterShadow = pUnit->GetBoolData( dSTORAGE_GRAPHIC_CHARACTER_SHADOW );
				if( CNtlSobCharDecorationProxy::m_bShadowRender != bCharacterShadow )
					GetNtlSobManager()->GameOptionShadowOnOff( bCharacterShadow );
			}

			if( uiFlags & eNTL_STORAGE_APPLY_CLIENT )
			{
				// Window mode or full screen mode
				RwBool bFullScreen = CNtlApplication::GetInstance()->IsFullScreen();
				RwBool bUserScreen = pUnit->GetBoolData( dSTORAGE_GRAPHIC_WINDOW_MODE );
				if( bUserScreen && bFullScreen )
					CNtlApplication::GetInstance()->ToggleFullMode();
				else if( !bUserScreen && !bFullScreen )
					CNtlApplication::GetInstance()->ToggleFullMode();

				// If you are in full screen mode after toggle, take resolution and apply.
				bFullScreen = CNtlApplication::GetInstance()->IsFullScreen();
				RwInt32 nWidth = pUnit->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH );
				RwInt32 nHeight = pUnit->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT );
				RwInt32 nDepth = pUnit->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH );


				// VideoMode
				//if( bFullScreen )
				//{
					if( nWidth != CNtlApplication::GetInstance()->GetUserWidth() ||
						nHeight != CNtlApplication::GetInstance()->GetUserHeight() ||
						nDepth != CNtlApplication::GetInstance()->GetBitDepth() )
						CNtlApplication::GetInstance()->ChangeVideoMode( nWidth, nHeight, nDepth );
				/*}*/

				// Gamma
				CNtlApplication::GetInstance()->SetGammaRamp( pUnit->GetFloatData(dSTORAGE_GRAPHIC_GAMMA) );

				// FPS
				CNtlApplication::GetInstance()->SetShowFps(pUnit->GetBoolData(dSTORAGE_GRAPHIC_SHOW_FPS));

				// FPS LIMIT
				CNtlApplication::GetInstance()->SetFrameRate(pUnit->GetIntData(dSTORAGE_GRAPHIC_FPS));
			}
		}
		break;
	case eNTL_STORAGE_SOUND:
		{
			if( uiFlags & eNTL_STORAGE_APPLY_CLIENT )
			{
				// Sound
				float fMainVolume = pUnit->GetFloatData( dSTORAGE_SOUND_MAIN_VOLUME );
				float fBGMVolume = pUnit->GetFloatData( dSTORAGE_SOUND_BACK_VOLUME );
				float fSEVolume = pUnit->GetFloatData( dSTORAGE_SOUND_EFFECT_VOLUME );
				float fENVVolume = pUnit->GetFloatData( dSTORAGE_SOUND_ENV_VOLUME );

				GetSoundManager()->SetMasterVolume(fMainVolume);
				CNtlSLEventGenerator::GameChatOption( GAME_OPTION_MOVIE_VOLUME, (RwUInt8(fMainVolume * 100.f)) );
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_BGM, fBGMVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_FLASH_MUSIC, fBGMVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_WEATHER_MUSIC, fBGMVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AMBIENT_MUSIC, fBGMVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_JINGLE_MUSIC, fBGMVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AVATAR_VOICE_SOUND, fSEVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AVATAR_EFFECT_SOUND, fSEVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_VOICE_SOUND, fSEVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_EFFECT_SOUND, fSEVolume);
				GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_OBJECT_MUSIC, fENVVolume);
			}
		}
		break;
	case eNTL_STORAGE_GAMEINFO:
		{
			if( uiFlags & eNTL_STORAGE_APPLY_PRESENTATION )
			{
				// Scouter
				CNtlPLGlobal::m_bRenderScouter = pUnit->GetBoolData( dSTORAGE_GAMEINFO_SCOUTER_VISIBLE );
			}
		}
		break;
	case eNTL_STORAGE_ETC:
		{
			if( uiFlags & eNTL_STORAGE_APPLY_SIMULATION )
			{
				if( pUnit->GetBoolData( dSTORAGE_ETC_MOVEABLEWINDOW ))
					CNtlSLEventGenerator::DialogEvent( TRM_DIALOG_DIALOG_MOVABLE );
				else
					CNtlSLEventGenerator::DialogEvent( TRM_DIALOG_DIALOG_DISMOVABLE );

				API_SetDoubleClickInterval( pUnit->GetFloatData( dSTORAGE_ETC_MOUSE_DASH ) );
			}

			float fFov = NTL_CAMERA_DEFAULT_FOV + pUnit->GetFloatData(dSTORAGE_ETC_MOUSE_FOV);
			if (fFov > NTL_CAMERA_DEFAULT_FOV + NTL_CAMERA_FOV_MAX_BONUS)
				fFov = NTL_CAMERA_DEFAULT_FOV + NTL_CAMERA_FOV_MAX_BONUS;

			GetNtlGameCameraManager()->SetFov(fFov);

			GetNtlGameCameraManager()->SetHeightRatioBonus(pUnit->GetFloatData(dSTORAGE_ETC_MOUSE_VIEW_POINT));
		}
		break;
	case eNTL_STORAGE_CHAT:
		{
			if( uiFlags & eNTL_STORAGE_APPLY_SIMULATION )
			{
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_INPUTMODE, pUnit->GetBoolData( dSTORAGE_CHAT_INPUTMODE ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_WINDOWLOCK, pUnit->GetBoolData( dSTORAGE_CHAT_WINDOW_LOCK ) );
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_ADDEDWINDOW, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND_WINDOW), 1 );
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_ADDEDWINDOW, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_WINDOW), 2);
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_WINDOWOPACITY, (RwUInt8)pUnit->GetIntData( dSTORAGE_CHAT_WINDOW_OPACITY ) );

				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_NORMAL, 0, pUnit->GetBoolData( dSTORAGE_CHAT_BASIC_NORMAL ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_WISPHER, 0, pUnit->GetBoolData( dSTORAGE_CHAT_BASIC_WISPHER ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_PARTY, 0, pUnit->GetBoolData( dSTORAGE_CHAT_BASIC_PARTY ) );
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_FIND_PARTY, 0, pUnit->GetBoolData(dSTORAGE_CHAT_BASIC_FIND_PARTY));
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_GUILD, 0, pUnit->GetBoolData( dSTORAGE_CHAT_BASIC_GUILD ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_TRADE, 0, pUnit->GetBoolData( dSTORAGE_CHAT_BASIC_TRADE ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_SHOUT, 0, pUnit->GetBoolData( dSTORAGE_CHAT_BASIC_SHOUT ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_SYSTEM, 0, pUnit->GetBoolData( dSTORAGE_CHAT_BASIC_SYSTEM ) );

				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_NORMAL, 1, pUnit->GetBoolData( dSTORAGE_CHAT_EXTEND_NORMAL ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_WISPHER, 1, pUnit->GetBoolData( dSTORAGE_CHAT_EXTEND_WISPHER ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_PARTY, 1, pUnit->GetBoolData( dSTORAGE_CHAT_EXTEND_PARTY ) );
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_FIND_PARTY, 1, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND_FIND_PARTY));
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_GUILD, 1, pUnit->GetBoolData( dSTORAGE_CHAT_EXTEND_GUILD ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_TRADE, 1, pUnit->GetBoolData( dSTORAGE_CHAT_EXTEND_TRADE ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_SHOUT, 1, pUnit->GetBoolData( dSTORAGE_CHAT_EXTEND_SHOUT ) );
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_SYSTEM, 1, pUnit->GetBoolData( dSTORAGE_CHAT_EXTEND_SYSTEM ) );

				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_NORMAL, 2, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_NORMAL));
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_WISPHER, 2, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_WISPHER));
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_PARTY, 2, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_PARTY));
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_FIND_PARTY, 2, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_FIND_PARTY));
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_GUILD, 2, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_GUILD));
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_TRADE, 2, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_TRADE));
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_SHOUT, 2, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_SHOUT));
				CNtlSLEventGenerator::GameChatOption(OPTION_CHAT_SYSTEM, 2, pUnit->GetBoolData(dSTORAGE_CHAT_EXTEND2_SYSTEM));

				// Chat Option을 리프레쉬하라.
				CNtlSLEventGenerator::GameChatOption( OPTION_CHAT_REFRESH, OPTION_CHAT_REFRESH );
			}
		}
		break;
	case eNTL_STORAGE_CHARACTER:
		{
			if( uiFlags & eNTL_STORAGE_APPLY_CLIENT )
			{
				// 퀵슬롯 관련 이벤트
				CNtlSLEventGenerator::QuickSlotGuiMode( pUnit->GetBoolData( dSTORAGE_ETC_EX_QUICKSLOT1 ), pUnit->GetBoolData( dSTORAGE_ETC_EX_QUICKSLOT2 ) );
				CNtlSLEventGenerator::QuickSlotLockMode( pUnit->GetBoolData( dSTORAGE_ETC_QUICKSLOT_LOCK ) );
			}
		}
		break;
	case eNTL_STORAGE_CHARACTER_SCOUTER:
	{
		if (uiFlags & eNTL_STORAGE_APPLY_CLIENT)
		{
			CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_STATUS, true, pUnit->GetBoolData(dSTORAGE_CHARACTER_SCOUTER_ON));
		}
	}
	break;
	default:
		break;
	}

	return true;
}