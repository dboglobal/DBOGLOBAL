/*****************************************************************************
*
* File			: NtlPLEmblemMaker.h
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2008. 3. 4
* Abstract		: Presentation layer emblem maker
*****************************************************************************
* Desc          : 길드 엠블렘을 생성/반환 한다
*				  재료 텍스처 : 엠블렘을 만들기 위해 조합하는 로컬상의 텍스처
*****************************************************************************/

#ifndef __NTL_PLEMBLEM_MAKER_H__
#define __NTL_PLEMBLEM_MAKER_H__

#include "NtlPLDef.h"
#include "NtlPLEmblemStuffList.h"


#define dPLEMBLEM_INVALID_FACTOR			(0xff)

#define dMAX_EMBLEM_TEXTURE_SIZE			(256)
#define dEMBLEM_TEXTURE_DEFAULT_SIZE		(64)


class CNtlPLCameraRT;

struct sEmblemFactor
{
	RwUInt8		byTypeA;
	RwUInt8		byTypeB;
	RwUInt8		byTypeC;
	RwUInt8		byTypeAColor;
	RwUInt8		byTypeBColor;
	RwUInt8		byTypeCColor;

	sEmblemFactor()
	:byTypeA(dPLEMBLEM_INVALID_FACTOR)
	,byTypeB(dPLEMBLEM_INVALID_FACTOR)
	,byTypeC(dPLEMBLEM_INVALID_FACTOR)
	,byTypeAColor(0)
	,byTypeBColor(0)
	,byTypeCColor(0)
	{
	}
};

class CNtlPLEmblemMaker
{
public:
	struct sEmblemData
	{
		sEmblemFactor	factor;
		RwTexture*		pTexture;
	};

	typedef std::map<std::string, sEmblemData*>					MAP_TEXTURE;
	typedef std::map<std::string, sEmblemData*>::iterator		ITER_MAP_TEXTURE;

	typedef std::map<RwUInt8, CNtlPLCameraRT*>					MAP_RT_CAMERA;
	typedef std::map<RwUInt8, CNtlPLCameraRT*>::iterator		ITER_RT_CAMERA;

public:
	static CNtlPLEmblemMaker* GetInstance() { return m_pInstance; }
	


	CNtlPLEmblemMaker();
	virtual ~CNtlPLEmblemMaker();

	static VOID	CreateInstance();
	static VOID	DestoryInstance();

	VOID		DestoyEmblem(const char* pcTextureName);
	VOID		DestoyEmblem(RwTexture* pTexture);

	///< 기존 텍스처를 반환, 없다면 새로 생성하여 반환
	///< 주의 : 랜더링 타임에 이 함수가 호출되고 반환되는 RwTexture가 NULL이 아니면 뻑이 난다.
	///<		이 함수의 내부에서 호출되는 Camera->CameraBeginUpdate는 현재 랜더링되고 있는
	///<		Current Camera의 포인터를 변경하니 반드시 이 함수는 랜더링 이외의 타임에 호출한다.
	RwTexture*	CreateEmblem(const sEmblemFactor* pEmblemFactor,
							 RwUInt8 byEmblemSize = dEMBLEM_TEXTURE_DEFAULT_SIZE,
							 const RwChar* pcBaseTextureName = NULL);


	///< 로컬상에 존재하는 엠블렘 재료 텍스처의 이름의 형태로 만들어서 반환한다
	VOID		MakeEmblemResourceName(eEmblemType eType, RwUInt8 byValue, char* pcOutBuffer, RwInt32 iBufferLength,
									   RwUInt8 byEmblemSize = dEMBLEM_TEXTURE_DEFAULT_SIZE);
	
	const MAP_STUFF* GetStuffList(eEmblemType eType);

protected:
	///< 재료 텍스처의 인자로 만들어진 엠블렘 텍스처의 이름을 반환한다, 캐릭터 모델의 엠블렘은 유효하지 않다
	///< Suffix : 접미사
	VOID		MakeEmblemTextureName(char* pcOutBuffer, RwInt32 iBufferLength, const sEmblemFactor* pEmblemFactor,
									  RwUInt8 byEmblemSize, const RwChar* pcBaseTextureName);

	RwTexture*	MakeEmblemImmidately(const sEmblemFactor* pEmblemFactor, const char* pcEmblemName,
									 RwUInt8 byEmblemSize, const RwChar* pcBaseTextureName = NULL);

	///< 별도의 버퍼에 넘겨받은 텍스처를 그린다
	VOID		RecoatEmblem(RwTexture* pTexture, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwTexture*	CopyToTexture(RwTexture* pTexture);

	///< 텍스처 사이즈에 맞는 카메라 반환
	///< 엠블렘에 쓸 텍스처의 사이즈가 아주 다양하지는 않을 것이기에 사이즈 별로
	///< 카메라를 따로 만들어 텍스처를 찍는다
	CNtlPLCameraRT* GetRTCamera(RwUInt8 bySize);

protected:
	static CNtlPLEmblemMaker* m_pInstance;

	MAP_RT_CAMERA			m_mapRTCamera;
	RwIm2DVertex			m_2dVertices[4];
	MAP_TEXTURE				m_mapTexture;

	CNtlPLEmblemStuffList*	m_pStuffList;
};

static CNtlPLEmblemMaker* GetEmblemMaker(VOID)
{
	return CNtlPLEmblemMaker::GetInstance();
}

#endif