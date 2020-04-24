#pragma once

#include <iostream>

const char* NtlGetPacketName(WORD wOpCode);

bool NtlCheckPacketName(FILE * fp = NULL);