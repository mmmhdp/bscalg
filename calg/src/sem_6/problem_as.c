#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

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
  int nops;
  uint8_t bin;
} Command;

const char *
instr_to_str (Instruction instr)
{
  return MNEMONICS[instr];
}

const char *
reg_to_str (Register reg)
{
  return REGISTERS[reg];
}

int
instr_is_legal_mnemonic (char *raw_instr)
{
  int i;
  for (i = 0; i < MNEMONICS_DICT_SZ; i++)
    if (strcmp (raw_instr, MNEMONICS[i]) == 0)
      return i;

  return -1;
}

int
op_is_legal_reg (char *reg)
{
  int i;
  for (i = 0; i < REGISTERS_DICT_SZ; i++)
    if (strcmp (reg, REGISTERS[i]) == 0)
      return i;

  return -1;
}

void
print_cmd_with_imm_ops (Command *cmd, int nops)
{
  switch (nops)
    {
    case 1:
      {
        printf ("%s with op1 %d\n", instr_to_str (cmd->instr), cmd->op1);
        break;
      }
    default:
      fprintf (stderr, "%s with illegal number of ops\n",
               instr_to_str (cmd->instr));
      abort ();
    }
}

void
print_cmd_with_reg_ops (Command *cmd, int nops)
{
  switch (nops)
    {
    case 0:
      {
        printf ("%s with no ops\n", instr_to_str (cmd->instr));
        break;
      }
    case 1:
      {
        printf ("%s with op1 %s\n", instr_to_str (cmd->instr),
                reg_to_str (cmd->op1));
        break;
      }
    case 2:
      {
        printf ("%s with op1 %s and op2 %s\n", instr_to_str (cmd->instr),
                reg_to_str (cmd->op1), reg_to_str (cmd->op2));
        break;
      }
    default:
      fprintf (stderr, "%s with illegal number of ops\n",
               instr_to_str (cmd->instr));
      abort ();
    }
}

void
dry_run (Command *cmd)
{
  printf ("%#x ", cmd->bin);
}

void
printf_read_ops_err (Command *cmd)
{
  fprintf (stderr, "FAIL TO READ OPERANDS FOR INSTRUCTION: |%s|\n",
           instr_to_str (cmd->instr));
}

void
read_and_set_single_imm (Command *cmd)
{
  int imm;
  if (scanf ("%d", &imm))
    {
      cmd->op1 = imm;
      return;
    }
  else
    {
      printf_read_ops_err (cmd);
      abort ();
    }

  cmd->op1 = ILL_REG;
}

void
read_and_set_single_register (Command *cmd)
{
  int reg;
  char *raw_reg;

  raw_reg = calloc (1, 2);
  if (scanf ("%s", raw_reg) == 1)
    {
      reg = op_is_legal_reg (raw_reg);
      cmd->op1 = reg;

      if (reg == ILL_REG)
        {
          printf_read_ops_err (cmd);
          free (raw_reg);
          abort ();
        }
    }
  else
    printf_read_ops_err (cmd);

  free (raw_reg);
}

void
read_and_set_two_registers (Command *cmd)
{
  int reg;
  char *raw_reg;

  raw_reg = calloc (1, REG_BUFF_SZ);
  if (scanf ("%s", raw_reg) == 1)
    {
      raw_reg[1] = '\0'; // dummy ',' trim
      reg = op_is_legal_reg (raw_reg);
      cmd->op1 = reg;

      if (reg == ILL_REG)
        {
          printf_read_ops_err (cmd);
          free (raw_reg);
          abort ();
        }
    }
  else
    printf_read_ops_err (cmd);

  if (scanf ("%s", raw_reg) == 1)
    {
      reg = op_is_legal_reg (raw_reg);
      cmd->op2 = reg;

      if (reg == ILL_REG)
        {
          printf_read_ops_err (cmd);
          free (raw_reg);
          abort ();
        }
    }
  else
    printf_read_ops_err (cmd);

  free (raw_reg);
}

void
read_operands (Command *cmd)
{
  switch (cmd->instr)
    {
    case MOVI:
      {
        read_and_set_single_imm (cmd);
        REGISTERS_MEM[D] = cmd->op1;

        uint8_t bin_cmd = OPCODES[cmd->instr];
        bin_cmd |= cmd->op1;
        cmd->bin = bin_cmd;
        cmd->nops = 1;

        break;
      }
    case ADD:
      {
        read_and_set_two_registers (cmd);

        uint8_t bin_cmd = OPCODES[cmd->instr];
        bin_cmd |= (cmd->op1 << 2);
        bin_cmd |= cmd->op2;
        cmd->bin = bin_cmd;
        cmd->nops = 2;

        break;
      }
    case SUB:
      {
        read_and_set_two_registers (cmd);

        uint8_t bin_cmd = OPCODES[cmd->instr];
        bin_cmd |= (cmd->op1 << 2);
        bin_cmd |= cmd->op2;
        cmd->bin = bin_cmd;
        cmd->nops = 2;

        break;
      }
    case MUL:
      {
        read_and_set_two_registers (cmd);

        uint8_t bin_cmd = OPCODES[cmd->instr];
        bin_cmd |= (cmd->op1 << 2);
        bin_cmd |= cmd->op2;
        cmd->bin = bin_cmd;
        cmd->nops = 2;

        break;
      }
    case DIV:
      {
        read_and_set_two_registers (cmd);

        uint8_t bin_cmd = OPCODES[cmd->instr];
        bin_cmd |= (cmd->op1 << 2);
        bin_cmd |= cmd->op2;
        cmd->bin = bin_cmd;
        cmd->nops = 2;

        break;
      }
    case IN:
      {
        read_and_set_single_register (cmd);

        uint8_t bin_cmd = OPCODES[cmd->instr];
        bin_cmd |= cmd->op1;
        cmd->bin = bin_cmd;
        cmd->nops = 1;

        break;
      }
    case OUT:
      {
        read_and_set_single_register (cmd);

        uint8_t bin_cmd = OPCODES[cmd->instr];
        bin_cmd |= cmd->op1;
        cmd->bin = bin_cmd;
        cmd->nops = 1;

        break;
      }
    default:
      {
        fprintf (stderr, "FAIL TO READ OPERANDS FOR INSTRUCTION: |%d|\n",
                 cmd->instr);
        abort ();
      }
    }
}

void
imulate_dry_run ()
{
  Command cmd;
  int instr;
  char *raw_instr;
  raw_instr = calloc (1, 5);

  while ((scanf ("%s", raw_instr)) == 1)
    {
      if ((instr = instr_is_legal_mnemonic (raw_instr)) != -1)
        {
          cmd.instr = instr;
          read_operands (&cmd);
          dry_run (&cmd);
        }
      else
        {
          fprintf (stderr, "ILLEGAL INSTRUCTION: |%s|\n", raw_instr);
          abort ();
        }
    }
  free (raw_instr);
}

int
main (void)
{
  imulate_dry_run ();
}
