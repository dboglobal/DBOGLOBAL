//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "ExecuteMaxScript.h"

#include <Max.h>
#include <utilapi.h>
#include <Maxscrpt/maxscrpt.h>
#include <Maxscrpt/parser.h>

bool ExecuteMaxScript(Interface* ip, char* command)
{
    bool result=TRUE;
    init_thread_locals();
    four_typed_value_locals(Parser* parser,Value* code,Value* result,StringStream* source);

    vl.parser = new Parser (thread_local(current_stdout));
    vl.source = new StringStream (command);
    vl.source->log_to(thread_local(current_stdout));

    save_current_frames();

    try
    {
        ip->RedrawViews(ip->GetTime(),REDRAW_BEGIN);

        vl.source->flush_whitespace();

        vl.code = vl.parser->compile_all(vl.source);
        vl.result = vl.code->eval();
        vl.result->print();

        vl.source->flush_whitespace();
        vl.source->close();

        ip->RedrawViews(ip->GetTime(),REDRAW_END);
    }
    catch (...)
    {
        restore_current_frames();
        result=FALSE;
        vl.source->close();
    }

    pop_value_locals();

    return result;
}
