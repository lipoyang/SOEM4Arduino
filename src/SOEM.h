#include "soem/ethercat.h"

// this is for printf() compatibility
#ifndef ESP32
#define printf(...) debug_print(__VA_ARGS__) 
#endif
