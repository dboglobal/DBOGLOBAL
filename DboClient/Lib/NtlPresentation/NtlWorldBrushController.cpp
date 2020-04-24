#include "precomp_ntlpresentation.h"
#include "ntlworldbrushcontroller.h"
#include "NtlDebug.h"
#include "ntlworldcommon.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"


//CUSTOM_RTTI_SYSTEM_IMPL_NOPARENT(CNtlWorldBrushController);
//CUSTOM_RTTI_SYSTEM_IMPL(CNtlWB_Circle<CNtlWorldFieldManager>, CNtlWorldBrushController);
//CUSTOM_RTTI_SYSTEM_IMPL(CNtlWB_Rectangle<CNtlWorldFieldManager>, CNtlWorldBrushController);


CNtlWorldBrushController::CNtlWorldBrushController(void)
{
	m_PosWorldMin.x	= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin);
	m_PosWorldMin.y	= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin);

	m_ExtWorldHSize	= dGET_WORLD_PARAM()->WorldSizeHalf;
	m_Seam2Surface	= 0.5f;
}

CNtlWorldBrushController::~CNtlWorldBrushController(void)
{
}

eNTL_BRUSH_TYPE CNtlWorldBrushController::GetID()
{
	NTL_FUNCTION("CNtlWorldBrushController::GetID");

	NTL_RETURN(m_ID);
}