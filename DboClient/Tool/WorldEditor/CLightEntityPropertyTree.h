#ifndef C_LIGHT_ENTITY_PROPERTY_TREE
#define C_LIGHT_ENTITY_PROPERTY_TREE

#include "CEntityPropertyTree.h"

class CLightEntityPropertyTree : public CEntityPropertyTree
{
	protected:

		CPropTreeItem *					m_pLight;

		CPropTreeItem *					m_pLightType;
		CPropTreeItem *					m_pAffectType;

		CPropTreeItem *					m_pLightColor;
		CPropTreeItemEdit *				m_pLightColorR;
		CPropTreeItemEdit *				m_pLightColorG;
		CPropTreeItemEdit *				m_pLightColorB;
		CPropTreeItemEdit *				m_pLightColorA;

		CPropTreeItem *					m_pShadow;
		CPropTreeItemEdit *				m_pShadowOffset;
		CPropTreeItemEdit *				m_pShadowMultipleFactor;
		CPropTreeItemEdit *				m_pShadowSubShadowFactor;

	public:

										CLightEntityPropertyTree() { Initialize(); }
		virtual							~CLightEntityPropertyTree() { Destroy(); }

	protected:

		virtual void					Initialize( void );

	public:

		virtual bool					Create( CWnd * pWnd, CEntity * pEntity = NULL, bool bUpdate = true );

		virtual bool					Update( CEntity * pEntity, bool bUpdate = true );
		virtual bool					Apply( CPropTreeItem * pProperty, bool bUpdate = false );
};

#endif // C_LIGHT_ENTITY_PROPERTY_TREE