#include<stdio.h>
#include"debug.h"

int main(){
    int debug_level;
    printf("Please enter the debug level:");
    scanf("%d",&debug_level);
    if(DEBUG_LEVEL_ERROR <= debug_level){
        debug_printf(DEBUG_LEVEL_ERROR,"This is level 1.\n");
    }
    if(DEBUG_LEVEL_INFO <= debug_level){
        debug_printf(DEBUG_LEVEL_INFO,"This is level 2.\n");
    }
    if(DEBUG_LEVEL_VERBOSE <= debug_level){
        debug_printf(DEBUG_LEVEL_VERBOSE,"This is level 3.\n");
    }
    if(DEBUG_LEVEL_VVERBOSE <= debug_level){
        debug_printf(DEBUG_LEVEL_VVERBOSE,"This is level 4.\n");
    }
}