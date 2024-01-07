#include <cpu.h>
#include <bus.h>

cpu_context cpu_ctx = {0};

void cpu_init()
{
  cpu_ctx.registers.pc = 0x100;
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
    printf("Unknown opcode: 0x%02X\n", cpu_ctx.cur_opcode);
    exit(-6);
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
    printf("Unknown address mode: %d\n", cpu_ctx.cur_instruction->address_mode);
    exit(-7);
  }
}

static void execute_instruction()
{
  printf("Not implemented yet.\n");
}

bool cpu_step()
{
  if (!cpu_ctx.halted)
  {
    u16 pc = cpu_ctx.registers.pc;
    fetch_instruction();
    fetch_data();

    printf("Executing instruction: 0x%02X  PC: 0x%04X\n", cpu_ctx.cur_opcode, pc);

    execute_instruction();
  }
  return true;
}