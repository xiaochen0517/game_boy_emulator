#include <cpu.h>
#include <bus.h>
#include <emu.h>

static cpu_context cpu_ctx = {0};

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