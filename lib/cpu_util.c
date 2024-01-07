#include <cpu.h>

u16 reverse(u16 num)
{
  return ((num & 0xFF00) >> 8) | ((num & 0x00FF) << 8);
}

u16 cpu_read_reg(reg_type rt)
{
  switch (rt)
  {
  case RT_A:
    return cpu_ctx.registers.a;
  case RT_F:
    return cpu_ctx.registers.f;
  case RT_B:
    return cpu_ctx.registers.b;
  case RT_C:
    return cpu_ctx.registers.c;
  case RT_D:
    return cpu_ctx.registers.d;
  case RT_E:
    return cpu_ctx.registers.e;
  case RT_H:
    return cpu_ctx.registers.h;
  case RT_L:
    return cpu_ctx.registers.l;

  case RT_AF:
    return reverse(*((u16 *)&cpu_ctx.registers.a));
  case RT_BC:
    return reverse(*((u16 *)&cpu_ctx.registers.b));
  case RT_DE:
    return reverse(*((u16 *)&cpu_ctx.registers.d));
  case RT_HL:
    return reverse(*((u16 *)&cpu_ctx.registers.h));

  case RT_SP:
    return cpu_ctx.registers.sp;
  case RT_PC:
    return cpu_ctx.registers.pc;
  default:
    printf("Unknown register type: %d\n", rt);
    exit(-6);
  }
}