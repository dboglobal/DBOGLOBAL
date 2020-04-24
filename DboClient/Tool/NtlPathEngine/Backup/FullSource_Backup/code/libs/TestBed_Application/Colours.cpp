//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "Colours.h"
#include <string.h>

void RGBForString(const char* colourtext, float* r, float* g, float* b)
{
//    if(_strcmpi(colourtext,"aliceblue")==0)
//    {
//        *r=0.941176f;
//        *g=0.972549f;
//        *b=1.0f;
//        return;
//    }
//    if(_strcmpi(colourtext,"antiquewhite")==0)
//    {
//        *r=0.980392f;
//        *g=0.921569f;
//        *b=0.843137f;
//        return;
//    }
    if(_strcmpi(colourtext,"aquamarine")==0)
    {
        *r=0.498039f;
        *g=1.0f;
        *b=0.831373f;
        return;
    }
    if(_strcmpi(colourtext,"azure")==0)
    {
        *r=0.941176f;
        *g=1.0f;
        *b=1.0f;
        return;
    }
    if(_strcmpi(colourtext,"beet")==0)
    {
        *r=0.556863f;
        *g=0.219608f;
        *b=0.556863f;
        return;
    }
    if(_strcmpi(colourtext,"beige")==0)
    {
        *r=0.960784f;
        *g=0.960784f;
        *b=0.862745f;
        return;
    }
//    if(_strcmpi(colourtext,"bisque")==0)
//    {
//        *r=1.0f;
//        *g=0.894118f;
//        *b=0.768627f;
//        return;
//    }
    if(_strcmpi(colourtext,"black")==0)
    {
        *r=0.0f;
        *g=0.0f;
        *b=0.0f;
        return;
    }
//    if(_strcmpi(colourtext,"blanchedalmond")==0)
//    {
//        *r=1.0f;
//        *g=0.921569f;
//        *b=0.803922f;
//        return;
//    }
    if(_strcmpi(colourtext,"blue")==0)
    {
        *r=0.0f;
        *g=0.0f;
        *b=1.0f;
        return;
    }
//    if(_strcmpi(colourtext,"blueviolet")==0)
//    {
//        *r=0.541176f;
//        *g=0.168627f;
//        *b=0.886275f;
//        return;
//    }
//    if(_strcmpi(colourtext,"brightgray")==0)
//    {
//        *r=0.772549f;
//        *g=0.756863f;
//        *b=0.666667f;
//        return;
//    }
//    if(_strcmpi(colourtext,"brightgrey")==0)
//    {
//        *r=0.772549f;
//        *g=0.756863f;
//        *b=0.666667f;
//        return;
//    }
    if(_strcmpi(colourtext,"brown")==0)
    {
        *r=0.647059f;
        *g=0.164706f;
        *b=0.164706f;
        return;
    }
//    if(_strcmpi(colourtext,"burlywood")==0)
//    {
//        *r=0.870588f;
//        *g=0.721569f;
//        *b=0.529412f;
//        return;
//    }
//    if(_strcmpi(colourtext,"cadetblue")==0)
//    {
//        *r=0.372549f;
//        *g=0.619608f;
//        *b=0.627451f;
//        return;
//    }
    if(_strcmpi(colourtext,"chartreuse")==0)
    {
        *r=0.443137f;
        *g=0.776471f;
        *b=0.443137f;
        return;
    }
//    if(_strcmpi(colourtext,"chocolate")==0)
//    {
//        *r=0.823529f;
//        *g=0.411765f;
//        *b=0.117647f;
//        return;
//    }
//    if(_strcmpi(colourtext,"coral")==0)
//    {
//        *r=1.0f;
//        *g=0.498039f;
//        *b=0.313725f;
//        return;
//    }
//    if(_strcmpi(colourtext,"cornflowerblue")==0)
//    {
//        *r=0.392157f;
//        *g=0.584314f;
//        *b=0.929412f;
//        return;
//    }
//    if(_strcmpi(colourtext,"cornsilk")==0)
//    {
//        *r=1.0f;
//        *g=0.972549f;
//        *b=0.862745f;
//        return;
//    }
    if(_strcmpi(colourtext,"crimson")==0)
    {
        *r=0.862745f;
        *g=0.0784314f;
        *b=0.235294f;
        return;
    }
    if(_strcmpi(colourtext,"cyan")==0)
    {
        *r=0.0f;
        *g=1.0f;
        *b=1.0f;
        return;
    }
    if(_strcmpi(colourtext,"darkblue")==0)
    {
        *r=0.0f;
        *g=0.0f;
        *b=0.545098f;
        return;
    }
    if(_strcmpi(colourtext,"darkcyan")==0)
    {
        *r=0.0f;
        *g=0.545098f;
        *b=0.545098f;
        return;
    }
//    if(_strcmpi(colourtext,"darkgoldenrod")==0)
//    {
//        *r=0.721569f;
//        *g=0.52549f;
//        *b=0.0431373f;
//        return;
//    }
    if(_strcmpi(colourtext,"darkgray")==0)
    {
        *r=0.333333f;
        *g=0.333333f;
        *b=0.333333f;
        return;
    }
    if(_strcmpi(colourtext,"darkgreen")==0)
    {
        *r=0.0f;
        *g=0.392157f;
        *b=0.0f;
        return;
    }
    if(_strcmpi(colourtext,"darkgrey")==0)
    {
        *r=0.333333f;
        *g=0.333333f;
        *b=0.333333f;
        return;
    }
//    if(_strcmpi(colourtext,"darkkhaki")==0)
//    {
//        *r=0.741176f;
//        *g=0.717647f;
//        *b=0.419608f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkmagenta")==0)
//    {
//        *r=0.545098f;
//        *g=0.0f;
//        *b=0.545098f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkolivegreen")==0)
//    {
//        *r=0.333333f;
//        *g=0.419608f;
//        *b=0.184314f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkorange")==0)
//    {
//        *r=1.0f;
//        *g=0.54902f;
//        *b=0.0f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkorchid")==0)
//    {
//        *r=0.6f;
//        *g=0.196078f;
//        *b=0.8f;
//        return;
//    }
    if(_strcmpi(colourtext,"darkred")==0)
    {
        *r=0.545098f;
        *g=0.0f;
        *b=0.0f;
        return;
    }
//    if(_strcmpi(colourtext,"darksalmon")==0)
//    {
//        *r=0.913725f;
//        *g=0.588235f;
//        *b=0.478431f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkseagreen")==0)
//    {
//        *r=0.560784f;
//        *g=0.737255f;
//        *b=0.560784f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkslateblue")==0)
//    {
//        *r=0.282353f;
//        *g=0.239216f;
//        *b=0.545098f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkslategray")==0)
//    {
//        *r=0.184314f;
//        *g=0.309804f;
//        *b=0.309804f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkslategrey")==0)
//    {
//        *r=0.184314f;
//        *g=0.309804f;
//        *b=0.309804f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkturquoise")==0)
//    {
//        *r=0.0f;
//        *g=0.807843f;
//        *b=0.819608f;
//        return;
//    }
//    if(_strcmpi(colourtext,"darkviolet")==0)
//    {
//        *r=0.580392f;
//        *g=0.0f;
//        *b=0.827451f;
//        return;
//    }
//    if(_strcmpi(colourtext,"deeppink")==0)
//    {
//        *r=1.0f;
//        *g=0.0784314f;
//        *b=0.576471f;
//        return;
//    }
//    if(_strcmpi(colourtext,"deepskyblue")==0)
//    {
//        *r=0.0f;
//        *g=0.74902f;
//        *b=1.0f;
//        return;
//    }
//    if(_strcmpi(colourtext,"dodgerblue")==0)
//    {
//        *r=0.117647f;
//        *g=0.564706f;
//        *b=1.0f;
//        return;
//    }
//    if(_strcmpi(colourtext,"firebrick")==0)
//    {
//        *r=0.698039f;
//        *g=0.133333f;
//        *b=0.133333f;
//        return;
//    }
//    if(_strcmpi(colourtext,"floralwhite")==0)
//    {
//        *r=1.0f;
//        *g=0.980392f;
//        *b=0.941176f;
//        return;
//    }
//    if(_strcmpi(colourtext,"forestgreen")==0)
//    {
//        *r=0.133333f;
//        *g=0.545098f;
//        *b=0.133333f;
//        return;
//    }
//    if(_strcmpi(colourtext,"gainsboro")==0)
//    {
//        *r=0.862745f;
//        *g=0.862745f;
//        *b=0.862745f;
//        return;
//    }
//    if(_strcmpi(colourtext,"ghostwhite")==0)
//    {
//        *r=0.972549f;
//        *g=0.972549f;
//        *b=1.0f;
//        return;
//    }
    if(_strcmpi(colourtext,"gold")==0)
    {
        *r=1.0f;
        *g=0.843137f;
        *b=0.0f;
        return;
    }
//    if(_strcmpi(colourtext,"goldenrod")==0)
//    {
//        *r=0.854902f;
//        *g=0.647059f;
//        *b=0.12549f;
//        return;
//    }
    if(_strcmpi(colourtext,"gray")==0)
    {
        *r=0.666667f;
        *g=0.666667f;
        *b=0.666667f;
        return;
    }
    if(_strcmpi(colourtext,"green")==0)
    {
        *r=0.0f;
        *g=1.0f;
        *b=0.0f;
        return;
    }
//    if(_strcmpi(colourtext,"greenyellow")==0)
//    {
//        *r=0.678431f;
//        *g=1.0f;
//        *b=0.184314f;
//        return;
//    }
    if(_strcmpi(colourtext,"grey")==0)
    {
        *r=0.666667f;
        *g=0.666667f;
        *b=0.666667f;
        return;
    }
//    if(_strcmpi(colourtext,"honeydew")==0)
//    {
//        *r=0.941176f;
//        *g=1.0f;
//        *b=0.941176f;
//        return;
//    }
//    if(_strcmpi(colourtext,"hotpink")==0)
//    {
//        *r=1.0f;
//        *g=0.411765f;
//        *b=0.705882f;
//        return;
//    }
//    if(_strcmpi(colourtext,"indianred")==0)
//    {
//        *r=0.803922f;
//        *g=0.360784f;
//        *b=0.360784f;
//        return;
//    }
    if(_strcmpi(colourtext,"indigo")==0)
    {
        *r=0.294118f;
        *g=0.0f;
        *b=0.509804f;
        return;
    }
    if(_strcmpi(colourtext,"ivory")==0)
    {
        *r=1.0f;
        *g=1.0f;
        *b=0.941176f;
        return;
    }
    if(_strcmpi(colourtext,"khaki")==0)
    {
        *r=0.941176f;
        *g=0.901961f;
        *b=0.54902f;
        return;
    }
    if(_strcmpi(colourtext,"lavender")==0)
    {
        *r=0.901961f;
        *g=0.901961f;
        *b=0.980392f;
        return;
    }
//    if(_strcmpi(colourtext,"lavenderblush")==0)
//    {
//        *r=1.0f;
//        *g=0.941176f;
//        *b=0.960784f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lawngreen")==0)
//    {
//        *r=0.486275f;
//        *g=0.988235f;
//        *b=0.0f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lemonchiffon")==0)
//    {
//        *r=1.0f;
//        *g=0.980392f;
//        *b=0.803922f;
//        return;
//    }
    if(_strcmpi(colourtext,"lightblue")==0)
    {
        *r=0.490196f;
        *g=0.619608f;
        *b=0.752941f;
        return;
    }
//    if(_strcmpi(colourtext,"lightcoral")==0)
//    {
//        *r=0.941176f;
//        *g=0.501961f;
//        *b=0.501961f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightcyan")==0)
//    {
//        *r=0.878431f;
//        *g=1.0f;
//        *b=1.0f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightgoldenrod")==0)
//    {
//        *r=0.933333f;
//        *g=0.866667f;
//        *b=0.509804f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightgoldenrodyellow")==0)
//    {
//        *r=0.980392f;
//        *g=0.980392f;
//        *b=0.823529f;
//        return;
//    }
    if(_strcmpi(colourtext,"lightgray")==0)
    {
        *r=0.745098f;
        *g=0.745098f;
        *b=0.745098f;
        return;
    }
    if(_strcmpi(colourtext,"lightgreen")==0)
    {
        *r=0.564706f;
        *g=0.933333f;
        *b=0.564706f;
        return;
    }
    if(_strcmpi(colourtext,"lightgrey")==0)
    {
        *r=0.745098f;
        *g=0.745098f;
        *b=0.745098f;
        return;
    }
//    if(_strcmpi(colourtext,"lightpink")==0)
//    {
//        *r=1.0f;
//        *g=0.713725f;
//        *b=0.756863f;
//        return;
//    }
    if(_strcmpi(colourtext,"lightred")==0)
    {
        *r=1.0f;
        *g=1.6f;
        *b=1.6f;
        return;
    }
//    if(_strcmpi(colourtext,"lightsalmon")==0)
//    {
//        *r=1.0f;
//        *g=0.627451f;
//        *b=0.478431f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightseagreen")==0)
//    {
//        *r=0.12549f;
//        *g=0.698039f;
//        *b=0.666667f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightskyblue")==0)
//    {
//        *r=0.529412f;
//        *g=0.807843f;
//        *b=0.980392f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightslateblue")==0)
//    {
//        *r=0.517647f;
//        *g=0.439216f;
//        *b=1.0f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightslategray")==0)
//    {
//        *r=0.466667f;
//        *g=0.533333f;
//        *b=0.6f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightslategrey")==0)
//    {
//        *r=0.466667f;
//        *g=0.533333f;
//        *b=0.6f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightsteelblue")==0)
//    {
//        *r=0.690196f;
//        *g=0.768627f;
//        *b=0.870588f;
//        return;
//    }
//    if(_strcmpi(colourtext,"lightyellow")==0)
//    {
//        *r=1.0f;
//        *g=1.0f;
//        *b=0.878431f;
//        return;
//    }
//    if(_strcmpi(colourtext,"limegreen")==0)
//    {
//        *r=0.196078f;
//        *g=0.803922f;
//        *b=0.196078f;
//        return;
//    }
//    if(_strcmpi(colourtext,"linen")==0)
//    {
//        *r=0.980392f;
//        *g=0.941176f;
//        *b=0.901961f;
//        return;
//    }
    if(_strcmpi(colourtext,"magenta")==0)
    {
        *r=1.0f;
        *g=0.0f;
        *b=1.0f;
        return;
    }
    if(_strcmpi(colourtext,"maroon")==0)
    {
        *r=0.690196f;
        *g=0.188235f;
        *b=0.376471f;
        return;
    }
//    if(_strcmpi(colourtext,"mediumaquamarine")==0)
//    {
//        *r=0.4f;
//        *g=0.803922f;
//        *b=0.666667f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumblue")==0)
//    {
//        *r=0.0f;
//        *g=0.0f;
//        *b=0.803922f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumgray")==0)
//    {
//        *r=0.517647f;
//        *g=0.517647f;
//        *b=0.517647f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumgrey")==0)
//    {
//        *r=0.517647f;
//        *g=0.517647f;
//        *b=0.517647f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumorchid")==0)
//    {
//        *r=0.729412f;
//        *g=0.333333f;
//        *b=0.827451f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumpurple")==0)
//    {
//        *r=0.576471f;
//        *g=0.439216f;
//        *b=0.858824f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumseagreen")==0)
//    {
//        *r=0.235294f;
//        *g=0.701961f;
//        *b=0.443137f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumslateblue")==0)
//    {
//        *r=0.482353f;
//        *g=0.407843f;
//        *b=0.933333f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumspringgreen")==0)
//    {
//        *r=0.0f;
//        *g=0.980392f;
//        *b=0.603922f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumturquoise")==0)
//    {
//        *r=0.282353f;
//        *g=0.819608f;
//        *b=0.8f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mediumvioletred")==0)
//    {
//        *r=0.780392f;
//        *g=0.0823529f;
//        *b=0.521569f;
//        return;
//    }
//    if(_strcmpi(colourtext,"midnightblue")==0)
//    {
//        *r=0.0980392f;
//        *g=0.0980392f;
//        *b=0.439216f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mintcream")==0)
//    {
//        *r=0.960784f;
//        *g=1.0f;
//        *b=0.980392f;
//        return;
//    }
//    if(_strcmpi(colourtext,"mistyrose")==0)
//    {
//        *r=1.0f;
//        *g=0.894118f;
//        *b=0.882353f;
//        return;
//    }
//    if(_strcmpi(colourtext,"moccasin")==0)
//    {
//        *r=1.0f;
//        *g=0.894118f;
//        *b=0.709804f;
//        return;
//    }
//    if(_strcmpi(colourtext,"navajowhite")==0)
//    {
//        *r=1.0f;
//        *g=0.870588f;
//        *b=0.678431f;
//        return;
//    }
    if(_strcmpi(colourtext,"navy")==0)
    {
        *r=0.0f;
        *g=0.0f;
        *b=0.501961f;
        return;
    }
//    if(_strcmpi(colourtext,"navyblue")==0)
//    {
//        *r=0.0f;
//        *g=0.0f;
//        *b=0.501961f;
//        return;
//    }
//    if(_strcmpi(colourtext,"oldlace")==0)
//    {
//        *r=0.992157f;
//        *g=0.960784f;
//        *b=0.901961f;
//        return;
//    }
//    if(_strcmpi(colourtext,"olivedrab")==0)
//    {
//        *r=0.556863f;
//        *g=0.556863f;
//        *b=0.219608f;
//        return;
//    }
    if(_strcmpi(colourtext,"orange")==0)
    {
        *r=1.0f;
        *g=0.647059f;
        *b=0.0f;
        return;
    }
//    if(_strcmpi(colourtext,"orangered")==0)
//    {
//        *r=1.0f;
//        *g=0.270588f;
//        *b=0.0f;
//        return;
//    }
//    if(_strcmpi(colourtext,"orchid")==0)
//    {
//        *r=0.854902f;
//        *g=0.439216f;
//        *b=0.839216f;
//        return;
//    }
//    if(_strcmpi(colourtext,"palegoldenrod")==0)
//    {
//        *r=0.933333f;
//        *g=0.909804f;
//        *b=0.666667f;
//        return;
//    }
//    if(_strcmpi(colourtext,"palegreen")==0)
//    {
//        *r=0.596078f;
//        *g=0.984314f;
//        *b=0.596078f;
//        return;
//    }
//    if(_strcmpi(colourtext,"paleturquoise")==0)
//    {
//        *r=0.686275f;
//        *g=0.933333f;
//        *b=0.933333f;
//        return;
//    }
//    if(_strcmpi(colourtext,"palevioletred")==0)
//    {
//        *r=0.858824f;
//        *g=0.439216f;
//        *b=0.576471f;
//        return;
//    }
//    if(_strcmpi(colourtext,"papayawhip")==0)
//    {
//        *r=1.0f;
//        *g=0.937255f;
//        *b=0.835294f;
//        return;
//    }
//    if(_strcmpi(colourtext,"peachpuff")==0)
//    {
//        *r=1.0f;
//        *g=0.854902f;
//        *b=0.72549f;
//        return;
//    }
//    if(_strcmpi(colourtext,"peru")==0)
//    {
//        *r=0.803922f;
//        *g=0.521569f;
//        *b=0.247059f;
//        return;
//    }
    if(_strcmpi(colourtext,"pink")==0)
    {
        *r=1.0f;
        *g=0.752941f;
        *b=0.796078f;
        return;
    }
    if(_strcmpi(colourtext,"plum")==0)
    {
        *r=0.866667f;
        *g=0.627451f;
        *b=0.866667f;
        return;
    }
//    if(_strcmpi(colourtext,"powderblue")==0)
//    {
//        *r=0.690196f;
//        *g=0.878431f;
//        *b=0.901961f;
//        return;
//    }
    if(_strcmpi(colourtext,"purple")==0)
    {
        *r=0.627451f;
        *g=0.12549f;
        *b=0.941176f;
        return;
    }
    if(_strcmpi(colourtext,"red")==0)
    {
        *r=1.0f;
        *g=0.0f;
        *b=0.0f;
        return;
    }
//    if(_strcmpi(colourtext,"rosybrown")==0)
//    {
//        *r=0.737255f;
//        *g=0.560784f;
//        *b=0.560784f;
//        return;
//    }
//    if(_strcmpi(colourtext,"royalblue")==0)
//    {
//        *r=0.254902f;
//        *g=0.411765f;
//        *b=0.882353f;
//        return;
//    }
//    if(_strcmpi(colourtext,"saddlebrown")==0)
//    {
//        *r=0.545098f;
//        *g=0.270588f;
//        *b=0.0745098f;
//        return;
//    }
    if(_strcmpi(colourtext,"salmon")==0)
    {
        *r=0.980392f;
        *g=0.501961f;
        *b=0.447059f;
        return;
    }
//    if(_strcmpi(colourtext,"sandybrown")==0)
//    {
//        *r=0.956863f;
//        *g=0.643137f;
//        *b=0.376471f;
//        return;
//    }
//    if(_strcmpi(colourtext,"seagreen")==0)
//    {
//        *r=0.180392f;
//        *g=0.545098f;
//        *b=0.341176f;
//        return;
//    }
//    if(_strcmpi(colourtext,"seashell")==0)
//    {
//        *r=1.0f;
//        *g=0.960784f;
//        *b=0.933333f;
//        return;
//    }
    if(_strcmpi(colourtext,"sienna")==0)
    {
        *r=0.627451f;
        *g=0.321569f;
        *b=0.176471f;
        return;
    }
//    if(_strcmpi(colourtext,"skyblue")==0)
//    {
//        *r=0.529412f;
//        *g=0.807843f;
//        *b=0.921569f;
//        return;
//    }
//    if(_strcmpi(colourtext,"slateblue")==0)
//    {
//        *r=0.443137f;
//        *g=0.443137f;
//        *b=0.776471f;
//        return;
//    }
//    if(_strcmpi(colourtext,"slategray")==0)
//    {
//        *r=0.439216f;
//        *g=0.501961f;
//        *b=0.564706f;
//        return;
//    }
//    if(_strcmpi(colourtext,"slategrey")==0)
//    {
//        *r=0.439216f;
//        *g=0.501961f;
//        *b=0.564706f;
//        return;
//    }
//    if(_strcmpi(colourtext,"snow")==0)
//    {
//        *r=1.0f;
//        *g=0.980392f;
//        *b=0.980392f;
//        return;
//    }
//    if(_strcmpi(colourtext,"springgreen")==0)
//    {
//        *r=0.0f;
//        *g=1.0f;
//        *b=0.498039f;
//        return;
//    }
//    if(_strcmpi(colourtext,"steelblue")==0)
//    {
//        *r=0.27451f;
//        *g=0.509804f;
//        *b=0.705882f;
//        return;
//    }
    if(_strcmpi(colourtext,"tan")==0)
    {
        *r=0.823529f;
        *g=0.705882f;
        *b=0.54902f;
        return;
    }
    if(_strcmpi(colourtext,"teal")==0)
    {
        *r=0.219608f;
        *g=0.556863f;
        *b=0.556863f;
        return;
    }
//    if(_strcmpi(colourtext,"thistle")==0)
//    {
//        *r=0.847059f;
//        *g=0.74902f;
//        *b=0.847059f;
//        return;
//    }
//    if(_strcmpi(colourtext,"tomato")==0)
//    {
//        *r=1.0f;
//        *g=0.388235f;
//        *b=0.278431f;
//        return;
//    }
    if(_strcmpi(colourtext,"turquoise")==0)
    {
        *r=0.25098f;
        *g=0.878431f;
        *b=0.815686f;
        return;
    }
//    if(_strcmpi(colourtext,"verydarkgray")==0)
//    {
//        *r=0.156863f;
//        *g=0.156863f;
//        *b=0.156863f;
//        return;
//    }
//    if(_strcmpi(colourtext,"verydarkgrey")==0)
//    {
//        *r=0.156863f;
//        *g=0.156863f;
//        *b=0.156863f;
//        return;
//    }
//    if(_strcmpi(colourtext,"verylightgray")==0)
//    {
//        *r=0.839216f;
//        *g=0.839216f;
//        *b=0.839216f;
//        return;
//    }
//    if(_strcmpi(colourtext,"verylightgrey")==0)
//    {
//        *r=0.839216f;
//        *g=0.839216f;
//        *b=0.839216f;
//        return;
//    }
    if(_strcmpi(colourtext,"violet")==0)
    {
        *r=0.933333f;
        *g=0.509804f;
        *b=0.933333f;
        return;
    }
//    if(_strcmpi(colourtext,"violetred")==0)
//    {
//        *r=0.815686f;
//        *g=0.12549f;
//        *b=0.564706f;
//        return;
//    }
//    if(_strcmpi(colourtext,"wheat")==0)
//    {
//        *r=0.960784f;
//        *g=0.870588f;
//        *b=0.701961f;
//        return;
//    }
    if(_strcmpi(colourtext,"white")==0)
    {
        *r=1.0f;
        *g=1.0f;
        *b=1.0f;
        return;
    }
//    if(_strcmpi(colourtext,"whitesmoke")==0)
//    {
//        *r=0.960784f;
//        *g=0.960784f;
//        *b=0.960784f;
//        return;
//    }
    if(_strcmpi(colourtext,"yellow")==0)
    {
        *r=1.0f;
        *g=1.0f;
        *b=0.0f;
        return;
    }
//    if(_strcmpi(colourtext,"yellowgreen")==0)
//    {
//        *r=0.603922f;
//        *g=0.803922f;
//        *b=0.196078f;
//        return;
//    }

//================
// default colour

//    if(_strcmpi(colourtext,"hotpink")==0)
    {
        *r=1.0f;
        *g=0.411765f;
        *b=0.705882f;
        return;
    }
}