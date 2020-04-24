#include "precomp_soundenvironment.h"
#include "SoundInfo.h"

CSoundInfo::CSoundInfo()
{
}

CSoundInfo::~CSoundInfo()
{
}

bool CSoundInfo::Create()
{
	NTL_FUNCTION("CSoundInfo::Create");

	NTL_RETURN(true);
}

void CSoundInfo::Destory()
{
}

void CSoundInfo::ReportSoundEnvironment()
{
	FILE*					fp;
	FMOD::System*			pFMODSystem;
	FMOD_RESULT				result;

	if( fopen_s(&fp, "SoundInfo.txt", "wt") != 0 )
	{
		printf_s("Can not open 'SoundInfo.txt'");
		return;
	}

	// FMOD 시스템 생성
	result = FMOD::System_Create(&pFMODSystem);
	if( IsExistError(fp, result) )
	{
		fclose(fp);
		return;
	}
	

	DisplaySoundEnvironment(fp, pFMODSystem);


	pFMODSystem->close();
	pFMODSystem->release();
	fclose(fp);
}

void CSoundInfo::DisplaySoundEnvironment(FILE* fp, FMOD::System* pFMODSystem)
{
	unsigned int			uiVersion;				// FMOD 라이브러리 버전
	FMOD_SPEAKERMODE		SpeakerMode;			// 현재 컴퓨터의 스피커 모드
	char					acDriverName[256];		// 사운드 카드 이름
	int						iHardware2DChannels;	// 사운드 카드의 2D 채널
	int						iHardware3DChannels;	// 사운드 카드의 3D 채널
	int						iHardwareTotalChannels;	// 사운드 카드의 총 채널수
	
	FMOD_RESULT				result;
	FMOD_CAPS				caps;
	FMOD_SPEAKERMODE		userSetSpeakMode;	


	// 버전 체크
	result = pFMODSystem->getVersion(&uiVersion);
	if( IsExistError(fp, result) )
		return;

	// 사운드 드라이버 정보
	result = pFMODSystem->getDriverCaps(0, &caps, 0, 0, &SpeakerMode);
	if( IsExistError(fp, result) )
		return;

	// 컴퓨터의 스피커 모드
	result = pFMODSystem->setSpeakerMode(SpeakerMode);  /* Set the user selected speaker mode. */
	if( IsExistError(fp, result) )
		return;

	userSetSpeakMode = SpeakerMode;

	// 유저가 세팅한 사운드 모드를 사운드카드가 지원할 수 없으면 스테레오 모드로 플레이 한다
	result = pFMODSystem->init(100, FMOD_INIT_NORMAL, 0);    /* Replace with whatever channel count and flags you use! */
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
	{
		result = pFMODSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if( IsExistError(fp, result) )
			return;

		result = pFMODSystem->init(100, FMOD_INIT_NORMAL, 0); /* Replace with whatever channel count and flags you use! */
		if( IsExistError(fp, result) )
			return;
	}

	// 사운드 카드가 여러개일 시 가장 마지막(가장 최신)사운드 카드의 이름
	// 사운드 카드가 하나일 때도 주드라이버가 있다고 오류가 나와서 사운드
	// 카드 전부를 검사하지는 않는다
	int iCount;
	result = pFMODSystem->getNumDrivers(&iCount);
	if( IsExistError(fp, result) )
		return;

	result = pFMODSystem->getDriverName(iCount - 1, acDriverName, 256);
	if( IsExistError(fp, result) )
		return;

	// 사운드 카드의 채널 갯수
	pFMODSystem->getHardwareChannels(&iHardware2DChannels, &iHardware3DChannels, &iHardwareTotalChannels);
	if( IsExistError(fp, result) )
		return;


	fprintf_s(fp, "FMOD Version : %x\n\n", uiVersion);
	printf_s("FMOD Version : %x\n\n", uiVersion);

	char pcText[256] = "";

	if( SpeakerMode != userSetSpeakMode )
	{
		fprintf_s(fp, "유저가 지정한 스피커 모드를 사운드 카드에서 지원하지 않습니다\n");
		printf("유저가 지정한 스피커 모드를 사운드 카드에서 지원하지 않습니다\n");

		GetSpeakerModeName(pcText, 256, userSetSpeakMode);
		fprintf_s(fp, "유저지정 스피커 모드 모드 : %s\n\n", pcText);
		printf("유저지정 스피커 모드 : %s\n\n", pcText);
	}

	GetSpeakerModeName(pcText, 256, SpeakerMode);
	fprintf_s(fp, "스피커 모드 : %s\n\n", pcText);
	printf("스피커 모드 : %s\n\n", pcText);



	fprintf_s(fp, "드라이버 : %s\n", acDriverName);
	printf("드라이버 : %s\n", acDriverName);

	fprintf_s(fp, "사운드 카드 2D 채널 : %d\n", iHardware2DChannels);
	printf("사운드 카드 2D 채널 : %d\n", iHardware2DChannels);

	fprintf_s(fp, "사운드 카드 3D 채널 : %d\n", iHardware3DChannels);
	printf("사운드 카드 3D 채널 : %d\n", iHardware3DChannels);

	fprintf_s(fp, "사운드 카드 총 채널 : %d\n", iHardwareTotalChannels);
	printf("사운드 카드 총 채널 : %d\n", iHardwareTotalChannels);
}

