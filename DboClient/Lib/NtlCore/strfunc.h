/*****************************************************************************
*
* File :     strfunc.h
*
* Abstract : Definition of enum strfunc_func
*
*****************************************************************************
*
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on Criterion Software Ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __strfunc_H__
#define __strfunc_H__

namespace RWS
{
   /**
   *
   *  \ingroup Stream
   *
   *  RenderWare Studio Stream Functions- Remote function calls, this table defines the Id's of the functions
   *  that can be called by the RenderWare Studio Workspace via the network link.
   *
   */

   enum strfunc_func
   {
      strfunc_VersionNumber = -1,            /**<  Used to identify the version number
                                                   of this interface DWORD 0x0100 Revision 1.00 */
      
      strfunc_Reset = 0,                     /**<  Reset, sent at start of stream, used to purge system
                                                   during a connect.*/

      strfunc_Reserved1 = 1,                 /**<  Reserved */
      strfunc_Reserved2 = 2,                 /**<  Reserved */

      strfunc_SetDirectorsCameraMatrix = 3,  /**<  Positions Camera at new position defined by
                                                   gfCameraMsg */

      strfunc_CreateEntity = 4,              /**<  Create an entity, typically calls ClassFactory::MakeNew */
      strfunc_UpdateEntityAttributes = 5,    /**<  Update entity attributes, typically calls
                                                   CAttributeHandler::HandleAttributes */

      strfunc_SetFrozenMode = 6,             /**<  Pause Game */
      strfunc_SetRunningMode = 7,            /**<  Unpause Game */
      
      strfunc_EnableDirectorsCamera = 8,     /**<  Enable the Directors Camera */
      strfunc_DisableDirectorsCamera = 9,    /**<  Disble the Directors Camera */
      
      strfunc_TextComment = 10,              /**<  Send a text message - useful for debugging streams */
      
      strfunc_StartSystem = 11,              /**<  Start system, during loading system is halted */
      strfunc_StopSystem = 12,               /**<  Stop system */
      
      strfunc_DeleteEntity = 13,             /**<  Delete one entity */
      strfunc_DeleteAllEntities = 14,        /**<  Delete all entities, sent prior to resending
                                                   the entities to reset the scene */

      strfunc_UnLoadAsset = 15,              /**<  Delete an asset */

      strfunc_Shutdown = 16,                 /**<  Shut down console */
      strfunc_CloseConnection = 17,          /**<  Close connection to server */
      strfunc_SendTestEvent = 18,            /**<  Send a test event message, i.e. uses SendMsg */

      strfunc_Reserved3 = 19,                /**<  Reserved */

      strfunc_Reserved3b = 20,               /**<  Reserved */

      strfunc_LoadAsset = 21,                /**<  Specifies a resource to be loaded via the
                                                   current device, equivalent to load file by reference.*/

      strfunc_LoadEmbeddedAsset = 22,        /**<  UpLoad a resource, same as strfunc_LoadResources
                                                   but contains the actual data to be uploaded, equivalent
                                                   to load file by value.*/

      strfunc_Reserved4 = 23,                /**<  Reserved */

      strfunc_GetEntityMatrix = 24,          /**<  Workspace request for an entities transformation matrix
                                                   to be sent to the editor. Note there is a special case
                                                   where the Id is 0 meaning the current camera. */

      strfunc_CustomData = 25,               /**<  A chunk containing custom data. */
                                                   
      strfunc_FunctionProfiler   = 26,       /**<  Used by the function profiler for sending commands to 
                                                   the console.*/

      strfunc_ResetEntity = 27,              /**<  Reset an entity. The chunk data consists of the full
                                                   attribute packet for the entity, as if it were
                                                   being created for the first time. */

      strFunc_PlacementNew = 28,             /**<  A chunk containing data refering to placement new
                                                   information */
      
      strfunc_Initialize = 29,               /**<  Initialise the framework for the start of a game. */

      strfunc_UpdateAsset = 30,              /**<  Update resource, typically calls
                                                   CResourceHandler::Update */

      strfunc_DynamicSequence = 31           /**<  Sequence data provide from the workspace, contains
                                                   a sequence control data block for 'live update' of
                                                   entities on the console. */
   } ;

   const RwUInt32 ChunkHeaderSize = 12;
   enum strfuncsend_func // Outgoing commands
   {
      strfuncsend_NamedData = 0              /**<  A command to send arbitrary data to workspace,
                                                   along with a string identifying the data type. */
   } ;
}//namespace RWS

#endif /*__strfunc_func_H__*/
