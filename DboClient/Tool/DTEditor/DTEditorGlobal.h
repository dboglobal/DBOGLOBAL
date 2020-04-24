#pragma once

struct sWORLD_TBLDAT;
class CNtlDTProp;
class CDTServer;

enum EEditModeType
{
	EDIT_MODE_THIRD_CAMERA,
	EDIT_MODE_FREE_CAMERA
};

RwBool			CreateDTEditorGlobal(void);
void			DestroyDTEditorGlobal(void);

void			DTEditorUpdate(RwReal fElapsed);

void			SetEditModeType(EEditModeType);
EEditModeType	GetEditModeType(void);

// spline control data

void			SetActiveDTProp(const CNtlDTProp *pProp);
CNtlDTProp*		GetActiveDTProp(void);	

void			SetSplineEditPosition(RwV3d vPos);
RwV3d			GetSplineEditPosition(void);

void			SetActiveSelectControlPoint(int nSelSx, int nSelEx);
void			GetActiveSelectControlPoint(int& nSelSx, int& nSelEx);

void			SetFocusControlPoint(int nFocusIdx);
int				GetFocusControlPoint(void);

void			SetGridCenter(RwV3d vCenter);
RwV3d			GetGridCenter(void);

void			SetGridLineCx(RwInt32 iCx);
RwInt32			GetGridLineCx(void);

void			SetGridLineCy(RwInt32 iCy);
RwInt32			GetGridLineCy(void);

RwIm3DVertex*	GetLineAxisXVertex(void);
RwIm3DVertex*	GetLineAxisZVertex(void);
RwIm3DVertex*	GetLineXVertex(void);
RwIm3DVertex*	GetLineYVertex(void);

void			EnableChangePointPosition(RwBool bEnable);
RwBool			IsChangePointPosition(void);

void			EnableChangePointDirection(RwBool bEnable);
RwBool			IsChangePointDirection(void);

void			EnableCameraMovePointClick(RwBool bEnable);
RwBool			IsCameraMovePointClick(void);

void			SetSplinePointBoxSize(RwReal fSize);
RwReal			GetSplinePointBoxSize(void);

void			EnableCustomDataRender(RwBool bEnable);
RwBool			IsCustomDataRender(void);



// viewer control data

RwUInt32				AcquireSerialId(void);

CDTServer*				GetDTServer(void);

void					SetSelectTarget(SERIAL_HANDLE hSerialId);
SERIAL_HANDLE			GetSelectTarget(void);

void					SetSplineStandardPosition(RwV3d vPos);
RwV3d					GetSplineStandardPosition(void);

void					SetAvatarRace(RwUInt8 byRace);
RwUInt8					GetAvatarRace(void);

void					SetAvatarClass(RwUInt8 byClass);
RwUInt8					GetAvatarClass(void);

void					SetAvatarGender(RwUInt8 byGender);
RwUInt8					GetAvatarGender(void);

void					SetActiveWorldTableData(sWORLD_TBLDAT *pWorldTblData);
sWORLD_TBLDAT*			GetActiveWorldTableData(void);

void					SetActiveMonsterTableData(const sMOB_TBLDAT *pMobTblData);
const sMOB_TBLDAT*		GetActiveMonsterTableData(void);

void					SetActiveHTBTableData(const sHTB_SET_TBLDAT *pHTBSetTblData);
const sHTB_SET_TBLDAT*	GetActiveHTBTableData(void);


// grid rendering

void					MakeLineAxisVertex(RwReal fLineCx, RwReal fLineCy);

// viewer rendering

void					EditViewerRendering(RwReal fElapsed);


// camera attribute		
void					SetFreeCameraSpeed(RwReal fSpeed);
RwReal					GetFreeCameraSpeed(void);



