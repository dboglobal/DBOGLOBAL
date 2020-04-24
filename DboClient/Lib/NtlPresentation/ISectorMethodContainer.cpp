#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "ISectorMethodContainer.h"
#include "ISectorMethod.h"
#include "SM_SplatTexMinBlend.h"
#include "SM_SplatTexMaxBlend.h"
#include "SM_GetRuleOutDoodad.h"
#include "SM_GetRuleOutWater.h"
#include "SM_GetTileTransparency.h"

ISectorMethodContainer::ISectorMethodContainer(DWORD _SearchFlag)
{
	if(_SearchFlag & dSSM_FLAG_SPLAT_TEX_MIN_BLEND)		m_vecSM.push_back(NTL_NEW CSM_SplatTexMinBlend);
	if(_SearchFlag & dSSM_FLAG_SPLAT_TEX_MAX_BLEND)		m_vecSM.push_back(NTL_NEW CSM_SplatTexMaxBlend);
	if(_SearchFlag & dSSM_FLAG_GET_RULE_OUT_DOODAD)		m_vecSM.push_back(NTL_NEW CSM_GetRuleOutDoodad);
	if(_SearchFlag & dSSM_FLAG_GET_RULE_OUT_WATER)		m_vecSM.push_back(NTL_NEW CSM_GetRuleOutWater);
	if(_SearchFlag & dSSM_FLAG_GET_TILE_TRANSPARENCY)	m_vecSM.push_back(NTL_NEW CSM_GetTileTransparency);
}

ISectorMethodContainer::~ISectorMethodContainer(void)
{
	for(RwUInt32 i = 0; i < m_vecSM.size(); ++i)
	{
		ISectorMethod* pDel = static_cast<ISectorMethod*>(m_vecSM[i]);
		NTL_DELETE(pDel);
	}

	m_vecSM.clear();
}