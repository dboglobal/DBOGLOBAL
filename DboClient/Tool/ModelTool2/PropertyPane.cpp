// PropertyPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "AnimPlayPane.h"
#include "PropertyPane.h"
#include "LinkEffectPane.h"
#include "AnimSetPane.h"
#include "NtlPLHelpers.h"
#include "NtlSoundManager.h"

const RwChar* g_BlendString[] = {	"rwBLENDZERO",			\
                                    "rwBLENDONE",			\
                                    "rwBLENDSRCCOLOR",		\
                                    "rwBLENDINVSRCCOLOR",	\

                                    "rwBLENDSRCALPHA",		\
                                    "rwBLENDINVSRCALPHA",	\
                                    "rwBLENDDESTALPHA",		\
                                    "rwBLENDINVDESTALPHA",	\

                                    "rwBLENDDESTCOLOR",		\
                                    "rwBLENDINVDESTCOLOR",	\
                                    "rwBLENDDESTCOLOR",		\
                                    "rwBLENDSRCALPHASAT" };
const RwInt32 g_nBlendMaxCount = 12;


#define IDC_PROPERTY_GRID               101
#define EFFECT_HIT_WORD_PREFIX          "TXT_"          ///< Word Effect의 Prefix
#define EFFECT_POST_EFFECT_PREFIX       "POST_"         ///< Post Effect의 Prefix   

#define RwRGBA2RGB(color) RGB(color.red, color.green, color.blue)
#define RGB2RwRGBA(rw, rgb) rw.red = GetRValue(rgb); rw.green = GetGValue(rgb); rw.blue = GetBValue(rgb)

CPropertyPane* CPropertyPane::m_pInstance = NULL;

// CPropertyPane

IMPLEMENT_DYNCREATE(CPropertyPane, CXTResizeFormView)

CPropertyPane::CPropertyPane()
	: CXTResizeFormView(CPropertyPane::IDD)
{
    m_pInstance = this;
    m_pClump = NULL;
    m_pEventAnim = NULL;
    m_pObject = NULL;
    m_pItem = NULL;
    m_pCharacter= NULL;
    m_pAnimData = NULL;
    m_ePropertyMode = PROPERTY_MODE_NONE;
}

CPropertyPane::~CPropertyPane()
{
}

void CPropertyPane::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPropertyPane, CXTResizeFormView)
    ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()


// CPropertyPane diagnostics

#ifdef _DEBUG
void CPropertyPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropertyPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPropertyPane message handlers

