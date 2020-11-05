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
	if (_UpgradeIdx >= ITEM_GRADE_1 && _UpgradeIdx <= ITEM_GRADE_4)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade01Lvl;
	else if (_UpgradeIdx >= ITEM_GRADE_5 && _UpgradeIdx <= ITEM_GRADE_6)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade02Lvl;
	else if (_UpgradeIdx >= ITEM_GRADE_7 && _UpgradeIdx <= ITEM_GRADE_8)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade03Lvl;
	else if(_UpgradeIdx >= ITEM_GRADE_9 && _UpgradeIdx <= ITEM_GRADE_10)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade04Lvl;
	else if (_UpgradeIdx >= ITEM_GRADE_11 && _UpgradeIdx <= ITEM_GRADE_12)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade05Lvl;
	else if (_UpgradeIdx == ITEM_GRADE_13)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade06Lvl;
	else if (_UpgradeIdx == ITEM_GRADE_14)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade07Lvl;
	else if(_UpgradeIdx == ITEM_GRADE_15)
		_pCurController = NTL_NEW CNtlPLItemEMUV_Upgrade08Lvl;
		

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

CNtlPLItemEMUV_Upgrade01Lvl::CNtlPLItemEMUV_Upgrade01Lvl()
{
	m_UVSpeed = 0.85f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("IK_ITM_W_G_01", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE01LVL_TBN_NUM] = { 0.5f, 0.3f, 0.8f, 2.0f };

	for (RwInt32 i = 0; i < dEMUV_UPGRADE01LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay = TLTbl[i];
		sCurTBN._Vision = !(i % 2);
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade01Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if (m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if (m_CurTBNIdx == dEMUV_UPGRADE01LVL_TBN_NUM)
		{
			m_CurTBNIdx = 0;
		}

		// just ignore the over time which would give us more flexible random time
		m_CurTBNAccumulatedTime = 0.0f;

		m_CurEMUVRefresh = m_vecTBN[m_CurTBNIdx]._Vision;
	}

	if (m_CurEMUVRefresh)
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

	if (m_vecTBN[m_CurTBNIdx]._Vision)
	{
		m_TexMat.pos.y += (m_UVSpeed * _ElapsedTime);
	}
}

CNtlPLItemEMUV_Upgrade02Lvl::CNtlPLItemEMUV_Upgrade02Lvl()
{
	m_UVSpeed = 0.9f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("IK_ITM_W_G_02", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE02LVL_TBN_NUM] = { 1.0f, 0.5f, 3.0f, 5.0f, 2.0f, 1.0f };

	for (RwInt32 i = 0; i < dEMUV_UPGRADE02LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay = TLTbl[i];
		sCurTBN._Vision = !(i % 2);
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade02Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if (m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if (m_CurTBNIdx == dEMUV_UPGRADE02LVL_TBN_NUM)
		{
			m_CurTBNIdx = 0;
		}

		// just ignore the over time which would give us more flexible random time
		m_CurTBNAccumulatedTime = 0.0f;

		m_CurEMUVRefresh = m_vecTBN[m_CurTBNIdx]._Vision;
	}

	if (m_CurEMUVRefresh)
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

	if (m_vecTBN[m_CurTBNIdx]._Vision)
	{
		m_TexMat.pos.y += (m_UVSpeed * _ElapsedTime);
	}
}

CNtlPLItemEMUV_Upgrade03Lvl::CNtlPLItemEMUV_Upgrade03Lvl()
{
	m_UVSpeed = 0.92f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("IK_ITM_W_G_03", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE03LVL_TBN_NUM] = { 1.0f };

	for (RwInt32 i = 0; i < dEMUV_UPGRADE03LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay = TLTbl[i];
		sCurTBN._Vision = 1;
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade03Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if (m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if (m_CurTBNIdx == dEMUV_UPGRADE03LVL_TBN_NUM)
		{
			m_CurTBNIdx = 0;
		}

		// just ignore the over time which would give us more flexible random time
		m_CurTBNAccumulatedTime = 0.0f;

		m_CurEMUVRefresh = m_vecTBN[m_CurTBNIdx]._Vision;
	}

	if (m_CurEMUVRefresh)
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

	if (m_vecTBN[m_CurTBNIdx]._Vision)
	{
		m_TexMat.pos.y += (m_UVSpeed * _ElapsedTime);
	}
}

CNtlPLItemEMUV_Upgrade04Lvl::CNtlPLItemEMUV_Upgrade04Lvl()
{
	m_UVSpeed = 0.95f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("IK_ITM_W_G_04", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE04LVL_TBN_NUM] = { 1.0f };

	for (RwInt32 i = 0; i < dEMUV_UPGRADE04LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay = TLTbl[i];
		sCurTBN._Vision = 1;
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade04Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if (m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if (m_CurTBNIdx == dEMUV_UPGRADE04LVL_TBN_NUM)
		{
			m_CurTBNIdx = 0;
		}

		// just ignore the over time which would give us more flexible random time
		m_CurTBNAccumulatedTime = 0.0f;

		m_CurEMUVRefresh = m_vecTBN[m_CurTBNIdx]._Vision;
	}

	if (m_CurEMUVRefresh)
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

	if (m_vecTBN[m_CurTBNIdx]._Vision)
	{
		m_TexMat.pos.y += (m_UVSpeed * _ElapsedTime);
	}
}

CNtlPLItemEMUV_Upgrade05Lvl::CNtlPLItemEMUV_Upgrade05Lvl()
{
	m_UVSpeed = 0.95f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("IK_ITM_W_G_05", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE05LVL_TBN_NUM] = { 1.0f };

	for (RwInt32 i = 0; i < dEMUV_UPGRADE05LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay = TLTbl[i];
		sCurTBN._Vision = 1;
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade05Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if (m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if (m_CurTBNIdx == dEMUV_UPGRADE05LVL_TBN_NUM)
		{
			m_CurTBNIdx = 0;
		}

		// just ignore the over time which would give us more flexible random time
		m_CurTBNAccumulatedTime = 0.0f;

		m_CurEMUVRefresh = m_vecTBN[m_CurTBNIdx]._Vision;
	}

	if (m_CurEMUVRefresh)
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

	if (m_vecTBN[m_CurTBNIdx]._Vision)
	{
		m_TexMat.pos.y += (m_UVSpeed * _ElapsedTime);
	}
}

CNtlPLItemEMUV_Upgrade06Lvl::CNtlPLItemEMUV_Upgrade06Lvl()
{
	m_UVSpeed = 1.0f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("IK_ITM_W_G_06", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE06LVL_TBN_NUM] = { 1.0f };

	for (RwInt32 i = 0; i < dEMUV_UPGRADE06LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay = TLTbl[i];
		sCurTBN._Vision = 1;
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade06Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if (m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if (m_CurTBNIdx == dEMUV_UPGRADE06LVL_TBN_NUM)
		{
			m_CurTBNIdx = 0;
		}

		// just ignore the over time which would give us more flexible random time
		m_CurTBNAccumulatedTime = 0.0f;

		m_CurEMUVRefresh = m_vecTBN[m_CurTBNIdx]._Vision;
	}

	if (m_CurEMUVRefresh)
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

	if (m_vecTBN[m_CurTBNIdx]._Vision)
	{
		m_TexMat.pos.y += (m_UVSpeed * _ElapsedTime);
	}
}

CNtlPLItemEMUV_Upgrade07Lvl::CNtlPLItemEMUV_Upgrade07Lvl()
{
	m_UVSpeed = 1.25f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("IK_ITM_W_G_07", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE07LVL_TBN_NUM] = {1.0f};

	for(RwInt32 i = 0; i < dEMUV_UPGRADE07LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay	= TLTbl[i];
		sCurTBN._Vision		= 1;
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade07Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if(m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if(m_CurTBNIdx == dEMUV_UPGRADE07LVL_TBN_NUM)
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

CNtlPLItemEMUV_Upgrade08Lvl::CNtlPLItemEMUV_Upgrade08Lvl()
{
	m_UVSpeed = 1.5f;
	m_pEnvTex = CNtlPLResourceManager::GetInstance()->LoadTexture("IK_ITM_W_G_08", ".\\texture\\effect\\");

	sTIME_BLOCK_NODE	sCurTBN;
	RwReal				TLTbl[dEMUV_UPGRADE08LVL_TBN_NUM] = {5.0f};

	for(RwInt32 i = 0; i < dEMUV_UPGRADE08LVL_TBN_NUM; ++i)
	{
		sCurTBN._TimeDelay	= TLTbl[i]; //TLTbl[i]
		sCurTBN._Vision		= 1;
		m_vecTBN.push_back(sCurTBN);
	}
}

VOID CNtlPLItemEMUV_Upgrade08Lvl::Update(RwReal _ElapsedTime)
{
	m_CurTBNAccumulatedTime += _ElapsedTime;

	if(m_CurTBNAccumulatedTime > m_vecTBN[m_CurTBNIdx]._TimeDelay)
	{
		++m_CurTBNIdx;
		if(m_CurTBNIdx == dEMUV_UPGRADE08LVL_TBN_NUM)
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