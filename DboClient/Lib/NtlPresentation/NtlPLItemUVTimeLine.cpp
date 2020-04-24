#include "precomp_ntlpresentation.h"
#include "NtlPLItemUVTimeLine.h"
#include "NtlDebug.h"
#include "NtlPLResourceManager.h"
#include "NtlPLDef.h"


CNtlPLItemEMUVController::CNtlPLItemEMUVController()
{
	m_pEnvTex = NULL;
}

CNtlPLItemEMUVController::~CNtlPLItemEMUVController()
{
	RwTextureDestroy(m_pEnvTex);
	m_vecTBN.clear();
}

VOID CNtlPLItemEMUVController::SetActiveController(CNtlPLItemEMUVController*& _pCurController, RwInt32 _UpgradeIdx)
{
	NTL_DELETE(_pCurController);

// 	switch(_UpgradeIdx)
// 	{
// 	case 3:	
// 		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade03Lvl;	break;
// 	case 4:	
// 		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade04Lvl;	break;
// 	}

	if(_UpgradeIdx >= ITEM_GRADE_6 && _UpgradeIdx <= ITEM_GRADE_10)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade03Lvl;
	else if(_UpgradeIdx >= ITEM_GRADE_11 && _UpgradeIdx <= ITEM_GRADE_15)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade04Lvl;
		

	if(_pCurController)
	{
		_pCurController->m_ID						= _UpgradeIdx;
		_pCurController->m_CurTBNAccumulatedTime	= 0.0f;
		_pCurController->m_CurTBNIdx				= 0;
		_pCurController->m_CurEMUVRefresh			= TRUE;

		RwMatrix TmpTexMat =
		{
			{((RwReal)0.5), ((RwReal)0.0), ((RwReal)0.0)}, 0,
			{((RwReal)0.0),-((RwReal)0.5), ((RwReal)0.0)}, 0,
			{((RwReal)0.0), ((RwReal)0.0), ((RwReal)1.0)}, 0,
			{((RwReal)0.5), ((RwReal)0.5), ((RwReal)0.0)}, 0
		};

		_pCurController->m_TexMat = TmpTexMat;
	}
}

CNtlPLItemEMUV_Upgrade03Lvl::CNtlPLItemEMUV_Upgrade03Lvl()
{
	m_UVSpeed = 3.0f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("Armor_Grade_03", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE03LVL_TBN_NUM] = {0.5f, 4.0f};

	for(RwInt32 i = 0; i < dEMUV_UPGRADE03LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay	= TLTbl[i];
		sCurTBN._Vision		= !(i % 2);
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade03Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if(m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if(m_CurTBNIdx == dEMUV_UPGRADE03LVL_TBN_NUM)
		{
			m_CurTBNIdx = 0;
		}

		// just ignore the over time which would give us more flexible random time
		m_CurTBNAccumulatedTime = 0.0f;

		m_CurEMUVRefresh = m_vecTBN[m_CurTBNIdx]._Vision;
	}

	if(m_CurEMUVRefresh)
	{
		RwMatrix TmpTexMat =
		{
			{((RwReal)0.5), ((RwReal)0.0), ((RwReal)0.0)}, 0,
			{((RwReal)0.0),-((RwReal)0.5), ((RwReal)0.0)}, 0,
			{((RwReal)0.0), ((RwReal)0.0), ((RwReal)1.0)}, 0,
			{((RwReal)0.5), ((RwReal)0.5), ((RwReal)0.0)}, 0
		};
		
		m_TexMat = TmpTexMat;

		m_CurEMUVRefresh = FALSE;
	}

	if(m_vecTBN[m_CurTBNIdx]._Vision)
	{
		m_TexMat.pos.y += (m_UVSpeed * _ElapsedTime);
	}
}

CNtlPLItemEMUV_Upgrade04Lvl::CNtlPLItemEMUV_Upgrade04Lvl()
{
	m_UVSpeed = 3.0f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("Armor_Grade_04", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[8] = {0.2f, 0.3f, 0.2f, 0.5f, 0.4f, 0.5f, 0.2f, 4.0f};

	for(RwInt32 i = 0; i < dEMUV_UPGRADE04LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay	= TLTbl[i];
		sCurTBN._Vision		= !(i % 2);
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade04Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if(m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if(m_CurTBNIdx == dEMUV_UPGRADE04LVL_TBN_NUM)
		{
			m_CurTBNIdx = 0;
		}

		// just ignore the over time which would give us more flexible random time
		m_CurTBNAccumulatedTime = 0.0f;

		m_CurEMUVRefresh = m_vecTBN[m_CurTBNIdx]._Vision;
	}

	if(m_CurEMUVRefresh)
	{
		RwMatrix TmpTexMat =
		{
			{((RwReal)0.5), ((RwReal)0.0), ((RwReal)0.0)}, 0,
			{((RwReal)0.0),-((RwReal)0.5), ((RwReal)0.0)}, 0,
			{((RwReal)0.0), ((RwReal)0.0), ((RwReal)1.0)}, 0,
			{((RwReal)0.5), ((RwReal)0.5), ((RwReal)0.0)}, 0
		};

		m_TexMat = TmpTexMat;

		m_CurEMUVRefresh = FALSE;
	}

	if(m_vecTBN[m_CurTBNIdx]._Vision)
	{
		m_TexMat.pos.y += (m_UVSpeed * _ElapsedTime);
	}
}