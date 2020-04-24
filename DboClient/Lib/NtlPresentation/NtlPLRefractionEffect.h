#pragma once

struct WaveElement
{
    RwV2d   v2Pos;          //굴절파발생지점
    RwReal  fRound;         //파가번지는최대반지름
    RwReal  fMaxHeight;     //파의최초높이(z값)
    RwReal  fDelta;         //현재진행된시간(및현재파의반지름)
};

/**
 * \ingroup NtlPresentation
 * \brief 화면의 굴절 반사 이펙트 클래스
 *
 * \date 2008-08-20
 * \author agebreak
 */
class CNtlPLRefractionEffect
{
public:
    CNtlPLRefractionEffect(void);
    ~CNtlPLRefractionEffect(void);

    void Init(RwIm2DVertex* pVertices, RwInt32 nVertexCount);
    void Update(RwReal fElapsedTime);

    void InsertWave(RwReal fX, RwReal fY, RwReal fRound, RwReal fMaxHeight);

protected:

protected:
    std::list<WaveElement*> m_listWaveElement;           ///< 파동원 리스트
    RwInt32     m_nVertexCnt;    
    RwIm2DVertex* m_pVertices;
    RwV2d*      m_pUVVerticesOrg;
};
