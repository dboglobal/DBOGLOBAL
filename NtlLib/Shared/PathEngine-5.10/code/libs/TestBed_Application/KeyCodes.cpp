//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "KeyCodes.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int
CodeForKeyString(const char* keyString)
{
    if(keyString[0]=='_')
    {
        if (strcmp(keyString+1,"LMOUSE")==0)
            return VK_LBUTTON;
        if (strcmp(keyString+1,"MMOUSE")==0)
            return VK_MBUTTON;
        if (strcmp(keyString+1,"RMOUSE")==0)
            return VK_RBUTTON;
        if (strcmp(keyString+1,"PLUS")==0)
            return VK_ADD;
        if (strcmp(keyString+1,"MINUS")==0)
            return VK_SUBTRACT;
        if (strcmp(keyString+1,"F1")==0)
            return VK_F1;
        if (strcmp(keyString+1,"F2")==0)
            return VK_F2;
        if (strcmp(keyString+1,"F3")==0)
            return VK_F3;
        if (strcmp(keyString+1,"F4")==0)
            return VK_F4;
        if (strcmp(keyString+1,"F5")==0)
            return VK_F5;
        if (strcmp(keyString+1,"F6")==0)
            return VK_F6;
        if (strcmp(keyString+1,"F7")==0)
            return VK_F7;
        if (strcmp(keyString+1,"F8")==0)
            return VK_F8;
        if (strcmp(keyString+1,"F9")==0)
            return VK_F9;
        if (strcmp(keyString+1,"F10")==0)
            return VK_F10;
        if (strcmp(keyString+1,"F11")==0)
            return VK_F11;
        if (strcmp(keyString+1,"F12")==0)
            return VK_F12;
        if (strcmp(keyString+1,"UP")==0)
            return VK_UP;
        if (strcmp(keyString+1,"DOWN")==0)
            return VK_DOWN;
        if (strcmp(keyString+1,"LEFT")==0)
            return VK_LEFT;
        if (strcmp(keyString+1,"RIGHT")==0)
            return VK_RIGHT;
        if (strcmp(keyString+1,"RCONTROL")==0)
//            return VK_RCONTROL;
            return VK_CONTROL;
        if (strcmp(keyString+1,"LCONTROL")==0)
//            return VK_LCONTROL;
            return VK_CONTROL;
        if (strcmp(keyString+1,"CONTROL")==0)
            return VK_CONTROL;
        if (strcmp(keyString+1,"RSHIFT")==0)
            return VK_RSHIFT;
        if (strcmp(keyString+1,"LSHIFT")==0)
            return VK_LSHIFT;
        if (strcmp(keyString+1,"BACKSPACE")==0)
            return VK_BACK;
        if (strcmp(keyString+1,"SPACE")==0)
            return VK_SPACE;
        if (strcmp(keyString+1,"RETURN")==0)
            return VK_RETURN;
        if (strcmp(keyString+1,"DELETE")==0)
            return VK_DELETE;
        if (strcmp(keyString+1,"PAGEDOWN")==0)
            return VK_NEXT;
        if (strcmp(keyString+1,"PAGEUP")==0)
            return VK_PRIOR;
        if (strcmp(keyString+1,"END")==0)
            return VK_END;
        if (strcmp(keyString+1,"HOME")==0)
            return VK_HOME;
        if (strcmp(keyString+1,"INSERT")==0)
            return VK_INSERT;
        if (strcmp(keyString+1,"ESCAPE")==0)
            return VK_ESCAPE;
    }

    if(keyString[1])
        return 0;

    if((keyString[0]>='0' && keyString[0]<='9') ||
        (keyString[0]>='A' && keyString[0]<='Z') ||
        keyString[0]==' ')
        return keyString[0];

    if(keyString[0]=='+')
        return VK_ADD;
    if(keyString[0]=='-')
        return VK_SUBTRACT;
    if(keyString[0]=='.')
        return 0xbe;

    return 0;
}


