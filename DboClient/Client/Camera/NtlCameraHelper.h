/*****************************************************************************
 *
 * File			: NTLCameraHelper.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 25	
 * Abstract		: NTl CameraHelper
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_CAMERA_HELPER__
#define __NTL_CAMERA_HELPER__

/**
 * \ingroup Client
 * Camera 관련 Helper 함수들 이다.
 * 
 */

class CNtlCameraHelper
{

public:
	static RwBool CameraRotation(const RwCamera *pCamera, RwReal turn, RwReal tilt);
	static RwBool CameraTranslateAt(const RwCamera *pCamera, RwReal delta);
	static RwBool CameraTranslate(const RwCamera *pCamera, const RwV3d *delta);
};

#endif