void CPropertyPane::OnInitialUpdate()
{
    USES_CONVERSION;

    CXTResizeFormView::OnInitialUpdate();
    
    CRect rc;
    GetDlgItem(IDC_STATIC)->GetWindowRect(&rc);
    ScreenToClient(&rc);

    
    if(m_property.Create(rc, this, IDC_PROPERTY_GRID))
    {
        m_property.SetVariableItemsHeight(TRUE);        
        m_property.ShowToolBar(TRUE);
        m_property.ShowHelp(FALSE);
        m_property.HighlightChangedItems(TRUE);

        // Create Hit Event Category
        m_pCatHitEvent = m_property.AddCategory("HitEvent Properties");
        
        // Add Child Items to Hit Event
         m_pItemTime              = (CCustomItemSpinDouble*)m_pCatHitEvent->AddChildItem(new CCustomItemSpinDouble("Time"));                  
         m_pItemPowerEffect       = (CXTPPropertyGridItemBool*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemBool("PowerEffect", TRUE));                 
         m_pItemTargetBehavior    = (CXTPPropertyGridItemEnum*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemEnum("TargetBehavior"));
         m_pItemKB2Push           = (CXTPPropertyGridItemBool*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemBool("KB to Push"));
         m_pItemHandType          = (CXTPPropertyGridItemEnum*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemEnum("HandType"));
         m_pItemProjectileName    = m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItem("ProjectileEffect Name"));
         m_pItemProjectileType    = (CXTPPropertyGridItemEnum*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemEnum("ProjectileType"));
         m_pItemProjectileShot    = (CXTPPropertyGridItemEnum*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemEnum("Projectile Shot Type"));
         m_pItemBoneName          = m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItem("BoneName"));         
         m_pItemSubWeaponBoneFlag = (CXTPPropertyGridItemFlags*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemFlags("SubWeapon Bone"));
         m_pItemProjSpeed         = (CCustomItemSpinDouble*)m_pCatHitEvent->AddChildItem(new CCustomItemSpinDouble("Projectile Speed"));         
         m_pItemTargetAttach	  = (CXTPPropertyGridItemBool*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemBool("Target Attach"));
         m_pItemTargetEffectName  = m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItem("TargetEffect Name"));      
		 m_pItemTargetEffectType  = (CXTPPropertyGridItemEnum*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemEnum("TargetEffect Type"));
         m_pItemSubTargetEffect   = m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItem("Sub Target Effect"));
         m_pItemTargetHeight      = (CCustomItemSpinDouble*)m_pCatHitEvent->AddChildItem(new CCustomItemSpinDouble("Target Height"));
         m_pItemTargetSoundName   = (CCustomItemFileBox*)m_pCatHitEvent->AddChildItem(new CCustomItemFileBox("TargetSound Name", FILTER_SOUND));
         m_pItemSoundType         = (CXTPPropertyGridItemEnum*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemEnum("TargetSound Type"));
         m_pItemHitSoundEcho      = (CXTPPropertyGridItemBool*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemBool("Hit Sound Echo"));
         m_pItemWordEffect        = m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItem("Word Effect"));
         m_pItemCameraShake       = (CXTPPropertyGridItemBool*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemBool("Camera Shake"));
         m_pItemShakeFactor       = (CCustomItemSpinDouble*)m_pItemCameraShake->AddChildItem(new CCustomItemSpinDouble("Shake Factor"));
         m_pItemShakeMaxHeight    = (CCustomItemSpinDouble*)m_pItemCameraShake->AddChildItem(new CCustomItemSpinDouble("Shake Max Height Factor"));
         m_pHitHosidanOffset      = (CCustomItemOffset*)m_pCatHitEvent->AddChildItem(new CCustomItemOffset("Target Effect Offset"));
         m_pHitHosidanWaitTime    = (CCustomItemSpinDouble*)m_pCatHitEvent->AddChildItem(new CCustomItemSpinDouble("Target Effect Start WaitTIme"));
         m_pHitHosidanSpeed       = (CCustomItemSpinDouble*)m_pCatHitEvent->AddChildItem(new CCustomItemSpinDouble("Target Effect Speed"));
         m_pHitHissidanApplyAngle = (CXTPPropertyGridItemBool*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemBool("Hissidan Apply Angle"));
         m_pHitHissidianDirection = (CCustomItemOffset*)m_pCatHitEvent->AddChildItem(new CCustomItemOffset("Hissidan Start Direction", CCustomItemOffset::TYPE_XY));
         m_pHitMultiHissidanCount = (CXTPPropertyGridItemNumber*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemNumber("Multi Hissidan Count"));
         m_pHitMultiHissidanIndex = (CXTPPropertyGridItemEnum*)m_pCatHitEvent->AddChildItem(new CXTPPropertyGridItemEnum("Multi Hissidan Index"));
         m_pHitMultiHissidanDir   = (CCustomItemOffset*)m_pCatHitEvent->AddChildItem(new CCustomItemOffset("Multi Hissidan Dir", CCustomItemOffset::TYPE_XY));
        
         m_pItemTargetBehavior->GetConstraints()->AddConstraint("None", TARGET_BEHAVIOR_NONE);
         m_pItemTargetBehavior->GetConstraints()->AddConstraint("Push", TARGET_BEHAVIOR_PUSH);
         m_pItemTargetBehavior->GetConstraints()->AddConstraint("Toss", TARGET_BEHAVIOR_TOSS);
         m_pItemTargetBehavior->GetConstraints()->AddConstraint("Fall Down", TARGET_BEHAVIOR_FALL_DOWN);
         m_pItemTargetBehavior->GetConstraints()->AddConstraint("Knock Down", TARGET_BEHAVIOR_KNOCK_DOWN);
         m_pItemHandType->GetConstraints()->AddConstraint("Left Hand", HAND_TYPE_LEFT);
         m_pItemHandType->GetConstraints()->AddConstraint("Right Hand", HAND_TYPE_RIGHT);
         m_pItemSoundType->GetConstraints()->AddConstraint("Effect", CHANNEL_GROUP_EFFECT_SOUND);
         m_pItemSoundType->GetConstraints()->AddConstraint("Voice", CHANNEL_GROUP_VOICE_SOUND);
         m_pItemProjectileShot->GetConstraints()->AddConstraint("Character", BONE_CHARACTER);
         m_pItemProjectileShot->GetConstraints()->AddConstraint("Weapon", BONE_WEAPON);
         m_pItemProjectileShot->GetConstraints()->AddConstraint("Sub Weapon", BONE_SUB_WEAPON);
         m_pItemSubWeaponBoneFlag->GetConstraints()->AddConstraint("All SubWeapon Bone", SUB_WEAPON_PROJECTILE_1 + SUB_WEAPON_PROJECTILE_2 + SUB_WEAPON_PROJECTILE_3);
         m_pItemSubWeaponBoneFlag->GetConstraints()->AddConstraint("Projectile_1", SUB_WEAPON_PROJECTILE_1);
         m_pItemSubWeaponBoneFlag->GetConstraints()->AddConstraint("Projectile_2", SUB_WEAPON_PROJECTILE_2);
         m_pItemSubWeaponBoneFlag->GetConstraints()->AddConstraint("Projectile_3", SUB_WEAPON_PROJECTILE_3);
         m_pItemProjectileType->GetConstraints()->AddConstraint("PROJ_BALL", BEID_PROJ_BALL);
         m_pItemProjectileType->GetConstraints()->AddConstraint("PROJ_BEAM", BEID_PROJ_BEAM);
         m_pItemProjectileType->GetConstraints()->AddConstraint("PROJ_HISSIDAN", BEID_PROJ_HISSIDAN);
         m_pItemProjectileType->GetConstraints()->AddConstraint("PROJ_HELLZONE", BEID_PROJ_HELLZONE);
         m_pItemProjectileType->GetConstraints()->AddConstraint("PROJ_MULTI_HISSIDAN", BEID_PROJ_MULTI_HISSIDAN);
         m_pItemProjectileType->GetConstraints()->AddConstraint("PROJ_MAGARE", BEID_PROJ_MAGARE);
		 m_pItemTargetEffectType->GetConstraints()->AddConstraint("NONE", TARGET_EFFECT_TYPE_NONE);
		 m_pItemTargetEffectType->GetConstraints()->AddConstraint("SIDE", TARGET_EFFECT_TYPE_SIDE);
		 m_pItemTargetEffectType->GetConstraints()->AddConstraint("FRONT", TARGET_EFFECT_TYPE_FRONT);
       
         m_pItemProjectileName->SetFlags(xtpGridItemHasComboButton);         
         m_pItemTargetEffectName->SetFlags(xtpGridItemHasComboButton);
         m_pItemSubTargetEffect->SetFlags(xtpGridItemHasComboButton);
         m_pItemBoneName->SetFlags(xtpGridItemHasComboButton);
         m_pItemWordEffect->SetFlags(xtpGridItemHasComboButton);

         m_pItemTime->SetID(ID_HIT_TIME);
         m_pItemPowerEffect->SetID(ID_HIT_POWER_EFFECT);
         m_pItemTargetBehavior->SetID(ID_HIT_TARGET_BRHAVIOR);
         m_pItemKB2Push->SetID(ID_HIT_KB2PUSH);
         m_pItemHandType->SetID(ID_HIT_HAND_TYPE);
         m_pItemProjectileName->SetID(ID_HIT_PROJECTILE_NAME);
         m_pItemProjectileType->SetID(ID_HIT_PROJECTILE_TYPE);
         m_pItemProjectileShot->SetID(ID_HIT_PROJECTILE_SHOT);
         m_pItemProjSpeed->SetID(ID_HIT_PROJECTILE_SPEED);         
         m_pItemBoneName->SetID(ID_HIT_BONE_NAME);
         m_pItemSubWeaponBoneFlag->SetID(ID_HIT_SUBWEAPON_BONE_FLAG);
         m_pItemTargetAttach->SetID(ID_HIT_TARGET_ATTACH);
         m_pItemTargetEffectName->SetID(ID_HIT_TARGET_EFFECT_NAME);
		 m_pItemTargetEffectType->SetID(ID_HIT_TARGET_EFFECT_TYPE);
         m_pItemSubTargetEffect->SetID(ID_HIT_SUB_TARGET_EFFECT);
         m_pItemTargetSoundName->SetID(ID_HIT_TARGET_SOUND_NAME);
         m_pItemSoundType->SetID(ID_HIT_SOUND_TYPE);
         m_pItemHitSoundEcho->SetID(ID_HIT_SOUND_ECHO);
         m_pItemTargetHeight->SetID(ID_HIT_TARGET_HEIGHT);
         m_pItemWordEffect->SetID(ID_HIT_WORD_EFFECT);
         m_pItemCameraShake->SetID(ID_HIT_CAMERA_SHAKE);
         m_pItemShakeFactor->SetID(ID_HIT_SHAKE_FACTOR);
         m_pItemShakeMaxHeight->SetID(ID_HIT_SHAKE_MAX_HEIGHT);
         m_pHitHosidanOffset->SetID(ID_HIT_HOIDAN_OFFSET);
         m_pHitHosidanWaitTime->SetID(ID_HIT_HOIDAN_START_WAIT_TIME);
         m_pHitHosidanSpeed->SetID(ID_HIT_HOIDAN_SPEED);
         m_pHitHissidianDirection->SetID(ID_HIT_HISSDIAN_DIRECTION);
         m_pHitHissidanApplyAngle->SetID(ID_HIT_HISSIDAN_APPLY_ANGLE);
         m_pHitMultiHissidanCount->SetID(ID_HIT_MULTI_HISSIDAN_COUNT);
         m_pHitMultiHissidanIndex->SetID(ID_HIT_MULTI_HISSIDAN_INDEX);
         m_pHitMultiHissidanDir->SetID(ID_HIT_MULTI_HISSIDAN_DIR);

         m_pCatHitEvent->Expand();      

        // Create Visual Effect Category
         m_pCatVisualEffectEvent = m_property.AddCategory("VisualEffect Properties");
         m_pVETime        = (CCustomItemSpinDouble*)m_pCatVisualEffectEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
         m_pVEName        = m_pCatVisualEffectEvent->AddChildItem(new CXTPPropertyGridItem("Effect Name"));
         m_pVEProjectileType = (CXTPPropertyGridItemBool*)m_pCatVisualEffectEvent->AddChildItem(new CXTPPropertyGridItemBool("Projectile Type"));
         m_pVEAttach      = (CXTPPropertyGridItemBool*)m_pCatVisualEffectEvent->AddChildItem(new CXTPPropertyGridItemBool("Effect Attach"));
         m_pVEAttachType  = (CXTPPropertyGridItemEnum*)m_pCatVisualEffectEvent->AddChildItem(new CXTPPropertyGridItemEnum("Attach Type"));
         m_pVEBoneType    = (CXTPPropertyGridItemEnum*)m_pCatVisualEffectEvent->AddChildItem(new CXTPPropertyGridItemEnum("Attach Bone Type"));
         m_pVEBoneName    = m_pCatVisualEffectEvent->AddChildItem(new CXTPPropertyGridItem("Bone Name"));
         m_pVEWeaponBoneName = m_pCatVisualEffectEvent->AddChildItem(new CXTPPropertyGridItem("Weapon Bone Name"));
         m_pVEOffsetPos   = (CCustomItemOffset*)m_pCatVisualEffectEvent->AddChildItem(new CCustomItemOffset("Offset Pos"));                                    
         m_pVEApplyScale  = (CXTPPropertyGridItemBool*)m_pCatVisualEffectEvent->AddChildItem(new CXTPPropertyGridItemBool("Apply Scale", TRUE));

         m_pVEName->SetFlags(xtpGridItemHasComboButton | xtpGridItemHasEdit);         
         m_pVEBoneName->SetFlags(xtpGridItemHasComboButton);
         m_pVEWeaponBoneName->SetFlags(xtpGridItemHasComboButton);

         m_pVEAttachType->GetConstraints()->AddConstraint("Position", VE_ATTACH_POS);
         m_pVEAttachType->GetConstraints()->AddConstraint("Bone", VE_ATTACH_BONE);

         m_pVEBoneType->GetConstraints()->AddConstraint("Character Bone", BONE_CHARACTER);
         m_pVEBoneType->GetConstraints()->AddConstraint("Weapon Bone", BONE_WEAPON);
         m_pVEBoneType->GetConstraints()->AddConstraint("Sub Weapon Bone", BONE_SUB_WEAPON);
         
         m_pVEWeaponBoneName->GetConstraints()->AddConstraint("Weapon_VE_1");
         m_pVEWeaponBoneName->GetConstraints()->AddConstraint("Weapon_VE_2");
         m_pVEWeaponBoneName->GetConstraints()->AddConstraint("Weapon_VE_3");

         m_pVETime->SetID(ID_VE_TIME);
         m_pVEName->SetID(ID_VE_NAME);
         m_pVEAttach->SetID(ID_VE_ATTACH);
         m_pVEAttachType->SetID(ID_VE_ATTACH_BONE);         
         m_pVEProjectileType->SetID(ID_VE_PROJECTILE_TYPE);
         m_pVEBoneType->SetID(ID_VE_BONE_TYPE);
         m_pVEBoneName->SetID(ID_VE_BONE_NAME);
         m_pVEWeaponBoneName->SetID(ID_VE_SUB_WEAPON_BONE_NAME);         
         m_pVEOffsetPos->SetID(ID_VE_OFFSET_POS);         
         m_pVEApplyScale->SetID(ID_VE_APPLY_SCALE);

         m_pCatVisualEffectEvent->Expand();

        // Create Visual Sound Effect Category
        m_pCatVisualSoundEvent = m_property.AddCategory("Visual Sound Properties");
        m_pSETime       = (CCustomItemSpinDouble*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pSESoundName1  = (CCustomItemFileBox*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemFileBox("Sound File 1", FILTER_SOUND));
        m_pSESoundName2  = (CCustomItemFileBox*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemFileBox("Sound File 2", FILTER_SOUND));
        m_pSESoundName3  = (CCustomItemFileBox*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemFileBox("Sound File 3", FILTER_SOUND));
        m_pSESoundName4  = (CCustomItemFileBox*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemFileBox("Sound File 4", FILTER_SOUND));
        m_pSESoundType  = (CXTPPropertyGridItemEnum*)m_pCatVisualSoundEvent->AddChildItem(new CXTPPropertyGridItemEnum("Sound Type"));
        m_pSESoundLoop  = (CXTPPropertyGridItemBool*)m_pCatVisualSoundEvent->AddChildItem(new CXTPPropertyGridItemBool("Sound Loop"));
        m_pSESoundVolume = (CCustomItemSpin*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemSpin("Sound Volume", 0, 100));
        m_pSESoundDist   = (CCustomItemSpin*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemSpin("Sound Distance", 0, 100));
        m_pSESoundDecayDist = (CCustomItemSpin*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemSpin("Sound Decay Dist", 0, 100));
        m_pSESoundPitchMin  = (CCustomItemSpinDouble*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemSpinDouble("Sound Pitch Min", _T("%.2f"), 0.0f, 10.0f));
        m_pSESoundPitchMax  = (CCustomItemSpinDouble*)m_pCatVisualSoundEvent->AddChildItem(new CCustomItemSpinDouble("Sound Pitch Max", _T("%.2f"), 0.0f, 10.0f));

        m_pSESoundType->GetConstraints()->AddConstraint("Effect", CHANNEL_GROUP_EFFECT_SOUND);
        m_pSESoundType->GetConstraints()->AddConstraint("Voice", CHANNEL_GROUP_VOICE_SOUND);

        m_pSETime->SetID(ID_SE_TIME);
        m_pSESoundName1->SetID(ID_SE_SOUND_NAME1);
        m_pSESoundName2->SetID(ID_SE_SOUND_NAME2);
        m_pSESoundName3->SetID(ID_SE_SOUND_NAME3);
        m_pSESoundName4->SetID(ID_SE_SOUND_NAME4);
        m_pSESoundType->SetID(ID_SE_SOUND_TYPE);
        m_pSESoundLoop->SetID(ID_SE_SOUND_LOOP);
        m_pSESoundVolume->SetID(ID_SE_SOUND_VOLUME);
        m_pSESoundDist->SetID(ID_SE_SOUND_DIST);
        m_pSESoundDecayDist->SetID(ID_SE_SOUND_DECAY_DIST);
        m_pSESoundPitchMin->SetID(ID_SE_SOUND_PITCH_MIN);
        m_pSESoundPitchMax->SetID(ID_SE_SOUND_PITCH_MAX);

        m_pCatVisualSoundEvent->Expand();

        // Create Foot Step Effect Category
        m_pCatFootStepEvent = m_property.AddCategory("Foot Step Properties");
        m_pFSTime = (CCustomItemSpinDouble*)m_pCatFootStepEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pFSType = (CXTPPropertyGridItemEnum*)m_pCatFootStepEvent->AddChildItem(new CXTPPropertyGridItemEnum("TYPE"));
        m_pFSMobType = (CXTPPropertyGridItemEnum*)m_pCatFootStepEvent->AddChildItem(new CXTPPropertyGridItemEnum("Mob Type"));

        m_pFSType->GetConstraints()->AddConstraint("LEFT FOOT", FOOT_LEFT);
        m_pFSType->GetConstraints()->AddConstraint("RIGHT FOOT", FOOT_RIGHT);

        m_pFSMobType->GetConstraints()->AddConstraint("NORMAL", FOOT_TYPE_NORMAL);
        m_pFSMobType->GetConstraints()->AddConstraint("LARGE", FOOT_TYPE_LARGE);

        m_pFSTime->SetID(ID_FS_TIME);
        m_pFSType->SetID(ID_FS_TYPE);
        m_pFSMobType->SetID(ID_FS_MOB_TYPE);

        m_pCatFootStepEvent->Expand();

        // Create Link Effect Category
        m_pCatLinkEffectEvent = m_property.AddCategory("Link Event Properties");
        m_pLEName      = m_pCatLinkEffectEvent->AddChildItem(new CXTPPropertyGridItem("Effect Name"));
        m_pLEAttachBone = (CXTPPropertyGridItemBool*)m_pCatLinkEffectEvent->AddChildItem(new CXTPPropertyGridItemBool("Attach Bone"));
        m_pLEBoneName  = m_pCatLinkEffectEvent->AddChildItem(new CXTPPropertyGridItem("Bone Name"));
        m_pLEOffsetPos = (CCustomItemOffset*)m_pCatLinkEffectEvent->AddChildItem(new CCustomItemOffset("Offset Pos"));        
        m_pLEOffsetRot = (CCustomItemOffset*)m_pCatLinkEffectEvent->AddChildItem(new CCustomItemOffset("Offset Rot"));

        m_pLEOffsetRot->SetHidden(TRUE);
        m_pLEName->SetReadOnly(TRUE);
        m_pLEBoneName->SetFlags(xtpGridItemHasComboButton);
        
        m_pLEBoneName->SetID(ID_LE_BONE_NAME);
        m_pLEAttachBone->SetID(ID_LE_ATTACH_BONE);        
        m_pLEOffsetPos->SetID(ID_LE_OFFSET_POS);
        m_pLEOffsetRot->SetID(ID_LE_OFFSET_ROT);

        m_pCatLinkEffectEvent->Expand();

        // Create Weight TIme Category
        m_pCatSlowTimeEvent = m_property.AddCategory("Weight Time Event Properties");
        m_pSTTime       = (CCustomItemSpinDouble*)m_pCatSlowTimeEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pSTLifeTime   = (CCustomItemSpinDouble*)m_pCatSlowTimeEvent->AddChildItem(new CCustomItemSpinDouble("Life TIme"));
        m_pSTWeight     = (CCustomItemSpinDouble*)m_pCatSlowTimeEvent->AddChildItem(new CCustomItemSpinDouble("Weight Time"));


        m_pSTTime->SetID(ID_ST_TIME);
        m_pSTLifeTime->SetID(ID_ST_LIFE_TIME);
        m_pSTWeight->SetID(ID_ST_WEIGHT);

        m_pCatSlowTimeEvent->Expand();

        // Create Trace Event Category
        m_pCatTraceEvent    = m_property.AddCategory("Trace Event Properties");
        m_pTEStartTime      = (CCustomItemSpinDouble*)m_pCatTraceEvent->AddChildItem(new CCustomItemSpinDouble("Start Time"));
        m_pTELifeTime       = (CCustomItemSpinDouble*)m_pCatTraceEvent->AddChildItem(new CCustomItemSpinDouble("Life Time"));
        m_pTEEdgeLifeTime   = (CCustomItemSpinDouble*)m_pCatTraceEvent->AddChildItem(new CCustomItemSpinDouble("Edge Life Time"));
        m_pTEAttachType     = (CXTPPropertyGridItemEnum*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemEnum("Attach Type"));
        m_pTETraceKind      = (CXTPPropertyGridItemEnum*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemEnum("Trace Kind"));
        m_pTEStartBoneName  = m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItem("Start Bone Name"));
        m_pTEEndBoneName    = m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItem("End Bone Name"));
        m_pTETexture        = (CCustomItemFileBox*)m_pCatTraceEvent->AddChildItem(new CCustomItemFileBox("Texture Name", FILTER_TEXTURE));
        m_pTEStartBoneOffset= (CCustomItemOffset*)m_pCatTraceEvent->AddChildItem(new CCustomItemOffset("Start Bone Offset"));
        m_pTEEndBoneoffset  = (CCustomItemOffset*)m_pCatTraceEvent->AddChildItem(new CCustomItemOffset("End Bone Offset"));
        m_pTEEdgeGap = (CCustomItemSpinDouble*)m_pCatTraceEvent->AddChildItem(new CCustomItemSpinDouble("Edge Gap"));
        m_pTESplinePointCount = (CCustomItemSpin*)m_pCatTraceEvent->AddChildItem(new CCustomItemSpin("Spline Point Count"));
        m_pTEMaxEdgeCount   = (CXTPPropertyGridItemNumber*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemNumber("Max Edge Count"));
        m_pTEMaxLength      = (CCustomItemSpinDouble*)m_pCatTraceEvent->AddChildItem(new CCustomItemSpinDouble("Max Length(m)"));
        m_pTESrcBlend       = (CXTPPropertyGridItemEnum*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemEnum("Src Alpha Blend", rwBLENDSRCALPHA));
        m_pTEDestBlend      = (CXTPPropertyGridItemEnum*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemEnum("Dest Alpha Blend", rwBLENDONE));
        m_pTEStartColor     = (CXTPPropertyGridItemColor*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemColor("Start Color"));
        m_pTEStartAlpha     = (CXTPPropertyGridItemNumber*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemNumber("Start Alpha"));
        m_pTEEndColor       = (CXTPPropertyGridItemColor*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemColor("End Color"));
        m_pTEEndAlpha       = (CXTPPropertyGridItemNumber*)m_pCatTraceEvent->AddChildItem(new CXTPPropertyGridItemNumber("End Alpha"));

        for(int i = 0; i < g_nBlendMaxCount; ++i)
        {
            m_pTESrcBlend->GetConstraints()->AddConstraint((g_BlendString[i]), i + 1);
            m_pTEDestBlend->GetConstraints()->AddConstraint((g_BlendString[i]), i + 1);
        }

        m_pTEAttachType->GetConstraints()->AddConstraint("Character Bone", SEventTrace::CHARACTER_BONE);
        m_pTEAttachType->GetConstraints()->AddConstraint("Weapon Bone", SEventTrace::WEAPONE_BONE);
        m_pTEAttachType->GetConstraints()->AddConstraint("Sub Weapon bone", SEventTrace::SUB_WEAPON_BONE);

        m_pTETraceKind->GetConstraints()->AddConstraint("Event Trace", SEventTrace::EVENT_TRACE);
        m_pTETraceKind->GetConstraints()->AddConstraint("Item Trace", SEventTrace::ITEM_TRACE);

        m_pTEStartBoneName->SetFlags(xtpGridItemHasComboButton);
        m_pTEEndBoneName->SetFlags(xtpGridItemHasComboButton);

        m_pTEStartTime->SetID(ID_TE_TIME);
        m_pTELifeTime->SetID(ID_TE_LIFE_TIME);
        m_pTEEdgeLifeTime->SetID(ID_TE_EDGE_LIFE_TIME);
        m_pTESrcBlend->SetID(ID_TE_SRC_BLEND);
        m_pTEDestBlend->SetID(ID_TE_DEST_BLEND);
        m_pTEAttachType->SetID(ID_TE_ATTACH_TYPE);
        m_pTETraceKind->SetID(ID_TE_TRACE_KIND);
        m_pTEStartBoneName->SetID(ID_TE_START_BONE_NAME);
        m_pTEEndBoneName->SetID(ID_TE_END_BONE_NAME);
        m_pTETexture->SetID(ID_TE_TEXTURE_NAME);
        m_pTEStartBoneOffset->SetID(ID_TE_START_BONE_OFFSET);
        m_pTEEndBoneoffset->SetID(ID_TE_END_BONE_OFFSET);
        m_pTEEdgeGap->SetID(ID_TE_FRAME_SKIP_COUNT);
        m_pTESplinePointCount->SetID(ID_TE_SPLINE_POINT_COUNT);
        m_pTEMaxEdgeCount->SetID(ID_TE_MAX_EDGE_COUNT);
        m_pTEMaxLength->SetID(ID_TE_MAX_LENGTH);
        m_pTEStartColor->SetID(ID_TE_START_COLOR);
        m_pTEStartAlpha->SetID(ID_TE_START_ALPHA);
        m_pTEEndColor->SetID(ID_TE_END_COLOR);
        m_pTEEndAlpha->SetID(ID_TE_END_ALPHA);

        m_pCatTraceEvent->Expand();

        // Sub Weapon Event Properties
        m_pCatSubWeaponEvent = m_property.AddCategory("SubWeapon Event Properties");
        m_pSWTime = (CCustomItemSpinDouble*)m_pCatSubWeaponEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pSWActive = (CXTPPropertyGridItemEnum*)m_pCatSubWeaponEvent->AddChildItem(new CXTPPropertyGridItemEnum("Active"));

        m_pSWActive->GetConstraints()->AddConstraint("Active", SUB_WEAPON_ACTIVE);
        m_pSWActive->GetConstraints()->AddConstraint("DeActive", SUB_WEAPON_DEACTIVE);

        m_pSWTime->SetID(ID_SW_TIME);
        m_pSWActive->SetID(ID_SW_ACTIVE);

        m_pCatSubWeaponEvent->Expand();

        // Post Effect Event Properties
        m_pCatPostEffectEvent = m_property.AddCategory("Post Effect Event Properties");
        m_pPETime = (CCustomItemSpinDouble*)m_pCatPostEffectEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pPEEffectName = (CXTPPropertyGridItem*)m_pCatPostEffectEvent->AddChildItem(new CXTPPropertyGridItem("Effect"));
        m_pPETarget = (CXTPPropertyGridItemEnum*)m_pCatPostEffectEvent->AddChildItem(new CXTPPropertyGridItemEnum("Target"));
        m_pPEOffset = (CCustomItemOffset*)m_pCatPostEffectEvent->AddChildItem(new CCustomItemOffset("Offset"));
        m_pPECenterFix = (CXTPPropertyGridItemBool*)m_pCatPostEffectEvent->AddChildItem(new CXTPPropertyGridItemBool("Center Fix"));
        m_pPETargetHeight = (CCustomItemSpinDouble*)m_pCatPostEffectEvent->AddChildItem(new CCustomItemSpinDouble("Target Height"));
        m_pPEPCBoneName = m_pCatPostEffectEvent->AddChildItem(new CXTPPropertyGridItem("Attach Bone"));

        m_pPEEffectName->SetFlags(xtpGridItemHasComboButton);
        m_pPEPCBoneName->SetFlags(xtpGridItemHasComboButton);
        m_pPETarget->GetConstraints()->AddConstraint("SELF", POST_EFFECT_TARGET_TYPE_SELF);
        m_pPETarget->GetConstraints()->AddConstraint("TARGET", POST_EFFECT_TARGET_TYPE_TARGET);

        m_pPETime->SetID(ID_PE_TIME);
        m_pPEEffectName->SetID(ID_PE_NAME);
        m_pPETarget->SetID(ID_PE_TARGET);
        m_pPEOffset->SetID(ID_PE_OFFSET);
        m_pPECenterFix->SetID(ID_PE_CENTER_FIX);
        m_pPETargetHeight->SetID(ID_PE_TARGET_HEIGHT);
        m_pPEPCBoneName->SetID(ID_PE_PC_BONE);

        m_pCatPostEffectEvent->Expand();

		// Summon Pet Event Properties
		m_pCatSummonPetEvent = m_property.AddCategory("Summon Pet Event Properties");
		m_pSUETime = (CCustomItemSpinDouble*)m_pCatSummonPetEvent->AddChildItem(new CCustomItemSpinDouble("Time"));

		m_pSUETime->SetID(ID_SUE_TIME);

		m_pCatSummonPetEvent->Expand();

        // TMQ Event Properties
        m_pCatTMQEvent = m_property.AddCategory("TMQ Event Properties");
        m_pTMQTime = (CCustomItemSpinDouble*)m_pCatTMQEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pTMQType = (CXTPPropertyGridItemEnum*)m_pCatTMQEvent->AddChildItem(new CXTPPropertyGridItemEnum("TMQ Type"));

        m_pTMQType->GetConstraints()->AddConstraint("IN", E_ANIM_CINEMATIC_TMQ_IN);
        m_pTMQType->GetConstraints()->AddConstraint("OUT", E_ANIM_CINEMATIC_TMQ_OUT);

        m_pTMQTime->SetID(ID_TMQ_TIME);
        m_pTMQType->SetID(ID_TMQ_TYPE);
        m_pCatTMQEvent->Expand();

        // Alpha Event Properties
        m_pCatAlphaEvent = m_property.AddCategory("Alpha Fade Event Properties");
        m_pAlphaTime = (CCustomItemSpinDouble*)m_pCatAlphaEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pAlphaStart = (CXTPPropertyGridItemNumber*)m_pCatAlphaEvent->AddChildItem(new CXTPPropertyGridItemNumber("Start Alpha"));
        m_pAlphaDest = (CXTPPropertyGridItemNumber*)m_pCatAlphaEvent->AddChildItem(new CXTPPropertyGridItemNumber("Dest Alpha"));        
        m_pAlphaLifeTime = (CCustomItemSpinDouble*)m_pCatAlphaEvent->AddChildItem(new CCustomItemSpinDouble("Life Time"));
        m_pAlphaFadeTime = (CCustomItemSpinDouble*)m_pCatAlphaEvent->AddChildItem(new CCustomItemSpinDouble("Fade Time"));        
        m_pAlphaType = (CXTPPropertyGridItemEnum*)m_pCatAlphaEvent->AddChildItem(new CXTPPropertyGridItemEnum("Apply Type"));        

        m_pAlphaType->GetConstraints()->AddConstraint("CLUMP", SEventAlpha::E_ALPHA_EVENT_CLUMP);
        m_pAlphaType->GetConstraints()->AddConstraint("ATOMIC", SEventAlpha::E_ALPHA_EVENT_ATOMIC);

        m_pAlphaTime->SetID(ID_ALPHA_TIME);
        m_pAlphaStart->SetID(ID_ALPHA_START);
        m_pAlphaDest->SetID(ID_ALPHA_DEST);
        m_pAlphaLifeTime->SetID(ID_ALPHA_LIFE_TIME);
        m_pAlphaFadeTime->SetID(ID_ALPHA_FADE_TIME);
        m_pAlphaType->SetID(ID_ALPHA_TYPE);

        for(int i = 0; i < MAX_ALPHA_ATOMIC; ++i)       // Alpha Atomic List Control
        {
            m_pAlphaAtomic[i] = (CXTPPropertyGridItemBool*)m_pCatAlphaEvent->AddChildItem(new CXTPPropertyGridItemBool(""));
            m_pAlphaAtomic[i]->SetID(ID_ALPHA_ATOMIC + i);
            m_pAlphaAtomic[i]->SetHidden(TRUE);
            m_pAlphaAtomic[i]->SetCheckBoxStyle();
        }
        
        m_pCatAlphaEvent->Expand();

        // Explosion Event Properties
        m_pCatExplosionEvent = m_property.AddCategory("Explosion Event Properties");
        m_pExplosionTime = (CCustomItemSpinDouble*)m_pCatExplosionEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pExplosionType = (CXTPPropertyGridItemEnum*)m_pCatExplosionEvent->AddChildItem(new CXTPPropertyGridItemEnum("Explosion Type"));

        m_pExplosionType->GetConstraints()->AddConstraint("SMALL", EXPLOSION_EVENT_TYPE_SMALL);
        m_pExplosionType->GetConstraints()->AddConstraint("NORMAL", EXPLOSION_EVENT_TYPE_NORMAL);
        m_pExplosionType->GetConstraints()->AddConstraint("BIG", EXPLOSION_EVENT_TYPE_BIG);

        m_pExplosionTime->SetID(ID_EXPLOSION_TIME);
        m_pExplosionType->SetID(ID_EXPLOSION_TYPE);

        m_pCatExplosionEvent->Expand();
        
        // Create Object Properties Category
        m_pCatObjectProperty = m_property.AddCategory("Object Properties");
        m_pObjName      = m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItem("Name"));
        m_pObjClumpName = m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItem("Clump Name"));
        m_pObjShadow    = (CXTPPropertyGridItemBool*)m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItemBool("Shadow", TRUE));                
        m_pObjCullDist  = (CCustomItemSpinDouble*)m_pCatObjectProperty->AddChildItem(new CCustomItemSpinDouble("Cull Distance", _T("%.2f"), 100.0f, 500.0f));        
        m_pObjBBox      = (CCustomItemOffset*)m_pCatObjectProperty->AddChildItem(new CCustomItemOffset("Trigger BBox"));        
        m_pObjUVAnim    = (CCustomItemFileBox*)m_pCatObjectProperty->AddChildItem(new CCustomItemFileBox("UVAnimFile", FILTER_UVANIM));
        m_pObjUVAnimSpeed = (CCustomItemSpinDouble*)m_pCatObjectProperty->AddChildItem(new CCustomItemSpinDouble("UVAnim Speed"));
        m_pObjAnim      = (CCustomItemFileBox*)m_pCatObjectProperty->AddChildItem(new CCustomItemFileBox("Anim FIle", FILTER_ANIM));        
        m_pObjCollision = (CXTPPropertyGridItemBool*)m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItemBool("Path Engine Collision", FALSE));
        m_pObjCollisonMesh = (CCustomItemFileBox*)m_pCatObjectProperty->AddChildItem(new CCustomItemFileBox("Collison Mesh File", FILTER_CLUMP));
        m_pObjType      = (CXTPPropertyGridItemEnum*)m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItemEnum("Object Type"));
        m_pObjApplyPVS  = (CXTPPropertyGridItemBool*)m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItemBool("Apply PVS", TRUE));
        m_pObjSortByPos = (CXTPPropertyGridItemBool*)m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItemBool("Sort By Pos", FALSE));
        m_pObjForcePicking = (CXTPPropertyGridItemBool*)m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItemBool("Force Picking", FALSE));
        m_pObjPENaviPress = (CXTPPropertyGridItemBool*)m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItemBool("PE Navi Press", FALSE));
        m_pObjCullTestAllAtomic = (CXTPPropertyGridItemBool*)m_pCatObjectProperty->AddChildItem(new CXTPPropertyGridItemBool("Culling Test All Atomic"));
        m_pCatObjSoundProperty = m_property.AddCategory("Object Sound Properties");
        m_pObjSoundName = (CCustomItemFileBox*)m_pCatObjSoundProperty->AddChildItem(new CCustomItemFileBox("Sound File", FILTER_SOUND));
        m_pObjSoundVolume = (CCustomItemSpin*)m_pCatObjSoundProperty->AddChildItem(new CCustomItemSpin("Sound Volume"));
        m_pObjSoundDist = (CCustomItemSpin*)m_pCatObjSoundProperty->AddChildItem(new CCustomItemSpin("Sound Distance"));
        m_pObjSoundDecayDist = (CCustomItemSpin*)m_pCatObjSoundProperty->AddChildItem(new CCustomItemSpin("Sound Decay Distance"));
        m_pObjSoundPicth = (CCustomItemSpinDouble*)m_pCatObjSoundProperty->AddChildItem(new CCustomItemSpinDouble("Sound Pitch"));
        
        m_pObjName->SetReadOnly(TRUE);
        m_pObjClumpName->SetReadOnly(TRUE);        

        m_pObjShadow->SetID(ID_OBJ_SHADOW);  
        m_pObjCollision->SetID(ID_OBJ_COLLISION);
        m_pObjBBox->SetID(ID_OBJ_BBOX);
        m_pObjUVAnim->SetID(ID_OBJ_UVANIM);        
        m_pObjCullDist->SetID(ID_OBJ_CULLDIST);
        m_pObjUVAnimSpeed->SetID(ID_OBJ_UVANIM_SPEED);
        m_pObjAnim->SetID(ID_OBJ_ANIM);        
        m_pObjCollisonMesh->SetID(ID_OBJ_COLLISON_MESH);
        m_pObjType->SetID(ID_OBJ_TYPE);
        m_pObjApplyPVS->SetID(ID_OBJ_APPLY_PVS);
        m_pObjSortByPos->SetID(ID_OBJ_SORT_BY_POS);
        m_pObjForcePicking->SetID(ID_OBJ_FORCE_PICKING);
        m_pObjPENaviPress->SetID(ID_OBJ_PE_NAVI_PRESS);
        m_pObjCullTestAllAtomic->SetID(ID_OBJ_CULLTEST_ALL_ATOMIC);
        m_pObjSoundName->SetID(ID_SE_SOUND_NAME1);                  // 사운드 이벤트와 아이디를 공용으로 사용해 같은 이벤트 처리함수를 사용한다.
        m_pObjSoundVolume->SetID(ID_SE_SOUND_VOLUME);
        m_pObjSoundDist->SetID(ID_SE_SOUND_DIST);
        m_pObjSoundDecayDist->SetID(ID_SE_SOUND_DECAY_DIST);
        m_pObjSoundPicth->SetID(ID_SE_SOUND_PITCH_MIN);
        
        m_pObjCullDist->SetFlags(xtpGridItemHasComboButton);	
		m_pObjCullDist->GetConstraints()->AddConstraint("70");
        m_pObjCullDist->GetConstraints()->AddConstraint("100");
        m_pObjCullDist->GetConstraints()->AddConstraint("200");
        m_pObjCullDist->GetConstraints()->AddConstraint("300");
        m_pObjCullDist->GetConstraints()->AddConstraint("400");
        m_pObjCullDist->GetConstraints()->AddConstraint("500");

        m_pObjType->GetConstraints()->AddConstraint("Normal", E_OBJECT_NORMAL);
        m_pObjType->GetConstraints()->AddConstraint("MilePost", E_OBJECT_MILEPOST);

        m_pCatObjectProperty->Expand();
        m_pCatObjSoundProperty->Expand();

        // Create Item Properties Category
        m_pCatItemProperty  = m_property.AddCategory("Item Properties");
        m_pItemName         = m_pCatItemProperty->AddChildItem(new CXTPPropertyGridItem("Name"));
        m_pItemMeshPath     = m_pCatItemProperty->AddChildItem(new CXTPPropertyGridItem("Mesh Path"));
        m_pItemMeshName     = m_pCatItemProperty->AddChildItem(new CXTPPropertyGridItem("Mesh File"));
        m_pItemAttachType   = (CXTPPropertyGridItemEnum*)m_pCatItemProperty->AddChildItem(new CXTPPropertyGridItemEnum("Attach Type"));
        m_pItemAttachSlot   = (CXTPPropertyGridItemEnum*)m_pCatItemProperty->AddChildItem(new CXTPPropertyGridItemEnum("Attach Slot"));
        m_pItemAttachOffset = (CCustomItemOffset*)m_pCatItemProperty->AddChildItem(new CCustomItemOffset("Attach Offset"));
        m_pItemTraceEnable  = (CXTPPropertyGridItemBool*)m_pCatItemProperty->AddChildItem(new CXTPPropertyGridItemBool("Trace Effect Enable Default"));
        m_pItemApplyEmblem  = (CXTPPropertyGridItemBool*)m_pCatItemProperty->AddChildItem(new CXTPPropertyGridItemBool("Apply Emblem"));

        m_pItemName->SetID(ID_ITEM_NAME);
        m_pItemMeshPath->SetID(ID_ITEM_MESH_PATH);
        m_pItemMeshName->SetID(ID_ITEM_MESH_NAME);
        m_pItemAttachType->SetID(ID_ITEM_ATTACH_TYPE);
        m_pItemAttachSlot->SetID(ID_ITEM_ATTACH_SLOT);
        m_pItemAttachOffset->SetID(ID_ITEM_ATTACH_OFFSET);
        m_pItemTraceEnable->SetID(ID_ITEM_TRACE_ENABLE);
        m_pItemApplyEmblem->SetID(ID_ITEM_APPLY_EMBLEM);

        m_pItemName->SetReadOnly(TRUE);
        m_pItemMeshPath->SetReadOnly(TRUE);
        m_pItemMeshName->SetReadOnly(TRUE);        

        m_pItemAttachType->GetConstraints()->AddConstraint("EQUIP_BODY", ITEM_RES_EQUIP_BODY);        
		m_pItemAttachType->GetConstraints()->AddConstraint("EQUIP_MAIN", ITEM_RES_EQUIP_MAIN);
		m_pItemAttachType->GetConstraints()->AddConstraint("EQUIP_SUB", ITEM_RES_EQUIP_SUB);
        m_pItemAttachType->GetConstraints()->AddConstraint("NOT_EQUIP", ITEM_RES_NOT_EQUIP);

        m_pItemAttachSlot->GetConstraints()->AddConstraint("HEAD", ITEM_EQUIP_HEAD);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("FACE", ITEM_EQUIP_FACE);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("JACKET", ITEM_EQUIP_JACKET);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("PANTS", ITEM_EQUIP_PANTS);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("BOOTS", ITEM_EQUIP_BOOTS);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("TWO_HAND", ITEM_EQUIP_TWO_HAND);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("R_EYE", ITEM_EQUIP_R_EYE);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("R_HAND", ITEM_EQUIP_R_HAND);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("L_HAND", ITEM_EQUIP_L_HAND);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("BACK", ITEM_EQUIP_BACK);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("USER_MOVE1", ITEM_EQUIP_USER_MOVE1);
        m_pItemAttachSlot->GetConstraints()->AddConstraint("MASK", ITEM_EQUIP_MASK);

        m_pCatItemProperty->Expand();

        // Item Upgrade Properties
        m_pCatUpgradeProperty = m_property.AddCategory("Upgrade Properties");
        m_pUpgradeBone1         = m_pCatUpgradeProperty->AddChildItem(new CXTPPropertyGridItem("Bone 1"));
        m_pUpgradeOffset1       = (CCustomItemOffset*)m_pCatUpgradeProperty->AddChildItem(new CCustomItemOffset("Bone1 Offset"));
        m_pUpgradeBone2         = m_pCatUpgradeProperty->AddChildItem(new CXTPPropertyGridItem("Bone 2"));
        m_pUpgradeOffset2       = (CCustomItemOffset*)m_pCatUpgradeProperty->AddChildItem(new CCustomItemOffset("Bone2 Offset"));
        m_pUpgradeEffect1       = m_pCatUpgradeProperty->AddChildItem(new CXTPPropertyGridItem("Effect (3Grade)"));
        m_pUpgradeEffect2       = m_pCatUpgradeProperty->AddChildItem(new CXTPPropertyGridItem("Effect (4Grade)"));

        m_pUpgradeBone1->SetFlags(xtpGridItemHasComboButton);
        m_pUpgradeBone2->SetFlags(xtpGridItemHasComboButton);
        m_pUpgradeEffect1->SetFlags(xtpGridItemHasComboButton);
        m_pUpgradeEffect2->SetFlags(xtpGridItemHasComboButton);

        m_pUpgradeBone1->SetID(ID_UPGRADE_BONE1);
        m_pUpgradeBone2->SetID(ID_UPGRADE_BONE2);
        m_pUpgradeEffect1->SetID(ID_UPGRADE_EFFECT1);
        m_pUpgradeEffect2->SetID(ID_UPGRADE_EFFECT2);
        m_pUpgradeOffset1->SetID(ID_UPGRADE_OFFSET1);
        m_pUpgradeOffset2->SetID(ID_UPGRADE_OFFSET2);

        // Character
        m_pCatCharacterProperty = m_property.AddCategory("Character Properties");
        m_pCharFaceCameraPos    = (CCustomItemOffset*)m_pCatCharacterProperty->AddChildItem(new CCustomItemOffset("Face Camera Position"));
        m_pCharFaceCameraLookAt = (CCustomItemOffset*)m_pCatCharacterProperty->AddChildItem(new CCustomItemOffset("Face Camera Height"));
		m_pCharInitWalkSpeed	= (CCustomItemSpinDouble*)m_pCatCharacterProperty->AddChildItem(new CCustomItemSpinDouble("Init Walk Anim Speed"));
		m_pCharInitRunSpeed		= (CCustomItemSpinDouble*)m_pCatCharacterProperty->AddChildItem(new CCustomItemSpinDouble("Init Run Anim Speed"));
        m_pCharApplyTangent     = (CXTPPropertyGridItemBool*)m_pCatCharacterProperty->AddChildItem(new CXTPPropertyGridItemBool("Apply Normal"));
        m_pCharAlphaDistance    = (CCustomItemSpinDouble*)m_pCatCharacterProperty->AddChildItem(new CCustomItemSpinDouble("Alpha Distance"));

        m_pCharFaceCameraPos->SetID(ID_CHAR_FACE_CAMERA_POS);
        m_pCharFaceCameraLookAt->SetID(ID_CHAR_FACE_CAMERA_HEIGHT);
		m_pCharInitWalkSpeed->SetID(ID_CHAR_INIT_WALK_SPEED);
		m_pCharInitRunSpeed->SetID(ID_CHAR_INIT_RUN_SPEED);
        m_pCharApplyTangent->SetID(ID_CHAR_APPLY_TANGENT);
        m_pCharAlphaDistance->SetID(ID_CHAR_ALPHA_DIST);

        m_pCatCharacterProperty->Expand();

        // Direct Event
        m_pCatDirectEvent = m_property.AddCategory("Direct Event Properties");
        m_pDirectEventTime = (CCustomItemSpinDouble*)m_pCatDirectEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pDirectEventType = (CXTPPropertyGridItemEnum*)m_pCatDirectEvent->AddChildItem(new CXTPPropertyGridItemEnum("Direct Type"));
        m_pDirectEventType->GetConstraints()->AddConstraint("Camera Shake", DIRECT_EVENT_TYPE_CAMERA_SHAKE);
        m_pDirectEventTime->SetID(ID_DIRECT_TIME);
        m_pDirectEventType->SetID(ID_DIRECT_TYPE);

        m_pCatDirectEvent->Expand();

        // Color Change Event
        m_pCatColorChangeEvent = m_property.AddCategory("Color Change Event Properties");
        m_pColChangeEventTime = (CCustomItemSpinDouble*)m_pCatColorChangeEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pColChangeEventType = (CXTPPropertyGridItemEnum*)m_pCatColorChangeEvent->AddChildItem(new CXTPPropertyGridItemEnum("Change Type"));
        m_pColChangeEventEdge = (CXTPPropertyGridItemColor*)m_pCatColorChangeEvent->AddChildItem(new CXTPPropertyGridItemColor("Edge Color"));
        m_pColChangeEventBody = (CXTPPropertyGridItemColor*)m_pCatColorChangeEvent->AddChildItem(new CXTPPropertyGridItemColor("Body Color"));
        m_pColChangeEventAdd  = (CXTPPropertyGridItemColor*)m_pCatColorChangeEvent->AddChildItem(new CXTPPropertyGridItemColor("Add Color"));
        m_pColChangeEventType->GetConstraints()->AddConstraint("Start Point", COLOR_CHANGE_TYPE_START);
        m_pColChangeEventType->GetConstraints()->AddConstraint("End Point", COLOR_CHANGE_TYPE_END);
        m_pColChangeEventType->GetConstraints()->AddConstraint("Animation", COLOR_CHANGE_TYPE_ANIM);
        
        m_pColChangeEventTime->SetID(ID_COLCHANGE_TIME);
        m_pColChangeEventType->SetID(ID_COLCHANGE_TYPE);
        m_pColChangeEventEdge->SetID(ID_COLCHANGE_EDGE);
        m_pColChangeEventBody->SetID(ID_COLCHANGE_BODY);
        m_pColChangeEventAdd->SetID(ID_COLCHANGE_ADD);
        m_pCatColorChangeEvent->Expand();

        // Stretch Event
        m_pCatStretchEvent = m_property.AddCategory("Bone Stretch Event Properties");
        m_pStretchEventTime = (CCustomItemSpinDouble*)m_pCatStretchEvent->AddChildItem(new CCustomItemSpinDouble("Time"));
        m_pStretchEventType = (CXTPPropertyGridItemEnum*)m_pCatStretchEvent->AddChildItem(new CXTPPropertyGridItemEnum("Type"));
        m_pStretchBoneName[0]   = m_pCatStretchEvent->AddChildItem(new CXTPPropertyGridItem("Bone 1"));
        m_pStretchBoneName[1]   = m_pCatStretchEvent->AddChildItem(new CXTPPropertyGridItem("Bone 2"));
        m_pStretchSpeed         = (CCustomItemSpinDouble*)m_pCatStretchEvent->AddChildItem(new CCustomItemSpinDouble("Bone Speed"));
        m_pStretchAccel         = (CCustomItemSpinDouble*)m_pCatStretchEvent->AddChildItem(new CCustomItemSpinDouble("Bone Accel Speed"));
        m_pStretchWidth         = (CCustomItemSpinDouble*)m_pCatStretchEvent->AddChildItem(new CCustomItemSpinDouble("Bone Width"));        
        m_pStretchScaleName     = m_pCatStretchEvent->AddChildItem(new CXTPPropertyGridItem("Scale Bone"));
        m_pStretchScaleSize     = (CCustomItemSpinDouble*)m_pCatStretchEvent->AddChildItem(new CCustomItemSpinDouble("Scale Size"));
        m_pStretchAxisBoneName  = m_pCatStretchEvent->AddChildItem(new CXTPPropertyGridItem("Axis Bone"));
        m_pStretchTargetEffect  = m_pCatStretchEvent->AddChildItem(new CXTPPropertyGridItem("Target Effect"));        

        m_pStretchEventType->GetConstraints()->AddConstraint("PULLING", E_STRETCH_PULLING);
        m_pStretchEventType->GetConstraints()->AddConstraint("HIT", E_STRETCH_HIT);

        m_pStretchBoneName[0]->SetFlags(xtpGridItemHasComboButton);
        m_pStretchBoneName[1]->SetFlags(xtpGridItemHasComboButton);
        m_pStretchScaleName->SetFlags(xtpGridItemHasComboButton);
        m_pStretchAxisBoneName->SetFlags(xtpGridItemHasComboButton);
        m_pStretchTargetEffect->SetFlags(xtpGridItemHasComboButton);        

        m_pStretchEventTime->SetID(ID_STRETCH_TIME);
        m_pStretchEventType->SetID(ID_STRETCH_TYPE);
        m_pStretchBoneName[0]->SetID(ID_STRETCH_BONE1_NAME);
        m_pStretchBoneName[1]->SetID(ID_STRETCH_BONE2_NAME);
        m_pStretchSpeed->SetID(ID_STRETCH_BONE_SPEED);
        m_pStretchWidth->SetID(ID_STRETCH_BONE_WIDTH);                
        m_pStretchAccel->SetID(ID_STRETCH_BONE_ACCEL);
        m_pStretchScaleName->SetID(ID_STRETCH_SCALE_NAME);
        m_pStretchScaleSize->SetID(ID_STRETCH_SCALE_SIZE);
        m_pStretchAxisBoneName->SetID(ID_STRETCH_AXIS_BONE);
        m_pStretchTargetEffect->SetID(ID_STRETCH_TARGET_EFFECT);
        m_pCatStretchEvent->Expand();

        // Trigger Event
        m_pCatTriggerEvent = m_property.AddCategory("Trigger Event Properties");
        m_pTriggerEventTime = (CCustomItemSpinDouble*)m_pCatTriggerEvent->AddChildItem(new CCustomItemSpinDouble("Time"));

        m_pTriggerEventTime->SetID(ID_TRIGGER_TIME);
        m_pCatTriggerEvent->Expand();

        // Skill Cancel Event
        m_pCatSkillCancelEvent = m_property.AddCategory("Skill Cancel Properties");
        m_pSCTime = (CCustomItemSpinDouble*)m_pCatSkillCancelEvent->AddChildItem(new CCustomItemSpinDouble("Time"));

        m_pSCTime->SetID(ID_SC_TIME);
        m_pCatSkillCancelEvent->Expand();

        // Animation Property
        m_pCatAnimation = m_property.AddCategory("Animation Properties");
        m_pAnimCullTestAllAtomic = (CXTPPropertyGridItemBool*)m_pCatAnimation->AddChildItem(new CXTPPropertyGridItemBool("Cull Test All Atomic"));
        m_pAnimCullTestAllAtomic->SetID(ID_ANIM_CULL_CULLTEST);
        m_pCatAnimation->Expand();
        

        m_property.SetTheme(xtpGridThemeWhidbey);

    }

    SetResize(IDC_PROPERTY_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);   

    HideAllCategory();
    SetEvent(NULL, NULL);
}

