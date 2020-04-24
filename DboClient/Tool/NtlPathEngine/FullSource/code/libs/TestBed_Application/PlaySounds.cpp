//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/PlaySounds.h"
#include "PlaySounds_Init.h"
#include "common/interface/Error.h"
#include "dsutil.h"
#include <dsound.h>
#include <vector>

CSoundManager* gSoundManager;
std::vector<CSound*> gSounds;

void
InitDSound(HWND hwnd)
{
    HRESULT hr;

    // Create a static IDirectSound in the CSound class.  
    // Set coop level to DSSCL_PRIORITY, and set primary buffer 
    // format to stereo, 22kHz and 16-bit output.
    gSoundManager = new CSoundManager();
    if(gSoundManager == 0)
    {
        return;
    }

    if(FAILED(hr = gSoundManager->Initialize(hwnd, DSSCL_PRIORITY)))
    {
        ShutDownDSound();
        return;
    }
    
    if(FAILED(hr = gSoundManager->SetPrimaryBufferFormat(2, 22050, 16)))
    {
        ShutDownDSound();
        return;
    }
}

void
ShutDownDSound()
{
    size_t i;
    for(i = 0; i < gSounds.size(); i++)
    {
        delete gSounds[i];
    }
    delete gSoundManager;
}

tSigned32
CreateSound(const char* fileName, tSigned32 numberOfBuffers)
{
    HRESULT hr;

    CSound* newSound = 0;

    // Load the wave file into a DirectSound buffer
    if(FAILED(hr = gSoundManager->Create(&newSound, const_cast<char*>(fileName), 0, GUID_NULL, numberOfBuffers)))
    {
        return -1;
    }

    tSigned32 resultIndex = SizeL(gSounds);
    gSounds.push_back(newSound);
    return resultIndex;
}
void
PlaySoundZ(tSigned32 index)
{
    gSounds[index]->Play();
}
