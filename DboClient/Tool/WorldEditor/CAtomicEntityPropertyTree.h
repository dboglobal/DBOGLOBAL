#ifndef C_ATOMIC_ENTITY_PROPERTY_TREE
#define C_ATOMIC_ENTITY_PROPERTY_TREE

#include "CEntityPropertyTree.h"

class CAtomicEntityPropertyTree : public CEntityPropertyTree
{
	protected:

		CPropTreeItem *					m_pAtomic;

		CPropTreeItem *					m_pAtomicType;

	public:

										CAtomicEntityPropertyTree() { Initialize(); }
		virtual							~CAtomicEntityPropertyTree() { Destroy(); }

	protected:

		virtual void					Initialize( void );

	public:

		virtual bool					Create( CWnd * pWnd, CEntity * pEntity = NULL, bool bUpdate = true );

		virtual bool					Update( CEntity * pEntity, bool bUpdate = true );
		virtual bool					Apply( CPropTreeItem * pProperty, bool bUpdate = false );
};

#endif // C_ATOMIC_ENTITY_PROPERTY_TREE