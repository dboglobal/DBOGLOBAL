/*
 * RenderStateVectors in custom pipelines
 *
 * Copyright (c) Criterion Software Limited
 */
/****************************************************************************
 *                                                                          *
 *  Module  :   p2renderstate.c                                             *
 *                                                                          *
 *  Purpose :   RxRenderStateVector handling                                *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 Includes
 */

#include "bapipe.h"
#include "p2core.h"

#include "p2renderstate.h"


/*****************************************************************************
 Local (static) globals
 */


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwcoregeneric
 * \ref RxRenderStateVectorSetDefaultRenderStateVector sets a
 * RxRenderStateVector to the default state.
 *
 * This function should not be called before RwEngineStart() has been called.
 *
 * \param rsvp  A pointer to the vector to fill
 *
 * \return A pointer to the vector to fill on success, or NULL if there is an error
 *
 * \see RxRenderStateVectorGetDefaultRenderStateVector
 * \see RxRenderStateVectorLoadDriverState
 * \see RxRenderStateVectorCreate
 * \see RxRenderStateVectorDestroy
 */
RxRenderStateVector *
RxRenderStateVectorSetDefaultRenderStateVector(RxRenderStateVector * rsvp)
{
    RWAPIFUNCTION(RWSTRING("RxRenderStateVectorSetDefaultRenderStateVector"));
    RWASSERT(rsvp != NULL);

    /* This is used during _rxPipelineOpen()
     * which caches the result for reuse later
     * (e.g in RxRenderStateVectorCreate()
     */

    /* We assume that if this is called before the engine is started it
       is to setup RXPIPELINEGLOBAL(defaultRenderState) in _rxPipelineOpen() */

    if (rsvp != NULL)
    {
        if (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED)
        {
            *rsvp = RXPIPELINEGLOBAL(defaultRenderState);
        }
        else
        {
            if (rsvp != &RXPIPELINEGLOBAL(defaultRenderState))
            {
                RWERROR((E_RW_PLUGINNOTINIT));
                RWRETURN((RxRenderStateVector *)NULL);
            }
            else
            {
                RwRGBA OpaqueWhite = { 255, 255, 255, 255 };

                rsvp->Flags = rxRENDERSTATEFLAG_TEXTUREPERSPECTIVE |
        /*                    rxRENDERSTATEFLAG_VERTEXALPHAENABLE | */
                              rxRENDERSTATEFLAG_ZTESTENABLE |
                              rxRENDERSTATEFLAG_ZWRITEENABLE;

                rsvp->ShadeMode = rwSHADEMODEGOURAUD;
                rsvp->SrcBlend = rwBLENDSRCALPHA;
                rsvp->DestBlend = rwBLENDINVSRCALPHA;
                rsvp->TextureRaster = (RwRaster *)NULL;
                rsvp->AddressModeU = rwTEXTUREADDRESSWRAP;
                rsvp->AddressModeV = rwTEXTUREADDRESSWRAP;
                rsvp->FilterMode = rwFILTERLINEAR;
                rsvp->BorderColor = OpaqueWhite;
                rsvp->FogType = rwFOGTYPENAFOGTYPE;
                rsvp->FogColor = OpaqueWhite;
            }
        }
        RWRETURN(rsvp);
    }
    RWERROR((E_RW_NULLP));
    RWRETURN((RxRenderStateVector *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxRenderStateVectorGetDefaultRenderStateVector returns a CONST
 * pointer to the global default RxRenderStateVector.
 *
 * This function should not be called before RwEngineStart() has been called.
 *
 * \return A pointer to the vector to fill on success, or NULL if there is an error
 *
 * \see RxRenderStateVectorLoadDriverState
 * \see RxRenderStateVectorCreate
 * \see RxRenderStateVectorDestroy
 * \see RxRenderStateVectorSetDefaultRenderStateVector
 */
const RxRenderStateVector *
RxRenderStateVectorGetDefaultRenderStateVector(void)
{
    RWAPIFUNCTION(RWSTRING("RxRenderStateVectorGetDefaultRenderStateVector"));
    if (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED)
    {
        RWRETURN(&(RXPIPELINEGLOBAL(defaultRenderState)));
    }
    RWERROR((E_RW_PLUGINNOTINIT));
    RWRETURN((const RxRenderStateVector *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxRenderStateVectorCreate returns a RxRenderStateVector
 * set to the default or current state.
 *
 * The returned vector can be set to the current driver state or the
 * default render state depending on the value of the 'current' parameter.
 *
 * This function should not be called before RwEngineStart() has been called.
 *
 * \param current  if TRUE, the vector is set to the current state, else it is set to
 * the default state
 *
 * \return A pointer to the new \ref RxRenderStateVector on success or NULL
 *  if there is an error
 *
 * \see RxRenderStateVectorGetDefaultRenderStateVector
 * \see RxRenderStateVectorLoadDriverState
 * \see RxRenderStateVectorDestroy
 * \see RxRenderStateVectorSetDefaultRenderStateVector
 */
RxRenderStateVector *
RxRenderStateVectorCreate(RwBool current)
{
    RxRenderStateVector *NewVector;

    RWAPIFUNCTION(RWSTRING("RxRenderStateVectorCreate"));

    if (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED)
    {
        NewVector =
            (RxRenderStateVector *) RwMalloc(sizeof(RxRenderStateVector),
                                   rwMEMHINTDUR_EVENT | rwID_PIPEMODULE);

        if (NULL != NewVector)
        {
            if (current != FALSE)
            {
                if (!RxRenderStateVectorLoadDriverState(NewVector))
                {
                    RxRenderStateVectorDestroy(NewVector);
                }
            }
            else
            {
                *NewVector = RXPIPELINEGLOBAL(defaultRenderState);
            }
            RWRETURN(NewVector);
        }
        RWERROR((E_RW_NOMEM,sizeof(RxRenderStateVector)));
        RWRETURN((RxRenderStateVector *)NULL);
    }
    RWERROR((E_RW_PLUGINNOTINIT));
    RWRETURN((RxRenderStateVector *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxRenderStateVectorDestroy destroys a RxRenderStateVector
 *
 * \param rsvp  a pointer to the RxRenderStateVector to destroy
 *
 * \return None
 *
 * \see RxRenderStateVectorGetDefaultRenderStateVector
 * \see RxRenderStateVectorLoadDriverState
 * \see RxRenderStateVectorCreate
 * \see RxRenderStateVectorSetDefaultRenderStateVector
 */
void
RxRenderStateVectorDestroy(RxRenderStateVector * rsvp)
{
    RWAPIFUNCTION(RWSTRING("RxRenderStateVectorDestroy"));
    RWASSERT(rsvp != NULL);

    if (rsvp != NULL)
    {
        RwFree(rsvp);
        RWRETURNVOID();
    }
    RWERROR((E_RW_NULLP));
    RWRETURNVOID();
}

/**
 * \ingroup rwcoregeneric
 * \ref RxRenderStateVectorLoadDriverState sets a RxRenderStateVector
 * to the current driver render state.
 *
 * This function should not be called before RwEngineInit() has been called.
 *
 * \param rsvp  The RxRenderStateVector to set to the current driver state
 *
 * \return A pointer to the RxRenderStateVector on success, or NULL if there is an error
 *
 * \see RxRenderStateVectorGetDefaultRenderStateVector
 * \see RxRenderStateVectorCreate
 * \see RxRenderStateVectorDestroy
 * \see RxRenderStateVectorSetDefaultRenderStateVector
 */
RxRenderStateVector *
RxRenderStateVectorLoadDriverState(RxRenderStateVector * rsvp)
{
    RWAPIFUNCTION(RWSTRING("RxRenderStateVectorLoadDriverState"));
    RWASSERT(rsvp != NULL);

    if (rsvp != NULL)
    {
        RwBool              flag;

        rsvp->Flags = 0;       /* OR-ing into Flags is all very well, but not if
                                * you don't zero it at some stage! */
        RwRenderStateGet(rwRENDERSTATETEXTUREPERSPECTIVE, &flag);
        if (flag)
            rsvp->Flags |= rxRENDERSTATEFLAG_TEXTUREPERSPECTIVE;
        RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &flag);
        if (flag)
            rsvp->Flags |= rxRENDERSTATEFLAG_ZTESTENABLE;
        RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &flag);
        if (flag)
            rsvp->Flags |= rxRENDERSTATEFLAG_ZWRITEENABLE;
        RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &flag);
        if (flag)
            rsvp->Flags |= rxRENDERSTATEFLAG_VERTEXALPHAENABLE;

        RwRenderStateGet(rwRENDERSTATESHADEMODE,
                         (void *)&rsvp->ShadeMode);
        RwRenderStateGet(rwRENDERSTATESRCBLEND,
                         (void *)&rsvp->SrcBlend);
        RwRenderStateGet(rwRENDERSTATEDESTBLEND,
                         (void *)&rsvp->DestBlend);
        RwRenderStateGet(rwRENDERSTATETEXTURERASTER,
                         (void *)&rsvp->TextureRaster);
        if (RwRenderStateGet(rwRENDERSTATETEXTUREADDRESS,
                         (void *)&rsvp->AddressModeU))
        {
            rsvp->AddressModeV = rsvp->AddressModeU;
        }
        else
        {
            RwRenderStateGet(rwRENDERSTATETEXTUREADDRESSU,
                             (void *)&rsvp->AddressModeU);
            RwRenderStateGet(rwRENDERSTATETEXTUREADDRESSV,
                             (void *)&rsvp->AddressModeV);
        }
        RwRenderStateGet(rwRENDERSTATETEXTUREFILTER,
                         (void *)&rsvp->FilterMode);
        RwRenderStateGet(rwRENDERSTATEBORDERCOLOR,
                         (void *)&rsvp->BorderColor);
        RwRenderStateGet(rwRENDERSTATEFOGTYPE, (void *)&rsvp->FogType);
        RwRenderStateGet(rwRENDERSTATEFOGCOLOR,
                         (void *)&rsvp->FogColor);
        RWRETURN(rsvp);
    }
    RWERROR((E_RW_NULLP));
    RWRETURN((RxRenderStateVector *)NULL);
}

