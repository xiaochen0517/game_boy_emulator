#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// 获取一个数的第 n 位
#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)

// 设置一个数的第 n 位，on 为 true 时设置为 1，否则设置为 0
#define BIT_SET(a, n, on) \
  {                       \
    if (on)               \
      a |= (1 << n);      \
    else                  \
      a &= ~(1 << n);     \
  }

// 判断一个数是否在 [b, c] 之间
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

void delay(u32 ms);

// 未实现的函数
#define NOT_IMPLEMENTED                                     \
  {                                                         \
    printf("Not implemented: %s:%d\n", __FILE__, __LINE__); \
    exit(-10);                                              \
  }
