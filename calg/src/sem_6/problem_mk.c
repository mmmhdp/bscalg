#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DISAS_DBG

const uint8_t OPCODES[] = {
  0x0, // 0b0000 0000,

  0x80, // 0b1000 0000,
  0x90, // 0b1001 0000,
  0xa0, // 0b1010 0000,
  0xb0, // 0b1011 0000,

  0xc0, // 0b1100 0000,
  0xc4, // 0b1100 0100,
};

const char *MNEMONICS[] = {
  "MOVI", "ADD", "SUB", "MUL", "DIV", "IN", "OUT",
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

void
regs_show ()
{
  int i;
  for (i = 0; i < REGISTERS_DICT_SZ; i++)
    printf ("%d ", REGISTERS_MEM[i]);
  printf ("\n");
}

const char *
reg_to_str (Register reg)
{
  return REGISTERS[reg];
}

const char *
instr_to_str (Instruction instr)
{
  return MNEMONICS[instr];
}

Instruction
opcode_to_instr_mnem (uint8_t opcode)
{
  int opcode_idx;
  for (opcode_idx = 0; opcode_idx < MNEMONICS_DICT_SZ; opcode_idx++)
    {
      if (OPCODES[opcode_idx] == opcode)
        break;
    }

  switch (opcode_idx)
    {
    case 0:
      return MOVI;
    case 1:
      return ADD;
    case 2:
      return SUB;
    case 3:
      return MUL;
    case 4:
      return DIV;
    case 5:
      return IN;
    case 6:
      return OUT;
    default:
      return ILL_INSTR;
    }
}

void
cmd_decode_and_set_instr (Command *cmd)
{
  int i;
  uint8_t cmd_msk;

  // MOVI
  cmd_msk = (cmd->bin >> 7) << 7;
  if (!cmd_msk)
    {
      cmd->instr = opcode_to_instr_mnem (OPCODES[0]);
      return;
    }

  cmd_msk = (cmd->bin >> 6);
  if (cmd_msk == 0x3)
    cmd_msk = (cmd->bin >> 2) << 2;
  else
    cmd_msk = (cmd->bin >> 4) << 4;

  for (i = 1; i < MNEMONICS_DICT_SZ; i++)
    {
      if ((cmd_msk ^ OPCODES[i]) == 0)
        {
          cmd->instr = opcode_to_instr_mnem (OPCODES[i]);
          return;
        }
    }

  cmd->instr = opcode_to_instr_mnem (cmd->bin);
}

void
cmd_decode_and_set_ops (Command *cmd)
{
  switch (cmd->instr)
    {
    case MOVI:
      cmd->op1 = cmd->bin & (~(0x1 << 7));
      cmd->op2 = 0;
      return;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      cmd->op1 = (cmd->bin & (0x3 << 2)) >> 2;
      cmd->op2 = cmd->bin & (0x3);
      return;
    case IN:
    case OUT:
      cmd->op1 = cmd->bin & 0x3;
      cmd->op2 = 0;
      return;
    case ILL_INSTR:
      cmd->op1 = 0;
      cmd->op2 = 0;
      return;
    default:
      fprintf (stderr, "Failed to set operands: invalid instruction, %#x\n",
               cmd->bin);
      abort ();
    }
}

Command
cmd_decode (uint8_t *raw_cmd)
{
  Command cmd;
  cmd.bin = *raw_cmd;

  cmd_decode_and_set_instr (&cmd);
  cmd_decode_and_set_ops (&cmd);

  return cmd;
}

void
cmd_free (Command *cmd)
{
  free (cmd);
}

uint8_t *
cmd_read_next ()
{
  unsigned int tmp;
  uint8_t *cmd;

  cmd = NULL;

  if (scanf ("%x", &tmp) == 1)
    {
      cmd = malloc (sizeof (uint8_t));
      *cmd = (uint8_t)tmp;
    }

  return cmd;
}

typedef struct Programm
{
  struct Programm *next;
  Command *cmd;
} Programm;

void
prgm_exec (Programm *p)
{
  int tmp;
  while (p && p->cmd)
    {
#ifndef DISAS_DBG
      printf ("%#x\n", p->cmd->bin);
#endif

      switch (p->cmd->instr)
        {
        case MOVI:
          REGISTERS_MEM[3] = p->cmd->op1;
          break;
        case ADD:
          REGISTERS_MEM[p->cmd->op1] += REGISTERS_MEM[p->cmd->op2];
          break;
        case SUB:
          REGISTERS_MEM[p->cmd->op1] -= REGISTERS_MEM[p->cmd->op2];
          break;
        case MUL:
          REGISTERS_MEM[p->cmd->op1] *= REGISTERS_MEM[p->cmd->op2];
          break;
        case DIV:
          REGISTERS_MEM[p->cmd->op1] /= REGISTERS_MEM[p->cmd->op2];
          break;
        case IN:
          scanf ("%d", &tmp);
          REGISTERS_MEM[p->cmd->op1] = tmp;
          break;
        case OUT:
          printf ("%d\n", REGISTERS_MEM[p->cmd->op1]);
          break;
        case ILL_INSTR:
          printf ("\n");
          break;
        default:
          fprintf (stderr,
                   "Failed to set operands: corrupted instruction, %#x\n",
                   p->cmd->bin);
          abort ();
        }

      p = p->next;
    }
}

Programm *
prgm_add_cmd (Programm *p, Command cmd)
{
  p->cmd = calloc (1, sizeof (Command));

  p->cmd->instr = cmd.instr;
  p->cmd->op1 = cmd.op1;
  p->cmd->op2 = cmd.op2;
  p->cmd->bin = cmd.bin;

  p->next = calloc (1, sizeof (Programm));
  p = p->next;

  return p;
}

void
prgm_free (Programm *p)
{
  Programm *tmp_nxt;

  if (!p)
    return;

  while (p)
    {
      tmp_nxt = p->next;
      cmd_free (p->cmd);
      free (p);
      p = tmp_nxt;
    }
}

int
main (void)
{
  uint8_t *raw_cmd;
  Command cmd;
  Programm *prg, *prg_tail;

  prg = calloc (1, sizeof (Programm));
  prg_tail = prg;

  while ((raw_cmd = cmd_read_next ()) != NULL)
    {
      cmd = cmd_decode (raw_cmd);
      if (cmd.instr == ILL_INSTR)
        {
          free (raw_cmd);
          break;
        }

      prg_tail = prgm_add_cmd (prg_tail, cmd);
      free (raw_cmd);
    }

  prgm_exec (prg);
  prgm_free (prg);
}
