#ifndef C_CAMERA_ENTITY_PROPERTY_TREE
#define C_CAMERA_ENTITY_PROPERTY_TREE

#include "CEntityPropertyTree.h"

class CCameraEntityPropertyTree : public CEntityPropertyTree
{
	protected:

		CPropTreeItem *					m_pCamera;

		CPropTreeItem *					m_pCameraRaster;
		CPropTreeItemEdit *				m_pCameraRasterW;
		CPropTreeItemEdit *				m_pCameraRasterH;

		CPropTreeItem *					m_pCameraClipPlane;
		CPropTreeItemEdit *				m_pCameraClipPlaneN;
		CPropTreeItemEdit *				m_pCameraClipPlaneF;

		CPropTreeItem *					m_pCameraBackgroundColor;
		CPropTreeItemEdit *				m_pCameraBackgroundColorR;
		CPropTreeItemEdit *				m_pCameraBackgroundColorG;
		CPropTreeItemEdit *				m_pCameraBackgroundColorB;
		CPropTreeItemEdit *				m_pCameraBackgroundColorA;

		CPropTreeItemEdit *				m_pCameraFov;

	public:

										CCameraEntityPropertyTree() { Initialize(); }
		virtual							~CCameraEntityPropertyTree() { Destroy(); }

	protected:

		virtual void					Initialize( void );

	public:

		virtual bool					Create( CWnd * pWnd, CEntity * pEntity = NULL, bool bUpdate = true );

		virtual bool					Update( CEntity * pEntity, bool bUpdate = true );
		virtual bool					Apply( CPropTreeItem * pProperty, bool bUpdate = false );
};

#endif // C_CAMERA_ENTITY_PROPERTY_TREE