/******************************************************************************
* File			: ChatSender.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 7. 2
* Abstract		: 
*****************************************************************************
* Desc			: 채팅 GUI를 통해서 입력받은 문자열의 파싱과 채팅 서버로 전송
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// dbo
#include "ChatDefine.h"

// simulation
#include "NtlSLDef.h"

class CChatGui;
class CNtlSobActionSkill;

struct sCHAT_COMMAND_TBLDAT;


class CChatSender
{
public:
	struct sSOCIAL_ACTION
	{
		CNtlSobActionSkill*			pSobActionSkill;
		sCHAT_COMMAND_TBLDAT*		pCHAT_COMMAND_TBLDAT;
	};

	// 실제로 명령어를 통해서 소셜 액션을 쓰는 일은 드물 것이라 생각이 되어
	// 소셜 명령어를 키로 하는 멀티맵을 사용하여 자료구조를 정의하지 않았다
	typedef std::map<SERIAL_HANDLE, sSOCIAL_ACTION*>		MAP_SOCIAL_ACTION;

public:	
	CChatSender();
	virtual ~CChatSender();

	RwBool		Create(CChatGui* pChatGui);
	VOID		Destroy();

	VOID		RegSocialAction(CNtlSobActionSkill* pSobActionSkill);
	VOID		UnregAllSocialAction();

	VOID		InputText(const WCHAR* pwcText);	

protected:
	VOID		Process_Send(const WCHAR* pwcText);
	eChatType	Process_Parsing(std::wstring& wstrOutputText, const WCHAR* pInputText, RwUInt8& bySendWorkType);

public:

	RwBool		HaveShortCut(std::wstring& wstrOutput, std::wstring& wstrOriginal,
								std::string strShortcutStringIndex, std::string strLocalShortcutStringIndex);

protected:

	SERIAL_HANDLE GetSocialActionHandle(std::wstring& wstrOriginal);

protected:
	CChatGui*			m_pChatGui;
	std::wstring		m_wstrLastUserName;				// 최근 귓속말을 받은 유저네임.
	MAP_SOCIAL_ACTION	m_mapSocialAction;
};