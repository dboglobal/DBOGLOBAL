#include "stdafx.h"

#include "sliding.h"

enum eSide
{
    SIDE_CENTRE,
    SIDE_LEFT,
    SIDE_RIGHT
};

inline eSide SideOfLine(const long *line, long x, long y)
{
    long axisx=line[2]-line[0];
    long axisy=line[3]-line[1];
    x-=line[0];
    y-=line[1];
    long lhs,rhs;
    lhs=x*axisy;
    rhs=y*axisx;
    if(lhs==rhs)
        return SIDE_CENTRE;
    if(lhs<rhs)
        return SIDE_LEFT;
    return SIDE_RIGHT;    
}

inline void PushToLeftOfLine(const long *line, long &x, long &y)
{
    long axisx=line[2]-line[0];
    long axisy=line[3]-line[1];
    long absolute_x,absolute_y;

    absolute_x=axisx;
    if(absolute_x<0)
        absolute_x=-axisx;
    absolute_y=axisy;
    if(absolute_y<0)
        absolute_y=-axisy;
    
    // force rounding in axis with smaller component
    if(absolute_y>absolute_x)
    {
        if(axisy>0)
            x--;
        else
            x++;
    }
    else
    {
        if(axisx<0)
            y--;
        else
            y++;
    }
}

void SlideAgainst(const long* collidingline, long currentx, long currenty, long& dx, long& dy)
{
    float dotproduct = static_cast<float>(dx * (collidingline[2] - collidingline[0]) + dy * (collidingline[3] - collidingline[1]));
    float ratio = dotproduct;
    long axisx=collidingline[2]-collidingline[0];
    long axisy=collidingline[3]-collidingline[1];
    float axis_lengthsquared = static_cast<float>(axisx * axisx + axisy * axisy);
    ratio/=axis_lengthsquared;
    dx = static_cast<long>(static_cast<float>(axisx) * ratio);
    dy = static_cast<long>(static_cast<float>(axisy) * ratio);
    long targetx,targety;
    targetx=currentx+dx;
    targety=currenty+dy;
    if(SideOfLine(collidingline,targetx,targety)==SIDE_RIGHT)
    {
        PushToLeftOfLine(collidingline,targetx,targety);
        dx=targetx-currentx;
        dy=targety-currenty;
    }
}

void SlideAgainst(const long* collidingline, long currentX, long currentY, double& dx, double& dy)
{
    double dotproduct=dx*(collidingline[2]-collidingline[0])+dy*(collidingline[3]-collidingline[1]);
    double ratio=dotproduct;
    long axisx=collidingline[2]-collidingline[0];
    long axisy=collidingline[3]-collidingline[1];
    double axis_lengthsquared=axisx*axisx+axisy*axisy;
    ratio /= axis_lengthsquared;
    dx=axisx*ratio;
    dy=axisy*ratio;
    long targetX, targetY;
    targetX = static_cast<long>(currentX + dx);
    targetY = static_cast<long>(currentY + dy);
    if(SideOfLine(collidingline, targetX, targetY) == SIDE_RIGHT)
    {
        long pushedTargetX = targetX;
        long pushedTargetY = targetY;
        PushToLeftOfLine(collidingline, pushedTargetX, pushedTargetY);
        dx += pushedTargetX - targetX;
        dy += pushedTargetY - targetY;
    }
}
