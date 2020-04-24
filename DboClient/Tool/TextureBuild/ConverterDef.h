#pragma once

#include <windows.h>
#include <stdlib.h>

#define RELEASE(p) if (p) p->Release(); p = NULL;