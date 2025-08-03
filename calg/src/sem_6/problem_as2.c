#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DISAS_DBG 
//
const char *MNEMONICS[] = {
  "MOVI", "ADD", "SUB", "MUL", "DIV", "IN", "OUT",
};

const uint8_t OPCODES[] = {
  0x0,  // 0b0000 0000,
  0x80, // 0b1000 0000,
  0x90, // 0b1001 0000,
  0xa0, // 0b1010 0000,
  0xb0, // 0b1011 0000,
  0xc0, // 0b1100 0000,
  0xc4, // 0b1100 0100,
};

const char *REGISTERS[] = {
  "A",
  "B",
  "C",
  "D",
};

const int8_t REGS_OPCODES[] = {
  0x0, // 0b00,
  0x1, // 0b01,
  0x2, // 0b10,
  0x3, // 0b11,
};

typedef enum
{
  ILL_INSTR = -1,
  MOVI = 0,
  ADD,
  SUB,
  MUL,
  DIV,
  IN,
  OUT,
} Instruction;

typedef enum
{
  ILL_REG = -1,
  A = 0,
  B,
  C,
  D,
} Register;

uint8_t REGISTERS_MEM[4];
#define REG_BUFF_SZ 256

#define MNEMONICS_DICT_SZ ((int)((sizeof (MNEMONICS)) / (sizeof (char *))))
#define REGISTERS_DICT_SZ ((int)((sizeof (REGISTERS)) / (sizeof (char *))))

typedef struct
{
  Instruction instr;
  int op1;
  int op2;
  uint8_t bin;
} Command;

const char *
instr_to_str (Instruction instr)
{
  return MNEMONICS[instr];
}

Instruction
opcode_to_instr_mnem(uint8_t opcode)
{
  int opcode_idx;
  for (opcode_idx = 0; opcode_idx < MNEMONICS_DICT_SZ; opcode_idx++)
  {
    if (OPCODES[opcode_idx] == opcode)
      break;
  }

  switch (opcode_idx) {
    case 0: return MOVI;
    case 1: return ADD;
    case 2: return SUB;
    case 3: return MUL;
    case 4: return DIV;
    case 5: return IN;
    case 6: return OUT;
    default: return ILL_INSTR;
  }
}

const char *
reg_to_str (Register reg)
{
  return REGISTERS[reg];
}

void
decode_and_set_instr(Command *cmd)
{
  int i;
  uint8_t cmd_msk;

  cmd_msk = cmd->bin >> 7;

  if (!cmd_msk)
  {
    cmd->instr = opcode_to_instr_mnem(OPCODES[0]);
    return;
  }

  cmd_msk = (cmd->bin >> 4) << 4;
  for (i = 1; i < MNEMONICS_DICT_SZ; i++)
  {
    if ((cmd_msk ^ OPCODES[i]) == 0)
    {
      cmd->instr = opcode_to_instr_mnem(OPCODES[i]);
      return;
    }
  }

  fprintf(stderr, "Decode instr failed, %#x\n", cmd->bin);
}

Command 
cmd_decode(uint8_t *raw_cmd)
{
  Command cmd;
  cmd.bin = *raw_cmd;

  decode_and_set_instr(&cmd);

  return cmd;
}

void 
dry_run(Command cmd)
{
#ifndef DISAS_DBG
  printf("Hex instr: %#x\n", cmd.instr);
  printf("Instr mnem: %s\n", instr_to_str(cmd.instr));
  printf("Hex op1: %#x\n", cmd.op1);
  printf("Hex op2: %#x\n", cmd.op2);
  printf("Hex cmd: %#x\n", cmd.bin);
#endif
}

uint8_t *
read_nxt_cmd()
{
  unsigned int tmp;
  uint8_t *cmd;

  cmd = NULL;

  if (scanf("%x", &tmp) == 1)
  {
    cmd  =  malloc(sizeof(uint8_t));
    *cmd = (uint8_t) tmp;
  }

  return cmd;
}

int main(void){
  uint8_t *raw_cmd;
  Command cmd;
  while ((raw_cmd = read_nxt_cmd()) != NULL)
  {
    cmd = cmd_decode(raw_cmd);

    dry_run(cmd);
    free(raw_cmd);
  }
}
