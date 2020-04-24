#ifndef _DBO_TSEMSKILL_H_
#define _DBO_TSEMSKILL_H_


/** 
	Event mapper skill
	특정 Skill이 제공할 수 있는 Trigger id를 외부에 제공할 책임을 가진다
*/


class CDboTSEMSkill : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	CDboTSEMSkill( void );
	virtual ~CDboTSEMSkill( void );

// Methods
public:
	virtual bool							AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter );
	virtual bool							DelBuildData( const std::string& strKey );
};

#endif