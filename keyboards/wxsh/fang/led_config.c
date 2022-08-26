#include "fang.h"

#define XX NO_LED

// clang-format off

#define KEY_2_LED_LEFT \
/*col 1   2   3   4   5   6   7   8    row */\
    {39, 38, 37, 36, 35, 34, 33, XX},/*  1  */\
    {23, 24, 26, 27, 29, 30, 31, XX},/*  2  */\
    {22, 21, 20, 19, 18, 17, 16, XX},/*  3  */\
    { 9,  10, 12, 13, 14, 15, 0,  2},/*  4  */\
    {XX, XX, XX, XX,  7,  6,  4,  3} /*  5 */\

#define POS_LED_LEFT \
{90, 41},\
{91, 51},\
{105, 49},\
{90, 61},\
{79, 55},\
{67, 46},\
{67, 44},\
{52, 44},\
{43, 42},\
{0, 41},\
{14, 40},\
{14, 37},\
{28, 34},\
{41, 31},\
{55, 33},\
{69, 34},\
{82, 25},\
{69, 24},\
{55, 23},\
{41, 21},\
{28, 24},\
{14, 30},\
{0, 31},\
{0, 21},\
{14, 20},\
{14, 18},\
{28, 14},\
{41, 11},\
{41, 8},\
{55, 13},\
{69, 14},\
{82, 15},\
{82, 12},\
{82, 5},\
{69, 4},\
{55, 3},\
{41, 0},\
{28, 4},\
{14, 10},\
{0, 11}

#define KEY_2_LED_RIGHT \
/*col 9  10  11  12  13  14  15  16     row */\
    {XX, 73, 74, 75, 76, 77, 78, 79},/*  1  */\
    {XX, 71, 70, 69, 67, 66, 64, 63},/*  2  */\
    {XX, 56, 57, 58, 59, 60, 61, 62},/*  3  */\
    {42, 40, 55, 54, 53, 52, 50, 49},/*  4  */\
    {43, 44, 46, 47, XX, XX, XX, XX} /*  5  */\

#define POS_LED_RIGHT \
{133, 41},\
{132, 51},\
{119, 49},\
{132, 61},\
{144, 55},\
{156, 46},\
{156, 44},\
{172, 44},\
{181, 42},\
{224, 41},\
{210, 40},\
{210, 37},\
{196, 34},\
{182, 31},\
{168, 33},\
{154, 34},\
{140, 25},\
{154, 24},\
{168, 23},\
{182, 21},\
{196, 24},\
{210, 30},\
{224, 31},\
{224, 21},\
{210, 20},\
{210, 18},\
{196, 14},\
{182, 11},\
{182, 8},\
{168, 13},\
{154, 14},\
{140, 15},\
{140, 12},\
{140, 5},\
{154, 4},\
{168, 3},\
{182, 0},\
{196, 4},\
{210, 10},\
{224, 11}

// clang-format on

#define LED_FLAG 4, 2, 4, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4, 2, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4

#if defined(RGB_MATRIX_ENABLE)

led_config_t g_led_config = {{KEY_2_LED_LEFT, KEY_2_LED_RIGHT}, {POS_LED_LEFT, POS_LED_RIGHT}, {LED_FLAG, LED_FLAG}};

#endif

#undef KEY_2_LED_LEFT
#undef KEY_2_LED_RIGHT
#undef POS_LED_LEFT
#undef POS_LED_RIGHT
#undef LED_FLAG
#undef XX
