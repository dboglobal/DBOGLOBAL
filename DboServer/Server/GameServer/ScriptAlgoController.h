#ifndef __SCRIPT_ALGO_CONTROLLER_H__
#define __SCRIPT_ALGO_CONTROLLER_H__


#include "NtlLinkList.h"

class CObjectMsg;
class CControlStateComposite;
class CScriptAlgoObject;
enum eSSD_SCRIPT_TYPE;

class CScriptAlgoController
{

public:

	CScriptAlgoController();
	virtual	~CScriptAlgoController();

public:

	void						SetOwner(CScriptAlgoObject* pOwner);


protected:

	void						Init();

	void						Destroy();

public:

	int							OnUpdate(DWORD dwTickDiff);
	int							OnObjectMsg(CObjectMsg * pObjMsg);

public:

	bool						ChangeStage(CControlStateComposite* pStage);
	bool						AddStage(CControlStateComposite* pStage);

	void						RemoveAllControlStage();
	void						DestroyAllControlStage();

	void						AllStop();

	CControlStateComposite*		GetCurrentStage();
	int							GetCurrentStageID();
	bool						IsCurrentStageID(int id);

	CScriptAlgoObject*			GetContolObject() { return m_pOwner; }


	void						SetNextStage(CControlStateComposite* pNextStage);

	bool						IsEmptyNextControlStage();

	void						ChangeControlStage_Stage(BYTE byStage, eSSD_SCRIPT_TYPE scriptType);

	//TQS ONLY
	void						ChangeControlStage_Begin();
	void						ChangeControlStage_End();

	//WPS ONLY
	void						ChangeControlStage_Prepare();
	void						ChangeControlStage_Failed();


private:

	CScriptAlgoObject*			m_pOwner;

	CNtlLinkList				m_cAddStageList;
	CControlStateComposite*		m_pNextStage;
	CNtlLinkList				m_controlStageStack;

};




#endif