#ifndef _H_PLUGIN
#define _H_PLUGIN

#include <vector>

#include "SDK\amx\amx.h"
#include "SDK\plugincommon.h"
#include "natives.h"

typedef void 
    (*logprintf_t)(char* format, ...);

extern void 
    *pAMXFunctions;

#endif