void CPropertyPane::SetEvent(SEventAnim* pEventAnim, RpClump* pClump /* = NULL */, RwBool bRefresh /* = FALSE */)
{
    if(m_pEventAnim == pEventAnim)
    {
        if(bRefresh)
            Update();

        return;
    }
        

    m_ePropertyMode = PROPERTY_MODE_ANIM_EVENT;
    HideAllCategory();

    if(!pEventAnim)
    {
        m_pClump = NULL;
        m_pEventAnim = NULL;
        return;
    }

    switch(pEventAnim->eEventID)
    {    
    case EVENT_ANIM_HIT:
        m_pCatHitEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_VISUAL_EFFECT:
        m_pCatVisualEffectEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_VISUAL_SOUND:
        m_pCatVisualSoundEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_FOOT_STEP:
        m_pCatFootStepEvent->SetHidden(FALSE);
        break;    
    case EVENT_ANIM_LINK_EFFECT:
        m_pCatLinkEffectEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_WEIGHT_TIME:
        m_pCatSlowTimeEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_TRACE_EFFECT:
        m_pCatTraceEvent->SetHidden(FALSE);
        m_pCatTraceEvent->Expand();
        break;
    case EVENT_ANIM_SUB_WEAPON:
        m_pCatSubWeaponEvent->SetHidden(FALSE);        
        break;
    case EVENT_ANIM_POST_EFFECT:
        m_pCatPostEffectEvent->SetHidden(FALSE);
        break;
	case EVENT_ANIM_SUMMON_PET:
		m_pCatSummonPetEvent->SetHidden(FALSE);
		break;
    case EVENT_ANIM_TMQ:
        m_pCatTMQEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_ALPHA:
        m_pCatAlphaEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_EXPLOSION:
        m_pCatExplosionEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_DIRECT:
        m_pCatDirectEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_COLOR_CHANGE:
        m_pCatColorChangeEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_STRETCH:
        m_pCatStretchEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_TRIGGER:
        m_pCatTriggerEvent->SetHidden(FALSE);
        break;
    case EVENT_ANIM_SKILL_CANCEL:
        m_pCatSkillCancelEvent->SetHidden(FALSE);
        break;
	default:
		NTL_ASSERTE(!"Not Event ID");
		break;
    }

    m_pEventAnim = pEventAnim;

    if(pClump && m_pClump != pClump)
    {
        m_pClump = pClump;        
        SetBoneList();

        // Effect List가 없으면 읽어온다.
        if(m_pItemTargetEffectName->GetConstraints()->GetCount() <= 0)
        {
            SetEffectNameList();
        }
    }
    else
    {
        m_pClump = pClump;
    }

    Update();
}

