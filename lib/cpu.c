#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context cpu_ctx = {0};

void cpu_init()
{
  cpu_ctx.registers.pc = 0x100;
  cpu_ctx.registers.a = 0x01;
}

static void fetch_instruction()
{
  cpu_ctx.cur_opcode = bus_read(cpu_ctx.registers.pc++);
  cpu_ctx.cur_instruction = instruction_by_opcode(cpu_ctx.cur_opcode);
}

static void fetch_data()
{
  cpu_ctx.mem_destnation = 0;
  cpu_ctx.destnation_is_mem = false;

  if (cpu_ctx.cur_instruction == NULL)
  {
    return;
  }

  switch (cpu_ctx.cur_instruction->address_mode)
  {
    case AM_IMP:
      return;
    case AM_R:
      cpu_ctx.fetch_data = cpu_read_reg(cpu_ctx.cur_instruction->reg_1);
      return;
    case AM_R_D8:
      cpu_ctx.fetch_data = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      return;
    case AM_D16:
    {
      u16 low = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      u16 high = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      cpu_ctx.fetch_data = (high << 8) | low;
      return;
    }
    default:
      printf("Unknown address mode: %d (%02X)\n",
             cpu_ctx.cur_instruction->address_mode,
             cpu_ctx.cur_opcode);
      exit(-7);
  }
}

static void execute_instruction()
{
  IN_PROC proc = inst_get_processor(cpu_ctx.cur_instruction->instruction_type);
  if (!proc)
  {
    NOT_IMPLEMENTED
  }
  proc(&cpu_ctx);
}

bool cpu_step()
{
  if (!cpu_ctx.halted)
  {
    u16 pc = cpu_ctx.registers.pc;
    fetch_instruction();
    fetch_data();

    printf("%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n",
           pc,
           inst_name(cpu_ctx.cur_instruction->instruction_type),
           cpu_ctx.cur_opcode,
           bus_read(pc + 1),
           bus_read(pc + 2),
           cpu_ctx.registers.a,
           cpu_ctx.registers.b,
           cpu_ctx.registers.c);

    if (cpu_ctx.cur_instruction == NULL)
    {
      printf("Unknown opcode: 0x%02X\n", cpu_ctx.cur_opcode);
      exit(-6);
    }

    execute_instruction();
  }
  return true;
}