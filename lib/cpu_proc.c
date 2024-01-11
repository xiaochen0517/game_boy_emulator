#include <cpu.h>
#include <emu.h>
#include <bus.h>

static void cpu_set_flags(cpu_context *cpu_ctx, char z, char n, char h, char c)
{
  if (z != -1)
  {
    BIT_SET(cpu_ctx->registers.f, 7, z);
  }
  if (n != -1)
  {
    BIT_SET(cpu_ctx->registers.f, 6, n);
  }
  if (h != -1)
  {
    BIT_SET(cpu_ctx->registers.f, 5, h);
  }
  if (c != -1)
  {
    BIT_SET(cpu_ctx->registers.f, 4, c);
  }
}

static void proc_none(cpu_context *ctx)
{
  printf("Invalid instruction.\n");
  exit(-8);
}

static void proc_nop(cpu_context *ctx)
{
  emu_cycles(1);
}

static void proc_ld(cpu_context *ctx)
{
  if (ctx->destnation_is_mem)
  {
    if (ctx->cur_instruction->reg_2 == RT_AF)
    {
      emu_cycles(1);
      bus_write16(ctx->mem_destnation, ctx->fetch_data);
    } else
    {
      emu_cycles(1);
      bus_write(ctx->mem_destnation, ctx->fetch_data);
    }
    return;
  }
  if (ctx->cur_instruction->address_mode == AM_HL_SPR)
  {
    u8 hflag = (cpu_read_reg(ctx->cur_instruction->reg_2) & 0xF) + (ctx->fetch_data & 0xF) > 0xF;
    u8 cflag = (cpu_read_reg(ctx->cur_instruction->reg_2) & 0xFF) + (ctx->fetch_data & 0xFF) > 0xFF;
    cpu_set_flags(ctx, 0, 0, hflag, cflag);
    cpu_set_reg(ctx->cur_instruction->reg_1, cpu_read_reg(ctx->cur_instruction->reg_2) + ctx->fetch_data);
  }
  cpu_set_reg(ctx->cur_instruction->reg_1, ctx->fetch_data);
}

static bool check_cond(cpu_context *cpu_ctx)
{
  bool is_zero = CPU_FLAG_ZERO;
  bool is_carry = CPU_FLAG_CARRY;

  switch (cpu_ctx->cur_instruction->cond)
  {
    case CT_NONE:
      return true;
    case CT_CARRY:
      return is_carry;
    case CT_NO_CARRY:
      return !is_carry;
    case CT_ZERO:
      return is_zero;
    case CT_NOT_ZERO:
      return !is_zero;
    default:
      printf("Unknown condition: %d\n", cpu_ctx->cur_instruction->cond);
      exit(-9);
  }
}

static void proc_jp(cpu_context *ctx)
{
  if (check_cond(ctx))
  {
    ctx->registers.pc = ctx->fetch_data;
    emu_cycles(1);
  }
}

static void proc_di(cpu_context *ctx)
{
  ctx->interrupt_master_enabled = false;
}


static void
proc_xor(cpu_context *ctx)
{
  ctx->registers.a ^= ctx->fetch_data & 0xFF;
  cpu_set_flags(ctx, ctx->registers.a, 0, 0, 0);
  emu_cycles(1);
}

static IN_PROC processors[] = {
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di,
    [IN_XOR] = proc_xor,
};

IN_PROC inst_get_processor(in_type type)
{
  return processors[type];
}