void CPropertyPane::SetObject(CMTObject* pObject)
{
    USES_CONVERSION;

    m_ePropertyMode = PROPERTY_MODE_OBJECT;
    HideAllCategory();

    m_pItem = NULL;
    m_pEventAnim = NULL;
    m_pCharacter = NULL;

    if(!pObject)
    {
        m_pObject = NULL;
    }
    else
    {
        m_pCatObjectProperty->SetHidden(FALSE);
        m_pCatObjSoundProperty->SetHidden(FALSE);

        m_pObject = pObject;

        m_pObjName->SetValue((pObject->GetProperty()->GetName()));
        m_pObjClumpName->SetValue((pObject->GetProperty()->GetDffName()));
        m_pObjShadow->SetBool(pObject->GetProperty()->GetEnableShadow());         
        m_pObjCullDist->SetDouble(pObject->GetProperty()->GetCullingDistance());        
        m_pObjUVAnimSpeed->SetDouble(pObject->GetProperty()->GetUVAnimSpeed());
        m_pObjAnim->SetValue((pObject->GetProperty()->m_strAnimFileName.c_str()));
        m_pObjCollision->SetBool(pObject->GetProperty()->GetCollision());        
        m_pObjCollisonMesh->SetValue((pObject->GetProperty()->m_strCollisionMeshName.c_str()));
        m_pObjType->SetEnum(pObject->GetProperty()->GetObjectType());
        m_pObjApplyPVS->SetBool(pObject->GetProperty()->GetApplyPVS());
        m_pObjSortByPos->SetBool(pObject->GetProperty()->IsSortByPos());
        m_pObjPENaviPress->SetBool(pObject->GetProperty()->IsNaviPress());
        m_pObjCullTestAllAtomic->SetBool(pObject->GetProperty()->IsCullTestAllAtomic());
        m_pObjForcePicking->SetBool(pObject->GetProperty()->IsForcePicking());

        // BBox
        RwBBox* pBBox = m_pObject->GetProperty()->GetBBox();
        float fWidth = pBBox->sup.x - pBBox->inf.x;
        float fHeight = pBBox->sup.y;
        float fDepth = pBBox->sup.z - pBBox->inf.z;
        m_pObjBBox->SetOffset(fWidth, fHeight, fDepth);
        m_pObjUVAnim->SetValue((m_pObject->GetProperty()->m_strUVAnimFileName.c_str()));

        if(pObject->GetProperty()->GetCollision())
        {
            m_pObjCollisonMesh->SetHidden(FALSE);
        }
        else
        {
            m_pObjCollisonMesh->SetHidden(TRUE);
        }        

        // Sound Property
        SEventSound* pSoundProp = m_pObject->GetProperty()->GetSoundProp();
        m_pObjSoundName->SetValue((pSoundProp->chSoundName));
        m_pObjSoundVolume->SetNumber((int)pSoundProp->fSoundVolume);
        m_pObjSoundDist->SetNumber((int)pSoundProp->fSoundDist);
        m_pObjSoundDecayDist->SetNumber((int)pSoundProp->fSoundDecayDist);
        m_pObjSoundPicth->SetDouble(pSoundProp->fSoundPitchMin);
    }

    SetBoneList();    
}

void CPropertyPane::SetItem(CMTItem* pItem)
{
    USES_CONVERSION;

    m_ePropertyMode = PROPERTY_MODE_ITEM;
    HideAllCategory();

    m_pEventAnim = NULL;
    m_pObject = NULL;
    m_pCharacter = NULL;

    if(!pItem)
    {
        m_pItem = NULL;
    }
    else
    {
        m_pItem = pItem;

        m_pCatItemProperty->SetHidden(FALSE);   
        m_pItemName->SetValue((pItem->GetProperty()->GetName()));
        m_pItemMeshPath->SetValue((pItem->GetProperty()->GetMeshPath().c_str()));
        m_pItemMeshName->SetValue((pItem->GetProperty()->GetMeshFileName().c_str()));
        m_pItemAttachType->SetEnum(pItem->GetProperty()->GetResType());
        m_pItemAttachSlot->SetEnum(pItem->GetProperty()->GetEquipSlotType());
        m_pItemAttachOffset->SetOffset(pItem->GetProperty()->GetAttachOffset().x,
                                       pItem->GetProperty()->GetAttachOffset().y,
                                       pItem->GetProperty()->GetAttachOffset().z);
        m_pItemTraceEnable->SetBool(pItem->GetProperty()->GetTraceEnableDefault());

        // Flag
        if(pItem->GetProperty()->GetFlag() & ITEM_FLAG_APPLY_EMBLEM)
        {
            m_pItemApplyEmblem->SetBool(TRUE);
        }
        else
        {
            m_pItemApplyEmblem->SetBool(FALSE);
        }

        // Trace Effect 관련 설정 사항
        ApplyTraceEventProperties(&(pItem->GetProperty()->m_eventTrace));

        // Upgrade Effect 관련 설정
        ApplyUpgradeEffectProperties(pItem->GetProperty()->GetUpgradeEffectProperty());

        if(m_pClump != m_pItem->GetClumpInfo()->GetClump())
        {
            // Item의 Bone List를 Udpate한다.
            m_pClump = m_pItem->GetClumpInfo()->GetClump();            
            SetBoneList();
        }

        // Effect List가 없으면 읽어온다.
        if(m_pItemTargetEffectName->GetConstraints()->GetCount() <= 0)
        {
            SetEffectNameList();
        }
    }
}

void CPropertyPane::SetCharacter(CMTCharacter* pCharacter)
{
    USES_CONVERSION;

    m_ePropertyMode = PROPERTY_MODE_CHARACTER;
    HideAllCategory();

    m_pEventAnim = NULL;
    m_pObject = NULL;
    m_pItem = NULL;

    m_pCharacter = pCharacter;

    if(m_pCharacter)
    {
        RwV3d vFaceCameraPos = pCharacter->GetProperty()->GetFaceCameraPos();
        RwV3d vFaceCameraLookAt = pCharacter->GetProperty()->GetFaceCameraLookAt();
        m_pCatCharacterProperty->SetHidden(FALSE);
        m_pCharFaceCameraPos->SetOffset(vFaceCameraPos.x,
                                        vFaceCameraPos.y,
                                        vFaceCameraPos.z);
        m_pCharFaceCameraLookAt->SetOffset(vFaceCameraLookAt.x,
                                           vFaceCameraLookAt.y,
                                           vFaceCameraLookAt.z);
		m_pCharInitWalkSpeed->SetDouble(pCharacter->GetProperty()->GetInitWalkFrontAnimSpeed());
		m_pCharInitRunSpeed->SetDouble(pCharacter->GetProperty()->GetInitRunFrontAnimSpeed());
        m_pCharAlphaDistance->SetDouble(pCharacter->GetProperty()->GetAlphaDistance());
        
        // Flag
        m_pCharApplyTangent->SetBool(m_pCharacter->GetProperty()->GetFlag() & CHAR_FLAG_APPLY_NORMAL);
    }
}

void CPropertyPane::SetAnimation( STypeAnimData* pTypeAnimData ) 
{
    if(!pTypeAnimData)
        return;

    m_pEventAnim = NULL;

    m_ePropertyMode = PROPERTY_MODE_ANIMATION;
    HideAllCategory();
    m_pCatAnimation->SetHidden(FALSE);

    m_pAnimData = pTypeAnimData;
    m_pAnimCullTestAllAtomic->SetBool(pTypeAnimData->IsCullTestAllAtomic());
}

void CPropertyPane::SetAttackType(EAttackType eAttackType)
{
    switch(eAttackType)
    {
    case ATTACK_TYPE_PHYSICAL:        
        m_pItemProjectileName->SetHidden(TRUE);
        m_pItemProjectileType->SetHidden(TRUE);
        m_pItemProjectileShot->SetHidden(TRUE);        
        m_pItemProjSpeed->SetHidden(TRUE);        
        m_pItemBoneName->SetHidden(TRUE);
        m_pItemSubWeaponBoneFlag->SetHidden(TRUE);
        m_pItemHandType->SetHidden(TRUE);        

        if(m_pCharacter)
            m_pCharacter->SetRenderHissidian(NULL);

        break;
    case ATTACK_TYPE_ENERGY:
        m_pItemProjectileName->SetHidden(FALSE);
        m_pItemProjectileType->SetHidden(FALSE);
        m_pItemProjectileShot->SetHidden(FALSE);        
        m_pItemProjSpeed->SetHidden(FALSE);        
        m_pItemBoneName->SetHidden(FALSE);
        m_pItemSubWeaponBoneFlag->SetHidden(FALSE);
        m_pItemHandType->SetHidden(FALSE);

        m_pItemTargetHeight->SetHidden(FALSE);
        m_pItemTargetAttach->SetHidden(FALSE);
        m_pItemTargetEffectName->SetHidden(FALSE);
        m_pItemTargetSoundName->SetHidden(FALSE);
        m_pItemSoundType->SetHidden(FALSE);

        switch(m_pItemProjectileShot->GetEnum())
        {
        case BONE_CHARACTER:
            m_pItemSubWeaponBoneFlag->SetHidden(TRUE);
            break;
        case BONE_WEAPON:
            m_pItemBoneName->SetHidden(TRUE);
            m_pItemSubWeaponBoneFlag->SetHidden(TRUE);
            break;
        case BONE_SUB_WEAPON:
            m_pItemBoneName->SetHidden(TRUE);
            break;
        }  
        break;
    }

    SetProjectileType();
}

void CPropertyPane::Update()
{
    USES_CONVERSION;
 
    if(!m_pEventAnim)
        return;

    switch(m_pEventAnim->eEventID)
    {
    case EVENT_ANIM_HIT:
        ApplyHitEventProperties((SEventAnimHit*)m_pEventAnim);
        break;
    case EVENT_ANIM_VISUAL_EFFECT:
        ApplyVisualEffectEventProperties((SEventVisualEffect*)m_pEventAnim);
        break;
    case EVENT_ANIM_VISUAL_SOUND:
        ApplySoundEventProperties((SEventSound*)m_pEventAnim);
        break;
    case EVENT_ANIM_FOOT_STEP:        
        ApplyFootStepEventProperties((SEventFootStep*)m_pEventAnim);
        break;
    case EVENT_ANIM_LINK_EFFECT:
        {
            SEventLinkEffect* pEventLink = (SEventLinkEffect*)m_pEventAnim;           
            if(m_pClump)
            {
                m_pLEAttachBone->SetBool(pEventLink->bAttachBone);                  
                m_pLEBoneName->SetValue((pEventLink->chBoneName));                
                m_pLEAttachBone->SetHidden(FALSE);

                if(pEventLink->bAttachBone)
                {
                    m_pLEBoneName->SetHidden(FALSE);
                }
                else
                {
                    m_pLEBoneName->SetHidden(TRUE);
                }
            }
            else
            {
                m_pLEBoneName->SetHidden(TRUE);
                m_pLEAttachBone->SetHidden(TRUE);
            }
            
            m_pLEName->SetValue((pEventLink->chEffectName));                
            m_pLEOffsetPos->SetOffset(pEventLink->vOffsetPos.x, pEventLink->vOffsetPos.y, pEventLink->vOffsetPos.z);            
            
        }
        break; 
    case EVENT_ANIM_WEIGHT_TIME:
        {
            SEventWeightTime* pEventSlowTime = (SEventWeightTime*)m_pEventAnim;
            m_pSTTime->SetDouble(pEventSlowTime->fTime);
            m_pSTLifeTime->SetDouble(pEventSlowTime->fLifeTime);
            m_pSTWeight->SetDouble(pEventSlowTime->fWeightValue);
        }
        break;
    case EVENT_ANIM_TRACE_EFFECT:
        ApplyTraceEventProperties((SEventTrace*)m_pEventAnim);
        break;
    case EVENT_ANIM_SUB_WEAPON:
		ApplySubWeaponEventProperties((SEventSubWeapon*)m_pEventAnim);
        break;
	case EVENT_ANIM_POST_EFFECT:
		ApplyPostEffectEventProperties((SEventPostEffect*)m_pEventAnim);
		break;
	case EVENT_ANIM_SUMMON_PET:		
        ApplySummonPetEventProperties((SEventSummonPet*)m_pEventAnim);
		break;
    case EVENT_ANIM_TMQ:
        ApplyTMQEventProperties((SEventAnimCinematic*)m_pEventAnim);
        break;
    case EVENT_ANIM_ALPHA:
        ApplyAlphaEventProperties((SEventAlpha*)m_pEventAnim);
        break;        
    case EVENT_ANIM_EXPLOSION:
        ApplyExplosionEventProperties((SEventExplosion*)m_pEventAnim);
        break;
    case EVENT_ANIM_DIRECT:
        ApplyDirectEventProperties((SEventDirect*)m_pEventAnim);
        break;
    case EVENT_ANIM_COLOR_CHANGE:
        ApplyColorChangeEventProperties((SEventColorChange*)m_pEventAnim);
        break;
    case EVENT_ANIM_STRETCH:
        ApplyStretchEventProperties((SEventStretch*)m_pEventAnim);
        break;
    case EVENT_ANIM_TRIGGER:
        ApplyTriggerEventPrperties((SEventTrigger*)m_pEventAnim);
        break;
    case EVENT_ANIM_SKILL_CANCEL:
        ApplySkillCancelProperties((SEventSkillCancel*)m_pEventAnim);
        break;
	default:
		NTL_ASSERTE(!"Not Event ID");
		break;
    }
}

void CPropertyPane::SetBoneList()
{
    USES_CONVERSION;

    if(!m_pClump)
        return;

    if(m_pCharacter)
    {
        m_pItemBoneName->GetConstraints()->RemoveAll();
        m_pVEBoneName->GetConstraints()->RemoveAll();
        m_pLEBoneName->GetConstraints()->RemoveAll();    
        m_pTEStartBoneName->GetConstraints()->RemoveAll();
        m_pTEEndBoneName->GetConstraints()->RemoveAll();
        m_pPEPCBoneName->GetConstraints()->RemoveAll();
        m_pStretchBoneName[0]->GetConstraints()->RemoveAll();
        m_pStretchBoneName[1]->GetConstraints()->RemoveAll();
        m_pStretchScaleName->GetConstraints()->RemoveAll();
        m_pStretchAxisBoneName->GetConstraints()->RemoveAll();
    }
    else if(m_pItem)
    {
        m_pLEBoneName->GetConstraints()->RemoveAll();    
        m_pTEStartBoneName->GetConstraints()->RemoveAll();
        m_pTEEndBoneName->GetConstraints()->RemoveAll();        
        m_pUpgradeBone1->GetConstraints()->RemoveAll();
        m_pUpgradeBone2->GetConstraints()->RemoveAll();                
    }
	else if(m_pObject)
	{
		m_pVEBoneName->GetConstraints()->RemoveAll();
        m_pLEBoneName->GetConstraints()->RemoveAll();    
	}
    
    FRAME_MAP mapBoneList;
    Helper_GetBoneList(m_pClump, &mapBoneList);

    int nCount = 0;
    FRAME_MAP::iterator it = mapBoneList.begin();
    for(; it != mapBoneList.end(); ++it)
    {
        CString strBoneName = (it->first.c_str());
        if(m_pCharacter)
        {
            m_pItemBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pVEBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pLEBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pTEStartBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pTEEndBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pPEPCBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pStretchBoneName[0]->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pStretchBoneName[1]->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pStretchScaleName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pStretchAxisBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
        }
        else if(m_pItem)
        {
            m_pLEBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pTEStartBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pTEEndBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pUpgradeBone1->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pUpgradeBone2->GetConstraints()->AddConstraint(strBoneName, nCount);            
        }        
		else if(m_pObject)
		{
			m_pVEBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
            m_pLEBoneName->GetConstraints()->AddConstraint(strBoneName, nCount);
		}
    }   
}

void CPropertyPane::SetEffectTypeList()
{
    USES_CONVERSION;

    m_pItemProjectileType->GetConstraints()->RemoveAll();

    // 파서로부터 데이터를 가져와서 설정한다.
    KEY_STRING_MAP* pMap = CNtlPLCharacterParser::GetInstance().GetProjEffectTypeTable()->GetMapKeyToString();
    if(pMap)
    {
        KEY_STRING_MAP_ITER it;
        for(it = pMap->begin(); it != pMap->end(); ++it)
        {
            CString strEffectType = (it->second.c_str());
            m_pItemProjectileType->GetConstraints()->AddConstraint(strEffectType, it->first);
        }
    }

    m_pItemProjectileType->GetConstraints()->SetCurrent(0);
}

void CPropertyPane::SetEffectNameList()
{
    USES_CONVERSION;

    // 처음엔 공백을 하나 추가한다 (Null값을 위해여)
    m_pItemProjectileName->GetConstraints()->AddConstraint("");
    m_pItemTargetEffectName->GetConstraints()->AddConstraint("");
    m_pItemSubTargetEffect->GetConstraints()->AddConstraint("");
    m_pItemWordEffect->GetConstraints()->AddConstraint("");
    m_pVEName->GetConstraints()->AddConstraint("");
    m_pUpgradeEffect1->GetConstraints()->AddConstraint("");
    m_pUpgradeEffect2->GetConstraints()->AddConstraint("");
    m_pStretchTargetEffect->GetConstraints()->AddConstraint("");
    
    CNtlPLPropertyContainer::MapProp mapEffectProperty = CNtlPLPropertyContainer::GetInstance()->Gets(PLENTITY_EFFECT);
    
    CNtlPLPropertyContainer::MapProp::iterator it = mapEffectProperty.begin();
    for(; it != mapEffectProperty.end(); ++it)
    {
        std::string strEffectName = it->first;
        CString sEffectName = (strEffectName.c_str());        

        // 'TEXT'라는 글씨만 포함된 이펙트만 Word Effect에 포함시킨다.
        if(FindNoCase(sEffectName, EFFECT_HIT_WORD_PREFIX))
        {
            m_pItemWordEffect->GetConstraints()->AddConstraint(sEffectName);            
        }

        // 'POST_'라는 글자가 포함된 이펙트만 Post Effect에 포함시킨다.
        else if(FindNoCase(sEffectName, EFFECT_POST_EFFECT_PREFIX))
        {
            m_pPEEffectName->GetConstraints()->AddConstraint(sEffectName);
        }
        else
        {
            m_pItemProjectileName->GetConstraints()->AddConstraint(sEffectName);
            m_pItemTargetEffectName->GetConstraints()->AddConstraint(sEffectName);        
            m_pItemSubTargetEffect->GetConstraints()->AddConstraint(sEffectName);
            m_pVEName->GetConstraints()->AddConstraint(sEffectName);        
            m_pUpgradeEffect1->GetConstraints()->AddConstraint(sEffectName);
            m_pUpgradeEffect2->GetConstraints()->AddConstraint(sEffectName);
            m_pStretchTargetEffect->GetConstraints()->AddConstraint(sEffectName);
        }
    }

    m_pItemProjectileName->GetConstraints()->Sort();
    m_pItemTargetEffectName->GetConstraints()->Sort();
    m_pItemSubTargetEffect->GetConstraints()->Sort();
    m_pVEName->GetConstraints()->Sort();
    m_pUpgradeEffect1->GetConstraints()->Sort();
    m_pUpgradeEffect2->GetConstraints()->Sort();
    m_pItemWordEffect->GetConstraints()->Sort();    
    m_pPEEffectName->GetConstraints()->Sort();    
    m_pStretchTargetEffect->GetConstraints()->Sort();
}

