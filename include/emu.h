#pragma once

#include <common.h>

typedef struct
{
  bool paused;
  bool running;
  u64 ticks;
} emu_context;

int emu_run(int arc, char **argv);

emu_context *emu_get_context();

void emu_cycles(u64 cycles);