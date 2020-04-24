#include "precomp_dboclient.h"
#include ".\NtlCameraHelper.h"
#include "NtlDebug.h"

/**
 * Camera 자신을 좌,우 상,하로 움직인다.
 * \param *pCamera 회전하고자 하는 Camera
 * \param turn Yaw
 * \param tilt Pitch
 * \return 성공 유무
 */
RwBool CNtlCameraHelper::CameraRotation(const RwCamera *pCamera, RwReal turn, RwReal tilt)
{
	NTL_FUNCTION("CNtlCameraHelper::CameraRotation" );
	NTL_ASSERT( pCamera != NULL, "pCamera == NULL");

	if(pCamera == NULL)
		NTL_RETURN(FALSE);

	RwFrame *cameraFrame;
    RwV3d delta, pos, *right;

    static RwReal totalTilt = 0.0f;
    static RwV3d Yaxis = {0.0f, 1.0f, 0.0f};

    /*
     * Limit the camera's tilt so that it never quite reaches
     * exactly +90 or -90 degrees...
     */
    if( totalTilt + tilt > 89.0f )
    {
        tilt = 89.0f - totalTilt;

    }
    else if( totalTilt + tilt < -89.0f )
    {
        tilt = -89.0f - totalTilt;
    }

    totalTilt += tilt;

    cameraFrame = RwCameraGetFrame(pCamera);

    /*
     * Remember where the camera is...
     */
    pos = *RwMatrixGetPos(RwFrameGetMatrix(cameraFrame));

    /*
     * Remove the translation component...
     */
    RwV3dScale(&delta, &pos, -1.0f);
    RwFrameTranslate(cameraFrame, &delta, rwCOMBINEPOSTCONCAT);

    /*
     * Rotate to the new direction...
     */
    right = RwMatrixGetRight(RwFrameGetMatrix(cameraFrame));
    RwFrameRotate(cameraFrame, right, tilt, rwCOMBINEPOSTCONCAT);
    RwFrameRotate(cameraFrame, &Yaxis, -turn, rwCOMBINEPOSTCONCAT);

    /*
     * Put the camera back to where it was...
     */
    RwFrameTranslate(cameraFrame, &pos, rwCOMBINEPOSTCONCAT);

	NTL_RETURN(TRUE);
}



/**
 * Camera의 AtVector 방향으로 이동을 시킨다
 * \param *pCamera 이동하고자 하는 Camera
 * \param delta 회전 각도
 * \return 성공유무
 */
RwBool CNtlCameraHelper::CameraTranslateAt(const RwCamera *pCamera, RwReal delta)
{
	NTL_FUNCTION("CNtlCameraHelper::CameraTranslateAt" );
	NTL_ASSERT( pCamera != NULL, "pCamera == NULL");

	if(pCamera == NULL)
		return FALSE;
	
	RwFrame *frame;
    RwV3d at = { 0, 0, 1};

    frame = RwCameraGetFrame(pCamera);
    at = *RwMatrixGetAt(RwFrameGetMatrix(frame));

    RwV3dScale(&at, &at, delta );
    RwFrameTranslate(frame, &at, rwCOMBINEPOSTCONCAT);

	NTL_RETURN(TRUE);
}


/**
 * Camera를 Vector만큼 이동을 시킨다.
 * \param *pCamera 이동하고자 하는 Camera
 * \param *pDelta Camera의 이동값.
 * \return 성공유무
 */
RwBool CNtlCameraHelper::CameraTranslate(const RwCamera *pCamera, const RwV3d *delta)
{
	NTL_FUNCTION("CNtlCameraHelper::CameraTranslate" );
	NTL_ASSERT( pCamera != NULL, "pCamera == NULL");
	NTL_ASSERT( delta != NULL, "delta == NULL");

	if(pCamera == NULL || delta == NULL)
		return FALSE;

	RwV3d offset;
    RwFrame *cameraFrame;
    RwMatrix *cameraMatrix;
    RwV3d *at, *up, *right;

    cameraFrame = RwCameraGetFrame(pCamera);
    cameraMatrix = RwFrameGetMatrix(cameraFrame);
    
    at = RwMatrixGetAt(cameraMatrix);
    up = RwMatrixGetUp(cameraMatrix);
    right = RwMatrixGetRight(cameraMatrix);

    offset.x = delta->x * right->x + delta->y * up->x + delta->z * at->x;
    offset.y = delta->x * right->y + delta->y * up->y + delta->z * at->y;
    offset.z = delta->x * right->z + delta->y * up->z + delta->z * at->z;

    RwFrameTranslate(cameraFrame, &offset, rwCOMBINEPOSTCONCAT);

	NTL_RETURN(TRUE);
}
