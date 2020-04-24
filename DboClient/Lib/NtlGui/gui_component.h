#ifndef __GUI_COMPONENT_H__
#define __GUI_COMPONENT_H__

#include "position.h"
#include "rectangle.h"
#include "inputdevice.h"
#include "key.h"
#include "gui_define.h"
#include "gui_slots.h"
#include "gui_signals.h"
#include "gui_surfacemanager.h"


START_GUI

#define ENABLE_COMPONENT_STYLE(styles,check)  (styles & check) 

class CGUIManager;
class CComponentOptions;
class CComponent_Generic;

//: Component base class.
// The component class is the base class for any component/widget/control (or
// whatever you call it :)) in the GUI system. It is responsible for painting
// the component, painting of any child components of it, and it receives the
// input for a component.
class CComponent
{
//! Construction:
public:
	//: Component Constructor
	CComponent(CComponent *pParent,
						CSurfaceManager *pSurfaceManager);

	//: Component Constructor
	CComponent(const CRectangle &rtPos,
				CComponent *pParent,
				CSurfaceManager *pSurfaceManager);

	//: Component Destructor
	virtual ~CComponent();

protected:
	enum ChildCascadeType { CHILD_DO_MOVE = 0x01, CHILD_DO_RESIZE = 0x02 };
	VOID ChildCascade( DWORD flagCascadeType );

//! Attributes:
public:

	//: Returns the component manager page name
	const std::string& GetPage(VOID) const;

	//: Returns the current component id name
	const std::string& GetID(VOID) const;

	//: Returns the current component tool tip string
	const std::wstring& GetToolTip(VOID) const;

	//: Returns the current component tool tip string
	const std::string& GetDescription(VOID) const;

	//: Returns the parent component, or NULL if none.
	CComponent *GetParent(VOID) const;

	//: Returns the root parent.
	CComponent *GetRootParent(VOID);

	//: Returns the Surface manager that this component is attached to, or NULL if none.
	CSurfaceManager* GetSurfaceManager(VOID) const;
	
	//: Returns the GUI manager that this component is attached to, or NULL if none.
	CGUIManager *GetGuiManager(VOID) const;

	//: Returns the width of the component.
	int GetWidth(VOID) const;

	//: Returns the height of the component.
	int GetHeight(VOID) const;

	//: Returns the components position in screen coordinates.
	CRectangle GetScreenRect(VOID) const;

	//: Returns the components position in client to screen coordinates.
	CPos ClientToScreen(CPos pos) const;

	//: Returns the components position in screen to client coordinates.
	CPos ScreenToClient(CPos pos) const;

	//: Returns the components position in client coordinates.
	CRectangle GetClientRect(VOID) const;

	//: Returns the position of this component.
	const CRectangle& GetPosition(VOID) const;

	//: Returns true if the passed component is a child of this component.
	bool HasChild(CComponent *pComponent) const;

	//: Returns the list of child components.
	const std::list<CComponent*> &GetChildren(VOID) const;

	//: Returns the bounding rectangle of the component's children.
	CRectangle GetChildrenRect(VOID);

	//: Returns the component at (pos_x, pos_y).
	//: This functions searches for children recursively.
	//: NULL is returned if the point is outside the component.
	//: The component itself is returned if the component has no child at the specified point.
	CComponent* GetComponentAt(int nPosX, int nPosY);

	//: Returns the component at (pos_x, pos_y).
	//: This functions searches for children recursively.
	//: NULL is returned if the point is outside the component.
	//: The component itself is returned if the component has no child at the specified point.
	CComponent* GetComponentReverseAt(int nPosX, int nPosY);

	//: This functions searches for children NOT Recursively. Just Search Child Object.
	CComponent* GetChildComponentAt(int nPosX, int nPosY);
	CComponent* GetChildComponentReverseAt(int nPosX, int nPosY);

	//: Returns the component that currently has the focus.
	CComponent *GetFocus(VOID) const;

	//: Returns true if component has the focus.
	bool HasFocus(VOID) const;

	//: Returns true if the mouse is currently highlighting the component.
	bool HasMouseOver(VOID) const;

	//: Returns true if component is visible.
	bool IsVisible(VOID) const;

	//: Returns true if all parent and self is visible. truly visible.
	bool IsVisibleTruly(VOID) const;

	//: Returns true if component accepts user input.
	bool IsEnabled(VOID) const;

	//: Returns true if component is a popup window.
	bool IsPopup(VOID) const;

	//: Returns true if component has mouse captured.
	bool IsMouseCaptured(VOID) const;

	//: I have no idea what this does... -- mbn 3. okt 2000.
	bool IsRemoveflagSet(VOID) const;

	//: Returns the current mouse position, relative to the component.
	CPos GetMousePosition(VOID) const;

	//: Returns the tab id of the component.
	int GetTabID(VOID) const;

