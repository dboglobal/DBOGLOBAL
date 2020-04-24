#pragma once

#include "NtlItem.h"
#include "ceventhandler.h"

class CNtlSob;
class CNtlPLItem;
class CNtlPLCharacter;
class CNtlSubWeaponController;

struct sEmblemFactor;

struct SEquipItem
{
    RwUInt32	uiItemTblId;
    RwUInt8		byGrade;
    CNtlPLItem	*pPLItem;
    RwUInt8		bCharacterAdd;
    RwBool		bReCreate;	
};

class CNtlSobCharEquipProxy
{
private:

    enum { EMaxStorageWeapon = 2 };	

    typedef struct _SStorageEquipItem
    {
        RwUInt8		bySlotIdx;
        RwUInt32	uiItemTblId;
        RwUInt8		byGrade;
        CNtlPLItem	*pPLItem;
    }SStorageEquipItem;

    RwBool						m_bUIRender;
    CNtlSob						*m_pSobObj;
    CNtlPLCharacter				*m_pPLCharacter;						/** character resource */
    SEquipItem					m_arrSlotItem[NTL_MAX_EQUIP_ITEM_SLOT];
    RwBool						m_bSunWeaponReCreate;
    CNtlSubWeaponController		*m_pSubWeaponController;				/** gem 과 같이 sub weapon control이 필요한 경우 */

    RwBool						m_bActiveSubWeapon;
    RwUInt8						m_byStorageWeaponNum;					/** sub weapon 을 사용하면서 보관중인 main weapon 갯수 */
    SStorageEquipItem			m_sStorageWeapon[EMaxStorageWeapon];	/** sub weapon 을 사용하면서 보관할 main weapon buffer */

    RwBool						m_bVisible;
    RwBool						m_bResLoaded;
    RwBool						m_bResLoadComplete;
    RwUInt8						m_byAlpha;
    RwUInt8						m_byRed;
    RwUInt8						m_byGreen;
    RwUInt8						m_byBlue;

    // guild emblem
    sEmblemFactor				*m_pEmblemFactor;
    RwTexture					*m_pTexEmblem;

private:

    CNtlPLItem*		CreatePLItem(RwUInt8 bySlotIdx, const RwChar *pModelName, RwUInt32 uiSerialId, RwUInt8 byUpgrade);
    void			DeleteAllItem(void);    

    void			MakeItemModelName(std::string& strModelName, RwUInt8 byModeNameRule);
    void			MakeItemModelName(std::string& strModelName, RwUInt8 byRace, RwUInt8 byGender, RwUInt8 bySkinColor, RwUInt8 byModeNameRule);

    void			StorageMainWeapon(void);
    void			RestoreMainWeapon(void);

    void			ProcNotAttachSubWeapon(CNtlPLItem *pPLItem);

    void			UpdateResourceLoadCheck(RwReal fElapsed);

    void            SetAnimationCostumeItem(CNtlPLItem* pItem);                     ///< 코스튬 아이템에 애니메이션을 세팅한다 (등뒤에지는 특수한 아이템들의 애니메이션)

public:
    CNtlSobCharEquipProxy();
    CNtlSobCharEquipProxy& operator = (CNtlSobCharEquipProxy& EquipProxy);
    ~CNtlSobCharEquipProxy();

    void	SetData(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter, RwBool bUIRender);
    void	Update(RwReal fElapsed);
    void	HandleEvents(RWS::CMsg &pMsg);

    void	NotifyOwnerStateEnter(RwUInt32 uiStateId);
    void	NotifyOwnerStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState);

    //-------------------------------------------------------------------------------
    // event handler
    //-------------------------------------------------------------------------------
    void	SobEquipCreateEventHandler(RWS::CMsg &pMsg);
    void	SobEquipChangeEventHandler(RWS::CMsg &pMsg);
    void	SobSubWeaponActiveEventHandler(RWS::CMsg &pMsg);
    void	SobSubWeaponDeActiveEventHandler(RWS::CMsg &pMsg);

    //////////////////////////////////////////////////////////////////////////

    void	AddWorld(void);
    void	RemoveWorld(void);	

    void	SetVisible(RwBool bVisible);
    void    SetVisible(RwBool bVisible, RwUInt8 bySlotIdx);
    void	SetAlpha(RwUInt8 byAlpha);
    void	SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
    void	SetPicking(RwBool bPicking);
    RwV3d	GetWeaponBonePosition(const RwChar *pBoneName);
    RwV3d	GetSubWeaponBonePosition(const RwChar *pBoneName);

    void	DetachConvertClassEquipItem(void);
    void	AttachConvertClassEquipItem(CNtlPLCharacter *pPLCharacter);

    void	SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,	
        RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
        RwUInt8 byTypeC, RwUInt8 byTypeCColor);

    void			CreateEquipItem(RwUInt32 uiTblId, RwUInt8 bySlotIdx, RwUInt8 byGrade);
    void			DeleteEquipItem(RwUInt8 bySlotIdx);

    SEquipItem*					GetEquipItem(RwInt32 iSlotIdx);	
    CNtlSubWeaponController*	GetSubWeaponController(void) const;
    RwUInt8						GetStorageWeaponNum(void) const;
    SStorageEquipItem*			GetStorageWeaponItem(RwInt32 iSlotIdx);	
    RwBool						GetActiveSubWeapon(void) const;

    void						ResourceLoadComplete(RwBool bVisible);
    RwBool						IsResourceLoadComplete(void) const;
    RwBool						IsResourceLoaded(void) const;
};