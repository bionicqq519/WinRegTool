#pragma once
typedef enum {
    alPP_DEBUG_FLAG_DISABLE = 0x00000000,
    alPP_DEBUG_FLAG_BYPASS_MODE = 0x00000001, /*Bypass all API processing*/
    alPP_DEBUG_FLAG_ENABLE_WATERMARK = 0x00000002, /*Injection watermark in output image*/
    alPP_DEBUG_FLAG_LOG_DEBUG = 0x00000004, /*Print all input parameters and API name*/
    alPP_DEBUG_FLAG_LOG_PROFILE = 0x00000008, /*Print all API processing time*/
    alPP_DEBUG_FLAG_DUMP_DATA = 0x00000010, /*Dump input image, tuning data, output image*/
    alPP_DEBUG_FLAG_ENABLE_MASK = 0xFFFFFFFF
} alPP_DEBUG_FLAG;