	//: Returns the Priority of the commponent.
	WORD GetPriority(VOID) const;

	//: Returns State of the Position, Position is must be Screen Coordination.
	enum POSITIONSTATE { INRECT = 0x00, OUT_LEFT = 0x01, OUT_RIGHT = 0x02, OUT_UP = 0x10, OUT_DOWN = 0x20,
						 OUT_UPLEFT = 0x11, OUT_UPRIGHT = 0x12, OUT_DOWN_LEFT = 0x21, OUT_DOWN_RIGHT = 0x22 };
	POSITIONSTATE PosInRect( INT nX, INT nY );	

	//: Returns Clipping Information
	CRectangle* GetClippingRect(VOID) const;
	bool IsClipped(VOID);

	BYTE GetAlpha(VOID) const;

	DWORD GetColor(VOID) const;
	
	CComponent_Generic*	GetComponent_Generic(VOID) const;			//m_pImpl; /// woosungs_test
//! Operations:
public:

	//: Set the ID of the component.
	VOID SetPage(const std::string& strPage); 

	//: Set the ID of the component.
	VOID SetID(const std::string& strID);

	//: Set the current component tool tip string
	VOID SetToolTip( const std::string& strToolTip );
	VOID SetToolTip( const std::wstring& wstrToolTip );
	VOID DeleteToolTip(VOID);

	VOID ShowToolTip( bool bShow );
	
	//: Set the tab id of the component.
	VOID SetTabID(int id);

	//: Set Priority
	VOID SetPriority( WORD wPriority );

	//: Sets the position (relative to its parent) and size of this component.
	VOID SetPosition(const CRectangle &newpos);

	//: Sets the position of this component (relative to its parent).
	VOID SetPosition(int nNewX, int nNewY);

	//: Sets the size of this component.
	VOID SetSize(int nNewWidth, int nNewHeight);

	//: Sets the size of this component.	
	//VOID Resize(CRectangle oldpos);

	//: Changes the width of the component.
	VOID SetWidth(int nWidth);

	//: Changes the height of the component.
	VOID SetHeight(int nHeight);

	//: Sets the parent of this component.
	VOID SetParent(CComponent *pParent, BOOL bParentAddRemoveProc = FALSE);

	//: Sets the GUI manager this component is attached to.
	VOID SetGuiManager(CGUIManager *gui);

	//: Changes the visibility flag.
	VOID Show(bool bShow = true);

	//: Enable/Disable user input.
	virtual VOID Enable(bool bEnable = true);

	//: Change popup flag.
	VOID Popup(bool bPopup);

	//: Set Render Top flag.
	VOID SetRenderTop(bool bTop);

	//: Get Render Top flag.
	bool GetRenderTop(VOID);

	//: Give focus to this component.
	VOID SetFocus(VOID);

	//: Raises this component to the top of the parent component's stack in same priority. 
	VOID Raise(VOID);

	//: Lowers this component to the bottom of the parent component's stack in same priority.
	VOID Lower(VOID);

	//: Capture the mouse on the currently attached GUI manager. All mouse input
	//: will be directed to this component until release_mouse() is called.
	VOID CaptureMouse(VOID);

	//: Releases the mouse capture.
	VOID ReleaseMouse(VOID);

	//: Capture the mouse on the current mouse over component.
	CComponent* GetMouseOver(VOID);

	//: Calls sig_begin_paint(), then sig_paint(), and finally sig_end_paint().
	VOID Paint(VOID);

	//: Run the component as a modal component until it returns
	VOID Run(CComponent *pParent = NULL);

	//: Quit the modal loop of the component
	VOID Quit(VOID);

	//: "Closes" this component.
	//: Hides and detaches component from parent.
	VOID Close(VOID);

	//: Clears the remove flag... dunno what the remove flag means. -- mbn 3. okt 2000
	VOID ClearRemoveflag(VOID);

	//: Adds the specified component as a children to this component.	
	VOID AddChild( CComponent* pChild );

	//: Removes the specified child from the component.
	VOID RemoveChild( CComponent* pChild );

	//: Add all components in a component manager.
	VOID AddComponents(class CComponentManager *pComponentManager);

	//: Calculate a default size
	VOID FindPreferredSize(VOID);

	//: Returns the component with the specified tab ID.
	CComponent *FindTabID(int nTabID);

	//: setting last order(paint)
	VOID TopTabOrder(CComponent *pComp);

	//: setting texture alpha
	VOID SetAlpha( unsigned char ucAlpha, BOOL bMemo = TRUE );

	//: setting texture color
	VOID SetColor( unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, BOOL bMemo = TRUE );

	//: setting clipping mode
	virtual VOID SetClippingMode( bool bClipping );

//! Signals:
public:
	//: Update component with a new set of component options.
	CSignal_v1<const CComponentOptions &>& SigSetOptions(VOID);