char*
StringForKeyCode(int code)
{
    static char buffer[50];

    buffer[0]='_';

    if(code == VK_LBUTTON)
    {
        strcpy(buffer+1,"_LMOUSE");
        return buffer;
    }
    if(code == VK_MBUTTON)
    {
        strcpy(buffer+1,"_MMOUSE");
        return buffer;
    }
    if(code == VK_RBUTTON)
    {
        strcpy(buffer+1,"_RMOUSE");
        return buffer;
    }
    if(code==VK_ADD)
    {
        strcpy(buffer+1,"_PLUS");
        return buffer;
    }
    if(code==VK_SUBTRACT)
    {
        strcpy(buffer+1,"_MINUS");
        return buffer;
    }
    if(code==VK_F1)
    {
        strcpy(buffer+1,"_F1");
        return buffer;
    }
    if(code==VK_F2)
    {
        strcpy(buffer+1,"_F2");
        return buffer;
    }
    if(code==VK_F3)
    {
        strcpy(buffer+1,"_F3");
        return buffer;
    }
    if(code==VK_F4)
    {
        strcpy(buffer+1,"_F4");
        return buffer;
    }
    if(code==VK_F5)
    {
        strcpy(buffer+1,"_F5");
        return buffer;
    }
    if(code==VK_F6)
    {
        strcpy(buffer+1,"_F6");
        return buffer;
    }
    if(code==VK_F7)
    {
        strcpy(buffer+1,"_F7");
        return buffer;
    }
    if(code==VK_F8)
    {
        strcpy(buffer+1,"_F8");
        return buffer;
    }
    if(code==VK_F9)
    {
        strcpy(buffer+1,"_F9");
        return buffer;
    }
    if(code==VK_F10)
    {
        strcpy(buffer+1,"_F10");
        return buffer;
    }
    if(code==VK_F11)
    {
        strcpy(buffer+1,"_F11");
        return buffer;
    }
    if(code==VK_F12)
    {
        strcpy(buffer+1,"_F12");
        return buffer;
    }
    if(code==VK_UP)
    {
        strcpy(buffer+1,"_UP");
        return buffer;
    }
    if(code==VK_DOWN)
    {
        strcpy(buffer+1,"_DOWN");
        return buffer;
    }
    if(code==VK_LEFT)
    {
        strcpy(buffer+1,"_LEFT");
        return buffer;
    }
    if(code==VK_RIGHT)
    {
        strcpy(buffer+1,"_RIGHT");
        return buffer;
    }
    if(code==VK_RCONTROL)
    {
        strcpy(buffer+1,"_CONTROL");
        return buffer;
    }
    if(code==VK_LCONTROL)
    {
        strcpy(buffer+1,"_CONTROL");
        return buffer;
    }
    if(code==VK_RSHIFT)
    {
        strcpy(buffer+1,"_RSHIFT");
        return buffer;
    }
    if(code==VK_LSHIFT)
    {
        strcpy(buffer+1,"_LSHIFT");
        return buffer;
    }
    if(code==VK_BACK)
    {
        strcpy(buffer+1,"_BACKSPACE");
        return buffer;
    }
    if(code==VK_SPACE)
    {
        strcpy(buffer+1,"_SPACE");
        return buffer;
    }
    if(code==VK_RETURN)
    {
        strcpy(buffer+1,"_RETURN");
        return buffer;
    }
    if(code==VK_DELETE)
    {
        strcpy(buffer+1,"_DELETE");
        return buffer;
    }
    if(code==VK_PRIOR)
    {
        strcpy(buffer+1,"_PAGEUP");
        return buffer;
    }
    if(code==VK_NEXT)
    {
        strcpy(buffer+1,"_PAGEDOWN");
        return buffer;
    }
    if(code==VK_END)
    {
        strcpy(buffer+1,"_END");
        return buffer;
    }
    if(code==VK_HOME)
    {
        strcpy(buffer+1,"_HOME");
        return buffer;
    }
    if(code==VK_INSERT)
    {
        strcpy(buffer+1,"_INSERT");
        return buffer;
    }
    if(code==VK_ESCAPE)
    {
        strcpy(buffer+1,"_ESCAPE");
        return buffer;
    }

    if((code>='0' && code<='9') ||
        (code>='A' && code<='Z') ||
        code==' '
        )
    {
        buffer[1]=(char)code;
        buffer[2]=0;
        return buffer;
    }
    if(code==VK_ADD)
    {
        buffer[1]='+';
        buffer[2]=0;
        return buffer;
    }
    if(code==VK_SUBTRACT)
    {
        buffer[1]='-';
        buffer[2]=0;
        return buffer;
    }
    if(code==0xbe)
    {
        buffer[1]='.';
        buffer[2]=0;
        return buffer;
    }
    return 0;
}

