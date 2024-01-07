#pragma once

#include <common.h>
#include <instructions.h>

/**
 * |-------|
 * | A | F |
 * |-------|
 * | B | C |
 * |-------|
 * | D | E |
 * |-------|
 * | H | L |
 * |-------|
 * |   SP  |
 * |-------|
 * |   PC  |
 * |-------|
 */
typedef struct
{
  u8 a;
  u8 f;
  u8 b;
  u8 c;
  u8 d;
  u8 e;
  u8 h;
  u8 l;
  u16 sp;
  u16 pc;
} cpu_registers;

typedef struct
{
  cpu_registers registers;
  u16 fetch_data;
  u16 mem_destnation;
  u8 cur_opcode;
  instruction *cur_instruction;

  bool destnation_is_mem;
  bool halted;
  bool stepping;
} cpu_context;

cpu_context cpu_ctx;

void cpu_init();
bool cpu_step();