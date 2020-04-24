#ifndef __USERMESSAGEID_H__
#define __USERMESSAGEID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_SET_DATA				(WM_USER + 3)
#define WM_USER_ON_SELECTION_CANCEL		(WM_USER + 4)
#define WM_USER_ON_SELECTION_OK			(WM_USER + 5)
#define WM_USER_ON_NEW_SELECTION		(WM_USER + 6)


#define WM_USER_SELECT_EFFECT_ITEM		(WM_USER + 10)
#define WM_USER_EFFECT_LOAD				(WM_USER + 11)
#define WM_USER_ITEM_NAME_CHANGE		(WM_USER + 12)


class CEffItem;
class CEffAct;

enum TREE_ITEM_ATTR
{
	TI_ATTR_EFFECT_NONE,
	TI_ATTR_EFFECT_FRAME,
	TI_ATTR_EFFECT_ATOMIC
};

typedef struct _tagTIAttr
{
	int nAttrID;
	CEffItem *pEffItem;
	int h;
}TI_ATTR;

typedef struct _tagActAttr
{
	CEffAct *pAct;
}ACT_ATTR;


#endif // !defined(__USERMESSAGEID_H__)