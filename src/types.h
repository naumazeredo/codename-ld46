#pragma once

#include <stdint.h>

//#define internal static

using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

//struct Color { f32 r, g, b, a; };

//Direction
enum Direction {UP, DOWN, LEFT, RIGHT};
const int NUM_DIRECTIONS = 4;
const int dx[NUM_DIRECTIONS] = {0, 0, -1, 1},
          dy[NUM_DIRECTIONS] = {1, -1, 0, 0};
