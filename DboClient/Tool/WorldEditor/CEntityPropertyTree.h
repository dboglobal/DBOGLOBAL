#ifndef C_ENTITY_PROPERTY_TREE
#define C_ENTITY_PROPERTY_TREE

#include "PropTree.h"

class CEntityPropertyTree : public CPropTree
{
	protected:

		CEntity *						m_pEntity;

		CPropTreeItem *					m_pBase;

		CPropTreeItem *					m_pSerialID;
		CPropTreeItem *					m_pParentID;

		CPropTreeItem *					m_pCategory;
		CPropTreeItem *					m_pName;
		CPropTreeItem *					m_pLocality;

		CPropTreeItem *					m_pLocation;
		CPropTreeItemEdit *				m_pLocationX;
		CPropTreeItemEdit *				m_pLocationY;
		CPropTreeItemEdit *				m_pLocationZ;

		CPropTreeItem *					m_pDirection;
		CPropTreeItemEdit *				m_pDirectionX;
		CPropTreeItemEdit *				m_pDirectionY;
		CPropTreeItemEdit *				m_pDirectionZ;

		CPropTreeItem *					m_pScale;
		CPropTreeItemEdit *				m_pScaleX;
		CPropTreeItemEdit *				m_pScaleY;
		CPropTreeItemEdit *				m_pScaleZ;

	public:

										CEntityPropertyTree() { Initialize(); }
		virtual							~CEntityPropertyTree() { Destroy(); }

	protected:

		virtual void					Initialize( void );
		void							SetEntity( CEntity * pEntity );

	public:

		virtual bool					Create( CWnd * pWnd, CEntity * pEntity = NULL, bool bUpdate = true );
		virtual void					Destroy( void );

		virtual bool					Update( CEntity * pEntity, bool bUpdate = true );
		virtual bool					Apply( CPropTreeItem * pProperty, bool bUpdate = false );
};

#endif // C_ENTITY_PROPERTY_TREE