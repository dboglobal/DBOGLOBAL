/*****************************************************************************
 *
 * File			: NTLGammaController
 * Author		: Haesung Cho
 * Copyright	: (аж)NTL
 * Date			: 2009. 4. 10	
 * Abstract		: NTL Gamma Controller
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef NTL_GAMMA_CONTROLLER_H_
#define NTL_GAMMA_CONTROLLER_H_

#include "ceventhandler.h"
#include "NtlDeviceRestoreHandler.h"

class CNtlGammaController : public RWS::CEventHandler
{
public:
	CNtlGammaController();
	~CNtlGammaController();

	RwBool	Create();
	RwBool	Create( RwReal fGamma );
	void	Destroy();

	void	BackupBackGroundGamma();
	void	RestoreBackGroundGamma();

	void	SetGamma( RwReal fGamma );
	void	SetCurrentGamma();
	void	GetGammaRamp(void* pGammapRamp, RwReal fGamma, RwReal fOverBright = 1.0f, RwReal fContrast = 1.0f);

	void	HandleEvents(RWS::CMsg &pMsg);

	int		RestoreHandler();

protected:
	D3DGAMMARAMP	m_rampBackGroundBackup;
	D3DGAMMARAMP	m_rampCurrent;

	RwReal			m_fGamma;
	RwBool			m_bGammaOffset;

	RwBool			m_bCreate;

	// device restore
	DEVICE_RESTORE_HANDLE m_hRestore;
};

#endif