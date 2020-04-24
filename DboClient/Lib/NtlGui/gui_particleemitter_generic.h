#include "gui_particle.h"
#include "gui_particleEmitter.h"
#include "gui_surface.h"

//#ifndef __GUI_PARTICLE_EMITTER_GENERIC_H__
//#define __GUI_PARTICLE_EMITTER_GENERIC_H__

//START_GUI
//
//class CParticleEmitter_Generic
//{
//public:
//	// Construction & Destruction
//	CParticleEmitter_Generic( CParticleEmitter* pSelf );
//	~CParticleEmitter_Generic(VOID);
//
//	// Operation:
//	VOID Start(VOID);
//	VOID End(VOID);
//
//	VOID Update( FLOAT fElapsed );
//
//private:
//	// Implementation:
//	inline VOID CreateParticle(VOID);
//		
//	// CallBack:
//	VOID OnSetOptions(const CComponentOptions& options);
//	VOID OnPaint(VOID);
//	VOID OnMove(INT nX,INT nY);
//	
//	CSlot m_SlotSetOption;
//	CSlot m_SlotPaint;
//	CSlot m_SlotMove;
//	
//public:
//	//! Variable
//	BOOL		m_bStart;
//	
//	FLOAT		m_fElapsedTime;
//
//	std::vector<CSurface> m_vecSurface;
//	std::list<CParticle*> m_listParticle;
//	std::vector<stControlData> m_vecControl;
//
//	DWORD		m_dwEmitMaxCount;		// 한번의 스타트에 생기는 파티클의 갯수 : 0xffffffff이면 무한.
//	DWORD		m_dwEmitCurCount;
//
//	FLOAT		m_fEmitRate1;			// 파티클 생성 속도 : 두값사이의 랜덤값. 둘다 0이면 초기 한번만 생성.
//	FLOAT		m_fEmitRate2;
//
//	FLOAT		m_fLifeTime1;			// 파티클 생명 시간 : 두값사이의 랜덤값
//	FLOAT		m_fLifeTime2;
//	
//	CRectangle	m_rtEmitterPos;			// 파티클 초기 위치 : 사각형 안의 랜덤값.
//	
//	CRectangle	m_rtSize1;				// 파티클의 초기 크기 : 두값사이의 랜덤값
//	CRectangle	m_rtSize2;	
//
//	BYTE		m_ucRed1;				// 파티클의 초기 색상 : 두값사이의 랜덤값
//	BYTE		m_ucGreen1;
//	BYTE		m_ucBlue1;
//	BYTE		m_ucAlpha1;
//	BYTE		m_ucRed2;
//	BYTE		m_ucGreen2;
//	BYTE		m_ucBlue2;
//	BYTE		m_ucAlpha2;
//
//	FLOAT		m_fZRot1;				// 파티클의 초기 회전값 : 두값사이의 랜덤값
//	FLOAT		m_fZRot2;
//
//	BYTE		m_ucSurfaceIndex1;		// 파티클의 초기 서피스값 : 두값사이의 랜덤값
//	BYTE		m_ucSurfaceIndex2;
//	
//	CParticleEmitter* m_pParticleEmitter;
//};  
//
//END_GUI
//
//#endif
