#include "stdafx.h"
#include "TQSNodeAction_BroadMessage.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeAction_BroadMessage::CTQSNodeAction_BroadMessage(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_BROAD_MESSAGE, lpszName)
{
}

CTQSNodeAction_BroadMessage::~CTQSNodeAction_BroadMessage()
{

}

CTQSNodeAction_BroadMessage::sPARAMETER_INFO* CTQSNodeAction_BroadMessage::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_BroadMessage::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "owner tblidx") == 0)
	{
		m_uiOwnerTblIdx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "speech index") == 0)
	{
		m_uiQuestTextTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "display time") == 0)
	{
		m_fDisplayTime = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_BroadMessage: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeAction_BroadMessage::AddParam(CControlScriptNodeParam_String* pNode)
{

	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();

	if (_stricmp(name, "execution type") == 0)
	{
		if (_stricmp(value, "start") == 0)
			m_eExecutionType = eBROAD_MSG_EXCUTION_TYPE_START;
		else if (_stricmp(value, "end") == 0)
			m_eExecutionType = eBROAD_MSG_EXCUTION_TYPE_END;
		else if (_stricmp(value, "time") == 0)
			m_eExecutionType = eBROAD_MSG_EXCUTION_TYPE_TIME;
		else
			printf("CTQSNodeAction_BroadMessage; value %s not found \n", value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "broad type") == 0)
	{
		if (_stricmp(value, "normal") == 0)
			m_eUIDirType = eBROAD_MSG_UI_DIR_TYPE_NORMAL;
		else if (_stricmp(value, "warning") == 0)
			m_eUIDirType = eBROAD_MSG_UI_DIR_TYPE_WARNING;
		else if (_stricmp(value, "danger") == 0)
			m_eUIDirType = eBROAD_MSG_UI_DIR_TYPE_DANGER;
		else
			printf("CTQSNodeAction_BroadMessage; value %s not found \n", value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "showhide type") == 0)
	{
		if (_stricmp(value, "normal") == 0)
			m_eUIShowHideType = eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL;
		else if (_stricmp(value, "fade") == 0)
			m_eUIShowHideType = eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE;
		else if (_stricmp(value, "slide") == 0)
			m_eUIShowHideType = eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE;
		else if (_stricmp(value, "fadeslide") == 0)
			m_eUIShowHideType = eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE;
		else
			printf("CTQSNodeAction_BroadMessage; value %s not found \n", value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "ballon shape type") == 0)
	{
		if (_stricmp(value, "normal") == 0)
			m_eUIBalloonShapeType = eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL;
		else if (_stricmp(value, "star") == 0)
			m_eUIBalloonShapeType = eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_STAR;
		else if (_stricmp(value, "think") == 0)
			m_eUIBalloonShapeType = eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_THINK;
		else
			printf("CTQSNodeAction_BroadMessage; value %s not found \n", value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "speech dir type") == 0)
	{
		if (_stricmp(value, "normal") == 0)
			m_eUISpeechDirType = eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL;
		else if (_stricmp(value, "blending") == 0)
			m_eUISpeechDirType = eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING;
		else if (_stricmp(value, "line") == 0)
			m_eUISpeechDirType = eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE;
		else
			printf("CTQSNodeAction_BroadMessage; value %s not found \n", value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "owner condition") == 0)
	{
		if (_stricmp(value, "condition_1") == 0)
			m_eOwnerCondition = eBROAD_MSG_OWNER_CONDITION_1;
		else if (_stricmp(value, "condition_2") == 0)
			m_eOwnerCondition = eBROAD_MSG_OWNER_CONDITION_2;
		else if (_stricmp(value, "condition_3") == 0)
			m_eOwnerCondition = eBROAD_MSG_OWNER_CONDITION_3;
		else if (_stricmp(value, "condition_4") == 0)
			m_eOwnerCondition = eBROAD_MSG_OWNER_CONDITION_4;
		else
			printf("CTQSNodeAction_BroadMessage; value %s not found \n", value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "owner type") == 0)
	{
		if (_stricmp(value, "npc") == 0)
			m_eOwnerType = eBROAD_MSG_OWNER_TYPE_NPC;
		else if (_stricmp(value, "mob") == 0)
			m_eOwnerType = eBROAD_MSG_OWNER_TYPE_MOB;
		else if (_stricmp(value, "pc") == 0)
			m_eOwnerType = eBROAD_MSG_OWNER_TYPE_PC;
		else
			printf("CTQSNodeAction_BroadMessage; value %s not found \n", value);

		return CControlScriptNode::AddParam(pNode);
	}


	printf("CTQSNodeAction_BroadMessage: [%s] not exist \n", name);
	return false;
}