#pragma once


#include <RwCore.h>
#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLApi.h"
#include "ceventhandler.h"
#include "NtlWorldSector.h"


using std::map;
#include <map>


class CNtlPLWaterProp;
class CNtlWorldSector;
class CNtlPLShoreLineProp;
class CShoreLineContainer;


class CNtlPLShoreLine : public CNtlPLEntity, public RWS::CEventHandler
{
public:
	CNtlPLShoreLine(void);
	virtual ~CNtlPLShoreLine(void);

private:
	RwBool					m_Visible;
	//RwBool					m_PVSCulled;
	RwV3d					m_SectorMidPos;
	RwInt32					m_VertNum;
	RwIm3DVertex*			m_pVBOld;
	RwIm3DVertex*			m_pVBNew;
	CNtlPLShoreLineProp*	m_pShoreLineProp;

	// script contents
	RwInt32					m_ShoreLineType;
	CShoreLineContainer*	m_pShoreLineContainer;
	RwChar					m_ScriptKeyName[32];

public:
	virtual RwBool	SetProperty(const CNtlPLProperty *pProperty);
	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();
	virtual void	HandleEvents(RWS::CMsg &pMsg);
	//virtual void	SetVisible(RwBool bVisible) { m_PVSCulled = !bVisible; }

	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);

public:
	RwIm3DVertex*	GetVB() { return m_pVBNew; }
	RwInt32			GetVN() { return m_VertNum; }
	
	RwBool			Insert(RwIm3DVertex* pIm3DVertex, RwInt32 _Size, RwChar* pScriptKeyName);
	VOID			Delete(RwInt32 _IdxNum);
	
	//VOID			Save(FILE* _pFile);
	//VOID			Load(FILE* _pFile);

	RwBool			IsValidShoreline();

public:
	virtual RwBool	LoadFromFile(FILE* pFile);
	virtual RwBool	SaveIntoFile(FILE* pFile);
	static	RwInt32	SkipToFile(FILE* pFile);

	static	BYTE*	SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	static	BYTE*	SkipToFileMem(BYTE* pFileMem);
};
