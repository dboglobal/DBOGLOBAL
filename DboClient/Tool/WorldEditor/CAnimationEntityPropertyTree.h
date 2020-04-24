#ifndef C_ANIMATION_ENTITY_PROPERTY_TREE
#define C_ANIMATION_ENTITY_PROPERTY_TREE

#include "CEntityPropertyTree.h"

class CAnimationEntityPropertyTree : public CEntityPropertyTree
{
	protected:

		CPropTreeItem *					m_pAnimation;

		CPropTreeItemEdit *				m_pAnimationSpeed;
		CPropTreeItemEdit *				m_pAnimationOffset;

	public:

										CAnimationEntityPropertyTree() { Initialize(); }
		virtual							~CAnimationEntityPropertyTree() { Destroy(); }

	protected:

		virtual void					Initialize( void );

	public:

		virtual bool					Create( CWnd * pWnd, CEntity * pEntity = NULL, bool bUpdate = true );

		virtual bool					Update( CEntity * pEntity, bool bUpdate = true );
		virtual bool					Apply( CPropTreeItem * pProperty, bool bUpdate = false );
};

#endif // C_ANIMATION_ENTITY_PROPERTY_TREE