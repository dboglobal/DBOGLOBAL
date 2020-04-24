#pragma once

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"

enum ENodeType
{
	NODE_FOLDER,
	NODE_SCIRPT,
};

struct SItemNode
{
	CString strNodeName;
    ENodeType eNodeType;
	
	std::vector<SItemNode*> vecChildList;		// 자식 노드들 

	SItemNode()
	{
		eNodeType = NODE_FOLDER;
		vecChildList.reserve(30);
	}

	~SItemNode()
	{
		for each(SItemNode* pItemNode in vecChildList)
		{
			delete pItemNode;
			pItemNode = NULL;
		}
		vecChildList.clear();
	}
};

typedef std::vector<SItemNode*> VEC_ITEM_NODE;

/** 
 * \ingroup NtlXMLLoader
 * \brief 트리에 사용될 XML을 다루는 클래스. 파일생성/추가를 위해서 기존의 클래스에서 상속받아서 만들었다.
 * \date 2006-04-26
 * \author agebreak
 */
class CNtlMTXml : public CNtlXMLDoc
{

public:
    CNtlMTXml(void);
    virtual ~CNtlMTXml(void);

    BOOL CreateXML(WCHAR* szRootNodeName);                                           ///< 새로운 XML 객체를 만든다.
    bool Save(WCHAR* szFileName);                                  ///< XML을 파일로 저장한다.    
    bool LoadTreeXML(WCHAR* szFileName, SItemNode* itemNode);               ///< 트리구조의 XML파일에서 아이템 벡터를 반환한다.
    void AddFolderNode(WCHAR* szParentFolder,  WCHAR* szFolderName); ///< 새로운 폴더노드를 추가한다.
    void AddScriptNode(WCHAR* szParentFolder,  WCHAR* szScriptName); ///< 새로운 스크립트 노드를 추가한다

protected:
	void LoadScipt(IXMLDOMNode* pNode, SItemNode* pParentItem, bool bRoot = FALSE);		///< 스크립트를 로드하는 재귀 함수

protected:
    IXMLDOMElement*     m_pRootElem;                            ///< Root Element
};
