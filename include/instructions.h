#pragma once

#include <common.h>

typedef enum
{
  AM_IMP,    // 不需要额外操作数
  AM_R_D16,  // 8位寄存器，16位字面量
  AM_R_R,    // 8位寄存器，8位寄存器
  AM_MR_R,   // 内存寄存器，8位寄存器
  AM_R,      // 8位寄存器
  AM_R_D8,   // 8位寄存器，8位字面量
  AM_R_MR,   // 8位寄存器，内存寄存器
  AM_R_HLI,  // 8位寄存器，内存寄存器（HLI）
  AM_R_HLD,  // 8位寄存器，内存寄存器（HLD）
  AM_HLI_R,  // 内存寄存器（HLI），8位寄存器
  AM_HLD_R,  // 内存寄存器（HLD），8位寄存器
  AM_R_A8,   // 8位寄存器，8位地址
  AM_A8_R,   // 8位地址，8位寄存器
  AM_HL_SPR, // HL+SP+8位字面量地址
  AM_D16,    // 16位字面量
  AM_D8,     // 8位字面量
  AM_D16_R,  // 16位字面量，8位寄存器
  AM_MR_D8,  // 内存寄存器，8位字面量
  AM_MR,     // 内存寄存器
  AM_A16_R,  // 16位地址，8位寄存器
  AM_R_A16   // 8位寄存器，16位地址
} addr_mode;

/**
 * Register Type
 */
typedef enum
{
  RT_NONE,
  RT_A,
  RT_F,
  RT_B,
  RT_C,
  RT_D,
  RT_E,
  RT_H,
  RT_L,
  RT_AF,
  RT_BC,
  RT_DE,
  RT_HL,
  RT_SP,
  RT_PC
} reg_type;

/**
 * Instruction Type
 */
typedef enum
{
  IN_NONE,
  IN_NOP,
  IN_LD,
  IN_INC,
  IN_DEC,
  IN_RLCA,
  IN_ADD,
  IN_RRCA,
  IN_STOP,
  IN_RLA,
  IN_JR,
  IN_RRA,
  IN_DAA,
  IN_CPL,
  IN_SCF,
  IN_CCF,
  IN_HALT,
  IN_ADC,
  IN_SUB,
  IN_SBC,
  IN_AND,
  IN_XOR,
  IN_OR,
  IN_CP,
  IN_POP,
  IN_JP,
  IN_PUSH,
  IN_RET,
  IN_CB,
  IN_CALL,
  IN_RETI,
  IN_LDH,
  IN_JPHL,
  IN_DI,
  IN_EI,
  IN_RST,
  IN_ERR,
  // CB instructions...
  IN_RLC,
  IN_RRC,
  IN_RL,
  IN_RR,
  IN_SLA,
  IN_SRA,
  IN_SWAP,
  IN_SRL,
  IN_BIT,
  IN_RES,
  IN_SET
} in_type;

/**
 * Condition Type
 */
typedef enum
{
  CT_NONE,
  CT_NOT_ZERO,
  CT_ZERO,
  CT_NO_CARRY,
  CT_CARRY
} cond_type;

typedef struct
{
  in_type instruction_type;
  addr_mode address_mode;
  reg_type reg_1;
  reg_type reg_2;
  cond_type cond;
  u8 param;
} instruction;

instruction *instruction_by_opcode(u8 opcode);

char *inst_name(in_type t);