void CPropertyPane::HideAllCategory()
{
    m_pCatHitEvent->SetHidden(TRUE);
    m_pCatVisualEffectEvent->SetHidden(TRUE);
    m_pCatVisualSoundEvent->SetHidden(TRUE);
    m_pCatFootStepEvent->SetHidden(TRUE);   
    m_pCatObjectProperty->SetHidden(TRUE);
    m_pCatObjSoundProperty->SetHidden(TRUE);
    m_pCatItemProperty->SetHidden(TRUE);
    m_pCatSlowTimeEvent->SetHidden(TRUE);
    m_pCatCharacterProperty->SetHidden(TRUE);
    m_pCatLinkEffectEvent->SetHidden(TRUE);
    m_pCatTraceEvent->SetHidden(TRUE);
    m_pCatSubWeaponEvent->SetHidden(TRUE);
    m_pCatUpgradeProperty->SetHidden(TRUE);
    m_pCatPostEffectEvent->SetHidden(TRUE);
	m_pCatSummonPetEvent->SetHidden(TRUE);
    m_pCatTMQEvent->SetHidden(TRUE);
    m_pCatAlphaEvent->SetHidden(TRUE);
    m_pCatExplosionEvent->SetHidden(TRUE);
    m_pCatDirectEvent->SetHidden(TRUE);
    m_pCatColorChangeEvent->SetHidden(TRUE);
    m_pCatStretchEvent->SetHidden(TRUE);
    m_pCatTriggerEvent->SetHidden(TRUE);
    m_pCatSkillCancelEvent->SetHidden(TRUE);
    m_pCatAnimation->SetHidden(TRUE);
}

