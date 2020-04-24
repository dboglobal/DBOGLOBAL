#ifndef __NTL_CORE_API__
#define __NTL_CORE_API__

RwBool API_CoreInit(void);
void API_CoreTerminate(void);

void API_CoreUpdate(RwReal fElapsed);
void API_CoreToggleFullScreenNotify(bool bFullScreen);

#endif
