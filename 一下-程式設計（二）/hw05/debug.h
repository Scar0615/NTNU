#include<stdio.h>
#include<stdarg.h>

#define DEBUG_LEVEL_NONE 0
#define DEBUG_LEVEL_ERROR 1
#define DEBUG_LEVEL_INFO 2
#define DEBUG_LEVEL_VERBOSE 3
#define DEBUG_LEVEL_VVERBOSE 4

#define debug_printf(level,fmt) printf(fmt)