LRESULT CPropertyPane::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
    USES_CONVERSION;

    if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
    {
        CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

        if(m_ePropertyMode == PROPERTY_MODE_ANIM_EVENT && m_pEventAnim)
        {            
            switch(m_pEventAnim->eEventID)
            {
            case EVENT_ANIM_HIT:            
                SetHitEventProperties((SEventAnimHit*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_VISUAL_EFFECT:
                SetVisualEffectEventProperties((SEventVisualEffect*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_VISUAL_SOUND:
                SetSoundEventProperties((SEventSound*)m_pEventAnim, pItem->GetID());                
                break;
            case EVENT_ANIM_FOOT_STEP:
                SetFootStepEventProperties((SEventFootStep*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_LINK_EFFECT:
                switch(pItem->GetID())
                {
                case ID_LE_BONE_NAME:
                    OnLEBoneName();
                    break;
                case ID_LE_ATTACH_BONE:
                    OnLEAttachBone();
                    break;                                    
                case ID_LE_OFFSET_POS:
                    OnLEOffsetPos();
                    break;
                case ID_LE_OFFSET_ROT:
                    OnLEOffsetRot();
                    break;
                }
                break;
            case EVENT_ANIM_WEIGHT_TIME:
                switch(pItem->GetID())
                {
                case ID_ST_TIME:
                    OnSTTIme();
                    break;
                case ID_ST_LIFE_TIME:
                    OnSTLifeTime();
                    break;
                case ID_ST_WEIGHT:
                    OnSTWeight();
                    break;
                }
                break;
            case EVENT_ANIM_TRACE_EFFECT:
                SetTraceEventProperties((SEventTrace*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_SUB_WEAPON:
                SetSubWeaponProperties((SEventSubWeapon*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_POST_EFFECT:
                SetPostEffectEventProperties((SEventPostEffect*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_SUMMON_PET:                
                SetSummonPetEventProperties((SEventSummonPet*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_TMQ:
                SetTMQEventProperties((SEventAnimCinematic*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_ALPHA:
                SetAlphaEventProperties((SEventAlpha*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_EXPLOSION:
                SetExplosionEventProperties((SEventExplosion*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_DIRECT:
                SetDirectEventProperties((SEventDirect*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_COLOR_CHANGE:
                SetColorChangeEventProperties((SEventColorChange*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_STRETCH:
                SetStretchEventProperties((SEventStretch*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_TRIGGER:
                SetTriggerEventProperties((SEventTrigger*)m_pEventAnim, pItem->GetID());
                break;
            case EVENT_ANIM_SKILL_CANCEL:
                SetSkillCancelProperties((SEventSkillCancel*)m_pEventAnim, pItem->GetID());
                break;
            default:
                NTL_ASSERTE(!"Not Event ID");
                break;
            }
        }
        else if(m_ePropertyMode == PROPERTY_MODE_OBJECT && m_pObject)   // AnimEvent 가 아닐때
        {
            SetObjectProperties(m_pObject, pItem->GetID());
        }
        else if(m_ePropertyMode == PROPERTY_MODE_ITEM && m_pItem)    // Item
        {
            SetItemProperties(m_pItem->GetProperty(), pItem->GetID());
        }
        else if(m_ePropertyMode == PROPERTY_MODE_CHARACTER && m_pCharacter)   // Character
        {
            SetCharacterProperties(m_pCharacter->GetProperty(), pItem->GetID());            
        }
        else if(m_ePropertyMode == PROPERTY_MODE_ANIMATION && m_pAnimData)
        {
            SetAnimationProperties(m_pAnimData, pItem->GetID());
        }

        CModelToolApplication::GetInstance()->SetDataChanged();        
        ::SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir()); // 파일 Open창을 연후에 작업 폴더가 바뀐다. 그래서 원래작업폴더로 설정해준다.
    }
    return 0;
}

void CPropertyPane::ApplyHitEventProperties( SEventAnimHit* pAnimHit ) 
{
    USES_CONVERSION;

    m_pItemTime->SetDouble(pAnimHit->fTime);
    m_pItemPowerEffect->SetBool(pAnimHit->bPowerEffect);            
    m_pItemTargetBehavior->SetEnum(pAnimHit->eTargetBehavior);            
    m_pItemKB2Push->SetBool(pAnimHit->bKB2Push);            
    m_pItemKB2Push->SetHidden(pAnimHit->eTargetBehavior != TARGET_BEHAVIOR_KNOCK_DOWN);
    m_pItemHandType->SetEnum(pAnimHit->eHandType);
    m_pItemProjectileType->SetEnum(pAnimHit->uiProjectileEffectType);            
    m_pItemProjectileShot->SetEnum(pAnimHit->eProjectileShotType);
    m_pItemProjSpeed->SetDouble(pAnimHit->fProjectileSpeed);
    m_pItemTargetHeight->SetDouble(pAnimHit->fTargetHeight);
    m_pItemBoneName->SetValue((pAnimHit->chBoneName));
    m_pItemSubWeaponBoneFlag->SetFlags(pAnimHit->nSubWeaponFlag);
    m_pItemProjectileName->SetValue((pAnimHit->chProjectileEffectName));
    m_pItemTargetAttach->SetBool(pAnimHit->bTargetAttach);
    m_pItemTargetEffectName->SetValue((pAnimHit->chTargetEffectName));
    m_pItemTargetEffectType->SetEnum(pAnimHit->eTargetEffectType);
    m_pItemSubTargetEffect->SetValue((pAnimHit->chSubTargetEffect));
    m_pItemTargetSoundName->SetValue((pAnimHit->chTargetSoundName));
    m_pItemSoundType->SetEnum((int)pAnimHit->eSoundType);
    m_pItemHitSoundEcho->SetBool(pAnimHit->bHitSoundEcho);
    m_pItemWordEffect->SetValue((pAnimHit->chWordEffect));
    m_pItemCameraShake->SetBool(pAnimHit->bCameraShake);
    m_pItemShakeFactor->SetDouble(pAnimHit->fCameraShakeFactor);
    m_pItemShakeMaxHeight->SetDouble(pAnimHit->fCameraShakeMaxHeight);

    switch(pAnimHit->uiProjectileEffectType)
    {
    case BEID_PROJ_HISSIDAN:
        m_pHitHissidanApplyAngle->SetBool(pAnimHit->uEffectTypeExtraData.hissidanData.bApplyAngle);
        m_pHitHissidianDirection->SetOffset(pAnimHit->uEffectTypeExtraData.hissidanData.v2dAngle.x,
            pAnimHit->uEffectTypeExtraData.hissidanData.v2dAngle.y);                                                    
        break;
    case BEID_PROJ_HELLZONE:
        m_pHitHosidanOffset->SetOffset(pAnimHit->uEffectTypeExtraData.hellZoneData.vTargetEffectOffset.x,
            pAnimHit->uEffectTypeExtraData.hellZoneData.vTargetEffectOffset.y,
            pAnimHit->uEffectTypeExtraData.hellZoneData.vTargetEffectOffset.z);
        m_pHitHosidanWaitTime->SetDouble(pAnimHit->uEffectTypeExtraData.hellZoneData.fTargetEffectStartWaitTime);
        m_pHitHosidanSpeed->SetDouble(pAnimHit->uEffectTypeExtraData.hellZoneData.fTargetEffectSpeed);
        break;
    case BEID_PROJ_MULTI_HISSIDAN:
        m_pHitMultiHissidanCount->SetNumber(pAnimHit->uEffectTypeExtraData.multiHissidanData.nCount);                
        OnUpdateMultiHissidanIndex(pAnimHit);
        break;
    }

    if(pAnimHit->bCameraShake)
    {
        m_pItemCameraShake->Expand();
    }
    else
    {
        m_pItemCameraShake->Collapse();
    }

    SetAttackType(pAnimHit->eAttackType);
}

void CPropertyPane::SetHitEventProperties( SEventAnimHit* pEventHit, UINT nID ) 
{
    USES_CONVERSION;

    switch(nID)
    {
    case ID_HIT_TIME:        
        pEventHit->fTime = (RwReal)m_pItemTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventHit);
        break;
    case ID_HIT_POWER_EFFECT:        
        pEventHit->bPowerEffect = (RwBool)m_pItemPowerEffect->GetBool();
        break;
    case ID_HIT_TARGET_BRHAVIOR:        
        pEventHit->eTargetBehavior = (ETargetBehavior)m_pItemTargetBehavior->GetEnum();
        m_pItemKB2Push->SetHidden(pEventHit->eTargetBehavior != TARGET_BEHAVIOR_KNOCK_DOWN);
        break;
    case ID_HIT_KB2PUSH:
        pEventHit->bKB2Push = m_pItemKB2Push->GetBool();
        break;
    case ID_HIT_HAND_TYPE:        
        pEventHit->eHandType = (EHandType)m_pItemHandType->GetEnum(); 
        break;
    case ID_HIT_PROJECTILE_TYPE:        
        pEventHit->uiProjectileEffectType = (EProjectileEffectType)m_pItemProjectileType->GetEnum();

        // Projectile Type이 변환되면 Extra Data를 초기화 한다. (Union이기 때문)
        ZeroMemory(&pEventHit->uEffectTypeExtraData, sizeof(UEffectTypeExtraData));

        SetProjectileType();
        break;                
    case ID_HIT_PROJECTILE_SHOT:        
        pEventHit->eProjectileShotType = (EBoneType)m_pItemProjectileShot->GetEnum();

        switch(m_pItemProjectileShot->GetEnum())
        {
        case BONE_CHARACTER:
            m_pItemBoneName->SetHidden(FALSE);
            m_pItemSubWeaponBoneFlag->SetHidden(TRUE);
            break;
        case BONE_WEAPON:
            m_pItemBoneName->SetHidden(TRUE);
            m_pItemSubWeaponBoneFlag->SetHidden(TRUE);
            break;
        case BONE_SUB_WEAPON:
            m_pItemBoneName->SetHidden(TRUE);
            m_pItemSubWeaponBoneFlag->SetHidden(FALSE);
            break;
        }   
        break;
    case ID_HIT_BONE_NAME:                
        strcpy_s(pEventHit->chBoneName, (m_pItemBoneName->GetValue()));
        break;
    case ID_HIT_SUBWEAPON_BONE_FLAG:                    
        pEventHit->nSubWeaponFlag = m_pItemSubWeaponBoneFlag->GetFlags();
        break;
    case ID_HIT_PROJECTILE_NAME:                        
        strcpy_s(pEventHit->chProjectileEffectName, (m_pItemProjectileName->GetValue()));
        break;
    case ID_HIT_PROJECTILE_SPEED:        
        pEventHit->fProjectileSpeed = (RwReal)m_pItemProjSpeed->GetDouble();
        break;
    case ID_HIT_TARGET_ATTACH:        
        pEventHit->bTargetAttach = (RwBool)m_pItemTargetAttach->GetBool();
        break;
    case ID_HIT_TARGET_EFFECT_NAME:                
        strcpy_s(pEventHit->chTargetEffectName, (m_pItemTargetEffectName->GetValue()));
        break;
    case ID_HIT_TARGET_EFFECT_TYPE:
        pEventHit->eTargetEffectType = (ETargetEffectType)m_pItemTargetEffectType->GetEnum();
        break;
    case ID_HIT_SUB_TARGET_EFFECT:
        strcpy_s(pEventHit->chSubTargetEffect, (m_pItemSubTargetEffect->GetValue()));
        break;
    case ID_HIT_TARGET_SOUND_NAME:                
        {
            CString strTargetSoundName = GetFileNameFromPath(m_pItemTargetSoundName->GetValue());
            m_pItemTargetSoundName->SetValue(m_pItemTargetSoundName->GetValue());
            strcpy_s(pEventHit->chTargetSoundName, (strTargetSoundName));
        }        
        break;
    case ID_HIT_SOUND_TYPE:        
        pEventHit->eSoundType = (eChannelGroupType)m_pItemSoundType->GetEnum();
        break;
    case ID_HIT_SOUND_ECHO:
        pEventHit->bHitSoundEcho = m_pItemHitSoundEcho->GetBool();
        break;
    case ID_HIT_TARGET_HEIGHT:        
        pEventHit->fTargetHeight = (RwReal)m_pItemTargetHeight->GetDouble();
        break;   
    case ID_HIT_WORD_EFFECT:                
        strcpy_s(pEventHit->chWordEffect, (m_pItemWordEffect->GetValue()));
        break;
    case ID_HIT_CAMERA_SHAKE:        
        pEventHit->bCameraShake = m_pItemCameraShake->GetBool();        
        if(pEventHit->bCameraShake)
        {
            m_pItemCameraShake->Expand();
        }
        else
        {
            m_pItemCameraShake->Collapse();
        }
        break;
    case ID_HIT_SHAKE_FACTOR:
        pEventHit->fCameraShakeFactor = (RwReal)m_pItemShakeFactor->GetDouble();
        break;
    case ID_HIT_SHAKE_MAX_HEIGHT:
        pEventHit->fCameraShakeMaxHeight = (RwReal)m_pItemShakeMaxHeight->GetDouble();
        break;
    case ID_HIT_HOIDAN_OFFSET:
        pEventHit->uEffectTypeExtraData.hellZoneData.vTargetEffectOffset.x = (RwReal)m_pHitHosidanOffset->GetValueX();
        pEventHit->uEffectTypeExtraData.hellZoneData.vTargetEffectOffset.y = (RwReal)m_pHitHosidanOffset->GetValueY();
        pEventHit->uEffectTypeExtraData.hellZoneData.vTargetEffectOffset.z = (RwReal)m_pHitHosidanOffset->GetValueZ();
        break;
    case ID_HIT_HOIDAN_START_WAIT_TIME:
        pEventHit->uEffectTypeExtraData.hellZoneData.fTargetEffectStartWaitTime = (RwReal)m_pHitHosidanWaitTime->GetDouble();
        break;
    case ID_HIT_HOIDAN_SPEED:
        pEventHit->uEffectTypeExtraData.hellZoneData.fTargetEffectSpeed = (RwReal)m_pHitHosidanSpeed->GetDouble();
        break;
    case ID_HIT_HISSIDAN_APPLY_ANGLE:
        pEventHit->uEffectTypeExtraData.hissidanData.bApplyAngle = (RwBool)m_pHitHissidanApplyAngle->GetBool();
        m_pHitHissidianDirection->SetHidden(!m_pHitHissidanApplyAngle->GetBool());
        break;
    case ID_HIT_HISSDIAN_DIRECTION:
        pEventHit->uEffectTypeExtraData.hissidanData.v2dAngle.x = (RwReal)m_pHitHissidianDirection->GetValueX();
        pEventHit->uEffectTypeExtraData.hissidanData.v2dAngle.y = (RwReal)m_pHitHissidianDirection->GetValueY();                    

        // 라인을 렌더링한다.
        if(m_pCharacter && m_pEventAnim)
            m_pCharacter->SetRenderHissidian((SEventAnimHit*)m_pEventAnim);
        break;                
    case ID_HIT_MULTI_HISSIDAN_COUNT:
        {
            int nCount = (int)m_pHitMultiHissidanCount->GetNumber();
            if(nCount > 0)
            {
                // 새로운 버퍼를 만들어 기존 내용을 카피하고, 버퍼를 변경한다.
                RwV2d* pNewBuf = NTL_NEW RwV2d[nCount];
                ZeroMemory(pNewBuf, sizeof(RwV2d) * nCount);

                int nCopyCnt = (nCount < pEventHit->uEffectTypeExtraData.multiHissidanData.nCount) ? nCount : pEventHit->uEffectTypeExtraData.multiHissidanData.nCount;
                memcpy(pNewBuf, pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle, sizeof(RwV2d) * nCopyCnt);
                NTL_ARRAY_DELETE(pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle);

                pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle = pNewBuf;
                pEventHit->uEffectTypeExtraData.multiHissidanData.nCount = nCount;

                OnUpdateMultiHissidanIndex(pEventHit);
                m_pHitMultiHissidanIndex->SetHidden(FALSE);
                m_pHitMultiHissidanDir->SetHidden(FALSE);

                // 0번째 인덱스를 선택한다.
                m_pHitMultiHissidanIndex->SetEnum(0);
                m_pHitMultiHissidanDir->SetOffset(pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[0].x,
                    pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[0].y);                                                              

            }
            else
            {
                m_pHitMultiHissidanIndex->SetHidden(TRUE);
                m_pHitMultiHissidanDir->SetHidden(TRUE);
            }
        }
        break;
    case ID_HIT_MULTI_HISSIDAN_INDEX:
        {
            // 선택한 INDEX의 내용으로 DIR을 변경한다.
            int nIndex = m_pHitMultiHissidanIndex->GetEnum();
            m_pHitMultiHissidanDir->SetOffset(pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[nIndex].x,
                pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[nIndex].y);                                                          
        }
        break;
    case ID_HIT_MULTI_HISSIDAN_DIR:
        {
            int nIndex = m_pHitMultiHissidanIndex->GetEnum();
            pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[nIndex].x = (RwReal)m_pHitMultiHissidanDir->GetValueX();
            pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[nIndex].y = (RwReal)m_pHitMultiHissidanDir->GetValueY();

            // 라인을 렌더링한다.
            if(m_pCharacter && m_pEventAnim)
                m_pCharacter->SetRenderHissidian((SEventAnimHit*)m_pEventAnim);
        }                    
        break;
    }
}  

void CPropertyPane::OnLEBoneName()
{
    USES_CONVERSION;
    SEventLinkEffect* pEvent = (SEventLinkEffect*)m_pEventAnim;
    strncpy(pEvent->chBoneName, (m_pLEBoneName->GetValue()), MAX_DEFAULT_NAME);

    GetSafeInstance(CLinkEffectPane)->UpdateEffect(pEvent);
}

void CPropertyPane::OnLEAttachBone()
{
    SEventLinkEffect* pEvent = (SEventLinkEffect*)m_pEventAnim;
    pEvent->bAttachBone = (RwBool)m_pLEAttachBone->GetBool();

    if(pEvent->bAttachBone)
    {
        m_pLEBoneName->SetHidden(FALSE);
    }
    else
    {
        m_pLEBoneName->SetHidden(TRUE);
    }

    GetSafeInstance(CLinkEffectPane)->UpdateEffect(pEvent);
}

void CPropertyPane::OnLEOffsetPos()
{
    SEventLinkEffect* pEvent = (SEventLinkEffect*)m_pEventAnim;
    pEvent->vOffsetPos.x = (RwReal)m_pLEOffsetPos->GetValueX();
    pEvent->vOffsetPos.y = (RwReal)m_pLEOffsetPos->GetValueY();
    pEvent->vOffsetPos.z = (RwReal)m_pLEOffsetPos->GetValueZ();

    GetSafeInstance(CLinkEffectPane)->UpdateEffect(pEvent);
}

void CPropertyPane::OnLEOffsetRot()
{
}

void CPropertyPane::OnSTTIme()
{
    SEventWeightTime* pEvent = (SEventWeightTime*)m_pEventAnim;
    pEvent->fTime = (RwReal)m_pSTTime->GetDouble();

    GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEvent);
}

void CPropertyPane::OnSTLifeTime()
{
    SEventWeightTime* pEvent = (SEventWeightTime*)m_pEventAnim;
    pEvent->fLifeTime = (RwReal)m_pSTLifeTime->GetDouble();
}

void CPropertyPane::OnSTWeight()
{
    SEventWeightTime* pEvent = (SEventWeightTime*)m_pEventAnim;
    pEvent->fWeightValue = (RwReal)m_pSTWeight->GetDouble();
}

void CPropertyPane::OnCharFaceCameraPos()
{
    if(!m_pCharacter)
        return;

    RwV3d vFaceCameraPos;
    vFaceCameraPos.x = (RwReal)m_pCharFaceCameraPos->GetValueX();
    vFaceCameraPos.y = (RwReal)m_pCharFaceCameraPos->GetValueY();
    vFaceCameraPos.z = (RwReal)m_pCharFaceCameraPos->GetValueZ();

    m_pCharacter->GetProperty()->SetFaceCameraPos(vFaceCameraPos);

    GetSafeInstance(CModelToolApplication)->UpdateFaceCamera(vFaceCameraPos, m_pCharacter->GetProperty()->GetFaceCameraLookAt());
}

void CPropertyPane::OnCharFaceCameraHeight()
{
    if(!m_pCharacter)
        return;

    RwV3d vFaceCameraLookAt;
    vFaceCameraLookAt.x = (RwReal)m_pCharFaceCameraLookAt->GetValueX();
    vFaceCameraLookAt.y = (RwReal)m_pCharFaceCameraLookAt->GetValueY();
    vFaceCameraLookAt.z = (RwReal)m_pCharFaceCameraLookAt->GetValueZ();

    m_pCharacter->GetProperty()->SetFaceCameraLookAt(vFaceCameraLookAt);

    GetSafeInstance(CModelToolApplication)->UpdateFaceCamera(m_pCharacter->GetProperty()->GetFaceCameraPos(), vFaceCameraLookAt);
}

void CPropertyPane::SetTraceEventProperties( SEventTrace* pEventTrace, UINT nID ) 
{
    USES_CONVERSION;

    if(!pEventTrace)
        return;

    switch(nID)
    {
    case ID_TE_TIME:
        pEventTrace->fTime = (RwReal)m_pTEStartTime->GetDouble();
        break;
    case ID_TE_LIFE_TIME:            
        pEventTrace->fLifeTime = (RwReal)m_pTELifeTime->GetDouble();
        break;
    case ID_TE_EDGE_LIFE_TIME:
        pEventTrace->fEdgeLifeTime = (RwReal)m_pTEEdgeLifeTime->GetDouble();
        break;
    case ID_TE_ATTACH_TYPE:
        pEventTrace->eAttachType = (SEventTrace::EAttachType)m_pTEAttachType->GetEnum();                    
        Update();
        break;
    case ID_TE_TRACE_KIND:
        pEventTrace->eTraceKind = (SEventTrace::ETraceKind)m_pTETraceKind->GetEnum();
        Update();
        break;
    case ID_TE_START_BONE_NAME:
        strncpy(pEventTrace->strStartBoneName, (m_pTEStartBoneName->GetValue()), MAX_DEFAULT_NAME);
        break;
    case ID_TE_END_BONE_NAME:
        strncpy(pEventTrace->strEndBoneName, (m_pTEEndBoneName->GetValue()), MAX_DEFAULT_NAME);
        break;
    case ID_TE_TEXTURE_NAME:
        {
            CString strTextureName = m_pTETexture->GetValue();
            strTextureName = GetFileNameFromPath(strTextureName);
            m_pTETexture->SetValue(strTextureName);

            strncpy(pEventTrace->strTexture, (strTextureName), MAX_DEFAULT_NAME);
        }                    
        break;
    case ID_TE_START_BONE_OFFSET:
        pEventTrace->v3dStartBoneOffset.x = (RwReal)m_pTEStartBoneOffset->GetValueX();
        pEventTrace->v3dStartBoneOffset.y = (RwReal)m_pTEStartBoneOffset->GetValueY();
        pEventTrace->v3dStartBoneOffset.z = (RwReal)m_pTEStartBoneOffset->GetValueZ();                    
        break;
    case ID_TE_END_BONE_OFFSET:
        pEventTrace->v3dEndBoneOffset.x = (RwReal)m_pTEEndBoneoffset->GetValueX();
        pEventTrace->v3dEndBoneOffset.y = (RwReal)m_pTEEndBoneoffset->GetValueY();
        pEventTrace->v3dEndBoneOffset.z = (RwReal)m_pTEEndBoneoffset->GetValueZ();
        break;
    case ID_TE_FRAME_SKIP_COUNT:
        pEventTrace->fEdgeGap = (RwReal)m_pTEEdgeGap->GetDouble();
        break;
    case ID_TE_SPLINE_POINT_COUNT:
        pEventTrace->nSplinePointCount = m_pTESplinePointCount->GetNumber();
        break;
    case ID_TE_MAX_EDGE_COUNT:
        pEventTrace->nMaxEdgeCount = m_pTEMaxEdgeCount->GetNumber();
        break;                
    case ID_TE_MAX_LENGTH:
        pEventTrace->fMaxLength = (RwReal)m_pTEMaxLength->GetDouble();
        break;
    case ID_TE_SRC_BLEND:
        pEventTrace->eSrcBlend = (RwBlendFunction)m_pTESrcBlend->GetEnum();
        break;
    case ID_TE_DEST_BLEND:
        pEventTrace->eDestBlend = (RwBlendFunction)m_pTEDestBlend->GetEnum();
        break;
    case ID_TE_START_COLOR:
        pEventTrace->colStartColor.red = GetRValue(m_pTEStartColor->GetColor());
        pEventTrace->colStartColor.green = GetGValue(m_pTEStartColor->GetColor());
        pEventTrace->colStartColor.blue = GetBValue(m_pTEStartColor->GetColor());
        break;
    case ID_TE_END_COLOR:        
        pEventTrace->colEndColor.red = GetRValue(m_pTEEndColor->GetColor());
        pEventTrace ->colEndColor.green = GetGValue(m_pTEEndColor->GetColor());
        pEventTrace->colEndColor.blue = GetBValue(m_pTEEndColor->GetColor());
        break;
    case ID_TE_START_ALPHA:
        pEventTrace->colStartColor.alpha = (RwUInt8)m_pTEStartAlpha->GetNumber();
        break;
    case ID_TE_END_ALPHA:
        pEventTrace->colEndColor.alpha = (RwUInt8)m_pTEEndAlpha->GetNumber();
        break;
    }
}

void CPropertyPane::ApplyTraceEventProperties(SEventTrace* pEventTrace) 
{
    USES_CONVERSION;

    if(CModelToolApplication::GetInstance()->GetAppMode() == MT_MODE_ITEM)      // Item의 Trace Event
    {
        m_pCatTraceEvent->SetHidden(FALSE);
        m_pCatTraceEvent->Collapse();
        m_pTEStartTime->SetHidden(TRUE);
        m_pTELifeTime->SetHidden(TRUE);
        m_pTEEdgeLifeTime->SetHidden(TRUE);
        m_pTEAttachType->SetHidden(TRUE);
        m_pTETraceKind->SetHidden(TRUE);
        m_pTEStartBoneName->SetHidden(FALSE);
        m_pTEEndBoneName->SetHidden(FALSE);
        m_pTETexture->SetHidden(FALSE);
        m_pTEStartBoneOffset->SetHidden(FALSE);
        m_pTEEndBoneoffset->SetHidden(FALSE);
        m_pTEEdgeGap->SetHidden(FALSE);
        m_pTESplinePointCount->SetHidden(FALSE);
        m_pTEMaxEdgeCount->SetHidden(FALSE);
        m_pTEMaxLength->SetHidden(FALSE);
        m_pTESrcBlend->SetHidden(FALSE);
        m_pTEDestBlend->SetHidden(FALSE);
        m_pTEStartColor->SetHidden(FALSE);
        m_pTEEndColor->SetHidden(FALSE);
        m_pTEStartAlpha->SetHidden(FALSE);
        m_pTEEndAlpha->SetHidden(FALSE);
    }
    else                // PC의 Trace Event
    {
        m_pTEStartTime->SetHidden(FALSE);
        m_pTELifeTime->SetHidden(FALSE);
        m_pTEEdgeLifeTime->SetHidden(FALSE);                        
        m_pTEAttachType->SetHidden(FALSE);
        m_pTETraceKind->SetHidden(FALSE);

        if(pEventTrace->eAttachType == SEventTrace::CHARACTER_BONE)
        {
            m_pTEStartBoneName->SetHidden(FALSE);
            m_pTEEndBoneName->SetHidden(FALSE);
            m_pTETraceKind->SetHidden(TRUE);

            pEventTrace->eTraceKind = SEventTrace::EVENT_TRACE;
        }
        else
        {
            m_pTEStartBoneName->SetHidden(TRUE);
            m_pTEEndBoneName->SetHidden(TRUE);
            m_pTETraceKind->SetHidden(FALSE);                
        }

        if(pEventTrace->eTraceKind == SEventTrace::EVENT_TRACE)
        {
            m_pTEStartBoneOffset->SetHidden(FALSE);
            m_pTEEndBoneoffset->SetHidden(FALSE);
            m_pTETexture->SetHidden(FALSE);
            m_pTEEdgeGap->SetHidden(FALSE);
            m_pTESplinePointCount->SetHidden(FALSE);
            m_pTEMaxEdgeCount->SetHidden(FALSE);
            m_pTEMaxLength->SetHidden(FALSE);
            m_pTESrcBlend->SetHidden(FALSE);
            m_pTEDestBlend->SetHidden(FALSE);
            m_pTEStartColor->SetHidden(FALSE);
            m_pTEEndColor->SetHidden(FALSE);
            m_pTEStartAlpha->SetHidden(FALSE);
            m_pTEEndAlpha->SetHidden(FALSE);
        }
        else
        {
            m_pTEStartBoneOffset->SetHidden(TRUE);
            m_pTEEndBoneoffset->SetHidden(TRUE);
            m_pTETexture->SetHidden(TRUE);
            m_pTEEdgeGap->SetHidden(TRUE);
            m_pTESplinePointCount->SetHidden(TRUE);
            m_pTEMaxEdgeCount->SetHidden(TRUE);
            m_pTEMaxLength->SetHidden(TRUE);
            m_pTESrcBlend->SetHidden(TRUE);
            m_pTEDestBlend->SetHidden(TRUE);
            m_pTEStartColor->SetHidden(TRUE);
            m_pTEEndColor->SetHidden(TRUE);
            m_pTEStartAlpha->SetHidden(TRUE);
            m_pTEEndAlpha->SetHidden(TRUE);
        }          
    }

    m_pTEStartTime->SetDouble(pEventTrace->fTime);
    m_pTELifeTime->SetDouble(pEventTrace->fLifeTime);
    m_pTEEdgeLifeTime->SetDouble(pEventTrace->fEdgeLifeTime);
    m_pTEAttachType->SetEnum(pEventTrace->eAttachType);
    m_pTETraceKind->SetEnum(pEventTrace->eTraceKind);
    m_pTEStartBoneName->SetValue((pEventTrace->strStartBoneName));
    m_pTEEndBoneName->SetValue((pEventTrace->strEndBoneName));
    m_pTEStartBoneOffset->SetOffset(pEventTrace->v3dStartBoneOffset.x, pEventTrace->v3dStartBoneOffset.y, pEventTrace->v3dStartBoneOffset.z);
    m_pTEEndBoneoffset->SetOffset(pEventTrace->v3dEndBoneOffset.x, pEventTrace->v3dEndBoneOffset.y, pEventTrace->v3dEndBoneOffset.z);
    m_pTETexture->SetValue((pEventTrace->strTexture));
    m_pTEEdgeGap->SetDouble(pEventTrace->fEdgeGap);
    m_pTESplinePointCount->SetNumber(pEventTrace->nSplinePointCount);
    m_pTEMaxEdgeCount->SetNumber(pEventTrace->nMaxEdgeCount);
    m_pTEMaxLength->SetDouble(pEventTrace->fMaxLength);
    m_pTESrcBlend->SetEnum(pEventTrace->eSrcBlend);
    m_pTEDestBlend->SetEnum(pEventTrace->eDestBlend);
    m_pTEStartColor->SetColor(RGB(pEventTrace->colStartColor.red, pEventTrace->colStartColor.green, pEventTrace->colStartColor.blue));
    m_pTEStartAlpha->SetNumber(pEventTrace->colStartColor.alpha);
    m_pTEEndColor->SetColor(RGB(pEventTrace->colEndColor.red, pEventTrace->colEndColor.green, pEventTrace->colEndColor.blue));
    m_pTEEndAlpha->SetNumber(pEventTrace->colEndColor.alpha);                            
}

void CPropertyPane::ApplyVisualEffectEventProperties( const SEventVisualEffect* pEventVisualEffect ) 
{
    USES_CONVERSION;

    m_pVETime->SetDouble(pEventVisualEffect->fTime);
    m_pVEAttach->SetBool(pEventVisualEffect->bAttach);
    m_pVEAttachType->SetEnum(pEventVisualEffect->bAttachBone);            
    m_pVEProjectileType->SetBool(pEventVisualEffect->bProjectileType);
    m_pVEOffsetPos->SetOffset(pEventVisualEffect->vOffSetPos.x, pEventVisualEffect->vOffSetPos.y, pEventVisualEffect->vOffSetPos.z);                                    
    m_pVEName->SetValue((pEventVisualEffect->chEffectName));
    m_pVEBoneType->SetEnum(pEventVisualEffect->eBoneType);
    m_pVEBoneName->SetValue((pEventVisualEffect->chBoneName));            
    m_pVEWeaponBoneName->SetValue((pEventVisualEffect->chBoneName));
    m_pVEApplyScale->SetBool(pEventVisualEffect->bApplyScale);

    if(!pEventVisualEffect->bAttach)
    {        
        m_pVEAttachType->SetHidden(TRUE);
        m_pVEBoneType->SetHidden(TRUE);        
        m_pVEBoneName->SetHidden(TRUE);
        m_pVEWeaponBoneName->SetHidden(TRUE);
    }    
    else
    {
        m_pVEAttachType->SetHidden(FALSE);

        if(!pEventVisualEffect->bAttachBone)
        {
            m_pVEBoneType->SetHidden(TRUE);
            m_pVEBoneName->SetHidden(TRUE);
            m_pVEWeaponBoneName->SetHidden(TRUE);
        }
        else
        {
            m_pVEBoneType->SetHidden(FALSE);

            switch(m_pVEBoneType->GetEnum())
            {
            case BONE_CHARACTER:
                m_pVEBoneName->SetHidden(FALSE);
                m_pVEWeaponBoneName->SetHidden(TRUE);                
                break;
            case BONE_WEAPON:
                m_pVEBoneName->SetHidden(TRUE);
                m_pVEWeaponBoneName->SetHidden(TRUE);
                break;
            case BONE_SUB_WEAPON:
                m_pVEBoneName->SetHidden(TRUE);
                m_pVEWeaponBoneName->SetHidden(FALSE);
                break;
            }
        }   
    } 
}

void CPropertyPane::SetVisualEffectEventProperties( SEventVisualEffect* pEventVisualEffect, UINT nID ) 
{
    USES_CONVERSION;

    switch(nID)
    {
    case ID_VE_TIME:        
        pEventVisualEffect->fTime = (RwReal)m_pVETime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventVisualEffect);        
        break;
    case ID_VE_NAME:        
        if(!CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_EFFECT, (m_pVEName->GetValue())))
        {
            MessageBox("Not Exist Effect!");
            return;
        }
        else
        {
            strcpy_s(pEventVisualEffect->chEffectName, ( m_pVEName->GetValue()));
        }        
        break;
    case ID_VE_PROJECTILE_TYPE:
        pEventVisualEffect->bProjectileType = (RwBool)m_pVEProjectileType->GetBool();
        break;
    case ID_VE_ATTACH:
        pEventVisualEffect->bAttach = (RwBool)m_pVEAttach->GetBool();
        if(!pEventVisualEffect->bAttach)
        {
            m_pVEAttachType->SetHidden(TRUE);
            m_pVEBoneType->SetHidden(TRUE);        
            m_pVEBoneName->SetHidden(TRUE);
            m_pVEWeaponBoneName->SetHidden(TRUE);
            break;
        }
        else
        {
            m_pVEAttachType->SetHidden(FALSE);
        }
    case ID_VE_ATTACH_BONE:        
        pEventVisualEffect->bAttachBone = (RwBool)m_pVEAttachType->GetEnum();
        if(!pEventVisualEffect->bAttachBone)
        {
            m_pVEBoneType->SetHidden(TRUE);
            m_pVEBoneName->SetHidden(TRUE);
            m_pVEWeaponBoneName->SetHidden(TRUE);
            break;
        }
        else
        {
            m_pVEBoneType->SetHidden(FALSE);
        }
    case ID_VE_BONE_TYPE:
        pEventVisualEffect->eBoneType = (EBoneType)m_pVEBoneType->GetEnum();

        switch(m_pVEBoneType->GetEnum())
        {
        case BONE_CHARACTER:
            m_pVEBoneName->SetHidden(FALSE);
            m_pVEWeaponBoneName->SetHidden(TRUE);
            m_pVEOffsetPos->SetHidden(FALSE);
            break;
        case BONE_WEAPON:
        case BONE_SUB_WEAPON:
            m_pVEBoneName->SetHidden(TRUE);
            m_pVEWeaponBoneName->SetHidden(FALSE);            
            m_pVEOffsetPos->SetHidden(TRUE);
            break;
        }
        break;    
    case ID_VE_BONE_NAME:         
        strncpy(pEventVisualEffect->chBoneName, (m_pVEBoneName->GetValue()), MAX_DEFAULT_NAME);
        break;
    case ID_VE_SUB_WEAPON_BONE_NAME:
        strcpy(pEventVisualEffect->chBoneName, (m_pVEWeaponBoneName->GetValue()));
        break;    
    case ID_VE_OFFSET_POS:
        pEventVisualEffect->vOffSetPos.x = (RwReal)m_pVEOffsetPos->GetValueX();
        pEventVisualEffect->vOffSetPos.y = (RwReal)m_pVEOffsetPos->GetValueY();
        pEventVisualEffect->vOffSetPos.z = (RwReal)m_pVEOffsetPos->GetValueZ();
        break;                            
    case ID_VE_APPLY_SCALE:
        pEventVisualEffect->bApplyScale = m_pVEApplyScale->GetBool();
        break;
    }
}

void CPropertyPane::ApplySubWeaponEventProperties( const SEventSubWeapon* pEventSubWeapon ) 
{
    if(!pEventSubWeapon)
        return;

    m_pSWTime->SetDouble(pEventSubWeapon->fTime);
    m_pSWActive->SetEnum(pEventSubWeapon->eSubWeaponActiveFlag);
}

void CPropertyPane::SetSubWeaponProperties( SEventSubWeapon* pEventSubWeapon, UINT nID ) 
{
    switch(nID)
    {
    case ID_SW_TIME:
        pEventSubWeapon->fTime = (RwReal)m_pSWTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventSubWeapon);
        break;
    case ID_SW_ACTIVE:
        pEventSubWeapon->eSubWeaponActiveFlag = (ESubWeaponActiveFlag)m_pSWActive->GetEnum();
        break;
    }
}

void CPropertyPane::SetProjectileType() 
{
    m_pHitHissidanApplyAngle->SetHidden(TRUE);
    m_pHitHissidianDirection->SetHidden(TRUE);
    m_pHitHosidanOffset->SetHidden(TRUE);
    m_pHitHosidanWaitTime->SetHidden(TRUE);
    m_pHitHosidanSpeed->SetHidden(TRUE);
    m_pHitMultiHissidanCount->SetHidden(TRUE);
    m_pHitMultiHissidanIndex->SetHidden(TRUE);
    m_pHitMultiHissidanDir->SetHidden(TRUE);

    if(m_pItemProjectileType->GetEnum() == BEID_PROJ_HISSIDAN)
    {
        m_pHitHissidanApplyAngle->SetHidden(FALSE);
        if(m_pHitHissidanApplyAngle->GetBool())
        {
            m_pHitHissidianDirection->SetHidden(FALSE);

            if(m_pCharacter && m_pEventAnim)
                m_pCharacter->SetRenderHissidian((SEventAnimHit*)m_pEventAnim);
        }
        else
        {
            m_pHitHissidianDirection->SetHidden(TRUE);
        }

    }
    else if(m_pItemProjectileType->GetEnum() == BEID_PROJ_MULTI_HISSIDAN)
    {
        m_pHitMultiHissidanCount->SetHidden(FALSE);
        if(m_pHitMultiHissidanCount->GetNumber() > 0)
        {
            m_pHitMultiHissidanIndex->SetHidden(FALSE);
            m_pHitMultiHissidanDir->SetHidden(FALSE);
        }

        if(m_pCharacter && m_pEventAnim)
            m_pCharacter->SetRenderHissidian((SEventAnimHit*)m_pEventAnim);                
    }
    else if(m_pItemProjectileType->GetEnum() == BEID_PROJ_HELLZONE)
    {
        m_pHitHosidanOffset->SetHidden(FALSE);
        m_pHitHosidanWaitTime->SetHidden(FALSE);
        m_pHitHosidanSpeed->SetHidden(FALSE);

        if(m_pCharacter)
            m_pCharacter->SetRenderHissidian(NULL);
    }
    else
    {
        if(m_pCharacter)
            m_pCharacter->SetRenderHissidian(NULL);
    }
}
void CPropertyPane::ApplyPostEffectEventProperties( const SEventPostEffect* pEventPostEffect ) 
{
	if(!pEventPostEffect)
		return;

	USES_CONVERSION;

	m_pPETime->SetDouble(pEventPostEffect->fTime);	
	m_pPEEffectName->SetValue((pEventPostEffect->szPostEffectName));
	m_pPETarget->SetEnum(pEventPostEffect->eTarget);
	m_pPEOffset->SetOffset(pEventPostEffect->v3dOffset.x, pEventPostEffect->v3dOffset.y, pEventPostEffect->v3dOffset.z);
    m_pPECenterFix->SetBool(pEventPostEffect->bCenterFixEnable);
    m_pPETargetHeight->SetDouble(pEventPostEffect->fTargetHeight);
    m_pPEPCBoneName->SetValue((pEventPostEffect->szPCBoneName));

    if(pEventPostEffect->eTarget == POST_EFFECT_TARGET_TYPE_SELF)
    {
        m_pPEPCBoneName->SetHidden(FALSE);
        m_pPETargetHeight->SetHidden(TRUE);
    }
    else if(pEventPostEffect->eTarget == POST_EFFECT_TARGET_TYPE_TARGET)
    {
        m_pPEPCBoneName->SetHidden(TRUE);
        m_pPETargetHeight->SetHidden(FALSE);
    }
}

void CPropertyPane::SetPostEffectEventProperties( SEventPostEffect* pEventPostEffect, UINT nID ) 
{
	if(!pEventPostEffect)
		return;

	USES_CONVERSION;

    switch(nID)
    {
    case ID_PE_TIME:
        pEventPostEffect->fTime = (RwReal)m_pPETime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventPostEffect);
        break;
    case ID_PE_NAME:
        strcpy(pEventPostEffect->szPostEffectName, (m_pPEEffectName->GetValue()));
        break;
    case ID_PE_TARGET:
        pEventPostEffect->eTarget = (EPostEffectTypeFlag)m_pPETarget->GetEnum();
        if(pEventPostEffect->eTarget == POST_EFFECT_TARGET_TYPE_SELF)
        {
            m_pPEPCBoneName->SetHidden(FALSE);
            m_pPETargetHeight->SetHidden(TRUE);
        }
        else if(pEventPostEffect->eTarget == POST_EFFECT_TARGET_TYPE_TARGET)
        {
            m_pPEPCBoneName->SetHidden(TRUE);
            m_pPETargetHeight->SetHidden(FALSE);
        }
        break;
    case ID_PE_OFFSET:
        pEventPostEffect->v3dOffset.x = (RwReal)m_pPEOffset->GetValueX();
        pEventPostEffect->v3dOffset.y = (RwReal)m_pPEOffset->GetValueY();
        pEventPostEffect->v3dOffset.z = (RwReal)m_pPEOffset->GetValueZ();
        break;
    case ID_PE_CENTER_FIX:
        pEventPostEffect->bCenterFixEnable = (RwBool)m_pPECenterFix->GetBool();
        break;
    case ID_PE_TARGET_HEIGHT:
        pEventPostEffect->fTargetHeight = (RwReal)m_pPETargetHeight->GetDouble();
        break;
    case ID_PE_PC_BONE:
        strcpy_s(pEventPostEffect->szPCBoneName, (m_pPEPCBoneName->GetValue()));
        break;
    }
}

void CPropertyPane::OnUpdateMultiHissidanIndex(SEventAnimHit* pAnimHit)
{
    m_pHitMultiHissidanIndex->GetConstraints()->RemoveAll();
    if(pAnimHit->uEffectTypeExtraData.multiHissidanData.nCount > 0)
    {
        CHAR buf[4] = {0,};
        for(int i = 0; i < pAnimHit->uEffectTypeExtraData.multiHissidanData.nCount; ++i)
        {
            sprintf_s(buf, "%d", i);
            m_pHitMultiHissidanIndex->GetConstraints()->AddConstraint(buf, i);
        }

        m_pHitMultiHissidanIndex->SetEnum(0);        
        m_pHitMultiHissidanDir->SetOffset(pAnimHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[0].x,
                                          pAnimHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[0].y,
                                          0.0f);
    }
}

void CPropertyPane::ApplySummonPetEventProperties( const SEventSummonPet* pEventSummonPet ) 
{
    m_pSUETime->SetDouble(pEventSummonPet->fTime);
}
void CPropertyPane::SetSummonPetEventProperties( SEventSummonPet* pEventSummonPet, UINT nID ) 
{
    switch(nID)
    {
    case ID_SUE_TIME:
        pEventSummonPet->fTime = (RwReal)m_pSUETime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventSummonPet);        
        break;
    }
}

void CPropertyPane::ApplyTMQEventProperties( const SEventAnimCinematic* pEventTMQ ) 
{
    m_pTMQTime->SetDouble(pEventTMQ->fTime);
    m_pTMQType->SetEnum(pEventTMQ->eEventID);
}

void CPropertyPane::SetTMQEventProperties( SEventAnimCinematic* pEventTMQ, UINT nID ) 
{
    switch(nID)
    {
    case ID_TMQ_TIME:
        pEventTMQ->fTime = (RwReal)m_pTMQTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventTMQ);        
        break;
    case ID_TMQ_TYPE:
        pEventTMQ->eAnimCinematicEventType = (EAnimCinematicEventType)m_pTMQType->GetEnum();
        break;    
    }
}

void CPropertyPane::ApplyAlphaEventProperties( const SEventAlpha* pEventAlpha ) 
{
    USES_CONVERSION;

    m_pAlphaTime->SetDouble(pEventAlpha->fTime);
    m_pAlphaStart->SetNumber(pEventAlpha->nStartAlpha);
    m_pAlphaDest->SetNumber(pEventAlpha->nDestAlpha);
    m_pAlphaLifeTime->SetDouble(pEventAlpha->fLifeTime);
    m_pAlphaFadeTime->SetDouble(pEventAlpha->fFadeTime);
    m_pAlphaType->SetEnum(pEventAlpha->eAlphaEventType);    
    
    if(m_pAlphaType->GetEnum() == SEventAlpha::E_ALPHA_EVENT_CLUMP)
    {
        for(int i = 0; i < MAX_ALPHA_ATOMIC; ++i)
        {
            m_pAlphaAtomic[i]->SetHidden(TRUE);

        }
    }
    else
    {
        SetAtomicList(pEventAlpha);        
    }
}

void CPropertyPane::SetAlphaEventProperties( SEventAlpha* pEventAlpha, UINT nID ) 
{
    USES_CONVERSION;

    switch(nID)
    {
    case ID_ALPHA_TIME:
        pEventAlpha->fTime = (RwReal)m_pAlphaTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventAlpha);        
        break;
    case ID_ALPHA_START:
        pEventAlpha->nStartAlpha = (RwInt32 )m_pAlphaStart->GetNumber();
        break;
    case ID_ALPHA_DEST:
        pEventAlpha->nDestAlpha = (RwInt32)m_pAlphaDest->GetNumber();
        break;
    case ID_ALPHA_LIFE_TIME:
        pEventAlpha->fLifeTime = (RwReal)m_pAlphaLifeTime->GetDouble();
        break;
    case ID_ALPHA_FADE_TIME:
        pEventAlpha->fFadeTime = (RwReal)m_pAlphaFadeTime->GetDouble();
        break;
    case ID_ALPHA_TYPE:
        pEventAlpha->eAlphaEventType = (SEventAlpha::EAlphaEventType)m_pAlphaType->GetEnum();

        if(pEventAlpha->eAlphaEventType == SEventAlpha::E_ALPHA_EVENT_CLUMP)
        {
            for(int i = 0; i < MAX_ALPHA_ATOMIC; ++i)
            {
                m_pAlphaAtomic[i]->SetHidden(TRUE);
            }
        }
        else
        {
            SetAtomicList(pEventAlpha);            
        }
        break;    
    }

    if(nID >= ID_ALPHA_ATOMIC && nID < ID_ALPHA_ATOMIC + MAX_ALPHA_ATOMIC)
    {
        int nIndex = nID - ID_ALPHA_ATOMIC;
        Helper_SetBigFlag(&pEventAlpha->bfAtomicList, nIndex , m_pAlphaAtomic[nIndex]->GetBool());
    }
}

void CPropertyPane::SetAtomicList(const SEventAlpha* pEventAlpah)
{
    USES_CONVERSION;

    ENTITY_ATOMIC_VEC vAtomicNames;
    if(m_pCharacter)
    {
        vAtomicNames = m_pCharacter->GetAtomicList();
    }
    else if(m_pObject)
    {
        vAtomicNames = m_pObject->GetAtomicList();
    }
    else
    {
        return;
    }

    for(UINT i = 0; i < vAtomicNames.size(); ++i)
    {
        std::string strAtomicName = Helper_AtomicName(vAtomicNames[i]->pData);
        m_pAlphaAtomic[i]->SetCaption((strAtomicName.c_str()));
        m_pAlphaAtomic[i]->SetHidden(FALSE);

        // 이벤트의 비트 플래그로부터 설정값을 가져온다.
        if(Helper_GetBitFlag(pEventAlpah->bfAtomicList, i))
        {
            m_pAlphaAtomic[i]->SetBool(TRUE);
        }
        else
        {
            m_pAlphaAtomic[i]->SetBool(FALSE);
        }
    }
    for(UINT i = vAtomicNames.size(); i < MAX_ALPHA_ATOMIC; ++i)
    {
        m_pAlphaAtomic[i]->SetHidden(TRUE);
    }    
}

void CPropertyPane::ApplyFootStepEventProperties( const SEventFootStep* pEventFootStep ) 
{
    m_pFSTime->SetDouble(pEventFootStep->fTime);
    m_pFSType->SetEnum(pEventFootStep->eFootStepType);    
    m_pFSMobType->SetEnum(pEventFootStep->eFootStepMobType);
}

void CPropertyPane::SetFootStepEventProperties( SEventFootStep* pEventFootStep, UINT nID ) 
{
    switch(nID)
    {
    case ID_FS_TIME:
        pEventFootStep->fTime = (RwReal)m_pFSTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventFootStep);
        break;
    case ID_FS_TYPE:
        pEventFootStep->eFootStepType = (EFootStepType)m_pFSType->GetEnum();
        break;
    case ID_FS_MOB_TYPE:
        pEventFootStep->eFootStepMobType = (EFootStepMobType)m_pFSMobType->GetEnum();
        break;
    }
}

void CPropertyPane::ApplyExplosionEventProperties( const SEventExplosion* pEventExplosion ) 
{
    m_pExplosionTime->SetDouble(pEventExplosion->fTime);
    m_pExplosionType->SetEnum(pEventExplosion->eType);
}

void CPropertyPane::SetExplosionEventProperties( SEventExplosion* pEventExplosion, UINT nID ) 
{
    switch(nID)
    {
    case ID_EXPLOSION_TIME:
        pEventExplosion->fTime = (RwReal)m_pExplosionTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventExplosion);
        break;
    case ID_EXPLOSION_TYPE:
        pEventExplosion->eType = (ENtlPLExplosionEventType)m_pExplosionType->GetEnum();
        break;
    }
}

void CPropertyPane::ApplyUpgradeEffectProperties( const SUpgradeEffectProperty* pUpgradeEffectProperty ) 
{
    USES_CONVERSION;

    m_pCatUpgradeProperty->SetHidden(FALSE);

    m_pUpgradeBone1->SetValue((pUpgradeEffectProperty->szBone1));
    m_pUpgradeBone2->SetValue((pUpgradeEffectProperty->szBone2));

	// to do
    m_pUpgradeEffect1->SetValue((pUpgradeEffectProperty->szEffect[0]));
    m_pUpgradeEffect2->SetValue((pUpgradeEffectProperty->szEffect[1]));

    m_pUpgradeOffset1->SetOffset(pUpgradeEffectProperty->vOffset1.x, 
                                 pUpgradeEffectProperty->vOffset1.y,
                                 pUpgradeEffectProperty->vOffset1.z);
    m_pUpgradeOffset2->SetOffset(pUpgradeEffectProperty->vOffset2.x,
                                 pUpgradeEffectProperty->vOffset2.y,
                                 pUpgradeEffectProperty->vOffset2.z);
}

void CPropertyPane::SetUpgradeEffectProperties( SUpgradeEffectProperty* pUpgradeEffectProperty, UINT nID ) 
{
    USES_CONVERSION;

	// to do:
	// aff effect 3-15

    switch(nID)
    {    
    case ID_UPGRADE_BONE1:
        sprintf_s(pUpgradeEffectProperty->szBone1, "%s", (m_pUpgradeBone1->GetValue()));
        break;
    case ID_UPGRADE_BONE2:
        sprintf_s(pUpgradeEffectProperty->szBone2, "%s", (m_pUpgradeBone2->GetValue()));
        break;
    case ID_UPGRADE_EFFECT1:
        sprintf_s(pUpgradeEffectProperty->szEffect[0], "%s", (m_pUpgradeEffect1->GetValue()));
        break;
    case ID_UPGRADE_EFFECT2:
        sprintf_s(pUpgradeEffectProperty->szEffect[1], "%s", (m_pUpgradeEffect2->GetValue()));
        break;
    case ID_UPGRADE_OFFSET1:
        pUpgradeEffectProperty->vOffset1.x = (RwReal)m_pUpgradeOffset1->GetValueX();
        pUpgradeEffectProperty->vOffset1.y = (RwReal)m_pUpgradeOffset1->GetValueY();
        pUpgradeEffectProperty->vOffset1.z = (RwReal)m_pUpgradeOffset1->GetValueZ();
        break;
    case ID_UPGRADE_OFFSET2:
        pUpgradeEffectProperty->vOffset2.x = (RwReal)m_pUpgradeOffset2->GetValueX();
        pUpgradeEffectProperty->vOffset2.y = (RwReal)m_pUpgradeOffset2->GetValueY();
        pUpgradeEffectProperty->vOffset2.z = (RwReal)m_pUpgradeOffset2->GetValueZ();
        break;
    }

    // Effect Update
    if(m_pItem)
    {
        RwInt32 nGrade = m_pItem->GetUpgradeEffect();
        m_pItem->SetUpgradeEffect(ITEM_GRADE_NONE);
        m_pItem->SetUpgradeEffect((ENtlPLItemGrade)nGrade);
    }
}

void CPropertyPane::ApplySoundEventProperties( const SEventSound* pEventSound ) 
{
    USES_CONVERSION;

    m_pSETime->SetDouble(pEventSound->fTime);           
    m_pSESoundName1->SetValue((pEventSound->chSoundName));
    m_pSESoundName2->SetValue((pEventSound->chSoundName2));
    m_pSESoundName3->SetValue((pEventSound->chSoundName3));
    m_pSESoundName4->SetValue((pEventSound->chSoundName4));
    m_pSESoundType->SetEnum((int)pEventSound->eSoundType);
    m_pSESoundLoop->SetBool(pEventSound->bLoop);
    m_pSESoundVolume->SetNumber((int)pEventSound->fSoundVolume);
    m_pSESoundDist->SetNumber((int)pEventSound->fSoundDist);
    m_pSESoundDecayDist->SetNumber((int)pEventSound->fSoundDecayDist);
    m_pSESoundPitchMin->SetDouble(pEventSound->fSoundPitchMin);
    m_pSESoundPitchMax->SetDouble(pEventSound->fSoundPitchMax);
}

void CPropertyPane::SetSoundEventProperties( SEventSound* pEventSound, UINT nID ) 
{
    USES_CONVERSION;

    switch(nID)
    {
    case ID_SE_TIME:        
        pEventSound->fTime = (RwReal)m_pSETime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventSound);
        break;
    case ID_SE_SOUND_NAME1:      
        {
            // 사운드 폴더의 하위 경로까지만 반환한다.
            CString strSoundName = m_pSESoundName1->GetValue();            
            strSoundName = GetSoundFilePath(strSoundName);
            m_pSESoundName1->SetValue(strSoundName);
            sprintf_s(pEventSound->chSoundName, "%s", (strSoundName));
        }        
        break;
    case ID_SE_SOUND_NAME2:
        {
            // 사운드 폴더의 하위 경로까지만 반환한다.
            CString strSoundName = m_pSESoundName2->GetValue();            
            strSoundName = GetSoundFilePath(strSoundName);
            m_pSESoundName2->SetValue(strSoundName);
            sprintf_s(pEventSound->chSoundName2, "%s", (strSoundName));
        }        
        break;
    case ID_SE_SOUND_NAME3:
        {
            // 사운드 폴더의 하위 경로까지만 반환한다.
            CString strSoundName = m_pSESoundName3->GetValue();            
            strSoundName = GetSoundFilePath(strSoundName);
            m_pSESoundName3->SetValue(strSoundName);
            sprintf_s(pEventSound->chSoundName3, "%s", (strSoundName));
        }        
        break;
    case ID_SE_SOUND_NAME4:
        {
            // 사운드 폴더의 하위 경로까지만 반환한다.
            CString strSoundName = m_pSESoundName4->GetValue();            
            strSoundName = GetSoundFilePath(strSoundName);
            m_pSESoundName4->SetValue(strSoundName);
            sprintf_s(pEventSound->chSoundName4, "%s", (strSoundName));
        }        
        break;
    case ID_SE_SOUND_TYPE:
        pEventSound->eSoundType = (eChannelGroupType)m_pSESoundType->GetEnum();
        break;
    case ID_SE_SOUND_LOOP:
        pEventSound->bLoop = m_pSESoundLoop->GetBool();
        break;
    case ID_SE_SOUND_VOLUME:
        pEventSound->fSoundVolume = (RwReal)m_pSESoundVolume->GetNumber();
        break;
    case ID_SE_SOUND_DIST:
        pEventSound->fSoundDist = (RwReal)m_pSESoundDist->GetNumber();
        break;
    case ID_SE_SOUND_DECAY_DIST:
        pEventSound->fSoundDecayDist = (RwReal)m_pSESoundDecayDist->GetNumber();
        break;
    case ID_SE_SOUND_PITCH_MIN:
        pEventSound->fSoundPitchMin = (RwReal)m_pSESoundPitchMin->GetDouble();
        break;
    case ID_SE_SOUND_PITCH_MAX:
        pEventSound->fSoundPitchMax = (RwReal)m_pSESoundPitchMax->GetDouble();
        break;
    }
}

void CPropertyPane::ApplyDirectEventProperties( const SEventDirect* pEventDirect ) 
{
    m_pDirectEventTime->SetDouble(pEventDirect->fTime);
    m_pDirectEventType->SetEnum(pEventDirect->eType);    
}

void CPropertyPane::SetDirectEventProperties( SEventDirect* pEventDirect, UINT nID ) 
{
    switch(nID)
    {
    case ID_DIRECT_TIME:
        pEventDirect->fTime = (RwReal)m_pDirectEventTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventDirect);
    case ID_DIRECT_TYPE:
        pEventDirect->eType = (ENtlPLDirectEventType)m_pDirectEventType->GetEnum();
        break;
    }
}


void CPropertyPane::ApplyColorChangeEventProperties( const SEventColorChange* pEventColorChange ) 
{
    m_pColChangeEventTime->SetDouble(pEventColorChange->fTime);
    m_pColChangeEventType->SetEnum(pEventColorChange->eType);
    m_pColChangeEventEdge->SetColor(RwRGBA2RGB(pEventColorChange->colorEdge));
    m_pColChangeEventBody->SetColor(RwRGBA2RGB(pEventColorChange->colorBody));
    m_pColChangeEventAdd->SetColor(RwRGBA2RGB(pEventColorChange->colorAdd));
    
    if(pEventColorChange->eType == COLOR_CHANGE_TYPE_END)
    {
        m_pColChangeEventEdge->SetHidden(TRUE);
        m_pColChangeEventBody->SetHidden(TRUE);
        m_pColChangeEventAdd->SetHidden(TRUE);     

    }
    else
    {
        m_pColChangeEventEdge->SetHidden(FALSE);
        m_pColChangeEventBody->SetHidden(FALSE);
        m_pColChangeEventAdd->SetHidden(FALSE);
    }
}

void CPropertyPane::SetColorChangeEventProperties( SEventColorChange* pEventColorChange, UINT nID ) 
{
    switch(nID)
    {
    case ID_COLCHANGE_TIME:
        pEventColorChange->fTime = (RwReal)m_pColChangeEventTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventColorChange);
        break;
    case ID_COLCHANGE_TYPE:
        pEventColorChange->eType = (EColorChangeType)m_pColChangeEventType->GetEnum();
        if(pEventColorChange->eType == COLOR_CHANGE_TYPE_END)
        {
            m_pColChangeEventEdge->SetHidden(TRUE);
            m_pColChangeEventBody->SetHidden(TRUE);
            m_pColChangeEventAdd->SetHidden(TRUE);     

        }
        else
        {
            m_pColChangeEventEdge->SetHidden(FALSE);
            m_pColChangeEventBody->SetHidden(FALSE);
            m_pColChangeEventAdd->SetHidden(FALSE);
        }
        break;
    case ID_COLCHANGE_EDGE:
        RGB2RwRGBA(pEventColorChange->colorEdge, m_pColChangeEventEdge->GetColor());        
        break;
    case ID_COLCHANGE_BODY:
        RGB2RwRGBA(pEventColorChange->colorBody, m_pColChangeEventBody->GetColor());
        break;
    case ID_COLCHANGE_ADD:
        RGB2RwRGBA(pEventColorChange->colorAdd, m_pColChangeEventAdd->GetColor());
        break;
    }
}


void CPropertyPane::ApplyStretchEventProperties( const SEventStretch* pEventStretch ) 
{
    USES_CONVERSION;
    m_pStretchEventTime->SetDouble(pEventStretch->fTime);
    m_pStretchEventType->SetEnum(pEventStretch->eType);
    m_pStretchBoneName[0]->SetValue((pEventStretch->szBoneName[0]));
    m_pStretchBoneName[1]->SetValue((pEventStretch->szBoneName[1]));
    m_pStretchSpeed->SetDouble(pEventStretch->fStretchSpeed);
    m_pStretchWidth->SetDouble(pEventStretch->fWidth);    
    m_pStretchAccel->SetDouble(pEventStretch->fAccel);
    m_pStretchScaleName->SetValue((pEventStretch->szScaleBone));
    m_pStretchScaleSize->SetDouble(pEventStretch->fScaleSize);
    m_pStretchAxisBoneName->SetValue((pEventStretch->szAxisBone));
    m_pStretchTargetEffect->SetValue((pEventStretch->szTargetEffect));
}

void CPropertyPane::SetStretchEventProperties( SEventStretch* pEventStretch, UINT nID ) 
{
    USES_CONVERSION;

    switch(nID)
    {
    case ID_STRETCH_TIME:
        pEventStretch->fTime = (RwReal)m_pStretchEventTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventStretch);
        break;
    case ID_STRETCH_TYPE:
        pEventStretch->eType = (EStretchEventType)m_pStretchEventType->GetEnum();
        break;
    case ID_STRETCH_BONE1_NAME:                
        sprintf_s(pEventStretch->szBoneName[0], "%s", (m_pStretchBoneName[0]->GetValue()));
        break;
    case ID_STRETCH_BONE2_NAME:        
        sprintf_s(pEventStretch->szBoneName[1], "%s", (m_pStretchBoneName[1]->GetValue()));
        break;
    case ID_STRETCH_BONE_SPEED:
        pEventStretch->fStretchSpeed = (RwReal)m_pStretchSpeed->GetDouble();
        break;    
    case ID_STRETCH_BONE_WIDTH:
        pEventStretch->fWidth = (RwReal)m_pStretchWidth->GetDouble();
        break;
    case ID_STRETCH_BONE_ACCEL:
        pEventStretch->fAccel = (RwReal)m_pStretchAccel->GetDouble();
        break;
    case ID_STRETCH_SCALE_NAME:
        sprintf_s(pEventStretch->szScaleBone, "%s", (m_pStretchScaleName->GetValue()));
        break;
    case ID_STRETCH_SCALE_SIZE:
        pEventStretch->fScaleSize = (RwReal)m_pStretchScaleSize->GetDouble();
        break;
    case ID_STRETCH_AXIS_BONE:
        sprintf_s(pEventStretch->szAxisBone, "%s", (m_pStretchAxisBoneName->GetValue()));
        break;
    case ID_STRETCH_TARGET_EFFECT:
        sprintf_s(pEventStretch->szTargetEffect, "%s", (m_pStretchTargetEffect->GetValue()));
        break;
    }
}

void CPropertyPane::ApplyTriggerEventPrperties( const SEventTrigger* pEventTrigger ) 
{
    m_pTriggerEventTime->SetDouble(pEventTrigger->fTime);
}

void CPropertyPane::SetTriggerEventProperties( SEventTrigger* pEventTrigger, UINT nID ) 
{
    switch(nID)
    {
    case ID_TRIGGER_TIME:
        pEventTrigger->fTime = (RwReal)m_pTriggerEventTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventTrigger);
        break;
    }
}

void CPropertyPane::SetObjectProperties( CMTObject* pObject, UINT nID ) 
{
    if(!pObject)
        return;

    USES_CONVERSION;  

    RwBool bSoundChange = FALSE;
    
    switch(nID)
    {
    case ID_OBJ_SHADOW:
        pObject->GetProperty()->SetEnableShadow((RwBool)m_pObjShadow->GetBool());
        break;
    case ID_OBJ_PICKING:
        //pObject->GetProperty()->SetPicking((RwBool)m_pObjPicking->GetBool());
        break;
    case ID_OBJ_CULLDIST:
        pObject->GetProperty()->SetCullingDistance((RwReal)m_pObjCullDist->GetDouble());
        pObject->SetVisibleCullingDistance((RwReal)m_pObjCullDist->GetDouble());
        break;
    case ID_OBJ_COLLISION:
        pObject->GetProperty()->SetCollision(m_pObjCollision->GetBool());
        if(m_pObjCollision->GetBool())
            m_pObjCollisonMesh->SetHidden(FALSE);
        else
            m_pObjCollisonMesh->SetHidden(TRUE);
        break;
    case ID_OBJ_BBOX:
        {
            RwBBox bbox;
            RwReal fWidth = (RwReal)m_pObjBBox->GetValueX();
            RwReal fHeight = (RwReal)m_pObjBBox->GetValueY();
            RwReal fDepth = (RwReal)m_pObjBBox->GetValueZ();

            bbox.sup.x = fWidth / 2.0f;
            bbox.sup.y = fHeight;
            bbox.sup.z = fDepth / 2.0f;
            bbox.inf.x = -bbox.sup.x;
            bbox.inf.y = 0.0f;
            bbox.inf.z = -bbox.sup.z;

            pObject->GetProperty()->SetBBox(bbox);
        }        
        break;
    case ID_OBJ_UVANIM:
        {
            CString strWorkFolder = (CModelToolApplication::GetInstance()->GetWorkDir());
            CString strUVAnimFileName = m_pObjUVAnim->GetValue();
            strWorkFolder.MakeUpper();
            strUVAnimFileName.MakeUpper();
            strUVAnimFileName.Replace(strWorkFolder, ".");
            m_pObjUVAnim->SetValue(strUVAnimFileName);

            pObject->GetProperty()->m_strUVAnimFileName = (strUVAnimFileName);

            ::SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir());
            if(pObject->SetUVAnim((strUVAnimFileName)))    
            {
                // UVAnim을 적용한후 Clump를 ReLoad한다.
                pObject->LoadClump((RwChar*)pObject->GetClumpInfo()->GetClumpName());
                pObject->GetUVAnim()->SetClump(pObject->GetClump());
            }
            else
            {
                MessageBox("UV Anim Setup Fail", NULL, MB_ICONERROR);
            }
        }        
        break;
    case ID_OBJ_UVANIM_SPEED:
        {
            RwReal fSpeed = (RwReal)m_pObjUVAnimSpeed->GetDouble();
            pObject->GetProperty()->SetUVAnimSpeed(fSpeed);
            pObject->GetUVAnim()->SetUVAnimSpeed(fSpeed);
        }        
        break;
    case ID_OBJ_ANIM:
        {
            CString strWorkFolder = (CModelToolApplication::GetInstance()->GetWorkDir());
            CString strAnimFileName =m_pObjAnim->GetValue();
            if(strAnimFileName == "")
            {
                pObject->GetProperty()->m_strAnimFileName = "";
                pObject->SetAnimation(NULL);
                return;
            }

            strWorkFolder.MakeUpper();
            strAnimFileName.MakeUpper();
            strAnimFileName.Replace(strWorkFolder, ".");

            ::SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir());
            m_pObjAnim->SetValue(strAnimFileName);
            pObject->GetProperty()->m_strAnimFileName = (strAnimFileName);
            if(!pObject->SetAnimation((strAnimFileName)))
            {
                MessageBox("Animation Setup Fail!", NULL, MB_ICONERROR);
            }
        }
        break;
    case ID_OBJ_COLLISON_MESH:
        {
            CString strCollisonMeshName = m_pObjCollisonMesh->GetValue();
            strCollisonMeshName = GetFileNameFromPath(strCollisonMeshName);
            m_pObjCollisonMesh->SetValue(strCollisonMeshName);
            pObject->GetProperty()->m_strCollisionMeshName = (strCollisonMeshName);               
        }                
        break;
    case ID_OBJ_TYPE:
        pObject->GetProperty()->SetObjectType((EPLObjectType)m_pObjType->GetEnum());
        break;
    case ID_OBJ_APPLY_PVS:
        pObject->GetProperty()->SetApplyPVS((RwBool)m_pObjApplyPVS->GetBool());
        break;
    case ID_OBJ_SORT_BY_POS:
        {
            RwInt32 nFlag = pObject->GetProperty()->GetFlag();
            nFlag = m_pObjSortByPos->GetBool() ? nFlag | OBJ_FLAG_SORT_BY_POS : nFlag & ~OBJ_FLAG_SORT_BY_POS;            
            pObject->GetProperty()->SetFlag(nFlag);
        }        
        break;
    case ID_OBJ_FORCE_PICKING:
        {
            RwInt32 nFlag = pObject->GetProperty()->GetFlag();
            nFlag = m_pObjForcePicking->GetBool() ? nFlag | OBJ_FLAG_FORCE_PICKING : nFlag & ~OBJ_FLAG_FORCE_PICKING;
            pObject->GetProperty()->SetFlag(nFlag);
        }
        break;
    case ID_OBJ_PE_NAVI_PRESS:
        {
            RwUInt32 flag = pObject->GetProperty()->GetFlag();
            flag = m_pObjPENaviPress->GetBool() ? flag | OBJ_FLAG_PE_NAVI_PRESS : flag & ~OBJ_FLAG_PE_NAVI_PRESS;
            pObject->GetProperty()->SetFlag(flag);
        }
        break;
    case ID_OBJ_CULLTEST_ALL_ATOMIC:
        {
            RwUInt32 flag = pObject->GetProperty()->GetFlag();
            flag = m_pObjCullTestAllAtomic->GetBool() ? flag | OBJ_FLAG_CULLTEST_ATOMIC : flag & ~OBJ_FLAG_CULLTEST_ATOMIC;
            pObject->GetProperty()->SetFlag(flag);
        }
        break;
    case ID_SE_SOUND_NAME1:
        {
            // 사운드 폴더의 하위 경로까지만 반환한다.
            CString strSoundName = m_pObjSoundName->GetValue();            
            strSoundName = GetSoundFilePath(strSoundName);
            m_pObjSoundName->SetValue(strSoundName);
            sprintf_s(pObject->GetProperty()->GetSoundProp()->chSoundName, "%s", (strSoundName));

            bSoundChange = TRUE;
        }        
        break;
    case ID_SE_SOUND_VOLUME:
        pObject->GetProperty()->GetSoundProp()->fSoundVolume = (RwReal)m_pObjSoundVolume->GetNumber();
        bSoundChange = TRUE;
        break;
    case ID_SE_SOUND_DIST:
        pObject->GetProperty()->GetSoundProp()->fSoundDist = (RwReal)m_pObjSoundDist->GetNumber();
        bSoundChange = TRUE;
        break;
    case ID_SE_SOUND_DECAY_DIST:
        pObject->GetProperty()->GetSoundProp()->fSoundDecayDist = (RwReal)m_pObjSoundDecayDist->GetNumber();
        bSoundChange = TRUE;
        break;
    case ID_SE_SOUND_PITCH_MIN:        
        pObject->GetProperty()->GetSoundProp()->fSoundPitchMin = (RwReal)m_pObjSoundPicth->GetDouble();
        bSoundChange = TRUE;
        break;
    }

    // 사운드 관련 설정이 변경되었을때
    if(bSoundChange)
    {
        pObject->ClearLoopSound();
        SEventSound* pEventSound = pObject->GetProperty()->GetSoundProp();        

		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_OBJECT_MUSIC;
		tSoundParam.pcFileName		= pEventSound->chSoundName;
		tSoundParam.bLoop			= true;
		tSoundParam.fVolume			= pEventSound->fSoundVolume * 0.01f;
		tSoundParam.fPitch			= pEventSound->fSoundPitchMin;
		tSoundParam.fMinDistance	= pEventSound->fSoundDist;
		tSoundParam.fMaxDistance	= pEventSound->fSoundDecayDist;

		GetSoundManager()->Play(&tSoundParam);

		pObject->AddLoopSound(tSoundParam.hHandle);
    }
}

void CPropertyPane::SetItemProperties(CNtlPLItemProperty* pProperty, UINT nID)
{
    switch(nID)
    {
    case ID_ITEM_ATTACH_TYPE:
        pProperty->SetResType((EItemResType)m_pItemAttachType->GetEnum());
        break;
    case ID_ITEM_ATTACH_SLOT:
        pProperty->SetEquipSlotType((EItemEquipSlotType)m_pItemAttachSlot->GetEnum());                
        break;            
    case ID_ITEM_ATTACH_OFFSET:
        {
            RwV3d v3dOffset;
            v3dOffset.x = (RwReal)m_pItemAttachOffset->GetValueX();
            v3dOffset.y = (RwReal)m_pItemAttachOffset->GetValueY();
            v3dOffset.z = (RwReal)m_pItemAttachOffset->GetValueZ();
            pProperty->SetAttachOffset(v3dOffset);
            m_pItem->SetPosition(&v3dOffset);
        }                
        break;
    case ID_ITEM_TRACE_ENABLE:
        pProperty->SetTraceEnableDefault((RwBool)m_pItemTraceEnable->GetBool());
        break;            
    case ID_ITEM_APPLY_EMBLEM:
        {
            RwBool bFlag = m_pItemApplyEmblem->GetBool();
            if(bFlag)
            {
                pProperty->SetFlag(pProperty->GetFlag() | ITEM_FLAG_APPLY_EMBLEM);
            }            
            else
            {
                pProperty->SetFlag(pProperty->GetFlag() & ~ITEM_FLAG_APPLY_EMBLEM);
            }
        }        
        break;
    }

    SetTraceEventProperties(&m_pItem->GetProperty()->m_eventTrace, nID);
    SetUpgradeEffectProperties(m_pItem->GetProperty()->GetUpgradeEffectProperty(), nID);
}

void CPropertyPane::SetCharacterProperties( CNtlPLCharacterProperty* pProperty, UINT nID ) 
{
    switch(nID)
    {
    case ID_CHAR_FACE_CAMERA_POS:
        OnCharFaceCameraPos();
        break;
    case ID_CHAR_FACE_CAMERA_HEIGHT:
        OnCharFaceCameraHeight();
        break;
    case ID_CHAR_INIT_WALK_SPEED:
        pProperty->SetInitWalkFrontAnimSpeed((RwReal)m_pCharInitWalkSpeed->GetDouble());
        break;
    case ID_CHAR_INIT_RUN_SPEED:
        pProperty->SetInitRunFrontAnimSpeed((RwReal)m_pCharInitRunSpeed->GetDouble());
        break;
    case ID_CHAR_APPLY_TANGENT:
        {
            RwBool bFlag = m_pCharApplyTangent->GetBool();
            if(bFlag)
            {
                pProperty->SetFlag(pProperty->GetFlag() | CHAR_FLAG_APPLY_NORMAL);
            }
            else
            {
                pProperty->SetFlag(pProperty->GetFlag() & ~CHAR_FLAG_APPLY_NORMAL);
            }
        }
    case ID_CHAR_ALPHA_DIST:
        pProperty->SetAlphaDistance((RwReal)m_pCharAlphaDistance->GetDouble());
        break;
    }
}



void CPropertyPane::SetAnimationProperties( STypeAnimData* pAnimData, UINT nID ) 
{
    NTL_ASSERT(pAnimData, __FUNCTION__<<__LINE__<<" Anim Data is NULL");
    if(!pAnimData)
        return;

    switch(nID)
    {
    case ID_ANIM_CULL_CULLTEST:
        pAnimData->SetCullTestAllAtomic(m_pAnimCullTestAllAtomic->GetBool());
        break;
    }    
}

void CPropertyPane::ApplySkillCancelProperties(const SEventSkillCancel* pEventSkillCancel)
{
    m_pSCTime->SetDouble(pEventSkillCancel->fTime);
}

void CPropertyPane::SetSkillCancelProperties(SEventSkillCancel* pEventSkillCancel, UINT nID)
{
    switch(nID)
    {
    case ID_SC_TIME:
        pEventSkillCancel->fTime = (RwReal)m_pSCTime->GetDouble();
        GetSafeInstance(CAnimPlayPane)->SetMarkerPos(pEventSkillCancel);
        break;
    }
}