	//: <p>Draws the component. Must be called in a
	//: begin_paint() / end_paint() session.</p>
	CSignal_v0 & SigPaint(VOID);

	//: <p>Called prior to component drawing. Sets up a cliprect and translocates
	//: coordinates accordingly to this component.</p>
	CSignal_v0 & SigBeginPaint(VOID);

	//: Called after component drawing, and draws all the children.
	CSignal_v0 & SigPaintChildren(VOID);

	//: <p>Cleans up when component drawing is finished (cleans up cliprects and
	//: translocation stuff).</p>
	CSignal_v0 & SigEndPaint(VOID);
	
	//: Called when a key is pressed.
	CSignal_v3<CComponent *, CInputDevice *, const CKey &>& SigKeyDown(VOID);

	//: Called when a key is released.
	CSignal_v3<CComponent *, CInputDevice *, const CKey &>& SigKeyUp(VOID);

	//: Called when the mouse is moved.
	CSignal_v3<int,int,int>& SigMouseMove(VOID);

	//: Called when the mouse is wheeled.
	CSignal_v3<int ,short ,CPos& >& SigMouseWheel(VOID);

	//: Called when the mousebutton is clicked.
	//: The CL_Key contains the coordinates and which button was pressed
	CSignal_v1<const CKey &>& SigMouseDown(VOID);

	//: Called when the mousebutton is released.
	CSignal_v1<const CKey &>& SigMouseUp(VOID);

	//: Called when the mousebutton is released.
	CSignal_v2<int, int>& SigTracking(VOID);

	//: Called when the mouse left button is drag and drop pressed.
	CSignal_v2<const CKey &,int& >& SigDragDropQuery(VOID);

	//: Called when the mouse left button is drag and drop up.
	CSignal_v4<CComponent *,const CKey &,int,bool& >& SigDragDropRevote(VOID);

	//: Called when the mouse drag and drop not prop
	CSignal_v1<int>& SigDragDropNotProc(VOID);

	//: Called when mouse enters the component area.
	CSignal_v1<CComponent *>& SigMouseEnter(VOID);

	//: Called when mouse leaves the component area.
	CSignal_v1<CComponent *>& SigMouseLeave(VOID);

	//: Called when component gains focus.
	CSignal_v0& SigGotFocus(VOID);

	//: Called when component lose focus.
	CSignal_v0& SigLostFocus(VOID);

	//: Called when component is resized.
	//: The two parameters are the OLD width and height of the component.
	CSignal_v2<int, int>& SigResize(VOID);

	//: Called when component is moved.
	//: The two parameters are the OLD positions of the component.
	CSignal_v2<int, int>& SigMove(VOID);

	//: Called when a component is about to be removed from the component
	CSignal_v1<CComponent *>& SigBeforeRemove(VOID);

	//: Called when a child component is about to be added to the component.
	CSignal_v1<CComponent *>& SigChildBeforeAdd(VOID);

	//: Called when a child component is added to the component.
	CSignal_v1<CComponent *>& SigChildAdd(VOID);

	//: Called when a child component is about to be removed from the component.
	CSignal_v1<CComponent *>& SigChildBeforeRemove(VOID);
	
	//: Called when a child component is removed from the component.
	CSignal_v1<CComponent *>& SigChildRemove(VOID);

	//: Called when needing to convert parent coordinates into coordinates used by this component.
	//: <p>This is used in 3D guis to convert from screen coordinates to gui coordinates.
	//: (project the screen cordinates down on the plane that the gui is laying on)</p>
	CSignal_v2<int &, int &>& SigTransformCoords(VOID);
	
	//: Called when needing to find the minimum size of the component.
	//: Normally used by the themes.
	CSignal_v1<CPos &>& SigGetMinimumSize(VOID);

	//: Called when needing to find the maximum size of the component.
	//: Normally used by the themes.
	CSignal_v1<CPos &>& SigGetMaximumSize(VOID);

	//: Called when needing to find the preferred size of the component.
	//: Normally used by the themes.
	CSignal_v1<CPos &>& SigGetPreferredSize(VOID);

	//: Called when send message. 
	CSignal_v3<int , int, int>& SigMessage(VOID); 

	//: Called when SetAlpha Function 
	CSignal_v1<unsigned char>& SigSetAlpha(VOID);

	//: Called When SetColor Function
	CSignal_v3<unsigned char, unsigned char, unsigned char>& SigSetColor(VOID);

// virtual operator
public:

	VOID SendMsg (CComponent *pComponent,int nMsg,int lParam,int wParam);
	
	virtual VOID TopTabOrder(VOID);
	virtual LRESULT OnWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) { return 0;}
 
//! Implementation:
protected:

	CComponent_Generic *m_pImpl;

	friend class CComponent_Generic;

};

END_GUI

#endif
