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

  bool interrupt_master_enabled;
} cpu_context;

cpu_context cpu_ctx;

void cpu_init();
bool cpu_step();

typedef void (*IN_PROC)(cpu_context *);

IN_PROC inst_get_processor(in_type type);

u16 cpu_read_reg(reg_type rt);

#define CPU_FLAG_ZERO BIT(cpu_ctx->registers.f, 7);
#define CPU_FLAG_SUBTRACT BIT(cpu_ctx->registers.f, 6);
#define CPU_FLAG_HALF_CARRY BIT(cpu_ctx->registers.f, 5);
#define CPU_FLAG_CARRY BIT(cpu_ctx->registers.f, 4);