bool CSoundInfo::GetSpeakerModeName(char* pcResultText, int ibufferSize, FMOD_SPEAKERMODE mode)
{
	if( !pcResultText )
		return false;

	switch(mode)
	{
	case FMOD_SPEAKERMODE_RAW:
		{
			sprintf_s(pcResultText, ibufferSize, "%s", "There is no specific speakermode");
			break;
		}
	case FMOD_SPEAKERMODE_MONO:
		{
			sprintf_s(pcResultText, ibufferSize, "%s", "The speakers are monaural");
			break;
		}
	case FMOD_SPEAKERMODE_STEREO:
		{
			sprintf_s(pcResultText, ibufferSize, "%s", "The speakers are stereo (DEFAULT)");
			break;
		}
	case FMOD_SPEAKERMODE_QUAD:		
		{
			sprintf_s(pcResultText, ibufferSize, "%s", "4 speaker setup.\n\t  This includes front left, front right, rear left, rear right.");
			break;
		}
	case FMOD_SPEAKERMODE_SURROUND:
		{
			sprintf_s(pcResultText, ibufferSize, "%s", "4 speaker setup.\n\t  This includes front left, front right, center, rear center (rear left/rear right are averaged).");
			break;
		}
	case FMOD_SPEAKERMODE_5POINT1:
		{
			sprintf_s(pcResultText, ibufferSize, "%s", "5.1 speaker setup.\n\t  This includes front left, front right, center, \n\trear left, rear right and a subwoofer.");
			break;
		}
	case FMOD_SPEAKERMODE_7POINT1:
		{
			sprintf_s(pcResultText, ibufferSize, "%s", "7.1 speaker setup.\n\t  This includes front left, front right, center,\n\t rear left, rear right, side left, side right and a subwoofer.");
			break;
		}
	case FMOD_SPEAKERMODE_PROLOGIC:
		{
			sprintf_s(pcResultText, ibufferSize, "%s", "Stereo output, but data is encoded in a way\n\t that is picked up by a Prologic/Prologic2 decoder and split into a 5.1 speaker setup.");
			break;
		}
	default:
		return false;
	}

	return true;
}

bool CSoundInfo::IsExistError(FILE* fp, FMOD_RESULT result)
{
	static char acBuffer[256] = "";

	if( result != FMOD_OK )
	{
		sprintf_s(acBuffer, 256, "FMOD error! (%d) %s", result, FMOD_ErrorString(result));

		printf(acBuffer);
		fprintf_s(fp, "%s", acBuffer);

		return true;
	}

	return false;
}