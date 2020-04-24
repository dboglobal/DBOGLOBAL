#ifndef PROGRESS_BAR_INCLUDED
#define PROGRESS_BAR_INCLUDED

#include "platform_common/Timer.h"
#include "i_pathengine.h"

class iGraphicsInterface;
void DrawProgressBar(iGraphicsInterface *gip, const char* description, float percent, tSigned32 baseheight=10);

class cGIPProgressCallBack : public iProgressCallBack
{
    cTimerValue _lastUpdate;
    const char* _lastDescription;

    bool enoughTimePassed() const
    {
        cTimerValue expired;
        expired.read();
        expired -= _lastUpdate;
        return expired.asSeconds() > 0.2;
    }

public:

    cGIPProgressCallBack()
    {
        _lastUpdate.read();
        _lastDescription = 0;
    }
    void updateProgress(const char* description, float percent);
};

#endif
