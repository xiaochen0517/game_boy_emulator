#include <cpu.h>
#include <bus.h>
#include <emu.h>

static cpu_context cpu_ctx = {0};

void fetch_data()
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
    case AM_R_R:
      cpu_ctx.fetch_data = cpu_read_reg(cpu_ctx.cur_instruction->reg_2);
      return;
    case AM_R_D8:
      cpu_ctx.fetch_data = bus_read(cpu_ctx.registers.pc);
      cpu_ctx.registers.pc++;
      return;
    case AM_R_D16:
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
    case AM_MR_R:
    {
      cpu_ctx.fetch_data = cpu_read_reg(cpu_ctx.cur_instruction->reg_2);
      cpu_ctx.mem_destnation = cpu_read_reg(cpu_ctx.cur_instruction->reg_1);
      cpu_ctx.destnation_is_mem = true;

      if (cpu_ctx.cur_instruction->reg_1 == RT_C)
      {
        cpu_ctx.mem_destnation |= 0xFF00;
      }
      return;
    }
    case AM_R_MR:
    {
      u16 addrs = cpu_read_reg(cpu_ctx.cur_instruction->reg_2);
      if (cpu_ctx.cur_instruction->reg_1 == RT_C)
      {
        cpu_ctx.mem_destnation |= 0xFF00;
      }
      cpu_ctx.fetch_data = bus_read(addrs);
      emu_cycles(1);
      return;
    }
    case AM_R_HLI:
    {
      cpu_ctx.fetch_data = bus_read(cpu_read_reg(cpu_ctx.cur_instruction->reg_2));
      emu_cycles(1);
      cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
      return;
    }
    case AM_R_HLD:
    {
      cpu_ctx.fetch_data = bus_read(cpu_read_reg(cpu_ctx.cur_instruction->reg_2));
      emu_cycles(1);
      cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
      return;
    }
    case AM_HLI_R:
    {
      cpu_ctx.fetch_data = cpu_read_reg(cpu_ctx.cur_instruction->reg_2);
      cpu_ctx.mem_destnation = cpu_read_reg(cpu_ctx.cur_instruction->reg_1);
      cpu_ctx.destnation_is_mem = true;
      cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
      return;
    }
    case AM_HLD_R:
    {
      cpu_ctx.fetch_data = cpu_read_reg(cpu_ctx.cur_instruction->reg_2);
      cpu_ctx.mem_destnation = cpu_read_reg(cpu_ctx.cur_instruction->reg_1);
      cpu_ctx.destnation_is_mem = true;
      cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
      return;
    }
    case AM_R_A8:
      cpu_ctx.fetch_data = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      return;
    case AM_A8_R:
      cpu_ctx.mem_destnation = bus_read(cpu_ctx.registers.pc) | 0xFF00;
      cpu_ctx.destnation_is_mem = true;
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      return;
    case AM_HL_SPR:
      cpu_ctx.fetch_data = cpu_read_reg(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      return;
    case AM_D8:
      cpu_ctx.fetch_data = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      return;
    case AM_A16_R:
    case AM_D16_R:
    {
      u16 low = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      u16 high = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      cpu_ctx.mem_destnation = (high << 8) | low;
      cpu_ctx.fetch_data = cpu_read_reg(cpu_ctx.cur_instruction->reg_2);
      cpu_ctx.destnation_is_mem = true;
      return;
    }
    case AM_MR_D8:
      cpu_ctx.fetch_data = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      cpu_ctx.mem_destnation = cpu_read_reg(cpu_ctx.cur_instruction->reg_1);
      cpu_ctx.destnation_is_mem = true;
      return;
    case AM_MR:
      cpu_ctx.mem_destnation = cpu_read_reg(cpu_ctx.cur_instruction->reg_1);
      cpu_ctx.destnation_is_mem = true;
      cpu_ctx.fetch_data = bus_read(cpu_ctx.mem_destnation);
      emu_cycles(1);
      return;
    case AM_R_A16:
    {
      u16 low = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      u16 high = bus_read(cpu_ctx.registers.pc);
      emu_cycles(1);
      cpu_ctx.registers.pc++;
      u16 addr = (high << 8) | low;
      cpu_ctx.fetch_data = bus_read(addr);
      emu_cycles(1);
      return;
    }
    default:
      printf("Unknown address mode: %d (%02X)\n",
             cpu_ctx.cur_instruction->address_mode,
             cpu_ctx.cur_opcode);
      exit(-7);
  }
}