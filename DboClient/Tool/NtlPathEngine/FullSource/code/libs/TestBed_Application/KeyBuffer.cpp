
#include "KeyBuffer.h"
#include "KeyCodes.h"

namespace nKeyBuffer
{

const size_t KEYBUF_SIZE = 100;

short keybuf[KEYBUF_SIZE];
int keybuf_isup[KEYBUF_SIZE];
int keybuf_top;

void
clear()
{
    keybuf_top = 0;
}

void
keyPressed(short keyCode)
{
    if(keybuf_top < KEYBUF_SIZE)
    {
        keybuf[keybuf_top] = keyCode;
        keybuf_isup[keybuf_top] = 0;
        keybuf_top++;
    }
}
void
keyReleased(short keyCode)
{
    if(keybuf_top < KEYBUF_SIZE)
    {
        keybuf[keybuf_top] = keyCode;
        keybuf_isup[keybuf_top] = 1;
        keybuf_top++;
    }
}

char*
getKeyMessage()
{
    char* buffer;

    while(keybuf_top)
    {
        keybuf_top--;

        buffer = StringForKeyCode(keybuf[keybuf_top]);

        if(buffer)
        {
            if(keybuf_isup[keybuf_top])
            {
                buffer[0] = 'u';
            }
            else
            {
                buffer[0] = 'd';
            }
            return buffer;
        }
    }
    return 0;
}

}
