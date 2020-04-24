#ifndef C_CLUMP_ENTITY_PROPERTY_TREE
#define C_CLUMP_ENTITY_PROPERTY_TREE

#include "CEntityPropertyTree.h"

class CClumpEntityPropertyTree : public CEntityPropertyTree
{
	protected:

		CPropTreeItem *					m_pClump;

		CPropTreeItem *					m_pPropertyName;

	public:

										CClumpEntityPropertyTree() { Initialize(); }
		virtual							~CClumpEntityPropertyTree() { Destroy(); }

	protected:

		virtual void					Initialize( void );

	public:

		virtual bool					Create( CWnd * pWnd, CEntity * pEntity = NULL, bool bUpdate = true );

		virtual bool					Update( CEntity * pEntity, bool bUpdate = true );
		virtual bool					Apply( CPropTreeItem * pProperty, bool bUpdate = false );
};

#endif // C_CLUMP_ENTITY_PROPERTY_TREE